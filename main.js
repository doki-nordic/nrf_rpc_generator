
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

let mod = new Units.Module('bluetooth/bt_gap_cli.c');
mod.execute();
mod.save();
