const { StringDecoder } = require('string_decoder');
const { execSync } = require('child_process');
const { writeFileSync, readFileSync, existsSync, lstatSync, fstat } = require('fs');
const fs = require('fs');
const os = require('os');
const path = require('path');

/** @module Options */

let paramList = {
	'--help': ['',
		`Show this help.`,
		() => showHelp()],
	'--config-file': ['=path',
		`Use custom config file. By default search for
		'nrf_rpc_generator.json' file from the input file's directory
		down to the root directory.`,
		v => options.configFile = v],
	'--clang-path': ['=path',
		`Path to Clang command. By default Clang from PATH environment
		variable is used.`,
		v => options.clangPath = v],
	'--dump-ast': ['',
		`Dump Clang AST JSON files (for debugging only).`,
		() => options.dumpIntermediateFiles = true]
};

function parseSingleParam(arg, name, v, f) {
	let valueRequired = (v != '');
	let value = arg.substr(name.length);
	if (valueRequired && value == '') {
		throw Error(`Value required for parameter '${name}'.`);
	}
	if (!valueRequired && value != '') {
		throw Error(`Value not expected for parameter '${name}'.`);
	}
	if (valueRequired) {
		if (value.startsWith('=')) {
			value = value.substr(1);
		}
		f(value);
	} else {
		f();
	}
}


function parseParams() {
	let args = process.argv.slice(2);
	for (let i = 0; i < args.length; i++) {
		let arg = args[i];
		let match = false;
		for (let name in paramList) {
			if (arg.startsWith(name)) {
				parseSingleParam(arg, name, paramList[name][0], paramList[name][2]);
				match = true;
			}
		}
		if (!match) {
			options.inputFile = arg;
		}
	}
	if (options.inputFile === null) {
		throw Error('No input file.');
	}
}

function showHelp() {
	console.log(`\nUSAGE node nrf_rpc_generator/main.js [options] input_file\n`);
	for (let name in paramList) {
		let [value, help] = paramList[name];
		console.log(`${name}${value}\n\t${help.replace(/\s*\r?\n\s*/g, '\n        ').trim()}\n`);
	}
	console.log('');
	process.exit();
}

let options = {
	inputFile: null,
	configFile: null,
	configDir: null,
	scriptDir: null,
	tmpDir: null,
	cliFile: '',
	hostFile: '',
	clangPath: 'clang',
	clangFullPath: null,
	clangParams: '',
	clangCliParams: '',
	clangHostParams: '',
	dumpIntermediateFiles: false,
	indentSize: 8,
	indentString: '\t',
	generator: 'NrfRpcCborGenerator',
	cmdIdPattern: '$_CMD',
	evtIdPattern: '$_EVT',
};

function createIncludeStubs(stubs) {
	let incPath = path.join(options.tmpDir, 'include');
	for (let fileName in stubs) {
		let filePath = path.resolve(incPath, fileName);
		let fileDir = path.dirname(filePath);
		fs.mkdirSync(fileDir, { recursive: true });
		if (typeof (stubs[fileName]) == 'string') {
			fs.writeFileSync(filePath, stubs[fileName]);
		} else {
			fs.writeFileSync(filePath, stubs[fileName].join('\n'));
		}
	}
}

function loadConfig() {
	if (options.configFile === null) {
		let configFile;
		let configDir = path.dirname(options.inputFile);
		let startDir = configDir;
		do {
			configFile = path.resolve(configDir, 'nrf_rpc_generator.json');
			if (existsSync(configFile)) {
				break;
			}
			let next = path.resolve(configDir, '..');
			if (next == configDir || next.endsWith('..') || next.endsWith('..' + path.sep)) {
				throw Error(`Cannot find a 'nrf_rpc_generator.json' config file in '${startDir}' or any of its parent directories.`);
			}
			configDir = next;
		} while (true);
		options.configFile = configFile;
		options.configDir = configDir;
	} else {
		options.configDir = path.dirname(options.configFile);
	}

	let file = readFileSync(options.configFile, 'utf-8');
	let conf = JSON.parse(file);
	let inputReal = fs.realpathSync(options.inputFile);
	let i;
	for (i = 0; i < conf['source-pairs'].length; i++) {
		let pair = conf['source-pairs'][i];
		if (inputReal == fs.realpathSync(path.resolve(options.configDir, pair.cli))
			|| inputReal == fs.realpathSync(path.resolve(options.configDir, pair.host))) {
			for (let k in pair) {
				if (k != 'include-stubs') {
					conf[k] = pair[k];
				} else {
					conf[k] = conf[k] || {};
					for (let k2 in pair[k]) {
						conf[k][k2] = pair[k][k2];
					}
				}
			}
			break;
		}
	}
	if (i >= conf['source-pairs'].length) {
		throw Error(`Cannot find '${options.inputFile}' file in configuration ${options.configFile}`);
	}
	let copyOptions = [
		['cliFile', 'cli'],
		['hostFile', 'host'],
		['clangParams', 'clang-params'],
		['clangCliParams', 'clang-cli-params'],
		['clangHostParams', 'clang-host-params'],
		['indentSize', 'indent-size'],
		['indentString', 'indent-string'],
		['generator', 'generator'],
		['cmdIdPattern', 'cmd-id-pattern'],
		['evtIdPattern', 'evt-id-pattern'],
	];
	for (let o of copyOptions) {
		if (o[1] in conf) {
			options[o[0]] = conf[o[1]];
		}
	}
	options.tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), 'nrf_rpc_gen_'));
	options.scriptDir = fs.realpathSync(path.resolve(__dirname, '..'));
	checkClangVersion();
	createIncludeStubs(conf['include-stubs']);
	if (options.dumpIntermediateFiles) {
		console.log('Options:\n' + JSON.stringify(options, null, 4));
	}
};


function checkClangVersion() {

	function pathType(path) {
		if (!existsSync(path)) return null;
		let stat = lstatSync(path);
		if (stat.isDirectory()) return 'dir';
		return 'file';
	}

	if (pathType(options.clangPath) == 'dir') {
		options.clangFullPath = options.clangPath.replace(/[\/\\]+$/, '');
		if (pathType(`${options.clangFullPath}/clang`) == 'file') {
			options.clangFullPath = `${options.clangFullPath}/clang`;
		} else if (pathType(`${options.clangFullPath}/bin/clang`) == 'file') {
			options.clangFullPath = `${options.clangFullPath}/bin/clang`;
		} else {
			options.clangFullPath = options.clangFullPath;
		}
	} else {
		options.clangFullPath = options.clangPath;
	}


	let output;
	try {
		output = execSync(`${options.clangFullPath} --version`);
	} catch (ex) {
		throw Error('Cannot execute clang command.');
	}
	output = (new StringDecoder()).write(output);
	let m = output.match(/([0-9]+)\.([0-9]+)\.([0-9]+)/);

	if (!m) {
		throw Error('Cannot find clang version information.');
	}

	if (parseInt(m[1]) < 10) {
		throw Error(`Minimum required clang version is 10.0.0. Detected version is ${m[0]}.`);
	}
}

exports.options = options;
exports.parseParams = parseParams;
exports.loadConfig = loadConfig;
