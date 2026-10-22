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

#ಅನ್ತರ್ಭಾವ "hb-decycler.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-dual.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

ಸಂರಚನಾ hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> x_deltas;
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> shared_indices;
  hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> private_indices;
};

ನಾಮಾಕಾಶ OT {

template <typename OffsetType>
ಸಂರಚನಾ glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  ಗುಪ್ತ:
  unsigned shared_tuples_ಎಣಿಕೆ = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  ಸಾರ್ವಜನಿಕ:
  unsigned compiled_shared_tuples_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ shared_tuples_ಎಣಿಕೆ; }

  unsigned compiled_byte_size () ಧ್ರುವ
  {
    unsigned byte_size = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    ಪ್ರತ್ಯಾಗಮ byte_size;
  }

  ತಾರ್ಕಿಕ create_from_glyphs_var_data (unsigned axis_ಎಣಿಕೆ,
                                    ಧ್ರುವ hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
                                    ಧ್ರುವ hb_subset_plan_t *plan,
                                    ಧ್ರುವ hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    ಯದಿ (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto it = hb_iter (plan->new_to_old_gid_list);
    ಪ್ರತಿ (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      ಯದಿ (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ಧ್ರುವ GlyphVariationData* p = reinterpret_cast<ಧ್ರುವ GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      ಯದಿ (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_ಎಣಿಕೆ,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        ಅನುವರ್ತ;
      }

      ತಾರ್ಕಿಕ is_composite_glyph = ಅಸತ್ಯ;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      ಯದಿ (!p->decompile_tuple_variations (all_contour_points->length, ಸತ್ಯ /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      glyph_variations.push (std::move (tuple_vars));
    }
    ಪ್ರತ್ಯಾಗಮ !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  ತಾರ್ಕಿಕ instantiate (ಧ್ರುವ hb_subset_plan_t *plan)
  {
    unsigned ಎಣಿಕೆ = plan->new_to_old_gid_list.length;
    ತಾರ್ಕಿಕ iup_optimize = ಅಸತ್ಯ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      ಯದಿ (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      ಯದಿ (iup_optimize)
      {
        points = *all_points;
        ಯದಿ (unlikely (points.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      ಯದಿ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      ಯದಿ (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ compile_bytes (ಧ್ರುವ hb_map_t& axes_index_map,
                      ಧ್ರುವ hb_map_t& axes_old_index_tag_map)
  {
    ಯದಿ (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (tuple_variations_t& vars: glyph_variations)
      ಯದಿ (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               ಸತ್ಯ, /* use shared points*/
                               ಸತ್ಯ,
                               &shared_tuples_idx_map,
			       &pool))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ compile_shared_tuples (ಧ್ರುವ hb_map_t& axes_index_map,
                              ಧ್ರುವ hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned> coords_ಎಣಿಕೆ_map;

    /* count the num of shared coords */
    ಪ್ರತಿ (tuple_variations_t& vars: glyph_variations)
    {
      ಪ್ರತಿ (tuple_delta_t& var : vars.tuple_vars)
      {
        ಯದಿ (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        unsigned *ಎಣಿಕೆ;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        ಯದಿ (coords_ಎಣಿಕೆ_map.has_with_hash (&(var.compiled_peak_coords), hash, &ಎಣಿಕೆ))
	  (*ಎಣಿಕೆ)++;
        ಅನ್ಯಥಾ
          coords_ಎಣಿಕೆ_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    ಯದಿ (!coords_ಎಣಿಕೆ_map || coords_ಎಣಿಕೆ_map.in_error ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_ಎಣಿಕೆ_map)
      | hb_filter ([] (ಧ್ರುವ hb_pair_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned>& p) { ಪ್ರತ್ಯಾಗಮ p.second > 1; })
    };
    ಯದಿ (unlikely (shared_coords.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* no shared tuples: no coords are used more than once */
    ಯದಿ (!shared_coords) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_ಎಣಿಕೆ = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_ಎಣಿಕೆ * (shared_coords[0].first->length);
    ಯದಿ (unlikely (!compiled_shared_tuples.alloc (len)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < shared_tuples_ಎಣಿಕೆ; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      ಪ್ರತಿ (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _cmp_coords (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ಧ್ರುವ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ಧ್ರುವ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ಧ್ರುವ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    ಯದಿ (a->second != b->second)
      ಪ್ರತ್ಯಾಗಮ b->second - a->second; // high to low

    ಪ್ರತ್ಯಾಗಮ b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 ತಾರ್ಕಿಕ long_offset,
                                 unsigned ಅಂಕೆ_glyphs,
                                 ವರ್ಣ* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    ಅನ್ಯಥಾ
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    ಯದಿ (!cur_glyph) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      ಯದಿ (long_offset)
        ಪ್ರತಿ (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      ಯದಿ (idx >= glyph_variations.length) return_trace (ಅಸತ್ಯ);
      ಯದಿ (!cur_glyph->serialize (c, ಸತ್ಯ, glyph_variations[idx])) return_trace (ಅಸತ್ಯ);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (ವರ್ಣ *) next_glyph - (ವರ್ಣ *) cur_glyph;

      ಯದಿ (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      ಅನ್ಯಥಾ
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    ಯದಿ (long_offset)
      ಪ್ರತಿ (; last_gid < ಅಂಕೆ_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    ಅನ್ಯಥಾ
      ಪ್ರತಿ (; last_gid < ಅಂಕೆ_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (ಸತ್ಯ);
  }
};

template <typename T, typename Types>
ಸಂರಚನಾ gvar_GVAR
{
  using Glyph_ಎಣಿಕೆ_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_ಎಣಿಕೆ_Type>;
  ಸ್ಥಿರ constexpr unsigned glyph_ಎಣಿಕೆ_max ()
  { ಪ್ರತ್ಯಾಗಮ (1u << (8 * Glyph_ಎಣಿಕೆ_Type::static_size)) - 1; }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int () != 0; }

  ತಾರ್ಕಿಕ sanitize_shallow (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, ಏತತ್, axis_ಎಣಿಕೆ_2 * shared_Tuple_ಎಣಿಕೆ) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_ಅಂಕೆ_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_ಅಂಕೆ_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ sanitize_shallow (c); }

  ತಾರ್ಕಿಕ decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_ಎಣಿಕೆ_Type>& glyph_vars /* OUT */) ಧ್ರುವ
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    ಯದಿ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    ಪ್ರತಿ (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ಎಣಿಕೆ_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    ಯದಿ (new_gid_var_data_map.in_error ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples = (ಏತತ್+sharedTuples).as_array ((unsigned) shared_Tuple_ಎಣಿಕೆ * (unsigned) axis_ಎಣಿಕೆ_2);
    ಪ್ರತ್ಯಾಗಮ glyph_vars.create_from_glyphs_var_data (axis_ಎಣಿಕೆ_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ಧ್ರುವ glyph_variations_t<Glyph_ಎಣಿಕೆ_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_ಎಣಿಕೆ,
                  unsigned ಅಂಕೆ_glyphs,
                  ತಾರ್ಕಿಕ force_long_offsets) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ಎಣಿಕೆ_2 = axis_ಎಣಿಕೆ;
    out->glyph_ಎಣಿಕೆ_X = hb_min (glyph_ಎಣಿಕೆ_max (), ಅಂಕೆ_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ತಾರ್ಕಿಕ long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (ಅಂಕೆ_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, ಅಸತ್ಯ);
    ಯದಿ (!glyph_var_data_offsets) return_trace (ಅಸತ್ಯ);

    /* shared tuples */
    unsigned shared_tuple_ಎಣಿಕೆ = glyph_vars.compiled_shared_tuples_ಎಣಿಕೆ ();
    out->shared_Tuple_ಎಣಿಕೆ = shared_tuple_ಎಣಿಕೆ;

    ಯದಿ (!shared_tuple_ಎಣಿಕೆ)
      out->sharedTuples = (ವರ್ಣ *) glyph_var_data_offsets + glyph_var_data_offsets_size - (ವರ್ಣ *) out;
    ಅನ್ಯಥಾ
    {
      hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      ಯದಿ (!shared_tuples.arrayZ) return_trace (ಅಸತ್ಯ);
      out->sharedTuples = (ಧ್ರುವ ವರ್ಣ *) shared_tuples.arrayZ - (ವರ್ಣ *) out;
    }

    ವರ್ಣ *glyph_var_data = c->start_embed<ವರ್ಣ> ();
    ಯದಿ (!glyph_var_data) return_trace (ಅಸತ್ಯ);
    out->dataZ = glyph_var_data - (ವರ್ಣ *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, ಅಂಕೆ_glyphs,
                                                       (ವರ್ಣ *) glyph_var_data_offsets));
  }

  ತಾರ್ಕಿಕ instantiate (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    glyph_variations_t<Glyph_ಎಣಿಕೆ_Type> glyph_vars;
    ಯದಿ (!decompile_glyph_variations (c, glyph_vars))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (!glyph_vars.instantiate (c->plan)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (ಅಸತ್ಯ);

    unsigned axis_ಎಣಿಕೆ = c->plan->axes_index_map.get_population ();
    unsigned ಅಂಕೆ_glyphs = c->plan->ಅಂಕೆ_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    ತಾರ್ಕಿಕ force_long_offsets = ಅಸತ್ಯ;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_ಎಣಿಕೆ, ಅಂಕೆ_glyphs, force_long_offsets));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_ಎಣಿಕೆ = version.to_int () ? c->plan->source->get_ಅಂಕೆ_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ಎಣಿಕೆ_2 = axis_ಎಣಿಕೆ_2;
    out->shared_Tuple_ಎಣಿಕೆ = shared_Tuple_ಎಣಿಕೆ;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = c->plan->ಅಂಕೆ_output_glyphs ();
    out->glyph_ಎಣಿಕೆ_X = hb_min (glyph_ಎಣಿಕೆ_max (), ಅಂಕೆ_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<ವರ್ಣ>> culled_vars;
    hb_set_t culled_gids;
    ಯದಿ (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples_array =
	  (ಏತತ್+sharedTuples).as_array ((unsigned) shared_Tuple_ಎಣಿಕೆ * (unsigned) axis_ಎಣಿಕೆ_2);
      ಯದಿ (unlikely (!culled_vars.resize (ಅಂಕೆ_glyphs))) return_trace (ಅಸತ್ಯ);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      ಯದಿ (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      ಪ್ರತಿ (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ಎಣಿಕೆ, _.second);
	ಯದಿ (var_data_bytes.length < GlyphVariationData::min_size) ಅನುವರ್ತ;
	ತಾರ್ಕಿಕ changed = ಅಸತ್ಯ;
	ಯದಿ (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_ಎಣಿಕೆ_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (ಅಸತ್ಯ);
	ಯದಿ (changed) culled_gids.add (_.first);
      }
      ಯದಿ (unlikely (culled_gids.in_error ())) return_trace (ಅಸತ್ಯ);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    ಯದಿ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    ಪ್ರತಿ (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_ಎಣಿಕೆ, old_gid).length;
      ಯದಿ (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      ಯದಿ (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (ಅಸತ್ಯ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ತಾರ್ಕಿಕ long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (ಅಂಕೆ_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, ಅಸತ್ಯ);
    ಯದಿ (!subset_offsets) return_trace (ಅಸತ್ಯ);

    /* shared tuples */
    ಯದಿ (!shared_Tuple_ಎಣಿಕೆ || !sharedTuples)
      out->sharedTuples = (ವರ್ಣ *) subset_offsets + subset_offsets_size - (ವರ್ಣ *) out;
    ಅನ್ಯಥಾ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ shared_tuple_size = F2DOT14::static_size * axis_ಎಣಿಕೆ_2 * shared_Tuple_ಎಣಿಕೆ;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      ಯದಿ (!tuples) return_trace (ಅಸತ್ಯ);
      out->sharedTuples = (ವರ್ಣ *) tuples - (ವರ್ಣ *) out;
      hb_memcpy (tuples, ಏತತ್+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    ಯದಿ (long_offset)
      subset_data_size -= padding_size;
    ವರ್ಣ *subset_data = c->serializer->allocate_size<ವರ್ಣ> (subset_data_size, ಅಸತ್ಯ);
    ಯದಿ (!subset_data) return_trace (ಅಸತ್ಯ);
    out->dataZ = subset_data - (ವರ್ಣ *) out;


    ಯದಿ (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    ಅನ್ಯಥಾ
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    ಯದಿ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    ಪ್ರತಿ (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      ಯದಿ (long_offset)
	ಪ್ರತಿ (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      ಅನ್ಯಥಾ
	ಪ್ರತಿ (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_ಎಣಿಕೆ,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      ಯದಿ (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      ಯದಿ (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      ಅನ್ಯಥಾ
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    ಯದಿ (long_offset)
      ಪ್ರತಿ (; last < ಅಂಕೆ_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    ಅನ್ಯಥಾ
      ಪ್ರತಿ (; last < ಅಂಕೆ_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  ಧ್ರುವ hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_ಎಣಿಕೆ,
					     hb_codepoint_t glyph) ಧ್ರುವ
  {
    unsigned start_offset = get_offset (glyph_ಎಣಿಕೆ, glyph);
    unsigned end_offset = get_offset (glyph_ಎಣಿಕೆ, glyph+1);
    ಯದಿ (unlikely (end_offset < start_offset)) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    ಪ್ರತ್ಯಾಗಮ likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  ತಾರ್ಕಿಕ is_long_offset () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ flags & 1; }

  unsigned get_offset (unsigned glyph_ಎಣಿಕೆ, unsigned i) ಧ್ರುವ
  {
    ಯದಿ (unlikely (i > glyph_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ 0;
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ಧ್ರುವ HBUINT32 * get_long_offset_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ HBUINT32 *) &offsetZ; }
  ಧ್ರುವ HBUINT16 *get_short_offset_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ HBUINT16 *) &offsetZ; }

  ಸಾರ್ವಜನಿಕ:
  ಸಂರಚನಾ accelerator_t
  {

    hb_scalar_cache_t *create_cache () ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_ಎಣಿಕೆ,
						+TupleVariationHeader::max_shared_tuple_ಎಣಿಕೆ));
    }

    ಸ್ಥಿರ ಶೂನ್ಯ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_ಎಣಿಕೆ_2 = table->version.to_int () ? face->get_ಅಂಕೆ_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    ಗುಪ್ತ:

    ಸ್ಥಿರ ಭಿನ್ನ infer_delta (ಧ್ರುವ hb_array_t<contour_point_t> points,
			      ಧ್ರುವ hb_array_t<contour_point_t> deltas,
			      unsigned ಪೂರ್ಣಾಙ್ಕ target, unsigned ಪೂರ್ಣಾಙ್ಕ prev, unsigned ಪೂರ್ಣಾಙ್ಕ next,
			      ಭಿನ್ನ contour_point_t::*m)
    {
      ಭಿನ್ನ target_val = points.arrayZ[target].*m;
      ಭಿನ್ನ prev_val = points.arrayZ[prev].*m;
      ಭಿನ್ನ next_val = points.arrayZ[next].*m;
      ಭಿನ್ನ prev_delta =  deltas.arrayZ[prev].*m;
      ಭಿನ್ನ next_delta =  deltas.arrayZ[next].*m;

      ಯದಿ (prev_val == next_val)
	ಪ್ರತ್ಯಾಗಮ (prev_delta == next_delta) ? prev_delta : 0.f;
      ಅನ್ಯಥಾ ಯದಿ (target_val <= hb_min (prev_val, next_val))
	ಪ್ರತ್ಯಾಗಮ (prev_val < next_val) ? prev_delta : next_delta;
      ಅನ್ಯಥಾ ಯದಿ (target_val >= hb_max (prev_val, next_val))
	ಪ್ರತ್ಯಾಗಮ (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      ಭಿನ್ನ r = (target_val - prev_val) / (next_val - prev_val);
      ಪ್ರತ್ಯಾಗಮ prev_delta + r * (next_delta - prev_delta);
    }

    ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ next_index (unsigned ಪೂರ್ಣಾಙ್ಕ i, unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
    { ಪ್ರತ್ಯಾಗಮ (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <ತಾರ್ಕಿಕ is_x>
#endif
    ಸ್ಥಿರ ತಾರ್ಕಿಕ decompile_deltas_add_to_points (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						ಭಿನ್ನ scalar,
						ಧ್ರುವ HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, ತಾರ್ಕಿಕ is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned ಎಣಿಕೆ = points.length;
      ಯಾವತ್ (i < ಎಣಿಕೆ)
      {
	ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	unsigned control = *p++;
	unsigned run_ಎಣಿಕೆ = (control & Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯ_RUN_ಎಣಿಕೆ_MASK) + 1;
	unsigned stop = i + run_ಎಣಿಕೆ;
	ಯದಿ (unlikely (stop > ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	unsigned skip = i < start ? hb_min (start - i, run_ಎಣಿಕೆ) : 0;
	i += skip;

	ವಿಕಲ್ಪ (control & Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯಗಳು_SIZE_MASK)
	{
	  ಪಕ್ಷ Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯಗಳು_ARE_ZEROS:
	    i = stop;
	    ಭಙ್ಗ;
	  ಪಕ್ಷ Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯಗಳು_ARE_ಪದಗಳು:
	  {
	    ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * HBINT16::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    p += skip * HBINT16::static_size;
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT16 *) p;
	    ಪ್ರತಿ (; i < stop; i++)
	    {
	      ಭಿನ್ನ v = *pp++ * scalar;
	      ಯದಿ (is_x) points.arrayZ[i].x += v;
	      ಅನ್ಯಥಾ points.arrayZ[i].y += v;
	    }
	    p = (ಧ್ರುವ HBUINT8 *) pp;
	  }
	  ಭಙ್ಗ;
	  ಪಕ್ಷ Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯಗಳು_ARE_LONGS:
	  {
	    ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * HBINT32::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    p += skip * HBINT32::static_size;
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT32 *) p;
	    ಪ್ರತಿ (; i < stop; i++)
	    {
	      ಭಿನ್ನ v = *pp++ * scalar;
	      ಯದಿ (is_x) points.arrayZ[i].x += v;
	      ಅನ್ಯಥಾ points.arrayZ[i].y += v;
	    }
	    p = (ಧ್ರುವ HBUINT8 *) pp;
	  }
	  ಭಙ್ಗ;
	  ಪಕ್ಷ Tuple_ಮೌಲ್ಯಗಳು::ಮೌಲ್ಯಗಳು_ARE_BYTES:
	  {
	    ಯದಿ (unlikely (p + run_ಎಣಿಕೆ > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    p += skip * HBINT8::static_size;
	    ಧ್ರುವ auto *pp = (ಧ್ರುವ HBINT8 *) p;
	    ಪ್ರತಿ (; i < stop; i++)
	    {
	      ಭಿನ್ನ v = *pp++ * scalar;
	      ಯದಿ (is_x) points.arrayZ[i].x += v;
	      ಅನ್ಯಥಾ points.arrayZ[i].y += v;
	    }
	    p = (ಧ್ರುವ HBUINT8 *) pp;
	  }
	  ಭಙ್ಗ;
	}
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
				 ಧ್ರುವ hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 ತಾರ್ಕಿಕ phantom_only = ಅಸತ್ಯ,
				 int64_t *budget = nullptr) ಧ್ರುವ
    {
      ಯದಿ (unlikely (glyph >= glyph_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_ಎಣಿಕೆ_2, glyph);
      ಯದಿ (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      ಯದಿ (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_ಎಣಿಕೆ_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* so isn't applied at all */

      ತಾರ್ಕಿಕ any_private_points = ಅಸತ್ಯ;
      ತಾರ್ಕಿಕ private_points_checked = ಅಸತ್ಯ;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned ಅಂಕೆ_coords = table->axis_ಎಣಿಕೆ_2;
      hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_ಎಣಿಕೆ * ಅಂಕೆ_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned ಎಣಿಕೆ = points.length;
      ತಾರ್ಕಿಕ flush = ಅಸತ್ಯ;

      ಕುರು
      {
	ಭಿನ್ನ scalar = iterator.current_tuple->calculate_scalar (coords, ಅಂಕೆ_coords, shared_tuples,
								 scalar_cache);

	ಯದಿ (scalar == 0.f) ಅನುವರ್ತ;

	ಯದಿ (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && ಎಣಿಕೆ >= 4 ? 4 : ಎಣಿಕೆ)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	ಯದಿ (!private_points_checked)
	{
	  auto scan = iterator;
	  ಕುರು
	  {
	    ಯದಿ (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = ಸತ್ಯ;
	      ಭಙ್ಗ;
	    }
	  } ಯಾವತ್ (scan.move_to_next ());
	  private_points_checked = ಸತ್ಯ;
	}
	ಧ್ರುವ HBUINT8 *p = iterator.get_serialized_data ();
	unsigned ಪೂರ್ಣಾಙ್ಕ length = iterator.current_tuple->get_data_size ();
	ಯದಿ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	ಯದಿ (!deltas)
	{
	  ಯದಿ (unlikely (!deltas_vec.resize_dirty  (ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? ಎಣಿಕೆ - 4 : 0), 0,
		     (phantom_only ? 4 : ಎಣಿಕೆ) * ಪರಿಮಾಣ (deltas[0]));
	}

	ಧ್ರುವ HBUINT8 *end = p + length;

	ತಾರ್ಕಿಕ has_private_points = iterator.current_tuple->has_private_points ();
	ಯದಿ (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಧ್ರುವ hb_array_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &indices = has_private_points ? private_indices : shared_indices;

	ತಾರ್ಕಿಕ apply_to_all = (indices.length == 0);
	unsigned ಅಂಕೆ_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && ಅಂಕೆ_deltas >= 4 ? ಅಂಕೆ_deltas - 4 : 0);

	ಯದಿ (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  ಯದಿ (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, ಸತ್ಯ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  ಯದಿ (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, ಅಸತ್ಯ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
	  ಯದಿ (unlikely (!decompile_deltas_add_to_points<ಸತ್ಯ> (p, points, scalar, end, start_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  ಯದಿ (unlikely (!decompile_deltas_add_to_points<ಅಸತ್ಯ> (p, points, scalar, end, start_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
	  ಅನುವರ್ತ;
	}

	ಯದಿ (unlikely (!x_deltas.resize_dirty  (ಅಂಕೆ_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಯದಿ (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, ಅಸತ್ಯ, start_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಯದಿ (unlikely (!y_deltas.resize_dirty  (ಅಂಕೆ_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಯದಿ (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, ಅಸತ್ಯ, start_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	ಯದಿ (!apply_to_all)
	{
	  ಯದಿ (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    ಯದಿ (unlikely (orig_points_vec.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  ಯದಿ (flush)
	  {
	    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = phantom_only ? ಎಣಿಕೆ - 4 : 0; i < ಎಣಿಕೆ; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? ಎಣಿಕೆ - 4 : 0), 0,
		     (phantom_only ? 4 : ಎಣಿಕೆ) * ಪರಿಮಾಣ (deltas[0]));
	}

	ಯದಿ (HB_OPTIMIZE_SIZE_VAL)
	{
	  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_deltas; i++)
	  {
	    unsigned ಪೂರ್ಣಾಙ್ಕ pt_index;
	    ಯದಿ (apply_to_all)
	      pt_index = i;
	    ಅನ್ಯಥಾ
	    {
	      pt_index = indices[i];
	      ಯದಿ (unlikely (pt_index >= deltas.length)) ಅನುವರ್ತ;
	    }
	    ಯದಿ (phantom_only && pt_index < ಎಣಿಕೆ - 4) ಅನುವರ್ತ;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	ಅನ್ಯಥಾ
	{
	  /* Ouch. Four cases... for optimization. */
	  ಯದಿ (scalar != 1.0f)
	  {
	    ಯದಿ (apply_to_all)
	      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = phantom_only ? ಎಣಿಕೆ - 4 : 0; i < ಎಣಿಕೆ; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    ಅನ್ಯಥಾ
	      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_deltas; i++)
	      {
		unsigned ಪೂರ್ಣಾಙ್ಕ pt_index = indices[i];
		ಯದಿ (unlikely (pt_index >= deltas.length)) ಅನುವರ್ತ;
		ಯದಿ (phantom_only && pt_index < ಎಣಿಕೆ - 4) ಅನುವರ್ತ;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  ಅನ್ಯಥಾ
	  {
	    ಯದಿ (apply_to_all)
	      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = phantom_only ? ಎಣಿಕೆ - 4 : 0; i < ಎಣಿಕೆ; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    ಅನ್ಯಥಾ
	      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_deltas; i++)
	      {
		unsigned ಪೂರ್ಣಾಙ್ಕ pt_index = indices[i];
		ಯದಿ (unlikely (pt_index >= deltas.length)) ಅನುವರ್ತ;
		ಯದಿ (phantom_only && pt_index < ಎಣಿಕೆ - 4) ಅನುವರ್ತ;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	ಯದಿ (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  ಯಾವತ್ (ಸತ್ಯ)
	  {
	    ಯಾವತ್ (end_point < ಎಣಿಕೆ && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    ಯದಿ (unlikely (end_point == ಎಣಿಕೆ)) ಭಙ್ಗ;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_ಎಣಿಕೆ = 0;
	    ಪ್ರತಿ (unsigned i = start_point; i < end_point + 1; i++)
	      unref_ಎಣಿಕೆ += deltas.arrayZ[i].flag;
	    unref_ಎಣಿಕೆ = (end_point - start_point + 1) - unref_ಎಣಿಕೆ;

	    unsigned j = start_point;
	    ಯದಿ (unref_ಎಣಿಕೆ == 0 || unref_ಎಣಿಕೆ > end_point - start_point)
	      ಗಚ್ಛ no_more_gaps;

	    ಪ್ರತಿ (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned ಪೂರ್ಣಾಙ್ಕ prev, next, i;
	      ಪ್ರತಿ (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		ಯದಿ (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) ಭಙ್ಗ;
	      }
	      prev = j = i;
	      ಪ್ರತಿ (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		ಯದಿ (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) ಭಙ್ಗ;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      ಪ್ರತಿ (;;)
	      {
		i = next_index (i, start_point, end_point);
		ಯದಿ (i == next) ಭಙ್ಗ;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		ಯದಿ (--unref_ಎಣಿಕೆ == 0) ಗಚ್ಛ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = ಸತ್ಯ;

      } ಯಾವತ್ (iterator.move_to_next ());

      ಯದಿ (flush)
      {
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = phantom_only ? ಎಣಿಕೆ - 4 : 0; i < ಎಣಿಕೆ; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table->axis_ಎಣಿಕೆ_2; }

    ಗುಪ್ತ:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_ಎಣಿಕೆ_2;
  };

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_ಎಣಿಕೆ_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_ಎಣಿಕೆ;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_ಎಣಿಕೆ_Type
		glyph_ಎಣಿಕೆ_X;	/* The number of glyphs in this font. This must match the number of
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (18 + Glyph_ಎಣಿಕೆ_Type::static_size, offsetZ);
};

ಸಂರಚನಾ gvar : gvar_GVAR<gvar, SmallTypes>
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

ಸಂರಚನಾ GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

ಸಂರಚನಾ gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ಧ್ರುವ
  {
    ಯದಿ (has_data ()) ಪ್ರತ್ಯಾಗಮ HB_DUAL_GET (*ಏತತ್, create_cache ());
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ತಾರ್ಕಿಕ apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
			       ಧ್ರುವ hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       ತಾರ್ಕಿಕ phantom_only = ಅಸತ್ಯ,
			       int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ HB_DUAL_GET (*ಏತತ್, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ಯದಿ (has_data ()) ಪ್ರತ್ಯಾಗಮ HB_DUAL_GET (*ಏತತ್, get_axis_ಎಣಿಕೆ ());
    ಪ್ರತ್ಯಾಗಮ 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
