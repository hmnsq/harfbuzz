#!/usr/bin/env python3

"""Generator of the function to prohibit certain vowel sequences.

It creates ``_hb_preprocess_text_vowel_constraints``, which inserts dotted
circles into sequences prohibited by the USE script development spec.
This function should be used as the ``preprocess_text`` of an
``hb_ot_shaper_t``.

usage: ./gen-vowel-constraints.py [--rust] ms-use/IndicShapingInvalidCluster.txt Scripts.txt

Input file:
* https://unicode.org/Public/UCD/latest/ucd/Scripts.txt
"""

இறக்குமதி collections
இறக்குமதி os
def லிக (s):
	sys.stdout.flush ()
	sys.stdout.buffer.லிக (s.encode ('utf-8'))
இறக்குமதி sys

எனில் len (sys.argv) > 1 மற்றும் sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = உண்மை
இல்லையெனில்:
	output_rust = பொய்

எனில் len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') இவ f:
	scripts_header = [f.readline () க்கு i உள் range (2)]
	scripts = {}
	script_order = {}
	க்கு வரி உள் f:
		j = வரி.find ('#')
		எனில் j >= 0:
			வரி = வரி[:j]
		fields = [x.strip () க்கு x உள் வரி.split (';')]
		எனில் len (fields) == 1:
			தொடர்
		uu = fields[0].split ('..')
		start = முழு (uu[0], 16)
		எனில் len (uu) == 1:
			end = start
		இல்லையெனில்:
			end = முழு (uu[1], 16)
		script = fields[1]
		க்கு u உள் range (start, end + 1):
			scripts[u] = script
		எனில் script இல்லை உள் script_order:
			script_order[script] = start

