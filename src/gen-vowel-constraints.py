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

استورد collections
استورد os
def اكتب (s):
	sys.stdout.flush ()
	sys.stdout.buffer.اكتب (s.encode ('utf-8'))
استورد sys

إذا len (sys.argv) > 1 و sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = صحيح
وإلا:
	output_rust = خطأ

إذا len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') كـ f:
	scripts_header = [f.readline () لكل i في range (2)]
	scripts = {}
	script_order = {}
	لكل سطر في f:
		j = سطر.find ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		fields = [x.strip () لكل x في سطر.split (';')]
		إذا len (fields) == 1:
			استمر
		uu = fields[0].split ('..')
		start = int (uu[0], 16)
		إذا len (uu) == 1:
			end = start
		وإلا:
			end = int (uu[1], 16)
		script = fields[1]
		لكل u في range (start, end + 1):
			scripts[u] = script
		إذا script ليس في script_order:
			script_order[script] = start

صنف ConstraintSet (object):
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
		إذا ليس constraint:
			إرجاع
		first = constraint[0]
		rest = constraint[1:]
		إذا isinstance (self._c, list):
			إذا constraint == self._c[:len (constraint)]:
				self._c = constraint
			وإلاإذا self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		إذا isinstance (self._c, dict):
			إذا first في self._c:
				self._c[first].add (rest)
			وإلا:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		إرجاع ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		إذا isinstance (self._c, list):
			إذا len (self._c) == 0:
				أكد index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وإلاإذا len (self._c) == 1:
				أكد index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index أو ''))
			وإلا:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index أو ''))
				إذا index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				لكل i, cp في enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' إذا i == len (self._c) - 1 وإلا ' &&'))
				s.append ('{}{{\n'.format (indent))
				لكل i في range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		وإلا:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index أو ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			لكل first, rest في sorted (self._c.عناصر ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			لكل body, labels في sorted (cases.عناصر (), مفتاح_2=مجهول b_ls: sorted (b_ls[1])[0]):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i % 4 == 0:
						s.append (self._indent (depth + 1))
					وإلا:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				إذا len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		إرجاع ''.join (s)

	def rust_str (self, index=0):
		s = []
		إذا isinstance (self._c, list):
			إذا len (self._c) == 0:
				أكد index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وإلاإذا len (self._c) == 1:
				أكد index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			وإلا:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				إذا index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				لكل i, cp في enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' إذا i == len (self._c) - 1 وإلا ' &&'))
				s.append ('{\n')
				لكل i في range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		وإلا:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			لكل first, rest في sorted (self._c.عناصر ()):
				cases[rest.rust_str (index + 1)].add (first)
			عناصر = sorted (cases.عناصر (), مفتاح_2=مجهول b_ls: sorted (b_ls[1])[0])
			لكل ii, (body, labels) في enumerate (عناصر):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
					وإلا:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				s.append (body)
				s.append ('}')
				إذا ii == len (عناصر) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		إرجاع ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') كـ f:
	constraints_header = []
	بينما صحيح:
		سطر = f.readline ().strip ()
		إذا سطر == '#':
			اكسر
		constraints_header.append(سطر)
	لكل سطر في f:
		j = سطر.find ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		constraint = [int (cp, 16) لكل cp في سطر.split (';')[0].split ()]
		إذا ليس constraint: استمر
		أكد 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		إذا script في constraints:
			constraints[script].add (constraint)
		وإلا:
			constraints[script] = ConstraintSet (constraint)
		أكد constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
إذا output_rust:
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

	لكل script, constraints في sorted (constraints.عناصر (), مفتاح_2=مجهول s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		اكتب (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
وإلا:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	لكل سطر في constraints_header:
		print (' * %s' % سطر.strip ())
	print (' *')
	لكل سطر في scripts_header:
		print (' * %s' % سطر.strip ())
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

	لكل script, constraints في sorted (constraints.عناصر (), مفتاح_2=مجهول s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		اكتب (str (constraints))
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
