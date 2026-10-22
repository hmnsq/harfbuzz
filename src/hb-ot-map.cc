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

#ifndef HB_NO_OT_SHAPE

#شامل "hb-ot-map.hh"
#شامل "hb-ot-shape.hh"
#شامل "hb-ot-layout.hh"


تهی hb_ot_map_t::collect_lookups (unsigned صحیح table_index, hb_set_t *lookups_out) ثابت
{
  برای (unsigned صحیح i = 0; i < lookups[table_index].length; i++)
    lookups_out->add (lookups[table_index][i].index);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ثابت hb_segment_properties_t &props_)
{
  hb_memset (این, 0, اندازه (*این));

  feature_infos.init ();
  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    stages[table_index].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned صحیح script_شمار = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned صحیح language_شمار = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_شمار,
				       script_tags,
				       &language_شمار,
				       language_tags);

  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
  {
    hb_tag_t table_tag = table_tags[table_index];
    found_script[table_index] = (منطقی) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_شمار,
									 script_tags,
									 &script_index[table_index],
									 &chosen_script[table_index]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_index[table_index],
					 language_شمار,
					 language_tags,
					 &language_index[table_index]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    stages[table_index].fini ();
}

تهی hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned صحیح value)
{
  اگر (unlikely (!tag)) بازگشت;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.length;
  info->max_مقدار = value;
  info->flags = flags;
  info->default_مقدار = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

منطقی hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
  {
    اگر (hb_ot_layout_language_find_feature (face,
					    table_tags[table_index],
					    script_index[table_index],
					    language_index[table_index],
					    tag,
					    nullptr))
      بازگشت درست;
  }
  بازگشت نادرست;
}

تهی
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned صحیح  table_index,
				  unsigned صحیح  feature_index,
				  ثابت hb_ot_shape_plan_کلید_t &key,
				  hb_mask_t     mask,
				  منطقی          auto_zwnj,
				  منطقی          auto_zwj,
				  منطقی          random,
				  منطقی          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned صحیح lookup_indices[32];
  unsigned صحیح offset, len;
  unsigned صحیح table_lookup_شمار;

  table_lookup_شمار = hb_ot_layout_table_get_lookup_شمار (face, table_tags[table_index]);

  hb_set_t varied_lookups;
  اگر (key.get_lookup_variations (face, table_index, feature_index, &varied_lookups))
  {
    اگر (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_index].ensure_error ();
      بازگشت;
    }
    برای (unsigned صحیح lookup_index : varied_lookups)
    {
      اگر (lookup_index >= table_lookup_شمار)
	ادامه;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_index].push ();
      lookup->mask = mask;
      lookup->index = lookup_index;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    بازگشت;
  }

  offset = 0;
  انجامبده {
    len = ARRAY_LENGTH (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_index],
					      feature_index,
					      key.variations_index[table_index],
						      offset, &len,
						      lookup_indices);

    برای (unsigned صحیح i = 0; i < len; i++)
    {
      اگر (lookup_indices[i] >= table_lookup_شمار)
	ادامه;
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
  } وقتی (len == ARRAY_LENGTH (lookup_indices));
}


تهی hb_ot_map_builder_t::add_pause (unsigned صحیح table_index, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_index].push ();
  s->index = current_stage[table_index];
  s->pause_func = pause_func;

  current_stage[table_index]++;
}

