
#line 1 "hb-ot-shaper-khmer-machine.rl"
/*
 * Copyright © 2011,2012  Google, Inc.
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

#ifndef HB_OT_SHAPER_KHMER_MACHINE_HH
#define HB_OT_SHAPER_KHMER_MACHINE_HH

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-ot-layout.hh"
#অন্তর্ভাব "hb-ot-shaper-indic.hh"

/* buffer var allocations */
#define khmer_category() ot_shaper_var_u8_category() /* khmer_category_t */

using khmer_category_t = unsigned;

#define K_Cat(Cat) khmer_syllable_machine_ex_##Cat

গণনা khmer_syllable_type_t {
  khmer_consonant_syllable,
  khmer_broken_cluster,
  khmer_non_khmer_cluster,
};


#line 49 "hb-ot-shaper-khmer-machine.hh"
#define khmer_syllable_machine_ex_C 1u
#define khmer_syllable_machine_ex_DOTTEDCIRCLE 11u
#define khmer_syllable_machine_ex_H 4u
#define khmer_syllable_machine_ex_PLACEHOLDER 10u
#define khmer_syllable_machine_ex_Ra 15u
#define khmer_syllable_machine_ex_Robatic 25u
#define khmer_syllable_machine_ex_V 2u
#define khmer_syllable_machine_ex_VAbv 20u
#define khmer_syllable_machine_ex_VBlw 21u
#define khmer_syllable_machine_ex_VPre 22u
#define khmer_syllable_machine_ex_VPst 23u
#define khmer_syllable_machine_ex_Xgroup 26u
#define khmer_syllable_machine_ex_Ygroup 27u
#define khmer_syllable_machine_ex_ZWJ 6u
#define khmer_syllable_machine_ex_ZWNJ 5u


#line 65 "hb-ot-shaper-khmer-machine.hh"
স্থির ধ্রুবক unsigned অক্ষর khmer_syllable_machine_trans_চাবিগুলি[] = {
	5u, 26u, 5u, 26u, 1u, 15u, 5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 
	5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 1u, 15u, 5u, 26u, 5u, 26u, 
	5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 5u, 26u, 1u, 27u, 4u, 27u, 1u, 15u, 
	4u, 27u, 4u, 27u, 27u, 27u, 4u, 27u, 4u, 27u, 4u, 27u, 4u, 27u, 4u, 27u, 
	4u, 27u, 1u, 15u, 4u, 27u, 4u, 27u, 27u, 27u, 4u, 27u, 4u, 27u, 4u, 27u, 
	4u, 27u, 4u, 27u, 5u, 26u, 0
};

স্থির ধ্রুবক অক্ষর khmer_syllable_machine_চাবি_spans[] = {
	22, 22, 15, 22, 22, 22, 22, 22, 
	22, 22, 22, 22, 22, 15, 22, 22, 
	22, 22, 22, 22, 22, 27, 24, 15, 
	24, 24, 1, 24, 24, 24, 24, 24, 
	24, 15, 24, 24, 1, 24, 24, 24, 
	24, 24, 22
};

স্থির ধ্রুবক short khmer_syllable_machine_সূচক_offsets[] = {
	0, 23, 46, 62, 85, 108, 131, 154, 
	177, 200, 223, 246, 269, 292, 308, 331, 
	354, 377, 400, 423, 446, 469, 497, 522, 
	538, 563, 588, 590, 615, 640, 665, 690, 
	715, 740, 756, 781, 806, 808, 833, 858, 
	883, 908, 933
};

