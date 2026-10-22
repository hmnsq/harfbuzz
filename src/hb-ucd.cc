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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-unicode.hh"
#অন্তর্ভাৱ "hb-machinery.hh"

#অন্তর্ভাৱ "hb-ucd-table.hh"

স্থির hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t unicode,
			শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

স্থির hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			 hb_codepoint_t unicode,
			 শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

স্থির hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t unicode,
		  শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই unicode + _hb_ucd_bmg (unicode);
}

স্থির hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
	       hb_codepoint_t unicode,
	       শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
}


#define SBASE 0xAC00u
#define LBASE 0x1100u
#define VBASE 0x1161u
#define TBASE 0x11A7u
#define SCOUNT 11172u
#define LCOUNT 19u
#define VCOUNT 21u
#define TCOUNT 28u
#define NCOUNT (VCOUNT * TCOUNT)

স্থির অন্তঃস্থ তার্কিক
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  unsigned si = ab - SBASE;

  যদি (si >= SCOUNT)
    ঘূৰাই মিছা;

  যদি (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    ঘূৰাই সঁচা;
  } অন্যথা {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    ঘূৰাই সঁচা;
  }
}

স্থির অন্তঃস্থ তার্কিক
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  যদি (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    ঘূৰাই সঁচা;
  }
  অন্যথা যদি (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    পূর্ণাঙ্ক li = a - LBASE;
    পূর্ণাঙ্ক vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    ঘূৰাই সঁচা;
  }
  অন্যথা
    ঘূৰাই মিছা;
}

স্থির পূর্ণাঙ্ক
_cmp_pair (ধ্রুৱ শূন্য *চাবি_3, ধ্রুৱ শূন্য *_item)
{
  uint64_t& a = * (uint64_t*) চাবি_3;
  uint64_t b = (* (uint64_t*) _item) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  ঘূৰাই a < b ? -1 : a > b ? +1 : 0;
}
স্থির পূর্ণাঙ্ক
_cmp_pair_11_7_14 (ধ্রুৱ শূন্য *চাবি_3, ধ্রুৱ শূন্য *_item)
{
  uint32_t& a = * (uint32_t*) চাবি_3;
  uint32_t b = (* (uint32_t*) _item) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  ঘূৰাই a < b ? -1 : a > b ? +1 : 0;
}

স্থির hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		শূন্য *user_data HB_UNUSED)
{
  // Hangul is handled algorithmically.
  যদি (_hb_ucd_compose_hangul (a, b, ab)) ঘূৰাই সঁচা;

  hb_codepoint_t u = 0;

  যদি ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    ধ্রুৱ uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_LENGTH (_hb_ucd_dm2_u32_map),
				    পরিমাণ (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    যদি (likely (!v)) ঘূৰাই মিছা;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  অন্যথা
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    ধ্রুৱ uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_LENGTH (_hb_ucd_dm2_u64_map),
				    পরিমাণ (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    যদি (likely (!v)) ঘূৰাই মিছা;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  যদি (unlikely (!u)) ঘূৰাই মিছা;
  *ab = u;
  ঘূৰাই সঁচা;
}

স্থির hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  শূন্য *user_data HB_UNUSED)
{
  যদি (_hb_ucd_decompose_hangul (ab, a, b)) ঘূৰাই সঁচা;

  unsigned i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  যদি (likely (!i)) ঘূৰাই মিছা;
  i--;

  /* Check if it's a single-character decomposition. */
  যদি (i < ARRAY_LENGTH (_hb_ucd_dm1_p0_map) + ARRAY_LENGTH (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    যদি (i < ARRAY_LENGTH (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    অন্যথা
    {
      /* Plane 2. */
      i -= ARRAY_LENGTH (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    ঘূৰাই সঁচা;
  }
  i -= ARRAY_LENGTH (_hb_ucd_dm1_p0_map) + ARRAY_LENGTH (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  যদি (i < ARRAY_LENGTH (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    ঘূৰাই সঁচা;
  }
  i -= ARRAY_LENGTH (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  ঘূৰাই সঁচা;
}


স্থির শূন্য free_static_ucd_funcs ();

স্থির সংরচনা hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  স্থির hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_ucd_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_ucd_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_ucd_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_ucd_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_ucd_compose, nullptr, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_ucd_decompose, nullptr, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_ucd_funcs);

    ঘূৰাই funcs;
  }
} static_ucd_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#ifdef HB_NO_UCD
  ঘূৰাই hb_unicode_funcs_get_empty ();
#endif
  ঘূৰাই static_ucd_funcs.get_unconst ();
}
