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

#اگر_تعریف_نشده HB_OT_MAP_HH
#تعریف_کن HB_OT_MAP_HH

#ضمن "hb-buffer.hh"


#تعریف_کن HB_OT_MAP_MAX_بیتها 8u
#تعریف_کن HB_OT_MAP_MAX_مقدار ((1u << HB_OT_MAP_MAX_بیتها) - 1u)

بنية hb_ot_shape_plan_t;

ساكن ثابت hb_برچسب_t جدول_برچسبها[2] = {HB_OT_برچسب_GSUB, HB_OT_برچسب_GPOS};

بنية hb_ot_map_t
{
  دوست بنية hb_ot_map_builder_t;

  عام:

  بنية feature_map_t {
    hb_برچسب_t برچسب_2; /* should be first for our bsearch to work */
    بیعلامت صحيح شاخص_2[2]; /* GSUB/GPOS */
    بیعلامت صحيح stage[2]; /* GSUB/GPOS */
    بیعلامت صحيح لغزش;
    hb_ماسک_t ماسک;
    hb_ماسک_t _1_mask; /* mask for value=1, for quick access */
    بیعلامت صحيح needs_fallback : 1;
    بیعلامت صحيح auto_zwnj : 1;
    بیعلامت صحيح auto_zwj : 1;
    بیعلامت صحيح random : 1;
    بیعلامت صحيح در_هر_syllable : 1;

    صحيح cmp (ثابت hb_برچسب_t برچسب_4) ثابت
    { أرجع برچسب_4 < برچسب_2 ? -1 : برچسب_4 > برچسب_2 ? 1 : 0; }

    HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      أرجع a->برچسب_2 < b->برچسب_2 ? -1 : a->برچسب_2 > b->برچسب_2 ? 1 : 0;
    }
  };

  بنية lookup_map_t {
    بیعلامت کوتاه شاخص_2;
    بیعلامت کوتاه auto_zwnj : 1;
    بیعلامت کوتاه auto_zwj : 1;
    بیعلامت کوتاه random : 1;
    بیعلامت کوتاه در_هر_syllable : 1;
    hb_ماسک_t ماسک;
    hb_برچسب_t feature_برچسب;

    HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعريفنوع منطقي (*pause_func_t) (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_میانگیر_t *میانگیر);

  بنية stage_map_t {
    بیعلامت صحيح آخرین_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  فراغ init ()
  {
    hb_memset (هذا, 0, حجم (*هذا));

    features.init0 ();
    لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      lookups[جدول_شاخص].init0 ();
      stages[جدول_شاخص].init0 ();
    }
  }
  فراغ fini ()
  {
    features.fini ();
    لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      lookups[جدول_شاخص].fini ();
      stages[جدول_شاخص].fini ();
    }
  }

  hb_ماسک_t get_global_ماسک () ثابت { أرجع global_ماسک; }

  hb_ماسک_t get_ماسک (hb_برچسب_t feature_برچسب, بیعلامت صحيح *لغزش = اشاره_تهی) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_برچسب);
    إذا (لغزش) *لغزش = map ? map->لغزش : 0;
    أرجع map ? map->ماسک : 0;
  }

  منطقي needs_fallback (hb_برچسب_t feature_برچسب) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_برچسب);
    أرجع map ? map->needs_fallback : خطأ;
  }

  hb_ماسک_t get_1_ماسک (hb_برچسب_t feature_برچسب) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_برچسب);
    أرجع map ? map->_1_mask : 0;
  }

  بیعلامت صحيح get_feature_شاخص (بیعلامت صحيح جدول_شاخص, hb_برچسب_t feature_برچسب) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_برچسب);
    أرجع map ? map->شاخص_2[جدول_شاخص] : HB_OT_LAYOUT_NO_FEATURE_شاخص;
  }

  بیعلامت صحيح get_feature_stage (بیعلامت صحيح جدول_شاخص, hb_برچسب_t feature_برچسب) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_برچسب);
    أرجع map ? map->stage[جدول_شاخص] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (بیعلامت صحيح جدول_شاخص, بیعلامت صحيح stage) ثابت
  {
    إذا (unlikely (stage > stages[جدول_شاخص].درازی))
      أرجع hb_array<ثابت hb_ot_map_t::lookup_map_t> (اشاره_تهی, 0);

    بیعلامت صحيح شروع = stage ? stages[جدول_شاخص][stage - 1].آخرین_lookup : 0;
    بیعلامت صحيح انتها   = stage < stages[جدول_شاخص].درازی ? stages[جدول_شاخص][stage].آخرین_lookup : lookups[جدول_شاخص].درازی;
    أرجع lookups[جدول_شاخص].as_array ().sub_array (شروع, انتها - شروع);
  }

  HB_داخلی فراغ collect_lookups (بیعلامت صحيح جدول_شاخص, hb_مجموعه_t *lookups) ثابت;
  قالب <نام_نوع Proxy>
  HB_داخلی فراغ apply (ثابت Proxy &proxy,
			  ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_میانگیر_t *میانگیر) ثابت;
  HB_داخلی فراغ substitute (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_میانگیر_t *میانگیر) ثابت;
  HB_داخلی فراغ position (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_میانگیر_t *میانگیر) ثابت;
  HB_داخلی بیعلامت صحيح get_feature_برچسبها (بیعلامت صحيح  شروع_جابه_جایی,
					     بیعلامت صحيح *برچسب_شمار, /* IN/OUT */
					     hb_برچسب_t     *برچسبها /*بیرون*/) ثابت;

  عام:
  hb_برچسب_t chosen_script[2];
  منطقي یافت_script[2];

  خاص:

  hb_ماسک_t global_ماسک = 0;

  hb_sorted_وکتور_t<feature_map_t> features;
  hb_وکتور_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_وکتور_t<stage_map_t> stages[2]; /* GSUB/GPOS */
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
  F_GLOBAL_جستجو	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_در_هر_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


