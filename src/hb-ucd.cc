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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-unicode.hh"
#અન્તર્ભાવ "hb-machinery.hh"

#અન્તર્ભાવ "hb-ucd-table.hh"

સ્થિર hb_unicode_combining_class_t
hb_ucd_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t unicode,
			શૂન્ય *user_data HB_UNUSED)
{
  પાછા (hb_unicode_combining_class_t) _hb_ucd_ccc (unicode);
}

સ્થિર hb_unicode_general_category_t
hb_ucd_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			 hb_codepoint_t unicode,
			 શૂન્ય *user_data HB_UNUSED)
{
  પાછા (hb_unicode_general_category_t) _hb_ucd_gc (unicode);
}

સ્થિર hb_codepoint_t
hb_ucd_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t unicode,
		  શૂન્ય *user_data HB_UNUSED)
{
  પાછા unicode + _hb_ucd_bmg (unicode);
}

સ્થિર hb_script_t
hb_ucd_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
	       hb_codepoint_t unicode,
	       શૂન્ય *user_data HB_UNUSED)
{
  પાછા _hb_ucd_sc_map[_hb_ucd_sc (unicode)];
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

સ્થિર અંતઃરેખ તાર્કિક
_hb_ucd_decompose_hangul (hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b)
{
  unsigned si = ab - SBASE;

  જો (si >= SCOUNT)
    પાછા ખોટું;

  જો (si % TCOUNT)
  {
    /* LV,T */
    *a = SBASE + (si / TCOUNT) * TCOUNT;
    *b = TBASE + (si % TCOUNT);
    પાછા સાચું;
  } નહીંતર {
    /* L,V */
    *a = LBASE + (si / NCOUNT);
    *b = VBASE + (si % NCOUNT) / TCOUNT;
    પાછા સાચું;
  }
}

સ્થિર અંતઃરેખ તાર્કિક
_hb_ucd_compose_hangul (hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab)
{
  જો (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) &&
    !((a - SBASE) % TCOUNT))
  {
    /* LV,T */
    *ab = a + (b - TBASE);
    પાછા સાચું;
  }
  નહીંતર જો (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
  {
    /* L,V */
    પૂર્ણાંક li = a - LBASE;
    પૂર્ણાંક vi = b - VBASE;
    *ab = SBASE + li * NCOUNT + vi * TCOUNT;
    પાછા સાચું;
  }
  નહીંતર
    પાછા ખોટું;
}

સ્થિર પૂર્ણાંક
_cmp_pair (અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item)
{
  uint64_t& a = * (uint64_t*) ચાવી_3;
  uint64_t b = (* (uint64_t*) _item) & HB_CODEPOINT_ENCODE3(0x1FFFFFu, 0x1FFFFFu, 0);

  પાછા a < b ? -1 : a > b ? +1 : 0;
}
સ્થિર પૂર્ણાંક
_cmp_pair_11_7_14 (અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item)
{
  uint32_t& a = * (uint32_t*) ચાવી_3;
  uint32_t b = (* (uint32_t*) _item) & HB_CODEPOINT_ENCODE3_11_7_14(0x1FFFFFu, 0x1FFFFFu, 0);

  પાછા a < b ? -1 : a > b ? +1 : 0;
}

સ્થિર hb_bool_t
hb_ucd_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		hb_codepoint_t a, hb_codepoint_t b, hb_codepoint_t *ab,
		શૂન્ય *user_data HB_UNUSED)
{
  // Hangul is handled algorithmically.
  જો (_hb_ucd_compose_hangul (a, b, ab)) પાછા સાચું;

  hb_codepoint_t u = 0;

  જો ((a & 0xFFFFF800u) == 0x0000u && (b & 0xFFFFFF80) == 0x0300u)
  {
    /* If "a" is small enough and "b" is in the U+0300 range,
     * the composition data is encoded in a 32bit array sorted
     * by "a,b" pair. */
    uint32_t k = HB_CODEPOINT_ENCODE3_11_7_14 (a, b, 0);
    અચલ uint32_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u32_map,
				    ARRAY_LENGTH (_hb_ucd_dm2_u32_map),
				    કદ (*_hb_ucd_dm2_u32_map),
				    _cmp_pair_11_7_14);
    જો (likely (!v)) પાછા ખોટું;
    u = HB_CODEPOINT_DECODE3_11_7_14_3 (*v);
  }
  નહીંતર
  {
    /* Otherwise it is stored in a 64bit array sorted by
     * "a,b" pair. */
    uint64_t k = HB_CODEPOINT_ENCODE3 (a, b, 0);
    અચલ uint64_t *v = hb_bsearch (k,
				    _hb_ucd_dm2_u64_map,
				    ARRAY_LENGTH (_hb_ucd_dm2_u64_map),
				    કદ (*_hb_ucd_dm2_u64_map),
				    _cmp_pair);
    જો (likely (!v)) પાછા ખોટું;
    u = HB_CODEPOINT_DECODE3_3 (*v);
  }

  જો (unlikely (!u)) પાછા ખોટું;
  *ab = u;
  પાછા સાચું;
}

