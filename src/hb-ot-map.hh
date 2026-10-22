/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2010,2011,2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_MAP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAP_MAX_ਬਿੱਟ 8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAP_MAX_ਮੁੱਲ ((1u << HB_OT_MAP_MAX_ਬਿੱਟ) - 1u)

ਬਣਤਰ hb_ot_shape_plan_t;

ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[2] = {HB_OT_ਟੈਗ_GSUB, HB_OT_ਟੈਗ_GPOS};

ਬਣਤਰ hb_ot_map_t
{
  ਮਿੱਤਰ ਬਣਤਰ hb_ot_map_builder_t;

  ਜਨਤਕ:

  ਬਣਤਰ feature_map_t {
    hb_ਟੈਗ_t ਟੈਗ_2; /* should be first for our bsearch to work */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2[2]; /* GSUB/GPOS */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage[2]; /* GSUB/GPOS */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ;
    hb_ਨਕਾਬ_t ਨਕਾਬ;
    hb_ਨਕਾਬ_t _1_mask; /* mask for value=1, for quick access */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ needs_fallback : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ auto_zwnj : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ auto_zwj : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ random : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪ੍ਰਤੀ_syllable : 1;

    ਪੂਰਨ cmp (ਅਚਲ hb_ਟੈਗ_t ਟੈਗ_4) ਅਚਲ
    { ਵਾਪਸ ਟੈਗ_4 < ਟੈਗ_2 ? -1 : ਟੈਗ_4 > ਟੈਗ_2 ? 1 : 0; }

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
    {
      ਅਚਲ feature_map_t *a = (ਅਚਲ feature_map_t *) pa;
      ਅਚਲ feature_map_t *b = (ਅਚਲ feature_map_t *) pb;
      ਵਾਪਸ a->ਟੈਗ_2 < b->ਟੈਗ_2 ? -1 : a->ਟੈਗ_2 > b->ਟੈਗ_2 ? 1 : 0;
    }
  };

  ਬਣਤਰ lookup_map_t {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਸੂਚਕਾਂਕ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ auto_zwnj : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ auto_zwj : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ random : 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪ੍ਰਤੀ_syllable : 1;
    hb_ਨਕਾਬ_t ਨਕਾਬ;
    hb_ਟੈਗ_t feature_ਟੈਗ;

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
    {
      ਅਚਲ lookup_map_t *a = (ਅਚਲ lookup_map_t *) pa;
      ਅਚਲ lookup_map_t *b = (ਅਚਲ lookup_map_t *) pb;
      ਵਾਪਸ a->ਸੂਚਕਾਂਕ_2 < b->ਸੂਚਕਾਂਕ_2 ? -1 : a->ਸੂਚਕਾਂਕ_2 > b->ਸੂਚਕਾਂਕ_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*pause_func_t) (ਅਚਲ ਬਣਤਰ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ);

  ਬਣਤਰ stage_map_t {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  ਖਾਲੀ init ()
  {
    hb_memset (ਇਹ, 0, ਆਕਾਰ (*ਇਹ));

    features.init0 ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    {
      lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].init0 ();
      stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].init0 ();
    }
  }
  ਖਾਲੀ fini ()
  {
    features.fini ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    {
      lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].fini ();
      stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].fini ();
    }
  }

  hb_ਨਕਾਬ_t get_global_ਨਕਾਬ () ਅਚਲ { ਵਾਪਸ global_ਨਕਾਬ; }

  hb_ਨਕਾਬ_t get_ਨਕਾਬ (hb_ਟੈਗ_t feature_ਟੈਗ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਖਿਸਕਾਓ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਅਚਲ feature_map_t *map = features.bsearch (feature_ਟੈਗ);
    ਜੇ (ਖਿਸਕਾਓ) *ਖਿਸਕਾਓ = map ? map->ਖਿਸਕਾਓ : 0;
    ਵਾਪਸ map ? map->ਨਕਾਬ : 0;
  }

  ਤਾਰਕਿਕ needs_fallback (hb_ਟੈਗ_t feature_ਟੈਗ) ਅਚਲ
  {
    ਅਚਲ feature_map_t *map = features.bsearch (feature_ਟੈਗ);
    ਵਾਪਸ map ? map->needs_fallback : ਝੂਠ;
  }

  hb_ਨਕਾਬ_t get_1_ਨਕਾਬ (hb_ਟੈਗ_t feature_ਟੈਗ) ਅਚਲ
  {
    ਅਚਲ feature_map_t *map = features.bsearch (feature_ਟੈਗ);
    ਵਾਪਸ map ? map->_1_mask : 0;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ਟੈਗ_t feature_ਟੈਗ) ਅਚਲ
  {
    ਅਚਲ feature_map_t *map = features.bsearch (feature_ਟੈਗ);
    ਵਾਪਸ map ? map->ਸੂਚਕਾਂਕ_2[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] : HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_stage (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ਟੈਗ_t feature_ਟੈਗ) ਅਚਲ
  {
    ਅਚਲ feature_map_t *map = features.bsearch (feature_ਟੈਗ);
    ਵਾਪਸ map ? map->stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] : UINT_MAX;
  }

  hb_array_t<ਅਚਲ hb_ot_map_t::lookup_map_t>
  get_stage_lookups (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage) ਅਚਲ
  {
    ਜੇ (unlikely (stage > stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ))
      ਵਾਪਸ hb_array<ਅਚਲ hb_ot_map_t::lookup_map_t> (ਖਾਲੀ_ਸੂਚਕ, 0);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = stage ? stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][stage - 1].ਆਖਰੀ_lookup : 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ   = stage < stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ ? stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][stage].ਆਖਰੀ_lookup : lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ;
    ਵਾਪਸ lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].as_array ().sub_array (ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ);
  }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ collect_lookups (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *lookups) ਅਚਲ;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proxy>
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ apply (ਅਚਲ Proxy &proxy,
			  ਅਚਲ ਬਣਤਰ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ substitute (ਅਚਲ ਬਣਤਰ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ position (ਅਚਲ ਬਣਤਰ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਟੈਗ_ਗਿਣਤੀ, /* IN/OUT */
					     hb_ਟੈਗ_t     *ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ;

  ਜਨਤਕ:
  hb_ਟੈਗ_t chosen_script[2];
  ਤਾਰਕਿਕ ਮਿਲਿਆ_script[2];

  ਨਿਜੀ:

  hb_ਨਕਾਬ_t global_ਨਕਾਬ = 0;

  hb_sorted_ਸਦਿਸ਼_t<feature_map_t> features;
  hb_ਸਦਿਸ਼_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_ਸਦਿਸ਼_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

ਗਿਣਤੀ hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_ਖੋਜ	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_ਪ੍ਰਤੀ_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ਬਣਤਰ hb_ot_map_feature_t
{
  hb_ਟੈਗ_t ਟੈਗ_2;
  hb_ot_map_feature_flags_t flags;
};

ਬਣਤਰ hb_ot_shape_plan_ਕੁੰਜੀ_t;

ਬਣਤਰ hb_ot_map_builder_t
{
  ਜਨਤਕ:

  HB_ਅੰਦਰੂਨੀ hb_ot_map_builder_t (hb_face_t *face_,
				   ਅਚਲ hb_segment_properties_t &props_);

  HB_ਅੰਦਰੂਨੀ ~hb_ot_map_builder_t ();

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_feature (hb_ਟੈਗ_t ਟੈਗ_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ value=1);

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ has_feature (hb_ਟੈਗ_t ਟੈਗ_2);

  ਖਾਲੀ ਜੋੜੋ_feature (ਅਚਲ hb_ot_map_feature_t &feat)
  { ਜੋੜੋ_feature (feat.ਟੈਗ_2, feat.flags); }

  ਖਾਲੀ enable_feature (hb_ਟੈਗ_t ਟੈਗ_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ value=1)
  { ਜੋੜੋ_feature (ਟੈਗ_2, F_GLOBAL | flags, value); }

  ਖਾਲੀ disable_feature (hb_ਟੈਗ_t ਟੈਗ_2)
  { ਜੋੜੋ_feature (ਟੈਗ_2, F_GLOBAL, 0); }

  ਖਾਲੀ ਜੋੜੋ_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { ਜੋੜੋ_pause (0, pause_func); }
  ਖਾਲੀ ਜੋੜੋ_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { ਜੋੜੋ_pause (1, pause_func); }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ compile (hb_ot_map_t                  &m,
			    ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key);

  ਨਿਜੀ:

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_lookups (hb_ot_map_t  &m,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
				ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key,
				hb_ਨਕਾਬ_t     ਨਕਾਬ,
				ਤਾਰਕਿਕ          auto_zwnj = ਸੱਚ,
				ਤਾਰਕਿਕ          auto_zwj = ਸੱਚ,
				ਤਾਰਕਿਕ          random = ਝੂਠ,
				ਤਾਰਕਿਕ          ਪ੍ਰਤੀ_syllable = ਝੂਠ,
				hb_ਟੈਗ_t      feature_ਟੈਗ = HB_ਟੈਗ(' ',' ',' ',' '));

  ਬਣਤਰ feature_info_t {
    hb_ਟੈਗ_t ਟੈਗ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq; /* sequence#, used for stable sorting only */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_ਮੁੱਲ;
    hb_ot_map_feature_flags_t flags;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_ਮੁੱਲ; /* for non-global features, what should the unset glyphs take */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage[2]; /* GSUB/GPOS */

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
    {
      ਅਚਲ feature_info_t *a = (ਅਚਲ feature_info_t *) pa;
      ਅਚਲ feature_info_t *b = (ਅਚਲ feature_info_t *) pb;
      ਵਾਪਸ (a->ਟੈਗ_2 != b->ਟੈਗ_2) ?  (a->ਟੈਗ_2 < b->ਟੈਗ_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ਬਣਤਰ stage_info_t {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_pause (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ot_map_t::pause_func_t pause_func);

  ਜਨਤਕ:

  hb_face_t *face;
  hb_segment_properties_t props;
  ਤਾਰਕਿਕ is_simple;

  hb_ਟੈਗ_t chosen_script[2];
  ਤਾਰਕਿਕ ਮਿਲਿਆ_script[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਸੂਚਕਾਂਕ[2], language_ਸੂਚਕਾਂਕ[2];

  ਨਿਜੀ:

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ current_stage[2]; /* GSUB/GPOS */
  hb_ਸਦਿਸ਼_t<feature_info_t> feature_infos;
  hb_ਸਦਿਸ਼_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#ਜੇ_ਅੰਤ /* HB_OT_MAP_HH */
