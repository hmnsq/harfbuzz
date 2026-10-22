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

దిగుమతి collections
దిగుమతి os
నిర్వచనం లిఖ (s):
	sys.stdout.flush ()
	sys.stdout.బఫరు.లిఖ (s.encode ('utf-8'))
దిగుమతి sys

ఒకవేళ పొడవు (sys.argv) > 1 మరియు sys.argv[1] == '--rust':
	తీసివేయి sys.argv[1]
	output_rust = నిజం
లేకపోతే:
	output_rust = అబద్ధం

ఒకవేళ పొడవు (sys.argv) != 3:
	sys.exit (__doc__)

తోడుగా open (sys.argv[2], encoding='utf-8') ఇవ f:
	scripts_శీర్షిక = [f.readline () కొరకు i లో శ్రేణి (2)]
	scripts = {}
	script_క్రమం = {}
	కొరకు పంక్తి లో f:
		j = పంక్తి.కనుగొను ('#')
		ఒకవేళ j >= 0:
			పంక్తి = పంక్తి[:j]
		fields = [x.strip () కొరకు x లో పంక్తి.విభజించు (';')]
		ఒకవేళ పొడవు (fields) == 1:
			కొనసాగు
		uu = fields[0].విభజించు ('..')
		ప్రారంభం = పూర్ణాంకం (uu[0], 16)
		ఒకవేళ పొడవు (uu) == 1:
			చివర = ప్రారంభం
		లేకపోతే:
			చివర = పూర్ణాంకం (uu[1], 16)
		script = fields[1]
		కొరకు u లో శ్రేణి (ప్రారంభం, చివర + 1):
			scripts[u] = script
		ఒకవేళ script కాదు లో script_క్రమం:
			script_క్రమం[script] = ప్రారంభం

