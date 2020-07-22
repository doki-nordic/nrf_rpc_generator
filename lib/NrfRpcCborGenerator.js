const CodeBlocks = require('./CodeBlocks');
const Units = require('./Units');
const { platformIntTypes } = require('./Utils');


/** @module NrfRpcCborGenerator */

let predefinedResponseHandlers = {
	'bool': 'ser_rsp_simple_bool',
	'float': 'ser_rsp_simple_float',
	'double': 'ser_rsp_simple_double',
	'int8_t': 'ser_rsp_simple_i8',
	'uint8_t': 'ser_rsp_simple_u8',
	'int16_t': 'ser_rsp_simple_i16',
	'uint16_t': 'ser_rsp_simple_u16',
	'int32_t': 'ser_rsp_simple_i32',
	'uint32_t': 'ser_rsp_simple_u32',
	'int64_t': 'ser_rsp_simple_i64',
	'uint64_t': 'ser_rsp_simple_u64'
};



/** Generate C source code that encodes a variable.
 * 
 * @param {Object} state State structure that will be updated with a new encoder <br/><code>{code: string, maxSize: number}</code>.
 * @param {string} name Variable name
 * @param {string} type Variable type
 */
function generateEncoder(base, state, name, type, options) {

	if (options.isString) {
		state.maxSize += 5;
		state.locals += `
			size_t _${name}_strlen;
		`;
		state.calc += `
			_${name}_strlen = strlen(${name});
			_buffer_size_max += _${name}_strlen;
		`;
		state.code += `
			ser_encode_str(&_ctx.encoder, ${name}, _${name}_strlen);
		`;
		return;
	}

	if (base.module.isCallback(type)) {
		state.maxSize += 5;
		state.code += `
			ser_encode_callback(&_ctx.encoder, ${name}, ${type}${Units.CALLBACK_RECV_POSTFIX});
		`;
		return;
	}

	if (type.endsWith('*')) {
		name = `(*${name})`;
		type = type.substring(0, type.length - 1).trim();
		generateEncoder(base, state, name, type, options);
		return;
	} else if (type.startsWith('const ')) {
		type = type.substr(6);
	}


	let info = platformIntTypes[type];

	if (info) {
		if (info.signed) {
			state.code += `ser_encode_int(&_ctx.encoder, ${name});\n`;
		} else {
			state.code += `ser_encode_uint(&_ctx.encoder, ${name});\n`;
		}
		state.maxSize += 1 + info.bytes;
	} else if (type == 'bool') {
		state.code += `ser_encode_boolean(&_ctx.encoder, ${name});\n`;
		state.maxSize += 1;
	} else if (type == 'float') {
		state.code += `ser_encode_float(&_ctx.encoder, ${name});\n`;
		state.maxSize += 5;
	} else if (type == 'double') {
		state.code += `ser_encode_double(&_ctx.encoder, ${name});\n`;
		state.maxSize += 9;
	} else {
		throw new Error(`Unknown encoder for type '${type}'.`);
	}
}


/** Generate a sending function on a client side.
 * @param {Func} base ApiFunction object.
 * @returns {Object} Generated code blocks.
 */
