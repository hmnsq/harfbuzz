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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-map.hh"
#অন্তর্ভাব "hb-ot-shape.hh"
#অন্তর্ভাব "hb-ot-layout.hh"


শূন্য hb_ot_map_t::collect_lookups (unsigned পূর্ণসংখ্যা table_সূচক, hb_set_t *lookups_out) ধ্রুবক
{
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < lookups[table_সূচক].দৈর্ঘ্য; i++)
    lookups_out->add (lookups[table_সূচক][i].সূচক_2);
}


hb_ot_map_builder_t::hb_ot_map_builder_t (hb_face_t *face_,
					  ধ্রুবক hb_segment_properties_t &props_)
{
  hb_memset (এই, 0, আকার (*এই));

  feature_infos.init ();
  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    stages[table_সূচক].init ();

  face = face_;
  props = props_;

  /* Fetch script/language indices for GSUB/GPOS.  We need these later to skip
   * features not available in either table and not waste precious bits for them. */

  unsigned পূর্ণসংখ্যা script_গুনতি = HB_OT_MAX_TAGS_PER_SCRIPT;
  unsigned পূর্ণসংখ্যা language_গুনতি = HB_OT_MAX_TAGS_PER_LANGUAGE;
  hb_tag_t script_tags[HB_OT_MAX_TAGS_PER_SCRIPT];
  hb_tag_t language_tags[HB_OT_MAX_TAGS_PER_LANGUAGE];

  hb_ot_tags_from_script_and_language (props.script,
				       props.language,
				       &script_গুনতি,
				       script_tags,
				       &language_গুনতি,
				       language_tags);

  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
  {
    hb_tag_t table_tag = table_tags[table_সূচক];
    found_script[table_সূচক] = (যৌক্তিক) hb_ot_layout_table_select_script (face,
									 table_tag,
									 script_গুনতি,
									 script_tags,
									 &script_সূচক[table_সূচক],
									 &chosen_script[table_সূচক]);
    hb_ot_layout_script_select_language (face,
					 table_tag,
					 script_সূচক[table_সূচক],
					 language_গুনতি,
					 language_tags,
					 &language_সূচক[table_সূচক]);
  }
}

hb_ot_map_builder_t::~hb_ot_map_builder_t ()
{
  feature_infos.fini ();
  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    stages[table_সূচক].fini ();
}

শূন্য hb_ot_map_builder_t::add_feature (hb_tag_t tag,
				       hb_ot_map_feature_flags_t flags,
				       unsigned পূর্ণসংখ্যা value)
{
  যদি (unlikely (!tag)) ফেরত;
  feature_info_t *info = feature_infos.push();
  info->tag = tag;
  info->seq = feature_infos.দৈর্ঘ্য;
  info->max_মান = value;
  info->flags = flags;
  info->default_মান = (flags & F_GLOBAL) ? value : 0;
  info->stage[0] = current_stage[0];
  info->stage[1] = current_stage[1];
}

যৌক্তিক hb_ot_map_builder_t::has_feature (hb_tag_t tag)
{
  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
  {
    যদি (hb_ot_layout_language_find_feature (face,
					    table_tags[table_সূচক],
					    script_সূচক[table_সূচক],
					    language_সূচক[table_সূচক],
					    tag,
					    nullptr))
      ফেরত সত্য;
  }
  ফেরত মিথ্যা;
}

শূন্য
hb_ot_map_builder_t::add_lookups (hb_ot_map_t  &m,
				  unsigned পূর্ণসংখ্যা  table_সূচক,
				  unsigned পূর্ণসংখ্যা  feature_সূচক,
				  ধ্রুবক hb_ot_shape_plan_চাবি_t &key,
				  hb_mask_t     mask,
				  যৌক্তিক          auto_zwnj,
				  যৌক্তিক          auto_zwj,
				  যৌক্তিক          random,
				  যৌক্তিক          per_syllable,
				  hb_tag_t      feature_tag)
{
  unsigned পূর্ণসংখ্যা lookup_indices[32];
  unsigned পূর্ণসংখ্যা offset, len;
  unsigned পূর্ণসংখ্যা table_lookup_গুনতি;

  table_lookup_গুনতি = hb_ot_layout_table_get_lookup_গুনতি (face, table_tags[table_সূচক]);

  hb_set_t varied_lookups;
  যদি (key.get_lookup_variations (face, table_সূচক, feature_সূচক, &varied_lookups))
  {
    যদি (unlikely (varied_lookups.in_error ()))
    {
      m.lookups[table_সূচক].ensure_error ();
      ফেরত;
    }
    জন্য (unsigned পূর্ণসংখ্যা lookup_সূচক : varied_lookups)
    {
      যদি (lookup_সূচক >= table_lookup_গুনতি)
	চালু;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_সূচক].push ();
      lookup->mask = mask;
      lookup->সূচক_2 = lookup_সূচক;
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }
    ফেরত;
  }

  offset = 0;
  করো {
    len = ARRAY_দৈর্ঘ্য (lookup_indices);
    hb_ot_layout_feature_with_variations_get_lookups (face,
					      table_tags[table_সূচক],
					      feature_সূচক,
					      key.variations_সূচক[table_সূচক],
						      offset, &len,
						      lookup_indices);

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < len; i++)
    {
      যদি (lookup_indices[i] >= table_lookup_গুনতি)
	চালু;
      hb_ot_map_t::lookup_map_t *lookup = m.lookups[table_সূচক].push ();
      lookup->mask = mask;
      lookup->সূচক_2 = lookup_indices[i];
      lookup->auto_zwnj = auto_zwnj;
      lookup->auto_zwj = auto_zwj;
      lookup->random = random;
      lookup->per_syllable = per_syllable;
      lookup->feature_tag = feature_tag;
    }

    offset += len;
  } যখন (len == ARRAY_দৈর্ঘ্য (lookup_indices));
}


