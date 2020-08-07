
function exec() {
	if (!('fromEntries' in Object)) {
		Object.fromEntries = arr => Object.assign({}, ...Array.from(arr, ([k, v]) => ({ [k]: v })));
	}
}

exports.exec = exec;
