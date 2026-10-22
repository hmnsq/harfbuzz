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

#ଅନ୍ତର୍ଭାଵ "hb-decycler.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

ସଂରଚନା hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> x_deltas;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> shared_indices;
  hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> private_indices;
};

ନାମାକାଶ OT {

template <typename OffsetType>
ସଂରଚନା glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  ବ୍ୟକ୍ତିଗତ:
  unsigned shared_tuples_ଗଣନା = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  ସର୍ବସାଧାରଣ:
  unsigned compiled_shared_tuples_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ shared_tuples_ଗଣନା; }

  unsigned compiled_byte_size () ଅଚଳ
  {
    unsigned byte_size = 0;
    ପାଇଁ (ଅଚଳ auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    ଫେରନ୍ତୁ byte_size;
  }

  ତାର୍କିକ create_from_glyphs_var_data (unsigned axis_ଗଣନା,
                                    ଅଚଳ hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
                                    ଅଚଳ hb_subset_plan_t *plan,
                                    ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    ଯଦି (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto it = hb_iter (plan->new_to_old_gid_list);
    ପାଇଁ (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      ଯଦି (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ଅଚଳ GlyphVariationData* p = reinterpret_cast<ଅଚଳ GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      ଯଦି (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_ଗଣନା,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        ଜାରି;
      }

      ତାର୍କିକ is_composite_glyph = ମିଥ୍ୟା;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      ଯଦି (!p->decompile_tuple_variations (all_contour_points->length, ସତ୍ୟ /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      glyph_variations.push (std::move (tuple_vars));
    }
    ଫେରନ୍ତୁ !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  ତାର୍କିକ instantiate (ଅଚଳ hb_subset_plan_t *plan)
  {
    unsigned ଗଣନା_2 = plan->new_to_old_gid_list.length;
    ତାର୍କିକ iup_optimize = ମିଥ୍ୟା;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      ଯଦି (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      ଯଦି (iup_optimize)
      {
        points = *all_points;
        ଯଦି (unlikely (points.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      ଯଦି (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      ଯଦି (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ compile_bytes (ଅଚଳ hb_map_t& axes_index_map,
                      ଅଚଳ hb_map_t& axes_old_index_tag_map)
  {
    ଯଦି (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (tuple_variations_t& vars: glyph_variations)
      ଯଦି (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               ସତ୍ୟ, /* use shared points*/
                               ସତ୍ୟ,
                               &shared_tuples_idx_map,
			       &pool))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ compile_shared_tuples (ଅଚଳ hb_map_t& axes_index_map,
                              ଅଚଳ hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned> coords_ଗଣନା_map;

    /* count the num of shared coords */
    ପାଇଁ (tuple_variations_t& vars: glyph_variations)
    {
      ପାଇଁ (tuple_delta_t& var : vars.tuple_vars)
      {
        ଯଦି (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        unsigned *ଗଣନା_2;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        ଯଦି (coords_ଗଣନା_map.has_with_hash (&(var.compiled_peak_coords), hash, &ଗଣନା_2))
	  (*ଗଣନା_2)++;
        ଅନ୍ୟଥା
          coords_ଗଣନା_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    ଯଦି (!coords_ଗଣନା_map || coords_ଗଣନା_map.in_error ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_ଗଣନା_map)
      | hb_filter ([] (ଅଚଳ hb_pair_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned>& p) { ଫେରନ୍ତୁ p.second > 1; })
    };
    ଯଦି (unlikely (shared_coords.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* no shared tuples: no coords are used more than once */
    ଯଦି (!shared_coords) ଫେରନ୍ତୁ ସତ୍ୟ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_ଗଣନା = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_ଗଣନା * (shared_coords[0].first->length);
    ଯଦି (unlikely (!compiled_shared_tuples.alloc (len)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < shared_tuples_ଗଣନା; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      ପାଇଁ (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cmp_coords (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ଅଚଳ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ଅଚଳ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ଅଚଳ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    ଯଦି (a->second != b->second)
      ଫେରନ୍ତୁ b->second - a->second; // high to low

    ଫେରନ୍ତୁ b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 ତାର୍କିକ long_offset,
                                 unsigned ଅଙ୍କ_glyphs,
                                 ଅକ୍ଷର* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    ଅନ୍ୟଥା
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    ଯଦି (!cur_glyph) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      ଯଦି (long_offset)
        ପାଇଁ (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      ଅନ୍ୟଥା
        ପାଇଁ (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      ଯଦି (idx >= glyph_variations.length) return_trace (ମିଥ୍ୟା);
      ଯଦି (!cur_glyph->serialize (c, ସତ୍ୟ, glyph_variations[idx])) return_trace (ମିଥ୍ୟା);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (ଅକ୍ଷର *) next_glyph - (ଅକ୍ଷର *) cur_glyph;

      ଯଦି (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      ଅନ୍ୟଥା
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    ଯଦି (long_offset)
      ପାଇଁ (; last_gid < ଅଙ୍କ_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    ଅନ୍ୟଥା
      ପାଇଁ (; last_gid < ଅଙ୍କ_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (ସତ୍ୟ);
  }
};

template <typename T, typename Types>
ସଂରଚନା gvar_GVAR
{
  using Glyph_ଗଣନା_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_ଗଣନା_Type>;
  ସ୍ଥିର constexpr unsigned glyph_ଗଣନା_max ()
  { ଫେରନ୍ତୁ (1u << (8 * Glyph_ଗଣନା_Type::static_size)) - 1; }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int () != 0; }

  ତାର୍କିକ sanitize_shallow (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, ଏହି, axis_ଗଣନା_2 * shared_Tuple_ଗଣନା) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_ଅଙ୍କ_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_ଅଙ୍କ_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ sanitize_shallow (c); }

  ତାର୍କିକ decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_ଗଣନା_Type>& glyph_vars /* OUT */) ଅଚଳ
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    ଯଦି (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    ପାଇଁ (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ଗଣନା_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    ଯଦି (new_gid_var_data_map.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_array_t<ଅଚଳ F2DOT14> shared_tuples = (ଏହି+sharedTuples).as_array ((unsigned) shared_Tuple_ଗଣନା * (unsigned) axis_ଗଣନା_2);
    ଫେରନ୍ତୁ glyph_vars.create_from_glyphs_var_data (axis_ଗଣନା_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ଅଚଳ glyph_variations_t<Glyph_ଗଣନା_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_ଗଣନା,
                  unsigned ଅଙ୍କ_glyphs,
                  ତାର୍କିକ force_long_offsets) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ଗଣନା_2 = axis_ଗଣନା;
    out->glyph_ଗଣନା_X = hb_min (glyph_ଗଣନା_max (), ଅଙ୍କ_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ତାର୍କିକ long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (ଅଙ୍କ_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, ମିଥ୍ୟା);
    ଯଦି (!glyph_var_data_offsets) return_trace (ମିଥ୍ୟା);

    /* shared tuples */
    unsigned shared_tuple_ଗଣନା = glyph_vars.compiled_shared_tuples_ଗଣନା ();
    out->shared_Tuple_ଗଣନା = shared_tuple_ଗଣନା;

    ଯଦି (!shared_tuple_ଗଣନା)
      out->sharedTuples = (ଅକ୍ଷର *) glyph_var_data_offsets + glyph_var_data_offsets_size - (ଅକ୍ଷର *) out;
    ଅନ୍ୟଥା
    {
      hb_array_t<ଅଚଳ F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      ଯଦି (!shared_tuples.arrayZ) return_trace (ମିଥ୍ୟା);
      out->sharedTuples = (ଅଚଳ ଅକ୍ଷର *) shared_tuples.arrayZ - (ଅକ୍ଷର *) out;
    }

    ଅକ୍ଷର *glyph_var_data = c->start_embed<ଅକ୍ଷର> ();
    ଯଦି (!glyph_var_data) return_trace (ମିଥ୍ୟା);
    out->dataZ = glyph_var_data - (ଅକ୍ଷର *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, ଅଙ୍କ_glyphs,
                                                       (ଅକ୍ଷର *) glyph_var_data_offsets));
  }

  ତାର୍କିକ instantiate (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    glyph_variations_t<Glyph_ଗଣନା_Type> glyph_vars;
    ଯଦି (!decompile_glyph_variations (c, glyph_vars))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (!glyph_vars.instantiate (c->plan)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (ମିଥ୍ୟା);

    unsigned axis_ଗଣନା = c->plan->axes_index_map.get_population ();
    unsigned ଅଙ୍କ_glyphs = c->plan->ଅଙ୍କ_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    ତାର୍କିକ force_long_offsets = ମିଥ୍ୟା;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_ଗଣନା, ଅଙ୍କ_glyphs, force_long_offsets));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ମିଥ୍ୟା);

    ଯଦି (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_ଗଣନା = version.to_int () ? c->plan->source->get_ଅଙ୍କ_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ଗଣନା_2 = axis_ଗଣନା_2;
    out->shared_Tuple_ଗଣନା = shared_Tuple_ଗଣନା;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = c->plan->ଅଙ୍କ_output_glyphs ();
    out->glyph_ଗଣନା_X = hb_min (glyph_ଗଣନା_max (), ଅଙ୍କ_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<ଅକ୍ଷର>> culled_vars;
    hb_set_t culled_gids;
    ଯଦି (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ଅଚଳ F2DOT14> shared_tuples_array =
	  (ଏହି+sharedTuples).as_array ((unsigned) shared_Tuple_ଗଣନା * (unsigned) axis_ଗଣନା_2);
      ଯଦି (unlikely (!culled_vars.resize (ଅଙ୍କ_glyphs))) return_trace (ମିଥ୍ୟା);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      ଯଦି (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      ପାଇଁ (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ଗଣନା, _.second);
	ଯଦି (var_data_bytes.length < GlyphVariationData::min_size) ଜାରି;
	ତାର୍କିକ changed = ମିଥ୍ୟା;
	ଯଦି (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_ଗଣନା_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (ମିଥ୍ୟା);
	ଯଦି (changed) culled_gids.add (_.first);
      }
      ଯଦି (unlikely (culled_gids.in_error ())) return_trace (ମିଥ୍ୟା);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    ଯଦି (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    ପାଇଁ (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_ଗଣନା, old_gid).length;
      ଯଦି (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      ଯଦି (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (ମିଥ୍ୟା);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ତାର୍କିକ long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (ଅଙ୍କ_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, ମିଥ୍ୟା);
    ଯଦି (!subset_offsets) return_trace (ମିଥ୍ୟା);

    /* shared tuples */
    ଯଦି (!shared_Tuple_ଗଣନା || !sharedTuples)
      out->sharedTuples = (ଅକ୍ଷର *) subset_offsets + subset_offsets_size - (ଅକ୍ଷର *) out;
    ଅନ୍ୟଥା
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shared_tuple_size = F2DOT14::static_size * axis_ଗଣନା_2 * shared_Tuple_ଗଣନା;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      ଯଦି (!tuples) return_trace (ମିଥ୍ୟା);
      out->sharedTuples = (ଅକ୍ଷର *) tuples - (ଅକ୍ଷର *) out;
      hb_memcpy (tuples, ଏହି+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    ଯଦି (long_offset)
      subset_data_size -= padding_size;
    ଅକ୍ଷର *subset_data = c->serializer->allocate_size<ଅକ୍ଷର> (subset_data_size, ମିଥ୍ୟା);
    ଯଦି (!subset_data) return_trace (ମିଥ୍ୟା);
    out->dataZ = subset_data - (ଅକ୍ଷର *) out;


    ଯଦି (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    ଅନ୍ୟଥା
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    ଯଦି (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    ପାଇଁ (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      ଯଦି (long_offset)
	ପାଇଁ (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      ଅନ୍ୟଥା
	ପାଇଁ (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_ଗଣନା,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      ଯଦି (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      ଯଦି (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      ଅନ୍ୟଥା
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    ଯଦି (long_offset)
      ପାଇଁ (; last < ଅଙ୍କ_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    ଅନ୍ୟଥା
      ପାଇଁ (; last < ଅଙ୍କ_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  ଅଚଳ hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_ଗଣନା,
					     hb_codepoint_t glyph) ଅଚଳ
  {
    unsigned start_offset = get_offset (glyph_ଗଣନା, glyph);
    unsigned end_offset = get_offset (glyph_ଗଣନା, glyph+1);
    ଯଦି (unlikely (end_offset < start_offset)) ଫେରନ୍ତୁ hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    ଫେରନ୍ତୁ likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  ତାର୍କିକ is_long_offset () ଅଚଳ { ଫେରନ୍ତୁ flags & 1; }

  unsigned get_offset (unsigned glyph_ଗଣନା, unsigned i) ଅଚଳ
  {
    ଯଦି (unlikely (i > glyph_ଗଣନା)) ଫେରନ୍ତୁ 0;
    hb_barrier ();
    ଫେରନ୍ତୁ is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ଅଚଳ HBUINT32 * get_long_offset_array () ଅଚଳ { ଫେରନ୍ତୁ (ଅଚଳ HBUINT32 *) &offsetZ; }
  ଅଚଳ HBUINT16 *get_short_offset_array () ଅଚଳ { ଫେରନ୍ତୁ (ଅଚଳ HBUINT16 *) &offsetZ; }

  ସର୍ବସାଧାରଣ:
  ସଂରଚନା accelerator_t
  {

    hb_scalar_cache_t *create_cache () ଅଚଳ
    {
      ଫେରନ୍ତୁ hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_ଗଣନା,
						+TupleVariationHeader::max_shared_tuple_ଗଣନା));
    }

    ସ୍ଥିର ଶୂନ୍ୟ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_ଗଣନା_2 = table->version.to_int () ? face->get_ଅଙ୍କ_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    ବ୍ୟକ୍ତିଗତ:

    ସ୍ଥିର ଦଶମିକ infer_delta (ଅଚଳ hb_array_t<contour_point_t> points,
			      ଅଚଳ hb_array_t<contour_point_t> deltas,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା target, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next,
			      ଦଶମିକ contour_point_t::*m)
    {
      ଦଶମିକ target_val = points.arrayZ[target].*m;
      ଦଶମିକ prev_val = points.arrayZ[prev].*m;
      ଦଶମିକ next_val = points.arrayZ[next].*m;
      ଦଶମିକ prev_delta =  deltas.arrayZ[prev].*m;
      ଦଶମିକ next_delta =  deltas.arrayZ[next].*m;

      ଯଦି (prev_val == next_val)
	ଫେରନ୍ତୁ (prev_delta == next_delta) ? prev_delta : 0.f;
      ଅନ୍ୟଥା ଯଦି (target_val <= hb_min (prev_val, next_val))
	ଫେରନ୍ତୁ (prev_val < next_val) ? prev_delta : next_delta;
      ଅନ୍ୟଥା ଯଦି (target_val >= hb_max (prev_val, next_val))
	ଫେରନ୍ତୁ (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      ଦଶମିକ r = (target_val - prev_val) / (next_val - prev_val);
      ଫେରନ୍ତୁ prev_delta + r * (next_delta - prev_delta);
    }

    ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
    { ଫେରନ୍ତୁ (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <ତାର୍କିକ is_x>
#endif
    ସ୍ଥିର ତାର୍କିକ decompile_deltas_add_to_points (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						ଦଶମିକ scalar,
						ଅଚଳ HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, ତାର୍କିକ is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned ଗଣନା_2 = points.length;
      ଯେତେବେଳେ (i < ଗଣନା_2)
      {
	ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	unsigned control = *p++;
	unsigned run_ଗଣନା = (control & Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟ_RUN_ଗଣନା_MASK) + 1;
	unsigned stop = i + run_ଗଣନା;
	ଯଦି (unlikely (stop > ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

	unsigned skip = i < start ? hb_min (start - i, run_ଗଣନା) : 0;
	i += skip;

	ବାଛ (control & Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟଗୁଡ଼ିକ_SIZE_MASK)
	{
	  ସ୍ଥିତି Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ZEROS:
	    i = stop;
	    ଭାଙ୍ଗ;
	  ସ୍ଥିତି Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_ଶବ୍ଦଗୁଡ଼ିକ:
	  {
	    ଯଦି (unlikely (p + run_ଗଣନା * HBINT16::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    p += skip * HBINT16::static_size;
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT16 *) p;
	    ପାଇଁ (; i < stop; i++)
	    {
	      ଦଶମିକ v = *pp++ * scalar;
	      ଯଦି (is_x) points.arrayZ[i].x += v;
	      ଅନ୍ୟଥା points.arrayZ[i].y += v;
	    }
	    p = (ଅଚଳ HBUINT8 *) pp;
	  }
	  ଭାଙ୍ଗ;
	  ସ୍ଥିତି Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_LONGS:
	  {
	    ଯଦି (unlikely (p + run_ଗଣନା * HBINT32::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    p += skip * HBINT32::static_size;
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT32 *) p;
	    ପାଇଁ (; i < stop; i++)
	    {
	      ଦଶମିକ v = *pp++ * scalar;
	      ଯଦି (is_x) points.arrayZ[i].x += v;
	      ଅନ୍ୟଥା points.arrayZ[i].y += v;
	    }
	    p = (ଅଚଳ HBUINT8 *) pp;
	  }
	  ଭାଙ୍ଗ;
	  ସ୍ଥିତି Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::ମୂଲ୍ୟଗୁଡ଼ିକ_ARE_BYTES:
	  {
	    ଯଦି (unlikely (p + run_ଗଣନା > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    p += skip * HBINT8::static_size;
	    ଅଚଳ auto *pp = (ଅଚଳ HBINT8 *) p;
	    ପାଇଁ (; i < stop; i++)
	    {
	      ଦଶମିକ v = *pp++ * scalar;
	      ଯଦି (is_x) points.arrayZ[i].x += v;
	      ଅନ୍ୟଥା points.arrayZ[i].y += v;
	    }
	    p = (ଅଚଳ HBUINT8 *) pp;
	  }
	  ଭାଙ୍ଗ;
	}
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ସର୍ବସାଧାରଣ:
    ତାର୍କିକ apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
				 ଅଚଳ hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 ତାର୍କିକ phantom_only = ମିଥ୍ୟା,
				 int64_t *budget = nullptr) ଅଚଳ
    {
      ଯଦି (unlikely (glyph >= glyph_ଗଣନା_2)) ଫେରନ୍ତୁ ସତ୍ୟ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_ଗଣନା_2, glyph);
      ଯଦି (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) ଫେରନ୍ତୁ ସତ୍ୟ;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      ଯଦି (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_ଗଣନା_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	ଫେରନ୍ତୁ ସତ୍ୟ; /* so isn't applied at all */

      ତାର୍କିକ any_private_points = ମିଥ୍ୟା;
      ତାର୍କିକ private_points_checked = ମିଥ୍ୟା;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned ଅଙ୍କ_coords = table->axis_ଗଣନା_2;
      hb_array_t<ଅଚଳ F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_ଗଣନା * ଅଙ୍କ_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned ଗଣନା_2 = points.length;
      ତାର୍କିକ flush = ମିଥ୍ୟା;

      କର
      {
	ଦଶମିକ scalar = iterator.current_tuple->calculate_scalar (coords, ଅଙ୍କ_coords, shared_tuples,
								 scalar_cache);

	ଯଦି (scalar == 0.f) ଜାରି;

	ଯଦି (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && ଗଣନା_2 >= 4 ? 4 : ଗଣନା_2)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

	ଯଦି (!private_points_checked)
	{
	  auto scan = iterator;
	  କର
	  {
	    ଯଦି (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = ସତ୍ୟ;
	      ଭାଙ୍ଗ;
	    }
	  } ଯେତେବେଳେ (scan.move_to_next ());
	  private_points_checked = ସତ୍ୟ;
	}
	ଅଚଳ HBUINT8 *p = iterator.get_serialized_data ();
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = iterator.current_tuple->get_data_size ();
	ଯଦି (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

	ଯଦି (!deltas)
	{
	  ଯଦି (unlikely (!deltas_vec.resize_dirty  (ଗଣନା_2))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? ଗଣନା_2 - 4 : 0), 0,
		     (phantom_only ? 4 : ଗଣନା_2) * ଆକାର (deltas[0]));
	}

	ଅଚଳ HBUINT8 *end = p + length;

	ତାର୍କିକ has_private_points = iterator.current_tuple->has_private_points ();
	ଯଦି (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଅଚଳ hb_array_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &indices = has_private_points ? private_indices : shared_indices;

	ତାର୍କିକ apply_to_all = (indices.length == 0);
	unsigned ଅଙ୍କ_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && ଅଙ୍କ_deltas >= 4 ? ଅଙ୍କ_deltas - 4 : 0);

	ଯଦି (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  ଯଦି (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, ସତ୍ୟ))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  ଯଦି (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, ମିଥ୍ୟା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା
	  ଯଦି (unlikely (!decompile_deltas_add_to_points<ସତ୍ୟ> (p, points, scalar, end, start_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  ଯଦି (unlikely (!decompile_deltas_add_to_points<ମିଥ୍ୟା> (p, points, scalar, end, start_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
	  ଜାରି;
	}

	ଯଦି (unlikely (!x_deltas.resize_dirty  (ଅଙ୍କ_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଯଦି (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, ମିଥ୍ୟା, start_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଯଦି (unlikely (!y_deltas.resize_dirty  (ଅଙ୍କ_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ଯଦି (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, ମିଥ୍ୟା, start_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

	ଯଦି (!apply_to_all)
	{
	  ଯଦି (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    ଯଦି (unlikely (orig_points_vec.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    orig_points = orig_points_vec.as_array ();
	  }

	  ଯଦି (flush)
	  {
	    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = phantom_only ? ଗଣନା_2 - 4 : 0; i < ଗଣନା_2; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? ଗଣନା_2 - 4 : 0), 0,
		     (phantom_only ? 4 : ଗଣନା_2) * ଆକାର (deltas[0]));
	}

	ଯଦି (HB_OPTIMIZE_SIZE_VAL)
	{
	  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_deltas; i++)
	  {
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pt_index;
	    ଯଦି (apply_to_all)
	      pt_index = i;
	    ଅନ୍ୟଥା
	    {
	      pt_index = indices[i];
	      ଯଦି (unlikely (pt_index >= deltas.length)) ଜାରି;
	    }
	    ଯଦି (phantom_only && pt_index < ଗଣନା_2 - 4) ଜାରି;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	ଅନ୍ୟଥା
	{
	  /* Ouch. Four cases... for optimization. */
	  ଯଦି (scalar != 1.0f)
	  {
	    ଯଦି (apply_to_all)
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = phantom_only ? ଗଣନା_2 - 4 : 0; i < ଗଣନା_2; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    ଅନ୍ୟଥା
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_deltas; i++)
	      {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pt_index = indices[i];
		ଯଦି (unlikely (pt_index >= deltas.length)) ଜାରି;
		ଯଦି (phantom_only && pt_index < ଗଣନା_2 - 4) ଜାରି;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  ଅନ୍ୟଥା
	  {
	    ଯଦି (apply_to_all)
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = phantom_only ? ଗଣନା_2 - 4 : 0; i < ଗଣନା_2; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    ଅନ୍ୟଥା
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_deltas; i++)
	      {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pt_index = indices[i];
		ଯଦି (unlikely (pt_index >= deltas.length)) ଜାରି;
		ଯଦି (phantom_only && pt_index < ଗଣନା_2 - 4) ଜାରି;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	ଯଦି (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  ଯେତେବେଳେ (ସତ୍ୟ)
	  {
	    ଯେତେବେଳେ (end_point < ଗଣନା_2 && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    ଯଦି (unlikely (end_point == ଗଣନା_2)) ଭାଙ୍ଗ;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_ଗଣନା = 0;
	    ପାଇଁ (unsigned i = start_point; i < end_point + 1; i++)
	      unref_ଗଣନା += deltas.arrayZ[i].flag;
	    unref_ଗଣନା = (end_point - start_point + 1) - unref_ଗଣନା;

	    unsigned j = start_point;
	    ଯଦି (unref_ଗଣନା == 0 || unref_ଗଣନା > end_point - start_point)
	      ଯାଅ no_more_gaps;

	    ପାଇଁ (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev, next, i;
	      ପାଇଁ (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		ଯଦି (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) ଭାଙ୍ଗ;
	      }
	      prev = j = i;
	      ପାଇଁ (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		ଯଦି (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) ଭାଙ୍ଗ;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      ପାଇଁ (;;)
	      {
		i = next_index (i, start_point, end_point);
		ଯଦି (i == next) ଭାଙ୍ଗ;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		ଯଦି (--unref_ଗଣନା == 0) ଯାଅ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = ସତ୍ୟ;

      } ଯେତେବେଳେ (iterator.move_to_next ());

      ଯଦି (flush)
      {
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = phantom_only ? ଗଣନା_2 - 4 : 0; i < ଗଣନା_2; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ table->axis_ଗଣନା_2; }

    ବ୍ୟକ୍ତିଗତ:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_ଗଣନା_2;
  };

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_ଗଣନା_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_ଗଣନା;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_ଗଣନା_Type
		glyph_ଗଣନା_X;	/* The number of glyphs in this font. This must match the number of
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (18 + Glyph_ଗଣନା_Type::static_size, offsetZ);
};

ସଂରଚନା gvar : gvar_GVAR<gvar, SmallTypes>
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

ସଂରଚନା GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

ସଂରଚନା gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ଅଚଳ
  {
    ଯଦି (has_data ()) ଫେରନ୍ତୁ HB_DUAL_GET (*ଏହି, create_cache ());
    ଫେରନ୍ତୁ nullptr;
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ତାର୍କିକ apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
			       ଅଚଳ hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       ତାର୍କିକ phantom_only = ମିଥ୍ୟା,
			       int64_t *budget = nullptr) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DUAL_GET (*ଏହି, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_ଗଣନା () ଅଚଳ
  {
    ଯଦି (has_data ()) ଫେରନ୍ତୁ HB_DUAL_GET (*ଏହି, get_axis_ଗଣନା ());
    ଫେରନ୍ତୁ 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
