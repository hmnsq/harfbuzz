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

#समावेश "hb.hh"
#समावेश "hb-unicode.hh"
#समावेश "hb-machinery.hh"

#समावेश "hb-ucd-table.hh"

अटल hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t unicode,
			व्योम *user_तथ्य HB_UNUSED)
{
  वापस (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

अटल hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			 hb_codepoint_t unicode,
			 व्योम *user_तथ्य HB_UNUSED)
{
  वापस (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

अटल hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t unicode,
		  व्योम *user_तथ्य HB_UNUSED)
{
  वापस unicode + _hb_ucd_bmg (unicode);
}

अटल hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
	       hb_codepoint_t unicode,
	       व्योम *user_तथ्य HB_UNUSED)
{
  वापस _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
}


#घोषणा SBASE 0xAC00u
#घोषणा LBASE 0x1100u
#घोषणा VBASE 0x1161u
#घोषणा TBASE 0x11A7u
#घोषणा SCOUNT 11172u
#घोषणा LCOUNT 19u
#घोषणा VCOUNT 21u
#घोषणा TCOUNT 28u
#घोषणा NCOUNT (VCOUNT * TCOUNT)

अटल अंतरभूत bool
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  अचिन्हित si = ab - SBASE;

  अगर (si >= SCOUNT)
    वापस false;

  अगर (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    वापस true;
  } अन्यथा {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    वापस true;
  }
}

अटल अंतरभूत bool
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  अगर (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    वापस true;
  }
  अन्यथा अगर (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    पूर्णांक li = a - LBASE;
    पूर्णांक vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    वापस true;
  }
  अन्यथा
    वापस false;
}

अटल पूर्णांक
_cmp_pair (स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item)
{
  uint64_t& a = * (uint64_t*) कुंजी_3;
  uint64_t b = (* (uint64_t*) _item) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  वापस a < b ? -1 : a > b ? +1 : 0;
}
अटल पूर्णांक
_cmp_pair_11_7_14 (स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item)
{
  uint32_t& a = * (uint32_t*) कुंजी_3;
  uint32_t b = (* (uint32_t*) _item) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  वापस a < b ? -1 : a > b ? +1 : 0;
}

अटल hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		व्योम *user_तथ्य HB_UNUSED)
{
  // Hangul is handled algorithmically.
  अगर (_hb_ucd_compose_hangul (a, b, ab)) वापस true;

  hb_codepoint_t u = 0;

  अगर ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    स्थिर uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_लंबाई (_hb_ucd_dm2_u32_map),
				    माप (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    अगर (likely (!v)) वापस false;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  अन्यथा
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    स्थिर uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_लंबाई (_hb_ucd_dm2_u64_map),
				    माप (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    अगर (likely (!v)) वापस false;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  अगर (unlikely (!u)) वापस false;
  *ab = u;
  वापस true;
}

अटल hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  व्योम *user_तथ्य HB_UNUSED)
{
  अगर (_hb_ucd_decompose_hangul (ab, a, b)) वापस true;

  अचिन्हित i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  अगर (likely (!i)) वापस false;
  i--;

  /* Check if it's a single-character decomposition. */
  अगर (i < ARRAY_लंबाई (_hb_ucd_dm1_p0_map) + ARRAY_लंबाई (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    अगर (i < ARRAY_लंबाई (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    अन्यथा
    {
      /* Plane 2. */
      i -= ARRAY_लंबाई (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    वापस true;
  }
  i -= ARRAY_लंबाई (_hb_ucd_dm1_p0_map) + ARRAY_लंबाई (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  अगर (i < ARRAY_लंबाई (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    वापस true;
  }
  i -= ARRAY_लंबाई (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  वापस true;
}


अटल व्योम free_static_ucd_funcs ();

अटल काष्ठा hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  अटल hb_unicode_funcs_t *create ()
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

    वापस funcs;
  }
} static_ucd_funcs;

अटल अंतरभूत
व्योम free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#अगर_घोषित HB_NO_UCD
  वापस hb_unicode_funcs_get_empty ();
#पूर्ण_अगर
  वापस static_ucd_funcs.get_unconst ();
}
