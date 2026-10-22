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
  hb_vector_t<अचिन्हित पूर्णाङ्क> shared_indices;
  hb_vector_t<अचिन्हित पूर्णाङ्क> private_indices;
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
  गुप्त:
  अचिन्हित shared_tuples_गिनती = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  सार्वजनिक:
  अचिन्हित compiled_shared_tuples_गिनती () ध्रुव
  { प्रत्यागम shared_tuples_गिनती; }

  अचिन्हित compiled_byte_size () ध्रुव
  {
    अचिन्हित byte_size = 0;
    प्रति (ध्रुव स्वतः& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    प्रत्यागम byte_size;
  }

  तार्किक create_from_glyphs_var_data (अचिन्हित axis_गिनती,
                                    ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                                    ध्रुव hb_subset_plan_t *plan,
                                    ध्रुव hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    यदि (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      प्रत्यागम असत्य;

    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    प्रति (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      यदि (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        प्रत्यागम असत्य;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      ध्रुव GlyphVariationData* p = reinterpret_cast<ध्रुव GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<अचिन्हित> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      यदि (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_गिनती,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        अनुवर्त;
      }

      तार्किक is_composite_glyph = असत्य;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      यदि (!p->decompile_tuple_variations (all_contour_points->length, सत्य /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        प्रत्यागम असत्य;
      glyph_variations.push (std::move (tuple_vars));
    }
    प्रत्यागम !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  तार्किक instantiate (ध्रुव hb_subset_plan_t *plan)
  {
    अचिन्हित गिनती = plan->new_to_old_gid_list.length;
    तार्किक iup_optimize = असत्य;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      यदि (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        प्रत्यागम असत्य;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      यदि (iup_optimize)
      {
        points = *all_points;
        यदि (unlikely (points.in_error ())) प्रत्यागम असत्य;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      यदि (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      यदि (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
  }

  तार्किक compile_bytes (ध्रुव hb_map_t& axes_index_map,
                      ध्रुव hb_map_t& axes_old_index_tag_map)
  {
    यदि (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      प्रत्यागम असत्य;
    प्रति (tuple_variations_t& vars: glyph_variations)
      यदि (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               सत्य, /* use shared points*/
                               सत्य,
                               &shared_tuples_idx_map,
			       &pool))
        प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }

  तार्किक compile_shared_tuples (ध्रुव hb_map_t& axes_index_map,
                              ध्रुव hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित> coords_गिनती_map;

    /* count the num of shared coords */
    प्रति (tuple_variations_t& vars: glyph_variations)
    {
      प्रति (tuple_delta_t& var : vars.tuple_vars)
      {
        यदि (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          प्रत्यागम असत्य;
        अचिन्हित *गिनती;
	अचिन्हित hash = hb_hash (&var.compiled_peak_coords);
        यदि (coords_गिनती_map.has_with_hash (&(var.compiled_peak_coords), hash, &गिनती))
	  (*गिनती)++;
        अन्यथा
          coords_गिनती_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    यदि (!coords_गिनती_map || coords_गिनती_map.in_error ())
      प्रत्यागम असत्य;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित>> shared_coords {
      + hb_iter (coords_गिनती_map)
      | hb_filter ([] (ध्रुव hb_pair_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित>& p) { प्रत्यागम p.second > 1; })
    };
    यदि (unlikely (shared_coords.in_error ())) प्रत्यागम असत्य;

    /* no shared tuples: no coords are used more than once */
    यदि (!shared_coords) प्रत्यागम सत्य;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.क्विक (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_गिनती = hb_min (0xFFFu + 1, shared_coords.length);
    अचिन्हित len = shared_tuples_गिनती * (shared_coords[0].first->length);
    यदि (unlikely (!compiled_shared_tuples.alloc (len)))
      प्रत्यागम असत्य;

    प्रति (अचिन्हित i = 0; i < shared_tuples_गिनती; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      प्रति (स्वतः c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    प्रत्यागम सत्य;
  }

  स्थिर पूर्णाङ्क _cmp_coords (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *a = (ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pa;
    ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *b = (ध्रुव hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pb;

    यदि (a->second != b->second)
      प्रत्यागम b->second - a->second; // high to low

    प्रत्यागम b->first->as_array().cmp (a->first->as_array ());
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 तार्किक long_offset,
                                 अचिन्हित अंक_glyphs,
                                 वर्ण* glyph_var_data_offsets /* OUT: glyph var data offsets array */) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    यदि (long_offset)
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
    यदि (!cur_glyph) return_trace (असत्य);
    प्रति (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      यदि (long_offset)
        प्रति (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      अन्यथा
        प्रति (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      यदि (idx >= glyph_variations.length) return_trace (असत्य);
      यदि (!cur_glyph->serialize (c, सत्य, glyph_variations[idx])) return_trace (असत्य);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (वर्ण *) next_glyph - (वर्ण *) cur_glyph;

      यदि (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      अन्यथा
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    यदि (long_offset)
      प्रति (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    अन्यथा
      प्रति (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (सत्य);
  }
};

ढाँचा <typename T, typename Types>
संरचना gvar_GVAR
{
  using Glyph_गिनती_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_गिनती_Type>;
  स्थिर constexpr अचिन्हित glyph_गिनती_max ()
  { प्रत्यागम (1u << (8 * Glyph_गिनती_Type::static_size)) - 1; }

  तार्किक has_data () ध्रुव { प्रत्यागम version.to_int () != 0; }

  तार्किक sanitize_shallow (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, एतत्, axis_गिनती_2 * shared_Tuple_गिनती) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_अंक_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_अंक_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  { प्रत्यागम sanitize_shallow (c); }

  तार्किक decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_गिनती_Type>& glyph_vars /* OUT */) ध्रुव
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    यदि (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    प्रति (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_गिनती_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    यदि (new_gid_var_data_map.in_error ()) प्रत्यागम असत्य;

    hb_array_t<ध्रुव F2DOT14> shared_tuples = (एतत्+sharedTuples).as_array ((अचिन्हित) shared_Tuple_गिनती * (अचिन्हित) axis_गिनती_2);
    प्रत्यागम glyph_vars.create_from_glyphs_var_data (axis_गिनती_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
                  ध्रुव glyph_variations_t<Glyph_गिनती_Type>& glyph_vars,
                  Iterator it,
                  अचिन्हित axis_गिनती,
                  अचिन्हित अंक_glyphs,
                  तार्किक force_long_offsets) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    यदि (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गिनती_2 = axis_गिनती;
    out->glyph_गिनती_X = hb_min (glyph_गिनती_max (), अंक_glyphs);

    अचिन्हित glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    तार्किक long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    अचिन्हित glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (अंक_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, असत्य);
    यदि (!glyph_var_data_offsets) return_trace (असत्य);

    /* shared tuples */
    अचिन्हित shared_tuple_गिनती = glyph_vars.compiled_shared_tuples_गिनती ();
    out->shared_Tuple_गिनती = shared_tuple_गिनती;

    यदि (!shared_tuple_गिनती)
      out->sharedTuples = (वर्ण *) glyph_var_data_offsets + glyph_var_data_offsets_size - (वर्ण *) out;
    अन्यथा
    {
      hb_array_t<ध्रुव F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      यदि (!shared_tuples.arrayZ) return_trace (असत्य);
      out->sharedTuples = (ध्रुव वर्ण *) shared_tuples.arrayZ - (वर्ण *) out;
    }

    वर्ण *glyph_var_data = c->start_embed<वर्ण> ();
    यदि (!glyph_var_data) return_trace (असत्य);
    out->dataZ = glyph_var_data - (वर्ण *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, अंक_glyphs,
                                                       (वर्ण *) glyph_var_data_offsets));
  }

  तार्किक instantiate (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    glyph_variations_t<Glyph_गिनती_Type> glyph_vars;
    यदि (!decompile_glyph_variations (c, glyph_vars))
      return_trace (असत्य);

    यदि (!glyph_vars.instantiate (c->plan)) return_trace (असत्य);
    यदि (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (असत्य);

    अचिन्हित axis_गिनती = c->plan->axes_index_map.get_population ();
    अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);

    तार्किक force_long_offsets = असत्य;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_गिनती, अंक_glyphs, force_long_offsets));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (c->plan->all_axes_pinned)
      return_trace (असत्य);

    यदि (c->plan->normalized_coords)
      return_trace (instantiate (c));

    अचिन्हित glyph_गिनती = version.to_int () ? c->plan->source->get_अंक_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    यदि (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गिनती_2 = axis_गिनती_2;
    out->shared_Tuple_गिनती = shared_Tuple_गिनती;

    अचिन्हित पूर्णाङ्क अंक_glyphs = c->plan->अंक_output_glyphs ();
    out->glyph_गिनती_X = hb_min (glyph_गिनती_max (), अंक_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<वर्ण>> culled_vars;
    hb_set_t culled_gids;
    यदि (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ध्रुव F2DOT14> shared_tuples_array =
	  (एतत्+sharedTuples).as_array ((अचिन्हित) shared_Tuple_गिनती * (अचिन्हित) axis_गिनती_2);
      यदि (unlikely (!culled_vars.resize (अंक_glyphs))) return_trace (असत्य);
      स्वतः cull_it = hb_iter (c->plan->new_to_old_gid_list);
      यदि (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      प्रति (स्वतः &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_गिनती, _.second);
	यदि (var_data_bytes.length < GlyphVariationData::min_size) अनुवर्त;
	तार्किक changed = असत्य;
	यदि (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_गिनती_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (असत्य);
	यदि (changed) culled_gids.add (_.first);
      }
      यदि (unlikely (culled_gids.in_error ())) return_trace (असत्य);
    }

    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    यदि (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    अचिन्हित subset_data_size = 0;
    अचिन्हित padding_size = 0;
    प्रति (स्वतः &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      अचिन्हित glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_गिनती, old_gid).length;
      यदि (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      यदि (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
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
    यदि (!subset_offsets) return_trace (असत्य);

    /* shared tuples */
    यदि (!shared_Tuple_गिनती || !sharedTuples)
      out->sharedTuples = (वर्ण *) subset_offsets + subset_offsets_size - (वर्ण *) out;
    अन्यथा
    {
      अचिन्हित पूर्णाङ्क shared_tuple_size = F2DOT14::static_size * axis_गिनती_2 * shared_Tuple_गिनती;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      यदि (!tuples) return_trace (असत्य);
      out->sharedTuples = (वर्ण *) tuples - (वर्ण *) out;
      hb_memcpy (tuples, एतत्+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    यदि (long_offset)
      subset_data_size -= padding_size;
    वर्ण *subset_data = c->serializer->allocate_size<वर्ण> (subset_data_size, असत्य);
    यदि (!subset_data) return_trace (असत्य);
    out->dataZ = subset_data - (वर्ण *) out;


    यदि (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    अन्यथा
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    अचिन्हित पूर्णाङ्क glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    यदि (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    प्रति (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      यदि (long_offset)
	प्रति (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      अन्यथा
	प्रति (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_गिनती,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      अचिन्हित glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      यदि (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      यदि (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      अन्यथा
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    यदि (long_offset)
      प्रति (; last < अंक_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    अन्यथा
      प्रति (; last < अंक_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (सत्य);
  }

  रक्षित:
  ध्रुव hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     अचिन्हित glyph_गिनती,
					     hb_codepoint_t glyph) ध्रुव
  {
    अचिन्हित start_offset = get_offset (glyph_गिनती, glyph);
    अचिन्हित end_offset = get_offset (glyph_गिनती, glyph+1);
    यदि (unlikely (end_offset < start_offset)) प्रत्यागम hb_bytes_t ();
    अचिन्हित length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((अचिन्हित) dataZ) + start_offset, length);
    प्रत्यागम likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  तार्किक is_long_offset () ध्रुव { प्रत्यागम flags & 1; }

  अचिन्हित get_offset (अचिन्हित glyph_गिनती, अचिन्हित i) ध्रुव
  {
    यदि (unlikely (i > glyph_गिनती)) प्रत्यागम 0;
    hb_barrier ();
    प्रत्यागम is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  ध्रुव HBUINT32 * get_long_offset_array () ध्रुव { प्रत्यागम (ध्रुव HBUINT32 *) &offsetZ; }
  ध्रुव HBUINT16 *get_short_offset_array () ध्रुव { प्रत्यागम (ध्रुव HBUINT16 *) &offsetZ; }

  सार्वजनिक:
  संरचना accelerator_t
  {

    hb_scalar_cache_t *create_cache () ध्रुव
    {
      प्रत्यागम hb_scalar_cache_t::create (hb_min ((अचिन्हित) table->shared_Tuple_गिनती,
						+TupleVariationHeader::max_shared_tuple_गिनती));
    }

    स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    तार्किक has_data () ध्रुव { प्रत्यागम table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_गिनती_2 = table->version.to_int () ? face->get_अंक_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    गुप्त:

    स्थिर भिन्न infer_delta (ध्रुव hb_array_t<contour_point_t> points,
			      ध्रुव hb_array_t<contour_point_t> deltas,
			      अचिन्हित पूर्णाङ्क target, अचिन्हित पूर्णाङ्क prev, अचिन्हित पूर्णाङ्क next,
			      भिन्न contour_point_t::*m)
    {
      भिन्न target_val = points.arrayZ[target].*m;
      भिन्न prev_val = points.arrayZ[prev].*m;
      भिन्न next_val = points.arrayZ[next].*m;
      भिन्न prev_delta =  deltas.arrayZ[prev].*m;
      भिन्न next_delta =  deltas.arrayZ[next].*m;

      यदि (prev_val == next_val)
	प्रत्यागम (prev_delta == next_delta) ? prev_delta : 0.f;
      अन्यथा यदि (target_val <= hb_min (prev_val, next_val))
	प्रत्यागम (prev_val < next_val) ? prev_delta : next_delta;
      अन्यथा यदि (target_val >= hb_max (prev_val, next_val))
	प्रत्यागम (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      भिन्न r = (target_val - prev_val) / (next_val - prev_val);
      प्रत्यागम prev_delta + r * (next_delta - prev_delta);
    }

    स्थिर अचिन्हित पूर्णाङ्क next_index (अचिन्हित पूर्णाङ्क i, अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
    { प्रत्यागम (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    ढाँचा <तार्किक is_x>
#endif
    स्थिर तार्किक decompile_deltas_add_to_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						भिन्न scalar,
						ध्रुव HBUINT8 *end,
						अचिन्हित start
#ifdef HB_OPTIMIZE_SIZE
						, तार्किक is_x
#endif
						)
    {
      अचिन्हित i = 0;
      अचिन्हित गिनती = points.length;
      यावत् (i < गिनती)
      {
	यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
	अचिन्हित control = *p++;
	अचिन्हित run_गिनती = (control & Tuple_मूल्यावली::मूल्य_RUN_गिनती_MASK) + 1;
	अचिन्हित stop = i + run_गिनती;
	यदि (unlikely (stop > गिनती)) प्रत्यागम असत्य;

	अचिन्हित skip = i < start ? hb_min (start - i, run_गिनती) : 0;
	i += skip;

	विकल्प (control & Tuple_मूल्यावली::मूल्यावली_SIZE_MASK)
	{
	  पक्ष Tuple_मूल्यावली::मूल्यावली_ARE_ZEROS:
	    i = stop;
	    भङ्ग;
	  पक्ष Tuple_मूल्यावली::मूल्यावली_ARE_शब्दगण:
	  {
	    यदि (unlikely (p + run_गिनती * HBINT16::static_size > end)) प्रत्यागम असत्य;
	    p += skip * HBINT16::static_size;
	    ध्रुव स्वतः *pp = (ध्रुव HBINT16 *) p;
	    प्रति (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      यदि (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	  पक्ष Tuple_मूल्यावली::मूल्यावली_ARE_LONGS:
	  {
	    यदि (unlikely (p + run_गिनती * HBINT32::static_size > end)) प्रत्यागम असत्य;
	    p += skip * HBINT32::static_size;
	    ध्रुव स्वतः *pp = (ध्रुव HBINT32 *) p;
	    प्रति (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      यदि (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	  पक्ष Tuple_मूल्यावली::मूल्यावली_ARE_BYTES:
	  {
	    यदि (unlikely (p + run_गिनती > end)) प्रत्यागम असत्य;
	    p += skip * HBINT8::static_size;
	    ध्रुव स्वतः *pp = (ध्रुव HBINT8 *) p;
	    प्रति (; i < stop; i++)
	    {
	      भिन्न v = *pp++ * scalar;
	      यदि (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (ध्रुव HBUINT8 *) pp;
	  }
	  भङ्ग;
	}
      }
      प्रत्यागम सत्य;
    }
    सार्वजनिक:
    तार्किक apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<ध्रुव पूर्णाङ्क> coords,
				 ध्रुव hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 तार्किक phantom_only = असत्य,
				 int64_t *budget = nullptr) ध्रुव
    {
      यदि (unlikely (glyph >= glyph_गिनती_2)) प्रत्यागम सत्य;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_गिनती_2, glyph);
      यदि (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) प्रत्यागम सत्य;

      स्वतः &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      यदि (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_गिनती_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	प्रत्यागम सत्य; /* so isn't applied at all */

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

      अचिन्हित अंक_coords = table->axis_गिनती_2;
      hb_array_t<ध्रुव F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_गिनती * अंक_coords);

      स्वतः &private_indices = scratch.private_indices;
      स्वतः &x_deltas = scratch.x_deltas;
      स्वतः &y_deltas = scratch.y_deltas;

      अचिन्हित गिनती = points.length;
      तार्किक flush = असत्य;

      कुरु
      {
	भिन्न scalar = iterator.current_tuple->calculate_scalar (coords, अंक_coords, shared_tuples,
								 scalar_cache);

	यदि (scalar == 0.f) अनुवर्त;

	यदि (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && गिनती >= 4 ? 4 : गिनती)))
	  प्रत्यागम असत्य;

	यदि (!private_points_checked)
	{
	  स्वतः scan = iterator;
	  कुरु
	  {
	    यदि (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = सत्य;
	      भङ्ग;
	    }
	  } यावत् (scan.move_to_next ());
	  private_points_checked = सत्य;
	}
	ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
	अचिन्हित पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
	यदि (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  प्रत्यागम असत्य;

	यदि (!deltas)
	{
	  यदि (unlikely (!deltas_vec.resize_dirty  (गिनती))) प्रत्यागम असत्य;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? गिनती - 4 : 0), 0,
		     (phantom_only ? 4 : गिनती) * परिमाण (deltas[0]));
	}

	ध्रुव HBUINT8 *end = p + length;

	तार्किक has_private_points = iterator.current_tuple->has_private_points ();
	यदि (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  प्रत्यागम असत्य;
	ध्रुव hb_array_t<अचिन्हित पूर्णाङ्क> &indices = has_private_points ? private_indices : shared_indices;

	तार्किक apply_to_all = (indices.length == 0);
	अचिन्हित अंक_deltas = apply_to_all ? points.length : indices.length;
	अचिन्हित start_deltas = (apply_to_all && phantom_only && अंक_deltas >= 4 ? अंक_deltas - 4 : 0);

	यदि (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  यदि (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, सत्य))) प्रत्यागम असत्य;
	  यदि (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, असत्य))) प्रत्यागम असत्य;
#अन्यथा
	  यदि (unlikely (!decompile_deltas_add_to_points<सत्य> (p, points, scalar, end, start_deltas))) प्रत्यागम असत्य;
	  यदि (unlikely (!decompile_deltas_add_to_points<असत्य> (p, points, scalar, end, start_deltas))) प्रत्यागम असत्य;
#endif
	  अनुवर्त;
	}

	यदि (unlikely (!x_deltas.resize_dirty  (अंक_deltas))) प्रत्यागम असत्य;
	यदि (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, असत्य, start_deltas))) प्रत्यागम असत्य;
	यदि (unlikely (!y_deltas.resize_dirty  (अंक_deltas))) प्रत्यागम असत्य;
	यदि (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, असत्य, start_deltas))) प्रत्यागम असत्य;

	यदि (!apply_to_all)
	{
	  यदि (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    यदि (unlikely (orig_points_vec.in_error ())) प्रत्यागम असत्य;
	    orig_points = orig_points_vec.as_array ();
	  }

	  यदि (flush)
	  {
	    प्रति (अचिन्हित पूर्णाङ्क i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? गिनती - 4 : 0), 0,
		     (phantom_only ? 4 : गिनती) * परिमाण (deltas[0]));
	}

	यदि (HB_OPTIMIZE_SIZE_VAL)
	{
	  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_deltas; i++)
	  {
	    अचिन्हित पूर्णाङ्क pt_index;
	    यदि (apply_to_all)
	      pt_index = i;
	    अन्यथा
	    {
	      pt_index = indices[i];
	      यदि (unlikely (pt_index >= deltas.length)) अनुवर्त;
	    }
	    यदि (phantom_only && pt_index < गिनती - 4) अनुवर्त;
	    स्वतः &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	अन्यथा
	{
	  /* Ouch. Four cases... for optimization. */
	  यदि (scalar != 1.0f)
	  {
	    यदि (apply_to_all)
	      प्रति (अचिन्हित पूर्णाङ्क i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    अन्यथा
	      प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णाङ्क pt_index = indices[i];
		यदि (unlikely (pt_index >= deltas.length)) अनुवर्त;
		यदि (phantom_only && pt_index < गिनती - 4) अनुवर्त;
		स्वतः &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  अन्यथा
	  {
	    यदि (apply_to_all)
	      प्रति (अचिन्हित पूर्णाङ्क i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    अन्यथा
	      प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णाङ्क pt_index = indices[i];
		यदि (unlikely (pt_index >= deltas.length)) अनुवर्त;
		यदि (phantom_only && pt_index < गिनती - 4) अनुवर्त;
		स्वतः &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	यदि (!apply_to_all && !phantom_only)
	{
	  अचिन्हित start_point = 0;
	  अचिन्हित end_point = 0;
	  यावत् (सत्य)
	  {
	    यावत् (end_point < गिनती && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    यदि (unlikely (end_point == गिनती)) भङ्ग;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    अचिन्हित unref_गिनती = 0;
	    प्रति (अचिन्हित i = start_point; i < end_point + 1; i++)
	      unref_गिनती += deltas.arrayZ[i].flag;
	    unref_गिनती = (end_point - start_point + 1) - unref_गिनती;

	    अचिन्हित j = start_point;
	    यदि (unref_गिनती == 0 || unref_गिनती > end_point - start_point)
	      गच्छ no_more_gaps;

	    प्रति (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      अचिन्हित पूर्णाङ्क prev, next, i;
	      प्रति (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		यदि (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) भङ्ग;
	      }
	      prev = j = i;
	      प्रति (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		यदि (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) भङ्ग;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      प्रति (;;)
	      {
		i = next_index (i, start_point, end_point);
		यदि (i == next) भङ्ग;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		यदि (--unref_गिनती == 0) गच्छ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = सत्य;

      } यावत् (iterator.move_to_next ());

      यदि (flush)
      {
	प्रति (अचिन्हित पूर्णाङ्क i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      प्रत्यागम सत्य;
    }

    अचिन्हित पूर्णाङ्क get_axis_गिनती () ध्रुव { प्रत्यागम table->axis_गिनती_2; }

    गुप्त:
    hb_blob_ptr_t<gvar_GVAR> table;
    अचिन्हित glyph_गिनती_2;
  };

  रक्षित:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_गिनती_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_गिनती;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_गिनती_Type
		glyph_गिनती_X;	/* The number of glyphs in this font. This must match the number of
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
  DEFINE_SIZE_ARRAY (18 + Glyph_गिनती_Type::static_size, offsetZ);
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
    यदि (has_data ()) प्रत्यागम HB_DUAL_GET (*एतत्, create_cache ());
    प्रत्यागम nullptr;
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
			       तार्किक phantom_only = असत्य,
			       int64_t *budget = nullptr) ध्रुव
  {
    प्रत्यागम HB_DUAL_GET (*एतत्, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  अचिन्हित पूर्णाङ्क get_axis_गिनती () ध्रुव
  {
    यदि (has_data ()) प्रत्यागम HB_DUAL_GET (*एतत्, get_axis_गिनती ());
    प्रत्यागम 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