function generateCliSendFunc(base) {

	let blocks = {};
	let encodeParamsState = { maxSize: 0, code: '', calc: '', locals: '' };

	for (let param of Object.values(base.params).filter(x => x.dir == 'IN' || x.dir == 'INOUT')) {
		generateEncoder(base, encodeParamsState, param.name, param.type, param);
	}

	blocks[CodeBlocks.HEADER] = `
		\\void func() // TODO: header
		\\{
	`;

	blocks[CodeBlocks.LOCALS] = `
		struct nrf_rpc_cbor_ctx _ctx;
		size_t _buffer_size_max = ${encodeParamsState.maxSize};
		${encodeParamsState.locals}
	`;
	if (base.params._result) {
		blocks[CodeBlocks.LOCALS] += `${base.params._result.type} _result;\n`;
	}

	if (encodeParamsState.calc.length > 0) {
		blocks[CodeBlocks.CALC] = encodeParamsState.calc;
	}

	blocks[CodeBlocks.ALLOCATE] = `
		NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);
	`;

	if (encodeParamsState.code.length > 0) {
		blocks[CodeBlocks.ENCODE] = encodeParamsState.code;
	}

	if (base.isInlineResponse) {
		throw new Error('Not implemented!')
	} else if (base.isEvent) {
		blocks[CodeBlocks.SEND] = `
			nrf_rpc_cbor_evt_no_err(&${base.getGroup()},
			@	${base.getId()}, &_ctx);
		`;
	} else {
		let rsp = getPredefinedResponseHandler(base);
		if (rsp) {
			base.responseHandlerGenerate = false;
			blocks[CodeBlocks.SEND] = `
				nrf_rpc_cbor_cmd_no_err(&${base.getGroup()}, ${base.getId()},
				@	&_ctx, ${rsp}, ${base.params._result ? '&_result' : 'NULL'});
			`;
		} else {
			blocks[CodeBlocks.SEND] = `
				nrf_rpc_cbor_cmd_no_err(&${base.getGroup()}, ${base.getId()},
				@	&_ctx, TODO, &result);
			`; // TODO: custom response handler
		}
	}

	if (base.params._result) {
		blocks[CodeBlocks.RETURN] = `return _result;`;
	}

	blocks[CodeBlocks.FOOTER] = `
		\\}
	`;

	return blocks;
}


function generateCliRspFunc(base) {
	let blocks = {};

	blocks[CodeBlocks.HEADER] = `
		\\static void ${base.name}${Units.RSP_FUNC_POSTFIX}(CborValue *value, void *handler_data)
		\\{
	`;

	blocks[CodeBlocks.FOOTER] = `
		\\}
	`;

	return blocks;
}

function generateCliResStruct(base) {
	let blocks = {};

	blocks[CodeBlocks.HEADER] = `
		\\struct ${base.name}${Units.RES_STRUCT_POSTFIX}
		\\{
	`;

	blocks[CodeBlocks.FOOTER] = `
		\\};
	`;

	return blocks;
}

function getParamKind(param) {
	if (param.kind !== undefined) {
		return param.kind;
	}

	let kind = param.dir == Units.IN ? 'in ' : param.dir == Units.OUT ? 'out ' : param.dir == Units.INOUT ? 'inout ' : '';

	if (param.isString)

		param.kind = kind;
	return kind;
}

/*

Trzeba stworzyć klasę siostraną do UnitFunc w tym module i na wstępnie wybrać wszystkige encodery/decodery
a później generować kod.

Liczenie rozmiarów buforów może być zarówno w czasie inicjalizacji jak i genracji kodu.

 * TODO: Nie takie podejście: 
 */
