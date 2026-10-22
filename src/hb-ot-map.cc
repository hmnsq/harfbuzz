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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"


ਖਾਲੀ hb_ot_map_t::collect_lookups (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *lookups_ਬਾਹਰਲਾ) ਅਚਲ
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ; i++)
    lookups_ਬਾਹਰਲਾ->ਜੋੜੋ (lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][i].ਸੂਚਕਾਂਕ_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ਅਚਲ hb_segment_properties_t &props_)
{
  hb_memset (ਇਹ, 0, ਆਕਾਰ (*ਇਹ));

  feature_infos.init ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਗਿਣਤੀ = HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_SCRIPT;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ language_ਗਿਣਤੀ = HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_LANGUAGE;
  hb_ਟੈਗ_t script_ਟੈਗ_ਸੂਚੀ[HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_SCRIPT];
  hb_ਟੈਗ_t language_ਟੈਗ_ਸੂਚੀ[HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_LANGUAGE];

  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (props.script,
				       props.language,
				       &script_ਗਿਣਤੀ,
				       script_ਟੈਗ_ਸੂਚੀ,
				       &language_ਗਿਣਤੀ,
				       language_ਟੈਗ_ਸੂਚੀ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
  {
    hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2 = ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];
    ਮਿਲਿਆ_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = (ਤਾਰਕਿਕ) hb_ot_layout_ਸਾਰਣੀ_select_script (face,
									 ਸਾਰਣੀ_ਟੈਗ_2,
									 script_ਗਿਣਤੀ,
									 script_ਟੈਗ_ਸੂਚੀ,
									 &script_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
									 &chosen_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);
    hb_ot_layout_script_select_language (face,
					 ਸਾਰਣੀ_ਟੈਗ_2,
					 script_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
					 language_ਗਿਣਤੀ,
					 language_ਟੈਗ_ਸੂਚੀ,
					 &language_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].fini ();
}

ਖਾਲੀ hb_ot_map_builder_t::ਜੋੜੋ_feature (hb_ਟੈਗ_t ਟੈਗ_2,
				       hb_ot_map_feature_flags_t flags,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ value)
{
  ਜੇ (unlikely (!ਟੈਗ_2)) ਵਾਪਸ;
  feature_info_t *info = feature_infos.push();
  info->ਟੈਗ_2 = ਟੈਗ_2;
  info->seq = feature_infos.ਲੰਮਾਈ;
  info->max_ਮੁੱਲ = value;
  info->flags = flags;
  info->default_ਮੁੱਲ = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

ਤਾਰਕਿਕ hb_ot_map_builder_t::has_feature (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
  {
    ਜੇ (hb_ot_layout_language_ਲੱਭੋ_feature (face,
					    ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
					    script_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
					    language_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
					    ਟੈਗ_2,
					    ਖਾਲੀ_ਸੂਚਕ))
      ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਖਾਲੀ
hb_ot_map_builder_t::ਜੋੜੋ_lookups (hb_ot_map_t  &m,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
				  ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key,
				  hb_ਨਕਾਬ_t     ਨਕਾਬ,
				  ਤਾਰਕਿਕ          auto_zwnj,
				  ਤਾਰਕਿਕ          auto_zwj,
				  ਤਾਰਕਿਕ          random,
				  ਤਾਰਕਿਕ          ਪ੍ਰਤੀ_syllable,
				  hb_ਟੈਗ_t      feature_ਟੈਗ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_indices[32];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, len;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_lookup_ਗਿਣਤੀ;

  ਸਾਰਣੀ_lookup_ਗਿਣਤੀ = hb_ot_layout_ਸਾਰਣੀ_get_lookup_ਗਿਣਤੀ (face, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);

  hb_ਸੈੱਟ_t varied_lookups;
  ਜੇ (key.get_lookup_variations (face, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, feature_ਸੂਚਕਾਂਕ, &varied_lookups))
  {
    ਜੇ (unlikely (varied_lookups.ਵਿੱਚ_error ()))
    {
      m.lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ensure_error ();
      ਵਾਪਸ;
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ : varied_lookups)
    {
      ਜੇ (lookup_ਸੂਚਕਾਂਕ >= ਸਾਰਣੀ_lookup_ਗਿਣਤੀ)
	ਜਾਰੀ;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].push ();
      lookup->ਨਕਾਬ = ਨਕਾਬ;
      lookup->ਸੂਚਕਾਂਕ_2 = lookup_ਸੂਚਕਾਂਕ;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->ਪ੍ਰਤੀ_syllable = ਪ੍ਰਤੀ_syllable;
      lookup->feature_ਟੈਗ = feature_ਟੈਗ;
    }
    ਵਾਪਸ;
  }

  ਵਿੱਥ = 0;
  ਕਰੋ {
    len = ARRAY_ਲੰਮਾਈ (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
					      feature_ਸੂਚਕਾਂਕ,
					      key.variations_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
						      ਵਿੱਥ, &len,
						      lookup_indices);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len; i++)
    {
      ਜੇ (lookup_indices[i] >= ਸਾਰਣੀ_lookup_ਗਿਣਤੀ)
	ਜਾਰੀ;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].push ();
      lookup->ਨਕਾਬ = ਨਕਾਬ;
      lookup->ਸੂਚਕਾਂਕ_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->ਪ੍ਰਤੀ_syllable = ਪ੍ਰਤੀ_syllable;
      lookup->feature_ਟੈਗ = feature_ਟੈਗ;
    }

    ਵਿੱਥ += len;
  } ਜਦੋਂ (len == ARRAY_ਲੰਮਾਈ (lookup_indices));
}


ਖਾਲੀ hb_ot_map_builder_t::ਜੋੜੋ_pause (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].push ();
  s->ਸੂਚਕਾਂਕ_2 = current_stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];
  s->pause_func = pause_func;

  current_stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]++;
}

