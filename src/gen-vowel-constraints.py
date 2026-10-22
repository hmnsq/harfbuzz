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

ഇറക്കുമതി collections
ഇറക്കുമതി os
def ലിഖ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.ലിഖ (s.encode ('utf-8'))
ഇറക്കുമതി sys

എങ്കിൽ len (sys.argv) > 1 ഒപ്പം sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = സത്യം
അല്ലെങ്കിൽ:
	output_rust = അസത്യം

എങ്കിൽ len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') ഇവ f:
	scripts_header = [f.readline () വേണ്ടി i ഇൽ range (2)]
	scripts = {}
	script_order = {}
	വേണ്ടി വരി ഇൽ f:
		j = വരി.find ('#')
		എങ്കിൽ j >= 0:
			വരി = വരി[:j]
		fields = [x.strip () വേണ്ടി x ഇൽ വരി.split (';')]
		എങ്കിൽ len (fields) == 1:
			തുടരുക
		uu = fields[0].split ('..')
		start = പൂർണ്ണസംഖ്യ (uu[0], 16)
		എങ്കിൽ len (uu) == 1:
			end = start
		അല്ലെങ്കിൽ:
			end = പൂർണ്ണസംഖ്യ (uu[1], 16)
		script = fields[1]
		വേണ്ടി u ഇൽ range (start, end + 1):
			scripts[u] = script
		എങ്കിൽ script അല്ല ഇൽ script_order:
			script_order[script] = start

വർഗ്ഗം ConstraintSet (object):
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
		എങ്കിൽ അല്ല constraint:
			തിരികെ
		first = constraint[0]
		rest = constraint[1:]
		എങ്കിൽ isinstance (self._c, list):
			എങ്കിൽ constraint == self._c[:len (constraint)]:
				self._c = constraint
			അല്ലെങ്കിലെങ്കിൽ self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		എങ്കിൽ isinstance (self._c, dict):
			എങ്കിൽ first ഇൽ self._c:
				self._c[first].add (rest)
			അല്ലെങ്കിൽ:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		തിരികെ ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		എങ്കിൽ isinstance (self._c, list):
			എങ്കിൽ len (self._c) == 0:
				പ്രതിജ്ഞാ index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			അല്ലെങ്കിലെങ്കിൽ len (self._c) == 1:
				പ്രതിജ്ഞാ index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index അഥവാ ''))
			അല്ലെങ്കിൽ:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index അഥവാ ''))
				എങ്കിൽ index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				വേണ്ടി i, cp ഇൽ enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' എങ്കിൽ i == len (self._c) - 1 അല്ലെങ്കിൽ ' &&'))
				s.append ('{}{{\n'.format (indent))
				വേണ്ടി i ഇൽ range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		അല്ലെങ്കിൽ:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index അഥവാ ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			വേണ്ടി first, rest ഇൽ sorted (self._c.ഇനങ്ങൾ ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			വേണ്ടി body, labels ഇൽ sorted (cases.ഇനങ്ങൾ (), താക്കോൽ_2=അജ്ഞാത b_ls: sorted (b_ls[1])[0]):
				വേണ്ടി i, cp ഇൽ enumerate (sorted (labels)):
					എങ്കിൽ i % 4 == 0:
						s.append (self._indent (depth + 1))
					അല്ലെങ്കിൽ:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' എങ്കിൽ i % 4 == 3 അല്ലെങ്കിൽ ''))
				എങ്കിൽ len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		തിരികെ ''.join (s)

	def rust_str (self, index=0):
		s = []
		എങ്കിൽ isinstance (self._c, list):
			എങ്കിൽ len (self._c) == 0:
				പ്രതിജ്ഞാ index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			അല്ലെങ്കിലെങ്കിൽ len (self._c) == 1:
				പ്രതിജ്ഞാ index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			അല്ലെങ്കിൽ:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				എങ്കിൽ index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				വേണ്ടി i, cp ഇൽ enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' എങ്കിൽ i == len (self._c) - 1 അല്ലെങ്കിൽ ' &&'))
				s.append ('{\n')
				വേണ്ടി i ഇൽ range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		അല്ലെങ്കിൽ:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			വേണ്ടി first, rest ഇൽ sorted (self._c.ഇനങ്ങൾ ()):
				cases[rest.rust_str (index + 1)].add (first)
			ഇനങ്ങൾ = sorted (cases.ഇനങ്ങൾ (), താക്കോൽ_2=അജ്ഞാത b_ls: sorted (b_ls[1])[0])
			വേണ്ടി ii, (body, labels) ഇൽ enumerate (ഇനങ്ങൾ):
				വേണ്ടി i, cp ഇൽ enumerate (sorted (labels)):
					എങ്കിൽ i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' എങ്കിൽ i % 4 == 3 അല്ലെങ്കിൽ ''))
					അല്ലെങ്കിൽ:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' എങ്കിൽ i % 4 == 3 അല്ലെങ്കിൽ ''))
				s.append (body)
				s.append ('}')
				എങ്കിൽ ii == len (ഇനങ്ങൾ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		തിരികെ ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') ഇവ f:
	constraints_header = []
	ആകുമ്പോൾ സത്യം:
		വരി = f.readline ().strip ()
		എങ്കിൽ വരി == '#':
			നിർത്തുക
		constraints_header.append(വരി)
	വേണ്ടി വരി ഇൽ f:
		j = വരി.find ('#')
		എങ്കിൽ j >= 0:
			വരി = വരി[:j]
		constraint = [പൂർണ്ണസംഖ്യ (cp, 16) വേണ്ടി cp ഇൽ വരി.split (';')[0].split ()]
		എങ്കിൽ അല്ല constraint: തുടരുക
		പ്രതിജ്ഞാ 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		എങ്കിൽ script ഇൽ constraints:
			constraints[script].add (constraint)
		അല്ലെങ്കിൽ:
			constraints[script] = ConstraintSet (constraint)
		പ്രതിജ്ഞാ constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
എങ്കിൽ output_rust:
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

	വേണ്ടി script, constraints ഇൽ sorted (constraints.ഇനങ്ങൾ (), താക്കോൽ_2=അജ്ഞാത s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		ലിഖ (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
അല്ലെങ്കിൽ:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	വേണ്ടി വരി ഇൽ constraints_header:
		print (' * %s' % വരി.strip ())
	print (' *')
	വേണ്ടി വരി ഇൽ scripts_header:
		print (' * %s' % വരി.strip ())
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

	വേണ്ടി script, constraints ഇൽ sorted (constraints.ഇനങ്ങൾ (), താക്കോൽ_2=അജ്ഞാത s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		ലിഖ (str (constraints))
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