let paramTemplates = {
	'in T': {
		sendFuncEnc: p => `${p.encoderName}(&_ctx.encoder, ${p.name});`,
		sendFuncAssign: p => p.isSize ? `_result.${p.name} = ${p.name};` : '',
		recvFuncLoc: p => `${p.type} ${p.name};`,
		recvFuncDec: p => `${p.decoderName}(_value, &${p.name});`,
		recvFuncPar: p => `${p.name}`,
		resStructField: p => p.isSize ? `${p.type} ${p.name};` : '',
	},
	'ret T': {
		sendFuncReturn: p => `return _result.${p.name};`,
		resStructField: p => `${p.type} ${p.name};`,
		rspFuncDec: p => `${p.decoderName}(_value, &_res->${p.name});`,
		recvFuncLoc: p => `${p.type} ${p.name};`,
		recvFuncPar: p => `${p.name}`,
		recvFuncEnc: p => `${p.encoderName}(&_ctx.encoder, &${p.name});`,
	},
	'in S': {},
	'out T*': {},
	'inout T*': {},
	'in S*': {},
	'out S*': {},
	'inout S*': {},

	'in T[in]': {},
	'out T[in]': {},
	'inout T[in]': {},
	'out T[inout]': {},
	'inout T[inout]': {},

	'in STR': {
		sendFuncLoc: p => `size_t _${p.name}_strlen;`,
		sendFuncCalc: p => `_${p.name}_strlen = strlen(${p.name});\n_buffer_size_max += _${p.name}_strlen;`,
		sendFuncEnc: p => `ser_encode_str(&_ctx.encoder, ${p.name}, _${p.name}_strlen);`,
		recvFuncDec: p => `char ${p.name}[ser_decode_str_len(_value) + 1];\nser_decode_str(_value, ${p.name});`,
		recvFuncPar: p => `${p.name}`,
	},
	'inout STR': {},
	'out STR[]': {
		sendFuncAssign: p => `_result.${p.name} = ${p.name};`,
		rspFuncDec: p => `ser_decode_str(_value, _res->${p.name}, ${p.getArraySize('(_res->$)')});`,
		resStructField: p => `${p.type} ${p.name};`,
		recvFuncLoc: p => `size_t _${p.name}_strlen;`,
		recvFuncOutDec: p => `char ${p.name}[${p.getArraySize()} + 1];`,
		recvFuncPar: p => `${p.name}`,
		recvFuncCalc: p => `_${p.name}_strlen = strlen(${p.name});\n_buffer_size_max += _${p.name}_strlen;`,
		recvFuncEnc: p => `ser_encode_str(&_ctx.encoder, ${p.name}, -1);`,
	},
	'inout STR[]': {},

	'in CBK': {
		sendFuncEnc: p => `ser_encode_callback(&_ctx.encoder, ${p.name});`,
		bufferConst: p => 5
	},
	/*'out T': {
		recvFuncLoc: p => `${p.type} ${p.name};`,
		recvFuncPar: p => `&${p.name}`,
		recvFuncEnc: p => `${p.encoderName}(value, ${p.name});`,
	},
	/*'in STR': {
		sendFuncLoc: p => `size_t _${p.name}_strlen;`,
		sendFuncCalc: p => `_${p.name}_strlen = strlen(${p.name});\n_buffer_size_max += _${p.name}_strlen;`,
		sendFuncEnc: p => `ser_encode_str(&_ctx.encoder, ${p.name}, _${p.name}_strlen);`
	},
	'out T': {
		resStruct: p => `${p.type} ${p.name};`,
		recvFuncLoc: p => `${p.type} ${p.name};`,
		recvFuncPar: p => p.isReturn ? p.name : `&${p.name}`,
		recvFuncEnc: p => `${getEncoder(p)}(&_ctx.encoder, ${p.name});`,
		rspFuncDec: p => `${getDecoder(p)}(value, &_outputs->${p.name});`,
		inlineRspDec: p => `${getDecoder(p)}(value, &${p.name});`
	},
	'out T[inout]': {
		resStruct: p => `${p.type} ${p.name};`,
		recvFuncLoc: p => `${p.baseType} ${p.name};`,
		recvFuncPar: p => `&${p.name}`,
		recvFuncEnc: p => `${getEncoder(p)}(&_ctx.encoder, ${p.name});`,
		rspFuncDec: p => `${getDecoder(p)}(value, _outputs->${p.name});`,
		inlineRspDec: p => `${getDecoder(p)}(value, ${p.name});`
	}*/
};


function getBasicType(type) {

	if (type in platformIntTypes) {
		let info = platformIntTypes[type];
		return {
			bufferSize: 1 + info.bytes,
			encoderName: info.signed ? 'ser_encode_int' : 'ser_encode_uint',
			decoderName: info.signed ? 'ser_decode_int' : 'ser_decode_uint',
		}
	} else if (type == 'bool') {
		return {
			bufferSize: 1,
			encoderName: 'ser_encode_bool',
			decoderName: 'ser_decode_bool',
		}
	} else if (type == 'float') {
		return {
			bufferSize: 5,
			encoderName: 'ser_encode_float',
			decoderName: 'ser_decode_float',
		}
	} else if (type == 'double') {
		return {
			bufferSize: 9,
			encoderName: 'ser_encode_double',
			decoderName: 'ser_decode_double',
		}
	}

	return null;
}


class Param {

