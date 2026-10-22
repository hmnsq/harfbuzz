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

#که_نه_تعریف_شوی HB_OT_MAP_HH
#تعریف_کړه HB_OT_MAP_HH

#ضمن "hb-buffer.hh"


#تعریف_کړه HB_OT_MAP_MAX_بټونه 8u
#تعریف_کړه HB_OT_MAP_MAX_ارزښت ((1u << HB_OT_MAP_MAX_بټونه) - 1u)

بنية hb_ot_shape_plan_t;

ساكن ثابت hb_نښان_t جدول_نښانونه[2] = {HB_OT_نښان_GSUB, HB_OT_نښان_GPOS};

بنية hb_ot_map_t
{
  ملګری بنية hb_ot_map_builder_t;

  عام:

  بنية feature_map_t {
    hb_نښان_t نښان_2; /* should be first for our bsearch to work */
    بینښه صحيح شاخص_2[2]; /* GSUB/GPOS */
    بینښه صحيح stage[2]; /* GSUB/GPOS */
    بینښه صحيح ښويدنه;
    hb_ماسک_t ماسک;
    hb_ماسک_t _1_mask; /* mask for value=1, for quick access */
    بینښه صحيح needs_fallback : 1;
    بینښه صحيح auto_zwnj : 1;
    بینښه صحيح auto_zwj : 1;
    بینښه صحيح random : 1;
    بینښه صحيح فی_syllable : 1;

    صحيح cmp (ثابت hb_نښان_t نښان_4) ثابت
    { أرجع نښان_4 < نښان_2 ? -1 : نښان_4 > نښان_2 ? 1 : 0; }

    HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      أرجع a->نښان_2 < b->نښان_2 ? -1 : a->نښان_2 > b->نښان_2 ? 1 : 0;
    }
  };

  بنية lookup_map_t {
    بینښه لنډ شاخص_2;
    بینښه لنډ auto_zwnj : 1;
    بینښه لنډ auto_zwj : 1;
    بینښه لنډ random : 1;
    بینښه لنډ فی_syllable : 1;
    hb_ماسک_t ماسک;
    hb_نښان_t feature_نښان;

    HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعريفنوع منطقي (*pause_func_t) (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر);

  بنية stage_map_t {
    بینښه صحيح وروستی_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  فراغ init ()
  {
    hb_memset (هذا, 0, حجم (*هذا));

    features.init0 ();
    لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      lookups[جدول_شاخص].init0 ();
      stages[جدول_شاخص].init0 ();
    }
  }
  فراغ fini ()
  {
    features.fini ();
    لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      lookups[جدول_شاخص].fini ();
      stages[جدول_شاخص].fini ();
    }
  }

  hb_ماسک_t get_global_ماسک () ثابت { أرجع global_ماسک; }

  hb_ماسک_t get_ماسک (hb_نښان_t feature_نښان, بینښه صحيح *ښويدنه = تش_اشاره) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_نښان);
    إذا (ښويدنه) *ښويدنه = map ? map->ښويدنه : 0;
    أرجع map ? map->ماسک : 0;
  }

  منطقي needs_fallback (hb_نښان_t feature_نښان) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_نښان);
    أرجع map ? map->needs_fallback : خطأ;
  }

  hb_ماسک_t get_1_ماسک (hb_نښان_t feature_نښان) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_نښان);
    أرجع map ? map->_1_mask : 0;
  }

  بینښه صحيح get_feature_شاخص (بینښه صحيح جدول_شاخص, hb_نښان_t feature_نښان) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_نښان);
    أرجع map ? map->شاخص_2[جدول_شاخص] : HB_OT_LAYOUT_NO_FEATURE_شاخص;
  }

  بینښه صحيح get_feature_stage (بینښه صحيح جدول_شاخص, hb_نښان_t feature_نښان) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_نښان);
    أرجع map ? map->stage[جدول_شاخص] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (بینښه صحيح جدول_شاخص, بینښه صحيح stage) ثابت
  {
    إذا (unlikely (stage > stages[جدول_شاخص].اوږدتیا))
      أرجع hb_array<ثابت hb_ot_map_t::lookup_map_t> (تش_اشاره, 0);

    بینښه صحيح پیل = stage ? stages[جدول_شاخص][stage - 1].وروستی_lookup : 0;
    بینښه صحيح پای_ټکی   = stage < stages[جدول_شاخص].اوږدتیا ? stages[جدول_شاخص][stage].وروستی_lookup : lookups[جدول_شاخص].اوږدتیا;
    أرجع lookups[جدول_شاخص].as_array ().sub_array (پیل, پای_ټکی - پیل);
  }

  HB_دننی فراغ collect_lookups (بینښه صحيح جدول_شاخص, hb_سېټ_t *lookups) ثابت;
  کينډۍ <ډول_نوم Proxy>
  HB_دننی فراغ apply (ثابت Proxy &proxy,
			  ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_دننی فراغ substitute (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_دننی فراغ position (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_دننی بینښه صحيح get_feature_نښانونه (بینښه صحيح  پیل_دننه_واټن,
					     بینښه صحيح *نښان_شمېر, /* IN/OUT */
					     hb_نښان_t     *نښانونه /*بهر*/) ثابت;

  عام:
  hb_نښان_t chosen_script[2];
  منطقي وموندل_script[2];

  خاص:

  hb_ماسک_t global_ماسک = 0;

  hb_sorted_ویکټور_t<feature_map_t> features;
  hb_ویکټور_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_ویکټور_t<stage_map_t> stages[2]; /* GSUB/GPOS */
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
  F_GLOBAL_پلټنه	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_فی_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


بنية hb_ot_map_feature_t
{
  hb_نښان_t نښان_2;
  hb_ot_map_feature_flags_t flags;
};

بنية hb_ot_shape_plan_کلی_t;

بنية hb_ot_map_builder_t
{
  عام:

  HB_دننی hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_دننی ~hb_ot_map_builder_t ();

  HB_دننی فراغ جمع_کړه_feature (hb_نښان_t نښان_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				بینښه صحيح value=1);

  HB_دننی منطقي has_feature (hb_نښان_t نښان_2);

  فراغ جمع_کړه_feature (ثابت hb_ot_map_feature_t &feat)
  { جمع_کړه_feature (feat.نښان_2, feat.flags); }

  فراغ enable_feature (hb_نښان_t نښان_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       بینښه صحيح value=1)
  { جمع_کړه_feature (نښان_2, F_GLOBAL | flags, value); }

  فراغ disable_feature (hb_نښان_t نښان_2)
  { جمع_کړه_feature (نښان_2, F_GLOBAL, 0); }

  فراغ جمع_کړه_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { جمع_کړه_pause (0, pause_func); }
  فراغ جمع_کړه_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { جمع_کړه_pause (1, pause_func); }

  HB_دننی فراغ compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_کلی_t &key);

  خاص:

  HB_دننی فراغ جمع_کړه_lookups (hb_ot_map_t  &m,
				بینښه صحيح  جدول_شاخص,
				بینښه صحيح  feature_شاخص,
				ثابت hb_ot_shape_plan_کلی_t &key,
				hb_ماسک_t     ماسک,
				منطقي          auto_zwnj = ریښتیا,
				منطقي          auto_zwj = ریښتیا,
				منطقي          random = خطأ,
				منطقي          فی_syllable = خطأ,
				hb_نښان_t      feature_نښان = HB_نښان(' ',' ',' ',' '));

  بنية feature_info_t {
    hb_نښان_t نښان_2;
    بینښه صحيح seq; /* sequence#, used for stable sorting only */
    بینښه صحيح max_ارزښت;
    hb_ot_map_feature_flags_t flags;
    بینښه صحيح default_ارزښت; /* for non-global features, what should the unset glyphs take */
    بینښه صحيح stage[2]; /* GSUB/GPOS */

    HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      أرجع (a->نښان_2 != b->نښان_2) ?  (a->نښان_2 < b->نښان_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  بنية stage_info_t {
    بینښه صحيح شاخص_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_دننی فراغ جمع_کړه_pause (بینښه صحيح جدول_شاخص, hb_ot_map_t::pause_func_t pause_func);

  عام:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقي is_simple;

  hb_نښان_t chosen_script[2];
  منطقي وموندل_script[2];
  بینښه صحيح script_شاخص[2], language_شاخص[2];

  خاص:

  بینښه صحيح current_stage[2]; /* GSUB/GPOS */
  hb_ویکټور_t<feature_info_t> feature_infos;
  hb_ویکټور_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#د_که_پای /* HB_OT_MAP_HH */
