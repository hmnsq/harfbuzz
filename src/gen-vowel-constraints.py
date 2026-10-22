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

আনযন collections
আনযন os
def লিখ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.লিখ (s.encode ('utf-8'))
আনযন sys

যদি len (sys.argv) > 1 আৰু sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = সঁচা
অন্যথা:
	output_rust = মিছা

যদি len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') ইৱ f:
	scripts_header = [f.readline () বাবে i অন্তঃ range (2)]
	scripts = {}
	script_order = {}
	বাবে সাৰি অন্তঃ f:
		j = সাৰি.find ('#')
		যদি j >= 0:
			সাৰি = সাৰি[:j]
		fields = [x.strip () বাবে x অন্তঃ সাৰি.split (';')]
		যদি len (fields) == 1:
			অনুৱর্ত
		uu = fields[0].split ('..')
		start = পূর্ণাঙ্ক (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণাঙ্ক (uu[1], 16)
		script = fields[1]
		বাবে u অন্তঃ range (start, end + 1):
			scripts[u] = script
		যদি script নহয় অন্তঃ script_order:
			script_order[script] = start

শ্ৰেণী ConstraintSet (object):
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
		যদি নহয় constraint:
			ঘূৰাই
		first = constraint[0]
		rest = constraint[1:]
		যদি isinstance (self._c, list):
			যদি constraint == self._c[:len (constraint)]:
				self._c = constraint
			নোচেদ্যদি self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		যদি isinstance (self._c, dict):
			যদি first অন্তঃ self._c:
				self._c[first].add (rest)
			অন্যথা:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		ঘূৰাই ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		যদি isinstance (self._c, list):
			যদি len (self._c) == 0:
				প্রতিজ্ঞা index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			নোচেদ্যদি len (self._c) == 1:
				প্রতিজ্ঞা index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index বা ''))
			অন্যথা:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index বা ''))
				যদি index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				বাবে i, cp অন্তঃ enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' যদি i == len (self._c) - 1 অন্যথা ' &&'))
				s.append ('{}{{\n'.format (indent))
				বাবে i অন্তঃ range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		অন্যথা:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index বা ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			বাবে first, rest অন্তঃ sorted (self._c.বস্তুসমূহ ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			বাবে body, labels অন্তঃ sorted (cases.বস্তুসমূহ (), চাবি_2=অনামন্ b_ls: sorted (b_ls[1])[0]):
				বাবে i, cp অন্তঃ enumerate (sorted (labels)):
					যদি i % 4 == 0:
						s.append (self._indent (depth + 1))
					অন্যথা:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' যদি i % 4 == 3 অন্যথা ''))
				যদি len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		ঘূৰাই ''.join (s)

	def rust_str (self, index=0):
		s = []
		যদি isinstance (self._c, list):
			যদি len (self._c) == 0:
				প্রতিজ্ঞা index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			নোচেদ্যদি len (self._c) == 1:
				প্রতিজ্ঞা index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			অন্যথা:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				যদি index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				বাবে i, cp অন্তঃ enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' যদি i == len (self._c) - 1 অন্যথা ' &&'))
				s.append ('{\n')
				বাবে i অন্তঃ range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		অন্যথা:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			বাবে first, rest অন্তঃ sorted (self._c.বস্তুসমূহ ()):
				cases[rest.rust_str (index + 1)].add (first)
			বস্তুসমূহ = sorted (cases.বস্তুসমূহ (), চাবি_2=অনামন্ b_ls: sorted (b_ls[1])[0])
			বাবে ii, (body, labels) অন্তঃ enumerate (বস্তুসমূহ):
				বাবে i, cp অন্তঃ enumerate (sorted (labels)):
					যদি i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' যদি i % 4 == 3 অন্যথা ''))
					অন্যথা:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' যদি i % 4 == 3 অন্যথা ''))
				s.append (body)
				s.append ('}')
				যদি ii == len (বস্তুসমূহ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		ঘূৰাই ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') ইৱ f:
	constraints_header = []
	যেতিয়ালৈ সঁচা:
		সাৰি = f.readline ().strip ()
		যদি সাৰি == '#':
			ভঙ্গ
		constraints_header.append(সাৰি)
	বাবে সাৰি অন্তঃ f:
		j = সাৰি.find ('#')
		যদি j >= 0:
			সাৰি = সাৰি[:j]
		constraint = [পূর্ণাঙ্ক (cp, 16) বাবে cp অন্তঃ সাৰি.split (';')[0].split ()]
		যদি নহয় constraint: অনুৱর্ত
		প্রতিজ্ঞা 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		যদি script অন্তঃ constraints:
			constraints[script].add (constraint)
		অন্যথা:
			constraints[script] = ConstraintSet (constraint)
		প্রতিজ্ঞা constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
যদি output_rust:
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

	বাবে script, constraints অন্তঃ sorted (constraints.বস্তুসমূহ (), চাবি_2=অনামন্ s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		লিখ (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
অন্যথা:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	বাবে সাৰি অন্তঃ constraints_header:
		print (' * %s' % সাৰি.strip ())
	print (' *')
	বাবে সাৰি অন্তঃ scripts_header:
		print (' * %s' % সাৰি.strip ())
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

	বাবে script, constraints অন্তঃ sorted (constraints.বস্তুসমূহ (), চাবি_2=অনামন্ s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		লিখ (str (constraints))
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
