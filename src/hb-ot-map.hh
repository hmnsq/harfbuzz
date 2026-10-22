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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_MAP_HH
#وصف_ڪريو HB_OT_MAP_HH

#ضمن "hb-buffer.hh"


#وصف_ڪريو HB_OT_MAP_MAX_بٽ 8u
#وصف_ڪريو HB_OT_MAP_MAX_قدر ((1u << HB_OT_MAP_MAX_بٽ) - 1u)

بنية hb_ot_shape_plan_t;

ساكن ثابت hb_ٽيگ_t جدول_ٽيگ_فهرست[2] = {HB_OT_ٽيگ_GSUB, HB_OT_ٽيگ_GPOS};

بنية hb_ot_map_t
{
  دوست بنية hb_ot_map_builder_t;

  عام:

  بنية feature_map_t {
    hb_ٽيگ_t ٽيگ_2; /* should be first for our bsearch to work */
    بنانشان صحيح اشاريو_2[2]; /* GSUB/GPOS */
    بنانشان صحيح stage[2]; /* GSUB/GPOS */
    بنانشان صحيح سرڪايو;
    hb_نقاب_t نقاب;
    hb_نقاب_t _1_mask; /* mask for value=1, for quick access */
    بنانشان صحيح needs_fallback : 1;
    بنانشان صحيح auto_zwnj : 1;
    بنانشان صحيح auto_zwj : 1;
    بنانشان صحيح random : 1;
    بنانشان صحيح في_syllable : 1;

    صحيح cmp (ثابت hb_ٽيگ_t ٽيگ_4) ثابت
    { أرجع ٽيگ_4 < ٽيگ_2 ? -1 : ٽيگ_4 > ٽيگ_2 ? 1 : 0; }

    HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_map_t *a = (ثابت feature_map_t *) pa;
      ثابت feature_map_t *b = (ثابت feature_map_t *) pb;
      أرجع a->ٽيگ_2 < b->ٽيگ_2 ? -1 : a->ٽيگ_2 > b->ٽيگ_2 ? 1 : 0;
    }
  };

  بنية lookup_map_t {
    بنانشان ننڍو اشاريو_2;
    بنانشان ننڍو auto_zwnj : 1;
    بنانشان ننڍو auto_zwj : 1;
    بنانشان ننڍو random : 1;
    بنانشان ننڍو في_syllable : 1;
    hb_نقاب_t نقاب;
    hb_ٽيگ_t feature_ٽيگ;

    HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت lookup_map_t *a = (ثابت lookup_map_t *) pa;
      ثابت lookup_map_t *b = (ثابت lookup_map_t *) pb;
      أرجع a->اشاريو_2 < b->اشاريو_2 ? -1 : a->اشاريو_2 > b->اشاريو_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  تعريفنوع منطقي (*pause_func_t) (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر);

  بنية stage_map_t {
    بنانشان صحيح آخري_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  فراغ init ()
  {
    hb_memset (هذا, 0, حجم (*هذا));

    features.init0 ();
    لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    {
      lookups[جدول_اشاريو].init0 ();
      stages[جدول_اشاريو].init0 ();
    }
  }
  فراغ fini ()
  {
    features.fini ();
    لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    {
      lookups[جدول_اشاريو].fini ();
      stages[جدول_اشاريو].fini ();
    }
  }

  hb_نقاب_t get_global_نقاب () ثابت { أرجع global_نقاب; }

  hb_نقاب_t get_نقاب (hb_ٽيگ_t feature_ٽيگ, بنانشان صحيح *سرڪايو = خالي_اشارو) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٽيگ);
    إذا (سرڪايو) *سرڪايو = map ? map->سرڪايو : 0;
    أرجع map ? map->نقاب : 0;
  }

  منطقي needs_fallback (hb_ٽيگ_t feature_ٽيگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٽيگ);
    أرجع map ? map->needs_fallback : خطأ;
  }

  hb_نقاب_t get_1_نقاب (hb_ٽيگ_t feature_ٽيگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٽيگ);
    أرجع map ? map->_1_mask : 0;
  }

  بنانشان صحيح get_feature_اشاريو (بنانشان صحيح جدول_اشاريو, hb_ٽيگ_t feature_ٽيگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٽيگ);
    أرجع map ? map->اشاريو_2[جدول_اشاريو] : HB_OT_LAYOUT_NO_FEATURE_اشاريو;
  }

  بنانشان صحيح get_feature_stage (بنانشان صحيح جدول_اشاريو, hb_ٽيگ_t feature_ٽيگ) ثابت
  {
    ثابت feature_map_t *map = features.bsearch (feature_ٽيگ);
    أرجع map ? map->stage[جدول_اشاريو] : UINT_MAX;
  }

  hb_array_t<ثابت hb_ot_map_t::lookup_map_t>
  get_stage_lookups (بنانشان صحيح جدول_اشاريو, بنانشان صحيح stage) ثابت
  {
    إذا (unlikely (stage > stages[جدول_اشاريو].ڊيگهائي))
      أرجع hb_array<ثابت hb_ot_map_t::lookup_map_t> (خالي_اشارو, 0);

    بنانشان صحيح شروعات = stage ? stages[جدول_اشاريو][stage - 1].آخري_lookup : 0;
    بنانشان صحيح پڇاڙي   = stage < stages[جدول_اشاريو].ڊيگهائي ? stages[جدول_اشاريو][stage].آخري_lookup : lookups[جدول_اشاريو].ڊيگهائي;
    أرجع lookups[جدول_اشاريو].as_array ().sub_array (شروعات, پڇاڙي - شروعات);
  }

  HB_اندروني فراغ collect_lookups (بنانشان صحيح جدول_اشاريو, hb_سيٽ_t *lookups) ثابت;
  سانچو <قسم_نالو Proxy>
  HB_اندروني فراغ apply (ثابت Proxy &proxy,
			  ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندروني فراغ substitute (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندروني فراغ position (ثابت بنية hb_ot_shape_plan_t *plan, hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندروني بنانشان صحيح get_feature_ٽيگ_فهرست (بنانشان صحيح  شروعات_وٿي,
					     بنانشان صحيح *ٽيگ_ڳڻپ, /* IN/OUT */
					     hb_ٽيگ_t     *ٽيگ_فهرست /*ٻاهريون*/) ثابت;

  عام:
  hb_ٽيگ_t chosen_script[2];
  منطقي مليو_script[2];

  خاص:

  hb_نقاب_t global_نقاب = 0;

  hb_sorted_ويڪٽر_t<feature_map_t> features;
  hb_ويڪٽر_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_ويڪٽر_t<stage_map_t> stages[2]; /* GSUB/GPOS */
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
  F_GLOBAL_ڳولا	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_في_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


بنية hb_ot_map_feature_t
{
  hb_ٽيگ_t ٽيگ_2;
  hb_ot_map_feature_flags_t flags;
};

بنية hb_ot_shape_plan_ڪنجي_t;

بنية hb_ot_map_builder_t
{
  عام:

  HB_اندروني hb_ot_map_builder_t (hb_face_t *face_,
				   ثابت hb_segment_properties_t &props_);

  HB_اندروني ~hb_ot_map_builder_t ();

  HB_اندروني فراغ جوڙيو_feature (hb_ٽيگ_t ٽيگ_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				بنانشان صحيح value=1);

  HB_اندروني منطقي has_feature (hb_ٽيگ_t ٽيگ_2);

  فراغ جوڙيو_feature (ثابت hb_ot_map_feature_t &feat)
  { جوڙيو_feature (feat.ٽيگ_2, feat.flags); }

  فراغ enable_feature (hb_ٽيگ_t ٽيگ_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       بنانشان صحيح value=1)
  { جوڙيو_feature (ٽيگ_2, F_GLOBAL | flags, value); }

  فراغ disable_feature (hb_ٽيگ_t ٽيگ_2)
  { جوڙيو_feature (ٽيگ_2, F_GLOBAL, 0); }

  فراغ جوڙيو_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڙيو_pause (0, pause_func); }
  فراغ جوڙيو_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { جوڙيو_pause (1, pause_func); }

  HB_اندروني فراغ compile (hb_ot_map_t                  &m,
			    ثابت hb_ot_shape_plan_ڪنجي_t &key);

  خاص:

  HB_اندروني فراغ جوڙيو_lookups (hb_ot_map_t  &m,
				بنانشان صحيح  جدول_اشاريو,
				بنانشان صحيح  feature_اشاريو,
				ثابت hb_ot_shape_plan_ڪنجي_t &key,
				hb_نقاب_t     نقاب,
				منطقي          auto_zwnj = سچ,
				منطقي          auto_zwj = سچ,
				منطقي          random = خطأ,
				منطقي          في_syllable = خطأ,
				hb_ٽيگ_t      feature_ٽيگ = HB_ٽيگ(' ',' ',' ',' '));

  بنية feature_info_t {
    hb_ٽيگ_t ٽيگ_2;
    بنانشان صحيح seq; /* sequence#, used for stable sorting only */
    بنانشان صحيح max_قدر;
    hb_ot_map_feature_flags_t flags;
    بنانشان صحيح default_قدر; /* for non-global features, what should the unset glyphs take */
    بنانشان صحيح stage[2]; /* GSUB/GPOS */

    HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      أرجع (a->ٽيگ_2 != b->ٽيگ_2) ?  (a->ٽيگ_2 < b->ٽيگ_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  بنية stage_info_t {
    بنانشان صحيح اشاريو_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_اندروني فراغ جوڙيو_pause (بنانشان صحيح جدول_اشاريو, hb_ot_map_t::pause_func_t pause_func);

  عام:

  hb_face_t *face;
  hb_segment_properties_t props;
  منطقي is_simple;

  hb_ٽيگ_t chosen_script[2];
  منطقي مليو_script[2];
  بنانشان صحيح script_اشاريو[2], language_اشاريو[2];

  خاص:

  بنانشان صحيح current_stage[2]; /* GSUB/GPOS */
  hb_ويڪٽر_t<feature_info_t> feature_infos;
  hb_ويڪٽر_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#جيڪڏهن_پورو /* HB_OT_MAP_HH */
