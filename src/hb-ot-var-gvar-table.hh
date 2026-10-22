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

#جے_بنا_تعریف HB_OT_VAR_GVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_GVAR_جدول_HH

#ضمن "hb-decycler.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#تعریف_کرو HB_OT_ٹیگ_gvar HB_ٹیگ('g','v','a','r')
#تعریف_کرو HB_OT_ٹیگ_GVAR HB_ٹیگ('G','V','A','R')

بنية hb_glyf_scratch_t
{
  // glyf
  contour_نقطہ_سمتیہ_t all_points;
  contour_نقطہ_سمتیہ_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_نقطہ_سمتیہ_t orig_points;
  hb_سمتیہ_t<صحيح> x_deltas;
  hb_سمتیہ_t<صحيح> y_deltas;
  contour_نقطہ_سمتیہ_t deltas;
  hb_سمتیہ_t<بنانشانی صحيح> shared_indices;
  hb_سمتیہ_t<بنانشانی صحيح> نجی_indices;
};

نطاق OT {

سانچا <قسم_ناں وتھ_Type>
بنية glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  ورتوں tuple_variations_t = قسم_ناں Tuple_Variation_انکڑے<وتھ_Type>::tuple_variations_t;
  ورتوں Glyph_Variation_انکڑے = Tuple_Variation_انکڑے<وتھ_Type>;

  hb_سمتیہ_t<tuple_variations_t> glyph_variations;

  hb_سمتیہ_t<F2DOT14> compiled_shared_tuples;
  خاص:
  بنانشانی shared_tuples_گنتی = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عام:
  بنانشانی compiled_shared_tuples_گنتی () ثابت
  { أرجع shared_tuples_گنتی; }

  بنانشانی compiled_byte_ناپ () ثابت
  {
    بنانشانی byte_ناپ = 0;
    لكل (ثابت خودکار& _ : glyph_variations)
      byte_ناپ += _.get_compiled_byte_ناپ ();

    أرجع byte_ناپ;
  }

  منطقي create_from_glyphs_var_انکڑے (بنانشانی axis_گنتی,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_بائٹاں_t>& new_gid_var_انکڑے_map)
  {
    إذا (unlikely (!glyph_variations.alloc_exact (plan->new_منزل_old_gid_list.لمائی)))
      أرجع خطأ;

    خودکار it = hb_iter (plan->new_منزل_old_gid_list);
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.پہلا;
      contour_نقطہ_سمتیہ_t *all_contour_points;
      إذا (!new_gid_var_انکڑے_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        أرجع خطأ;
      hb_بائٹاں_t var_انکڑے = new_gid_var_انکڑے_map.get (new_gid);

      ثابت Glyph_Variation_انکڑے* p = reinterpret_cast<ثابت Glyph_Variation_انکڑے*> (var_انکڑے.arrayZ);
      قسم_ناں Glyph_Variation_انکڑے::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_سمتیہ_t<بنانشانی> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      إذا (!var_انکڑے || ! p->has_انکڑے () || !all_contour_points->لمائی ||
          !Glyph_Variation_انکڑے::get_tuple_iterator (var_انکڑے, axis_گنتی,
                                                   var_انکڑے.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        استمر;
      }

      منطقي is_مرکب_glyph = خطأ;
      is_مرکب_glyph = plan->مرکب_new_gids.has (new_gid);

      إذا (!p->decompile_tuple_variations (all_contour_points->لمائی, سچ /* is_gvar */,
                                          iterator, &(plan->axes_old_اشاریہ_ٹیگ_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*باہرلا*/
					  &pool,
                                          is_مرکب_glyph))
        أرجع خطأ;
      glyph_variations.push (std::move (tuple_vars));
    }
    أرجع !glyph_variations.وچ_error () && glyph_variations.لمائی == plan->new_منزل_old_gid_list.لمائی;
  }

  منطقي instantiate (ثابت hb_subset_plan_t *plan)
  {
    بنانشانی گنتی = plan->new_منزل_old_gid_list.لمائی;
    منطقي iup_optimize = خطأ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_codepoint_t new_gid = plan->new_منزل_old_gid_list[i].پہلا;
      contour_نقطہ_سمتیہ_t *all_points;
      إذا (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        أرجع خطأ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_نقطہ_سمتیہ_t points;
      إذا (iup_optimize)
      {
        points = *all_points;
        إذا (unlikely (points.وچ_error ())) أرجع خطأ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      إذا (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي compile_بائٹاں (ثابت hb_map_t& axes_اشاریہ_map,
                      ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map)
  {
    إذا (!compile_shared_tuples (axes_اشاریہ_map, axes_old_اشاریہ_ٹیگ_map))
      أرجع خطأ;
    لكل (tuple_variations_t& vars: glyph_variations)
      إذا (!vars.compile_بائٹاں (axes_اشاریہ_map, axes_old_اشاریہ_ٹیگ_map,
                               سچ, /* use shared points*/
                               سچ,
                               &shared_tuples_idx_map,
			       &pool))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي compile_shared_tuples (ثابت hb_map_t& axes_اشاریہ_map,
                              ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی> coords_گنتی_map;

    /* count the num of shared coords */
    لكل (tuple_variations_t& vars: glyph_variations)
    {
      لكل (tuple_delta_t& var : vars.tuple_vars)
      {
        إذا (!var.compile_coords (axes_اشاریہ_map, axes_old_اشاریہ_ٹیگ_map, &pool))
          أرجع خطأ;
        بنانشانی *گنتی;
	بنانشانی hash = hb_hash (&var.compiled_peak_coords);
        إذا (coords_گنتی_map.has_with_hash (&(var.compiled_peak_coords), hash, &گنتی))
	  (*گنتی)++;
        وإلا
          coords_گنتی_map.سیٹ_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    إذا (!coords_گنتی_map || coords_گنتی_map.وچ_error ())
      أرجع خطأ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_سمتیہ_t<hb_pair_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی>> shared_coords {
      + hb_iter (coords_گنتی_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی>& p) { أرجع p.second > 1; })
    };
    إذا (unlikely (shared_coords.وچ_error ())) أرجع خطأ;

    /* no shared tuples: no coords are used more than once */
    إذا (!shared_coords) أرجع سچ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_گنتی = hb_min (0xFFFu + 1, shared_coords.لمائی);
    بنانشانی len = shared_tuples_گنتی * (shared_coords[0].پہلا->لمائی);
    إذا (unlikely (!compiled_shared_tuples.alloc (len)))
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < shared_tuples_گنتی; i++)
    {
      shared_tuples_idx_map.سیٹ (shared_coords[i].پہلا, i);
      /* add a concat() in hb_vector_t? */
      لكل (خودکار c : shared_coords[i].پہلا->iter ())
        compiled_shared_tuples.push (c);
    }

    أرجع سچ;
  }

  ساكن صحيح _cmp_coords (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, بنانشانی> *a = (ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, بنانشانی> *) pa;
    ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, بنانشانی> *b = (ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, بنانشانی> *) pb;

    إذا (a->second != b->second)
      أرجع b->second - a->second; // high to low

    أرجع b->پہلا->as_array().cmp (a->پہلا->as_array ());
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_glyph_var_انکڑے (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقي long_وتھ,
                                 بنانشانی عدد_glyphs,
                                 حرف* glyph_var_انکڑے_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (long_وتھ)
    {
      ((HBUINT32 *) glyph_var_انکڑے_offsets)[0] = 0;
      glyph_var_انکڑے_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) glyph_var_انکڑے_offsets)[0] = 0;
      glyph_var_انکڑے_offsets += 2;
    }
    بنانشانی glyph_وتھ = 0;
    hb_codepoint_t آخری_gid = 0;
    بنانشانی idx = 0;

    Glyph_Variation_انکڑے* cur_glyph = c->شروع_embed<Glyph_Variation_انکڑے> ();
    إذا (!cur_glyph) return_trace (خطأ);
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.پہلا;
      إذا (long_وتھ)
        لكل (; آخری_gid < gid; آخری_gid++)
          ((HBUINT32 *) glyph_var_انکڑے_offsets)[آخری_gid] = glyph_وتھ;
      وإلا
        لكل (; آخری_gid < gid; آخری_gid++)
          ((HBUINT16 *) glyph_var_انکڑے_offsets)[آخری_gid] = glyph_وتھ / 2;

      إذا (idx >= glyph_variations.لمائی) return_trace (خطأ);
      إذا (!cur_glyph->serialize (c, سچ, glyph_variations[idx])) return_trace (خطأ);
      Glyph_Variation_انکڑے* اگلا_glyph = c->شروع_embed<Glyph_Variation_انکڑے> ();
      glyph_وتھ += (حرف *) اگلا_glyph - (حرف *) cur_glyph;

      إذا (long_وتھ)
        ((HBUINT32 *) glyph_var_انکڑے_offsets)[gid] = glyph_وتھ;
      وإلا
        ((HBUINT16 *) glyph_var_انکڑے_offsets)[gid] = glyph_وتھ / 2;

      آخری_gid++;
      idx++;
      cur_glyph = اگلا_glyph;
    }

    إذا (long_وتھ)
      لكل (; آخری_gid < عدد_glyphs; آخری_gid++)
        ((HBUINT32 *) glyph_var_انکڑے_offsets)[آخری_gid] = glyph_وتھ;
    وإلا
      لكل (; آخری_gid < عدد_glyphs; آخری_gid++)
        ((HBUINT16 *) glyph_var_انکڑے_offsets)[آخری_gid] = glyph_وتھ / 2;
    return_trace (سچ);
  }
};

سانچا <قسم_ناں T, قسم_ناں Types>
بنية gvar_GVAR
{
  ورتوں Glyph_گنتی_Type = قسم_ناں Types::HBUINT;

  ورتوں Glyph_Variation_انکڑے = Tuple_Variation_انکڑے<Glyph_گنتی_Type>;
  ساكن constexpr بنانشانی glyph_گنتی_max ()
  { أرجع (1u << (8 * Glyph_گنتی_Type::static_ناپ)) - 1; }

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int () != 0; }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخہ.major == 1) &&
		  sharedTuples.sanitize (c, هذا, axis_گنتی_2 * shared_Tuple_گنتی) &&
		  (is_long_وتھ () ?
		     c->check_array (get_long_وتھ_array (), c->get_عدد_glyphs () + 1) :
		     c->check_array (get_short_وتھ_array (), c->get_عدد_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع sanitize_shallow (c); }

  منطقي decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_گنتی_Type>& glyph_vars /*باہرلا*/) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_بائٹاں_t> new_gid_var_انکڑے_map;
    خودکار it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_انکڑے_map.سیٹ (0, hb_بائٹاں_t ());
      it++;
    }

    لكل (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;
      hb_بائٹاں_t var_انکڑے_بائٹاں = get_glyph_var_انکڑے_بائٹاں (c->سروت_blob, glyph_گنتی_X, old_gid);
      new_gid_var_انکڑے_map.سیٹ (new_gid, var_انکڑے_بائٹاں);
    }

    إذا (new_gid_var_انکڑے_map.وچ_error ()) أرجع خطأ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (هذا+sharedTuples).as_array ((بنانشانی) shared_Tuple_گنتی * (بنانشانی) axis_گنتی_2);
    أرجع glyph_vars.create_from_glyphs_var_انکڑے (axis_گنتی_2, shared_tuples, c->plan, new_gid_var_انکڑے_map);
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_گنتی_Type>& glyph_vars,
                  Iterator it,
                  بنانشانی axis_گنتی,
                  بنانشانی عدد_glyphs,
                  منطقي force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (هذا);
    gvar_GVAR *باہرلا = c->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->نسخہ.major = 1;
    باہرلا->نسخہ.minor = 0;
    باہرلا->axis_گنتی_2 = axis_گنتی;
    باہرلا->glyph_گنتی_X = hb_min (glyph_گنتی_max (), عدد_glyphs);

    بنانشانی glyph_var_انکڑے_ناپ = glyph_vars.compiled_byte_ناپ ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_وتھ = glyph_var_انکڑے_ناپ > 0x1FFFEu || force_long_offsets;
    باہرلا->flags = long_وتھ ? 1 : 0;

    بنانشانی glyph_var_انکڑے_offsets_ناپ = (long_وتھ ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *glyph_var_انکڑے_offsets = c->allocate_ناپ<HBUINT8> (glyph_var_انکڑے_offsets_ناپ, خطأ);
    إذا (!glyph_var_انکڑے_offsets) return_trace (خطأ);

    /* shared tuples */
    بنانشانی shared_tuple_گنتی = glyph_vars.compiled_shared_tuples_گنتی ();
    باہرلا->shared_Tuple_گنتی = shared_tuple_گنتی;

    إذا (!shared_tuple_گنتی)
      باہرلا->sharedTuples = (حرف *) glyph_var_انکڑے_offsets + glyph_var_انکڑے_offsets_ناپ - (حرف *) باہرلا;
    وإلا
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      إذا (!shared_tuples.arrayZ) return_trace (خطأ);
      باہرلا->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) باہرلا;
    }

    حرف *glyph_var_انکڑے = c->شروع_embed<حرف> ();
    إذا (!glyph_var_انکڑے) return_trace (خطأ);
    باہرلا->انکڑے_Z = glyph_var_انکڑے - (حرف *) باہرلا;

    return_trace (glyph_vars.serialize_glyph_var_انکڑے (c, it, long_وتھ, عدد_glyphs,
                                                       (حرف *) glyph_var_انکڑے_offsets));
  }

  منطقي instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    glyph_variations_t<Glyph_گنتی_Type> glyph_vars;
    إذا (!decompile_glyph_variations (c, glyph_vars))
      return_trace (خطأ);

    إذا (!glyph_vars.instantiate (c->plan)) return_trace (خطأ);
    إذا (!glyph_vars.compile_بائٹاں (c->plan->axes_اشاریہ_map, c->plan->axes_old_اشاریہ_ٹیگ_map))
      return_trace (خطأ);

    بنانشانی axis_گنتی = c->plan->axes_اشاریہ_map.get_population ();
    بنانشانی عدد_glyphs = c->plan->عدد_output_glyphs ();
    خودکار it = hb_iter (c->plan->new_منزل_old_gid_list);

    منطقي force_long_offsets = خطأ;
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#جے_انت
    return_trace (serialize (c->serializer, glyph_vars, it, axis_گنتی, عدد_glyphs, force_long_offsets));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    إذا (c->plan->normalized_coords)
      return_trace (instantiate (c));

    بنانشانی glyph_گنتی = نسخہ.منزل_int () ? c->plan->سروت->get_عدد_glyphs () : 0;

    gvar_GVAR *باہرلا = c->serializer->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->نسخہ.major = 1;
    باہرلا->نسخہ.minor = 0;
    باہرلا->axis_گنتی_2 = axis_گنتی_2;
    باہرلا->shared_Tuple_گنتی = shared_Tuple_گنتی;

    بنانشانی صحيح عدد_glyphs = c->plan->عدد_output_glyphs ();
    باہرلا->glyph_گنتی_X = hb_min (glyph_گنتی_max (), عدد_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_سمتیہ_t<hb_سمتیہ_t<حرف>> culled_vars;
    hb_سیٹ_t culled_gids;
    إذا (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (هذا+sharedTuples).as_array ((بنانشانی) shared_Tuple_گنتی * (بنانشانی) axis_گنتی_2);
      إذا (unlikely (!culled_vars.resize (عدد_glyphs))) return_trace (خطأ);
      خودکار cull_it = hb_iter (c->plan->new_منزل_old_gid_list);
      إذا (cull_it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      لكل (خودکار &_ : cull_it)
      {
	hb_بائٹاں_t var_انکڑے_بائٹاں = get_glyph_var_انکڑے_بائٹاں (c->سروت_blob, glyph_گنتی, _.second);
	إذا (var_انکڑے_بائٹاں.لمائی < Glyph_Variation_انکڑے::min_ناپ) استمر;
	منطقي بدلیا = خطأ;
	إذا (unlikely (!var_انکڑے_بائٹاں.as<Glyph_Variation_انکڑے> ()
		      ->cull_tuple_variations (var_انکڑے_بائٹاں, axis_گنتی_2,
					       shared_tuples_array,
					       &c->plan->axes_old_اشاریہ_ٹیگ_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.پہلا], &بدلیا)))
	  return_trace (خطأ);
	إذا (بدلیا) culled_gids.جوڑو (_.پہلا);
      }
      إذا (unlikely (culled_gids.وچ_error ())) return_trace (خطأ);
    }

    خودکار it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    بنانشانی subset_انکڑے_ناپ = 0;
    بنانشانی padding_ناپ = 0;
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      بنانشانی glyph_انکڑے_ناپ = culled_gids.has (_.پہلا)
				 ? culled_vars[_.پہلا].لمائی
				 : get_glyph_var_انکڑے_بائٹاں (c->سروت_blob, glyph_گنتی, old_gid).لمائی;
      إذا (glyph_انکڑے_ناپ % 2)
      {
        glyph_انکڑے_ناپ++;
        padding_ناپ++;
      }

      إذا (unlikely (hb_unsigned_جوڑو_overflows (subset_انکڑے_ناپ, glyph_انکڑے_ناپ, &subset_انکڑے_ناپ)))
	return_trace (خطأ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_وتھ = subset_انکڑے_ناپ > 0x1FFFEu;
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    long_وتھ = long_وتھ || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#جے_انت
    باہرلا->flags = long_وتھ ? 1 : 0;

    بنانشانی subset_offsets_ناپ = (long_وتھ ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_ناپ<HBUINT8> (subset_offsets_ناپ, خطأ);
    إذا (!subset_offsets) return_trace (خطأ);

    /* shared tuples */
    إذا (!shared_Tuple_گنتی || !sharedTuples)
      باہرلا->sharedTuples = (حرف *) subset_offsets + subset_offsets_ناپ - (حرف *) باہرلا;
    وإلا
    {
      بنانشانی صحيح shared_tuple_ناپ = F2DOT14::static_ناپ * axis_گنتی_2 * shared_Tuple_گنتی;
      F2DOT14 *tuples = c->serializer->allocate_ناپ<F2DOT14> (shared_tuple_ناپ);
      إذا (!tuples) return_trace (خطأ);
      باہرلا->sharedTuples = (حرف *) tuples - (حرف *) باہرلا;
      hb_memcpy (tuples, هذا+sharedTuples, shared_tuple_ناپ);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    إذا (long_وتھ)
      subset_انکڑے_ناپ -= padding_ناپ;
    حرف *subset_انکڑے = c->serializer->allocate_ناپ<حرف> (subset_انکڑے_ناپ, خطأ);
    إذا (!subset_انکڑے) return_trace (خطأ);
    باہرلا->انکڑے_Z = subset_انکڑے - (حرف *) باہرلا;


    إذا (long_وتھ)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    بنانشانی صحيح glyph_وتھ = 0;

    hb_codepoint_t آخری = 0;
    it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    لكل (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;

      إذا (long_وتھ)
	لكل (; آخری < gid; آخری++)
	  ((HBUINT32 *) subset_offsets)[آخری] = glyph_وتھ;
      وإلا
	لكل (; آخری < gid; آخری++)
	  ((HBUINT16 *) subset_offsets)[آخری] = glyph_وتھ / 2;

      hb_بائٹاں_t var_انکڑے_بائٹاں = culled_gids.has (gid)
				  ? hb_بائٹاں_t (culled_vars[gid].arrayZ,
						culled_vars[gid].لمائی)
				  : get_glyph_var_انکڑے_بائٹاں (c->سروت_blob,
							      glyph_گنتی,
							      old_gid);

      hb_memcpy (subset_انکڑے, var_انکڑے_بائٹاں.arrayZ, var_انکڑے_بائٹاں.لمائی);
      بنانشانی glyph_انکڑے_ناپ = var_انکڑے_بائٹاں.لمائی;
      subset_انکڑے += glyph_انکڑے_ناپ;
      glyph_وتھ += glyph_انکڑے_ناپ;

      إذا (!long_وتھ && (glyph_انکڑے_ناپ % 2))
      {
        *subset_انکڑے = 0;
        subset_انکڑے++;
        glyph_وتھ++;
      }

      إذا (long_وتھ)
	((HBUINT32 *) subset_offsets)[gid] = glyph_وتھ;
      وإلا
	((HBUINT16 *) subset_offsets)[gid] = glyph_وتھ / 2;

      آخری++; // Skip over gid
    }

    إذا (long_وتھ)
      لكل (; آخری < عدد_glyphs; آخری++)
	((HBUINT32 *) subset_offsets)[آخری] = glyph_وتھ;
    وإلا
      لكل (; آخری < عدد_glyphs; آخری++)
	((HBUINT16 *) subset_offsets)[آخری] = glyph_وتھ / 2;

    return_trace (سچ);
  }

  محمي:
  ثابت hb_بائٹاں_t get_glyph_var_انکڑے_بائٹاں (hb_blob_t *blob,
					     بنانشانی glyph_گنتی,
					     hb_codepoint_t glyph) ثابت
  {
    بنانشانی شروع_وتھ = get_وتھ (glyph_گنتی, glyph);
    بنانشانی سرا_وتھ = get_وتھ (glyph_گنتی, glyph+1);
    إذا (unlikely (سرا_وتھ < شروع_وتھ)) أرجع hb_بائٹاں_t ();
    بنانشانی لمائی = سرا_وتھ - شروع_وتھ;
    hb_بائٹاں_t var_انکڑے = blob->as_بائٹاں ().sub_array (((بنانشانی) انکڑے_Z) + شروع_وتھ, لمائی);
    أرجع likely (var_انکڑے.لمائی >= Glyph_Variation_انکڑے::min_ناپ) ? var_انکڑے : hb_بائٹاں_t ();
  }

  منطقي is_long_وتھ () ثابت { أرجع flags & 1; }

  بنانشانی get_وتھ (بنانشانی glyph_گنتی, بنانشانی i) ثابت
  {
    إذا (unlikely (i > glyph_گنتی)) أرجع 0;
    hb_barrier ();
    أرجع is_long_وتھ () ? get_long_وتھ_array ()[i] : get_short_وتھ_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_وتھ_array () ثابت { أرجع (ثابت HBUINT32 *) &وتھ_Z; }
  ثابت HBUINT16 *get_short_وتھ_array () ثابت { أرجع (ثابت HBUINT16 *) &وتھ_Z; }

  عام:
  بنية accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      أرجع hb_scalar_cache_t::create (hb_min ((بنانشانی) جدول->shared_Tuple_گنتی,
						+Tuple_Variation_سرخی::max_shared_tuple_گنتی));
    }

    ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقي has_انکڑے () ثابت { أرجع جدول->has_انکڑے (); }

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<gvar_GVAR> (face, T::جدول_ٹیگ);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_گنتی_2 = جدول->نسخہ.منزل_int () ? face->get_عدد_glyphs () : 0;
    }
    ~accelerator_t () { جدول.destroy (); }

    خاص:

    ساكن عائم infer_delta (ثابت hb_array_t<contour_نقطہ_t> points,
			      ثابت hb_array_t<contour_نقطہ_t> deltas,
			      بنانشانی صحيح target, بنانشانی صحيح prev, بنانشانی صحيح next,
			      عائم contour_نقطہ_t::*m)
    {
      عائم نشانہ_val = points.arrayZ[target].*m;
      عائم prev_val = points.arrayZ[prev].*m;
      عائم اگلا_val = points.arrayZ[next].*m;
      عائم prev_delta =  deltas.arrayZ[prev].*m;
      عائم اگلا_delta =  deltas.arrayZ[next].*m;

      إذا (prev_val == اگلا_val)
	أرجع (prev_delta == اگلا_delta) ? prev_delta : 0.f;
      وإلا إذا (نشانہ_val <= hb_min (prev_val, اگلا_val))
	أرجع (prev_val < اگلا_val) ? prev_delta : اگلا_delta;
      وإلا إذا (نشانہ_val >= hb_max (prev_val, اگلا_val))
	أرجع (prev_val > اگلا_val) ? prev_delta : اگلا_delta;

      /* linear interpolation */
      عائم r = (نشانہ_val - prev_val) / (اگلا_val - prev_val);
      أرجع prev_delta + r * (اگلا_delta - prev_delta);
    }

    ساكن بنانشانی صحيح اگلا_اشاریہ (بنانشانی صحيح i, بنانشانی صحيح شروع, بنانشانی صحيح سرا)
    { أرجع (i >= سرا) ? شروع : (i + 1); }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
    سانچا <منطقي is_x>
#جے_انت
    ساكن منطقي decompile_deltas_جوڑو_منزل_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_نقطہ_t> points,
						عائم scalar,
						ثابت HBUINT8 *سرا,
						بنانشانی شروع
#جے_تعریف_شدہ HB_OPTIMIZE_ناپ
						, منطقي is_x
#جے_انت
						)
    {
      بنانشانی i = 0;
      بنانشانی گنتی = points.لمائی;
      طالما (i < گنتی)
      {
	إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
	بنانشانی control = *p++;
	بنانشانی لگاتار_گنتی = (control & Tuple_اقدار::قدر_لگاتار_گنتی_نقاب) + 1;
	بنانشانی stop = i + لگاتار_گنتی;
	إذا (unlikely (stop > گنتی)) أرجع خطأ;

	بنانشانی skip = i < شروع ? hb_min (شروع - i, لگاتار_گنتی) : 0;
	i += skip;

	بدل (control & Tuple_اقدار::اقدار_ناپ_نقاب)
	{
	  حالة Tuple_اقدار::اقدار_ARE_ZEROS:
	    i = stop;
	    اكسر;
	  حالة Tuple_اقدار::اقدار_ARE_الفاظ:
	  {
	    إذا (unlikely (p + لگاتار_گنتی * HBINT16::static_ناپ > سرا)) أرجع خطأ;
	    p += skip * HBINT16::static_ناپ;
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
	  حالة Tuple_اقدار::اقدار_ARE_LONGS:
	  {
	    إذا (unlikely (p + لگاتار_گنتی * HBINT32::static_ناپ > سرا)) أرجع خطأ;
	    p += skip * HBINT32::static_ناپ;
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
	  حالة Tuple_اقدار::اقدار_ARE_بائٹاں:
	  {
	    إذا (unlikely (p + لگاتار_گنتی > سرا)) أرجع خطأ;
	    p += skip * HBINT8::static_ناپ;
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
      أرجع سچ;
    }
    عام:
    منطقي apply_deltas_منزل_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحيح> coords,
				 ثابت hb_array_t<contour_نقطہ_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالی_سوچک,
				 منطقي phantom_only = خطأ,
				 int64_t *budget = خالی_سوچک) ثابت
    {
      إذا (unlikely (glyph >= glyph_گنتی_2)) أرجع سچ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_بائٹاں_t var_انکڑے_بائٹاں = جدول->get_glyph_var_انکڑے_بائٹاں (جدول.get_blob (), glyph_گنتی_2, glyph);
      إذا (!var_انکڑے_بائٹاں.as<Glyph_Variation_انکڑے> ()->has_انکڑے ()) أرجع سچ;

      خودکار &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      قسم_ناں Glyph_Variation_انکڑے::tuple_iterator_t iterator;
      إذا (!Glyph_Variation_انکڑے::get_tuple_iterator (var_انکڑے_بائٹاں, جدول->axis_گنتی_2,
						   var_انکڑے_بائٹاں.arrayZ,
						   shared_indices, &iterator))
	أرجع سچ; /* so isn't applied at all */

      منطقي any_نجی_points = خطأ;
      منطقي نجی_points_checked = خطأ;

      /* Save original points for inferred delta calculation */
      خودکار &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      خودکار orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      خودکار &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      خودکار deltas = deltas_vec.as_array ();

      بنانشانی عدد_coords = جدول->axis_گنتی_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (جدول+جدول->sharedTuples).as_array (جدول->shared_Tuple_گنتی * عدد_coords);

      خودکار &نجی_indices = scratch.نجی_indices;
      خودکار &x_deltas = scratch.x_deltas;
      خودکار &y_deltas = scratch.y_deltas;

      بنانشانی گنتی = points.لمائی;
      منطقي flush = خطأ;

      افعل
      {
	عائم scalar = iterator.current_tuple->calculate_scalar (coords, عدد_coords, shared_tuples,
								 scalar_cache);

	إذا (scalar == 0.f) استمر;

	إذا (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && گنتی >= 4 ? 4 : گنتی)))
	  أرجع خطأ;

	إذا (!نجی_points_checked)
	{
	  خودکار جانچ = iterator;
	  افعل
	  {
	    إذا (جانچ.current_tuple->has_نجی_points ())
	    {
	      any_نجی_points = سچ;
	      اكسر;
	    }
	  } طالما (جانچ.move_منزل_اگلا ());
	  نجی_points_checked = سچ;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_انکڑے ();
	بنانشانی صحيح لمائی = iterator.current_tuple->get_انکڑے_ناپ ();
	إذا (unlikely (!iterator.var_انکڑے_بائٹاں.check_range (p, لمائی)))
	  أرجع خطأ;

	إذا (!deltas)
	{
	  إذا (unlikely (!deltas_vec.resize_dirty  (گنتی))) أرجع خطأ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? گنتی - 4 : 0), 0,
		     (phantom_only ? 4 : گنتی) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *سرا = p + لمائی;

	منطقي has_نجی_points = iterator.current_tuple->has_نجی_points ();
	إذا (has_نجی_points &&
	    !Glyph_Variation_انکڑے::decompile_points (p, نجی_indices, سرا))
	  أرجع خطأ;
	ثابت hb_array_t<بنانشانی صحيح> &indices = has_نجی_points ? نجی_indices : shared_indices;

	منطقي apply_منزل_all = (indices.لمائی == 0);
	بنانشانی عدد_deltas = apply_منزل_all ? points.لمائی : indices.لمائی;
	بنانشانی شروع_deltas = (apply_منزل_all && phantom_only && عدد_deltas >= 4 ? عدد_deltas - 4 : 0);

	إذا (apply_منزل_all && !any_نجی_points)
	{
#جے_تعریف_شدہ HB_OPTIMIZE_ناپ
	  إذا (unlikely (!decompile_deltas_جوڑو_منزل_points (p, points, scalar, سرا, شروع_deltas, سچ))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جوڑو_منزل_points (p, points, scalar, سرا, شروع_deltas, خطأ))) أرجع خطأ;
#وإلا
	  إذا (unlikely (!decompile_deltas_جوڑو_منزل_points<سچ> (p, points, scalar, سرا, شروع_deltas))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جوڑو_منزل_points<خطأ> (p, points, scalar, سرا, شروع_deltas))) أرجع خطأ;
#جے_انت
	  استمر;
	}

	إذا (unlikely (!x_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_انکڑے::decompile_deltas (p, x_deltas, سرا, خطأ, شروع_deltas))) أرجع خطأ;
	إذا (unlikely (!y_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_انکڑے::decompile_deltas (p, y_deltas, سرا, خطأ, شروع_deltas))) أرجع خطأ;

	إذا (!apply_منزل_all)
	{
	  إذا (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    إذا (unlikely (orig_points_vec.وچ_error ())) أرجع خطأ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  إذا (flush)
	  {
	    لكل (بنانشانی صحيح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? گنتی - 4 : 0), 0,
		     (phantom_only ? 4 : گنتی) * حجم (deltas[0]));
	}

	إذا (HB_OPTIMIZE_ناپ_VAL)
	{
	  لكل (بنانشانی صحيح i = 0; i < عدد_deltas; i++)
	  {
	    بنانشانی صحيح pt_اشاریہ;
	    إذا (apply_منزل_all)
	      pt_اشاریہ = i;
	    وإلا
	    {
	      pt_اشاریہ = indices[i];
	      إذا (unlikely (pt_اشاریہ >= deltas.لمائی)) استمر;
	    }
	    إذا (phantom_only && pt_اشاریہ < گنتی - 4) استمر;
	    خودکار &delta = deltas.arrayZ[pt_اشاریہ];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وإلا
	{
	  /* Ouch. Four cases... for optimization. */
	  إذا (scalar != 1.0f)
	  {
	    إذا (apply_منزل_all)
	      لكل (بنانشانی صحيح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وإلا
	      لكل (بنانشانی صحيح i = 0; i < عدد_deltas; i++)
	      {
		بنانشانی صحيح pt_اشاریہ = indices[i];
		إذا (unlikely (pt_اشاریہ >= deltas.لمائی)) استمر;
		إذا (phantom_only && pt_اشاریہ < گنتی - 4) استمر;
		خودکار &delta = deltas.arrayZ[pt_اشاریہ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وإلا
	  {
	    إذا (apply_منزل_all)
	      لكل (بنانشانی صحيح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جوڑو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وإلا
	      لكل (بنانشانی صحيح i = 0; i < عدد_deltas; i++)
	      {
		بنانشانی صحيح pt_اشاریہ = indices[i];
		إذا (unlikely (pt_اشاریہ >= deltas.لمائی)) استمر;
		إذا (phantom_only && pt_اشاریہ < گنتی - 4) استمر;
		خودکار &delta = deltas.arrayZ[pt_اشاریہ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڑو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	إذا (!apply_منزل_all && !phantom_only)
	{
	  بنانشانی شروع_نقطہ_2 = 0;
	  بنانشانی سرا_نقطہ = 0;
	  طالما (سچ)
	  {
	    طالما (سرا_نقطہ < گنتی && !points.arrayZ[سرا_نقطہ].is_سرا_نقطہ)
	      سرا_نقطہ++;
	    إذا (unlikely (سرا_نقطہ == گنتی)) اكسر;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    بنانشانی unref_گنتی = 0;
	    لكل (بنانشانی i = شروع_نقطہ_2; i < سرا_نقطہ + 1; i++)
	      unref_گنتی += deltas.arrayZ[i].flag;
	    unref_گنتی = (سرا_نقطہ - شروع_نقطہ_2 + 1) - unref_گنتی;

	    بنانشانی j = شروع_نقطہ_2;
	    إذا (unref_گنتی == 0 || unref_گنتی > سرا_نقطہ - شروع_نقطہ_2)
	      اذهب no_ہور_gaps;

	    لكل (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      بنانشانی صحيح prev, next, i;
	      لكل (;;)
	      {
		i = j;
		j = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
		إذا (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) اكسر;
	      }
	      prev = j = i;
	      لكل (;;)
	      {
		i = j;
		j = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
		إذا (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) اكسر;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      لكل (;;)
	      {
		i = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
		إذا (i == next) اكسر;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطہ_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطہ_t::y);
		إذا (--unref_گنتی == 0) اذهب no_ہور_gaps;
	      }
	    }
	  no_ہور_gaps:
	    شروع_نقطہ_2 = سرا_نقطہ = سرا_نقطہ + 1;
	  }
	}

	flush = سچ;

      } طالما (iterator.move_منزل_اگلا ());

      إذا (flush)
      {
	لكل (بنانشانی صحيح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      أرجع سچ;
    }

    بنانشانی صحيح get_axis_گنتی () ثابت { أرجع جدول->axis_گنتی_2; }

    خاص:
    hb_blob_ptr_t<gvar_GVAR> جدول;
    بنانشانی glyph_گنتی_2;
  };

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_گنتی_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_گنتی;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_منزل<Unsized_Array_دا<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_گنتی_Type
		glyph_گنتی_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_منزل<Glyph_Variation_انکڑے>
		انکڑے_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_دا<HBUINT8>
		وتھ_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  عام:
  DEFINE_ناپ_ARRAY (18 + Glyph_گنتی_Type::static_ناپ, وتھ_Z);
};

بنية gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_gvar;
};

بنية GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_GVAR;
};

بنية gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    إذا (has_انکڑے ()) أرجع HB_DUAL_GET (*هذا, create_cache ());
    أرجع خالی_سوچک;
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقي apply_deltas_منزل_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحيح> coords,
			       ثابت hb_array_t<contour_نقطہ_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = خالی_سوچک,
			       منطقي phantom_only = خطأ,
			       int64_t *budget = خالی_سوچک) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, apply_deltas_منزل_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  بنانشانی صحيح get_axis_گنتی () ثابت
  {
    إذا (has_انکڑے ()) أرجع HB_DUAL_GET (*هذا, get_axis_گنتی ());
    أرجع 0;
  }
};

} /* namespace OT */

#جے_انت /* HB_OT_VAR_GVAR_TABLE_HH */
