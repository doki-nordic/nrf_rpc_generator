const Parsing = require("./Parsing");
const NrfRpcCborGenerator = require("./NrfRpcCborGenerator");
const { options } = require("./Options");


/** @module Units */


const IN = exports.IN = "IN";
const OUT = exports.OUT = "OUT";
const INOUT = exports.INOUT = "INOUT";


const RSP_FUNC_POSTFIX = exports.RSP_FUNC_POSTFIX = '_rpc_rsp';
const RES_STRUCT_POSTFIX = exports.RES_STRUCT_POSTFIX = '_rpc_res';
const RECV_FUNC_POSTFIX = exports.RECV_FUNC_POSTFIX = '_rpc_handler';
const CALLBACK_RECV_POSTFIX = exports.CALLBACK_RECV_POSTFIX = '_rpc_cbk_handler';

let generator = null;

function getGenerator()
{
	if (generator === null) {
		generator = require(`./${options.generator}`);
	}
	return generator;
}


/** Groups all functionality for function serialization.
 * * Client:
 *	* (sendFunc)  xyz - encode parameters, send and optionally parse response
 *	* (rspFunc)   xyz_rpc_rsp - (optional) parse response
 *	* (resStruct) struct xyz_rpc_res - (optional) parsing response results
 * * Host:
 *	* (recvFunc)  xyz_rpc_handler - decode parameters, execute, encode response
 *      * (regMacro)  xyz_rpc_reg - macro registering handler
 * 
 * 
 * 
 * SERIALIZE() macros:
 * 	EVENT - serialize a an event
 * 	INLINE_RESPONSE - parse response in the same function
 * 	IGNORE_RETURN - ignore that function returns a value, generate function as it returns 'void'.
 * 	CUSTOM_RESPONSE - prevent from using standard response handlers, but always generate a new response handler.
 * 	OUT(param) - serialize 'param' as an output.
 * 	INOUT(param) - serialize 'param' as on input-output.
 * 	STR(param) - serialize 'param' as a string (not a pointer to a 'char').
 * 	ADD_IN(type, param) - add custom input parameter.
 * 	ADD_OUT(type, param) - add custom output paramter.
 * 	ADD_INOUT(type, param)- add custom input-output paramter.
 * 
 */
class UnitFunc {

