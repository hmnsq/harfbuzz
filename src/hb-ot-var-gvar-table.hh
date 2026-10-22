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

#اگر_تعریف_نشده HB_OT_VAR_GVAR_جدول_HH
#تعریف_کن HB_OT_VAR_GVAR_جدول_HH

#ضمن "hb-decycler.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#تعریف_کن HB_OT_برچسب_gvar HB_برچسب('g','v','a','r')
#تعریف_کن HB_OT_برچسب_GVAR HB_برچسب('G','V','A','R')

بنية hb_glyf_scratch_t
{
  // glyf
  contour_نقطه_وکتور_t all_points;
  contour_نقطه_وکتور_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_نقطه_وکتور_t orig_points;
  hb_وکتور_t<صحيح> x_deltas;
  hb_وکتور_t<صحيح> y_deltas;
  contour_نقطه_وکتور_t deltas;
  hb_وکتور_t<بیعلامت صحيح> shared_indices;
  hb_وکتور_t<بیعلامت صحيح> خصوصی_indices;
};

نطاق OT {

قالب <نام_نوع جابه_جایی_Type>
بنية glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  استفاده tuple_variations_t = نام_نوع Tuple_Variation_داده<جابه_جایی_Type>::tuple_variations_t;
  استفاده Glyph_Variation_داده = Tuple_Variation_داده<جابه_جایی_Type>;

  hb_وکتور_t<tuple_variations_t> glyph_variations;

  hb_وکتور_t<F2DOT14> compiled_shared_tuples;
  خاص:
  بیعلامت shared_tuples_شمار = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عام:
  بیعلامت compiled_shared_tuples_شمار () ثابت
  { أرجع shared_tuples_شمار; }

  بیعلامت compiled_byte_اندازه () ثابت
  {
    بیعلامت byte_اندازه = 0;
    لكل (ثابت خودکار& _ : glyph_variations)
      byte_اندازه += _.get_compiled_byte_اندازه ();

    أرجع byte_اندازه;
  }

  منطقي create_from_glyphs_var_داده (بیعلامت axis_شمار,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_بایتها_t>& new_gid_var_داده_map)
  {
    إذا (unlikely (!glyph_variations.alloc_exact (plan->new_مقصد_old_gid_list.درازی)))
      أرجع خطأ;

    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.اولی;
      contour_نقطه_وکتور_t *all_contour_points;
      إذا (!new_gid_var_داده_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        أرجع خطأ;
      hb_بایتها_t var_داده = new_gid_var_داده_map.get (new_gid);

      ثابت Glyph_Variation_داده* p = reinterpret_cast<ثابت Glyph_Variation_داده*> (var_داده.arrayZ);
      نام_نوع Glyph_Variation_داده::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_وکتور_t<بیعلامت> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      إذا (!var_داده || ! p->has_داده () || !all_contour_points->درازی ||
          !Glyph_Variation_داده::get_tuple_iterator (var_داده, axis_شمار,
                                                   var_داده.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        استمر;
      }

      منطقي is_مرکب_glyph = خطأ;
      is_مرکب_glyph = plan->مرکب_new_gids.has (new_gid);

      إذا (!p->decompile_tuple_variations (all_contour_points->درازی, درست /* is_gvar */,
                                          iterator, &(plan->axes_old_شاخص_برچسب_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*بیرون*/
					  &pool,
                                          is_مرکب_glyph))
        أرجع خطأ;
      glyph_variations.push (std::move (tuple_vars));
    }
    أرجع !glyph_variations.درون_error () && glyph_variations.درازی == plan->new_مقصد_old_gid_list.درازی;
  }

  منطقي instantiate (ثابت hb_subset_plan_t *plan)
  {
    بیعلامت شمار = plan->new_مقصد_old_gid_list.درازی;
    منطقي iup_optimize = خطأ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_codepoint_t new_gid = plan->new_مقصد_old_gid_list[i].اولی;
      contour_نقطه_وکتور_t *all_points;
      إذا (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        أرجع خطأ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_نقطه_وکتور_t points;
      إذا (iup_optimize)
      {
        points = *all_points;
        إذا (unlikely (points.درون_error ())) أرجع خطأ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      إذا (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        أرجع خطأ;
    }
    أرجع درست;
  }

  منطقي compile_بایتها (ثابت hb_map_t& axes_شاخص_map,
                      ثابت hb_map_t& axes_old_شاخص_برچسب_map)
  {
    إذا (!compile_shared_tuples (axes_شاخص_map, axes_old_شاخص_برچسب_map))
      أرجع خطأ;
    لكل (tuple_variations_t& vars: glyph_variations)
      إذا (!vars.compile_بایتها (axes_شاخص_map, axes_old_شاخص_برچسب_map,
                               درست, /* use shared points*/
                               درست,
                               &shared_tuples_idx_map,
			       &pool))
        أرجع خطأ;

    أرجع درست;
  }

  منطقي compile_shared_tuples (ثابت hb_map_t& axes_شاخص_map,
                              ثابت hb_map_t& axes_old_شاخص_برچسب_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت> coords_شمار_map;

    /* count the num of shared coords */
    لكل (tuple_variations_t& vars: glyph_variations)
    {
      لكل (tuple_delta_t& var : vars.tuple_vars)
      {
        إذا (!var.compile_coords (axes_شاخص_map, axes_old_شاخص_برچسب_map, &pool))
          أرجع خطأ;
        بیعلامت *شمار;
	بیعلامت hash = hb_hash (&var.compiled_peak_coords);
        إذا (coords_شمار_map.has_with_hash (&(var.compiled_peak_coords), hash, &شمار))
	  (*شمار)++;
        وإلا
          coords_شمار_map.مجموعه_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    إذا (!coords_شمار_map || coords_شمار_map.درون_error ())
      أرجع خطأ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_وکتور_t<hb_pair_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت>> shared_coords {
      + hb_iter (coords_شمار_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت>& p) { أرجع p.second > 1; })
    };
    إذا (unlikely (shared_coords.درون_error ())) أرجع خطأ;

    /* no shared tuples: no coords are used more than once */
    إذا (!shared_coords) أرجع درست;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_شمار = hb_min (0xFFFu + 1, shared_coords.درازی);
    بیعلامت len = shared_tuples_شمار * (shared_coords[0].اولی->درازی);
    إذا (unlikely (!compiled_shared_tuples.alloc (len)))
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < shared_tuples_شمار; i++)
    {
      shared_tuples_idx_map.مجموعه (shared_coords[i].اولی, i);
      /* add a concat() in hb_vector_t? */
      لكل (خودکار c : shared_coords[i].اولی->iter ())
        compiled_shared_tuples.push (c);
    }

    أرجع درست;
  }

  ساكن صحيح _cmp_coords (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_pair_t<hb_وکتور_t<F2DOT14> *, بیعلامت> *a = (ثابت hb_pair_t<hb_وکتور_t<F2DOT14> *, بیعلامت> *) pa;
    ثابت hb_pair_t<hb_وکتور_t<F2DOT14> *, بیعلامت> *b = (ثابت hb_pair_t<hb_وکتور_t<F2DOT14> *, بیعلامت> *) pb;

    إذا (a->second != b->second)
      أرجع b->second - a->second; // high to low

    أرجع b->اولی->as_array().cmp (a->اولی->as_array ());
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_glyph_var_داده (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقي long_جابه_جایی,
                                 بیعلامت رقم_glyphs,
                                 حرف* glyph_var_داده_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (long_جابه_جایی)
    {
      ((HBUINT32 *) glyph_var_داده_offsets)[0] = 0;
      glyph_var_داده_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) glyph_var_داده_offsets)[0] = 0;
      glyph_var_داده_offsets += 2;
    }
    بیعلامت glyph_جابه_جایی = 0;
    hb_codepoint_t آخرین_gid = 0;
    بیعلامت idx = 0;

    Glyph_Variation_داده* cur_glyph = c->شروع_embed<Glyph_Variation_داده> ();
    إذا (!cur_glyph) return_trace (خطأ);
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.اولی;
      إذا (long_جابه_جایی)
        لكل (; آخرین_gid < gid; آخرین_gid++)
          ((HBUINT32 *) glyph_var_داده_offsets)[آخرین_gid] = glyph_جابه_جایی;
      وإلا
        لكل (; آخرین_gid < gid; آخرین_gid++)
          ((HBUINT16 *) glyph_var_داده_offsets)[آخرین_gid] = glyph_جابه_جایی / 2;

      إذا (idx >= glyph_variations.درازی) return_trace (خطأ);
      إذا (!cur_glyph->serialize (c, درست, glyph_variations[idx])) return_trace (خطأ);
      Glyph_Variation_داده* بعدی_glyph = c->شروع_embed<Glyph_Variation_داده> ();
      glyph_جابه_جایی += (حرف *) بعدی_glyph - (حرف *) cur_glyph;

      إذا (long_جابه_جایی)
        ((HBUINT32 *) glyph_var_داده_offsets)[gid] = glyph_جابه_جایی;
      وإلا
        ((HBUINT16 *) glyph_var_داده_offsets)[gid] = glyph_جابه_جایی / 2;

      آخرین_gid++;
      idx++;
      cur_glyph = بعدی_glyph;
    }

    إذا (long_جابه_جایی)
      لكل (; آخرین_gid < رقم_glyphs; آخرین_gid++)
        ((HBUINT32 *) glyph_var_داده_offsets)[آخرین_gid] = glyph_جابه_جایی;
    وإلا
      لكل (; آخرین_gid < رقم_glyphs; آخرین_gid++)
        ((HBUINT16 *) glyph_var_داده_offsets)[آخرین_gid] = glyph_جابه_جایی / 2;
    return_trace (درست);
  }
};

قالب <نام_نوع T, نام_نوع Types>
بنية gvar_GVAR
{
  استفاده Glyph_شمار_Type = نام_نوع Types::HBUINT;

  استفاده Glyph_Variation_داده = Tuple_Variation_داده<Glyph_شمار_Type>;
  ساكن constexpr بیعلامت glyph_شمار_max ()
  { أرجع (1u << (8 * Glyph_شمار_Type::static_اندازه)) - 1; }

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int () != 0; }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخه.major == 1) &&
		  sharedTuples.sanitize (c, هذا, axis_شمار_2 * shared_Tuple_شمار) &&
		  (is_long_جابه_جایی () ?
		     c->check_array (get_long_جابه_جایی_array (), c->get_رقم_glyphs () + 1) :
		     c->check_array (get_short_جابه_جایی_array (), c->get_رقم_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع sanitize_shallow (c); }

  منطقي decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_شمار_Type>& glyph_vars /*بیرون*/) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_بایتها_t> new_gid_var_داده_map;
    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);
    إذا (it->اولی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_داده_map.مجموعه (0, hb_بایتها_t ());
      it++;
    }

    لكل (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.اولی;
      hb_codepoint_t old_gid = _.second;
      hb_بایتها_t var_داده_بایتها = get_glyph_var_داده_بایتها (c->منبع_blob, glyph_شمار_X, old_gid);
      new_gid_var_داده_map.مجموعه (new_gid, var_داده_بایتها);
    }

    إذا (new_gid_var_داده_map.درون_error ()) أرجع خطأ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (هذا+sharedTuples).as_array ((بیعلامت) shared_Tuple_شمار * (بیعلامت) axis_شمار_2);
    أرجع glyph_vars.create_from_glyphs_var_داده (axis_شمار_2, shared_tuples, c->plan, new_gid_var_داده_map);
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_شمار_Type>& glyph_vars,
                  Iterator it,
                  بیعلامت axis_شمار,
                  بیعلامت رقم_glyphs,
                  منطقي force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (هذا);
    gvar_GVAR *بیرون = c->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->نسخه.major = 1;
    بیرون->نسخه.minor = 0;
    بیرون->axis_شمار_2 = axis_شمار;
    بیرون->glyph_شمار_X = hb_min (glyph_شمار_max (), رقم_glyphs);

    بیعلامت glyph_var_داده_اندازه = glyph_vars.compiled_byte_اندازه ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_جابه_جایی = glyph_var_داده_اندازه > 0x1FFFEu || force_long_offsets;
    بیرون->flags = long_جابه_جایی ? 1 : 0;

    بیعلامت glyph_var_داده_offsets_اندازه = (long_جابه_جایی ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *glyph_var_داده_offsets = c->allocate_اندازه<HBUINT8> (glyph_var_داده_offsets_اندازه, خطأ);
    إذا (!glyph_var_داده_offsets) return_trace (خطأ);

    /* shared tuples */
    بیعلامت shared_tuple_شمار = glyph_vars.compiled_shared_tuples_شمار ();
    بیرون->shared_Tuple_شمار = shared_tuple_شمار;

    إذا (!shared_tuple_شمار)
      بیرون->sharedTuples = (حرف *) glyph_var_داده_offsets + glyph_var_داده_offsets_اندازه - (حرف *) بیرون;
    وإلا
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      إذا (!shared_tuples.arrayZ) return_trace (خطأ);
      بیرون->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) بیرون;
    }

    حرف *glyph_var_داده = c->شروع_embed<حرف> ();
    إذا (!glyph_var_داده) return_trace (خطأ);
    بیرون->داده_Z = glyph_var_داده - (حرف *) بیرون;

    return_trace (glyph_vars.serialize_glyph_var_داده (c, it, long_جابه_جایی, رقم_glyphs,
                                                       (حرف *) glyph_var_داده_offsets));
  }

  منطقي instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    glyph_variations_t<Glyph_شمار_Type> glyph_vars;
    إذا (!decompile_glyph_variations (c, glyph_vars))
      return_trace (خطأ);

    إذا (!glyph_vars.instantiate (c->plan)) return_trace (خطأ);
    إذا (!glyph_vars.compile_بایتها (c->plan->axes_شاخص_map, c->plan->axes_old_شاخص_برچسب_map))
      return_trace (خطأ);

    بیعلامت axis_شمار = c->plan->axes_شاخص_map.get_population ();
    بیعلامت رقم_glyphs = c->plan->رقم_output_glyphs ();
    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);

    منطقي force_long_offsets = خطأ;
