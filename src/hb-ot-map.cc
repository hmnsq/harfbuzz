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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-map.hh"
#شامل "hb-ot-shape.hh"
#شامل "hb-ot-layout.hh"


خالی hb_ot_map_t::collect_lookups (غیرعلامتی صحیح جدول_اشاریہ, hb_سیٹ_t *lookups_برآمد) ثابت
{
  برائے (غیرعلامتی صحیح i = 0; i < lookups[جدول_اشاریہ].لمبائی; i++)
    lookups_برآمد->جوڑو (lookups[جدول_اشاریہ][i].اشاریہ_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (یہ, 0, حجم (*یہ));

  feature_infos.init ();
  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    stages[جدول_اشاریہ].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  غیرعلامتی صحیح script_گنتی = HB_OT_MAX_لیبل_فہرست_فی_SCRIPT;
  غیرعلامتی صحیح language_گنتی = HB_OT_MAX_لیبل_فہرست_فی_LANGUAGE;
  hb_لیبل_t script_لیبل_فہرست[HB_OT_MAX_لیبل_فہرست_فی_SCRIPT];
  hb_لیبل_t language_لیبل_فہرست[HB_OT_MAX_لیبل_فہرست_فی_LANGUAGE];

  hb_ot_لیبل_فہرست_from_script_and_language (props.script,
				       props.language,
				       &script_گنتی,
				       script_لیبل_فہرست,
				       &language_گنتی,
				       language_لیبل_فہرست);

  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    hb_لیبل_t جدول_لیبل_2 = جدول_لیبل_فہرست[جدول_اشاریہ];
    ملا_script[جدول_اشاریہ] = (منطقی) hb_ot_layout_جدول_select_script (face,
									 جدول_لیبل_2,
									 script_گنتی,
									 script_لیبل_فہرست,
									 &script_اشاریہ[جدول_اشاریہ],
									 &chosen_script[جدول_اشاریہ]);
    hb_ot_layout_script_select_language (face,
					 جدول_لیبل_2,
					 script_اشاریہ[جدول_اشاریہ],
					 language_گنتی,
					 language_لیبل_فہرست,
					 &language_اشاریہ[جدول_اشاریہ]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    stages[جدول_اشاریہ].fini ();
}

خالی hb_ot_map_builder_t::جوڑو_feature (hb_لیبل_t لیبل_2,
				       hb_ot_map_feature_flags_t flags,
				       غیرعلامتی صحیح value)
{
  اگر (unlikely (!لیبل_2)) واپس;
  feature_info_t *info = feature_infos.push();
  info->لیبل_2 = لیبل_2;
  info->seq = feature_infos.لمبائی;
  info->max_قدر = value;
  info->flags = flags;
  info->default_قدر = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقی hb_ot_map_builder_t::has_feature (hb_لیبل_t لیبل_2)
{
  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    اگر (hb_ot_layout_language_ڈھونڈو_feature (face,
					    جدول_لیبل_فہرست[جدول_اشاریہ],
					    script_اشاریہ[جدول_اشاریہ],
					    language_اشاریہ[جدول_اشاریہ],
					    لیبل_2,
					    خالی_اشاریہ))
      واپس سچ;
  }
  واپس جھوٹ;
}

خالی
hb_ot_map_builder_t::جوڑو_lookups (hb_ot_map_t  &m,
				  غیرعلامتی صحیح  جدول_اشاریہ,
				  غیرعلامتی صحیح  feature_اشاریہ,
				  ثابت hb_ot_shape_plan_کلید_t &key,
				  hb_نقاب_t     نقاب,
				  منطقی          auto_zwnj,
				  منطقی          auto_zwj,
				  منطقی          random,
				  منطقی          فی_syllable,
				  hb_لیبل_t      feature_لیبل)
{
  غیرعلامتی صحیح lookup_indices[32];
  غیرعلامتی صحیح انحراف, len;
  غیرعلامتی صحیح جدول_lookup_گنتی;

  جدول_lookup_گنتی = hb_ot_layout_جدول_get_lookup_گنتی (face, جدول_لیبل_فہرست[جدول_اشاریہ]);

  hb_سیٹ_t varied_lookups;
  اگر (key.get_lookup_variations (face, جدول_اشاریہ, feature_اشاریہ, &varied_lookups))
  {
    اگر (unlikely (varied_lookups.اندر_error ()))
    {
      m.lookups[جدول_اشاریہ].ensure_error ();
      واپس;
    }
    برائے (غیرعلامتی صحیح lookup_اشاریہ : varied_lookups)
    {
      اگر (lookup_اشاریہ >= جدول_lookup_گنتی)
	جاری;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاریہ].push ();
      lookup->نقاب = نقاب;
      lookup->اشاریہ_2 = lookup_اشاریہ;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_لیبل = feature_لیبل;
    }
    واپس;
  }

  انحراف = 0;
  کرو {
    len = ARRAY_لمبائی (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      جدول_لیبل_فہرست[جدول_اشاریہ],
					      feature_اشاریہ,
					      key.variations_اشاریہ[جدول_اشاریہ],
						      انحراف, &len,
						      lookup_indices);

    برائے (غیرعلامتی صحیح i = 0; i < len; i++)
    {
      اگر (lookup_indices[i] >= جدول_lookup_گنتی)
	جاری;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[جدول_اشاریہ].push ();
      lookup->نقاب = نقاب;
      lookup->اشاریہ_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->فی_syllable = فی_syllable;
      lookup->feature_لیبل = feature_لیبل;
    }

    انحراف += len;
  } جب (len == ARRAY_لمبائی (lookup_indices));
}


خالی hb_ot_map_builder_t::جوڑو_pause (غیرعلامتی صحیح جدول_اشاریہ, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[جدول_اشاریہ].push ();
  s->اشاریہ_2 = current_stage[جدول_اشاریہ];
  s->pause_func = pause_func;

  current_stage[جدول_اشاریہ]++;
}

خالی
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_کلید_t &key)
{
  غیرعلامتی صحیح global_ثنائی_ہندسہ_سرکاؤ = 8 * حجم (hb_نقاب_t) - 1;
  غیرعلامتی صحیح global_ثنائی_ہندسہ_نقاب = 1u << global_ثنائی_ہندسہ_سرکاؤ;

  m.global_نقاب = global_ثنائی_ہندسہ_نقاب;

  غیرعلامتی صحیح required_feature_اشاریہ[2];
  hb_لیبل_t required_feature_لیبل[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  غیرعلامتی صحیح required_feature_stage[2] = {0, 0};

  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    m.chosen_script[جدول_اشاریہ] = chosen_script[جدول_اشاریہ];
    m.ملا_script[جدول_اشاریہ] = ملا_script[جدول_اشاریہ];

    hb_ot_layout_language_get_required_feature (face,
						جدول_لیبل_فہرست[جدول_اشاریہ],
						script_اشاریہ[جدول_اشاریہ],
						language_اشاریہ[جدول_اشاریہ],
						&required_feature_اشاریہ[جدول_اشاریہ],
						&required_feature_لیبل[جدول_اشاریہ]);
  }

  /* Sort features and merge duplicates */
  اگر (feature_infos.لمبائی)
  {
    اگر (!is_simple)
      feature_infos.qsort ();
    خودکار *f = feature_infos.arrayZ;
    غیرعلامتی صحیح j = 0;
    غیرعلامتی گنتی = feature_infos.لمبائی;
    برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
      اگر (f[i].لیبل_2 != f[j].لیبل_2)
	f[++j] = f[i];
      ورنہ {
	اگر (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_قدر = f[i].max_قدر;
	  f[j].default_قدر = f[i].default_قدر;
	} ورنہ {
	  اگر (f[j].flags & F_GLOBAL)
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
  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    hb_ot_layout_collect_features_map (face,
				       جدول_لیبل_فہرست[جدول_اشاریہ],
				       script_اشاریہ[جدول_اشاریہ],
				       language_اشاریہ[جدول_اشاریہ],
				       &feature_indices[جدول_اشاریہ]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  غیرعلامتی صحیح اگلا_ثنائی_ہندسہ = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  غیرعلامتی گنتی = feature_infos.لمبائی;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    غیرعلامتی صحیح بٹ_needed;

    اگر ((info->flags & F_GLOBAL) && info->max_قدر == 1)
      /* Uses the global bit */
      بٹ_needed = 0;
    ورنہ
      /* Limit bits per feature. */
      بٹ_needed = hb_min (HB_OT_MAP_MAX_بٹ, hb_ثنائی_ہندسہ_storage (info->max_قدر));

    اگر (!info->max_قدر || اگلا_ثنائی_ہندسہ + بٹ_needed >= global_ثنائی_ہندسہ_سرکاؤ)
      جاری; /* Feature disabled, or not enough bits. */

    منطقی ملا = جھوٹ;
    غیرعلامتی صحیح feature_اشاریہ[2];
    برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
    {
      اگر (required_feature_لیبل[جدول_اشاریہ] == info->لیبل_2)
	required_feature_stage[جدول_اشاریہ] = info->stage[جدول_اشاریہ];

      hb_codepoint_t *اشاریہ_2;
      اگر (feature_indices[جدول_اشاریہ].has (info->لیبل_2, &اشاریہ_2))
      {
        feature_اشاریہ[جدول_اشاریہ] = *اشاریہ_2;
        ملا = سچ;
      }
      ورنہ
        feature_اشاریہ[جدول_اشاریہ] = HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
    }
    اگر (!ملا && (info->flags & F_GLOBAL_تلاش))
    {
      برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
      {
	ملا |= (منطقی) hb_ot_layout_جدول_ڈھونڈو_feature (face,
							 جدول_لیبل_فہرست[جدول_اشاریہ],
							 info->لیبل_2,
							 &feature_اشاریہ[جدول_اشاریہ]);
      }
    }
    اگر (!ملا && !(info->flags & F_HAS_FALLBACK))
      جاری;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->لیبل_2 = info->لیبل_2;
    map->اشاریہ_2[0] = feature_اشاریہ[0];
    map->اشاریہ_2[1] = feature_اشاریہ[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->فی_syllable = !!(info->flags & F_فی_SYLLABLE);
    اگر ((info->flags & F_GLOBAL) && info->max_قدر == 1) {
      /* Uses the global bit */
      map->سرکاؤ = global_ثنائی_ہندسہ_سرکاؤ;
      map->نقاب = global_ثنائی_ہندسہ_نقاب;
    } ورنہ {
      map->سرکاؤ = اگلا_ثنائی_ہندسہ;
      map->نقاب = (1u << (اگلا_ثنائی_ہندسہ + بٹ_needed)) - (1u << اگلا_ثنائی_ہندسہ);
      اگلا_ثنائی_ہندسہ += بٹ_needed;
      m.global_نقاب |= (info->default_قدر << map->سرکاؤ) & map->نقاب;
    }
    map->_1_mask = (1u << map->سرکاؤ) & map->نقاب;
    map->needs_fallback = !ملا;
  }
  //feature_infos.shrink (0); /* Done with these */
  اگر (is_simple)
    m.features.qsort ();

  جوڑو_gsub_pause (خالی_اشاریہ);
  جوڑو_gpos_pause (خالی_اشاریہ);

  برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
  {
    /* Collect lookup indices for features */
    خودکار &lookups = m.lookups[جدول_اشاریہ];

    غیرعلامتی صحیح stage_اشاریہ = 0;
    غیرعلامتی صحیح آخری_عدد_lookups = 0;
    برائے (غیرعلامتی stage = 0; stage < current_stage[جدول_اشاریہ]; stage++)
    {
      اگر (required_feature_اشاریہ[جدول_اشاریہ] != HB_OT_LAYOUT_NO_FEATURE_اشاریہ &&
	  required_feature_stage[جدول_اشاریہ] == stage)
	جوڑو_lookups (m, جدول_اشاریہ,
		     required_feature_اشاریہ[جدول_اشاریہ],
		     key,
		     global_ثنائی_ہندسہ_نقاب);

      برائے (خودکار &feature : m.features)
      {
	اگر (feature.stage[جدول_اشاریہ] == stage)
	  جوڑو_lookups (m, جدول_اشاریہ,
		       feature.اشاریہ_2[جدول_اشاریہ],
		       key,
		       feature.نقاب,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.فی_syllable,
		       feature.لیبل_2);
      }

      /* Sort lookups and merge duplicates */
      اگر (آخری_عدد_lookups + 1 < lookups.لمبائی)
      {
	lookups.as_array ().sub_array (آخری_عدد_lookups, lookups.لمبائی - آخری_عدد_lookups).qsort ();

	غیرعلامتی صحیح j = آخری_عدد_lookups;
	برائے (غیرعلامتی صحیح i = j + 1; i < lookups.لمبائی; i++)
	  اگر (lookups.arrayZ[i].اشاریہ_2 != lookups.arrayZ[j].اشاریہ_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  ورنہ
	  {
	    lookups.arrayZ[j].نقاب |= lookups.arrayZ[i].نقاب;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      آخری_عدد_lookups = lookups.لمبائی;

      اگر (stage_اشاریہ < stages[جدول_اشاریہ].لمبائی && stages[جدول_اشاریہ][stage_اشاریہ].اشاریہ_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[جدول_اشاریہ].push ();
	stage_map->آخری_lookup = آخری_عدد_lookups;
	stage_map->pause_func = stages[جدول_اشاریہ][stage_اشاریہ].pause_func;

	stage_اشاریہ++;
      }
    }
  }
}

غیرعلامتی صحیح hb_ot_map_t::get_feature_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف, غیرعلامتی صحیح *لیبل_گنتی, hb_لیبل_t *لیبل_فہرست) ثابت
{
  اگر (لیبل_گنتی)
  {
    خودکار sub_features = features.as_array ().sub_array (آغاز_انحراف, لیبل_گنتی);
    اگر (لیبل_فہرست)
    {
      برائے (غیرعلامتی صحیح i = 0; i < sub_features.لمبائی; i++)
        لیبل_فہرست[i] = sub_features[i].لیبل_2;
    }
  }

  واپس features.لمبائی;
}

#اگر_ختم
