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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_GVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_GVAR_جدول_HH

#شامل "hb-decycler.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"
#شامل "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#تعریف_کرو HB_OT_لیبل_gvar HB_لیبل('g','v','a','r')
#تعریف_کرو HB_OT_لیبل_GVAR HB_لیبل('G','V','A','R')

ساخت hb_glyf_scratch_t
{
  // glyf
  contour_نقطہ_سمتیہ_t all_points;
  contour_نقطہ_سمتیہ_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_نقطہ_سمتیہ_t orig_points;
  hb_سمتیہ_t<صحیح> x_deltas;
  hb_سمتیہ_t<صحیح> y_deltas;
  contour_نقطہ_سمتیہ_t deltas;
  hb_سمتیہ_t<غیرعلامتی صحیح> shared_indices;
  hb_سمتیہ_t<غیرعلامتی صحیح> نجی_indices;
};

نامخلا OT {

سانچہ <قسم_نام انحراف_Type>
ساخت glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  استعمال tuple_variations_t = قسم_نام Tuple_Variation_ڈیٹا<انحراف_Type>::tuple_variations_t;
  استعمال Glyph_Variation_ڈیٹا = Tuple_Variation_ڈیٹا<انحراف_Type>;

  hb_سمتیہ_t<tuple_variations_t> glyph_variations;

  hb_سمتیہ_t<F2DOT14> compiled_shared_tuples;
  نجی:
  غیرعلامتی shared_tuples_گنتی = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عوامی:
  غیرعلامتی compiled_shared_tuples_گنتی () ثابت
  { واپس shared_tuples_گنتی; }

  غیرعلامتی compiled_byte_سائز () ثابت
  {
    غیرعلامتی byte_سائز = 0;
    برائے (ثابت خودکار& _ : glyph_variations)
      byte_سائز += _.get_compiled_byte_سائز ();

    واپس byte_سائز;
  }

  منطقی create_from_glyphs_var_ڈیٹا (غیرعلامتی axis_گنتی,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_بائٹیں_t>& new_gid_var_ڈیٹا_map)
  {
    اگر (unlikely (!glyph_variations.alloc_exact (plan->new_مقصد_old_gid_list.لمبائی)))
      واپس جھوٹ;

    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    برائے (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.پہلا;
      contour_نقطہ_سمتیہ_t *all_contour_points;
      اگر (!new_gid_var_ڈیٹا_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        واپس جھوٹ;
      hb_بائٹیں_t var_ڈیٹا = new_gid_var_ڈیٹا_map.get (new_gid);

      ثابت Glyph_Variation_ڈیٹا* p = reinterpret_cast<ثابت Glyph_Variation_ڈیٹا*> (var_ڈیٹا.arrayZ);
      قسم_نام Glyph_Variation_ڈیٹا::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_سمتیہ_t<غیرعلامتی> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      اگر (!var_ڈیٹا || ! p->has_ڈیٹا () || !all_contour_points->لمبائی ||
          !Glyph_Variation_ڈیٹا::get_tuple_iterator (var_ڈیٹا, axis_گنتی,
                                                   var_ڈیٹا.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        جاری;
      }

      منطقی is_مرکب_glyph = جھوٹ;
      is_مرکب_glyph = plan->مرکب_new_gids.has (new_gid);

      اگر (!p->decompile_tuple_variations (all_contour_points->لمبائی, سچ /* is_gvar */,
                                          iterator, &(plan->axes_old_اشاریہ_لیبل_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*برآمد*/
					  &pool,
                                          is_مرکب_glyph))
        واپس جھوٹ;
      glyph_variations.push (std::move (tuple_vars));
    }
    واپس !glyph_variations.اندر_error () && glyph_variations.لمبائی == plan->new_مقصد_old_gid_list.لمبائی;
  }

  منطقی instantiate (ثابت hb_subset_plan_t *plan)
  {
    غیرعلامتی گنتی = plan->new_مقصد_old_gid_list.لمبائی;
    منطقی iup_optimize = جھوٹ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_codepoint_t new_gid = plan->new_مقصد_old_gid_list[i].پہلا;
      contour_نقطہ_سمتیہ_t *all_points;
      اگر (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        واپس جھوٹ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_نقطہ_سمتیہ_t points;
      اگر (iup_optimize)
      {
        points = *all_points;
        اگر (unlikely (points.اندر_error ())) واپس جھوٹ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      اگر (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      اگر (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        واپس جھوٹ;
    }
    واپس سچ;
  }

  منطقی compile_بائٹیں (ثابت hb_map_t& axes_اشاریہ_map,
                      ثابت hb_map_t& axes_old_اشاریہ_لیبل_map)
  {
    اگر (!compile_shared_tuples (axes_اشاریہ_map, axes_old_اشاریہ_لیبل_map))
      واپس جھوٹ;
    برائے (tuple_variations_t& vars: glyph_variations)
      اگر (!vars.compile_بائٹیں (axes_اشاریہ_map, axes_old_اشاریہ_لیبل_map,
                               سچ, /* use shared points*/
                               سچ,
                               &shared_tuples_idx_map,
			       &pool))
        واپس جھوٹ;

    واپس سچ;
  }

  منطقی compile_shared_tuples (ثابت hb_map_t& axes_اشاریہ_map,
                              ثابت hb_map_t& axes_old_اشاریہ_لیبل_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی> coords_گنتی_map;

    /* count the num of shared coords */
    برائے (tuple_variations_t& vars: glyph_variations)
    {
      برائے (tuple_delta_t& var : vars.tuple_vars)
      {
        اگر (!var.compile_coords (axes_اشاریہ_map, axes_old_اشاریہ_لیبل_map, &pool))
          واپس جھوٹ;
        غیرعلامتی *گنتی;
	غیرعلامتی hash = hb_hash (&var.compiled_peak_coords);
        اگر (coords_گنتی_map.has_with_hash (&(var.compiled_peak_coords), hash, &گنتی))
	  (*گنتی)++;
        ورنہ
          coords_گنتی_map.سیٹ_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    اگر (!coords_گنتی_map || coords_گنتی_map.اندر_error ())
      واپس جھوٹ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_سمتیہ_t<hb_pair_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی>> shared_coords {
      + hb_iter (coords_گنتی_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی>& p) { واپس p.second > 1; })
    };
    اگر (unlikely (shared_coords.اندر_error ())) واپس جھوٹ;

    /* no shared tuples: no coords are used more than once */
    اگر (!shared_coords) واپس سچ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_گنتی = hb_min (0xFFFu + 1, shared_coords.لمبائی);
    غیرعلامتی len = shared_tuples_گنتی * (shared_coords[0].پہلا->لمبائی);
    اگر (unlikely (!compiled_shared_tuples.alloc (len)))
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < shared_tuples_گنتی; i++)
    {
      shared_tuples_idx_map.سیٹ (shared_coords[i].پہلا, i);
      /* add a concat() in hb_vector_t? */
      برائے (خودکار c : shared_coords[i].پہلا->iter ())
        compiled_shared_tuples.push (c);
    }

    واپس سچ;
  }

  ساکن صحیح _cmp_coords (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, غیرعلامتی> *a = (ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, غیرعلامتی> *) pa;
    ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, غیرعلامتی> *b = (ثابت hb_pair_t<hb_سمتیہ_t<F2DOT14> *, غیرعلامتی> *) pb;

    اگر (a->second != b->second)
      واپس b->second - a->second; // high to low

    واپس b->پہلا->as_array().cmp (a->پہلا->as_array ());
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_glyph_var_ڈیٹا (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقی long_انحراف,
                                 غیرعلامتی عدد_glyphs,
                                 حرف* glyph_var_ڈیٹا_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (یہ);

    اگر (long_انحراف)
    {
      ((HBUINT32 *) glyph_var_ڈیٹا_offsets)[0] = 0;
      glyph_var_ڈیٹا_offsets += 4;
    }
    ورنہ
    {
      ((HBUINT16 *) glyph_var_ڈیٹا_offsets)[0] = 0;
      glyph_var_ڈیٹا_offsets += 2;
    }
    غیرعلامتی glyph_انحراف = 0;
    hb_codepoint_t آخری_gid = 0;
    غیرعلامتی idx = 0;

    Glyph_Variation_ڈیٹا* cur_glyph = c->آغاز_embed<Glyph_Variation_ڈیٹا> ();
    اگر (!cur_glyph) return_trace (جھوٹ);
    برائے (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.پہلا;
      اگر (long_انحراف)
        برائے (; آخری_gid < gid; آخری_gid++)
          ((HBUINT32 *) glyph_var_ڈیٹا_offsets)[آخری_gid] = glyph_انحراف;
      ورنہ
        برائے (; آخری_gid < gid; آخری_gid++)
          ((HBUINT16 *) glyph_var_ڈیٹا_offsets)[آخری_gid] = glyph_انحراف / 2;

      اگر (idx >= glyph_variations.لمبائی) return_trace (جھوٹ);
      اگر (!cur_glyph->serialize (c, سچ, glyph_variations[idx])) return_trace (جھوٹ);
      Glyph_Variation_ڈیٹا* اگلا_glyph = c->آغاز_embed<Glyph_Variation_ڈیٹا> ();
      glyph_انحراف += (حرف *) اگلا_glyph - (حرف *) cur_glyph;

      اگر (long_انحراف)
        ((HBUINT32 *) glyph_var_ڈیٹا_offsets)[gid] = glyph_انحراف;
      ورنہ
        ((HBUINT16 *) glyph_var_ڈیٹا_offsets)[gid] = glyph_انحراف / 2;

      آخری_gid++;
      idx++;
      cur_glyph = اگلا_glyph;
    }

    اگر (long_انحراف)
      برائے (; آخری_gid < عدد_glyphs; آخری_gid++)
        ((HBUINT32 *) glyph_var_ڈیٹا_offsets)[آخری_gid] = glyph_انحراف;
    ورنہ
      برائے (; آخری_gid < عدد_glyphs; آخری_gid++)
        ((HBUINT16 *) glyph_var_ڈیٹا_offsets)[آخری_gid] = glyph_انحراف / 2;
    return_trace (سچ);
  }
};

سانچہ <قسم_نام T, قسم_نام Types>
ساخت gvar_GVAR
{
  استعمال Glyph_گنتی_Type = قسم_نام Types::HBUINT;

  استعمال Glyph_Variation_ڈیٹا = Tuple_Variation_ڈیٹا<Glyph_گنتی_Type>;
  ساکن constexpr غیرعلامتی glyph_گنتی_max ()
  { واپس (1u << (8 * Glyph_گنتی_Type::static_سائز)) - 1; }

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int () != 0; }

  منطقی sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  (نسخہ.major == 1) &&
		  sharedTuples.sanitize (c, یہ, axis_گنتی_2 * shared_Tuple_گنتی) &&
		  (is_long_انحراف () ?
		     c->check_array (get_long_انحراف_array (), c->get_عدد_glyphs () + 1) :
		     c->check_array (get_short_انحراف_array (), c->get_عدد_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { واپس sanitize_shallow (c); }

  منطقی decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_گنتی_Type>& glyph_vars /*برآمد*/) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_بائٹیں_t> new_gid_var_ڈیٹا_map;
    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);
    اگر (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_ڈیٹا_map.سیٹ (0, hb_بائٹیں_t ());
      it++;
    }

    برائے (خودکار &_ : it)
    {
      hb_codepoint_t new_gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;
      hb_بائٹیں_t var_ڈیٹا_بائٹیں = get_glyph_var_ڈیٹا_بائٹیں (c->ماخذ_blob, glyph_گنتی_X, old_gid);
      new_gid_var_ڈیٹا_map.سیٹ (new_gid, var_ڈیٹا_بائٹیں);
    }

    اگر (new_gid_var_ڈیٹا_map.اندر_error ()) واپس جھوٹ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (یہ+sharedTuples).as_array ((غیرعلامتی) shared_Tuple_گنتی * (غیرعلامتی) axis_گنتی_2);
    واپس glyph_vars.create_from_glyphs_var_ڈیٹا (axis_گنتی_2, shared_tuples, c->plan, new_gid_var_ڈیٹا_map);
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_گنتی_Type>& glyph_vars,
                  Iterator it,
                  غیرعلامتی axis_گنتی,
                  غیرعلامتی عدد_glyphs,
                  منطقی force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (یہ);
    gvar_GVAR *برآمد = c->allocate_min<gvar_GVAR> ();
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->نسخہ.major = 1;
    برآمد->نسخہ.minor = 0;
    برآمد->axis_گنتی_2 = axis_گنتی;
    برآمد->glyph_گنتی_X = hb_min (glyph_گنتی_max (), عدد_glyphs);

    غیرعلامتی glyph_var_ڈیٹا_سائز = glyph_vars.compiled_byte_سائز ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقی long_انحراف = glyph_var_ڈیٹا_سائز > 0x1FFFEu || force_long_offsets;
    برآمد->flags = long_انحراف ? 1 : 0;

    غیرعلامتی glyph_var_ڈیٹا_offsets_سائز = (long_انحراف ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *glyph_var_ڈیٹا_offsets = c->allocate_سائز<HBUINT8> (glyph_var_ڈیٹا_offsets_سائز, جھوٹ);
    اگر (!glyph_var_ڈیٹا_offsets) return_trace (جھوٹ);

    /* shared tuples */
    غیرعلامتی shared_tuple_گنتی = glyph_vars.compiled_shared_tuples_گنتی ();
    برآمد->shared_Tuple_گنتی = shared_tuple_گنتی;

    اگر (!shared_tuple_گنتی)
      برآمد->sharedTuples = (حرف *) glyph_var_ڈیٹا_offsets + glyph_var_ڈیٹا_offsets_سائز - (حرف *) برآمد;
    ورنہ
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      اگر (!shared_tuples.arrayZ) return_trace (جھوٹ);
      برآمد->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) برآمد;
    }

    حرف *glyph_var_ڈیٹا = c->آغاز_embed<حرف> ();
    اگر (!glyph_var_ڈیٹا) return_trace (جھوٹ);
    برآمد->ڈیٹا_Z = glyph_var_ڈیٹا - (حرف *) برآمد;

    return_trace (glyph_vars.serialize_glyph_var_ڈیٹا (c, it, long_انحراف, عدد_glyphs,
                                                       (حرف *) glyph_var_ڈیٹا_offsets));
  }

  منطقی instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    glyph_variations_t<Glyph_گنتی_Type> glyph_vars;
    اگر (!decompile_glyph_variations (c, glyph_vars))
      return_trace (جھوٹ);

    اگر (!glyph_vars.instantiate (c->plan)) return_trace (جھوٹ);
    اگر (!glyph_vars.compile_بائٹیں (c->plan->axes_اشاریہ_map, c->plan->axes_old_اشاریہ_لیبل_map))
      return_trace (جھوٹ);

    غیرعلامتی axis_گنتی = c->plan->axes_اشاریہ_map.get_population ();
    غیرعلامتی عدد_glyphs = c->plan->عدد_output_glyphs ();
    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);

    منطقی force_long_offsets = جھوٹ;
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#اگر_ختم
    return_trace (serialize (c->serializer, glyph_vars, it, axis_گنتی, عدد_glyphs, force_long_offsets));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned)
      return_trace (جھوٹ);

    اگر (c->plan->normalized_coords)
      return_trace (instantiate (c));

    غیرعلامتی glyph_گنتی = نسخہ.مقصد_int () ? c->plan->ماخذ->get_عدد_glyphs () : 0;

    gvar_GVAR *برآمد = c->serializer->allocate_min<gvar_GVAR> ();
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->نسخہ.major = 1;
    برآمد->نسخہ.minor = 0;
    برآمد->axis_گنتی_2 = axis_گنتی_2;
    برآمد->shared_Tuple_گنتی = shared_Tuple_گنتی;

    غیرعلامتی صحیح عدد_glyphs = c->plan->عدد_output_glyphs ();
    برآمد->glyph_گنتی_X = hb_min (glyph_گنتی_max (), عدد_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_سمتیہ_t<hb_سمتیہ_t<حرف>> culled_vars;
    hb_سیٹ_t culled_gids;
    اگر (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (یہ+sharedTuples).as_array ((غیرعلامتی) shared_Tuple_گنتی * (غیرعلامتی) axis_گنتی_2);
      اگر (unlikely (!culled_vars.resize (عدد_glyphs))) return_trace (جھوٹ);
      خودکار cull_it = hb_iter (c->plan->new_مقصد_old_gid_list);
      اگر (cull_it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      برائے (خودکار &_ : cull_it)
      {
	hb_بائٹیں_t var_ڈیٹا_بائٹیں = get_glyph_var_ڈیٹا_بائٹیں (c->ماخذ_blob, glyph_گنتی, _.second);
	اگر (var_ڈیٹا_بائٹیں.لمبائی < Glyph_Variation_ڈیٹا::min_سائز) جاری;
	منطقی بدلا = جھوٹ;
	اگر (unlikely (!var_ڈیٹا_بائٹیں.as<Glyph_Variation_ڈیٹا> ()
		      ->cull_tuple_variations (var_ڈیٹا_بائٹیں, axis_گنتی_2,
					       shared_tuples_array,
					       &c->plan->axes_old_اشاریہ_لیبل_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.پہلا], &بدلا)))
	  return_trace (جھوٹ);
	اگر (بدلا) culled_gids.جوڑو (_.پہلا);
      }
      اگر (unlikely (culled_gids.اندر_error ())) return_trace (جھوٹ);
    }

    خودکار it = hb_iter (c->plan->new_مقصد_old_gid_list);
    اگر (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    غیرعلامتی subset_ڈیٹا_سائز = 0;
    غیرعلامتی padding_سائز = 0;
    برائے (خودکار &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      غیرعلامتی glyph_ڈیٹا_سائز = culled_gids.has (_.پہلا)
				 ? culled_vars[_.پہلا].لمبائی
				 : get_glyph_var_ڈیٹا_بائٹیں (c->ماخذ_blob, glyph_گنتی, old_gid).لمبائی;
      اگر (glyph_ڈیٹا_سائز % 2)
      {
        glyph_ڈیٹا_سائز++;
        padding_سائز++;
      }

      اگر (unlikely (hb_unsigned_جوڑو_overflows (subset_ڈیٹا_سائز, glyph_ڈیٹا_سائز, &subset_ڈیٹا_سائز)))
	return_trace (جھوٹ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقی long_انحراف = subset_ڈیٹا_سائز > 0x1FFFEu;
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    long_انحراف = long_انحراف || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#اگر_ختم
    برآمد->flags = long_انحراف ? 1 : 0;

    غیرعلامتی subset_offsets_سائز = (long_انحراف ? 4 : 2) * (عدد_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_سائز<HBUINT8> (subset_offsets_سائز, جھوٹ);
    اگر (!subset_offsets) return_trace (جھوٹ);

    /* shared tuples */
    اگر (!shared_Tuple_گنتی || !sharedTuples)
      برآمد->sharedTuples = (حرف *) subset_offsets + subset_offsets_سائز - (حرف *) برآمد;
    ورنہ
    {
      غیرعلامتی صحیح shared_tuple_سائز = F2DOT14::static_سائز * axis_گنتی_2 * shared_Tuple_گنتی;
      F2DOT14 *tuples = c->serializer->allocate_سائز<F2DOT14> (shared_tuple_سائز);
      اگر (!tuples) return_trace (جھوٹ);
      برآمد->sharedTuples = (حرف *) tuples - (حرف *) برآمد;
      hb_memcpy (tuples, یہ+sharedTuples, shared_tuple_سائز);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    اگر (long_انحراف)
      subset_ڈیٹا_سائز -= padding_سائز;
    حرف *subset_ڈیٹا = c->serializer->allocate_سائز<حرف> (subset_ڈیٹا_سائز, جھوٹ);
    اگر (!subset_ڈیٹا) return_trace (جھوٹ);
    برآمد->ڈیٹا_Z = subset_ڈیٹا - (حرف *) برآمد;


    اگر (long_انحراف)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    ورنہ
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    غیرعلامتی صحیح glyph_انحراف = 0;

    hb_codepoint_t آخری = 0;
    it = hb_iter (c->plan->new_مقصد_old_gid_list);
    اگر (it->پہلا == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    برائے (خودکار &_ : it)
    {
      hb_codepoint_t gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;

      اگر (long_انحراف)
	برائے (; آخری < gid; آخری++)
	  ((HBUINT32 *) subset_offsets)[آخری] = glyph_انحراف;
      ورنہ
	برائے (; آخری < gid; آخری++)
	  ((HBUINT16 *) subset_offsets)[آخری] = glyph_انحراف / 2;

      hb_بائٹیں_t var_ڈیٹا_بائٹیں = culled_gids.has (gid)
				  ? hb_بائٹیں_t (culled_vars[gid].arrayZ,
						culled_vars[gid].لمبائی)
				  : get_glyph_var_ڈیٹا_بائٹیں (c->ماخذ_blob,
							      glyph_گنتی,
							      old_gid);

      hb_memcpy (subset_ڈیٹا, var_ڈیٹا_بائٹیں.arrayZ, var_ڈیٹا_بائٹیں.لمبائی);
      غیرعلامتی glyph_ڈیٹا_سائز = var_ڈیٹا_بائٹیں.لمبائی;
      subset_ڈیٹا += glyph_ڈیٹا_سائز;
      glyph_انحراف += glyph_ڈیٹا_سائز;

      اگر (!long_انحراف && (glyph_ڈیٹا_سائز % 2))
      {
        *subset_ڈیٹا = 0;
        subset_ڈیٹا++;
        glyph_انحراف++;
      }

      اگر (long_انحراف)
	((HBUINT32 *) subset_offsets)[gid] = glyph_انحراف;
      ورنہ
	((HBUINT16 *) subset_offsets)[gid] = glyph_انحراف / 2;

      آخری++; // Skip over gid
    }

    اگر (long_انحراف)
      برائے (; آخری < عدد_glyphs; آخری++)
	((HBUINT32 *) subset_offsets)[آخری] = glyph_انحراف;
    ورنہ
      برائے (; آخری < عدد_glyphs; آخری++)
	((HBUINT16 *) subset_offsets)[آخری] = glyph_انحراف / 2;

    return_trace (سچ);
  }

  محفوظ:
  ثابت hb_بائٹیں_t get_glyph_var_ڈیٹا_بائٹیں (hb_blob_t *blob,
					     غیرعلامتی glyph_گنتی,
					     hb_codepoint_t glyph) ثابت
  {
    غیرعلامتی آغاز_انحراف = get_انحراف (glyph_گنتی, glyph);
    غیرعلامتی انتہا_انحراف = get_انحراف (glyph_گنتی, glyph+1);
    اگر (unlikely (انتہا_انحراف < آغاز_انحراف)) واپس hb_بائٹیں_t ();
    غیرعلامتی لمبائی = انتہا_انحراف - آغاز_انحراف;
    hb_بائٹیں_t var_ڈیٹا = blob->as_بائٹیں ().sub_array (((غیرعلامتی) ڈیٹا_Z) + آغاز_انحراف, لمبائی);
    واپس likely (var_ڈیٹا.لمبائی >= Glyph_Variation_ڈیٹا::min_سائز) ? var_ڈیٹا : hb_بائٹیں_t ();
  }

  منطقی is_long_انحراف () ثابت { واپس flags & 1; }

  غیرعلامتی get_انحراف (غیرعلامتی glyph_گنتی, غیرعلامتی i) ثابت
  {
    اگر (unlikely (i > glyph_گنتی)) واپس 0;
    hb_barrier ();
    واپس is_long_انحراف () ? get_long_انحراف_array ()[i] : get_short_انحراف_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_انحراف_array () ثابت { واپس (ثابت HBUINT32 *) &انحراف_Z; }
  ثابت HBUINT16 *get_short_انحراف_array () ثابت { واپس (ثابت HBUINT16 *) &انحراف_Z; }

  عوامی:
  ساخت accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      واپس hb_scalar_cache_t::create (hb_min ((غیرعلامتی) جدول->shared_Tuple_گنتی,
						+Tuple_Variation_سرخی::max_shared_tuple_گنتی));
    }

    ساکن خالی destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقی has_ڈیٹا () ثابت { واپس جدول->has_ڈیٹا (); }

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<gvar_GVAR> (face, T::جدول_لیبل);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_گنتی_2 = جدول->نسخہ.مقصد_int () ? face->get_عدد_glyphs () : 0;
    }
    ~accelerator_t () { جدول.destroy (); }

    نجی:

    ساکن اعشاری infer_delta (ثابت hb_array_t<contour_نقطہ_t> points,
			      ثابت hb_array_t<contour_نقطہ_t> deltas,
			      غیرعلامتی صحیح target, غیرعلامتی صحیح prev, غیرعلامتی صحیح next,
			      اعشاری contour_نقطہ_t::*m)
    {
      اعشاری ہدف_val = points.arrayZ[target].*m;
      اعشاری پچھلا_val = points.arrayZ[prev].*m;
      اعشاری اگلا_val = points.arrayZ[next].*m;
      اعشاری پچھلا_delta =  deltas.arrayZ[prev].*m;
      اعشاری اگلا_delta =  deltas.arrayZ[next].*m;

      اگر (پچھلا_val == اگلا_val)
	واپس (پچھلا_delta == اگلا_delta) ? پچھلا_delta : 0.f;
      ورنہ اگر (ہدف_val <= hb_min (پچھلا_val, اگلا_val))
	واپس (پچھلا_val < اگلا_val) ? پچھلا_delta : اگلا_delta;
      ورنہ اگر (ہدف_val >= hb_max (پچھلا_val, اگلا_val))
	واپس (پچھلا_val > اگلا_val) ? پچھلا_delta : اگلا_delta;

      /* linear interpolation */
      اعشاری r = (ہدف_val - پچھلا_val) / (اگلا_val - پچھلا_val);
      واپس پچھلا_delta + r * (اگلا_delta - پچھلا_delta);
    }

    ساکن غیرعلامتی صحیح اگلا_اشاریہ (غیرعلامتی صحیح i, غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
    { واپس (i >= انتہا) ? آغاز : (i + 1); }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
    سانچہ <منطقی is_x>
#اگر_ختم
    ساکن منطقی decompile_deltas_جوڑو_مقصد_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_نقطہ_t> points,
						اعشاری scalar,
						ثابت HBUINT8 *انتہا,
						غیرعلامتی آغاز
#اگر_تعریف_شدہ HB_OPTIMIZE_سائز
						, منطقی is_x
#اگر_ختم
						)
    {
      غیرعلامتی i = 0;
      غیرعلامتی گنتی = points.لمبائی;
      جب (i < گنتی)
      {
	اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
	غیرعلامتی control = *p++;
	غیرعلامتی تسلسل_گنتی = (control & Tuple_اقدار::قدر_تسلسل_گنتی_نقاب) + 1;
	غیرعلامتی stop = i + تسلسل_گنتی;
	اگر (unlikely (stop > گنتی)) واپس جھوٹ;

	غیرعلامتی skip = i < آغاز ? hb_min (آغاز - i, تسلسل_گنتی) : 0;
	i += skip;

	انتخاب (control & Tuple_اقدار::اقدار_سائز_نقاب)
	{
	  حالت Tuple_اقدار::اقدار_ARE_ZEROS:
	    i = stop;
	    توڑو;
	  حالت Tuple_اقدار::اقدار_ARE_الفاظ:
	  {
	    اگر (unlikely (p + تسلسل_گنتی * HBINT16::static_سائز > انتہا)) واپس جھوٹ;
	    p += skip * HBINT16::static_سائز;
	    ثابت خودکار *pp = (ثابت HBINT16 *) p;
	    برائے (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      ورنہ points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  توڑو;
	  حالت Tuple_اقدار::اقدار_ARE_LONGS:
	  {
	    اگر (unlikely (p + تسلسل_گنتی * HBINT32::static_سائز > انتہا)) واپس جھوٹ;
	    p += skip * HBINT32::static_سائز;
	    ثابت خودکار *pp = (ثابت HBINT32 *) p;
	    برائے (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      ورنہ points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  توڑو;
	  حالت Tuple_اقدار::اقدار_ARE_بائٹیں:
	  {
	    اگر (unlikely (p + تسلسل_گنتی > انتہا)) واپس جھوٹ;
	    p += skip * HBINT8::static_سائز;
	    ثابت خودکار *pp = (ثابت HBINT8 *) p;
	    برائے (; i < stop; i++)
	    {
	      اعشاری v = *pp++ * scalar;
	      اگر (is_x) points.arrayZ[i].x += v;
	      ورنہ points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  توڑو;
	}
      }
      واپس سچ;
    }
    عوامی:
    منطقی apply_deltas_مقصد_points (hb_codepoint_t glyph,
				 hb_array_t<ثابت صحیح> coords,
				 ثابت hb_array_t<contour_نقطہ_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
				 منطقی phantom_only = جھوٹ,
				 int64_t *budget = خالی_اشاریہ) ثابت
    {
      اگر (unlikely (glyph >= glyph_گنتی_2)) واپس سچ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_بائٹیں_t var_ڈیٹا_بائٹیں = جدول->get_glyph_var_ڈیٹا_بائٹیں (جدول.get_blob (), glyph_گنتی_2, glyph);
      اگر (!var_ڈیٹا_بائٹیں.as<Glyph_Variation_ڈیٹا> ()->has_ڈیٹا ()) واپس سچ;

      خودکار &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      قسم_نام Glyph_Variation_ڈیٹا::tuple_iterator_t iterator;
      اگر (!Glyph_Variation_ڈیٹا::get_tuple_iterator (var_ڈیٹا_بائٹیں, جدول->axis_گنتی_2,
						   var_ڈیٹا_بائٹیں.arrayZ,
						   shared_indices, &iterator))
	واپس سچ; /* so isn't applied at all */

      منطقی any_نجی_points = جھوٹ;
      منطقی نجی_points_checked = جھوٹ;

      /* Save original points for inferred delta calculation */
      خودکار &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      خودکار orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      خودکار &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      خودکار deltas = deltas_vec.as_array ();

      غیرعلامتی عدد_coords = جدول->axis_گنتی_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (جدول+جدول->sharedTuples).as_array (جدول->shared_Tuple_گنتی * عدد_coords);

      خودکار &نجی_indices = scratch.نجی_indices;
      خودکار &x_deltas = scratch.x_deltas;
      خودکار &y_deltas = scratch.y_deltas;

      غیرعلامتی گنتی = points.لمبائی;
      منطقی flush = جھوٹ;

      کرو
      {
	اعشاری scalar = iterator.current_tuple->calculate_scalar (coords, عدد_coords, shared_tuples,
								 scalar_cache);

	اگر (scalar == 0.f) جاری;

	اگر (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && گنتی >= 4 ? 4 : گنتی)))
	  واپس جھوٹ;

	اگر (!نجی_points_checked)
	{
	  خودکار جائزہ = iterator;
	  کرو
	  {
	    اگر (جائزہ.current_tuple->has_نجی_points ())
	    {
	      any_نجی_points = سچ;
	      توڑو;
	    }
	  } جب (جائزہ.move_مقصد_اگلا ());
	  نجی_points_checked = سچ;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_ڈیٹا ();
	غیرعلامتی صحیح لمبائی = iterator.current_tuple->get_ڈیٹا_سائز ();
	اگر (unlikely (!iterator.var_ڈیٹا_بائٹیں.check_range (p, لمبائی)))
	  واپس جھوٹ;

	اگر (!deltas)
	{
	  اگر (unlikely (!deltas_vec.resize_dirty  (گنتی))) واپس جھوٹ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? گنتی - 4 : 0), 0,
		     (phantom_only ? 4 : گنتی) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *انتہا = p + لمبائی;

	منطقی has_نجی_points = iterator.current_tuple->has_نجی_points ();
	اگر (has_نجی_points &&
	    !Glyph_Variation_ڈیٹا::decompile_points (p, نجی_indices, انتہا))
	  واپس جھوٹ;
	ثابت hb_array_t<غیرعلامتی صحیح> &indices = has_نجی_points ? نجی_indices : shared_indices;

	منطقی apply_مقصد_all = (indices.لمبائی == 0);
	غیرعلامتی عدد_deltas = apply_مقصد_all ? points.لمبائی : indices.لمبائی;
	غیرعلامتی آغاز_deltas = (apply_مقصد_all && phantom_only && عدد_deltas >= 4 ? عدد_deltas - 4 : 0);

	اگر (apply_مقصد_all && !any_نجی_points)
	{
#اگر_تعریف_شدہ HB_OPTIMIZE_سائز
	  اگر (unlikely (!decompile_deltas_جوڑو_مقصد_points (p, points, scalar, انتہا, آغاز_deltas, سچ))) واپس جھوٹ;
	  اگر (unlikely (!decompile_deltas_جوڑو_مقصد_points (p, points, scalar, انتہا, آغاز_deltas, جھوٹ))) واپس جھوٹ;
#ورنہ
	  اگر (unlikely (!decompile_deltas_جوڑو_مقصد_points<سچ> (p, points, scalar, انتہا, آغاز_deltas))) واپس جھوٹ;
	  اگر (unlikely (!decompile_deltas_جوڑو_مقصد_points<جھوٹ> (p, points, scalar, انتہا, آغاز_deltas))) واپس جھوٹ;
#اگر_ختم
	  جاری;
	}

	اگر (unlikely (!x_deltas.resize_dirty  (عدد_deltas))) واپس جھوٹ;
	اگر (unlikely (!Glyph_Variation_ڈیٹا::decompile_deltas (p, x_deltas, انتہا, جھوٹ, آغاز_deltas))) واپس جھوٹ;
	اگر (unlikely (!y_deltas.resize_dirty  (عدد_deltas))) واپس جھوٹ;
	اگر (unlikely (!Glyph_Variation_ڈیٹا::decompile_deltas (p, y_deltas, انتہا, جھوٹ, آغاز_deltas))) واپس جھوٹ;

	اگر (!apply_مقصد_all)
	{
	  اگر (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    اگر (unlikely (orig_points_vec.اندر_error ())) واپس جھوٹ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  اگر (flush)
	  {
	    برائے (غیرعلامتی صحیح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? گنتی - 4 : 0), 0,
		     (phantom_only ? 4 : گنتی) * حجم (deltas[0]));
	}

	اگر (HB_OPTIMIZE_سائز_VAL)
	{
	  برائے (غیرعلامتی صحیح i = 0; i < عدد_deltas; i++)
	  {
	    غیرعلامتی صحیح pt_اشاریہ;
	    اگر (apply_مقصد_all)
	      pt_اشاریہ = i;
	    ورنہ
	    {
	      pt_اشاریہ = indices[i];
	      اگر (unlikely (pt_اشاریہ >= deltas.لمبائی)) جاری;
	    }
	    اگر (phantom_only && pt_اشاریہ < گنتی - 4) جاری;
	    خودکار &delta = deltas.arrayZ[pt_اشاریہ];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	ورنہ
	{
	  /* Ouch. Four cases... for optimization. */
	  اگر (scalar != 1.0f)
	  {
	    اگر (apply_مقصد_all)
	      برائے (غیرعلامتی صحیح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    ورنہ
	      برائے (غیرعلامتی صحیح i = 0; i < عدد_deltas; i++)
	      {
		غیرعلامتی صحیح pt_اشاریہ = indices[i];
		اگر (unlikely (pt_اشاریہ >= deltas.لمبائی)) جاری;
		اگر (phantom_only && pt_اشاریہ < گنتی - 4) جاری;
		خودکار &delta = deltas.arrayZ[pt_اشاریہ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڑو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  ورنہ
	  {
	    اگر (apply_مقصد_all)
	      برائے (غیرعلامتی صحیح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	      {
		خودکار &delta = deltas.arrayZ[i];
		delta.جوڑو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    ورنہ
	      برائے (غیرعلامتی صحیح i = 0; i < عدد_deltas; i++)
	      {
		غیرعلامتی صحیح pt_اشاریہ = indices[i];
		اگر (unlikely (pt_اشاریہ >= deltas.لمبائی)) جاری;
		اگر (phantom_only && pt_اشاریہ < گنتی - 4) جاری;
		خودکار &delta = deltas.arrayZ[pt_اشاریہ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڑو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	اگر (!apply_مقصد_all && !phantom_only)
	{
	  غیرعلامتی آغاز_نقطہ_2 = 0;
	  غیرعلامتی انتہا_نقطہ = 0;
	  جب (سچ)
	  {
	    جب (انتہا_نقطہ < گنتی && !points.arrayZ[انتہا_نقطہ].is_انتہا_نقطہ)
	      انتہا_نقطہ++;
	    اگر (unlikely (انتہا_نقطہ == گنتی)) توڑو;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    غیرعلامتی unref_گنتی = 0;
	    برائے (غیرعلامتی i = آغاز_نقطہ_2; i < انتہا_نقطہ + 1; i++)
	      unref_گنتی += deltas.arrayZ[i].flag;
	    unref_گنتی = (انتہا_نقطہ - آغاز_نقطہ_2 + 1) - unref_گنتی;

	    غیرعلامتی j = آغاز_نقطہ_2;
	    اگر (unref_گنتی == 0 || unref_گنتی > انتہا_نقطہ - آغاز_نقطہ_2)
	      جاؤ no_مزید_gaps;

	    برائے (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      غیرعلامتی صحیح prev, next, i;
	      برائے (;;)
	      {
		i = j;
		j = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
		اگر (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) توڑو;
	      }
	      prev = j = i;
	      برائے (;;)
	      {
		i = j;
		j = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
		اگر (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) توڑو;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      برائے (;;)
	      {
		i = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
		اگر (i == next) توڑو;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطہ_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطہ_t::y);
		اگر (--unref_گنتی == 0) جاؤ no_مزید_gaps;
	      }
	    }
	  no_مزید_gaps:
	    آغاز_نقطہ_2 = انتہا_نقطہ = انتہا_نقطہ + 1;
	  }
	}

	flush = سچ;

      } جب (iterator.move_مقصد_اگلا ());

      اگر (flush)
      {
	برائے (غیرعلامتی صحیح i = phantom_only ? گنتی - 4 : 0; i < گنتی; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      واپس سچ;
    }

    غیرعلامتی صحیح get_axis_گنتی () ثابت { واپس جدول->axis_گنتی_2; }

    نجی:
    hb_blob_ptr_t<gvar_GVAR> جدول;
    غیرعلامتی glyph_گنتی_2;
  };

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_گنتی_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_گنتی;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_مقصد<Unsized_Array_کا<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_گنتی_Type
		glyph_گنتی_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_مقصد<Glyph_Variation_ڈیٹا>
		ڈیٹا_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_کا<HBUINT8>
		انحراف_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  عوامی:
  DEFINE_سائز_ARRAY (18 + Glyph_گنتی_Type::static_سائز, انحراف_Z);
};

ساخت gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_gvar;
};

ساخت GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_GVAR;
};

ساخت gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    اگر (has_ڈیٹا ()) واپس HB_DUAL_GET (*یہ, create_cache ());
    واپس خالی_اشاریہ;
  }

  ساکن خالی destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقی apply_deltas_مقصد_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحیح> coords,
			       ثابت hb_array_t<contour_نقطہ_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
			       منطقی phantom_only = جھوٹ,
			       int64_t *budget = خالی_اشاریہ) ثابت
  {
    واپس HB_DUAL_GET (*یہ, apply_deltas_مقصد_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  غیرعلامتی صحیح get_axis_گنتی () ثابت
  {
    اگر (has_ڈیٹا ()) واپس HB_DUAL_GET (*یہ, get_axis_گنتی ());
    واپس 0;
  }
};

} /* namespace OT */

#اگر_ختم /* HB_OT_VAR_GVAR_TABLE_HH */
