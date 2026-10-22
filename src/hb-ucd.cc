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

#ضمن "hb.hh"
#ضمن "hb-unicode.hh"
#ضمن "hb-machinery.hh"

#ضمن "hb-ucd-table.hh"

ساكن hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
			hb_codepoint_t unicode,
			فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

ساكن hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
			 hb_codepoint_t unicode,
			 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

ساكن hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
		  hb_codepoint_t unicode,
		  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع unicode + _hb_ucd_bmg (unicode);
}

ساكن hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
	       hb_codepoint_t unicode,
	       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
}


#وصف_ڪريو SBASE 0xAC00u
#وصف_ڪريو LBASE 0x1100u
#وصف_ڪريو VBASE 0x1161u
#وصف_ڪريو TBASE 0x11A7u
#وصف_ڪريو SCOUNT 11172u
#وصف_ڪريو LCOUNT 19u
#وصف_ڪريو VCOUNT 21u
#وصف_ڪريو TCOUNT 28u
#وصف_ڪريو NCOUNT (VCOUNT * TCOUNT)

ساكن مضمن منطقي
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  بنانشان si = ab - SBASE;

  إذا (si >= SCOUNT)
    أرجع خطأ;

  إذا (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    أرجع سچ;
  } وإلا {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    أرجع سچ;
  }
}

ساكن مضمن منطقي
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  إذا (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    أرجع سچ;
  }
  وإلا إذا (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    صحيح li = a - LBASE;
    صحيح vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    أرجع سچ;
  }
  وإلا
    أرجع خطأ;
}

ساكن صحيح
_cmp_pair (ثابت فراغ *ڪنجي_3, ثابت فراغ *شي_2)
{
  uint64_t& a = * (uint64_t*) ڪنجي_3;
  uint64_t b = (* (uint64_t*) شي_2) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  أرجع a < b ? -1 : a > b ? +1 : 0;
}
ساكن صحيح
_cmp_pair_11_7_14 (ثابت فراغ *ڪنجي_3, ثابت فراغ *شي_2)
{
  uint32_t& a = * (uint32_t*) ڪنجي_3;
  uint32_t b = (* (uint32_t*) شي_2) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  أرجع a < b ? -1 : a > b ? +1 : 0;
}

ساكن hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  // Hangul is handled algorithmically.
  إذا (_hb_ucd_compose_hangul (a, b, ab)) أرجع سچ;

  hb_codepoint_t u = 0;

  إذا ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    ثابت uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_ڊيگهائي (_hb_ucd_dm2_u32_map),
				    حجم (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    إذا (likely (!v)) أرجع خطأ;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  وإلا
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    ثابت uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_ڊيگهائي (_hb_ucd_dm2_u64_map),
				    حجم (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    إذا (likely (!v)) أرجع خطأ;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  إذا (unlikely (!u)) أرجع خطأ;
  *ab = u;
  أرجع سچ;
}

ساكن hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_اڻ_استعمال_ٿيل,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  إذا (_hb_ucd_decompose_hangul (ab, a, b)) أرجع سچ;

  بنانشان i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  إذا (likely (!i)) أرجع خطأ;
  i--;

  /* Check if it's a single-character decomposition. */
  إذا (i < ARRAY_ڊيگهائي (_hb_ucd_dm1_p0_map) + ARRAY_ڊيگهائي (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    إذا (i < ARRAY_ڊيگهائي (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    وإلا
    {
      /* Plane 2. */
      i -= ARRAY_ڊيگهائي (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    أرجع سچ;
  }
  i -= ARRAY_ڊيگهائي (_hb_ucd_dm1_p0_map) + ARRAY_ڊيگهائي (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  إذا (i < ARRAY_ڊيگهائي (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    أرجع سچ;
  }
  i -= ARRAY_ڊيگهائي (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  أرجع سچ;
}


ساكن فراغ free_static_ucd_funcs ();

ساكن بنية hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  ساكن hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (خالي_اشارو);

    hb_unicode_funcs_سيٽ_combining_class_func (funcs, hb_ucd_combining_class, خالي_اشارو, خالي_اشارو);
    hb_unicode_funcs_سيٽ_general_category_func (funcs, hb_ucd_general_category, خالي_اشارو, خالي_اشارو);
    hb_unicode_funcs_سيٽ_mirroring_func (funcs, hb_ucd_mirroring, خالي_اشارو, خالي_اشارو);
    hb_unicode_funcs_سيٽ_script_func (funcs, hb_ucd_script, خالي_اشارو, خالي_اشارو);
    hb_unicode_funcs_سيٽ_compose_func (funcs, hb_ucd_compose, خالي_اشارو, خالي_اشارو);
    hb_unicode_funcs_سيٽ_decompose_func (funcs, hb_ucd_decompose, خالي_اشارو, خالي_اشارو);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_ucd_funcs);

    أرجع funcs;
  }
} static_ucd_funcs;

ساكن مضمن
فراغ free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#جيڪڏهن_وصف_ٿيل HB_NO_UCD
  أرجع hb_unicode_funcs_get_خالي_جايون ();
#جيڪڏهن_پورو
  أرجع static_ucd_funcs.get_unconst ();
}
