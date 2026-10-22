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

#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"

ନାମାକାଶ OT {


ସଂରଚନା index_map_subset_plan_t
{
  ଗଣନା index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  ଶୂନ୍ୟ init (ଅଚଳ DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ଅଚଳ hb_subset_plan_t  *plan,
	     ତାର୍କିକ bypass_empty = ସତ୍ୟ)
  {
    map_ଗଣନା = 0;
    full_map_ଗଣନା = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    ଯଦି (bypass_empty && (!index_map || !index_map->get_map_ଗଣନା ())) ଫେରନ୍ତୁ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	last_val = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    ପାଇଁ (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned ଗଣନା_2 = new_to_old_gid_list.length;
    ଯଦି (!index_map)
    {
      map_ଗଣନା = new_to_old_gid_list.tail ().first + 1;
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (unsigned j = ଗଣନା_2; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = index_map->map (old_gid);
        ଯଦି (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	ଜାରି;
        }
        ଯଦି (v != last_val)
  	ଭାଙ୍ଗ;
  
        last_gid = gid;
      }
  
      ଯଦି (unlikely (last_gid == (hb_codepoint_t)-1)) ଫେରନ୍ତୁ;
      map_ଗଣନା = last_gid + 1;
    }

    ପାଇଁ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      ଯଦି (gid >= map_ଗଣନା) ଭାଙ୍ଗ;

      hb_codepoint_t old_gid = _.second;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = index_map ? index_map->map (old_gid): old_gid;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = v >> 16;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = v & 0xFFFF;
      outer_map.add (outer);
      ଯଦି (inner > max_inners[outer]) max_inners[outer] = inner;
      ଯଦି (outer >= inner_sets.length) ଫେରନ୍ତୁ;
      inner_sets[outer]->add (inner);
    }
  }

  ଶୂନ୍ୟ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  ଶୂନ୍ୟ remap (ଅଚଳ DeltaSetIndexMap *ନିବେଶ_map,
	      ଅଚଳ hb_inc_bimap_t &outer_map,
	      ଅଚଳ hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ଅଚଳ hb_subset_plan_t *plan)
  {
    outer_bit_ଗଣନା = 1;
    inner_bit_ଗଣନା = 1;

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < max_inners.length; i++)
    {
      ଯଦି (inner_maps[i].get_population () == 0) ଜାରି;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_ଗଣନା = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      ଯଦି (bit_ଗଣନା > inner_bit_ଗଣନା) inner_bit_ଗଣନା = bit_ଗଣନା;
    }

    ଯଦି (unlikely (!output_map.resize (map_ଗଣନା))) ଫେରନ୍ତୁ;
    ପାଇଁ (ଅଚଳ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      ଯଦି (unlikely (new_gid >= map_ଗଣନା)) ଭାଙ୍ଗ;

      uint32_t v = ନିବେଶ_map? ନିବେଶ_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_ଗଣନା = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_ଗଣନା = hb_max (bit_ଗଣନା, outer_bit_ଗଣନା);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  ତାର୍କିକ remap_after_instantiation (ଅଚଳ hb_subset_plan_t *plan,
                                  ଅଚଳ hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_ଗଣନା = 1;
    inner_bit_ଗଣନା = 1;

    ପାଇଁ (ଅଚଳ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      ଯଦି (unlikely (new_gid >= map_ଗଣନା)) ଭାଙ୍ଗ;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      ଯଦି (!varidx_map.has (v, &new_varidx))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_ଗଣନା = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ଗଣନା = hb_max (bit_ଗଣନା, outer_bit_ଗଣନା);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_ଗଣନା = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ଗଣନା = hb_max (bit_ଗଣନା, inner_bit_ଗଣନା);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_inner_bit_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ inner_bit_ଗଣନା; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_width ()           ଅଚଳ { ଫେରନ୍ତୁ ((outer_bit_ଗଣନା + inner_bit_ଗଣନା + 7) / 8); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_map_ଗଣନା ()       ଅଚଳ { ଫେରନ୍ତୁ map_ଗଣନା; }

  size_t get_size () ଅଚଳ
  {
    ଯଦି (!map_ଗଣନା) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (get_width (), map_ଗଣନା,
					 DeltaSetIndexMap::min_size);
  }

  ତାର୍କିକ is_identity () ଅଚଳ
  {
    ଯଦି (!output_map) ଫେରନ୍ତୁ ସତ୍ୟ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    ଯଦି (!can_drop_to_implicit || map_ଗଣନା != full_map_ଗଣନା) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < map_ଗଣନା; i++)
      ଯଦି (output_map.arrayZ[i] != i) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  hb_array_t<ଅଚଳ uint32_t> get_output_map () ଅଚଳ { ଫେରନ୍ତୁ output_map.as_array (); }

  ସୁରକ୍ଷିତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା map_ଗଣନା;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା full_map_ଗଣନା;
  ତାର୍କିକ can_drop_to_implicit;
  hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> max_inners;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer_bit_ଗଣନା;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner_bit_ଗଣନା;
  hb_vector_t<uint32_t> output_map;
};

ସଂରଚନା hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  ଶୂନ୍ୟ init (ଅଚଳ hb_array_t<ଅଚଳ DeltaSetIndexMap *> &index_maps,
	     ଅଚଳ ItemVariationStore &_var_store,
	     ଅଚଳ hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_ଗଣନା ());
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_ଗଣନା ());

    ଯଦି (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) ଫେରନ୍ତୁ;

    ତାର୍କିକ retain_adv_map = ମିଥ୍ୟା;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, ମିଥ୍ୟା);
    ଯଦି (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      ପାଇଁ (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    ଯଦି (retain_adv_map)
    {
      ପାଇଁ (ଅଚଳ auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    ଅନ୍ୟଥା
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  ତାର୍କିକ remap_index_map_plans (ଅଚଳ hb_subset_plan_t *plan,
                              ଅଚଳ hb_map_t& varidx_map)
  {
    ପାଇଁ (unsigned i = 0; i < index_map_plans.length; i++)
      ଯଦି (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ fini ()
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ଅଚଳ ItemVariationStore *var_store;

  ସୁରକ୍ଷିତ:
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

ସଂରଚନା HVARVVAR
{
  ସ୍ଥିର constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  ସ୍ଥିର constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.major != 0; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, ଏହି) &&
		  advMap.sanitize (c, ଏହି) &&
		  lsbMap.sanitize (c, ଏହି) &&
		  rsbMap.sanitize (c, ଏହି));
  }

  ଅଚଳ ItemVariationStore& get_var_store () ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+varStore; }

  ଶୂନ୍ୟ listup_index_maps (hb_vector_t<ଅଚଳ DeltaSetIndexMap *> &index_maps) ଅଚଳ
  {
    ଯଦି (advMap) index_maps.push (&(ଏହି+advMap));
    ଅନ୍ୟଥା index_maps.push (nullptr);

    ଯଦି (lsbMap) index_maps.push (&(ଏହି+lsbMap));
    ଅନ୍ୟଥା index_maps.push (nullptr);

    ଯଦି (rsbMap) index_maps.push (&(ଏହି+rsbMap));
    ଅନ୍ୟଥା index_maps.push (nullptr);
  }

  ତାର୍କିକ serialize_index_maps (hb_serialize_context_t *c,
			     ଅଚଳ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    ଅନ୍ୟଥା ଯଦି (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    ଅନ୍ୟଥା ଯଦି (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    ଅନ୍ୟଥା ଯଦି (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  template <typename T>
  ତାର୍କିକ _subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ମିଥ୍ୟା);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ଅଚଳ DeltaSetIndexMap *>
				index_maps;

    ((T*)ଏହି)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), ଏହି+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->version.major = 1;
    out->version.minor = 0;

    ଯଦି (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      ଯଦି (!item_vars.instantiate (ଏହି+varStore, c->plan,
                                  advMap == 0 ? ମିଥ୍ୟା : ସତ୍ୟ,
                                  ମିଥ୍ୟା, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (ମିଥ୍ୟା);

      ଯଦି (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ମିଥ୍ୟା);

      /* if varstore is optimized, remap output_map */
      ଯଦି (advMap)
      {
        ଯଦି (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (ମିଥ୍ୟା);
      }
    }
    ଅନ୍ୟଥା
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      ଯଦି (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      ଯଦି (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (ମିଥ୍ୟା);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  ଦଶମିକ get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
				    hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    ଯଦି (!advMap && glyph > 0xFFFFu) ଫେରନ୍ତୁ 0.f;
    uint32_t varidx = (ଏହି+advMap).map (glyph);
    ଫେରନ୍ତୁ (ଏହି+varStore).get_delta (varidx,
				      coords, coord_ଗଣନା,
				      store_cache);
  }

  ସର୍ବସାଧାରଣ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (20);
};

ସଂରଚନା HVAR : HVARVVAR {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ { ଫେରନ୍ତୁ HVARVVAR::_subset<HVAR> (c); }
};
ସଂରଚନା VVAR : HVARVVAR {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (static_cast<ଅଚଳ HVARVVAR *> (ଏହି)->sanitize (c) &&
		  vorgMap.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ listup_index_maps (hb_vector_t<ଅଚଳ DeltaSetIndexMap *> &index_maps) ଅଚଳ
  {
    HVARVVAR::listup_index_maps (index_maps);
    ଯଦି (vorgMap) index_maps.push (&(ଏହି+vorgMap));
    ଅନ୍ୟଥା index_maps.push (nullptr);
  }

  ତାର୍କିକ serialize_index_maps (hb_serialize_context_t *c,
			     ଅଚଳ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_ଗଣନା ())
      vorgMap = 0;
    ଅନ୍ୟଥା ଯଦି (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ { ଫେରନ୍ତୁ HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  ଦଶମିକ get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
				 hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    ଯଦି (!vorgMap) ଫେରନ୍ତୁ 0.f;
    uint32_t varidx = (ଏହି+vorgMap).map (glyph);
    ଫେରନ୍ତୁ (ଏହି+varStore).get_delta (varidx,
				      coords, coord_ଗଣନା,
				      store_cache);
  }

  ସୁରକ୍ଷିତ:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
