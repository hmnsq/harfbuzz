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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout.hh"


فراغ hb_ot_map_t::collect_lookups (بنانشان صحيح جدول_اشاريو, hb_سيٽ_t *lookups_ٻاهريون) ثابت
{
  لكل (بنانشان صحيح i = 0; i < lookups[جدول_اشاريو].ڊيگهائي; i++)
    lookups_ٻاهريون->جوڙيو (lookups[جدول_اشاريو][i].اشاريو_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (هذا, 0, حجم (*هذا));

  feature_infos.init ();
  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    stages[جدول_اشاريو].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  بنانشان صحيح script_ڳڻپ = HB_OT_MAX_ٽيگ_فهرست_في_SCRIPT;
  بنانشان صحيح language_ڳڻپ = HB_OT_MAX_ٽيگ_فهرست_في_LANGUAGE;
  hb_ٽيگ_t script_ٽيگ_فهرست[HB_OT_MAX_ٽيگ_فهرست_في_SCRIPT];
  hb_ٽيگ_t language_ٽيگ_فهرست[HB_OT_MAX_ٽيگ_فهرست_في_LANGUAGE];

  hb_ot_ٽيگ_فهرست_from_script_and_language (props.script,
				       props.language,
				       &script_ڳڻپ,
				       script_ٽيگ_فهرست,
				       &language_ڳڻپ,
				       language_ٽيگ_فهرست);

  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
  {
    hb_ٽيگ_t جدول_ٽيگ_2 = جدول_ٽيگ_فهرست[جدول_اشاريو];
    مليو_script[جدول_اشاريو] = (منطقي) hb_ot_layout_جدول_select_script (face,
									 جدول_ٽيگ_2,
									 script_ڳڻپ,
									 script_ٽيگ_فهرست,
									 &script_اشاريو[جدول_اشاريو],
									 &chosen_script[جدول_اشاريو]);
    hb_ot_layout_script_select_language (face,
					 جدول_ٽيگ_2,
					 script_اشاريو[جدول_اشاريو],
					 language_ڳڻپ,
					 language_ٽيگ_فهرست,
					 &language_اشاريو[جدول_اشاريو]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    stages[جدول_اشاريو].fini ();
}

فراغ hb_ot_map_builder_t::جوڙيو_feature (hb_ٽيگ_t ٽيگ_2,
				       hb_ot_map_feature_flags_t flags,
				       بنانشان صحيح value)
{
  إذا (unlikely (!ٽيگ_2)) أرجع;
  feature_info_t *info = feature_infos.push();
  info->ٽيگ_2 = ٽيگ_2;
  info->seq = feature_infos.ڊيگهائي;
  info->max_قدر = value;
  info->flags = flags;
  info->default_قدر = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقي hb_ot_map_builder_t::has_feature (hb_ٽيگ_t ٽيگ_2)
{
  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
  {
    إذا (hb_ot_layout_language_ڳوليو_feature (face,
					    جدول_ٽيگ_فهرست[جدول_اشاريو],
					    script_اشاريو[جدول_اشاريو],
					    language_اشاريو[جدول_اشاريو],
					    ٽيگ_2,
					    خالي_اشارو))
      أرجع سچ;
  }
  أرجع خطأ;
}

فراغ
hb_ot_map_builder_t::جوڙيو_lookups (hb_ot_map_t  &m,
				  بنانشان صحيح  جدول_اشاريو,
				  بنانشان صحيح  feature_اشاريو,
				  ثابت hb_ot_shape_plan_ڪنجي_t &key,
				  hb_نقاب_t     نقاب,
				  منطقي          auto_zwnj,
				  منطقي          auto_zwj,
				  منطقي          random,
				  منطقي          في_syllable,
				  hb_ٽيگ_t      feature_ٽيگ)
{
  بنانشان صحيح lookup_indices[32];
  بنانشان صحيح وٿي, len;
  بنانشان صحيح جدول_lookup_ڳڻپ;

  جدول_lookup_ڳڻپ = hb_ot_layout_جدول_get_lookup_ڳڻپ (face, جدول_ٽيگ_فهرست[جدول_اشاريو]);

  hb_سيٽ_t varied_lookups;
  إذا (key.get_lookup_variations (face, جدول_اشاريو, feature_اشاريو, &varied_lookups))
  {
    إذا (unlikely (varied_lookups.اندريون_error ()))
    {
      m.lookups[جدول_اشاريو].ensure_error ();
      أرجع;
    }
    لكل (بنانشان صحيح lookup_اشاريو : varied_lookups)
    {
      إذا (lookup_اشاريو >= جدول_lookup_ڳڻپ)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاريو].push ();
      lookup->نقاب = نقاب;
      lookup->اشاريو_2 = lookup_اشاريو;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->في_syllable = في_syllable;
      lookup->feature_ٽيگ = feature_ٽيگ;
    }
    أرجع;
  }

  وٿي = 0;
  افعل {
    len = ARRAY_ڊيگهائي (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      جدول_ٽيگ_فهرست[جدول_اشاريو],
					      feature_اشاريو,
					      key.variations_اشاريو[جدول_اشاريو],
						      وٿي, &len,
						      lookup_indices);

    لكل (بنانشان صحيح i = 0; i < len; i++)
    {
      إذا (lookup_indices[i] >= جدول_lookup_ڳڻپ)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاريو].push ();
      lookup->نقاب = نقاب;
      lookup->اشاريو_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->في_syllable = في_syllable;
      lookup->feature_ٽيگ = feature_ٽيگ;
    }

    وٿي += len;
  } طالما (len == ARRAY_ڊيگهائي (lookup_indices));
}


فراغ hb_ot_map_builder_t::جوڙيو_pause (بنانشان صحيح جدول_اشاريو, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[جدول_اشاريو].push ();
  s->اشاريو_2 = current_stage[جدول_اشاريو];
  s->pause_func = pause_func;

  current_stage[جدول_اشاريو]++;
}

فراغ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_ڪنجي_t &key)
{
  بنانشان صحيح global_ٻٽو_انگ_سرڪايو = 8 * حجم (hb_نقاب_t) - 1;
  بنانشان صحيح global_ٻٽو_انگ_نقاب = 1u << global_ٻٽو_انگ_سرڪايو;

  m.global_نقاب = global_ٻٽو_انگ_نقاب;

  بنانشان صحيح required_feature_اشاريو[2];
  hb_ٽيگ_t required_feature_ٽيگ[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  بنانشان صحيح required_feature_stage[2] = {0, 0};

  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
  {
    m.chosen_script[جدول_اشاريو] = chosen_script[جدول_اشاريو];
    m.مليو_script[جدول_اشاريو] = مليو_script[جدول_اشاريو];

    hb_ot_layout_language_get_required_feature (face,
						جدول_ٽيگ_فهرست[جدول_اشاريو],
						script_اشاريو[جدول_اشاريو],
						language_اشاريو[جدول_اشاريو],
						&required_feature_اشاريو[جدول_اشاريو],
						&required_feature_ٽيگ[جدول_اشاريو]);
  }

  /* Sort features and merge duplicates */
  إذا (feature_infos.ڊيگهائي)
  {
    إذا (!is_simple)
      feature_infos.qsort ();
    خودڪار *f = feature_infos.arrayZ;
    بنانشان صحيح j = 0;
    بنانشان ڳڻپ = feature_infos.ڊيگهائي;
    لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
      إذا (f[i].ٽيگ_2 != f[j].ٽيگ_2)
	f[++j] = f[i];
      وإلا {
	إذا (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_قدر = f[i].max_قدر;
	  f[j].default_قدر = f[i].default_قدر;
	} وإلا {
	  إذا (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_قدر = hb_max (f[j].max_قدر, f[i].max_قدر);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    hb_ot_layout_collect_features_map (face,
				       جدول_ٽيگ_فهرست[جدول_اشاريو],
				       script_اشاريو[جدول_اشاريو],
				       language_اشاريو[جدول_اشاريو],
				       &feature_indices[جدول_اشاريو]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  بنانشان صحيح ايندڙ_ٻٽو_انگ = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  بنانشان ڳڻپ = feature_infos.ڊيگهائي;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    بنانشان صحيح بٽ_needed;

    إذا ((info->flags & F_GLOBAL) && info->max_قدر == 1)
      /* Uses the global bit */
      بٽ_needed = 0;
    وإلا
      /* Limit bits per feature. */
      بٽ_needed = hb_min (HB_OT_MAP_MAX_بٽ, hb_ٻٽو_انگ_storage (info->max_قدر));

    إذا (!info->max_قدر || ايندڙ_ٻٽو_انگ + بٽ_needed >= global_ٻٽو_انگ_سرڪايو)
      استمر; /* Feature disabled, or not enough bits. */

    منطقي مليو = خطأ;
    بنانشان صحيح feature_اشاريو[2];
    لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
    {
      إذا (required_feature_ٽيگ[جدول_اشاريو] == info->ٽيگ_2)
	required_feature_stage[جدول_اشاريو] = info->stage[جدول_اشاريو];

      hb_codepoint_t *اشاريو_2;
      إذا (feature_indices[جدول_اشاريو].has (info->ٽيگ_2, &اشاريو_2))
      {
        feature_اشاريو[جدول_اشاريو] = *اشاريو_2;
        مليو = سچ;
      }
      وإلا
        feature_اشاريو[جدول_اشاريو] = HB_OT_LAYOUT_NO_FEATURE_اشاريو;
    }
    إذا (!مليو && (info->flags & F_GLOBAL_ڳولا))
    {
      لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
      {
	مليو |= (منطقي) hb_ot_layout_جدول_ڳوليو_feature (face,
							 جدول_ٽيگ_فهرست[جدول_اشاريو],
							 info->ٽيگ_2,
							 &feature_اشاريو[جدول_اشاريو]);
      }
    }
    إذا (!مليو && !(info->flags & F_HAS_FALLBACK))
      استمر;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->ٽيگ_2 = info->ٽيگ_2;
    map->اشاريو_2[0] = feature_اشاريو[0];
    map->اشاريو_2[1] = feature_اشاريو[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->في_syllable = !!(info->flags & F_في_SYLLABLE);
    إذا ((info->flags & F_GLOBAL) && info->max_قدر == 1) {
      /* Uses the global bit */
      map->سرڪايو = global_ٻٽو_انگ_سرڪايو;
      map->نقاب = global_ٻٽو_انگ_نقاب;
    } وإلا {
      map->سرڪايو = ايندڙ_ٻٽو_انگ;
      map->نقاب = (1u << (ايندڙ_ٻٽو_انگ + بٽ_needed)) - (1u << ايندڙ_ٻٽو_انگ);
      ايندڙ_ٻٽو_انگ += بٽ_needed;
      m.global_نقاب |= (info->default_قدر << map->سرڪايو) & map->نقاب;
    }
    map->_1_mask = (1u << map->سرڪايو) & map->نقاب;
    map->needs_fallback = !مليو;
  }
  //feature_infos.shrink (0); /* Done with these */
  إذا (is_simple)
    m.features.qsort ();

  جوڙيو_gsub_pause (خالي_اشارو);
  جوڙيو_gpos_pause (خالي_اشارو);

  لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
  {
    /* Collect lookup indices for features */
    خودڪار &lookups = m.lookups[جدول_اشاريو];

    بنانشان صحيح stage_اشاريو = 0;
    بنانشان صحيح آخري_انگ_lookups = 0;
    لكل (بنانشان stage = 0; stage < current_stage[جدول_اشاريو]; stage++)
    {
      إذا (required_feature_اشاريو[جدول_اشاريو] != HB_OT_LAYOUT_NO_FEATURE_اشاريو &&
	  required_feature_stage[جدول_اشاريو] == stage)
	جوڙيو_lookups (m, جدول_اشاريو,
		     required_feature_اشاريو[جدول_اشاريو],
		     key,
		     global_ٻٽو_انگ_نقاب);

      لكل (خودڪار &feature : m.features)
      {
	إذا (feature.stage[جدول_اشاريو] == stage)
	  جوڙيو_lookups (m, جدول_اشاريو,
		       feature.اشاريو_2[جدول_اشاريو],
		       key,
		       feature.نقاب,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.في_syllable,
		       feature.ٽيگ_2);
      }

      /* Sort lookups and merge duplicates */
      إذا (آخري_انگ_lookups + 1 < lookups.ڊيگهائي)
      {
	lookups.as_array ().sub_array (آخري_انگ_lookups, lookups.ڊيگهائي - آخري_انگ_lookups).qsort ();

	بنانشان صحيح j = آخري_انگ_lookups;
	لكل (بنانشان صحيح i = j + 1; i < lookups.ڊيگهائي; i++)
	  إذا (lookups.arrayZ[i].اشاريو_2 != lookups.arrayZ[j].اشاريو_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  وإلا
	  {
	    lookups.arrayZ[j].نقاب |= lookups.arrayZ[i].نقاب;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      آخري_انگ_lookups = lookups.ڊيگهائي;

      إذا (stage_اشاريو < stages[جدول_اشاريو].ڊيگهائي && stages[جدول_اشاريو][stage_اشاريو].اشاريو_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[جدول_اشاريو].push ();
	stage_map->آخري_lookup = آخري_انگ_lookups;
	stage_map->pause_func = stages[جدول_اشاريو][stage_اشاريو].pause_func;

	stage_اشاريو++;
      }
    }
  }
}

بنانشان صحيح hb_ot_map_t::get_feature_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي, بنانشان صحيح *ٽيگ_ڳڻپ, hb_ٽيگ_t *ٽيگ_فهرست) ثابت
{
  إذا (ٽيگ_ڳڻپ)
  {
    خودڪار sub_features = features.as_array ().sub_array (شروعات_وٿي, ٽيگ_ڳڻپ);
    إذا (ٽيگ_فهرست)
    {
      لكل (بنانشان صحيح i = 0; i < sub_features.ڊيگهائي; i++)
        ٽيگ_فهرست[i] = sub_features[i].ٽيگ_2;
    }
  }

  أرجع features.ڊيگهائي;
}

#جيڪڏهن_پورو
