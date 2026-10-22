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

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout.hh"


فراغ hb_ot_map_t::collect_lookups (بینښه صحيح جدول_شاخص, hb_سېټ_t *lookups_بهر) ثابت
{
  لكل (بینښه صحيح i = 0; i < lookups[جدول_شاخص].اوږدتیا; i++)
    lookups_بهر->جمع_کړه (lookups[جدول_شاخص][i].شاخص_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (هذا, 0, حجم (*هذا));

  feature_infos.init ();
  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    stages[جدول_شاخص].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  بینښه صحيح script_شمېر = HB_OT_MAX_نښانونه_فی_SCRIPT;
  بینښه صحيح language_شمېر = HB_OT_MAX_نښانونه_فی_LANGUAGE;
  hb_نښان_t script_نښانونه[HB_OT_MAX_نښانونه_فی_SCRIPT];
  hb_نښان_t language_نښانونه[HB_OT_MAX_نښانونه_فی_LANGUAGE];

  hb_ot_نښانونه_from_script_and_language (props.script,
				       props.language,
				       &script_شمېر,
				       script_نښانونه,
				       &language_شمېر,
				       language_نښانونه);

  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    hb_نښان_t جدول_نښان_2 = جدول_نښانونه[جدول_شاخص];
    وموندل_script[جدول_شاخص] = (منطقي) hb_ot_layout_جدول_select_script (face,
									 جدول_نښان_2,
									 script_شمېر,
									 script_نښانونه,
									 &script_شاخص[جدول_شاخص],
									 &chosen_script[جدول_شاخص]);
    hb_ot_layout_script_select_language (face,
					 جدول_نښان_2,
					 script_شاخص[جدول_شاخص],
					 language_شمېر,
					 language_نښانونه,
					 &language_شاخص[جدول_شاخص]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    stages[جدول_شاخص].fini ();
}

فراغ hb_ot_map_builder_t::جمع_کړه_feature (hb_نښان_t نښان_2,
				       hb_ot_map_feature_flags_t flags,
				       بینښه صحيح value)
{
  إذا (unlikely (!نښان_2)) أرجع;
  feature_info_t *info = feature_infos.push();
  info->نښان_2 = نښان_2;
  info->seq = feature_infos.اوږدتیا;
  info->max_ارزښت = value;
  info->flags = flags;
  info->default_ارزښت = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقي hb_ot_map_builder_t::has_feature (hb_نښان_t نښان_2)
{
  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    إذا (hb_ot_layout_language_پیدا_کړه_feature (face,
					    جدول_نښانونه[جدول_شاخص],
					    script_شاخص[جدول_شاخص],
					    language_شاخص[جدول_شاخص],
					    نښان_2,
					    تش_اشاره))
      أرجع ریښتیا;
  }
  أرجع خطأ;
}

فراغ
hb_ot_map_builder_t::جمع_کړه_lookups (hb_ot_map_t  &m,
				  بینښه صحيح  جدول_شاخص,
				  بینښه صحيح  feature_شاخص,
				  ثابت hb_ot_shape_plan_کلی_t &key,
				  hb_ماسک_t     ماسک,
				  منطقي          auto_zwnj,
				  منطقي          auto_zwj,
				  منطقي          random,
				  منطقي          فی_syllable,
				  hb_نښان_t      feature_نښان)
{
  بینښه صحيح lookup_indices[32];
  بینښه صحيح دننه_واټن, len;
  بینښه صحيح جدول_lookup_شمېر;

  جدول_lookup_شمېر = hb_ot_layout_جدول_get_lookup_شمېر (face, جدول_نښانونه[جدول_شاخص]);

  hb_سېټ_t varied_lookups;
  إذا (key.get_lookup_variations (face, جدول_شاخص, feature_شاخص, &varied_lookups))
  {
    إذا (unlikely (varied_lookups.دننه_error ()))
    {
      m.lookups[جدول_شاخص].ensure_error ();
      أرجع;
    }
    لكل (بینښه صحيح lookup_شاخص : varied_lookups)
    {
      إذا (lookup_شاخص >= جدول_lookup_شمېر)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_شاخص].push ();
      lookup->ماسک = ماسک;
      lookup->شاخص_2 = lookup_شاخص;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_نښان = feature_نښان;
    }
    أرجع;
  }

  دننه_واټن = 0;
  افعل {
    len = ARRAY_اوږدتیا (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      جدول_نښانونه[جدول_شاخص],
					      feature_شاخص,
					      key.variations_شاخص[جدول_شاخص],
						      دننه_واټن, &len,
						      lookup_indices);

    لكل (بینښه صحيح i = 0; i < len; i++)
    {
      إذا (lookup_indices[i] >= جدول_lookup_شمېر)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_شاخص].push ();
      lookup->ماسک = ماسک;
      lookup->شاخص_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_نښان = feature_نښان;
    }

    دننه_واټن += len;
  } طالما (len == ARRAY_اوږدتیا (lookup_indices));
}


