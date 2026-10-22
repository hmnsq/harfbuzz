
#line 1 "hb-buffer-deserialize-text-unicode.rl"
/*
 * Copyright © 2013  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH
#define HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"


#line 33 "hb-buffer-deserialize-text-unicode.hh"
ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର deserialize_text_unicode_trans_ଚାବିଗୁଡ଼ିକ[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର deserialize_text_unicode_ଚାବି_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

ସ୍ଥିର ଅଚଳ short _deserialize_text_unicode_index_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର _deserialize_text_unicode_indicies[] = {
	0, 1, 1, 1, 1, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 2, 2, 2, 2, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 2, 2, 2, 2, 1, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 2, 2, 2, 2, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 2, 2, 2, 2, 1, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	1, 1, 1, 4, 5, 1, 1, 3, 
	3, 3, 3, 3, 3, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 3, 3, 3, 3, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 5, 1, 6, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 1, 8, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 8, 1, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 1, 1, 1, 1, 8, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 8, 1, 10, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 11, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 11, 1, 
	11, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	11, 1, 12, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 12, 1, 0
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର _deserialize_text_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର _deserialize_text_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର _deserialize_text_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deserialize_text_unicode_start = 7;
ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deserialize_text_unicode_first_final = 7;
ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deserialize_text_unicode_error = 0;

ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deserialize_text_unicode_en_main = 7;


#line 80 "hb-buffer-deserialize-text-unicode.rl"


ସ୍ଥିର hb_bool_t
_hb_buffer_deserialize_text_unicode (hb_buffer_t *buffer,
				     ଅଚଳ ଅକ୍ଷର *buf,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_len,
				     ଅଚଳ ଅକ୍ଷର **end_ptr,
				     hb_font_t *font)
{
  ଅଚଳ ଅକ୍ଷର *p = buf, *pe = buf + buf_len, *eof = pe;

  ଅଚଳ ଅକ୍ଷର *tok = nullptr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cs;
  hb_glyph_info_t info = {0};
  ଅଚଳ hb_glyph_position_t pos = {0};
  
#line 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_text_unicode_start;
	}

#line 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା _slen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା _trans;
	ଅଚଳ unsigned ଅକ୍ଷର *ଚାବିଗୁଡ଼ିକ_2;
	ଅଚଳ ଅକ୍ଷର *_inds;
	ଯଦି ( p == pe )
		ଯାଅ _test_eof;
	ଯଦି ( cs == 0 )
		ଯାଅ _out;
_resume:
	ଚାବିଗୁଡ଼ିକ_2 = deserialize_text_unicode_trans_ଚାବିଗୁଡ଼ିକ + (cs<<1);
	_inds = _deserialize_text_unicode_indicies + _deserialize_text_unicode_index_offsets[cs];

	_slen = deserialize_text_unicode_ଚାବି_spans[cs];
	_trans = _inds[ _slen > 0 && ଚାବିଗୁଡ଼ିକ_2[0] <=(*p) &&
		(*p) <= ଚାବିଗୁଡ଼ିକ_2[1] ?
		(*p) - ଚାବିଗୁଡ଼ିକ_2[0] : _slen ];

	cs = _deserialize_text_unicode_trans_targs[_trans];

	ଯଦି ( _deserialize_text_unicode_trans_actions[_trans] == 0 )
		ଯାଅ _again;

	ବାଛ ( _deserialize_text_unicode_trans_actions[_trans] ) {
	ସ୍ଥିତି 4:
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, ଆକାର (info));
}
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 1:
#line 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 2:
#line 55 "hb-buffer-deserialize-text-unicode.rl"
	{ଯଦି (!parse_hex (tok, p, &info.codepoint )) ଫେରନ୍ତୁ ମିଥ୍ୟା; }
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 3:
#line 57 "hb-buffer-deserialize-text-unicode.rl"
	{ ଯଦି (!parse_uint (tok, p, &info.cluster )) ଫେରନ୍ତୁ ମିଥ୍ୟା; }
	ଭାଙ୍ଗ;
	ସ୍ଥିତି 6:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	ଯଦି (unlikely (!buffer->successful))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଯଦି (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, ଆକାର (info));
}
	ଭାଙ୍ଗ;
#line 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	ଯଦି ( cs == 0 )
		ଯାଅ _out;
	ଯଦି ( ++p != pe )
		ଯାଅ _resume;
	_test_eof: {}
	ଯଦି ( p == eof )
	{
	ବାଛ ( _deserialize_text_unicode_eof_actions[cs] ) {
	ସ୍ଥିତି 5:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	ଯଦି (unlikely (!buffer->successful))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଯଦି (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
	ଭାଙ୍ଗ;
#line 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	_out: {}
	}

#line 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  ଫେରନ୍ତୁ p == pe;
}

#endif /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
