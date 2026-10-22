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

#अगर_अघोषित HB_OT_MAP_HH
#घोषणा HB_OT_MAP_HH

#समावेश "hb-buffer.hh"


#घोषणा HB_OT_MAP_MAX_BITS 8u
#घोषणा HB_OT_MAP_MAX_मूल्य ((1u << HB_OT_MAP_MAX_BITS) - 1u)

काष्ठा hb_ot_shape_plan_t;

अटल स्थिर hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

काष्ठा hb_ot_map_t
{
  मित्र काष्ठा hb_ot_map_builder_t;

  खुला:

  काष्ठा feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    अचिन्हित पूर्णांक सूचकांक_2[2]; /* GSUB/GPOS */
    अचिन्हित पूर्णांक stage[2]; /* GSUB/GPOS */
    अचिन्हित पूर्णांक shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    अचिन्हित पूर्णांक needs_fallback : 1;
    अचिन्हित पूर्णांक auto_zwnj : 1;
    अचिन्हित पूर्णांक auto_zwj : 1;
    अचिन्हित पूर्णांक random : 1;
    अचिन्हित पूर्णांक per_syllable : 1;

    पूर्णांक cmp (स्थिर hb_tag_t tag_) स्थिर
    { वापस tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
    {
      स्थिर feature_map_t *a = (स्थिर feature_map_t *) pa;
      स्थिर feature_map_t *b = (स्थिर feature_map_t *) pb;
      वापस a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  काष्ठा lookup_map_t {
    अचिन्हित लघु सूचकांक_2;
    अचिन्हित लघु auto_zwnj : 1;
    अचिन्हित लघु auto_zwj : 1;
    अचिन्हित लघु random : 1;
    अचिन्हित लघु per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
    {
      स्थिर lookup_map_t *a = (स्थिर lookup_map_t *) pa;
      स्थिर lookup_map_t *b = (स्थिर lookup_map_t *) pb;
      वापस a->सूचकांक_2 < b->सूचकांक_2 ? -1 : a->सूचकांक_2 > b->सूचकांक_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  प्रकार bool (*pause_func_t) (स्थिर काष्ठा hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर);

  काष्ठा stage_map_t {
    अचिन्हित पूर्णांक last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  व्योम init ()
  {
    hb_memset (यह, 0, माप (*यह));

    features.init0 ();
    क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    {
      lookups[table_सूचकांक].init0 ();
      stages[table_सूचकांक].init0 ();
    }
  }
  व्योम fini ()
  {
    features.fini ();
    क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    {
      lookups[table_सूचकांक].fini ();
      stages[table_सूचकांक].fini ();
    }
  }

  hb_mask_t get_global_mask () स्थिर { वापस global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, अचिन्हित पूर्णांक *shift = nullptr) स्थिर
  {
    स्थिर feature_map_t *map = features.द्वा_खोज (feature_tag);
    अगर (shift) *shift = map ? map->shift : 0;
    वापस map ? map->mask : 0;
  }

  bool needs_fallback (hb_tag_t feature_tag) स्थिर
  {
    स्थिर feature_map_t *map = features.द्वा_खोज (feature_tag);
    वापस map ? map->needs_fallback : false;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) स्थिर
  {
    स्थिर feature_map_t *map = features.द्वा_खोज (feature_tag);
    वापस map ? map->_1_mask : 0;
  }

  अचिन्हित पूर्णांक get_feature_सूचकांक (अचिन्हित पूर्णांक table_सूचकांक, hb_tag_t feature_tag) स्थिर
  {
    स्थिर feature_map_t *map = features.द्वा_खोज (feature_tag);
    वापस map ? map->सूचकांक_2[table_सूचकांक] : HB_OT_LAYOUT_NO_FEATURE_सूचकांक;
  }

  अचिन्हित पूर्णांक get_feature_stage (अचिन्हित पूर्णांक table_सूचकांक, hb_tag_t feature_tag) स्थिर
  {
    स्थिर feature_map_t *map = features.द्वा_खोज (feature_tag);
    वापस map ? map->stage[table_सूचकांक] : अच_पूर्णांक_उच्च;
  }

  hb_array_t<स्थिर hb_ot_map_t::lookup_map_t>
  get_stage_lookups (अचिन्हित पूर्णांक table_सूचकांक, अचिन्हित पूर्णांक stage) स्थिर
  {
    अगर (unlikely (stage > stages[table_सूचकांक].लंबाई))
      वापस hb_array<स्थिर hb_ot_map_t::lookup_map_t> (nullptr, 0);

    अचिन्हित पूर्णांक start = stage ? stages[table_सूचकांक][stage - 1].last_lookup : 0;
    अचिन्हित पूर्णांक end   = stage < stages[table_सूचकांक].लंबाई ? stages[table_सूचकांक][stage].last_lookup : lookups[table_सूचकांक].लंबाई;
    वापस lookups[table_सूचकांक].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL व्योम collect_lookups (अचिन्हित पूर्णांक table_सूचकांक, hb_set_t *lookups) स्थिर;
  ढाँचा <typename Proxy>
  HB_INTERNAL व्योम apply (स्थिर Proxy &proxy,
			  स्थिर काष्ठा hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर) स्थिर;
  HB_INTERNAL व्योम substitute (स्थिर काष्ठा hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर) स्थिर;
  HB_INTERNAL व्योम position (स्थिर काष्ठा hb_ot_shape_plan_t *plan, hb_font_t *font, hb_बफर_t *बफर) स्थिर;
  HB_INTERNAL अचिन्हित पूर्णांक get_feature_tags (अचिन्हित पूर्णांक  start_offset,
					     अचिन्हित पूर्णांक *tag_गिनती, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) स्थिर;

  खुला:
  hb_tag_t chosen_script[2];
  bool found_script[2];

  निजी:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

क्रमागत hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_खोज	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


काष्ठा hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

काष्ठा hb_ot_shape_plan_कुंजी_t;

काष्ठा hb_ot_map_builder_t
{
  खुला:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   स्थिर hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL व्योम add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				अचिन्हित पूर्णांक value=1);

  HB_INTERNAL bool has_feature (hb_tag_t tag);

  व्योम add_feature (स्थिर hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  व्योम enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       अचिन्हित पूर्णांक value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  व्योम disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  व्योम add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  व्योम add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL व्योम compile (hb_ot_map_t                  &m,
			    स्थिर hb_ot_shape_plan_कुंजी_t &key);

  निजी:

  HB_INTERNAL व्योम add_lookups (hb_ot_map_t  &m,
				अचिन्हित पूर्णांक  table_सूचकांक,
				अचिन्हित पूर्णांक  feature_सूचकांक,
				स्थिर hb_ot_shape_plan_कुंजी_t &key,
				hb_mask_t     mask,
				bool          auto_zwnj = true,
				bool          auto_zwj = true,
				bool          random = false,
				bool          per_syllable = false,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  काष्ठा feature_info_t {
    hb_tag_t tag;
    अचिन्हित पूर्णांक seq; /* sequence#, used for stable sorting only */
    अचिन्हित पूर्णांक max_मूल्य;
    hb_ot_map_feature_flags_t flags;
    अचिन्हित पूर्णांक default_मूल्य; /* for non-global features, what should the unset glyphs take */
    अचिन्हित पूर्णांक stage[2]; /* GSUB/GPOS */

    HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
    {
      स्थिर feature_info_t *a = (स्थिर feature_info_t *) pa;
      स्थिर feature_info_t *b = (स्थिर feature_info_t *) pb;
      वापस (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  काष्ठा stage_info_t {
    अचिन्हित पूर्णांक सूचकांक_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL व्योम add_pause (अचिन्हित पूर्णांक table_सूचकांक, hb_ot_map_t::pause_func_t pause_func);

  खुला:

  hb_face_t *face;
  hb_segment_properties_t props;
  bool is_simple;

  hb_tag_t chosen_script[2];
  bool found_script[2];
  अचिन्हित पूर्णांक script_सूचकांक[2], language_सूचकांक[2];

  निजी:

  अचिन्हित पूर्णांक current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#पूर्ण_अगर /* HB_OT_MAP_HH */
