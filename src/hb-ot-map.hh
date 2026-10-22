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

#അന്തര്ഭാവ "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_മൂല്യം ((1u << HB_OT_MAP_MAX_BITS) - 1u)

ഘടന hb_ot_shape_plan_t;

സ്ഥിതം സ്ഥിരം hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

ഘടന hb_ot_map_t
{
  friend ഘടന hb_ot_map_builder_t;

  പൊതു:

  ഘടന feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned പൂർണ്ണസംഖ്യ index[2]; /* GSUB/GPOS */
    unsigned പൂർണ്ണസംഖ്യ stage[2]; /* GSUB/GPOS */
    unsigned പൂർണ്ണസംഖ്യ shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned പൂർണ്ണസംഖ്യ needs_fallback : 1;
    unsigned പൂർണ്ണസംഖ്യ auto_zwnj : 1;
    unsigned പൂർണ്ണസംഖ്യ auto_zwj : 1;
    unsigned പൂർണ്ണസംഖ്യ random : 1;
    unsigned പൂർണ്ണസംഖ്യ per_syllable : 1;

    പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_tag_t tag_) സ്ഥിരം
    { തിരികെ tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
    {
      സ്ഥിരം feature_map_t *a = (സ്ഥിരം feature_map_t *) pa;
      സ്ഥിരം feature_map_t *b = (സ്ഥിരം feature_map_t *) pb;
      തിരികെ a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  ഘടന lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
    {
      സ്ഥിരം lookup_map_t *a = (സ്ഥിരം lookup_map_t *) pa;
      സ്ഥിരം lookup_map_t *b = (സ്ഥിരം lookup_map_t *) pb;
      തിരികെ a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  തരനിർവചനം തർക്കം (*pause_func_t) (സ്ഥിരം ഘടന hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  ഘടന stage_map_t {
    unsigned പൂർണ്ണസംഖ്യ last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  ശൂന്യം init ()
  {
    hb_memset (ഇത്, 0, വലുപ്പം (*ഇത്));

    features.init0 ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  ശൂന്യം fini ()
  {
    features.fini ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () സ്ഥിരം { തിരികെ global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned പൂർണ്ണസംഖ്യ *shift = nullptr) സ്ഥിരം
  {
    സ്ഥിരം feature_map_t *map = features.bsearch (feature_tag);
    എങ്കിൽ (shift) *shift = map ? map->shift : 0;
    തിരികെ map ? map->mask : 0;
  }

  തർക്കം needs_fallback (hb_tag_t feature_tag) സ്ഥിരം
  {
    സ്ഥിരം feature_map_t *map = features.bsearch (feature_tag);
    തിരികെ map ? map->needs_fallback : അസത്യം;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) സ്ഥിരം
  {
    സ്ഥിരം feature_map_t *map = features.bsearch (feature_tag);
    തിരികെ map ? map->_1_mask : 0;
  }

  unsigned പൂർണ്ണസംഖ്യ get_feature_index (unsigned പൂർണ്ണസംഖ്യ table_index, hb_tag_t feature_tag) സ്ഥിരം
  {
    സ്ഥിരം feature_map_t *map = features.bsearch (feature_tag);
    തിരികെ map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned പൂർണ്ണസംഖ്യ get_feature_stage (unsigned പൂർണ്ണസംഖ്യ table_index, hb_tag_t feature_tag) സ്ഥിരം
  {
    സ്ഥിരം feature_map_t *map = features.bsearch (feature_tag);
    തിരികെ map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<സ്ഥിരം hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned പൂർണ്ണസംഖ്യ table_index, unsigned പൂർണ്ണസംഖ്യ stage) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (stage > stages[table_index].length))
      തിരികെ hb_array<സ്ഥിരം hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned പൂർണ്ണസംഖ്യ start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned പൂർണ്ണസംഖ്യ end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    തിരികെ lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL ശൂന്യം collect_lookups (unsigned പൂർണ്ണസംഖ്യ table_index, hb_set_t *lookups) സ്ഥിരം;
  template <typename Proxy>
  HB_INTERNAL ശൂന്യം apply (സ്ഥിരം Proxy &proxy,
			  സ്ഥിരം ഘടന hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം;
  HB_INTERNAL ശൂന്യം substitute (സ്ഥിരം ഘടന hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം;
  HB_INTERNAL ശൂന്യം position (സ്ഥിരം ഘടന hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം;
  HB_INTERNAL unsigned പൂർണ്ണസംഖ്യ get_feature_tags (unsigned പൂർണ്ണസംഖ്യ  start_offset,
					     unsigned പൂർണ്ണസംഖ്യ *tag_എണ്ണം, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) സ്ഥിരം;

  പൊതു:
  hb_tag_t chosen_script[2];
  തർക്കം found_script[2];

  സ്വകാര്യ:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

എണ്ണൽ hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_തിരയൽ	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ഘടന hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

ഘടന hb_ot_shape_plan_താക്കോൽ_t;

ഘടന hb_ot_map_builder_t
{
  പൊതു:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   സ്ഥിരം hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL ശൂന്യം add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned പൂർണ്ണസംഖ്യ value=1);

  HB_INTERNAL തർക്കം has_feature (hb_tag_t tag);

  ശൂന്യം add_feature (സ്ഥിരം hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  ശൂന്യം enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned പൂർണ്ണസംഖ്യ value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  ശൂന്യം disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  ശൂന്യം add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  ശൂന്യം add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL ശൂന്യം compile (hb_ot_map_t                  &m,
			    സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key);

  സ്വകാര്യ:

  HB_INTERNAL ശൂന്യം add_lookups (hb_ot_map_t  &m,
				unsigned പൂർണ്ണസംഖ്യ  table_index,
				unsigned പൂർണ്ണസംഖ്യ  feature_index,
				സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key,
				hb_mask_t     mask,
				തർക്കം          auto_zwnj = സത്യം,
				തർക്കം          auto_zwj = സത്യം,
				തർക്കം          random = അസത്യം,
				തർക്കം          per_syllable = അസത്യം,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  ഘടന feature_info_t {
    hb_tag_t tag;
    unsigned പൂർണ്ണസംഖ്യ seq; /* sequence#, used for stable sorting only */
    unsigned പൂർണ്ണസംഖ്യ max_മൂല്യം;
    hb_ot_map_feature_flags_t flags;
    unsigned പൂർണ്ണസംഖ്യ default_മൂല്യം; /* for non-global features, what should the unset glyphs take */
    unsigned പൂർണ്ണസംഖ്യ stage[2]; /* GSUB/GPOS */

    HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
    {
      സ്ഥിരം feature_info_t *a = (സ്ഥിരം feature_info_t *) pa;
      സ്ഥിരം feature_info_t *b = (സ്ഥിരം feature_info_t *) pb;
      തിരികെ (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ഘടന stage_info_t {
    unsigned പൂർണ്ണസംഖ്യ index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL ശൂന്യം add_pause (unsigned പൂർണ്ണസംഖ്യ table_index, hb_ot_map_t::pause_func_t pause_func);

  പൊതു:

  hb_face_t *face;
  hb_segment_properties_t props;
  തർക്കം is_simple;

  hb_tag_t chosen_script[2];
  തർക്കം found_script[2];
  unsigned പൂർണ്ണസംഖ്യ script_index[2], language_index[2];

  സ്വകാര്യ:

  unsigned പൂർണ്ണസംഖ്യ current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
