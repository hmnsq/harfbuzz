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

#অন্তর্ভাৱ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাৱ "hb-ot-map.hh"
#অন্তর্ভাৱ "hb-ot-shape.hh"
#অন্তর্ভাৱ "hb-ot-layout.hh"


শূন্য hb_ot_map_t::collect_lookups (unsigned পূর্ণাঙ্ক table_index, hb_set_t *lookups_out) ধ্রুৱ
{
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ধ্রুৱ hb_segment_properties_t &props_)
{
  hb_memset (এতত্, 0, পরিমাণ (*এতত্));

  feature_infos.init ();
  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned পূর্ণাঙ্ক script_গুনতি = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned পূর্ণাঙ্ক language_গুনতি = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_গুনতি,
				       script_tags,
				       &language_গুনতি,
				       language_tags);

  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (তার্কিক) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_গুনতি,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_গুনতি,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

শূন্য hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned পূর্ণাঙ্ক value)
{
  যদি (unlikely (!tag)) ঘূৰাই;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_মান = value;
  info->flags = flags;
  info->default_মান = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

তার্কিক hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
  {
    যদি (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      ঘূৰাই সঁচা;
  }
  ঘূৰাই মিছা;
}

শূন্য
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned পূর্ণাঙ্ক  table_index,
				  unsigned পূর্ণাঙ্ক  feature_index,
				  ধ্রুৱ hb_ot_shape_plan_চাবি_t &key,
				  hb_mask_t     mask,
				  তার্কিক          auto_zwnj,
				  তার্কিক          auto_zwj,
				  তার্কিক          random,
				  তার্কিক          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned পূর্ণাঙ্ক lookup_indices[32];
  unsigned পূর্ণাঙ্ক offset, len;
  unsigned পূর্ণাঙ্ক table_lookup_গুনতি;

  table_lookup_গুনতি = hb_ot_layout_table_get_lookup_গুনতি (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  যদি (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    যদি (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      ঘূৰাই;
    }
    বাবে (unsigned পূর্ণাঙ্ক lookup_index : varied_lookups)
    {
      যদি (lookup_index >= table_lookup_গুনতি)
	অনুৱর্ত;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    ঘূৰাই;
  }

  offset = 0;
  কৰা {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < len; i++)
    {
      যদি (lookup_indices[i] >= table_lookup_গুনতি)
	অনুৱর্ত;
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
  } যেতিয়ালৈ (len == ARRAY_LENGTH (lookup_indices));
}


শূন্য hb_ot_map_builder_t::add_pause (unsigned পূর্ণাঙ্ক table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

শূন্য
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ধ্রুৱ hb_ot_shape_plan_চাবি_t &key)
{
  unsigned পূর্ণাঙ্ক global_bit_shift = 8 * পরিমাণ (hb_mask_t) - 1;
  unsigned পূর্ণাঙ্ক global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned পূর্ণাঙ্ক required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned পূর্ণাঙ্ক required_feature_stage[2] = {0, 0};

  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
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
  যদি (feature_infos.length)
  {
    যদি (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned পূর্ণাঙ্ক j = 0;
    unsigned গুনতি = feature_infos.length;
    বাবে (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
      যদি (f[i].tag != f[j].tag)
	f[++j] = f[i];
      অন্যথা {
	যদি (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_মান = f[i].max_মান;
	  f[j].default_মান = f[i].default_মান;
	} অন্যথা {
	  যদি (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_মান = hb_max (f[j].max_মান, f[i].max_মান);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned পূর্ণাঙ্ক next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned গুনতি = feature_infos.length;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
  {
    ধ্রুৱ feature_info_t *info = &feature_infos[i];

    unsigned পূর্ণাঙ্ক bits_needed;

    যদি ((info->flags & F_GLOBAL) && info->max_মান == 1)
      /* Uses the global bit */
      bits_needed = 0;
    অন্যথা
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_মান));

    যদি (!info->max_মান || next_bit + bits_needed >= global_bit_shift)
      অনুৱর্ত; /* Feature disabled, or not enough bits. */

    তার্কিক found = মিছা;
    unsigned পূর্ণাঙ্ক feature_index[2];
    বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
    {
      যদি (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      যদি (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = সঁচা;
      }
      অন্যথা
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    যদি (!found && (info->flags & F_GLOBAL_অনুসন্ধান))
    {
      বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
      {
	found |= (তার্কিক) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    যদি (!found && !(info->flags & F_HAS_FALLBACK))
      অনুৱর্ত;


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
    যদি ((info->flags & F_GLOBAL) && info->max_মান == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } অন্যথা {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_মান << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  যদি (is_simple)
    m.features.qsort ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  বাবে (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_index];

    unsigned পূর্ণাঙ্ক stage_index = 0;
    unsigned পূর্ণাঙ্ক last_অঙ্ক_lookups = 0;
    বাবে (unsigned stage = 0; stage < current_stage[table_index]; stage++)
    {
      যদি (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      বাবে (auto &feature : m.features)
      {
	যদি (feature.stage[table_index] == stage)
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
      যদি (last_অঙ্ক_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_অঙ্ক_lookups, lookups.length - last_অঙ্ক_lookups).qsort ();

	unsigned পূর্ণাঙ্ক j = last_অঙ্ক_lookups;
	বাবে (unsigned পূর্ণাঙ্ক i = j + 1; i < lookups.length; i++)
	  যদি (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  অন্যথা
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_অঙ্ক_lookups = lookups.length;

      যদি (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_অঙ্ক_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

unsigned পূর্ণাঙ্ক hb_ot_map_t::get_feature_tags (unsigned পূর্ণাঙ্ক start_offset, unsigned পূর্ণাঙ্ক *tag_গুনতি, hb_tag_t *tags) ধ্রুৱ
{
  যদি (tag_গুনতি)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_গুনতি);
    যদি (tags)
    {
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  ঘূৰাই features.length;
}

#endif
