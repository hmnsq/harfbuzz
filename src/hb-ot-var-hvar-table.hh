/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_VAR_HVAR_TABLE_HH
#define HB_OT_VAR_HVAR_TABLE_HH

#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-var-common.hh"

नामाकाश OT {


संरचना index_map_subset_plan_t
{
  गणना index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  शून्य init (ध्रुव DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ध्रुव hb_subset_plan_t  *plan,
	     तार्किक bypass_empty = सच्च)
  {
    map_गणनम् = 0;
    full_map_गणनम् = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    जइ (bypass_empty && (!index_map || !index_map->get_map_गणनम् ())) प्रत्यागम;

    unsigned पूर्णाङ्क	last_val = (unsigned पूर्णाङ्क)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    प्रति (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned गणनम् = new_to_old_gid_list.length;
    जइ (!index_map)
    {
      map_गणनम् = new_to_old_gid_list.tail ().first + 1;
    }
    अण्णहा
    {
      प्रति (unsigned j = गणनम्; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned पूर्णाङ्क v = index_map->map (old_gid);
        जइ (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	अनुवर्त;
        }
        जइ (v != last_val)
  	भङ्ग;
  
        last_gid = gid;
      }
  
      जइ (unlikely (last_gid == (hb_codepoint_t)-1)) प्रत्यागम;
      map_गणनम् = last_gid + 1;
    }

    प्रति (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      जइ (gid >= map_गणनम्) भङ्ग;

      hb_codepoint_t old_gid = _.second;
      unsigned पूर्णाङ्क v = index_map ? index_map->map (old_gid): old_gid;
      unsigned पूर्णाङ्क outer = v >> 16;
      unsigned पूर्णाङ्क inner = v & 0xFFFF;
      outer_map.add (outer);
      जइ (inner > max_inners[outer]) max_inners[outer] = inner;
      जइ (outer >= inner_sets.length) प्रत्यागम;
      inner_sets[outer]->add (inner);
    }
  }

  शून्य fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  शून्य remap (ध्रुव DeltaSetIndexMap *निवेशः_map,
	      ध्रुव hb_inc_bimap_t &outer_map,
	      ध्रुव hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ध्रुव hb_subset_plan_t *plan)
  {
    outer_bit_गणनम् = 1;
    inner_bit_गणनम् = 1;

    प्रति (unsigned पूर्णाङ्क i = 0; i < max_inners.length; i++)
    {
      जइ (inner_maps[i].get_population () == 0) अनुवर्त;
      unsigned पूर्णाङ्क bit_गणनम् = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      जइ (bit_गणनम् > inner_bit_गणनम्) inner_bit_गणनम् = bit_गणनम्;
    }

    जइ (unlikely (!output_map.resize (map_गणनम्))) प्रत्यागम;
    प्रति (ध्रुव auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      जइ (unlikely (new_gid >= map_गणनम्)) भङ्ग;

      uint32_t v = निवेशः_map? निवेशः_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_गणनम् = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_गणनम् = hb_max (bit_गणनम्, outer_bit_गणनम्);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  तार्किक remap_after_instantiation (ध्रुव hb_subset_plan_t *plan,
                                  ध्रुव hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_गणनम् = 1;
    inner_bit_गणनम् = 1;

    प्रति (ध्रुव auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      जइ (unlikely (new_gid >= map_गणनम्)) भङ्ग;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      जइ (!varidx_map.has (v, &new_varidx))
        प्रत्यागम मिच्छ;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_गणनम् = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_गणनम् = hb_max (bit_गणनम्, outer_bit_गणनम्);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_गणनम् = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_गणनम् = hb_max (bit_गणनम्, inner_bit_गणनम्);
    }
    प्रत्यागम सच्च;
  }

  unsigned पूर्णाङ्क get_inner_bit_गणनम् () ध्रुव { प्रत्यागम inner_bit_गणनम्; }
  unsigned पूर्णाङ्क get_width ()           ध्रुव { प्रत्यागम ((outer_bit_गणनम् + inner_bit_गणनम् + 7) / 8); }
  unsigned पूर्णाङ्क get_map_गणनम् ()       ध्रुव { प्रत्यागम map_गणनम्; }

  size_t get_size () ध्रुव
  {
    जइ (!map_गणनम्) प्रत्यागम 0;
    प्रत्यागम hb_unsigned_mul_add_saturate (get_width (), map_गणनम्,
					 DeltaSetIndexMap::min_size);
  }

  तार्किक is_identity () ध्रुव
  {
    जइ (!output_map) प्रत्यागम सच्च;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    जइ (!can_drop_to_implicit || map_गणनम् != full_map_गणनम्) प्रत्यागम मिच्छ;
    प्रति (unsigned i = 0; i < map_गणनम्; i++)
      जइ (output_map.arrayZ[i] != i) प्रत्यागम मिच्छ;
    प्रत्यागम सच्च;
  }
  hb_array_t<ध्रुव uint32_t> get_output_map () ध्रुव { प्रत्यागम output_map.as_array (); }

  रक्षित:
  unsigned पूर्णाङ्क map_गणनम्;
  unsigned पूर्णाङ्क full_map_गणनम्;
  तार्किक can_drop_to_implicit;
  hb_vector_t<unsigned पूर्णाङ्क> max_inners;
  unsigned पूर्णाङ्क outer_bit_गणनम्;
  unsigned पूर्णाङ्क inner_bit_गणनम्;
  hb_vector_t<uint32_t> output_map;
};

संरचना hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  शून्य init (ध्रुव hb_array_t<ध्रुव DeltaSetIndexMap *> &index_maps,
	     ध्रुव ItemVariationStore &_var_store,
	     ध्रुव hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_गणनम् ());
    प्रति (unsigned पूर्णाङ्क i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_गणनम् ());

    जइ (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) प्रत्यागम;

    तार्किक retain_adv_map = मिच्छ;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, मिच्छ);
    जइ (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      प्रति (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    प्रति (unsigned पूर्णाङ्क i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    जइ (retain_adv_map)
    {
      प्रति (ध्रुव auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    अण्णहा
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    प्रति (unsigned पूर्णाङ्क i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    प्रति (unsigned पूर्णाङ्क i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  तार्किक remap_index_map_plans (ध्रुव hb_subset_plan_t *plan,
                              ध्रुव hb_map_t& varidx_map)
  {
    प्रति (unsigned i = 0; i < index_map_plans.length; i++)
      जइ (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        प्रत्यागम मिच्छ;
    प्रत्यागम सच्च;
  }

  शून्य fini ()
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ध्रुव ItemVariationStore *var_store;

  रक्षित:
  hb_vector_t<hb_set_t *> inner_sets;
  hb_set_t *adv_set;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#define HB_OT_TAG_HVAR HB_TAG('H','V','A','R')
#define HB_OT_TAG_VVAR HB_TAG('V','V','A','R')

संरचना HVARVVAR
{
  स्थिर constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  स्थिर constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  तार्किक has_data () ध्रुव { प्रत्यागम version.major != 0; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, एतत्) &&
		  advMap.sanitize (c, एतत्) &&
		  lsbMap.sanitize (c, एतत्) &&
		  rsbMap.sanitize (c, एतत्));
  }

  ध्रुव ItemVariationStore& get_var_store () ध्रुव
  { प्रत्यागम एतत्+varStore; }

  शून्य listup_index_maps (hb_vector_t<ध्रुव DeltaSetIndexMap *> &index_maps) ध्रुव
  {
    जइ (advMap) index_maps.push (&(एतत्+advMap));
    अण्णहा index_maps.push (nullptr);

    जइ (lsbMap) index_maps.push (&(एतत्+lsbMap));
    अण्णहा index_maps.push (nullptr);

    जइ (rsbMap) index_maps.push (&(एतत्+rsbMap));
    अण्णहा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     ध्रुव hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    अण्णहा जइ (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (मिच्छ);
    जइ (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    अण्णहा जइ (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (मिच्छ);
    जइ (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    अण्णहा जइ (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (मिच्छ);

    return_trace (सच्च);
  }

  template <typename T>
  तार्किक _subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (c->plan->all_axes_pinned)
      return_trace (मिच्छ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ध्रुव DeltaSetIndexMap *>
				index_maps;

    ((T*)एतत्)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), एतत्+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    जइ (unlikely (!out)) return_trace (मिच्छ);

    out->version.major = 1;
    out->version.minor = 0;

    जइ (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      जइ (!item_vars.instantiate (एतत्+varStore, c->plan,
                                  advMap == 0 ? मिच्छ : सच्च,
                                  मिच्छ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (मिच्छ);

      जइ (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (मिच्छ);

      /* if varstore is optimized, remap output_map */
      जइ (advMap)
      {
        जइ (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (मिच्छ);
      }
    }
    अण्णहा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      जइ (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      जइ (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (मिच्छ);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  भिन्न get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
				    hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    जइ (!advMap && glyph > 0xFFFFu) प्रत्यागम 0.f;
    uint32_t varidx = (एतत्+advMap).map (glyph);
    प्रत्यागम (एतत्+varStore).get_delta (varidx,
				      coords, coord_गणनम्,
				      store_cache);
  }

  सार्वजनिक:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  Offset32To<DeltaSetIndexMap>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32To<DeltaSetIndexMap>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32To<DeltaSetIndexMap>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना HVAR : HVARVVAR {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  तार्किक subset (hb_subset_context_t *c) ध्रुव { प्रत्यागम HVARVVAR::_subset<HVAR> (c); }
};
संरचना VVAR : HVARVVAR {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (static_cast<ध्रुव HVARVVAR *> (एतत्)->sanitize (c) &&
		  vorgMap.sanitize (c, एतत्));
  }

  शून्य listup_index_maps (hb_vector_t<ध्रुव DeltaSetIndexMap *> &index_maps) ध्रुव
  {
    HVARVVAR::listup_index_maps (index_maps);
    जइ (vorgMap) index_maps.push (&(एतत्+vorgMap));
    अण्णहा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     ध्रुव hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (मिच्छ);
    जइ (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_गणनम् ())
      vorgMap = 0;
    अण्णहा जइ (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (मिच्छ);

    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव { प्रत्यागम HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  भिन्न get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
				 hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    जइ (!vorgMap) प्रत्यागम 0.f;
    uint32_t varidx = (एतत्+vorgMap).map (glyph);
    प्रत्यागम (एतत्+varStore).get_delta (varidx,
				      coords, coord_गणनम्,
				      store_cache);
  }

  रक्षित:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