فراغ hb_ot_map_builder_t::جمع_کړه_pause (بینښه صحيح جدول_شاخص, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[جدول_شاخص].push ();
  s->شاخص_2 = current_stage[جدول_شاخص];
  s->pause_func = pause_func;

  current_stage[جدول_شاخص]++;
}

فراغ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_کلی_t &key)
{
  بینښه صحيح global_بټ_ښويدنه = 8 * حجم (hb_ماسک_t) - 1;
  بینښه صحيح global_بټ_ماسک = 1u << global_بټ_ښويدنه;

  m.global_ماسک = global_بټ_ماسک;

  بینښه صحيح required_feature_شاخص[2];
  hb_نښان_t required_feature_نښان[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  بینښه صحيح required_feature_stage[2] = {0, 0};

  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    m.chosen_script[جدول_شاخص] = chosen_script[جدول_شاخص];
    m.وموندل_script[جدول_شاخص] = وموندل_script[جدول_شاخص];

    hb_ot_layout_language_get_required_feature (face,
						جدول_نښانونه[جدول_شاخص],
						script_شاخص[جدول_شاخص],
						language_شاخص[جدول_شاخص],
						&required_feature_شاخص[جدول_شاخص],
						&required_feature_نښان[جدول_شاخص]);
  }

  /* Sort features and merge duplicates */
  إذا (feature_infos.اوږدتیا)
  {
    إذا (!is_simple)
      feature_infos.qsort ();
    خپلکاری *f = feature_infos.arrayZ;
    بینښه صحيح j = 0;
    بینښه شمېر = feature_infos.اوږدتیا;
    لكل (بینښه صحيح i = 1; i < شمېر; i++)
      إذا (f[i].نښان_2 != f[j].نښان_2)
	f[++j] = f[i];
      وإلا {
	إذا (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_ارزښت = f[i].max_ارزښت;
	  f[j].default_ارزښت = f[i].default_ارزښت;
	} وإلا {
	  إذا (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_ارزښت = hb_max (f[j].max_ارزښت, f[i].max_ارزښت);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    hb_ot_layout_collect_features_map (face,
				       جدول_نښانونه[جدول_شاخص],
				       script_شاخص[جدول_شاخص],
				       language_شاخص[جدول_شاخص],
				       &feature_indices[جدول_شاخص]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  بینښه صحيح بل_بټ = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  بینښه شمېر = feature_infos.اوږدتیا;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    بینښه صحيح بټونه_needed;

    إذا ((info->flags & F_GLOBAL) && info->max_ارزښت == 1)
      /* Uses the global bit */
      بټونه_needed = 0;
    وإلا
      /* Limit bits per feature. */
      بټونه_needed = hb_min (HB_OT_MAP_MAX_بټونه, hb_بټ_storage (info->max_ارزښت));

    إذا (!info->max_ارزښت || بل_بټ + بټونه_needed >= global_بټ_ښويدنه)
      استمر; /* Feature disabled, or not enough bits. */

    منطقي وموندل = خطأ;
    بینښه صحيح feature_شاخص[2];
    لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
    {
      إذا (required_feature_نښان[جدول_شاخص] == info->نښان_2)
	required_feature_stage[جدول_شاخص] = info->stage[جدول_شاخص];

      hb_codepoint_t *شاخص_2;
      إذا (feature_indices[جدول_شاخص].has (info->نښان_2, &شاخص_2))
      {
        feature_شاخص[جدول_شاخص] = *شاخص_2;
        وموندل = ریښتیا;
      }
      وإلا
        feature_شاخص[جدول_شاخص] = HB_OT_LAYOUT_NO_FEATURE_شاخص;
    }
    إذا (!وموندل && (info->flags & F_GLOBAL_پلټنه))
    {
      لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
      {
	وموندل |= (منطقي) hb_ot_layout_جدول_پیدا_کړه_feature (face,
							 جدول_نښانونه[جدول_شاخص],
							 info->نښان_2,
							 &feature_شاخص[جدول_شاخص]);
      }
    }
    إذا (!وموندل && !(info->flags & F_HAS_FALLBACK))
      استمر;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->نښان_2 = info->نښان_2;
    map->شاخص_2[0] = feature_شاخص[0];
    map->شاخص_2[1] = feature_شاخص[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->فی_syllable = !!(info->flags & F_فی_SYLLABLE);
    إذا ((info->flags & F_GLOBAL) && info->max_ارزښت == 1) {
      /* Uses the global bit */
      map->ښويدنه = global_بټ_ښويدنه;
      map->ماسک = global_بټ_ماسک;
    } وإلا {
      map->ښويدنه = بل_بټ;
      map->ماسک = (1u << (بل_بټ + بټونه_needed)) - (1u << بل_بټ);
      بل_بټ += بټونه_needed;
      m.global_ماسک |= (info->default_ارزښت << map->ښويدنه) & map->ماسک;
    }
    map->_1_mask = (1u << map->ښويدنه) & map->ماسک;
    map->needs_fallback = !وموندل;
  }
  //feature_infos.shrink (0); /* Done with these */
  إذا (is_simple)
    m.features.qsort ();

  جمع_کړه_gsub_pause (تش_اشاره);
  جمع_کړه_gpos_pause (تش_اشاره);

  لكل (بینښه صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
  {
    /* Collect lookup indices for features */
    خپلکاری &lookups = m.lookups[جدول_شاخص];

    بینښه صحيح stage_شاخص = 0;
    بینښه صحيح وروستی_عدد_lookups = 0;
    لكل (بینښه stage = 0; stage < current_stage[جدول_شاخص]; stage++)
    {
      إذا (required_feature_شاخص[جدول_شاخص] != HB_OT_LAYOUT_NO_FEATURE_شاخص &&
	  required_feature_stage[جدول_شاخص] == stage)
	جمع_کړه_lookups (m, جدول_شاخص,
		     required_feature_شاخص[جدول_شاخص],
		     key,
		     global_بټ_ماسک);

      لكل (خپلکاری &feature : m.features)
      {
	إذا (feature.stage[جدول_شاخص] == stage)
	  جمع_کړه_lookups (m, جدول_شاخص,
		       feature.شاخص_2[جدول_شاخص],
		       key,
		       feature.ماسک,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.فی_syllable,
		       feature.نښان_2);
      }

      /* Sort lookups and merge duplicates */
      إذا (وروستی_عدد_lookups + 1 < lookups.اوږدتیا)
      {
	lookups.as_array ().sub_array (وروستی_عدد_lookups, lookups.اوږدتیا - وروستی_عدد_lookups).qsort ();

	بینښه صحيح j = وروستی_عدد_lookups;
	لكل (بینښه صحيح i = j + 1; i < lookups.اوږدتیا; i++)
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

      وروستی_عدد_lookups = lookups.اوږدتیا;

      إذا (stage_شاخص < stages[جدول_شاخص].اوږدتیا && stages[جدول_شاخص][stage_شاخص].شاخص_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[جدول_شاخص].push ();
	stage_map->وروستی_lookup = وروستی_عدد_lookups;
	stage_map->pause_func = stages[جدول_شاخص][stage_شاخص].pause_func;

	stage_شاخص++;
      }
    }
  }
}

بینښه صحيح hb_ot_map_t::get_feature_نښانونه (بینښه صحيح پیل_دننه_واټن, بینښه صحيح *نښان_شمېر, hb_نښان_t *نښانونه) ثابت
{
  إذا (نښان_شمېر)
  {
    خپلکاری sub_features = features.as_array ().sub_array (پیل_دننه_واټن, نښان_شمېر);
    إذا (نښانونه)
    {
      لكل (بینښه صحيح i = 0; i < sub_features.اوږدتیا; i++)
        نښانونه[i] = sub_features[i].نښان_2;
    }
  }

  أرجع features.اوږدتیا;
}

#د_که_پای