ਖਾਲੀ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖਿਸਕਾਓ = 8 * ਆਕਾਰ (hb_ਨਕਾਬ_t) - 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ = 1u << global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖਿਸਕਾਓ;

  m.global_ਨਕਾਬ = global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ required_feature_ਸੂਚਕਾਂਕ[2];
  hb_ਟੈਗ_t required_feature_ਟੈਗ[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ required_feature_stage[2] = {0, 0};

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
  {
    m.chosen_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = chosen_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];
    m.ਮਿਲਿਆ_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = ਮਿਲਿਆ_script[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];

    hb_ot_layout_language_get_required_feature (face,
						ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
						script_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
						language_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
						&required_feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
						&required_feature_ਟੈਗ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);
  }

  /* Sort features and merge duplicates */
  ਜੇ (feature_infos.ਲੰਮਾਈ)
  {
    ਜੇ (!is_simple)
      feature_infos.qsort ();
    ਸਵੈਚਾਲਿਤ *f = feature_infos.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = feature_infos.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (f[i].ਟੈਗ_2 != f[j].ਟੈਗ_2)
	f[++j] = f[i];
      ਨਹੀਂਤਾਂ {
	ਜੇ (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_ਮੁੱਲ = f[i].max_ਮੁੱਲ;
	  f[j].default_ਮੁੱਲ = f[i].default_ਮੁੱਲ;
	} ਨਹੀਂਤਾਂ {
	  ਜੇ (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_ਮੁੱਲ = hb_max (f[j].max_ਮੁੱਲ, f[i].max_ਮੁੱਲ);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    hb_ot_layout_collect_features_map (face,
				       ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
				       script_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
				       language_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
				       &feature_indices[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = feature_infos.ਲੰਮਾਈ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਅਚਲ feature_info_t *info = &feature_infos[i];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ_needed;

    ਜੇ ((info->flags & F_GLOBAL) && info->max_ਮੁੱਲ == 1)
      /* Uses the global bit */
      ਬਿੱਟ_needed = 0;
    ਨਹੀਂਤਾਂ
      /* Limit bits per feature. */
      ਬਿੱਟ_needed = hb_min (HB_OT_MAP_MAX_ਬਿੱਟ, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (info->max_ਮੁੱਲ));

    ਜੇ (!info->max_ਮੁੱਲ || ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ + ਬਿੱਟ_needed >= global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖਿਸਕਾਓ)
      ਜਾਰੀ; /* Feature disabled, or not enough bits. */

    ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ[2];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
    {
      ਜੇ (required_feature_ਟੈਗ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] == info->ਟੈਗ_2)
	required_feature_stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = info->stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];

      hb_codepoint_t *ਸੂਚਕਾਂਕ_2;
      ਜੇ (feature_indices[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].has (info->ਟੈਗ_2, &ਸੂਚਕਾਂਕ_2))
      {
        feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = *ਸੂਚਕਾਂਕ_2;
        ਮਿਲਿਆ = ਸੱਚ;
      }
      ਨਹੀਂਤਾਂ
        feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] = HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ;
    }
    ਜੇ (!ਮਿਲਿਆ && (info->flags & F_GLOBAL_ਖੋਜ))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
      {
	ਮਿਲਿਆ |= (ਤਾਰਕਿਕ) hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_feature (face,
							 ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
							 info->ਟੈਗ_2,
							 &feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]);
      }
    }
    ਜੇ (!ਮਿਲਿਆ && !(info->flags & F_HAS_FALLBACK))
      ਜਾਰੀ;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->ਟੈਗ_2 = info->ਟੈਗ_2;
    map->ਸੂਚਕਾਂਕ_2[0] = feature_ਸੂਚਕਾਂਕ[0];
    map->ਸੂਚਕਾਂਕ_2[1] = feature_ਸੂਚਕਾਂਕ[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->ਪ੍ਰਤੀ_syllable = !!(info->flags & F_ਪ੍ਰਤੀ_SYLLABLE);
    ਜੇ ((info->flags & F_GLOBAL) && info->max_ਮੁੱਲ == 1) {
      /* Uses the global bit */
      map->ਖਿਸਕਾਓ = global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖਿਸਕਾਓ;
      map->ਨਕਾਬ = global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ;
    } ਨਹੀਂਤਾਂ {
      map->ਖਿਸਕਾਓ = ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ;
      map->ਨਕਾਬ = (1u << (ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ + ਬਿੱਟ_needed)) - (1u << ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ);
      ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ += ਬਿੱਟ_needed;
      m.global_ਨਕਾਬ |= (info->default_ਮੁੱਲ << map->ਖਿਸਕਾਓ) & map->ਨਕਾਬ;
    }
    map->_1_mask = (1u << map->ਖਿਸਕਾਓ) & map->ਨਕਾਬ;
    map->needs_fallback = !ਮਿਲਿਆ;
  }
  //feature_infos.shrink (0); /* Done with these */
  ਜੇ (is_simple)
    m.features.qsort ();

  ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);
  ਜੋੜੋ_gpos_pause (ਖਾਲੀ_ਸੂਚਕ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
  {
    /* Collect lookup indices for features */
    ਸਵੈਚਾਲਿਤ &lookups = m.lookups[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਅੰਕ_lookups = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ stage = 0; stage < current_stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ]; stage++)
    {
      ਜੇ (required_feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] != HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ &&
	  required_feature_stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] == stage)
	ਜੋੜੋ_lookups (m, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
		     required_feature_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
		     key,
		     global_ਦੋਆਧਾਰੀ_ਅੰਕ_ਨਕਾਬ);

      ਲਈ (ਸਵੈਚਾਲਿਤ &feature : m.features)
      {
	ਜੇ (feature.stage[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ] == stage)
	  ਜੋੜੋ_lookups (m, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
		       feature.ਸੂਚਕਾਂਕ_2[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
		       key,
		       feature.ਨਕਾਬ,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.ਪ੍ਰਤੀ_syllable,
		       feature.ਟੈਗ_2);
      }

      /* Sort lookups and merge duplicates */
      ਜੇ (ਆਖਰੀ_ਅੰਕ_lookups + 1 < lookups.ਲੰਮਾਈ)
      {
	lookups.as_array ().sub_array (ਆਖਰੀ_ਅੰਕ_lookups, lookups.ਲੰਮਾਈ - ਆਖਰੀ_ਅੰਕ_lookups).qsort ();

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਆਖਰੀ_ਅੰਕ_lookups;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = j + 1; i < lookups.ਲੰਮਾਈ; i++)
	  ਜੇ (lookups.arrayZ[i].ਸੂਚਕਾਂਕ_2 != lookups.arrayZ[j].ਸੂਚਕਾਂਕ_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  ਨਹੀਂਤਾਂ
	  {
	    lookups.arrayZ[j].ਨਕਾਬ |= lookups.arrayZ[i].ਨਕਾਬ;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      ਆਖਰੀ_ਅੰਕ_lookups = lookups.ਲੰਮਾਈ;

      ਜੇ (stage_ਸੂਚਕਾਂਕ < stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].ਲੰਮਾਈ && stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][stage_ਸੂਚਕਾਂਕ].ਸੂਚਕਾਂਕ_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].push ();
	stage_map->ਆਖਰੀ_lookup = ਆਖਰੀ_ਅੰਕ_lookups;
	stage_map->pause_func = stages[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ][stage_ਸੂਚਕਾਂਕ].pause_func;

	stage_ਸੂਚਕਾਂਕ++;
      }
    }
  }
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hb_ot_map_t::get_feature_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਟੈਗ_ਗਿਣਤੀ, hb_ਟੈਗ_t *ਟੈਗ_ਸੂਚੀ) ਅਚਲ
{
  ਜੇ (ਟੈਗ_ਗਿਣਤੀ)
  {
    ਸਵੈਚਾਲਿਤ sub_features = features.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਟੈਗ_ਗਿਣਤੀ);
    ਜੇ (ਟੈਗ_ਸੂਚੀ)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < sub_features.ਲੰਮਾਈ; i++)
        ਟੈਗ_ਸੂਚੀ[i] = sub_features[i].ਟੈਗ_2;
    }
  }

  ਵਾਪਸ features.ਲੰਮਾਈ;
}

#ਜੇ_ਅੰਤ