	constructor(base, info) {

		this.base = base;
		this.ptrCount = 0;
		this.name = info.name;
		this.type = info.type;
		this.dir = info.dir;
		this.isString = info.isString;
		this.isArray = info.isArray;
		this.sizePattern = info.sizePattern;
		this.sizeParam = info.sizeParam;
		this.isSize = info.isSize;
		this.isReturn = info.isReturn;
		this.isCallback = false;
		this.basicType = null;

		this.shortType = this.type
			.replace(/(^|[^a-z0-9_])(const|volatile)($|[^a-z0-9_])/gi, '$1$3')
			.replace(/(^|[^a-z0-9_])(const|volatile)($|[^a-z0-9_])/gi, '$1$3')
			.replace(/(^|[^a-z0-9_])(const|volatile)($|[^a-z0-9_])/gi, '$1$3')
			.replace(/\s+/g, ' ')
			.trim();

		while (this.shortType.endsWith('*')) {
			this.shortType = this.shortType
				.substr(0, this.shortType.length - 1)
				.trim();
			this.ptrCount++;
		}

		let templName;

		if (this.isReturn) {
			templName = 'ret ';
		} else if (this.dir == Units.IN) {
			templName = 'in ';
		} else if (this.dir == Units.OUT) {
			templName = 'out ';
		} else {
			templName = 'inout ';
		}


		if (this.isString) {
			templName += 'STR';
			if (this.ptrCount == 0 || !(this.shortType in platformIntTypes) || platformIntTypes[this.shortType].bytes != 1) {
				throw Error(`Parameter '${this.name}' cannot be a string in functions '${base.name}'.`);
			}
			if (!this.isArray) {
				this.ptrCount--;
			}
		} else {
			this.basicType = getBasicType(this.shortType);
			if (this.basicType) {
				templName += 'T';
				this.encoderName = this.basicType.encoderName;
				this.decoderName = this.basicType.decoderName;
			} else {
				this.isCallback = base.module.isCallback(this.shortType);
				if (this.isCallback) {
					templName += 'CBK';
				} else {
					// TODO: if (!base.module.isStruct(type)) throw
					templName += 'S';
				}
			}
		}

		if (this.isArray) {
			if (this.ptrCount == 0) {
				throw Error(`Parameter '${this.name}' cannot be an array in function '${base.name}'.`);
			}
			templName += '*'.repeat(this.ptrCount - 1) + '[]';
		} else {
			templName += '*'.repeat(this.ptrCount);
		}

		console.log(`${this.shortType} ${this.ptrCount} ${this.name} ---- ${templName}`);

		if (!(templName in paramTemplates)) {
			throw Error(`No template for '${templName}' (generated from ${this.type}) in function '${base.name}'.`);
		}

		this.templ = paramTemplates[templName];
	}

	_generateBlock(blocks, codeBlock, templField) {

		if (templField in this.templ) {
			let code = this.templ[templField](this);
			if (code.length > 0) {
				blocks[codeBlock] = (blocks[codeBlock] || '') + code + '\n';
			}
		}
	}

	generateCliSendFunc(blocks) {

		this._generateBlock(blocks, CodeBlocks.LOCALS, 'sendFuncLoc');
		this._generateBlock(blocks, CodeBlocks.ENCODE, 'sendFuncEnc');
		this._generateBlock(blocks, CodeBlocks.CALC, 'sendFuncCalc');
		this._generateBlock(blocks, CodeBlocks.ASSIGN, 'sendFuncAssign');
		this._generateBlock(blocks, CodeBlocks.RETURN, 'sendFuncReturn');

	}

	generateCliRspFunc(blocks) {

		this._generateBlock(blocks, CodeBlocks.LOCALS, 'rspFuncLoc');
		this._generateBlock(blocks, CodeBlocks.DECODE_RES, 'rspFuncDec');

	}

	generateCliResStruct(blocks) {

		this._generateBlock(blocks, CodeBlocks.FIELDS, 'resStructField');

	}

	generateHostRecvFunc(blocks) {

		this._generateBlock(blocks, CodeBlocks.LOCALS, 'recvFuncLoc');
		this._generateBlock(blocks, CodeBlocks.DECODE, 'recvFuncDec');
		this._generateBlock(blocks, CodeBlocks.ENCODE_RES, 'recvFuncEnc');
		this._generateBlock(blocks, CodeBlocks.OUTPUT_ALLOC, 'recvFuncOutDec');
		this._generateBlock(blocks, CodeBlocks.CALC, 'recvFuncCalc');
		
		
	}

