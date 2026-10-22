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

#جے_بنا_تعریف HB_OT_MAP_HH
#تعریف_کرو HB_OT_MAP_HH

#ضمن "hb-buffer.hh"


#تعریف_کرو HB_OT_MAP_MAX_بٹ 8u
#تعریف_کرو HB_OT_MAP_MAX_قدر ((1u << HB_OT_MAP_MAX_بٹ) - 1u)

بنية hb_ot_shape_plan_t;

ساكن ثابت hb_ٹیگ_t جدول_ٹیگ_سوچی[2] = {HB_OT_ٹیگ_GSUB, HB_OT_ٹیگ_GPOS};

بنية hb_ot_map_t
{
  یار بنية hb_ot_map_builder_t;

  عام:

  بنية feature_map_t {
    hb_ٹیگ_t ٹیگ_2; /* should be first for our bsearch to work */
    بنانشانی صحيح اشاریہ_2[2]; /* GSUB/GPOS */
    بنانشانی صحيح stage[2]; /* GSUB/GPOS */
    بنانشانی صحيح کھسکاؤ;
    hb_نقاب_t نقاب;
    hb_نقاب_t _1_mask; /* mask for value=1, for quick access */
    بنانشانی صحيح needs_fallback : 1;
    بنانشانی صحيح auto_zwnj : 1;
    بنانشانی صحيح auto_zwj : 1;
    بنانشانی صحيح random : 1;
    بنانشانی صحيح فی_syllable : 1;

    صحيح cmp (ثابت hb_ٹیگ_t ٹیگ_4) ثابت
    { أرجع ٹیگ_4 < ٹیگ_2 ? -1 : ٹیگ_4 > ٹیگ_2 ? 1 : 0; }

    HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      أرجع a->ٹیگ_2 < b->ٹیگ_2 ? -1 : a->ٹیگ_2 > b->ٹیگ_2 ? 1 : 0;
    }
  };

  بنية lookup_map_t {
    بنانشانی چھوٹا اشاریہ_2;
    بنانشانی چھوٹا auto_zwnj : 1;
    بنانشانی چھوٹا auto_zwj : 1;
    بنانشانی چھوٹا random : 1;
    بنانشانی چھوٹا فی_syllable : 1;
    hb_نقاب_t نقاب;
    hb_ٹیگ_t feature_ٹیگ;

    HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      أرجع a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعريفنوع منطقي (*pause_func_t) (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر);

  بنية stage_map_t {
    بنانشانی صحيح آخری_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  فراغ init ()
  {
    hb_memset (هذا, 0, حجم (*هذا));

    features.init0 ();
    لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      lookups[جدول_اشاریہ].init0 ();
      stages[جدول_اشاریہ].init0 ();
    }
  }
  فراغ fini ()
  {
    features.fini ();
    لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      lookups[جدول_اشاریہ].fini ();
      stages[جدول_اشاریہ].fini ();
    }
  }

  hb_نقاب_t get_global_نقاب () ثابت { أرجع global_نقاب; }

  hb_نقاب_t get_نقاب (hb_ٹیگ_t feature_ٹیگ, بنانشانی صحيح *کھسکاؤ = خالی_سوچک) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٹیگ);
    إذا (کھسکاؤ) *کھسکاؤ = map ? map->کھسکاؤ : 0;
    أرجع map ? map->نقاب : 0;
  }

  منطقي needs_fallback (hb_ٹیگ_t feature_ٹیگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٹیگ);
    أرجع map ? map->needs_fallback : خطأ;
  }

  hb_نقاب_t get_1_نقاب (hb_ٹیگ_t feature_ٹیگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٹیگ);
    أرجع map ? map->_1_mask : 0;
  }

  بنانشانی صحيح get_feature_اشاریہ (بنانشانی صحيح جدول_اشاریہ, hb_ٹیگ_t feature_ٹیگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٹیگ);
    أرجع map ? map->اشاریہ_2[جدول_اشاریہ] : HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
  }

  بنانشانی صحيح get_feature_stage (بنانشانی صحيح جدول_اشاریہ, hb_ٹیگ_t feature_ٹیگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٹیگ);
    أرجع map ? map->stage[جدول_اشاریہ] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (بنانشانی صحيح جدول_اشاریہ, بنانشانی صحيح stage) ثابت
  {
    إذا (unlikely (stage > stages[جدول_اشاریہ].لمائی))
      أرجع hb_array<ثابت hb_ot_map_t::lookup_map_t> (خالی_سوچک, 0);

    بنانشانی صحيح شروع = stage ? stages[جدول_اشاریہ][stage - 1].آخری_lookup : 0;
    بنانشانی صحيح سرا   = stage < stages[جدول_اشاریہ].لمائی ? stages[جدول_اشاریہ][stage].آخری_lookup : lookups[جدول_اشاریہ].لمائی;
    أرجع lookups[جدول_اشاریہ].as_array ().sub_array (شروع, سرا - شروع);
  }

  HB_اندرونی فراغ collect_lookups (بنانشانی صحيح جدول_اشاریہ, hb_سیٹ_t *lookups) ثابت;
  سانچا <قسم_ناں Proxy>
  HB_اندرونی فراغ apply (ثابت Proxy &proxy,
			  ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی فراغ substitute (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی فراغ position (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی بنانشانی صحيح get_feature_ٹیگ_سوچی (بنانشانی صحيح  شروع_وتھ,
					     بنانشانی صحيح *ٹیگ_گنتی, /* IN/OUT */
					     hb_ٹیگ_t     *ٹیگ_سوچی /*باہرلا*/) ثابت;

  عام:
  hb_ٹیگ_t chosen_script[2];
  منطقي لبھیا_script[2];

  خاص:

  hb_نقاب_t global_نقاب = 0;

  hb_sorted_سمتیہ_t<feature_map_t> features;
  hb_سمتیہ_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_سمتیہ_t<stage_map_t> stages[2]; /* GSUB/GPOS */
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
  F_GLOBAL_تلاش	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_فی_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


بنية hb_ot_map_feature_t
{
  hb_ٹیگ_t ٹیگ_2;
  hb_ot_map_feature_flags_t flags;
};

بنية hb_ot_shape_plan_کلید_t;

بنية hb_ot_map_builder_t
{
  عام:

  HB_اندرونی hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_اندرونی ~hb_ot_map_builder_t ();

  HB_اندرونی فراغ جوڑو_feature (hb_ٹیگ_t ٹیگ_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				بنانشانی صحيح value=1);

  HB_اندرونی منطقي has_feature (hb_ٹیگ_t ٹیگ_2);

  فراغ جوڑو_feature (ثابت hb_ot_map_feature_t &feat)
  { جوڑو_feature (feat.ٹیگ_2, feat.flags); }

  فراغ enable_feature (hb_ٹیگ_t ٹیگ_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       بنانشانی صحيح value=1)
  { جوڑو_feature (ٹیگ_2, F_GLOBAL | flags, value); }

  فراغ disable_feature (hb_ٹیگ_t ٹیگ_2)
  { جوڑو_feature (ٹیگ_2, F_GLOBAL, 0); }

  فراغ جوڑو_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڑو_pause (0, pause_func); }
  فراغ جوڑو_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڑو_pause (1, pause_func); }

  HB_اندرونی فراغ compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_کلید_t &key);

  خاص:

  HB_اندرونی فراغ جوڑو_lookups (hb_ot_map_t  &m,
				بنانشانی صحيح  جدول_اشاریہ,
				بنانشانی صحيح  feature_اشاریہ,
				ثابت hb_ot_shape_plan_کلید_t &key,
				hb_نقاب_t     نقاب,
				منطقي          auto_zwnj = سچ,
				منطقي          auto_zwj = سچ,
				منطقي          random = خطأ,
				منطقي          فی_syllable = خطأ,
				hb_ٹیگ_t      feature_ٹیگ = HB_ٹیگ(' ',' ',' ',' '));

  بنية feature_info_t {
    hb_ٹیگ_t ٹیگ_2;
    بنانشانی صحيح seq; /* sequence#, used for stable sorting only */
    بنانشانی صحيح max_قدر;
    hb_ot_map_feature_flags_t flags;
    بنانشانی صحيح default_قدر; /* for non-global features, what should the unset glyphs take */
    بنانشانی صحيح stage[2]; /* GSUB/GPOS */

    HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      أرجع (a->ٹیگ_2 != b->ٹیگ_2) ?  (a->ٹیگ_2 < b->ٹیگ_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  بنية stage_info_t {
    بنانشانی صحيح اشاریہ_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_اندرونی فراغ جوڑو_pause (بنانشانی صحيح جدول_اشاریہ, hb_ot_map_t::pause_func_t pause_func);

  عام:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقي is_simple;

  hb_ٹیگ_t chosen_script[2];
  منطقي لبھیا_script[2];
  بنانشانی صحيح script_اشاریہ[2], language_اشاریہ[2];

  خاص:

  بنانشانی صحيح current_stage[2]; /* GSUB/GPOS */
  hb_سمتیہ_t<feature_info_t> feature_infos;
  hb_سمتیہ_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#جے_انت /* HB_OT_MAP_HH */
