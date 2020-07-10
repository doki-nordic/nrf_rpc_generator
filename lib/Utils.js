

function* filterRecursive(obj, condition) {
	if (typeof (obj) != 'object') return;
	if (condition(obj)) yield obj;
	if (obj instanceof Array) {
		for (let item of obj) {
			for (let x of filterRecursive(item, condition)) yield x;
		}
	} else {
		for (let key in obj) {
			for (let x of filterRecursive(obj[key], condition)) yield x;
		}
	}
}


function findRecursive(obj, condition) {
	for (let x of filterRecursive(obj, condition)) {
		return x;
	}
	return null;
}


exports.filterRecursive = filterRecursive;
exports.findRecursive = findRecursive;
