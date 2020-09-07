require('./lib/Polyfill').exec();

const Options = require('./lib/Options');
const Units = require('./lib/Units');
const { pathType } = require('./lib/Utils');
const fs = require('fs');
const path = require('path');

function rmRecursive(name)
{
	let type = pathType(name);
	if (type == 'file') {
		fs.unlinkSync(name);
	} else if (type == 'dir') {
		for (let f of fs.readdirSync(name)) {
			if (f == '.' || f == '..') continue;
			rmRecursive(path.join(name, f));
		}
		fs.rmdirSync(name);
	}
}


try {
	Options.parseParams();
	Options.loadConfig();
	let mod = new Units.Module();
	mod.execute();
	mod.save();
} finally {
	if (Options.options.tmpDir !== null) {
		if (Options.options.dumpIntermediateFiles) {
			console.log(`Skipping removal of temporary directory: ${Options.options.tmpDir}`);
		} else {
			rmRecursive(Options.options.tmpDir);
		}
	}
}
