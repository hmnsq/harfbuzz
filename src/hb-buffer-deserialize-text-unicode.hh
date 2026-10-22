
#سطر 1 "hb-buffer-deserialize-text-unicode.rl"
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

#اگر_غیر_تعریف_شدہ HB_بفر_DESERIALIZE_متن_UNICODE_HH
#تعریف_کرو HB_بفر_DESERIALIZE_متن_UNICODE_HH

#شامل "hb.hh"


#سطر 33 "hb-buffer-deserialize-text-unicode.hh"
ساکن ثابت غیرعلامتی حرف deserialize_متن_unicode_trans_کلیدیں[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

ساکن ثابت حرف deserialize_متن_unicode_کلید_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

ساکن ثابت مختصر deserialize_متن_unicode_اشاریہ_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

ساکن ثابت حرف deserialize_متن_unicode_indicies[] = {
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

ساکن ثابت حرف deserialize_متن_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

ساکن ثابت حرف deserialize_متن_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

ساکن ثابت حرف deserialize_متن_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

ساکن ثابت صحیح deserialize_متن_unicode_آغاز = 7;
ساکن ثابت صحیح deserialize_متن_unicode_پہلا_final = 7;
ساکن ثابت صحیح deserialize_متن_unicode_error = 0;

ساکن ثابت صحیح deserialize_متن_unicode_en_main = 7;


#سطر 80 "hb-buffer-deserialize-text-unicode.rl"


ساکن hb_bool_t
hb_بفر_deserialize_متن_unicode (hb_بفر_t *بفر,
				     ثابت حرف *buf,
				     غیرعلامتی صحیح buf_len,
				     ثابت حرف **end_ptr,
				     hb_font_t *font)
{
  ثابت حرف *p = buf, *pe = buf + buf_len, *eof = pe;

  ثابت حرف *tok = خالی_اشاریہ;
  صحیح cs;
  hb_glyph_info_t info = {0};
  ثابت hb_glyph_position_t pos = {0};
  
#سطر 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_متن_unicode_آغاز;
	}

#سطر 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	صحیح _slen;
	صحیح _trans;
	ثابت غیرعلامتی حرف *کلیدیں_2;
	ثابت حرف *_inds;
	اگر ( p == pe )
		جاؤ _test_eof;
	اگر ( cs == 0 )
		جاؤ برآمد_2;
_resume:
	کلیدیں_2 = deserialize_متن_unicode_trans_کلیدیں + (cs<<1);
	_inds = deserialize_متن_unicode_indicies + deserialize_متن_unicode_اشاریہ_offsets[cs];

	_slen = deserialize_متن_unicode_کلید_spans[cs];
	_trans = _inds[ _slen > 0 && کلیدیں_2[0] <=(*p) &&
		(*p) <= کلیدیں_2[1] ?
		(*p) - کلیدیں_2[0] : _slen ];

	cs = deserialize_متن_unicode_trans_targs[_trans];

	اگر ( deserialize_متن_unicode_trans_actions[_trans] == 0 )
		جاؤ _again;

	انتخاب ( deserialize_متن_unicode_trans_actions[_trans] ) {
	حالت 4:
#سطر 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, حجم (info));
}
	توڑو;
	حالت 1:
#سطر 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	توڑو;
	حالت 2:
#سطر 55 "hb-buffer-deserialize-text-unicode.rl"
	{اگر (!parse_hex (tok, p, &info.codepoint )) واپس جھوٹ; }
	توڑو;
	حالت 3:
#سطر 57 "hb-buffer-deserialize-text-unicode.rl"
	{ اگر (!parse_uint (tok, p, &info.cluster )) واپس جھوٹ; }
	توڑو;
	حالت 6:
#سطر 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	بفر->جوڑو_info (info);
	اگر (unlikely (!بفر->successful))
	  واپس جھوٹ;
	اگر (بفر->have_positions)
	  بفر->pos[بفر->len - 1] = pos;
	*end_ptr = p;
}
#سطر 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, حجم (info));
}
	توڑو;
#سطر 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	اگر ( cs == 0 )
		جاؤ برآمد_2;
	اگر ( ++p != pe )
		جاؤ _resume;
	_test_eof: {}
	اگر ( p == eof )
	{
	انتخاب ( deserialize_متن_unicode_eof_actions[cs] ) {
	حالت 5:
#سطر 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	بفر->جوڑو_info (info);
	اگر (unlikely (!بفر->successful))
	  واپس جھوٹ;
	اگر (بفر->have_positions)
	  بفر->pos[بفر->len - 1] = pos;
	*end_ptr = p;
}
	توڑو;
#سطر 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	برآمد_2: {}
	}

#سطر 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  واپس p == pe;
}

#اگر_ختم /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
