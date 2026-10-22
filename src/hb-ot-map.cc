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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_SHAPE

#अन्तर्भाव "hb-ot-map.hh"
#अन्तर्भाव "hb-ot-shape.hh"
#अन्तर्भाव "hb-ot-layout.hh"


शून्य hb_ot_map_t::collect_lookups (अचिन्हित पूर्णांक table_index, hb_set_t *lookups_out) स्थिरांक
{
  साठी (अचिन्हित पूर्णांक i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  स्थिरांक hb_segment_properties_t &props_)
{
  hb_memset (हे, 0, आकार (*हे));

  feature_infos.init ();
  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  अचिन्हित पूर्णांक script_मोजणी = HB_OT_MAX_TAGS_PER_SCRIPT;
  अचिन्हित पूर्णांक language_मोजणी = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_मोजणी,
				       script_tags,
				       &language_मोजणी,
				       language_tags);

  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (तार्किक) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_मोजणी,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_मोजणी,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

शून्य hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       अचिन्हित पूर्णांक value)
{
  जर (unlikely (!tag)) परत;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_मूल्य = value;
  info->flags = flags;
  info->default_मूल्य = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

तार्किक hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
  {
    जर (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      परत सत्य;
  }
  परत असत्य;
}

शून्य
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  अचिन्हित पूर्णांक  table_index,
				  अचिन्हित पूर्णांक  feature_index,
				  स्थिरांक hb_ot_shape_plan_किल्ली_t &key,
				  hb_mask_t     mask,
				  तार्किक          auto_zwnj,
				  तार्किक          auto_zwj,
				  तार्किक          random,
				  तार्किक          per_syllable,
				  hb_tag_t      feature_tag)
{
  अचिन्हित पूर्णांक lookup_indices[32];
  अचिन्हित पूर्णांक offset, len;
  अचिन्हित पूर्णांक table_lookup_मोजणी;

  table_lookup_मोजणी = hb_ot_layout_table_get_lookup_मोजणी (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  जर (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    जर (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      परत;
    }
    साठी (अचिन्हित पूर्णांक lookup_index : varied_lookups)
    {
      जर (lookup_index >= table_lookup_मोजणी)
	सुरूठेवा;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    परत;
  }

  offset = 0;
  करा {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    साठी (अचिन्हित पूर्णांक i = 0; i < len; i++)
    {
      जर (lookup_indices[i] >= table_lookup_मोजणी)
	सुरूठेवा;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }

    offset += len;
  } पर्यंत (len == ARRAY_LENGTH (lookup_indices));
}


शून्य hb_ot_map_builder_t::add_pause (अचिन्हित पूर्णांक table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

शून्य
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      स्थिरांक hb_ot_shape_plan_किल्ली_t &key)
{
  अचिन्हित पूर्णांक global_bit_shift = 8 * आकार (hb_mask_t) - 1;
  अचिन्हित पूर्णांक global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  अचिन्हित पूर्णांक required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  अचिन्हित पूर्णांक required_feature_stage[2] = {0, 0};

  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
  {
    m.chosen_script[table_index] = chosen_script[table_index];
    m.found_script[table_index] = found_script[table_index];

    hb_ot_layout_language_get_required_feature (face,
						table_tags[table_index],
						script_index[table_index],
						language_index[table_index],
						&required_feature_index[table_index],
						&required_feature_tag[table_index]);
  }

  /* Sort features and merge duplicates */
  जर (feature_infos.length)
  {
    जर (!is_simple)
      feature_infos.क्विक ();
    स्वतः *f = feature_infos.arrayZ;
    अचिन्हित पूर्णांक j = 0;
    अचिन्हित मोजणी = feature_infos.length;
    साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
      जर (f[i].tag != f[j].tag)
	f[++j] = f[i];
      अन्यथा {
	जर (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_मूल्य = f[i].max_मूल्य;
	  f[j].default_मूल्य = f[i].default_मूल्य;
	} अन्यथा {
	  जर (f[j].flags & F_GLOBAL)
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
  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  अचिन्हित पूर्णांक next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  अचिन्हित मोजणी = feature_infos.length;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
  {
    स्थिरांक feature_info_t *info = &feature_infos[i];

    अचिन्हित पूर्णांक bits_needed;

    जर ((info->flags & F_GLOBAL) && info->max_मूल्य == 1)
      /* Uses the global bit */
      bits_needed = 0;
    अन्यथा
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_मूल्य));

    जर (!info->max_मूल्य || next_bit + bits_needed >= global_bit_shift)
      सुरूठेवा; /* Feature disabled, or not enough bits. */

    तार्किक found = असत्य;
    अचिन्हित पूर्णांक feature_index[2];
    साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
    {
      जर (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      जर (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = सत्य;
      }
      अन्यथा
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    जर (!found && (info->flags & F_GLOBAL_शोध))
    {
      साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
      {
	found |= (तार्किक) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    जर (!found && !(info->flags & F_HAS_FALLBACK))
      सुरूठेवा;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->tag = info->tag;
    map->index[0] = feature_index[0];
    map->index[1] = feature_index[1];
    map->stage[0] = info->stage[0];
    map->stage[1] = info->stage[1];
    map->auto_zwnj = !(info->flags & F_MANUAL_ZWNJ);
    map->auto_zwj = !(info->flags & F_MANUAL_ZWJ);
    map->random = !!(info->flags & F_RANDOM);
    map->per_syllable = !!(info->flags & F_PER_SYLLABLE);
    जर ((info->flags & F_GLOBAL) && info->max_मूल्य == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } अन्यथा {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_मूल्य << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  जर (is_simple)
    m.features.क्विक ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  साठी (अचिन्हित पूर्णांक table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    स्वतः &lookups = m.lookups[table_index];

    अचिन्हित पूर्णांक stage_index = 0;
    अचिन्हित पूर्णांक last_अंक_lookups = 0;
    साठी (अचिन्हित stage = 0; stage < current_stage[table_index]; stage++)
    {
      जर (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      साठी (स्वतः &feature : m.features)
      {
	जर (feature.stage[table_index] == stage)
	  add_lookups (m, table_index,
		       feature.index[table_index],
		       key,
		       feature.mask,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.per_syllable,
		       feature.tag);
      }

      /* Sort lookups and merge duplicates */
      जर (last_अंक_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_अंक_lookups, lookups.length - last_अंक_lookups).क्विक ();

	अचिन्हित पूर्णांक j = last_अंक_lookups;
	साठी (अचिन्हित पूर्णांक i = j + 1; i < lookups.length; i++)
	  जर (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  अन्यथा
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_अंक_lookups = lookups.length;

      जर (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_अंक_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

अचिन्हित पूर्णांक hb_ot_map_t::get_feature_tags (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक *tag_मोजणी, hb_tag_t *tags) स्थिरांक
{
  जर (tag_मोजणी)
  {
    स्वतः sub_features = features.as_array ().sub_array (start_offset, tag_मोजणी);
    जर (tags)
    {
      साठी (अचिन्हित पूर्णांक i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  परत features.length;
}

#endif
