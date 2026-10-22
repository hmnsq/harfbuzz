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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-map.hh"
#समावेश "hb-ot-shape.hh"
#समावेश "hb-ot-layout.hh"


व्योम hb_ot_map_t::collect_lookups (अचिन्हित पूर्णांक table_सूचकांक, hb_set_t *lookups_out) स्थिर
{
  क्रम (अचिन्हित पूर्णांक i = 0; i < lookups[table_सूचकांक].लंबाई; i++)
    lookups_out->add (lookups[table_सूचकांक][i].सूचकांक_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  स्थिर hb_segment_properties_t &props_)
{
  hb_memset (यह, 0, माप (*यह));

  feature_infos.init ();
  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    stages[table_सूचकांक].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  अचिन्हित पूर्णांक script_गिनती = HB_OT_MAX_TAGS_PER_SCRIPT;
  अचिन्हित पूर्णांक language_गिनती = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_गिनती,
				       script_tags,
				       &language_गिनती,
				       language_tags);

  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
  {
    hb_tag_t table_tag = table_tags[table_सूचकांक];
    found_script[table_सूचकांक] = (bool) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_गिनती,
									 script_tags,
									 &script_सूचकांक[table_सूचकांक],
									 &chosen_script[table_सूचकांक]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_सूचकांक[table_सूचकांक],
					 language_गिनती,
					 language_tags,
					 &language_सूचकांक[table_सूचकांक]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    stages[table_सूचकांक].fini ();
}

व्योम hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       अचिन्हित पूर्णांक value)
{
  अगर (unlikely (!tag)) वापस;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.लंबाई;
  info->max_मूल्य = value;
  info->flags = flags;
  info->default_मूल्य = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

bool hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
  {
    अगर (hb_ot_layout_language_find_feature (face,
					    table_tags[table_सूचकांक],
					    script_सूचकांक[table_सूचकांक],
					    language_सूचकांक[table_सूचकांक],
					    tag,
					    nullptr))
      वापस true;
  }
  वापस false;
}

व्योम
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  अचिन्हित पूर्णांक  table_सूचकांक,
				  अचिन्हित पूर्णांक  feature_सूचकांक,
				  स्थिर hb_ot_shape_plan_कुंजी_t &key,
				  hb_mask_t     mask,
				  bool          auto_zwnj,
				  bool          auto_zwj,
				  bool          random,
				  bool          per_syllable,
				  hb_tag_t      feature_tag)
{
  अचिन्हित पूर्णांक lookup_indices[32];
  अचिन्हित पूर्णांक offset, len;
  अचिन्हित पूर्णांक table_lookup_गिनती;

  table_lookup_गिनती = hb_ot_layout_table_get_lookup_गिनती (face, table_tags[table_सूचकांक]);

  hb_set_t varied_lookups;
  अगर (key.get_lookup_variations (face, table_सूचकांक, feature_सूचकांक, &varied_lookups))
  {
    अगर (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_सूचकांक].ensure_error ();
      वापस;
    }
    क्रम (अचिन्हित पूर्णांक lookup_सूचकांक : varied_lookups)
    {
      अगर (lookup_सूचकांक >= table_lookup_गिनती)
	जारी;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_सूचकांक].push ();
      lookup->mask = mask;
      lookup->सूचकांक_2 = lookup_सूचकांक;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    वापस;
  }

  offset = 0;
  करो {
    len = ARRAY_लंबाई (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_सूचकांक],
					      feature_सूचकांक,
					      key.variations_सूचकांक[table_सूचकांक],
						      offset, &len,
						      lookup_indices);

    क्रम (अचिन्हित पूर्णांक i = 0; i < len; i++)
    {
      अगर (lookup_indices[i] >= table_lookup_गिनती)
	जारी;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_सूचकांक].push ();
      lookup->mask = mask;
      lookup->सूचकांक_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }

    offset += len;
  } जबतक (len == ARRAY_लंबाई (lookup_indices));
}


व्योम hb_ot_map_builder_t::add_pause (अचिन्हित पूर्णांक table_सूचकांक, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_सूचकांक].push ();
  s->सूचकांक_2 = current_stage[table_सूचकांक];
  s->pause_func = pause_func;

  current_stage[table_सूचकांक]++;
}