	getBufferSize() {
		if (this.basicType) {
			return this.basicType.bufferSize;
		} else if (this.isString || this.isCallback) {
			return 5;
		}
	}

	getArraySize(innerPattern)
	{
		if (innerPattern) {
			return this.sizePattern.replace('$', innerPattern.replace('$', this.sizeParam));
		} else {
			return this.sizePattern.replace('$', this.sizeParam);
		}
	}
}

class GenFunc {

	constructor(base) {

		this.base = base;
		this.params = [];

		for (let name in base.params) {
			let param = base.params[name];
			this.params.push(new Param(base, param));
		}
	}

	_getPredefinedResponseHandler() {
		let base = this.base;

		if (base.customResponse) {
			return null;
		}

		let num = 0;
		for (let name in base.params) {
			if (name != '_result' && base.params[name].dir != Units.IN) {
				num++;
			}
		}
		if (num > 0) {
			return null;
		}

		if (!base.params._result) {
			return 'ser_rsp_simple_void';
		}

		let type = base.params._result.type;

		if (type in predefinedResponseHandlers) {
			return predefinedResponseHandlers[type];
		}

		let info = platformIntTypes[type];

		if (info) {
			return predefinedResponseHandlers[info.stdint];
		}

		return null;
	}

	generateCliSendFunc(func) {
		let base = this.base;

		let blocks = {};
		let bufferSize = 0;

		let rsp = this._getPredefinedResponseHandler(base);

		let params;
		if (rsp) {
			params = this.params.filter(param => !param.isReturn)
		} else {
			params = this.params;
		}

		params
			.filter(param => param.dir != Units.OUT)
			.forEach(param => bufferSize += param.getBufferSize());

		//TODO: Generate CodeBlocks.HEADER if function declared, but not defined.

		blocks[CodeBlocks.LOCALS] = `
			struct nrf_rpc_cbor_ctx _ctx;
			size_t _buffer_size_max = ${bufferSize};
		`;

		for (let param of params) {
			param.generateCliSendFunc(blocks);
		}

		blocks[CodeBlocks.ALLOCATE] = `
			NRF_RPC_CBOR_ALLOC(_ctx, _buffer_size_max);
		`;

		if (base.isInlineResponse) {
			throw new Error('Not implemented!')
		} else if (base.isEvent) {
			blocks[CodeBlocks.SEND] = `
				nrf_rpc_cbor_evt_no_err(&${base.getGroup()},
				@	${base.getId()}, &_ctx);
			`;
		} else {
			if (rsp) {
				base.responseHandlerGenerate = false;
				blocks[CodeBlocks.SEND] = `
					nrf_rpc_cbor_cmd_no_err(&${base.getGroup()}, ${base.getId()},
					@	&_ctx, ${rsp}, ${base.params._result ? '&_result' : 'NULL'});
				`;
				if (base.params._result) {
					blocks[CodeBlocks.LOCALS] += `${base.params._result.type} _result;\n`;
					blocks[CodeBlocks.RETURN] = `return _result;\n`;
				}
			} else {
				blocks[CodeBlocks.SEND] = `
					nrf_rpc_cbor_cmd_no_err(&${base.getGroup()}, ${base.getId()},
					@	&_ctx, ${base.name}${Units.RSP_FUNC_POSTFIX}, &_result);
				`;
				blocks[CodeBlocks.LOCALS] += `struct ${base.name}${Units.RES_STRUCT_POSTFIX} _result;\n`;
			}
		}

		let blockOrder = [
			CodeBlocks.BEGIN,
			CodeBlocks.LOCALS,
			CodeBlocks.CALC,
			CodeBlocks.ALLOCATE,
			CodeBlocks.ENCODE,
			CodeBlocks.ASSIGN,
			CodeBlocks.SEND,
			CodeBlocks.RETURN,
		];

		func.regenerate(blocks, blockOrder, false);
	}


