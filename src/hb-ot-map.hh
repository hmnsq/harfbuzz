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

#شامل "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_مقدار ((1u << HB_OT_MAP_MAX_BITS) - 1u)

ساختار hb_ot_shape_plan_t;

ایستا ثابت hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

ساختار hb_ot_map_t
{
  friend ساختار hb_ot_map_builder_t;

  عمومی:

  ساختار feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned صحیح index[2]; /* GSUB/GPOS */
    unsigned صحیح stage[2]; /* GSUB/GPOS */
    unsigned صحیح shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned صحیح needs_fallback : 1;
    unsigned صحیح auto_zwnj : 1;
    unsigned صحیح auto_zwj : 1;
    unsigned صحیح random : 1;
    unsigned صحیح per_syllable : 1;

    صحیح cmp (ثابت hb_tag_t tag_) ثابت
    { بازگشت tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      بازگشت a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  ساختار lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      بازگشت a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعریفنوع منطقی (*pause_func_t) (ثابت ساختار hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  ساختار stage_map_t {
    unsigned صحیح last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  تهی init ()
  {
    hb_memset (این, 0, اندازه (*این));

    features.init0 ();
    برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  تهی fini ()
  {
    features.fini ();
    برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ثابت { بازگشت global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned صحیح *shift = nullptr) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    اگر (shift) *shift = map ? map->shift : 0;
    بازگشت map ? map->mask : 0;
  }

  منطقی needs_fallback (hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    بازگشت map ? map->needs_fallback : نادرست;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    بازگشت map ? map->_1_mask : 0;
  }

  unsigned صحیح get_feature_index (unsigned صحیح table_index, hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    بازگشت map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned صحیح get_feature_stage (unsigned صحیح table_index, hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    بازگشت map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned صحیح table_index, unsigned صحیح stage) ثابت
  {
    اگر (unlikely (stage > stages[table_index].length))
      بازگشت hb_array<ثابت hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned صحیح start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned صحیح end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    بازگشت lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL تهی collect_lookups (unsigned صحیح table_index, hb_set_t *lookups) ثابت;
  template <typename Proxy>
  HB_INTERNAL تهی apply (ثابت Proxy &proxy,
			  ثابت ساختار hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL تهی substitute (ثابت ساختار hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL تهی position (ثابت ساختار hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL unsigned صحیح get_feature_tags (unsigned صحیح  start_offset,
					     unsigned صحیح *tag_شمار, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ثابت;

  عمومی:
  hb_tag_t chosen_script[2];
  منطقی found_script[2];

  خصوصی:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

شمارش hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_جستجو	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ساختار hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

ساختار hb_ot_shape_plan_کلید_t;

ساختار hb_ot_map_builder_t
{
  عمومی:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL تهی add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned صحیح value=1);

  HB_INTERNAL منطقی has_feature (hb_tag_t tag);

  تهی add_feature (ثابت hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  تهی enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned صحیح value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  تهی disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  تهی add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  تهی add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL تهی compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_کلید_t &key);

  خصوصی:

  HB_INTERNAL تهی add_lookups (hb_ot_map_t  &m,
				unsigned صحیح  table_index,
				unsigned صحیح  feature_index,
				ثابت hb_ot_shape_plan_کلید_t &key,
				hb_mask_t     mask,
				منطقی          auto_zwnj = درست,
				منطقی          auto_zwj = درست,
				منطقی          random = نادرست,
				منطقی          per_syllable = نادرست,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  ساختار feature_info_t {
    hb_tag_t tag;
    unsigned صحیح seq; /* sequence#, used for stable sorting only */
    unsigned صحیح max_مقدار;
    hb_ot_map_feature_flags_t flags;
    unsigned صحیح default_مقدار; /* for non-global features, what should the unset glyphs take */
    unsigned صحیح stage[2]; /* GSUB/GPOS */

    HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      بازگشت (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ساختار stage_info_t {
    unsigned صحیح index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL تهی add_pause (unsigned صحیح table_index, hb_ot_map_t::pause_func_t pause_func);

  عمومی:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقی is_simple;

  hb_tag_t chosen_script[2];
  منطقی found_script[2];
  unsigned صحیح script_index[2], language_index[2];

  خصوصی:

  unsigned صحیح current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
