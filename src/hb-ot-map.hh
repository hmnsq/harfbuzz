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

সংরচনা hb_ot_shape_plan_t;

স্থির ধ্রুব hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

সংরচনা hb_ot_map_t
{
  friend সংরচনা hb_ot_map_builder_t;

  সার্বজনিক:

  সংরচনা feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned পূর্ণাঙ্ক index[2]; /* GSUB/GPOS */
    unsigned পূর্ণাঙ্ক stage[2]; /* GSUB/GPOS */
    unsigned পূর্ণাঙ্ক shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned পূর্ণাঙ্ক needs_fallback : 1;
    unsigned পূর্ণাঙ্ক auto_zwnj : 1;
    unsigned পূর্ণাঙ্ক auto_zwj : 1;
    unsigned পূর্ণাঙ্ক random : 1;
    unsigned পূর্ণাঙ্ক per_syllable : 1;

    পূর্ণাঙ্ক cmp (ধ্রুব hb_tag_t tag_) ধ্রুব
    { প্রত্যাগম tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
    {
      ধ্রুব feature_map_t *a = (ধ্রুব feature_map_t *) pa;
      ধ্রুব feature_map_t *b = (ধ্রুব feature_map_t *) pb;
      প্রত্যাগম a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  সংরচনা lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
    {
      ধ্রুব lookup_map_t *a = (ধ্রুব lookup_map_t *) pa;
      ধ্রুব lookup_map_t *b = (ধ্রুব lookup_map_t *) pb;
      প্রত্যাগম a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  প্রকারনির্দেশ তার্কিক (*pause_func_t) (ধ্রুব সংরচনা hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  সংরচনা stage_map_t {
    unsigned পূর্ণাঙ্ক last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  শূন্য init ()
  {
    hb_memset (এতত্, 0, পরিমাণ (*এতত্));

    features.init0 ();
    প্রতি (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  শূন্য fini ()
  {
    features.fini ();
    প্রতি (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ধ্রুব { প্রত্যাগম global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned পূর্ণাঙ্ক *shift = nullptr) ধ্রুব
  {
    ধ্রুব feature_map_t *map = features.bsearch (feature_tag);
    যদি (shift) *shift = map ? map->shift : 0;
    প্রত্যাগম map ? map->mask : 0;
  }

  তার্কিক needs_fallback (hb_tag_t feature_tag) ধ্রুব
  {
    ধ্রুব feature_map_t *map = features.bsearch (feature_tag);
    প্রত্যাগম map ? map->needs_fallback : অসত্য;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ধ্রুব
  {
    ধ্রুব feature_map_t *map = features.bsearch (feature_tag);
    প্রত্যাগম map ? map->_1_mask : 0;
  }

  unsigned পূর্ণাঙ্ক get_feature_index (unsigned পূর্ণাঙ্ক table_index, hb_tag_t feature_tag) ধ্রুব
  {
    ধ্রুব feature_map_t *map = features.bsearch (feature_tag);
    প্রত্যাগম map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned পূর্ণাঙ্ক get_feature_stage (unsigned পূর্ণাঙ্ক table_index, hb_tag_t feature_tag) ধ্রুব
  {
    ধ্রুব feature_map_t *map = features.bsearch (feature_tag);
    প্রত্যাগম map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ধ্রুব hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned পূর্ণাঙ্ক table_index, unsigned পূর্ণাঙ্ক stage) ধ্রুব
  {
    যদি (unlikely (stage > stages[table_index].length))
      প্রত্যাগম hb_array<ধ্রুব hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned পূর্ণাঙ্ক start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned পূর্ণাঙ্ক end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    প্রত্যাগম lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL শূন্য collect_lookups (unsigned পূর্ণাঙ্ক table_index, hb_set_t *lookups) ধ্রুব;
  template <typename Proxy>
  HB_INTERNAL শূন্য apply (ধ্রুব Proxy &proxy,
			  ধ্রুব সংরচনা hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ধ্রুব;
  HB_INTERNAL শূন্য substitute (ধ্রুব সংরচনা hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ধ্রুব;
  HB_INTERNAL শূন্য position (ধ্রুব সংরচনা hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ধ্রুব;
  HB_INTERNAL unsigned পূর্ণাঙ্ক get_feature_tags (unsigned পূর্ণাঙ্ক  start_offset,
					     unsigned পূর্ণাঙ্ক *tag_গুনতি, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ধ্রুব;

  সার্বজনিক:
  hb_tag_t chosen_script[2];
  তার্কিক found_script[2];

  গুপ্ত:

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


সংরচনা hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

সংরচনা hb_ot_shape_plan_চাবি_t;

সংরচনা hb_ot_map_builder_t
{
  সার্বজনিক:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ধ্রুব hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL শূন্য add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned পূর্ণাঙ্ক value=1);

  HB_INTERNAL তার্কিক has_feature (hb_tag_t tag);

  শূন্য add_feature (ধ্রুব hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  শূন্য enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned পূর্ণাঙ্ক value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  শূন্য disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  শূন্য add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  শূন্য add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL শূন্য compile (hb_ot_map_t                  &m,
			    ধ্রুব hb_ot_shape_plan_চাবি_t &key);

  গুপ্ত:

  HB_INTERNAL শূন্য add_lookups (hb_ot_map_t  &m,
				unsigned পূর্ণাঙ্ক  table_index,
				unsigned পূর্ণাঙ্ক  feature_index,
				ধ্রুব hb_ot_shape_plan_চাবি_t &key,
				hb_mask_t     mask,
				তার্কিক          auto_zwnj = সত্য,
				তার্কিক          auto_zwj = সত্য,
				তার্কিক          random = অসত্য,
				তার্কিক          per_syllable = অসত্য,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  সংরচনা feature_info_t {
    hb_tag_t tag;
    unsigned পূর্ণাঙ্ক seq; /* sequence#, used for stable sorting only */
    unsigned পূর্ণাঙ্ক max_মান;
    hb_ot_map_feature_flags_t flags;
    unsigned পূর্ণাঙ্ক default_মান; /* for non-global features, what should the unset glyphs take */
    unsigned পূর্ণাঙ্ক stage[2]; /* GSUB/GPOS */

    HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
    {
      ধ্রুব feature_info_t *a = (ধ্রুব feature_info_t *) pa;
      ধ্রুব feature_info_t *b = (ধ্রুব feature_info_t *) pb;
      প্রত্যাগম (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  সংরচনা stage_info_t {
    unsigned পূর্ণাঙ্ক index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL শূন্য add_pause (unsigned পূর্ণাঙ্ক table_index, hb_ot_map_t::pause_func_t pause_func);

  সার্বজনিক:

  hb_face_t *face;
  hb_segment_properties_t props;
  তার্কিক is_simple;

  hb_tag_t chosen_script[2];
  তার্কিক found_script[2];
  unsigned পূর্ণাঙ্ক script_index[2], language_index[2];

  গুপ্ত:

  unsigned পূর্ণাঙ্ক current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
