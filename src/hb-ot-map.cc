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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#അന്തര്ഭാവ "hb-ot-map.hh"
#അന്തര്ഭാവ "hb-ot-shape.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"


ശൂന്യം hb_ot_map_t::collect_lookups (unsigned പൂർണ്ണസംഖ്യ table_index, hb_set_t *lookups_out) സ്ഥിരം
{
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  സ്ഥിരം hb_segment_properties_t &props_)
{
  hb_memset (ഇത്, 0, വലുപ്പം (*ഇത്));

  feature_infos.init ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned പൂർണ്ണസംഖ്യ script_എണ്ണം = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned പൂർണ്ണസംഖ്യ language_എണ്ണം = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_എണ്ണം,
				       script_tags,
				       &language_എണ്ണം,
				       language_tags);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (തർക്കം) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_എണ്ണം,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_എണ്ണം,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

ശൂന്യം hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned പൂർണ്ണസംഖ്യ value)
{
  എങ്കിൽ (unlikely (!tag)) തിരികെ;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_മൂല്യം = value;
  info->flags = flags;
  info->default_മൂല്യം = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

തർക്കം hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
  {
    എങ്കിൽ (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      തിരികെ സത്യം;
  }
  തിരികെ അസത്യം;
}

ശൂന്യം
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned പൂർണ്ണസംഖ്യ  table_index,
				  unsigned പൂർണ്ണസംഖ്യ  feature_index,
				  സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key,
				  hb_mask_t     mask,
				  തർക്കം          auto_zwnj,
				  തർക്കം          auto_zwj,
				  തർക്കം          random,
				  തർക്കം          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned പൂർണ്ണസംഖ്യ lookup_indices[32];
  unsigned പൂർണ്ണസംഖ്യ offset, len;
  unsigned പൂർണ്ണസംഖ്യ table_lookup_എണ്ണം;

  table_lookup_എണ്ണം = hb_ot_layout_table_get_lookup_എണ്ണം (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  എങ്കിൽ (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    എങ്കിൽ (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      തിരികെ;
    }
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ lookup_index : varied_lookups)
    {
      എങ്കിൽ (lookup_index >= table_lookup_എണ്ണം)
	തുടരുക;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    തിരികെ;
  }

  offset = 0;
  ചെയ്യുക {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
    {
      എങ്കിൽ (lookup_indices[i] >= table_lookup_എണ്ണം)
	തുടരുക;
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
  } ആകുമ്പോൾ (len == ARRAY_LENGTH (lookup_indices));
}


ശൂന്യം hb_ot_map_builder_t::add_pause (unsigned പൂർണ്ണസംഖ്യ table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

ശൂന്യം
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key)
{
  unsigned പൂർണ്ണസംഖ്യ global_bit_shift = 8 * വലുപ്പം (hb_mask_t) - 1;
  unsigned പൂർണ്ണസംഖ്യ global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned പൂർണ്ണസംഖ്യ required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned പൂർണ്ണസംഖ്യ required_feature_stage[2] = {0, 0};

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
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
  എങ്കിൽ (feature_infos.length)
  {
    എങ്കിൽ (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned പൂർണ്ണസംഖ്യ j = 0;
    unsigned എണ്ണം = feature_infos.length;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
      എങ്കിൽ (f[i].tag != f[j].tag)
	f[++j] = f[i];
      അല്ലെങ്കിൽ {
	എങ്കിൽ (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_മൂല്യം = f[i].max_മൂല്യം;
	  f[j].default_മൂല്യം = f[i].default_മൂല്യം;
	} അല്ലെങ്കിൽ {
	  എങ്കിൽ (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_മൂല്യം = hb_max (f[j].max_മൂല്യം, f[i].max_മൂല്യം);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned പൂർണ്ണസംഖ്യ next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned എണ്ണം = feature_infos.length;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    സ്ഥിരം feature_info_t *info = &feature_infos[i];

    unsigned പൂർണ്ണസംഖ്യ bits_needed;

    എങ്കിൽ ((info->flags & F_GLOBAL) && info->max_മൂല്യം == 1)
      /* Uses the global bit */
      bits_needed = 0;
    അല്ലെങ്കിൽ
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_മൂല്യം));

    എങ്കിൽ (!info->max_മൂല്യം || next_bit + bits_needed >= global_bit_shift)
      തുടരുക; /* Feature disabled, or not enough bits. */

    തർക്കം found = അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ feature_index[2];
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
    {
      എങ്കിൽ (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      എങ്കിൽ (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = സത്യം;
      }
      അല്ലെങ്കിൽ
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    എങ്കിൽ (!found && (info->flags & F_GLOBAL_തിരയൽ))
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
      {
	found |= (തർക്കം) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    എങ്കിൽ (!found && !(info->flags & F_HAS_FALLBACK))
      തുടരുക;


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
    എങ്കിൽ ((info->flags & F_GLOBAL) && info->max_മൂല്യം == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } അല്ലെങ്കിൽ {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_മൂല്യം << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  എങ്കിൽ (is_simple)
    m.features.qsort ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_index];

    unsigned പൂർണ്ണസംഖ്യ stage_index = 0;
    unsigned പൂർണ്ണസംഖ്യ last_അക്കം_lookups = 0;
    വേണ്ടി (unsigned stage = 0; stage < current_stage[table_index]; stage++)
    {
      എങ്കിൽ (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      വേണ്ടി (auto &feature : m.features)
      {
	എങ്കിൽ (feature.stage[table_index] == stage)
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
      എങ്കിൽ (last_അക്കം_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_അക്കം_lookups, lookups.length - last_അക്കം_lookups).qsort ();

	unsigned പൂർണ്ണസംഖ്യ j = last_അക്കം_lookups;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = j + 1; i < lookups.length; i++)
	  എങ്കിൽ (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  അല്ലെങ്കിൽ
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_അക്കം_lookups = lookups.length;

      എങ്കിൽ (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_അക്കം_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

unsigned പൂർണ്ണസംഖ്യ hb_ot_map_t::get_feature_tags (unsigned പൂർണ്ണസംഖ്യ start_offset, unsigned പൂർണ്ണസംഖ്യ *tag_എണ്ണം, hb_tag_t *tags) സ്ഥിരം
{
  എങ്കിൽ (tag_എണ്ണം)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_എണ്ണം);
    എങ്കിൽ (tags)
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  തിരികെ features.length;
}

#endif