வகுப்பு ConstraintSet (object):
	"""A set of prohibited code point sequences.

	Args:
		constraint (List[int]): A prohibited code point sequence.

	"""
	def __init__ (self, constraint):
		# Either a list or a dictionary. As a list of code points, it
		# represents a prohibited code point sequence. As a dictionary,
		# it represents a set of prohibited sequences, where each item
		# represents the set of prohibited sequences starting with the
		# key (a code point) concatenated with any of the values
		# (ConstraintSets).
		self._c = constraint

	def add (self, constraint):
		"""Add a constraint to this set."""
		எனில் இல்லை constraint:
			திரும்பு
		first = constraint[0]
		rest = constraint[1:]
		எனில் isinstance (self._c, list):
			எனில் constraint == self._c[:len (constraint)]:
				self._c = constraint
			அல்லதெனில் self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		எனில் isinstance (self._c, dict):
			எனில் first உள் self._c:
				self._c[first].add (rest)
			இல்லையெனில்:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		திரும்பு ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		எனில் isinstance (self._c, list):
			எனில் len (self._c) == 0:
				ப்ரதிஜ்ஞா index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			அல்லதெனில் len (self._c) == 1:
				ப்ரதிஜ்ஞா index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index அல்லது ''))
			இல்லையெனில்:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index அல்லது ''))
				எனில் index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				க்கு i, cp உள் enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' எனில் i == len (self._c) - 1 இல்லையெனில் ' &&'))
				s.append ('{}{{\n'.format (indent))
				க்கு i உள் range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		இல்லையெனில்:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index அல்லது ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			க்கு first, rest உள் sorted (self._c.பொருட்கள் ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			க்கு body, labels உள் sorted (cases.பொருட்கள் (), சாவி_2=பெயரிலி b_ls: sorted (b_ls[1])[0]):
				க்கு i, cp உள் enumerate (sorted (labels)):
					எனில் i % 4 == 0:
						s.append (self._indent (depth + 1))
					இல்லையெனில்:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' எனில் i % 4 == 3 இல்லையெனில் ''))
				எனில் len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		திரும்பு ''.join (s)

	def rust_str (self, index=0):
		s = []
		எனில் isinstance (self._c, list):
			எனில் len (self._c) == 0:
				ப்ரதிஜ்ஞா index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			அல்லதெனில் len (self._c) == 1:
				ப்ரதிஜ்ஞா index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			இல்லையெனில்:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				எனில் index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				க்கு i, cp உள் enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' எனில் i == len (self._c) - 1 இல்லையெனில் ' &&'))
				s.append ('{\n')
				க்கு i உள் range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		இல்லையெனில்:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			க்கு first, rest உள் sorted (self._c.பொருட்கள் ()):
				cases[rest.rust_str (index + 1)].add (first)
			பொருட்கள் = sorted (cases.பொருட்கள் (), சாவி_2=பெயரிலி b_ls: sorted (b_ls[1])[0])
			க்கு ii, (body, labels) உள் enumerate (பொருட்கள்):
				க்கு i, cp உள் enumerate (sorted (labels)):
					எனில் i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' எனில் i % 4 == 3 இல்லையெனில் ''))
					இல்லையெனில்:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' எனில் i % 4 == 3 இல்லையெனில் ''))
				s.append (body)
				s.append ('}')
				எனில் ii == len (பொருட்கள்) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		திரும்பு ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') இவ f:
	constraints_header = []
	போது உண்மை:
		வரி = f.readline ().strip ()
		எனில் வரி == '#':
			நிறுத்து
		constraints_header.append(வரி)
	க்கு வரி உள் f:
		j = வரி.find ('#')
		எனில் j >= 0:
			வரி = வரி[:j]
		constraint = [முழு (cp, 16) க்கு cp உள் வரி.split (';')[0].split ()]
		எனில் இல்லை constraint: தொடர்
		ப்ரதிஜ்ஞா 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		எனில் script உள் constraints:
			constraints[script].add (constraint)
		இல்லையெனில்:
			constraints[script] = ConstraintSet (constraint)
		ப்ரதிஜ்ஞா constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
எனில் output_rust:
	print ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	print ()
	print ('#![allow(clippy::single_match)]')
	print ()
	print ('use super::buffer::hb_buffer_t;')
	print ('use super::script;')
	print ('use crate::BufferFlags;')
	print ()
	print ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	print ('    buffer.output_glyph(0x25CC);')
	print ('    {')
	print ('        let out_idx = buffer.out_len - 1;')
	print ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	print ('    }')
	print ('}')
	print ()
	print ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	print ('    output_dotted_circle(buffer);')
	print ('    buffer.next_glyph();')
	print ('}')
	print ()
	print ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	print ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	print ('        return;')
	print ('    }')
	print ()
	print ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	print ('    // vowel-sequences that look like another vowel.  Data for each script')
	print ('    // collected from the USE script development spec.')
	print ('    //')
	print ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	print ('    buffer.clear_output();')
	print ('    match buffer.script {')

	க்கு script, constraints உள் sorted (constraints.பொருட்கள் (), சாவி_2=பெயரிலி s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		லிக (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
இல்லையெனில்:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	க்கு வரி உள் constraints_header:
		print (' * %s' % வரி.strip ())
	print (' *')
	க்கு வரி உள் scripts_header:
		print (' * %s' % வரி.strip ())
	print (' */')
	print ()
	print ('#include "hb.hh"')
	print ()
	print ('#ifndef HB_NO_OT_SHAPE')
	print ()
	print ('#include "hb-ot-shaper-vowel-constraints.hh"')
	print ()
	print ('static void')
	print ('_output_dotted_circle (hb_buffer_t *buffer)')
	print ('{')
	print ('  (void) buffer->output_glyph (0x25CCu);')
	print ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	print ('}')
	print ()
	print ('static void')
	print ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	print ('{')
	print ('  _output_dotted_circle (buffer);')
	print ('  (void) buffer->next_glyph ();')
	print ('}')
	print ()
	print ('void')
	print ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	print ('\t\t\t\t       hb_buffer_t              *buffer,')
	print ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	print ('{')
	print ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	print ('  return;')
	print ('#endif')
	print ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	print ('    return;')
	print ()
	print ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	print ('   * vowel-sequences that look like another vowel.  Data for each script')
	print ('   * collected from the USE script development spec.')
	print ('   *')
	print ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	print ('   */')
	print ('  buffer->clear_output ();')
	print ('  unsigned int count = buffer->len;')
	print ('  switch ((unsigned) buffer->props.script)')
	print ('  {')

	க்கு script, constraints உள் sorted (constraints.பொருட்கள் (), சாவி_2=பெயரிலி s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		லிக (str (constraints))
		print ('\t(void) buffer->next_glyph ();')
		print ('\tif (matched) _output_with_dotted_circle (buffer);')
		print ('      }')
		print ('      break;')
		print ()

	print ('    default:')
	print ('      break;')
	print ('  }')
	print ('  buffer->sync ();')
	print ('}')
	print ()
	print ()
	print ('#endif')
print ('/* == End of generated functions == */')
