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

#அந்தர்பாவ "hb-decycler.hh"
#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-dual.hh"
#அந்தர்பாவ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

கட்டமைப்பு hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<முழு> x_deltas;
  hb_vector_t<முழு> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned முழு> shared_indices;
  hb_vector_t<unsigned முழு> private_indices;
};

நாமாகாஶ OT {

template <typename OffsetType>
கட்டமைப்பு glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  தனிப்பட்ட:
  unsigned shared_tuples_மொத்தம் = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  பொது:
  unsigned compiled_shared_tuples_மொத்தம் () மாறிலி
  { திரும்பு shared_tuples_மொத்தம்; }

  unsigned compiled_byte_size () மாறிலி
  {
    unsigned byte_size = 0;
    க்கு (மாறிலி auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    திரும்பு byte_size;
  }

  தருக்க create_from_glyphs_var_data (unsigned axis_மொத்தம்,
                                    மாறிலி hb_array_t<மாறிலி F2DOT14> shared_tuples,
                                    மாறிலி hb_subset_plan_t *plan,
                                    மாறிலி hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    எனில் (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      திரும்பு பொய்;

    auto it = hb_iter (plan->new_to_old_gid_list);
    க்கு (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      எனில் (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        திரும்பு பொய்;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      மாறிலி GlyphVariationData* p = reinterpret_cast<மாறிலி GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      எனில் (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_மொத்தம்,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        தொடர்;
      }

      தருக்க is_composite_glyph = பொய்;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      எனில் (!p->decompile_tuple_variations (all_contour_points->length, உண்மை /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        திரும்பு பொய்;
      glyph_variations.push (std::move (tuple_vars));
    }
    திரும்பு !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  தருக்க instantiate (மாறிலி hb_subset_plan_t *plan)
  {
    unsigned மொத்தம் = plan->new_to_old_gid_list.length;
    தருக்க iup_optimize = பொய்;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      எனில் (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        திரும்பு பொய்;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      எனில் (iup_optimize)
      {
        points = *all_points;
        எனில் (unlikely (points.in_error ())) திரும்பு பொய்;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      எனில் (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      எனில் (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  தருக்க compile_bytes (மாறிலி hb_map_t& axes_index_map,
                      மாறிலி hb_map_t& axes_old_index_tag_map)
  {
    எனில் (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      திரும்பு பொய்;
    க்கு (tuple_variations_t& vars: glyph_variations)
      எனில் (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               உண்மை, /* use shared points*/
                               உண்மை,
                               &shared_tuples_idx_map,
			       &pool))
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க compile_shared_tuples (மாறிலி hb_map_t& axes_index_map,
                              மாறிலி hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned> coords_மொத்தம்_map;

    /* count the num of shared coords */
    க்கு (tuple_variations_t& vars: glyph_variations)
    {
      க்கு (tuple_delta_t& var : vars.tuple_vars)
      {
        எனில் (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          திரும்பு பொய்;
        unsigned *மொத்தம்;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        எனில் (coords_மொத்தம்_map.has_with_hash (&(var.compiled_peak_coords), hash, &மொத்தம்))
	  (*மொத்தம்)++;
        இல்லையெனில்
          coords_மொத்தம்_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    எனில் (!coords_மொத்தம்_map || coords_மொத்தம்_map.in_error ())
      திரும்பு பொய்;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_மொத்தம்_map)
      | hb_filter ([] (மாறிலி hb_pair_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned>& p) { திரும்பு p.second > 1; })
    };
    எனில் (unlikely (shared_coords.in_error ())) திரும்பு பொய்;

    /* no shared tuples: no coords are used more than once */
    எனில் (!shared_coords) திரும்பு உண்மை;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_மொத்தம் = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_மொத்தம் * (shared_coords[0].first->length);
    எனில் (unlikely (!compiled_shared_tuples.alloc (len)))
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < shared_tuples_மொத்தம்; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      க்கு (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    திரும்பு உண்மை;
  }

  நிலையான முழு _cmp_coords (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (மாறிலி hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    மாறிலி hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (மாறிலி hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    எனில் (a->second != b->second)
      திரும்பு b->second - a->second; // high to low

    திரும்பு b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 தருக்க long_offset,
                                 unsigned இலக்கம்_glyphs,
                                 எழுத்து* glyph_var_data_offsets /* OUT: glyph var data offsets array */) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    எனில் (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    இல்லையெனில்
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    எனில் (!cur_glyph) return_trace (பொய்);
    க்கு (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      எனில் (long_offset)
        க்கு (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      இல்லையெனில்
        க்கு (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      எனில் (idx >= glyph_variations.length) return_trace (பொய்);
      எனில் (!cur_glyph->serialize (c, உண்மை, glyph_variations[idx])) return_trace (பொய்);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (எழுத்து *) next_glyph - (எழுத்து *) cur_glyph;

      எனில் (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      இல்லையெனில்
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    எனில் (long_offset)
      க்கு (; last_gid < இலக்கம்_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    இல்லையெனில்
      க்கு (; last_gid < இலக்கம்_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (உண்மை);
  }
};

template <typename T, typename Types>
கட்டமைப்பு gvar_GVAR
{
  using Glyph_மொத்தம்_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_மொத்தம்_Type>;
  நிலையான constexpr unsigned glyph_மொத்தம்_max ()
  { திரும்பு (1u << (8 * Glyph_மொத்தம்_Type::static_size)) - 1; }

  தருக்க has_data () மாறிலி { திரும்பு version.to_int () != 0; }

  தருக்க sanitize_shallow (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, இது, axis_மொத்தம்_2 * shared_Tuple_மொத்தம்) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_இலக்கம்_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_இலக்கம்_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  { திரும்பு sanitize_shallow (c); }

  தருக்க decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_மொத்தம்_Type>& glyph_vars /* OUT */) மாறிலி
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    எனில் (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    க்கு (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_மொத்தம்_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    எனில் (new_gid_var_data_map.in_error ()) திரும்பு பொய்;

    hb_array_t<மாறிலி F2DOT14> shared_tuples = (இது+sharedTuples).as_array ((unsigned) shared_Tuple_மொத்தம் * (unsigned) axis_மொத்தம்_2);
    திரும்பு glyph_vars.create_from_glyphs_var_data (axis_மொத்தம்_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize (hb_serialize_context_t *c,
                  மாறிலி glyph_variations_t<Glyph_மொத்தம்_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_மொத்தம்,
                  unsigned இலக்கம்_glyphs,
                  தருக்க force_long_offsets) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_மொத்தம்_2 = axis_மொத்தம்;
    out->glyph_மொத்தம்_X = hb_min (glyph_மொத்தம்_max (), இலக்கம்_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    தருக்க long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (இலக்கம்_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, பொய்);
    எனில் (!glyph_var_data_offsets) return_trace (பொய்);

    /* shared tuples */
    unsigned shared_tuple_மொத்தம் = glyph_vars.compiled_shared_tuples_மொத்தம் ();
    out->shared_Tuple_மொத்தம் = shared_tuple_மொத்தம்;

    எனில் (!shared_tuple_மொத்தம்)
      out->sharedTuples = (எழுத்து *) glyph_var_data_offsets + glyph_var_data_offsets_size - (எழுத்து *) out;
    இல்லையெனில்
    {
      hb_array_t<மாறிலி F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      எனில் (!shared_tuples.arrayZ) return_trace (பொய்);
      out->sharedTuples = (மாறிலி எழுத்து *) shared_tuples.arrayZ - (எழுத்து *) out;
    }

    எழுத்து *glyph_var_data = c->start_embed<எழுத்து> ();
    எனில் (!glyph_var_data) return_trace (பொய்);
    out->dataZ = glyph_var_data - (எழுத்து *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, இலக்கம்_glyphs,
                                                       (எழுத்து *) glyph_var_data_offsets));
  }

  தருக்க instantiate (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    glyph_variations_t<Glyph_மொத்தம்_Type> glyph_vars;
    எனில் (!decompile_glyph_variations (c, glyph_vars))
      return_trace (பொய்);

    எனில் (!glyph_vars.instantiate (c->plan)) return_trace (பொய்);
    எனில் (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (பொய்);

    unsigned axis_மொத்தம் = c->plan->axes_index_map.get_population ();
    unsigned இலக்கம்_glyphs = c->plan->இலக்கம்_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    தருக்க force_long_offsets = பொய்;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_மொத்தம், இலக்கம்_glyphs, force_long_offsets));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned)
      return_trace (பொய்);

    எனில் (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_மொத்தம் = version.to_int () ? c->plan->source->get_இலக்கம்_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_மொத்தம்_2 = axis_மொத்தம்_2;
    out->shared_Tuple_மொத்தம் = shared_Tuple_மொத்தம்;

    unsigned முழு இலக்கம்_glyphs = c->plan->இலக்கம்_output_glyphs ();
    out->glyph_மொத்தம்_X = hb_min (glyph_மொத்தம்_max (), இலக்கம்_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<எழுத்து>> culled_vars;
    hb_set_t culled_gids;
    எனில் (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<மாறிலி F2DOT14> shared_tuples_array =
	  (இது+sharedTuples).as_array ((unsigned) shared_Tuple_மொத்தம் * (unsigned) axis_மொத்தம்_2);
      எனில் (unlikely (!culled_vars.resize (இலக்கம்_glyphs))) return_trace (பொய்);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      எனில் (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      க்கு (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_மொத்தம், _.second);
	எனில் (var_data_bytes.length < GlyphVariationData::min_size) தொடர்;
	தருக்க changed = பொய்;
	எனில் (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_மொத்தம்_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (பொய்);
	எனில் (changed) culled_gids.add (_.first);
      }
      எனில் (unlikely (culled_gids.in_error ())) return_trace (பொய்);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    எனில் (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    க்கு (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_மொத்தம், old_gid).length;
      எனில் (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      எனில் (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (பொய்);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    தருக்க long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (இலக்கம்_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, பொய்);
    எனில் (!subset_offsets) return_trace (பொய்);

    /* shared tuples */
    எனில் (!shared_Tuple_மொத்தம் || !sharedTuples)
      out->sharedTuples = (எழுத்து *) subset_offsets + subset_offsets_size - (எழுத்து *) out;
    இல்லையெனில்
    {
      unsigned முழு shared_tuple_size = F2DOT14::static_size * axis_மொத்தம்_2 * shared_Tuple_மொத்தம்;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      எனில் (!tuples) return_trace (பொய்);
      out->sharedTuples = (எழுத்து *) tuples - (எழுத்து *) out;
      hb_memcpy (tuples, இது+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    எனில் (long_offset)
      subset_data_size -= padding_size;
    எழுத்து *subset_data = c->serializer->allocate_size<எழுத்து> (subset_data_size, பொய்);
    எனில் (!subset_data) return_trace (பொய்);
    out->dataZ = subset_data - (எழுத்து *) out;


    எனில் (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    இல்லையெனில்
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned முழு glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    எனில் (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    க்கு (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      எனில் (long_offset)
	க்கு (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      இல்லையெனில்
	க்கு (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_மொத்தம்,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      எனில் (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      எனில் (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      இல்லையெனில்
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    எனில் (long_offset)
      க்கு (; last < இலக்கம்_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    இல்லையெனில்
      க்கு (; last < இலக்கம்_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  மாறிலி hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_மொத்தம்,
					     hb_codepoint_t glyph) மாறிலி
  {
    unsigned start_offset = get_offset (glyph_மொத்தம், glyph);
    unsigned end_offset = get_offset (glyph_மொத்தம், glyph+1);
    எனில் (unlikely (end_offset < start_offset)) திரும்பு hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    திரும்பு likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  தருக்க is_long_offset () மாறிலி { திரும்பு flags & 1; }

  unsigned get_offset (unsigned glyph_மொத்தம், unsigned i) மாறிலி
  {
    எனில் (unlikely (i > glyph_மொத்தம்)) திரும்பு 0;
    hb_barrier ();
    திரும்பு is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  மாறிலி HBUINT32 * get_long_offset_array () மாறிலி { திரும்பு (மாறிலி HBUINT32 *) &offsetZ; }
  மாறிலி HBUINT16 *get_short_offset_array () மாறிலி { திரும்பு (மாறிலி HBUINT16 *) &offsetZ; }

  பொது:
  கட்டமைப்பு accelerator_t
  {

    hb_scalar_cache_t *create_cache () மாறிலி
    {
      திரும்பு hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_மொத்தம்,
						+TupleVariationHeader::max_shared_tuple_மொத்தம்));
    }

    நிலையான வெறுமை destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    தருக்க has_data () மாறிலி { திரும்பு table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_மொத்தம்_2 = table->version.to_int () ? face->get_இலக்கம்_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    தனிப்பட்ட:

    நிலையான தசம infer_delta (மாறிலி hb_array_t<contour_point_t> points,
			      மாறிலி hb_array_t<contour_point_t> deltas,
			      unsigned முழு target, unsigned முழு prev, unsigned முழு next,
			      தசம contour_point_t::*m)
    {
      தசம target_val = points.arrayZ[target].*m;
      தசம prev_val = points.arrayZ[prev].*m;
      தசம next_val = points.arrayZ[next].*m;
      தசம prev_delta =  deltas.arrayZ[prev].*m;
      தசம next_delta =  deltas.arrayZ[next].*m;

      எனில் (prev_val == next_val)
	திரும்பு (prev_delta == next_delta) ? prev_delta : 0.f;
      இல்லையெனில் எனில் (target_val <= hb_min (prev_val, next_val))
	திரும்பு (prev_val < next_val) ? prev_delta : next_delta;
      இல்லையெனில் எனில் (target_val >= hb_max (prev_val, next_val))
	திரும்பு (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      தசம r = (target_val - prev_val) / (next_val - prev_val);
      திரும்பு prev_delta + r * (next_delta - prev_delta);
    }

    நிலையான unsigned முழு next_index (unsigned முழு i, unsigned முழு start, unsigned முழு end)
    { திரும்பு (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <தருக்க is_x>
#endif
    நிலையான தருக்க decompile_deltas_add_to_points (மாறிலி HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						தசம scalar,
						மாறிலி HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, தருக்க is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned மொத்தம் = points.length;
      போது (i < மொத்தம்)
      {
	எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
	unsigned control = *p++;
	unsigned run_மொத்தம் = (control & Tuple_மதிப்புகள்::மதிப்பு_RUN_மொத்தம்_MASK) + 1;
	unsigned stop = i + run_மொத்தம்;
	எனில் (unlikely (stop > மொத்தம்)) திரும்பு பொய்;

	unsigned skip = i < start ? hb_min (start - i, run_மொத்தம்) : 0;
	i += skip;

	தேர்வு (control & Tuple_மதிப்புகள்::மதிப்புகள்_SIZE_MASK)
	{
	  நிலை Tuple_மதிப்புகள்::மதிப்புகள்_ARE_ZEROS:
	    i = stop;
	    நிறுத்து;
	  நிலை Tuple_மதிப்புகள்::மதிப்புகள்_ARE_சொற்கள்:
	  {
	    எனில் (unlikely (p + run_மொத்தம் * HBINT16::static_size > end)) திரும்பு பொய்;
	    p += skip * HBINT16::static_size;
	    மாறிலி auto *pp = (மாறிலி HBINT16 *) p;
	    க்கு (; i < stop; i++)
	    {
	      தசம v = *pp++ * scalar;
	      எனில் (is_x) points.arrayZ[i].x += v;
	      இல்லையெனில் points.arrayZ[i].y += v;
	    }
	    p = (மாறிலி HBUINT8 *) pp;
	  }
	  நிறுத்து;
	  நிலை Tuple_மதிப்புகள்::மதிப்புகள்_ARE_LONGS:
	  {
	    எனில் (unlikely (p + run_மொத்தம் * HBINT32::static_size > end)) திரும்பு பொய்;
	    p += skip * HBINT32::static_size;
	    மாறிலி auto *pp = (மாறிலி HBINT32 *) p;
	    க்கு (; i < stop; i++)
	    {
	      தசம v = *pp++ * scalar;
	      எனில் (is_x) points.arrayZ[i].x += v;
	      இல்லையெனில் points.arrayZ[i].y += v;
	    }
	    p = (மாறிலி HBUINT8 *) pp;
	  }
	  நிறுத்து;
	  நிலை Tuple_மதிப்புகள்::மதிப்புகள்_ARE_BYTES:
	  {
	    எனில் (unlikely (p + run_மொத்தம் > end)) திரும்பு பொய்;
	    p += skip * HBINT8::static_size;
	    மாறிலி auto *pp = (மாறிலி HBINT8 *) p;
	    க்கு (; i < stop; i++)
	    {
	      தசம v = *pp++ * scalar;
	      எனில் (is_x) points.arrayZ[i].x += v;
	      இல்லையெனில் points.arrayZ[i].y += v;
	    }
	    p = (மாறிலி HBUINT8 *) pp;
	  }
	  நிறுத்து;
	}
      }
      திரும்பு உண்மை;
    }
    பொது:
    தருக்க apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<மாறிலி முழு> coords,
				 மாறிலி hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 தருக்க phantom_only = பொய்,
				 int64_t *budget = nullptr) மாறிலி
    {
      எனில் (unlikely (glyph >= glyph_மொத்தம்_2)) திரும்பு உண்மை;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_மொத்தம்_2, glyph);
      எனில் (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) திரும்பு உண்மை;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      எனில் (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_மொத்தம்_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	திரும்பு உண்மை; /* so isn't applied at all */

      தருக்க any_private_points = பொய்;
      தருக்க private_points_checked = பொய்;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned இலக்கம்_coords = table->axis_மொத்தம்_2;
      hb_array_t<மாறிலி F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_மொத்தம் * இலக்கம்_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned மொத்தம் = points.length;
      தருக்க flush = பொய்;

      செய்
      {
	தசம scalar = iterator.current_tuple->calculate_scalar (coords, இலக்கம்_coords, shared_tuples,
								 scalar_cache);

	எனில் (scalar == 0.f) தொடர்;

	எனில் (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && மொத்தம் >= 4 ? 4 : மொத்தம்)))
	  திரும்பு பொய்;

	எனில் (!private_points_checked)
	{
	  auto scan = iterator;
	  செய்
	  {
	    எனில் (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = உண்மை;
	      நிறுத்து;
	    }
	  } போது (scan.move_to_next ());
	  private_points_checked = உண்மை;
	}
	மாறிலி HBUINT8 *p = iterator.get_serialized_data ();
	unsigned முழு length = iterator.current_tuple->get_data_size ();
	எனில் (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  திரும்பு பொய்;

	எனில் (!deltas)
	{
	  எனில் (unlikely (!deltas_vec.resize_dirty  (மொத்தம்))) திரும்பு பொய்;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? மொத்தம் - 4 : 0), 0,
		     (phantom_only ? 4 : மொத்தம்) * அளவு (deltas[0]));
	}

	மாறிலி HBUINT8 *end = p + length;

	தருக்க has_private_points = iterator.current_tuple->has_private_points ();
	எனில் (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  திரும்பு பொய்;
	மாறிலி hb_array_t<unsigned முழு> &indices = has_private_points ? private_indices : shared_indices;

	தருக்க apply_to_all = (indices.length == 0);
	unsigned இலக்கம்_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && இலக்கம்_deltas >= 4 ? இலக்கம்_deltas - 4 : 0);

	எனில் (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  எனில் (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, உண்மை))) திரும்பு பொய்;
	  எனில் (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, பொய்))) திரும்பு பொய்;
#இல்லையெனில்
	  எனில் (unlikely (!decompile_deltas_add_to_points<உண்மை> (p, points, scalar, end, start_deltas))) திரும்பு பொய்;
	  எனில் (unlikely (!decompile_deltas_add_to_points<பொய்> (p, points, scalar, end, start_deltas))) திரும்பு பொய்;
#endif
	  தொடர்;
	}

	எனில் (unlikely (!x_deltas.resize_dirty  (இலக்கம்_deltas))) திரும்பு பொய்;
	எனில் (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, பொய், start_deltas))) திரும்பு பொய்;
	எனில் (unlikely (!y_deltas.resize_dirty  (இலக்கம்_deltas))) திரும்பு பொய்;
	எனில் (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, பொய், start_deltas))) திரும்பு பொய்;

	எனில் (!apply_to_all)
	{
	  எனில் (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    எனில் (unlikely (orig_points_vec.in_error ())) திரும்பு பொய்;
	    orig_points = orig_points_vec.as_array ();
	  }

	  எனில் (flush)
	  {
	    க்கு (unsigned முழு i = phantom_only ? மொத்தம் - 4 : 0; i < மொத்தம்; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? மொத்தம் - 4 : 0), 0,
		     (phantom_only ? 4 : மொத்தம்) * அளவு (deltas[0]));
	}

	எனில் (HB_OPTIMIZE_SIZE_VAL)
	{
	  க்கு (unsigned முழு i = 0; i < இலக்கம்_deltas; i++)
	  {
	    unsigned முழு pt_index;
	    எனில் (apply_to_all)
	      pt_index = i;
	    இல்லையெனில்
	    {
	      pt_index = indices[i];
	      எனில் (unlikely (pt_index >= deltas.length)) தொடர்;
	    }
	    எனில் (phantom_only && pt_index < மொத்தம் - 4) தொடர்;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	இல்லையெனில்
	{
	  /* Ouch. Four cases... for optimization. */
	  எனில் (scalar != 1.0f)
	  {
	    எனில் (apply_to_all)
	      க்கு (unsigned முழு i = phantom_only ? மொத்தம் - 4 : 0; i < மொத்தம்; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    இல்லையெனில்
	      க்கு (unsigned முழு i = 0; i < இலக்கம்_deltas; i++)
	      {
		unsigned முழு pt_index = indices[i];
		எனில் (unlikely (pt_index >= deltas.length)) தொடர்;
		எனில் (phantom_only && pt_index < மொத்தம் - 4) தொடர்;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  இல்லையெனில்
	  {
	    எனில் (apply_to_all)
	      க்கு (unsigned முழு i = phantom_only ? மொத்தம் - 4 : 0; i < மொத்தம்; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    இல்லையெனில்
	      க்கு (unsigned முழு i = 0; i < இலக்கம்_deltas; i++)
	      {
		unsigned முழு pt_index = indices[i];
		எனில் (unlikely (pt_index >= deltas.length)) தொடர்;
		எனில் (phantom_only && pt_index < மொத்தம் - 4) தொடர்;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	எனில் (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  போது (உண்மை)
	  {
	    போது (end_point < மொத்தம் && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    எனில் (unlikely (end_point == மொத்தம்)) நிறுத்து;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_மொத்தம் = 0;
	    க்கு (unsigned i = start_point; i < end_point + 1; i++)
	      unref_மொத்தம் += deltas.arrayZ[i].flag;
	    unref_மொத்தம் = (end_point - start_point + 1) - unref_மொத்தம்;

	    unsigned j = start_point;
	    எனில் (unref_மொத்தம் == 0 || unref_மொத்தம் > end_point - start_point)
	      செல் no_more_gaps;

	    க்கு (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned முழு prev, next, i;
	      க்கு (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		எனில் (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) நிறுத்து;
	      }
	      prev = j = i;
	      க்கு (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		எனில் (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) நிறுத்து;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      க்கு (;;)
	      {
		i = next_index (i, start_point, end_point);
		எனில் (i == next) நிறுத்து;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		எனில் (--unref_மொத்தம் == 0) செல் no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = உண்மை;

      } போது (iterator.move_to_next ());

      எனில் (flush)
      {
	க்கு (unsigned முழு i = phantom_only ? மொத்தம் - 4 : 0; i < மொத்தம்; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      திரும்பு உண்மை;
    }

    unsigned முழு get_axis_மொத்தம் () மாறிலி { திரும்பு table->axis_மொத்தம்_2; }

    தனிப்பட்ட:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_மொத்தம்_2;
  };

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_மொத்தம்_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_மொத்தம்;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_மொத்தம்_Type
		glyph_மொத்தம்_X;	/* The number of glyphs in this font. This must match the number of
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
  பொது:
  DEFINE_SIZE_ARRAY (18 + Glyph_மொத்தம்_Type::static_size, offsetZ);
};

கட்டமைப்பு gvar : gvar_GVAR<gvar, SmallTypes>
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

கட்டமைப்பு GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

கட்டமைப்பு gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () மாறிலி
  {
    எனில் (has_data ()) திரும்பு HB_DUAL_GET (*இது, create_cache ());
    திரும்பு nullptr;
  }

  நிலையான வெறுமை destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  தருக்க apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<மாறிலி முழு> coords,
			       மாறிலி hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       தருக்க phantom_only = பொய்,
			       int64_t *budget = nullptr) மாறிலி
  {
    திரும்பு HB_DUAL_GET (*இது, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned முழு get_axis_மொத்தம் () மாறிலி
  {
    எனில் (has_data ()) திரும்பு HB_DUAL_GET (*இது, get_axis_மொத்தம் ());
    திரும்பு 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
