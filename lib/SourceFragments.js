
const { readFileSync, writeFileSync } = require('fs');


/** @module SourceFragments */


/** Class loads a source code and allows dividing it to fragments that can be
 * modified independently.
 */
class SourceFragments {

	/** Load a source code and creates one immutable fragment from it.
	 */
	constructor(sourceFile) {
		let src = readFileSync(sourceFile, 'utf-8');
		this.frags = [{
			begin: 0,
			end: src.length,
			mutable: false,
			text: src
		}];
		this.length = src.length;
	}

	/** Create a mutable fragment by dividing a immutable fragment into
	 * three fragments: immutable before requested one, requested one and
	 * immutable after requested one.
	 * 
	 * `text` field of returned object is a fragment string that can be
	 * modified.
	 */
	create(begin, end, atTheEnd) {

		let i;
		if (atTheEnd) {
			let k;
			for (k = this.frags.length - 1; k >= 0; k--) {
				let x = this.frags[k];
				if (x.begin <= begin && end <= x.end && !x.mutable) {
					break;
				}
			}
			i = k;
		} else {
			i = this.frags.findIndex(x => (x.begin <= begin && end <= x.end && !x.mutable));
		}
		if (i < 0) {
			throw Error(`Internal error: Requested fragment is invalid or overlaps with others.`);
		}

		let frag = this.frags[i];

		let prevFrag = {
			begin: frag.begin,
			end: begin,
			mutable: false,
			text: frag.text.substr(0, begin - frag.begin)
		};
		let thisFrag = {
			begin: begin,
			end: end,
			mutable: true,
			text: frag.text.substr(begin - frag.begin, end - begin)
		};
		let nextFrag = {
			begin: end,
			end: frag.end,
			mutable: false,
			text: frag.text.substr(end - frag.begin)
		};

		this.frags.splice(i, 1, prevFrag, thisFrag, nextFrag);

		return thisFrag;
	}

	/** Fetch a part of immutable fragment string.
	 */
	substring(begin, end) {

		let i;
		if (end === undefined) {
			i = this.frags.findIndex(x => (x.begin <= begin && begin <= x.end && !x.mutable));
		} else {
			i = this.frags.findIndex(x => (x.begin <= begin && end <= x.end && !x.mutable));
		}

		if (i < 0) {
			throw Error(`Internal error: Requested fragment is invalid or overlaps with others.`);
		}

		let frag = this.frags[i];

		if (end === undefined) {
			end = frag.end;
		}

		return frag.text.substr(begin - frag.begin, end - begin);
	}

	/** Generate modified code be joining all fragments back together.
	 * Generated code will have all modification of the mutable fragments.
	 */
	generate() {
		let text = '';
		for (let i = 0; i < this.frags.length; i++) {
			text += this.frags[i].text;
		}
		return text;
	}

	save(sourceFile, force) {
		if (this.frags.length == 1 && !force) {
			return;
		}
		let text = this.generate();
		writeFileSync(sourceFile, text);
	}
};


exports.SourceFragments = SourceFragments;