	generateCliRspFunc(func) {
		let base = this.base;

		let blocks = {};

		blocks[CodeBlocks.HEADER] = `
			\\static void ${base.name}${Units.RSP_FUNC_POSTFIX}(CborValue *_value, void *_handler_data)
			\\{
		`;

		blocks[CodeBlocks.LOCALS] = `
			struct entropy_get_result *_res =
			@	(struct ${base.name}${Units.RES_STRUCT_POSTFIX} *)_handler_data;
		`;

		for (let param of this.params) {
			param.generateCliRspFunc(blocks);
		}

		blocks[CodeBlocks.FOOTER] = `
			\\}
		`;

		let blockOrder = [
			CodeBlocks.HEADER,
			CodeBlocks.BEGIN,
			CodeBlocks.LOCALS,
			CodeBlocks.DECODE_RES,
			CodeBlocks.FOOTER,
		];

		func.regenerate(blocks, blockOrder, true); // TODO: remove withHeaders: do it based on CodeBlocks.HEADER existence
	}

	generateCliResStruct(func) {
		let base = this.base;

		let blocks = {};

		blocks[CodeBlocks.HEADER] = `
			\\struct ${base.name}${Units.RES_STRUCT_POSTFIX}
			\\{
		`;

		for (let param of this.params) {
			param.generateCliResStruct(blocks);
		}

		blocks[CodeBlocks.FOOTER] = `
			\\};
		`;

		let blockOrder = [
			CodeBlocks.HEADER,
			CodeBlocks.BEGIN,
			CodeBlocks.FIELDS,
			CodeBlocks.FOOTER,
		];

		func.regenerate(blocks, blockOrder, true);
	}


	generateHostRecvFunc(func) {
		let base = this.base;

		let blocks = {};

		blocks[CodeBlocks.HEADER] = `
			\\static void ${base.name}${Units.RECV_FUNC_POSTFIX}(CborValue *_value, void *_handler_data)
			\\{
		`;

		let bufferSize = 0;
		this.params
			.filter(param => param.dir != Units.IN)
			.forEach(param => bufferSize += param.getBufferSize());

		blocks[CodeBlocks.LOCALS] = `
			struct nrf_rpc_cbor_ctx _ctx;
			size_t _buffer_size_max = ${bufferSize};
		`;

		for (let param of this.params) {
			param.generateHostRecvFunc(blocks);
		}

		if (CodeBlocks.DECODE in blocks && blocks[CodeBlocks.DECODE].length > 0) {
			blocks[CodeBlocks.DECODE_DONE] = `
				if (!ser_decoding_done_and_check(_value)) {
				@	goto decoding_error;
				}
			`;
		} else {
			blocks[CodeBlocks.DECODE_DONE] = `
				nrf_rpc_cbor_decoding_done(_value);
			`;
		}

		let paramNames = [];
		for (let p of this.params) {
			if (p.name != '_result' && 'recvFuncPar' in p.templ) {
				paramNames.push(p.templ.recvFuncPar(p));
			}
		}

		blocks[CodeBlocks.EXECUTE] = `
			${base.params._result ? '_result =' : ''} ${base.name}(${paramNames.join(', ')});
		`;

		blocks[CodeBlocks.FOOTER] = '';

		if (Object.values(blocks).some(t => t.indexOf('goto decoding_done_and_error') >= 0)) {
			blocks[CodeBlocks.FOOTER] += `
				return;
				\\decoding_done_and_error:
				nrf_rpc_cbor_decoding_done(_value);
				\\decoding_error:
				report_decoding_error(${base.getId()}, _handler_data);
			`;
		} else if (Object.values(blocks).some(t => t.indexOf('goto decoding_error') >= 0)) {
			blocks[CodeBlocks.FOOTER] += `
				return;
				\\decoding_error:
				report_decoding_error(${base.getId()}, _handler_data);
			`;
		}

		blocks[CodeBlocks.FOOTER] += `
			\\}
		`;

		let blockOrder = [
			CodeBlocks.HEADER,
			CodeBlocks.BEGIN,
			CodeBlocks.LOCALS,
			CodeBlocks.DECODE,
			CodeBlocks.DECODE_DONE,
			CodeBlocks.OUTPUT_ALLOC,
			CodeBlocks.EXECUTE,
			CodeBlocks.CALC,
			CodeBlocks.ENCODE_RES,
			CodeBlocks.FOOTER,
		];

		func.regenerate(blocks, blockOrder, true);
	}

};


exports.GenFunc = GenFunc;
