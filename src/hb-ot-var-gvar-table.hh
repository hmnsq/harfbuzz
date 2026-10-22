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

#شامل "hb-decycler.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"
#شامل "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

ساختار hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<صحیح> x_deltas;
  hb_vector_t<صحیح> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned صحیح> shared_indices;
  hb_vector_t<unsigned صحیح> private_indices;
};

فضاینام OT {

template <typename OffsetType>
ساختار glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  خصوصی:
  unsigned shared_tuples_شمار = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عمومی:
  unsigned compiled_shared_tuples_شمار () ثابت
  { بازگشت shared_tuples_شمار; }

  unsigned compiled_byte_size () ثابت
  {
    unsigned byte_size = 0;
    برای (ثابت auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    بازگشت byte_size;
  }

  منطقی create_from_glyphs_var_data (unsigned axis_شمار,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    اگر (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      بازگشت نادرست;

    auto it = hb_iter (plan->new_to_old_gid_list);
    برای (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      اگر (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        بازگشت نادرست;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ثابت GlyphVariationData* p = reinterpret_cast<ثابت GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      اگر (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_شمار,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        ادامه;
      }

      منطقی is_composite_glyph = نادرست;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      اگر (!p->decompile_tuple_variations (all_contour_points->length, درست /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        بازگشت نادرست;
      glyph_variations.push (std::move (tuple_vars));
    }
    بازگشت !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  منطقی instantiate (ثابت hb_subset_plan_t *plan)
  {
    unsigned شمار = plan->new_to_old_gid_list.length;
    منطقی iup_optimize = نادرست;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    برای (unsigned i = 0; i < شمار; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      اگر (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        بازگشت نادرست;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      اگر (iup_optimize)
      {
        points = *all_points;
        اگر (unlikely (points.in_error ())) بازگشت نادرست;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      اگر (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      اگر (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        بازگشت نادرست;
    }
    بازگشت درست;
  }

  منطقی compile_bytes (ثابت hb_map_t& axes_index_map,
                      ثابت hb_map_t& axes_old_index_tag_map)
  {
    اگر (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      بازگشت نادرست;
    برای (tuple_variations_t& vars: glyph_variations)
      اگر (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               درست, /* use shared points*/
                               درست,
                               &shared_tuples_idx_map,
			       &pool))
        بازگشت نادرست;

    بازگشت درست;
  }

  منطقی compile_shared_tuples (ثابت hb_map_t& axes_index_map,
                              ثابت hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned> coords_شمار_map;

    /* count the num of shared coords */
    برای (tuple_variations_t& vars: glyph_variations)
    {
      برای (tuple_delta_t& var : vars.tuple_vars)
      {
        اگر (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          بازگشت نادرست;
        unsigned *شمار;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        اگر (coords_شمار_map.has_with_hash (&(var.compiled_peak_coords), hash, &شمار))
	  (*شمار)++;
        وگرنه
          coords_شمار_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    اگر (!coords_شمار_map || coords_شمار_map.in_error ())
      بازگشت نادرست;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ثابت hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_شمار_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_vector_t<F2DOT14>*, unsigned>& p) { بازگشت p.second > 1; })
    };
    اگر (unlikely (shared_coords.in_error ())) بازگشت نادرست;

    /* no shared tuples: no coords are used more than once */
    اگر (!shared_coords) بازگشت درست;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_شمار = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_شمار * (shared_coords[0].first->length);
    اگر (unlikely (!compiled_shared_tuples.alloc (len)))
      بازگشت نادرست;

    برای (unsigned i = 0; i < shared_tuples_شمار; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      برای (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    بازگشت درست;
  }

  ایستا صحیح _cmp_coords (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (ثابت hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    اگر (a->second != b->second)
      بازگشت b->second - a->second; // high to low

    بازگشت b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقی long_offset,
                                 unsigned رقم_glyphs,
                                 نویسه* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (این);

    اگر (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    وگرنه
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    اگر (!cur_glyph) return_trace (نادرست);
    برای (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      اگر (long_offset)
        برای (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      وگرنه
        برای (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      اگر (idx >= glyph_variations.length) return_trace (نادرست);
      اگر (!cur_glyph->serialize (c, درست, glyph_variations[idx])) return_trace (نادرست);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (نویسه *) next_glyph - (نویسه *) cur_glyph;

      اگر (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      وگرنه
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    اگر (long_offset)
      برای (; last_gid < رقم_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    وگرنه
      برای (; last_gid < رقم_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (درست);
  }
};

template <typename T, typename Types>
ساختار gvar_GVAR
{
  using Glyph_شمار_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_شمار_Type>;
  ایستا constexpr unsigned glyph_شمار_max ()
  { بازگشت (1u << (8 * Glyph_شمار_Type::static_size)) - 1; }

  منطقی has_data () ثابت { بازگشت version.to_int () != 0; }

  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, این, axis_شمار_2 * shared_Tuple_شمار) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_رقم_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_رقم_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { بازگشت sanitize_shallow (c); }

  منطقی decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_شمار_Type>& glyph_vars /* OUT */) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    اگر (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    برای (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_شمار_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    اگر (new_gid_var_data_map.in_error ()) بازگشت نادرست;

    hb_array_t<ثابت F2DOT14> shared_tuples = (این+sharedTuples).as_array ((unsigned) shared_Tuple_شمار * (unsigned) axis_شمار_2);
    بازگشت glyph_vars.create_from_glyphs_var_data (axis_شمار_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_شمار_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_شمار,
                  unsigned رقم_glyphs,
                  منطقی force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (این);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    اگر (unlikely (!out)) return_trace (نادرست);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_شمار_2 = axis_شمار;
    out->glyph_شمار_X = hb_min (glyph_شمار_max (), رقم_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقی long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, نادرست);
    اگر (!glyph_var_data_offsets) return_trace (نادرست);

    /* shared tuples */
    unsigned shared_tuple_شمار = glyph_vars.compiled_shared_tuples_شمار ();
    out->shared_Tuple_شمار = shared_tuple_شمار;

    اگر (!shared_tuple_شمار)
      out->sharedTuples = (نویسه *) glyph_var_data_offsets + glyph_var_data_offsets_size - (نویسه *) out;
    وگرنه
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      اگر (!shared_tuples.arrayZ) return_trace (نادرست);
      out->sharedTuples = (ثابت نویسه *) shared_tuples.arrayZ - (نویسه *) out;
    }

    نویسه *glyph_var_data = c->start_embed<نویسه> ();
    اگر (!glyph_var_data) return_trace (نادرست);
    out->dataZ = glyph_var_data - (نویسه *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, رقم_glyphs,
                                                       (نویسه *) glyph_var_data_offsets));
  }

  منطقی instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    glyph_variations_t<Glyph_شمار_Type> glyph_vars;
    اگر (!decompile_glyph_variations (c, glyph_vars))
      return_trace (نادرست);

    اگر (!glyph_vars.instantiate (c->plan)) return_trace (نادرست);
    اگر (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (نادرست);

    unsigned axis_شمار = c->plan->axes_index_map.get_population ();
    unsigned رقم_glyphs = c->plan->رقم_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    منطقی force_long_offsets = نادرست;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_شمار, رقم_glyphs, force_long_offsets));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned)
      return_trace (نادرست);

    اگر (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_شمار = version.to_int () ? c->plan->source->get_رقم_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    اگر (unlikely (!out)) return_trace (نادرست);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_شمار_2 = axis_شمار_2;
    out->shared_Tuple_شمار = shared_Tuple_شمار;

    unsigned صحیح رقم_glyphs = c->plan->رقم_output_glyphs ();
    out->glyph_شمار_X = hb_min (glyph_شمار_max (), رقم_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<نویسه>> culled_vars;
    hb_set_t culled_gids;
    اگر (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (این+sharedTuples).as_array ((unsigned) shared_Tuple_شمار * (unsigned) axis_شمار_2);
      اگر (unlikely (!culled_vars.resize (رقم_glyphs))) return_trace (نادرست);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      اگر (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      برای (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_شمار, _.second);
	اگر (var_data_bytes.length < GlyphVariationData::min_size) ادامه;
	منطقی changed = نادرست;
	اگر (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_شمار_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (نادرست);
	اگر (changed) culled_gids.add (_.first);
      }
      اگر (unlikely (culled_gids.in_error ())) return_trace (نادرست);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    اگر (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    برای (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_شمار, old_gid).length;
      اگر (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      اگر (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (نادرست);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقی long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, نادرست);
    اگر (!subset_offsets) return_trace (نادرست);

    /* shared tuples */
    اگر (!shared_Tuple_شمار || !sharedTuples)
      out->sharedTuples = (نویسه *) subset_offsets + subset_offsets_size - (نویسه *) out;
    وگرنه
    {
      unsigned صحیح shared_tuple_size = F2DOT14::static_size * axis_شمار_2 * shared_Tuple_شمار;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      اگر (!tuples) return_trace (نادرست);
      out->sharedTuples = (نویسه *) tuples - (نویسه *) out;
      hb_memcpy (tuples, این+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    اگر (long_offset)
      subset_data_size -= padding_size;
    نویسه *subset_data = c->serializer->allocate_size<نویسه> (subset_data_size, نادرست);
    اگر (!subset_data) return_trace (نادرست);
    out->dataZ = subset_data - (نویسه *) out;


    اگر (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وگرنه
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned صحیح glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    اگر (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    برای (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      اگر (long_offset)
	برای (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      وگرنه
	برای (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_شمار,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      اگر (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      اگر (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      وگرنه
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    اگر (long_offset)
      برای (; last < رقم_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    وگرنه
      برای (; last < رقم_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (درست);
  }

  محافظتشده:
  ثابت hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_شمار,
					     hb_codepoint_t glyph) ثابت
  {
    unsigned start_offset = get_offset (glyph_شمار, glyph);
    unsigned end_offset = get_offset (glyph_شمار, glyph+1);
    اگر (unlikely (end_offset < start_offset)) بازگشت hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    بازگشت likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  منطقی is_long_offset () ثابت { بازگشت flags & 1; }

  unsigned get_offset (unsigned glyph_شمار, unsigned i) ثابت
  {
    اگر (unlikely (i > glyph_شمار)) بازگشت 0;
    hb_barrier ();
    بازگشت is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_offset_array () ثابت { بازگشت (ثابت HBUINT32 *) &offsetZ; }
  ثابت HBUINT16 *get_short_offset_array () ثابت { بازگشت (ثابت HBUINT16 *) &offsetZ; }

  عمومی:
  ساختار accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      بازگشت hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_شمار,
						+TupleVariationHeader::max_shared_tuple_شمار));
    }

    ایستا تهی destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقی has_data () ثابت { بازگشت table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_شمار_2 = table->version.to_int () ? face->get_رقم_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    خصوصی:

    ایستا اعشاری infer_delta (ثابت hb_array_t<contour_point_t> points,
			      ثابت hb_array_t<contour_point_t> deltas,
			      unsigned صحیح target, unsigned صحیح prev, unsigned صحیح next,
			      اعشاری contour_point_t::*m)
    {
      اعشاری target_val = points.arrayZ[target].*m;
      اعشاری prev_val = points.arrayZ[prev].*m;
      اعشاری next_val = points.arrayZ[next].*m;
      اعشاری prev_delta =  deltas.arrayZ[prev].*m;
      اعشاری next_delta =  deltas.arrayZ[next].*m;

      اگر (prev_val == next_val)
	بازگشت (prev_delta == next_delta) ? prev_delta : 0.f;
      وگرنه اگر (target_val <= hb_min (prev_val, next_val))
	بازگشت (prev_val < next_val) ? prev_delta : next_delta;
      وگرنه اگر (target_val >= hb_max (prev_val, next_val))
	بازگشت (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      اعشاری r = (target_val - prev_val) / (next_val - prev_val);
      بازگشت prev_delta + r * (next_delta - prev_delta);
    }

    ایستا unsigned صحیح next_index (unsigned صحیح i, unsigned صحیح start, unsigned صحیح end)
    { بازگشت (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <منطقی is_x>
#endif
    ایستا منطقی decompile_deltas_add_to_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						اعشاری scalar,
						ثابت HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, منطقی is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned شمار = points.length;
      وقتی (i < شمار)
      {
	اگر (unlikely (p + 1 > end)) بازگشت نادرست;
	unsigned control = *p++;
	unsigned run_شمار = (control & Tuple_مقادیر::مقدار_RUN_شمار_MASK) + 1;
	unsigned stop = i + run_شمار;
	اگر (unlikely (stop > شمار)) بازگشت نادرست;

	unsigned skip = i < start ? hb_min (start - i, run_شمار) : 0;
	i += skip;

	انتخاب (control & Tuple_مقادیر::مقادیر_SIZE_MASK)
	{
	  حالت Tuple_مقادیر::مقادیر_ARE_ZEROS:
	    i = stop;
	    بشکن;
	  حالت Tuple_مقادیر::مقادیر_ARE_واژگان:
	  {
	    اگر (unlikely (p + run_شمار * HBINT16::static_size > end)) بازگشت نادرست;
	    p += skip * HBINT16::static_size;
	    ثابت auto *pp = (ثابت HBINT16 *) p;
	    برای (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      وگرنه points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  بشکن;
	  حالت Tuple_مقادیر::مقادیر_ARE_LONGS:
	  {
	    اگر (unlikely (p + run_شمار * HBINT32::static_size > end)) بازگشت نادرست;
	    p += skip * HBINT32::static_size;
	    ثابت auto *pp = (ثابت HBINT32 *) p;
	    برای (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      وگرنه points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  بشکن;
	  حالت Tuple_مقادیر::مقادیر_ARE_BYTES:
	  {
	    اگر (unlikely (p + run_شمار > end)) بازگشت نادرست;
	    p += skip * HBINT8::static_size;
	    ثابت auto *pp = (ثابت HBINT8 *) p;
	    برای (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      وگرنه points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  بشکن;
	}
      }
      بازگشت درست;
    }
    عمومی:
    منطقی apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحیح> coords,
				 ثابت hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 منطقی phantom_only = نادرست,
				 int64_t *budget = nullptr) ثابت
    {
      اگر (unlikely (glyph >= glyph_شمار_2)) بازگشت درست;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_شمار_2, glyph);
      اگر (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) بازگشت درست;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      اگر (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_شمار_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	بازگشت درست; /* so isn't applied at all */

      منطقی any_private_points = نادرست;
      منطقی private_points_checked = نادرست;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned رقم_coords = table->axis_شمار_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_شمار * رقم_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned شمار = points.length;
      منطقی flush = نادرست;

      انجامبده
      {
	اعشاری scalar = iterator.current_tuple->calculate_scalar (coords, رقم_coords, shared_tuples,
								 scalar_cache);

	اگر (scalar == 0.f) ادامه;

	اگر (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && شمار >= 4 ? 4 : شمار)))
	  بازگشت نادرست;

	اگر (!private_points_checked)
	{
	  auto scan = iterator;
	  انجامبده
	  {
	    اگر (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = درست;
	      بشکن;
	    }
	  } وقتی (scan.move_to_next ());
	  private_points_checked = درست;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_data ();
	unsigned صحیح length = iterator.current_tuple->get_data_size ();
	اگر (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  بازگشت نادرست;

	اگر (!deltas)
	{
	  اگر (unlikely (!deltas_vec.resize_dirty  (شمار))) بازگشت نادرست;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? شمار - 4 : 0), 0,
		     (phantom_only ? 4 : شمار) * اندازه (deltas[0]));
	}

	ثابت HBUINT8 *end = p + length;

	منطقی has_private_points = iterator.current_tuple->has_private_points ();
	اگر (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  بازگشت نادرست;
	ثابت hb_array_t<unsigned صحیح> &indices = has_private_points ? private_indices : shared_indices;

	منطقی apply_to_all = (indices.length == 0);
	unsigned رقم_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && رقم_deltas >= 4 ? رقم_deltas - 4 : 0);

	اگر (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  اگر (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, درست))) بازگشت نادرست;
	  اگر (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, نادرست))) بازگشت نادرست;
#وگرنه
	  اگر (unlikely (!decompile_deltas_add_to_points<درست> (p, points, scalar, end, start_deltas))) بازگشت نادرست;
	  اگر (unlikely (!decompile_deltas_add_to_points<نادرست> (p, points, scalar, end, start_deltas))) بازگشت نادرست;
#endif
	  ادامه;
	}

	اگر (unlikely (!x_deltas.resize_dirty  (رقم_deltas))) بازگشت نادرست;
	اگر (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, نادرست, start_deltas))) بازگشت نادرست;
	اگر (unlikely (!y_deltas.resize_dirty  (رقم_deltas))) بازگشت نادرست;
	اگر (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, نادرست, start_deltas))) بازگشت نادرست;

	اگر (!apply_to_all)
	{
	  اگر (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    اگر (unlikely (orig_points_vec.in_error ())) بازگشت نادرست;
	    orig_points = orig_points_vec.as_array ();
	  }

	  اگر (flush)
	  {
	    برای (unsigned صحیح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? شمار - 4 : 0), 0,
		     (phantom_only ? 4 : شمار) * اندازه (deltas[0]));
	}

	اگر (HB_OPTIMIZE_SIZE_VAL)
	{
	  برای (unsigned صحیح i = 0; i < رقم_deltas; i++)
	  {
	    unsigned صحیح pt_index;
	    اگر (apply_to_all)
	      pt_index = i;
	    وگرنه
	    {
	      pt_index = indices[i];
	      اگر (unlikely (pt_index >= deltas.length)) ادامه;
	    }
	    اگر (phantom_only && pt_index < شمار - 4) ادامه;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وگرنه
	{
	  /* Ouch. Four cases... for optimization. */
	  اگر (scalar != 1.0f)
	  {
	    اگر (apply_to_all)
	      برای (unsigned صحیح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وگرنه
	      برای (unsigned صحیح i = 0; i < رقم_deltas; i++)
	      {
		unsigned صحیح pt_index = indices[i];
		اگر (unlikely (pt_index >= deltas.length)) ادامه;
		اگر (phantom_only && pt_index < شمار - 4) ادامه;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وگرنه
	  {
	    اگر (apply_to_all)
	      برای (unsigned صحیح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وگرنه
	      برای (unsigned صحیح i = 0; i < رقم_deltas; i++)
	      {
		unsigned صحیح pt_index = indices[i];
		اگر (unlikely (pt_index >= deltas.length)) ادامه;
		اگر (phantom_only && pt_index < شمار - 4) ادامه;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	اگر (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  وقتی (درست)
	  {
	    وقتی (end_point < شمار && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    اگر (unlikely (end_point == شمار)) بشکن;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_شمار = 0;
	    برای (unsigned i = start_point; i < end_point + 1; i++)
	      unref_شمار += deltas.arrayZ[i].flag;
	    unref_شمار = (end_point - start_point + 1) - unref_شمار;

	    unsigned j = start_point;
	    اگر (unref_شمار == 0 || unref_شمار > end_point - start_point)
	      برو no_more_gaps;

	    برای (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned صحیح prev, next, i;
	      برای (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		اگر (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) بشکن;
	      }
	      prev = j = i;
	      برای (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		اگر (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) بشکن;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      برای (;;)
	      {
		i = next_index (i, start_point, end_point);
		اگر (i == next) بشکن;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		اگر (--unref_شمار == 0) برو no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = درست;

      } وقتی (iterator.move_to_next ());

      اگر (flush)
      {
	برای (unsigned صحیح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      بازگشت درست;
    }

    unsigned صحیح get_axis_شمار () ثابت { بازگشت table->axis_شمار_2; }

    خصوصی:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_شمار_2;
  };

  محافظتشده:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_شمار_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_شمار;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_شمار_Type
		glyph_شمار_X;	/* The number of glyphs in this font. This must match the number of
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
  عمومی:
  DEFINE_SIZE_ARRAY (18 + Glyph_شمار_Type::static_size, offsetZ);
};

ساختار gvar : gvar_GVAR<gvar, SmallTypes>
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

ساختار GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

ساختار gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    اگر (has_data ()) بازگشت HB_DUAL_GET (*این, create_cache ());
    بازگشت nullptr;
  }

  ایستا تهی destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقی apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحیح> coords,
			       ثابت hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       منطقی phantom_only = نادرست,
			       int64_t *budget = nullptr) ثابت
  {
    بازگشت HB_DUAL_GET (*این, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned صحیح get_axis_شمار () ثابت
  {
    اگر (has_data ()) بازگشت HB_DUAL_GET (*این, get_axis_شمار ());
    بازگشت 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
