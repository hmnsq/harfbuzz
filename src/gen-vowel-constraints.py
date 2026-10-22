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
	sys.stdout.میانگیر.اكتب (s.encode ('utf-8'))
استورد sys

إذا طول (sys.argv) > 1 و sys.argv[1] == '--rust':
	پاک sys.argv[1]
	output_rust = درست
وإلا:
	output_rust = خطأ

إذا طول (sys.argv) != 3:
	sys.exit (__doc__)

با open (sys.argv[2], encoding='utf-8') كـ f:
	scripts_سرآیند = [f.readline () لكل i في دامنه (2)]
	scripts = {}
	script_ترتیب = {}
	لكل سطر في f:
		j = سطر.پیدا_کن ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		fields = [x.strip () لكل x في سطر.تقسیم (';')]
		إذا طول (fields) == 1:
			استمر
		uu = fields[0].تقسیم ('..')
		شروع = صحيح (uu[0], 16)
		إذا طول (uu) == 1:
			انتها = شروع
		وإلا:
			انتها = صحيح (uu[1], 16)
		script = fields[1]
		لكل u في دامنه (شروع, انتها + 1):
			scripts[u] = script
		إذا script ليس في script_ترتیب:
			script_ترتیب[script] = شروع

صنف Constraint_مجموعه (object):
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

	تعریف جمع_کن (self, constraint):
		"""Add a constraint to this set."""
		إذا ليس constraint:
			أرجع
		اولی = constraint[0]
		rest = constraint[1:]
		إذا isinstance (self._c, list):
			إذا constraint == self._c[:طول (constraint)]:
				self._c = constraint
			وإلاإذا self._c != constraint[:طول (self._c)]:
				self._c = {self._c[0]: Constraint_مجموعه (self._c[1:])}
		إذا isinstance (self._c, dict):
			إذا اولی في self._c:
				self._c[اولی].جمع_کن (rest)
			وإلا:
				self._c[اولی] = Constraint_مجموعه (rest)

	@staticmethod
	تعریف _indent (عمق):
		أرجع ('  ' * عمق).replace ('        ', '\t')

	تعریف __str__ (self, شاخص_2=0, عمق=4):
		s = []
		indent = self._indent (عمق)
		إذا isinstance (self._c, list):
			إذا طول (self._c) == 0:
				أكد شاخص_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وإلاإذا طول (self._c) == 1:
				أكد شاخص_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, بعدی (iter (self._c)), شاخص_2 أو ''))
			وإلا:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], شاخص_2 أو ''))
				إذا شاخص_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (عمق + 2), شاخص_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروع=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (عمق + 2), cp, شاخص_2 + i, ')' إذا i == طول (self._c) - 1 وإلا ' &&'))
				s.append ('{}{{\n'.format (indent))
				لكل i في دامنه (شاخص_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (عمق + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (عمق + 1)))
				s.append ('{}}}\n'.format (indent))
		وإلا:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, شاخص_2 أو ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (مجموعه)
			لكل اولی, rest في sorted (self._c.اقلام ()):
				cases[rest.__str__ (شاخص_2 + 1, عمق + 2)].جمع_کن (اولی)
			لكل body, labels في sorted (cases.اقلام (), کلید_2=مجهول b_ls: sorted (b_ls[1])[0]):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i % 4 == 0:
						s.append (self._indent (عمق + 1))
					وإلا:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				إذا طول (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (عمق + 2)))
			s.append ('{}}}\n'.format (indent))
		أرجع ''.join (s)

	تعریف rust_str (self, شاخص_2=0):
		s = []
		إذا isinstance (self._c, list):
			إذا طول (self._c) == 0:
				أكد شاخص_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وإلاإذا طول (self._c) == 1:
				أكد شاخص_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (بعدی (iter (self._c)), شاخص_2))
			وإلا:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], شاخص_2))
				إذا شاخص_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (شاخص_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروع=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, شاخص_2 + i, '' إذا i == طول (self._c) - 1 وإلا ' &&'))
				s.append ('{\n')
				لكل i في دامنه (شاخص_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		وإلا:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (شاخص_2))
			cases = collections.defaultdict (مجموعه)
			لكل اولی, rest في sorted (self._c.اقلام ()):
				cases[rest.rust_str (شاخص_2 + 1)].جمع_کن (اولی)
			اقلام = sorted (cases.اقلام (), کلید_2=مجهول b_ls: sorted (b_ls[1])[0])
			لكل ii, (body, labels) في enumerate (اقلام):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i == طول (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
					وإلا:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				s.append (body)
				s.append ('}')
				إذا ii == طول (اقلام) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		أرجع ''.join (s)

constraints = {}
با open (sys.argv[1], encoding='utf-8') كـ f:
	constraints_سرآیند = []
	طالما درست:
		سطر = f.readline ().strip ()
		إذا سطر == '#':
			اكسر
		constraints_سرآیند.append(سطر)
	لكل سطر في f:
		j = سطر.پیدا_کن ('#')
		إذا j >= 0:
			سطر = سطر[:j]
		constraint = [صحيح (cp, 16) لكل cp في سطر.تقسیم (';')[0].تقسیم ()]
		إذا ليس constraint: استمر
		أكد 2 <= طول (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		إذا script في constraints:
			constraints[script].جمع_کن (constraint)
		وإلا:
			constraints[script] = Constraint_مجموعه (constraint)
		أكد constraints, 'No constraints found'

چاپ ('/* == Start of generated functions == */')
إذا output_rust:
	چاپ ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	چاپ ()
	چاپ ('#![allow(clippy::single_match)]')
	چاپ ()
	چاپ ('use super::buffer::hb_buffer_t;')
	چاپ ('use super::script;')
	چاپ ('use crate::BufferFlags;')
	چاپ ()
	چاپ ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	چاپ ('    buffer.output_glyph(0x25CC);')
	چاپ ('    {')
	چاپ ('        let out_idx = buffer.out_len - 1;')
	چاپ ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	چاپ ('    }')
	چاپ ('}')
	چاپ ()
	چاپ ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	چاپ ('    output_dotted_circle(buffer);')
	چاپ ('    buffer.next_glyph();')
	چاپ ('}')
	چاپ ()
	چاپ ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	چاپ ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	چاپ ('        return;')
	چاپ ('    }')
	چاپ ()
	چاپ ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	چاپ ('    // vowel-sequences that look like another vowel.  Data for each script')
	چاپ ('    // collected from the USE script development spec.')
	چاپ ('    //')
	چاپ ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	چاپ ('    buffer.clear_output();')
	چاپ ('    match buffer.script {')

	لكل script, constraints في sorted (constraints.اقلام (), کلید_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چاپ ('        Some(script::{}) => {{'.format (script.فوقانی ()))
		چاپ ('            buffer.idx = 0;')
		چاپ ('            while buffer.idx + 1 < buffer.len {')
		چاپ ('                #[allow(unused_mut)]')
		چاپ ('                let mut matched = false;')
		اكتب (constraints.rust_str ())
		چاپ ('                buffer.next_glyph();')
		چاپ ('                if matched { output_with_dotted_circle(buffer); }')
		چاپ ('            }')
		چاپ ('        }')
		چاپ ()

	چاپ ('        _ => {}')
	چاپ ('    }')
	چاپ ('    buffer.sync();')
	چاپ ('}')
وإلا:
	چاپ ('/*')
	چاپ (' * The following functions are generated by running:')
	چاپ (' *')
	چاپ (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.مسیر.basename (sys.argv[0]))
	چاپ (' *')
	چاپ (' * on files with these headers:')
	چاپ (' *')
	لكل سطر في constraints_سرآیند:
		چاپ (' * %s' % سطر.strip ())
	چاپ (' *')
	لكل سطر في scripts_سرآیند:
		چاپ (' * %s' % سطر.strip ())
	چاپ (' */')
	چاپ ()
	چاپ ('#include "hb.hh"')
	چاپ ()
	چاپ ('#ifndef HB_NO_OT_SHAPE')
	چاپ ()
	چاپ ('#include "hb-ot-shaper-vowel-constraints.hh"')
	چاپ ()
	چاپ ('static void')
	چاپ ('_output_dotted_circle (hb_buffer_t *buffer)')
	چاپ ('{')
	چاپ ('  (void) buffer->output_glyph (0x25CCu);')
	چاپ ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	چاپ ('}')
	چاپ ()
	چاپ ('static void')
	چاپ ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	چاپ ('{')
	چاپ ('  _output_dotted_circle (buffer);')
	چاپ ('  (void) buffer->next_glyph ();')
	چاپ ('}')
	چاپ ()
	چاپ ('void')
	چاپ ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	چاپ ('\t\t\t\t       hb_buffer_t              *buffer,')
	چاپ ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	چاپ ('{')
	چاپ ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	چاپ ('  return;')
	چاپ ('#endif')
	چاپ ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	چاپ ('    return;')
	چاپ ()
	چاپ ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	چاپ ('   * vowel-sequences that look like another vowel.  Data for each script')
	چاپ ('   * collected from the USE script development spec.')
	چاپ ('   *')
	چاپ ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	چاپ ('   */')
	چاپ ('  buffer->clear_output ();')
	چاپ ('  unsigned int count = buffer->len;')
	چاپ ('  switch ((unsigned) buffer->props.script)')
	چاپ ('  {')

	لكل script, constraints في sorted (constraints.اقلام (), کلید_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چاپ ('    case HB_SCRIPT_{}:'.format (script.فوقانی ()))
		چاپ ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		چاپ ('      {')
		چاپ ('\tbool matched = false;')
		اكتب (str (constraints))
		چاپ ('\t(void) buffer->next_glyph ();')
		چاپ ('\tif (matched) _output_with_dotted_circle (buffer);')
		چاپ ('      }')
		چاپ ('      break;')
		چاپ ()

	چاپ ('    default:')
	چاپ ('      break;')
	چاپ ('  }')
	چاپ ('  buffer->sync ();')
	چاپ ('}')
	چاپ ()
	چاپ ()
	چاپ ('#endif')
چاپ ('/* == End of generated functions == */')
