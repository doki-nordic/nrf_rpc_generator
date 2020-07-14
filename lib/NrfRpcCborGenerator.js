const CodeBlocks = require('./CodeBlocks');
const { platformIntTypes } = require('./Utils');


/** Generate C source code that encodes a variable.
 * 
 * @param {Object} state State structure that will be updated with a new encoder <br/><code>{code: string, maxSize: number}</code>.
 * @param {string} name Variable name
 * @param {string} type Variable type
 */
function generateEncoder(state, name, type) {

	if (type.endsWith('*')) {
		name = `(*${name})`;
		type = type.substring(0, type.length - 1).trim();
		generateEncoder(state, name, type);
		return;
	}

	let info = platformIntTypes[type];

	if (info) {
		if (info.signed) {
			state.code += `ser_encode_int(&ctx.encoder, ${name});\n`;
		} else {
			state.code += `ser_encode_uint(&ctx.encoder, ${name});\n`;
		}
		state.maxSize += 1 + info.bytes;
	} else if (type == 'bool') {
		state.code += `ser_encode_boolean(&ctx.encoder, ${name});\n`;
		state.maxSize += 1;
	} else if (type == 'float') {
		state.code += `ser_encode_float(&ctx.encoder, ${name});\n`;
		state.maxSize += 5;
	} else if (type == 'double') {
		state.code += `ser_encode_double(&ctx.encoder, ${name});\n`;
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
	let encodeParamsState = { maxSize: 0, code: '' };

	for (let param of Object.values(base.params).filter(x => x.dir == 'IN' || x.dir == 'INOUT')) {
		generateEncoder(encodeParamsState, param.name, param.type);
	}

	blocks[CodeBlocks.HEADER] = `
		void func() // TODO: header
		{
	`;

	blocks[CodeBlocks.LOCALS] = `
		struct nrf_rpc_cbor_ctx _ctx;
		size_t _buffer_size_max = ${encodeParamsState.maxSize};
	`;
	if (base.params._result) {
		blocks[CodeBlocks.LOCALS] += `${base.params._result.type} _result;\n`;
	}

	blocks[CodeBlocks.ALLOCATE] = `
		NRF_RPC_CBOR_ALLOC(ctx, cbor_buffer_size_max);
	`;

	blocks[CodeBlocks.ENCODE_PARAMS] = encodeParamsState.code;

	if (base.isInlineResponse()) {
		throw new Error('Not implemented!')
	} else {
		blocks[CodeBlocks.SEND] = `
			nrf_rpc_cbor_cmd_no_err(&${base.getGroup()}, ${base.getId()}, &_ctx, ${'abc' /* TODO: implement */}, &result);
			`;
	}

	if (base.params._result) {
		blocks[CodeBlocks.RETURN] = `return _result;`;
	}

	return blocks;
}


exports.generateCliSendFunc = generateCliSendFunc;