తరగతి Constraint_సమితి (object):
	"""A set of prohibited code point sequences.

	Args:
		constraint (List[int]): A prohibited code point sequence.

	"""
	నిర్వచనం __init__ (self, constraint):
		# Either a list or a dictionary. As a list of code points, it
		# represents a prohibited code point sequence. As a dictionary,
		# it represents a set of prohibited sequences, where each item
		# represents the set of prohibited sequences starting with the
		# key (a code point) concatenated with any of the values
		# (ConstraintSets).
		self._c = constraint

	నిర్వచనం కూడు (self, constraint):
		"""Add a constraint to this set."""
		ఒకవేళ కాదు constraint:
			తిరిగి
		మొదటి = constraint[0]
		rest = constraint[1:]
		ఒకవేళ isinstance (self._c, list):
			ఒకవేళ constraint == self._c[:పొడవు (constraint)]:
				self._c = constraint
			లేదావేళ self._c != constraint[:పొడవు (self._c)]:
				self._c = {self._c[0]: Constraint_సమితి (self._c[1:])}
		ఒకవేళ isinstance (self._c, dict):
			ఒకవేళ మొదటి లో self._c:
				self._c[మొదటి].కూడు (rest)
			లేకపోతే:
				self._c[మొదటి] = Constraint_సమితి (rest)

	@staticmethod
	నిర్వచనం _indent (లోతు):
		తిరిగి ('  ' * లోతు).replace ('        ', '\t')

	నిర్వచనం __str__ (self, సూచీ_2=0, లోతు=4):
		s = []
		indent = self._indent (లోతు)
		ఒకవేళ isinstance (self._c, list):
			ఒకవేళ పొడవు (self._c) == 0:
				ప్రతిజ్ఞా సూచీ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			లేదావేళ పొడవు (self._c) == 1:
				ప్రతిజ్ఞా సూచీ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, తదుపరి (iter (self._c)), సూచీ_2 లేదా ''))
			లేకపోతే:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], సూచీ_2 లేదా ''))
				ఒకవేళ సూచీ_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (లోతు + 2), సూచీ_2 + 1))
				కొరకు i, cp లో enumerate (self._c[1:], ప్రారంభం=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (లోతు + 2), cp, సూచీ_2 + i, ')' ఒకవేళ i == పొడవు (self._c) - 1 లేకపోతే ' &&'))
				s.append ('{}{{\n'.format (indent))
				కొరకు i లో శ్రేణి (సూచీ_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (లోతు + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (లోతు + 1)))
				s.append ('{}}}\n'.format (indent))
		లేకపోతే:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, సూచీ_2 లేదా ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (సమితి)
			కొరకు మొదటి, rest లో sorted (self._c.వస్తువులు ()):
				cases[rest.__str__ (సూచీ_2 + 1, లోతు + 2)].కూడు (మొదటి)
			కొరకు body, labels లో sorted (cases.వస్తువులు (), కీలకం_2=అజ్ఞాత b_ls: sorted (b_ls[1])[0]):
				కొరకు i, cp లో enumerate (sorted (labels)):
					ఒకవేళ i % 4 == 0:
						s.append (self._indent (లోతు + 1))
					లేకపోతే:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' ఒకవేళ i % 4 == 3 లేకపోతే ''))
				ఒకవేళ పొడవు (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (లోతు + 2)))
			s.append ('{}}}\n'.format (indent))
		తిరిగి ''.join (s)

	నిర్వచనం rust_str (self, సూచీ_2=0):
		s = []
		ఒకవేళ isinstance (self._c, list):
			ఒకవేళ పొడవు (self._c) == 0:
				ప్రతిజ్ఞా సూచీ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			లేదావేళ పొడవు (self._c) == 1:
				ప్రతిజ్ఞా సూచీ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (తదుపరి (iter (self._c)), సూచీ_2))
			లేకపోతే:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], సూచీ_2))
				ఒకవేళ సూచీ_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (సూచీ_2 + 1))
				కొరకు i, cp లో enumerate (self._c[1:], ప్రారంభం=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, సూచీ_2 + i, '' ఒకవేళ i == పొడవు (self._c) - 1 లేకపోతే ' &&'))
				s.append ('{\n')
				కొరకు i లో శ్రేణి (సూచీ_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		లేకపోతే:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (సూచీ_2))
			cases = collections.defaultdict (సమితి)
			కొరకు మొదటి, rest లో sorted (self._c.వస్తువులు ()):
				cases[rest.rust_str (సూచీ_2 + 1)].కూడు (మొదటి)
			వస్తువులు = sorted (cases.వస్తువులు (), కీలకం_2=అజ్ఞాత b_ls: sorted (b_ls[1])[0])
			కొరకు ii, (body, labels) లో enumerate (వస్తువులు):
				కొరకు i, cp లో enumerate (sorted (labels)):
					ఒకవేళ i == పొడవు (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' ఒకవేళ i % 4 == 3 లేకపోతే ''))
					లేకపోతే:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' ఒకవేళ i % 4 == 3 లేకపోతే ''))
				s.append (body)
				s.append ('}')
				ఒకవేళ ii == పొడవు (వస్తువులు) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		తిరిగి ''.join (s)

constraints = {}
తోడుగా open (sys.argv[1], encoding='utf-8') ఇవ f:
	constraints_శీర్షిక = []
	అయితే నిజం:
		పంక్తి = f.readline ().strip ()
		ఒకవేళ పంక్తి == '#':
			ఆపు
		constraints_శీర్షిక.append(పంక్తి)
	కొరకు పంక్తి లో f:
		j = పంక్తి.కనుగొను ('#')
		ఒకవేళ j >= 0:
			పంక్తి = పంక్తి[:j]
		constraint = [పూర్ణాంకం (cp, 16) కొరకు cp లో పంక్తి.విభజించు (';')[0].విభజించు ()]
		ఒకవేళ కాదు constraint: కొనసాగు
		ప్రతిజ్ఞా 2 <= పొడవు (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		ఒకవేళ script లో constraints:
			constraints[script].కూడు (constraint)
		లేకపోతే:
			constraints[script] = Constraint_సమితి (constraint)
		ప్రతిజ్ఞా constraints, 'No constraints found'

ముద్రించు ('/* == Start of generated functions == */')
ఒకవేళ output_rust:
	ముద్రించు ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	ముద్రించు ()
	ముద్రించు ('#![allow(clippy::single_match)]')
	ముద్రించు ()
	ముద్రించు ('use super::buffer::hb_buffer_t;')
	ముద్రించు ('use super::script;')
	ముద్రించు ('use crate::BufferFlags;')
	ముద్రించు ()
	ముద్రించు ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	ముద్రించు ('    buffer.output_glyph(0x25CC);')
	ముద్రించు ('    {')
	ముద్రించు ('        let out_idx = buffer.out_len - 1;')
	ముద్రించు ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	ముద్రించు ('    }')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	ముద్రించు ('    output_dotted_circle(buffer);')
	ముద్రించు ('    buffer.next_glyph();')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	ముద్రించు ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	ముద్రించు ('        return;')
	ముద్రించు ('    }')
	ముద్రించు ()
	ముద్రించు ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ముద్రించు ('    // vowel-sequences that look like another vowel.  Data for each script')
	ముద్రించు ('    // collected from the USE script development spec.')
	ముద్రించు ('    //')
	ముద్రించు ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	ముద్రించు ('    buffer.clear_output();')
	ముద్రించు ('    match buffer.script {')

	కొరకు script, constraints లో sorted (constraints.వస్తువులు (), కీలకం_2=అజ్ఞాత s_c: script_క్రమం[s_c[0]]):
		ముద్రించు ('        Some(script::{}) => {{'.format (script.పై ()))
		ముద్రించు ('            buffer.idx = 0;')
		ముద్రించు ('            while buffer.idx + 1 < buffer.len {')
		ముద్రించు ('                #[allow(unused_mut)]')
		ముద్రించు ('                let mut matched = false;')
		లిఖ (constraints.rust_str ())
		ముద్రించు ('                buffer.next_glyph();')
		ముద్రించు ('                if matched { output_with_dotted_circle(buffer); }')
		ముద్రించు ('            }')
		ముద్రించు ('        }')
		ముద్రించు ()

	ముద్రించు ('        _ => {}')
	ముద్రించు ('    }')
	ముద్రించు ('    buffer.sync();')
	ముద్రించు ('}')
లేకపోతే:
	ముద్రించు ('/*')
	ముద్రించు (' * The following functions are generated by running:')
	ముద్రించు (' *')
	ముద్రించు (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.మార్గం.basename (sys.argv[0]))
	ముద్రించు (' *')
	ముద్రించు (' * on files with these headers:')
	ముద్రించు (' *')
	కొరకు పంక్తి లో constraints_శీర్షిక:
		ముద్రించు (' * %s' % పంక్తి.strip ())
	ముద్రించు (' *')
	కొరకు పంక్తి లో scripts_శీర్షిక:
		ముద్రించు (' * %s' % పంక్తి.strip ())
	ముద్రించు (' */')
	ముద్రించు ()
	ముద్రించు ('#include "hb.hh"')
	ముద్రించు ()
	ముద్రించు ('#ifndef HB_NO_OT_SHAPE')
	ముద్రించు ()
	ముద్రించు ('#include "hb-ot-shaper-vowel-constraints.hh"')
	ముద్రించు ()
	ముద్రించు ('static void')
	ముద్రించు ('_output_dotted_circle (hb_buffer_t *buffer)')
	ముద్రించు ('{')
	ముద్రించు ('  (void) buffer->output_glyph (0x25CCu);')
	ముద్రించు ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('static void')
	ముద్రించు ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	ముద్రించు ('{')
	ముద్రించు ('  _output_dotted_circle (buffer);')
	ముద్రించు ('  (void) buffer->next_glyph ();')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('void')
	ముద్రించు ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	ముద్రించు ('\t\t\t\t       hb_buffer_t              *buffer,')
	ముద్రించు ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	ముద్రించు ('{')
	ముద్రించు ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	ముద్రించు ('  return;')
	ముద్రించు ('#endif')
	ముద్రించు ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	ముద్రించు ('    return;')
	ముద్రించు ()
	ముద్రించు ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ముద్రించు ('   * vowel-sequences that look like another vowel.  Data for each script')
	ముద్రించు ('   * collected from the USE script development spec.')
	ముద్రించు ('   *')
	ముద్రించు ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	ముద్రించు ('   */')
	ముద్రించు ('  buffer->clear_output ();')
	ముద్రించు ('  unsigned int count = buffer->len;')
	ముద్రించు ('  switch ((unsigned) buffer->props.script)')
	ముద్రించు ('  {')

	కొరకు script, constraints లో sorted (constraints.వస్తువులు (), కీలకం_2=అజ్ఞాత s_c: script_క్రమం[s_c[0]]):
		ముద్రించు ('    case HB_SCRIPT_{}:'.format (script.పై ()))
		ముద్రించు ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		ముద్రించు ('      {')
		ముద్రించు ('\tbool matched = false;')
		లిఖ (str (constraints))
		ముద్రించు ('\t(void) buffer->next_glyph ();')
		ముద్రించు ('\tif (matched) _output_with_dotted_circle (buffer);')
		ముద్రించు ('      }')
		ముద్రించు ('      break;')
		ముద్రించు ()

	ముద్రించు ('    default:')
	ముద్రించు ('      break;')
	ముద్రించు ('  }')
	ముద్రించు ('  buffer->sync ();')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ()
	ముద్రించు ('#endif')
ముద్రించు ('/* == End of generated functions == */')
