/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2010,2011,2013  Google, Inc.
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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout.hh"


فراغ hb_ot_map_t::collect_lookups (بیعلامت صحيح جدول_شاخص, hb_مجموعه_t *lookups_بیرون) ثابت
{
  لكل (بیعلامت صحيح i = 0; i < lookups[جدول_شاخص].درازی; i++)
    lookups_بیرون->جمع_کن (lookups[جدول_شاخص][i].شاخص_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (هذا, 0, حجم (*هذا));

  feature_infos.init ();
  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    stages[جدول_شاخص].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  بیعلامت صحيح script_شمار = HB_OT_MAX_برچسبها_در_هر_SCRIPT;
  بیعلامت صحيح language_شمار = HB_OT_MAX_برچسبها_در_هر_LANGUAGE;
  hb_برچسب_t script_برچسبها[HB_OT_MAX_برچسبها_در_هر_SCRIPT];
  hb_برچسب_t language_برچسبها[HB_OT_MAX_برچسبها_در_هر_LANGUAGE];

  hb_ot_برچسبها_from_script_and_language (props.script,
				       props.language,
				       &script_شمار,
				       script_برچسبها,
				       &language_شمار,
				       language_برچسبها);

  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    hb_برچسب_t جدول_برچسب_2 = جدول_برچسبها[جدول_شاخص];
    یافت_script[جدول_شاخص] = (منطقي) hb_ot_layout_جدول_select_script (face,
									 جدول_برچسب_2,
									 script_شمار,
									 script_برچسبها,
									 &script_شاخص[جدول_شاخص],
									 &chosen_script[جدول_شاخص]);
    hb_ot_layout_script_select_language (face,
					 جدول_برچسب_2,
					 script_شاخص[جدول_شاخص],
					 language_شمار,
					 language_برچسبها,
					 &language_شاخص[جدول_شاخص]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    stages[جدول_شاخص].fini ();
}

فراغ hb_ot_map_builder_t::جمع_کن_feature (hb_برچسب_t برچسب_2,
				       hb_ot_map_feature_flags_t flags,
				       بیعلامت صحيح value)
{
  إذا (unlikely (!برچسب_2)) أرجع;
  feature_info_t *info = feature_infos.push();
  info->برچسب_2 = برچسب_2;
  info->seq = feature_infos.درازی;
  info->max_مقدار = value;
  info->flags = flags;
  info->default_مقدار = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقي hb_ot_map_builder_t::has_feature (hb_برچسب_t برچسب_2)
{
  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    إذا (hb_ot_layout_language_پیدا_کن_feature (face,
					    جدول_برچسبها[جدول_شاخص],
					    script_شاخص[جدول_شاخص],
					    language_شاخص[جدول_شاخص],
					    برچسب_2,
					    اشاره_تهی))
      أرجع درست;
  }
  أرجع خطأ;
}

فراغ
hb_ot_map_builder_t::جمع_کن_lookups (hb_ot_map_t  &m,
				  بیعلامت صحيح  جدول_شاخص,
				  بیعلامت صحيح  feature_شاخص,
				  ثابت hb_ot_shape_plan_کلید_t &key,
				  hb_ماسک_t     ماسک,
				  منطقي          auto_zwnj,
				  منطقي          auto_zwj,
				  منطقي          random,
				  منطقي          در_هر_syllable,
				  hb_برچسب_t      feature_برچسب)
{
  بیعلامت صحيح lookup_indices[32];
  بیعلامت صحيح جابه_جایی, len;
  بیعلامت صحيح جدول_lookup_شمار;

  جدول_lookup_شمار = hb_ot_layout_جدول_get_lookup_شمار (face, جدول_برچسبها[جدول_شاخص]);

  hb_مجموعه_t varied_lookups;
  إذا (key.get_lookup_variations (face, جدول_شاخص, feature_شاخص, &varied_lookups))
  {
    إذا (unlikely (varied_lookups.درون_error ()))
    {
      m.lookups[جدول_شاخص].ensure_error ();
      أرجع;
    }
    لكل (بیعلامت صحيح lookup_شاخص : varied_lookups)
    {
      إذا (lookup_شاخص >= جدول_lookup_شمار)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_شاخص].push ();
      lookup->ماسک = ماسک;
      lookup->شاخص_2 = lookup_شاخص;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->در_هر_syllable = در_هر_syllable;
      lookup->feature_برچسب = feature_برچسب;
    }
    أرجع;
  }

  جابه_جایی = 0;
  افعل {
    len = ARRAY_درازی (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      جدول_برچسبها[جدول_شاخص],
					      feature_شاخص,
					      key.variations_شاخص[جدول_شاخص],
						      جابه_جایی, &len,
						      lookup_indices);

    لكل (بیعلامت صحيح i = 0; i < len; i++)
    {
      إذا (lookup_indices[i] >= جدول_lookup_شمار)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_شاخص].push ();
      lookup->ماسک = ماسک;
      lookup->شاخص_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->در_هر_syllable = در_هر_syllable;
      lookup->feature_برچسب = feature_برچسب;
    }

    جابه_جایی += len;
  } طالما (len == ARRAY_درازی (lookup_indices));
}


