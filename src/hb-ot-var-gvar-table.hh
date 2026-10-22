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

#અન્તર્ભાવ "hb-decycler.hh"
#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"
#અન્તર્ભાવ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

સંરચના hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<પૂર્ણાંક> x_deltas;
  hb_vector_t<પૂર્ણાંક> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned પૂર્ણાંક> shared_indices;
  hb_vector_t<unsigned પૂર્ણાંક> private_indices;
};

નામાકાશ OT {

template <typename OffsetType>
સંરચના glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  ખાનગી:
  unsigned shared_tuples_ગણતરી = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<અચલ hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  જાહેર:
  unsigned compiled_shared_tuples_ગણતરી () અચલ
  { પાછા shared_tuples_ગણતરી; }

  unsigned compiled_byte_size () અચલ
  {
    unsigned byte_size = 0;
    માટે (અચલ auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    પાછા byte_size;
  }

  તાર્કિક create_from_glyphs_var_data (unsigned axis_ગણતરી,
                                    અચલ hb_array_t<અચલ F2DOT14> shared_tuples,
                                    અચલ hb_subset_plan_t *plan,
                                    અચલ hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    જો (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      પાછા ખોટું;

    auto it = hb_iter (plan->new_to_old_gid_list);
    માટે (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      જો (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        પાછા ખોટું;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      અચલ GlyphVariationData* p = reinterpret_cast<અચલ GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      જો (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_ગણતરી,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        ચાલુ;
      }

      તાર્કિક is_composite_glyph = ખોટું;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      જો (!p->decompile_tuple_variations (all_contour_points->length, સાચું /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        પાછા ખોટું;
      glyph_variations.push (std::move (tuple_vars));
    }
    પાછા !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  તાર્કિક instantiate (અચલ hb_subset_plan_t *plan)
  {
    unsigned ગણતરી = plan->new_to_old_gid_list.length;
    તાર્કિક iup_optimize = ખોટું;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      જો (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        પાછા ખોટું;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      જો (iup_optimize)
      {
        points = *all_points;
        જો (unlikely (points.in_error ())) પાછા ખોટું;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      જો (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      જો (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        પાછા ખોટું;
    }
    પાછા સાચું;
  }

  તાર્કિક compile_bytes (અચલ hb_map_t& axes_index_map,
                      અચલ hb_map_t& axes_old_index_tag_map)
  {
    જો (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      પાછા ખોટું;
    માટે (tuple_variations_t& vars: glyph_variations)
      જો (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               સાચું, /* use shared points*/
                               સાચું,
                               &shared_tuples_idx_map,
			       &pool))
        પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક compile_shared_tuples (અચલ hb_map_t& axes_index_map,
                              અચલ hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<અચલ hb_vector_t<F2DOT14>*, unsigned> coords_ગણતરી_map;

    /* count the num of shared coords */
    માટે (tuple_variations_t& vars: glyph_variations)
    {
      માટે (tuple_delta_t& var : vars.tuple_vars)
      {
        જો (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          પાછા ખોટું;
        unsigned *ગણતરી;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        જો (coords_ગણતરી_map.has_with_hash (&(var.compiled_peak_coords), hash, &ગણતરી))
	  (*ગણતરી)++;
        નહીંતર
          coords_ગણતરી_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    જો (!coords_ગણતરી_map || coords_ગણતરી_map.in_error ())
      પાછા ખોટું;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<અચલ hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_ગણતરી_map)
      | hb_filter ([] (અચલ hb_pair_t<અચલ hb_vector_t<F2DOT14>*, unsigned>& p) { પાછા p.second > 1; })
    };
    જો (unlikely (shared_coords.in_error ())) પાછા ખોટું;

    /* no shared tuples: no coords are used more than once */
    જો (!shared_coords) પાછા સાચું;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_ગણતરી = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_ગણતરી * (shared_coords[0].first->length);
    જો (unlikely (!compiled_shared_tuples.alloc (len)))
      પાછા ખોટું;

    માટે (unsigned i = 0; i < shared_tuples_ગણતરી; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      માટે (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    પાછા સાચું;
  }

  સ્થિર પૂર્ણાંક _cmp_coords (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (અચલ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    અચલ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (અચલ hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    જો (a->second != b->second)
      પાછા b->second - a->second; // high to low

    પાછા b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 તાર્કિક long_offset,
                                 unsigned અંક_glyphs,
                                 અક્ષર* glyph_var_data_offsets /* OUT: glyph var data offsets array */) અચલ
  {
    TRACE_SERIALIZE (આ);

    જો (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    નહીંતર
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    જો (!cur_glyph) return_trace (ખોટું);
    માટે (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      જો (long_offset)
        માટે (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      નહીંતર
        માટે (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      જો (idx >= glyph_variations.length) return_trace (ખોટું);
      જો (!cur_glyph->serialize (c, સાચું, glyph_variations[idx])) return_trace (ખોટું);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (અક્ષર *) next_glyph - (અક્ષર *) cur_glyph;

      જો (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      નહીંતર
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    જો (long_offset)
      માટે (; last_gid < અંક_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    નહીંતર
      માટે (; last_gid < અંક_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (સાચું);
  }
};

template <typename T, typename Types>
સંરચના gvar_GVAR
{
  using Glyph_ગણતરી_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_ગણતરી_Type>;
  સ્થિર constexpr unsigned glyph_ગણતરી_max ()
  { પાછા (1u << (8 * Glyph_ગણતરી_Type::static_size)) - 1; }

  તાર્કિક has_data () અચલ { પાછા version.to_int () != 0; }

  તાર્કિક sanitize_shallow (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, આ, axis_ગણતરી_2 * shared_Tuple_ગણતરી) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_અંક_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_અંક_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  { પાછા sanitize_shallow (c); }

  તાર્કિક decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_ગણતરી_Type>& glyph_vars /* OUT */) અચલ
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    જો (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    માટે (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ગણતરી_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    જો (new_gid_var_data_map.in_error ()) પાછા ખોટું;

    hb_array_t<અચલ F2DOT14> shared_tuples = (આ+sharedTuples).as_array ((unsigned) shared_Tuple_ગણતરી * (unsigned) axis_ગણતરી_2);
    પાછા glyph_vars.create_from_glyphs_var_data (axis_ગણતરી_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  અચલ glyph_variations_t<Glyph_ગણતરી_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_ગણતરી,
                  unsigned અંક_glyphs,
                  તાર્કિક force_long_offsets) અચલ
  {
    TRACE_SERIALIZE (આ);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    જો (unlikely (!out)) return_trace (ખોટું);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ગણતરી_2 = axis_ગણતરી;
    out->glyph_ગણતરી_X = hb_min (glyph_ગણતરી_max (), અંક_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    તાર્કિક long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (અંક_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, ખોટું);
    જો (!glyph_var_data_offsets) return_trace (ખોટું);

    /* shared tuples */
    unsigned shared_tuple_ગણતરી = glyph_vars.compiled_shared_tuples_ગણતરી ();
    out->shared_Tuple_ગણતરી = shared_tuple_ગણતરી;

    જો (!shared_tuple_ગણતરી)
      out->sharedTuples = (અક્ષર *) glyph_var_data_offsets + glyph_var_data_offsets_size - (અક્ષર *) out;
    નહીંતર
    {
      hb_array_t<અચલ F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      જો (!shared_tuples.arrayZ) return_trace (ખોટું);
      out->sharedTuples = (અચલ અક્ષર *) shared_tuples.arrayZ - (અક્ષર *) out;
    }

    અક્ષર *glyph_var_data = c->start_embed<અક્ષર> ();
    જો (!glyph_var_data) return_trace (ખોટું);
    out->dataZ = glyph_var_data - (અક્ષર *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, અંક_glyphs,
                                                       (અક્ષર *) glyph_var_data_offsets));
  }

  તાર્કિક instantiate (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    glyph_variations_t<Glyph_ગણતરી_Type> glyph_vars;
    જો (!decompile_glyph_variations (c, glyph_vars))
      return_trace (ખોટું);

    જો (!glyph_vars.instantiate (c->plan)) return_trace (ખોટું);
    જો (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (ખોટું);

    unsigned axis_ગણતરી = c->plan->axes_index_map.get_population ();
    unsigned અંક_glyphs = c->plan->અંક_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    તાર્કિક force_long_offsets = ખોટું;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_ગણતરી, અંક_glyphs, force_long_offsets));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned)
      return_trace (ખોટું);

    જો (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_ગણતરી = version.to_int () ? c->plan->source->get_અંક_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    જો (unlikely (!out)) return_trace (ખોટું);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_ગણતરી_2 = axis_ગણતરી_2;
    out->shared_Tuple_ગણતરી = shared_Tuple_ગણતરી;

    unsigned પૂર્ણાંક અંક_glyphs = c->plan->અંક_output_glyphs ();
    out->glyph_ગણતરી_X = hb_min (glyph_ગણતરી_max (), અંક_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<અક્ષર>> culled_vars;
    hb_set_t culled_gids;
    જો (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<અચલ F2DOT14> shared_tuples_array =
	  (આ+sharedTuples).as_array ((unsigned) shared_Tuple_ગણતરી * (unsigned) axis_ગણતરી_2);
      જો (unlikely (!culled_vars.resize (અંક_glyphs))) return_trace (ખોટું);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      જો (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      માટે (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_ગણતરી, _.second);
	જો (var_data_bytes.length < GlyphVariationData::min_size) ચાલુ;
	તાર્કિક changed = ખોટું;
	જો (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_ગણતરી_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (ખોટું);
	જો (changed) culled_gids.add (_.first);
      }
      જો (unlikely (culled_gids.in_error ())) return_trace (ખોટું);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    જો (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    માટે (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_ગણતરી, old_gid).length;
      જો (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      જો (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (ખોટું);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    તાર્કિક long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (અંક_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, ખોટું);
    જો (!subset_offsets) return_trace (ખોટું);

    /* shared tuples */
    જો (!shared_Tuple_ગણતરી || !sharedTuples)
      out->sharedTuples = (અક્ષર *) subset_offsets + subset_offsets_size - (અક્ષર *) out;
    નહીંતર
    {
      unsigned પૂર્ણાંક shared_tuple_size = F2DOT14::static_size * axis_ગણતરી_2 * shared_Tuple_ગણતરી;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      જો (!tuples) return_trace (ખોટું);
      out->sharedTuples = (અક્ષર *) tuples - (અક્ષર *) out;
      hb_memcpy (tuples, આ+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    જો (long_offset)
      subset_data_size -= padding_size;
    અક્ષર *subset_data = c->serializer->allocate_size<અક્ષર> (subset_data_size, ખોટું);
    જો (!subset_data) return_trace (ખોટું);
    out->dataZ = subset_data - (અક્ષર *) out;


    જો (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    નહીંતર
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned પૂર્ણાંક glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    જો (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    માટે (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      જો (long_offset)
	માટે (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      નહીંતર
	માટે (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_ગણતરી,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      જો (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      જો (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      નહીંતર
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    જો (long_offset)
      માટે (; last < અંક_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    નહીંતર
      માટે (; last < અંક_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (સાચું);
  }

  સુરક્ષિત:
  અચલ hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_ગણતરી,
					     hb_codepoint_t glyph) અચલ
  {
    unsigned start_offset = get_offset (glyph_ગણતરી, glyph);
    unsigned end_offset = get_offset (glyph_ગણતરી, glyph+1);
    જો (unlikely (end_offset < start_offset)) પાછા hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    પાછા likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  તાર્કિક is_long_offset () અચલ { પાછા flags & 1; }

  unsigned get_offset (unsigned glyph_ગણતરી, unsigned i) અચલ
  {
    જો (unlikely (i > glyph_ગણતરી)) પાછા 0;
    hb_barrier ();
    પાછા is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  અચલ HBUINT32 * get_long_offset_array () અચલ { પાછા (અચલ HBUINT32 *) &offsetZ; }
  અચલ HBUINT16 *get_short_offset_array () અચલ { પાછા (અચલ HBUINT16 *) &offsetZ; }

  જાહેર:
  સંરચના accelerator_t
  {

    hb_scalar_cache_t *create_cache () અચલ
    {
      પાછા hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_ગણતરી,
						+TupleVariationHeader::max_shared_tuple_ગણતરી));
    }

    સ્થિર શૂન્ય destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    તાર્કિક has_data () અચલ { પાછા table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_ગણતરી_2 = table->version.to_int () ? face->get_અંક_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    ખાનગી:

    સ્થિર દશાંશ infer_delta (અચલ hb_array_t<contour_point_t> points,
			      અચલ hb_array_t<contour_point_t> deltas,
			      unsigned પૂર્ણાંક target, unsigned પૂર્ણાંક prev, unsigned પૂર્ણાંક next,
			      દશાંશ contour_point_t::*m)
    {
      દશાંશ target_val = points.arrayZ[target].*m;
      દશાંશ prev_val = points.arrayZ[prev].*m;
      દશાંશ next_val = points.arrayZ[next].*m;
      દશાંશ prev_delta =  deltas.arrayZ[prev].*m;
      દશાંશ next_delta =  deltas.arrayZ[next].*m;

      જો (prev_val == next_val)
	પાછા (prev_delta == next_delta) ? prev_delta : 0.f;
      નહીંતર જો (target_val <= hb_min (prev_val, next_val))
	પાછા (prev_val < next_val) ? prev_delta : next_delta;
      નહીંતર જો (target_val >= hb_max (prev_val, next_val))
	પાછા (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      દશાંશ r = (target_val - prev_val) / (next_val - prev_val);
      પાછા prev_delta + r * (next_delta - prev_delta);
    }

    સ્થિર unsigned પૂર્ણાંક next_index (unsigned પૂર્ણાંક i, unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
    { પાછા (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <તાર્કિક is_x>
#endif
    સ્થિર તાર્કિક decompile_deltas_add_to_points (અચલ HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						દશાંશ scalar,
						અચલ HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, તાર્કિક is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned ગણતરી = points.length;
      જ્યારે (i < ગણતરી)
      {
	જો (unlikely (p + 1 > end)) પાછા ખોટું;
	unsigned control = *p++;
	unsigned run_ગણતરી = (control & Tuple_કિંમતો::કિંમત_RUN_ગણતરી_MASK) + 1;
	unsigned stop = i + run_ગણતરી;
	જો (unlikely (stop > ગણતરી)) પાછા ખોટું;

	unsigned skip = i < start ? hb_min (start - i, run_ગણતરી) : 0;
	i += skip;

	પસંદગી (control & Tuple_કિંમતો::કિંમતો_SIZE_MASK)
	{
	  સ્થિતિ Tuple_કિંમતો::કિંમતો_ARE_ZEROS:
	    i = stop;
	    તોડો;
	  સ્થિતિ Tuple_કિંમતો::કિંમતો_ARE_શબ્દો:
	  {
	    જો (unlikely (p + run_ગણતરી * HBINT16::static_size > end)) પાછા ખોટું;
	    p += skip * HBINT16::static_size;
	    અચલ auto *pp = (અચલ HBINT16 *) p;
	    માટે (; i < stop; i++)
	    {
	      દશાંશ v = *pp++ * scalar;
	      જો (is_x) points.arrayZ[i].x += v;
	      નહીંતર points.arrayZ[i].y += v;
	    }
	    p = (અચલ HBUINT8 *) pp;
	  }
	  તોડો;
	  સ્થિતિ Tuple_કિંમતો::કિંમતો_ARE_LONGS:
	  {
	    જો (unlikely (p + run_ગણતરી * HBINT32::static_size > end)) પાછા ખોટું;
	    p += skip * HBINT32::static_size;
	    અચલ auto *pp = (અચલ HBINT32 *) p;
	    માટે (; i < stop; i++)
	    {
	      દશાંશ v = *pp++ * scalar;
	      જો (is_x) points.arrayZ[i].x += v;
	      નહીંતર points.arrayZ[i].y += v;
	    }
	    p = (અચલ HBUINT8 *) pp;
	  }
	  તોડો;
	  સ્થિતિ Tuple_કિંમતો::કિંમતો_ARE_BYTES:
	  {
	    જો (unlikely (p + run_ગણતરી > end)) પાછા ખોટું;
	    p += skip * HBINT8::static_size;
	    અચલ auto *pp = (અચલ HBINT8 *) p;
	    માટે (; i < stop; i++)
	    {
	      દશાંશ v = *pp++ * scalar;
	      જો (is_x) points.arrayZ[i].x += v;
	      નહીંતર points.arrayZ[i].y += v;
	    }
	    p = (અચલ HBUINT8 *) pp;
	  }
	  તોડો;
	}
      }
      પાછા સાચું;
    }
    જાહેર:
    તાર્કિક apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<અચલ પૂર્ણાંક> coords,
				 અચલ hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 તાર્કિક phantom_only = ખોટું,
				 int64_t *budget = nullptr) અચલ
    {
      જો (unlikely (glyph >= glyph_ગણતરી_2)) પાછા સાચું;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_ગણતરી_2, glyph);
      જો (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) પાછા સાચું;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      જો (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_ગણતરી_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	પાછા સાચું; /* so isn't applied at all */

      તાર્કિક any_private_points = ખોટું;
      તાર્કિક private_points_checked = ખોટું;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned અંક_coords = table->axis_ગણતરી_2;
      hb_array_t<અચલ F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_ગણતરી * અંક_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned ગણતરી = points.length;
      તાર્કિક flush = ખોટું;

      કરો
      {
	દશાંશ scalar = iterator.current_tuple->calculate_scalar (coords, અંક_coords, shared_tuples,
								 scalar_cache);

	જો (scalar == 0.f) ચાલુ;

	જો (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && ગણતરી >= 4 ? 4 : ગણતરી)))
	  પાછા ખોટું;

	જો (!private_points_checked)
	{
	  auto scan = iterator;
	  કરો
	  {
	    જો (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = સાચું;
	      તોડો;
	    }
	  } જ્યારે (scan.move_to_next ());
	  private_points_checked = સાચું;
	}
	અચલ HBUINT8 *p = iterator.get_serialized_data ();
	unsigned પૂર્ણાંક length = iterator.current_tuple->get_data_size ();
	જો (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  પાછા ખોટું;

	જો (!deltas)
	{
	  જો (unlikely (!deltas_vec.resize_dirty  (ગણતરી))) પાછા ખોટું;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? ગણતરી - 4 : 0), 0,
		     (phantom_only ? 4 : ગણતરી) * કદ (deltas[0]));
	}

	અચલ HBUINT8 *end = p + length;

	તાર્કિક has_private_points = iterator.current_tuple->has_private_points ();
	જો (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  પાછા ખોટું;
	અચલ hb_array_t<unsigned પૂર્ણાંક> &indices = has_private_points ? private_indices : shared_indices;

	તાર્કિક apply_to_all = (indices.length == 0);
	unsigned અંક_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && અંક_deltas >= 4 ? અંક_deltas - 4 : 0);

	જો (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  જો (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, સાચું))) પાછા ખોટું;
	  જો (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, ખોટું))) પાછા ખોટું;
#નહીંતર
	  જો (unlikely (!decompile_deltas_add_to_points<સાચું> (p, points, scalar, end, start_deltas))) પાછા ખોટું;
	  જો (unlikely (!decompile_deltas_add_to_points<ખોટું> (p, points, scalar, end, start_deltas))) પાછા ખોટું;
#endif
	  ચાલુ;
	}

	જો (unlikely (!x_deltas.resize_dirty  (અંક_deltas))) પાછા ખોટું;
	જો (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, ખોટું, start_deltas))) પાછા ખોટું;
	જો (unlikely (!y_deltas.resize_dirty  (અંક_deltas))) પાછા ખોટું;
	જો (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, ખોટું, start_deltas))) પાછા ખોટું;

	જો (!apply_to_all)
	{
	  જો (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    જો (unlikely (orig_points_vec.in_error ())) પાછા ખોટું;
	    orig_points = orig_points_vec.as_array ();
	  }

	  જો (flush)
	  {
	    માટે (unsigned પૂર્ણાંક i = phantom_only ? ગણતરી - 4 : 0; i < ગણતરી; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? ગણતરી - 4 : 0), 0,
		     (phantom_only ? 4 : ગણતરી) * કદ (deltas[0]));
	}

	જો (HB_OPTIMIZE_SIZE_VAL)
	{
	  માટે (unsigned પૂર્ણાંક i = 0; i < અંક_deltas; i++)
	  {
	    unsigned પૂર્ણાંક pt_index;
	    જો (apply_to_all)
	      pt_index = i;
	    નહીંતર
	    {
	      pt_index = indices[i];
	      જો (unlikely (pt_index >= deltas.length)) ચાલુ;
	    }
	    જો (phantom_only && pt_index < ગણતરી - 4) ચાલુ;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	નહીંતર
	{
	  /* Ouch. Four cases... for optimization. */
	  જો (scalar != 1.0f)
	  {
	    જો (apply_to_all)
	      માટે (unsigned પૂર્ણાંક i = phantom_only ? ગણતરી - 4 : 0; i < ગણતરી; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    નહીંતર
	      માટે (unsigned પૂર્ણાંક i = 0; i < અંક_deltas; i++)
	      {
		unsigned પૂર્ણાંક pt_index = indices[i];
		જો (unlikely (pt_index >= deltas.length)) ચાલુ;
		જો (phantom_only && pt_index < ગણતરી - 4) ચાલુ;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  નહીંતર
	  {
	    જો (apply_to_all)
	      માટે (unsigned પૂર્ણાંક i = phantom_only ? ગણતરી - 4 : 0; i < ગણતરી; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    નહીંતર
	      માટે (unsigned પૂર્ણાંક i = 0; i < અંક_deltas; i++)
	      {
		unsigned પૂર્ણાંક pt_index = indices[i];
		જો (unlikely (pt_index >= deltas.length)) ચાલુ;
		જો (phantom_only && pt_index < ગણતરી - 4) ચાલુ;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	જો (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  જ્યારે (સાચું)
	  {
	    જ્યારે (end_point < ગણતરી && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    જો (unlikely (end_point == ગણતરી)) તોડો;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_ગણતરી = 0;
	    માટે (unsigned i = start_point; i < end_point + 1; i++)
	      unref_ગણતરી += deltas.arrayZ[i].flag;
	    unref_ગણતરી = (end_point - start_point + 1) - unref_ગણતરી;

	    unsigned j = start_point;
	    જો (unref_ગણતરી == 0 || unref_ગણતરી > end_point - start_point)
	      જાઓ no_more_gaps;

	    માટે (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned પૂર્ણાંક prev, next, i;
	      માટે (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		જો (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) તોડો;
	      }
	      prev = j = i;
	      માટે (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		જો (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) તોડો;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      માટે (;;)
	      {
		i = next_index (i, start_point, end_point);
		જો (i == next) તોડો;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		જો (--unref_ગણતરી == 0) જાઓ no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = સાચું;

      } જ્યારે (iterator.move_to_next ());

      જો (flush)
      {
	માટે (unsigned પૂર્ણાંક i = phantom_only ? ગણતરી - 4 : 0; i < ગણતરી; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      પાછા સાચું;
    }

    unsigned પૂર્ણાંક get_axis_ગણતરી () અચલ { પાછા table->axis_ગણતરી_2; }

    ખાનગી:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_ગણતરી_2;
  };

  સુરક્ષિત:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_ગણતરી_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_ગણતરી;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_ગણતરી_Type
		glyph_ગણતરી_X;	/* The number of glyphs in this font. This must match the number of
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
  જાહેર:
  DEFINE_SIZE_ARRAY (18 + Glyph_ગણતરી_Type::static_size, offsetZ);
};

સંરચના gvar : gvar_GVAR<gvar, SmallTypes>
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

સંરચના GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

સંરચના gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () અચલ
  {
    જો (has_data ()) પાછા HB_DUAL_GET (*આ, create_cache ());
    પાછા nullptr;
  }

  સ્થિર શૂન્ય destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  તાર્કિક apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<અચલ પૂર્ણાંક> coords,
			       અચલ hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       તાર્કિક phantom_only = ખોટું,
			       int64_t *budget = nullptr) અચલ
  {
    પાછા HB_DUAL_GET (*આ, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned પૂર્ણાંક get_axis_ગણતરી () અચલ
  {
    જો (has_data ()) પાછા HB_DUAL_GET (*આ, get_axis_ગણતરી ());
    પાછા 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
