const { writeFileSync } = require('fs');
const { execSync } = require('child_process');
const { StringDecoder } = require('string_decoder');
const { SourceFragments } = require('./SourceFragments');
const { findRecursive, filterRecursive } = require('./Utils');
const CodeBlocks = require('./CodeBlocks');
const NoiselessTags = require('./NoiselessTags');


const CLIENT = exports.CLIENT = 'CLIENT';
const HOST = exports.HOST = 'HOST';
const OTHER = exports.OTHER = 'OTHER';


class Funct {
	constructor(name) {
		this.name = name;
		this.node = null;
		this.defined = false;
		this.side = null;
		this.src = null;
	}

	add(node, side, src) {
		if (this.defined) {
			// ignore if already defined
			return;
		}

		this.defined = !!findRecursive(node.inner, inner => (
			inner.kind == 'CompoundStmt'
		));

		if (!this.defined && this.node) {
			// ignore repeated declarations
			return;
		}

		this.node = node;
		this.side = node.loc.isInput ? side : OTHER;
		this.src = node.loc.isInput ? src : null;
	}

	static getName(node) {
		return node.name;
	}

	getReturnType() {

		let type = this.node.type.qualType;
		let index = type.length - 1;
		let brackets = 1;
		while (index > 0 && brackets > 0) {
			index--;
			if (type[index] == '(') {
				brackets--;
			} else if (type[index] == ')') {
				brackets++;
			}
		}
		return type.substr(0, index).trim();
	}

	*getParams() {

		let paramsNode = this.node.inner.filter(n => n.kind == 'ParmVarDecl');
		for (let node of paramsNode) {
			yield [node.name, node.type.qualType.trim()];
		}
	}

	*getSerializeMacros(key) {

		let list;

		if (key) {
			list = filterRecursive(this.node, inner => (
				inner.kind == 'StringLiteral' &&
				(inner.value.startsWith(`"__SERIALIZE__:${key}=`) ||
					inner.value == `"__SERIALIZE__:${key}`)
			));
		} else {
			list = filterRecursive(this.node, inner => (
				inner.kind == 'StringLiteral' &&
				inner.value.startsWith('"__SERIALIZE__:')
			));
		}
		for (let node of list) {
			let text = node.value.substring(15, node.value.length - 1);
			let index = text.indexOf('=');
			if (index >= 0) {
				yield [text.substr(index + 1), text.substr(0, index)];
			} else {
				yield [undefined, text];
			}
		}
	}

	regenerate(blocks) {

		blocks = Object.assign({}, blocks);
		delete blocks[CodeBlocks.HEADER];
		delete blocks[CodeBlocks.FOOTER];

		if (!this.defined) {
			throw Error('Internal: Cannot regenerate not defined function.');
		}

		let bodyNode = this.node.inner.find(node => node.kind == 'CompoundStmt');
		let bodyStart = bodyNode.range.begin.offset;
		let bodyEnd = bodyNode.range.end.offset + 1;

		let userBlocks = NoiselessTags.extract(this.src.substring(bodyStart + 1, bodyEnd - 1), CodeBlocks.BEGIN);

		let newCode = NoiselessTags.generate(blocks, userBlocks);

		let bodyFragment = this.src.create(bodyStart + 1, bodyEnd - 1);
		bodyFragment.text = '\n' + newCode;
	}

};


class Struct {
	constructor(name) {
		this.name = name;
		this.node = null;
		this.defined = false;
		this.side = null;
		this.src = null;
	}

	add(node, side, src) {
		if (this.defined) {
			// ignore declarations if already defined
			return;
		}

		this.defined = !!findRecursive(node.inner, inner => (
			inner.kind == 'FieldDecl'
		));

		if (!this.defined && this.node) {
			// ignore repeated declarations
			return;
		}

		this.node = node;
		this.side = node.loc.isInput ? side : OTHER;
		this.src = node.loc.isInput ? src : null;
	}

	static getName(node) {
		return `struct ${node.name}`;
	}
};