	constructor(module, name) {

		this.module = module;
		this.parser = module.parser;
		this.name = name;
		this.sendFunc = null;
		this.sendFuncPlaceholder = null;
		this.rspFunc = null;
		this.rspFuncPlaceholder = null;
		this.params = {};
		this.isEvent = false;
		this.functions = [];
		this.generator = null;

		this._collectSymbols();

		this.isEvent = this._checkAnnotation('EVENT');
		this.ignoreReturn = this._checkAnnotation('IGNORE_RETURN');
		this.customResponse = this._checkAnnotation('CUSTOM_RESPONSE');
		this.inlineResponse = this._checkAnnotation('INLINE_RESPONSE');
		this.responseHandlerGenerate = !this.inlineResponse;

		if (this.isEvent && this.inlineResponse) {
			throw Error(`Inline response is not possible for event in function '${this.name}'.`);
		}

		let returnType = this.sendFunc.getReturnType();

		if (returnType != 'void' && !this.ignoreReturn) {
			this.params['_result'] = {
				name: '_result',
				type: returnType,
				dir: OUT,
				isReturn: true
			}
		}

		for (let [name, type] of this.sendFunc.getParams()) {
			if (name === undefined) {
				throw Error(`All parameters in '${this.name}' must be named.`);
			}
			this.params[name] = {
				name: name,
				type: type,
				dir: IN
			}
		}

		for (let m of this.sendFunc.getSerializeMacros('OUT')) {
			if (!(m.value in this.params)) {
				throw new Error(`Parameter '${m.value}' not found for SERIALIZE(OUT(...)) in '${this.name}'.`);
			}
			this.params[m.value].dir = OUT;
		}

		for (let m of this.sendFunc.getSerializeMacros('INOUT')) {
			if (!(m.value in this.params)) {
				throw new Error(`Parameter '${m.value}' not found for SERIALIZE(INOUT(...)) in '${this.name}'.`);
			}
			this.params[m.value].dir = INOUT;
		}

		for (let m of this.sendFunc.getSerializeMacros('ADD')) {
			let [dir, type, name] = m.value.split('`');
			dir = dir.toUpperCase();
			this.params[name] = {
				name: name,
				type: type,
				dir: dir == 'IN' ? IN : dir == 'OUT' ? OUT : dir == 'INOUT' ? INOUT : null
			}
			if (this.params[name].dir === null) {
				throw Error(`Invalid direction specifier for SERIALIZE(ADD(...)) in function ${this.name}`);
			}
		}

		for (let m of this.sendFunc.getSerializeMacros('STR')) {
			if (!(m.value in this.params)) {
				throw new Error(`Parameter '${m.value}' not found for SERIALIZE(STR(...)) in '${this.name}'.`);
			}
			this.params[m.value].isString = true;
		}

		for (let m of this.sendFunc.getSerializeMacros('SIZE')) {
			let [name, size] = m.value.split('`');
			this.params[name].isArray = true;
			this.params[name].sizePattern = size;
			this.params[name].sizeParam = null;
		}

		for (let m of this.sendFunc.getSerializeMacros('SIZE_PARAM')) {
			let [name, size] = m.value.split('`');
			this.params[name].isArray = true;
			this.params[name].sizePattern = '$';
			this.params[name].sizeParam = size;
			this.params[size].isSize = true;
			this._putParamBefore(size, name);
		}

		for (let m of this.sendFunc.getSerializeMacros('SIZE_PARAM_EX')) {
			let [name, pattern, size] = m.value.split('`');
			this.params[name].isArray = true;
			this.params[name].sizePattern = pattern;
			this.params[name].sizeParam = size;
			this.params[size].isSize = true;
			this._putParamBefore(size, name);
		}

	}

	_putParamBefore(first, later)
	{
		let keys = Object.keys(this.params);
		let firstIndex = keys.indexOf(first);
		let laterIndex = keys.indexOf(later);
		if (firstIndex < 0 || laterIndex < 0) {
			throw Error(`Cannot find '${first}' or '${later}' parameter.`);
		}
		if (firstIndex <= laterIndex) {
			return;
		}
		let entries = Object.entries(this.params);
		let x = entries.splice(firstIndex, 1)[0];
		entries.splice(laterIndex, 0, x);
		this.params = Object.fromEntries(entries);
	}

	_checkAnnotation(name) // TODO: Rename serialize macro to annotation
	{
		for (let f of this.functions) {
			if (f.getSerializeMacros(name).length > 0) {
				return true;
			}
		}
		return false;
	}

	_collectSymbols() {
		if (!(this.name in this.parser.cliFunctions)) {
			throw Error(`Generation of function '${this.name}' was requested, but function is undefined.`);
		}

		this.sendFunc = this.parser.cliFunctions[this.name];

		if (this.sendFunc.defined) {
			this.functions.push(this.sendFunc);
			if (this.sendFunc.side != Parsing.CLIENT) {
				throw Error(`Function '${this.name}' defined on side different than client side.`);
			}
		} else {
			let list = this.parser.getSerializeMacros('FUNC').filter(m => m.value == this.name);
			if (list.length < 1) {
				throw Error(`Function '${this.name}' generation location is unknown. Use 'SERIALIZE(FUNC(${this.name}))'.`);
			}
			this.sendFuncPlaceholder = list[0];
		}

		this.rspFunc = this.parser.cliFunctions[this.name + RSP_FUNC_POSTFIX];

		if (!this.rspFunc) {
			this.rspFuncPlaceholder = this.sendFuncPlaceholder ? this.sendFuncPlaceholder.placeholderBefore() : this.sendFunc.placeholderBefore();
		} else {
			this.functions.push(this.rspFunc);
		}

		this.resStruct = this.parser.cliStructures['struct ' + this.name + RES_STRUCT_POSTFIX];

		if (!this.resStruct) {
			this.resStructPlaceholder = this.rspFunc ? this.rspFunc.placeholderBefore() : this.rspFuncPlaceholder.placeholderBefore();
		}

		this.recvFunc = this.parser.hostFunctions[this.name + RECV_FUNC_POSTFIX];

		if (!this.recvFunc) {
			this.recvFuncPlaceholder = this.parser.placeholderOnHost();
		} else {
			this.functions.push(this.recvFunc);
		}

		// TODO: this.regMacro

	}

