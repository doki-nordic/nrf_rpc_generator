const { readFileSync } = require('fs');
const { StringDecoder } = require('string_decoder');
const { execSync } = require('child_process');


let json = new StringDecoder().write(execSync('/dk/apps/clang/bin/clang -Xclang -ast-dump=json -fsyntax-only -include rp_ser_gen_intern.h api.c'));

let ast = JSON.parse(json);
let src = readFileSync('api.c', 'utf-8');

function resolveLocationsInner(obj, lineMap, current)
{
	if (typeof (obj) != 'object') return;
	if (obj instanceof Array) {
		for (let item of obj) {
			resolveLocationsInner(item, lineMap, current);
		}
	} else {
		if ('loc' in obj) {
			if ('line' in obj.loc) current = { line: obj.loc.line, col: obj.loc.col };
			if ('col' in obj.loc) current = { line: current.line, col: obj.loc.col };
			obj.loc.line = current.line;
			obj.loc.col = current.col;
			obj.loc.abs = lineMap[obj.loc.line] + obj.loc.col - 1;
		}
		if ('range' in obj && 'begin' in obj.range) {
			if (!('line' in obj.range.begin)) obj.range.begin.line = current.line;
			if (!('col' in obj.range.begin)) obj.range.begin.col = current.col;
			obj.range.begin.abs = lineMap[obj.range.begin.line] + obj.range.begin.col - 1;
		}
		if ('range' in obj && 'end' in obj.range) {
			if (!('line' in obj.range.end)) obj.range.end.line = current.line;
			if (!('col' in obj.range.end)) obj.range.end.col = current.col;
			obj.range.end.abs = lineMap[obj.range.end.line] + obj.range.end.col - 1;
		}
		for (let key in obj) {
			resolveLocationsInner(obj[key], lineMap, current);
		}
	}
}

function resolveLocations(ast, src)
{
	let lineMap = [ 0 ];
	let list = src.split('\n');
	let pos = 0;
	for (let i = 0; i < list.length; i++)
	{
		lineMap[i + 1] = pos;
		pos += list[i].length + 1;
	}
	resolveLocationsInner(ast, lineMap, {line: 1, col: 1});
}

resolveLocations(ast, src);

function* findRecursive(obj, condition) {
	if (condition(obj)) yield obj;
	if (typeof (obj) != 'object') return;
	if (obj instanceof Array) {
		for (let item of obj) {
			for (let x of findRecursive(item, condition)) yield x;
		}
	} else {
		for (let key in obj) {
			for (let x of findRecursive(obj[key], condition)) yield x;
		}
	}
}

function findRecursiveFirst(obj, condition) {
	for (let x of findRecursive(obj, condition)) {
		return x;
	}
	return null;
}

function* findSerializeDirectives(decl)
{
	let items = findRecursive(decl, (item) => (typeof(item) == 'object'
		&& item.kind == 'StringLiteral'
		&& item.value.startsWith('"SERIALIZE:')));
	for (let item of items) {
		let str = item.value.substring(11, item.value.length - 1);
		yield str;
	}
}

function warning()
{
}

for (let decl of ast.inner) {
	if (decl.kind == 'TypedefDecl') {
		// TODO: keep callback typedef
	} else if (decl.kind == 'FunctionDecl') {
		let directives = [ ...findSerializeDirectives(decl) ];
		if (directives.length == 0) continue;
		let body = decl.inner.filter(obj => obj.kind == 'CompoundStmt')
		if (body.length == 0) continue;
		body = body[0];
		let func = {
			name: decl.name,
			definition: src.substring(decl.range.begin.abs, body.range.begin.abs),
			defBefore: src.substring(decl.range.begin.abs, decl.loc.abs),
			defName: src.substring(decl.loc.abs, decl.loc.abs + decl.loc.tokLen),
			defAfter: src.substring(decl.loc.abs + decl.loc.tokLen, body.range.begin.abs),
			type: decl.type.qualType,
			params: {},
			directives: directives
		};
		for (let paramDecl of decl.inner.filter(obj => obj.kind == 'ParmVarDecl'))
		{
			let param = {
				name: paramDecl.name,
				type: paramDecl.type.qualType,
				typeBefore: src.substring(paramDecl.range.begin.abs, paramDecl.loc.abs),
				typeAfter: src.substring(paramDecl.loc.abs + paramDecl.loc.tokLen, paramDecl.range.end.abs + paramDecl.range.end.tokLen)
			};
			func.params[param.name] = param;
		}
		console.log(JSON.stringify(func, null, 4));
	}
}

console.log(ast);

setTimeout(() => { }, 10000000);
