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

ਆਯਾਤ collections
ਆਯਾਤ os
ਪਰਿਭਾਸ਼ਾ ਲਿਖ (s):
	sys.stdout.flush ()
	sys.stdout.ਬਫਰ.ਲਿਖ (s.encode ('utf-8'))
ਆਯਾਤ sys

ਜੇ ਲੰਬਾਈ (sys.argv) > 1 ਅਤੇ sys.argv[1] == '--rust':
	ਹਟਾਓ sys.argv[1]
	output_rust = ਸੱਚ
ਨਹੀਂਤਾਂ:
	output_rust = ਝੂਠ

ਜੇ ਲੰਬਾਈ (sys.argv) != 3:
	sys.exit (__doc__)

ਨਾਲ open (sys.argv[2], encoding='utf-8') ਇਵ f:
	scripts_ਸਿਰਲੇਖ = [f.readline () ਲਈ i ਵਿੱਚ ਸੀਮਾ (2)]
	scripts = {}
	script_ਕ੍ਰਮ = {}
	ਲਈ ਸਤਰ ਵਿੱਚ f:
		j = ਸਤਰ.ਲੱਭੋ ('#')
		ਜੇ j >= 0:
			ਸਤਰ = ਸਤਰ[:j]
		fields = [x.strip () ਲਈ x ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')]
		ਜੇ ਲੰਬਾਈ (fields) == 1:
			ਜਾਰੀ
		uu = fields[0].ਵੰਡੋ ('..')
		ਸ਼ੁਰੂ = ਪੂਰਨ (uu[0], 16)
		ਜੇ ਲੰਬਾਈ (uu) == 1:
			ਸਿਰਾ = ਸ਼ੁਰੂ
		ਨਹੀਂਤਾਂ:
			ਸਿਰਾ = ਪੂਰਨ (uu[1], 16)
		script = fields[1]
		ਲਈ u ਵਿੱਚ ਸੀਮਾ (ਸ਼ੁਰੂ, ਸਿਰਾ + 1):
			scripts[u] = script
		ਜੇ script ਨਹੀਂ ਵਿੱਚ script_ਕ੍ਰਮ:
			script_ਕ੍ਰਮ[script] = ਸ਼ੁਰੂ

ਵਰਗ Constraint_ਸੈੱਟ (object):
	"""A set of prohibited code point sequences.

	Args:
		constraint (List[int]): A prohibited code point sequence.

	"""
	ਪਰਿਭਾਸ਼ਾ __init__ (self, constraint):
		# Either a list or a dictionary. As a list of code points, it
		# represents a prohibited code point sequence. As a dictionary,
		# it represents a set of prohibited sequences, where each item
		# represents the set of prohibited sequences starting with the
		# key (a code point) concatenated with any of the values
		# (ConstraintSets).
		self._c = constraint

	ਪਰਿਭਾਸ਼ਾ ਜੋੜੋ (self, constraint):
		"""Add a constraint to this set."""
		ਜੇ ਨਹੀਂ constraint:
			ਵਾਪਸ
		ਪਹਿਲਾ = constraint[0]
		rest = constraint[1:]
		ਜੇ isinstance (self._c, list):
			ਜੇ constraint == self._c[:ਲੰਬਾਈ (constraint)]:
				self._c = constraint
			ਜਾਂਜੇ self._c != constraint[:ਲੰਬਾਈ (self._c)]:
				self._c = {self._c[0]: Constraint_ਸੈੱਟ (self._c[1:])}
		ਜੇ isinstance (self._c, dict):
			ਜੇ ਪਹਿਲਾ ਵਿੱਚ self._c:
				self._c[ਪਹਿਲਾ].ਜੋੜੋ (rest)
			ਨਹੀਂਤਾਂ:
				self._c[ਪਹਿਲਾ] = Constraint_ਸੈੱਟ (rest)

	@staticmethod
	ਪਰਿਭਾਸ਼ਾ _indent (ਡੂੰਘਾਈ):
		ਵਾਪਸ ('  ' * ਡੂੰਘਾਈ).replace ('        ', '\t')

	ਪਰਿਭਾਸ਼ਾ __str__ (self, ਸੂਚਕਾਂਕ_2=0, ਡੂੰਘਾਈ=4):
		s = []
		indent = self._indent (ਡੂੰਘਾਈ)
		ਜੇ isinstance (self._c, list):
			ਜੇ ਲੰਬਾਈ (self._c) == 0:
				ਪ੍ਰਤਿਜ੍ਞਾ ਸੂਚਕਾਂਕ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			ਜਾਂਜੇ ਲੰਬਾਈ (self._c) == 1:
				ਪ੍ਰਤਿਜ੍ਞਾ ਸੂਚਕਾਂਕ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, ਅਗਲਾ (iter (self._c)), ਸੂਚਕਾਂਕ_2 ਜਾਂ ''))
			ਨਹੀਂਤਾਂ:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], ਸੂਚਕਾਂਕ_2 ਜਾਂ ''))
				ਜੇ ਸੂਚਕਾਂਕ_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (ਡੂੰਘਾਈ + 2), ਸੂਚਕਾਂਕ_2 + 1))
				ਲਈ i, cp ਵਿੱਚ enumerate (self._c[1:], ਸ਼ੁਰੂ=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (ਡੂੰਘਾਈ + 2), cp, ਸੂਚਕਾਂਕ_2 + i, ')' ਜੇ i == ਲੰਬਾਈ (self._c) - 1 ਨਹੀਂਤਾਂ ' &&'))
				s.append ('{}{{\n'.format (indent))
				ਲਈ i ਵਿੱਚ ਸੀਮਾ (ਸੂਚਕਾਂਕ_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (ਡੂੰਘਾਈ + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (ਡੂੰਘਾਈ + 1)))
				s.append ('{}}}\n'.format (indent))
		ਨਹੀਂਤਾਂ:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, ਸੂਚਕਾਂਕ_2 ਜਾਂ ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (ਸੈੱਟ)
			ਲਈ ਪਹਿਲਾ, rest ਵਿੱਚ sorted (self._c.ਚੀਜ਼ਾਂ ()):
				cases[rest.__str__ (ਸੂਚਕਾਂਕ_2 + 1, ਡੂੰਘਾਈ + 2)].ਜੋੜੋ (ਪਹਿਲਾ)
			ਲਈ body, labels ਵਿੱਚ sorted (cases.ਚੀਜ਼ਾਂ (), ਕੁੰਜੀ_2=ਅਨਾਮ b_ls: sorted (b_ls[1])[0]):
				ਲਈ i, cp ਵਿੱਚ enumerate (sorted (labels)):
					ਜੇ i % 4 == 0:
						s.append (self._indent (ਡੂੰਘਾਈ + 1))
					ਨਹੀਂਤਾਂ:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' ਜੇ i % 4 == 3 ਨਹੀਂਤਾਂ ''))
				ਜੇ ਲੰਬਾਈ (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (ਡੂੰਘਾਈ + 2)))
			s.append ('{}}}\n'.format (indent))
		ਵਾਪਸ ''.join (s)

	ਪਰਿਭਾਸ਼ਾ rust_str (self, ਸੂਚਕਾਂਕ_2=0):
		s = []
		ਜੇ isinstance (self._c, list):
			ਜੇ ਲੰਬਾਈ (self._c) == 0:
				ਪ੍ਰਤਿਜ੍ਞਾ ਸੂਚਕਾਂਕ_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			ਜਾਂਜੇ ਲੰਬਾਈ (self._c) == 1:
				ਪ੍ਰਤਿਜ੍ਞਾ ਸੂਚਕਾਂਕ_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (ਅਗਲਾ (iter (self._c)), ਸੂਚਕਾਂਕ_2))
			ਨਹੀਂਤਾਂ:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], ਸੂਚਕਾਂਕ_2))
				ਜੇ ਸੂਚਕਾਂਕ_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (ਸੂਚਕਾਂਕ_2 + 1))
				ਲਈ i, cp ਵਿੱਚ enumerate (self._c[1:], ਸ਼ੁਰੂ=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, ਸੂਚਕਾਂਕ_2 + i, '' ਜੇ i == ਲੰਬਾਈ (self._c) - 1 ਨਹੀਂਤਾਂ ' &&'))
				s.append ('{\n')
				ਲਈ i ਵਿੱਚ ਸੀਮਾ (ਸੂਚਕਾਂਕ_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		ਨਹੀਂਤਾਂ:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (ਸੂਚਕਾਂਕ_2))
			cases = collections.defaultdict (ਸੈੱਟ)
			ਲਈ ਪਹਿਲਾ, rest ਵਿੱਚ sorted (self._c.ਚੀਜ਼ਾਂ ()):
				cases[rest.rust_str (ਸੂਚਕਾਂਕ_2 + 1)].ਜੋੜੋ (ਪਹਿਲਾ)
			ਚੀਜ਼ਾਂ = sorted (cases.ਚੀਜ਼ਾਂ (), ਕੁੰਜੀ_2=ਅਨਾਮ b_ls: sorted (b_ls[1])[0])
			ਲਈ ii, (body, labels) ਵਿੱਚ enumerate (ਚੀਜ਼ਾਂ):
				ਲਈ i, cp ਵਿੱਚ enumerate (sorted (labels)):
					ਜੇ i == ਲੰਬਾਈ (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' ਜੇ i % 4 == 3 ਨਹੀਂਤਾਂ ''))
					ਨਹੀਂਤਾਂ:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' ਜੇ i % 4 == 3 ਨਹੀਂਤਾਂ ''))
				s.append (body)
				s.append ('}')
				ਜੇ ii == ਲੰਬਾਈ (ਚੀਜ਼ਾਂ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		ਵਾਪਸ ''.join (s)

constraints = {}
ਨਾਲ open (sys.argv[1], encoding='utf-8') ਇਵ f:
	constraints_ਸਿਰਲੇਖ = []
	ਜਦੋਂ ਸੱਚ:
		ਸਤਰ = f.readline ().strip ()
		ਜੇ ਸਤਰ == '#':
			ਰੋਕੋ
		constraints_ਸਿਰਲੇਖ.append(ਸਤਰ)
	ਲਈ ਸਤਰ ਵਿੱਚ f:
		j = ਸਤਰ.ਲੱਭੋ ('#')
		ਜੇ j >= 0:
			ਸਤਰ = ਸਤਰ[:j]
		constraint = [ਪੂਰਨ (cp, 16) ਲਈ cp ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')[0].ਵੰਡੋ ()]
		ਜੇ ਨਹੀਂ constraint: ਜਾਰੀ
		ਪ੍ਰਤਿਜ੍ਞਾ 2 <= ਲੰਬਾਈ (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		ਜੇ script ਵਿੱਚ constraints:
			constraints[script].ਜੋੜੋ (constraint)
		ਨਹੀਂਤਾਂ:
			constraints[script] = Constraint_ਸੈੱਟ (constraint)
		ਪ੍ਰਤਿਜ੍ਞਾ constraints, 'No constraints found'

ਛਾਪੋ ('/* == Start of generated functions == */')
ਜੇ output_rust:
	ਛਾਪੋ ('// WARNING: this file was generated by ../../harfbuzz/src/gen-vowel-constraints.py --rust')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#![allow(clippy::single_match)]')
	ਛਾਪੋ ()
	ਛਾਪੋ ('use super::buffer::hb_buffer_t;')
	ਛਾਪੋ ('use super::script;')
	ਛਾਪੋ ('use crate::BufferFlags;')
	ਛਾਪੋ ()
	ਛਾਪੋ ('fn output_dotted_circle(buffer: &mut hb_buffer_t) {')
	ਛਾਪੋ ('    buffer.output_glyph(0x25CC);')
	ਛਾਪੋ ('    {')
	ਛਾਪੋ ('        let out_idx = buffer.out_len - 1;')
	ਛਾਪੋ ('        buffer.out_info_mut()[out_idx].clear_continuation();')
	ਛਾਪੋ ('    }')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('fn output_with_dotted_circle(buffer: &mut hb_buffer_t) {')
	ਛਾਪੋ ('    output_dotted_circle(buffer);')
	ਛਾਪੋ ('    buffer.next_glyph();')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('pub fn preprocess_text_vowel_constraints(buffer: &mut hb_buffer_t) {')
	ਛਾਪੋ ('    if buffer.flags.contains(BufferFlags::DO_NOT_INSERT_DOTTED_CIRCLE) {')
	ਛਾਪੋ ('        return;')
	ਛਾਪੋ ('    }')
	ਛਾਪੋ ()
	ਛਾਪੋ ('    // UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ਛਾਪੋ ('    // vowel-sequences that look like another vowel.  Data for each script')
	ਛਾਪੋ ('    // collected from the USE script development spec.')
	ਛਾਪੋ ('    //')
	ਛਾਪੋ ('    // https://github.com/harfbuzz/harfbuzz/issues/1019')
	ਛਾਪੋ ('    buffer.clear_output();')
	ਛਾਪੋ ('    match buffer.script {')

	ਲਈ script, constraints ਵਿੱਚ sorted (constraints.ਚੀਜ਼ਾਂ (), ਕੁੰਜੀ_2=ਅਨਾਮ s_c: script_ਕ੍ਰਮ[s_c[0]]):
		ਛਾਪੋ ('        Some(script::{}) => {{'.format (script.ਉੱਤਲਾ ()))
		ਛਾਪੋ ('            buffer.idx = 0;')
		ਛਾਪੋ ('            while buffer.idx + 1 < buffer.len {')
		ਛਾਪੋ ('                #[allow(unused_mut)]')
		ਛਾਪੋ ('                let mut matched = false;')
		ਲਿਖ (constraints.rust_str ())
		ਛਾਪੋ ('                buffer.next_glyph();')
		ਛਾਪੋ ('                if matched { output_with_dotted_circle(buffer); }')
		ਛਾਪੋ ('            }')
		ਛਾਪੋ ('        }')
		ਛਾਪੋ ()

	ਛਾਪੋ ('        _ => {}')
	ਛਾਪੋ ('    }')
	ਛਾਪੋ ('    buffer.sync();')
	ਛਾਪੋ ('}')
ਨਹੀਂਤਾਂ:
	ਛਾਪੋ ('/*')
	ਛਾਪੋ (' * The following functions are generated by running:')
	ਛਾਪੋ (' *')
	ਛਾਪੋ (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.ਰਸਤਾ.basename (sys.argv[0]))
	ਛਾਪੋ (' *')
	ਛਾਪੋ (' * on files with these headers:')
	ਛਾਪੋ (' *')
	ਲਈ ਸਤਰ ਵਿੱਚ constraints_ਸਿਰਲੇਖ:
		ਛਾਪੋ (' * %s' % ਸਤਰ.strip ())
	ਛਾਪੋ (' *')
	ਲਈ ਸਤਰ ਵਿੱਚ scripts_ਸਿਰਲੇਖ:
		ਛਾਪੋ (' * %s' % ਸਤਰ.strip ())
	ਛਾਪੋ (' */')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#include "hb.hh"')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#ifndef HB_NO_OT_SHAPE')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#include "hb-ot-shaper-vowel-constraints.hh"')
	ਛਾਪੋ ()
	ਛਾਪੋ ('static void')
	ਛਾਪੋ ('_output_dotted_circle (hb_buffer_t *buffer)')
	ਛਾਪੋ ('{')
	ਛਾਪੋ ('  (void) buffer->output_glyph (0x25CCu);')
	ਛਾਪੋ ('  _hb_glyph_info_clear_continuation (&buffer->prev());')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('static void')
	ਛਾਪੋ ('_output_with_dotted_circle (hb_buffer_t *buffer)')
	ਛਾਪੋ ('{')
	ਛਾਪੋ ('  _output_dotted_circle (buffer);')
	ਛਾਪੋ ('  (void) buffer->next_glyph ();')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('void')
	ਛਾਪੋ ('_hb_preprocess_text_vowel_constraints (const hb_ot_shape_plan_t *plan HB_UNUSED,')
	ਛਾਪੋ ('\t\t\t\t       hb_buffer_t              *buffer,')
	ਛਾਪੋ ('\t\t\t\t       hb_font_t                *font HB_UNUSED)')
	ਛਾਪੋ ('{')
	ਛਾਪੋ ('#ifdef HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS')
	ਛਾਪੋ ('  return;')
	ਛਾਪੋ ('#endif')
	ਛਾਪੋ ('  if (buffer->flags & HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE)')
	ਛਾਪੋ ('    return;')
	ਛਾਪੋ ()
	ਛਾਪੋ ('  /* UGLY UGLY UGLY business of adding dotted-circle in the middle of')
	ਛਾਪੋ ('   * vowel-sequences that look like another vowel.  Data for each script')
	ਛਾਪੋ ('   * collected from the USE script development spec.')
	ਛਾਪੋ ('   *')
	ਛਾਪੋ ('   * https://github.com/harfbuzz/harfbuzz/issues/1019')
	ਛਾਪੋ ('   */')
	ਛਾਪੋ ('  buffer->clear_output ();')
	ਛਾਪੋ ('  unsigned int count = buffer->len;')
	ਛਾਪੋ ('  switch ((unsigned) buffer->props.script)')
	ਛਾਪੋ ('  {')

	ਲਈ script, constraints ਵਿੱਚ sorted (constraints.ਚੀਜ਼ਾਂ (), ਕੁੰਜੀ_2=ਅਨਾਮ s_c: script_ਕ੍ਰਮ[s_c[0]]):
		ਛਾਪੋ ('    case HB_SCRIPT_{}:'.format (script.ਉੱਤਲਾ ()))
		ਛਾਪੋ ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		ਛਾਪੋ ('      {')
		ਛਾਪੋ ('\tbool matched = false;')
		ਲਿਖ (str (constraints))
		ਛਾਪੋ ('\t(void) buffer->next_glyph ();')
		ਛਾਪੋ ('\tif (matched) _output_with_dotted_circle (buffer);')
		ਛਾਪੋ ('      }')
		ਛਾਪੋ ('      break;')
		ਛਾਪੋ ()

	ਛਾਪੋ ('    default:')
	ਛਾਪੋ ('      break;')
	ਛਾਪੋ ('  }')
	ਛਾਪੋ ('  buffer->sync ();')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ()
	ਛਾਪੋ ('#endif')
ਛਾਪੋ ('/* == End of generated functions == */')
