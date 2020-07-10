
const { readFileSync } = require('fs');

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
	}

	/** Create a mutable fragment by dividing a immutable fragment into
	 * three fragments: immutable before requested one, requested one and
	 * immutable after requested one.
	 * 
	 * `text` field of returned object is a fragment string that can be
	 * modified.
	 */
	create(begin, end) {

		let i = this.frags.findIndex(x => (begin >= x.begin && end <= x.end && !x.mutable));
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
		let i = this.frags.findIndex(x => (begin >= x.begin && end <= x.end && !x.mutable));
		if (i < 0) {
			throw Error(`Internal error: Requested fragment is invalid or overlaps with others.`);
		}

		let frag = this.frags[i];

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
};


exports.SourceFragments = SourceFragments;
