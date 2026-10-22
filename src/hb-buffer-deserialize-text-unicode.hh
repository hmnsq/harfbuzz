
#पंक्ति 1 "hb-buffer-deserialize-text-unicode.rl"
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

#अगर_अघोषित HB_बफर_DESERIALIZE_TEXT_UNICODE_HH
#घोषणा HB_बफर_DESERIALIZE_TEXT_UNICODE_HH

#समावेश "hb.hh"


#पंक्ति 33 "hb-buffer-deserialize-text-unicode.hh"
अटल स्थिर अचिन्हित अक्षर deserialize_text_unicode_trans_कुंजियाँ[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

अटल स्थिर अक्षर deserialize_text_unicode_कुंजी_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

अटल स्थिर लघु deserialize_text_unicode_सूचकांक_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

अटल स्थिर अक्षर _deserialize_text_unicode_indicies[] = {
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

अटल स्थिर अक्षर _deserialize_text_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

अटल स्थिर अक्षर _deserialize_text_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

अटल स्थिर अक्षर _deserialize_text_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

अटल स्थिर पूर्णांक deserialize_text_unicode_start = 7;
अटल स्थिर पूर्णांक deserialize_text_unicode_first_final = 7;
अटल स्थिर पूर्णांक deserialize_text_unicode_error = 0;

अटल स्थिर पूर्णांक deserialize_text_unicode_en_main = 7;


#पंक्ति 80 "hb-buffer-deserialize-text-unicode.rl"


अटल hb_bool_t
hb_बफर_deserialize_text_unicode (hb_बफर_t *बफर,
				     स्थिर अक्षर *buf,
				     अचिन्हित पूर्णांक buf_len,
				     स्थिर अक्षर **end_ptr,
				     hb_font_t *font)
{
  स्थिर अक्षर *p = buf, *pe = buf + buf_len, *खप = pe;

  स्थिर अक्षर *tok = nullptr;
  पूर्णांक cs;
  hb_glyph_info_t info = {0};
  स्थिर hb_glyph_position_t pos = {0};
  
#पंक्ति 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_text_unicode_start;
	}

#पंक्ति 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	पूर्णांक _slen;
	पूर्णांक _trans;
	स्थिर अचिन्हित अक्षर *कुंजियाँ_2;
	स्थिर अक्षर *_inds;
	अगर ( p == pe )
		जाओ _test_eof;
	अगर ( cs == 0 )
		जाओ _out;
_resume:
	कुंजियाँ_2 = deserialize_text_unicode_trans_कुंजियाँ + (cs<<1);
	_inds = _deserialize_text_unicode_indicies + deserialize_text_unicode_सूचकांक_offsets[cs];

	_slen = deserialize_text_unicode_कुंजी_spans[cs];
	_trans = _inds[ _slen > 0 && कुंजियाँ_2[0] <=(*p) &&
		(*p) <= कुंजियाँ_2[1] ?
		(*p) - कुंजियाँ_2[0] : _slen ];

	cs = _deserialize_text_unicode_trans_targs[_trans];

	अगर ( _deserialize_text_unicode_trans_actions[_trans] == 0 )
		जाओ _again;

	चयन ( _deserialize_text_unicode_trans_actions[_trans] ) {
	हाल 4:
#पंक्ति 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, माप (info));
}
	अवरोध;
	हाल 1:
#पंक्ति 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	अवरोध;
	हाल 2:
#पंक्ति 55 "hb-buffer-deserialize-text-unicode.rl"
	{अगर (!parse_hex (tok, p, &info.codepoint )) वापस false; }
	अवरोध;
	हाल 3:
#पंक्ति 57 "hb-buffer-deserialize-text-unicode.rl"
	{ अगर (!parse_uint (tok, p, &info.cluster )) वापस false; }
	अवरोध;
	हाल 6:
#पंक्ति 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	बफर->add_info (info);
	अगर (unlikely (!बफर->successful))
	  वापस false;
	अगर (बफर->have_positions)
	  बफर->pos[बफर->len - 1] = pos;
	*end_ptr = p;
}
#पंक्ति 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, माप (info));
}
	अवरोध;
#पंक्ति 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	अगर ( cs == 0 )
		जाओ _out;
	अगर ( ++p != pe )
		जाओ _resume;
	_test_eof: {}
	अगर ( p == खप )
	{
	चयन ( _deserialize_text_unicode_eof_actions[cs] ) {
	हाल 5:
#पंक्ति 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	बफर->add_info (info);
	अगर (unlikely (!बफर->successful))
	  वापस false;
	अगर (बफर->have_positions)
	  बफर->pos[बफर->len - 1] = pos;
	*end_ptr = p;
}
	अवरोध;
#पंक्ति 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	_out: {}
	}

#पंक्ति 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  वापस p == pe;
}

#पूर्ण_अगर /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
