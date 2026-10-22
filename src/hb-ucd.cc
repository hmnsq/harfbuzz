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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-unicode.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ucd-table.hh"

ਸਥਿਰ hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			hb_codepoint_t unicode,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

ਸਥਿਰ hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			 hb_codepoint_t unicode,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

ਸਥਿਰ hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
		  hb_codepoint_t unicode,
		  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ unicode + _hb_ucd_bmg (unicode);
}

ਸਥਿਰ hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
	       hb_codepoint_t unicode,
	       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SBASE 0xAC00u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBASE 0x1100u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VBASE 0x1161u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TBASE 0x11A7u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCOUNT 11172u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LCOUNT 19u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VCOUNT 21u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TCOUNT 28u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NCOUNT (VCOUNT * TCOUNT)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ si = ab - SBASE;

  ਜੇ (si >= SCOUNT)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    ਵਾਪਸ ਸੱਚ;
  } ਨਹੀਂਤਾਂ {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    ਵਾਪਸ ਸੱਚ;
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  ਜੇ (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    ਵਾਪਸ ਸੱਚ;
  }
  ਨਹੀਂਤਾਂ ਜੇ (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    ਪੂਰਨ li = a - LBASE;
    ਪੂਰਨ vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    ਵਾਪਸ ਸੱਚ;
  }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਪੂਰਨ
_cmp_pair (ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2)
{
  uint64_t& a = * (uint64_t*) ਕੁੰਜੀ_3;
  uint64_t b = (* (uint64_t*) ਵਸਤੂ_2) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  ਵਾਪਸ a < b ? -1 : a > b ? +1 : 0;
}
ਸਥਿਰ ਪੂਰਨ
_cmp_pair_11_7_14 (ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2)
{
  uint32_t& a = * (uint32_t*) ਕੁੰਜੀ_3;
  uint32_t b = (* (uint32_t*) ਵਸਤੂ_2) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  ਵਾਪਸ a < b ? -1 : a > b ? +1 : 0;
}

ਸਥਿਰ hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  // Hangul is handled algorithmically.
  ਜੇ (_hb_ucd_compose_hangul (a, b, ab)) ਵਾਪਸ ਸੱਚ;

  hb_codepoint_t u = 0;

  ਜੇ ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    ਅਚਲ uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_ਲੰਮਾਈ (_hb_ucd_dm2_u32_map),
				    ਆਕਾਰ (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    ਜੇ (likely (!v)) ਵਾਪਸ ਝੂਠ;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  ਨਹੀਂਤਾਂ
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    ਅਚਲ uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_ਲੰਮਾਈ (_hb_ucd_dm2_u64_map),
				    ਆਕਾਰ (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    ਜੇ (likely (!v)) ਵਾਪਸ ਝੂਠ;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  ਜੇ (unlikely (!u)) ਵਾਪਸ ਝੂਠ;
  *ab = u;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (_hb_ucd_decompose_hangul (ab, a, b)) ਵਾਪਸ ਸੱਚ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  ਜੇ (likely (!i)) ਵਾਪਸ ਝੂਠ;
  i--;

  /* Check if it's a single-character decomposition. */
  ਜੇ (i < ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p0_map) + ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    ਜੇ (i < ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    ਨਹੀਂਤਾਂ
    {
      /* Plane 2. */
      i -= ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    ਵਾਪਸ ਸੱਚ;
  }
  i -= ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p0_map) + ARRAY_ਲੰਮਾਈ (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  ਜੇ (i < ARRAY_ਲੰਮਾਈ (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    ਵਾਪਸ ਸੱਚ;
  }
  i -= ARRAY_ਲੰਮਾਈ (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  ਵਾਪਸ ਸੱਚ;
}


ਸਥਿਰ ਖਾਲੀ free_static_ucd_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_ਸੈੱਟ_combining_class_func (funcs, hb_ucd_combining_class, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_general_category_func (funcs, hb_ucd_general_category, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_mirroring_func (funcs, hb_ucd_mirroring, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_script_func (funcs, hb_ucd_script, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_compose_func (funcs, hb_ucd_compose, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_decompose_func (funcs, hb_ucd_decompose, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_ucd_funcs);

    ਵਾਪਸ funcs;
  }
} static_ucd_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_UCD
  ਵਾਪਸ hb_unicode_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ();
#ਜੇ_ਅੰਤ
  ਵਾਪਸ static_ucd_funcs.get_unconst ();
}
