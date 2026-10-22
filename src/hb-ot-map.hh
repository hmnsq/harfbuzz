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

#اگر_غیر_تعریف_شدہ HB_OT_MAP_HH
#تعریف_کرو HB_OT_MAP_HH

#شامل "hb-buffer.hh"


#تعریف_کرو HB_OT_MAP_MAX_بٹ 8u
#تعریف_کرو HB_OT_MAP_MAX_قدر ((1u << HB_OT_MAP_MAX_بٹ) - 1u)

ساخت hb_ot_shape_plan_t;

ساکن ثابت hb_لیبل_t جدول_لیبل_فہرست[2] = {HB_OT_لیبل_GSUB, HB_OT_لیبل_GPOS};

ساخت hb_ot_map_t
{
  دوست ساخت hb_ot_map_builder_t;

  عوامی:

  ساخت feature_map_t {
    hb_لیبل_t لیبل_2; /* should be first for our bsearch to work */
    غیرعلامتی صحیح اشاریہ_2[2]; /* GSUB/GPOS */
    غیرعلامتی صحیح stage[2]; /* GSUB/GPOS */
    غیرعلامتی صحیح سرکاؤ;
    hb_نقاب_t نقاب;
    hb_نقاب_t _1_mask; /* mask for value=1, for quick access */
    غیرعلامتی صحیح needs_fallback : 1;
    غیرعلامتی صحیح auto_zwnj : 1;
    غیرعلامتی صحیح auto_zwj : 1;
    غیرعلامتی صحیح random : 1;
    غیرعلامتی صحیح فی_syllable : 1;

    صحیح cmp (ثابت hb_لیبل_t لیبل_4) ثابت
    { واپس لیبل_4 < لیبل_2 ? -1 : لیبل_4 > لیبل_2 ? 1 : 0; }

    HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      واپس a->لیبل_2 < b->لیبل_2 ? -1 : a->لیبل_2 > b->لیبل_2 ? 1 : 0;
    }
  };

  ساخت lookup_map_t {
    غیرعلامتی مختصر اشاریہ_2;
    غیرعلامتی مختصر auto_zwnj : 1;
    غیرعلامتی مختصر auto_zwj : 1;
    غیرعلامتی مختصر random : 1;
    غیرعلامتی مختصر فی_syllable : 1;
    hb_نقاب_t نقاب;
    hb_لیبل_t feature_لیبل;

    HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      واپس a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  قسمتعریف منطقی (*pause_func_t) (ثابت ساخت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر);

  ساخت stage_map_t {
    غیرعلامتی صحیح آخری_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  خالی init ()
  {
    hb_memset (یہ, 0, حجم (*یہ));

    features.init0 ();
    برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      lookups[جدول_اشاریہ].init0 ();
      stages[جدول_اشاریہ].init0 ();
    }
  }
  خالی fini ()
  {
    features.fini ();
    برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      lookups[جدول_اشاریہ].fini ();
      stages[جدول_اشاریہ].fini ();
    }
  }

  hb_نقاب_t get_global_نقاب () ثابت { واپس global_نقاب; }

  hb_نقاب_t get_نقاب (hb_لیبل_t feature_لیبل, غیرعلامتی صحیح *سرکاؤ = خالی_اشاریہ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_لیبل);
    اگر (سرکاؤ) *سرکاؤ = map ? map->سرکاؤ : 0;
    واپس map ? map->نقاب : 0;
  }

  منطقی needs_fallback (hb_لیبل_t feature_لیبل) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_لیبل);
    واپس map ? map->needs_fallback : جھوٹ;
  }

  hb_نقاب_t get_1_نقاب (hb_لیبل_t feature_لیبل) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_لیبل);
    واپس map ? map->_1_mask : 0;
  }

  غیرعلامتی صحیح get_feature_اشاریہ (غیرعلامتی صحیح جدول_اشاریہ, hb_لیبل_t feature_لیبل) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_لیبل);
    واپس map ? map->اشاریہ_2[جدول_اشاریہ] : HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
  }

  غیرعلامتی صحیح get_feature_stage (غیرعلامتی صحیح جدول_اشاریہ, hb_لیبل_t feature_لیبل) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_لیبل);
    واپس map ? map->stage[جدول_اشاریہ] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (غیرعلامتی صحیح جدول_اشاریہ, غیرعلامتی صحیح stage) ثابت
  {
    اگر (unlikely (stage > stages[جدول_اشاریہ].لمبائی))
      واپس hb_array<ثابت hb_ot_map_t::lookup_map_t> (خالی_اشاریہ, 0);

    غیرعلامتی صحیح آغاز = stage ? stages[جدول_اشاریہ][stage - 1].آخری_lookup : 0;
    غیرعلامتی صحیح انتہا   = stage < stages[جدول_اشاریہ].لمبائی ? stages[جدول_اشاریہ][stage].آخری_lookup : lookups[جدول_اشاریہ].لمبائی;
    واپس lookups[جدول_اشاریہ].as_array ().sub_array (آغاز, انتہا - آغاز);
  }

  HB_اندرونی خالی collect_lookups (غیرعلامتی صحیح جدول_اشاریہ, hb_سیٹ_t *lookups) ثابت;
  سانچہ <قسم_نام Proxy>
  HB_اندرونی خالی apply (ثابت Proxy &proxy,
			  ثابت ساخت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی خالی substitute (ثابت ساخت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی خالی position (ثابت ساخت hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی غیرعلامتی صحیح get_feature_لیبل_فہرست (غیرعلامتی صحیح  آغاز_انحراف,
					     غیرعلامتی صحیح *لیبل_گنتی, /* IN/OUT */
					     hb_لیبل_t     *لیبل_فہرست /*برآمد*/) ثابت;

  عوامی:
  hb_لیبل_t chosen_script[2];
  منطقی ملا_script[2];

  نجی:

  hb_نقاب_t global_نقاب = 0;

  hb_sorted_سمتیہ_t<feature_map_t> features;
  hb_سمتیہ_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_سمتیہ_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

شمار hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_تلاش	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_فی_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


ساخت hb_ot_map_feature_t
{
  hb_لیبل_t لیبل_2;
  hb_ot_map_feature_flags_t flags;
};

ساخت hb_ot_shape_plan_کلید_t;

ساخت hb_ot_map_builder_t
{
  عوامی:

  HB_اندرونی hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_اندرونی ~hb_ot_map_builder_t ();

  HB_اندرونی خالی جوڑو_feature (hb_لیبل_t لیبل_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				غیرعلامتی صحیح value=1);

  HB_اندرونی منطقی has_feature (hb_لیبل_t لیبل_2);

  خالی جوڑو_feature (ثابت hb_ot_map_feature_t &feat)
  { جوڑو_feature (feat.لیبل_2, feat.flags); }

  خالی enable_feature (hb_لیبل_t لیبل_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       غیرعلامتی صحیح value=1)
  { جوڑو_feature (لیبل_2, F_GLOBAL | flags, value); }

  خالی disable_feature (hb_لیبل_t لیبل_2)
  { جوڑو_feature (لیبل_2, F_GLOBAL, 0); }

  خالی جوڑو_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڑو_pause (0, pause_func); }
  خالی جوڑو_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڑو_pause (1, pause_func); }

  HB_اندرونی خالی compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_کلید_t &key);

  نجی:

  HB_اندرونی خالی جوڑو_lookups (hb_ot_map_t  &m,
				غیرعلامتی صحیح  جدول_اشاریہ,
				غیرعلامتی صحیح  feature_اشاریہ,
				ثابت hb_ot_shape_plan_کلید_t &key,
				hb_نقاب_t     نقاب,
				منطقی          auto_zwnj = سچ,
				منطقی          auto_zwj = سچ,
				منطقی          random = جھوٹ,
				منطقی          فی_syllable = جھوٹ,
				hb_لیبل_t      feature_لیبل = HB_لیبل(' ',' ',' ',' '));

  ساخت feature_info_t {
    hb_لیبل_t لیبل_2;
    غیرعلامتی صحیح seq; /* sequence#, used for stable sorting only */
    غیرعلامتی صحیح max_قدر;
    hb_ot_map_feature_flags_t flags;
    غیرعلامتی صحیح default_قدر; /* for non-global features, what should the unset glyphs take */
    غیرعلامتی صحیح stage[2]; /* GSUB/GPOS */

    HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      واپس (a->لیبل_2 != b->لیبل_2) ?  (a->لیبل_2 < b->لیبل_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  ساخت stage_info_t {
    غیرعلامتی صحیح اشاریہ_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_اندرونی خالی جوڑو_pause (غیرعلامتی صحیح جدول_اشاریہ, hb_ot_map_t::pause_func_t pause_func);

  عوامی:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقی is_simple;

  hb_لیبل_t chosen_script[2];
  منطقی ملا_script[2];
  غیرعلامتی صحیح script_اشاریہ[2], language_اشاریہ[2];

  نجی:

  غیرعلامتی صحیح current_stage[2]; /* GSUB/GPOS */
  hb_سمتیہ_t<feature_info_t> feature_infos;
  hb_سمتیہ_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#اگر_ختم /* HB_OT_MAP_HH */
