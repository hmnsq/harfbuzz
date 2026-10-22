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

#अन्तर्भाव "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_मूल्य ((1u << HB_OT_MAP_MAX_BITS) - 1u)

संरचना hb_ot_shape_plan_t;

स्थिर ध्रुव hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

संरचना hb_ot_map_t
{
  friend संरचना hb_ot_map_builder_t;

  सार्वजनिक:

  संरचना feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned पूर्णाङ्क index[2]; /* GSUB/GPOS */
    unsigned पूर्णाङ्क stage[2]; /* GSUB/GPOS */
    unsigned पूर्णाङ्क shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned पूर्णाङ्क needs_fallback : 1;
    unsigned पूर्णाङ्क auto_zwnj : 1;
    unsigned पूर्णाङ्क auto_zwj : 1;
    unsigned पूर्णाङ्क random : 1;
    unsigned पूर्णाङ्क per_syllable : 1;

    पूर्णाङ्क cmp (ध्रुव hb_tag_t tag_) ध्रुव
    { प्रत्यागम tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
    {
      ध्रुव feature_map_t *a = (ध्रुव feature_map_t *) pa;
      ध्रुव feature_map_t *b = (ध्रुव feature_map_t *) pb;
      प्रत्यागम a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  संरचना lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
    {
      ध्रुव lookup_map_t *a = (ध्रुव lookup_map_t *) pa;
      ध्रुव lookup_map_t *b = (ध्रुव lookup_map_t *) pb;
      प्रत्यागम a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  प्रकारनिर्देश तार्किक (*pause_func_t) (ध्रुव संरचना hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  संरचना stage_map_t {
    unsigned पूर्णाङ्क last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  शून्य init ()
  {
    hb_memset (एतत्, 0, परिमाण (*एतत्));

    features.init0 ();
    प्रति (unsigned पूर्णाङ्क table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  शून्य fini ()
  {
    features.fini ();
    प्रति (unsigned पूर्णाङ्क table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ध्रुव { प्रत्यागम global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned पूर्णाङ्क *shift = nullptr) ध्रुव
  {
    ध्रुव feature_map_t *map = features.bsearch (feature_tag);
    यदि (shift) *shift = map ? map->shift : 0;
    प्रत्यागम map ? map->mask : 0;
  }

  तार्किक needs_fallback (hb_tag_t feature_tag) ध्रुव
  {
    ध्रुव feature_map_t *map = features.bsearch (feature_tag);
    प्रत्यागम map ? map->needs_fallback : असत्य;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ध्रुव
  {
    ध्रुव feature_map_t *map = features.bsearch (feature_tag);
    प्रत्यागम map ? map->_1_mask : 0;
  }

  unsigned पूर्णाङ्क get_feature_index (unsigned पूर्णाङ्क table_index, hb_tag_t feature_tag) ध्रुव
  {
    ध्रुव feature_map_t *map = features.bsearch (feature_tag);
    प्रत्यागम map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned पूर्णाङ्क get_feature_stage (unsigned पूर्णाङ्क table_index, hb_tag_t feature_tag) ध्रुव
  {
    ध्रुव feature_map_t *map = features.bsearch (feature_tag);
    प्रत्यागम map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ध्रुव hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned पूर्णाङ्क table_index, unsigned पूर्णाङ्क stage) ध्रुव
  {
    यदि (unlikely (stage > stages[table_index].length))
      प्रत्यागम hb_array<ध्रुव hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned पूर्णाङ्क start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned पूर्णाङ्क end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    प्रत्यागम lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL शून्य collect_lookups (unsigned पूर्णाङ्क table_index, hb_set_t *lookups) ध्रुव;
  template <typename Proxy>
  HB_INTERNAL शून्य apply (ध्रुव Proxy &proxy,
			  ध्रुव संरचना hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ध्रुव;
  HB_INTERNAL शून्य substitute (ध्रुव संरचना hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ध्रुव;
  HB_INTERNAL शून्य position (ध्रुव संरचना hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ध्रुव;
  HB_INTERNAL unsigned पूर्णाङ्क get_feature_tags (unsigned पूर्णाङ्क  start_offset,
					     unsigned पूर्णाङ्क *tag_मोजणी, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ध्रुव;

  सार्वजनिक:
  hb_tag_t chosen_script[2];
  तार्किक found_script[2];

  गुप्त:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

गणना hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_शोध	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


संरचना hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

संरचना hb_ot_shape_plan_किल्ली_t;

संरचना hb_ot_map_builder_t
{
  सार्वजनिक:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ध्रुव hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL शून्य add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned पूर्णाङ्क value=1);

  HB_INTERNAL तार्किक has_feature (hb_tag_t tag);

  शून्य add_feature (ध्रुव hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  शून्य enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned पूर्णाङ्क value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  शून्य disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  शून्य add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  शून्य add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL शून्य compile (hb_ot_map_t                  &m,
			    ध्रुव hb_ot_shape_plan_किल्ली_t &key);

  गुप्त:

  HB_INTERNAL शून्य add_lookups (hb_ot_map_t  &m,
				unsigned पूर्णाङ्क  table_index,
				unsigned पूर्णाङ्क  feature_index,
				ध्रुव hb_ot_shape_plan_किल्ली_t &key,
				hb_mask_t     mask,
				तार्किक          auto_zwnj = सत्य,
				तार्किक          auto_zwj = सत्य,
				तार्किक          random = असत्य,
				तार्किक          per_syllable = असत्य,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  संरचना feature_info_t {
    hb_tag_t tag;
    unsigned पूर्णाङ्क seq; /* sequence#, used for stable sorting only */
    unsigned पूर्णाङ्क max_मूल्य;
    hb_ot_map_feature_flags_t flags;
    unsigned पूर्णाङ्क default_मूल्य; /* for non-global features, what should the unset glyphs take */
    unsigned पूर्णाङ्क stage[2]; /* GSUB/GPOS */

    HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
    {
      ध्रुव feature_info_t *a = (ध्रुव feature_info_t *) pa;
      ध्रुव feature_info_t *b = (ध्रुव feature_info_t *) pb;
      प्रत्यागम (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  संरचना stage_info_t {
    unsigned पूर्णाङ्क index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL शून्य add_pause (unsigned पूर्णाङ्क table_index, hb_ot_map_t::pause_func_t pause_func);

  सार्वजनिक:

  hb_face_t *face;
  hb_segment_properties_t props;
  तार्किक is_simple;

  hb_tag_t chosen_script[2];
  तार्किक found_script[2];
  unsigned पूर्णाङ्क script_index[2], language_index[2];

  गुप्त:

  unsigned पूर्णाङ्क current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
