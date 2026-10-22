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

আমদানি collections
আমদানি os
def লিখ (s):
	sys.stdout.flush ()
	sys.stdout.বাফার.লিখ (s.encode ('utf-8'))
আমদানি sys

যদি len (sys.argv) > 1 এবং sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = সত্য
অন্যথা:
	output_rust = মিথ্যা

যদি len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') ইব f:
	scripts_header = [f.readline () জন্য i মধ্যে range (2)]
	scripts = {}
	script_order = {}
	জন্য সারি মধ্যে f:
		j = সারি.find ('#')
		যদি j >= 0:
			সারি = সারি[:j]
		fields = [x.strip () জন্য x মধ্যে সারি.split (';')]
		যদি len (fields) == 1:
			চালু
		uu = fields[0].split ('..')
		start = পূর্ণসংখ্যা (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণসংখ্যা (uu[1], 16)
		script = fields[1]
		জন্য u মধ্যে range (start, end + 1):
			scripts[u] = script
		যদি script নয় মধ্যে script_order:
			script_order[script] = start

শ্রেণী ConstraintSet (object):
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
		যদি নয় constraint:
			ফেরত
		first = constraint[0]
		rest = constraint[1:]
		যদি isinstance (self._c, list):
			যদি constraint == self._c[:len (constraint)]:
				self._c = constraint
			নতুবা_যদি self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		যদি isinstance (self._c, dict):
			যদি first মধ্যে self._c:
				self._c[first].add (rest)
			অন্যথা:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		ফেরত ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, সূচক_2=0, depth=4):
		s = []
		indent = self._indent (depth)
		যদি isinstance (self._c, list):
			যদি len (self._c) == 0:
				প্রতিজ্ঞা সূচক_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			নতুবা_যদি len (self._c) == 1:
				প্রতিজ্ঞা সূচক_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, পরবর্তী (iter (self._c)), সূচক_2 অথবা ''))
			অন্যথা:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], সূচক_2 অথবা ''))
				যদি সূচক_2:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), সূচক_2 + 1))
				জন্য i, cp মধ্যে enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, সূচক_2 + i, ')' যদি i == len (self._c) - 1 অন্যথা ' &&'))
				s.append ('{}{{\n'.format (indent))
				জন্য i মধ্যে range (সূচক_2):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		অন্যথা:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, সূচক_2 অথবা ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			জন্য first, rest মধ্যে sorted (self._c.বস্তুসমূহ ()):
				cases[rest.__str__ (সূচক_2 + 1, depth + 2)].add (first)
			জন্য body, labels মধ্যে sorted (cases.বস্তুসমূহ (), চাবি_2=অনামা b_ls: sorted (b_ls[1])[0]):
				জন্য i, cp মধ্যে enumerate (sorted (labels)):
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
		ফেরত ''.join (s)

	def rust_str (self, সূচক_2=0):
		s = []
		যদি isinstance (self._c, list):
			যদি len (self._c) == 0:
				প্রতিজ্ঞা সূচক_2 == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			নতুবা_যদি len (self._c) == 1:
				প্রতিজ্ঞা সূচক_2 == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (পরবর্তী (iter (self._c)), সূচক_2))
			অন্যথা:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], সূচক_2))
				যদি সূচক_2:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (সূচক_2 + 1))
				জন্য i, cp মধ্যে enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, সূচক_2 + i, '' যদি i == len (self._c) - 1 অন্যথা ' &&'))
				s.append ('{\n')
				জন্য i মধ্যে range (সূচক_2 + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		অন্যথা:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (সূচক_2))
			cases = collections.defaultdict (set)
			জন্য first, rest মধ্যে sorted (self._c.বস্তুসমূহ ()):
				cases[rest.rust_str (সূচক_2 + 1)].add (first)
			বস্তুসমূহ = sorted (cases.বস্তুসমূহ (), চাবি_2=অনামা b_ls: sorted (b_ls[1])[0])
			জন্য ii, (body, labels) মধ্যে enumerate (বস্তুসমূহ):
				জন্য i, cp মধ্যে enumerate (sorted (labels)):
					যদি i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' যদি i % 4 == 3 অন্যথা ''))
					অন্যথা:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' যদি i % 4 == 3 অন্যথা ''))
				s.append (body)
				s.append ('}')
				যদি ii == len (বস্তুসমূহ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		ফেরত ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') ইব f:
	constraints_header = []
	যখন সত্য:
		সারি = f.readline ().strip ()
		যদি সারি == '#':
			বিরতি
		constraints_header.append(সারি)
	জন্য সারি মধ্যে f:
		j = সারি.find ('#')
		যদি j >= 0:
			সারি = সারি[:j]
		constraint = [পূর্ণসংখ্যা (cp, 16) জন্য cp মধ্যে সারি.split (';')[0].split ()]
		যদি নয় constraint: চালু
		প্রতিজ্ঞা 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		যদি script মধ্যে constraints:
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

	জন্য script, constraints মধ্যে sorted (constraints.বস্তুসমূহ (), চাবি_2=অনামা s_c: script_order[s_c[0]]):
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
	জন্য সারি মধ্যে constraints_header:
		print (' * %s' % সারি.strip ())
	print (' *')
	জন্য সারি মধ্যে scripts_header:
		print (' * %s' % সারি.strip ())
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

	জন্য script, constraints মধ্যে sorted (constraints.বস্তুসমূহ (), চাবি_2=অনামা s_c: script_order[s_c[0]]):
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
