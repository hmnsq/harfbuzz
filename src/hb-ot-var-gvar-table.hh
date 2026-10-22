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

#నిర్వచితం_కాకపోతే HB_OT_VAR_GVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_GVAR_పట్టిక_HH

#అన్తర్భావ "hb-decycler.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"
#అన్తర్భావ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#నిర్వచించు HB_OT_ట్యాగ్_gvar HB_ట్యాగ్('g','v','a','r')
#నిర్వచించు HB_OT_ట్యాగ్_GVAR HB_ట్యాగ్('G','V','A','R')

నిర్మాణం hb_glyf_scratch_t
{
  // glyf
  contour_బిందువు_సదిశ_t all_points;
  contour_బిందువు_సదిశ_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_బిందువు_సదిశ_t orig_points;
  hb_సదిశ_t<పూర్ణాంకం> x_deltas;
  hb_సదిశ_t<పూర్ణాంకం> y_deltas;
  contour_బిందువు_సదిశ_t deltas;
  hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> shared_indices;
  hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> వ్యక్తిగత_indices;
};

నామాకాశ OT {

మూస <రకం_పేరు స్థానభ్రంశం_Type>
నిర్మాణం glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  ఉపయోగించి tuple_variations_t = రకం_పేరు Tuple_Variation_దత్తాంశం<స్థానభ్రంశం_Type>::tuple_variations_t;
  ఉపయోగించి Glyph_Variation_దత్తాంశం = Tuple_Variation_దత్తాంశం<స్థానభ్రంశం_Type>;

  hb_సదిశ_t<tuple_variations_t> glyph_variations;

  hb_సదిశ_t<F2DOT14> compiled_shared_tuples;
  అంతరంగ:
  గుర్తులేని shared_tuples_లెక్క = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  బహిరంగ:
  గుర్తులేని compiled_shared_tuples_లెక్క () స్థిరరాశి
  { తిరిగి shared_tuples_లెక్క; }

  గుర్తులేని compiled_byte_కొలత () స్థిరరాశి
  {
    గుర్తులేని byte_కొలత = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : glyph_variations)
      byte_కొలత += _.get_compiled_byte_కొలత ();

    తిరిగి byte_కొలత;
  }

  తార్కిక create_from_glyphs_var_దత్తాంశం (గుర్తులేని axis_లెక్క,
                                    స్థిరరాశి hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
                                    స్థిరరాశి hb_subset_plan_t *plan,
                                    స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_బైట్లు_t>& new_gid_var_దత్తాంశం_map)
  {
    ఒకవేళ (unlikely (!glyph_variations.alloc_exact (plan->new_గమ్యం_old_gid_list.నిడివి)))
      తిరిగి అబద్ధం;

    స్వయంచాలక it = hb_iter (plan->new_గమ్యం_old_gid_list);
    కొరకు (స్వయంచాలక &_ : it)
    {
      hb_codepoint_t new_gid = _.మొదటి;
      contour_బిందువు_సదిశ_t *all_contour_points;
      ఒకవేళ (!new_gid_var_దత్తాంశం_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        తిరిగి అబద్ధం;
      hb_బైట్లు_t var_దత్తాంశం = new_gid_var_దత్తాంశం_map.get (new_gid);

      స్థిరరాశి Glyph_Variation_దత్తాంశం* p = reinterpret_cast<స్థిరరాశి Glyph_Variation_దత్తాంశం*> (var_దత్తాంశం.arrayZ);
      రకం_పేరు Glyph_Variation_దత్తాంశం::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_సదిశ_t<గుర్తులేని> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      ఒకవేళ (!var_దత్తాంశం || ! p->has_దత్తాంశం () || !all_contour_points->నిడివి ||
          !Glyph_Variation_దత్తాంశం::get_tuple_iterator (var_దత్తాంశం, axis_లెక్క,
                                                   var_దత్తాంశం.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        కొనసాగు;
      }

      తార్కిక is_సంయుక్త_glyph = అబద్ధం;
      is_సంయుక్త_glyph = plan->సంయుక్త_new_gids.has (new_gid);

      ఒకవేళ (!p->decompile_tuple_variations (all_contour_points->నిడివి, నిజం /* is_gvar */,
                                          iterator, &(plan->axes_old_సూచీ_ట్యాగ్_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*బహిర్గతం*/
					  &pool,
                                          is_సంయుక్త_glyph))
        తిరిగి అబద్ధం;
      glyph_variations.push (std::move (tuple_vars));
    }
    తిరిగి !glyph_variations.లోపల_error () && glyph_variations.నిడివి == plan->new_గమ్యం_old_gid_list.నిడివి;
  }

  తార్కిక instantiate (స్థిరరాశి hb_subset_plan_t *plan)
  {
    గుర్తులేని లెక్క = plan->new_గమ్యం_old_gid_list.నిడివి;
    తార్కిక iup_optimize = అబద్ధం;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_codepoint_t new_gid = plan->new_గమ్యం_old_gid_list[i].మొదటి;
      contour_బిందువు_సదిశ_t *all_points;
      ఒకవేళ (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        తిరిగి అబద్ధం;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_బిందువు_సదిశ_t points;
      ఒకవేళ (iup_optimize)
      {
        points = *all_points;
        ఒకవేళ (unlikely (points.లోపల_error ())) తిరిగి అబద్ధం;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      ఒకవేళ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      ఒకవేళ (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  తార్కిక compile_బైట్లు (స్థిరరాశి hb_map_t& axes_సూచీ_map,
                      స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map)
  {
    ఒకవేళ (!compile_shared_tuples (axes_సూచీ_map, axes_old_సూచీ_ట్యాగ్_map))
      తిరిగి అబద్ధం;
    కొరకు (tuple_variations_t& vars: glyph_variations)
      ఒకవేళ (!vars.compile_బైట్లు (axes_సూచీ_map, axes_old_సూచీ_ట్యాగ్_map,
                               నిజం, /* use shared points*/
                               నిజం,
                               &shared_tuples_idx_map,
			       &pool))
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక compile_shared_tuples (స్థిరరాశి hb_map_t& axes_సూచీ_map,
                              స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని> coords_లెక్క_map;

    /* count the num of shared coords */
    కొరకు (tuple_variations_t& vars: glyph_variations)
    {
      కొరకు (tuple_delta_t& var : vars.tuple_vars)
      {
        ఒకవేళ (!var.compile_coords (axes_సూచీ_map, axes_old_సూచీ_ట్యాగ్_map, &pool))
          తిరిగి అబద్ధం;
        గుర్తులేని *లెక్క;
	గుర్తులేని hash = hb_hash (&var.compiled_peak_coords);
        ఒకవేళ (coords_లెక్క_map.has_with_hash (&(var.compiled_peak_coords), hash, &లెక్క))
	  (*లెక్క)++;
        లేకపోతే
          coords_లెక్క_map.సమితి_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    ఒకవేళ (!coords_లెక్క_map || coords_లెక్క_map.లోపల_error ())
      తిరిగి అబద్ధం;

    /* add only those coords that are used more than once into the vector and sort */
    hb_సదిశ_t<hb_pair_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని>> shared_coords {
      + hb_iter (coords_లెక్క_map)
      | hb_filter ([] (స్థిరరాశి hb_pair_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని>& p) { తిరిగి p.second > 1; })
    };
    ఒకవేళ (unlikely (shared_coords.లోపల_error ())) తిరిగి అబద్ధం;

    /* no shared tuples: no coords are used more than once */
    ఒకవేళ (!shared_coords) తిరిగి నిజం;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_లెక్క = hb_min (0xFFFu + 1, shared_coords.నిడివి);
    గుర్తులేని len = shared_tuples_లెక్క * (shared_coords[0].మొదటి->నిడివి);
    ఒకవేళ (unlikely (!compiled_shared_tuples.alloc (len)))
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < shared_tuples_లెక్క; i++)
    {
      shared_tuples_idx_map.సమితి (shared_coords[i].మొదటి, i);
      /* add a concat() in hb_vector_t? */
      కొరకు (స్వయంచాలక c : shared_coords[i].మొదటి->iter ())
        compiled_shared_tuples.push (c);
    }

    తిరిగి నిజం;
  }

  స్థిర పూర్ణాంకం _cmp_coords (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి hb_pair_t<hb_సదిశ_t<F2DOT14> *, గుర్తులేని> *a = (స్థిరరాశి hb_pair_t<hb_సదిశ_t<F2DOT14> *, గుర్తులేని> *) pa;
    స్థిరరాశి hb_pair_t<hb_సదిశ_t<F2DOT14> *, గుర్తులేని> *b = (స్థిరరాశి hb_pair_t<hb_సదిశ_t<F2DOT14> *, గుర్తులేని> *) pb;

    ఒకవేళ (a->second != b->second)
      తిరిగి b->second - a->second; // high to low

    తిరిగి b->మొదటి->as_array().cmp (a->మొదటి->as_array ());
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_glyph_var_దత్తాంశం (hb_serialize_context_t *c,
                                 Iterator it,
                                 తార్కిక long_స్థానభ్రంశం,
                                 గుర్తులేని అంకె_glyphs,
                                 అక్షరం* glyph_var_దత్తాంశం_offsets /* OUT: glyph var data offsets array */) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (long_స్థానభ్రంశం)
    {
      ((HBUINT32 *) glyph_var_దత్తాంశం_offsets)[0] = 0;
      glyph_var_దత్తాంశం_offsets += 4;
    }
    లేకపోతే
    {
      ((HBUINT16 *) glyph_var_దత్తాంశం_offsets)[0] = 0;
      glyph_var_దత్తాంశం_offsets += 2;
    }
    గుర్తులేని glyph_స్థానభ్రంశం = 0;
    hb_codepoint_t చివరి_gid = 0;
    గుర్తులేని idx = 0;

    Glyph_Variation_దత్తాంశం* cur_glyph = c->ప్రారంభం_embed<Glyph_Variation_దత్తాంశం> ();
    ఒకవేళ (!cur_glyph) return_trace (అబద్ధం);
    కొరకు (స్వయంచాలక &_ : it)
    {
      hb_codepoint_t gid = _.మొదటి;
      ఒకవేళ (long_స్థానభ్రంశం)
        కొరకు (; చివరి_gid < gid; చివరి_gid++)
          ((HBUINT32 *) glyph_var_దత్తాంశం_offsets)[చివరి_gid] = glyph_స్థానభ్రంశం;
      లేకపోతే
        కొరకు (; చివరి_gid < gid; చివరి_gid++)
          ((HBUINT16 *) glyph_var_దత్తాంశం_offsets)[చివరి_gid] = glyph_స్థానభ్రంశం / 2;

      ఒకవేళ (idx >= glyph_variations.నిడివి) return_trace (అబద్ధం);
      ఒకవేళ (!cur_glyph->serialize (c, నిజం, glyph_variations[idx])) return_trace (అబద్ధం);
      Glyph_Variation_దత్తాంశం* తదుపరి_glyph = c->ప్రారంభం_embed<Glyph_Variation_దత్తాంశం> ();
      glyph_స్థానభ్రంశం += (అక్షరం *) తదుపరి_glyph - (అక్షరం *) cur_glyph;

      ఒకవేళ (long_స్థానభ్రంశం)
        ((HBUINT32 *) glyph_var_దత్తాంశం_offsets)[gid] = glyph_స్థానభ్రంశం;
      లేకపోతే
        ((HBUINT16 *) glyph_var_దత్తాంశం_offsets)[gid] = glyph_స్థానభ్రంశం / 2;

      చివరి_gid++;
      idx++;
      cur_glyph = తదుపరి_glyph;
    }

    ఒకవేళ (long_స్థానభ్రంశం)
      కొరకు (; చివరి_gid < అంకె_glyphs; చివరి_gid++)
        ((HBUINT32 *) glyph_var_దత్తాంశం_offsets)[చివరి_gid] = glyph_స్థానభ్రంశం;
    లేకపోతే
      కొరకు (; చివరి_gid < అంకె_glyphs; చివరి_gid++)
        ((HBUINT16 *) glyph_var_దత్తాంశం_offsets)[చివరి_gid] = glyph_స్థానభ్రంశం / 2;
    return_trace (నిజం);
  }
};

మూస <రకం_పేరు T, రకం_పేరు Types>
నిర్మాణం gvar_GVAR
{
  ఉపయోగించి Glyph_లెక్క_Type = రకం_పేరు Types::HBUINT;

  ఉపయోగించి Glyph_Variation_దత్తాంశం = Tuple_Variation_దత్తాంశం<Glyph_లెక్క_Type>;
  స్థిర constexpr గుర్తులేని glyph_లెక్క_max ()
  { తిరిగి (1u << (8 * Glyph_లెక్క_Type::static_కొలత)) - 1; }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int () != 0; }

  తార్కిక sanitize_shallow (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  (సంస్కరణ.major == 1) &&
		  sharedTuples.sanitize (c, ఇది, axis_లెక్క_2 * shared_Tuple_లెక్క) &&
		  (is_long_స్థానభ్రంశం () ?
		     c->check_array (get_long_స్థానభ్రంశం_array (), c->get_అంకె_glyphs () + 1) :
		     c->check_array (get_short_స్థానభ్రంశం_array (), c->get_అంకె_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  { తిరిగి sanitize_shallow (c); }

  తార్కిక decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_లెక్క_Type>& glyph_vars /*బహిర్గతం*/) స్థిరరాశి
  {
    hb_hashmap_t<hb_codepoint_t, hb_బైట్లు_t> new_gid_var_దత్తాంశం_map;
    స్వయంచాలక it = hb_iter (c->plan->new_గమ్యం_old_gid_list);
    ఒకవేళ (it->మొదటి == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_దత్తాంశం_map.సమితి (0, hb_బైట్లు_t ());
      it++;
    }

    కొరకు (స్వయంచాలక &_ : it)
    {
      hb_codepoint_t new_gid = _.మొదటి;
      hb_codepoint_t old_gid = _.second;
      hb_బైట్లు_t var_దత్తాంశం_బైట్లు = get_glyph_var_దత్తాంశం_బైట్లు (c->ఆధారం_blob, glyph_లెక్క_X, old_gid);
      new_gid_var_దత్తాంశం_map.సమితి (new_gid, var_దత్తాంశం_బైట్లు);
    }

    ఒకవేళ (new_gid_var_దత్తాంశం_map.లోపల_error ()) తిరిగి అబద్ధం;

    hb_array_t<స్థిరరాశి F2DOT14> shared_tuples = (ఇది+sharedTuples).as_array ((గుర్తులేని) shared_Tuple_లెక్క * (గుర్తులేని) axis_లెక్క_2);
    తిరిగి glyph_vars.create_from_glyphs_var_దత్తాంశం (axis_లెక్క_2, shared_tuples, c->plan, new_gid_var_దత్తాంశం_map);
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  స్థిరరాశి glyph_variations_t<Glyph_లెక్క_Type>& glyph_vars,
                  Iterator it,
                  గుర్తులేని axis_లెక్క,
                  గుర్తులేని అంకె_glyphs,
                  తార్కిక force_long_offsets) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    gvar_GVAR *బహిర్గతం = c->allocate_min<gvar_GVAR> ();
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = 1;
    బహిర్గతం->సంస్కరణ.minor = 0;
    బహిర్గతం->axis_లెక్క_2 = axis_లెక్క;
    బహిర్గతం->glyph_లెక్క_X = hb_min (glyph_లెక్క_max (), అంకె_glyphs);

    గుర్తులేని glyph_var_దత్తాంశం_కొలత = glyph_vars.compiled_byte_కొలత ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    తార్కిక long_స్థానభ్రంశం = glyph_var_దత్తాంశం_కొలత > 0x1FFFEu || force_long_offsets;
    బహిర్గతం->flags = long_స్థానభ్రంశం ? 1 : 0;

    గుర్తులేని glyph_var_దత్తాంశం_offsets_కొలత = (long_స్థానభ్రంశం ? 4 : 2) * (అంకె_glyphs + 1);
    HBUINT8 *glyph_var_దత్తాంశం_offsets = c->allocate_కొలత<HBUINT8> (glyph_var_దత్తాంశం_offsets_కొలత, అబద్ధం);
    ఒకవేళ (!glyph_var_దత్తాంశం_offsets) return_trace (అబద్ధం);

    /* shared tuples */
    గుర్తులేని shared_tuple_లెక్క = glyph_vars.compiled_shared_tuples_లెక్క ();
    బహిర్గతం->shared_Tuple_లెక్క = shared_tuple_లెక్క;

    ఒకవేళ (!shared_tuple_లెక్క)
      బహిర్గతం->sharedTuples = (అక్షరం *) glyph_var_దత్తాంశం_offsets + glyph_var_దత్తాంశం_offsets_కొలత - (అక్షరం *) బహిర్గతం;
    లేకపోతే
    {
      hb_array_t<స్థిరరాశి F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      ఒకవేళ (!shared_tuples.arrayZ) return_trace (అబద్ధం);
      బహిర్గతం->sharedTuples = (స్థిరరాశి అక్షరం *) shared_tuples.arrayZ - (అక్షరం *) బహిర్గతం;
    }

    అక్షరం *glyph_var_దత్తాంశం = c->ప్రారంభం_embed<అక్షరం> ();
    ఒకవేళ (!glyph_var_దత్తాంశం) return_trace (అబద్ధం);
    బహిర్గతం->దత్తాంశం_Z = glyph_var_దత్తాంశం - (అక్షరం *) బహిర్గతం;

    return_trace (glyph_vars.serialize_glyph_var_దత్తాంశం (c, it, long_స్థానభ్రంశం, అంకె_glyphs,
                                                       (అక్షరం *) glyph_var_దత్తాంశం_offsets));
  }

  తార్కిక instantiate (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    glyph_variations_t<Glyph_లెక్క_Type> glyph_vars;
    ఒకవేళ (!decompile_glyph_variations (c, glyph_vars))
      return_trace (అబద్ధం);

    ఒకవేళ (!glyph_vars.instantiate (c->plan)) return_trace (అబద్ధం);
    ఒకవేళ (!glyph_vars.compile_బైట్లు (c->plan->axes_సూచీ_map, c->plan->axes_old_సూచీ_ట్యాగ్_map))
      return_trace (అబద్ధం);

    గుర్తులేని axis_లెక్క = c->plan->axes_సూచీ_map.get_population ();
    గుర్తులేని అంకె_glyphs = c->plan->అంకె_output_glyphs ();
    స్వయంచాలక it = hb_iter (c->plan->new_గమ్యం_old_gid_list);

    తార్కిక force_long_offsets = అబద్ధం;
#నిర్వచితమైతే HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#అయితే_ముగింపు
    return_trace (serialize (c->serializer, glyph_vars, it, axis_లెక్క, అంకె_glyphs, force_long_offsets));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (అబద్ధం);

    ఒకవేళ (c->plan->normalized_coords)
      return_trace (instantiate (c));

    గుర్తులేని glyph_లెక్క = సంస్కరణ.గమ్యం_int () ? c->plan->ఆధారం->get_అంకె_glyphs () : 0;

    gvar_GVAR *బహిర్గతం = c->serializer->allocate_min<gvar_GVAR> ();
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = 1;
    బహిర్గతం->సంస్కరణ.minor = 0;
    బహిర్గతం->axis_లెక్క_2 = axis_లెక్క_2;
    బహిర్గతం->shared_Tuple_లెక్క = shared_Tuple_లెక్క;

    గుర్తులేని పూర్ణాంకం అంకె_glyphs = c->plan->అంకె_output_glyphs ();
    బహిర్గతం->glyph_లెక్క_X = hb_min (glyph_లెక్క_max (), అంకె_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_సదిశ_t<hb_సదిశ_t<అక్షరం>> culled_vars;
    hb_సమితి_t culled_gids;
    ఒకవేళ (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<స్థిరరాశి F2DOT14> shared_tuples_array =
	  (ఇది+sharedTuples).as_array ((గుర్తులేని) shared_Tuple_లెక్క * (గుర్తులేని) axis_లెక్క_2);
      ఒకవేళ (unlikely (!culled_vars.resize (అంకె_glyphs))) return_trace (అబద్ధం);
      స్వయంచాలక cull_it = hb_iter (c->plan->new_గమ్యం_old_gid_list);
      ఒకవేళ (cull_it->మొదటి == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      కొరకు (స్వయంచాలక &_ : cull_it)
      {
	hb_బైట్లు_t var_దత్తాంశం_బైట్లు = get_glyph_var_దత్తాంశం_బైట్లు (c->ఆధారం_blob, glyph_లెక్క, _.second);
	ఒకవేళ (var_దత్తాంశం_బైట్లు.నిడివి < Glyph_Variation_దత్తాంశం::min_కొలత) కొనసాగు;
	తార్కిక మారింది = అబద్ధం;
	ఒకవేళ (unlikely (!var_దత్తాంశం_బైట్లు.as<Glyph_Variation_దత్తాంశం> ()
		      ->cull_tuple_variations (var_దత్తాంశం_బైట్లు, axis_లెక్క_2,
					       shared_tuples_array,
					       &c->plan->axes_old_సూచీ_ట్యాగ్_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.మొదటి], &మారింది)))
	  return_trace (అబద్ధం);
	ఒకవేళ (మారింది) culled_gids.కూడు (_.మొదటి);
      }
      ఒకవేళ (unlikely (culled_gids.లోపల_error ())) return_trace (అబద్ధం);
    }

    స్వయంచాలక it = hb_iter (c->plan->new_గమ్యం_old_gid_list);
    ఒకవేళ (it->మొదటి == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    గుర్తులేని subset_దత్తాంశం_కొలత = 0;
    గుర్తులేని padding_కొలత = 0;
    కొరకు (స్వయంచాలక &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      గుర్తులేని glyph_దత్తాంశం_కొలత = culled_gids.has (_.మొదటి)
				 ? culled_vars[_.మొదటి].నిడివి
				 : get_glyph_var_దత్తాంశం_బైట్లు (c->ఆధారం_blob, glyph_లెక్క, old_gid).నిడివి;
      ఒకవేళ (glyph_దత్తాంశం_కొలత % 2)
      {
        glyph_దత్తాంశం_కొలత++;
        padding_కొలత++;
      }

      ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (subset_దత్తాంశం_కొలత, glyph_దత్తాంశం_కొలత, &subset_దత్తాంశం_కొలత)))
	return_trace (అబద్ధం);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    తార్కిక long_స్థానభ్రంశం = subset_దత్తాంశం_కొలత > 0x1FFFEu;
#నిర్వచితమైతే HB_EXPERIMENTAL_API
    long_స్థానభ్రంశం = long_స్థానభ్రంశం || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#అయితే_ముగింపు
    బహిర్గతం->flags = long_స్థానభ్రంశం ? 1 : 0;

    గుర్తులేని subset_offsets_కొలత = (long_స్థానభ్రంశం ? 4 : 2) * (అంకె_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_కొలత<HBUINT8> (subset_offsets_కొలత, అబద్ధం);
    ఒకవేళ (!subset_offsets) return_trace (అబద్ధం);

    /* shared tuples */
    ఒకవేళ (!shared_Tuple_లెక్క || !sharedTuples)
      బహిర్గతం->sharedTuples = (అక్షరం *) subset_offsets + subset_offsets_కొలత - (అక్షరం *) బహిర్గతం;
    లేకపోతే
    {
      గుర్తులేని పూర్ణాంకం shared_tuple_కొలత = F2DOT14::static_కొలత * axis_లెక్క_2 * shared_Tuple_లెక్క;
      F2DOT14 *tuples = c->serializer->allocate_కొలత<F2DOT14> (shared_tuple_కొలత);
      ఒకవేళ (!tuples) return_trace (అబద్ధం);
      బహిర్గతం->sharedTuples = (అక్షరం *) tuples - (అక్షరం *) బహిర్గతం;
      hb_memcpy (tuples, ఇది+sharedTuples, shared_tuple_కొలత);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    ఒకవేళ (long_స్థానభ్రంశం)
      subset_దత్తాంశం_కొలత -= padding_కొలత;
    అక్షరం *subset_దత్తాంశం = c->serializer->allocate_కొలత<అక్షరం> (subset_దత్తాంశం_కొలత, అబద్ధం);
    ఒకవేళ (!subset_దత్తాంశం) return_trace (అబద్ధం);
    బహిర్గతం->దత్తాంశం_Z = subset_దత్తాంశం - (అక్షరం *) బహిర్గతం;


    ఒకవేళ (long_స్థానభ్రంశం)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    లేకపోతే
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    గుర్తులేని పూర్ణాంకం glyph_స్థానభ్రంశం = 0;

    hb_codepoint_t చివరి = 0;
    it = hb_iter (c->plan->new_గమ్యం_old_gid_list);
    ఒకవేళ (it->మొదటి == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    కొరకు (స్వయంచాలక &_ : it)
    {
      hb_codepoint_t gid = _.మొదటి;
      hb_codepoint_t old_gid = _.second;

      ఒకవేళ (long_స్థానభ్రంశం)
	కొరకు (; చివరి < gid; చివరి++)
	  ((HBUINT32 *) subset_offsets)[చివరి] = glyph_స్థానభ్రంశం;
      లేకపోతే
	కొరకు (; చివరి < gid; చివరి++)
	  ((HBUINT16 *) subset_offsets)[చివరి] = glyph_స్థానభ్రంశం / 2;

      hb_బైట్లు_t var_దత్తాంశం_బైట్లు = culled_gids.has (gid)
				  ? hb_బైట్లు_t (culled_vars[gid].arrayZ,
						culled_vars[gid].నిడివి)
				  : get_glyph_var_దత్తాంశం_బైట్లు (c->ఆధారం_blob,
							      glyph_లెక్క,
							      old_gid);

      hb_memcpy (subset_దత్తాంశం, var_దత్తాంశం_బైట్లు.arrayZ, var_దత్తాంశం_బైట్లు.నిడివి);
      గుర్తులేని glyph_దత్తాంశం_కొలత = var_దత్తాంశం_బైట్లు.నిడివి;
      subset_దత్తాంశం += glyph_దత్తాంశం_కొలత;
      glyph_స్థానభ్రంశం += glyph_దత్తాంశం_కొలత;

      ఒకవేళ (!long_స్థానభ్రంశం && (glyph_దత్తాంశం_కొలత % 2))
      {
        *subset_దత్తాంశం = 0;
        subset_దత్తాంశం++;
        glyph_స్థానభ్రంశం++;
      }

      ఒకవేళ (long_స్థానభ్రంశం)
	((HBUINT32 *) subset_offsets)[gid] = glyph_స్థానభ్రంశం;
      లేకపోతే
	((HBUINT16 *) subset_offsets)[gid] = glyph_స్థానభ్రంశం / 2;

      చివరి++; // Skip over gid
    }

    ఒకవేళ (long_స్థానభ్రంశం)
      కొరకు (; చివరి < అంకె_glyphs; చివరి++)
	((HBUINT32 *) subset_offsets)[చివరి] = glyph_స్థానభ్రంశం;
    లేకపోతే
      కొరకు (; చివరి < అంకె_glyphs; చివరి++)
	((HBUINT16 *) subset_offsets)[చివరి] = glyph_స్థానభ్రంశం / 2;

    return_trace (నిజం);
  }

  రక్షిత:
  స్థిరరాశి hb_బైట్లు_t get_glyph_var_దత్తాంశం_బైట్లు (hb_blob_t *blob,
					     గుర్తులేని glyph_లెక్క,
					     hb_codepoint_t glyph) స్థిరరాశి
  {
    గుర్తులేని ప్రారంభం_స్థానభ్రంశం = get_స్థానభ్రంశం (glyph_లెక్క, glyph);
    గుర్తులేని చివర_స్థానభ్రంశం = get_స్థానభ్రంశం (glyph_లెక్క, glyph+1);
    ఒకవేళ (unlikely (చివర_స్థానభ్రంశం < ప్రారంభం_స్థానభ్రంశం)) తిరిగి hb_బైట్లు_t ();
    గుర్తులేని నిడివి = చివర_స్థానభ్రంశం - ప్రారంభం_స్థానభ్రంశం;
    hb_బైట్లు_t var_దత్తాంశం = blob->as_బైట్లు ().sub_array (((గుర్తులేని) దత్తాంశం_Z) + ప్రారంభం_స్థానభ్రంశం, నిడివి);
    తిరిగి likely (var_దత్తాంశం.నిడివి >= Glyph_Variation_దత్తాంశం::min_కొలత) ? var_దత్తాంశం : hb_బైట్లు_t ();
  }

  తార్కిక is_long_స్థానభ్రంశం () స్థిరరాశి { తిరిగి flags & 1; }

  గుర్తులేని get_స్థానభ్రంశం (గుర్తులేని glyph_లెక్క, గుర్తులేని i) స్థిరరాశి
  {
    ఒకవేళ (unlikely (i > glyph_లెక్క)) తిరిగి 0;
    hb_barrier ();
    తిరిగి is_long_స్థానభ్రంశం () ? get_long_స్థానభ్రంశం_array ()[i] : get_short_స్థానభ్రంశం_array ()[i] * 2;
  }

  స్థిరరాశి HBUINT32 * get_long_స్థానభ్రంశం_array () స్థిరరాశి { తిరిగి (స్థిరరాశి HBUINT32 *) &స్థానభ్రంశం_Z; }
  స్థిరరాశి HBUINT16 *get_short_స్థానభ్రంశం_array () స్థిరరాశి { తిరిగి (స్థిరరాశి HBUINT16 *) &స్థానభ్రంశం_Z; }

  బహిరంగ:
  నిర్మాణం accelerator_t
  {

    hb_scalar_cache_t *create_cache () స్థిరరాశి
    {
      తిరిగి hb_scalar_cache_t::create (hb_min ((గుర్తులేని) పట్టిక->shared_Tuple_లెక్క,
						+Tuple_Variation_శీర్షిక::max_shared_tuple_లెక్క));
    }

    స్థిర శూన్య destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక->has_దత్తాంశం (); }

    accelerator_t (hb_face_t *face)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<gvar_GVAR> (face, T::పట్టిక_ట్యాగ్);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_లెక్క_2 = పట్టిక->సంస్కరణ.గమ్యం_int () ? face->get_అంకె_glyphs () : 0;
    }
    ~accelerator_t () { పట్టిక.destroy (); }

    అంతరంగ:

    స్థిర దశాంశం infer_delta (స్థిరరాశి hb_array_t<contour_బిందువు_t> points,
			      స్థిరరాశి hb_array_t<contour_బిందువు_t> deltas,
			      గుర్తులేని పూర్ణాంకం target, గుర్తులేని పూర్ణాంకం prev, గుర్తులేని పూర్ణాంకం next,
			      దశాంశం contour_బిందువు_t::*m)
    {
      దశాంశం లక్ష్యం_val = points.arrayZ[target].*m;
      దశాంశం prev_val = points.arrayZ[prev].*m;
      దశాంశం తదుపరి_val = points.arrayZ[next].*m;
      దశాంశం prev_delta =  deltas.arrayZ[prev].*m;
      దశాంశం తదుపరి_delta =  deltas.arrayZ[next].*m;

      ఒకవేళ (prev_val == తదుపరి_val)
	తిరిగి (prev_delta == తదుపరి_delta) ? prev_delta : 0.f;
      లేకపోతే ఒకవేళ (లక్ష్యం_val <= hb_min (prev_val, తదుపరి_val))
	తిరిగి (prev_val < తదుపరి_val) ? prev_delta : తదుపరి_delta;
      లేకపోతే ఒకవేళ (లక్ష్యం_val >= hb_max (prev_val, తదుపరి_val))
	తిరిగి (prev_val > తదుపరి_val) ? prev_delta : తదుపరి_delta;

      /* linear interpolation */
      దశాంశం r = (లక్ష్యం_val - prev_val) / (తదుపరి_val - prev_val);
      తిరిగి prev_delta + r * (తదుపరి_delta - prev_delta);
    }

    స్థిర గుర్తులేని పూర్ణాంకం తదుపరి_సూచీ (గుర్తులేని పూర్ణాంకం i, గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
    { తిరిగి (i >= చివర) ? ప్రారంభం : (i + 1); }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
    మూస <తార్కిక is_x>
#అయితే_ముగింపు
    స్థిర తార్కిక decompile_deltas_కూడు_గమ్యం_points (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_బిందువు_t> points,
						దశాంశం scalar,
						స్థిరరాశి HBUINT8 *చివర,
						గుర్తులేని ప్రారంభం
#నిర్వచితమైతే HB_OPTIMIZE_కొలత
						, తార్కిక is_x
#అయితే_ముగింపు
						)
    {
      గుర్తులేని i = 0;
      గుర్తులేని లెక్క = points.నిడివి;
      అయితే (i < లెక్క)
      {
	ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
	గుర్తులేని control = *p++;
	గుర్తులేని నిరంతరం_లెక్క = (control & Tuple_విలువలు::విలువ_నిరంతరం_లెక్క_ముసుగు) + 1;
	గుర్తులేని stop = i + నిరంతరం_లెక్క;
	ఒకవేళ (unlikely (stop > లెక్క)) తిరిగి అబద్ధం;

	గుర్తులేని skip = i < ప్రారంభం ? hb_min (ప్రారంభం - i, నిరంతరం_లెక్క) : 0;
	i += skip;

	ఎంపిక (control & Tuple_విలువలు::విలువలు_కొలత_ముసుగు)
	{
	  స్థితి Tuple_విలువలు::విలువలు_ARE_ZEROS:
	    i = stop;
	    ఆపు;
	  స్థితి Tuple_విలువలు::విలువలు_ARE_పదాలు:
	  {
	    ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * HBINT16::static_కొలత > చివర)) తిరిగి అబద్ధం;
	    p += skip * HBINT16::static_కొలత;
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT16 *) p;
	    కొరకు (; i < stop; i++)
	    {
	      దశాంశం v = *pp++ * scalar;
	      ఒకవేళ (is_x) points.arrayZ[i].x += v;
	      లేకపోతే points.arrayZ[i].y += v;
	    }
	    p = (స్థిరరాశి HBUINT8 *) pp;
	  }
	  ఆపు;
	  స్థితి Tuple_విలువలు::విలువలు_ARE_LONGS:
	  {
	    ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * HBINT32::static_కొలత > చివర)) తిరిగి అబద్ధం;
	    p += skip * HBINT32::static_కొలత;
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT32 *) p;
	    కొరకు (; i < stop; i++)
	    {
	      దశాంశం v = *pp++ * scalar;
	      ఒకవేళ (is_x) points.arrayZ[i].x += v;
	      లేకపోతే points.arrayZ[i].y += v;
	    }
	    p = (స్థిరరాశి HBUINT8 *) pp;
	  }
	  ఆపు;
	  స్థితి Tuple_విలువలు::విలువలు_ARE_బైట్లు:
	  {
	    ఒకవేళ (unlikely (p + నిరంతరం_లెక్క > చివర)) తిరిగి అబద్ధం;
	    p += skip * HBINT8::static_కొలత;
	    స్థిరరాశి స్వయంచాలక *pp = (స్థిరరాశి HBINT8 *) p;
	    కొరకు (; i < stop; i++)
	    {
	      దశాంశం v = *pp++ * scalar;
	      ఒకవేళ (is_x) points.arrayZ[i].x += v;
	      లేకపోతే points.arrayZ[i].y += v;
	    }
	    p = (స్థిరరాశి HBUINT8 *) pp;
	  }
	  ఆపు;
	}
      }
      తిరిగి నిజం;
    }
    బహిరంగ:
    తార్కిక apply_deltas_గమ్యం_points (hb_codepoint_t glyph,
				 hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
				 స్థిరరాశి hb_array_t<contour_బిందువు_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
				 తార్కిక phantom_only = అబద్ధం,
				 int64_t *budget = శూన్యసూచిక) స్థిరరాశి
    {
      ఒకవేళ (unlikely (glyph >= glyph_లెక్క_2)) తిరిగి నిజం;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_బైట్లు_t var_దత్తాంశం_బైట్లు = పట్టిక->get_glyph_var_దత్తాంశం_బైట్లు (పట్టిక.get_blob (), glyph_లెక్క_2, glyph);
      ఒకవేళ (!var_దత్తాంశం_బైట్లు.as<Glyph_Variation_దత్తాంశం> ()->has_దత్తాంశం ()) తిరిగి నిజం;

      స్వయంచాలక &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      రకం_పేరు Glyph_Variation_దత్తాంశం::tuple_iterator_t iterator;
      ఒకవేళ (!Glyph_Variation_దత్తాంశం::get_tuple_iterator (var_దత్తాంశం_బైట్లు, పట్టిక->axis_లెక్క_2,
						   var_దత్తాంశం_బైట్లు.arrayZ,
						   shared_indices, &iterator))
	తిరిగి నిజం; /* so isn't applied at all */

      తార్కిక any_వ్యక్తిగత_points = అబద్ధం;
      తార్కిక వ్యక్తిగత_points_checked = అబద్ధం;

      /* Save original points for inferred delta calculation */
      స్వయంచాలక &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      స్వయంచాలక orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      స్వయంచాలక &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      స్వయంచాలక deltas = deltas_vec.as_array ();

      గుర్తులేని అంకె_coords = పట్టిక->axis_లెక్క_2;
      hb_array_t<స్థిరరాశి F2DOT14> shared_tuples = (పట్టిక+పట్టిక->sharedTuples).as_array (పట్టిక->shared_Tuple_లెక్క * అంకె_coords);

      స్వయంచాలక &వ్యక్తిగత_indices = scratch.వ్యక్తిగత_indices;
      స్వయంచాలక &x_deltas = scratch.x_deltas;
      స్వయంచాలక &y_deltas = scratch.y_deltas;

      గుర్తులేని లెక్క = points.నిడివి;
      తార్కిక flush = అబద్ధం;

      చేయి
      {
	దశాంశం scalar = iterator.current_tuple->calculate_scalar (coords, అంకె_coords, shared_tuples,
								 scalar_cache);

	ఒకవేళ (scalar == 0.f) కొనసాగు;

	ఒకవేళ (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && లెక్క >= 4 ? 4 : లెక్క)))
	  తిరిగి అబద్ధం;

	ఒకవేళ (!వ్యక్తిగత_points_checked)
	{
	  స్వయంచాలక పరిశీలన = iterator;
	  చేయి
	  {
	    ఒకవేళ (పరిశీలన.current_tuple->has_వ్యక్తిగత_points ())
	    {
	      any_వ్యక్తిగత_points = నిజం;
	      ఆపు;
	    }
	  } అయితే (పరిశీలన.move_గమ్యం_తదుపరి ());
	  వ్యక్తిగత_points_checked = నిజం;
	}
	స్థిరరాశి HBUINT8 *p = iterator.get_serialized_దత్తాంశం ();
	గుర్తులేని పూర్ణాంకం నిడివి = iterator.current_tuple->get_దత్తాంశం_కొలత ();
	ఒకవేళ (unlikely (!iterator.var_దత్తాంశం_బైట్లు.check_range (p, నిడివి)))
	  తిరిగి అబద్ధం;

	ఒకవేళ (!deltas)
	{
	  ఒకవేళ (unlikely (!deltas_vec.resize_dirty  (లెక్క))) తిరిగి అబద్ధం;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? లెక్క - 4 : 0), 0,
		     (phantom_only ? 4 : లెక్క) * పరిమాణం (deltas[0]));
	}

	స్థిరరాశి HBUINT8 *చివర = p + నిడివి;

	తార్కిక has_వ్యక్తిగత_points = iterator.current_tuple->has_వ్యక్తిగత_points ();
	ఒకవేళ (has_వ్యక్తిగత_points &&
	    !Glyph_Variation_దత్తాంశం::decompile_points (p, వ్యక్తిగత_indices, చివర))
	  తిరిగి అబద్ధం;
	స్థిరరాశి hb_array_t<గుర్తులేని పూర్ణాంకం> &indices = has_వ్యక్తిగత_points ? వ్యక్తిగత_indices : shared_indices;

	తార్కిక apply_గమ్యం_all = (indices.నిడివి == 0);
	గుర్తులేని అంకె_deltas = apply_గమ్యం_all ? points.నిడివి : indices.నిడివి;
	గుర్తులేని ప్రారంభం_deltas = (apply_గమ్యం_all && phantom_only && అంకె_deltas >= 4 ? అంకె_deltas - 4 : 0);

	ఒకవేళ (apply_గమ్యం_all && !any_వ్యక్తిగత_points)
	{
#నిర్వచితమైతే HB_OPTIMIZE_కొలత
	  ఒకవేళ (unlikely (!decompile_deltas_కూడు_గమ్యం_points (p, points, scalar, చివర, ప్రారంభం_deltas, నిజం))) తిరిగి అబద్ధం;
	  ఒకవేళ (unlikely (!decompile_deltas_కూడు_గమ్యం_points (p, points, scalar, చివర, ప్రారంభం_deltas, అబద్ధం))) తిరిగి అబద్ధం;
#లేకపోతే
	  ఒకవేళ (unlikely (!decompile_deltas_కూడు_గమ్యం_points<నిజం> (p, points, scalar, చివర, ప్రారంభం_deltas))) తిరిగి అబద్ధం;
	  ఒకవేళ (unlikely (!decompile_deltas_కూడు_గమ్యం_points<అబద్ధం> (p, points, scalar, చివర, ప్రారంభం_deltas))) తిరిగి అబద్ధం;
#అయితే_ముగింపు
	  కొనసాగు;
	}

	ఒకవేళ (unlikely (!x_deltas.resize_dirty  (అంకె_deltas))) తిరిగి అబద్ధం;
	ఒకవేళ (unlikely (!Glyph_Variation_దత్తాంశం::decompile_deltas (p, x_deltas, చివర, అబద్ధం, ప్రారంభం_deltas))) తిరిగి అబద్ధం;
	ఒకవేళ (unlikely (!y_deltas.resize_dirty  (అంకె_deltas))) తిరిగి అబద్ధం;
	ఒకవేళ (unlikely (!Glyph_Variation_దత్తాంశం::decompile_deltas (p, y_deltas, చివర, అబద్ధం, ప్రారంభం_deltas))) తిరిగి అబద్ధం;

	ఒకవేళ (!apply_గమ్యం_all)
	{
	  ఒకవేళ (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    ఒకవేళ (unlikely (orig_points_vec.లోపల_error ())) తిరిగి అబద్ధం;
	    orig_points = orig_points_vec.as_array ();
	  }

	  ఒకవేళ (flush)
	  {
	    కొరకు (గుర్తులేని పూర్ణాంకం i = phantom_only ? లెక్క - 4 : 0; i < లెక్క; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? లెక్క - 4 : 0), 0,
		     (phantom_only ? 4 : లెక్క) * పరిమాణం (deltas[0]));
	}

	ఒకవేళ (HB_OPTIMIZE_కొలత_VAL)
	{
	  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_deltas; i++)
	  {
	    గుర్తులేని పూర్ణాంకం pt_సూచీ;
	    ఒకవేళ (apply_గమ్యం_all)
	      pt_సూచీ = i;
	    లేకపోతే
	    {
	      pt_సూచీ = indices[i];
	      ఒకవేళ (unlikely (pt_సూచీ >= deltas.నిడివి)) కొనసాగు;
	    }
	    ఒకవేళ (phantom_only && pt_సూచీ < లెక్క - 4) కొనసాగు;
	    స్వయంచాలక &delta = deltas.arrayZ[pt_సూచీ];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.కూడు_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	లేకపోతే
	{
	  /* Ouch. Four cases... for optimization. */
	  ఒకవేళ (scalar != 1.0f)
	  {
	    ఒకవేళ (apply_గమ్యం_all)
	      కొరకు (గుర్తులేని పూర్ణాంకం i = phantom_only ? లెక్క - 4 : 0; i < లెక్క; i++)
	      {
		స్వయంచాలక &delta = deltas.arrayZ[i];
		delta.కూడు_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    లేకపోతే
	      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_deltas; i++)
	      {
		గుర్తులేని పూర్ణాంకం pt_సూచీ = indices[i];
		ఒకవేళ (unlikely (pt_సూచీ >= deltas.నిడివి)) కొనసాగు;
		ఒకవేళ (phantom_only && pt_సూచీ < లెక్క - 4) కొనసాగు;
		స్వయంచాలక &delta = deltas.arrayZ[pt_సూచీ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.కూడు_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  లేకపోతే
	  {
	    ఒకవేళ (apply_గమ్యం_all)
	      కొరకు (గుర్తులేని పూర్ణాంకం i = phantom_only ? లెక్క - 4 : 0; i < లెక్క; i++)
	      {
		స్వయంచాలక &delta = deltas.arrayZ[i];
		delta.కూడు_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    లేకపోతే
	      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_deltas; i++)
	      {
		గుర్తులేని పూర్ణాంకం pt_సూచీ = indices[i];
		ఒకవేళ (unlikely (pt_సూచీ >= deltas.నిడివి)) కొనసాగు;
		ఒకవేళ (phantom_only && pt_సూచీ < లెక్క - 4) కొనసాగు;
		స్వయంచాలక &delta = deltas.arrayZ[pt_సూచీ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.కూడు_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	ఒకవేళ (!apply_గమ్యం_all && !phantom_only)
	{
	  గుర్తులేని ప్రారంభం_బిందువు_2 = 0;
	  గుర్తులేని చివర_బిందువు = 0;
	  అయితే (నిజం)
	  {
	    అయితే (చివర_బిందువు < లెక్క && !points.arrayZ[చివర_బిందువు].is_చివర_బిందువు)
	      చివర_బిందువు++;
	    ఒకవేళ (unlikely (చివర_బిందువు == లెక్క)) ఆపు;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    గుర్తులేని unref_లెక్క = 0;
	    కొరకు (గుర్తులేని i = ప్రారంభం_బిందువు_2; i < చివర_బిందువు + 1; i++)
	      unref_లెక్క += deltas.arrayZ[i].flag;
	    unref_లెక్క = (చివర_బిందువు - ప్రారంభం_బిందువు_2 + 1) - unref_లెక్క;

	    గుర్తులేని j = ప్రారంభం_బిందువు_2;
	    ఒకవేళ (unref_లెక్క == 0 || unref_లెక్క > చివర_బిందువు - ప్రారంభం_బిందువు_2)
	      వెళ్ళు no_మరిన్ని_gaps;

	    కొరకు (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      గుర్తులేని పూర్ణాంకం prev, next, i;
	      కొరకు (;;)
	      {
		i = j;
		j = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
		ఒకవేళ (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) ఆపు;
	      }
	      prev = j = i;
	      కొరకు (;;)
	      {
		i = j;
		j = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
		ఒకవేళ (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) ఆపు;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      కొరకు (;;)
	      {
		i = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
		ఒకవేళ (i == next) ఆపు;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_బిందువు_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_బిందువు_t::y);
		ఒకవేళ (--unref_లెక్క == 0) వెళ్ళు no_మరిన్ని_gaps;
	      }
	    }
	  no_మరిన్ని_gaps:
	    ప్రారంభం_బిందువు_2 = చివర_బిందువు = చివర_బిందువు + 1;
	  }
	}

	flush = నిజం;

      } అయితే (iterator.move_గమ్యం_తదుపరి ());

      ఒకవేళ (flush)
      {
	కొరకు (గుర్తులేని పూర్ణాంకం i = phantom_only ? లెక్క - 4 : 0; i < లెక్క; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      తిరిగి నిజం;
    }

    గుర్తులేని పూర్ణాంకం get_axis_లెక్క () స్థిరరాశి { తిరిగి పట్టిక->axis_లెక్క_2; }

    అంతరంగ:
    hb_blob_ptr_t<gvar_GVAR> పట్టిక;
    గుర్తులేని glyph_లెక్క_2;
  };

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_లెక్క_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_లెక్క;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_లెక్క_Type
		glyph_లెక్క_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_గమ్యం<Glyph_Variation_దత్తాంశం>
		దత్తాంశం_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_యొక్క<HBUINT8>
		స్థానభ్రంశం_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (18 + Glyph_లెక్క_Type::static_కొలత, స్థానభ్రంశం_Z);
};

నిర్మాణం gvar : gvar_GVAR<gvar, SmallTypes>
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_gvar;
};

నిర్మాణం GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_GVAR;
};

నిర్మాణం gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () స్థిరరాశి
  {
    ఒకవేళ (has_దత్తాంశం ()) తిరిగి HB_DUAL_GET (*ఇది, create_cache ());
    తిరిగి శూన్యసూచిక;
  }

  స్థిర శూన్య destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  తార్కిక apply_deltas_గమ్యం_points (hb_codepoint_t glyph,
			       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
			       స్థిరరాశి hb_array_t<contour_బిందువు_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
			       తార్కిక phantom_only = అబద్ధం,
			       int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    తిరిగి HB_DUAL_GET (*ఇది, apply_deltas_గమ్యం_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  గుర్తులేని పూర్ణాంకం get_axis_లెక్క () స్థిరరాశి
  {
    ఒకవేళ (has_దత్తాంశం ()) తిరిగి HB_DUAL_GET (*ఇది, get_axis_లెక్క ());
    తిరిగి 0;
  }
};

} /* namespace OT */

#అయితే_ముగింపు /* HB_OT_VAR_GVAR_TABLE_HH */