	generate() {

		let generator = new (getGenerator().GenFunc)(this);

		generator.generateCliSendFunc(this.sendFunc.defined ? this.sendFunc : this.sendFuncPlaceholder); // false

		if (this.responseHandlerGenerate) {
			generator.generateCliRspFunc(this.rspFunc || this.rspFuncPlaceholder); // true
			generator.generateCliResStruct(this.resStruct || this.resStructPlaceholder); // true
		} else {
			if (this.rspFunc) {
				this.rspFunc.regenerate({}, true);
			}
			if (this.resStruct) {
				this.resStruct.regenerate({}, true);
			}
		}

		generator.generateHostRecvFunc(this.recvFunc || this.recvFuncPlaceholder); // true
	}

	getGroup() {
		return this.module.group;
	}

	getId() {
		let pattern = this.isEvent ? this.module.evtPattern : this.module.cmdPattern;
		return pattern.replace('$', this.name.toUpperCase());
	}

}


/** Top level class */
class Module {

	constructor(filePath) {
		this.parser = new Parsing.Parser(filePath);
		this.generatedUnitFunc = {};
		this.cmdPattern = this.parser.getSerializeMacro('CMD_ID');
		this.evtPattern = this.parser.getSerializeMacro('EVT_ID');
		if (!this.cmdPattern || !this.evtPattern) {
			throw Error(`Cannot create command or event ID. Use both 'SERIALIZE(CMD_ID(...))' and 'SERIALIZE(EVT_ID(...))'.`);
		}
		this.cmdPattern = this.cmdPattern.value;
		this.evtPattern = this.evtPattern.value;
		this.group = this.parser.getSerializeMacro('GROUP');
		if (!this.group) {
			throw Error(`Group is unknown. Use 'SERIALIZE(GROUP(...))'.`);
		}
		this.group = this.group.value;

		this.definedCallbacks = {};
	}


	isCallback(name) {
		return !!this.definedCallbacks[name];
	}


	execute() {

		for (let name in this.parser.cliCallbacks) {
			this.definedCallbacks[name] = true;
		}

		for (let name in this.parser.commonFunctions) {

			let func = this.parser.commonFunctions[name];

			if (name.endsWith(RSP_FUNC_POSTFIX)) {
				this._generateUnitFunc(name.substr(0, name.length - RSP_FUNC_POSTFIX.length));
			} else if (name.endsWith(RECV_FUNC_POSTFIX)) {
				this._generateUnitFunc(name.substr(0, name.length - RECV_FUNC_POSTFIX.length));
			} else if (func.isSerializable()) {
				this._generateUnitFunc(name);
			}
		}

		for (let name in this.parser.commonStructures) {

			if (!name.startsWith('struct ')) {
				continue;
			}

			if (name.endsWith(RES_STRUCT_POSTFIX)) {
				this._generateUnitFunc(name.substring(7, name.length - RSP_FUNC_POSTFIX.length));
			}
		}

		for (let m of this.parser.getSerializeMacros('FUNC')) {

			this._generateUnitFunc(m.value);
		}
	}


	_generateUnitFunc(name) {

		if (name in this.generatedUnitFunc) {
			return;
		}
		this.generatedUnitFunc[name] = true;

		let f = new UnitFunc(this, name);
		f.generate();
	}


	save() {
		this.parser.save();
	}

}


exports.UnitFunc = UnitFunc;
exports.Module = Module;
