/*
 * Copyright (C) 2012 Grigori Goronzy <greg@kinoho.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-unicode.hh"
#అన్తర్భావ "hb-machinery.hh"

#అన్తర్భావ "hb-ucd-table.hh"

స్థిర hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_వాడని,
			hb_codepoint_t unicode,
			శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

స్థిర hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_వాడని,
			 hb_codepoint_t unicode,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

స్థిర hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_వాడని,
		  hb_codepoint_t unicode,
		  శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి unicode + _hb_ucd_bmg (unicode);
}

స్థిర hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_వాడని,
	       hb_codepoint_t unicode,
	       శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
}


#నిర్వచించు SBASE 0xAC00u
#నిర్వచించు LBASE 0x1100u
#నిర్వచించు VBASE 0x1161u
#నిర్వచించు TBASE 0x11A7u
#నిర్వచించు SCOUNT 11172u
#నిర్వచించు LCOUNT 19u
#నిర్వచించు VCOUNT 21u
#నిర్వచించు TCOUNT 28u
#నిర్వచించు NCOUNT (VCOUNT * TCOUNT)

స్థిర అంతఃవరుస తార్కిక
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  గుర్తులేని si = ab - SBASE;

  ఒకవేళ (si >= SCOUNT)
    తిరిగి అబద్ధం;

  ఒకవేళ (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    తిరిగి నిజం;
  } లేకపోతే {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    తిరిగి నిజం;
  }
}

స్థిర అంతఃవరుస తార్కిక
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  ఒకవేళ (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    తిరిగి నిజం;
  }
  లేకపోతే ఒకవేళ (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    పూర్ణాంకం li = a - LBASE;
    పూర్ణాంకం vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    తిరిగి నిజం;
  }
  లేకపోతే
    తిరిగి అబద్ధం;
}

స్థిర పూర్ణాంకం
_cmp_pair (స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2)
{
  uint64_t& a = * (uint64_t*) కీలకం_3;
  uint64_t b = (* (uint64_t*) అంశం_2) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  తిరిగి a < b ? -1 : a > b ? +1 : 0;
}
స్థిర పూర్ణాంకం
_cmp_pair_11_7_14 (స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2)
{
  uint32_t& a = * (uint32_t*) కీలకం_3;
  uint32_t b = (* (uint32_t*) అంశం_2) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  తిరిగి a < b ? -1 : a > b ? +1 : 0;
}

స్థిర hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_వాడని,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		శూన్య *user_దత్తాంశం HB_వాడని)
{
  // Hangul is handled algorithmically.
  ఒకవేళ (_hb_ucd_compose_hangul (a, b, ab)) తిరిగి నిజం;

  hb_codepoint_t u = 0;

  ఒకవేళ ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    స్థిరరాశి uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_నిడివి (_hb_ucd_dm2_u32_map),
				    పరిమాణం (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    ఒకవేళ (likely (!v)) తిరిగి అబద్ధం;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  లేకపోతే
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    స్థిరరాశి uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_నిడివి (_hb_ucd_dm2_u64_map),
				    పరిమాణం (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    ఒకవేళ (likely (!v)) తిరిగి అబద్ధం;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  ఒకవేళ (unlikely (!u)) తిరిగి అబద్ధం;
  *ab = u;
  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_వాడని,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  శూన్య *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (_hb_ucd_decompose_hangul (ab, a, b)) తిరిగి నిజం;

  గుర్తులేని i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  ఒకవేళ (likely (!i)) తిరిగి అబద్ధం;
  i--;

  /* Check if it's a single-character decomposition. */
  ఒకవేళ (i < ARRAY_నిడివి (_hb_ucd_dm1_p0_map) + ARRAY_నిడివి (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    ఒకవేళ (i < ARRAY_నిడివి (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    లేకపోతే
    {
      /* Plane 2. */
      i -= ARRAY_నిడివి (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    తిరిగి నిజం;
  }
  i -= ARRAY_నిడివి (_hb_ucd_dm1_p0_map) + ARRAY_నిడివి (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  ఒకవేళ (i < ARRAY_నిడివి (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    తిరిగి నిజం;
  }
  i -= ARRAY_నిడివి (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  తిరిగి నిజం;
}


స్థిర శూన్య free_static_ucd_funcs ();

స్థిర నిర్మాణం hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  స్థిర hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (శూన్యసూచిక);

    hb_unicode_funcs_సమితి_combining_class_func (funcs, hb_ucd_combining_class, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_general_category_func (funcs, hb_ucd_general_category, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_mirroring_func (funcs, hb_ucd_mirroring, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_script_func (funcs, hb_ucd_script, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_compose_func (funcs, hb_ucd_compose, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_decompose_func (funcs, hb_ucd_decompose, శూన్యసూచిక, శూన్యసూచిక);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_ucd_funcs);

    తిరిగి funcs;
  }
} static_ucd_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#నిర్వచితమైతే HB_NO_UCD
  తిరిగి hb_unicode_funcs_get_ఖాళీలు ();
#అయితే_ముగింపు
  తిరిగి static_ucd_funcs.get_unconst ();
}