স্থির ধ্রুবক অক্ষর _khmer_syllable_machine_indicies[] = {
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 2, 
	0, 0, 0, 0, 3, 4, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 4, 0, 5, 5, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 5, 0, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 2, 0, 0, 
	0, 0, 0, 4, 0, 6, 6, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 2, 0, 7, 7, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 8, 0, 9, 9, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 2, 0, 0, 0, 0, 0, 
	10, 0, 9, 9, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 10, 
	0, 11, 11, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	2, 0, 0, 0, 0, 0, 12, 0, 
	11, 11, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 12, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 2, 0, 
	0, 0, 0, 13, 4, 0, 15, 15, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 16, 14, 14, 
	14, 14, 17, 18, 14, 15, 15, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 18, 19, 20, 20, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 20, 14, 15, 15, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 16, 14, 14, 14, 14, 
	14, 18, 14, 21, 21, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	16, 14, 22, 22, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 23, 
	14, 24, 24, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	16, 14, 14, 14, 14, 14, 25, 14, 
	24, 24, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 25, 14, 26, 
	26, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 16, 14, 
	14, 14, 14, 14, 27, 14, 26, 26, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 14, 14, 14, 14, 14, 
	14, 14, 14, 27, 14, 29, 29, 28, 
	30, 31, 31, 28, 28, 28, 13, 13, 
	28, 28, 28, 29, 28, 28, 28, 28, 
	16, 25, 27, 23, 28, 17, 18, 20, 
	28, 33, 34, 34, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 2, 10, 12, 8, 32, 13, 4, 
	5, 32, 35, 35, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	35, 32, 33, 36, 36, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 2, 10, 12, 8, 32, 3, 
	4, 5, 32, 37, 38, 38, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 2, 10, 12, 8, 32, 
	32, 4, 5, 32, 5, 32, 37, 6, 
	6, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 8, 32, 32, 2, 5, 32, 37, 
	7, 7, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 8, 5, 32, 
	37, 39, 39, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	2, 32, 32, 8, 32, 32, 10, 5, 
	32, 37, 40, 40, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 2, 10, 32, 8, 32, 32, 12, 
	5, 32, 33, 38, 38, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 2, 10, 12, 8, 32, 32, 
	4, 5, 32, 33, 38, 38, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 2, 10, 12, 8, 32, 
	3, 4, 5, 32, 42, 42, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 42, 41, 30, 43, 43, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 16, 25, 27, 23, 
	41, 17, 18, 20, 41, 44, 45, 45, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 16, 25, 27, 
	23, 41, 41, 18, 20, 41, 20, 41, 
	44, 21, 21, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 23, 41, 41, 16, 20, 
	41, 44, 22, 22, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 23, 
	20, 41, 44, 46, 46, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 16, 41, 41, 23, 41, 41, 
	25, 20, 41, 44, 47, 47, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 16, 25, 41, 23, 41, 
	41, 27, 20, 41, 30, 45, 45, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 16, 25, 27, 23, 
	41, 41, 18, 20, 41, 15, 15, 48, 
	48, 48, 48, 48, 48, 48, 48, 48, 
	48, 48, 48, 48, 16, 48, 48, 48, 
	48, 48, 18, 48, 0
};

স্থির ধ্রুবক অক্ষর _khmer_syllable_machine_trans_targs[] = {
	21, 1, 27, 31, 25, 26, 4, 5, 
	28, 7, 29, 9, 30, 32, 21, 12, 
	37, 41, 35, 21, 36, 15, 16, 38, 
	18, 39, 20, 40, 21, 22, 33, 42, 
	21, 23, 10, 24, 0, 2, 3, 6, 
	8, 21, 34, 11, 13, 14, 17, 19, 
	21
};

স্থির ধ্রুবক অক্ষর _khmer_syllable_machine_trans_actions[] = {
	1, 0, 2, 2, 2, 0, 0, 0, 
	2, 0, 2, 0, 2, 2, 3, 0, 
	2, 4, 4, 5, 0, 0, 0, 2, 
	0, 2, 0, 2, 8, 2, 0, 9, 
	10, 0, 0, 2, 0, 0, 0, 0, 
	0, 11, 4, 0, 0, 0, 0, 0, 
	12
};

স্থির ধ্রুবক অক্ষর _khmer_syllable_machine_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 6, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

স্থির ধ্রুবক অক্ষর _khmer_syllable_machine_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 7, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

স্থির ধ্রুবক short _khmer_syllable_machine_eof_trans[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 15, 20, 15, 15, 15, 
	15, 15, 15, 15, 15, 0, 33, 33, 
	33, 33, 33, 33, 33, 33, 33, 33, 
	33, 42, 42, 42, 42, 42, 42, 42, 
	42, 42, 49
};

স্থির ধ্রুবক পূর্ণসংখ্যা khmer_syllable_machine_start = 21;
স্থির ধ্রুবক পূর্ণসংখ্যা khmer_syllable_machine_first_final = 21;
স্থির ধ্রুবক পূর্ণসংখ্যা khmer_syllable_machine_error = -1;

স্থির ধ্রুবক পূর্ণসংখ্যা khmer_syllable_machine_en_main = 21;


#line 53 "hb-ot-shaper-khmer-machine.rl"



#line 102 "hb-ot-shaper-khmer-machine.rl"


