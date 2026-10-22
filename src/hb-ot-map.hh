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

#અન્તર્ભાવ "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_કિંમત ((1u << HB_OT_MAP_MAX_BITS) - 1u)

સંરચના hb_ot_shape_plan_t;

સ્થિર અચલ hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

સંરચના hb_ot_map_t
{
  friend સંરચના hb_ot_map_builder_t;

  જાહેર:

  સંરચના feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned પૂર્ણાંક index[2]; /* GSUB/GPOS */
    unsigned પૂર્ણાંક stage[2]; /* GSUB/GPOS */
    unsigned પૂર્ણાંક shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned પૂર્ણાંક needs_fallback : 1;
    unsigned પૂર્ણાંક auto_zwnj : 1;
    unsigned પૂર્ણાંક auto_zwj : 1;
    unsigned પૂર્ણાંક random : 1;
    unsigned પૂર્ણાંક per_syllable : 1;

    પૂર્ણાંક cmp (અચલ hb_tag_t tag_) અચલ
    { પાછા tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
    {
      અચલ feature_map_t *a = (અચલ feature_map_t *) pa;
      અચલ feature_map_t *b = (અચલ feature_map_t *) pb;
      પાછા a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  સંરચના lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
    {
      અચલ lookup_map_t *a = (અચલ lookup_map_t *) pa;
      અચલ lookup_map_t *b = (અચલ lookup_map_t *) pb;
      પાછા a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  પ્રકારવ્યાખ્યા તાર્કિક (*pause_func_t) (અચલ સંરચના hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  સંરચના stage_map_t {
    unsigned પૂર્ણાંક last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  શૂન્ય init ()
  {
    hb_memset (આ, 0, કદ (*આ));

    features.init0 ();
    માટે (unsigned પૂર્ણાંક table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  શૂન્ય fini ()
  {
    features.fini ();
    માટે (unsigned પૂર્ણાંક table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () અચલ { પાછા global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned પૂર્ણાંક *shift = nullptr) અચલ
  {
    અચલ feature_map_t *map = features.bsearch (feature_tag);
    જો (shift) *shift = map ? map->shift : 0;
    પાછા map ? map->mask : 0;
  }

  તાર્કિક needs_fallback (hb_tag_t feature_tag) અચલ
  {
    અચલ feature_map_t *map = features.bsearch (feature_tag);
    પાછા map ? map->needs_fallback : ખોટું;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) અચલ
  {
    અચલ feature_map_t *map = features.bsearch (feature_tag);
    પાછા map ? map->_1_mask : 0;
  }

  unsigned પૂર્ણાંક get_feature_index (unsigned પૂર્ણાંક table_index, hb_tag_t feature_tag) અચલ
  {
    અચલ feature_map_t *map = features.bsearch (feature_tag);
    પાછા map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned પૂર્ણાંક get_feature_stage (unsigned પૂર્ણાંક table_index, hb_tag_t feature_tag) અચલ
  {
    અચલ feature_map_t *map = features.bsearch (feature_tag);
    પાછા map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<અચલ hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned પૂર્ણાંક table_index, unsigned પૂર્ણાંક stage) અચલ
  {
    જો (unlikely (stage > stages[table_index].length))
      પાછા hb_array<અચલ hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned પૂર્ણાંક start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned પૂર્ણાંક end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    પાછા lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL શૂન્ય collect_lookups (unsigned પૂર્ણાંક table_index, hb_set_t *lookups) અચલ;
  template <typename Proxy>
  HB_INTERNAL શૂન્ય apply (અચલ Proxy &proxy,
			  અચલ સંરચના hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) અચલ;
  HB_INTERNAL શૂન્ય substitute (અચલ સંરચના hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) અચલ;
  HB_INTERNAL શૂન્ય position (અચલ સંરચના hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) અચલ;
  HB_INTERNAL unsigned પૂર્ણાંક get_feature_tags (unsigned પૂર્ણાંક  start_offset,
					     unsigned પૂર્ણાંક *tag_ગણતરી, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) અચલ;

  જાહેર:
  hb_tag_t chosen_script[2];
  તાર્કિક found_script[2];

  ખાનગી:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

ગણના hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_શોધ	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


સંરચના hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

સંરચના hb_ot_shape_plan_ચાવી_t;

સંરચના hb_ot_map_builder_t
{
  જાહેર:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   અચલ hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL શૂન્ય add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned પૂર્ણાંક value=1);

  HB_INTERNAL તાર્કિક has_feature (hb_tag_t tag);

  શૂન્ય add_feature (અચલ hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  શૂન્ય enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned પૂર્ણાંક value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  શૂન્ય disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  શૂન્ય add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  શૂન્ય add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL શૂન્ય compile (hb_ot_map_t                  &m,
			    અચલ hb_ot_shape_plan_ચાવી_t &key);

  ખાનગી:

  HB_INTERNAL શૂન્ય add_lookups (hb_ot_map_t  &m,
				unsigned પૂર્ણાંક  table_index,
				unsigned પૂર્ણાંક  feature_index,
				અચલ hb_ot_shape_plan_ચાવી_t &key,
				hb_mask_t     mask,
				તાર્કિક          auto_zwnj = સાચું,
				તાર્કિક          auto_zwj = સાચું,
				તાર્કિક          random = ખોટું,
				તાર્કિક          per_syllable = ખોટું,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  સંરચના feature_info_t {
    hb_tag_t tag;
    unsigned પૂર્ણાંક seq; /* sequence#, used for stable sorting only */
    unsigned પૂર્ણાંક max_કિંમત;
    hb_ot_map_feature_flags_t flags;
    unsigned પૂર્ણાંક default_કિંમત; /* for non-global features, what should the unset glyphs take */
    unsigned પૂર્ણાંક stage[2]; /* GSUB/GPOS */

    HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
    {
      અચલ feature_info_t *a = (અચલ feature_info_t *) pa;
      અચલ feature_info_t *b = (અચલ feature_info_t *) pb;
      પાછા (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  સંરચના stage_info_t {
    unsigned પૂર્ણાંક index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL શૂન્ય add_pause (unsigned પૂર્ણાંક table_index, hb_ot_map_t::pause_func_t pause_func);

  જાહેર:

  hb_face_t *face;
  hb_segment_properties_t props;
  તાર્કિક is_simple;

  hb_tag_t chosen_script[2];
  તાર્કિક found_script[2];
  unsigned પૂર્ણાંક script_index[2], language_index[2];

  ખાનગી:

  unsigned પૂર્ણાંક current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
