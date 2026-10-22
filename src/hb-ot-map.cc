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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#அந்தர்பாவ "hb-ot-map.hh"
#அந்தர்பாவ "hb-ot-shape.hh"
#அந்தர்பாவ "hb-ot-layout.hh"


வெறுமை hb_ot_map_t::collect_lookups (unsigned முழு table_index, hb_set_t *lookups_out) மாறிலி
{
  க்கு (unsigned முழு i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  மாறிலி hb_segment_properties_t &props_)
{
  hb_memset (இது, 0, அளவு (*இது));

  feature_infos.init ();
  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned முழு script_மொத்தம் = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned முழு language_மொத்தம் = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_மொத்தம்,
				       script_tags,
				       &language_மொத்தம்,
				       language_tags);

  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (தருக்க) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_மொத்தம்,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_மொத்தம்,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

வெறுமை hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned முழு value)
{
  எனில் (unlikely (!tag)) திரும்பு;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_மதிப்பு = value;
  info->flags = flags;
  info->default_மதிப்பு = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

தருக்க hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
  {
    எனில் (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      திரும்பு உண்மை;
  }
  திரும்பு பொய்;
}

வெறுமை
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned முழு  table_index,
				  unsigned முழு  feature_index,
				  மாறிலி hb_ot_shape_plan_சாவி_t &key,
				  hb_mask_t     mask,
				  தருக்க          auto_zwnj,
				  தருக்க          auto_zwj,
				  தருக்க          random,
				  தருக்க          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned முழு lookup_indices[32];
  unsigned முழு offset, len;
  unsigned முழு table_lookup_மொத்தம்;

  table_lookup_மொத்தம் = hb_ot_layout_table_get_lookup_மொத்தம் (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  எனில் (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    எனில் (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      திரும்பு;
    }
    க்கு (unsigned முழு lookup_index : varied_lookups)
    {
      எனில் (lookup_index >= table_lookup_மொத்தம்)
	தொடர்;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    திரும்பு;
  }

  offset = 0;
  செய் {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    க்கு (unsigned முழு i = 0; i < len; i++)
    {
      எனில் (lookup_indices[i] >= table_lookup_மொத்தம்)
	தொடர்;
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
  } போது (len == ARRAY_LENGTH (lookup_indices));
}


வெறுமை hb_ot_map_builder_t::add_pause (unsigned முழு table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

வெறுமை
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      மாறிலி hb_ot_shape_plan_சாவி_t &key)
{
  unsigned முழு global_bit_shift = 8 * அளவு (hb_mask_t) - 1;
  unsigned முழு global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned முழு required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned முழு required_feature_stage[2] = {0, 0};

  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
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
  எனில் (feature_infos.length)
  {
    எனில் (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned முழு j = 0;
    unsigned மொத்தம் = feature_infos.length;
    க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
      எனில் (f[i].tag != f[j].tag)
	f[++j] = f[i];
      இல்லையெனில் {
	எனில் (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_மதிப்பு = f[i].max_மதிப்பு;
	  f[j].default_மதிப்பு = f[i].default_மதிப்பு;
	} இல்லையெனில் {
	  எனில் (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_மதிப்பு = hb_max (f[j].max_மதிப்பு, f[i].max_மதிப்பு);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned முழு next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned மொத்தம் = feature_infos.length;
  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
  {
    மாறிலி feature_info_t *info = &feature_infos[i];

    unsigned முழு bits_needed;

    எனில் ((info->flags & F_GLOBAL) && info->max_மதிப்பு == 1)
      /* Uses the global bit */
      bits_needed = 0;
    இல்லையெனில்
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_மதிப்பு));

    எனில் (!info->max_மதிப்பு || next_bit + bits_needed >= global_bit_shift)
      தொடர்; /* Feature disabled, or not enough bits. */

    தருக்க found = பொய்;
    unsigned முழு feature_index[2];
    க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
    {
      எனில் (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      எனில் (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = உண்மை;
      }
      இல்லையெனில்
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    எனில் (!found && (info->flags & F_GLOBAL_தேடல்))
    {
      க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
      {
	found |= (தருக்க) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    எனில் (!found && !(info->flags & F_HAS_FALLBACK))
      தொடர்;


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
    எனில் ((info->flags & F_GLOBAL) && info->max_மதிப்பு == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } இல்லையெனில் {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_மதிப்பு << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  எனில் (is_simple)
    m.features.qsort ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_index];

    unsigned முழு stage_index = 0;
    unsigned முழு last_இலக்கம்_lookups = 0;
    க்கு (unsigned stage = 0; stage < current_stage[table_index]; stage++)
    {
      எனில் (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      க்கு (auto &feature : m.features)
      {
	எனில் (feature.stage[table_index] == stage)
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
      எனில் (last_இலக்கம்_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_இலக்கம்_lookups, lookups.length - last_இலக்கம்_lookups).qsort ();

	unsigned முழு j = last_இலக்கம்_lookups;
	க்கு (unsigned முழு i = j + 1; i < lookups.length; i++)
	  எனில் (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  இல்லையெனில்
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_இலக்கம்_lookups = lookups.length;

      எனில் (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_இலக்கம்_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

unsigned முழு hb_ot_map_t::get_feature_tags (unsigned முழு start_offset, unsigned முழு *tag_மொத்தம், hb_tag_t *tags) மாறிலி
{
  எனில் (tag_மொத்தம்)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_மொத்தம்);
    எனில் (tags)
    {
      க்கு (unsigned முழு i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  திரும்பு features.length;
}

#endif
