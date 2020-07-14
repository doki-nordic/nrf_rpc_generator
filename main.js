const { writeFileSync } = require('fs');
const Parser = require('./lib/Parser');
const CodeBlocks = require('./lib/CodeBlocks');
const NrfRpcCborGenerator = require('./lib/NrfRpcCborGenerator');
const { findRecursive } = require('./lib/Utils');

setTimeout(() => { }, 2300);

let options = {
	cliSrc: 'api_cli.c',
	hostSrc: 'api_host.c',
	clangPath: '/dk/apps/clang/bin/clang',
	clangParams: '',
	clangCliParams: '',
	clangHostParams: '',
	generatorInclude: 'rp_ser_gen_intern.h',
	dumpIntermediateFiles: true
};

let parser = new Parser.Parser(options);

const IN = "IN";
const OUT = "OUT";
const INOUT = "INOUT";

class Func {

	constructor(parser, name) {

		this.parser = parser;
		this.name = name;

		if (!(name in this.parser.functions)) {
			throw Error(`Handler or response parser defined for function '${func.name}', but function itself is undefined.`);
		}

		this.sendFunc = this.parser.functions[name];
		if (!this.sendFunc.defined) {
			let frag = this.parser.cliFragments.create(this.parser.cliFragments.length, this.parser.cliFragments.length);
			//frag.text = `// TODO: Implement ${this.name}\n`; // TODO: Implement send function code generation.
			throw Error('Unimplemented');
		}
		if (this.sendFunc.side != Parser.CLIENT) {
			throw Error(`Function '${name}' defined on side different than client side.`);
		}

		this.params = {};

		let returnType = this.sendFunc.getReturnType();

		if (returnType != 'void') {
			this.params['_result'] = {
				name: '_result',
				type: returnType,
				dir: OUT
			}
		}

		for (let [name, type] of this.sendFunc.getParams()) {
			if (name === undefined) {
				throw Error(`All parameters in '${this.name}' must be named.`);
			}
			this.params[name] = {
				name: name,
				type: type,
				dir: IN
			}
		}

		for (let [value] of this.sendFunc.getSerializeMacros('OUT')) {
			if (!(value in this.params)) {
				throw new Error(`Input parameter '${value}' not found for SERIALIZE(OUT(...)) in '${this.name}'.`);
			}
			this.params[value].dir = OUT;
			break;
		}

		for (let [value] of this.sendFunc.getSerializeMacros('INOUT')) {
			if (!(value in this.params)) {
				throw new Error(`Input parameter '${value}' not found for SERIALIZE(INOUT(...)) in '${this.name}'.`);
			}
			this.params[value].dir = INOUT;
			break;
		}

		console.log(JSON.stringify(this.params, null, 4));
	}

	generate() {

		let g = new NrfRpcCborGenerator.Func(this);

		g.generate();

	}

	isInlineResponse()
	{
		return false; // TODO: implement
	}

	getGroup()
	{
		return 'GROUP'; // TODO: implement
	}

	getId()
	{
		return 'SER_SAMPLE_ID'; // TODO: implement
	}

}

/*

##HEADER
${declaration}
{
##LOCALS
	struct nrf_rpc_cbor_ctx _ctx;
	size_t _cbor_buffer_size_max = ${items.reduce((x, item)) => x + item.max_size};
	int _result;
##ALLOC
	NRF_RPC_CBOR_ALLOC(_ctx, _cbor_buffer_size_max);
##ENCODE
	$${for x  }
##SEND
	nrf_rpc_cbor_cmd_no_err(&entropy_group, RPC_COMMAND_ENTROPY_GET, &ctx,
			        ser_simple_rsp_int, &result);
##RETURN
	return result;
##FOOTER
}
*/

const FUNCTION_RESPONSE_POSTFIX = '_rpc_response';
const FUNCTION_HANDLER_POSTFIX = '_rpc_handler';


function isSerializable(func) {

	return !!findRecursive(func.node.inner, inner => (
		inner.kind == 'CompoundStmt' &&
		findRecursive(inner.inner, inner2 => (
			inner2.kind == 'StringLiteral' &&
			inner2.value.startsWith('"__SERIALIZE__:')
		))
	));
}


for (let name in parser.functions) {

	let func = parser.functions[name];
	let baseName;
	let Class;

	if (name.endsWith(FUNCTION_RESPONSE_POSTFIX)) {
		baseName = name.substr(0, name.length - FUNCTION_RESPONSE_POSTFIX.length);
		Class = Func;
	} else if (name.endsWith(FUNCTION_HANDLER_POSTFIX)) {
		baseName = name.substr(0, name.length - FUNCTION_HANDLER_POSTFIX.length);
		Class = Func;
	} else if (isSerializable(func)) {
		baseName = name;
		Class = Func;
	} else {
		// Ignore irrelevant functions
		continue;
	}

	let entity = new Class(parser, baseName);

	entity.generate();

}

writeFileSync('api_cli.c', parser.cliFragments.generate());


/*

FUNCTION
	client:
	xyz - encode parameters, send and optionally parse response
	xyz_rpc_response - (optional) parse response

	host:
	xyz_rpc_handler - decode parameters, execute, encode response

CALLBACK
	any:
	xyz - callback typedef

	client:
	xyz_rpc_cbk_handler - decode, execute and send response

	host:
	xyz_rpc_cbk - encode parameters, send and optionally parse response
	xyz_rpc_cbk_response - (optional) parse response

STRUCTURE:
	any:
	struct xyz - structure definition

	client/host:
	xyz_rpc_struct_enc
	xyz_rpc_struct_dec

*/