#define found_syllable(syllable_type) \
  HB_STMT_START { \
    যদি (0) fprintf (stderr, "syllable %u..%u %s\n", ts, te, #syllable_type); \
    জন্য (unsigned পূর্ণসংখ্যা i = ts; i < te; i++) \
      info[i].syllable() = (syllable_serial << 4) | syllable_type; \
    syllable_serial++; \
    যদি (syllable_serial == 16) syllable_serial = 1; \
  } HB_STMT_END

অন্তঃরেখ শূন্য
find_syllables_khmer (hb_বাফার_t *বাফার)
{
  unsigned পূর্ণসংখ্যা p, pe, eof, ts, te, act HB_UNUSED;
  পূর্ণসংখ্যা cs;
  hb_glyph_info_t *info = বাফার->info;
  
#line 287 "hb-ot-shaper-khmer-machine.hh"
	{
	cs = khmer_syllable_machine_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 122 "hb-ot-shaper-khmer-machine.rl"


  p = 0;
  pe = eof = বাফার->len;

  unsigned পূর্ণসংখ্যা syllable_serial = 1;
  
#line 299 "hb-ot-shaper-khmer-machine.hh"
	{
	পূর্ণসংখ্যা _slen;
	পূর্ণসংখ্যা _trans;
	ধ্রুবক unsigned অক্ষর *চাবিগুলি_2;
	ধ্রুবক অক্ষর *_inds;
	যদি ( p == pe )
		যাও _test_eof;
_resume:
	নির্বাচন ( _khmer_syllable_machine_from_state_actions[cs] ) {
	ক্ষেত্র 7:
#line 1 "NONE"
	{ts = p;}
	বিরতি;
#line 311 "hb-ot-shaper-khmer-machine.hh"
	}

	চাবিগুলি_2 = khmer_syllable_machine_trans_চাবিগুলি + (cs<<1);
	_inds = _khmer_syllable_machine_indicies + khmer_syllable_machine_সূচক_offsets[cs];

	_slen = khmer_syllable_machine_চাবি_spans[cs];
	_trans = _inds[ _slen > 0 && চাবিগুলি_2[0] <=( info[p].khmer_category()) &&
		( info[p].khmer_category()) <= চাবিগুলি_2[1] ?
		( info[p].khmer_category()) - চাবিগুলি_2[0] : _slen ];

_eof_trans:
	cs = _khmer_syllable_machine_trans_targs[_trans];

	যদি ( _khmer_syllable_machine_trans_actions[_trans] == 0 )
		যাও _again;

	নির্বাচন ( _khmer_syllable_machine_trans_actions[_trans] ) {
	ক্ষেত্র 2:
#line 1 "NONE"
	{te = p+1;}
	বিরতি;
	ক্ষেত্র 8:
#line 98 "hb-ot-shaper-khmer-machine.rl"
	{te = p+1;{ found_syllable (khmer_non_khmer_cluster); }}
	বিরতি;
	ক্ষেত্র 10:
#line 96 "hb-ot-shaper-khmer-machine.rl"
	{te = p;p--;{ found_syllable (khmer_consonant_syllable); }}
	বিরতি;
	ক্ষেত্র 11:
#line 97 "hb-ot-shaper-khmer-machine.rl"
	{te = p;p--;{ found_syllable (khmer_broken_cluster); বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE; }}
	বিরতি;
	ক্ষেত্র 12:
#line 98 "hb-ot-shaper-khmer-machine.rl"
	{te = p;p--;{ found_syllable (khmer_non_khmer_cluster); }}
	বিরতি;
	ক্ষেত্র 1:
#line 96 "hb-ot-shaper-khmer-machine.rl"
	{{p = ((te))-1;}{ found_syllable (khmer_consonant_syllable); }}
	বিরতি;
	ক্ষেত্র 3:
#line 97 "hb-ot-shaper-khmer-machine.rl"
	{{p = ((te))-1;}{ found_syllable (khmer_broken_cluster); বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE; }}
	বিরতি;
	ক্ষেত্র 5:
#line 1 "NONE"
	{	নির্বাচন( act ) {
	ক্ষেত্র 2:
	{{p = ((te))-1;} found_syllable (khmer_broken_cluster); বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE; }
	বিরতি;
	ক্ষেত্র 3:
	{{p = ((te))-1;} found_syllable (khmer_non_khmer_cluster); }
	বিরতি;
	}
	}
	বিরতি;
	ক্ষেত্র 4:
#line 1 "NONE"
	{te = p+1;}
#line 97 "hb-ot-shaper-khmer-machine.rl"
	{act = 2;}
	বিরতি;
	ক্ষেত্র 9:
#line 1 "NONE"
	{te = p+1;}
#line 98 "hb-ot-shaper-khmer-machine.rl"
	{act = 3;}
	বিরতি;
#line 368 "hb-ot-shaper-khmer-machine.hh"
	}

_again:
	নির্বাচন ( _khmer_syllable_machine_to_state_actions[cs] ) {
	ক্ষেত্র 6:
#line 1 "NONE"
	{ts = 0;}
	বিরতি;
#line 375 "hb-ot-shaper-khmer-machine.hh"
	}

	যদি ( ++p != pe )
		যাও _resume;
	_test_eof: {}
	যদি ( p == eof )
	{
	যদি ( _khmer_syllable_machine_eof_trans[cs] > 0 ) {
		_trans = _khmer_syllable_machine_eof_trans[cs] - 1;
		যাও _eof_trans;
	}
	}

	}

#line 130 "hb-ot-shaper-khmer-machine.rl"

}

#undef found_syllable

#endif /* HB_OT_SHAPER_KHMER_MACHINE_HH */
