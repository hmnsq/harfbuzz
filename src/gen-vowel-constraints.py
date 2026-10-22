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

ಆನಯನ collections
ಆನಯನ os
def ಲಿಖ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.ಲಿಖ (s.encode ('utf-8'))
ಆನಯನ sys

ಯದಿ len (sys.argv) > 1 ಚ sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = ಸತ್ಯ
ಅನ್ಯಥಾ:
	output_rust = ಅಸತ್ಯ

ಯದಿ len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') ಇವ f:
	scripts_header = [f.readline () ಪ್ರತಿ i ಅನ್ತಃ range (2)]
	scripts = {}
	script_order = {}
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ f:
		j = ಸಾಲು.find ('#')
		ಯದಿ j >= 0:
			ಸಾಲು = ಸಾಲು[:j]
		fields = [x.strip () ಪ್ರತಿ x ಅನ್ತಃ ಸಾಲು.split (';')]
		ಯದಿ len (fields) == 1:
			ಅನುವರ್ತ
		uu = fields[0].split ('..')
		start = ಪೂರ್ಣಾಙ್ಕ (uu[0], 16)
		ಯದಿ len (uu) == 1:
			end = start
		ಅನ್ಯಥಾ:
			end = ಪೂರ್ಣಾಙ್ಕ (uu[1], 16)
		script = fields[1]
		ಪ್ರತಿ u ಅನ್ತಃ range (start, end + 1):
			scripts[u] = script
		ಯದಿ script ನ ಅನ್ತಃ script_order:
			script_order[script] = start

ವರ್ಗ ConstraintSet (object):
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
		ಯದಿ ನ constraint:
			ಪ್ರತ್ಯಾಗಮ
		first = constraint[0]
		rest = constraint[1:]
		ಯದಿ isinstance (self._c, list):
			ಯದಿ constraint == self._c[:len (constraint)]:
				self._c = constraint
			ನೋಚೇದ್ಯದಿ self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		ಯದಿ isinstance (self._c, dict):
			ಯದಿ first ಅನ್ತಃ self._c:
				self._c[first].add (rest)
			ಅನ್ಯಥಾ:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		ಪ್ರತ್ಯಾಗಮ ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		ಯದಿ isinstance (self._c, list):
			ಯದಿ len (self._c) == 0:
				ಪ್ರತಿಜ್ಞಾ index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			ನೋಚೇದ್ಯದಿ len (self._c) == 1:
				ಪ್ರತಿಜ್ಞಾ index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index ವಾ ''))
			ಅನ್ಯಥಾ:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index ವಾ ''))
				ಯದಿ index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				ಪ್ರತಿ i, cp ಅನ್ತಃ enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' ಯದಿ i == len (self._c) - 1 ಅನ್ಯಥಾ ' &&'))
				s.append ('{}{{\n'.format (indent))
				ಪ್ರತಿ i ಅನ್ತಃ range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		ಅನ್ಯಥಾ:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index ವಾ ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			ಪ್ರತಿ first, rest ಅನ್ತಃ sorted (self._c.ವಸ್ತುಗಳು ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			ಪ್ರತಿ body, labels ಅನ್ತಃ sorted (cases.ವಸ್ತುಗಳು (), ಕೀಲಿ_2=ಅನಾಮನ್ b_ls: sorted (b_ls[1])[0]):
				ಪ್ರತಿ i, cp ಅನ್ತಃ enumerate (sorted (labels)):
					ಯದಿ i % 4 == 0:
						s.append (self._indent (depth + 1))
					ಅನ್ಯಥಾ:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' ಯದಿ i % 4 == 3 ಅನ್ಯಥಾ ''))
				ಯದಿ len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		ಪ್ರತ್ಯಾಗಮ ''.join (s)

	def rust_str (self, index=0):
		s = []
		ಯದಿ isinstance (self._c, list):
			ಯದಿ len (self._c) == 0:
				ಪ್ರತಿಜ್ಞಾ index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			ನೋಚೇದ್ಯದಿ len (self._c) == 1:
				ಪ್ರತಿಜ್ಞಾ index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			ಅನ್ಯಥಾ:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				ಯದಿ index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				ಪ್ರತಿ i, cp ಅನ್ತಃ enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' ಯದಿ i == len (self._c) - 1 ಅನ್ಯಥಾ ' &&'))
				s.append ('{\n')
				ಪ್ರತಿ i ಅನ್ತಃ range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		ಅನ್ಯಥಾ:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			ಪ್ರತಿ first, rest ಅನ್ತಃ sorted (self._c.ವಸ್ತುಗಳು ()):
				cases[rest.rust_str (index + 1)].add (first)
			ವಸ್ತುಗಳು = sorted (cases.ವಸ್ತುಗಳು (), ಕೀಲಿ_2=ಅನಾಮನ್ b_ls: sorted (b_ls[1])[0])
			ಪ್ರತಿ ii, (body, labels) ಅನ್ತಃ enumerate (ವಸ್ತುಗಳು):
				ಪ್ರತಿ i, cp ಅನ್ತಃ enumerate (sorted (labels)):
					ಯದಿ i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' ಯದಿ i % 4 == 3 ಅನ್ಯಥಾ ''))
					ಅನ್ಯಥಾ:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' ಯದಿ i % 4 == 3 ಅನ್ಯಥಾ ''))
				s.append (body)
				s.append ('}')
				ಯದಿ ii == len (ವಸ್ತುಗಳು) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		ಪ್ರತ್ಯಾಗಮ ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') ಇವ f:
	constraints_header = []
	ಯಾವತ್ ಸತ್ಯ:
		ಸಾಲು = f.readline ().strip ()
		ಯದಿ ಸಾಲು == '#':
			ಭಙ್ಗ
		constraints_header.append(ಸಾಲು)
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ f:
		j = ಸಾಲು.find ('#')
		ಯದಿ j >= 0:
			ಸಾಲು = ಸಾಲು[:j]
		constraint = [ಪೂರ್ಣಾಙ್ಕ (cp, 16) ಪ್ರತಿ cp ಅನ್ತಃ ಸಾಲು.split (';')[0].split ()]
		ಯದಿ ನ constraint: ಅನುವರ್ತ
		ಪ್ರತಿಜ್ಞಾ 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		ಯದಿ script ಅನ್ತಃ constraints:
			constraints[script].add (constraint)
		ಅನ್ಯಥಾ:
			constraints[script] = ConstraintSet (constraint)
		ಪ್ರತಿಜ್ಞಾ constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
ಯದಿ output_rust:
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

	ಪ್ರತಿ script, constraints ಅನ್ತಃ sorted (constraints.ವಸ್ತುಗಳು (), ಕೀಲಿ_2=ಅನಾಮನ್ s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		ಲಿಖ (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
ಅನ್ಯಥಾ:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ constraints_header:
		print (' * %s' % ಸಾಲು.strip ())
	print (' *')
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ scripts_header:
		print (' * %s' % ಸಾಲು.strip ())
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

	ಪ್ರತಿ script, constraints ಅನ್ತಃ sorted (constraints.ವಸ್ತುಗಳು (), ಕೀಲಿ_2=ಅನಾಮನ್ s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		ಲಿಖ (str (constraints))
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
