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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-map.hh"
#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-ot-layout.hh"


శూన్య hb_ot_map_t::collect_lookups (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_సమితి_t *lookups_బహిర్గతం) స్థిరరాశి
{
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < lookups[పట్టిక_సూచీ].నిడివి; i++)
    lookups_బహిర్గతం->కూడు (lookups[పట్టిక_సూచీ][i].సూచీ_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  స్థిరరాశి hb_segment_properties_t &props_)
{
  hb_memset (ఇది, 0, పరిమాణం (*ఇది));

  feature_infos.init ();
  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    stages[పట్టిక_సూచీ].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  గుర్తులేని పూర్ణాంకం script_లెక్క = HB_OT_MAX_ట్యాగులు_చొప్పున_SCRIPT;
  గుర్తులేని పూర్ణాంకం language_లెక్క = HB_OT_MAX_ట్యాగులు_చొప్పున_LANGUAGE;
  hb_ట్యాగ్_t script_ట్యాగులు[HB_OT_MAX_ట్యాగులు_చొప్పున_SCRIPT];
  hb_ట్యాగ్_t language_ట్యాగులు[HB_OT_MAX_ట్యాగులు_చొప్పున_LANGUAGE];

  hb_ot_ట్యాగులు_from_script_and_language (props.script,
				       props.language,
				       &script_లెక్క,
				       script_ట్యాగులు,
				       &language_లెక్క,
				       language_ట్యాగులు);

  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
  {
    hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2 = పట్టిక_ట్యాగులు[పట్టిక_సూచీ];
    దొరికింది_script[పట్టిక_సూచీ] = (తార్కిక) hb_ot_layout_పట్టిక_select_script (face,
									 పట్టిక_ట్యాగ్_2,
									 script_లెక్క,
									 script_ట్యాగులు,
									 &script_సూచీ[పట్టిక_సూచీ],
									 &chosen_script[పట్టిక_సూచీ]);
    hb_ot_layout_script_select_language (face,
					 పట్టిక_ట్యాగ్_2,
					 script_సూచీ[పట్టిక_సూచీ],
					 language_లెక్క,
					 language_ట్యాగులు,
					 &language_సూచీ[పట్టిక_సూచీ]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    stages[పట్టిక_సూచీ].fini ();
}

శూన్య hb_ot_map_builder_t::కూడు_feature (hb_ట్యాగ్_t ట్యాగ్_2,
				       hb_ot_map_feature_flags_t flags,
				       గుర్తులేని పూర్ణాంకం value)
{
  ఒకవేళ (unlikely (!ట్యాగ్_2)) తిరిగి;
  feature_info_t *info = feature_infos.push();
  info->ట్యాగ్_2 = ట్యాగ్_2;
  info->seq = feature_infos.నిడివి;
  info->max_విలువ = value;
  info->flags = flags;
  info->default_విలువ = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

తార్కిక hb_ot_map_builder_t::has_feature (hb_ట్యాగ్_t ట్యాగ్_2)
{
  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
  {
    ఒకవేళ (hb_ot_layout_language_కనుగొను_feature (face,
					    పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
					    script_సూచీ[పట్టిక_సూచీ],
					    language_సూచీ[పట్టిక_సూచీ],
					    ట్యాగ్_2,
					    శూన్యసూచిక))
      తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

శూన్య
hb_ot_map_builder_t::కూడు_lookups (hb_ot_map_t  &m,
				  గుర్తులేని పూర్ణాంకం  పట్టిక_సూచీ,
				  గుర్తులేని పూర్ణాంకం  feature_సూచీ,
				  స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key,
				  hb_ముసుగు_t     ముసుగు,
				  తార్కిక          auto_zwnj,
				  తార్కిక          auto_zwj,
				  తార్కిక          random,
				  తార్కిక          చొప్పున_syllable,
				  hb_ట్యాగ్_t      feature_ట్యాగ్)
{
  గుర్తులేని పూర్ణాంకం lookup_indices[32];
  గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, len;
  గుర్తులేని పూర్ణాంకం పట్టిక_lookup_లెక్క;

  పట్టిక_lookup_లెక్క = hb_ot_layout_పట్టిక_get_lookup_లెక్క (face, పట్టిక_ట్యాగులు[పట్టిక_సూచీ]);

  hb_సమితి_t varied_lookups;
  ఒకవేళ (key.get_lookup_variations (face, పట్టిక_సూచీ, feature_సూచీ, &varied_lookups))
  {
    ఒకవేళ (unlikely (varied_lookups.లోపల_error ()))
    {
      m.lookups[పట్టిక_సూచీ].ensure_error ();
      తిరిగి;
    }
    కొరకు (గుర్తులేని పూర్ణాంకం lookup_సూచీ : varied_lookups)
    {
      ఒకవేళ (lookup_సూచీ >= పట్టిక_lookup_లెక్క)
	కొనసాగు;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[పట్టిక_సూచీ].push ();
      lookup->ముసుగు = ముసుగు;
      lookup->సూచీ_2 = lookup_సూచీ;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->చొప్పున_syllable = చొప్పున_syllable;
      lookup->feature_ట్యాగ్ = feature_ట్యాగ్;
    }
    తిరిగి;
  }

  స్థానభ్రంశం = 0;
  చేయి {
    len = ARRAY_నిడివి (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
					      feature_సూచీ,
					      key.variations_సూచీ[పట్టిక_సూచీ],
						      స్థానభ్రంశం, &len,
						      lookup_indices);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len; i++)
    {
      ఒకవేళ (lookup_indices[i] >= పట్టిక_lookup_లెక్క)
	కొనసాగు;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[పట్టిక_సూచీ].push ();
      lookup->ముసుగు = ముసుగు;
      lookup->సూచీ_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->చొప్పున_syllable = చొప్పున_syllable;
      lookup->feature_ట్యాగ్ = feature_ట్యాగ్;
    }

    స్థానభ్రంశం += len;
  } అయితే (len == ARRAY_నిడివి (lookup_indices));
}


శూన్య hb_ot_map_builder_t::కూడు_pause (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[పట్టిక_సూచీ].push ();
  s->సూచీ_2 = current_stage[పట్టిక_సూచీ];
  s->pause_func = pause_func;

  current_stage[పట్టిక_సూచీ]++;
}

శూన్య
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key)
{
  గుర్తులేని పూర్ణాంకం global_బిట్టు_జరుపు = 8 * పరిమాణం (hb_ముసుగు_t) - 1;
  గుర్తులేని పూర్ణాంకం global_బిట్టు_ముసుగు = 1u << global_బిట్టు_జరుపు;

  m.global_ముసుగు = global_బిట్టు_ముసుగు;

  గుర్తులేని పూర్ణాంకం required_feature_సూచీ[2];
  hb_ట్యాగ్_t required_feature_ట్యాగ్[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  గుర్తులేని పూర్ణాంకం required_feature_stage[2] = {0, 0};

  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
  {
    m.chosen_script[పట్టిక_సూచీ] = chosen_script[పట్టిక_సూచీ];
    m.దొరికింది_script[పట్టిక_సూచీ] = దొరికింది_script[పట్టిక_సూచీ];

    hb_ot_layout_language_get_required_feature (face,
						పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
						script_సూచీ[పట్టిక_సూచీ],
						language_సూచీ[పట్టిక_సూచీ],
						&required_feature_సూచీ[పట్టిక_సూచీ],
						&required_feature_ట్యాగ్[పట్టిక_సూచీ]);
  }

  /* Sort features and merge duplicates */
  ఒకవేళ (feature_infos.నిడివి)
  {
    ఒకవేళ (!is_simple)
      feature_infos.qsort ();
    స్వయంచాలక *f = feature_infos.arrayZ;
    గుర్తులేని పూర్ణాంకం j = 0;
    గుర్తులేని లెక్క = feature_infos.నిడివి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
      ఒకవేళ (f[i].ట్యాగ్_2 != f[j].ట్యాగ్_2)
	f[++j] = f[i];
      లేకపోతే {
	ఒకవేళ (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_విలువ = f[i].max_విలువ;
	  f[j].default_విలువ = f[i].default_విలువ;
	} లేకపోతే {
	  ఒకవేళ (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_విలువ = hb_max (f[j].max_విలువ, f[i].max_విలువ);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    hb_ot_layout_collect_features_map (face,
				       పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
				       script_సూచీ[పట్టిక_సూచీ],
				       language_సూచీ[పట్టిక_సూచీ],
				       &feature_indices[పట్టిక_సూచీ]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  గుర్తులేని పూర్ణాంకం తదుపరి_బిట్టు = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  గుర్తులేని లెక్క = feature_infos.నిడివి;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    స్థిరరాశి feature_info_t *info = &feature_infos[i];

    గుర్తులేని పూర్ణాంకం బిట్లు_needed;

    ఒకవేళ ((info->flags & F_GLOBAL) && info->max_విలువ == 1)
      /* Uses the global bit */
      బిట్లు_needed = 0;
    లేకపోతే
      /* Limit bits per feature. */
      బిట్లు_needed = hb_min (HB_OT_MAP_MAX_బిట్లు, hb_బిట్టు_storage (info->max_విలువ));

    ఒకవేళ (!info->max_విలువ || తదుపరి_బిట్టు + బిట్లు_needed >= global_బిట్టు_జరుపు)
      కొనసాగు; /* Feature disabled, or not enough bits. */

    తార్కిక దొరికింది = అబద్ధం;
    గుర్తులేని పూర్ణాంకం feature_సూచీ[2];
    కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    {
      ఒకవేళ (required_feature_ట్యాగ్[పట్టిక_సూచీ] == info->ట్యాగ్_2)
	required_feature_stage[పట్టిక_సూచీ] = info->stage[పట్టిక_సూచీ];

      hb_codepoint_t *సూచీ_2;
      ఒకవేళ (feature_indices[పట్టిక_సూచీ].has (info->ట్యాగ్_2, &సూచీ_2))
      {
        feature_సూచీ[పట్టిక_సూచీ] = *సూచీ_2;
        దొరికింది = నిజం;
      }
      లేకపోతే
        feature_సూచీ[పట్టిక_సూచీ] = HB_OT_LAYOUT_NO_FEATURE_సూచీ;
    }
    ఒకవేళ (!దొరికింది && (info->flags & F_GLOBAL_శోధన))
    {
      కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
      {
	దొరికింది |= (తార్కిక) hb_ot_layout_పట్టిక_కనుగొను_feature (face,
							 పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
							 info->ట్యాగ్_2,
							 &feature_సూచీ[పట్టిక_సూచీ]);
      }
    }
    ఒకవేళ (!దొరికింది && !(info->flags & F_HAS_FALLBACK))
      కొనసాగు;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->ట్యాగ్_2 = info->ట్యాగ్_2;
    map->సూచీ_2[0] = feature_సూచీ[0];
    map->సూచీ_2[1] = feature_సూచీ[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->చొప్పున_syllable = !!(info->flags & F_చొప్పున_SYLLABLE);
    ఒకవేళ ((info->flags & F_GLOBAL) && info->max_విలువ == 1) {
      /* Uses the global bit */
      map->జరుపు = global_బిట్టు_జరుపు;
      map->ముసుగు = global_బిట్టు_ముసుగు;
    } లేకపోతే {
      map->జరుపు = తదుపరి_బిట్టు;
      map->ముసుగు = (1u << (తదుపరి_బిట్టు + బిట్లు_needed)) - (1u << తదుపరి_బిట్టు);
      తదుపరి_బిట్టు += బిట్లు_needed;
      m.global_ముసుగు |= (info->default_విలువ << map->జరుపు) & map->ముసుగు;
    }
    map->_1_mask = (1u << map->జరుపు) & map->ముసుగు;
    map->needs_fallback = !దొరికింది;
  }
  //feature_infos.shrink (0); /* Done with these */
  ఒకవేళ (is_simple)
    m.features.qsort ();

  కూడు_gsub_pause (శూన్యసూచిక);
  కూడు_gpos_pause (శూన్యసూచిక);

  కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
  {
    /* Collect lookup indices for features */
    స్వయంచాలక &lookups = m.lookups[పట్టిక_సూచీ];

    గుర్తులేని పూర్ణాంకం stage_సూచీ = 0;
    గుర్తులేని పూర్ణాంకం చివరి_అంకె_lookups = 0;
    కొరకు (గుర్తులేని stage = 0; stage < current_stage[పట్టిక_సూచీ]; stage++)
    {
      ఒకవేళ (required_feature_సూచీ[పట్టిక_సూచీ] != HB_OT_LAYOUT_NO_FEATURE_సూచీ &&
	  required_feature_stage[పట్టిక_సూచీ] == stage)
	కూడు_lookups (m, పట్టిక_సూచీ,
		     required_feature_సూచీ[పట్టిక_సూచీ],
		     key,
		     global_బిట్టు_ముసుగు);

      కొరకు (స్వయంచాలక &feature : m.features)
      {
	ఒకవేళ (feature.stage[పట్టిక_సూచీ] == stage)
	  కూడు_lookups (m, పట్టిక_సూచీ,
		       feature.సూచీ_2[పట్టిక_సూచీ],
		       key,
		       feature.ముసుగు,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.చొప్పున_syllable,
		       feature.ట్యాగ్_2);
      }

      /* Sort lookups and merge duplicates */
      ఒకవేళ (చివరి_అంకె_lookups + 1 < lookups.నిడివి)
      {
	lookups.as_array ().sub_array (చివరి_అంకె_lookups, lookups.నిడివి - చివరి_అంకె_lookups).qsort ();

	గుర్తులేని పూర్ణాంకం j = చివరి_అంకె_lookups;
	కొరకు (గుర్తులేని పూర్ణాంకం i = j + 1; i < lookups.నిడివి; i++)
	  ఒకవేళ (lookups.arrayZ[i].సూచీ_2 != lookups.arrayZ[j].సూచీ_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  లేకపోతే
	  {
	    lookups.arrayZ[j].ముసుగు |= lookups.arrayZ[i].ముసుగు;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      చివరి_అంకె_lookups = lookups.నిడివి;

      ఒకవేళ (stage_సూచీ < stages[పట్టిక_సూచీ].నిడివి && stages[పట్టిక_సూచీ][stage_సూచీ].సూచీ_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[పట్టిక_సూచీ].push ();
	stage_map->చివరి_lookup = చివరి_అంకె_lookups;
	stage_map->pause_func = stages[పట్టిక_సూచీ][stage_సూచీ].pause_func;

	stage_సూచీ++;
      }
    }
  }
}

గుర్తులేని పూర్ణాంకం hb_ot_map_t::get_feature_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం, గుర్తులేని పూర్ణాంకం *ట్యాగ్_లెక్క, hb_ట్యాగ్_t *ట్యాగులు) స్థిరరాశి
{
  ఒకవేళ (ట్యాగ్_లెక్క)
  {
    స్వయంచాలక sub_features = features.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, ట్యాగ్_లెక్క);
    ఒకవేళ (ట్యాగులు)
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < sub_features.నిడివి; i++)
        ట్యాగులు[i] = sub_features[i].ట్యాగ్_2;
    }
  }

  తిరిగి features.నిడివి;
}

#అయితే_ముగింపు
