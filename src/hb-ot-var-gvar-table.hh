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

কাঠামো hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<পূর্ণসংখ্যা> x_deltas;
  hb_vector_t<পূর্ণসংখ্যা> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned পূর্ণসংখ্যা> shared_indices;
  hb_vector_t<unsigned পূর্ণসংখ্যা> private_indices;
};

নামাকাশ OT {

template <typename OffsetType>
কাঠামো glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename Tuple_Variation_তথ্য<OffsetType>::tuple_variations_t;
  using Glyph_Variation_তথ্য = Tuple_Variation_তথ্য<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  ব্যক্তিগত:
  unsigned shared_tuples_গুনতি = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  সর্বজনীন:
  unsigned compiled_shared_tuples_গুনতি () ধ্রুবক
  { ফেরত shared_tuples_গুনতি; }

  unsigned compiled_byte_আকার () ধ্রুবক
  {
    unsigned byte_আকার = 0;
    জন্য (ধ্রুবক auto& _ : glyph_variations)
      byte_আকার += _.get_compiled_byte_আকার ();

    ফেরত byte_আকার;
  }

  যৌক্তিক create_from_glyphs_var_তথ্য (unsigned axis_গুনতি,
                                    ধ্রুবক hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
                                    ধ্রুবক hb_subset_plan_t *plan,
                                    ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_তথ্য_map)
  {
    যদি (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.দৈর্ঘ্য)))
      ফেরত মিথ্যা;

    auto it = hb_iter (plan->new_to_old_gid_list);
    জন্য (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      যদি (!new_gid_var_তথ্য_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        ফেরত মিথ্যা;
      hb_bytes_t var_তথ্য = new_gid_var_তথ্য_map.get (new_gid);

      ধ্রুবক Glyph_Variation_তথ্য* p = reinterpret_cast<ধ্রুবক Glyph_Variation_তথ্য*> (var_তথ্য.arrayZ);
      typename Glyph_Variation_তথ্য::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      যদি (!var_তথ্য || ! p->has_তথ্য () || !all_contour_points->দৈর্ঘ্য ||
          !Glyph_Variation_তথ্য::get_tuple_iterator (var_তথ্য, axis_গুনতি,
                                                   var_তথ্য.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        চালু;
      }

      যৌক্তিক is_composite_glyph = মিথ্যা;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      যদি (!p->decompile_tuple_variations (all_contour_points->দৈর্ঘ্য, সত্য /* is_gvar */,
                                          iterator, &(plan->axes_old_সূচক_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        ফেরত মিথ্যা;
      glyph_variations.push (std::move (tuple_vars));
    }
    ফেরত !glyph_variations.in_error () && glyph_variations.দৈর্ঘ্য == plan->new_to_old_gid_list.দৈর্ঘ্য;
  }

  যৌক্তিক instantiate (ধ্রুবক hb_subset_plan_t *plan)
  {
    unsigned গুনতি = plan->new_to_old_gid_list.দৈর্ঘ্য;
    যৌক্তিক iup_optimize = মিথ্যা;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      যদি (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        ফেরত মিথ্যা;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      যদি (iup_optimize)
      {
        points = *all_points;
        যদি (unlikely (points.in_error ())) ফেরত মিথ্যা;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      যদি (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      যদি (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  যৌক্তিক compile_bytes (ধ্রুবক hb_map_t& axes_সূচক_map,
                      ধ্রুবক hb_map_t& axes_old_সূচক_tag_map)
  {
    যদি (!compile_shared_tuples (axes_সূচক_map, axes_old_সূচক_tag_map))
      ফেরত মিথ্যা;
    জন্য (tuple_variations_t& vars: glyph_variations)
      যদি (!vars.compile_bytes (axes_সূচক_map, axes_old_সূচক_tag_map,
                               সত্য, /* use shared points*/
                               সত্য,
                               &shared_tuples_idx_map,
			       &pool))
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক compile_shared_tuples (ধ্রুবক hb_map_t& axes_সূচক_map,
                              ধ্রুবক hb_map_t& axes_old_সূচক_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned> coords_গুনতি_map;

    /* count the num of shared coords */
    জন্য (tuple_variations_t& vars: glyph_variations)
    {
      জন্য (tuple_delta_t& var : vars.tuple_vars)
      {
        যদি (!var.compile_coords (axes_সূচক_map, axes_old_সূচক_tag_map, &pool))
          ফেরত মিথ্যা;
        unsigned *গুনতি;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        যদি (coords_গুনতি_map.has_with_hash (&(var.compiled_peak_coords), hash, &গুনতি))
	  (*গুনতি)++;
        অন্যথা
          coords_গুনতি_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    যদি (!coords_গুনতি_map || coords_গুনতি_map.in_error ())
      ফেরত মিথ্যা;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_গুনতি_map)
      | hb_filter ([] (ধ্রুবক hb_pair_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned>& p) { ফেরত p.second > 1; })
    };
    যদি (unlikely (shared_coords.in_error ())) ফেরত মিথ্যা;

    /* no shared tuples: no coords are used more than once */
    যদি (!shared_coords) ফেরত সত্য;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_গুনতি = hb_min (0xFFFu + 1, shared_coords.দৈর্ঘ্য);
    unsigned len = shared_tuples_গুনতি * (shared_coords[0].first->দৈর্ঘ্য);
    যদি (unlikely (!compiled_shared_tuples.alloc (len)))
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < shared_tuples_গুনতি; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      জন্য (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    ফেরত সত্য;
  }

  স্থির পূর্ণসংখ্যা _cmp_coords (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ধ্রুবক hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ধ্রুবক hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ধ্রুবক hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    যদি (a->second != b->second)
      ফেরত b->second - a->second; // high to low

    ফেরত b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_glyph_var_তথ্য (hb_serialize_context_t *c,
                                 Iterator it,
                                 যৌক্তিক long_offset,
                                 unsigned অঙ্ক_glyphs,
                                 অক্ষর* glyph_var_তথ্য_offsets /* OUT: glyph var data offsets array */) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    যদি (long_offset)
    {
      ((HBUINT32 *) glyph_var_তথ্য_offsets)[0] = 0;
      glyph_var_তথ্য_offsets += 4;
    }
    অন্যথা
    {
      ((HBUINT16 *) glyph_var_তথ্য_offsets)[0] = 0;
      glyph_var_তথ্য_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    Glyph_Variation_তথ্য* cur_glyph = c->start_embed<Glyph_Variation_তথ্য> ();
    যদি (!cur_glyph) return_trace (মিথ্যা);
    জন্য (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      যদি (long_offset)
        জন্য (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_তথ্য_offsets)[last_gid] = glyph_offset;
      অন্যথা
        জন্য (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_তথ্য_offsets)[last_gid] = glyph_offset / 2;

      যদি (idx >= glyph_variations.দৈর্ঘ্য) return_trace (মিথ্যা);
      যদি (!cur_glyph->serialize (c, সত্য, glyph_variations[idx])) return_trace (মিথ্যা);
      Glyph_Variation_তথ্য* পরবর্তী_glyph = c->start_embed<Glyph_Variation_তথ্য> ();
      glyph_offset += (অক্ষর *) পরবর্তী_glyph - (অক্ষর *) cur_glyph;

      যদি (long_offset)
        ((HBUINT32 *) glyph_var_তথ্য_offsets)[gid] = glyph_offset;
      অন্যথা
        ((HBUINT16 *) glyph_var_তথ্য_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = পরবর্তী_glyph;
    }

    যদি (long_offset)
      জন্য (; last_gid < অঙ্ক_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_তথ্য_offsets)[last_gid] = glyph_offset;
    অন্যথা
      জন্য (; last_gid < অঙ্ক_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_তথ্য_offsets)[last_gid] = glyph_offset / 2;
    return_trace (সত্য);
  }
};

template <typename T, typename Types>
কাঠামো gvar_GVAR
{
  using Glyph_গুনতি_Type = typename Types::HBUINT;

  using Glyph_Variation_তথ্য = Tuple_Variation_তথ্য<Glyph_গুনতি_Type>;
  স্থির constexpr unsigned glyph_গুনতি_max ()
  { ফেরত (1u << (8 * Glyph_গুনতি_Type::static_আকার)) - 1; }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int () != 0; }

  যৌক্তিক sanitize_shallow (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, এই, axis_গুনতি_2 * shared_Tuple_গুনতি) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_অঙ্ক_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_অঙ্ক_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  { ফেরত sanitize_shallow (c); }

  যৌক্তিক decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_গুনতি_Type>& glyph_vars /* OUT */) ধ্রুবক
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_তথ্য_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_তথ্য_map.set (0, hb_bytes_t ());
      it++;
    }

    জন্য (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_তথ্য_bytes = get_glyph_var_তথ্য_bytes (c->source_blob, glyph_গুনতি_X, old_gid);
      new_gid_var_তথ্য_map.set (new_gid, var_তথ্য_bytes);
    }

    যদি (new_gid_var_তথ্য_map.in_error ()) ফেরত মিথ্যা;

    hb_array_t<ধ্রুবক F2DOT14> shared_tuples = (এই+sharedTuples).as_array ((unsigned) shared_Tuple_গুনতি * (unsigned) axis_গুনতি_2);
    ফেরত glyph_vars.create_from_glyphs_var_তথ্য (axis_গুনতি_2, shared_tuples, c->plan, new_gid_var_তথ্য_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  ধ্রুবক glyph_variations_t<Glyph_গুনতি_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_গুনতি,
                  unsigned অঙ্ক_glyphs,
                  যৌক্তিক force_long_offsets) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_গুনতি_2 = axis_গুনতি;
    out->glyph_গুনতি_X = hb_min (glyph_গুনতি_max (), অঙ্ক_glyphs);

    unsigned glyph_var_তথ্য_আকার = glyph_vars.compiled_byte_আকার ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    যৌক্তিক long_offset = glyph_var_তথ্য_আকার > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_তথ্য_offsets_আকার = (long_offset ? 4 : 2) * (অঙ্ক_glyphs + 1);
    HBUINT8 *glyph_var_তথ্য_offsets = c->allocate_আকার<HBUINT8> (glyph_var_তথ্য_offsets_আকার, মিথ্যা);
    যদি (!glyph_var_তথ্য_offsets) return_trace (মিথ্যা);

    /* shared tuples */
    unsigned shared_tuple_গুনতি = glyph_vars.compiled_shared_tuples_গুনতি ();
    out->shared_Tuple_গুনতি = shared_tuple_গুনতি;

    যদি (!shared_tuple_গুনতি)
      out->sharedTuples = (অক্ষর *) glyph_var_তথ্য_offsets + glyph_var_তথ্য_offsets_আকার - (অক্ষর *) out;
    অন্যথা
    {
      hb_array_t<ধ্রুবক F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      যদি (!shared_tuples.arrayZ) return_trace (মিথ্যা);
      out->sharedTuples = (ধ্রুবক অক্ষর *) shared_tuples.arrayZ - (অক্ষর *) out;
    }

    অক্ষর *glyph_var_তথ্য = c->start_embed<অক্ষর> ();
    যদি (!glyph_var_তথ্য) return_trace (মিথ্যা);
    out->তথ্য_Z = glyph_var_তথ্য - (অক্ষর *) out;

    return_trace (glyph_vars.serialize_glyph_var_তথ্য (c, it, long_offset, অঙ্ক_glyphs,
                                                       (অক্ষর *) glyph_var_তথ্য_offsets));
  }

  যৌক্তিক instantiate (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    glyph_variations_t<Glyph_গুনতি_Type> glyph_vars;
    যদি (!decompile_glyph_variations (c, glyph_vars))
      return_trace (মিথ্যা);

    যদি (!glyph_vars.instantiate (c->plan)) return_trace (মিথ্যা);
    যদি (!glyph_vars.compile_bytes (c->plan->axes_সূচক_map, c->plan->axes_old_সূচক_tag_map))
      return_trace (মিথ্যা);

    unsigned axis_গুনতি = c->plan->axes_সূচক_map.get_population ();
    unsigned অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    যৌক্তিক force_long_offsets = মিথ্যা;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_গুনতি, অঙ্ক_glyphs, force_long_offsets));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned)
      return_trace (মিথ্যা);

    যদি (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_গুনতি = version.to_int () ? c->plan->source->get_অঙ্ক_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_গুনতি_2 = axis_গুনতি_2;
    out->shared_Tuple_গুনতি = shared_Tuple_গুনতি;

    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    out->glyph_গুনতি_X = hb_min (glyph_গুনতি_max (), অঙ্ক_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<অক্ষর>> culled_vars;
    hb_set_t culled_gids;
    যদি (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ধ্রুবক F2DOT14> shared_tuples_array =
	  (এই+sharedTuples).as_array ((unsigned) shared_Tuple_গুনতি * (unsigned) axis_গুনতি_2);
      যদি (unlikely (!culled_vars.resize (অঙ্ক_glyphs))) return_trace (মিথ্যা);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      যদি (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      জন্য (auto &_ : cull_it)
      {
	hb_bytes_t var_তথ্য_bytes = get_glyph_var_তথ্য_bytes (c->source_blob, glyph_গুনতি, _.second);
	যদি (var_তথ্য_bytes.দৈর্ঘ্য < Glyph_Variation_তথ্য::min_আকার) চালু;
	যৌক্তিক changed = মিথ্যা;
	যদি (unlikely (!var_তথ্য_bytes.as<Glyph_Variation_তথ্য> ()
		      ->cull_tuple_variations (var_তথ্য_bytes, axis_গুনতি_2,
					       shared_tuples_array,
					       &c->plan->axes_old_সূচক_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (মিথ্যা);
	যদি (changed) culled_gids.add (_.first);
      }
      যদি (unlikely (culled_gids.in_error ())) return_trace (মিথ্যা);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_তথ্য_আকার = 0;
    unsigned padding_আকার = 0;
    জন্য (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_তথ্য_আকার = culled_gids.has (_.first)
				 ? culled_vars[_.first].দৈর্ঘ্য
				 : get_glyph_var_তথ্য_bytes (c->source_blob, glyph_গুনতি, old_gid).দৈর্ঘ্য;
      যদি (glyph_তথ্য_আকার % 2)
      {
        glyph_তথ্য_আকার++;
        padding_আকার++;
      }

      যদি (unlikely (hb_unsigned_add_overflows (subset_তথ্য_আকার, glyph_তথ্য_আকার, &subset_তথ্য_আকার)))
	return_trace (মিথ্যা);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    যৌক্তিক long_offset = subset_তথ্য_আকার > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_আকার = (long_offset ? 4 : 2) * (অঙ্ক_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_আকার<HBUINT8> (subset_offsets_আকার, মিথ্যা);
    যদি (!subset_offsets) return_trace (মিথ্যা);

    /* shared tuples */
    যদি (!shared_Tuple_গুনতি || !sharedTuples)
      out->sharedTuples = (অক্ষর *) subset_offsets + subset_offsets_আকার - (অক্ষর *) out;
    অন্যথা
    {
      unsigned পূর্ণসংখ্যা shared_tuple_আকার = F2DOT14::static_আকার * axis_গুনতি_2 * shared_Tuple_গুনতি;
      F2DOT14 *tuples = c->serializer->allocate_আকার<F2DOT14> (shared_tuple_আকার);
      যদি (!tuples) return_trace (মিথ্যা);
      out->sharedTuples = (অক্ষর *) tuples - (অক্ষর *) out;
      hb_memcpy (tuples, এই+sharedTuples, shared_tuple_আকার);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    যদি (long_offset)
      subset_তথ্য_আকার -= padding_আকার;
    অক্ষর *subset_তথ্য = c->serializer->allocate_আকার<অক্ষর> (subset_তথ্য_আকার, মিথ্যা);
    যদি (!subset_তথ্য) return_trace (মিথ্যা);
    out->তথ্য_Z = subset_তথ্য - (অক্ষর *) out;


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
    unsigned পূর্ণসংখ্যা glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    যদি (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    জন্য (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      যদি (long_offset)
	জন্য (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      অন্যথা
	জন্য (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_তথ্য_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].দৈর্ঘ্য)
				  : get_glyph_var_তথ্য_bytes (c->source_blob,
							      glyph_গুনতি,
							      old_gid);

      hb_memcpy (subset_তথ্য, var_তথ্য_bytes.arrayZ, var_তথ্য_bytes.দৈর্ঘ্য);
      unsigned glyph_তথ্য_আকার = var_তথ্য_bytes.দৈর্ঘ্য;
      subset_তথ্য += glyph_তথ্য_আকার;
      glyph_offset += glyph_তথ্য_আকার;

      যদি (!long_offset && (glyph_তথ্য_আকার % 2))
      {
        *subset_তথ্য = 0;
        subset_তথ্য++;
        glyph_offset++;
      }

      যদি (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      অন্যথা
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    যদি (long_offset)
      জন্য (; last < অঙ্ক_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    অন্যথা
      জন্য (; last < অঙ্ক_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (সত্য);
  }

  সুরক্ষিত:
  ধ্রুবক hb_bytes_t get_glyph_var_তথ্য_bytes (hb_blob_t *blob,
					     unsigned glyph_গুনতি,
					     hb_codepoint_t glyph) ধ্রুবক
  {
    unsigned start_offset = get_offset (glyph_গুনতি, glyph);
    unsigned end_offset = get_offset (glyph_গুনতি, glyph+1);
    যদি (unlikely (end_offset < start_offset)) ফেরত hb_bytes_t ();
    unsigned দৈর্ঘ্য = end_offset - start_offset;
    hb_bytes_t var_তথ্য = blob->as_bytes ().sub_array (((unsigned) তথ্য_Z) + start_offset, দৈর্ঘ্য);
    ফেরত likely (var_তথ্য.দৈর্ঘ্য >= Glyph_Variation_তথ্য::min_আকার) ? var_তথ্য : hb_bytes_t ();
  }

  যৌক্তিক is_long_offset () ধ্রুবক { ফেরত flags & 1; }

  unsigned get_offset (unsigned glyph_গুনতি, unsigned i) ধ্রুবক
  {
    যদি (unlikely (i > glyph_গুনতি)) ফেরত 0;
    hb_barrier ();
    ফেরত is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ধ্রুবক HBUINT32 * get_long_offset_array () ধ্রুবক { ফেরত (ধ্রুবক HBUINT32 *) &offsetZ; }
  ধ্রুবক HBUINT16 *get_short_offset_array () ধ্রুবক { ফেরত (ধ্রুবক HBUINT16 *) &offsetZ; }

  সর্বজনীন:
  কাঠামো accelerator_t
  {

    hb_scalar_cache_t *create_cache () ধ্রুবক
    {
      ফেরত hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_গুনতি,
						+TupleVariationHeader::max_shared_tuple_গুনতি));
    }

    স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত table->has_তথ্য (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_গুনতি_2 = table->version.to_int () ? face->get_অঙ্ক_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    ব্যক্তিগত:

    স্থির দশমিক infer_delta (ধ্রুবক hb_array_t<contour_point_t> points,
			      ধ্রুবক hb_array_t<contour_point_t> deltas,
			      unsigned পূর্ণসংখ্যা target, unsigned পূর্ণসংখ্যা prev, unsigned পূর্ণসংখ্যা next,
			      দশমিক contour_point_t::*m)
    {
      দশমিক target_val = points.arrayZ[target].*m;
      দশমিক পূর্ব_val = points.arrayZ[prev].*m;
      দশমিক পরবর্তী_val = points.arrayZ[next].*m;
      দশমিক পূর্ব_delta =  deltas.arrayZ[prev].*m;
      দশমিক পরবর্তী_delta =  deltas.arrayZ[next].*m;

      যদি (পূর্ব_val == পরবর্তী_val)
	ফেরত (পূর্ব_delta == পরবর্তী_delta) ? পূর্ব_delta : 0.f;
      অন্যথা যদি (target_val <= hb_min (পূর্ব_val, পরবর্তী_val))
	ফেরত (পূর্ব_val < পরবর্তী_val) ? পূর্ব_delta : পরবর্তী_delta;
      অন্যথা যদি (target_val >= hb_max (পূর্ব_val, পরবর্তী_val))
	ফেরত (পূর্ব_val > পরবর্তী_val) ? পূর্ব_delta : পরবর্তী_delta;

      /* linear interpolation */
      দশমিক r = (target_val - পূর্ব_val) / (পরবর্তী_val - পূর্ব_val);
      ফেরত পূর্ব_delta + r * (পরবর্তী_delta - পূর্ব_delta);
    }

    স্থির unsigned পূর্ণসংখ্যা পরবর্তী_সূচক (unsigned পূর্ণসংখ্যা i, unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
    { ফেরত (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_আকার
    template <যৌক্তিক is_x>
#endif
    স্থির যৌক্তিক decompile_deltas_add_to_points (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						দশমিক scalar,
						ধ্রুবক HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_আকার
						, যৌক্তিক is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned গুনতি = points.দৈর্ঘ্য;
      যখন (i < গুনতি)
      {
	যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
	unsigned control = *p++;
	unsigned run_গুনতি = (control & Tuple_মানসমূহ::মান_RUN_গুনতি_MASK) + 1;
	unsigned stop = i + run_গুনতি;
	যদি (unlikely (stop > গুনতি)) ফেরত মিথ্যা;

	unsigned skip = i < start ? hb_min (start - i, run_গুনতি) : 0;
	i += skip;

	নির্বাচন (control & Tuple_মানসমূহ::মানসমূহ_আকার_MASK)
	{
	  ক্ষেত্র Tuple_মানসমূহ::মানসমূহ_ARE_ZEROS:
	    i = stop;
	    বিরতি;
	  ক্ষেত্র Tuple_মানসমূহ::মানসমূহ_ARE_শব্দগুলি:
	  {
	    যদি (unlikely (p + run_গুনতি * HBINT16::static_আকার > end)) ফেরত মিথ্যা;
	    p += skip * HBINT16::static_আকার;
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT16 *) p;
	    জন্য (; i < stop; i++)
	    {
	      দশমিক v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুবক HBUINT8 *) pp;
	  }
	  বিরতি;
	  ক্ষেত্র Tuple_মানসমূহ::মানসমূহ_ARE_LONGS:
	  {
	    যদি (unlikely (p + run_গুনতি * HBINT32::static_আকার > end)) ফেরত মিথ্যা;
	    p += skip * HBINT32::static_আকার;
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT32 *) p;
	    জন্য (; i < stop; i++)
	    {
	      দশমিক v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুবক HBUINT8 *) pp;
	  }
	  বিরতি;
	  ক্ষেত্র Tuple_মানসমূহ::মানসমূহ_ARE_BYTES:
	  {
	    যদি (unlikely (p + run_গুনতি > end)) ফেরত মিথ্যা;
	    p += skip * HBINT8::static_আকার;
	    ধ্রুবক auto *pp = (ধ্রুবক HBINT8 *) p;
	    জন্য (; i < stop; i++)
	    {
	      দশমিক v = *pp++ * scalar;
	      যদি (is_x) points.arrayZ[i].x += v;
	      অন্যথা points.arrayZ[i].y += v;
	    }
	    p = (ধ্রুবক HBUINT8 *) pp;
	  }
	  বিরতি;
	}
      }
      ফেরত সত্য;
    }
    সর্বজনীন:
    যৌক্তিক apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
				 ধ্রুবক hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 যৌক্তিক phantom_only = মিথ্যা,
				 int64_t *budget = nullptr) ধ্রুবক
    {
      যদি (unlikely (glyph >= glyph_গুনতি_2)) ফেরত সত্য;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_তথ্য_bytes = table->get_glyph_var_তথ্য_bytes (table.get_blob (), glyph_গুনতি_2, glyph);
      যদি (!var_তথ্য_bytes.as<Glyph_Variation_তথ্য> ()->has_তথ্য ()) ফেরত সত্য;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename Glyph_Variation_তথ্য::tuple_iterator_t iterator;
      যদি (!Glyph_Variation_তথ্য::get_tuple_iterator (var_তথ্য_bytes, table->axis_গুনতি_2,
						   var_তথ্য_bytes.arrayZ,
						   shared_indices, &iterator))
	ফেরত সত্য; /* so isn't applied at all */

      যৌক্তিক any_private_points = মিথ্যা;
      যৌক্তিক private_points_checked = মিথ্যা;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned অঙ্ক_coords = table->axis_গুনতি_2;
      hb_array_t<ধ্রুবক F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_গুনতি * অঙ্ক_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned গুনতি = points.দৈর্ঘ্য;
      যৌক্তিক flush = মিথ্যা;

      করো
      {
	দশমিক scalar = iterator.current_tuple->calculate_scalar (coords, অঙ্ক_coords, shared_tuples,
								 scalar_cache);

	যদি (scalar == 0.f) চালু;

	যদি (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && গুনতি >= 4 ? 4 : গুনতি)))
	  ফেরত মিথ্যা;

	যদি (!private_points_checked)
	{
	  auto scan = iterator;
	  করো
	  {
	    যদি (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = সত্য;
	      বিরতি;
	    }
	  } যখন (scan.move_to_পরবর্তী ());
	  private_points_checked = সত্য;
	}
	ধ্রুবক HBUINT8 *p = iterator.get_serialized_তথ্য ();
	unsigned পূর্ণসংখ্যা দৈর্ঘ্য = iterator.current_tuple->get_তথ্য_আকার ();
	যদি (unlikely (!iterator.var_তথ্য_bytes.check_range (p, দৈর্ঘ্য)))
	  ফেরত মিথ্যা;

	যদি (!deltas)
	{
	  যদি (unlikely (!deltas_vec.resize_dirty  (গুনতি))) ফেরত মিথ্যা;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? গুনতি - 4 : 0), 0,
		     (phantom_only ? 4 : গুনতি) * আকার (deltas[0]));
	}

	ধ্রুবক HBUINT8 *end = p + দৈর্ঘ্য;

	যৌক্তিক has_private_points = iterator.current_tuple->has_private_points ();
	যদি (has_private_points &&
	    !Glyph_Variation_তথ্য::decompile_points (p, private_indices, end))
	  ফেরত মিথ্যা;
	ধ্রুবক hb_array_t<unsigned পূর্ণসংখ্যা> &indices = has_private_points ? private_indices : shared_indices;

	যৌক্তিক apply_to_all = (indices.দৈর্ঘ্য == 0);
	unsigned অঙ্ক_deltas = apply_to_all ? points.দৈর্ঘ্য : indices.দৈর্ঘ্য;
	unsigned start_deltas = (apply_to_all && phantom_only && অঙ্ক_deltas >= 4 ? অঙ্ক_deltas - 4 : 0);

	যদি (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_আকার
	  যদি (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, সত্য))) ফেরত মিথ্যা;
	  যদি (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, মিথ্যা))) ফেরত মিথ্যা;
#অন্যথা
	  যদি (unlikely (!decompile_deltas_add_to_points<সত্য> (p, points, scalar, end, start_deltas))) ফেরত মিথ্যা;
	  যদি (unlikely (!decompile_deltas_add_to_points<মিথ্যা> (p, points, scalar, end, start_deltas))) ফেরত মিথ্যা;
#endif
	  চালু;
	}

	যদি (unlikely (!x_deltas.resize_dirty  (অঙ্ক_deltas))) ফেরত মিথ্যা;
	যদি (unlikely (!Glyph_Variation_তথ্য::decompile_deltas (p, x_deltas, end, মিথ্যা, start_deltas))) ফেরত মিথ্যা;
	যদি (unlikely (!y_deltas.resize_dirty  (অঙ্ক_deltas))) ফেরত মিথ্যা;
	যদি (unlikely (!Glyph_Variation_তথ্য::decompile_deltas (p, y_deltas, end, মিথ্যা, start_deltas))) ফেরত মিথ্যা;

	যদি (!apply_to_all)
	{
	  যদি (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    যদি (unlikely (orig_points_vec.in_error ())) ফেরত মিথ্যা;
	    orig_points = orig_points_vec.as_array ();
	  }

	  যদি (flush)
	  {
	    জন্য (unsigned পূর্ণসংখ্যা i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? গুনতি - 4 : 0), 0,
		     (phantom_only ? 4 : গুনতি) * আকার (deltas[0]));
	}

	যদি (HB_OPTIMIZE_আকার_VAL)
	{
	  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_deltas; i++)
	  {
	    unsigned পূর্ণসংখ্যা pt_সূচক;
	    যদি (apply_to_all)
	      pt_সূচক = i;
	    অন্যথা
	    {
	      pt_সূচক = indices[i];
	      যদি (unlikely (pt_সূচক >= deltas.দৈর্ঘ্য)) চালু;
	    }
	    যদি (phantom_only && pt_সূচক < গুনতি - 4) চালু;
	    auto &delta = deltas.arrayZ[pt_সূচক];
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
	      জন্য (unsigned পূর্ণসংখ্যা i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    অন্যথা
	      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_deltas; i++)
	      {
		unsigned পূর্ণসংখ্যা pt_সূচক = indices[i];
		যদি (unlikely (pt_সূচক >= deltas.দৈর্ঘ্য)) চালু;
		যদি (phantom_only && pt_সূচক < গুনতি - 4) চালু;
		auto &delta = deltas.arrayZ[pt_সূচক];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  অন্যথা
	  {
	    যদি (apply_to_all)
	      জন্য (unsigned পূর্ণসংখ্যা i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    অন্যথা
	      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_deltas; i++)
	      {
		unsigned পূর্ণসংখ্যা pt_সূচক = indices[i];
		যদি (unlikely (pt_সূচক >= deltas.দৈর্ঘ্য)) চালু;
		যদি (phantom_only && pt_সূচক < গুনতি - 4) চালু;
		auto &delta = deltas.arrayZ[pt_সূচক];
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
	  যখন (সত্য)
	  {
	    যখন (end_point < গুনতি && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    যদি (unlikely (end_point == গুনতি)) বিরতি;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_গুনতি = 0;
	    জন্য (unsigned i = start_point; i < end_point + 1; i++)
	      unref_গুনতি += deltas.arrayZ[i].flag;
	    unref_গুনতি = (end_point - start_point + 1) - unref_গুনতি;

	    unsigned j = start_point;
	    যদি (unref_গুনতি == 0 || unref_গুনতি > end_point - start_point)
	      যাও no_more_gaps;

	    জন্য (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned পূর্ণসংখ্যা prev, next, i;
	      জন্য (;;)
	      {
		i = j;
		j = পরবর্তী_সূচক (i, start_point, end_point);
		যদি (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) বিরতি;
	      }
	      prev = j = i;
	      জন্য (;;)
	      {
		i = j;
		j = পরবর্তী_সূচক (i, start_point, end_point);
		যদি (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) বিরতি;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      জন্য (;;)
	      {
		i = পরবর্তী_সূচক (i, start_point, end_point);
		যদি (i == next) বিরতি;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		যদি (--unref_গুনতি == 0) যাও no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = সত্য;

      } যখন (iterator.move_to_পরবর্তী ());

      যদি (flush)
      {
	জন্য (unsigned পূর্ণসংখ্যা i = phantom_only ? গুনতি - 4 : 0; i < গুনতি; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      ফেরত সত্য;
    }

    unsigned পূর্ণসংখ্যা get_axis_গুনতি () ধ্রুবক { ফেরত table->axis_গুনতি_2; }

    ব্যক্তিগত:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_গুনতি_2;
  };

  সুরক্ষিত:
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
  Offset32To<Glyph_Variation_তথ্য>
		তথ্য_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  UnsizedArrayOf<HBUINT8>
		offsetZ;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (18 + Glyph_গুনতি_Type::static_আকার, offsetZ);
};

কাঠামো gvar : gvar_GVAR<gvar, SmallTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

কাঠামো GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

কাঠামো gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ধ্রুবক
  {
    যদি (has_তথ্য ()) ফেরত HB_DUAL_GET (*এই, create_cache ());
    ফেরত nullptr;
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  যৌক্তিক apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
			       ধ্রুবক hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       যৌক্তিক phantom_only = মিথ্যা,
			       int64_t *budget = nullptr) ধ্রুবক
  {
    ফেরত HB_DUAL_GET (*এই, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned পূর্ণসংখ্যা get_axis_গুনতি () ধ্রুবক
  {
    যদি (has_তথ্য ()) ফেরত HB_DUAL_GET (*এই, get_axis_গুনতি ());
    ফেরত 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
