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

#ضمن "hb-buffer.hh"


#define HB_OT_MAP_MAX_BITS 8u
#define HB_OT_MAP_MAX_قيمة ((1u << HB_OT_MAP_MAX_BITS) - 1u)

بنية hb_ot_shape_plan_t;

ساكن ثابت hb_tag_t table_tags[2] = {HB_OT_TAG_GSUB, HB_OT_TAG_GPOS};

بنية hb_ot_map_t
{
  friend بنية hb_ot_map_builder_t;

  عام:

  بنية feature_map_t {
    hb_tag_t tag; /* should be first for our bsearch to work */
    unsigned صحيح index[2]; /* GSUB/GPOS */
    unsigned صحيح stage[2]; /* GSUB/GPOS */
    unsigned صحيح shift;
    hb_mask_t mask;
    hb_mask_t _1_mask; /* mask for value=1, for quick access */
    unsigned صحيح needs_fallback : 1;
    unsigned صحيح auto_zwnj : 1;
    unsigned صحيح auto_zwj : 1;
    unsigned صحيح random : 1;
    unsigned صحيح per_syllable : 1;

    صحيح cmp (ثابت hb_tag_t tag_) ثابت
    { إرجاع tag_ < tag ? -1 : tag_ > tag ? 1 : 0; }

    HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      إرجاع a->tag < b->tag ? -1 : a->tag > b->tag ? 1 : 0;
    }
  };

  بنية lookup_map_t {
    unsigned short index;
    unsigned short auto_zwnj : 1;
    unsigned short auto_zwj : 1;
    unsigned short random : 1;
    unsigned short per_syllable : 1;
    hb_mask_t mask;
    hb_tag_t feature_tag;

    HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      إرجاع a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعريفنوع منطقي (*pause_func_t) (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer);

  بنية stage_map_t {
    unsigned صحيح last_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  فراغ init ()
  {
    hb_memset (هذا, 0, حجم (*هذا));

    features.init0 ();
    لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].init0 ();
      stages[table_index].init0 ();
    }
  }
  فراغ fini ()
  {
    features.fini ();
    لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    {
      lookups[table_index].fini ();
      stages[table_index].fini ();
    }
  }

  hb_mask_t get_global_mask () ثابت { إرجاع global_mask; }

  hb_mask_t get_mask (hb_tag_t feature_tag, unsigned صحيح *shift = nullptr) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    إذا (shift) *shift = map ? map->shift : 0;
    إرجاع map ? map->mask : 0;
  }

  منطقي needs_fallback (hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    إرجاع map ? map->needs_fallback : خطأ;
  }

  hb_mask_t get_1_mask (hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    إرجاع map ? map->_1_mask : 0;
  }

  unsigned صحيح get_feature_index (unsigned صحيح table_index, hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    إرجاع map ? map->index[table_index] : HB_OT_LAYOUT_NO_FEATURE_INDEX;
  }

  unsigned صحيح get_feature_stage (unsigned صحيح table_index, hb_tag_t feature_tag) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_tag);
    إرجاع map ? map->stage[table_index] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (unsigned صحيح table_index, unsigned صحيح stage) ثابت
  {
    إذا (unlikely (stage > stages[table_index].length))
      إرجاع hb_array<ثابت hb_ot_map_t::lookup_map_t> (nullptr, 0);

    unsigned صحيح start = stage ? stages[table_index][stage - 1].last_lookup : 0;
    unsigned صحيح end   = stage < stages[table_index].length ? stages[table_index][stage].last_lookup : lookups[table_index].length;
    إرجاع lookups[table_index].as_array ().sub_array (start, end - start);
  }

  HB_INTERNAL فراغ collect_lookups (unsigned صحيح table_index, hb_set_t *lookups) ثابت;
  template <typename Proxy>
  HB_INTERNAL فراغ apply (ثابت Proxy &proxy,
			  ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL فراغ substitute (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL فراغ position (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL unsigned صحيح get_feature_tags (unsigned صحيح  start_offset,
					     unsigned صحيح *tag_تعداد, /* IN/OUT */
					     hb_tag_t     *tags /* OUT */) ثابت;

  عام:
  hb_tag_t chosen_script[2];
  منطقي found_script[2];

  خاص:

  hb_mask_t global_mask = 0;

  hb_sorted_vector_t<feature_map_t> features;
  hb_vector_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_vector_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

تعداد hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_بحث	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_PER_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


بنية hb_ot_map_feature_t
{
  hb_tag_t tag;
  hb_ot_map_feature_flags_t flags;
};

بنية hb_ot_shape_plan_مفتاح_t;

بنية hb_ot_map_builder_t
{
  عام:

  HB_INTERNAL hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_INTERNAL ~hb_ot_map_builder_t ();

  HB_INTERNAL فراغ add_feature (hb_tag_t tag,
				hb_ot_map_feature_flags_t flags=F_NONE,
				unsigned صحيح value=1);

  HB_INTERNAL منطقي has_feature (hb_tag_t tag);

  فراغ add_feature (ثابت hb_ot_map_feature_t &feat)
  { add_feature (feat.tag, feat.flags); }

  فراغ enable_feature (hb_tag_t tag,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       unsigned صحيح value=1)
  { add_feature (tag, F_GLOBAL | flags, value); }

  فراغ disable_feature (hb_tag_t tag)
  { add_feature (tag, F_GLOBAL, 0); }

  فراغ add_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (0, pause_func); }
  فراغ add_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { add_pause (1, pause_func); }

  HB_INTERNAL فراغ compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_مفتاح_t &key);

  خاص:

  HB_INTERNAL فراغ add_lookups (hb_ot_map_t  &m,
				unsigned صحيح  table_index,
				unsigned صحيح  feature_index,
				ثابت hb_ot_shape_plan_مفتاح_t &key,
				hb_mask_t     mask,
				منطقي          auto_zwnj = true,
				منطقي          auto_zwj = true,
				منطقي          random = خطأ,
				منطقي          per_syllable = خطأ,
				hb_tag_t      feature_tag = HB_TAG(' ',' ',' ',' '));

  بنية feature_info_t {
    hb_tag_t tag;
    unsigned صحيح seq; /* sequence#, used for stable sorting only */
    unsigned صحيح max_قيمة;
    hb_ot_map_feature_flags_t flags;
    unsigned صحيح default_قيمة; /* for non-global features, what should the unset glyphs take */
    unsigned صحيح stage[2]; /* GSUB/GPOS */

    HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      إرجاع (a->tag != b->tag) ?  (a->tag < b->tag ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  بنية stage_info_t {
    unsigned صحيح index;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_INTERNAL فراغ add_pause (unsigned صحيح table_index, hb_ot_map_t::pause_func_t pause_func);

  عام:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقي is_simple;

  hb_tag_t chosen_script[2];
  منطقي found_script[2];
  unsigned صحيح script_index[2], language_index[2];

  خاص:

  unsigned صحيح current_stage[2]; /* GSUB/GPOS */
  hb_vector_t<feature_info_t> feature_infos;
  hb_vector_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#endif /* HB_OT_MAP_HH */
