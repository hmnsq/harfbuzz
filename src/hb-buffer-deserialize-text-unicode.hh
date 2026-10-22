
#పంక్తి 1 "hb-buffer-deserialize-text-unicode.rl"
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

#నిర్వచితం_కాకపోతే HB_బఫరు_DESERIALIZE_పాఠ్యం_UNICODE_HH
#నిర్వచించు HB_బఫరు_DESERIALIZE_పాఠ్యం_UNICODE_HH

#అన్తర్భావ "hb.hh"


#పంక్తి 33 "hb-buffer-deserialize-text-unicode.hh"
స్థిర స్థిరరాశి గుర్తులేని అక్షరం deserialize_పాఠ్యం_unicode_trans_కీలకాలు[] = {
	0u, 0u, 43u, 102u, 48u, 102u, 48u, 124u, 48u, 57u, 62u, 124u, 48u, 124u, 60u, 117u, 
	85u, 117u, 85u, 117u, 0
};

స్థిర స్థిరరాశి అక్షరం deserialize_పాఠ్యం_unicode_కీలకం_spans[] = {
	0, 60, 55, 77, 10, 63, 77, 58, 
	33, 33
};

స్థిర స్థిరరాశి పొట్టి deserialize_పాఠ్యం_unicode_సూచీ_offsets[] = {
	0, 0, 61, 117, 195, 206, 270, 348, 
	407, 441
};

స్థిర స్థిరరాశి అక్షరం deserialize_పాఠ్యం_unicode_indicies[] = {
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

స్థిర స్థిరరాశి అక్షరం deserialize_పాఠ్యం_unicode_trans_targs[] = {
	2, 0, 3, 3, 4, 9, 5, 6, 
	9, 6, 8, 1, 1
};

స్థిర స్థిరరాశి అక్షరం deserialize_పాఠ్యం_unicode_trans_actions[] = {
	0, 0, 1, 0, 2, 2, 1, 1, 
	3, 0, 0, 4, 6
};

స్థిర స్థిరరాశి అక్షరం deserialize_పాఠ్యం_unicode_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 5
};

స్థిర స్థిరరాశి పూర్ణాంకం deserialize_పాఠ్యం_unicode_ప్రారంభం = 7;
స్థిర స్థిరరాశి పూర్ణాంకం deserialize_పాఠ్యం_unicode_మొదటి_final = 7;
స్థిర స్థిరరాశి పూర్ణాంకం deserialize_పాఠ్యం_unicode_error = 0;

స్థిర స్థిరరాశి పూర్ణాంకం deserialize_పాఠ్యం_unicode_en_main = 7;


#పంక్తి 80 "hb-buffer-deserialize-text-unicode.rl"


స్థిర hb_bool_t
hb_బఫరు_deserialize_పాఠ్యం_unicode (hb_బఫరు_t *బఫరు,
				     స్థిరరాశి అక్షరం *buf,
				     గుర్తులేని పూర్ణాంకం buf_len,
				     స్థిరరాశి అక్షరం **end_ptr,
				     hb_font_t *font)
{
  స్థిరరాశి అక్షరం *p = buf, *pe = buf + buf_len, *eof = pe;

  స్థిరరాశి అక్షరం *tok = శూన్యసూచిక;
  పూర్ణాంకం cs;
  hb_glyph_info_t info = {0};
  స్థిరరాశి hb_glyph_position_t pos = {0};
  
#పంక్తి 147 "hb-buffer-deserialize-text-unicode.hh"
	{
	cs = deserialize_పాఠ్యం_unicode_ప్రారంభం;
	}

#పంక్తి 150 "hb-buffer-deserialize-text-unicode.hh"
	{
	పూర్ణాంకం _slen;
	పూర్ణాంకం _trans;
	స్థిరరాశి గుర్తులేని అక్షరం *కీలకాలు_2;
	స్థిరరాశి అక్షరం *_inds;
	ఒకవేళ ( p == pe )
		వెళ్ళు _test_eof;
	ఒకవేళ ( cs == 0 )
		వెళ్ళు బహిర్గతం_2;
_resume:
	కీలకాలు_2 = deserialize_పాఠ్యం_unicode_trans_కీలకాలు + (cs<<1);
	_inds = deserialize_పాఠ్యం_unicode_indicies + deserialize_పాఠ్యం_unicode_సూచీ_offsets[cs];

	_slen = deserialize_పాఠ్యం_unicode_కీలకం_spans[cs];
	_trans = _inds[ _slen > 0 && కీలకాలు_2[0] <=(*p) &&
		(*p) <= కీలకాలు_2[1] ?
		(*p) - కీలకాలు_2[0] : _slen ];

	cs = deserialize_పాఠ్యం_unicode_trans_targs[_trans];

	ఒకవేళ ( deserialize_పాఠ్యం_unicode_trans_actions[_trans] == 0 )
		వెళ్ళు _again;

	ఎంపిక ( deserialize_పాఠ్యం_unicode_trans_actions[_trans] ) {
	స్థితి 4:
#పంక్తి 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, పరిమాణం (info));
}
	ఆపు;
	స్థితి 1:
#పంక్తి 51 "hb-buffer-deserialize-text-unicode.rl"
	{
	tok = p;
}
	ఆపు;
	స్థితి 2:
#పంక్తి 55 "hb-buffer-deserialize-text-unicode.rl"
	{ఒకవేళ (!parse_hex (tok, p, &info.codepoint )) తిరిగి అబద్ధం; }
	ఆపు;
	స్థితి 3:
#పంక్తి 57 "hb-buffer-deserialize-text-unicode.rl"
	{ ఒకవేళ (!parse_uint (tok, p, &info.cluster )) తిరిగి అబద్ధం; }
	ఆపు;
	స్థితి 6:
#పంక్తి 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	బఫరు->కూడు_info (info);
	ఒకవేళ (unlikely (!బఫరు->successful))
	  తిరిగి అబద్ధం;
	ఒకవేళ (బఫరు->have_positions)
	  బఫరు->pos[బఫరు->len - 1] = pos;
	*end_ptr = p;
}
#పంక్తి 38 "hb-buffer-deserialize-text-unicode.rl"
	{
	hb_memset (&info, 0, పరిమాణం (info));
}
	ఆపు;
#పంక్తి 203 "hb-buffer-deserialize-text-unicode.hh"
	}

_again:
	ఒకవేళ ( cs == 0 )
		వెళ్ళు బహిర్గతం_2;
	ఒకవేళ ( ++p != pe )
		వెళ్ళు _resume;
	_test_eof: {}
	ఒకవేళ ( p == eof )
	{
	ఎంపిక ( deserialize_పాఠ్యం_unicode_eof_actions[cs] ) {
	స్థితి 5:
#పంక్తి 42 "hb-buffer-deserialize-text-unicode.rl"
	{
	బఫరు->కూడు_info (info);
	ఒకవేళ (unlikely (!బఫరు->successful))
	  తిరిగి అబద్ధం;
	ఒకవేళ (బఫరు->have_positions)
	  బఫరు->pos[బఫరు->len - 1] = pos;
	*end_ptr = p;
}
	ఆపు;
#పంక్తి 224 "hb-buffer-deserialize-text-unicode.hh"
	}
	}

	బహిర్గతం_2: {}
	}

#పంక్తి 98 "hb-buffer-deserialize-text-unicode.rl"


  *end_ptr = p;

  తిరిగి p == pe;
}

#అయితే_ముగింపు /* HB_BUFFER_DESERIALIZE_TEXT_UNICODE_HH */
