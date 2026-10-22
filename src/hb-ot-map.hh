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

#ಅನ್ತರ್ಭಾವ "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_ಮೌಲ್ಯ ((1u << HB_OT_MAP_MAX_BITS) - 1u)

ಸಂರಚನಾ hb_ot_shape_plan_t;

ಸ್ಥಿರ ಧ್ರುವ hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

ಸಂರಚನಾ hb_ot_map_t
{
  friend ಸಂರಚನಾ hb_ot_map_builder_t;

  ಸಾರ್ವಜನಿಕ:

  ಸಂರಚನಾ feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned ಪೂರ್ಣಾಙ್ಕ index[2]; /* GSUB/GPOS */
    unsigned ಪೂರ್ಣಾಙ್ಕ stage[2]; /* GSUB/GPOS */
    unsigned ಪೂರ್ಣಾಙ್ಕ shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned ಪೂರ್ಣಾಙ್ಕ needs_fallback : 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ auto_zwnj : 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ auto_zwj : 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ random : 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ per_syllable : 1;

    ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_tag_t tag_) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
    {
      ಧ್ರುವ feature_map_t *a = (ಧ್ರುವ feature_map_t *) pa;
      ಧ್ರುವ feature_map_t *b = (ಧ್ರುವ feature_map_t *) pb;
      ಪ್ರತ್ಯಾಗಮ a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  ಸಂರಚನಾ lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
    {
      ಧ್ರುವ lookup_map_t *a = (ಧ್ರುವ lookup_map_t *) pa;
      ಧ್ರುವ lookup_map_t *b = (ಧ್ರುವ lookup_map_t *) pb;
      ಪ್ರತ್ಯಾಗಮ a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*pause_func_t) (ಧ್ರುವ ಸಂರಚನಾ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  ಸಂರಚನಾ stage_map_t {
    unsigned ಪೂರ್ಣಾಙ್ಕ last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  ಶೂನ್ಯ init ()
  {
    hb_memset (ಏತತ್, 0, ಪರಿಮಾಣ (*ಏತತ್));

    features.init0 ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  ಶೂನ್ಯ fini ()
  {
    features.fini ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned ಪೂರ್ಣಾಙ್ಕ *shift = nullptr) ಧ್ರುವ
  {
    ಧ್ರುವ feature_map_t *map = features.bsearch (feature_tag);
    ಯದಿ (shift) *shift = map ? map->shift : 0;
    ಪ್ರತ್ಯಾಗಮ map ? map->mask : 0;
  }

  ತಾರ್ಕಿಕ needs_fallback (hb_tag_t feature_tag) ಧ್ರುವ
  {
    ಧ್ರುವ feature_map_t *map = features.bsearch (feature_tag);
    ಪ್ರತ್ಯಾಗಮ map ? map->needs_fallback : ಅಸತ್ಯ;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ಧ್ರುವ
  {
    ಧ್ರುವ feature_map_t *map = features.bsearch (feature_tag);
    ಪ್ರತ್ಯಾಗಮ map ? map->_1_mask : 0;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_index (unsigned ಪೂರ್ಣಾಙ್ಕ table_index, hb_tag_t feature_tag) ಧ್ರುವ
  {
    ಧ್ರುವ feature_map_t *map = features.bsearch (feature_tag);
    ಪ್ರತ್ಯಾಗಮ map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_stage (unsigned ಪೂರ್ಣಾಙ್ಕ table_index, hb_tag_t feature_tag) ಧ್ರುವ
  {
    ಧ್ರುವ feature_map_t *map = features.bsearch (feature_tag);
    ಪ್ರತ್ಯಾಗಮ map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ಧ್ರುವ hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned ಪೂರ್ಣಾಙ್ಕ table_index, unsigned ಪೂರ್ಣಾಙ್ಕ stage) ಧ್ರುವ
  {
    ಯದಿ (unlikely (stage > stages[table_index].length))
      ಪ್ರತ್ಯಾಗಮ hb_array<ಧ್ರುವ hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned ಪೂರ್ಣಾಙ್ಕ start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    ಪ್ರತ್ಯಾಗಮ lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL ಶೂನ್ಯ collect_lookups (unsigned ಪೂರ್ಣಾಙ್ಕ table_index, hb_set_t *lookups) ಧ್ರುವ;
  template <typename Proxy>
  HB_INTERNAL ಶೂನ್ಯ apply (ಧ್ರುವ Proxy &proxy,
			  ಧ್ರುವ ಸಂರಚನಾ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ substitute (ಧ್ರುವ ಸಂರಚನಾ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ position (ಧ್ರುವ ಸಂರಚನಾ hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ಧ್ರುವ;
  HB_INTERNAL unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_tags (unsigned ಪೂರ್ಣಾಙ್ಕ  start_offset,
					     unsigned ಪೂರ್ಣಾಙ್ಕ *tag_ಎಣಿಕೆ, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ಧ್ರುವ;

  ಸಾರ್ವಜನಿಕ:
  hb_tag_t chosen_script[2];
  ತಾರ್ಕಿಕ found_script[2];

  ಗುಪ್ತ:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

ಗಣನಾ hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_ಹುಡುಕು	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ಸಂರಚನಾ hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

ಸಂರಚನಾ hb_ot_shape_plan_ಕೀಲಿ_t;

ಸಂರಚನಾ hb_ot_map_builder_t
{
  ಸಾರ್ವಜನಿಕ:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ಧ್ರುವ hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL ಶೂನ್ಯ add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned ಪೂರ್ಣಾಙ್ಕ value=1);

  HB_INTERNAL ತಾರ್ಕಿಕ has_feature (hb_tag_t tag);

  ಶೂನ್ಯ add_feature (ಧ್ರುವ hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  ಶೂನ್ಯ enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned ಪೂರ್ಣಾಙ್ಕ value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  ಶೂನ್ಯ disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  ಶೂನ್ಯ add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  ಶೂನ್ಯ add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL ಶೂನ್ಯ compile (hb_ot_map_t                  &m,
			    ಧ್ರುವ hb_ot_shape_plan_ಕೀಲಿ_t &key);

  ಗುಪ್ತ:

  HB_INTERNAL ಶೂನ್ಯ add_lookups (hb_ot_map_t  &m,
				unsigned ಪೂರ್ಣಾಙ್ಕ  table_index,
				unsigned ಪೂರ್ಣಾಙ್ಕ  feature_index,
				ಧ್ರುವ hb_ot_shape_plan_ಕೀಲಿ_t &key,
				hb_mask_t     mask,
				ತಾರ್ಕಿಕ          auto_zwnj = ಸತ್ಯ,
				ತಾರ್ಕಿಕ          auto_zwj = ಸತ್ಯ,
				ತಾರ್ಕಿಕ          random = ಅಸತ್ಯ,
				ತಾರ್ಕಿಕ          per_syllable = ಅಸತ್ಯ,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  ಸಂರಚನಾ feature_info_t {
    hb_tag_t tag;
    unsigned ಪೂರ್ಣಾಙ್ಕ seq; /* sequence#, used for stable sorting only */
    unsigned ಪೂರ್ಣಾಙ್ಕ max_ಮೌಲ್ಯ;
    hb_ot_map_feature_flags_t flags;
    unsigned ಪೂರ್ಣಾಙ್ಕ default_ಮೌಲ್ಯ; /* for non-global features, what should the unset glyphs take */
    unsigned ಪೂರ್ಣಾಙ್ಕ stage[2]; /* GSUB/GPOS */

    HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
    {
      ಧ್ರುವ feature_info_t *a = (ಧ್ರುವ feature_info_t *) pa;
      ಧ್ರುವ feature_info_t *b = (ಧ್ರುವ feature_info_t *) pb;
      ಪ್ರತ್ಯಾಗಮ (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ಸಂರಚನಾ stage_info_t {
    unsigned ಪೂರ್ಣಾಙ್ಕ index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL ಶೂನ್ಯ add_pause (unsigned ಪೂರ್ಣಾಙ್ಕ table_index, hb_ot_map_t::pause_func_t pause_func);

  ಸಾರ್ವಜನಿಕ:

  hb_face_t *face;
  hb_segment_properties_t props;
  ತಾರ್ಕಿಕ is_simple;

  hb_tag_t chosen_script[2];
  ತಾರ್ಕಿಕ found_script[2];
  unsigned ಪೂರ್ಣಾಙ್ಕ script_index[2], language_index[2];

  ಗುಪ್ತ:

  unsigned ಪೂರ್ಣಾಙ್ಕ current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