শূন্য hb_ot_map_builder_t::add_pause (unsigned পূর্ণসংখ্যা table_সূচক, hb_ot_map_t::pause_func_t pause_func)
{
  stage_info_t *s = stages[table_সূচক].push ();
  s->সূচক_2 = current_stage[table_সূচক];
  s->pause_func = pause_func;

  current_stage[table_সূচক]++;
}

শূন্য
hb_ot_map_builder_t::compile (hb_ot_map_t                  &m,
			      ধ্রুবক hb_ot_shape_plan_চাবি_t &key)
{
  unsigned পূর্ণসংখ্যা global_bit_shift = 8 * আকার (hb_mask_t) - 1;
  unsigned পূর্ণসংখ্যা global_bit_mask = 1u << global_bit_shift;

  m.global_mask = global_bit_mask;

  unsigned পূর্ণসংখ্যা required_feature_সূচক[2];
  hb_tag_t required_feature_tag[2];
  /* We default to applying required feature in stage 0.  If the required
   * feature has a tag that is known to the shaper, we apply required feature
   * in the stage for that tag.
   */
  unsigned পূর্ণসংখ্যা required_feature_stage[2] = {0, 0};

  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
  {
    m.chosen_script[table_সূচক] = chosen_script[table_সূচক];
    m.found_script[table_সূচক] = found_script[table_সূচক];

    hb_ot_layout_language_get_required_feature (face,
						table_tags[table_সূচক],
						script_সূচক[table_সূচক],
						language_সূচক[table_সূচক],
						&required_feature_সূচক[table_সূচক],
						&required_feature_tag[table_সূচক]);
  }

  /* Sort features and merge duplicates */
  যদি (feature_infos.দৈর্ঘ্য)
  {
    যদি (!is_simple)
      feature_infos.qsort ();
    auto *f = feature_infos.arrayZ;
    unsigned পূর্ণসংখ্যা j = 0;
    unsigned গুনতি = feature_infos.দৈর্ঘ্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
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
  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    hb_ot_layout_collect_features_map (face,
				       table_tags[table_সূচক],
				       script_সূচক[table_সূচক],
				       language_সূচক[table_সূচক],
				       &feature_indices[table_সূচক]);

  /* Allocate bits now */
  static_assert ((!(HB_GLYPH_FLAG_DEFINED & (HB_GLYPH_FLAG_DEFINED + 1))), "");
  unsigned পূর্ণসংখ্যা পরবর্তী_bit = hb_popcount (HB_GLYPH_FLAG_DEFINED) + 1;

  unsigned গুনতি = feature_infos.দৈর্ঘ্য;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    ধ্রুবক feature_info_t *info = &feature_infos[i];

    unsigned পূর্ণসংখ্যা bits_needed;

    যদি ((info->flags & F_GLOBAL) && info->max_মান == 1)
      /* Uses the global bit */
      bits_needed = 0;
    অন্যথা
      /* Limit bits per feature. */
      bits_needed = hb_min (HB_OT_MAP_MAX_BITS, hb_bit_storage (info->max_মান));

    যদি (!info->max_মান || পরবর্তী_bit + bits_needed >= global_bit_shift)
      চালু; /* Feature disabled, or not enough bits. */

    যৌক্তিক found = মিথ্যা;
    unsigned পূর্ণসংখ্যা feature_সূচক[2];
    জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
    {
      যদি (required_feature_tag[table_সূচক] == info->tag)
	required_feature_stage[table_সূচক] = info->stage[table_সূচক];

      hb_codepoint_t *সূচক_2;
      যদি (feature_indices[table_সূচক].has (info->tag, &সূচক_2))
      {
        feature_সূচক[table_সূচক] = *সূচক_2;
        found = সত্য;
      }
      অন্যথা
        feature_সূচক[table_সূচক] = HB_OT_LAYOUT_NO_FEATURE_সূচক;
    }
    যদি (!found && (info->flags & F_GLOBAL_অনুসন্ধান))
    {
      জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
      {
	found |= (যৌক্তিক) hb_ot_layout_table_find_feature (face,
							 table_tags[table_সূচক],
							 info->tag,
							 &feature_সূচক[table_সূচক]);
      }
    }
    যদি (!found && !(info->flags & F_HAS_FALLBACK))
      চালু;


    hb_ot_map_t::feature_map_t *map = m.features.push ();

    map->tag = info->tag;
    map->সূচক_2[0] = feature_সূচক[0];
    map->সূচক_2[1] = feature_সূচক[1];
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
      map->shift = পরবর্তী_bit;
      map->mask = (1u << (পরবর্তী_bit + bits_needed)) - (1u << পরবর্তী_bit);
      পরবর্তী_bit += bits_needed;
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

  জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
  {
    /* Collect lookup indices for features */
    auto &lookups = m.lookups[table_সূচক];

    unsigned পূর্ণসংখ্যা stage_সূচক = 0;
    unsigned পূর্ণসংখ্যা last_অঙ্ক_lookups = 0;
    জন্য (unsigned stage = 0; stage < current_stage[table_সূচক]; stage++)
    {
      যদি (required_feature_সূচক[table_সূচক] != HB_OT_LAYOUT_NO_FEATURE_সূচক &&
	  required_feature_stage[table_সূচক] == stage)
	add_lookups (m, table_সূচক,
		     required_feature_সূচক[table_সূচক],
		     key,
		     global_bit_mask);

      জন্য (auto &feature : m.features)
      {
	যদি (feature.stage[table_সূচক] == stage)
	  add_lookups (m, table_সূচক,
		       feature.সূচক_2[table_সূচক],
		       key,
		       feature.mask,
		       feature.auto_zwnj,
		       feature.auto_zwj,
		       feature.random,
		       feature.per_syllable,
		       feature.tag);
      }

      /* Sort lookups and merge duplicates */
      যদি (last_অঙ্ক_lookups + 1 < lookups.দৈর্ঘ্য)
      {
	lookups.as_array ().sub_array (last_অঙ্ক_lookups, lookups.দৈর্ঘ্য - last_অঙ্ক_lookups).qsort ();

	unsigned পূর্ণসংখ্যা j = last_অঙ্ক_lookups;
	জন্য (unsigned পূর্ণসংখ্যা i = j + 1; i < lookups.দৈর্ঘ্য; i++)
	  যদি (lookups.arrayZ[i].সূচক_2 != lookups.arrayZ[j].সূচক_2)
	    lookups.arrayZ[++j] = lookups.arrayZ[i];
	  অন্যথা
	  {
	    lookups.arrayZ[j].mask |= lookups.arrayZ[i].mask;
	    lookups.arrayZ[j].auto_zwnj &= lookups.arrayZ[i].auto_zwnj;
	    lookups.arrayZ[j].auto_zwj &= lookups.arrayZ[i].auto_zwj;
	  }
	lookups.shrink (j + 1);
      }

      last_অঙ্ক_lookups = lookups.দৈর্ঘ্য;

      যদি (stage_সূচক < stages[table_সূচক].দৈর্ঘ্য && stages[table_সূচক][stage_সূচক].সূচক_2 == stage) {
	hb_ot_map_t::stage_map_t *stage_map = m.stages[table_সূচক].push ();
	stage_map->last_lookup = last_অঙ্ক_lookups;
	stage_map->pause_func = stages[table_সূচক][stage_সূচক].pause_func;

	stage_সূচক++;
      }
    }
  }
}

unsigned পূর্ণসংখ্যা hb_ot_map_t::get_feature_tags (unsigned পূর্ণসংখ্যা start_offset, unsigned পূর্ণসংখ্যা *tag_গুনতি, hb_tag_t *tags) ধ্রুবক
{
  যদি (tag_গুনতি)
  {
    auto sub_features = features.as_array ().sub_array (start_offset, tag_গুনতি);
    যদি (tags)
    {
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < sub_features.দৈর্ঘ্য; i++)
        tags[i] = sub_features[i].tag;
    }
  }

  ফেরত features.দৈর্ঘ্য;
}

#endif
