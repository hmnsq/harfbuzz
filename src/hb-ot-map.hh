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

#ifndef HB_OT_MAP_HH
#define HB_OT_MAP_HH

#অন্তর্ভাব "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_মান ((1u << HB_OT_MAP_MAX_BITS) - 1u)

কাঠামো hb_ot_shape_plan_t;

স্থির ধ্রুবক hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

কাঠামো hb_ot_map_t
{
  friend কাঠামো hb_ot_map_builder_t;

  সর্বজনীন:

  কাঠামো feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned পূর্ণসংখ্যা সূচক_2[2]; /* GSUB/GPOS */
    unsigned পূর্ণসংখ্যা stage[2]; /* GSUB/GPOS */
    unsigned পূর্ণসংখ্যা shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned পূর্ণসংখ্যা needs_fallback : 1;
    unsigned পূর্ণসংখ্যা auto_zwnj : 1;
    unsigned পূর্ণসংখ্যা auto_zwj : 1;
    unsigned পূর্ণসংখ্যা random : 1;
    unsigned পূর্ণসংখ্যা per_syllable : 1;

    পূর্ণসংখ্যা cmp (ধ্রুবক hb_tag_t tag_) ধ্রুবক
    { ফেরত tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
    {
      ধ্রুবক feature_map_t *a = (ধ্রুবক feature_map_t *) pa;
      ধ্রুবক feature_map_t *b = (ধ্রুবক feature_map_t *) pb;
      ফেরত a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  কাঠামো lookup_map_t {
    unsigned short সূচক_2;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
    {
      ধ্রুবক lookup_map_t *a = (ধ্রুবক lookup_map_t *) pa;
      ধ্রুবক lookup_map_t *b = (ধ্রুবক lookup_map_t *) pb;
      ফেরত a->সূচক_2 < b->সূচক_2 ? -1 : a->সূচক_2 > b->সূচক_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  প্রকারলেখ যৌক্তিক (*pause_func_t) (ধ্রুবক কাঠামো hb_ot_shape_plan_t *plan, hb_font_t *font, hb_বাফার_t *বাফার);

  কাঠামো stage_map_t {
    unsigned পূর্ণসংখ্যা last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  শূন্য init ()
  {
    hb_memset (এই, 0, আকার (*এই));

    features.init0 ();
    জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    {
      lookups[table_সূচক].init0 ();
      stages[table_সূচক].init0 ();
    }
  }
  শূন্য fini ()
  {
    features.fini ();
    জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    {
      lookups[table_সূচক].fini ();
      stages[table_সূচক].fini ();
    }
  }

  hb_mask_t get_global_mask () ধ্রুবক { ফেরত global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned পূর্ণসংখ্যা *shift = nullptr) ধ্রুবক
  {
    ধ্রুবক feature_map_t *map = features.bsearch (feature_tag);
    যদি (shift) *shift = map ? map->shift : 0;
    ফেরত map ? map->mask : 0;
  }

  যৌক্তিক needs_fallback (hb_tag_t feature_tag) ধ্রুবক
  {
    ধ্রুবক feature_map_t *map = features.bsearch (feature_tag);
    ফেরত map ? map->needs_fallback : মিথ্যা;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ধ্রুবক
  {
    ধ্রুবক feature_map_t *map = features.bsearch (feature_tag);
    ফেরত map ? map->_1_mask : 0;
  }

  unsigned পূর্ণসংখ্যা get_feature_সূচক (unsigned পূর্ণসংখ্যা table_সূচক, hb_tag_t feature_tag) ধ্রুবক
  {
    ধ্রুবক feature_map_t *map = features.bsearch (feature_tag);
    ফেরত map ? map->সূচক_2[table_সূচক] : HB_OT_LAYOUT_NO_FEATURE_সূচক;
  }

  unsigned পূর্ণসংখ্যা get_feature_stage (unsigned পূর্ণসংখ্যা table_সূচক, hb_tag_t feature_tag) ধ্রুবক
  {
    ধ্রুবক feature_map_t *map = features.bsearch (feature_tag);
    ফেরত map ? map->stage[table_সূচক] : UINT_MAX;
  }

  hb_array_t<ধ্রুবক hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned পূর্ণসংখ্যা table_সূচক, unsigned পূর্ণসংখ্যা stage) ধ্রুবক
  {
    যদি (unlikely (stage > stages[table_সূচক].দৈর্ঘ্য))
      ফেরত hb_array<ধ্রুবক hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned পূর্ণসংখ্যা start = stage ? stages[table_সূচক][stage - 1].last_lookup : 0;
    unsigned পূর্ণসংখ্যা end   = stage < stages[table_সূচক].দৈর্ঘ্য ? stages[table_সূচক][stage].last_lookup : lookups[table_সূচক].দৈর্ঘ্য;
    ফেরত lookups[table_সূচক].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL শূন্য collect_lookups (unsigned পূর্ণসংখ্যা table_সূচক, hb_set_t *lookups) ধ্রুবক;
  template <typename Proxy>
  HB_INTERNAL শূন্য apply (ধ্রুবক Proxy &proxy,
			  ধ্রুবক কাঠামো hb_ot_shape_plan_t *plan, hb_font_t *font, hb_বাফার_t *বাফার) ধ্রুবক;
  HB_INTERNAL শূন্য substitute (ধ্রুবক কাঠামো hb_ot_shape_plan_t *plan, hb_font_t *font, hb_বাফার_t *বাফার) ধ্রুবক;
  HB_INTERNAL শূন্য position (ধ্রুবক কাঠামো hb_ot_shape_plan_t *plan, hb_font_t *font, hb_বাফার_t *বাফার) ধ্রুবক;
  HB_INTERNAL unsigned পূর্ণসংখ্যা get_feature_tags (unsigned পূর্ণসংখ্যা  start_offset,
					     unsigned পূর্ণসংখ্যা *tag_গুনতি, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ধ্রুবক;

  সর্বজনীন:
  hb_tag_t chosen_script[2];
  যৌক্তিক found_script[2];

  ব্যক্তিগত:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

গণনা hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_অনুসন্ধান	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


কাঠামো hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

কাঠামো hb_ot_shape_plan_চাবি_t;

কাঠামো hb_ot_map_builder_t
{
  সর্বজনীন:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ধ্রুবক hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL শূন্য add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned পূর্ণসংখ্যা value=1);

  HB_INTERNAL যৌক্তিক has_feature (hb_tag_t tag);

  শূন্য add_feature (ধ্রুবক hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  শূন্য enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned পূর্ণসংখ্যা value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  শূন্য disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  শূন্য add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  শূন্য add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL শূন্য compile (hb_ot_map_t                  &m,
			    ধ্রুবক hb_ot_shape_plan_চাবি_t &key);

  ব্যক্তিগত:

  HB_INTERNAL শূন্য add_lookups (hb_ot_map_t  &m,
				unsigned পূর্ণসংখ্যা  table_সূচক,
				unsigned পূর্ণসংখ্যা  feature_সূচক,
				ধ্রুবক hb_ot_shape_plan_চাবি_t &key,
				hb_mask_t     mask,
				যৌক্তিক          auto_zwnj = সত্য,
				যৌক্তিক          auto_zwj = সত্য,
				যৌক্তিক          random = মিথ্যা,
				যৌক্তিক          per_syllable = মিথ্যা,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  কাঠামো feature_info_t {
    hb_tag_t tag;
    unsigned পূর্ণসংখ্যা seq; /* sequence#, used for stable sorting only */
    unsigned পূর্ণসংখ্যা max_মান;
    hb_ot_map_feature_flags_t flags;
    unsigned পূর্ণসংখ্যা default_মান; /* for non-global features, what should the unset glyphs take */
    unsigned পূর্ণসংখ্যা stage[2]; /* GSUB/GPOS */

    HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
    {
      ধ্রুবক feature_info_t *a = (ধ্রুবক feature_info_t *) pa;
      ধ্রুবক feature_info_t *b = (ধ্রুবক feature_info_t *) pb;
      ফেরত (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  কাঠামো stage_info_t {
    unsigned পূর্ণসংখ্যা সূচক_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL শূন্য add_pause (unsigned পূর্ণসংখ্যা table_সূচক, hb_ot_map_t::pause_func_t pause_func);

  সর্বজনীন:

  hb_face_t *face;
  hb_segment_properties_t props;
  যৌক্তিক is_simple;

  hb_tag_t chosen_script[2];
  যৌক্তিক found_script[2];
  unsigned পূর্ণসংখ্যা script_সূচক[2], language_সূচক[2];

  ব্যক্তিগত:

  unsigned পূর্ণসংখ্যা current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
