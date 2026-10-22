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

#अन्तर्भाव "hb-decycler.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-dual.hh"
#अन्तर्भाव "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

संरचना hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<पूर्णाङ्क> x_deltas;
  hb_vector_t<पूर्णाङ्क> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned पूर्णाङ्क> shared_indices;
  hb_vector_t<unsigned पूर्णाङ्क> private_indices;
};

नामाकाश OT {

template <typename OffsetType>
संरचना glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  गुप्त:
  unsigned shared_tuples_गणनम् = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  सार्वजनिक:
  unsigned compiled_shared_tuples_गणनम् () ध्रुव
  { पटिनिवत्त shared_tuples_गणनम्; }

  unsigned compiled_byte_size () ध्रुव
  {
    unsigned byte_size = 0;
    अत्थाय (ध्रुव auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    पटिनिवत्त byte_size;
  }

  तार्किक create_from_glyphs_var_data (unsigned axis_गणनम्,
                                    ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                                    ध्रुव hb_subset_plan_t *plan,
                                    ध्रुव hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    सचे (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      पटिनिवत्त मिच्छा;

    auto it = hb_iter (plan->new_to_old_gid_list);
    अत्थाय (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      सचे (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        पटिनिवत्त मिच्छा;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ध्रुव GlyphVariationData* p = reinterpret_cast<ध्रुव GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      सचे (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_गणनम्,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        अनुवर्त;
      }

      तार्किक is_composite_glyph = मिच्छा;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      सचे (!p->decompile_tuple_variations (all_contour_points->length, सच्च /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        पटिनिवत्त मिच्छा;
      glyph_variations.push (std::move (tuple_vars));
    }
    पटिनिवत्त !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  तार्किक instantiate (ध्रुव hb_subset_plan_t *plan)
  {
    unsigned गणनम् = plan->new_to_old_gid_list.length;
    तार्किक iup_optimize = मिच्छा;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      सचे (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        पटिनिवत्त मिच्छा;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      सचे (iup_optimize)
      {
        points = *all_points;
        सचे (unlikely (points.in_error ())) पटिनिवत्त मिच्छा;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      सचे (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      सचे (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }

  तार्किक compile_bytes (ध्रुव hb_map_t& axes_index_map,
                      ध्रुव hb_map_t& axes_old_index_tag_map)
  {
    सचे (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      पटिनिवत्त मिच्छा;
    अत्थाय (tuple_variations_t& vars: glyph_variations)
      सचे (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               सच्च, /* use shared points*/
                               सच्च,
                               &shared_tuples_idx_map,
			       &pool))
        पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
  }

  तार्किक compile_shared_tuples (ध्रुव hb_map_t& axes_index_map,
                              ध्रुव hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned> coords_गणनम्_map;

    /* count the num of shared coords */
    अत्थाय (tuple_variations_t& vars: glyph_variations)
    {
      अत्थाय (tuple_delta_t& var : vars.tuple_vars)
      {
        सचे (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          पटिनिवत्त मिच्छा;
        unsigned *गणनम्;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        सचे (coords_गणनम्_map.has_with_hash (&(var.compiled_peak_coords), hash, &गणनम्))
	  (*गणनम्)++;
        अञ्ञथा
          coords_गणनम्_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    सचे (!coords_गणनम्_map || coords_गणनम्_map.in_error ())
      पटिनिवत्त मिच्छा;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_गणनम्_map)
      | hb_filter ([] (ध्रुव hb_pair_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned>& p) { पटिनिवत्त p.second > 1; })
    };
    सचे (unlikely (shared_coords.in_error ())) पटिनिवत्त मिच्छा;

    /* no shared tuples: no coords are used more than once */
    सचे (!shared_coords) पटिनिवत्त सच्च;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_गणनम् = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_गणनम् * (shared_coords[0].first->length);
    सचे (unlikely (!compiled_shared_tuples.alloc (len)))
      पटिनिवत्त मिच्छा;

    अत्थाय (unsigned i = 0; i < shared_tuples_गणनम्; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      अत्थाय (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    पटिनिवत्त सच्च;
  }

  स्थिर पूर्णाङ्क _cmp_coords (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    सचे (a->second != b->second)
      पटिनिवत्त b->second - a->second; // high to low

    पटिनिवत्त b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 तार्किक long_offset,
                                 unsigned अङ्कः_glyphs,
                                 वर्ण* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    सचे (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    अञ्ञथा
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    सचे (!cur_glyph) return_trace (मिच्छा);
    अत्थाय (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      सचे (long_offset)
        अत्थाय (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      अञ्ञथा
        अत्थाय (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      सचे (idx >= glyph_variations.length) return_trace (मिच्छा);
      सचे (!cur_glyph->serialize (c, सच्च, glyph_variations[idx])) return_trace (मिच्छा);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (वर्ण *) next_glyph - (वर्ण *) cur_glyph;

      सचे (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      अञ्ञथा
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    सचे (long_offset)
      अत्थाय (; last_gid < अङ्कः_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    अञ्ञथा
      अत्थाय (; last_gid < अङ्कः_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (सच्च);
  }
};

template <typename T, typename Types>
संरचना gvar_GVAR
{
  using Glyph_गणनम्_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_गणनम्_Type>;
  स्थिर constexpr unsigned glyph_गणनम्_max ()
  { पटिनिवत्त (1u << (8 * Glyph_गणनम्_Type::static_size)) - 1; }

  तार्किक has_data () ध्रुव { पटिनिवत्त version.to_int () != 0; }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, एतत्, axis_गणनम्_2 * shared_Tuple_गणनम्) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_अङ्कः_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_अङ्कः_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  { पटिनिवत्त sanitize_shallow (c); }

  तार्किक decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_गणनम्_Type>& glyph_vars /* OUT */) ध्रुव
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    सचे (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    अत्थाय (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_गणनम्_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    सचे (new_gid_var_data_map.in_error ()) पटिनिवत्त मिच्छा;

    hb_array_t<ध्रुव F2DOT14> shared_tuples = (एतत्+sharedTuples).as_array ((unsigned) shared_Tuple_गणनम् * (unsigned) axis_गणनम्_2);
    पटिनिवत्त glyph_vars.create_from_glyphs_var_data (axis_गणनम्_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
                  ध्रुव glyph_variations_t<Glyph_गणनम्_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_गणनम्,
                  unsigned अङ्कः_glyphs,
                  तार्किक force_long_offsets) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गणनम्_2 = axis_गणनम्;
    out->glyph_गणनम्_X = hb_min (glyph_गणनम्_max (), अङ्कः_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    तार्किक long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (अङ्कः_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, मिच्छा);
    सचे (!glyph_var_data_offsets) return_trace (मिच्छा);

    /* shared tuples */
    unsigned shared_tuple_गणनम् = glyph_vars.compiled_shared_tuples_गणनम् ();
    out->shared_Tuple_गणनम् = shared_tuple_गणनम्;

    सचे (!shared_tuple_गणनम्)
      out->sharedTuples = (वर्ण *) glyph_var_data_offsets + glyph_var_data_offsets_size - (वर्ण *) out;
    अञ्ञथा
    {
      hb_array_t<ध्रुव F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      सचे (!shared_tuples.arrayZ) return_trace (मिच्छा);
      out->sharedTuples = (ध्रुव वर्ण *) shared_tuples.arrayZ - (वर्ण *) out;
    }

    वर्ण *glyph_var_data = c->start_embed<वर्ण> ();
    सचे (!glyph_var_data) return_trace (मिच्छा);
    out->dataZ = glyph_var_data - (वर्ण *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, अङ्कः_glyphs,
                                                       (वर्ण *) glyph_var_data_offsets));
  }

  तार्किक instantiate (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    glyph_variations_t<Glyph_गणनम्_Type> glyph_vars;
    सचे (!decompile_glyph_variations (c, glyph_vars))
      return_trace (मिच्छा);

    सचे (!glyph_vars.instantiate (c->plan)) return_trace (मिच्छा);
    सचे (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (मिच्छा);

    unsigned axis_गणनम् = c->plan->axes_index_map.get_population ();
    unsigned अङ्कः_glyphs = c->plan->अङ्कः_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    तार्किक force_long_offsets = मिच्छा;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_गणनम्, अङ्कः_glyphs, force_long_offsets));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    सचे (c->plan->all_axes_pinned)
      return_trace (मिच्छा);

    सचे (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_गणनम् = version.to_int () ? c->plan->source->get_अङ्कः_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गणनम्_2 = axis_गणनम्_2;
    out->shared_Tuple_गणनम् = shared_Tuple_गणनम्;

    unsigned पूर्णाङ्क अङ्कः_glyphs = c->plan->अङ्कः_output_glyphs ();
    out->glyph_गणनम्_X = hb_min (glyph_गणनम्_max (), अङ्कः_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<वर्ण>> culled_vars;
    hb_set_t culled_gids;
    सचे (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ध्रुव F2DOT14> shared_tuples_array =
	  (एतत्+sharedTuples).as_array ((unsigned) shared_Tuple_गणनम् * (unsigned) axis_गणनम्_2);
      सचे (unlikely (!culled_vars.resize (अङ्कः_glyphs))) return_trace (मिच्छा);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      सचे (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      अत्थाय (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_गणनम्, _.second);
	सचे (var_data_bytes.length < GlyphVariationData::min_size) अनुवर्त;
	तार्किक changed = मिच्छा;
	सचे (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_गणनम्_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (मिच्छा);
	सचे (changed) culled_gids.add (_.first);
      }
      सचे (unlikely (culled_gids.in_error ())) return_trace (मिच्छा);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    सचे (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    अत्थाय (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_गणनम्, old_gid).length;
      सचे (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      सचे (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (मिच्छा);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    तार्किक long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (अङ्कः_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, मिच्छा);
    सचे (!subset_offsets) return_trace (मिच्छा);

    /* shared tuples */
    सचे (!shared_Tuple_गणनम् || !sharedTuples)
      out->sharedTuples = (वर्ण *) subset_offsets + subset_offsets_size - (वर्ण *) out;
    अञ्ञथा
    {
      unsigned पूर्णाङ्क shared_tuple_size = F2DOT14::static_size * axis_गणनम्_2 * shared_Tuple_गणनम्;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      सचे (!tuples) return_trace (मिच्छा);
      out->sharedTuples = (वर्ण *) tuples - (वर्ण *) out;
      hb_memcpy (tuples, एतत्+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    सचे (long_offset)
      subset_data_size -= padding_size;
    वर्ण *subset_data = c->serializer->allocate_size<वर्ण> (subset_data_size, मिच्छा);
    सचे (!subset_data) return_trace (मिच्छा);
    out->dataZ = subset_data - (वर्ण *) out;


    सचे (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    अञ्ञथा
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned पूर्णाङ्क glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    सचे (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    अत्थाय (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      सचे (long_offset)
	अत्थाय (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      अञ्ञथा
	अत्थाय (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_गणनम्,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      सचे (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      सचे (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      अञ्ञथा
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    सचे (long_offset)
      अत्थाय (; last < अङ्कः_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    अञ्ञथा
      अत्थाय (; last < अङ्कः_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (सच्च);
  }

  रक्षित:
  ध्रुव hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_गणनम्,
					     hb_codepoint_t glyph) ध्रुव
  {
    unsigned start_offset = get_offset (glyph_गणनम्, glyph);
    unsigned end_offset = get_offset (glyph_गणनम्, glyph+1);
    सचे (unlikely (end_offset < start_offset)) पटिनिवत्त hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    पटिनिवत्त likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  तार्किक is_long_offset () ध्रुव { पटिनिवत्त flags & 1; }

  unsigned get_offset (unsigned glyph_गणनम्, unsigned i) ध्रुव
  {
    सचे (unlikely (i > glyph_गणनम्)) पटिनिवत्त 0;
    hb_barrier ();
    पटिनिवत्त is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ध्रुव HBUINT32 * get_long_offset_array () ध्रुव { पटिनिवत्त (ध्रुव HBUINT32 *) &offsetZ; }
  ध्रुव HBUINT16 *get_short_offset_array () ध्रुव { पटिनिवत्त (ध्रुव HBUINT16 *) &offsetZ; }

  सार्वजनिक:
  संरचना accelerator_t
  {

    hb_scalar_cache_t *create_cache () ध्रुव
    {
      पटिनिवत्त hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_गणनम्,
						+TupleVariationHeader::max_shared_tuple_गणनम्));
    }

    स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    तार्किक has_data () ध्रुव { पटिनिवत्त table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_गणनम्_2 = table->version.to_int () ? face->get_अङ्कः_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    गुप्त:

    स्थिर भिन्न infer_delta (ध्रुव hb_array_t<contour_point_t> points,
			      ध्रुव hb_array_t<contour_point_t> deltas,
			      unsigned पूर्णाङ्क target, unsigned पूर्णाङ्क prev, unsigned पूर्णाङ्क next,
			      भिन्न contour_point_t::*m)
    {
      भिन्न target_val = points.arrayZ[target].*m;
      भिन्न prev_val = points.arrayZ[prev].*m;
      भिन्न next_val = points.arrayZ[next].*m;
      भिन्न prev_delta =  deltas.arrayZ[prev].*m;
      भिन्न next_delta =  deltas.arrayZ[next].*m;

      सचे (prev_val == next_val)
	पटिनिवत्त (prev_delta == next_delta) ? prev_delta : 0.f;
      अञ्ञथा सचे (target_val <= hb_min (prev_val, next_val))
	पटिनिवत्त (prev_val < next_val) ? prev_delta : next_delta;
      अञ्ञथा सचे (target_val >= hb_max (prev_val, next_val))
	पटिनिवत्त (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      भिन्न r = (target_val - prev_val) / (next_val - prev_val);
      पटिनिवत्त prev_delta + r * (next_delta - prev_delta);
    }

    स्थिर unsigned पूर्णाङ्क next_index (unsigned पूर्णाङ्क i, unsigned पूर्णाङ्क start, unsigned पूर्णाङ्क end)
    { पटिनिवत्त (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <तार्किक is_x>
#endif
    स्थिर तार्किक decompile_deltas_add_to_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						भिन्न scalar,
						ध्रुव HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, तार्किक is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned गणनम् = points.length;
      याव (i < गणनम्)
      {
	सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
	unsigned control = *p++;
	unsigned run_गणनम् = (control & Tuple_मानानि::मानम्_RUN_गणनम्_MASK) + 1;
	unsigned stop = i + run_गणनम्;
	सचे (unlikely (stop > गणनम्)) पटिनिवत्त मिच्छा;

	unsigned skip = i < start ? hb_min (start - i, run_गणनम्) : 0;
	i += skip;

	विकल्प (control & Tuple_मानानि::मानानि_SIZE_MASK)
	{
	  पक्ष Tuple_मानानि::मानानि_ARE_ZEROS:
	    i = stop;
	    भङ्ग;
	  पक्ष Tuple_मानानि::मानानि_ARE_शब्दाः:
	  {
	    सचे (unlikely (p + run_गणनम् * HBINT16::static_size > end)) पटिनिवत्त मिच्छा;
	    p += skip * HBINT16::static_size;
	    ध्रुव auto *pp = (ध्रुव HBINT16 *) p;
	    अत्थाय (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      सचे (is_x) points.arrayZ[i].x += v;
	      अञ्ञथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	  पक्ष Tuple_मानानि::मानानि_ARE_LONGS:
	  {
	    सचे (unlikely (p + run_गणनम् * HBINT32::static_size > end)) पटिनिवत्त मिच्छा;
	    p += skip * HBINT32::static_size;
	    ध्रुव auto *pp = (ध्रुव HBINT32 *) p;
	    अत्थाय (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      सचे (is_x) points.arrayZ[i].x += v;
	      अञ्ञथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	  पक्ष Tuple_मानानि::मानानि_ARE_BYTES:
	  {
	    सचे (unlikely (p + run_गणनम् > end)) पटिनिवत्त मिच्छा;
	    p += skip * HBINT8::static_size;
	    ध्रुव auto *pp = (ध्रुव HBINT8 *) p;
	    अत्थाय (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      सचे (is_x) points.arrayZ[i].x += v;
	      अञ्ञथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	}
      }
      पटिनिवत्त सच्च;
    }
    सार्वजनिक:
    तार्किक apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ध्रुव पूर्णाङ्क> coords,
				 ध्रुव hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 तार्किक phantom_only = मिच्छा,
				 int64_t *budget = nullptr) ध्रुव
    {
      सचे (unlikely (glyph >= glyph_गणनम्_2)) पटिनिवत्त सच्च;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_गणनम्_2, glyph);
      सचे (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) पटिनिवत्त सच्च;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      सचे (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_गणनम्_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	पटिनिवत्त सच्च; /* so isn't applied at all */

      तार्किक any_private_points = मिच्छा;
      तार्किक private_points_checked = मिच्छा;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned अङ्कः_coords = table->axis_गणनम्_2;
      hb_array_t<ध्रुव F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_गणनम् * अङ्कः_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned गणनम् = points.length;
      तार्किक flush = मिच्छा;

      करोहि
      {
	भिन्न scalar = iterator.current_tuple->calculate_scalar (coords, अङ्कः_coords, shared_tuples,
								 scalar_cache);

	सचे (scalar == 0.f) अनुवर्त;

	सचे (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && गणनम् >= 4 ? 4 : गणनम्)))
	  पटिनिवत्त मिच्छा;

	सचे (!private_points_checked)
	{
	  auto scan = iterator;
	  करोहि
	  {
	    सचे (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = सच्च;
	      भङ्ग;
	    }
	  } याव (scan.move_to_next ());
	  private_points_checked = सच्च;
	}
	ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
	unsigned पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
	सचे (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  पटिनिवत्त मिच्छा;

	सचे (!deltas)
	{
	  सचे (unlikely (!deltas_vec.resize_dirty  (गणनम्))) पटिनिवत्त मिच्छा;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? गणनम् - 4 : 0), 0,
		     (phantom_only ? 4 : गणनम्) * परिमाण (deltas[0]));
	}

	ध्रुव HBUINT8 *end = p + length;

	तार्किक has_private_points = iterator.current_tuple->has_private_points ();
	सचे (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  पटिनिवत्त मिच्छा;
	ध्रुव hb_array_t<unsigned पूर्णाङ्क> &indices = has_private_points ? private_indices : shared_indices;

	तार्किक apply_to_all = (indices.length == 0);
	unsigned अङ्कः_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && अङ्कः_deltas >= 4 ? अङ्कः_deltas - 4 : 0);

	सचे (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  सचे (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, सच्च))) पटिनिवत्त मिच्छा;
	  सचे (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, मिच्छा))) पटिनिवत्त मिच्छा;
#अञ्ञथा
	  सचे (unlikely (!decompile_deltas_add_to_points<सच्च> (p, points, scalar, end, start_deltas))) पटिनिवत्त मिच्छा;
	  सचे (unlikely (!decompile_deltas_add_to_points<मिच्छा> (p, points, scalar, end, start_deltas))) पटिनिवत्त मिच्छा;
#endif
	  अनुवर्त;
	}

	सचे (unlikely (!x_deltas.resize_dirty  (अङ्कः_deltas))) पटिनिवत्त मिच्छा;
	सचे (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, मिच्छा, start_deltas))) पटिनिवत्त मिच्छा;
	सचे (unlikely (!y_deltas.resize_dirty  (अङ्कः_deltas))) पटिनिवत्त मिच्छा;
	सचे (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, मिच्छा, start_deltas))) पटिनिवत्त मिच्छा;

	सचे (!apply_to_all)
	{
	  सचे (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    सचे (unlikely (orig_points_vec.in_error ())) पटिनिवत्त मिच्छा;
	    orig_points = orig_points_vec.as_array ();
	  }

	  सचे (flush)
	  {
	    अत्थाय (unsigned पूर्णाङ्क i = phantom_only ? गणनम् - 4 : 0; i < गणनम्; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? गणनम् - 4 : 0), 0,
		     (phantom_only ? 4 : गणनम्) * परिमाण (deltas[0]));
	}

	सचे (HB_OPTIMIZE_SIZE_VAL)
	{
	  अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_deltas; i++)
	  {
	    unsigned पूर्णाङ्क pt_index;
	    सचे (apply_to_all)
	      pt_index = i;
	    अञ्ञथा
	    {
	      pt_index = indices[i];
	      सचे (unlikely (pt_index >= deltas.length)) अनुवर्त;
	    }
	    सचे (phantom_only && pt_index < गणनम् - 4) अनुवर्त;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	अञ्ञथा
	{
	  /* Ouch. Four cases... for optimization. */
	  सचे (scalar != 1.0f)
	  {
	    सचे (apply_to_all)
	      अत्थाय (unsigned पूर्णाङ्क i = phantom_only ? गणनम् - 4 : 0; i < गणनम्; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    अञ्ञथा
	      अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_deltas; i++)
	      {
		unsigned पूर्णाङ्क pt_index = indices[i];
		सचे (unlikely (pt_index >= deltas.length)) अनुवर्त;
		सचे (phantom_only && pt_index < गणनम् - 4) अनुवर्त;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  अञ्ञथा
	  {
	    सचे (apply_to_all)
	      अत्थाय (unsigned पूर्णाङ्क i = phantom_only ? गणनम् - 4 : 0; i < गणनम्; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    अञ्ञथा
	      अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_deltas; i++)
	      {
		unsigned पूर्णाङ्क pt_index = indices[i];
		सचे (unlikely (pt_index >= deltas.length)) अनुवर्त;
		सचे (phantom_only && pt_index < गणनम् - 4) अनुवर्त;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	सचे (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  याव (सच्च)
	  {
	    याव (end_point < गणनम् && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    सचे (unlikely (end_point == गणनम्)) भङ्ग;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_गणनम् = 0;
	    अत्थाय (unsigned i = start_point; i < end_point + 1; i++)
	      unref_गणनम् += deltas.arrayZ[i].flag;
	    unref_गणनम् = (end_point - start_point + 1) - unref_गणनम्;

	    unsigned j = start_point;
	    सचे (unref_गणनम् == 0 || unref_गणनम् > end_point - start_point)
	      गच्छ no_more_gaps;

	    अत्थाय (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned पूर्णाङ्क prev, next, i;
	      अत्थाय (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		सचे (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) भङ्ग;
	      }
	      prev = j = i;
	      अत्थाय (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		सचे (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) भङ्ग;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      अत्थाय (;;)
	      {
		i = next_index (i, start_point, end_point);
		सचे (i == next) भङ्ग;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		सचे (--unref_गणनम् == 0) गच्छ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = सच्च;

      } याव (iterator.move_to_next ());

      सचे (flush)
      {
	अत्थाय (unsigned पूर्णाङ्क i = phantom_only ? गणनम् - 4 : 0; i < गणनम्; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      पटिनिवत्त सच्च;
    }

    unsigned पूर्णाङ्क get_axis_गणनम् () ध्रुव { पटिनिवत्त table->axis_गणनम्_2; }

    गुप्त:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_गणनम्_2;
  };

  रक्षित:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_गणनम्_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_गणनम्;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_गणनम्_Type
		glyph_गणनम्_X;	/* The number of glyphs in this font. This must match the number of
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (18 + Glyph_गणनम्_Type::static_size, offsetZ);
};

संरचना gvar : gvar_GVAR<gvar, SmallTypes>
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

संरचना GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

संरचना gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ध्रुव
  {
    सचे (has_data ()) पटिनिवत्त HB_DUAL_GET (*एतत्, create_cache ());
    पटिनिवत्त nullptr;
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  तार्किक apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ध्रुव पूर्णाङ्क> coords,
			       ध्रुव hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       तार्किक phantom_only = मिच्छा,
			       int64_t *budget = nullptr) ध्रुव
  {
    पटिनिवत्त HB_DUAL_GET (*एतत्, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned पूर्णाङ्क get_axis_गणनम् () ध्रुव
  {
    सचे (has_data ()) पटिनिवत्त HB_DUAL_GET (*एतत्, get_axis_गणनम् ());
    पटिनिवत्त 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
