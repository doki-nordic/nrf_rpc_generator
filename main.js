require('./lib/Polyfill').exec();

const Options = require('./lib/Options');
const Units = require('./lib/Units');
const fs = require('fs');

try {
	Options.parseParams();
	Options.loadConfig();
	let mod = new Units.Module();
	mod.execute();
	mod.save();
} finally {
	if (Options.options.tmpDir !== null) {
		//TODO: fs.rmdirSync(Options.options.tmpDir, { recursive: true });
	}
}
