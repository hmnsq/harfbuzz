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

درآمد collections
درآمد os
تعریف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))
درآمد sys

اگر لمبائی (sys.argv) > 1 اور sys.argv[1] == '--rust':
	مٹاؤ sys.argv[1]
	output_rust = سچ
ورنہ:
	output_rust = جھوٹ

اگر لمبائی (sys.argv) != 3:
	sys.exit (__doc__)

ساتھ open (sys.argv[2], encoding='utf-8') بطور f:
	scripts_سرخی = [f.readline () برائے i میں سلسلہ (2)]
	scripts = {}
	script_ترتیب = {}
	برائے سطر میں f:
		j = سطر.ڈھونڈو ('#')
		اگر j >= 0:
			سطر = سطر[:j]
		fields = [x.strip () برائے x میں سطر.بانٹو (';')]
		اگر لمبائی (fields) == 1:
			جاری
		uu = fields[0].بانٹو ('..')
		آغاز = صحیح (uu[0], 16)
		اگر لمبائی (uu) == 1:
			انتہا = آغاز
		ورنہ:
			انتہا = صحیح (uu[1], 16)
		script = fields[1]
		برائے u میں سلسلہ (آغاز, انتہا + 1):
			scripts[u] = script
		اگر script نہیں میں script_ترتیب:
			script_ترتیب[script] = آغاز

جماعت Constraint_سیٹ (object):
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
		اگر نہیں constraint:
			واپس
		پہلا = constraint[0]
		rest = constraint[1:]
		اگر isinstance (self._c, list):
			اگر constraint == self._c[:لمبائی (constraint)]:
				self._c = constraint
			وگرنہ_اگر self._c != constraint[:لمبائی (self._c)]:
				self._c = {self._c[0]: Constraint_سیٹ (self._c[1:])}
		اگر isinstance (self._c, dict):
			اگر پہلا میں self._c:
				self._c[پہلا].جوڑو (rest)
			ورنہ:
				self._c[پہلا] = Constraint_سیٹ (rest)

	@staticmethod
	تعریف _indent (گہرائی):
		واپس ('  ' * گہرائی).replace ('        ', '\t')

	تعریف __str__ (self, اشاریہ_2=0, گہرائی=4):
		s = []
		indent = self._indent (گہرائی)
		اگر isinstance (self._c, list):
			اگر لمبائی (self._c) == 0:
				أكد اشاریہ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			وگرنہ_اگر لمبائی (self._c) == 1:
				أكد اشاریہ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, اگلا (iter (self._c)), اشاریہ_2 یا ''))
			ورنہ:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], اشاریہ_2 یا ''))
				اگر اشاریہ_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (گہرائی + 2), اشاریہ_2 + 1))
				برائے i, cp میں enumerate (self._c[1:], آغاز=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (گہرائی + 2), cp, اشاریہ_2 + i, ')' اگر i == لمبائی (self._c) - 1 ورنہ ' &&'))
				s.append ('{}{{\n'.format (indent))
				برائے i میں سلسلہ (اشاریہ_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (گہرائی + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (گہرائی + 1)))
				s.append ('{}}}\n'.format (indent))
		ورنہ:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, اشاریہ_2 یا ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (سیٹ)
			برائے پہلا, rest میں sorted (self._c.اشیاء ()):
				cases[rest.__str__ (اشاریہ_2 + 1, گہرائی + 2)].جوڑو (پہلا)
			برائے body, labels میں sorted (cases.اشیاء (), کلید_2=بےنام b_ls: sorted (b_ls[1])[0]):
				برائے i, cp میں enumerate (sorted (labels)):
					اگر i % 4 == 0:
						s.append (self._indent (گہرائی + 1))
					ورنہ:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' اگر i % 4 == 3 ورنہ ''))
				اگر لمبائی (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (گہرائی + 2)))
			s.append ('{}}}\n'.format (indent))
		واپس ''.join (s)

	تعریف rust_str (self, اشاریہ_2=0):
		s = []
		اگر isinstance (self._c, list):
			اگر لمبائی (self._c) == 0:
				أكد اشاریہ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			وگرنہ_اگر لمبائی (self._c) == 1:
				أكد اشاریہ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (اگلا (iter (self._c)), اشاریہ_2))
			ورنہ:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], اشاریہ_2))
				اگر اشاریہ_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (اشاریہ_2 + 1))
				برائے i, cp میں enumerate (self._c[1:], آغاز=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, اشاریہ_2 + i, '' اگر i == لمبائی (self._c) - 1 ورنہ ' &&'))
				s.append ('{\n')
				برائے i میں سلسلہ (اشاریہ_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		ورنہ:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (اشاریہ_2))
			cases = collections.defaultdict (سیٹ)
			برائے پہلا, rest میں sorted (self._c.اشیاء ()):
				cases[rest.rust_str (اشاریہ_2 + 1)].جوڑو (پہلا)
			اشیاء = sorted (cases.اشیاء (), کلید_2=بےنام b_ls: sorted (b_ls[1])[0])
			برائے ii, (body, labels) میں enumerate (اشیاء):
				برائے i, cp میں enumerate (sorted (labels)):
					اگر i == لمبائی (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' اگر i % 4 == 3 ورنہ ''))
					ورنہ:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' اگر i % 4 == 3 ورنہ ''))
				s.append (body)
				s.append ('}')
				اگر ii == لمبائی (اشیاء) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		واپس ''.join (s)

constraints = {}
ساتھ open (sys.argv[1], encoding='utf-8') بطور f:
	constraints_سرخی = []
	جب سچ:
		سطر = f.readline ().strip ()
		اگر سطر == '#':
			توڑو
		constraints_سرخی.append(سطر)
	برائے سطر میں f:
		j = سطر.ڈھونڈو ('#')
		اگر j >= 0:
			سطر = سطر[:j]
		constraint = [صحیح (cp, 16) برائے cp میں سطر.بانٹو (';')[0].بانٹو ()]
		اگر نہیں constraint: جاری
		أكد 2 <= لمبائی (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		اگر script میں constraints:
			constraints[script].جوڑو (constraint)
		ورنہ:
			constraints[script] = Constraint_سیٹ (constraint)
		أكد constraints, 'No constraints found'

چھاپو ('/* == Start of generated functions == */')
اگر output_rust:
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

	برائے script, constraints میں sorted (constraints.اشیاء (), کلید_2=بےنام s_c: script_ترتیب[s_c[0]]):
		چھاپو ('        Some(script::{}) => {{'.format (script.بالائی ()))
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
ورنہ:
	چھاپو ('/*')
	چھاپو (' * The following functions are generated by running:')
	چھاپو (' *')
	چھاپو (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.راستہ.basename (sys.argv[0]))
	چھاپو (' *')
	چھاپو (' * on files with these headers:')
	چھاپو (' *')
	برائے سطر میں constraints_سرخی:
		چھاپو (' * %s' % سطر.strip ())
	چھاپو (' *')
	برائے سطر میں scripts_سرخی:
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

	برائے script, constraints میں sorted (constraints.اشیاء (), کلید_2=بےنام s_c: script_ترتیب[s_c[0]]):
		چھاپو ('    case HB_SCRIPT_{}:'.format (script.بالائی ()))
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
