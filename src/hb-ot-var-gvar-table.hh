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

#که_نه_تعریف_شوی HB_OT_VAR_GVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_GVAR_جدول_HH

#ضمن "hb-decycler.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#تعریف_کړه HB_OT_نښان_gvar HB_نښان('g','v','a','r')
#تعریف_کړه HB_OT_نښان_GVAR HB_نښان('G','V','A','R')

بنية hb_glyf_scratch_t
{
  // glyf
  contour_ټکی_ویکټور_t all_points;
  contour_ټکی_ویکټور_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_ټکی_ویکټور_t orig_points;
  hb_ویکټور_t<صحيح> x_deltas;
  hb_ویکټور_t<صحيح> y_deltas;
  contour_ټکی_ویکټور_t deltas;
  hb_ویکټور_t<بینښه صحيح> shared_indices;
  hb_ویکټور_t<بینښه صحيح> شخصي_indices;
};

نطاق OT {

کينډۍ <ډول_نوم دننه_واټن_Type>
بنية glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  کارول tuple_variations_t = ډول_نوم Tuple_Variation_معلومات<دننه_واټن_Type>::tuple_variations_t;
  کارول Glyph_Variation_معلومات = Tuple_Variation_معلومات<دننه_واټن_Type>;

  hb_ویکټور_t<tuple_variations_t> glyph_variations;

  hb_ویکټور_t<F2DOT14> compiled_shared_tuples;
  خاص:
  بینښه shared_tuples_شمېر = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عام:
  بینښه compiled_shared_tuples_شمېر () ثابت
  { أرجع shared_tuples_شمېر; }

  بینښه compiled_byte_اندازه () ثابت
  {
    بینښه byte_اندازه = 0;
    لكل (ثابت خپلکاری& _ : glyph_variations)
      byte_اندازه += _.get_compiled_byte_اندازه ();

    أرجع byte_اندازه;
  }

  منطقي create_from_glyphs_var_معلومات (بینښه axis_شمېر,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_بایټونه_t>& new_gid_var_معلومات_map)
  {
    إذا (unlikely (!glyph_variations.alloc_exact (plan->new_منزل_old_gid_list.اوږدتیا)))
      أرجع خطأ;

    خپلکاری it = hb_iter (plan->new_منزل_old_gid_list);
    لكل (خپلکاری &_ : it)
    {
      hb_codepoint_t new_gid = _.لومړی;
      contour_ټکی_ویکټور_t *all_contour_points;
      إذا (!new_gid_var_معلومات_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        أرجع خطأ;
      hb_بایټونه_t var_معلومات = new_gid_var_معلومات_map.get (new_gid);

      ثابت Glyph_Variation_معلومات* p = reinterpret_cast<ثابت Glyph_Variation_معلومات*> (var_معلومات.arrayZ);
      ډول_نوم Glyph_Variation_معلومات::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_ویکټور_t<بینښه> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      إذا (!var_معلومات || ! p->has_معلومات () || !all_contour_points->اوږدتیا ||
          !Glyph_Variation_معلومات::get_tuple_iterator (var_معلومات, axis_شمېر,
                                                   var_معلومات.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        استمر;
      }

      منطقي is_مرکب_glyph = خطأ;
      is_مرکب_glyph = plan->مرکب_new_gids.has (new_gid);

      إذا (!p->decompile_tuple_variations (all_contour_points->اوږدتیا, ریښتیا /* is_gvar */,
                                          iterator, &(plan->axes_old_شاخص_نښان_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*بهر*/
					  &pool,
                                          is_مرکب_glyph))
        أرجع خطأ;
      glyph_variations.push (std::move (tuple_vars));
    }
    أرجع !glyph_variations.دننه_error () && glyph_variations.اوږدتیا == plan->new_منزل_old_gid_list.اوږدتیا;
  }

  منطقي instantiate (ثابت hb_subset_plan_t *plan)
  {
    بینښه شمېر = plan->new_منزل_old_gid_list.اوږدتیا;
    منطقي iup_optimize = خطأ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      hb_codepoint_t new_gid = plan->new_منزل_old_gid_list[i].لومړی;
      contour_ټکی_ویکټور_t *all_points;
      إذا (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        أرجع خطأ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_ټکی_ویکټور_t points;
      إذا (iup_optimize)
      {
        points = *all_points;
        إذا (unlikely (points.دننه_error ())) أرجع خطأ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      إذا (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        أرجع خطأ;
    }
    أرجع ریښتیا;
  }

  منطقي compile_بایټونه (ثابت hb_map_t& axes_شاخص_map,
                      ثابت hb_map_t& axes_old_شاخص_نښان_map)
  {
    إذا (!compile_shared_tuples (axes_شاخص_map, axes_old_شاخص_نښان_map))
      أرجع خطأ;
    لكل (tuple_variations_t& vars: glyph_variations)
      إذا (!vars.compile_بایټونه (axes_شاخص_map, axes_old_شاخص_نښان_map,
                               ریښتیا, /* use shared points*/
                               ریښتیا,
                               &shared_tuples_idx_map,
			       &pool))
        أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي compile_shared_tuples (ثابت hb_map_t& axes_شاخص_map,
                              ثابت hb_map_t& axes_old_شاخص_نښان_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه> coords_شمېر_map;

    /* count the num of shared coords */
    لكل (tuple_variations_t& vars: glyph_variations)
    {
      لكل (tuple_delta_t& var : vars.tuple_vars)
      {
        إذا (!var.compile_coords (axes_شاخص_map, axes_old_شاخص_نښان_map, &pool))
          أرجع خطأ;
        بینښه *شمېر;
	بینښه hash = hb_hash (&var.compiled_peak_coords);
        إذا (coords_شمېر_map.has_with_hash (&(var.compiled_peak_coords), hash, &شمېر))
	  (*شمېر)++;
        وإلا
          coords_شمېر_map.سېټ_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    إذا (!coords_شمېر_map || coords_شمېر_map.دننه_error ())
      أرجع خطأ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_ویکټور_t<hb_pair_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه>> shared_coords {
      + hb_iter (coords_شمېر_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه>& p) { أرجع p.second > 1; })
    };
    إذا (unlikely (shared_coords.دننه_error ())) أرجع خطأ;

    /* no shared tuples: no coords are used more than once */
    إذا (!shared_coords) أرجع ریښتیا;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_شمېر = hb_min (0xFFFu + 1, shared_coords.اوږدتیا);
    بینښه len = shared_tuples_شمېر * (shared_coords[0].لومړی->اوږدتیا);
    إذا (unlikely (!compiled_shared_tuples.alloc (len)))
      أرجع خطأ;

    لكل (بینښه i = 0; i < shared_tuples_شمېر; i++)
    {
      shared_tuples_idx_map.سېټ (shared_coords[i].لومړی, i);
      /* add a concat() in hb_vector_t? */
      لكل (خپلکاری c : shared_coords[i].لومړی->iter ())
        compiled_shared_tuples.push (c);
    }

    أرجع ریښتیا;
  }

  ساكن صحيح _cmp_coords (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_pair_t<hb_ویکټور_t<F2DOT14> *, بینښه> *a = (ثابت hb_pair_t<hb_ویکټور_t<F2DOT14> *, بینښه> *) pa;
    ثابت hb_pair_t<hb_ویکټور_t<F2DOT14> *, بینښه> *b = (ثابت hb_pair_t<hb_ویکټور_t<F2DOT14> *, بینښه> *) pb;

    إذا (a->second != b->second)
      أرجع b->second - a->second; // high to low

    أرجع b->لومړی->as_array().cmp (a->لومړی->as_array ());
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_glyph_var_معلومات (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقي long_دننه_واټن,
                                 بینښه عدد_glyphs,
                                 حرف* glyph_var_معلومات_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (long_دننه_واټن)
    {
      ((HBUINT32 *) glyph_var_معلومات_offsets)[0] = 0;
      glyph_var_معلومات_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) glyph_var_معلومات_offsets)[0] = 0;
      glyph_var_معلومات_offsets += 2;
    }
    بینښه glyph_دننه_واټن = 0;
    hb_codepoint_t وروستی_gid = 0;
    بینښه idx = 0;

    Glyph_Variation_معلومات* cur_glyph = c->پیل_embed<Glyph_Variation_معلومات> ();
    إذا (!cur_glyph) return_trace (خطأ);
    لكل (خپلکاری &_ : it)
    {
      hb_codepoint_t gid = _.لومړی;
      إذا (long_دننه_واټن)
        لكل (; وروستی_gid < gid; وروستی_gid++)
          ((HBUINT32 *) glyph_var_معلومات_offsets)[وروستی_gid] = glyph_دننه_واټن;
      وإلا
        لكل (; وروستی_gid < gid; وروستی_gid++)
          ((HBUINT16 *) glyph_var_معلومات_offsets)[وروستی_gid] = glyph_دننه_واټن / 2;

      إذا (idx >= glyph_variations.اوږدتیا) return_trace (خطأ);
      إذا (!cur_glyph->serialize (c, ریښتیا, glyph_variations[idx])) return_trace (خطأ);
      Glyph_Variation_معلومات* بل_glyph = c->پیل_embed<Glyph_Variation_معلومات> ();
      glyph_دننه_واټن += (حرف *) بل_glyph - (حرف *) cur_glyph;

      إذا (long_دننه_واټن)
        ((HBUINT32 *) glyph_var_معلومات_offsets)[gid] = glyph_دننه_واټن;
      وإلا
        ((HBUINT16 *) glyph_var_معلومات_offsets)[gid] = glyph_دننه_واټن / 2;

      وروستی_gid++;
      idx++;
      cur_glyph = بل_glyph;
    }

    إذا (long_دننه_واټن)
      لكل (; وروستی_gid < عدد_glyphs; وروستی_gid++)
        ((HBUINT32 *) glyph_var_معلومات_offsets)[وروستی_gid] = glyph_دننه_واټن;
    وإلا
      لكل (; وروستی_gid < عدد_glyphs; وروستی_gid++)
        ((HBUINT16 *) glyph_var_معلومات_offsets)[وروستی_gid] = glyph_دننه_واټن / 2;
    return_trace (ریښتیا);
  }
};

کينډۍ <ډول_نوم T, ډول_نوم Types>
بنية gvar_GVAR
{
  کارول Glyph_شمېر_Type = ډول_نوم Types::HBUINT;

  کارول Glyph_Variation_معلومات = Tuple_Variation_معلومات<Glyph_شمېر_Type>;
  ساكن constexpr بینښه glyph_شمېر_max ()
  { أرجع (1u << (8 * Glyph_شمېر_Type::static_اندازه)) - 1; }

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int () != 0; }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخه.major == 1) &&
		  sharedTuples.sanitize (c, هذا, axis_شمېر_2 * shared_Tuple_شمېر) &&
		  (is_long_دننه_واټن () ?
		     c->check_array (get_long_دننه_واټن_array (), c->get_عدد_glyphs () + 1) :
		     c->check_array (get_short_دننه_واټن_array (), c->get_عدد_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع sanitize_shallow (c); }

  منطقي decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_شمېر_Type>& glyph_vars /*بهر*/) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_بایټونه_t> new_gid_var_معلومات_map;
    خپلکاری it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->لومړی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_معلومات_map.سېټ (0, hb_بایټونه_t ());
      it++;
    }

    لكل (خپلکاری &_ : it)
    {
      hb_codepoint_t new_gid = _.لومړی;
      hb_codepoint_t old_gid = _.second;
      hb_بایټونه_t var_معلومات_بایټونه = get_glyph_var_معلومات_بایټونه (c->سرچینه_blob, glyph_شمېر_X, old_gid);
      new_gid_var_معلومات_map.سېټ (new_gid, var_معلومات_بایټونه);
    }

    إذا (new_gid_var_معلومات_map.دننه_error ()) أرجع خطأ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (هذا+sharedTuples).as_array ((بینښه) shared_Tuple_شمېر * (بینښه) axis_شمېر_2);
    أرجع glyph_vars.create_from_glyphs_var_معلومات (axis_شمېر_2, shared_tuples, c->plan, new_gid_var_معلومات_map);
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_شمېر_Type>& glyph_vars,
                  Iterator it,
                  بینښه axis_شمېر,
                  بینښه عدد_glyphs,
                  منطقي force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (هذا);
    gvar_GVAR *بهر = c->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->نسخه.major = 1;
    بهر->نسخه.minor = 0;
    بهر->axis_شمېر_2 = axis_شمېر;
    بهر->glyph_شمېر_X = hb_min (glyph_شمېر_max (), عدد_glyphs);

    بینښه glyph_var_معلومات_اندازه = glyph_vars.compiled_byte_اندازه ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_دننه_واټن = glyph_var_معلومات_اندازه > 0x1FFFEu || force_long_offsets;
    بهر->flags = long_دننه_واټن ? 1 : 0;

    بینښه glyph_var_معلومات_offsets_اندازه = (long_دننه_واټن ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *glyph_var_معلومات_offsets = c->allocate_اندازه<HBUINT8> (glyph_var_معلومات_offsets_اندازه, خطأ);
    إذا (!glyph_var_معلومات_offsets) return_trace (خطأ);

    /* shared tuples */
    بینښه shared_tuple_شمېر = glyph_vars.compiled_shared_tuples_شمېر ();
    بهر->shared_Tuple_شمېر = shared_tuple_شمېر;

    إذا (!shared_tuple_شمېر)
      بهر->sharedTuples = (حرف *) glyph_var_معلومات_offsets + glyph_var_معلومات_offsets_اندازه - (حرف *) بهر;
    وإلا
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      إذا (!shared_tuples.arrayZ) return_trace (خطأ);
      بهر->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) بهر;
    }

    حرف *glyph_var_معلومات = c->پیل_embed<حرف> ();
    إذا (!glyph_var_معلومات) return_trace (خطأ);
    بهر->معلومات_Z = glyph_var_معلومات - (حرف *) بهر;

    return_trace (glyph_vars.serialize_glyph_var_معلومات (c, it, long_دننه_واټن, عدد_glyphs,
                                                       (حرف *) glyph_var_معلومات_offsets));
  }

  منطقي instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    glyph_variations_t<Glyph_شمېر_Type> glyph_vars;
    إذا (!decompile_glyph_variations (c, glyph_vars))
      return_trace (خطأ);

    إذا (!glyph_vars.instantiate (c->plan)) return_trace (خطأ);
    إذا (!glyph_vars.compile_بایټونه (c->plan->axes_شاخص_map, c->plan->axes_old_شاخص_نښان_map))
      return_trace (خطأ);

    بینښه axis_شمېر = c->plan->axes_شاخص_map.get_population ();
    بینښه عدد_glyphs = c->plan->عدد_output_glyphs ();
    خپلکاری it = hb_iter (c->plan->new_منزل_old_gid_list);

    منطقي force_long_offsets = خطأ;
#که_تعریف_شوی HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#د_که_پای
    return_trace (serialize (c->serializer, glyph_vars, it, axis_شمېر, عدد_glyphs, force_long_offsets));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    إذا (c->plan->normalized_coords)
      return_trace (instantiate (c));

    بینښه glyph_شمېر = نسخه.منزل_int () ? c->plan->سرچینه->get_عدد_glyphs () : 0;

    gvar_GVAR *بهر = c->serializer->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->نسخه.major = 1;
    بهر->نسخه.minor = 0;
    بهر->axis_شمېر_2 = axis_شمېر_2;
    بهر->shared_Tuple_شمېر = shared_Tuple_شمېر;

    بینښه صحيح عدد_glyphs = c->plan->عدد_output_glyphs ();
    بهر->glyph_شمېر_X = hb_min (glyph_شمېر_max (), عدد_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_ویکټور_t<hb_ویکټور_t<حرف>> culled_vars;
    hb_سېټ_t culled_gids;
    إذا (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (هذا+sharedTuples).as_array ((بینښه) shared_Tuple_شمېر * (بینښه) axis_شمېر_2);
      إذا (unlikely (!culled_vars.resize (عدد_glyphs))) return_trace (خطأ);
      خپلکاری cull_it = hb_iter (c->plan->new_منزل_old_gid_list);
      إذا (cull_it->لومړی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      لكل (خپلکاری &_ : cull_it)
      {
	hb_بایټونه_t var_معلومات_بایټونه = get_glyph_var_معلومات_بایټونه (c->سرچینه_blob, glyph_شمېر, _.second);
	إذا (var_معلومات_بایټونه.اوږدتیا < Glyph_Variation_معلومات::min_اندازه) استمر;
	منطقي بدل_شوی = خطأ;
	إذا (unlikely (!var_معلومات_بایټونه.as<Glyph_Variation_معلومات> ()
		      ->cull_tuple_variations (var_معلومات_بایټونه, axis_شمېر_2,
					       shared_tuples_array,
					       &c->plan->axes_old_شاخص_نښان_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.لومړی], &بدل_شوی)))
	  return_trace (خطأ);
	إذا (بدل_شوی) culled_gids.جمع_کړه (_.لومړی);
      }
      إذا (unlikely (culled_gids.دننه_error ())) return_trace (خطأ);
    }

    خپلکاری it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->لومړی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    بینښه subset_معلومات_اندازه = 0;
    بینښه padding_اندازه = 0;
    لكل (خپلکاری &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      بینښه glyph_معلومات_اندازه = culled_gids.has (_.لومړی)
				 ? culled_vars[_.لومړی].اوږدتیا
				 : get_glyph_var_معلومات_بایټونه (c->سرچینه_blob, glyph_شمېر, old_gid).اوږدتیا;
      إذا (glyph_معلومات_اندازه % 2)
      {
        glyph_معلومات_اندازه++;
        padding_اندازه++;
      }

      إذا (unlikely (hb_unsigned_جمع_کړه_overflows (subset_معلومات_اندازه, glyph_معلومات_اندازه, &subset_معلومات_اندازه)))
	return_trace (خطأ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_دننه_واټن = subset_معلومات_اندازه > 0x1FFFEu;
#که_تعریف_شوی HB_EXPERIMENTAL_API
    long_دننه_واټن = long_دننه_واټن || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#د_که_پای
    بهر->flags = long_دننه_واټن ? 1 : 0;

    بینښه subset_offsets_اندازه = (long_دننه_واټن ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_اندازه<HBUINT8> (subset_offsets_اندازه, خطأ);
    إذا (!subset_offsets) return_trace (خطأ);

    /* shared tuples */
    إذا (!shared_Tuple_شمېر || !sharedTuples)
      بهر->sharedTuples = (حرف *) subset_offsets + subset_offsets_اندازه - (حرف *) بهر;
    وإلا
    {
      بینښه صحيح shared_tuple_اندازه = F2DOT14::static_اندازه * axis_شمېر_2 * shared_Tuple_شمېر;
      F2DOT14 *tuples = c->serializer->allocate_اندازه<F2DOT14> (shared_tuple_اندازه);
      إذا (!tuples) return_trace (خطأ);
      بهر->sharedTuples = (حرف *) tuples - (حرف *) بهر;
      hb_memcpy (tuples, هذا+sharedTuples, shared_tuple_اندازه);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    إذا (long_دننه_واټن)
      subset_معلومات_اندازه -= padding_اندازه;
    حرف *subset_معلومات = c->serializer->allocate_اندازه<حرف> (subset_معلومات_اندازه, خطأ);
    إذا (!subset_معلومات) return_trace (خطأ);
    بهر->معلومات_Z = subset_معلومات - (حرف *) بهر;


    إذا (long_دننه_واټن)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    بینښه صحيح glyph_دننه_واټن = 0;

    hb_codepoint_t وروستی = 0;
    it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->لومړی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    لكل (خپلکاری &_ : it)
    {
      hb_codepoint_t gid = _.لومړی;
      hb_codepoint_t old_gid = _.second;

      إذا (long_دننه_واټن)
	لكل (; وروستی < gid; وروستی++)
	  ((HBUINT32 *) subset_offsets)[وروستی] = glyph_دننه_واټن;
      وإلا
	لكل (; وروستی < gid; وروستی++)
	  ((HBUINT16 *) subset_offsets)[وروستی] = glyph_دننه_واټن / 2;

      hb_بایټونه_t var_معلومات_بایټونه = culled_gids.has (gid)
				  ? hb_بایټونه_t (culled_vars[gid].arrayZ,
						culled_vars[gid].اوږدتیا)
				  : get_glyph_var_معلومات_بایټونه (c->سرچینه_blob,
							      glyph_شمېر,
							      old_gid);

      hb_memcpy (subset_معلومات, var_معلومات_بایټونه.arrayZ, var_معلومات_بایټونه.اوږدتیا);
      بینښه glyph_معلومات_اندازه = var_معلومات_بایټونه.اوږدتیا;
      subset_معلومات += glyph_معلومات_اندازه;
      glyph_دننه_واټن += glyph_معلومات_اندازه;

      إذا (!long_دننه_واټن && (glyph_معلومات_اندازه % 2))
      {
        *subset_معلومات = 0;
        subset_معلومات++;
        glyph_دننه_واټن++;
      }

      إذا (long_دننه_واټن)
	((HBUINT32 *) subset_offsets)[gid] = glyph_دننه_واټن;
      وإلا
	((HBUINT16 *) subset_offsets)[gid] = glyph_دننه_واټن / 2;

      وروستی++; // Skip over gid
    }

    إذا (long_دننه_واټن)
      لكل (; وروستی < عدد_glyphs; وروستی++)
	((HBUINT32 *) subset_offsets)[وروستی] = glyph_دننه_واټن;
    وإلا
      لكل (; وروستی < عدد_glyphs; وروستی++)
	((HBUINT16 *) subset_offsets)[وروستی] = glyph_دننه_واټن / 2;

    return_trace (ریښتیا);
  }

  محمي:
  ثابت hb_بایټونه_t get_glyph_var_معلومات_بایټونه (hb_blob_t *blob,
					     بینښه glyph_شمېر,
					     hb_codepoint_t glyph) ثابت
  {
    بینښه پیل_دننه_واټن = get_دننه_واټن (glyph_شمېر, glyph);
    بینښه پای_ټکی_دننه_واټن = get_دننه_واټن (glyph_شمېر, glyph+1);
    إذا (unlikely (پای_ټکی_دننه_واټن < پیل_دننه_واټن)) أرجع hb_بایټونه_t ();
    بینښه اوږدتیا = پای_ټکی_دننه_واټن - پیل_دننه_واټن;
    hb_بایټونه_t var_معلومات = blob->as_بایټونه ().sub_array (((بینښه) معلومات_Z) + پیل_دننه_واټن, اوږدتیا);
    أرجع likely (var_معلومات.اوږدتیا >= Glyph_Variation_معلومات::min_اندازه) ? var_معلومات : hb_بایټونه_t ();
  }

  منطقي is_long_دننه_واټن () ثابت { أرجع flags & 1; }

  بینښه get_دننه_واټن (بینښه glyph_شمېر, بینښه i) ثابت
  {
    إذا (unlikely (i > glyph_شمېر)) أرجع 0;
    hb_barrier ();
    أرجع is_long_دننه_واټن () ? get_long_دننه_واټن_array ()[i] : get_short_دننه_واټن_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_دننه_واټن_array () ثابت { أرجع (ثابت HBUINT32 *) &دننه_واټن_Z; }
  ثابت HBUINT16 *get_short_دننه_واټن_array () ثابت { أرجع (ثابت HBUINT16 *) &دننه_واټن_Z; }

  عام:
  بنية accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      أرجع hb_scalar_cache_t::create (hb_min ((بینښه) جدول->shared_Tuple_شمېر,
						+Tuple_Variation_سرلیک::max_shared_tuple_شمېر));
    }

    ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<gvar_GVAR> (face, T::جدول_نښان);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_شمېر_2 = جدول->نسخه.منزل_int () ? face->get_عدد_glyphs () : 0;
    }
    ~accelerator_t () { جدول.destroy (); }

    خاص:

    ساكن عائم infer_delta (ثابت hb_array_t<contour_ټکی_t> points,
			      ثابت hb_array_t<contour_ټکی_t> deltas,
			      بینښه صحيح target, بینښه صحيح prev, بینښه صحيح next,
			      عائم contour_ټکی_t::*m)
    {
      عائم موخه_val = points.arrayZ[target].*m;
      عائم prev_val = points.arrayZ[prev].*m;
      عائم بل_val = points.arrayZ[next].*m;
      عائم prev_delta =  deltas.arrayZ[prev].*m;
      عائم بل_delta =  deltas.arrayZ[next].*m;

      إذا (prev_val == بل_val)
	أرجع (prev_delta == بل_delta) ? prev_delta : 0.f;
      وإلا إذا (موخه_val <= hb_min (prev_val, بل_val))
	أرجع (prev_val < بل_val) ? prev_delta : بل_delta;
      وإلا إذا (موخه_val >= hb_max (prev_val, بل_val))
	أرجع (prev_val > بل_val) ? prev_delta : بل_delta;

      /* linear interpolation */
      عائم r = (موخه_val - prev_val) / (بل_val - prev_val);
      أرجع prev_delta + r * (بل_delta - prev_delta);
    }

    ساكن بینښه صحيح بل_شاخص (بینښه صحيح i, بینښه صحيح پیل, بینښه صحيح پای_ټکی)
    { أرجع (i >= پای_ټکی) ? پیل : (i + 1); }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
    کينډۍ <منطقي is_x>
#د_که_پای
    ساكن منطقي decompile_deltas_جمع_کړه_منزل_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_ټکی_t> points,
						عائم scalar,
						ثابت HBUINT8 *پای_ټکی,
						بینښه پیل
#که_تعریف_شوی HB_OPTIMIZE_اندازه
						, منطقي is_x
#د_که_پای
						)
    {
      بینښه i = 0;
      بینښه شمېر = points.اوږدتیا;
      طالما (i < شمېر)
      {
	إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
	بینښه control = *p++;
	بینښه پرلپسې_شمېر = (control & Tuple_ارزښتونه::ارزښت_پرلپسې_شمېر_ماسک) + 1;
	بینښه stop = i + پرلپسې_شمېر;
	إذا (unlikely (stop > شمېر)) أرجع خطأ;

	بینښه skip = i < پیل ? hb_min (پیل - i, پرلپسې_شمېر) : 0;
	i += skip;

	بدل (control & Tuple_ارزښتونه::ارزښتونه_اندازه_ماسک)
	{
	  حالة Tuple_ارزښتونه::ارزښتونه_ARE_ZEROS:
	    i = stop;
	    اكسر;
	  حالة Tuple_ارزښتونه::ارزښتونه_ARE_ټکي:
	  {
	    إذا (unlikely (p + پرلپسې_شمېر * HBINT16::static_اندازه > پای_ټکی)) أرجع خطأ;
	    p += skip * HBINT16::static_اندازه;
	    ثابت خپلکاری *pp = (ثابت HBINT16 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_ارزښتونه::ارزښتونه_ARE_LONGS:
	  {
	    إذا (unlikely (p + پرلپسې_شمېر * HBINT32::static_اندازه > پای_ټکی)) أرجع خطأ;
	    p += skip * HBINT32::static_اندازه;
	    ثابت خپلکاری *pp = (ثابت HBINT32 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_ارزښتونه::ارزښتونه_ARE_بایټونه:
	  {
	    إذا (unlikely (p + پرلپسې_شمېر > پای_ټکی)) أرجع خطأ;
	    p += skip * HBINT8::static_اندازه;
	    ثابت خپلکاری *pp = (ثابت HBINT8 *) p;
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
      أرجع ریښتیا;
    }
    عام:
    منطقي apply_deltas_منزل_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحيح> coords,
				 ثابت hb_array_t<contour_ټکی_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = تش_اشاره,
				 منطقي phantom_only = خطأ,
				 int64_t *budget = تش_اشاره) ثابت
    {
      إذا (unlikely (glyph >= glyph_شمېر_2)) أرجع ریښتیا;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_بایټونه_t var_معلومات_بایټونه = جدول->get_glyph_var_معلومات_بایټونه (جدول.get_blob (), glyph_شمېر_2, glyph);
      إذا (!var_معلومات_بایټونه.as<Glyph_Variation_معلومات> ()->has_معلومات ()) أرجع ریښتیا;

      خپلکاری &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      ډول_نوم Glyph_Variation_معلومات::tuple_iterator_t iterator;
      إذا (!Glyph_Variation_معلومات::get_tuple_iterator (var_معلومات_بایټونه, جدول->axis_شمېر_2,
						   var_معلومات_بایټونه.arrayZ,
						   shared_indices, &iterator))
	أرجع ریښتیا; /* so isn't applied at all */

      منطقي any_شخصي_points = خطأ;
      منطقي شخصي_points_checked = خطأ;

      /* Save original points for inferred delta calculation */
      خپلکاری &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      خپلکاری orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      خپلکاری &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      خپلکاری deltas = deltas_vec.as_array ();

      بینښه عدد_coords = جدول->axis_شمېر_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (جدول+جدول->sharedTuples).as_array (جدول->shared_Tuple_شمېر * عدد_coords);

      خپلکاری &شخصي_indices = scratch.شخصي_indices;
      خپلکاری &x_deltas = scratch.x_deltas;
      خپلکاری &y_deltas = scratch.y_deltas;

      بینښه شمېر = points.اوږدتیا;
      منطقي flush = خطأ;

      افعل
      {
	عائم scalar = iterator.current_tuple->calculate_scalar (coords, عدد_coords, shared_tuples,
								 scalar_cache);

	إذا (scalar == 0.f) استمر;

	إذا (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && شمېر >= 4 ? 4 : شمېر)))
	  أرجع خطأ;

	إذا (!شخصي_points_checked)
	{
	  خپلکاری لټون = iterator;
	  افعل
	  {
	    إذا (لټون.current_tuple->has_شخصي_points ())
	    {
	      any_شخصي_points = ریښتیا;
	      اكسر;
	    }
	  } طالما (لټون.move_منزل_بل ());
	  شخصي_points_checked = ریښتیا;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
	بینښه صحيح اوږدتیا = iterator.current_tuple->get_معلومات_اندازه ();
	إذا (unlikely (!iterator.var_معلومات_بایټونه.check_range (p, اوږدتیا)))
	  أرجع خطأ;

	إذا (!deltas)
	{
	  إذا (unlikely (!deltas_vec.resize_dirty  (شمېر))) أرجع خطأ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? شمېر - 4 : 0), 0,
		     (phantom_only ? 4 : شمېر) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *پای_ټکی = p + اوږدتیا;

	منطقي has_شخصي_points = iterator.current_tuple->has_شخصي_points ();
	إذا (has_شخصي_points &&
	    !Glyph_Variation_معلومات::decompile_points (p, شخصي_indices, پای_ټکی))
	  أرجع خطأ;
	ثابت hb_array_t<بینښه صحيح> &indices = has_شخصي_points ? شخصي_indices : shared_indices;

	منطقي apply_منزل_all = (indices.اوږدتیا == 0);
	بینښه عدد_deltas = apply_منزل_all ? points.اوږدتیا : indices.اوږدتیا;
	بینښه پیل_deltas = (apply_منزل_all && phantom_only && عدد_deltas >= 4 ? عدد_deltas - 4 : 0);

	إذا (apply_منزل_all && !any_شخصي_points)
	{
#که_تعریف_شوی HB_OPTIMIZE_اندازه
	  إذا (unlikely (!decompile_deltas_جمع_کړه_منزل_points (p, points, scalar, پای_ټکی, پیل_deltas, ریښتیا))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جمع_کړه_منزل_points (p, points, scalar, پای_ټکی, پیل_deltas, خطأ))) أرجع خطأ;
#وإلا
	  إذا (unlikely (!decompile_deltas_جمع_کړه_منزل_points<ریښتیا> (p, points, scalar, پای_ټکی, پیل_deltas))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جمع_کړه_منزل_points<خطأ> (p, points, scalar, پای_ټکی, پیل_deltas))) أرجع خطأ;
#د_که_پای
	  استمر;
	}

	إذا (unlikely (!x_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_معلومات::decompile_deltas (p, x_deltas, پای_ټکی, خطأ, پیل_deltas))) أرجع خطأ;
	إذا (unlikely (!y_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_معلومات::decompile_deltas (p, y_deltas, پای_ټکی, خطأ, پیل_deltas))) أرجع خطأ;

	إذا (!apply_منزل_all)
	{
	  إذا (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    إذا (unlikely (orig_points_vec.دننه_error ())) أرجع خطأ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  إذا (flush)
	  {
	    لكل (بینښه صحيح i = phantom_only ? شمېر - 4 : 0; i < شمېر; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? شمېر - 4 : 0), 0,
		     (phantom_only ? 4 : شمېر) * حجم (deltas[0]));
	}

	إذا (HB_OPTIMIZE_اندازه_VAL)
	{
	  لكل (بینښه صحيح i = 0; i < عدد_deltas; i++)
	  {
	    بینښه صحيح pt_شاخص;
	    إذا (apply_منزل_all)
	      pt_شاخص = i;
	    وإلا
	    {
	      pt_شاخص = indices[i];
	      إذا (unlikely (pt_شاخص >= deltas.اوږدتیا)) استمر;
	    }
	    إذا (phantom_only && pt_شاخص < شمېر - 4) استمر;
	    خپلکاری &delta = deltas.arrayZ[pt_شاخص];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.جمع_کړه_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وإلا
	{
	  /* Ouch. Four cases... for optimization. */
	  إذا (scalar != 1.0f)
	  {
	    إذا (apply_منزل_all)
	      لكل (بینښه صحيح i = phantom_only ? شمېر - 4 : 0; i < شمېر; i++)
	      {
		خپلکاری &delta = deltas.arrayZ[i];
		delta.جمع_کړه_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وإلا
	      لكل (بینښه صحيح i = 0; i < عدد_deltas; i++)
	      {
		بینښه صحيح pt_شاخص = indices[i];
		إذا (unlikely (pt_شاخص >= deltas.اوږدتیا)) استمر;
		إذا (phantom_only && pt_شاخص < شمېر - 4) استمر;
		خپلکاری &delta = deltas.arrayZ[pt_شاخص];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جمع_کړه_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وإلا
	  {
	    إذا (apply_منزل_all)
	      لكل (بینښه صحيح i = phantom_only ? شمېر - 4 : 0; i < شمېر; i++)
	      {
		خپلکاری &delta = deltas.arrayZ[i];
		delta.جمع_کړه_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وإلا
	      لكل (بینښه صحيح i = 0; i < عدد_deltas; i++)
	      {
		بینښه صحيح pt_شاخص = indices[i];
		إذا (unlikely (pt_شاخص >= deltas.اوږدتیا)) استمر;
		إذا (phantom_only && pt_شاخص < شمېر - 4) استمر;
		خپلکاری &delta = deltas.arrayZ[pt_شاخص];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جمع_کړه_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	إذا (!apply_منزل_all && !phantom_only)
	{
	  بینښه پیل_ټکی_2 = 0;
	  بینښه پای_ټکی_ټکی = 0;
	  طالما (ریښتیا)
	  {
	    طالما (پای_ټکی_ټکی < شمېر && !points.arrayZ[پای_ټکی_ټکی].is_پای_ټکی_ټکی)
	      پای_ټکی_ټکی++;
	    إذا (unlikely (پای_ټکی_ټکی == شمېر)) اكسر;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    بینښه unref_شمېر = 0;
	    لكل (بینښه i = پیل_ټکی_2; i < پای_ټکی_ټکی + 1; i++)
	      unref_شمېر += deltas.arrayZ[i].flag;
	    unref_شمېر = (پای_ټکی_ټکی - پیل_ټکی_2 + 1) - unref_شمېر;

	    بینښه j = پیل_ټکی_2;
	    إذا (unref_شمېر == 0 || unref_شمېر > پای_ټکی_ټکی - پیل_ټکی_2)
	      اذهب no_نور_gaps;

	    لكل (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      بینښه صحيح prev, next, i;
	      لكل (;;)
	      {
		i = j;
		j = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
		إذا (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) اكسر;
	      }
	      prev = j = i;
	      لكل (;;)
	      {
		i = j;
		j = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
		إذا (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) اكسر;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      لكل (;;)
	      {
		i = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
		إذا (i == next) اكسر;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_ټکی_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_ټکی_t::y);
		إذا (--unref_شمېر == 0) اذهب no_نور_gaps;
	      }
	    }
	  no_نور_gaps:
	    پیل_ټکی_2 = پای_ټکی_ټکی = پای_ټکی_ټکی + 1;
	  }
	}

	flush = ریښتیا;

      } طالما (iterator.move_منزل_بل ());

      إذا (flush)
      {
	لكل (بینښه صحيح i = phantom_only ? شمېر - 4 : 0; i < شمېر; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      أرجع ریښتیا;
    }

    بینښه صحيح get_axis_شمېر () ثابت { أرجع جدول->axis_شمېر_2; }

    خاص:
    hb_blob_ptr_t<gvar_GVAR> جدول;
    بینښه glyph_شمېر_2;
  };

  محمي:
  Fixed_نسخه<>نسخه;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_شمېر_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_شمېر;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_منزل<Unsized_Array_د<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_شمېر_Type
		glyph_شمېر_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_منزل<Glyph_Variation_معلومات>
		معلومات_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_د<HBUINT8>
		دننه_واټن_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  عام:
  DEFINE_اندازه_ARRAY (18 + Glyph_شمېر_Type::static_اندازه, دننه_واټن_Z);
};

بنية gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_gvar;
};

بنية GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_GVAR;
};

بنية gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    إذا (has_معلومات ()) أرجع HB_DUAL_GET (*هذا, create_cache ());
    أرجع تش_اشاره;
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقي apply_deltas_منزل_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحيح> coords,
			       ثابت hb_array_t<contour_ټکی_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = تش_اشاره,
			       منطقي phantom_only = خطأ,
			       int64_t *budget = تش_اشاره) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, apply_deltas_منزل_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  بینښه صحيح get_axis_شمېر () ثابت
  {
    إذا (has_معلومات ()) أرجع HB_DUAL_GET (*هذا, get_axis_شمېر ());
    أرجع 0;
  }
};

} /* namespace OT */

#د_که_پای /* HB_OT_VAR_GVAR_TABLE_HH */
