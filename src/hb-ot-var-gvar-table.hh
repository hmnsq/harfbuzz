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
  hb_vector_t<पूर्णांक> x_deltas;
  hb_vector_t<पूर्णांक> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<अचिन्हित पूर्णांक> shared_indices;
  hb_vector_t<अचिन्हित पूर्णांक> private_indices;
};

नामाकाश OT {

ढाँचा <typename OffsetType>
संरचना glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  खाजगी:
  अचिन्हित shared_tuples_मोजणी = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  सार्वजनिक:
  अचिन्हित compiled_shared_tuples_मोजणी () स्थिरांक
  { परत shared_tuples_मोजणी; }

  अचिन्हित compiled_byte_size () स्थिरांक
  {
    अचिन्हित byte_size = 0;
    साठी (स्थिरांक स्वतः& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    परत byte_size;
  }

  तार्किक create_from_glyphs_var_data (अचिन्हित axis_मोजणी,
                                    स्थिरांक hb_array_t<स्थिरांक F2DOT14> shared_tuples,
                                    स्थिरांक hb_subset_plan_t *plan,
                                    स्थिरांक hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    जर (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      परत असत्य;

    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    साठी (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      जर (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        परत असत्य;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      स्थिरांक GlyphVariationData* p = reinterpret_cast<स्थिरांक GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<अचिन्हित> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      जर (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_मोजणी,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        सुरूठेवा;
      }

      तार्किक is_composite_glyph = असत्य;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      जर (!p->decompile_tuple_variations (all_contour_points->length, सत्य /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        परत असत्य;
      glyph_variations.push (std::move (tuple_vars));
    }
    परत !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  तार्किक instantiate (स्थिरांक hb_subset_plan_t *plan)
  {
    अचिन्हित मोजणी = plan->new_to_old_gid_list.length;
    तार्किक iup_optimize = असत्य;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      जर (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        परत असत्य;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      जर (iup_optimize)
      {
        points = *all_points;
        जर (unlikely (points.in_error ())) परत असत्य;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      जर (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      जर (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        परत असत्य;
    }
    परत सत्य;
  }

  तार्किक compile_bytes (स्थिरांक hb_map_t& axes_index_map,
                      स्थिरांक hb_map_t& axes_old_index_tag_map)
  {
    जर (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      परत असत्य;
    साठी (tuple_variations_t& vars: glyph_variations)
      जर (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               सत्य, /* use shared points*/
                               सत्य,
                               &shared_tuples_idx_map,
			       &pool))
        परत असत्य;

    परत सत्य;
  }

  तार्किक compile_shared_tuples (स्थिरांक hb_map_t& axes_index_map,
                              स्थिरांक hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित> coords_मोजणी_map;

    /* count the num of shared coords */
    साठी (tuple_variations_t& vars: glyph_variations)
    {
      साठी (tuple_delta_t& var : vars.tuple_vars)
      {
        जर (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          परत असत्य;
        अचिन्हित *मोजणी;
	अचिन्हित hash = hb_hash (&var.compiled_peak_coords);
        जर (coords_मोजणी_map.has_with_hash (&(var.compiled_peak_coords), hash, &मोजणी))
	  (*मोजणी)++;
        अन्यथा
          coords_मोजणी_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    जर (!coords_मोजणी_map || coords_मोजणी_map.in_error ())
      परत असत्य;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित>> shared_coords {
      + hb_iter (coords_मोजणी_map)
      | hb_filter ([] (स्थिरांक hb_pair_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित>& p) { परत p.second > 1; })
    };
    जर (unlikely (shared_coords.in_error ())) परत असत्य;

    /* no shared tuples: no coords are used more than once */
    जर (!shared_coords) परत सत्य;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.क्विक (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_मोजणी = hb_min (0xFFFu + 1, shared_coords.length);
    अचिन्हित len = shared_tuples_मोजणी * (shared_coords[0].first->length);
    जर (unlikely (!compiled_shared_tuples.alloc (len)))
      परत असत्य;

    साठी (अचिन्हित i = 0; i < shared_tuples_मोजणी; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      साठी (स्वतः c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    परत सत्य;
  }

  स्थिर पूर्णांक _cmp_coords (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    स्थिरांक hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *a = (स्थिरांक hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pa;
    स्थिरांक hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *b = (स्थिरांक hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pb;

    जर (a->second != b->second)
      परत b->second - a->second; // high to low

    परत b->first->as_array().cmp (a->first->as_array ());
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 तार्किक long_offset,
                                 अचिन्हित अंक_glyphs,
                                 अक्षर* glyph_var_data_offsets /* OUT: glyph var data offsets array */) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    जर (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    अन्यथा
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    अचिन्हित glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    अचिन्हित idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    जर (!cur_glyph) return_trace (असत्य);
    साठी (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      जर (long_offset)
        साठी (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      अन्यथा
        साठी (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      जर (idx >= glyph_variations.length) return_trace (असत्य);
      जर (!cur_glyph->serialize (c, सत्य, glyph_variations[idx])) return_trace (असत्य);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (अक्षर *) next_glyph - (अक्षर *) cur_glyph;

      जर (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      अन्यथा
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    जर (long_offset)
      साठी (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    अन्यथा
      साठी (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (सत्य);
  }
};

ढाँचा <typename T, typename Types>
संरचना gvar_GVAR
{
  using Glyph_मोजणी_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_मोजणी_Type>;
  स्थिर constexpr अचिन्हित glyph_मोजणी_max ()
  { परत (1u << (8 * Glyph_मोजणी_Type::static_size)) - 1; }

  तार्किक has_data () स्थिरांक { परत version.to_int () != 0; }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, हे, axis_मोजणी_2 * shared_Tuple_मोजणी) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_अंक_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_अंक_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  { परत sanitize_shallow (c); }

  तार्किक decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_मोजणी_Type>& glyph_vars /* OUT */) स्थिरांक
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    जर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    साठी (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_मोजणी_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    जर (new_gid_var_data_map.in_error ()) परत असत्य;

    hb_array_t<स्थिरांक F2DOT14> shared_tuples = (हे+sharedTuples).as_array ((अचिन्हित) shared_Tuple_मोजणी * (अचिन्हित) axis_मोजणी_2);
    परत glyph_vars.create_from_glyphs_var_data (axis_मोजणी_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
                  स्थिरांक glyph_variations_t<Glyph_मोजणी_Type>& glyph_vars,
                  Iterator it,
                  अचिन्हित axis_मोजणी,
                  अचिन्हित अंक_glyphs,
                  तार्किक force_long_offsets) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    जर (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_मोजणी_2 = axis_मोजणी;
    out->glyph_मोजणी_X = hb_min (glyph_मोजणी_max (), अंक_glyphs);

    अचिन्हित glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    तार्किक long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    अचिन्हित glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (अंक_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, असत्य);
    जर (!glyph_var_data_offsets) return_trace (असत्य);

    /* shared tuples */
    अचिन्हित shared_tuple_मोजणी = glyph_vars.compiled_shared_tuples_मोजणी ();
    out->shared_Tuple_मोजणी = shared_tuple_मोजणी;

    जर (!shared_tuple_मोजणी)
      out->sharedTuples = (अक्षर *) glyph_var_data_offsets + glyph_var_data_offsets_size - (अक्षर *) out;
    अन्यथा
    {
      hb_array_t<स्थिरांक F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      जर (!shared_tuples.arrayZ) return_trace (असत्य);
      out->sharedTuples = (स्थिरांक अक्षर *) shared_tuples.arrayZ - (अक्षर *) out;
    }

    अक्षर *glyph_var_data = c->start_embed<अक्षर> ();
    जर (!glyph_var_data) return_trace (असत्य);
    out->dataZ = glyph_var_data - (अक्षर *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, अंक_glyphs,
                                                       (अक्षर *) glyph_var_data_offsets));
  }

  तार्किक instantiate (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    glyph_variations_t<Glyph_मोजणी_Type> glyph_vars;
    जर (!decompile_glyph_variations (c, glyph_vars))
      return_trace (असत्य);

    जर (!glyph_vars.instantiate (c->plan)) return_trace (असत्य);
    जर (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (असत्य);

    अचिन्हित axis_मोजणी = c->plan->axes_index_map.get_population ();
    अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);

    तार्किक force_long_offsets = असत्य;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_मोजणी, अंक_glyphs, force_long_offsets));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned)
      return_trace (असत्य);

    जर (c->plan->normalized_coords)
      return_trace (instantiate (c));

    अचिन्हित glyph_मोजणी = version.to_int () ? c->plan->source->get_अंक_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    जर (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_मोजणी_2 = axis_मोजणी_2;
    out->shared_Tuple_मोजणी = shared_Tuple_मोजणी;

    अचिन्हित पूर्णांक अंक_glyphs = c->plan->अंक_output_glyphs ();
    out->glyph_मोजणी_X = hb_min (glyph_मोजणी_max (), अंक_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<अक्षर>> culled_vars;
    hb_set_t culled_gids;
    जर (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<स्थिरांक F2DOT14> shared_tuples_array =
	  (हे+sharedTuples).as_array ((अचिन्हित) shared_Tuple_मोजणी * (अचिन्हित) axis_मोजणी_2);
      जर (unlikely (!culled_vars.resize (अंक_glyphs))) return_trace (असत्य);
      स्वतः cull_it = hb_iter (c->plan->new_to_old_gid_list);
      जर (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      साठी (स्वतः &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_मोजणी, _.second);
	जर (var_data_bytes.length < GlyphVariationData::min_size) सुरूठेवा;
	तार्किक changed = असत्य;
	जर (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_मोजणी_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (असत्य);
	जर (changed) culled_gids.add (_.first);
      }
      जर (unlikely (culled_gids.in_error ())) return_trace (असत्य);
    }

    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    जर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    अचिन्हित subset_data_size = 0;
    अचिन्हित padding_size = 0;
    साठी (स्वतः &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      अचिन्हित glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_मोजणी, old_gid).length;
      जर (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      जर (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (असत्य);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    तार्किक long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    अचिन्हित subset_offsets_size = (long_offset ? 4 : 2) * (अंक_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, असत्य);
    जर (!subset_offsets) return_trace (असत्य);

    /* shared tuples */
    जर (!shared_Tuple_मोजणी || !sharedTuples)
      out->sharedTuples = (अक्षर *) subset_offsets + subset_offsets_size - (अक्षर *) out;
    अन्यथा
    {
      अचिन्हित पूर्णांक shared_tuple_size = F2DOT14::static_size * axis_मोजणी_2 * shared_Tuple_मोजणी;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      जर (!tuples) return_trace (असत्य);
      out->sharedTuples = (अक्षर *) tuples - (अक्षर *) out;
      hb_memcpy (tuples, हे+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    जर (long_offset)
      subset_data_size -= padding_size;
    अक्षर *subset_data = c->serializer->allocate_size<अक्षर> (subset_data_size, असत्य);
    जर (!subset_data) return_trace (असत्य);
    out->dataZ = subset_data - (अक्षर *) out;


    जर (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    अन्यथा
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    अचिन्हित पूर्णांक glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    जर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    साठी (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      जर (long_offset)
	साठी (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      अन्यथा
	साठी (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_मोजणी,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      अचिन्हित glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      जर (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      जर (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      अन्यथा
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    जर (long_offset)
      साठी (; last < अंक_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    अन्यथा
      साठी (; last < अंक_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (सत्य);
  }

  संरक्षित:
  स्थिरांक hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     अचिन्हित glyph_मोजणी,
					     hb_codepoint_t glyph) स्थिरांक
  {
    अचिन्हित start_offset = get_offset (glyph_मोजणी, glyph);
    अचिन्हित end_offset = get_offset (glyph_मोजणी, glyph+1);
    जर (unlikely (end_offset < start_offset)) परत hb_bytes_t ();
    अचिन्हित length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((अचिन्हित) dataZ) + start_offset, length);
    परत likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  तार्किक is_long_offset () स्थिरांक { परत flags & 1; }

  अचिन्हित get_offset (अचिन्हित glyph_मोजणी, अचिन्हित i) स्थिरांक
  {
    जर (unlikely (i > glyph_मोजणी)) परत 0;
    hb_barrier ();
    परत is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  स्थिरांक HBUINT32 * get_long_offset_array () स्थिरांक { परत (स्थिरांक HBUINT32 *) &offsetZ; }
  स्थिरांक HBUINT16 *get_short_offset_array () स्थिरांक { परत (स्थिरांक HBUINT16 *) &offsetZ; }

  सार्वजनिक:
  संरचना accelerator_t
  {

    hb_scalar_cache_t *create_cache () स्थिरांक
    {
      परत hb_scalar_cache_t::create (hb_min ((अचिन्हित) table->shared_Tuple_मोजणी,
						+TupleVariationHeader::max_shared_tuple_मोजणी));
    }

    स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    तार्किक has_data () स्थिरांक { परत table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_मोजणी_2 = table->version.to_int () ? face->get_अंक_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    खाजगी:

    स्थिर दशांश infer_delta (स्थिरांक hb_array_t<contour_point_t> points,
			      स्थिरांक hb_array_t<contour_point_t> deltas,
			      अचिन्हित पूर्णांक target, अचिन्हित पूर्णांक prev, अचिन्हित पूर्णांक next,
			      दशांश contour_point_t::*m)
    {
      दशांश target_val = points.arrayZ[target].*m;
      दशांश prev_val = points.arrayZ[prev].*m;
      दशांश next_val = points.arrayZ[next].*m;
      दशांश prev_delta =  deltas.arrayZ[prev].*m;
      दशांश next_delta =  deltas.arrayZ[next].*m;

      जर (prev_val == next_val)
	परत (prev_delta == next_delta) ? prev_delta : 0.f;
      अन्यथा जर (target_val <= hb_min (prev_val, next_val))
	परत (prev_val < next_val) ? prev_delta : next_delta;
      अन्यथा जर (target_val >= hb_max (prev_val, next_val))
	परत (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      दशांश r = (target_val - prev_val) / (next_val - prev_val);
      परत prev_delta + r * (next_delta - prev_delta);
    }

    स्थिर अचिन्हित पूर्णांक next_index (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
    { परत (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    ढाँचा <तार्किक is_x>
#endif
    स्थिर तार्किक decompile_deltas_add_to_points (स्थिरांक HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						दशांश scalar,
						स्थिरांक HBUINT8 *end,
						अचिन्हित start
#ifdef HB_OPTIMIZE_SIZE
						, तार्किक is_x
#endif
						)
    {
      अचिन्हित i = 0;
      अचिन्हित मोजणी = points.length;
      पर्यंत (i < मोजणी)
      {
	जर (unlikely (p + 1 > end)) परत असत्य;
	अचिन्हित control = *p++;
	अचिन्हित run_मोजणी = (control & Tuple_मूल्ये::मूल्य_RUN_मोजणी_MASK) + 1;
	अचिन्हित stop = i + run_मोजणी;
	जर (unlikely (stop > मोजणी)) परत असत्य;

	अचिन्हित skip = i < start ? hb_min (start - i, run_मोजणी) : 0;
	i += skip;

	निवड (control & Tuple_मूल्ये::मूल्ये_SIZE_MASK)
	{
	  स्थिती Tuple_मूल्ये::मूल्ये_ARE_ZEROS:
	    i = stop;
	    थांबा;
	  स्थिती Tuple_मूल्ये::मूल्ये_ARE_शब्द:
	  {
	    जर (unlikely (p + run_मोजणी * HBINT16::static_size > end)) परत असत्य;
	    p += skip * HBINT16::static_size;
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT16 *) p;
	    साठी (; i < stop; i++)
	    {
	      दशांश v = *pp++ * scalar;
	      जर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिरांक HBUINT8 *) pp;
	  }
	  थांबा;
	  स्थिती Tuple_मूल्ये::मूल्ये_ARE_LONGS:
	  {
	    जर (unlikely (p + run_मोजणी * HBINT32::static_size > end)) परत असत्य;
	    p += skip * HBINT32::static_size;
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT32 *) p;
	    साठी (; i < stop; i++)
	    {
	      दशांश v = *pp++ * scalar;
	      जर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिरांक HBUINT8 *) pp;
	  }
	  थांबा;
	  स्थिती Tuple_मूल्ये::मूल्ये_ARE_BYTES:
	  {
	    जर (unlikely (p + run_मोजणी > end)) परत असत्य;
	    p += skip * HBINT8::static_size;
	    स्थिरांक स्वतः *pp = (स्थिरांक HBINT8 *) p;
	    साठी (; i < stop; i++)
	    {
	      दशांश v = *pp++ * scalar;
	      जर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिरांक HBUINT8 *) pp;
	  }
	  थांबा;
	}
      }
      परत सत्य;
    }
    सार्वजनिक:
    तार्किक apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<स्थिरांक पूर्णांक> coords,
				 स्थिरांक hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 तार्किक phantom_only = असत्य,
				 int64_t *budget = nullptr) स्थिरांक
    {
      जर (unlikely (glyph >= glyph_मोजणी_2)) परत सत्य;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_मोजणी_2, glyph);
      जर (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) परत सत्य;

      स्वतः &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      जर (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_मोजणी_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	परत सत्य; /* so isn't applied at all */

      तार्किक any_private_points = असत्य;
      तार्किक private_points_checked = असत्य;

      /* Save original points for inferred delta calculation */
      स्वतः &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      स्वतः orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      स्वतः &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      स्वतः deltas = deltas_vec.as_array ();

      अचिन्हित अंक_coords = table->axis_मोजणी_2;
      hb_array_t<स्थिरांक F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_मोजणी * अंक_coords);

      स्वतः &private_indices = scratch.private_indices;
      स्वतः &x_deltas = scratch.x_deltas;
      स्वतः &y_deltas = scratch.y_deltas;

      अचिन्हित मोजणी = points.length;
      तार्किक flush = असत्य;

      करा
      {
	दशांश scalar = iterator.current_tuple->calculate_scalar (coords, अंक_coords, shared_tuples,
								 scalar_cache);

	जर (scalar == 0.f) सुरूठेवा;

	जर (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && मोजणी >= 4 ? 4 : मोजणी)))
	  परत असत्य;

	जर (!private_points_checked)
	{
	  स्वतः scan = iterator;
	  करा
	  {
	    जर (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = सत्य;
	      थांबा;
	    }
	  } पर्यंत (scan.move_to_next ());
	  private_points_checked = सत्य;
	}
	स्थिरांक HBUINT8 *p = iterator.get_serialized_data ();
	अचिन्हित पूर्णांक length = iterator.current_tuple->get_data_size ();
	जर (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  परत असत्य;

	जर (!deltas)
	{
	  जर (unlikely (!deltas_vec.resize_dirty  (मोजणी))) परत असत्य;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? मोजणी - 4 : 0), 0,
		     (phantom_only ? 4 : मोजणी) * आकार (deltas[0]));
	}

	स्थिरांक HBUINT8 *end = p + length;

	तार्किक has_private_points = iterator.current_tuple->has_private_points ();
	जर (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  परत असत्य;
	स्थिरांक hb_array_t<अचिन्हित पूर्णांक> &indices = has_private_points ? private_indices : shared_indices;

	तार्किक apply_to_all = (indices.length == 0);
	अचिन्हित अंक_deltas = apply_to_all ? points.length : indices.length;
	अचिन्हित start_deltas = (apply_to_all && phantom_only && अंक_deltas >= 4 ? अंक_deltas - 4 : 0);

	जर (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  जर (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, सत्य))) परत असत्य;
	  जर (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, असत्य))) परत असत्य;
#अन्यथा
	  जर (unlikely (!decompile_deltas_add_to_points<सत्य> (p, points, scalar, end, start_deltas))) परत असत्य;
	  जर (unlikely (!decompile_deltas_add_to_points<असत्य> (p, points, scalar, end, start_deltas))) परत असत्य;
#endif
	  सुरूठेवा;
	}

	जर (unlikely (!x_deltas.resize_dirty  (अंक_deltas))) परत असत्य;
	जर (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, असत्य, start_deltas))) परत असत्य;
	जर (unlikely (!y_deltas.resize_dirty  (अंक_deltas))) परत असत्य;
	जर (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, असत्य, start_deltas))) परत असत्य;

	जर (!apply_to_all)
	{
	  जर (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    जर (unlikely (orig_points_vec.in_error ())) परत असत्य;
	    orig_points = orig_points_vec.as_array ();
	  }

	  जर (flush)
	  {
	    साठी (अचिन्हित पूर्णांक i = phantom_only ? मोजणी - 4 : 0; i < मोजणी; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? मोजणी - 4 : 0), 0,
		     (phantom_only ? 4 : मोजणी) * आकार (deltas[0]));
	}

	जर (HB_OPTIMIZE_SIZE_VAL)
	{
	  साठी (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	  {
	    अचिन्हित पूर्णांक pt_index;
	    जर (apply_to_all)
	      pt_index = i;
	    अन्यथा
	    {
	      pt_index = indices[i];
	      जर (unlikely (pt_index >= deltas.length)) सुरूठेवा;
	    }
	    जर (phantom_only && pt_index < मोजणी - 4) सुरूठेवा;
	    स्वतः &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	अन्यथा
	{
	  /* Ouch. Four cases... for optimization. */
	  जर (scalar != 1.0f)
	  {
	    जर (apply_to_all)
	      साठी (अचिन्हित पूर्णांक i = phantom_only ? मोजणी - 4 : 0; i < मोजणी; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    अन्यथा
	      साठी (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णांक pt_index = indices[i];
		जर (unlikely (pt_index >= deltas.length)) सुरूठेवा;
		जर (phantom_only && pt_index < मोजणी - 4) सुरूठेवा;
		स्वतः &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  अन्यथा
	  {
	    जर (apply_to_all)
	      साठी (अचिन्हित पूर्णांक i = phantom_only ? मोजणी - 4 : 0; i < मोजणी; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    अन्यथा
	      साठी (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णांक pt_index = indices[i];
		जर (unlikely (pt_index >= deltas.length)) सुरूठेवा;
		जर (phantom_only && pt_index < मोजणी - 4) सुरूठेवा;
		स्वतः &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	जर (!apply_to_all && !phantom_only)
	{
	  अचिन्हित start_point = 0;
	  अचिन्हित end_point = 0;
	  पर्यंत (सत्य)
	  {
	    पर्यंत (end_point < मोजणी && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    जर (unlikely (end_point == मोजणी)) थांबा;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    अचिन्हित unref_मोजणी = 0;
	    साठी (अचिन्हित i = start_point; i < end_point + 1; i++)
	      unref_मोजणी += deltas.arrayZ[i].flag;
	    unref_मोजणी = (end_point - start_point + 1) - unref_मोजणी;

	    अचिन्हित j = start_point;
	    जर (unref_मोजणी == 0 || unref_मोजणी > end_point - start_point)
	      जा no_more_gaps;

	    साठी (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      अचिन्हित पूर्णांक prev, next, i;
	      साठी (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		जर (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) थांबा;
	      }
	      prev = j = i;
	      साठी (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		जर (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) थांबा;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      साठी (;;)
	      {
		i = next_index (i, start_point, end_point);
		जर (i == next) थांबा;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		जर (--unref_मोजणी == 0) जा no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = सत्य;

      } पर्यंत (iterator.move_to_next ());

      जर (flush)
      {
	साठी (अचिन्हित पूर्णांक i = phantom_only ? मोजणी - 4 : 0; i < मोजणी; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      परत सत्य;
    }

    अचिन्हित पूर्णांक get_axis_मोजणी () स्थिरांक { परत table->axis_मोजणी_2; }

    खाजगी:
    hb_blob_ptr_t<gvar_GVAR> table;
    अचिन्हित glyph_मोजणी_2;
  };

  संरक्षित:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_मोजणी_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_मोजणी;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_मोजणी_Type
		glyph_मोजणी_X;	/* The number of glyphs in this font. This must match the number of
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
  DEFINE_SIZE_ARRAY (18 + Glyph_मोजणी_Type::static_size, offsetZ);
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

  hb_scalar_cache_t *create_cache () स्थिरांक
  {
    जर (has_data ()) परत HB_DUAL_GET (*हे, create_cache ());
    परत nullptr;
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  तार्किक apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<स्थिरांक पूर्णांक> coords,
			       स्थिरांक hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       तार्किक phantom_only = असत्य,
			       int64_t *budget = nullptr) स्थिरांक
  {
    परत HB_DUAL_GET (*हे, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  अचिन्हित पूर्णांक get_axis_मोजणी () स्थिरांक
  {
    जर (has_data ()) परत HB_DUAL_GET (*हे, get_axis_मोजणी ());
    परत 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
