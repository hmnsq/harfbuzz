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

ଆମଦାନୀ collections
ଆମଦାନୀ os
def ଲିଖ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.ଲିଖ (s.encode ('utf-8'))
ଆମଦାନୀ sys

ଯଦି len (sys.argv) > 1 ଏବଂ sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = ସତ୍ୟ
ଅନ୍ୟଥା:
	output_rust = ମିଥ୍ୟା

ଯଦି len (sys.argv) != 3:
	sys.exit (__doc__)

with open (sys.argv[2], encoding='utf-8') ଇଵ f:
	scripts_header = [f.readline () ପାଇଁ i ଭିତରେ range (2)]
	scripts = {}
	script_order = {}
	ପାଇଁ ଧାଡ଼ି ଭିତରେ f:
		j = ଧାଡ଼ି.find ('#')
		ଯଦି j >= 0:
			ଧାଡ଼ି = ଧାଡ଼ି[:j]
		fields = [x.strip () ପାଇଁ x ଭିତରେ ଧାଡ଼ି.split (';')]
		ଯଦି len (fields) == 1:
			ଜାରି
		uu = fields[0].split ('..')
		start = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[0], 16)
		ଯଦି len (uu) == 1:
			end = start
		ଅନ୍ୟଥା:
			end = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[1], 16)
		script = fields[1]
		ପାଇଁ u ଭିତରେ range (start, end + 1):
			scripts[u] = script
		ଯଦି script ନୁହେଁ ଭିତରେ script_order:
			script_order[script] = start

ଶ୍ରେଣୀ ConstraintSet (object):
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
		ଯଦି ନୁହେଁ constraint:
			ଫେରନ୍ତୁ
		first = constraint[0]
		rest = constraint[1:]
		ଯଦି isinstance (self._c, list):
			ଯଦି constraint == self._c[:len (constraint)]:
				self._c = constraint
			ନଚେତଯଦି self._c != constraint[:len (self._c)]:
				self._c = {self._c[0]: ConstraintSet (self._c[1:])}
		ଯଦି isinstance (self._c, dict):
			ଯଦି first ଭିତରେ self._c:
				self._c[first].add (rest)
			ଅନ୍ୟଥା:
				self._c[first] = ConstraintSet (rest)

	@staticmethod
	def _indent (depth):
		ଫେରନ୍ତୁ ('  ' * depth).replace ('        ', '\t')

	def __str__ (self, index=0, depth=4):
		s = []
		indent = self._indent (depth)
		ଯଦି isinstance (self._c, list):
			ଯଦି len (self._c) == 0:
				ପ୍ରତିଜ୍ଞା index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = true;\n'.format (indent))
			ନଚେତଯଦି len (self._c) == 1:
				ପ୍ରତିଜ୍ଞା index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('{}matched = 0x{:04X}u == buffer->cur ({}).codepoint;\n'.format (indent, next (iter (self._c)), index କିମ୍ବା ''))
			ଅନ୍ୟଥା:
				s.append ('{}if (0x{:04X}u == buffer->cur ({}).codepoint &&\n'.format (indent, self._c[0], index କିମ୍ବା ''))
				ଯଦି index:
					s.append ('{}buffer->idx + {} < count &&\n'.format (self._indent (depth + 2), index + 1))
				ପାଇଁ i, cp ଭିତରେ enumerate (self._c[1:], start=1):
					s.append ('{}0x{:04X}u == buffer->cur ({}).codepoint{}\n'.format (
						self._indent (depth + 2), cp, index + i, ')' ଯଦି i == len (self._c) - 1 ଅନ୍ୟଥା ' &&'))
				s.append ('{}{{\n'.format (indent))
				ପାଇଁ i ଭିତରେ range (index):
					s.append ('{}(void) buffer->next_glyph ();\n'.format (self._indent (depth + 1)))
				s.append ('{}matched = true;\n'.format (self._indent (depth + 1)))
				s.append ('{}}}\n'.format (indent))
		ଅନ୍ୟଥା:
			s.append ('{}switch (buffer->cur ({}).codepoint)\n'.format(indent, index କିମ୍ବା ''))
			s.append ('{}{{\n'.format (indent))
			cases = collections.defaultdict (set)
			ପାଇଁ first, rest ଭିତରେ sorted (self._c.ବସ୍ତୁଗୁଡ଼ିକ ()):
				cases[rest.__str__ (index + 1, depth + 2)].add (first)
			ପାଇଁ body, labels ଭିତରେ sorted (cases.ବସ୍ତୁଗୁଡ଼ିକ (), ଚାବି_2=ଅନାମ b_ls: sorted (b_ls[1])[0]):
				ପାଇଁ i, cp ଭିତରେ enumerate (sorted (labels)):
					ଯଦି i % 4 == 0:
						s.append (self._indent (depth + 1))
					ଅନ୍ୟଥା:
						s.append (' ')
					s.append ('case 0x{:04X}u:{}'.format (cp, '\n' ଯଦି i % 4 == 3 ଅନ୍ୟଥା ''))
				ଯଦି len (labels) % 4 != 0:
					s.append ('\n')
				s.append (body)
				s.append ('{}break;\n'.format (self._indent (depth + 2)))
			s.append ('{}}}\n'.format (indent))
		ଫେରନ୍ତୁ ''.join (s)

	def rust_str (self, index=0):
		s = []
		ଯଦି isinstance (self._c, list):
			ଯଦି len (self._c) == 0:
				ପ୍ରତିଜ୍ଞା index == 2, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = true;\n')
			ନଚେତଯଦି len (self._c) == 1:
				ପ୍ରତିଜ୍ଞା index == 1, 'Cannot use `matched` for this constraint; the general case has not been implemented'
				s.append ('matched = 0x{:04X} == buffer.cur({}).glyph_id;\n'.format (next (iter (self._c)), index))
			ଅନ୍ୟଥା:
				s.append ('if 0x{:04X} == buffer.cur({}).glyph_id &&\n'.format (self._c[0], index))
				ଯଦି index:
					s.append ('buffer.idx + {} < buffer.len &&\n'.format (index + 1))
				ପାଇଁ i, cp ଭିତରେ enumerate (self._c[1:], start=1):
					s.append ('0x{:04X} == buffer.cur({}).glyph_id{}\n'.format (
						cp, index + i, '' ଯଦି i == len (self._c) - 1 ଅନ୍ୟଥା ' &&'))
				s.append ('{\n')
				ପାଇଁ i ଭିତରେ range (index + 1):
					s.append ('buffer.next_glyph();\n')
				s.append ('output_dotted_circle(buffer);\n')
				s.append ('}\n')
		ଅନ୍ୟଥା:
			s.append ('match buffer.cur({}).glyph_id {{\n'.format (index))
			cases = collections.defaultdict (set)
			ପାଇଁ first, rest ଭିତରେ sorted (self._c.ବସ୍ତୁଗୁଡ଼ିକ ()):
				cases[rest.rust_str (index + 1)].add (first)
			ବସ୍ତୁଗୁଡ଼ିକ = sorted (cases.ବସ୍ତୁଗୁଡ଼ିକ (), ଚାବି_2=ଅନାମ b_ls: sorted (b_ls[1])[0])
			ପାଇଁ ii, (body, labels) ଭିତରେ enumerate (ବସ୍ତୁଗୁଡ଼ିକ):
				ପାଇଁ i, cp ଭିତରେ enumerate (sorted (labels)):
					ଯଦି i == len (labels) - 1:
						s.append (' 0x{:04X} => {{ {}'.format (cp, '\n' ଯଦି i % 4 == 3 ଅନ୍ୟଥା ''))
					ଅନ୍ୟଥା:
						s.append (' 0x{:04X} | {}'.format (cp, '\n' ଯଦି i % 4 == 3 ଅନ୍ୟଥା ''))
				s.append (body)
				s.append ('}')
				ଯଦି ii == len (ବସ୍ତୁଗୁଡ଼ିକ) - 1:
					s.append ('_ => {}')
			s.append ('}\n')
		ଫେରନ୍ତୁ ''.join (s)

