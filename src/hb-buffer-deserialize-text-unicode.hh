
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

#അന്തര്ഭാവ "hb.hh"


#line 33 "hb-buffer-deserialize-text-unicode.hh"
സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം deserialize_text_unicode_trans_താക്കോലുകൾ[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

സ്ഥിതം സ്ഥിരം അക്ഷരം deserialize_text_unicode_താക്കോൽ_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

സ്ഥിതം സ്ഥിരം short _deserialize_text_unicode_index_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

സ്ഥിതം സ്ഥിരം അക്ഷരം _deserialize_text_unicode_indicies[] = {
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

സ്ഥിതം സ്ഥിരം അക്ഷരം _deserialize_text_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

സ്ഥിതം സ്ഥിരം അക്ഷരം _deserialize_text_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

സ്ഥിതം സ്ഥിരം അക്ഷരം _deserialize_text_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ deserialize_text_unicode_start = 7;
സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ deserialize_text_unicode_first_final = 7;
സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ deserialize_text_unicode_error = 0;

സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ deserialize_text_unicode_en_main = 7;


#line 80 "hb-buffer-deserialize-text-unicode.rl"


സ്ഥിതം hb_bool_t
_hb_buffer_deserialize_text_unicode (hb_buffer_t *buffer,
				     സ്ഥിരം അക്ഷരം *buf,
				     unsigned പൂർണ്ണസംഖ്യ buf_len,
				     സ്ഥിരം അക്ഷരം **end_ptr,
				     hb_font_t *font)
{
  സ്ഥിരം അക്ഷരം *p = buf, *pe = buf + buf_len, *eof = pe;

  സ്ഥിരം അക്ഷരം *tok = nullptr;
  പൂർണ്ണസംഖ്യ cs;
  hb_glyph_info_t info = {0};
  സ്ഥിരം hb_glyph_position_t pos = {0};
  
#line 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_text_unicode_start;
	}

#line 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	പൂർണ്ണസംഖ്യ _slen;
	പൂർണ്ണസംഖ്യ _trans;
	സ്ഥിരം unsigned അക്ഷരം *താക്കോലുകൾ_2;
	സ്ഥിരം അക്ഷരം *_inds;
	എങ്കിൽ ( p == pe )
		പോകുക _test_eof;
	എങ്കിൽ ( cs == 0 )
		പോകുക _out;
_resume:
	താക്കോലുകൾ_2 = deserialize_text_unicode_trans_താക്കോലുകൾ + (cs<<1);
	_inds = _deserialize_text_unicode_indicies + _deserialize_text_unicode_index_offsets[cs];

	_slen = deserialize_text_unicode_താക്കോൽ_spans[cs];
	_trans = _inds[ _slen > 0 && താക്കോലുകൾ_2[0] <=(*p) &&
		(*p) <= താക്കോലുകൾ_2[1] ?
		(*p) - താക്കോലുകൾ_2[0] : _slen ];

	cs = _deserialize_text_unicode_trans_targs[_trans];

	എങ്കിൽ ( _deserialize_text_unicode_trans_actions[_trans] == 0 )
		പോകുക _again;

	തിരഞ്ഞെടുപ്പ് ( _deserialize_text_unicode_trans_actions[_trans] ) {
	അവസ്ഥ 4:
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, വലുപ്പം (info));
}
	നിർത്തുക;
	അവസ്ഥ 1:
#line 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	നിർത്തുക;
	അവസ്ഥ 2:
#line 55 "hb-buffer-deserialize-text-unicode.rl"
	{എങ്കിൽ (!parse_hex (tok, p, &info.codepoint )) തിരികെ അസത്യം; }
	നിർത്തുക;
	അവസ്ഥ 3:
#line 57 "hb-buffer-deserialize-text-unicode.rl"
	{ എങ്കിൽ (!parse_uint (tok, p, &info.cluster )) തിരികെ അസത്യം; }
	നിർത്തുക;
	അവസ്ഥ 6:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	എങ്കിൽ (unlikely (!buffer->successful))
	  തിരികെ അസത്യം;
	എങ്കിൽ (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
#line 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, വലുപ്പം (info));
}
	നിർത്തുക;
#line 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	എങ്കിൽ ( cs == 0 )
		പോകുക _out;
	എങ്കിൽ ( ++p != pe )
		പോകുക _resume;
	_test_eof: {}
	എങ്കിൽ ( p == eof )
	{
	തിരഞ്ഞെടുപ്പ് ( _deserialize_text_unicode_eof_actions[cs] ) {
	അവസ്ഥ 5:
#line 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	buffer->add_info (info);
	എങ്കിൽ (unlikely (!buffer->successful))
	  തിരികെ അസത്യം;
	എങ്കിൽ (buffer->have_positions)
	  buffer->pos[buffer->len - 1] = pos;
	*end_ptr = p;
}
	നിർത്തുക;
#line 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	_out: {}
	}

#line 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  തിരികെ p == pe;
}

#endif /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
