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

#অন্তর্ভাব "hb-decycler.hh"
#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-dual.hh"
#অন্তর্ভাব "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

সংরচনা hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<পূর্ণাঙ্ক> x_deltas;
  hb_vector_t<পূর্ণাঙ্ক> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned পূর্ণাঙ্ক> shared_indices;
  hb_vector_t<unsigned পূর্ণাঙ্ক> private_indices;
};

নামাকাশ OT {

template <typename OffsetType>
সংরচনা glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  গুপ্ত:
  unsigned shared_tuples_গুনতি = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ধ্রুব hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  সার্বজনিক:
  unsigned compiled_shared_tuples_গুনতি () ধ্রুব
  { প্রত্যাগম shared_tuples_গুনতি; }

  unsigned compiled_byte_size () ধ্রুব
  {
    unsigned byte_size = 0;
    প্রতি (ধ্রুব auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    প্রত্যাগম byte_size;
  }

  তার্কিক create_from_glyphs_var_data (unsigned axis_গুনতি,
                                    ধ্রুব hb_array_t<ধ্রুব F2DOT14> shared_tuples,
                                    ধ্রুব hb_subset_plan_t *plan,
                                    ধ্রুব hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    যদি (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      প্রত্যাগম অসত্য;

    auto it = hb_iter (plan->new_to_old_gid_list);
    প্রতি (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      যদি (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        প্রত্যাগম অসত্য;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ধ্রুব GlyphVariationData* p = reinterpret_cast<ধ্রুব GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      যদি (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_গুনতি,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        অনুবর্ত;
      }

      তার্কিক is_composite_glyph = অসত্য;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      যদি (!p->decompile_tuple_variations (all_contour_points->length, সত্য /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        প্রত্যাগম অসত্য;
      glyph_variations.push (std::move (tuple_vars));
    }
    প্রত্যাগম !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  তার্কিক instantiate (ধ্রুব hb_subset_plan_t *plan)
  {
    unsigned গুনতি = plan->new_to_old_gid_list.length;
    তার্কিক iup_optimize = অসত্য;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      যদি (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        প্রত্যাগম অসত্য;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      যদি (iup_optimize)
      {
        points = *all_points;
        যদি (unlikely (points.in_error ())) প্রত্যাগম অসত্য;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      যদি (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      যদি (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        প্রত্যাগম অসত্য;
    }
    প্রত্যাগম সত্য;
  }

  তার্কিক compile_bytes (ধ্রুব hb_map_t& axes_index_map,
                      ধ্রুব hb_map_t& axes_old_index_tag_map)
  {
    যদি (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      প্রত্যাগম অসত্য;
    প্রতি (tuple_variations_t& vars: glyph_variations)
      যদি (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               সত্য, /* use shared points*/
                               সত্য,
                               &shared_tuples_idx_map,
			       &pool))
        প্রত্যাগম অসত্য;

    প্রত্যাগম সত্য;
  }

  তার্কিক compile_shared_tuples (ধ্রুব hb_map_t& axes_index_map,
                              ধ্রুব hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ধ্রুব hb_vector_t<F2DOT14>*, unsigned> coords_গুনতি_map;

    /* count the num of shared coords */
    প্রতি (tuple_variations_t& vars: glyph_variations)
    {
      প্রতি (tuple_delta_t& var : vars.tuple_vars)
      {
        যদি (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          প্রত্যাগম অসত্য;
        unsigned *গুনতি;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        যদি (coords_গুনতি_map.has_with_hash (&(var.compiled_peak_coords), hash, &গুনতি))
	  (*গুনতি)++;
        অন্যথা
          coords_গুনতি_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    যদি (!coords_গুনতি_map || coords_গুনতি_map.in_error ())
      প্রত্যাগম অসত্য;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ধ্রুব hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_গুনতি_map)
      | hb_filter ([] (ধ্রুব hb_pair_t<ধ্রুব hb_vector_t<F2DOT14>*, unsigned>& p) { প্রত্যাগম p.second > 1; })
    };
    যদি (unlikely (shared_coords.in_error ())) প্রত্যাগম অসত্য;

    /* no shared tuples: no coords are used more than once */
    যদি (!shared_coords) প্রত্যাগম সত্য;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_গুনতি = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_গুনতি * (shared_coords[0].first->length);
    যদি (unlikely (!compiled_shared_tuples.alloc (len)))
      প্রত্যাগম অসত্য;

    প্রতি (unsigned i = 0; i < shared_tuples_গুনতি; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      প্রতি (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    প্রত্যাগম সত্য;
  }

  স্থির পূর্ণাঙ্ক _cmp_coords (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
  {
    ধ্রুব hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ধ্রুব hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ধ্রুব hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ধ্রুব hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    যদি (a->second != b->second)
      প্রত্যাগম b->second - a->second; // high to low

    প্রত্যাগম b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 তার্কিক long_offset,
                                 unsigned অঙ্ক_glyphs,
                                 বর্ণ* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);

    যদি (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    অন্যথা
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    যদি (!cur_glyph) return_trace (অসত্য);
    প্রতি (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      যদি (long_offset)
        প্রতি (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      অন্যথা
        প্রতি (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      যদি (idx >= glyph_variations.length) return_trace (অসত্য);
      যদি (!cur_glyph->serialize (c, সত্য, glyph_variations[idx])) return_trace (অসত্য);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (বর্ণ *) next_glyph - (বর্ণ *) cur_glyph;

      যদি (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      অন্যথা
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    যদি (long_offset)
      প্রতি (; last_gid < অঙ্ক_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    অন্যথা
      প্রতি (; last_gid < অঙ্ক_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (সত্য);
  }
};

template <typename T, typename Types>
সংরচনা gvar_GVAR
{
  using Glyph_গুনতি_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_গুনতি_Type>;
  স্থির constexpr unsigned glyph_গুনতি_max ()
  { প্রত্যাগম (1u << (8 * Glyph_গুনতি_Type::static_size)) - 1; }

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.to_int () != 0; }

  তার্কিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, এতত্, axis_গুনতি_2 * shared_Tuple_গুনতি) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_অঙ্ক_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_অঙ্ক_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  { প্রত্যাগম sanitize_shallow (c); }

  তার্কিক decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_গুনতি_Type>& glyph_vars /* OUT */) ধ্রুব
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    প্রতি (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_গুনতি_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    যদি (new_gid_var_data_map.in_error ()) প্রত্যাগম অসত্য;

    hb_array_t<ধ্রুব F2DOT14> shared_tuples = (এতত্+sharedTuples).as_array ((unsigned) shared_Tuple_গুনতি * (unsigned) axis_গুনতি_2);
    প্রত্যাগম glyph_vars.create_from_glyphs_var_data (axis_গুনতি_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  ধ্রুব glyph_variations_t<Glyph_গুনতি_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_গুনতি,
                  unsigned অঙ্ক_glyphs,
                  তার্কিক force_long_offsets) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_গুনতি_2 = axis_গুনতি;
    out->glyph_গুনতি_X = hb_min (glyph_গুনতি_max (), অঙ্ক_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    তার্কিক long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (অঙ্ক_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, অসত্য);
    যদি (!glyph_var_data_offsets) return_trace (অসত্য);

    /* shared tuples */
    unsigned shared_tuple_গুনতি = glyph_vars.compiled_shared_tuples_গুনতি ();
    out->shared_Tuple_গুনতি = shared_tuple_গুনতি;

    যদি (!shared_tuple_গুনতি)
      out->sharedTuples = (বর্ণ *) glyph_var_data_offsets + glyph_var_data_offsets_size - (বর্ণ *) out;
    অন্যথা
    {
      hb_array_t<ধ্রুব F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      যদি (!shared_tuples.arrayZ) return_trace (অসত্য);
      out->sharedTuples = (ধ্রুব বর্ণ *) shared_tuples.arrayZ - (বর্ণ *) out;
    }

    বর্ণ *glyph_var_data = c->start_embed<বর্ণ> ();
    যদি (!glyph_var_data) return_trace (অসত্য);
    out->dataZ = glyph_var_data - (বর্ণ *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, অঙ্ক_glyphs,
                                                       (বর্ণ *) glyph_var_data_offsets));
  }

  তার্কিক instantiate (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    glyph_variations_t<Glyph_গুনতি_Type> glyph_vars;
    যদি (!decompile_glyph_variations (c, glyph_vars))
      return_trace (অসত্য);

    যদি (!glyph_vars.instantiate (c->plan)) return_trace (অসত্য);
    যদি (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (অসত্য);

    unsigned axis_গুনতি = c->plan->axes_index_map.get_population ();
    unsigned অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    তার্কিক force_long_offsets = অসত্য;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_গুনতি, অঙ্ক_glyphs, force_long_offsets));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned)
      return_trace (অসত্য);

    যদি (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_গুনতি = version.to_int () ? c->plan->source->get_অঙ্ক_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_গুনতি_2 = axis_গুনতি_2;
    out->shared_Tuple_গুনতি = shared_Tuple_গুনতি;

    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    out->glyph_গুনতি_X = hb_min (glyph_গুনতি_max (), অঙ্ক_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<বর্ণ>> culled_vars;
    hb_set_t culled_gids;
    যদি (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ধ্রুব F2DOT14> shared_tuples_array =
	  (এতত্+sharedTuples).as_array ((unsigned) shared_Tuple_গুনতি * (unsigned) axis_গুনতি_2);
      যদি (unlikely (!culled_vars.resize (অঙ্ক_glyphs))) return_trace (অসত্য);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      যদি (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      প্রতি (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_গুনতি, _.second);
	যদি (var_data_bytes.length < GlyphVariationData::min_size) অনুবর্ত;
	তার্কিক changed = অসত্য;
	যদি (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_গুনতি_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (অসত্য);
	যদি (changed) culled_gids.add (_.first);
      }
      যদি (unlikely (culled_gids.in_error ())) return_trace (অসত্য);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    প্রতি (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_গুনতি, old_gid).length;
      যদি (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      যদি (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (অসত্য);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    তার্কিক long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (অঙ্ক_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, অসত্য);
    যদি (!subset_offsets) return_trace (অসত্য);

    /* shared tuples */
    যদি (!shared_Tuple_গুনতি || !sharedTuples)
      out->sharedTuples = (বর্ণ *) subset_offsets + subset_offsets_size - (বর্ণ *) out;
    অন্যথা
    {
      unsigned পূর্ণাঙ্ক shared_tuple_size = F2DOT14::static_size * axis_গুনতি_2 * shared_Tuple_গুনতি;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      যদি (!tuples) return_trace (অসত্য);
      out->sharedTuples = (বর্ণ *) tuples - (বর্ণ *) out;
      hb_memcpy (tuples, এতত্+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    যদি (long_offset)
      subset_data_size -= padding_size;
    বর্ণ *subset_data = c->serializer->allocate_size<বর্ণ> (subset_data_size, অসত্য);
    যদি (!subset_data) return_trace (অসত্য);
    out->dataZ = subset_data - (বর্ণ *) out;


    যদি (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    অন্যথা
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned পূর্ণাঙ্ক glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    প্রতি (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      যদি (long_offset)
	প্রতি (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      অন্যথা
	প্রতি (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_গুনতি,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      যদি (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      যদি (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      অন্যথা
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    যদি (long_offset)
      প্রতি (; last < অঙ্ক_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    অন্যথা
      প্রতি (; last < অঙ্ক_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (সত্য);
  }

  রক্ষিত:
  ধ্রুব hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_গুনতি,
					     hb_codepoint_t glyph) ধ্রুব
  {
    unsigned start_offset = get_offset (glyph_গুনতি, glyph);
    unsigned end_offset = get_offset (glyph_গুনতি, glyph+1);
    যদি (unlikely (end_offset < start_offset)) প্রত্যাগম hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    প্রত্যাগম likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  তার্কিক is_long_offset () ধ্রুব { প্রত্যাগম flags & 1; }

  unsigned get_offset (unsigned glyph_গুনতি, unsigned i) ধ্রুব
  {
    যদি (unlikely (i > glyph_গুনতি)) প্রত্যাগম 0;
    hb_barrier ();
    প্রত্যাগম is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ধ্রুব HBUINT32 * get_long_offset_array () ধ্রুব { প্রত্যাগম (ধ্রুব HBUINT32 *) &offsetZ; }
  ধ্রুব HBUINT16 *get_short_offset_array () ধ্রুব { প্রত্যাগম (ধ্রুব HBUINT16 *) &offsetZ; }

  সার্বজনিক:
  সংরচনা accelerator_t
  {

    hb_scalar_cache_t *create_cache () ধ্রুব
    {
      প্রত্যাগম hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_গুনতি,
						+TupleVariationHeader::max_shared_tuple_গুনতি));
    }

    স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    তার্কিক has_data () ধ্রুব { প্রত্যাগম table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_গুনতি_2 = table->version.to_int () ? face->get_অঙ্ক_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    গুপ্ত:

    স্থির ভিন্ন infer_delta (ধ্রুব hb_array_t<contour_point_t> points,
			      ধ্রুব hb_array_t<contour_point_t> deltas,
			      unsigned পূর্ণাঙ্ক target, unsigned পূর্ণাঙ্ক prev, unsigned পূর্ণাঙ্ক next,
			      ভিন্ন contour_point_t::*m)
    {
      ভিন্ন target_val = points.arrayZ[target].*m;
      ভিন্ন prev_val = points.arrayZ[prev].*m;
      ভিন্ন next_val = points.arrayZ[next].*m;
      ভিন্ন prev_delta =  deltas.arrayZ[prev].*m;
      ভিন্ন next_delta =  deltas.arrayZ[next].*m;

      যদি (prev_val == next_val)
	প্রত্যাগম (prev_delta == next_delta) ? prev_delta : 0.f;
      অন্যথা যদি (target_val <= hb_min (prev_val, next_val))
	প্রত্যাগম (prev_val < next_val) ? prev_delta : next_delta;
      অন্যথা যদি (target_val >= hb_max (prev_val, next_val))
	প্রত্যাগম (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      ভিন্ন r = (target_val - prev_val) / (next_val - prev_val);
      প্রত্যাগম prev_delta + r * (next_delta - prev_delta);
    }

    স্থির unsigned পূর্ণাঙ্ক next_index (unsigned পূর্ণাঙ্ক i, unsigned পূর্ণাঙ্ক start, unsigned পূর্ণাঙ্ক end)
    { প্রত্যাগম (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <তার্কিক is_x>
#endif
    স্থির তার্কিক decompile_deltas_add_to_points (ধ্রুব HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						ভিন্ন scalar,
						ধ্রুব HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, তার্কিক is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned গুনতি = points.length;
      যাবত্ (i < গুনতি)
      {
	যদি (unlikely (p + 1 > end)) প্রত্যাগম অসত্য;
	unsigned control = *p++;
	unsigned run_গুনতি = (control & Tuple_মানসমূহ::মান_RUN_গুনতি_MASK) + 1;
	unsigned stop = i + run_গুনতি;
	যদি (unlikely (stop > গুনতি)) প্রত্যাগম অসত্য;

	unsigned skip = i < start ? hb_min (start - i, run_গুনতি) : 0;
	i += skip;

	বিকল্প (control & Tuple_মানসমূহ::মানসমূহ_SIZE_MASK)
	{
	  পক্ষ Tuple_মানসমূহ::মানসমূহ_ARE_ZEROS:
	    i = stop;
	    ভঙ্গ;
	  পক্ষ Tuple_মানসমূহ::মানসমূহ_ARE_শব্দগুলি:
	  {
	    যদি (unlikely (p + run_গুনতি * HBINT16::static_size > end)) প্রত্যাগম অসত্য;
	    p += skip * HBINT16::static_size;
	    ধ্রুব auto *pp = (ধ্রুব HBINT16 *) p;
	    প্রতি (; i < stop; i++)
	    {
	      ভিন্ন v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুব HBUINT8 *) pp;
	  }
	  ভঙ্গ;
	  পক্ষ Tuple_মানসমূহ::মানসমূহ_ARE_LONGS:
	  {
	    যদি (unlikely (p + run_গুনতি * HBINT32::static_size > end)) প্রত্যাগম অসত্য;
	    p += skip * HBINT32::static_size;
	    ধ্রুব auto *pp = (ধ্রুব HBINT32 *) p;
	    প্রতি (; i < stop; i++)
	    {
	      ভিন্ন v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুব HBUINT8 *) pp;
	  }
	  ভঙ্গ;
	  পক্ষ Tuple_মানসমূহ::মানসমূহ_ARE_BYTES:
	  {
	    যদি (unlikely (p + run_গুনতি > end)) প্রত্যাগম অসত্য;
	    p += skip * HBINT8::static_size;
	    ধ্রুব auto *pp = (ধ্রুব HBINT8 *) p;
	    প্রতি (; i < stop; i++)
	    {
	      ভিন্ন v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুব HBUINT8 *) pp;
	  }
	  ভঙ্গ;
	}
      }
      প্রত্যাগম সত্য;
    }
    সার্বজনিক:
    তার্কিক apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
				 ধ্রুব hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 তার্কিক phantom_only = অসত্য,
				 int64_t *budget = nullptr) ধ্রুব
    {
      যদি (unlikely (glyph >= glyph_গুনতি_2)) প্রত্যাগম সত্য;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_গুনতি_2, glyph);
      যদি (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) প্রত্যাগম সত্য;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      যদি (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_গুনতি_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	প্রত্যাগম সত্য; /* so isn't applied at all */

      তার্কিক any_private_points = অসত্য;
      তার্কিক private_points_checked = অসত্য;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned অঙ্ক_coords = table->axis_গুনতি_2;
      hb_array_t<ধ্রুব F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_গুনতি * অঙ্ক_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned গুনতি = points.length;
      তার্কিক flush = অসত্য;

      কুরু
      {
	ভিন্ন scalar = iterator.current_tuple->calculate_scalar (coords, অঙ্ক_coords, shared_tuples,
								 scalar_cache);

	যদি (scalar == 0.f) অনুবর্ত;

	যদি (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && গুনতি >= 4 ? 4 : গুনতি)))
	  প্রত্যাগম অসত্য;

	যদি (!private_points_checked)
	{
	  auto scan = iterator;
	  কুরু
	  {
	    যদি (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = সত্য;
	      ভঙ্গ;
	    }
	  } যাবত্ (scan.move_to_next ());
	  private_points_checked = সত্য;
	}
	ধ্রুব HBUINT8 *p = iterator.get_serialized_data ();
	unsigned পূর্ণাঙ্ক length = iterator.current_tuple->get_data_size ();
	যদি (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  প্রত্যাগম অসত্য;

	যদি (!deltas)
	{
	  যদি (unlikely (!deltas_vec.resize_dirty  (গুনতি))) প্রত্যাগম অসত্য;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? গুনতি - 4 : 0), 0,
		     (phantom_only ? 4 : গুনতি) * পরিমাণ (deltas[0]));
	}

	ধ্রুব HBUINT8 *end = p + length;

	তার্কিক has_private_points = iterator.current_tuple->has_private_points ();
	যদি (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  প্রত্যাগম অসত্য;
	ধ্রুব hb_array_t<unsigned পূর্ণাঙ্ক> &indices = has_private_points ? private_indices : shared_indices;

	তার্কিক apply_to_all = (indices.length == 0);
	unsigned অঙ্ক_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && অঙ্ক_deltas >= 4 ? অঙ্ক_deltas - 4 : 0);

	যদি (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  যদি (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, সত্য))) প্রত্যাগম অসত্য;
	  যদি (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, অসত্য))) প্রত্যাগম অসত্য;
#অন্যথা
	  যদি (unlikely (!decompile_deltas_add_to_points<সত্য> (p, points, scalar, end, start_deltas))) প্রত্যাগম অসত্য;
	  যদি (unlikely (!decompile_deltas_add_to_points<অসত্য> (p, points, scalar, end, start_deltas))) প্রত্যাগম অসত্য;
#endif
	  অনুবর্ত;
	}

	যদি (unlikely (!x_deltas.resize_dirty  (অঙ্ক_deltas))) প্রত্যাগম অসত্য;
	যদি (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, অসত্য, start_deltas))) প্রত্যাগম অসত্য;
	যদি (unlikely (!y_deltas.resize_dirty  (অঙ্ক_deltas))) প্রত্যাগম অসত্য;
	যদি (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, অসত্য, start_deltas))) প্রত্যাগম অসত্য;

	যদি (!apply_to_all)
	{
	  যদি (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    যদি (unlikely (orig_points_vec.in_error ())) প্রত্যাগম অসত্য;
	    orig_points = orig_points_vec.as_array ();
	  }

	  যদি (flush)
	  {
	    প্রতি (unsigned পূর্ণাঙ্ক i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? গুনতি - 4 : 0), 0,
		     (phantom_only ? 4 : গুনতি) * পরিমাণ (deltas[0]));
	}

	যদি (HB_OPTIMIZE_SIZE_VAL)
	{
	  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_deltas; i++)
	  {
	    unsigned পূর্ণাঙ্ক pt_index;
	    যদি (apply_to_all)
	      pt_index = i;
	    অন্যথা
	    {
	      pt_index = indices[i];
	      যদি (unlikely (pt_index >= deltas.length)) অনুবর্ত;
	    }
	    যদি (phantom_only && pt_index < গুনতি - 4) অনুবর্ত;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	অন্যথা
	{
	  /* Ouch. Four cases... for optimization. */
	  যদি (scalar != 1.0f)
	  {
	    যদি (apply_to_all)
	      প্রতি (unsigned পূর্ণাঙ্ক i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    অন্যথা
	      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_deltas; i++)
	      {
		unsigned পূর্ণাঙ্ক pt_index = indices[i];
		যদি (unlikely (pt_index >= deltas.length)) অনুবর্ত;
		যদি (phantom_only && pt_index < গুনতি - 4) অনুবর্ত;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  অন্যথা
	  {
	    যদি (apply_to_all)
	      প্রতি (unsigned পূর্ণাঙ্ক i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    অন্যথা
	      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_deltas; i++)
	      {
		unsigned পূর্ণাঙ্ক pt_index = indices[i];
		যদি (unlikely (pt_index >= deltas.length)) অনুবর্ত;
		যদি (phantom_only && pt_index < গুনতি - 4) অনুবর্ত;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	যদি (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  যাবত্ (সত্য)
	  {
	    যাবত্ (end_point < গুনতি && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    যদি (unlikely (end_point == গুনতি)) ভঙ্গ;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_গুনতি = 0;
	    প্রতি (unsigned i = start_point; i < end_point + 1; i++)
	      unref_গুনতি += deltas.arrayZ[i].flag;
	    unref_গুনতি = (end_point - start_point + 1) - unref_গুনতি;

	    unsigned j = start_point;
	    যদি (unref_গুনতি == 0 || unref_গুনতি > end_point - start_point)
	      গচ্ছ no_more_gaps;

	    প্রতি (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned পূর্ণাঙ্ক prev, next, i;
	      প্রতি (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		যদি (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) ভঙ্গ;
	      }
	      prev = j = i;
	      প্রতি (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		যদি (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) ভঙ্গ;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      প্রতি (;;)
	      {
		i = next_index (i, start_point, end_point);
		যদি (i == next) ভঙ্গ;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		যদি (--unref_গুনতি == 0) গচ্ছ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = সত্য;

      } যাবত্ (iterator.move_to_next ());

      যদি (flush)
      {
	প্রতি (unsigned পূর্ণাঙ্ক i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      প্রত্যাগম সত্য;
    }

    unsigned পূর্ণাঙ্ক get_axis_গুনতি () ধ্রুব { প্রত্যাগম table->axis_গুনতি_2; }

    গুপ্ত:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_গুনতি_2;
  };

  রক্ষিত:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_গুনতি_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_গুনতি;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_গুনতি_Type
		glyph_গুনতি_X;	/* The number of glyphs in this font. This must match the number of
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
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (18 + Glyph_গুনতি_Type::static_size, offsetZ);
};

সংরচনা gvar : gvar_GVAR<gvar, SmallTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

সংরচনা GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

সংরচনা gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ধ্রুব
  {
    যদি (has_data ()) প্রত্যাগম HB_DUAL_GET (*এতত্, create_cache ());
    প্রত্যাগম nullptr;
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  তার্কিক apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
			       ধ্রুব hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       তার্কিক phantom_only = অসত্য,
			       int64_t *budget = nullptr) ধ্রুব
  {
    প্রত্যাগম HB_DUAL_GET (*এতত্, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned পূর্ণাঙ্ক get_axis_গুনতি () ধ্রুব
  {
    যদি (has_data ()) প্রত্যাগম HB_DUAL_GET (*এতত্, get_axis_গুনতি ());
    প্রত্যাগম 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
