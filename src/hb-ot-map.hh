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

#அந்தர்பாவ "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_மதிப்பு ((1u << HB_OT_MAP_MAX_BITS) - 1u)

கட்டமைப்பு hb_ot_shape_plan_t;

நிலையான மாறிலி hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

கட்டமைப்பு hb_ot_map_t
{
  friend கட்டமைப்பு hb_ot_map_builder_t;

  பொது:

  கட்டமைப்பு feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned முழு index[2]; /* GSUB/GPOS */
    unsigned முழு stage[2]; /* GSUB/GPOS */
    unsigned முழு shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned முழு needs_fallback : 1;
    unsigned முழு auto_zwnj : 1;
    unsigned முழு auto_zwj : 1;
    unsigned முழு random : 1;
    unsigned முழு per_syllable : 1;

    முழு cmp (மாறிலி hb_tag_t tag_) மாறிலி
    { திரும்பு tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
    {
      மாறிலி feature_map_t *a = (மாறிலி feature_map_t *) pa;
      மாறிலி feature_map_t *b = (மாறிலி feature_map_t *) pb;
      திரும்பு a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  கட்டமைப்பு lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
    {
      மாறிலி lookup_map_t *a = (மாறிலி lookup_map_t *) pa;
      மாறிலி lookup_map_t *b = (மாறிலி lookup_map_t *) pb;
      திரும்பு a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  வகைவரை தருக்க (*pause_func_t) (மாறிலி கட்டமைப்பு hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  கட்டமைப்பு stage_map_t {
    unsigned முழு last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  வெறுமை init ()
  {
    hb_memset (இது, 0, அளவு (*இது));

    features.init0 ();
    க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  வெறுமை fini ()
  {
    features.fini ();
    க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () மாறிலி { திரும்பு global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned முழு *shift = nullptr) மாறிலி
  {
    மாறிலி feature_map_t *map = features.bsearch (feature_tag);
    எனில் (shift) *shift = map ? map->shift : 0;
    திரும்பு map ? map->mask : 0;
  }

  தருக்க needs_fallback (hb_tag_t feature_tag) மாறிலி
  {
    மாறிலி feature_map_t *map = features.bsearch (feature_tag);
    திரும்பு map ? map->needs_fallback : பொய்;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) மாறிலி
  {
    மாறிலி feature_map_t *map = features.bsearch (feature_tag);
    திரும்பு map ? map->_1_mask : 0;
  }

  unsigned முழு get_feature_index (unsigned முழு table_index, hb_tag_t feature_tag) மாறிலி
  {
    மாறிலி feature_map_t *map = features.bsearch (feature_tag);
    திரும்பு map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned முழு get_feature_stage (unsigned முழு table_index, hb_tag_t feature_tag) மாறிலி
  {
    மாறிலி feature_map_t *map = features.bsearch (feature_tag);
    திரும்பு map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<மாறிலி hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned முழு table_index, unsigned முழு stage) மாறிலி
  {
    எனில் (unlikely (stage > stages[table_index].length))
      திரும்பு hb_array<மாறிலி hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned முழு start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned முழு end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    திரும்பு lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL வெறுமை collect_lookups (unsigned முழு table_index, hb_set_t *lookups) மாறிலி;
  template <typename Proxy>
  HB_INTERNAL வெறுமை apply (மாறிலி Proxy &proxy,
			  மாறிலி கட்டமைப்பு hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) மாறிலி;
  HB_INTERNAL வெறுமை substitute (மாறிலி கட்டமைப்பு hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) மாறிலி;
  HB_INTERNAL வெறுமை position (மாறிலி கட்டமைப்பு hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) மாறிலி;
  HB_INTERNAL unsigned முழு get_feature_tags (unsigned முழு  start_offset,
					     unsigned முழு *tag_மொத்தம், /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) மாறிலி;

  பொது:
  hb_tag_t chosen_script[2];
  தருக்க found_script[2];

  தனிப்பட்ட:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

எண்ணிக்கை hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_தேடல்	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


கட்டமைப்பு hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

கட்டமைப்பு hb_ot_shape_plan_சாவி_t;

கட்டமைப்பு hb_ot_map_builder_t
{
  பொது:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   மாறிலி hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL வெறுமை add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned முழு value=1);

  HB_INTERNAL தருக்க has_feature (hb_tag_t tag);

  வெறுமை add_feature (மாறிலி hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  வெறுமை enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned முழு value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  வெறுமை disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  வெறுமை add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  வெறுமை add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL வெறுமை compile (hb_ot_map_t                  &m,
			    மாறிலி hb_ot_shape_plan_சாவி_t &key);

  தனிப்பட்ட:

  HB_INTERNAL வெறுமை add_lookups (hb_ot_map_t  &m,
				unsigned முழு  table_index,
				unsigned முழு  feature_index,
				மாறிலி hb_ot_shape_plan_சாவி_t &key,
				hb_mask_t     mask,
				தருக்க          auto_zwnj = உண்மை,
				தருக்க          auto_zwj = உண்மை,
				தருக்க          random = பொய்,
				தருக்க          per_syllable = பொய்,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  கட்டமைப்பு feature_info_t {
    hb_tag_t tag;
    unsigned முழு seq; /* sequence#, used for stable sorting only */
    unsigned முழு max_மதிப்பு;
    hb_ot_map_feature_flags_t flags;
    unsigned முழு default_மதிப்பு; /* for non-global features, what should the unset glyphs take */
    unsigned முழு stage[2]; /* GSUB/GPOS */

    HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
    {
      மாறிலி feature_info_t *a = (மாறிலி feature_info_t *) pa;
      மாறிலி feature_info_t *b = (மாறிலி feature_info_t *) pb;
      திரும்பு (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  கட்டமைப்பு stage_info_t {
    unsigned முழு index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL வெறுமை add_pause (unsigned முழு table_index, hb_ot_map_t::pause_func_t pause_func);

  பொது:

  hb_face_t *face;
  hb_segment_properties_t props;
  தருக்க is_simple;

  hb_tag_t chosen_script[2];
  தருக்க found_script[2];
  unsigned முழு script_index[2], language_index[2];

  தனிப்பட்ட:

  unsigned முழு current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
