const CodeBlocks = require('./CodeBlocks');
const Units = require('./Units');
const { platformIntTypes } = require('./Utils');


/** @module NrfRpcCborGenerator */

let predefinedResponseHandlers = {
	'int8_t': 'ser_rsp_simple_i8',
	'uint8_t': 'ser_rsp_simple_u8',
	'int16_t': 'ser_rsp_simple_i16',
	'uint16_t': 'ser_rsp_simple_u16',
	'int32_t': 'ser_rsp_simple_i32',
	'uint32_t': 'ser_rsp_simple_u32',
	'int64_t': 'ser_rsp_simple_i64',
	'uint64_t': 'ser_rsp_simple_u64'
};

function getPredefinedResponseHandler(base)
{
	if (!base.params._result) {
		return 'ser_rsp_simple_void';
	}

	let info = platformIntTypes[base.params._result.type];

	if (info) {
		return predefinedResponseHandlers[info.stdint];
	}

	return undefined;
}


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
		blocks[CodeBlocks.ENCODE_PARAMS] = encodeParamsState.code;
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


exports.generateCliSendFunc = generateCliSendFunc;
