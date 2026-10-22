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

#ifndef HB_NO_OT_SHAPE

#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout.hh"


فراغ hb_ot_map_t::collect_lookups (unsigned صحيح table_index, hb_set_t *lookups_out) ثابت
{
  لكل (unsigned صحيح i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (هذا, 0, حجم (*هذا));

  feature_infos.init ();
  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned صحيح script_count = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned صحيح language_count = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_count,
				       script_tags,
				       &language_count,
				       language_tags);

  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (منطقي) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_count,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_count,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

فراغ hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned صحيح value)
{
  ܐܢ (unlikely (!tag)) ܦܢܐ;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_value = value;
  info->flags = flags;
  info->default_value = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقي hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
  {
    ܐܢ (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      ܦܢܐ ܫܪܪܐ;
  }
  ܦܢܐ ܕܓܠܐ;
}

فراغ
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned صحيح  table_index,
				  unsigned صحيح  feature_index,
				  ثابت hb_ot_shape_plan_key_t &key,
				  hb_mask_t     mask,
				  منطقي          auto_zwnj,
				  منطقي          auto_zwj,
				  منطقي          random,
				  منطقي          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned صحيح lookup_indices[32];
  unsigned صحيح offset, len;
  unsigned صحيح table_lookup_count;

  table_lookup_count = hb_ot_layout_table_get_lookup_count (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  ܐܢ (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    ܐܢ (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      ܦܢܐ;
    }
    لكل (unsigned صحيح lookup_index : varied_lookups)
    {
      ܐܢ (lookup_index >= table_lookup_count)
	استمر;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    ܦܢܐ;
  }

  offset = 0;
  ܥܒܕ {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    لكل (unsigned صحيح i = 0; i < len; i++)
    {
      ܐܢ (lookup_indices[i] >= table_lookup_count)
	استمر;
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
  } ܟܡܐ (len == ARRAY_LENGTH (lookup_indices));
}


فراغ hb_ot_map_builder_t::add_pause (unsigned صحيح table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

فراغ
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_key_t &key)
{
  unsigned صحيح global_bit_shift = 8 * حجم (hb_mask_t) - 1;
  unsigned صحيح global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned صحيح required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned صحيح required_feature_stage[2] = {0, 0};

  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
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
  ܐܢ (feature_infos.length)
  {
    ܐܢ (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned صحيح j = 0;
    unsigned count = feature_infos.length;
    لكل (unsigned صحيح i = 1; i < count; i++)
      ܐܢ (f[i].tag != f[j].tag)
	f[++j] = f[i];
      ܐܚܪܢܐ {
	ܐܢ (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_value = f[i].max_value;
	  f[j].default_value = f[i].default_value;
	} ܐܚܪܢܐ {
	  ܐܢ (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_value = hb_max (f[j].max_value, f[i].max_value);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned صحيح next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned count = feature_infos.length;
  لكل (unsigned صحيح i = 0; i < count; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    unsigned صحيح bits_needed;

    ܐܢ ((info->flags & F_GLOBAL) && info->max_value == 1)
      /* Uses the global bit */
      bits_needed = 0;
    ܐܚܪܢܐ
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_value));

    ܐܢ (!info->max_value || next_bit + bits_needed >= global_bit_shift)
      استمر; /* Feature disabled, or not enough bits. */

    منطقي found = ܕܓܠܐ;
    unsigned صحيح feature_index[2];
    لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
    {
      ܐܢ (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      ܐܢ (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = ܫܪܪܐ;
      }
      ܐܚܪܢܐ
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    ܐܢ (!found && (info->flags & F_GLOBAL_SEARCH))
    {
      لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
      {
	found |= (منطقي) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    ܐܢ (!found && !(info->flags & F_HAS_FALLBACK))
      استمر;


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
    ܐܢ ((info->flags & F_GLOBAL) && info->max_value == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } ܐܚܪܢܐ {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_value << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  ܐܢ (is_simple)
    m.features.qsort ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_index];

    unsigned صحيح stage_index = 0;
    unsigned صحيح last_num_lookups = 0;
    لكل (unsigned stage = 0; stage < current_stage[table_index]; stage++)
    {
      ܐܢ (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      لكل (auto &feature : m.features)
      {
	ܐܢ (feature.stage[table_index] == stage)
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
      ܐܢ (last_num_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_num_lookups, lookups.length - last_num_lookups).qsort ();

	unsigned صحيح j = last_num_lookups;
	لكل (unsigned صحيح i = j + 1; i < lookups.length; i++)
	  ܐܢ (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  ܐܚܪܢܐ
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_num_lookups = lookups.length;

      ܐܢ (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_num_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

unsigned صحيح hb_ot_map_t::get_feature_tags (unsigned صحيح start_offset, unsigned صحيح *tag_count, hb_tag_t *tags) ثابت
{
  ܐܢ (tag_count)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_count);
    ܐܢ (tags)
    {
      لكل (unsigned صحيح i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  ܦܢܐ features.length;
}

#endif