بنية hb_ot_map_feature_t
{
  hb_برچسب_t برچسب_2;
  hb_ot_map_feature_flags_t flags;
};

بنية hb_ot_shape_plan_کلید_t;

بنية hb_ot_map_builder_t
{
  عام:

  HB_داخلی hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_داخلی ~hb_ot_map_builder_t ();

  HB_داخلی فراغ جمع_کن_feature (hb_برچسب_t برچسب_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				بیعلامت صحيح value=1);

  HB_داخلی منطقي has_feature (hb_برچسب_t برچسب_2);

  فراغ جمع_کن_feature (ثابت hb_ot_map_feature_t &feat)
  { جمع_کن_feature (feat.برچسب_2, feat.flags); }

  فراغ enable_feature (hb_برچسب_t برچسب_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       بیعلامت صحيح value=1)
  { جمع_کن_feature (برچسب_2, F_GLOBAL | flags, value); }

  فراغ disable_feature (hb_برچسب_t برچسب_2)
  { جمع_کن_feature (برچسب_2, F_GLOBAL, 0); }

  فراغ جمع_کن_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { جمع_کن_pause (0, pause_func); }
  فراغ جمع_کن_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { جمع_کن_pause (1, pause_func); }

  HB_داخلی فراغ compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_کلید_t &key);

  خاص:

  HB_داخلی فراغ جمع_کن_lookups (hb_ot_map_t  &m,
				بیعلامت صحيح  جدول_شاخص,
				بیعلامت صحيح  feature_شاخص,
				ثابت hb_ot_shape_plan_کلید_t &key,
				hb_ماسک_t     ماسک,
				منطقي          auto_zwnj = درست,
				منطقي          auto_zwj = درست,
				منطقي          random = خطأ,
				منطقي          در_هر_syllable = خطأ,
				hb_برچسب_t      feature_برچسب = HB_برچسب(' ',' ',' ',' '));

  بنية feature_info_t {
    hb_برچسب_t برچسب_2;
    بیعلامت صحيح seq; /* sequence#, used for stable sorting only */
    بیعلامت صحيح max_مقدار;
    hb_ot_map_feature_flags_t flags;
    بیعلامت صحيح default_مقدار; /* for non-global features, what should the unset glyphs take */
    بیعلامت صحيح stage[2]; /* GSUB/GPOS */

    HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      أرجع (a->برچسب_2 != b->برچسب_2) ?  (a->برچسب_2 < b->برچسب_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  بنية stage_info_t {
    بیعلامت صحيح شاخص_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_داخلی فراغ جمع_کن_pause (بیعلامت صحيح جدول_شاخص, hb_ot_map_t::pause_func_t pause_func);

  عام:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقي is_simple;

  hb_برچسب_t chosen_script[2];
  منطقي یافت_script[2];
  بیعلامت صحيح script_شاخص[2], language_شاخص[2];

  خاص:

  بیعلامت صحيح current_stage[2]; /* GSUB/GPOS */
  hb_وکتور_t<feature_info_t> feature_infos;
  hb_وکتور_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#پایان_اگر /* HB_OT_MAP_HH */