class Callback {
	constructor(name) {
		this.name = name;
		this.node = null;
		this.side = null;
		this.src = null;
	}

	add(node, side, src) {
		if (this.node) {
			// ignore repeated declarations
			return;
		}

		this.node = node;
		this.side = node.loc.isInput ? side : OTHER;
		this.src = node.loc.isInput ? src : null;
	}

	static getName(node) {
		return node.name;
	}
};


class Parser {

	constructor(options) {

		this.cliSrc = options.cliSrc;
		this.hostSrc = options.hostSrc;
		this.clangPath = options.clangPath;
		this.clangParams = options.clangParams;
		this.clangCliParams = options.clangCliParams;
		this.clangHostParams = options.clangHostParams;
		this.generatorInclude = options.generatorInclude;
		this.dumpIntermediateFiles = options.dumpIntermediateFiles;

		this.symbols = {};
		this.functions = {};
		this.structures = {};
		this.callbacks = {};

		[this.cliAst, this.cliFragments] = this._parseFile(this.cliSrc, this.clangCliParams);
		[this.hostAst, this.hostFragments] = this._parseFile(this.hostSrc, this.clangHostParams);

		this._collectSymbols(this.cliAst, CLIENT);
		this._collectSymbols(this.hostAst, HOST);
	}


	_collectSymbols(ast, side) {

		this._collectItems(ast, this.functions, side, Funct, node =>
			node.kind == 'FunctionDecl'
		);

		this._collectItems(ast, this.structures, side, Struct, node =>
			node.kind == 'RecordDecl' &&
			node.tagUsed == 'struct' &&
			'name' in node
		);

		this._collectItems(ast, this.callbacks, side, Callback, node =>
			node.kind == 'TypedefDecl' &&
			'inner' in node &&
			node.inner.some(node2 =>
				node2.kind == 'PointerType' &&
				'inner' in node2 &&
				node2.inner.some(node3 =>
					node3.kind == 'ParenType' &&
					'inner' in node3 &&
					node3.inner.some(node4 =>
						node4.kind == 'FunctionProtoType'
					)
				)
			)
		);
	}


	_parseFile(file, specificParams) {

		let json;
		try {
			// TODO: more advanced method of executing clang
			let output = execSync(`${this.clangPath} -Xclang -ast-dump=json -fsyntax-only -include ${this.generatorInclude} ${this.clangParams} ${specificParams} ${file}`);
			json = new StringDecoder().write(output);
		} catch (ex) {
			if ('stdout' in ex && ex.stdout.length > 0) {
				json = new StringDecoder().write(ex.stdout);
			} else {
				console.log(ex);
				process.exit(1);
			}
		}

		if (this.dumpIntermediateFiles) {
			writeFileSync(`${file}.ast.json`, json);
		};

		let ast = JSON.parse(json);
		Parser._checkAstFiles(ast, file);

		return [ast, new SourceFragments(file)];
	}


	static _checkAstFiles(node, fileName, state) {
		if (typeof (node) == 'object') {
			state = state || { isInput: false };
			if (node instanceof Array) {
				for (let item of node) {
					Parser._checkAstFiles(item, fileName, state);
				}
			} else {
				if ('offset' in node && 'col' in node) {
					if ('file' in node) {
						state.isInput = (node.file == fileName);
					}
					node.isInput = state.isInput;
				}
				for (let i in node) {
					Parser._checkAstFiles(node[i], fileName, state);
				}
			}
		}
	}


	_collectItems(ast, collection, side, Class, callback) {

		let list = ast.inner.filter(callback);

		for (let node of list) {
			let name = Class.getName(node);
			if (name in this.symbols) {
				if (!(this.symbols[name] instanceof Class)) {
					throw Error(`Symbol ${name} declared multiple times.`);
				}
			} else {
				this.symbols[name] = new Class(name);
				collection[name] = this.symbols[name];
			}
			this.symbols[name].add(node, side, side == CLIENT ? this.cliFragments : this.hostFragments);
		}
	}

};


exports.Parser = Parser;