فراغ hb_ot_map_builder_t::جمع_کن_pause (بیعلامت صحيح جدول_شاخص, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[جدول_شاخص].push ();
  s->شاخص_2 = current_stage[جدول_شاخص];
  s->pause_func = pause_func;

  current_stage[جدول_شاخص]++;
}

فراغ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_کلید_t &key)
{
  بیعلامت صحيح global_بیت_لغزش = 8 * حجم (hb_ماسک_t) - 1;
  بیعلامت صحيح global_بیت_ماسک = 1u << global_بیت_لغزش;

  m.global_ماسک = global_بیت_ماسک;

  بیعلامت صحيح required_feature_شاخص[2];
  hb_برچسب_t required_feature_برچسب[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  بیعلامت صحيح required_feature_stage[2] = {0, 0};

  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    m.chosen_script[جدول_شاخص] = chosen_script[جدول_شاخص];
    m.یافت_script[جدول_شاخص] = یافت_script[جدول_شاخص];

    hb_ot_layout_language_get_required_feature (face,
						جدول_برچسبها[جدول_شاخص],
						script_شاخص[جدول_شاخص],
						language_شاخص[جدول_شاخص],
						&required_feature_شاخص[جدول_شاخص],
						&required_feature_برچسب[جدول_شاخص]);
  }

  /* Sort features and merge duplicates */
  إذا (feature_infos.درازی)
  {
    إذا (!is_simple)
      feature_infos.qsort ();
    خودکار *f = feature_infos.arrayZ;
    بیعلامت صحيح j = 0;
    بیعلامت شمار = feature_infos.درازی;
    لكل (بیعلامت صحيح i = 1; i < شمار; i++)
      إذا (f[i].برچسب_2 != f[j].برچسب_2)
	f[++j] = f[i];
      وإلا {
	إذا (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_مقدار = f[i].max_مقدار;
	  f[j].default_مقدار = f[i].default_مقدار;
	} وإلا {
	  إذا (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_مقدار = hb_max (f[j].max_مقدار, f[i].max_مقدار);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    hb_ot_layout_collect_features_map (face,
				       جدول_برچسبها[جدول_شاخص],
				       script_شاخص[جدول_شاخص],
				       language_شاخص[جدول_شاخص],
				       &feature_indices[جدول_شاخص]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  بیعلامت صحيح بعدی_بیت = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  بیعلامت شمار = feature_infos.درازی;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    بیعلامت صحيح بیتها_needed;

    إذا ((info->flags & F_GLOBAL) && info->max_مقدار == 1)
      /* Uses the global bit */
      بیتها_needed = 0;
    وإلا
      /* Limit bits per feature. */
      بیتها_needed = hb_min (HB_OT_MAP_MAX_بیتها, hb_بیت_storage (info->max_مقدار));

    إذا (!info->max_مقدار || بعدی_بیت + بیتها_needed >= global_بیت_لغزش)
      استمر; /* Feature disabled, or not enough bits. */

    منطقي یافت = خطأ;
    بیعلامت صحيح feature_شاخص[2];
    لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      إذا (required_feature_برچسب[جدول_شاخص] == info->برچسب_2)
	required_feature_stage[جدول_شاخص] = info->stage[جدول_شاخص];

      hb_codepoint_t *شاخص_2;
      إذا (feature_indices[جدول_شاخص].has (info->برچسب_2, &شاخص_2))
      {
        feature_شاخص[جدول_شاخص] = *شاخص_2;
        یافت = درست;
      }
      وإلا
        feature_شاخص[جدول_شاخص] = HB_OT_LAYOUT_NO_FEATURE_شاخص;
    }
    إذا (!یافت && (info->flags & F_GLOBAL_جستجو))
    {
      لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
      {
	یافت |= (منطقي) hb_ot_layout_جدول_پیدا_کن_feature (face,
							 جدول_برچسبها[جدول_شاخص],
							 info->برچسب_2,
							 &feature_شاخص[جدول_شاخص]);
      }
    }
    إذا (!یافت && !(info->flags & F_HAS_FALLBACK))
      استمر;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->برچسب_2 = info->برچسب_2;
    map->شاخص_2[0] = feature_شاخص[0];
    map->شاخص_2[1] = feature_شاخص[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->در_هر_syllable = !!(info->flags & F_در_هر_SYLLABLE);
    إذا ((info->flags & F_GLOBAL) && info->max_مقدار == 1) {
      /* Uses the global bit */
      map->لغزش = global_بیت_لغزش;
      map->ماسک = global_بیت_ماسک;
    } وإلا {
      map->لغزش = بعدی_بیت;
      map->ماسک = (1u << (بعدی_بیت + بیتها_needed)) - (1u << بعدی_بیت);
      بعدی_بیت += بیتها_needed;
      m.global_ماسک |= (info->default_مقدار << map->لغزش) & map->ماسک;
    }
    map->_1_mask = (1u << map->لغزش) & map->ماسک;
    map->needs_fallback = !یافت;
  }
  //feature_infos.shrink (0); /* Done with these */
  إذا (is_simple)
    m.features.qsort ();

  جمع_کن_gsub_pause (اشاره_تهی);
  جمع_کن_gpos_pause (اشاره_تهی);

  لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    /* Collect lookup indices for features */
    خودکار &lookups = m.lookups[جدول_شاخص];

    بیعلامت صحيح stage_شاخص = 0;
    بیعلامت صحيح آخرین_رقم_lookups = 0;
    لكل (بیعلامت stage = 0; stage < current_stage[جدول_شاخص]; stage++)
    {
      إذا (required_feature_شاخص[جدول_شاخص] != HB_OT_LAYOUT_NO_FEATURE_شاخص &&
	  required_feature_stage[جدول_شاخص] == stage)
	جمع_کن_lookups (m, جدول_شاخص,
		     required_feature_شاخص[جدول_شاخص],
		     key,
		     global_بیت_ماسک);

      لكل (خودکار &feature : m.features)
      {
	إذا (feature.stage[جدول_شاخص] == stage)
	  جمع_کن_lookups (m, جدول_شاخص,
		       feature.شاخص_2[جدول_شاخص],
		       key,
		       feature.ماسک,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.در_هر_syllable,
		       feature.برچسب_2);
      }

      /* Sort lookups and merge duplicates */
      إذا (آخرین_رقم_lookups + 1 < lookups.درازی)
      {
	lookups.as_array ().sub_array (آخرین_رقم_lookups, lookups.درازی - آخرین_رقم_lookups).qsort ();

	بیعلامت صحيح j = آخرین_رقم_lookups;
	لكل (بیعلامت صحيح i = j + 1; i < lookups.درازی; i++)
	  إذا (lookups.arrayZ[i].شاخص_2 != lookups.arrayZ[j].شاخص_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  وإلا
	  {
	    lookups.arrayZ[j].ماسک |= lookups.arrayZ[i].ماسک;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      آخرین_رقم_lookups = lookups.درازی;

      إذا (stage_شاخص < stages[جدول_شاخص].درازی && stages[جدول_شاخص][stage_شاخص].شاخص_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[جدول_شاخص].push ();
	stage_map->آخرین_lookup = آخرین_رقم_lookups;
	stage_map->pause_func = stages[جدول_شاخص][stage_شاخص].pause_func;

	stage_شاخص++;
      }
    }
  }
}

بیعلامت صحيح hb_ot_map_t::get_feature_برچسبها (بیعلامت صحيح شروع_جابه_جایی, بیعلامت صحيح *برچسب_شمار, hb_برچسب_t *برچسبها) ثابت
{
  إذا (برچسب_شمار)
  {
    خودکار sub_features = features.as_array ().sub_array (شروع_جابه_جایی, برچسب_شمار);
    إذا (برچسبها)
    {
      لكل (بیعلامت صحيح i = 0; i < sub_features.درازی; i++)
        برچسبها[i] = sub_features[i].برچسب_2;
    }
  }

  أرجع features.درازی;
}

#پایان_اگر
