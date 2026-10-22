
#ਸਤਰ 1 "hb-buffer-deserialize-text-unicode.rl"
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_DESERIALIZE_ਪਾਠ_UNICODE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_DESERIALIZE_ਪਾਠ_UNICODE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


#ਸਤਰ 33 "hb-buffer-deserialize-text-unicode.hh"
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ deserialize_ਪਾਠ_unicode_trans_ਕੁੰਜੀਆਂ[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ deserialize_ਪਾਠ_unicode_ਕੁੰਜੀ_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

ਸਥਿਰ ਅਚਲ ਛੋਟਾ deserialize_ਪਾਠ_unicode_ਸੂਚਕਾਂਕ_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ deserialize_ਪਾਠ_unicode_indicies[] = {
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

ਸਥਿਰ ਅਚਲ ਅੱਖਰ deserialize_ਪਾਠ_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ deserialize_ਪਾਠ_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ deserialize_ਪਾਠ_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

ਸਥਿਰ ਅਚਲ ਪੂਰਨ deserialize_ਪਾਠ_unicode_ਸ਼ੁਰੂ = 7;
ਸਥਿਰ ਅਚਲ ਪੂਰਨ deserialize_ਪਾਠ_unicode_ਪਹਿਲਾ_final = 7;
ਸਥਿਰ ਅਚਲ ਪੂਰਨ deserialize_ਪਾਠ_unicode_error = 0;

ਸਥਿਰ ਅਚਲ ਪੂਰਨ deserialize_ਪਾਠ_unicode_en_main = 7;


#ਸਤਰ 80 "hb-buffer-deserialize-text-unicode.rl"


ਸਥਿਰ hb_bool_t
hb_ਬਫਰ_deserialize_ਪਾਠ_unicode (hb_ਬਫਰ_t *ਬਫਰ,
				     ਅਚਲ ਅੱਖਰ *buf,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_len,
				     ਅਚਲ ਅੱਖਰ **end_ptr,
				     hb_font_t *font)
{
  ਅਚਲ ਅੱਖਰ *p = buf, *pe = buf + buf_len, *eof = pe;

  ਅਚਲ ਅੱਖਰ *tok = ਖਾਲੀ_ਸੂਚਕ;
  ਪੂਰਨ cs;
  hb_glyph_info_t info = {0};
  ਅਚਲ hb_glyph_position_t pos = {0};
  
#ਸਤਰ 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_ਪਾਠ_unicode_ਸ਼ੁਰੂ;
	}

#ਸਤਰ 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	ਪੂਰਨ _slen;
	ਪੂਰਨ _trans;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਕੁੰਜੀਆਂ_2;
	ਅਚਲ ਅੱਖਰ *_inds;
	ਜੇ ( p == pe )
		ਜਾਓ _test_eof;
	ਜੇ ( cs == 0 )
		ਜਾਓ ਬਾਹਰਲਾ_2;
_resume:
	ਕੁੰਜੀਆਂ_2 = deserialize_ਪਾਠ_unicode_trans_ਕੁੰਜੀਆਂ + (cs<<1);
	_inds = deserialize_ਪਾਠ_unicode_indicies + deserialize_ਪਾਠ_unicode_ਸੂਚਕਾਂਕ_offsets[cs];

	_slen = deserialize_ਪਾਠ_unicode_ਕੁੰਜੀ_spans[cs];
	_trans = _inds[ _slen > 0 && ਕੁੰਜੀਆਂ_2[0] <=(*p) &&
		(*p) <= ਕੁੰਜੀਆਂ_2[1] ?
		(*p) - ਕੁੰਜੀਆਂ_2[0] : _slen ];

	cs = deserialize_ਪਾਠ_unicode_trans_targs[_trans];

	ਜੇ ( deserialize_ਪਾਠ_unicode_trans_actions[_trans] == 0 )
		ਜਾਓ _again;

	ਚੋਣ ( deserialize_ਪਾਠ_unicode_trans_actions[_trans] ) {
	ਹਾਲਤ 4:
#ਸਤਰ 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, ਆਕਾਰ (info));
}
	ਰੋਕੋ;
	ਹਾਲਤ 1:
#ਸਤਰ 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	ਰੋਕੋ;
	ਹਾਲਤ 2:
#ਸਤਰ 55 "hb-buffer-deserialize-text-unicode.rl"
	{ਜੇ (!parse_hex (tok, p, &info.codepoint )) ਵਾਪਸ ਝੂਠ; }
	ਰੋਕੋ;
	ਹਾਲਤ 3:
#ਸਤਰ 57 "hb-buffer-deserialize-text-unicode.rl"
	{ ਜੇ (!parse_uint (tok, p, &info.cluster )) ਵਾਪਸ ਝੂਠ; }
	ਰੋਕੋ;
	ਹਾਲਤ 6:
#ਸਤਰ 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	ਬਫਰ->ਜੋੜੋ_info (info);
	ਜੇ (unlikely (!ਬਫਰ->successful))
	  ਵਾਪਸ ਝੂਠ;
	ਜੇ (ਬਫਰ->have_positions)
	  ਬਫਰ->pos[ਬਫਰ->len - 1] = pos;
	*end_ptr = p;
}
#ਸਤਰ 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, ਆਕਾਰ (info));
}
	ਰੋਕੋ;
#ਸਤਰ 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	ਜੇ ( cs == 0 )
		ਜਾਓ ਬਾਹਰਲਾ_2;
	ਜੇ ( ++p != pe )
		ਜਾਓ _resume;
	_test_eof: {}
	ਜੇ ( p == eof )
	{
	ਚੋਣ ( deserialize_ਪਾਠ_unicode_eof_actions[cs] ) {
	ਹਾਲਤ 5:
#ਸਤਰ 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	ਬਫਰ->ਜੋੜੋ_info (info);
	ਜੇ (unlikely (!ਬਫਰ->successful))
	  ਵਾਪਸ ਝੂਠ;
	ਜੇ (ਬਫਰ->have_positions)
	  ਬਫਰ->pos[ਬਫਰ->len - 1] = pos;
	*end_ptr = p;
}
	ਰੋਕੋ;
#ਸਤਰ 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	ਬਾਹਰਲਾ_2: {}
	}

#ਸਤਰ 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  ਵਾਪਸ p == pe;
}

#ਜੇ_ਅੰਤ /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
