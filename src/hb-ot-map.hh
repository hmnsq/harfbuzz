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

#ଅନ୍ତର୍ଭାଵ "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_ମୂଲ୍ୟ ((1u << HB_OT_MAP_MAX_BITS) - 1u)

ସଂରଚନା hb_ot_shape_plan_t;

ସ୍ଥିର ଅଚଳ hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

ସଂରଚନା hb_ot_map_t
{
  friend ସଂରଚନା hb_ot_map_builder_t;

  ସର୍ବସାଧାରଣ:

  ସଂରଚନା feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index[2]; /* GSUB/GPOS */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage[2]; /* GSUB/GPOS */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା needs_fallback : 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_zwnj : 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_zwj : 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା random : 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା per_syllable : 1;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_tag_t tag_) ଅଚଳ
    { ଫେରନ୍ତୁ tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
    {
      ଅଚଳ feature_map_t *a = (ଅଚଳ feature_map_t *) pa;
      ଅଚଳ feature_map_t *b = (ଅଚଳ feature_map_t *) pb;
      ଫେରନ୍ତୁ a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  ସଂରଚନା lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
    {
      ଅଚଳ lookup_map_t *a = (ଅଚଳ lookup_map_t *) pa;
      ଅଚଳ lookup_map_t *b = (ଅଚଳ lookup_map_t *) pb;
      ଫେରନ୍ତୁ a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*pause_func_t) (ଅଚଳ ସଂରଚନା hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  ସଂରଚନା stage_map_t {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  ଶୂନ୍ୟ init ()
  {
    hb_memset (ଏହି, 0, ଆକାର (*ଏହି));

    features.init0 ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  ଶୂନ୍ୟ fini ()
  {
    features.fini ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ଅଚଳ { ଫେରନ୍ତୁ global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *shift = nullptr) ଅଚଳ
  {
    ଅଚଳ feature_map_t *map = features.bsearch (feature_tag);
    ଯଦି (shift) *shift = map ? map->shift : 0;
    ଫେରନ୍ତୁ map ? map->mask : 0;
  }

  ତାର୍କିକ needs_fallback (hb_tag_t feature_tag) ଅଚଳ
  {
    ଅଚଳ feature_map_t *map = features.bsearch (feature_tag);
    ଫେରନ୍ତୁ map ? map->needs_fallback : ମିଥ୍ୟା;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ଅଚଳ
  {
    ଅଚଳ feature_map_t *map = features.bsearch (feature_tag);
    ଫେରନ୍ତୁ map ? map->_1_mask : 0;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index, hb_tag_t feature_tag) ଅଚଳ
  {
    ଅଚଳ feature_map_t *map = features.bsearch (feature_tag);
    ଫେରନ୍ତୁ map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_stage (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index, hb_tag_t feature_tag) ଅଚଳ
  {
    ଅଚଳ feature_map_t *map = features.bsearch (feature_tag);
    ଫେରନ୍ତୁ map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ଅଚଳ hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage) ଅଚଳ
  {
    ଯଦି (unlikely (stage > stages[table_index].length))
      ଫେରନ୍ତୁ hb_array<ଅଚଳ hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    ଫେରନ୍ତୁ lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL ଶୂନ୍ୟ collect_lookups (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index, hb_set_t *lookups) ଅଚଳ;
  template <typename Proxy>
  HB_INTERNAL ଶୂନ୍ୟ apply (ଅଚଳ Proxy &proxy,
			  ଅଚଳ ସଂରଚନା hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ substitute (ଅଚଳ ସଂରଚନା hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ position (ଅଚଳ ସଂରଚନା hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ଅଚଳ;
  HB_INTERNAL unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  start_offset,
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *tag_ଗଣନା, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ଅଚଳ;

  ସର୍ବସାଧାରଣ:
  hb_tag_t chosen_script[2];
  ତାର୍କିକ found_script[2];

  ବ୍ୟକ୍ତିଗତ:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

ଗଣନା hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_ସନ୍ଧାନ	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ସଂରଚନା hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

ସଂରଚନା hb_ot_shape_plan_ଚାବି_t;

ସଂରଚନା hb_ot_map_builder_t
{
  ସର୍ବସାଧାରଣ:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ଅଚଳ hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL ଶୂନ୍ୟ add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା value=1);

  HB_INTERNAL ତାର୍କିକ has_feature (hb_tag_t tag);

  ଶୂନ୍ୟ add_feature (ଅଚଳ hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  ଶୂନ୍ୟ enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  ଶୂନ୍ୟ disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  ଶୂନ୍ୟ add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  ଶୂନ୍ୟ add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL ଶୂନ୍ୟ compile (hb_ot_map_t                  &m,
			    ଅଚଳ hb_ot_shape_plan_ଚାବି_t &key);

  ବ୍ୟକ୍ତିଗତ:

  HB_INTERNAL ଶୂନ୍ୟ add_lookups (hb_ot_map_t  &m,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  table_index,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  feature_index,
				ଅଚଳ hb_ot_shape_plan_ଚାବି_t &key,
				hb_mask_t     mask,
				ତାର୍କିକ          auto_zwnj = ସତ୍ୟ,
				ତାର୍କିକ          auto_zwj = ସତ୍ୟ,
				ତାର୍କିକ          random = ମିଥ୍ୟା,
				ତାର୍କିକ          per_syllable = ମିଥ୍ୟା,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  ସଂରଚନା feature_info_t {
    hb_tag_t tag;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seq; /* sequence#, used for stable sorting only */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_ମୂଲ୍ୟ;
    hb_ot_map_feature_flags_t flags;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_ମୂଲ୍ୟ; /* for non-global features, what should the unset glyphs take */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage[2]; /* GSUB/GPOS */

    HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
    {
      ଅଚଳ feature_info_t *a = (ଅଚଳ feature_info_t *) pa;
      ଅଚଳ feature_info_t *b = (ଅଚଳ feature_info_t *) pb;
      ଫେରନ୍ତୁ (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ସଂରଚନା stage_info_t {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL ଶୂନ୍ୟ add_pause (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index, hb_ot_map_t::pause_func_t pause_func);

  ସର୍ବସାଧାରଣ:

  hb_face_t *face;
  hb_segment_properties_t props;
  ତାର୍କିକ is_simple;

  hb_tag_t chosen_script[2];
  ତାର୍କିକ found_script[2];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା script_index[2], language_index[2];

  ବ୍ୟକ୍ତିଗତ:

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
