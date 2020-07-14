const CodeBlocks = require('./CodeBlocks');

let platformIntTypesText = `
s8:
	int8_t
	i8_t
	char
	signed char

u8:
	uint8_t
	u8_t
	unsigned char

s16:
	int16_t
	i16_t
	short
	short int
	signed short
	signed short int

u32:
	uint16_t
	u16_t
	unsigned short
	unsigned short int

s32:
	int32_t
	i32_t
	ssize_t
	intptr_t
	int
	signed int
	long
	long int
	signed long
	signed long int

u32:
	uint32_t
	u32_t
	size_t
	uintptr_t
	unsigned
	unsigned int
	unsigned long
	unsigned long int

s64:
	int64_t
	i64_t
	long long
	long long int
	signed long long
	signed long long int

u64:
	uint64_t
	u64_t
	unsigned long long
	unsigned long long int
`;


let platformIntTypes = {};

let info = null;
for (let line of platformIntTypesText.split('\n')) {
	line = line.trim();
	if (line.length == 0) continue;
	let m = line.match(/^(s|u)([0-9]{1,2}):$/);
	if (m) {
		info = {signed: (m[1] == 's'), bytes: m[2] / 8};
	} else {
		platformIntTypes[line] = info;
	}
}

class Func {

	constructor(baseFunc) {
		this.base = baseFunc;
		this.blocks = {};
	}

	generateEncode(state, name, type) {

		if (type.endsWith('*')) {
			name = `(*${name})`;
			type = type.substring(0, type.length - 1).trim();
			this.generateEncode(state, name, type);
			return;
		}

		info = platformIntTypes[type];

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

	generate() {

		let blocks = {};
		this.blocks = blocks;
		let base = this.base;

		let encodeParamsState = {maxSize:0, code:''};

		for (let param of Object.values(base.params).filter(x => x.dir == 'IN' || x.dir == 'INOUT')) {
			this.generateEncode(encodeParamsState, param.name, param.type);
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

		if (base.sendFunc.defined) {
			base.sendFunc.regenerate(blocks);
		} else {
			// TODO: Put entire function on SERIALIZE(FUNC(xyz)) macro.
			throw new Error('Not implemented');
		}

		console.log(JSON.stringify(blocks, null, 4));
		console.log(Object.values(blocks).join('\n'));
	}

}


exports.Func = Func;