व्योम
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      स्थिर hb_ot_shape_plan_कुंजी_t &key)
{
  अचिन्हित पूर्णांक global_bit_shift = 8 * माप (hb_mask_t) - 1;
  अचिन्हित पूर्णांक global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  अचिन्हित पूर्णांक required_feature_सूचकांक[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  अचिन्हित पूर्णांक required_feature_stage[2] = {0, 0};

  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
  {
    m.chosen_script[table_सूचकांक] = chosen_script[table_सूचकांक];
    m.found_script[table_सूचकांक] = found_script[table_सूचकांक];

    hb_ot_layout_language_get_required_feature (face,
						table_tags[table_सूचकांक],
						script_सूचकांक[table_सूचकांक],
						language_सूचकांक[table_सूचकांक],
						&required_feature_सूचकांक[table_सूचकांक],
						&required_feature_tag[table_सूचकांक]);
  }

  /* Sort features and merge duplicates */
  अगर (feature_infos.लंबाई)
  {
    अगर (!is_simple)
      feature_infos.क्विक ();
    स्वतः *f = feature_infos.arrayZ;
    अचिन्हित पूर्णांक j = 0;
    अचिन्हित गिनती = feature_infos.लंबाई;
    क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
      अगर (f[i].tag != f[j].tag)
	f[++j] = f[i];
      अन्यथा {
	अगर (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_मूल्य = f[i].max_मूल्य;
	  f[j].default_मूल्य = f[i].default_मूल्य;
	} अन्यथा {
	  अगर (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_मूल्य = hb_max (f[j].max_मूल्य, f[i].max_मूल्य);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_सूचकांक],
				       script_सूचकांक[table_सूचकांक],
				       language_सूचकांक[table_सूचकांक],
				       &feature_indices[table_सूचकांक]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  अचिन्हित पूर्णांक अग्र_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  अचिन्हित गिनती = feature_infos.लंबाई;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    स्थिर feature_info_t *info = &feature_infos[i];

    अचिन्हित पूर्णांक bits_needed;

    अगर ((info->flags & F_GLOBAL) && info->max_मूल्य == 1)
      /* Uses the global bit */
      bits_needed = 0;
    अन्यथा
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_मूल्य));

    अगर (!info->max_मूल्य || अग्र_bit + bits_needed >= global_bit_shift)
      जारी; /* Feature disabled, or not enough bits. */

    bool found = false;
    अचिन्हित पूर्णांक feature_सूचकांक[2];
    क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
    {
      अगर (required_feature_tag[table_सूचकांक] == info->tag)
	required_feature_stage[table_सूचकांक] = info->stage[table_सूचकांक];

      hb_codepoint_t *सूचकांक_2;
      अगर (feature_indices[table_सूचकांक].has (info->tag, &सूचकांक_2))
      {
        feature_सूचकांक[table_सूचकांक] = *सूचकांक_2;
        found = true;
      }
      अन्यथा
        feature_सूचकांक[table_सूचकांक] = HB_OT_LAYOUT_NO_FEATURE_सूचकांक;
    }
    अगर (!found && (info->flags & F_GLOBAL_खोज))
    {
      क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
      {
	found |= (bool) hb_ot_layout_table_find_feature (face,
							 table_tags[table_सूचकांक],
							 info->tag,
							 &feature_सूचकांक[table_सूचकांक]);
      }
    }
    अगर (!found && !(info->flags & F_HAS_FALLBACK))
      जारी;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->tag = info->tag;
    map->सूचकांक_2[0] = feature_सूचकांक[0];
    map->सूचकांक_2[1] = feature_सूचकांक[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->per_syllable = !!(info->flags & F_PER_SYLLABLE);
    अगर ((info->flags & F_GLOBAL) && info->max_मूल्य == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } अन्यथा {
      map->shift = अग्र_bit;
      map->mask = (1u << (अग्र_bit + bits_needed)) - (1u << अग्र_bit);
      अग्र_bit += bits_needed;
      m.global_mask |= (info->default_मूल्य << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  अगर (is_simple)
    m.features.क्विक ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
  {
    /* Collect lookup indices for features */
    स्वतः &lookups = m.lookups[table_सूचकांक];

    अचिन्हित पूर्णांक stage_सूचकांक = 0;
    अचिन्हित पूर्णांक last_अंक_lookups = 0;
    क्रम (अचिन्हित stage = 0; stage < current_stage[table_सूचकांक]; stage++)
    {
      अगर (required_feature_सूचकांक[table_सूचकांक] != HB_OT_LAYOUT_NO_FEATURE_सूचकांक &&
	  required_feature_stage[table_सूचकांक] == stage)
	add_lookups (m, table_सूचकांक,
		     required_feature_सूचकांक[table_सूचकांक],
		     key,
		     global_bit_mask);

      क्रम (स्वतः &feature : m.features)
      {
	अगर (feature.stage[table_सूचकांक] == stage)
	  add_lookups (m, table_सूचकांक,
		       feature.सूचकांक_2[table_सूचकांक],
		       key,
		       feature.mask,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.per_syllable,
		       feature.tag);
      }

      /* Sort lookups and merge duplicates */
      अगर (last_अंक_lookups + 1 < lookups.लंबाई)
      {
	lookups.as_array ().sub_array (last_अंक_lookups, lookups.लंबाई - last_अंक_lookups).क्विक ();

	अचिन्हित पूर्णांक j = last_अंक_lookups;
	क्रम (अचिन्हित पूर्णांक i = j + 1; i < lookups.लंबाई; i++)
	  अगर (lookups.arrayZ[i].सूचकांक_2 != lookups.arrayZ[j].सूचकांक_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  अन्यथा
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_अंक_lookups = lookups.लंबाई;

      अगर (stage_सूचकांक < stages[table_सूचकांक].लंबाई && stages[table_सूचकांक][stage_सूचकांक].सूचकांक_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_सूचकांक].push ();
	stage_map->last_lookup = last_अंक_lookups;
	stage_map->pause_func = stages[table_सूचकांक][stage_सूचकांक].pause_func;

	stage_सूचकांक++;
      }
    }
  }
}

अचिन्हित पूर्णांक hb_ot_map_t::get_feature_tags (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक *tag_गिनती, hb_tag_t *tags) स्थिर
{
  अगर (tag_गिनती)
  {
    स्वतः sub_features = features.as_array ().sub_array (start_offset, tag_गिनती);
    अगर (tags)
    {
      क्रम (अचिन्हित पूर्णांक i = 0; i < sub_features.लंबाई; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  वापस features.लंबाई;
}

#पूर्ण_अगर