સ્થિર hb_bool_t
hb_ucd_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		  hb_codepoint_t ab, hb_codepoint_t *a, hb_codepoint_t *b,
		  શૂન્ય *user_data HB_UNUSED)
{
  જો (_hb_ucd_decompose_hangul (ab, a, b)) પાછા સાચું;

  unsigned i = _hb_ucd_dm (ab);

  /* If no data, there's no decomposition. */
  જો (likely (!i)) પાછા ખોટું;
  i--;

  /* Check if it's a single-character decomposition. */
  જો (i < ARRAY_LENGTH (_hb_ucd_dm1_p0_map) + ARRAY_LENGTH (_hb_ucd_dm1_p2_map))
  {
    /* Single-character decompositions currently are only in plane 0 or plane 2. */
    જો (i < ARRAY_LENGTH (_hb_ucd_dm1_p0_map))
    {
      /* Plane 0. */
      *a = _hb_ucd_dm1_p0_map[i];
    }
    નહીંતર
    {
      /* Plane 2. */
      i -= ARRAY_LENGTH (_hb_ucd_dm1_p0_map);
      *a = 0x20000 | _hb_ucd_dm1_p2_map[i];
    }
    *b = 0;
    પાછા સાચું;
  }
  i -= ARRAY_LENGTH (_hb_ucd_dm1_p0_map) + ARRAY_LENGTH (_hb_ucd_dm1_p2_map);

  /* Otherwise they are encoded either in a 32bit array or a 64bit array. */
  જો (i < ARRAY_LENGTH (_hb_ucd_dm2_u32_map))
  {
    /* 32bit array. */
    uint32_t v = _hb_ucd_dm2_u32_map[i];
    *a = HB_CODEPOINT_DECODE3_11_7_14_1 (v);
    *b = HB_CODEPOINT_DECODE3_11_7_14_2 (v);
    પાછા સાચું;
  }
  i -= ARRAY_LENGTH (_hb_ucd_dm2_u32_map);

  /* 64bit array. */
  uint64_t v = _hb_ucd_dm2_u64_map[i];
  *a = HB_CODEPOINT_DECODE3_1 (v);
  *b = HB_CODEPOINT_DECODE3_2 (v);
  પાછા સાચું;
}


સ્થિર શૂન્ય free_static_ucd_funcs ();

સ્થિર સંરચના hb_ucd_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_ucd_unicode_funcs_lazy_loader_t>
{
  સ્થિર hb_unicode_funcs_t *create ()
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

    પાછા funcs;
  }
} static_ucd_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_ucd_funcs ()
{
  static_ucd_funcs.free_instance ();
}

hb_unicode_funcs_t *
hb_ucd_get_unicode_funcs ()
{
#ifdef HB_NO_UCD
  પાછા hb_unicode_funcs_get_empty ();
#endif
  પાછા static_ucd_funcs.get_unconst ();
}