تهی
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ثابت hb_ot_shape_plan_کلید_t &key)
{
  unsigned صحیح global_bit_shift = 8 * اندازه (hb_mask_t) - 1;
  unsigned صحیح global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned صحیح required_feature_index[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned صحیح required_feature_stage[2] = {0, 0};

  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
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
  اگر (feature_infos.length)
  {
    اگر (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned صحیح j = 0;
    unsigned شمار = feature_infos.length;
    برای (unsigned صحیح i = 1; i < شمار; i++)
      اگر (f[i].tag != f[j].tag)
	f[++j] = f[i];
      وگرنه {
	اگر (f[i].flags & F_GLOBAL) {
	  f[j].flags |= F_GLOBAL;
	  f[j].max_مقدار = f[i].max_مقدار;
	  f[j].default_مقدار = f[i].default_مقدار;
	} وگرنه {
	  اگر (f[j].flags & F_GLOBAL)
	    f[j].flags ^= F_GLOBAL;
	  f[j].max_مقدار = hb_max (f[j].max_مقدار, f[i].max_مقدار);
	  /* Inherit default_value from j */
	}
	f[j].flags |= (f[i].flags & F_HAS_FALLBACK);
	f[j].stage[0] = hb_min (f[j].stage[0], f[i].stage[0]);
	f[j].stage[1] = hb_min (f[j].stage[1], f[i].stage[1]);
      }
    feature_infos.shrink (j + 1);
  }

  hb_map_t feature_indices[2];
  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_index],
				       script_index[table_index],
				       language_index[table_index],
				       &feature_indices[table_index]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned صحیح next_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned شمار = feature_infos.length;
  برای (unsigned صحیح i = 0; i < شمار; i++)
  {
    ثابت feature_info_t *info = &feature_infos[i];

    unsigned صحیح bits_needed;

    اگر ((info->flags & F_GLOBAL) && info->max_مقدار == 1)
      /* Uses the global bit */
      bits_needed = 0;
    وگرنه
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_مقدار));

    اگر (!info->max_مقدار || next_bit + bits_needed >= global_bit_shift)
      ادامه; /* Feature disabled, or not enough bits. */

    منطقی found = نادرست;
    unsigned صحیح feature_index[2];
    برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
    {
      اگر (required_feature_tag[table_index] == info->tag)
	required_feature_stage[table_index] = info->stage[table_index];

      hb_codepoint_t *index;
      اگر (feature_indices[table_index].has (info->tag, &index))
      {
        feature_index[table_index] = *index;
        found = درست;
      }
      وگرنه
        feature_index[table_index] = HB_OT_LAYOUT_NO_FEATURE_INDEX;
    }
    اگر (!found && (info->flags & F_GLOBAL_جستجو))
    {
      برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
      {
	found |= (منطقی) hb_ot_layout_table_find_feature (face,
							 table_tags[table_index],
							 info->tag,
							 &feature_index[table_index]);
      }
    }
    اگر (!found && !(info->flags & F_HAS_FALLBACK))
      ادامه;


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
    اگر ((info->flags & F_GLOBAL) && info->max_مقدار == 1) {
      /* Uses the global bit */
      map->shift = global_bit_shift;
      map->mask = global_bit_mask;
    } وگرنه {
      map->shift = next_bit;
      map->mask = (1u << (next_bit + bits_needed)) - (1u << next_bit);
      next_bit += bits_needed;
      m.global_mask |= (info->default_مقدار << map->shift) & map->mask;
    }
    map->_1_mask = (1u << map->shift) & map->mask;
    map->needs_fallback = !found;
  }
  //feature_infos.shrink (0); /* Done with these */
  اگر (is_simple)
    m.features.qsort ();

  add_gsub_pause (nullptr);
  add_gpos_pause (nullptr);

  برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_index];

    unsigned صحیح stage_index = 0;
    unsigned صحیح last_رقم_lookups = 0;
    برای (unsigned stage = 0; stage < current_stage[table_index]; stage++)
    {
      اگر (required_feature_index[table_index] != HB_OT_LAYOUT_NO_FEATURE_INDEX &&
	  required_feature_stage[table_index] == stage)
	add_lookups (m, table_index,
		     required_feature_index[table_index],
		     key,
		     global_bit_mask);

      برای (auto &feature : m.features)
      {
	اگر (feature.stage[table_index] == stage)
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
      اگر (last_رقم_lookups + 1 < lookups.length)
      {
	lookups.as_array ().sub_array (last_رقم_lookups, lookups.length - last_رقم_lookups).qsort ();

	unsigned صحیح j = last_رقم_lookups;
	برای (unsigned صحیح i = j + 1; i < lookups.length; i++)
	  اگر (lookups.arrayZ[i].index != lookups.arrayZ[j].index)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  وگرنه
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_رقم_lookups = lookups.length;

      اگر (stage_index < stages[table_index].length && stages[table_index][stage_index].index == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_index].push ();
	stage_map->last_lookup = last_رقم_lookups;
	stage_map->pause_func = stages[table_index][stage_index].pause_func;

	stage_index++;
      }
    }
  }
}

unsigned صحیح hb_ot_map_t::get_feature_tags (unsigned صحیح start_offset, unsigned صحیح *tag_شمار, hb_tag_t *tags) ثابت
{
  اگر (tag_شمار)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_شمار);
    اگر (tags)
    {
      برای (unsigned صحیح i = 0; i < sub_features.length; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  بازگشت features.length;
}

#endif
