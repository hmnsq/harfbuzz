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

#ضمن "hb-decycler.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

بنية hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<صحيح> x_deltas;
  hb_vector_t<صحيح> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned صحيح> shared_indices;
  hb_vector_t<unsigned صحيح> private_indices;
};

نطاق OT {

template <typename OffsetType>
بنية glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  خاص:
  unsigned shared_tuples_عدد = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عام:
  unsigned compiled_shared_tuples_عدد () ثابت
  { أرجع shared_tuples_عدد; }

  unsigned compiled_byte_size () ثابت
  {
    unsigned byte_size = 0;
    لكل (ثابت auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    أرجع byte_size;
  }

  منطقي create_from_glyphs_var_data (unsigned axis_عدد,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    إذا (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      أرجع خطأ;

    auto it = hb_iter (plan->new_to_old_gid_list);
    لكل (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      إذا (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        أرجع خطأ;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ثابت GlyphVariationData* p = reinterpret_cast<ثابت GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      إذا (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_عدد,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        استمر;
      }

      منطقي is_composite_glyph = خطأ;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      إذا (!p->decompile_tuple_variations (all_contour_points->length, true /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        أرجع خطأ;
      glyph_variations.push (std::move (tuple_vars));
    }
    أرجع !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  منطقي instantiate (ثابت hb_subset_plan_t *plan)
  {
    unsigned عدد = plan->new_to_old_gid_list.length;
    منطقي iup_optimize = خطأ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    لكل (unsigned i = 0; i < عدد; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      إذا (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        أرجع خطأ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      إذا (iup_optimize)
      {
        points = *all_points;
        إذا (unlikely (points.in_error ())) أرجع خطأ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      إذا (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        أرجع خطأ;
    }
    أرجع true;
  }

  منطقي compile_bytes (ثابت hb_map_t& axes_index_map,
                      ثابت hb_map_t& axes_old_index_tag_map)
  {
    إذا (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      أرجع خطأ;
    لكل (tuple_variations_t& vars: glyph_variations)
      إذا (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               true, /* use shared points*/
                               true,
                               &shared_tuples_idx_map,
			       &pool))
        أرجع خطأ;

    أرجع true;
  }

  منطقي compile_shared_tuples (ثابت hb_map_t& axes_index_map,
                              ثابت hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned> coords_عدد_map;

    /* count the num of shared coords */
    لكل (tuple_variations_t& vars: glyph_variations)
    {
      لكل (tuple_delta_t& var : vars.tuple_vars)
      {
        إذا (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          أرجع خطأ;
        unsigned *عدد;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        إذا (coords_عدد_map.has_with_hash (&(var.compiled_peak_coords), hash, &عدد))
	  (*عدد)++;
        وإلا
          coords_عدد_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    إذا (!coords_عدد_map || coords_عدد_map.in_error ())
      أرجع خطأ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ثابت hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_عدد_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_vector_t<F2DOT14>*, unsigned>& p) { أرجع p.second > 1; })
    };
    إذا (unlikely (shared_coords.in_error ())) أرجع خطأ;

    /* no shared tuples: no coords are used more than once */
    إذا (!shared_coords) أرجع true;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_عدد = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_عدد * (shared_coords[0].first->length);
    إذا (unlikely (!compiled_shared_tuples.alloc (len)))
      أرجع خطأ;

    لكل (unsigned i = 0; i < shared_tuples_عدد; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      لكل (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    أرجع true;
  }

  ساكن صحيح _cmp_coords (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    إذا (a->second != b->second)
      أرجع b->second - a->second; // high to low

    أرجع b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقي long_offset,
                                 unsigned رقم_glyphs,
                                 حرف* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    إذا (!cur_glyph) return_trace (خطأ);
    لكل (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      إذا (long_offset)
        لكل (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      وإلا
        لكل (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      إذا (idx >= glyph_variations.length) return_trace (خطأ);
      إذا (!cur_glyph->serialize (c, true, glyph_variations[idx])) return_trace (خطأ);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (حرف *) next_glyph - (حرف *) cur_glyph;

      إذا (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      وإلا
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    إذا (long_offset)
      لكل (; last_gid < رقم_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    وإلا
      لكل (; last_gid < رقم_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (true);
  }
};

template <typename T, typename Types>
بنية gvar_GVAR
{
  using Glyph_عدد_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_عدد_Type>;
  ساكن constexpr unsigned glyph_عدد_max ()
  { أرجع (1u << (8 * Glyph_عدد_Type::static_size)) - 1; }

  منطقي has_data () ثابت { أرجع version.to_int () != 0; }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, هذا, axis_عدد_2 * shared_Tuple_عدد) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_رقم_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_رقم_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع sanitize_shallow (c); }

  منطقي decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_عدد_Type>& glyph_vars /* OUT */) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    إذا (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    لكل (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_عدد_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    إذا (new_gid_var_data_map.in_error ()) أرجع خطأ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (هذا+sharedTuples).as_array ((unsigned) shared_Tuple_عدد * (unsigned) axis_عدد_2);
    أرجع glyph_vars.create_from_glyphs_var_data (axis_عدد_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_عدد_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_عدد,
                  unsigned رقم_glyphs,
                  منطقي force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (هذا);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!out)) return_trace (خطأ);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_عدد_2 = axis_عدد;
    out->glyph_عدد_X = hb_min (glyph_عدد_max (), رقم_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, خطأ);
    إذا (!glyph_var_data_offsets) return_trace (خطأ);

    /* shared tuples */
    unsigned shared_tuple_عدد = glyph_vars.compiled_shared_tuples_عدد ();
    out->shared_Tuple_عدد = shared_tuple_عدد;

    إذا (!shared_tuple_عدد)
      out->sharedTuples = (حرف *) glyph_var_data_offsets + glyph_var_data_offsets_size - (حرف *) out;
    وإلا
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      إذا (!shared_tuples.arrayZ) return_trace (خطأ);
      out->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) out;
    }

    حرف *glyph_var_data = c->start_embed<حرف> ();
    إذا (!glyph_var_data) return_trace (خطأ);
    out->dataZ = glyph_var_data - (حرف *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, رقم_glyphs,
                                                       (حرف *) glyph_var_data_offsets));
  }

  منطقي instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    glyph_variations_t<Glyph_عدد_Type> glyph_vars;
    إذا (!decompile_glyph_variations (c, glyph_vars))
      return_trace (خطأ);

    إذا (!glyph_vars.instantiate (c->plan)) return_trace (خطأ);
    إذا (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (خطأ);

    unsigned axis_عدد = c->plan->axes_index_map.get_population ();
    unsigned رقم_glyphs = c->plan->رقم_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    منطقي force_long_offsets = خطأ;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_عدد, رقم_glyphs, force_long_offsets));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    إذا (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_عدد = version.to_int () ? c->plan->source->get_رقم_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!out)) return_trace (خطأ);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_عدد_2 = axis_عدد_2;
    out->shared_Tuple_عدد = shared_Tuple_عدد;

    unsigned صحيح رقم_glyphs = c->plan->رقم_output_glyphs ();
    out->glyph_عدد_X = hb_min (glyph_عدد_max (), رقم_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<حرف>> culled_vars;
    hb_set_t culled_gids;
    إذا (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (هذا+sharedTuples).as_array ((unsigned) shared_Tuple_عدد * (unsigned) axis_عدد_2);
      إذا (unlikely (!culled_vars.resize (رقم_glyphs))) return_trace (خطأ);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      إذا (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      لكل (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_عدد, _.second);
	إذا (var_data_bytes.length < GlyphVariationData::min_size) استمر;
	منطقي changed = خطأ;
	إذا (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_عدد_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (خطأ);
	إذا (changed) culled_gids.add (_.first);
      }
      إذا (unlikely (culled_gids.in_error ())) return_trace (خطأ);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    إذا (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    لكل (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_عدد, old_gid).length;
      إذا (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      إذا (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (خطأ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, خطأ);
    إذا (!subset_offsets) return_trace (خطأ);

    /* shared tuples */
    إذا (!shared_Tuple_عدد || !sharedTuples)
      out->sharedTuples = (حرف *) subset_offsets + subset_offsets_size - (حرف *) out;
    وإلا
    {
      unsigned صحيح shared_tuple_size = F2DOT14::static_size * axis_عدد_2 * shared_Tuple_عدد;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      إذا (!tuples) return_trace (خطأ);
      out->sharedTuples = (حرف *) tuples - (حرف *) out;
      hb_memcpy (tuples, هذا+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    إذا (long_offset)
      subset_data_size -= padding_size;
    حرف *subset_data = c->serializer->allocate_size<حرف> (subset_data_size, خطأ);
    إذا (!subset_data) return_trace (خطأ);
    out->dataZ = subset_data - (حرف *) out;


    إذا (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned صحيح glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    إذا (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    لكل (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      إذا (long_offset)
	لكل (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      وإلا
	لكل (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_عدد,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      إذا (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      إذا (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      وإلا
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    إذا (long_offset)
      لكل (; last < رقم_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    وإلا
      لكل (; last < رقم_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (true);
  }

  محمي:
  ثابت hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_عدد,
					     hb_codepoint_t glyph) ثابت
  {
    unsigned start_offset = get_offset (glyph_عدد, glyph);
    unsigned end_offset = get_offset (glyph_عدد, glyph+1);
    إذا (unlikely (end_offset < start_offset)) أرجع hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    أرجع likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  منطقي is_long_offset () ثابت { أرجع flags & 1; }

  unsigned get_offset (unsigned glyph_عدد, unsigned i) ثابت
  {
    إذا (unlikely (i > glyph_عدد)) أرجع 0;
    hb_barrier ();
    أرجع is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_offset_array () ثابت { أرجع (ثابت HBUINT32 *) &offsetZ; }
  ثابت HBUINT16 *get_short_offset_array () ثابت { أرجع (ثابت HBUINT16 *) &offsetZ; }

  عام:
  بنية accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      أرجع hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_عدد,
						+TupleVariationHeader::max_shared_tuple_عدد));
    }

    ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقي has_data () ثابت { أرجع table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_عدد_2 = table->version.to_int () ? face->get_رقم_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    خاص:

    ساكن عائم infer_delta (ثابت hb_array_t<contour_point_t> points,
			      ثابت hb_array_t<contour_point_t> deltas,
			      unsigned صحيح target, unsigned صحيح prev, unsigned صحيح next,
			      عائم contour_point_t::*m)
    {
      عائم target_val = points.arrayZ[target].*m;
      عائم prev_val = points.arrayZ[prev].*m;
      عائم next_val = points.arrayZ[next].*m;
      عائم prev_delta =  deltas.arrayZ[prev].*m;
      عائم next_delta =  deltas.arrayZ[next].*m;

      إذا (prev_val == next_val)
	أرجع (prev_delta == next_delta) ? prev_delta : 0.f;
      وإلا إذا (target_val <= hb_min (prev_val, next_val))
	أرجع (prev_val < next_val) ? prev_delta : next_delta;
      وإلا إذا (target_val >= hb_max (prev_val, next_val))
	أرجع (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      عائم r = (target_val - prev_val) / (next_val - prev_val);
      أرجع prev_delta + r * (next_delta - prev_delta);
    }

    ساكن unsigned صحيح next_index (unsigned صحيح i, unsigned صحيح start, unsigned صحيح end)
    { أرجع (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <منطقي is_x>
#endif
    ساكن منطقي decompile_deltas_add_to_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						عائم scalar,
						ثابت HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, منطقي is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned عدد = points.length;
      طالما (i < عدد)
      {
	إذا (unlikely (p + 1 > end)) أرجع خطأ;
	unsigned control = *p++;
	unsigned run_عدد = (control & Tuple_قيم::قيمة_RUN_عدد_MASK) + 1;
	unsigned stop = i + run_عدد;
	إذا (unlikely (stop > عدد)) أرجع خطأ;

	unsigned skip = i < start ? hb_min (start - i, run_عدد) : 0;
	i += skip;

	بدل (control & Tuple_قيم::قيم_SIZE_MASK)
	{
	  حالة Tuple_قيم::قيم_ARE_ZEROS:
	    i = stop;
	    اكسر;
	  حالة Tuple_قيم::قيم_ARE_كلمات:
	  {
	    إذا (unlikely (p + run_عدد * HBINT16::static_size > end)) أرجع خطأ;
	    p += skip * HBINT16::static_size;
	    ثابت auto *pp = (ثابت HBINT16 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_قيم::قيم_ARE_LONGS:
	  {
	    إذا (unlikely (p + run_عدد * HBINT32::static_size > end)) أرجع خطأ;
	    p += skip * HBINT32::static_size;
	    ثابت auto *pp = (ثابت HBINT32 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_قيم::قيم_ARE_BYTES:
	  {
	    إذا (unlikely (p + run_عدد > end)) أرجع خطأ;
	    p += skip * HBINT8::static_size;
	    ثابت auto *pp = (ثابت HBINT8 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	}
      }
      أرجع true;
    }
    عام:
    منطقي apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحيح> coords,
				 ثابت hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 منطقي phantom_only = خطأ,
				 int64_t *budget = nullptr) ثابت
    {
      إذا (unlikely (glyph >= glyph_عدد_2)) أرجع true;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_عدد_2, glyph);
      إذا (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) أرجع true;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      إذا (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_عدد_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	أرجع true; /* so isn't applied at all */

      منطقي any_private_points = خطأ;
      منطقي private_points_checked = خطأ;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned رقم_coords = table->axis_عدد_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_عدد * رقم_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned عدد = points.length;
      منطقي flush = خطأ;

      افعل
      {
	عائم scalar = iterator.current_tuple->calculate_scalar (coords, رقم_coords, shared_tuples,
								 scalar_cache);

	إذا (scalar == 0.f) استمر;

	إذا (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && عدد >= 4 ? 4 : عدد)))
	  أرجع خطأ;

	إذا (!private_points_checked)
	{
	  auto scan = iterator;
	  افعل
	  {
	    إذا (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = true;
	      اكسر;
	    }
	  } طالما (scan.move_to_next ());
	  private_points_checked = true;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_data ();
	unsigned صحيح length = iterator.current_tuple->get_data_size ();
	إذا (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  أرجع خطأ;

	إذا (!deltas)
	{
	  إذا (unlikely (!deltas_vec.resize_dirty  (عدد))) أرجع خطأ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? عدد - 4 : 0), 0,
		     (phantom_only ? 4 : عدد) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *end = p + length;

	منطقي has_private_points = iterator.current_tuple->has_private_points ();
	إذا (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  أرجع خطأ;
	ثابت hb_array_t<unsigned صحيح> &indices = has_private_points ? private_indices : shared_indices;

	منطقي apply_to_all = (indices.length == 0);
	unsigned رقم_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && رقم_deltas >= 4 ? رقم_deltas - 4 : 0);

	إذا (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  إذا (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, true))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, خطأ))) أرجع خطأ;
#وإلا
	  إذا (unlikely (!decompile_deltas_add_to_points<true> (p, points, scalar, end, start_deltas))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_add_to_points<خطأ> (p, points, scalar, end, start_deltas))) أرجع خطأ;
#endif
	  استمر;
	}

	إذا (unlikely (!x_deltas.resize_dirty  (رقم_deltas))) أرجع خطأ;
	إذا (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, خطأ, start_deltas))) أرجع خطأ;
	إذا (unlikely (!y_deltas.resize_dirty  (رقم_deltas))) أرجع خطأ;
	إذا (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, خطأ, start_deltas))) أرجع خطأ;

	إذا (!apply_to_all)
	{
	  إذا (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    إذا (unlikely (orig_points_vec.in_error ())) أرجع خطأ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  إذا (flush)
	  {
	    لكل (unsigned صحيح i = phantom_only ? عدد - 4 : 0; i < عدد; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? عدد - 4 : 0), 0,
		     (phantom_only ? 4 : عدد) * حجم (deltas[0]));
	}

	إذا (HB_OPTIMIZE_SIZE_VAL)
	{
	  لكل (unsigned صحيح i = 0; i < رقم_deltas; i++)
	  {
	    unsigned صحيح pt_index;
	    إذا (apply_to_all)
	      pt_index = i;
	    وإلا
	    {
	      pt_index = indices[i];
	      إذا (unlikely (pt_index >= deltas.length)) استمر;
	    }
	    إذا (phantom_only && pt_index < عدد - 4) استمر;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وإلا
	{
	  /* Ouch. Four cases... for optimization. */
	  إذا (scalar != 1.0f)
	  {
	    إذا (apply_to_all)
	      لكل (unsigned صحيح i = phantom_only ? عدد - 4 : 0; i < عدد; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وإلا
	      لكل (unsigned صحيح i = 0; i < رقم_deltas; i++)
	      {
		unsigned صحيح pt_index = indices[i];
		إذا (unlikely (pt_index >= deltas.length)) استمر;
		إذا (phantom_only && pt_index < عدد - 4) استمر;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وإلا
	  {
	    إذا (apply_to_all)
	      لكل (unsigned صحيح i = phantom_only ? عدد - 4 : 0; i < عدد; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وإلا
	      لكل (unsigned صحيح i = 0; i < رقم_deltas; i++)
	      {
		unsigned صحيح pt_index = indices[i];
		إذا (unlikely (pt_index >= deltas.length)) استمر;
		إذا (phantom_only && pt_index < عدد - 4) استمر;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	إذا (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  طالما (true)
	  {
	    طالما (end_point < عدد && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    إذا (unlikely (end_point == عدد)) اكسر;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_عدد = 0;
	    لكل (unsigned i = start_point; i < end_point + 1; i++)
	      unref_عدد += deltas.arrayZ[i].flag;
	    unref_عدد = (end_point - start_point + 1) - unref_عدد;

	    unsigned j = start_point;
	    إذا (unref_عدد == 0 || unref_عدد > end_point - start_point)
	      اذهب no_more_gaps;

	    لكل (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned صحيح prev, next, i;
	      لكل (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		إذا (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) اكسر;
	      }
	      prev = j = i;
	      لكل (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		إذا (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) اكسر;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      لكل (;;)
	      {
		i = next_index (i, start_point, end_point);
		إذا (i == next) اكسر;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		إذا (--unref_عدد == 0) اذهب no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = true;

      } طالما (iterator.move_to_next ());

      إذا (flush)
      {
	لكل (unsigned صحيح i = phantom_only ? عدد - 4 : 0; i < عدد; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      أرجع true;
    }

    unsigned صحيح get_axis_عدد () ثابت { أرجع table->axis_عدد_2; }

    خاص:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_عدد_2;
  };

  محمي:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_عدد_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_عدد;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_عدد_Type
		glyph_عدد_X;	/* The number of glyphs in this font. This must match the number of
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
  عام:
  DEFINE_SIZE_ARRAY (18 + Glyph_عدد_Type::static_size, offsetZ);
};

بنية gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

بنية GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

بنية gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    إذا (has_data ()) أرجع HB_DUAL_GET (*هذا, create_cache ());
    أرجع nullptr;
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقي apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحيح> coords,
			       ثابت hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       منطقي phantom_only = خطأ,
			       int64_t *budget = nullptr) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned صحيح get_axis_عدد () ثابت
  {
    إذا (has_data ()) أرجع HB_DUAL_GET (*هذا, get_axis_عدد ());
    أرجع 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
