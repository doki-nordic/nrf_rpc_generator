const Parsing = require("./Parsing");
const NrfRpcCborGenerator = require("./NrfRpcCborGenerator");


/** @module Units */


const IN = exports.IN = "IN";
const OUT = exports.OUT = "OUT";
const INOUT = exports.INOUT = "INOUT";


const RSP_FUNC_POSTFIX = '_rpc_rsp';
const RES_STRUCT_POSTFIX = '_rpc_res';
const RECV_FUNC_POSTFIX = '_rpc_handler';

const CALLBACK_RECV_POSTFIX = exports.CALLBACK_RECV_POSTFIX = '_rpc_cbk_handler';



/** Groups all functionality for function serialization.
 * * Client:
 *	* (sendFunc)  xyz - encode parameters, send and optionally parse response
 *	* (rspFunc)   xyz_rpc_rsp - (optional) parse response
 *	* (resStruct) struct xyz_rpc_res - (optional) parsing response results
 * * Host:
 *	* (recvFunc)  xyz_rpc_handler - decode parameters, execute, encode response
 *      * (regMacro)  xyz_rpc_reg - macro registering handler
 */
class UnitFunc {

	constructor(module, name) {

		this.module = module;
		this.parser = module.parser;
		this.name = name;

		if (!(name in this.parser.functions)) {
			throw Error(`Generation of function '${func.name}' was requested, but function is undefined.`);
		}

		this.sendFunc = this.parser.functions[name];

		if (this.sendFunc.defined) {
			if (this.sendFunc.side != Parsing.CLIENT) {
				throw Error(`Function '${name}' defined on side different than client side.`);
			}
		} else {
			let list = this.parser.getSerializeMacros('FUNC').filter(m => m.value == name);
			if (list.length < 1) {
				throw Error(`Function '${name}' generation location is unknown. Use 'SERIALIZE(FUNC(${name}))'.`);
			}
			this.sendFuncPlaceholder = list[0];
		}

		this.params = {};

		let returnType = this.sendFunc.getReturnType();

		if (returnType != 'void' && !this.sendFunc.getSerializeMacros('IGNORE_RETURN').length) {
			this.params['_result'] = {
				name: '_result',
				type: returnType,
				dir: OUT
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
			break;
		}

		for (let m of this.sendFunc.getSerializeMacros('INOUT')) {
			if (!(m.value in this.params)) {
				throw new Error(`Parameter '${m.value}' not found for SERIALIZE(INOUT(...)) in '${this.name}'.`);
			}
			this.params[m.value].dir = INOUT;
			break;
		}

		for (let m of this.sendFunc.getSerializeMacros('STR')) {
			if (!(m.value in this.params)) {
				throw new Error(`Parameter '${m.value}' not found for SERIALIZE(STR(...)) in '${this.name}'.`);
			}
			this.params[m.value].isString = true;
			break;
		}

		this.isInlineResponse = !!this.sendFunc.getSerializeMacros('INLINE_RESPONSE').length;
		this.isEvent = !!this.sendFunc.getSerializeMacros('EVENT').length;

		if (this.isEvent && this.isInlineResponse) {
			throw Error(`Inline response is not possible for event in function '${this.name}'.`);
		}
	}

	generate() {

		let blocks = NrfRpcCborGenerator.generateCliSendFunc(this);
		if (this.sendFunc.defined) {
			this.sendFunc.regenerate(blocks);
		} else {
			this.sendFuncPlaceholder.regenerate(blocks);
		}
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

		for (let name in this.parser.callbacks) {
			this.definedCallbacks[name] = true;
		}

		for (let name in this.parser.functions) {

			let func = this.parser.functions[name];

			if (name.endsWith(RSP_FUNC_POSTFIX)) {
				this._generateUnitFunc(name.substr(0, name.length - RSP_FUNC_POSTFIX.length));
			} else if (name.endsWith(RECV_FUNC_POSTFIX)) {
				this._generateUnitFunc(name.substr(0, name.length - RECV_FUNC_POSTFIX.length));
			} else if (func.isSerializable()) {
				this._generateUnitFunc(name);
			}
		}

		for (let name in this.parser.structures) {

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
