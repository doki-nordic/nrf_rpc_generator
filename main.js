
if (!('fromEntries' in Object)) {
	Object.fromEntries = arr => Object.assign({}, ...Array.from(arr, ([k, v]) => ({ [k]: v })));
}

const { writeFileSync } = require('fs');
const Parser = require('./lib/Parsing');
const CodeBlocks = require('./lib/CodeBlocks');
const NrfRpcCborGenerator = require('./lib/NrfRpcCborGenerator');
const { findRecursive } = require('./lib/Utils');
const { parse } = require('path');
const Units = require('./lib/Units')

//setTimeout(() => { }, 2300);

let mod = new Units.Module('bluetooth/bt_gap_cli.c');
mod.execute();
mod.save();

/*

int x
	in

int *x
	in/out/inout

struct s x
	in

struct s *x
	in/out/inout

int *x, int size
	in+in, out+in,

int *x, int *size
	in+in, in+inout, out+in, out+out, out+inout

char *str
	in


*/
/*let parser = new Parser.Parser('api_cli.c');

const FUNCTION_RESPONSE_POSTFIX = '_rpc_response';
const FUNCTION_HANDLER_POSTFIX = '_rpc_handler';

let groupMacro = parser.getSerializeMacro('GROUP');
if (!groupMacro) {
	throw Error(`Unknown group. Specify group name with the SERIALIZE(GORUP(...)) macro.`);
}

console.log(groupMacro.value);


writeFileSync('api_cli.c', parser._cliSrc.generate());


/*

FUNCTION
	client:
	xyz - encode parameters, send and optionally parse response
	xyz_rpc_response - (optional) parse response
	struct xyz_rpc_results - (optional) parsing response results

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
