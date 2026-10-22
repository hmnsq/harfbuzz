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
وصف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))
استورد sys

إذا ڊيگهه (sys.argv) > 1 و sys.argv[1] == '--rust':
	هٽايو sys.argv[1]
	output_rust = سچ
وإلا:
	output_rust = خطأ

إذا ڊيگهه (sys.argv) != 3:
	sys.exit (__doc__)

سان open (sys.argv[2], encoding='utf-8') كـ f:
	scripts_سرو = [f.readline () لكل i في سلسلو (2)]
	scripts = {}
	script_ترتيب = {}
	لكل سٽ في f:
		j = سٽ.ڳوليو ('#')
		إذا j >= 0:
			سٽ = سٽ[:j]
		fields = [x.strip () لكل x في سٽ.ورهايو (';')]
		إذا ڊيگهه (fields) == 1:
			استمر
		uu = fields[0].ورهايو ('..')
		شروعات = صحيح (uu[0], 16)
		إذا ڊيگهه (uu) == 1:
			پڇاڙي = شروعات
		وإلا:
			پڇاڙي = صحيح (uu[1], 16)
		script = fields[1]
		لكل u في سلسلو (شروعات, پڇاڙي + 1):
			scripts[u] = script
		إذا script ليس في script_ترتيب:
			script_ترتيب[script] = شروعات

