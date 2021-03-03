
const fs = require('fs');
const { serialize } = require('v8');

function strp_file(f) {

	code = fs.readFileSync(f, 'utf-8')
	initial_code = code;

	if (!code.match(/^[ \t]*SERIALIZE\(/m) || !code.match(/\/\*##+[A-Za-z@%0-9\/\+]*\*\/$/m))
	{
		return;
	}

	console.log(f);

	let crlf = false;

	if (code.indexOf('\r') >= 0) {
		code.replace('\r', '');
		crlf = true;
	}

	code = code.replace(/\s*\/\*##+[A-Za-z@%0-9\/\+]*\*\/$/gm, '');
	code = code.replace(/[ \t]+$/gm, '');
	code = code.replace(/^[ \t]*SERIALIZE\(.*$/gm, 'ø');
	code = code.replace(/\{(\nø)+\n(?=\n)/g, '{');
	code = code.replace(/\n(\nø)+\n(?=\n)/g, '\n');
	code = code.replace(/(\nø)+\n(?=\n)/g, '\n');
	code = code.replace(/\n(\nø)+(?=\n)/g, '\n');
	code = code.replace(/(\nø)+(?=\n)/g, '\n');

	if (crlf) {
		code = code.replace('\n', '\r\n');
	}

	fs.writeFileSync(f, code);
}

function strip_dir(dir)
{
	for (let file of fs.readdirSync(dir)) {
		let path = `${dir}/${file}`;
		if (fs.lstatSync(path).isDirectory()) {
			strip_dir(path);
		} else if (file.endsWith('.c')) {
			strp_file(path);
		}
	}
}

if (process.argv.length < 3) {
	console.log('USAGE: node strip.js directory');
	process.exit(1);
}

strip_dir(process.argv[2]);