#اگر_تعریف_شده HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#پایان_اگر
    return_trace (serialize (c->serializer, glyph_vars, it, axis_شمار, رقم_glyphs, force_long_offsets));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    إذا (c->plan->normalized_coords)
      return_trace (instantiate (c));

    بیعلامت glyph_شمار = نسخه.مقصد_int () ? c->plan->منبع->get_رقم_glyphs () : 0;

    gvar_GVAR *بیرون = c->serializer->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->نسخه.major = 1;
    بیرون->نسخه.minor = 0;
    بیرون->axis_شمار_2 = axis_شمار_2;
    بیرون->shared_Tuple_شمار = shared_Tuple_شمار;

    بیعلامت صحيح رقم_glyphs = c->plan->رقم_output_glyphs ();
    بیرون->glyph_شمار_X = hb_min (glyph_شمار_max (), رقم_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_وکتور_t<hb_وکتور_t<حرف>> culled_vars;
    hb_مجموعه_t culled_gids;
    إذا (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (هذا+sharedTuples).as_array ((بیعلامت) shared_Tuple_شمار * (بیعلامت) axis_شمار_2);
      إذا (unlikely (!culled_vars.resize (رقم_glyphs))) return_trace (خطأ);
      خودکار cull_it = hb_iter (c->plan->new_مقصد_old_gid_list);
      إذا (cull_it->اولی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      لكل (خودکار &_ : cull_it)
      {
	hb_بایتها_t var_داده_بایتها = get_glyph_var_داده_بایتها (c->منبع_blob, glyph_شمار, _.second);
	إذا (var_داده_بایتها.درازی < Glyph_Variation_داده::min_اندازه) استمر;
	منطقي تغییریافته = خطأ;
	إذا (unlikely (!var_داده_بایتها.as<Glyph_Variation_داده> ()
		      ->cull_tuple_variations (var_داده_بایتها, axis_شمار_2,
					       shared_tuples_array,
					       &c->plan->axes_old_شاخص_برچسب_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.اولی], &تغییریافته)))
	  return_trace (خطأ);
	إذا (تغییریافته) culled_gids.جمع_کن (_.اولی);
      }
      إذا (unlikely (culled_gids.درون_error ())) return_trace (خطأ);
    }

    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);
    إذا (it->اولی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    بیعلامت subset_داده_اندازه = 0;
    بیعلامت padding_اندازه = 0;
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      بیعلامت glyph_داده_اندازه = culled_gids.has (_.اولی)
				 ? culled_vars[_.اولی].درازی
				 : get_glyph_var_داده_بایتها (c->منبع_blob, glyph_شمار, old_gid).درازی;
      إذا (glyph_داده_اندازه % 2)
      {
        glyph_داده_اندازه++;
        padding_اندازه++;
      }

      إذا (unlikely (hb_unsigned_جمع_کن_overflows (subset_داده_اندازه, glyph_داده_اندازه, &subset_داده_اندازه)))
	return_trace (خطأ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_جابه_جایی = subset_داده_اندازه > 0x1FFFEu;
#اگر_تعریف_شده HB_EXPERIMENTAL_API
    long_جابه_جایی = long_جابه_جایی || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#پایان_اگر
    بیرون->flags = long_جابه_جایی ? 1 : 0;

    بیعلامت subset_offsets_اندازه = (long_جابه_جایی ? 4 : 2) * (رقم_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_اندازه<HBUINT8> (subset_offsets_اندازه, خطأ);
    إذا (!subset_offsets) return_trace (خطأ);

    /* shared tuples */
    إذا (!shared_Tuple_شمار || !sharedTuples)
      بیرون->sharedTuples = (حرف *) subset_offsets + subset_offsets_اندازه - (حرف *) بیرون;
    وإلا
    {
      بیعلامت صحيح shared_tuple_اندازه = F2DOT14::static_اندازه * axis_شمار_2 * shared_Tuple_شمار;
      F2DOT14 *tuples = c->serializer->allocate_اندازه<F2DOT14> (shared_tuple_اندازه);
      إذا (!tuples) return_trace (خطأ);
      بیرون->sharedTuples = (حرف *) tuples - (حرف *) بیرون;
      hb_memcpy (tuples, هذا+sharedTuples, shared_tuple_اندازه);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    إذا (long_جابه_جایی)
      subset_داده_اندازه -= padding_اندازه;
    حرف *subset_داده = c->serializer->allocate_اندازه<حرف> (subset_داده_اندازه, خطأ);
    إذا (!subset_داده) return_trace (خطأ);
    بیرون->داده_Z = subset_داده - (حرف *) بیرون;


    إذا (long_جابه_جایی)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    بیعلامت صحيح glyph_جابه_جایی = 0;

    hb_codepoint_t آخرین = 0;
    it = hb_iter (c->plan->new_مقصد_old_gid_list);
    إذا (it->اولی == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.اولی;
      hb_codepoint_t old_gid = _.second;

      إذا (long_جابه_جایی)
	لكل (; آخرین < gid; آخرین++)
	  ((HBUINT32 *) subset_offsets)[آخرین] = glyph_جابه_جایی;
      وإلا
	لكل (; آخرین < gid; آخرین++)
	  ((HBUINT16 *) subset_offsets)[آخرین] = glyph_جابه_جایی / 2;

      hb_بایتها_t var_داده_بایتها = culled_gids.has (gid)
				  ? hb_بایتها_t (culled_vars[gid].arrayZ,
						culled_vars[gid].درازی)
				  : get_glyph_var_داده_بایتها (c->منبع_blob,
							      glyph_شمار,
							      old_gid);

      hb_memcpy (subset_داده, var_داده_بایتها.arrayZ, var_داده_بایتها.درازی);
      بیعلامت glyph_داده_اندازه = var_داده_بایتها.درازی;
      subset_داده += glyph_داده_اندازه;
      glyph_جابه_جایی += glyph_داده_اندازه;

      إذا (!long_جابه_جایی && (glyph_داده_اندازه % 2))
      {
        *subset_داده = 0;
        subset_داده++;
        glyph_جابه_جایی++;
      }

      إذا (long_جابه_جایی)
	((HBUINT32 *) subset_offsets)[gid] = glyph_جابه_جایی;
      وإلا
	((HBUINT16 *) subset_offsets)[gid] = glyph_جابه_جایی / 2;

      آخرین++; // Skip over gid
    }

    إذا (long_جابه_جایی)
      لكل (; آخرین < رقم_glyphs; آخرین++)
	((HBUINT32 *) subset_offsets)[آخرین] = glyph_جابه_جایی;
    وإلا
      لكل (; آخرین < رقم_glyphs; آخرین++)
	((HBUINT16 *) subset_offsets)[آخرین] = glyph_جابه_جایی / 2;

    return_trace (درست);
  }

  محمي:
  ثابت hb_بایتها_t get_glyph_var_داده_بایتها (hb_blob_t *blob,
					     بیعلامت glyph_شمار,
					     hb_codepoint_t glyph) ثابت
  {
    بیعلامت شروع_جابه_جایی = get_جابه_جایی (glyph_شمار, glyph);
    بیعلامت انتها_جابه_جایی = get_جابه_جایی (glyph_شمار, glyph+1);
    إذا (unlikely (انتها_جابه_جایی < شروع_جابه_جایی)) أرجع hb_بایتها_t ();
    بیعلامت درازی = انتها_جابه_جایی - شروع_جابه_جایی;
    hb_بایتها_t var_داده = blob->as_بایتها ().sub_array (((بیعلامت) داده_Z) + شروع_جابه_جایی, درازی);
    أرجع likely (var_داده.درازی >= Glyph_Variation_داده::min_اندازه) ? var_داده : hb_بایتها_t ();
  }

  منطقي is_long_جابه_جایی () ثابت { أرجع flags & 1; }

  بیعلامت get_جابه_جایی (بیعلامت glyph_شمار, بیعلامت i) ثابت
  {
    إذا (unlikely (i > glyph_شمار)) أرجع 0;
    hb_barrier ();
    أرجع is_long_جابه_جایی () ? get_long_جابه_جایی_array ()[i] : get_short_جابه_جایی_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_جابه_جایی_array () ثابت { أرجع (ثابت HBUINT32 *) &جابه_جایی_Z; }
  ثابت HBUINT16 *get_short_جابه_جایی_array () ثابت { أرجع (ثابت HBUINT16 *) &جابه_جایی_Z; }

  عام:
  بنية accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      أرجع hb_scalar_cache_t::create (hb_min ((بیعلامت) جدول->shared_Tuple_شمار,
						+Tuple_Variation_سرآیند::max_shared_tuple_شمار));
    }

    ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقي has_داده () ثابت { أرجع جدول->has_داده (); }

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<gvar_GVAR> (face, T::جدول_برچسب);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_شمار_2 = جدول->نسخه.مقصد_int () ? face->get_رقم_glyphs () : 0;
    }
    ~accelerator_t () { جدول.destroy (); }

    خاص:

    ساكن عائم infer_delta (ثابت hb_array_t<contour_نقطه_t> points,
			      ثابت hb_array_t<contour_نقطه_t> deltas,
			      بیعلامت صحيح target, بیعلامت صحيح prev, بیعلامت صحيح next,
			      عائم contour_نقطه_t::*m)
    {
      عائم هدف_val = points.arrayZ[target].*m;
      عائم prev_val = points.arrayZ[prev].*m;
      عائم بعدی_val = points.arrayZ[next].*m;
      عائم prev_delta =  deltas.arrayZ[prev].*m;
      عائم بعدی_delta =  deltas.arrayZ[next].*m;

      إذا (prev_val == بعدی_val)
	أرجع (prev_delta == بعدی_delta) ? prev_delta : 0.f;
      وإلا إذا (هدف_val <= hb_min (prev_val, بعدی_val))
	أرجع (prev_val < بعدی_val) ? prev_delta : بعدی_delta;
      وإلا إذا (هدف_val >= hb_max (prev_val, بعدی_val))
	أرجع (prev_val > بعدی_val) ? prev_delta : بعدی_delta;

      /* linear interpolation */
      عائم r = (هدف_val - prev_val) / (بعدی_val - prev_val);
      أرجع prev_delta + r * (بعدی_delta - prev_delta);
    }

    ساكن بیعلامت صحيح بعدی_شاخص (بیعلامت صحيح i, بیعلامت صحيح شروع, بیعلامت صحيح انتها)
    { أرجع (i >= انتها) ? شروع : (i + 1); }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
    قالب <منطقي is_x>
#پایان_اگر
    ساكن منطقي decompile_deltas_جمع_کن_مقصد_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_نقطه_t> points,
						عائم scalar,
						ثابت HBUINT8 *انتها,
						بیعلامت شروع
#اگر_تعریف_شده HB_OPTIMIZE_اندازه
						, منطقي is_x
#پایان_اگر
						)
    {
      بیعلامت i = 0;
      بیعلامت شمار = points.درازی;
      طالما (i < شمار)
      {
	إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
	بیعلامت control = *p++;
	بیعلامت پیاپی_شمار = (control & Tuple_مقادیر::مقدار_پیاپی_شمار_ماسک) + 1;
	بیعلامت stop = i + پیاپی_شمار;
	إذا (unlikely (stop > شمار)) أرجع خطأ;

	بیعلامت skip = i < شروع ? hb_min (شروع - i, پیاپی_شمار) : 0;
	i += skip;

	بدل (control & Tuple_مقادیر::مقادیر_اندازه_ماسک)
	{
	  حالة Tuple_مقادیر::مقادیر_ARE_ZEROS:
	    i = stop;
	    اكسر;
	  حالة Tuple_مقادیر::مقادیر_ARE_واژگان:
	  {
	    إذا (unlikely (p + پیاپی_شمار * HBINT16::static_اندازه > انتها)) أرجع خطأ;
	    p += skip * HBINT16::static_اندازه;
	    ثابت خودکار *pp = (ثابت HBINT16 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_مقادیر::مقادیر_ARE_LONGS:
	  {
	    إذا (unlikely (p + پیاپی_شمار * HBINT32::static_اندازه > انتها)) أرجع خطأ;
	    p += skip * HBINT32::static_اندازه;
	    ثابت خودکار *pp = (ثابت HBINT32 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_مقادیر::مقادیر_ARE_بایتها:
	  {
	    إذا (unlikely (p + پیاپی_شمار > انتها)) أرجع خطأ;
	    p += skip * HBINT8::static_اندازه;
	    ثابت خودکار *pp = (ثابت HBINT8 *) p;
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
      أرجع درست;
    }
    عام:
    منطقي apply_deltas_مقصد_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحيح> coords,
				 ثابت hb_array_t<contour_نقطه_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = اشاره_تهی,
				 منطقي phantom_only = خطأ,
				 int64_t *budget = اشاره_تهی) ثابت
    {
      إذا (unlikely (glyph >= glyph_شمار_2)) أرجع درست;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_بایتها_t var_داده_بایتها = جدول->get_glyph_var_داده_بایتها (جدول.get_blob (), glyph_شمار_2, glyph);
      إذا (!var_داده_بایتها.as<Glyph_Variation_داده> ()->has_داده ()) أرجع درست;

      خودکار &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      نام_نوع Glyph_Variation_داده::tuple_iterator_t iterator;
      إذا (!Glyph_Variation_داده::get_tuple_iterator (var_داده_بایتها, جدول->axis_شمار_2,
						   var_داده_بایتها.arrayZ,
						   shared_indices, &iterator))
	أرجع درست; /* so isn't applied at all */

      منطقي any_خصوصی_points = خطأ;
      منطقي خصوصی_points_checked = خطأ;

      /* Save original points for inferred delta calculation */
      خودکار &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      خودکار orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      خودکار &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      خودکار deltas = deltas_vec.as_array ();

      بیعلامت رقم_coords = جدول->axis_شمار_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (جدول+جدول->sharedTuples).as_array (جدول->shared_Tuple_شمار * رقم_coords);

      خودکار &خصوصی_indices = scratch.خصوصی_indices;
      خودکار &x_deltas = scratch.x_deltas;
      خودکار &y_deltas = scratch.y_deltas;

      بیعلامت شمار = points.درازی;
      منطقي flush = خطأ;

      افعل
      {
	عائم scalar = iterator.current_tuple->calculate_scalar (coords, رقم_coords, shared_tuples,
								 scalar_cache);

	إذا (scalar == 0.f) استمر;

	إذا (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && شمار >= 4 ? 4 : شمار)))
	  أرجع خطأ;

	إذا (!خصوصی_points_checked)
	{
	  خودکار پیمایش = iterator;
	  افعل
	  {
	    إذا (پیمایش.current_tuple->has_خصوصی_points ())
	    {
	      any_خصوصی_points = درست;
	      اكسر;
	    }
	  } طالما (پیمایش.move_مقصد_بعدی ());
	  خصوصی_points_checked = درست;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_داده ();
	بیعلامت صحيح درازی = iterator.current_tuple->get_داده_اندازه ();
	إذا (unlikely (!iterator.var_داده_بایتها.check_range (p, درازی)))
	  أرجع خطأ;

	إذا (!deltas)
	{
	  إذا (unlikely (!deltas_vec.resize_dirty  (شمار))) أرجع خطأ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? شمار - 4 : 0), 0,
		     (phantom_only ? 4 : شمار) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *انتها = p + درازی;

	منطقي has_خصوصی_points = iterator.current_tuple->has_خصوصی_points ();
	إذا (has_خصوصی_points &&
	    !Glyph_Variation_داده::decompile_points (p, خصوصی_indices, انتها))
	  أرجع خطأ;
	ثابت hb_array_t<بیعلامت صحيح> &indices = has_خصوصی_points ? خصوصی_indices : shared_indices;

	منطقي apply_مقصد_all = (indices.درازی == 0);
	بیعلامت رقم_deltas = apply_مقصد_all ? points.درازی : indices.درازی;
	بیعلامت شروع_deltas = (apply_مقصد_all && phantom_only && رقم_deltas >= 4 ? رقم_deltas - 4 : 0);

	إذا (apply_مقصد_all && !any_خصوصی_points)
	{
#اگر_تعریف_شده HB_OPTIMIZE_اندازه
	  إذا (unlikely (!decompile_deltas_جمع_کن_مقصد_points (p, points, scalar, انتها, شروع_deltas, درست))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جمع_کن_مقصد_points (p, points, scalar, انتها, شروع_deltas, خطأ))) أرجع خطأ;
#وإلا
	  إذا (unlikely (!decompile_deltas_جمع_کن_مقصد_points<درست> (p, points, scalar, انتها, شروع_deltas))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جمع_کن_مقصد_points<خطأ> (p, points, scalar, انتها, شروع_deltas))) أرجع خطأ;
#پایان_اگر
	  استمر;
	}

	إذا (unlikely (!x_deltas.resize_dirty  (رقم_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_داده::decompile_deltas (p, x_deltas, انتها, خطأ, شروع_deltas))) أرجع خطأ;
	إذا (unlikely (!y_deltas.resize_dirty  (رقم_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_داده::decompile_deltas (p, y_deltas, انتها, خطأ, شروع_deltas))) أرجع خطأ;

	إذا (!apply_مقصد_all)
	{
	  إذا (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    إذا (unlikely (orig_points_vec.درون_error ())) أرجع خطأ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  إذا (flush)
	  {
	    لكل (بیعلامت صحيح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? شمار - 4 : 0), 0,
		     (phantom_only ? 4 : شمار) * حجم (deltas[0]));
	}

	إذا (HB_OPTIMIZE_اندازه_VAL)
	{
	  لكل (بیعلامت صحيح i = 0; i < رقم_deltas; i++)
	  {
	    بیعلامت صحيح pt_شاخص;
	    إذا (apply_مقصد_all)
	      pt_شاخص = i;
	    وإلا
	    {
	      pt_شاخص = indices[i];
	      إذا (unlikely (pt_شاخص >= deltas.درازی)) استمر;
	    }
	    إذا (phantom_only && pt_شاخص < شمار - 4) استمر;
	    خودکار &delta = deltas.arrayZ[pt_شاخص];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.جمع_کن_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وإلا
	{
	  /* Ouch. Four cases... for optimization. */
	  إذا (scalar != 1.0f)
	  {
	    إذا (apply_مقصد_all)
	      لكل (بیعلامت صحيح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جمع_کن_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وإلا
	      لكل (بیعلامت صحيح i = 0; i < رقم_deltas; i++)
	      {
		بیعلامت صحيح pt_شاخص = indices[i];
		إذا (unlikely (pt_شاخص >= deltas.درازی)) استمر;
		إذا (phantom_only && pt_شاخص < شمار - 4) استمر;
		خودکار &delta = deltas.arrayZ[pt_شاخص];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جمع_کن_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وإلا
	  {
	    إذا (apply_مقصد_all)
	      لكل (بیعلامت صحيح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جمع_کن_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وإلا
	      لكل (بیعلامت صحيح i = 0; i < رقم_deltas; i++)
	      {
		بیعلامت صحيح pt_شاخص = indices[i];
		إذا (unlikely (pt_شاخص >= deltas.درازی)) استمر;
		إذا (phantom_only && pt_شاخص < شمار - 4) استمر;
		خودکار &delta = deltas.arrayZ[pt_شاخص];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جمع_کن_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	إذا (!apply_مقصد_all && !phantom_only)
	{
	  بیعلامت شروع_نقطه_2 = 0;
	  بیعلامت انتها_نقطه = 0;
	  طالما (درست)
	  {
	    طالما (انتها_نقطه < شمار && !points.arrayZ[انتها_نقطه].is_انتها_نقطه)
	      انتها_نقطه++;
	    إذا (unlikely (انتها_نقطه == شمار)) اكسر;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    بیعلامت unref_شمار = 0;
	    لكل (بیعلامت i = شروع_نقطه_2; i < انتها_نقطه + 1; i++)
	      unref_شمار += deltas.arrayZ[i].flag;
	    unref_شمار = (انتها_نقطه - شروع_نقطه_2 + 1) - unref_شمار;

	    بیعلامت j = شروع_نقطه_2;
	    إذا (unref_شمار == 0 || unref_شمار > انتها_نقطه - شروع_نقطه_2)
	      اذهب no_بیشتر_gaps;

	    لكل (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      بیعلامت صحيح prev, next, i;
	      لكل (;;)
	      {
		i = j;
		j = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
		إذا (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) اكسر;
	      }
	      prev = j = i;
	      لكل (;;)
	      {
		i = j;
		j = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
		إذا (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) اكسر;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      لكل (;;)
	      {
		i = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
		إذا (i == next) اكسر;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطه_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطه_t::y);
		إذا (--unref_شمار == 0) اذهب no_بیشتر_gaps;
	      }
	    }
	  no_بیشتر_gaps:
	    شروع_نقطه_2 = انتها_نقطه = انتها_نقطه + 1;
	  }
	}

	flush = درست;

      } طالما (iterator.move_مقصد_بعدی ());

      إذا (flush)
      {
	لكل (بیعلامت صحيح i = phantom_only ? شمار - 4 : 0; i < شمار; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      أرجع درست;
    }

    بیعلامت صحيح get_axis_شمار () ثابت { أرجع جدول->axis_شمار_2; }

    خاص:
    hb_blob_ptr_t<gvar_GVAR> جدول;
    بیعلامت glyph_شمار_2;
  };

  محمي:
  Fixed_نسخه<>نسخه;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_شمار_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_شمار;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_مقصد<Unsized_Array_از<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_شمار_Type
		glyph_شمار_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_مقصد<Glyph_Variation_داده>
		داده_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_از<HBUINT8>
		جابه_جایی_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  عام:
  DEFINE_اندازه_ARRAY (18 + Glyph_شمار_Type::static_اندازه, جابه_جایی_Z);
};

بنية gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_gvar;
};

بنية GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_GVAR;
};

بنية gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    إذا (has_داده ()) أرجع HB_DUAL_GET (*هذا, create_cache ());
    أرجع اشاره_تهی;
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقي apply_deltas_مقصد_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحيح> coords,
			       ثابت hb_array_t<contour_نقطه_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = اشاره_تهی,
			       منطقي phantom_only = خطأ,
			       int64_t *budget = اشاره_تهی) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, apply_deltas_مقصد_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  بیعلامت صحيح get_axis_شمار () ثابت
  {
    إذا (has_داده ()) أرجع HB_DUAL_GET (*هذا, get_axis_شمار ());
    أرجع 0;
  }
};

} /* namespace OT */

#پایان_اگر /* HB_OT_VAR_GVAR_TABLE_HH */
