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

#अगर_अघोषित HB_OT_VAR_GVAR_TABLE_HH
#घोषणा HB_OT_VAR_GVAR_TABLE_HH

#समावेश "hb-decycler.hh"
#समावेश "hb-open-type.hh"
#समावेश "hb-ot-dual.hh"
#समावेश "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#घोषणा HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#घोषणा HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

काष्ठा hb_glyf_scratch_t
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

namespace OT {

ढाँचा <typename OffsetType>
काष्ठा glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename Tuple_Variation_तथ्य<OffsetType>::tuple_variations_t;
  using Glyph_Variation_तथ्य = Tuple_Variation_तथ्य<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  निजी:
  अचिन्हित shared_tuples_गिनती = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  खुला:
  अचिन्हित compiled_shared_tuples_गिनती () स्थिर
  { वापस shared_tuples_गिनती; }

  अचिन्हित compiled_byte_आकार () स्थिर
  {
    अचिन्हित byte_आकार = 0;
    क्रम (स्थिर स्वतः& _ : glyph_variations)
      byte_आकार += _.get_compiled_byte_आकार ();

    वापस byte_आकार;
  }

  bool create_from_glyphs_var_तथ्य (अचिन्हित axis_गिनती,
                                    स्थिर hb_array_t<स्थिर F2DOT14> shared_tuples,
                                    स्थिर hb_subset_plan_t *plan,
                                    स्थिर hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_तथ्य_map)
  {
    अगर (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.लंबाई)))
      वापस false;

    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    क्रम (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      अगर (!new_gid_var_तथ्य_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        वापस false;
      hb_bytes_t var_तथ्य = new_gid_var_तथ्य_map.लो (new_gid);

      स्थिर Glyph_Variation_तथ्य* p = reinterpret_cast<स्थिर Glyph_Variation_तथ्य*> (var_तथ्य.arrayZ);
      typename Glyph_Variation_तथ्य::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<अचिन्हित> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      अगर (!var_तथ्य || ! p->has_तथ्य () || !all_contour_points->लंबाई ||
          !Glyph_Variation_तथ्य::get_tuple_iterator (var_तथ्य, axis_गिनती,
                                                   var_तथ्य.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        जारी;
      }

      bool is_composite_glyph = false;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      अगर (!p->decompile_tuple_variations (all_contour_points->लंबाई, true /* is_gvar */,
                                          iterator, &(plan->axes_old_सूचकांक_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        वापस false;
      glyph_variations.push (std::move (tuple_vars));
    }
    वापस !glyph_variations.in_error () && glyph_variations.लंबाई == plan->new_to_old_gid_list.लंबाई;
  }

  bool instantiate (स्थिर hb_subset_plan_t *plan)
  {
    अचिन्हित गिनती = plan->new_to_old_gid_list.लंबाई;
    bool iup_optimize = false;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      अगर (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        वापस false;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      अगर (iup_optimize)
      {
        points = *all_points;
        अगर (unlikely (points.in_error ())) वापस false;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      अगर (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      अगर (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        वापस false;
    }
    वापस true;
  }

  bool compile_bytes (स्थिर hb_map_t& axes_सूचकांक_map,
                      स्थिर hb_map_t& axes_old_सूचकांक_tag_map)
  {
    अगर (!compile_shared_tuples (axes_सूचकांक_map, axes_old_सूचकांक_tag_map))
      वापस false;
    क्रम (tuple_variations_t& vars: glyph_variations)
      अगर (!vars.compile_bytes (axes_सूचकांक_map, axes_old_सूचकांक_tag_map,
                               true, /* use shared points*/
                               true,
                               &shared_tuples_idx_map,
			       &pool))
        वापस false;

    वापस true;
  }

  bool compile_shared_tuples (स्थिर hb_map_t& axes_सूचकांक_map,
                              स्थिर hb_map_t& axes_old_सूचकांक_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित> coords_गिनती_map;

    /* count the num of shared coords */
    क्रम (tuple_variations_t& vars: glyph_variations)
    {
      क्रम (tuple_delta_t& var : vars.tuple_vars)
      {
        अगर (!var.compile_coords (axes_सूचकांक_map, axes_old_सूचकांक_tag_map, &pool))
          वापस false;
        अचिन्हित *गिनती;
	अचिन्हित hash = hb_hash (&var.compiled_peak_coords);
        अगर (coords_गिनती_map.has_with_hash (&(var.compiled_peak_coords), hash, &गिनती))
	  (*गिनती)++;
        अन्यथा
          coords_गिनती_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    अगर (!coords_गिनती_map || coords_गिनती_map.in_error ())
      वापस false;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित>> shared_coords {
      + hb_iter (coords_गिनती_map)
      | hb_filter ([] (स्थिर hb_pair_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित>& p) { वापस p.second > 1; })
    };
    अगर (unlikely (shared_coords.in_error ())) वापस false;

    /* no shared tuples: no coords are used more than once */
    अगर (!shared_coords) वापस true;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.क्विक (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_गिनती = hb_min (0xFFFu + 1, shared_coords.लंबाई);
    अचिन्हित len = shared_tuples_गिनती * (shared_coords[0].first->लंबाई);
    अगर (unlikely (!compiled_shared_tuples.alloc (len)))
      वापस false;

    क्रम (अचिन्हित i = 0; i < shared_tuples_गिनती; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      क्रम (स्वतः c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    वापस true;
  }

  अटल पूर्णांक _cmp_coords (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *a = (स्थिर hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pa;
    स्थिर hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *b = (स्थिर hb_pair_t<hb_vector_t<F2DOT14> *, अचिन्हित> *) pb;

    अगर (a->second != b->second)
      वापस b->second - a->second; // high to low

    वापस b->first->as_array().cmp (a->first->as_array ());
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_glyph_var_तथ्य (hb_serialize_context_t *c,
                                 Iterator it,
                                 bool long_offset,
                                 अचिन्हित अंक_glyphs,
                                 अक्षर* glyph_var_तथ्य_offsets /* OUT: glyph var data offsets array */) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अगर (long_offset)
    {
      ((HBUINT32 *) glyph_var_तथ्य_offsets)[0] = 0;
      glyph_var_तथ्य_offsets += 4;
    }
    अन्यथा
    {
      ((HBUINT16 *) glyph_var_तथ्य_offsets)[0] = 0;
      glyph_var_तथ्य_offsets += 2;
    }
    अचिन्हित glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    अचिन्हित idx = 0;

    Glyph_Variation_तथ्य* cur_glyph = c->start_embed<Glyph_Variation_तथ्य> ();
    अगर (!cur_glyph) return_trace (false);
    क्रम (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      अगर (long_offset)
        क्रम (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_तथ्य_offsets)[last_gid] = glyph_offset;
      अन्यथा
        क्रम (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_तथ्य_offsets)[last_gid] = glyph_offset / 2;

      अगर (idx >= glyph_variations.लंबाई) return_trace (false);
      अगर (!cur_glyph->serialize (c, true, glyph_variations[idx])) return_trace (false);
      Glyph_Variation_तथ्य* अग्र_glyph = c->start_embed<Glyph_Variation_तथ्य> ();
      glyph_offset += (अक्षर *) अग्र_glyph - (अक्षर *) cur_glyph;

      अगर (long_offset)
        ((HBUINT32 *) glyph_var_तथ्य_offsets)[gid] = glyph_offset;
      अन्यथा
        ((HBUINT16 *) glyph_var_तथ्य_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = अग्र_glyph;
    }

    अगर (long_offset)
      क्रम (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_तथ्य_offsets)[last_gid] = glyph_offset;
    अन्यथा
      क्रम (; last_gid < अंक_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_तथ्य_offsets)[last_gid] = glyph_offset / 2;
    return_trace (true);
  }
};

ढाँचा <typename T, typename Types>
काष्ठा gvar_GVAR
{
  using Glyph_गिनती_Type = typename Types::HBUINT;

  using Glyph_Variation_तथ्य = Tuple_Variation_तथ्य<Glyph_गिनती_Type>;
  अटल constexpr अचिन्हित glyph_गिनती_max ()
  { वापस (1u << (8 * Glyph_गिनती_Type::static_आकार)) - 1; }

  bool has_तथ्य () स्थिर { वापस version.to_int () != 0; }

  bool sanitize_shallow (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, यह, axis_गिनती_2 * shared_Tuple_गिनती) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_अंक_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_अंक_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  { वापस sanitize_shallow (c); }

  bool decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_गिनती_Type>& glyph_vars /* OUT */) स्थिर
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_तथ्य_map;
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    अगर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_तथ्य_map.set (0, hb_bytes_t ());
      it++;
    }

    क्रम (स्वतः &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_तथ्य_bytes = get_glyph_var_तथ्य_bytes (c->source_blob, glyph_गिनती_X, old_gid);
      new_gid_var_तथ्य_map.set (new_gid, var_तथ्य_bytes);
    }

    अगर (new_gid_var_तथ्य_map.in_error ()) वापस false;

    hb_array_t<स्थिर F2DOT14> shared_tuples = (यह+sharedTuples).as_array ((अचिन्हित) shared_Tuple_गिनती * (अचिन्हित) axis_गिनती_2);
    वापस glyph_vars.create_from_glyphs_var_तथ्य (axis_गिनती_2, shared_tuples, c->plan, new_gid_var_तथ्य_map);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
                  स्थिर glyph_variations_t<Glyph_गिनती_Type>& glyph_vars,
                  Iterator it,
                  अचिन्हित axis_गिनती,
                  अचिन्हित अंक_glyphs,
                  bool force_long_offsets) स्थिर
  {
    TRACE_SERIALIZE (यह);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    अगर (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गिनती_2 = axis_गिनती;
    out->glyph_गिनती_X = hb_min (glyph_गिनती_max (), अंक_glyphs);

    अचिन्हित glyph_var_तथ्य_आकार = glyph_vars.compiled_byte_आकार ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    bool long_offset = glyph_var_तथ्य_आकार > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    अचिन्हित glyph_var_तथ्य_offsets_आकार = (long_offset ? 4 : 2) * (अंक_glyphs + 1);
    HBUINT8 *glyph_var_तथ्य_offsets = c->allocate_आकार<HBUINT8> (glyph_var_तथ्य_offsets_आकार, false);
    अगर (!glyph_var_तथ्य_offsets) return_trace (false);

    /* shared tuples */
    अचिन्हित shared_tuple_गिनती = glyph_vars.compiled_shared_tuples_गिनती ();
    out->shared_Tuple_गिनती = shared_tuple_गिनती;

    अगर (!shared_tuple_गिनती)
      out->sharedTuples = (अक्षर *) glyph_var_तथ्य_offsets + glyph_var_तथ्य_offsets_आकार - (अक्षर *) out;
    अन्यथा
    {
      hb_array_t<स्थिर F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      अगर (!shared_tuples.arrayZ) return_trace (false);
      out->sharedTuples = (स्थिर अक्षर *) shared_tuples.arrayZ - (अक्षर *) out;
    }

    अक्षर *glyph_var_तथ्य = c->start_embed<अक्षर> ();
    अगर (!glyph_var_तथ्य) return_trace (false);
    out->तथ्य_Z = glyph_var_तथ्य - (अक्षर *) out;

    return_trace (glyph_vars.serialize_glyph_var_तथ्य (c, it, long_offset, अंक_glyphs,
                                                       (अक्षर *) glyph_var_तथ्य_offsets));
  }

  bool instantiate (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    glyph_variations_t<Glyph_गिनती_Type> glyph_vars;
    अगर (!decompile_glyph_variations (c, glyph_vars))
      return_trace (false);

    अगर (!glyph_vars.instantiate (c->plan)) return_trace (false);
    अगर (!glyph_vars.compile_bytes (c->plan->axes_सूचकांक_map, c->plan->axes_old_सूचकांक_tag_map))
      return_trace (false);

    अचिन्हित axis_गिनती = c->plan->axes_सूचकांक_map.get_population ();
    अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);

    bool force_long_offsets = false;
#अगर_घोषित HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#पूर्ण_अगर
    return_trace (serialize (c->serializer, glyph_vars, it, axis_गिनती, अंक_glyphs, force_long_offsets));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned)
      return_trace (false);

    अगर (c->plan->normalized_coords)
      return_trace (instantiate (c));

    अचिन्हित glyph_गिनती = version.to_int () ? c->plan->source->get_अंक_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    अगर (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_गिनती_2 = axis_गिनती_2;
    out->shared_Tuple_गिनती = shared_Tuple_गिनती;

    अचिन्हित पूर्णांक अंक_glyphs = c->plan->अंक_output_glyphs ();
    out->glyph_गिनती_X = hb_min (glyph_गिनती_max (), अंक_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<अक्षर>> culled_vars;
    hb_set_t culled_gids;
    अगर (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<स्थिर F2DOT14> shared_tuples_array =
	  (यह+sharedTuples).as_array ((अचिन्हित) shared_Tuple_गिनती * (अचिन्हित) axis_गिनती_2);
      अगर (unlikely (!culled_vars.resize (अंक_glyphs))) return_trace (false);
      स्वतः cull_it = hb_iter (c->plan->new_to_old_gid_list);
      अगर (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      क्रम (स्वतः &_ : cull_it)
      {
	hb_bytes_t var_तथ्य_bytes = get_glyph_var_तथ्य_bytes (c->source_blob, glyph_गिनती, _.second);
	अगर (var_तथ्य_bytes.लंबाई < Glyph_Variation_तथ्य::min_आकार) जारी;
	bool changed = false;
	अगर (unlikely (!var_तथ्य_bytes.as<Glyph_Variation_तथ्य> ()
		      ->cull_tuple_variations (var_तथ्य_bytes, axis_गिनती_2,
					       shared_tuples_array,
					       &c->plan->axes_old_सूचकांक_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (false);
	अगर (changed) culled_gids.add (_.first);
      }
      अगर (unlikely (culled_gids.in_error ())) return_trace (false);
    }

    स्वतः it = hb_iter (c->plan->new_to_old_gid_list);
    अगर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    अचिन्हित subset_तथ्य_आकार = 0;
    अचिन्हित padding_आकार = 0;
    क्रम (स्वतः &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      अचिन्हित glyph_तथ्य_आकार = culled_gids.has (_.first)
				 ? culled_vars[_.first].लंबाई
				 : get_glyph_var_तथ्य_bytes (c->source_blob, glyph_गिनती, old_gid).लंबाई;
      अगर (glyph_तथ्य_आकार % 2)
      {
        glyph_तथ्य_आकार++;
        padding_आकार++;
      }

      अगर (unlikely (hb_unsigned_add_overflows (subset_तथ्य_आकार, glyph_तथ्य_आकार, &subset_तथ्य_आकार)))
	return_trace (false);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    bool long_offset = subset_तथ्य_आकार > 0x1FFFEu;
#अगर_घोषित HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#पूर्ण_अगर
    out->flags = long_offset ? 1 : 0;

    अचिन्हित subset_offsets_आकार = (long_offset ? 4 : 2) * (अंक_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_आकार<HBUINT8> (subset_offsets_आकार, false);
    अगर (!subset_offsets) return_trace (false);

    /* shared tuples */
    अगर (!shared_Tuple_गिनती || !sharedTuples)
      out->sharedTuples = (अक्षर *) subset_offsets + subset_offsets_आकार - (अक्षर *) out;
    अन्यथा
    {
      अचिन्हित पूर्णांक shared_tuple_आकार = F2DOT14::static_आकार * axis_गिनती_2 * shared_Tuple_गिनती;
      F2DOT14 *tuples = c->serializer->allocate_आकार<F2DOT14> (shared_tuple_आकार);
      अगर (!tuples) return_trace (false);
      out->sharedTuples = (अक्षर *) tuples - (अक्षर *) out;
      hb_memcpy (tuples, यह+sharedTuples, shared_tuple_आकार);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    अगर (long_offset)
      subset_तथ्य_आकार -= padding_आकार;
    अक्षर *subset_तथ्य = c->serializer->allocate_आकार<अक्षर> (subset_तथ्य_आकार, false);
    अगर (!subset_तथ्य) return_trace (false);
    out->तथ्य_Z = subset_तथ्य - (अक्षर *) out;


    अगर (long_offset)
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
    अगर (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    क्रम (स्वतः &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      अगर (long_offset)
	क्रम (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      अन्यथा
	क्रम (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_तथ्य_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].लंबाई)
				  : get_glyph_var_तथ्य_bytes (c->source_blob,
							      glyph_गिनती,
							      old_gid);

      hb_memcpy (subset_तथ्य, var_तथ्य_bytes.arrayZ, var_तथ्य_bytes.लंबाई);
      अचिन्हित glyph_तथ्य_आकार = var_तथ्य_bytes.लंबाई;
      subset_तथ्य += glyph_तथ्य_आकार;
      glyph_offset += glyph_तथ्य_आकार;

      अगर (!long_offset && (glyph_तथ्य_आकार % 2))
      {
        *subset_तथ्य = 0;
        subset_तथ्य++;
        glyph_offset++;
      }

      अगर (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      अन्यथा
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    अगर (long_offset)
      क्रम (; last < अंक_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    अन्यथा
      क्रम (; last < अंक_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (true);
  }

  रक्षित:
  स्थिर hb_bytes_t get_glyph_var_तथ्य_bytes (hb_blob_t *blob,
					     अचिन्हित glyph_गिनती,
					     hb_codepoint_t glyph) स्थिर
  {
    अचिन्हित start_offset = get_offset (glyph_गिनती, glyph);
    अचिन्हित end_offset = get_offset (glyph_गिनती, glyph+1);
    अगर (unlikely (end_offset < start_offset)) वापस hb_bytes_t ();
    अचिन्हित लंबाई = end_offset - start_offset;
    hb_bytes_t var_तथ्य = blob->as_bytes ().sub_array (((अचिन्हित) तथ्य_Z) + start_offset, लंबाई);
    वापस likely (var_तथ्य.लंबाई >= Glyph_Variation_तथ्य::min_आकार) ? var_तथ्य : hb_bytes_t ();
  }

  bool is_long_offset () स्थिर { वापस flags & 1; }

  अचिन्हित get_offset (अचिन्हित glyph_गिनती, अचिन्हित i) स्थिर
  {
    अगर (unlikely (i > glyph_गिनती)) वापस 0;
    hb_barrier ();
    वापस is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  स्थिर HBUINT32 * get_long_offset_array () स्थिर { वापस (स्थिर HBUINT32 *) &offsetZ; }
  स्थिर HBUINT16 *get_short_offset_array () स्थिर { वापस (स्थिर HBUINT16 *) &offsetZ; }

  खुला:
  काष्ठा accelerator_t
  {

    hb_scalar_cache_t *create_cache () स्थिर
    {
      वापस hb_scalar_cache_t::create (hb_min ((अचिन्हित) table->shared_Tuple_गिनती,
						+TupleVariationHeader::max_shared_tuple_गिनती));
    }

    अटल व्योम destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    bool has_तथ्य () स्थिर { वापस table->has_तथ्य (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_गिनती_2 = table->version.to_int () ? face->get_अंक_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    निजी:

    अटल भग्न infer_delta (स्थिर hb_array_t<contour_point_t> points,
			      स्थिर hb_array_t<contour_point_t> deltas,
			      अचिन्हित पूर्णांक target, अचिन्हित पूर्णांक prev, अचिन्हित पूर्णांक next,
			      भग्न contour_point_t::*m)
    {
      भग्न target_val = points.arrayZ[target].*m;
      भग्न पूर्व_val = points.arrayZ[prev].*m;
      भग्न अग्र_val = points.arrayZ[next].*m;
      भग्न पूर्व_delta =  deltas.arrayZ[prev].*m;
      भग्न अग्र_delta =  deltas.arrayZ[next].*m;

      अगर (पूर्व_val == अग्र_val)
	वापस (पूर्व_delta == अग्र_delta) ? पूर्व_delta : 0.f;
      अन्यथा अगर (target_val <= hb_min (पूर्व_val, अग्र_val))
	वापस (पूर्व_val < अग्र_val) ? पूर्व_delta : अग्र_delta;
      अन्यथा अगर (target_val >= hb_max (पूर्व_val, अग्र_val))
	वापस (पूर्व_val > अग्र_val) ? पूर्व_delta : अग्र_delta;

      /* linear interpolation */
      भग्न r = (target_val - पूर्व_val) / (अग्र_val - पूर्व_val);
      वापस पूर्व_delta + r * (अग्र_delta - पूर्व_delta);
    }

    अटल अचिन्हित पूर्णांक अग्र_सूचकांक (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
    { वापस (i >= end) ? start : (i + 1); }

#अगर_अघोषित HB_OPTIMIZE_आकार
    ढाँचा <bool is_x>
#पूर्ण_अगर
    अटल bool decompile_deltas_add_to_points (स्थिर HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						भग्न scalar,
						स्थिर HBUINT8 *end,
						अचिन्हित start
#अगर_घोषित HB_OPTIMIZE_आकार
						, bool is_x
#पूर्ण_अगर
						)
    {
      अचिन्हित i = 0;
      अचिन्हित गिनती = points.लंबाई;
      जबतक (i < गिनती)
      {
	अगर (unlikely (p + 1 > end)) वापस false;
	अचिन्हित control = *p++;
	अचिन्हित run_गिनती = (control & Tuple_मूल्यावली::मूल्य_RUN_गिनती_MASK) + 1;
	अचिन्हित stop = i + run_गिनती;
	अगर (unlikely (stop > गिनती)) वापस false;

	अचिन्हित skip = i < start ? hb_min (start - i, run_गिनती) : 0;
	i += skip;

	चयन (control & Tuple_मूल्यावली::मूल्यावली_आकार_MASK)
	{
	  हाल Tuple_मूल्यावली::मूल्यावली_ARE_ZEROS:
	    i = stop;
	    अवरोध;
	  हाल Tuple_मूल्यावली::मूल्यावली_ARE_शब्दगण:
	  {
	    अगर (unlikely (p + run_गिनती * HBINT16::static_आकार > end)) वापस false;
	    p += skip * HBINT16::static_आकार;
	    स्थिर स्वतः *pp = (स्थिर HBINT16 *) p;
	    क्रम (; i < stop; i++)
	    {
	      भग्न v = *pp++ * scalar;
	      अगर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिर HBUINT8 *) pp;
	  }
	  अवरोध;
	  हाल Tuple_मूल्यावली::मूल्यावली_ARE_LONGS:
	  {
	    अगर (unlikely (p + run_गिनती * HBINT32::static_आकार > end)) वापस false;
	    p += skip * HBINT32::static_आकार;
	    स्थिर स्वतः *pp = (स्थिर HBINT32 *) p;
	    क्रम (; i < stop; i++)
	    {
	      भग्न v = *pp++ * scalar;
	      अगर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिर HBUINT8 *) pp;
	  }
	  अवरोध;
	  हाल Tuple_मूल्यावली::मूल्यावली_ARE_BYTES:
	  {
	    अगर (unlikely (p + run_गिनती > end)) वापस false;
	    p += skip * HBINT8::static_आकार;
	    स्थिर स्वतः *pp = (स्थिर HBINT8 *) p;
	    क्रम (; i < stop; i++)
	    {
	      भग्न v = *pp++ * scalar;
	      अगर (is_x) points.arrayZ[i].x += v;
	      अन्यथा points.arrayZ[i].y += v;
	    }
	    p = (स्थिर HBUINT8 *) pp;
	  }
	  अवरोध;
	}
      }
      वापस true;
    }
    खुला:
    bool apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<स्थिर पूर्णांक> coords,
				 स्थिर hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 bool phantom_only = false,
				 int64_t *budget = nullptr) स्थिर
    {
      अगर (unlikely (glyph >= glyph_गिनती_2)) वापस true;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_तथ्य_bytes = table->get_glyph_var_तथ्य_bytes (table.get_blob (), glyph_गिनती_2, glyph);
      अगर (!var_तथ्य_bytes.as<Glyph_Variation_तथ्य> ()->has_तथ्य ()) वापस true;

      स्वतः &shared_indices = scratch.shared_indices;
      shared_indices.सफा ();

      typename Glyph_Variation_तथ्य::tuple_iterator_t iterator;
      अगर (!Glyph_Variation_तथ्य::get_tuple_iterator (var_तथ्य_bytes, table->axis_गिनती_2,
						   var_तथ्य_bytes.arrayZ,
						   shared_indices, &iterator))
	वापस true; /* so isn't applied at all */

      bool any_private_points = false;
      bool private_points_checked = false;

      /* Save original points for inferred delta calculation */
      स्वतः &orig_points_vec = scratch.orig_points;
      orig_points_vec.सफा (); // Populated lazily
      स्वतः orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      स्वतः &deltas_vec = scratch.deltas;
      deltas_vec.सफा (); // Populated lazily
      स्वतः deltas = deltas_vec.as_array ();

      अचिन्हित अंक_coords = table->axis_गिनती_2;
      hb_array_t<स्थिर F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_गिनती * अंक_coords);

      स्वतः &private_indices = scratch.private_indices;
      स्वतः &x_deltas = scratch.x_deltas;
      स्वतः &y_deltas = scratch.y_deltas;

      अचिन्हित गिनती = points.लंबाई;
      bool सफाई = false;

      करो
      {
	भग्न scalar = iterator.current_tuple->calculate_scalar (coords, अंक_coords, shared_tuples,
								 scalar_cache);

	अगर (scalar == 0.f) जारी;

	अगर (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && गिनती >= 4 ? 4 : गिनती)))
	  वापस false;

	अगर (!private_points_checked)
	{
	  स्वतः scan = iterator;
	  करो
	  {
	    अगर (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = true;
	      अवरोध;
	    }
	  } जबतक (scan.move_to_अग्र ());
	  private_points_checked = true;
	}
	स्थिर HBUINT8 *p = iterator.get_serialized_तथ्य ();
	अचिन्हित पूर्णांक लंबाई = iterator.current_tuple->get_तथ्य_आकार ();
	अगर (unlikely (!iterator.var_तथ्य_bytes.check_range (p, लंबाई)))
	  वापस false;

	अगर (!deltas)
	{
	  अगर (unlikely (!deltas_vec.resize_dirty  (गिनती))) वापस false;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? गिनती - 4 : 0), 0,
		     (phantom_only ? 4 : गिनती) * माप (deltas[0]));
	}

	स्थिर HBUINT8 *end = p + लंबाई;

	bool has_private_points = iterator.current_tuple->has_private_points ();
	अगर (has_private_points &&
	    !Glyph_Variation_तथ्य::decompile_points (p, private_indices, end))
	  वापस false;
	स्थिर hb_array_t<अचिन्हित पूर्णांक> &indices = has_private_points ? private_indices : shared_indices;

	bool apply_to_all = (indices.लंबाई == 0);
	अचिन्हित अंक_deltas = apply_to_all ? points.लंबाई : indices.लंबाई;
	अचिन्हित start_deltas = (apply_to_all && phantom_only && अंक_deltas >= 4 ? अंक_deltas - 4 : 0);

	अगर (apply_to_all && !any_private_points)
	{
#अगर_घोषित HB_OPTIMIZE_आकार
	  अगर (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, true))) वापस false;
	  अगर (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, false))) वापस false;
#अन्यथा
	  अगर (unlikely (!decompile_deltas_add_to_points<true> (p, points, scalar, end, start_deltas))) वापस false;
	  अगर (unlikely (!decompile_deltas_add_to_points<false> (p, points, scalar, end, start_deltas))) वापस false;
#पूर्ण_अगर
	  जारी;
	}

	अगर (unlikely (!x_deltas.resize_dirty  (अंक_deltas))) वापस false;
	अगर (unlikely (!Glyph_Variation_तथ्य::decompile_deltas (p, x_deltas, end, false, start_deltas))) वापस false;
	अगर (unlikely (!y_deltas.resize_dirty  (अंक_deltas))) वापस false;
	अगर (unlikely (!Glyph_Variation_तथ्य::decompile_deltas (p, y_deltas, end, false, start_deltas))) वापस false;

	अगर (!apply_to_all)
	{
	  अगर (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    अगर (unlikely (orig_points_vec.in_error ())) वापस false;
	    orig_points = orig_points_vec.as_array ();
	  }

	  अगर (सफाई)
	  {
	    क्रम (अचिन्हित पूर्णांक i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? गिनती - 4 : 0), 0,
		     (phantom_only ? 4 : गिनती) * माप (deltas[0]));
	}

	अगर (HB_OPTIMIZE_आकार_VAL)
	{
	  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	  {
	    अचिन्हित पूर्णांक pt_सूचकांक;
	    अगर (apply_to_all)
	      pt_सूचकांक = i;
	    अन्यथा
	    {
	      pt_सूचकांक = indices[i];
	      अगर (unlikely (pt_सूचकांक >= deltas.लंबाई)) जारी;
	    }
	    अगर (phantom_only && pt_सूचकांक < गिनती - 4) जारी;
	    स्वतः &delta = deltas.arrayZ[pt_सूचकांक];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	अन्यथा
	{
	  /* Ouch. Four cases... for optimization. */
	  अगर (scalar != 1.0f)
	  {
	    अगर (apply_to_all)
	      क्रम (अचिन्हित पूर्णांक i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    अन्यथा
	      क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णांक pt_सूचकांक = indices[i];
		अगर (unlikely (pt_सूचकांक >= deltas.लंबाई)) जारी;
		अगर (phantom_only && pt_सूचकांक < गिनती - 4) जारी;
		स्वतः &delta = deltas.arrayZ[pt_सूचकांक];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  अन्यथा
	  {
	    अगर (apply_to_all)
	      क्रम (अचिन्हित पूर्णांक i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	      {
		स्वतः &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    अन्यथा
	      क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
	      {
		अचिन्हित पूर्णांक pt_सूचकांक = indices[i];
		अगर (unlikely (pt_सूचकांक >= deltas.लंबाई)) जारी;
		अगर (phantom_only && pt_सूचकांक < गिनती - 4) जारी;
		स्वतः &delta = deltas.arrayZ[pt_सूचकांक];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	अगर (!apply_to_all && !phantom_only)
	{
	  अचिन्हित start_point = 0;
	  अचिन्हित end_point = 0;
	  जबतक (true)
	  {
	    जबतक (end_point < गिनती && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    अगर (unlikely (end_point == गिनती)) अवरोध;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    अचिन्हित unref_गिनती = 0;
	    क्रम (अचिन्हित i = start_point; i < end_point + 1; i++)
	      unref_गिनती += deltas.arrayZ[i].flag;
	    unref_गिनती = (end_point - start_point + 1) - unref_गिनती;

	    अचिन्हित j = start_point;
	    अगर (unref_गिनती == 0 || unref_गिनती > end_point - start_point)
	      जाओ no_more_gaps;

	    क्रम (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      अचिन्हित पूर्णांक prev, next, i;
	      क्रम (;;)
	      {
		i = j;
		j = अग्र_सूचकांक (i, start_point, end_point);
		अगर (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) अवरोध;
	      }
	      prev = j = i;
	      क्रम (;;)
	      {
		i = j;
		j = अग्र_सूचकांक (i, start_point, end_point);
		अगर (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) अवरोध;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      क्रम (;;)
	      {
		i = अग्र_सूचकांक (i, start_point, end_point);
		अगर (i == next) अवरोध;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		अगर (--unref_गिनती == 0) जाओ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	सफाई = true;

      } जबतक (iterator.move_to_अग्र ());

      अगर (सफाई)
      {
	क्रम (अचिन्हित पूर्णांक i = phantom_only ? गिनती - 4 : 0; i < गिनती; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      वापस true;
    }

    अचिन्हित पूर्णांक get_axis_गिनती () स्थिर { वापस table->axis_गिनती_2; }

    निजी:
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
  Offset32To<Glyph_Variation_तथ्य>
		तथ्य_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  UnsizedArrayOf<HBUINT8>
		offsetZ;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  खुला:
  DEFINE_आकार_ARRAY (18 + Glyph_गिनती_Type::static_आकार, offsetZ);
};

काष्ठा gvar : gvar_GVAR<gvar, SmallTypes>
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

काष्ठा GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

काष्ठा gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () स्थिर
  {
    अगर (has_तथ्य ()) वापस HB_DUAL_GET (*यह, create_cache ());
    वापस nullptr;
  }

  अटल व्योम destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  bool apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<स्थिर पूर्णांक> coords,
			       स्थिर hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       bool phantom_only = false,
			       int64_t *budget = nullptr) स्थिर
  {
    वापस HB_DUAL_GET (*यह, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  अचिन्हित पूर्णांक get_axis_गिनती () स्थिर
  {
    अगर (has_तथ्य ()) वापस HB_DUAL_GET (*यह, get_axis_गिनती ());
    वापस 0;
  }
};

} /* namespace OT */

#पूर्ण_अगर /* HB_OT_VAR_GVAR_TABLE_HH */
