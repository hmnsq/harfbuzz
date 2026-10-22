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

#جے_بنا_تعریف HB_NO_OT_SHAPE

#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout.hh"


فراغ hb_ot_map_t::collect_lookups (بنانشانی صحيح جدول_اشاریہ, hb_سیٹ_t *lookups_باہرلا) ثابت
{
  لكل (بنانشانی صحيح i = 0; i < lookups[جدول_اشاریہ].لمائی; i++)
    lookups_باہرلا->جوڑو (lookups[جدول_اشاریہ][i].اشاریہ_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (هذا, 0, حجم (*هذا));

  feature_infos.init ();
  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    stages[جدول_اشاریہ].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  بنانشانی صحيح script_گنتی = HB_OT_MAX_ٹیگ_سوچی_فی_SCRIPT;
  بنانشانی صحيح language_گنتی = HB_OT_MAX_ٹیگ_سوچی_فی_LANGUAGE;
  hb_ٹیگ_t script_ٹیگ_سوچی[HB_OT_MAX_ٹیگ_سوچی_فی_SCRIPT];
  hb_ٹیگ_t language_ٹیگ_سوچی[HB_OT_MAX_ٹیگ_سوچی_فی_LANGUAGE];

  hb_ot_ٹیگ_سوچی_from_script_and_language (props.script,
				       props.language,
				       &script_گنتی,
				       script_ٹیگ_سوچی,
				       &language_گنتی,
				       language_ٹیگ_سوچی);

  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    hb_ٹیگ_t جدول_ٹیگ_2 = جدول_ٹیگ_سوچی[جدول_اشاریہ];
    لبھیا_script[جدول_اشاریہ] = (منطقي) hb_ot_layout_جدول_select_script (face,
									 جدول_ٹیگ_2,
									 script_گنتی,
									 script_ٹیگ_سوچی,
									 &script_اشاریہ[جدول_اشاریہ],
									 &chosen_script[جدول_اشاریہ]);
    hb_ot_layout_script_select_language (face,
					 جدول_ٹیگ_2,
					 script_اشاریہ[جدول_اشاریہ],
					 language_گنتی,
					 language_ٹیگ_سوچی,
					 &language_اشاریہ[جدول_اشاریہ]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    stages[جدول_اشاریہ].fini ();
}

فراغ hb_ot_map_builder_t::جوڑو_feature (hb_ٹیگ_t ٹیگ_2,
				       hb_ot_map_feature_flags_t flags,
				       بنانشانی صحيح value)
{
  إذا (unlikely (!ٹیگ_2)) أرجع;
  feature_info_t *info = feature_infos.push();
  info->ٹیگ_2 = ٹیگ_2;
  info->seq = feature_infos.لمائی;
  info->max_قدر = value;
  info->flags = flags;
  info->default_قدر = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقي hb_ot_map_builder_t::has_feature (hb_ٹیگ_t ٹیگ_2)
{
  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    إذا (hb_ot_layout_language_لبھو_feature (face,
					    جدول_ٹیگ_سوچی[جدول_اشاریہ],
					    script_اشاریہ[جدول_اشاریہ],
					    language_اشاریہ[جدول_اشاریہ],
					    ٹیگ_2,
					    خالی_سوچک))
      أرجع سچ;
  }
  أرجع خطأ;
}

فراغ
hb_ot_map_builder_t::جوڑو_lookups (hb_ot_map_t  &m,
				  بنانشانی صحيح  جدول_اشاریہ,
				  بنانشانی صحيح  feature_اشاریہ,
				  ثابت hb_ot_shape_plan_کلید_t &key,
				  hb_نقاب_t     نقاب,
				  منطقي          auto_zwnj,
				  منطقي          auto_zwj,
				  منطقي          random,
				  منطقي          فی_syllable,
				  hb_ٹیگ_t      feature_ٹیگ)
{
  بنانشانی صحيح lookup_indices[32];
  بنانشانی صحيح وتھ, len;
  بنانشانی صحيح جدول_lookup_گنتی;

  جدول_lookup_گنتی = hb_ot_layout_جدول_get_lookup_گنتی (face, جدول_ٹیگ_سوچی[جدول_اشاریہ]);

  hb_سیٹ_t varied_lookups;
  إذا (key.get_lookup_variations (face, جدول_اشاریہ, feature_اشاریہ, &varied_lookups))
  {
    إذا (unlikely (varied_lookups.وچ_error ()))
    {
      m.lookups[جدول_اشاریہ].ensure_error ();
      أرجع;
    }
    لكل (بنانشانی صحيح lookup_اشاریہ : varied_lookups)
    {
      إذا (lookup_اشاریہ >= جدول_lookup_گنتی)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاریہ].push ();
      lookup->نقاب = نقاب;
      lookup->اشاریہ_2 = lookup_اشاریہ;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_ٹیگ = feature_ٹیگ;
    }
    أرجع;
  }

  وتھ = 0;
  افعل {
    len = ARRAY_لمائی (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      جدول_ٹیگ_سوچی[جدول_اشاریہ],
					      feature_اشاریہ,
					      key.variations_اشاریہ[جدول_اشاریہ],
						      وتھ, &len,
						      lookup_indices);

    لكل (بنانشانی صحيح i = 0; i < len; i++)
    {
      إذا (lookup_indices[i] >= جدول_lookup_گنتی)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاریہ].push ();
      lookup->نقاب = نقاب;
      lookup->اشاریہ_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_ٹیگ = feature_ٹیگ;
    }

    وتھ += len;
  } طالما (len == ARRAY_لمائی (lookup_indices));
}