constraints = {}
with open (sys.argv[1], encoding='utf-8') ଇଵ f:
	constraints_header = []
	ଯେତେବେଳେ ସତ୍ୟ:
		ଧାଡ଼ି = f.readline ().strip ()
		ଯଦି ଧାଡ଼ି == '#':
			ଭାଙ୍ଗ
		constraints_header.append(ଧାଡ଼ି)
	ପାଇଁ ଧାଡ଼ି ଭିତରେ f:
		j = ଧାଡ଼ି.find ('#')
		ଯଦି j >= 0:
			ଧାଡ଼ି = ଧାଡ଼ି[:j]
		constraint = [ପୂର୍ଣ୍ଣସଂଖ୍ୟା (cp, 16) ପାଇଁ cp ଭିତରେ ଧାଡ଼ି.split (';')[0].split ()]
		ଯଦି ନୁହେଁ constraint: ଜାରି
		ପ୍ରତିଜ୍ଞା 2 <= len (constraint), 'Prohibited sequence is too short: {}'.format (constraint)
		script = scripts[constraint[0]]
		ଯଦି script ଭିତରେ constraints:
			constraints[script].add (constraint)
		ଅନ୍ୟଥା:
			constraints[script] = ConstraintSet (constraint)
		ପ୍ରତିଜ୍ଞା constraints, 'No constraints found'

print ('/* == Start of generated functions == */')
ଯଦି output_rust:
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

	ପାଇଁ script, constraints ଭିତରେ sorted (constraints.ବସ୍ତୁଗୁଡ଼ିକ (), ଚାବି_2=ଅନାମ s_c: script_order[s_c[0]]):
		print ('        Some(script::{}) => {{'.format (script.upper ()))
		print ('            buffer.idx = 0;')
		print ('            while buffer.idx + 1 < buffer.len {')
		print ('                #[allow(unused_mut)]')
		print ('                let mut matched = false;')
		ଲିଖ (constraints.rust_str ())
		print ('                buffer.next_glyph();')
		print ('                if matched { output_with_dotted_circle(buffer); }')
		print ('            }')
		print ('        }')
		print ()

	print ('        _ => {}')
	print ('    }')
	print ('    buffer.sync();')
	print ('}')
ଅନ୍ୟଥା:
	print ('/*')
	print (' * The following functions are generated by running:')
	print (' *')
	print (' *   %s ms-use/IndicShapingInvalidCluster.txt Scripts.txt' % os.path.basename (sys.argv[0]))
	print (' *')
	print (' * on files with these headers:')
	print (' *')
	ପାଇଁ ଧାଡ଼ି ଭିତରେ constraints_header:
		print (' * %s' % ଧାଡ଼ି.strip ())
	print (' *')
	ପାଇଁ ଧାଡ଼ି ଭିତରେ scripts_header:
		print (' * %s' % ଧାଡ଼ି.strip ())
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

	ପାଇଁ script, constraints ଭିତରେ sorted (constraints.ବସ୍ତୁଗୁଡ଼ିକ (), ଚାବି_2=ଅନାମ s_c: script_order[s_c[0]]):
		print ('    case HB_SCRIPT_{}:'.format (script.upper ()))
		print ('      for (buffer->idx = 0; buffer->idx + 1 < count && buffer->successful;)')
		print ('      {')
		print ('\tbool matched = false;')
		ଲିଖ (str (constraints))
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
