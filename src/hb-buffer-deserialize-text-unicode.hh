
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

#अन्तर्भाव "hb.hh"


#line 33 "hb-buffer-deserialize-text-unicode.hh"
स्थिर ध्रुव unsigned वर्ण deserialize_text_unicode_trans_किल्ल्या[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

स्थिर ध्रुव वर्ण deserialize_text_unicode_किल्ली_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

स्थिर ध्रुव short _deserialize_text_unicode_index_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

स्थिर ध्रुव वर्ण _deserialize_text_unicode_indicies[] = {
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

स्थिर ध्रुव वर्ण _deserialize_text_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

स्थिर ध्रुव वर्ण _deserialize_text_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

स्थिर ध्रुव वर्ण _deserialize_text_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

स्थिर ध्रुव पूर्णाङ्क deserialize_text_unicode_start = 7;
स्थिर ध्रुव पूर्णाङ्क deserialize_text_unicode_first_final = 7;
स्थिर ध्रुव पूर्णाङ्क deserialize_text_unicode_error = 0;

स्थिर ध्रुव पूर्णाङ्क deserialize_text_unicode_en_main = 7;


#line 80 "hb-buffer-deserialize-text-unicode.rl"


स्थिर hb_bool_t
_hb_buffer_deserialize_text_unicode (hb_buffer_t *buffer,
				     ध्रुव वर्ण *buf,
				     unsigned पूर्णाङ्क buf_len,
				     ध्रुव वर्ण **end_ptr,
				     hb_font_t *font)
{
  ध्रुव वर्ण *p = buf, *pe = buf + buf_len, *eof = pe;

  ध्रुव वर्ण *tok = nullptr;
  पूर्णाङ्क cs;
  hb_glyph_info_t info = {0};
  ध्रुव hb_glyph_position_t pos = {0};
  
#line 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_text_unicode_start;
	}

#line 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	पूर्णाङ्क _slen;
	पूर्णाङ्क _trans;
	ध्रुव unsigned वर्ण *किल्ल्या_2;
	ध्रुव वर्ण *_inds;
	यदि ( p == pe )
		गच्छ _test_eof;
	यदि ( cs == 0 )
		गच्छ _out;
_resume:
	किल्ल्या_2 = deserialize_text_unicode_trans_किल्ल्या + (cs<<1);
	_inds = _deserialize_text_unicode_indicies + _deserialize_text_unicode_index_offsets[cs];

	_slen = deserialize_text_unicode_किल्ली_spans[cs];
	_trans = _inds[ _slen > 0 && किल्ल्या_2[0] <=(*p) &&
		(*p) <= किल्ल्या_2[1] ?
		(*p) - किल्ल्या_2[0] : _slen ];

	cs = _deserialize_text_unicode_trans_targs[_trans];

	यदि ( _deserialize_text_unicode_trans_actions[_trans] == 0 )
		गच्छ _again;

	विकल्प ( _deserialize_text_unicode_trans_actions[_trans] ) {
	पक्ष 4:
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, परिमाण (info));
}
	भङ्ग;
	पक्ष 1:
#line 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	भङ्ग;
	पक्ष 2:
#line 55 "hb-buffer-deserialize-text-unicode.rl"
	{यदि (!parse_hex (tok, p, &info.codepoint )) प्रत्यागम असत्य; }
	भङ्ग;
	पक्ष 3:
#line 57 "hb-buffer-deserialize-text-unicode.rl"
	{ यदि (!parse_uint (tok, p, &info.cluster )) प्रत्यागम असत्य; }
	भङ्ग;
	पक्ष 6:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	यदि (unlikely (!buffer->successful))
	  प्रत्यागम असत्य;
	यदि (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, परिमाण (info));
}
	भङ्ग;
#line 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	यदि ( cs == 0 )
		गच्छ _out;
	यदि ( ++p != pe )
		गच्छ _resume;
	_test_eof: {}
	यदि ( p == eof )
	{
	विकल्प ( _deserialize_text_unicode_eof_actions[cs] ) {
	पक्ष 5:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	यदि (unlikely (!buffer->successful))
	  प्रत्यागम असत्य;
	यदि (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
	भङ्ग;
#line 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	_out: {}
	}

#line 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  प्रत्यागम p == pe;
}

#endif /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