فراغ hb_ot_map_builder_t::جوڑو_pause (بنانشانی صحيح جدول_اشاریہ, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[جدول_اشاریہ].push ();
  s->اشاریہ_2 = current_stage[جدول_اشاریہ];
  s->pause_func = pause_func;

  current_stage[جدول_اشاریہ]++;
}

فراغ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_کلید_t &key)
{
  بنانشانی صحيح global_ثنائی_ہندسہ_کھسکاؤ = 8 * حجم (hb_نقاب_t) - 1;
  بنانشانی صحيح global_ثنائی_ہندسہ_نقاب = 1u << global_ثنائی_ہندسہ_کھسکاؤ;

  m.global_نقاب = global_ثنائی_ہندسہ_نقاب;

  بنانشانی صحيح required_feature_اشاریہ[2];
  hb_ٹیگ_t required_feature_ٹیگ[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  بنانشانی صحيح required_feature_stage[2] = {0, 0};

  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    m.chosen_script[جدول_اشاریہ] = chosen_script[جدول_اشاریہ];
    m.لبھیا_script[جدول_اشاریہ] = لبھیا_script[جدول_اشاریہ];

    hb_ot_layout_language_get_required_feature (face,
						جدول_ٹیگ_سوچی[جدول_اشاریہ],
						script_اشاریہ[جدول_اشاریہ],
						language_اشاریہ[جدول_اشاریہ],
						&required_feature_اشاریہ[جدول_اشاریہ],
						&required_feature_ٹیگ[جدول_اشاریہ]);
  }

  /* Sort features and merge duplicates */
  إذا (feature_infos.لمائی)
  {
    إذا (!is_simple)
      feature_infos.qsort ();
    خودکار *f = feature_infos.arrayZ;
    بنانشانی صحيح j = 0;
    بنانشانی گنتی = feature_infos.لمائی;
    لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
      إذا (f[i].ٹیگ_2 != f[j].ٹیگ_2)
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
  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    hb_ot_layout_collect_features_map (face,
				       جدول_ٹیگ_سوچی[جدول_اشاریہ],
				       script_اشاریہ[جدول_اشاریہ],
				       language_اشاریہ[جدول_اشاریہ],
				       &feature_indices[جدول_اشاریہ]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  بنانشانی صحيح اگلا_ثنائی_ہندسہ = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  بنانشانی گنتی = feature_infos.لمائی;
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    بنانشانی صحيح بٹ_needed;

    إذا ((info->flags & F_GLOBAL) && info->max_قدر == 1)
      /* Uses the global bit */
      بٹ_needed = 0;
    وإلا
      /* Limit bits per feature. */
      بٹ_needed = hb_min (HB_OT_MAP_MAX_بٹ, hb_ثنائی_ہندسہ_storage (info->max_قدر));

    إذا (!info->max_قدر || اگلا_ثنائی_ہندسہ + بٹ_needed >= global_ثنائی_ہندسہ_کھسکاؤ)
      استمر; /* Feature disabled, or not enough bits. */

    منطقي لبھیا = خطأ;
    بنانشانی صحيح feature_اشاریہ[2];
    لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      إذا (required_feature_ٹیگ[جدول_اشاریہ] == info->ٹیگ_2)
	required_feature_stage[جدول_اشاریہ] = info->stage[جدول_اشاریہ];

      hb_codepoint_t *اشاریہ_2;
      إذا (feature_indices[جدول_اشاریہ].has (info->ٹیگ_2, &اشاریہ_2))
      {
        feature_اشاریہ[جدول_اشاریہ] = *اشاریہ_2;
        لبھیا = سچ;
      }
      وإلا
        feature_اشاریہ[جدول_اشاریہ] = HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
    }
    إذا (!لبھیا && (info->flags & F_GLOBAL_تلاش))
    {
      لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
      {
	لبھیا |= (منطقي) hb_ot_layout_جدول_لبھو_feature (face,
							 جدول_ٹیگ_سوچی[جدول_اشاریہ],
							 info->ٹیگ_2,
							 &feature_اشاریہ[جدول_اشاریہ]);
      }
    }
    إذا (!لبھیا && !(info->flags & F_HAS_FALLBACK))
      استمر;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->ٹیگ_2 = info->ٹیگ_2;
    map->اشاریہ_2[0] = feature_اشاریہ[0];
    map->اشاریہ_2[1] = feature_اشاریہ[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->فی_syllable = !!(info->flags & F_فی_SYLLABLE);
    إذا ((info->flags & F_GLOBAL) && info->max_قدر == 1) {
      /* Uses the global bit */
      map->کھسکاؤ = global_ثنائی_ہندسہ_کھسکاؤ;
      map->نقاب = global_ثنائی_ہندسہ_نقاب;
    } وإلا {
      map->کھسکاؤ = اگلا_ثنائی_ہندسہ;
      map->نقاب = (1u << (اگلا_ثنائی_ہندسہ + بٹ_needed)) - (1u << اگلا_ثنائی_ہندسہ);
      اگلا_ثنائی_ہندسہ += بٹ_needed;
      m.global_نقاب |= (info->default_قدر << map->کھسکاؤ) & map->نقاب;
    }
    map->_1_mask = (1u << map->کھسکاؤ) & map->نقاب;
    map->needs_fallback = !لبھیا;
  }
  //feature_infos.shrink (0); /* Done with these */
  إذا (is_simple)
    m.features.qsort ();

  جوڑو_gsub_pause (خالی_سوچک);
  جوڑو_gpos_pause (خالی_سوچک);

  لكل (بنانشانی صحيح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    /* Collect lookup indices for features */
    خودکار &lookups = m.lookups[جدول_اشاریہ];

    بنانشانی صحيح stage_اشاریہ = 0;
    بنانشانی صحيح آخری_عدد_lookups = 0;
    لكل (بنانشانی stage = 0; stage < current_stage[جدول_اشاریہ]; stage++)
    {
      إذا (required_feature_اشاریہ[جدول_اشاریہ] != HB_OT_LAYOUT_NO_FEATURE_اشاریہ &&
	  required_feature_stage[جدول_اشاریہ] == stage)
	جوڑو_lookups (m, جدول_اشاریہ,
		     required_feature_اشاریہ[جدول_اشاریہ],
		     key,
		     global_ثنائی_ہندسہ_نقاب);

      لكل (خودکار &feature : m.features)
      {
	إذا (feature.stage[جدول_اشاریہ] == stage)
	  جوڑو_lookups (m, جدول_اشاریہ,
		       feature.اشاریہ_2[جدول_اشاریہ],
		       key,
		       feature.نقاب,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.فی_syllable,
		       feature.ٹیگ_2);
      }

      /* Sort lookups and merge duplicates */
      إذا (آخری_عدد_lookups + 1 < lookups.لمائی)
      {
	lookups.as_array ().sub_array (آخری_عدد_lookups, lookups.لمائی - آخری_عدد_lookups).qsort ();

	بنانشانی صحيح j = آخری_عدد_lookups;
	لكل (بنانشانی صحيح i = j + 1; i < lookups.لمائی; i++)
	  إذا (lookups.arrayZ[i].اشاریہ_2 != lookups.arrayZ[j].اشاریہ_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  وإلا
	  {
	    lookups.arrayZ[j].نقاب |= lookups.arrayZ[i].نقاب;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      آخری_عدد_lookups = lookups.لمائی;

      إذا (stage_اشاریہ < stages[جدول_اشاریہ].لمائی && stages[جدول_اشاریہ][stage_اشاریہ].اشاریہ_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[جدول_اشاریہ].push ();
	stage_map->آخری_lookup = آخری_عدد_lookups;
	stage_map->pause_func = stages[جدول_اشاریہ][stage_اشاریہ].pause_func;

	stage_اشاریہ++;
      }
    }
  }
}

بنانشانی صحيح hb_ot_map_t::get_feature_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ, بنانشانی صحيح *ٹیگ_گنتی, hb_ٹیگ_t *ٹیگ_سوچی) ثابت
{
  إذا (ٹیگ_گنتی)
  {
    خودکار sub_features = features.as_array ().sub_array (شروع_وتھ, ٹیگ_گنتی);
    إذا (ٹیگ_سوچی)
    {
      لكل (بنانشانی صحيح i = 0; i < sub_features.لمائی; i++)
        ٹیگ_سوچی[i] = sub_features[i].ٹیگ_2;
    }
  }

  أرجع features.لمائی;
}

#جے_انت
