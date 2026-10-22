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

إذا اوږدوالی (sys.argv) > 1 و sys.argv[1] == '--rust':
	لرېکړه sys.argv[1]
	output_rust = ریښتیا
وإلا:
	output_rust = خطأ

إذا اوږدوالی (sys.argv) != 3:
	sys.exit (__doc__)

سره open (sys.argv[2], encoding='utf-8') كـ f:
	scripts_سرلیک = [f.readline () لكل i في لړ (2)]
	scripts = {}
	script_ترتیب = {}
	لكل کرښه في f:
		j = کرښه.پیدا_کړه ('#')
		إذا j >= 0:
			کرښه = کرښه[:j]
		fields = [x.strip () لكل x في کرښه.وېشل (';')]
		إذا اوږدوالی (fields) == 1:
			استمر
		uu = fields[0].وېشل ('..')
		پیل = صحيح (uu[0], 16)
		إذا اوږدوالی (uu) == 1:
			پای_ټکی = پیل
		وإلا:
			پای_ټکی = صحيح (uu[1], 16)
		script = fields[1]
		لكل u في لړ (پیل, پای_ټکی + 1):
			scripts[u] = script
		إذا script ليس في script_ترتیب:
			script_ترتیب[script] = پیل

صنف Constraint_سېټ (object):
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

	تعریف جمع_کړه (self, constraint):
		"""Add a constraint to this set."""
		إذا ليس constraint:
			أرجع
		لومړی = constraint[0]
		rest = constraint[1:]
		إذا isinstance (self._c, list):
			إذا constraint == self._c[:اوږدوالی (constraint)]:
				self._c = constraint
			وإلاإذا self._c != constraint[:اوږدوالی (self._c)]:
				self._c = {self._c[0]: Constraint_سېټ (self._c[1:])}
		إذا isinstance (self._c, dict):
			إذا لومړی في self._c:
				self._c[لومړی].جمع_کړه (rest)
			وإلا:
				self._c[لومړی] = Constraint_سېټ (rest)

	@staticmethod
	تعریف _indent (ژوروالی):
		أرجع ('  ' * ژوروالی).replace ('        ', '\t')

	تعریف __str__ (self, شاخص_2=0, ژوروالی=4):
		s = []
		indent = self._indent (ژوروالی)
		إذا isinstance (self._c, list):
			إذا اوږدوالی (self._c) == 0:
				أكد شاخص_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وإلاإذا اوږدوالی (self._c) == 1:
				أكد شاخص_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, بل (iter (self._c)), شاخص_2 أو ''))
			وإلا:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], شاخص_2 أو ''))
				إذا شاخص_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (ژوروالی + 2), شاخص_2 + 1))
				لكل i, cp في enumerate (self._c[1:], پیل=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (ژوروالی + 2), cp, شاخص_2 + i, ')' إذا i == اوږدوالی (self._c) - 1 وإلا ' &&'))
				s.append ('{}{{\n'.format (indent))
				لكل i في لړ (شاخص_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (ژوروالی + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (ژوروالی + 1)))
				s.append ('{}}}\n'.format (indent))
		وإلا:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, شاخص_2 أو ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (سېټ)
			لكل لومړی, rest في sorted (self._c.توکي ()):
				cases[rest.__str__ (شاخص_2 + 1, ژوروالی + 2)].جمع_کړه (لومړی)
			لكل body, labels في sorted (cases.توکي (), کلی_2=مجهول b_ls: sorted (b_ls[1])[0]):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i % 4 == 0:
						s.append (self._indent (ژوروالی + 1))
					وإلا:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				إذا اوږدوالی (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (ژوروالی + 2)))
			s.append ('{}}}\n'.format (indent))
		أرجع ''.join (s)

	تعریف rust_str (self, شاخص_2=0):
		s = []
		إذا isinstance (self._c, list):
			إذا اوږدوالی (self._c) == 0:
				أكد شاخص_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وإلاإذا اوږدوالی (self._c) == 1:
				أكد شاخص_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (بل (iter (self._c)), شاخص_2))
			وإلا:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], شاخص_2))
				إذا شاخص_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (شاخص_2 + 1))
				لكل i, cp في enumerate (self._c[1:], پیل=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, شاخص_2 + i, '' إذا i == اوږدوالی (self._c) - 1 وإلا ' &&'))
				s.append ('{\n')
				لكل i في لړ (شاخص_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		وإلا:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (شاخص_2))
			cases = collections.defaultdict (سېټ)
			لكل لومړی, rest في sorted (self._c.توکي ()):
				cases[rest.rust_str (شاخص_2 + 1)].جمع_کړه (لومړی)
			توکي = sorted (cases.توکي (), کلی_2=مجهول b_ls: sorted (b_ls[1])[0])
			لكل ii, (body, labels) في enumerate (توکي):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i == اوږدوالی (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
					وإلا:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				s.append (body)
				s.append ('}')
				إذا ii == اوږدوالی (توکي) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		أرجع ''.join (s)

constraints = {}
سره open (sys.argv[1], encoding='utf-8') كـ f:
	constraints_سرلیک = []
	طالما ریښتیا:
		کرښه = f.readline ().strip ()
		إذا کرښه == '#':
			اكسر
		constraints_سرلیک.append(کرښه)
	لكل کرښه في f:
		j = کرښه.پیدا_کړه ('#')
		إذا j >= 0:
			کرښه = کرښه[:j]
		constraint = [صحيح (cp, 16) لكل cp في کرښه.وېشل (';')[0].وېشل ()]
		إذا ليس constraint: استمر
		أكد 2 <= اوږدوالی (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		إذا script في constraints:
			constraints[script].جمع_کړه (constraint)
		وإلا:
			constraints[script] = Constraint_سېټ (constraint)
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

	لكل script, constraints في sorted (constraints.توکي (), کلی_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چاپ ('        Some(script::{}) => {{'.format (script.پاسنی ()))
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
	چاپ (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.لار.basename (sys.argv[0]))
	چاپ (' *')
	چاپ (' * on files with these headers:')
	چاپ (' *')
	لكل کرښه في constraints_سرلیک:
		چاپ (' * %s' % کرښه.strip ())
	چاپ (' *')
	لكل کرښه في scripts_سرلیک:
		چاپ (' * %s' % کرښه.strip ())
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

	لكل script, constraints في sorted (constraints.توکي (), کلی_2=مجهول s_c: script_ترتیب[s_c[0]]):
		چاپ ('    case HB_SCRIPT_{}:'.format (script.پاسنی ()))
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
