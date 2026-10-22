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

आयात collections
आयात os
def म_लिखो (s):
	sys.stdout.flush ()
	sys.stdout.बफर.म_लिखो (s.encode ('utf-8'))
आयात sys

अगर len (sys.argv) > 1 and sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = True
अन्यथा:
	output_rust = False

अगर len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') as f:
	scripts_header = [f.readline () क्रम i in range (2)]
	scripts = {}
	script_order = {}
	क्रम पंक्ति in f:
		j = पंक्ति.find ('#')
		अगर j >= 0:
			पंक्ति = पंक्ति[:j]
		fields = [x.strip () क्रम x in पंक्ति.split (';')]
		अगर len (fields) == 1:
			जारी
		uu = fields[0].split ('..')
		शुरू = पूर्णांक (uu[0], 16)
		अगर len (uu) == 1:
			end = शुरू
		अन्यथा:
			end = पूर्णांक (uu[1], 16)
		script = fields[1]
		क्रम u in range (शुरू, end + 1):
			scripts[u] = script
		अगर script not in script_order:
			script_order[script] = शुरू

श्रेणी ConstraintSet (object):
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
		अगर not constraint:
			वापस
		first = constraint[0]
		rest = constraint[1:]
		अगर isinstance (self._c, list):
			अगर constraint == self._c[:len (constraint)]:
				self._c = constraint
			elif self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		अगर isinstance (self._c, dict):
			अगर first in self._c:
				self._c[first].add (rest)
			अन्यथा:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		वापस ('  ' * depth).प्रतिस्थापन ('        ', '\t')

	def __str__ (self, सूचकांक_2=0, depth=4):
		s = []
		indent = self._indent (depth)
		अगर isinstance (self._c, list):
			अगर len (self._c) == 0:
				assert सूचकांक_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			elif len (self._c) == 1:
				assert सूचकांक_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, अग्र (iter (self._c)), सूचकांक_2 or ''))
			अन्यथा:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], सूचकांक_2 or ''))
				अगर सूचकांक_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), सूचकांक_2 + 1))
				क्रम i, cp in enumerate (self._c[1:], शुरू=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, सूचकांक_2 + i, ')' अगर i == len (self._c) - 1 अन्यथा ' &&'))
				s.append ('{}{{\n'.format (indent))
				क्रम i in range (सूचकांक_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		अन्यथा:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, सूचकांक_2 or ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			क्रम first, rest in sorted (self._c.वस्तुएँ ()):
				cases[rest.__str__ (सूचकांक_2 + 1, depth + 2)].add (first)
			क्रम body, labels in sorted (cases.वस्तुएँ (), कुंजी_2=lambda b_ls: sorted (b_ls[1])[0]):
				क्रम i, cp in enumerate (sorted (labels)):
					अगर i % 4 == 0:
						s.append (self._indent (depth + 1))
					अन्यथा:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' अगर i % 4 == 3 अन्यथा ''))
				अगर len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		वापस ''.मेल (s)

	def rust_str (self, सूचकांक_2=0):
		s = []
		अगर isinstance (self._c, list):
			अगर len (self._c) == 0:
				assert सूचकांक_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			elif len (self._c) == 1:
				assert सूचकांक_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (अग्र (iter (self._c)), सूचकांक_2))
			अन्यथा:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], सूचकांक_2))
				अगर सूचकांक_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (सूचकांक_2 + 1))
				क्रम i, cp in enumerate (self._c[1:], शुरू=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, सूचकांक_2 + i, '' अगर i == len (self._c) - 1 अन्यथा ' &&'))
				s.append ('{\n')
				क्रम i in range (सूचकांक_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		अन्यथा:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (सूचकांक_2))
			cases = collections.defaultdict (set)
			क्रम first, rest in sorted (self._c.वस्तुएँ ()):
				cases[rest.rust_str (सूचकांक_2 + 1)].add (first)
			वस्तुएँ = sorted (cases.वस्तुएँ (), कुंजी_2=lambda b_ls: sorted (b_ls[1])[0])
			क्रम ii, (body, labels) in enumerate (वस्तुएँ):
				क्रम i, cp in enumerate (sorted (labels)):
					अगर i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' अगर i % 4 == 3 अन्यथा ''))
					अन्यथा:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' अगर i % 4 == 3 अन्यथा ''))
				s.append (body)
				s.append ('}')
				अगर ii == len (वस्तुएँ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		वापस ''.मेल (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') as f:
	constraints_header = []
	जबतक True:
		पंक्ति = f.readline ().strip ()
		अगर पंक्ति == '#':
			अवरोध
		constraints_header.append(पंक्ति)
	क्रम पंक्ति in f:
		j = पंक्ति.find ('#')
		अगर j >= 0:
			पंक्ति = पंक्ति[:j]
		constraint = [पूर्णांक (cp, 16) क्रम cp in पंक्ति.split (';')[0].split ()]
		अगर not constraint: जारी
		assert 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		अगर script in constraints:
			constraints[script].add (constraint)
		अन्यथा:
			constraints[script] = ConstraintSet (constraint)
		assert constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
अगर output_rust:
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

	क्रम script, constraints in sorted (constraints.वस्तुएँ (), कुंजी_2=lambda s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		म_लिखो (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
अन्यथा:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	क्रम पंक्ति in constraints_header:
		print (' * %s' % पंक्ति.strip ())
	print (' *')
	क्रम पंक्ति in scripts_header:
		print (' * %s' % पंक्ति.strip ())
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

	क्रम script, constraints in sorted (constraints.वस्तुएँ (), कुंजी_2=lambda s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		म_लिखो (str (constraints))
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
