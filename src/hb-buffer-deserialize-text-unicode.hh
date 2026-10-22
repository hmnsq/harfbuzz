
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

#অন্তর্ভাৱ "hb.hh"


#line 33 "hb-buffer-deserialize-text-unicode.hh"
স্থির ধ্রুৱ unsigned ৱর্ণ deserialize_text_unicode_trans_চাবিগুলি[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

স্থির ধ্রুৱ ৱর্ণ deserialize_text_unicode_চাবি_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

স্থির ধ্রুৱ short _deserialize_text_unicode_index_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

স্থির ধ্রুৱ ৱর্ণ _deserialize_text_unicode_indicies[] = {
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

স্থির ধ্রুৱ ৱর্ণ _deserialize_text_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

স্থির ধ্রুৱ ৱর্ণ _deserialize_text_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

স্থির ধ্রুৱ ৱর্ণ _deserialize_text_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

স্থির ধ্রুৱ পূর্ণাঙ্ক deserialize_text_unicode_start = 7;
স্থির ধ্রুৱ পূর্ণাঙ্ক deserialize_text_unicode_first_final = 7;
স্থির ধ্রুৱ পূর্ণাঙ্ক deserialize_text_unicode_error = 0;

স্থির ধ্রুৱ পূর্ণাঙ্ক deserialize_text_unicode_en_main = 7;


#line 80 "hb-buffer-deserialize-text-unicode.rl"


স্থির hb_bool_t
_hb_buffer_deserialize_text_unicode (hb_buffer_t *buffer,
				     ধ্রুৱ ৱর্ণ *buf,
				     unsigned পূর্ণাঙ্ক buf_len,
				     ধ্রুৱ ৱর্ণ **end_ptr,
				     hb_font_t *font)
{
  ধ্রুৱ ৱর্ণ *p = buf, *pe = buf + buf_len, *eof = pe;

  ধ্রুৱ ৱর্ণ *tok = nullptr;
  পূর্ণাঙ্ক cs;
  hb_glyph_info_t info = {0};
  ধ্রুৱ hb_glyph_position_t pos = {0};
  
#line 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_text_unicode_start;
	}

#line 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	পূর্ণাঙ্ক _slen;
	পূর্ণাঙ্ক _trans;
	ধ্রুৱ unsigned ৱর্ণ *চাবিগুলি_2;
	ধ্রুৱ ৱর্ণ *_inds;
	যদি ( p == pe )
		গচ্ছ _test_eof;
	যদি ( cs == 0 )
		গচ্ছ _out;
_resume:
	চাবিগুলি_2 = deserialize_text_unicode_trans_চাবিগুলি + (cs<<1);
	_inds = _deserialize_text_unicode_indicies + _deserialize_text_unicode_index_offsets[cs];

	_slen = deserialize_text_unicode_চাবি_spans[cs];
	_trans = _inds[ _slen > 0 && চাবিগুলি_2[0] <=(*p) &&
		(*p) <= চাবিগুলি_2[1] ?
		(*p) - চাবিগুলি_2[0] : _slen ];

	cs = _deserialize_text_unicode_trans_targs[_trans];

	যদি ( _deserialize_text_unicode_trans_actions[_trans] == 0 )
		গচ্ছ _again;

	ৱিকল্প ( _deserialize_text_unicode_trans_actions[_trans] ) {
	পক্ষ 4:
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, পরিমাণ (info));
}
	ভঙ্গ;
	পক্ষ 1:
#line 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	ভঙ্গ;
	পক্ষ 2:
#line 55 "hb-buffer-deserialize-text-unicode.rl"
	{যদি (!parse_hex (tok, p, &info.codepoint )) ঘূৰাই মিছা; }
	ভঙ্গ;
	পক্ষ 3:
#line 57 "hb-buffer-deserialize-text-unicode.rl"
	{ যদি (!parse_uint (tok, p, &info.cluster )) ঘূৰাই মিছা; }
	ভঙ্গ;
	পক্ষ 6:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	যদি (unlikely (!buffer->successful))
	  ঘূৰাই মিছা;
	যদি (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, পরিমাণ (info));
}
	ভঙ্গ;
#line 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	যদি ( cs == 0 )
		গচ্ছ _out;
	যদি ( ++p != pe )
		গচ্ছ _resume;
	_test_eof: {}
	যদি ( p == eof )
	{
	ৱিকল্প ( _deserialize_text_unicode_eof_actions[cs] ) {
	পক্ষ 5:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	যদি (unlikely (!buffer->successful))
	  ঘূৰাই মিছা;
	যদি (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
	ভঙ্গ;
#line 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	_out: {}
	}

#line 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  ঘূৰাই p == pe;
}

#endif /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
