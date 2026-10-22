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

#include "hb-ot-layout-common.hh"
#include "hb-ot-var-common.hh"

namespace OT {


struct index_map_subset_plan_t
{
  enum index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  void init (const DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     const hb_subset_plan_t  *plan,
	     bool bypass_empty = true)
  {
    map_count = 0;
    full_map_count = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    만약 (bypass_empty && (!index_map || !index_map->get_map_count ())) 반환;

    unsigned 정수	last_val = (unsigned 정수)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    위해 (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned count = new_to_old_gid_list.length;
    만약 (!index_map)
    {
      map_count = new_to_old_gid_list.tail ().first + 1;
    }
    아니면
    {
      위해 (unsigned j = count; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned 정수 v = index_map->map (old_gid);
        만약 (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	continue;
        }
        만약 (v != last_val)
  	break;
  
        last_gid = gid;
      }
  
      만약 (unlikely (last_gid == (hb_codepoint_t)-1)) 반환;
      map_count = last_gid + 1;
    }

    위해 (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      만약 (gid >= map_count) break;

      hb_codepoint_t old_gid = _.second;
      unsigned 정수 v = index_map ? index_map->map (old_gid): old_gid;
      unsigned 정수 outer = v >> 16;
      unsigned 정수 inner = v & 0xFFFF;
      outer_map.add (outer);
      만약 (inner > max_inners[outer]) max_inners[outer] = inner;
      만약 (outer >= inner_sets.length) 반환;
      inner_sets[outer]->add (inner);
    }
  }

  void fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  void remap (const DeltaSetIndexMap *input_map,
	      const hb_inc_bimap_t &outer_map,
	      const hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      const hb_subset_plan_t *plan)
  {
    outer_bit_count = 1;
    inner_bit_count = 1;

    위해 (unsigned 정수 i = 0; i < max_inners.length; i++)
    {
      만약 (inner_maps[i].get_population () == 0) continue;
      unsigned 정수 bit_count = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      만약 (bit_count > inner_bit_count) inner_bit_count = bit_count;
    }

    만약 (unlikely (!output_map.resize (map_count))) 반환;
    위해 (const auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      만약 (unlikely (new_gid >= map_count)) break;

      uint32_t v = input_map? input_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_count = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_count = hb_max (bit_count, outer_bit_count);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  bool remap_after_instantiation (const hb_subset_plan_t *plan,
                                  const hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_count = 1;
    inner_bit_count = 1;

    위해 (const auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      만약 (unlikely (new_gid >= map_count)) break;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      만약 (!varidx_map.has (v, &new_varidx))
        반환 false;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_count = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_count = hb_max (bit_count, outer_bit_count);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_count = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_count = hb_max (bit_count, inner_bit_count);
    }
    반환 true;
  }

  unsigned 정수 get_inner_bit_count () const { 반환 inner_bit_count; }
  unsigned 정수 get_width ()           const { 반환 ((outer_bit_count + inner_bit_count + 7) / 8); }
  unsigned 정수 get_map_count ()       const { 반환 map_count; }

  size_t get_size () const
  {
    만약 (!map_count) 반환 0;
    반환 hb_unsigned_mul_add_saturate (get_width (), map_count,
					 DeltaSetIndexMap::min_size);
  }

  bool is_identity () const
  {
    만약 (!output_map) 반환 true;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    만약 (!can_drop_to_implicit || map_count != full_map_count) 반환 false;
    위해 (unsigned i = 0; i < map_count; i++)
      만약 (output_map.arrayZ[i] != i) 반환 false;
    반환 true;
  }
  hb_array_t<const uint32_t> get_output_map () const { 반환 output_map.as_array (); }

  protected:
  unsigned 정수 map_count;
  unsigned 정수 full_map_count;
  bool can_drop_to_implicit;
  hb_vector_t<unsigned 정수> max_inners;
  unsigned 정수 outer_bit_count;
  unsigned 정수 inner_bit_count;
  hb_vector_t<uint32_t> output_map;
};

struct hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  void init (const hb_array_t<const DeltaSetIndexMap *> &index_maps,
	     const ItemVariationStore &_var_store,
	     const hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_count ());
    위해 (unsigned 정수 i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_count ());

    만약 (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) 반환;

    bool retain_adv_map = false;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, false);
    만약 (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      위해 (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    위해 (unsigned 정수 i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    만약 (retain_adv_map)
    {
      위해 (const auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    아니면
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    위해 (unsigned 정수 i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    위해 (unsigned 정수 i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  bool remap_index_map_plans (const hb_subset_plan_t *plan,
                              const hb_map_t& varidx_map)
  {
    위해 (unsigned i = 0; i < index_map_plans.length; i++)
      만약 (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        반환 false;
    반환 true;
  }

  void fini ()
  {
    위해 (unsigned 정수 i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  const ItemVariationStore *var_store;

  protected:
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

struct HVARVVAR
{
  static constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  static constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  bool has_data () const { 반환 version.major != 0; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, this) &&
		  advMap.sanitize (c, this) &&
		  lsbMap.sanitize (c, this) &&
		  rsbMap.sanitize (c, this));
  }

  const ItemVariationStore& get_var_store () const
  { 반환 this+varStore; }

  void listup_index_maps (hb_vector_t<const DeltaSetIndexMap *> &index_maps) const
  {
    만약 (advMap) index_maps.push (&(this+advMap));
    아니면 index_maps.push (nullptr);

    만약 (lsbMap) index_maps.push (&(this+lsbMap));
    아니면 index_maps.push (nullptr);

    만약 (rsbMap) index_maps.push (&(this+rsbMap));
    아니면 index_maps.push (nullptr);
  }

  bool serialize_index_maps (hb_serialize_context_t *c,
			     const hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (this);
    만약 (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    아니면 만약 (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (false);
    만약 (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    아니면 만약 (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (false);
    만약 (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    아니면 만약 (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (false);

    return_trace (true);
  }

  template <typename T>
  bool _subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    만약 (c->plan->all_axes_pinned)
      return_trace (false);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<const DeltaSetIndexMap *>
				index_maps;

    ((T*)this)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), this+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    만약 (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;

    만약 (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      만약 (!item_vars.instantiate (this+varStore, c->plan,
                                  advMap == 0 ? false : true,
                                  false, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (false);

      만약 (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (false);

      /* if varstore is optimized, remap output_map */
      만약 (advMap)
      {
        만약 (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (false);
      }
    }
    아니면
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      만약 (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      만약 (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (false);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  float get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    const 정수 *coords, unsigned 정수 coord_count,
				    hb_scalar_cache_t *store_cache = nullptr) const
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    만약 (!advMap && glyph > 0xFFFFu) 반환 0.f;
    uint32_t varidx = (this+advMap).map (glyph);
    반환 (this+varStore).get_delta (varidx,
				      coords, coord_count,
				      store_cache);
  }

  public:
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

  public:
  DEFINE_SIZE_STATIC (20);
};

struct HVAR : HVARVVAR {
  static constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  bool subset (hb_subset_context_t *c) const { 반환 HVARVVAR::_subset<HVAR> (c); }
};
struct VVAR : HVARVVAR {
  static constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (static_cast<const HVARVVAR *> (this)->sanitize (c) &&
		  vorgMap.sanitize (c, this));
  }

  void listup_index_maps (hb_vector_t<const DeltaSetIndexMap *> &index_maps) const
  {
    HVARVVAR::listup_index_maps (index_maps);
    만약 (vorgMap) index_maps.push (&(this+vorgMap));
    아니면 index_maps.push (nullptr);
  }

  bool serialize_index_maps (hb_serialize_context_t *c,
			     const hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (this);
    만약 (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (false);
    만약 (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_count ())
      vorgMap = 0;
    아니면 만약 (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (false);

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const { 반환 HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  float get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 const 정수 *coords, unsigned 정수 coord_count,
				 hb_scalar_cache_t *store_cache = nullptr) const
  {
    만약 (!vorgMap) 반환 0.f;
    uint32_t varidx = (this+vorgMap).map (glyph);
    반환 (this+varStore).get_delta (varidx,
				      coords, coord_count,
				      store_cache);
  }

  protected:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  public:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
