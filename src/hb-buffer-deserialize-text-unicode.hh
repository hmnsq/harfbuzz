
#کرښه 1 "hb-buffer-deserialize-text-unicode.rl"
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

#که_نه_تعریف_شوی HB_بفر_DESERIALIZE_متن_UNICODE_HH
#تعریف_کړه HB_بفر_DESERIALIZE_متن_UNICODE_HH

#ضمن "hb.hh"


#کرښه 33 "hb-buffer-deserialize-text-unicode.hh"
ساكن ثابت بینښه حرف deserialize_متن_unicode_trans_کلیګانې[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

ساكن ثابت حرف deserialize_متن_unicode_کلی_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

ساكن ثابت لنډ deserialize_متن_unicode_شاخص_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

ساكن ثابت حرف deserialize_متن_unicode_indicies[] = {
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

ساكن ثابت حرف deserialize_متن_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

ساكن ثابت حرف deserialize_متن_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

ساكن ثابت حرف deserialize_متن_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

ساكن ثابت صحيح deserialize_متن_unicode_پیل = 7;
ساكن ثابت صحيح deserialize_متن_unicode_لومړی_final = 7;
ساكن ثابت صحيح deserialize_متن_unicode_error = 0;

ساكن ثابت صحيح deserialize_متن_unicode_en_main = 7;


#کرښه 80 "hb-buffer-deserialize-text-unicode.rl"


ساكن hb_bool_t
hb_بفر_deserialize_متن_unicode (hb_بفر_t *بفر,
				     ثابت حرف *buf,
				     بینښه صحيح buf_len,
				     ثابت حرف **end_ptr,
				     hb_font_t *font)
{
  ثابت حرف *p = buf, *pe = buf + buf_len, *eof = pe;

  ثابت حرف *tok = تش_اشاره;
  صحيح cs;
  hb_glyph_info_t info = {0};
  ثابت hb_glyph_position_t pos = {0};
  
#کرښه 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_متن_unicode_پیل;
	}

#کرښه 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	صحيح _slen;
	صحيح _trans;
	ثابت بینښه حرف *کلیګانې_2;
	ثابت حرف *_inds;
	إذا ( p == pe )
		اذهب _test_eof;
	إذا ( cs == 0 )
		اذهب بهر_2;
_resume:
	کلیګانې_2 = deserialize_متن_unicode_trans_کلیګانې + (cs<<1);
	_inds = deserialize_متن_unicode_indicies + deserialize_متن_unicode_شاخص_offsets[cs];

	_slen = deserialize_متن_unicode_کلی_spans[cs];
	_trans = _inds[ _slen > 0 && کلیګانې_2[0] <=(*p) &&
		(*p) <= کلیګانې_2[1] ?
		(*p) - کلیګانې_2[0] : _slen ];

	cs = deserialize_متن_unicode_trans_targs[_trans];

	إذا ( deserialize_متن_unicode_trans_actions[_trans] == 0 )
		اذهب _again;

	بدل ( deserialize_متن_unicode_trans_actions[_trans] ) {
	حالة 4:
#کرښه 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, حجم (info));
}
	اكسر;
	حالة 1:
#کرښه 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	اكسر;
	حالة 2:
#کرښه 55 "hb-buffer-deserialize-text-unicode.rl"
	{إذا (!parse_hex (tok, p, &info.codepoint )) أرجع خطأ; }
	اكسر;
	حالة 3:
#کرښه 57 "hb-buffer-deserialize-text-unicode.rl"
	{ إذا (!parse_uint (tok, p, &info.cluster )) أرجع خطأ; }
	اكسر;
	حالة 6:
#کرښه 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	بفر->جمع_کړه_info (info);
	إذا (unlikely (!بفر->successful))
	  أرجع خطأ;
	إذا (بفر->have_positions)
	  بفر->pos[بفر->len - 1] = pos;
	*end_ptr = p;
}
#کرښه 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, حجم (info));
}
	اكسر;
#کرښه 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	إذا ( cs == 0 )
		اذهب بهر_2;
	إذا ( ++p != pe )
		اذهب _resume;
	_test_eof: {}
	إذا ( p == eof )
	{
	بدل ( deserialize_متن_unicode_eof_actions[cs] ) {
	حالة 5:
#کرښه 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	بفر->جمع_کړه_info (info);
	إذا (unlikely (!بفر->successful))
	  أرجع خطأ;
	إذا (بفر->have_positions)
	  بفر->pos[بفر->len - 1] = pos;
	*end_ptr = p;
}
	اكسر;
#کرښه 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	بهر_2: {}
	}

#کرښه 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  أرجع p == pe;
}

#د_که_پای /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
