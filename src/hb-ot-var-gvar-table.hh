/*
 * Copyright © 2019  Adobe Inc.
 * Copyright © 2019  Ebrahim Byagowi
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
 * Adobe Author(s): Michiharu Ariza
 */

#ifndef HB_OT_VAR_GVAR_TABLE_HH
#define HB_OT_VAR_GVAR_TABLE_HH

#include "hb-decycler.hh"
#include "hb-open-type.hh"
#include "hb-ot-dual.hh"
#include "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

struct hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<số_nguyên> x_deltas;
  hb_vector_t<số_nguyên> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned số_nguyên> shared_indices;
  hb_vector_t<unsigned số_nguyên> private_indices;
};

namespace OT {

template <typename OffsetType>
struct glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  private:
  unsigned shared_tuples_count = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  public:
  unsigned compiled_shared_tuples_count () const
  { trả_về shared_tuples_count; }

  unsigned compiled_byte_size () const
  {
    unsigned byte_size = 0;
    cho (const auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    trả_về byte_size;
  }

  bool create_from_glyphs_var_data (unsigned axis_count,
                                    const hb_array_t<const F2DOT14> shared_tuples,
                                    const hb_subset_plan_t *plan,
                                    const hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    nếu (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      trả_về false;

    auto it = hb_iter (plan->new_to_old_gid_list);
    cho (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      nếu (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        trả_về false;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      const GlyphVariationData* p = reinterpret_cast<const GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      nếu (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_count,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        continue;
      }

      bool is_composite_glyph = false;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      nếu (!p->decompile_tuple_variations (all_contour_points->length, true /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        trả_về false;
      glyph_variations.push (std::move (tuple_vars));
    }
    trả_về !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  bool instantiate (const hb_subset_plan_t *plan)
  {
    unsigned count = plan->new_to_old_gid_list.length;
    bool iup_optimize = false;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    cho (unsigned i = 0; i < count; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      nếu (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        trả_về false;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      nếu (iup_optimize)
      {
        points = *all_points;
        nếu (unlikely (points.in_error ())) trả_về false;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      nếu (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      nếu (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        trả_về false;
    }
    trả_về true;
  }

  bool compile_bytes (const hb_map_t& axes_index_map,
                      const hb_map_t& axes_old_index_tag_map)
  {
    nếu (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      trả_về false;
    cho (tuple_variations_t& vars: glyph_variations)
      nếu (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               true, /* use shared points*/
                               true,
                               &shared_tuples_idx_map,
			       &pool))
        trả_về false;

    trả_về true;
  }

  bool compile_shared_tuples (const hb_map_t& axes_index_map,
                              const hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned> coords_count_map;

    /* count the num of shared coords */
    cho (tuple_variations_t& vars: glyph_variations)
    {
      cho (tuple_delta_t& var : vars.tuple_vars)
      {
        nếu (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          trả_về false;
        unsigned *count;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        nếu (coords_count_map.has_with_hash (&(var.compiled_peak_coords), hash, &count))
	  (*count)++;
        không_thì
          coords_count_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    nếu (!coords_count_map || coords_count_map.in_error ())
      trả_về false;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<const hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_count_map)
      | hb_filter ([] (const hb_pair_t<const hb_vector_t<F2DOT14>*, unsigned>& p) { trả_về p.second > 1; })
    };
    nếu (unlikely (shared_coords.in_error ())) trả_về false;

    /* no shared tuples: no coords are used more than once */
    nếu (!shared_coords) trả_về true;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_count = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_count * (shared_coords[0].first->length);
    nếu (unlikely (!compiled_shared_tuples.alloc (len)))
      trả_về false;

    cho (unsigned i = 0; i < shared_tuples_count; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      cho (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    trả_về true;
  }

  static số_nguyên _cmp_coords (const void *pa, const void *pb)
  {
    const hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (const hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    const hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (const hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    nếu (a->second != b->second)
      trả_về b->second - a->second; // high to low

    trả_về b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 bool long_offset,
                                 unsigned num_glyphs,
                                 char* glyph_var_data_offsets /* OUT: glyph var data offsets array */) const
  {
    TRACE_SERIALIZE (this);

    nếu (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    không_thì
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    nếu (!cur_glyph) return_trace (false);
    cho (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      nếu (long_offset)
        cho (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      không_thì
        cho (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      nếu (idx >= glyph_variations.length) return_trace (false);
      nếu (!cur_glyph->serialize (c, true, glyph_variations[idx])) return_trace (false);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (char *) next_glyph - (char *) cur_glyph;

      nếu (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      không_thì
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    nếu (long_offset)
      cho (; last_gid < num_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    không_thì
      cho (; last_gid < num_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (true);
  }
};

template <typename T, typename Types>
struct gvar_GVAR
{
  using GlyphCountType = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<GlyphCountType>;
  static constexpr unsigned glyph_count_max ()
  { trả_về (1u << (8 * GlyphCountType::static_size)) - 1; }

  bool has_data () const { trả_về version.to_int () != 0; }

  bool sanitize_shallow (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, this, axisCount * sharedTupleCount) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_num_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_num_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  bool sanitize (hb_sanitize_context_t *c) const
  { trả_về sanitize_shallow (c); }

  bool decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<GlyphCountType>& glyph_vars /* OUT */) const
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    nếu (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    cho (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyphCountX, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    nếu (new_gid_var_data_map.in_error ()) trả_về false;

    hb_array_t<const F2DOT14> shared_tuples = (this+sharedTuples).as_array ((unsigned) sharedTupleCount * (unsigned) axisCount);
    trả_về glyph_vars.create_from_glyphs_var_data (axisCount, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
                  const glyph_variations_t<GlyphCountType>& glyph_vars,
                  Iterator it,
                  unsigned axis_count,
                  unsigned num_glyphs,
                  bool force_long_offsets) const
  {
    TRACE_SERIALIZE (this);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    nếu (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;
    out->axisCount = axis_count;
    out->glyphCountX = hb_min (glyph_count_max (), num_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    bool long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (num_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, false);
    nếu (!glyph_var_data_offsets) return_trace (false);

    /* shared tuples */
    unsigned shared_tuple_count = glyph_vars.compiled_shared_tuples_count ();
    out->sharedTupleCount = shared_tuple_count;

    nếu (!shared_tuple_count)
      out->sharedTuples = (char *) glyph_var_data_offsets + glyph_var_data_offsets_size - (char *) out;
    không_thì
    {
      hb_array_t<const F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      nếu (!shared_tuples.arrayZ) return_trace (false);
      out->sharedTuples = (const char *) shared_tuples.arrayZ - (char *) out;
    }

    char *glyph_var_data = c->start_embed<char> ();
    nếu (!glyph_var_data) return_trace (false);
    out->dataZ = glyph_var_data - (char *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, num_glyphs,
                                                       (char *) glyph_var_data_offsets));
  }

  bool instantiate (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    glyph_variations_t<GlyphCountType> glyph_vars;
    nếu (!decompile_glyph_variations (c, glyph_vars))
      return_trace (false);

    nếu (!glyph_vars.instantiate (c->plan)) return_trace (false);
    nếu (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (false);

    unsigned axis_count = c->plan->axes_index_map.get_population ();
    unsigned num_glyphs = c->plan->num_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    bool force_long_offsets = false;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_count, num_glyphs, force_long_offsets));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    nếu (c->plan->all_axes_pinned)
      return_trace (false);

    nếu (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_count = version.to_int () ? c->plan->source->get_num_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    nếu (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;
    out->axisCount = axisCount;
    out->sharedTupleCount = sharedTupleCount;

    unsigned số_nguyên num_glyphs = c->plan->num_output_glyphs ();
    out->glyphCountX = hb_min (glyph_count_max (), num_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<char>> culled_vars;
    hb_set_t culled_gids;
    nếu (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<const F2DOT14> shared_tuples_array =
	  (this+sharedTuples).as_array ((unsigned) sharedTupleCount * (unsigned) axisCount);
      nếu (unlikely (!culled_vars.resize (num_glyphs))) return_trace (false);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      nếu (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      cho (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_count, _.second);
	nếu (var_data_bytes.length < GlyphVariationData::min_size) continue;
	bool changed = false;
	nếu (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axisCount,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (false);
	nếu (changed) culled_gids.add (_.first);
      }
      nếu (unlikely (culled_gids.in_error ())) return_trace (false);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    nếu (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    cho (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_count, old_gid).length;
      nếu (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      nếu (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (false);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    bool long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (num_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, false);
    nếu (!subset_offsets) return_trace (false);

    /* shared tuples */
    nếu (!sharedTupleCount || !sharedTuples)
      out->sharedTuples = (char *) subset_offsets + subset_offsets_size - (char *) out;
    không_thì
    {
      unsigned số_nguyên shared_tuple_size = F2DOT14::static_size * axisCount * sharedTupleCount;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      nếu (!tuples) return_trace (false);
      out->sharedTuples = (char *) tuples - (char *) out;
      hb_memcpy (tuples, this+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    nếu (long_offset)
      subset_data_size -= padding_size;
    char *subset_data = c->serializer->allocate_size<char> (subset_data_size, false);
    nếu (!subset_data) return_trace (false);
    out->dataZ = subset_data - (char *) out;


    nếu (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    không_thì
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned số_nguyên glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    nếu (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    cho (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      nếu (long_offset)
	cho (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      không_thì
	cho (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_count,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      nếu (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      nếu (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      không_thì
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    nếu (long_offset)
      cho (; last < num_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    không_thì
      cho (; last < num_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (true);
  }

  protected:
  const hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_count,
					     hb_codepoint_t glyph) const
  {
    unsigned start_offset = get_offset (glyph_count, glyph);
    unsigned end_offset = get_offset (glyph_count, glyph+1);
    nếu (unlikely (end_offset < start_offset)) trả_về hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    trả_về likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  bool is_long_offset () const { trả_về flags & 1; }

  unsigned get_offset (unsigned glyph_count, unsigned i) const
  {
    nếu (unlikely (i > glyph_count)) trả_về 0;
    hb_barrier ();
    trả_về is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  const HBUINT32 * get_long_offset_array () const { trả_về (const HBUINT32 *) &offsetZ; }
  const HBUINT16 *get_short_offset_array () const { trả_về (const HBUINT16 *) &offsetZ; }

  public:
  struct accelerator_t
  {

    hb_scalar_cache_t *create_cache () const
    {
      trả_về hb_scalar_cache_t::create (hb_min ((unsigned) table->sharedTupleCount,
						+TupleVariationHeader::max_shared_tuple_count));
    }

    static void destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    bool has_data () const { trả_về table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyphCount = table->version.to_int () ? face->get_num_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    private:

    static float infer_delta (const hb_array_t<contour_point_t> points,
			      const hb_array_t<contour_point_t> deltas,
			      unsigned số_nguyên target, unsigned số_nguyên prev, unsigned số_nguyên next,
			      float contour_point_t::*m)
    {
      float target_val = points.arrayZ[target].*m;
      float prev_val = points.arrayZ[prev].*m;
      float next_val = points.arrayZ[next].*m;
      float prev_delta =  deltas.arrayZ[prev].*m;
      float next_delta =  deltas.arrayZ[next].*m;

      nếu (prev_val == next_val)
	trả_về (prev_delta == next_delta) ? prev_delta : 0.f;
      không_thì nếu (target_val <= hb_min (prev_val, next_val))
	trả_về (prev_val < next_val) ? prev_delta : next_delta;
      không_thì nếu (target_val >= hb_max (prev_val, next_val))
	trả_về (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      float r = (target_val - prev_val) / (next_val - prev_val);
      trả_về prev_delta + r * (next_delta - prev_delta);
    }

    static unsigned số_nguyên next_index (unsigned số_nguyên i, unsigned số_nguyên start, unsigned số_nguyên end)
    { trả_về (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <bool is_x>
#endif
    static bool decompile_deltas_add_to_points (const HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						float scalar,
						const HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, bool is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned count = points.length;
      trong_khi (i < count)
      {
	nếu (unlikely (p + 1 > end)) trả_về false;
	unsigned control = *p++;
	unsigned run_count = (control & TupleValues::VALUE_RUN_COUNT_MASK) + 1;
	unsigned stop = i + run_count;
	nếu (unlikely (stop > count)) trả_về false;

	unsigned skip = i < start ? hb_min (start - i, run_count) : 0;
	i += skip;

	switch (control & TupleValues::VALUES_SIZE_MASK)
	{
	  case TupleValues::VALUES_ARE_ZEROS:
	    i = stop;
	    break;
	  case TupleValues::VALUES_ARE_WORDS:
	  {
	    nếu (unlikely (p + run_count * HBINT16::static_size > end)) trả_về false;
	    p += skip * HBINT16::static_size;
	    const auto *pp = (const HBINT16 *) p;
	    cho (; i < stop; i++)
	    {
	      float v = *pp++ * scalar;
	      nếu (is_x) points.arrayZ[i].x += v;
	      không_thì points.arrayZ[i].y += v;
	    }
	    p = (const HBUINT8 *) pp;
	  }
	  break;
	  case TupleValues::VALUES_ARE_LONGS:
	  {
	    nếu (unlikely (p + run_count * HBINT32::static_size > end)) trả_về false;
	    p += skip * HBINT32::static_size;
	    const auto *pp = (const HBINT32 *) p;
	    cho (; i < stop; i++)
	    {
	      float v = *pp++ * scalar;
	      nếu (is_x) points.arrayZ[i].x += v;
	      không_thì points.arrayZ[i].y += v;
	    }
	    p = (const HBUINT8 *) pp;
	  }
	  break;
	  case TupleValues::VALUES_ARE_BYTES:
	  {
	    nếu (unlikely (p + run_count > end)) trả_về false;
	    p += skip * HBINT8::static_size;
	    const auto *pp = (const HBINT8 *) p;
	    cho (; i < stop; i++)
	    {
	      float v = *pp++ * scalar;
	      nếu (is_x) points.arrayZ[i].x += v;
	      không_thì points.arrayZ[i].y += v;
	    }
	    p = (const HBUINT8 *) pp;
	  }
	  break;
	}
      }
      trả_về true;
    }
    public:
    bool apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<const số_nguyên> coords,
				 const hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 bool phantom_only = false,
				 int64_t *budget = nullptr) const
    {
      nếu (unlikely (glyph >= glyphCount)) trả_về true;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyphCount, glyph);
      nếu (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) trả_về true;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      nếu (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axisCount,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	trả_về true; /* so isn't applied at all */

      bool any_private_points = false;
      bool private_points_checked = false;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned num_coords = table->axisCount;
      hb_array_t<const F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->sharedTupleCount * num_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned count = points.length;
      bool flush = false;

      do
      {
	float scalar = iterator.current_tuple->calculate_scalar (coords, num_coords, shared_tuples,
								 scalar_cache);

	nếu (scalar == 0.f) continue;

	nếu (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && count >= 4 ? 4 : count)))
	  trả_về false;

	nếu (!private_points_checked)
	{
	  auto scan = iterator;
	  do
	  {
	    nếu (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = true;
	      break;
	    }
	  } trong_khi (scan.move_to_next ());
	  private_points_checked = true;
	}
	const HBUINT8 *p = iterator.get_serialized_data ();
	unsigned số_nguyên length = iterator.current_tuple->get_data_size ();
	nếu (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  trả_về false;

	nếu (!deltas)
	{
	  nếu (unlikely (!deltas_vec.resize_dirty  (count))) trả_về false;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? count - 4 : 0), 0,
		     (phantom_only ? 4 : count) * sizeof (deltas[0]));
	}

	const HBUINT8 *end = p + length;

	bool has_private_points = iterator.current_tuple->has_private_points ();
	nếu (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  trả_về false;
	const hb_array_t<unsigned số_nguyên> &indices = has_private_points ? private_indices : shared_indices;

	bool apply_to_all = (indices.length == 0);
	unsigned num_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && num_deltas >= 4 ? num_deltas - 4 : 0);

	nếu (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  nếu (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, true))) trả_về false;
	  nếu (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, false))) trả_về false;
#không_thì
	  nếu (unlikely (!decompile_deltas_add_to_points<true> (p, points, scalar, end, start_deltas))) trả_về false;
	  nếu (unlikely (!decompile_deltas_add_to_points<false> (p, points, scalar, end, start_deltas))) trả_về false;
#endif
	  continue;
	}

	nếu (unlikely (!x_deltas.resize_dirty  (num_deltas))) trả_về false;
	nếu (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, false, start_deltas))) trả_về false;
	nếu (unlikely (!y_deltas.resize_dirty  (num_deltas))) trả_về false;
	nếu (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, false, start_deltas))) trả_về false;

	nếu (!apply_to_all)
	{
	  nếu (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    nếu (unlikely (orig_points_vec.in_error ())) trả_về false;
	    orig_points = orig_points_vec.as_array ();
	  }

	  nếu (flush)
	  {
	    cho (unsigned số_nguyên i = phantom_only ? count - 4 : 0; i < count; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? count - 4 : 0), 0,
		     (phantom_only ? 4 : count) * sizeof (deltas[0]));
	}

	nếu (HB_OPTIMIZE_SIZE_VAL)
	{
	  cho (unsigned số_nguyên i = 0; i < num_deltas; i++)
	  {
	    unsigned số_nguyên pt_index;
	    nếu (apply_to_all)
	      pt_index = i;
	    không_thì
	    {
	      pt_index = indices[i];
	      nếu (unlikely (pt_index >= deltas.length)) continue;
	    }
	    nếu (phantom_only && pt_index < count - 4) continue;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	không_thì
	{
	  /* Ouch. Four cases... for optimization. */
	  nếu (scalar != 1.0f)
	  {
	    nếu (apply_to_all)
	      cho (unsigned số_nguyên i = phantom_only ? count - 4 : 0; i < count; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    không_thì
	      cho (unsigned số_nguyên i = 0; i < num_deltas; i++)
	      {
		unsigned số_nguyên pt_index = indices[i];
		nếu (unlikely (pt_index >= deltas.length)) continue;
		nếu (phantom_only && pt_index < count - 4) continue;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  không_thì
	  {
	    nếu (apply_to_all)
	      cho (unsigned số_nguyên i = phantom_only ? count - 4 : 0; i < count; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    không_thì
	      cho (unsigned số_nguyên i = 0; i < num_deltas; i++)
	      {
		unsigned số_nguyên pt_index = indices[i];
		nếu (unlikely (pt_index >= deltas.length)) continue;
		nếu (phantom_only && pt_index < count - 4) continue;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	nếu (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  trong_khi (true)
	  {
	    trong_khi (end_point < count && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    nếu (unlikely (end_point == count)) break;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_count = 0;
	    cho (unsigned i = start_point; i < end_point + 1; i++)
	      unref_count += deltas.arrayZ[i].flag;
	    unref_count = (end_point - start_point + 1) - unref_count;

	    unsigned j = start_point;
	    nếu (unref_count == 0 || unref_count > end_point - start_point)
	      goto no_more_gaps;

	    cho (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned số_nguyên prev, next, i;
	      cho (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		nếu (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) break;
	      }
	      prev = j = i;
	      cho (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		nếu (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) break;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      cho (;;)
	      {
		i = next_index (i, start_point, end_point);
		nếu (i == next) break;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		nếu (--unref_count == 0) goto no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = true;

      } trong_khi (iterator.move_to_next ());

      nếu (flush)
      {
	cho (unsigned số_nguyên i = phantom_only ? count - 4 : 0; i < count; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      trả_về true;
    }

    unsigned số_nguyên get_axis_count () const { trả_về table->axisCount; }

    private:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyphCount;
  };

  protected:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axisCount;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	sharedTupleCount;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  GlyphCountType
		glyphCountX;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32To<GlyphVariationData>
		dataZ;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  UnsizedArrayOf<HBUINT8>
		offsetZ;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  public:
  DEFINE_SIZE_ARRAY (18 + GlyphCountType::static_size, offsetZ);
};

struct gvar : gvar_GVAR<gvar, SmallTypes>
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

struct GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

struct gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () const
  {
    nếu (has_data ()) trả_về HB_DUAL_GET (*this, create_cache ());
    trả_về nullptr;
  }

  static void destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  bool apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<const số_nguyên> coords,
			       const hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       bool phantom_only = false,
			       int64_t *budget = nullptr) const
  {
    trả_về HB_DUAL_GET (*this, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned số_nguyên get_axis_count () const
  {
    nếu (has_data ()) trả_về HB_DUAL_GET (*this, get_axis_count ());
    trả_về 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
