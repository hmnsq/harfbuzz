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
تعریف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))
استورد sys

إذا لمبائی (sys.argv) > 1 و sys.argv[1] == '--rust':
	ہٹاؤ sys.argv[1]
	output_rust = سچ
وإلا:
	output_rust = خطأ

إذا لمبائی (sys.argv) != 3:
	sys.exit (__doc__)

نال open (sys.argv[2], encoding='utf-8') كـ f:
	scripts_سرخی = [f.readline () لكل i في سلسلہ (2)]
	scripts = {}
	script_ترتیب = {}
	لكل سطر في f:
		j = سطر.لبھو ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		fields = [x.strip () لكل x في سطر.ونڈو (';')]
		إذا لمبائی (fields) == 1:
			استمر
		uu = fields[0].ونڈو ('..')
		شروع = صحيح (uu[0], 16)
		إذا لمبائی (uu) == 1:
			سرا = شروع
		وإلا:
			سرا = صحيح (uu[1], 16)
		script = fields[1]
		لكل u في سلسلہ (شروع, سرا + 1):
			scripts[u] = script
		إذا script ليس في script_ترتیب:
			script_ترتیب[script] = شروع

صنف Constraint_سیٹ (object):
	"""A set of prohibited code point sequences.

	Args:
		constraint (List[int]): A prohibited code point sequence.

	"""
	تعریف __init__ (self, constraint):
		# Either a list or a dictionary. As a list of code points, it
		# represents a prohibited code point sequence. As a dictionary,
		# it represents a set of prohibited sequences, where each item
		# represents the set of prohibited sequences starting with the
		# key (a code point) concatenated with any of the values
		# (ConstraintSets).
		self._c = constraint

	تعریف جوڑو (self, constraint):
		"""Add a constraint to this set."""
		إذا ليس constraint:
			أرجع
		پہلا = constraint[0]
		rest = constraint[1:]
		إذا isinstance (self._c, list):
			إذا constraint == self._c[:لمبائی (constraint)]:
				self._c = constraint
			وإلاإذا self._c != constraint[:لمبائی (self._c)]:
				self._c = {self._c[0]: Constraint_سیٹ (self._c[1:])}
		إذا isinstance (self._c, dict):
			إذا پہلا في self._c:
				self._c[پہلا].جوڑو (rest)
			وإلا:
				self._c[پہلا] = Constraint_سیٹ (rest)

	@staticmethod
	تعریف _indent (ڈونگھائی):
		أرجع ('  ' * ڈونگھائی).replace ('        ', '\t')

	تعریف __str__ (self, اشاریہ_2=0, ڈونگھائی=4):
		s = []
		indent = self._indent (ڈونگھائی)
		إذا isinstance (self._c, list):
			إذا لمبائی (self._c) == 0:
				أكد اشاریہ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وإلاإذا لمبائی (self._c) == 1:
				أكد اشاریہ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, اگلا (iter (self._c)), اشاریہ_2 أو ''))
			وإلا:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], اشاریہ_2 أو ''))
				إذا اشاریہ_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (ڈونگھائی + 2), اشاریہ_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروع=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (ڈونگھائی + 2), cp, اشاریہ_2 + i, ')' إذا i == لمبائی (self._c) - 1 وإلا ' &&'))
				s.append ('{}{{\n'.format (indent))
				لكل i في سلسلہ (اشاریہ_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (ڈونگھائی + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (ڈونگھائی + 1)))
				s.append ('{}}}\n'.format (indent))
		وإلا:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, اشاریہ_2 أو ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (سیٹ)
			لكل پہلا, rest في sorted (self._c.اشیاء ()):
				cases[rest.__str__ (اشاریہ_2 + 1, ڈونگھائی + 2)].جوڑو (پہلا)
			لكل body, labels في sorted (cases.اشیاء (), کلید_2=مجهول b_ls: sorted (b_ls[1])[0]):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i % 4 == 0:
						s.append (self._indent (ڈونگھائی + 1))
					وإلا:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				إذا لمبائی (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (ڈونگھائی + 2)))
			s.append ('{}}}\n'.format (indent))
		أرجع ''.join (s)

	تعریف rust_str (self, اشاریہ_2=0):
		s = []
		إذا isinstance (self._c, list):
			إذا لمبائی (self._c) == 0:
				أكد اشاریہ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وإلاإذا لمبائی (self._c) == 1:
				أكد اشاریہ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (اگلا (iter (self._c)), اشاریہ_2))
			وإلا:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], اشاریہ_2))
				إذا اشاریہ_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (اشاریہ_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروع=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, اشاریہ_2 + i, '' إذا i == لمبائی (self._c) - 1 وإلا ' &&'))
				s.append ('{\n')
				لكل i في سلسلہ (اشاریہ_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		وإلا:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (اشاریہ_2))
			cases = collections.defaultdict (سیٹ)
			لكل پہلا, rest في sorted (self._c.اشیاء ()):
				cases[rest.rust_str (اشاریہ_2 + 1)].جوڑو (پہلا)
			اشیاء = sorted (cases.اشیاء (), کلید_2=مجهول b_ls: sorted (b_ls[1])[0])
			لكل ii, (body, labels) في enumerate (اشیاء):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i == لمبائی (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
					وإلا:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				s.append (body)
				s.append ('}')
				إذا ii == لمبائی (اشیاء) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		أرجع ''.join (s)

constraints = {}
نال open (sys.argv[1], encoding='utf-8') كـ f:
	constraints_سرخی = []
	طالما سچ:
		سطر = f.readline ().strip ()
		إذا سطر == '#':
			اكسر
		constraints_سرخی.append(سطر)
	لكل سطر في f:
		j = سطر.لبھو ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		constraint = [صحيح (cp, 16) لكل cp في سطر.ونڈو (';')[0].ونڈو ()]
		إذا ليس constraint: استمر
		أكد 2 <= لمبائی (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		إذا script في constraints:
			constraints[script].جوڑو (constraint)
		وإلا:
			constraints[script] = Constraint_سیٹ (constraint)
		أكد constraints, 'No constraints found'

چھاپو ('/* == Start of generated functions == */')
إذا output_rust:
	چھاپو ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	چھاپو ()
	چھاپو ('#![allow(clippy::single_match)]')
	چھاپو ()
	چھاپو ('use super::buffer::hb_buffer_t;')
	چھاپو ('use super::script;')
	چھاپو ('use crate::BufferFlags;')
	چھاپو ()
	چھاپو ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	چھاپو ('    buffer.output_glyph(0x25CC);')
	چھاپو ('    {')
	چھاپو ('        let out_idx = buffer.out_len - 1;')
	چھاپو ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	چھاپو ('    }')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	چھاپو ('    output_dotted_circle(buffer);')
	چھاپو ('    buffer.next_glyph();')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	چھاپو ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	چھاپو ('        return;')
	چھاپو ('    }')
	چھاپو ()
	چھاپو ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	چھاپو ('    // vowel-sequences that look like another vowel.  Data for each script')
	چھاپو ('    // collected from the USE script development spec.')
	چھاپو ('    //')
	چھاپو ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	چھاپو ('    buffer.clear_output();')
	چھاپو ('    match buffer.script {')

	لكل script, constraints في sorted (constraints.اشیاء (), کلید_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چھاپو ('        Some(script::{}) => {{'.format (script.اتلا ()))
		چھاپو ('            buffer.idx = 0;')
		چھاپو ('            while buffer.idx + 1 < buffer.len {')
		چھاپو ('                #[allow(unused_mut)]')
		چھاپو ('                let mut matched = false;')
		اكتب (constraints.rust_str ())
		چھاپو ('                buffer.next_glyph();')
		چھاپو ('                if matched { output_with_dotted_circle(buffer); }')
		چھاپو ('            }')
		چھاپو ('        }')
		چھاپو ()

	چھاپو ('        _ => {}')
	چھاپو ('    }')
	چھاپو ('    buffer.sync();')
	چھاپو ('}')
وإلا:
	چھاپو ('/*')
	چھاپو (' * The following functions are generated by running:')
	چھاپو (' *')
	چھاپو (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.رستہ.basename (sys.argv[0]))
	چھاپو (' *')
	چھاپو (' * on files with these headers:')
	چھاپو (' *')
	لكل سطر في constraints_سرخی:
		چھاپو (' * %s' % سطر.strip ())
	چھاپو (' *')
	لكل سطر في scripts_سرخی:
		چھاپو (' * %s' % سطر.strip ())
	چھاپو (' */')
	چھاپو ()
	چھاپو ('#include "hb.hh"')
	چھاپو ()
	چھاپو ('#ifndef HB_NO_OT_SHAPE')
	چھاپو ()
	چھاپو ('#include "hb-ot-shaper-vowel-constraints.hh"')
	چھاپو ()
	چھاپو ('static void')
	چھاپو ('_output_dotted_circle (hb_buffer_t *buffer)')
	چھاپو ('{')
	چھاپو ('  (void) buffer->output_glyph (0x25CCu);')
	چھاپو ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('static void')
	چھاپو ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	چھاپو ('{')
	چھاپو ('  _output_dotted_circle (buffer);')
	چھاپو ('  (void) buffer->next_glyph ();')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('void')
	چھاپو ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	چھاپو ('\t\t\t\t       hb_buffer_t              *buffer,')
	چھاپو ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	چھاپو ('{')
	چھاپو ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	چھاپو ('  return;')
	چھاپو ('#endif')
	چھاپو ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	چھاپو ('    return;')
	چھاپو ()
	چھاپو ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	چھاپو ('   * vowel-sequences that look like another vowel.  Data for each script')
	چھاپو ('   * collected from the USE script development spec.')
	چھاپو ('   *')
	چھاپو ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	چھاپو ('   */')
	چھاپو ('  buffer->clear_output ();')
	چھاپو ('  unsigned int count = buffer->len;')
	چھاپو ('  switch ((unsigned) buffer->props.script)')
	چھاپو ('  {')

	لكل script, constraints في sorted (constraints.اشیاء (), کلید_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چھاپو ('    case HB_SCRIPT_{}:'.format (script.اتلا ()))
		چھاپو ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		چھاپو ('      {')
		چھاپو ('\tbool matched = false;')
		اكتب (str (constraints))
		چھاپو ('\t(void) buffer->next_glyph ();')
		چھاپو ('\tif (matched) _output_with_dotted_circle (buffer);')
		چھاپو ('      }')
		چھاپو ('      break;')
		چھاپو ()

	چھاپو ('    default:')
	چھاپو ('      break;')
	چھاپو ('  }')
	چھاپو ('  buffer->sync ();')
	چھاپو ('}')
	چھاپو ()
	چھاپو ()
	چھاپو ('#endif')
چھاپو ('/* == End of generated functions == */')