صنف Constraint_سيٽ (object):
	"""A set of prohibited code point sequences.

	Args:
		constraint (List[int]): A prohibited code point sequence.

	"""
	وصف __init__ (self, constraint):
		# Either a list or a dictionary. As a list of code points, it
		# represents a prohibited code point sequence. As a dictionary,
		# it represents a set of prohibited sequences, where each item
		# represents the set of prohibited sequences starting with the
		# key (a code point) concatenated with any of the values
		# (ConstraintSets).
		self._c = constraint

	وصف جوڙيو (self, constraint):
		"""Add a constraint to this set."""
		إذا ليس constraint:
			أرجع
		پهريون = constraint[0]
		rest = constraint[1:]
		إذا isinstance (self._c, list):
			إذا constraint == self._c[:ڊيگهه (constraint)]:
				self._c = constraint
			وإلاإذا self._c != constraint[:ڊيگهه (self._c)]:
				self._c = {self._c[0]: Constraint_سيٽ (self._c[1:])}
		إذا isinstance (self._c, dict):
			إذا پهريون في self._c:
				self._c[پهريون].جوڙيو (rest)
			وإلا:
				self._c[پهريون] = Constraint_سيٽ (rest)

	@staticmethod
	وصف _indent (اونهائي):
		أرجع ('  ' * اونهائي).replace ('        ', '\t')

	وصف __str__ (self, اشاريو_2=0, اونهائي=4):
		s = []
		indent = self._indent (اونهائي)
		إذا isinstance (self._c, list):
			إذا ڊيگهه (self._c) == 0:
				أكد اشاريو_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وإلاإذا ڊيگهه (self._c) == 1:
				أكد اشاريو_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, ايندڙ (iter (self._c)), اشاريو_2 أو ''))
			وإلا:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], اشاريو_2 أو ''))
				إذا اشاريو_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (اونهائي + 2), اشاريو_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروعات=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (اونهائي + 2), cp, اشاريو_2 + i, ')' إذا i == ڊيگهه (self._c) - 1 وإلا ' &&'))
				s.append ('{}{{\n'.format (indent))
				لكل i في سلسلو (اشاريو_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (اونهائي + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (اونهائي + 1)))
				s.append ('{}}}\n'.format (indent))
		وإلا:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, اشاريو_2 أو ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (سيٽ)
			لكل پهريون, rest في sorted (self._c.شيون ()):
				cases[rest.__str__ (اشاريو_2 + 1, اونهائي + 2)].جوڙيو (پهريون)
			لكل body, labels في sorted (cases.شيون (), ڪنجي_2=مجهول b_ls: sorted (b_ls[1])[0]):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i % 4 == 0:
						s.append (self._indent (اونهائي + 1))
					وإلا:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				إذا ڊيگهه (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (اونهائي + 2)))
			s.append ('{}}}\n'.format (indent))
		أرجع ''.join (s)

	وصف rust_str (self, اشاريو_2=0):
		s = []
		إذا isinstance (self._c, list):
			إذا ڊيگهه (self._c) == 0:
				أكد اشاريو_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وإلاإذا ڊيگهه (self._c) == 1:
				أكد اشاريو_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (ايندڙ (iter (self._c)), اشاريو_2))
			وإلا:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], اشاريو_2))
				إذا اشاريو_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (اشاريو_2 + 1))
				لكل i, cp في enumerate (self._c[1:], شروعات=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, اشاريو_2 + i, '' إذا i == ڊيگهه (self._c) - 1 وإلا ' &&'))
				s.append ('{\n')
				لكل i في سلسلو (اشاريو_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		وإلا:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (اشاريو_2))
			cases = collections.defaultdict (سيٽ)
			لكل پهريون, rest في sorted (self._c.شيون ()):
				cases[rest.rust_str (اشاريو_2 + 1)].جوڙيو (پهريون)
			شيون = sorted (cases.شيون (), ڪنجي_2=مجهول b_ls: sorted (b_ls[1])[0])
			لكل ii, (body, labels) في enumerate (شيون):
				لكل i, cp في enumerate (sorted (labels)):
					إذا i == ڊيگهه (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
					وإلا:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' إذا i % 4 == 3 وإلا ''))
				s.append (body)
				s.append ('}')
				إذا ii == ڊيگهه (شيون) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		أرجع ''.join (s)

constraints = {}
سان open (sys.argv[1], encoding='utf-8') كـ f:
	constraints_سرو = []
	طالما سچ:
		سٽ = f.readline ().strip ()
		إذا سٽ == '#':
			اكسر
		constraints_سرو.append(سٽ)
	لكل سٽ في f:
		j = سٽ.ڳوليو ('#')
		إذا j >= 0:
			سٽ = سٽ[:j]
		constraint = [صحيح (cp, 16) لكل cp في سٽ.ورهايو (';')[0].ورهايو ()]
		إذا ليس constraint: استمر
		أكد 2 <= ڊيگهه (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		إذا script في constraints:
			constraints[script].جوڙيو (constraint)
		وإلا:
			constraints[script] = Constraint_سيٽ (constraint)
		أكد constraints, 'No constraints found'

ڇاپيو ('/* == Start of generated functions == */')
إذا output_rust:
	ڇاپيو ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	ڇاپيو ()
	ڇاپيو ('#![allow(clippy::single_match)]')
	ڇاپيو ()
	ڇاپيو ('use super::buffer::hb_buffer_t;')
	ڇاپيو ('use super::script;')
	ڇاپيو ('use crate::BufferFlags;')
	ڇاپيو ()
	ڇاپيو ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	ڇاپيو ('    buffer.output_glyph(0x25CC);')
	ڇاپيو ('    {')
	ڇاپيو ('        let out_idx = buffer.out_len - 1;')
	ڇاپيو ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	ڇاپيو ('    }')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	ڇاپيو ('    output_dotted_circle(buffer);')
	ڇاپيو ('    buffer.next_glyph();')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	ڇاپيو ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	ڇاپيو ('        return;')
	ڇاپيو ('    }')
	ڇاپيو ()
	ڇاپيو ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ڇاپيو ('    // vowel-sequences that look like another vowel.  Data for each script')
	ڇاپيو ('    // collected from the USE script development spec.')
	ڇاپيو ('    //')
	ڇاپيو ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	ڇاپيو ('    buffer.clear_output();')
	ڇاپيو ('    match buffer.script {')

	لكل script, constraints في sorted (constraints.شيون (), ڪنجي_2=مجهول s_c: script_ترتيب[s_c[0]]):
		ڇاپيو ('        Some(script::{}) => {{'.format (script.مٿانهون ()))
		ڇاپيو ('            buffer.idx = 0;')
		ڇاپيو ('            while buffer.idx + 1 < buffer.len {')
		ڇاپيو ('                #[allow(unused_mut)]')
		ڇاپيو ('                let mut matched = false;')
		اكتب (constraints.rust_str ())
		ڇاپيو ('                buffer.next_glyph();')
		ڇاپيو ('                if matched { output_with_dotted_circle(buffer); }')
		ڇاپيو ('            }')
		ڇاپيو ('        }')
		ڇاپيو ()

	ڇاپيو ('        _ => {}')
	ڇاپيو ('    }')
	ڇاپيو ('    buffer.sync();')
	ڇاپيو ('}')
وإلا:
	ڇاپيو ('/*')
	ڇاپيو (' * The following functions are generated by running:')
	ڇاپيو (' *')
	ڇاپيو (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.رستو.basename (sys.argv[0]))
	ڇاپيو (' *')
	ڇاپيو (' * on files with these headers:')
	ڇاپيو (' *')
	لكل سٽ في constraints_سرو:
		ڇاپيو (' * %s' % سٽ.strip ())
	ڇاپيو (' *')
	لكل سٽ في scripts_سرو:
		ڇاپيو (' * %s' % سٽ.strip ())
	ڇاپيو (' */')
	ڇاپيو ()
	ڇاپيو ('#include "hb.hh"')
	ڇاپيو ()
	ڇاپيو ('#ifndef HB_NO_OT_SHAPE')
	ڇاپيو ()
	ڇاپيو ('#include "hb-ot-shaper-vowel-constraints.hh"')
	ڇاپيو ()
	ڇاپيو ('static void')
	ڇاپيو ('_output_dotted_circle (hb_buffer_t *buffer)')
	ڇاپيو ('{')
	ڇاپيو ('  (void) buffer->output_glyph (0x25CCu);')
	ڇاپيو ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('static void')
	ڇاپيو ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	ڇاپيو ('{')
	ڇاپيو ('  _output_dotted_circle (buffer);')
	ڇاپيو ('  (void) buffer->next_glyph ();')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('void')
	ڇاپيو ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	ڇاپيو ('\t\t\t\t       hb_buffer_t              *buffer,')
	ڇاپيو ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	ڇاپيو ('{')
	ڇاپيو ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	ڇاپيو ('  return;')
	ڇاپيو ('#endif')
	ڇاپيو ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	ڇاپيو ('    return;')
	ڇاپيو ()
	ڇاپيو ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ڇاپيو ('   * vowel-sequences that look like another vowel.  Data for each script')
	ڇاپيو ('   * collected from the USE script development spec.')
	ڇاپيو ('   *')
	ڇاپيو ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	ڇاپيو ('   */')
	ڇاپيو ('  buffer->clear_output ();')
	ڇاپيو ('  unsigned int count = buffer->len;')
	ڇاپيو ('  switch ((unsigned) buffer->props.script)')
	ڇاپيو ('  {')

	لكل script, constraints في sorted (constraints.شيون (), ڪنجي_2=مجهول s_c: script_ترتيب[s_c[0]]):
		ڇاپيو ('    case HB_SCRIPT_{}:'.format (script.مٿانهون ()))
		ڇاپيو ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		ڇاپيو ('      {')
		ڇاپيو ('\tbool matched = false;')
		اكتب (str (constraints))
		ڇاپيو ('\t(void) buffer->next_glyph ();')
		ڇاپيو ('\tif (matched) _output_with_dotted_circle (buffer);')
		ڇاپيو ('      }')
		ڇاپيو ('      break;')
		ڇاپيو ()

	ڇاپيو ('    default:')
	ڇاپيو ('      break;')
	ڇاپيو ('  }')
	ڇاپيو ('  buffer->sync ();')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ()
	ڇاپيو ('#endif')
ڇاپيو ('/* == End of generated functions == */')
