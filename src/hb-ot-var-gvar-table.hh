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

#അന്തര്ഭാവ "hb-decycler.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-dual.hh"
#അന്തര്ഭാവ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#define HB_OT_TAG_gvar HB_TAG('g','v','a','r')
#define HB_OT_TAG_GVAR HB_TAG('G','V','A','R')

ഘടന hb_glyf_scratch_t
{
  // glyf
  contour_point_vector_t all_points;
  contour_point_vector_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_point_vector_t orig_points;
  hb_vector_t<പൂർണ്ണസംഖ്യ> x_deltas;
  hb_vector_t<പൂർണ്ണസംഖ്യ> y_deltas;
  contour_point_vector_t deltas;
  hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> shared_indices;
  hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> private_indices;
};

നാമാകാശ OT {

template <typename OffsetType>
ഘടന glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  using tuple_variations_t = typename TupleVariationData<OffsetType>::tuple_variations_t;
  using GlyphVariationData = TupleVariationData<OffsetType>;

  hb_vector_t<tuple_variations_t> glyph_variations;

  hb_vector_t<F2DOT14> compiled_shared_tuples;
  സ്വകാര്യ:
  unsigned shared_tuples_എണ്ണം = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  പൊതു:
  unsigned compiled_shared_tuples_എണ്ണം () സ്ഥിരം
  { തിരികെ shared_tuples_എണ്ണം; }

  unsigned compiled_byte_size () സ്ഥിരം
  {
    unsigned byte_size = 0;
    വേണ്ടി (സ്ഥിരം auto& _ : glyph_variations)
      byte_size += _.get_compiled_byte_size ();

    തിരികെ byte_size;
  }

  തർക്കം create_from_glyphs_var_data (unsigned axis_എണ്ണം,
                                    സ്ഥിരം hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
                                    സ്ഥിരം hb_subset_plan_t *plan,
                                    സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_bytes_t>& new_gid_var_data_map)
  {
    എങ്കിൽ (unlikely (!glyph_variations.alloc_exact (plan->new_to_old_gid_list.length)))
      തിരികെ അസത്യം;

    auto it = hb_iter (plan->new_to_old_gid_list);
    വേണ്ടി (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      contour_point_vector_t *all_contour_points;
      എങ്കിൽ (!new_gid_var_data_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        തിരികെ അസത്യം;
      hb_bytes_t var_data = new_gid_var_data_map.get (new_gid);

      സ്ഥിരം GlyphVariationData* p = reinterpret_cast<സ്ഥിരം GlyphVariationData*> (var_data.arrayZ);
      typename GlyphVariationData::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_vector_t<unsigned> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      എങ്കിൽ (!var_data || ! p->has_data () || !all_contour_points->length ||
          !GlyphVariationData::get_tuple_iterator (var_data, axis_എണ്ണം,
                                                   var_data.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        തുടരുക;
      }

      തർക്കം is_composite_glyph = അസത്യം;
      is_composite_glyph = plan->composite_new_gids.has (new_gid);

      എങ്കിൽ (!p->decompile_tuple_variations (all_contour_points->length, സത്യം /* is_gvar */,
                                          iterator, &(plan->axes_old_index_tag_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /* OUT */
					  &pool,
                                          is_composite_glyph))
        തിരികെ അസത്യം;
      glyph_variations.push (std::move (tuple_vars));
    }
    തിരികെ !glyph_variations.in_error () && glyph_variations.length == plan->new_to_old_gid_list.length;
  }

  തർക്കം instantiate (സ്ഥിരം hb_subset_plan_t *plan)
  {
    unsigned എണ്ണം = plan->new_to_old_gid_list.length;
    തർക്കം iup_optimize = അസത്യം;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_codepoint_t new_gid = plan->new_to_old_gid_list[i].first;
      contour_point_vector_t *all_points;
      എങ്കിൽ (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        തിരികെ അസത്യം;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_point_vector_t points;
      എങ്കിൽ (iup_optimize)
      {
        points = *all_points;
        എങ്കിൽ (unlikely (points.in_error ())) തിരികെ അസത്യം;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      എങ്കിൽ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      എങ്കിൽ (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  തർക്കം compile_bytes (സ്ഥിരം hb_map_t& axes_index_map,
                      സ്ഥിരം hb_map_t& axes_old_index_tag_map)
  {
    എങ്കിൽ (!compile_shared_tuples (axes_index_map, axes_old_index_tag_map))
      തിരികെ അസത്യം;
    വേണ്ടി (tuple_variations_t& vars: glyph_variations)
      എങ്കിൽ (!vars.compile_bytes (axes_index_map, axes_old_index_tag_map,
                               സത്യം, /* use shared points*/
                               സത്യം,
                               &shared_tuples_idx_map,
			       &pool))
        തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  തർക്കം compile_shared_tuples (സ്ഥിരം hb_map_t& axes_index_map,
                              സ്ഥിരം hb_map_t& axes_old_index_tag_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned> coords_എണ്ണം_map;

    /* count the num of shared coords */
    വേണ്ടി (tuple_variations_t& vars: glyph_variations)
    {
      വേണ്ടി (tuple_delta_t& var : vars.tuple_vars)
      {
        എങ്കിൽ (!var.compile_coords (axes_index_map, axes_old_index_tag_map, &pool))
          തിരികെ അസത്യം;
        unsigned *എണ്ണം;
	unsigned hash = hb_hash (&var.compiled_peak_coords);
        എങ്കിൽ (coords_എണ്ണം_map.has_with_hash (&(var.compiled_peak_coords), hash, &എണ്ണം))
	  (*എണ്ണം)++;
        അല്ലെങ്കിൽ
          coords_എണ്ണം_map.set_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    എങ്കിൽ (!coords_എണ്ണം_map || coords_എണ്ണം_map.in_error ())
      തിരികെ അസത്യം;

    /* add only those coords that are used more than once into the vector and sort */
    hb_vector_t<hb_pair_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned>> shared_coords {
      + hb_iter (coords_എണ്ണം_map)
      | hb_filter ([] (സ്ഥിരം hb_pair_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned>& p) { തിരികെ p.second > 1; })
    };
    എങ്കിൽ (unlikely (shared_coords.in_error ())) തിരികെ അസത്യം;

    /* no shared tuples: no coords are used more than once */
    എങ്കിൽ (!shared_coords) തിരികെ സത്യം;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_എണ്ണം = hb_min (0xFFFu + 1, shared_coords.length);
    unsigned len = shared_tuples_എണ്ണം * (shared_coords[0].first->length);
    എങ്കിൽ (unlikely (!compiled_shared_tuples.alloc (len)))
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < shared_tuples_എണ്ണം; i++)
    {
      shared_tuples_idx_map.set (shared_coords[i].first, i);
      /* add a concat() in hb_vector_t? */
      വേണ്ടി (auto c : shared_coords[i].first->iter ())
        compiled_shared_tuples.push (c);
    }

    തിരികെ സത്യം;
  }

  സ്ഥിതം പൂർണ്ണസംഖ്യ _cmp_coords (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *a = (സ്ഥിരം hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pa;
    സ്ഥിരം hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *b = (സ്ഥിരം hb_pair_t<hb_vector_t<F2DOT14> *, unsigned> *) pb;

    എങ്കിൽ (a->second != b->second)
      തിരികെ b->second - a->second; // high to low

    തിരികെ b->first->as_array().cmp (a->first->as_array ());
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_glyph_var_data (hb_serialize_context_t *c,
                                 Iterator it,
                                 തർക്കം long_offset,
                                 unsigned അക്കം_glyphs,
                                 അക്ഷരം* glyph_var_data_offsets /* OUT: glyph var data offsets array */) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (long_offset)
    {
      ((HBUINT32 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 4;
    }
    അല്ലെങ്കിൽ
    {
      ((HBUINT16 *) glyph_var_data_offsets)[0] = 0;
      glyph_var_data_offsets += 2;
    }
    unsigned glyph_offset = 0;
    hb_codepoint_t last_gid = 0;
    unsigned idx = 0;

    GlyphVariationData* cur_glyph = c->start_embed<GlyphVariationData> ();
    എങ്കിൽ (!cur_glyph) return_trace (അസത്യം);
    വേണ്ടി (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      എങ്കിൽ (long_offset)
        വേണ്ടി (; last_gid < gid; last_gid++)
          ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
      അല്ലെങ്കിൽ
        വേണ്ടി (; last_gid < gid; last_gid++)
          ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;

      എങ്കിൽ (idx >= glyph_variations.length) return_trace (അസത്യം);
      എങ്കിൽ (!cur_glyph->serialize (c, സത്യം, glyph_variations[idx])) return_trace (അസത്യം);
      GlyphVariationData* next_glyph = c->start_embed<GlyphVariationData> ();
      glyph_offset += (അക്ഷരം *) next_glyph - (അക്ഷരം *) cur_glyph;

      എങ്കിൽ (long_offset)
        ((HBUINT32 *) glyph_var_data_offsets)[gid] = glyph_offset;
      അല്ലെങ്കിൽ
        ((HBUINT16 *) glyph_var_data_offsets)[gid] = glyph_offset / 2;

      last_gid++;
      idx++;
      cur_glyph = next_glyph;
    }

    എങ്കിൽ (long_offset)
      വേണ്ടി (; last_gid < അക്കം_glyphs; last_gid++)
        ((HBUINT32 *) glyph_var_data_offsets)[last_gid] = glyph_offset;
    അല്ലെങ്കിൽ
      വേണ്ടി (; last_gid < അക്കം_glyphs; last_gid++)
        ((HBUINT16 *) glyph_var_data_offsets)[last_gid] = glyph_offset / 2;
    return_trace (സത്യം);
  }
};

template <typename T, typename Types>
ഘടന gvar_GVAR
{
  using Glyph_എണ്ണം_Type = typename Types::HBUINT;

  using GlyphVariationData = TupleVariationData<Glyph_എണ്ണം_Type>;
  സ്ഥിതം constexpr unsigned glyph_എണ്ണം_max ()
  { തിരികെ (1u << (8 * Glyph_എണ്ണം_Type::static_size)) - 1; }

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int () != 0; }

  തർക്കം sanitize_shallow (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  (version.major == 1) &&
		  sharedTuples.sanitize (c, ഇത്, axis_എണ്ണം_2 * shared_Tuple_എണ്ണം) &&
		  (is_long_offset () ?
		     c->check_array (get_long_offset_array (), c->get_അക്കം_glyphs () + 1) :
		     c->check_array (get_short_offset_array (), c->get_അക്കം_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  { തിരികെ sanitize_shallow (c); }

  തർക്കം decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_എണ്ണം_Type>& glyph_vars /* OUT */) സ്ഥിരം
  {
    hb_hashmap_t<hb_codepoint_t, hb_bytes_t> new_gid_var_data_map;
    auto it = hb_iter (c->plan->new_to_old_gid_list);
    എങ്കിൽ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_data_map.set (0, hb_bytes_t ());
      it++;
    }

    വേണ്ടി (auto &_ : it)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;
      hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_എണ്ണം_X, old_gid);
      new_gid_var_data_map.set (new_gid, var_data_bytes);
    }

    എങ്കിൽ (new_gid_var_data_map.in_error ()) തിരികെ അസത്യം;

    hb_array_t<സ്ഥിരം F2DOT14> shared_tuples = (ഇത്+sharedTuples).as_array ((unsigned) shared_Tuple_എണ്ണം * (unsigned) axis_എണ്ണം_2);
    തിരികെ glyph_vars.create_from_glyphs_var_data (axis_എണ്ണം_2, shared_tuples, c->plan, new_gid_var_data_map);
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  സ്ഥിരം glyph_variations_t<Glyph_എണ്ണം_Type>& glyph_vars,
                  Iterator it,
                  unsigned axis_എണ്ണം,
                  unsigned അക്കം_glyphs,
                  തർക്കം force_long_offsets) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    gvar_GVAR *out = c->allocate_min<gvar_GVAR> ();
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_എണ്ണം_2 = axis_എണ്ണം;
    out->glyph_എണ്ണം_X = hb_min (glyph_എണ്ണം_max (), അക്കം_glyphs);

    unsigned glyph_var_data_size = glyph_vars.compiled_byte_size ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    തർക്കം long_offset = glyph_var_data_size > 0x1FFFEu || force_long_offsets;
    out->flags = long_offset ? 1 : 0;

    unsigned glyph_var_data_offsets_size = (long_offset ? 4 : 2) * (അക്കം_glyphs + 1);
    HBUINT8 *glyph_var_data_offsets = c->allocate_size<HBUINT8> (glyph_var_data_offsets_size, അസത്യം);
    എങ്കിൽ (!glyph_var_data_offsets) return_trace (അസത്യം);

    /* shared tuples */
    unsigned shared_tuple_എണ്ണം = glyph_vars.compiled_shared_tuples_എണ്ണം ();
    out->shared_Tuple_എണ്ണം = shared_tuple_എണ്ണം;

    എങ്കിൽ (!shared_tuple_എണ്ണം)
      out->sharedTuples = (അക്ഷരം *) glyph_var_data_offsets + glyph_var_data_offsets_size - (അക്ഷരം *) out;
    അല്ലെങ്കിൽ
    {
      hb_array_t<സ്ഥിരം F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      എങ്കിൽ (!shared_tuples.arrayZ) return_trace (അസത്യം);
      out->sharedTuples = (സ്ഥിരം അക്ഷരം *) shared_tuples.arrayZ - (അക്ഷരം *) out;
    }

    അക്ഷരം *glyph_var_data = c->start_embed<അക്ഷരം> ();
    എങ്കിൽ (!glyph_var_data) return_trace (അസത്യം);
    out->dataZ = glyph_var_data - (അക്ഷരം *) out;

    return_trace (glyph_vars.serialize_glyph_var_data (c, it, long_offset, അക്കം_glyphs,
                                                       (അക്ഷരം *) glyph_var_data_offsets));
  }

  തർക്കം instantiate (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    glyph_variations_t<Glyph_എണ്ണം_Type> glyph_vars;
    എങ്കിൽ (!decompile_glyph_variations (c, glyph_vars))
      return_trace (അസത്യം);

    എങ്കിൽ (!glyph_vars.instantiate (c->plan)) return_trace (അസത്യം);
    എങ്കിൽ (!glyph_vars.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map))
      return_trace (അസത്യം);

    unsigned axis_എണ്ണം = c->plan->axes_index_map.get_population ();
    unsigned അക്കം_glyphs = c->plan->അക്കം_output_glyphs ();
    auto it = hb_iter (c->plan->new_to_old_gid_list);

    തർക്കം force_long_offsets = അസത്യം;
#ifdef HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#endif
    return_trace (serialize (c->serializer, glyph_vars, it, axis_എണ്ണം, അക്കം_glyphs, force_long_offsets));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (c->plan->all_axes_pinned)
      return_trace (അസത്യം);

    എങ്കിൽ (c->plan->normalized_coords)
      return_trace (instantiate (c));

    unsigned glyph_എണ്ണം = version.to_int () ? c->plan->source->get_അക്കം_glyphs () : 0;

    gvar_GVAR *out = c->serializer->allocate_min<gvar_GVAR> ();
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->version.major = 1;
    out->version.minor = 0;
    out->axis_എണ്ണം_2 = axis_എണ്ണം_2;
    out->shared_Tuple_എണ്ണം = shared_Tuple_എണ്ണം;

    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = c->plan->അക്കം_output_glyphs ();
    out->glyph_എണ്ണം_X = hb_min (glyph_എണ്ണം_max (), അക്കം_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_vector_t<hb_vector_t<അക്ഷരം>> culled_vars;
    hb_set_t culled_gids;
    എങ്കിൽ (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<സ്ഥിരം F2DOT14> shared_tuples_array =
	  (ഇത്+sharedTuples).as_array ((unsigned) shared_Tuple_എണ്ണം * (unsigned) axis_എണ്ണം_2);
      എങ്കിൽ (unlikely (!culled_vars.resize (അക്കം_glyphs))) return_trace (അസത്യം);
      auto cull_it = hb_iter (c->plan->new_to_old_gid_list);
      എങ്കിൽ (cull_it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      വേണ്ടി (auto &_ : cull_it)
      {
	hb_bytes_t var_data_bytes = get_glyph_var_data_bytes (c->source_blob, glyph_എണ്ണം, _.second);
	എങ്കിൽ (var_data_bytes.length < GlyphVariationData::min_size) തുടരുക;
	തർക്കം changed = അസത്യം;
	എങ്കിൽ (unlikely (!var_data_bytes.as<GlyphVariationData> ()
		      ->cull_tuple_variations (var_data_bytes, axis_എണ്ണം_2,
					       shared_tuples_array,
					       &c->plan->axes_old_index_tag_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.first], &changed)))
	  return_trace (അസത്യം);
	എങ്കിൽ (changed) culled_gids.add (_.first);
      }
      എങ്കിൽ (unlikely (culled_gids.in_error ())) return_trace (അസത്യം);
    }

    auto it = hb_iter (c->plan->new_to_old_gid_list);
    എങ്കിൽ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    unsigned subset_data_size = 0;
    unsigned padding_size = 0;
    വേണ്ടി (auto &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      unsigned glyph_data_size = culled_gids.has (_.first)
				 ? culled_vars[_.first].length
				 : get_glyph_var_data_bytes (c->source_blob, glyph_എണ്ണം, old_gid).length;
      എങ്കിൽ (glyph_data_size % 2)
      {
        glyph_data_size++;
        padding_size++;
      }

      എങ്കിൽ (unlikely (hb_unsigned_add_overflows (subset_data_size, glyph_data_size, &subset_data_size)))
	return_trace (അസത്യം);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    തർക്കം long_offset = subset_data_size > 0x1FFFEu;
#ifdef HB_EXPERIMENTAL_API
    long_offset = long_offset || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif
    out->flags = long_offset ? 1 : 0;

    unsigned subset_offsets_size = (long_offset ? 4 : 2) * (അക്കം_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_size<HBUINT8> (subset_offsets_size, അസത്യം);
    എങ്കിൽ (!subset_offsets) return_trace (അസത്യം);

    /* shared tuples */
    എങ്കിൽ (!shared_Tuple_എണ്ണം || !sharedTuples)
      out->sharedTuples = (അക്ഷരം *) subset_offsets + subset_offsets_size - (അക്ഷരം *) out;
    അല്ലെങ്കിൽ
    {
      unsigned പൂർണ്ണസംഖ്യ shared_tuple_size = F2DOT14::static_size * axis_എണ്ണം_2 * shared_Tuple_എണ്ണം;
      F2DOT14 *tuples = c->serializer->allocate_size<F2DOT14> (shared_tuple_size);
      എങ്കിൽ (!tuples) return_trace (അസത്യം);
      out->sharedTuples = (അക്ഷരം *) tuples - (അക്ഷരം *) out;
      hb_memcpy (tuples, ഇത്+sharedTuples, shared_tuple_size);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    എങ്കിൽ (long_offset)
      subset_data_size -= padding_size;
    അക്ഷരം *subset_data = c->serializer->allocate_size<അക്ഷരം> (subset_data_size, അസത്യം);
    എങ്കിൽ (!subset_data) return_trace (അസത്യം);
    out->dataZ = subset_data - (അക്ഷരം *) out;


    എങ്കിൽ (long_offset)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    അല്ലെങ്കിൽ
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    unsigned പൂർണ്ണസംഖ്യ glyph_offset = 0;

    hb_codepoint_t last = 0;
    it = hb_iter (c->plan->new_to_old_gid_list);
    എങ്കിൽ (it->first == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    വേണ്ടി (auto &_ : it)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_gid = _.second;

      എങ്കിൽ (long_offset)
	വേണ്ടി (; last < gid; last++)
	  ((HBUINT32 *) subset_offsets)[last] = glyph_offset;
      അല്ലെങ്കിൽ
	വേണ്ടി (; last < gid; last++)
	  ((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

      hb_bytes_t var_data_bytes = culled_gids.has (gid)
				  ? hb_bytes_t (culled_vars[gid].arrayZ,
						culled_vars[gid].length)
				  : get_glyph_var_data_bytes (c->source_blob,
							      glyph_എണ്ണം,
							      old_gid);

      hb_memcpy (subset_data, var_data_bytes.arrayZ, var_data_bytes.length);
      unsigned glyph_data_size = var_data_bytes.length;
      subset_data += glyph_data_size;
      glyph_offset += glyph_data_size;

      എങ്കിൽ (!long_offset && (glyph_data_size % 2))
      {
        *subset_data = 0;
        subset_data++;
        glyph_offset++;
      }

      എങ്കിൽ (long_offset)
	((HBUINT32 *) subset_offsets)[gid] = glyph_offset;
      അല്ലെങ്കിൽ
	((HBUINT16 *) subset_offsets)[gid] = glyph_offset / 2;

      last++; // Skip over gid
    }

    എങ്കിൽ (long_offset)
      വേണ്ടി (; last < അക്കം_glyphs; last++)
	((HBUINT32 *) subset_offsets)[last] = glyph_offset;
    അല്ലെങ്കിൽ
      വേണ്ടി (; last < അക്കം_glyphs; last++)
	((HBUINT16 *) subset_offsets)[last] = glyph_offset / 2;

    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  സ്ഥിരം hb_bytes_t get_glyph_var_data_bytes (hb_blob_t *blob,
					     unsigned glyph_എണ്ണം,
					     hb_codepoint_t glyph) സ്ഥിരം
  {
    unsigned start_offset = get_offset (glyph_എണ്ണം, glyph);
    unsigned end_offset = get_offset (glyph_എണ്ണം, glyph+1);
    എങ്കിൽ (unlikely (end_offset < start_offset)) തിരികെ hb_bytes_t ();
    unsigned length = end_offset - start_offset;
    hb_bytes_t var_data = blob->as_bytes ().sub_array (((unsigned) dataZ) + start_offset, length);
    തിരികെ likely (var_data.length >= GlyphVariationData::min_size) ? var_data : hb_bytes_t ();
  }

  തർക്കം is_long_offset () സ്ഥിരം { തിരികെ flags & 1; }

  unsigned get_offset (unsigned glyph_എണ്ണം, unsigned i) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (i > glyph_എണ്ണം)) തിരികെ 0;
    hb_barrier ();
    തിരികെ is_long_offset () ? get_long_offset_array ()[i] : get_short_offset_array ()[i] * 2;
  }

  സ്ഥിരം HBUINT32 * get_long_offset_array () സ്ഥിരം { തിരികെ (സ്ഥിരം HBUINT32 *) &offsetZ; }
  സ്ഥിരം HBUINT16 *get_short_offset_array () സ്ഥിരം { തിരികെ (സ്ഥിരം HBUINT16 *) &offsetZ; }

  പൊതു:
  ഘടന accelerator_t
  {

    hb_scalar_cache_t *create_cache () സ്ഥിരം
    {
      തിരികെ hb_scalar_cache_t::create (hb_min ((unsigned) table->shared_Tuple_എണ്ണം,
						+TupleVariationHeader::max_shared_tuple_എണ്ണം));
    }

    സ്ഥിതം ശൂന്യം destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    തർക്കം has_data () സ്ഥിരം { തിരികെ table->has_data (); }

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<gvar_GVAR> (face, T::tableTag);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_എണ്ണം_2 = table->version.to_int () ? face->get_അക്കം_glyphs () : 0;
    }
    ~accelerator_t () { table.destroy (); }

    സ്വകാര്യ:

    സ്ഥിതം ദശാംശം infer_delta (സ്ഥിരം hb_array_t<contour_point_t> points,
			      സ്ഥിരം hb_array_t<contour_point_t> deltas,
			      unsigned പൂർണ്ണസംഖ്യ target, unsigned പൂർണ്ണസംഖ്യ prev, unsigned പൂർണ്ണസംഖ്യ next,
			      ദശാംശം contour_point_t::*m)
    {
      ദശാംശം target_val = points.arrayZ[target].*m;
      ദശാംശം prev_val = points.arrayZ[prev].*m;
      ദശാംശം next_val = points.arrayZ[next].*m;
      ദശാംശം prev_delta =  deltas.arrayZ[prev].*m;
      ദശാംശം next_delta =  deltas.arrayZ[next].*m;

      എങ്കിൽ (prev_val == next_val)
	തിരികെ (prev_delta == next_delta) ? prev_delta : 0.f;
      അല്ലെങ്കിൽ എങ്കിൽ (target_val <= hb_min (prev_val, next_val))
	തിരികെ (prev_val < next_val) ? prev_delta : next_delta;
      അല്ലെങ്കിൽ എങ്കിൽ (target_val >= hb_max (prev_val, next_val))
	തിരികെ (prev_val > next_val) ? prev_delta : next_delta;

      /* linear interpolation */
      ദശാംശം r = (target_val - prev_val) / (next_val - prev_val);
      തിരികെ prev_delta + r * (next_delta - prev_delta);
    }

    സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ next_index (unsigned പൂർണ്ണസംഖ്യ i, unsigned പൂർണ്ണസംഖ്യ start, unsigned പൂർണ്ണസംഖ്യ end)
    { തിരികെ (i >= end) ? start : (i + 1); }

#ifndef HB_OPTIMIZE_SIZE
    template <തർക്കം is_x>
#endif
    സ്ഥിതം തർക്കം decompile_deltas_add_to_points (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_point_t> points,
						ദശാംശം scalar,
						സ്ഥിരം HBUINT8 *end,
						unsigned start
#ifdef HB_OPTIMIZE_SIZE
						, തർക്കം is_x
#endif
						)
    {
      unsigned i = 0;
      unsigned എണ്ണം = points.length;
      ആകുമ്പോൾ (i < എണ്ണം)
      {
	എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
	unsigned control = *p++;
	unsigned run_എണ്ണം = (control & Tuple_മൂല്യങ്ങൾ::മൂല്യം_RUN_എണ്ണം_MASK) + 1;
	unsigned stop = i + run_എണ്ണം;
	എങ്കിൽ (unlikely (stop > എണ്ണം)) തിരികെ അസത്യം;

	unsigned skip = i < start ? hb_min (start - i, run_എണ്ണം) : 0;
	i += skip;

	തിരഞ്ഞെടുപ്പ് (control & Tuple_മൂല്യങ്ങൾ::മൂല്യങ്ങൾ_SIZE_MASK)
	{
	  അവസ്ഥ Tuple_മൂല്യങ്ങൾ::മൂല്യങ്ങൾ_ARE_ZEROS:
	    i = stop;
	    നിർത്തുക;
	  അവസ്ഥ Tuple_മൂല്യങ്ങൾ::മൂല്യങ്ങൾ_ARE_വാക്കുകൾ:
	  {
	    എങ്കിൽ (unlikely (p + run_എണ്ണം * HBINT16::static_size > end)) തിരികെ അസത്യം;
	    p += skip * HBINT16::static_size;
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT16 *) p;
	    വേണ്ടി (; i < stop; i++)
	    {
	      ദശാംശം v = *pp++ * scalar;
	      എങ്കിൽ (is_x) points.arrayZ[i].x += v;
	      അല്ലെങ്കിൽ points.arrayZ[i].y += v;
	    }
	    p = (സ്ഥിരം HBUINT8 *) pp;
	  }
	  നിർത്തുക;
	  അവസ്ഥ Tuple_മൂല്യങ്ങൾ::മൂല്യങ്ങൾ_ARE_LONGS:
	  {
	    എങ്കിൽ (unlikely (p + run_എണ്ണം * HBINT32::static_size > end)) തിരികെ അസത്യം;
	    p += skip * HBINT32::static_size;
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT32 *) p;
	    വേണ്ടി (; i < stop; i++)
	    {
	      ദശാംശം v = *pp++ * scalar;
	      എങ്കിൽ (is_x) points.arrayZ[i].x += v;
	      അല്ലെങ്കിൽ points.arrayZ[i].y += v;
	    }
	    p = (സ്ഥിരം HBUINT8 *) pp;
	  }
	  നിർത്തുക;
	  അവസ്ഥ Tuple_മൂല്യങ്ങൾ::മൂല്യങ്ങൾ_ARE_BYTES:
	  {
	    എങ്കിൽ (unlikely (p + run_എണ്ണം > end)) തിരികെ അസത്യം;
	    p += skip * HBINT8::static_size;
	    സ്ഥിരം auto *pp = (സ്ഥിരം HBINT8 *) p;
	    വേണ്ടി (; i < stop; i++)
	    {
	      ദശാംശം v = *pp++ * scalar;
	      എങ്കിൽ (is_x) points.arrayZ[i].x += v;
	      അല്ലെങ്കിൽ points.arrayZ[i].y += v;
	    }
	    p = (സ്ഥിരം HBUINT8 *) pp;
	  }
	  നിർത്തുക;
	}
      }
      തിരികെ സത്യം;
    }
    പൊതു:
    തർക്കം apply_deltas_to_points (hb_codepoint_t glyph,
				 hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
				 സ്ഥിരം hb_array_t<contour_point_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr,
				 തർക്കം phantom_only = അസത്യം,
				 int64_t *budget = nullptr) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (glyph >= glyph_എണ്ണം_2)) തിരികെ സത്യം;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_bytes_t var_data_bytes = table->get_glyph_var_data_bytes (table.get_blob (), glyph_എണ്ണം_2, glyph);
      എങ്കിൽ (!var_data_bytes.as<GlyphVariationData> ()->has_data ()) തിരികെ സത്യം;

      auto &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      typename GlyphVariationData::tuple_iterator_t iterator;
      എങ്കിൽ (!GlyphVariationData::get_tuple_iterator (var_data_bytes, table->axis_എണ്ണം_2,
						   var_data_bytes.arrayZ,
						   shared_indices, &iterator))
	തിരികെ സത്യം; /* so isn't applied at all */

      തർക്കം any_private_points = അസത്യം;
      തർക്കം private_points_checked = അസത്യം;

      /* Save original points for inferred delta calculation */
      auto &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      auto orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      auto &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      auto deltas = deltas_vec.as_array ();

      unsigned അക്കം_coords = table->axis_എണ്ണം_2;
      hb_array_t<സ്ഥിരം F2DOT14> shared_tuples = (table+table->sharedTuples).as_array (table->shared_Tuple_എണ്ണം * അക്കം_coords);

      auto &private_indices = scratch.private_indices;
      auto &x_deltas = scratch.x_deltas;
      auto &y_deltas = scratch.y_deltas;

      unsigned എണ്ണം = points.length;
      തർക്കം flush = അസത്യം;

      ചെയ്യുക
      {
	ദശാംശം scalar = iterator.current_tuple->calculate_scalar (coords, അക്കം_coords, shared_tuples,
								 scalar_cache);

	എങ്കിൽ (scalar == 0.f) തുടരുക;

	എങ്കിൽ (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && എണ്ണം >= 4 ? 4 : എണ്ണം)))
	  തിരികെ അസത്യം;

	എങ്കിൽ (!private_points_checked)
	{
	  auto scan = iterator;
	  ചെയ്യുക
	  {
	    എങ്കിൽ (scan.current_tuple->has_private_points ())
	    {
	      any_private_points = സത്യം;
	      നിർത്തുക;
	    }
	  } ആകുമ്പോൾ (scan.move_to_next ());
	  private_points_checked = സത്യം;
	}
	സ്ഥിരം HBUINT8 *p = iterator.get_serialized_data ();
	unsigned പൂർണ്ണസംഖ്യ length = iterator.current_tuple->get_data_size ();
	എങ്കിൽ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
	  തിരികെ അസത്യം;

	എങ്കിൽ (!deltas)
	{
	  എങ്കിൽ (unlikely (!deltas_vec.resize_dirty  (എണ്ണം))) തിരികെ അസത്യം;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? എണ്ണം - 4 : 0), 0,
		     (phantom_only ? 4 : എണ്ണം) * വലുപ്പം (deltas[0]));
	}

	സ്ഥിരം HBUINT8 *end = p + length;

	തർക്കം has_private_points = iterator.current_tuple->has_private_points ();
	എങ്കിൽ (has_private_points &&
	    !GlyphVariationData::decompile_points (p, private_indices, end))
	  തിരികെ അസത്യം;
	സ്ഥിരം hb_array_t<unsigned പൂർണ്ണസംഖ്യ> &indices = has_private_points ? private_indices : shared_indices;

	തർക്കം apply_to_all = (indices.length == 0);
	unsigned അക്കം_deltas = apply_to_all ? points.length : indices.length;
	unsigned start_deltas = (apply_to_all && phantom_only && അക്കം_deltas >= 4 ? അക്കം_deltas - 4 : 0);

	എങ്കിൽ (apply_to_all && !any_private_points)
	{
#ifdef HB_OPTIMIZE_SIZE
	  എങ്കിൽ (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, സത്യം))) തിരികെ അസത്യം;
	  എങ്കിൽ (unlikely (!decompile_deltas_add_to_points (p, points, scalar, end, start_deltas, അസത്യം))) തിരികെ അസത്യം;
#അല്ലെങ്കിൽ
	  എങ്കിൽ (unlikely (!decompile_deltas_add_to_points<സത്യം> (p, points, scalar, end, start_deltas))) തിരികെ അസത്യം;
	  എങ്കിൽ (unlikely (!decompile_deltas_add_to_points<അസത്യം> (p, points, scalar, end, start_deltas))) തിരികെ അസത്യം;
#endif
	  തുടരുക;
	}

	എങ്കിൽ (unlikely (!x_deltas.resize_dirty  (അക്കം_deltas))) തിരികെ അസത്യം;
	എങ്കിൽ (unlikely (!GlyphVariationData::decompile_deltas (p, x_deltas, end, അസത്യം, start_deltas))) തിരികെ അസത്യം;
	എങ്കിൽ (unlikely (!y_deltas.resize_dirty  (അക്കം_deltas))) തിരികെ അസത്യം;
	എങ്കിൽ (unlikely (!GlyphVariationData::decompile_deltas (p, y_deltas, end, അസത്യം, start_deltas))) തിരികെ അസത്യം;

	എങ്കിൽ (!apply_to_all)
	{
	  എങ്കിൽ (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    എങ്കിൽ (unlikely (orig_points_vec.in_error ())) തിരികെ അസത്യം;
	    orig_points = orig_points_vec.as_array ();
	  }

	  എങ്കിൽ (flush)
	  {
	    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = phantom_only ? എണ്ണം - 4 : 0; i < എണ്ണം; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? എണ്ണം - 4 : 0), 0,
		     (phantom_only ? 4 : എണ്ണം) * വലുപ്പം (deltas[0]));
	}

	എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL)
	{
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_deltas; i++)
	  {
	    unsigned പൂർണ്ണസംഖ്യ pt_index;
	    എങ്കിൽ (apply_to_all)
	      pt_index = i;
	    അല്ലെങ്കിൽ
	    {
	      pt_index = indices[i];
	      എങ്കിൽ (unlikely (pt_index >= deltas.length)) തുടരുക;
	    }
	    എങ്കിൽ (phantom_only && pt_index < എണ്ണം - 4) തുടരുക;
	    auto &delta = deltas.arrayZ[pt_index];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.add_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	അല്ലെങ്കിൽ
	{
	  /* Ouch. Four cases... for optimization. */
	  എങ്കിൽ (scalar != 1.0f)
	  {
	    എങ്കിൽ (apply_to_all)
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = phantom_only ? എണ്ണം - 4 : 0; i < എണ്ണം; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    അല്ലെങ്കിൽ
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_deltas; i++)
	      {
		unsigned പൂർണ്ണസംഖ്യ pt_index = indices[i];
		എങ്കിൽ (unlikely (pt_index >= deltas.length)) തുടരുക;
		എങ്കിൽ (phantom_only && pt_index < എണ്ണം - 4) തുടരുക;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  അല്ലെങ്കിൽ
	  {
	    എങ്കിൽ (apply_to_all)
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = phantom_only ? എണ്ണം - 4 : 0; i < എണ്ണം; i++)
	      {
		auto &delta = deltas.arrayZ[i];
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    അല്ലെങ്കിൽ
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_deltas; i++)
	      {
		unsigned പൂർണ്ണസംഖ്യ pt_index = indices[i];
		എങ്കിൽ (unlikely (pt_index >= deltas.length)) തുടരുക;
		എങ്കിൽ (phantom_only && pt_index < എണ്ണം - 4) തുടരുക;
		auto &delta = deltas.arrayZ[pt_index];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.add_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	എങ്കിൽ (!apply_to_all && !phantom_only)
	{
	  unsigned start_point = 0;
	  unsigned end_point = 0;
	  ആകുമ്പോൾ (സത്യം)
	  {
	    ആകുമ്പോൾ (end_point < എണ്ണം && !points.arrayZ[end_point].is_end_point)
	      end_point++;
	    എങ്കിൽ (unlikely (end_point == എണ്ണം)) നിർത്തുക;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    unsigned unref_എണ്ണം = 0;
	    വേണ്ടി (unsigned i = start_point; i < end_point + 1; i++)
	      unref_എണ്ണം += deltas.arrayZ[i].flag;
	    unref_എണ്ണം = (end_point - start_point + 1) - unref_എണ്ണം;

	    unsigned j = start_point;
	    എങ്കിൽ (unref_എണ്ണം == 0 || unref_എണ്ണം > end_point - start_point)
	      പോകുക no_more_gaps;

	    വേണ്ടി (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      unsigned പൂർണ്ണസംഖ്യ prev, next, i;
	      വേണ്ടി (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		എങ്കിൽ (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) നിർത്തുക;
	      }
	      prev = j = i;
	      വേണ്ടി (;;)
	      {
		i = j;
		j = next_index (i, start_point, end_point);
		എങ്കിൽ (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) നിർത്തുക;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      വേണ്ടി (;;)
	      {
		i = next_index (i, start_point, end_point);
		എങ്കിൽ (i == next) നിർത്തുക;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_point_t::y);
		എങ്കിൽ (--unref_എണ്ണം == 0) പോകുക no_more_gaps;
	      }
	    }
	  no_more_gaps:
	    start_point = end_point = end_point + 1;
	  }
	}

	flush = സത്യം;

      } ആകുമ്പോൾ (iterator.move_to_next ());

      എങ്കിൽ (flush)
      {
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = phantom_only ? എണ്ണം - 4 : 0; i < എണ്ണം; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      തിരികെ സത്യം;
    }

    unsigned പൂർണ്ണസംഖ്യ get_axis_എണ്ണം () സ്ഥിരം { തിരികെ table->axis_എണ്ണം_2; }

    സ്വകാര്യ:
    hb_blob_ptr_t<gvar_GVAR> table;
    unsigned glyph_എണ്ണം_2;
  };

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_എണ്ണം_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_എണ്ണം;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32To<UnsizedArrayOf<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_എണ്ണം_Type
		glyph_എണ്ണം_X;	/* The number of glyphs in this font. This must match the number of
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
  പൊതു:
  DEFINE_SIZE_ARRAY (18 + Glyph_എണ്ണം_Type::static_size, offsetZ);
};

ഘടന gvar : gvar_GVAR<gvar, SmallTypes>
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_gvar;
};

ഘടന GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_GVAR;
};

ഘടന gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () സ്ഥിരം
  {
    എങ്കിൽ (has_data ()) തിരികെ HB_DUAL_GET (*ഇത്, create_cache ());
    തിരികെ nullptr;
  }

  സ്ഥിതം ശൂന്യം destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  തർക്കം apply_deltas_to_points (hb_codepoint_t glyph,
			       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
			       സ്ഥിരം hb_array_t<contour_point_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = nullptr,
			       തർക്കം phantom_only = അസത്യം,
			       int64_t *budget = nullptr) സ്ഥിരം
  {
    തിരികെ HB_DUAL_GET (*ഇത്, apply_deltas_to_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  unsigned പൂർണ്ണസംഖ്യ get_axis_എണ്ണം () സ്ഥിരം
  {
    എങ്കിൽ (has_data ()) തിരികെ HB_DUAL_GET (*ഇത്, get_axis_എണ്ണം ());
    തിരികെ 0;
  }
};

} /* namespace OT */

#endif /* HB_OT_VAR_GVAR_TABLE_HH */
