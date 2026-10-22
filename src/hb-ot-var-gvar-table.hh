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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_GVAR_جدول_HH
#وصف_ڪريو HB_OT_VAR_GVAR_جدول_HH

#ضمن "hb-decycler.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#وصف_ڪريو HB_OT_ٽيگ_gvar HB_ٽيگ('g','v','a','r')
#وصف_ڪريو HB_OT_ٽيگ_GVAR HB_ٽيگ('G','V','A','R')

بنية hb_glyf_scratch_t
{
  // glyf
  contour_نقطو_ويڪٽر_t all_points;
  contour_نقطو_ويڪٽر_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_نقطو_ويڪٽر_t orig_points;
  hb_ويڪٽر_t<صحيح> x_deltas;
  hb_ويڪٽر_t<صحيح> y_deltas;
  contour_نقطو_ويڪٽر_t deltas;
  hb_ويڪٽر_t<بنانشان صحيح> shared_indices;
  hb_ويڪٽر_t<بنانشان صحيح> ذاتي_indices;
};

نطاق OT {

سانچو <قسم_نالو وٿي_Type>
بنية glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  استعمال tuple_variations_t = قسم_نالو Tuple_Variation_معلومات<وٿي_Type>::tuple_variations_t;
  استعمال Glyph_Variation_معلومات = Tuple_Variation_معلومات<وٿي_Type>;

  hb_ويڪٽر_t<tuple_variations_t> glyph_variations;

  hb_ويڪٽر_t<F2DOT14> compiled_shared_tuples;
  خاص:
  بنانشان shared_tuples_ڳڻپ = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  عام:
  بنانشان compiled_shared_tuples_ڳڻپ () ثابت
  { أرجع shared_tuples_ڳڻپ; }

  بنانشان compiled_byte_سائيز () ثابت
  {
    بنانشان byte_سائيز = 0;
    لكل (ثابت خودڪار& _ : glyph_variations)
      byte_سائيز += _.get_compiled_byte_سائيز ();

    أرجع byte_سائيز;
  }

  منطقي create_from_glyphs_var_معلومات (بنانشان axis_ڳڻپ,
                                    ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                    ثابت hb_subset_plan_t *plan,
                                    ثابت hb_hashmap_t<hb_codepoint_t, hb_بائيٽون_t>& new_gid_var_معلومات_map)
  {
    إذا (unlikely (!glyph_variations.alloc_exact (plan->new_منزل_old_gid_list.ڊيگهائي)))
      أرجع خطأ;

    خودڪار it = hb_iter (plan->new_منزل_old_gid_list);
    لكل (خودڪار &_ : it)
    {
      hb_codepoint_t new_gid = _.پهريون;
      contour_نقطو_ويڪٽر_t *all_contour_points;
      إذا (!new_gid_var_معلومات_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        أرجع خطأ;
      hb_بائيٽون_t var_معلومات = new_gid_var_معلومات_map.get (new_gid);

      ثابت Glyph_Variation_معلومات* p = reinterpret_cast<ثابت Glyph_Variation_معلومات*> (var_معلومات.arrayZ);
      قسم_نالو Glyph_Variation_معلومات::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_ويڪٽر_t<بنانشان> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      إذا (!var_معلومات || ! p->has_معلومات () || !all_contour_points->ڊيگهائي ||
          !Glyph_Variation_معلومات::get_tuple_iterator (var_معلومات, axis_ڳڻپ,
                                                   var_معلومات.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        استمر;
      }

      منطقي is_مرکب_glyph = خطأ;
      is_مرکب_glyph = plan->مرکب_new_gids.has (new_gid);

      إذا (!p->decompile_tuple_variations (all_contour_points->ڊيگهائي, سچ /* is_gvar */,
                                          iterator, &(plan->axes_old_اشاريو_ٽيگ_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*ٻاهريون*/
					  &pool,
                                          is_مرکب_glyph))
        أرجع خطأ;
      glyph_variations.push (std::move (tuple_vars));
    }
    أرجع !glyph_variations.اندريون_error () && glyph_variations.ڊيگهائي == plan->new_منزل_old_gid_list.ڊيگهائي;
  }

  منطقي instantiate (ثابت hb_subset_plan_t *plan)
  {
    بنانشان ڳڻپ = plan->new_منزل_old_gid_list.ڊيگهائي;
    منطقي iup_optimize = خطأ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      hb_codepoint_t new_gid = plan->new_منزل_old_gid_list[i].پهريون;
      contour_نقطو_ويڪٽر_t *all_points;
      إذا (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        أرجع خطأ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_نقطو_ويڪٽر_t points;
      إذا (iup_optimize)
      {
        points = *all_points;
        إذا (unlikely (points.اندريون_error ())) أرجع خطأ;
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

  منطقي compile_بائيٽون (ثابت hb_map_t& axes_اشاريو_map,
                      ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map)
  {
    إذا (!compile_shared_tuples (axes_اشاريو_map, axes_old_اشاريو_ٽيگ_map))
      أرجع خطأ;
    لكل (tuple_variations_t& vars: glyph_variations)
      إذا (!vars.compile_بائيٽون (axes_اشاريو_map, axes_old_اشاريو_ٽيگ_map,
                               سچ, /* use shared points*/
                               سچ,
                               &shared_tuples_idx_map,
			       &pool))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي compile_shared_tuples (ثابت hb_map_t& axes_اشاريو_map,
                              ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان> coords_ڳڻپ_map;

    /* count the num of shared coords */
    لكل (tuple_variations_t& vars: glyph_variations)
    {
      لكل (tuple_delta_t& var : vars.tuple_vars)
      {
        إذا (!var.compile_coords (axes_اشاريو_map, axes_old_اشاريو_ٽيگ_map, &pool))
          أرجع خطأ;
        بنانشان *ڳڻپ;
	بنانشان hash = hb_hash (&var.compiled_peak_coords);
        إذا (coords_ڳڻپ_map.has_with_hash (&(var.compiled_peak_coords), hash, &ڳڻپ))
	  (*ڳڻپ)++;
        وإلا
          coords_ڳڻپ_map.سيٽ_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    إذا (!coords_ڳڻپ_map || coords_ڳڻپ_map.اندريون_error ())
      أرجع خطأ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_ويڪٽر_t<hb_pair_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان>> shared_coords {
      + hb_iter (coords_ڳڻپ_map)
      | hb_filter ([] (ثابت hb_pair_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان>& p) { أرجع p.second > 1; })
    };
    إذا (unlikely (shared_coords.اندريون_error ())) أرجع خطأ;

    /* no shared tuples: no coords are used more than once */
    إذا (!shared_coords) أرجع سچ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_ڳڻپ = hb_min (0xFFFu + 1, shared_coords.ڊيگهائي);
    بنانشان len = shared_tuples_ڳڻپ * (shared_coords[0].پهريون->ڊيگهائي);
    إذا (unlikely (!compiled_shared_tuples.alloc (len)))
      أرجع خطأ;

    لكل (بنانشان i = 0; i < shared_tuples_ڳڻپ; i++)
    {
      shared_tuples_idx_map.سيٽ (shared_coords[i].پهريون, i);
      /* add a concat() in hb_vector_t? */
      لكل (خودڪار c : shared_coords[i].پهريون->iter ())
        compiled_shared_tuples.push (c);
    }

    أرجع سچ;
  }

  ساكن صحيح _cmp_coords (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_pair_t<hb_ويڪٽر_t<F2DOT14> *, بنانشان> *a = (ثابت hb_pair_t<hb_ويڪٽر_t<F2DOT14> *, بنانشان> *) pa;
    ثابت hb_pair_t<hb_ويڪٽر_t<F2DOT14> *, بنانشان> *b = (ثابت hb_pair_t<hb_ويڪٽر_t<F2DOT14> *, بنانشان> *) pb;

    إذا (a->second != b->second)
      أرجع b->second - a->second; // high to low

    أرجع b->پهريون->as_array().cmp (a->پهريون->as_array ());
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_glyph_var_معلومات (hb_serialize_context_t *c,
                                 Iterator it,
                                 منطقي long_وٿي,
                                 بنانشان انگ_glyphs,
                                 حرف* glyph_var_معلومات_offsets /* OUT: glyph var data offsets array */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (long_وٿي)
    {
      ((HBUINT32 *) glyph_var_معلومات_offsets)[0] = 0;
      glyph_var_معلومات_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) glyph_var_معلومات_offsets)[0] = 0;
      glyph_var_معلومات_offsets += 2;
    }
    بنانشان glyph_وٿي = 0;
    hb_codepoint_t آخري_gid = 0;
    بنانشان idx = 0;

    Glyph_Variation_معلومات* cur_glyph = c->شروعات_embed<Glyph_Variation_معلومات> ();
    إذا (!cur_glyph) return_trace (خطأ);
    لكل (خودڪار &_ : it)
    {
      hb_codepoint_t gid = _.پهريون;
      إذا (long_وٿي)
        لكل (; آخري_gid < gid; آخري_gid++)
          ((HBUINT32 *) glyph_var_معلومات_offsets)[آخري_gid] = glyph_وٿي;
      وإلا
        لكل (; آخري_gid < gid; آخري_gid++)
          ((HBUINT16 *) glyph_var_معلومات_offsets)[آخري_gid] = glyph_وٿي / 2;

      إذا (idx >= glyph_variations.ڊيگهائي) return_trace (خطأ);
      إذا (!cur_glyph->serialize (c, سچ, glyph_variations[idx])) return_trace (خطأ);
      Glyph_Variation_معلومات* ايندڙ_glyph = c->شروعات_embed<Glyph_Variation_معلومات> ();
      glyph_وٿي += (حرف *) ايندڙ_glyph - (حرف *) cur_glyph;

      إذا (long_وٿي)
        ((HBUINT32 *) glyph_var_معلومات_offsets)[gid] = glyph_وٿي;
      وإلا
        ((HBUINT16 *) glyph_var_معلومات_offsets)[gid] = glyph_وٿي / 2;

      آخري_gid++;
      idx++;
      cur_glyph = ايندڙ_glyph;
    }

    إذا (long_وٿي)
      لكل (; آخري_gid < انگ_glyphs; آخري_gid++)
        ((HBUINT32 *) glyph_var_معلومات_offsets)[آخري_gid] = glyph_وٿي;
    وإلا
      لكل (; آخري_gid < انگ_glyphs; آخري_gid++)
        ((HBUINT16 *) glyph_var_معلومات_offsets)[آخري_gid] = glyph_وٿي / 2;
    return_trace (سچ);
  }
};

سانچو <قسم_نالو T, قسم_نالو Types>
بنية gvar_GVAR
{
  استعمال Glyph_ڳڻپ_Type = قسم_نالو Types::HBUINT;

  استعمال Glyph_Variation_معلومات = Tuple_Variation_معلومات<Glyph_ڳڻپ_Type>;
  ساكن constexpr بنانشان glyph_ڳڻپ_max ()
  { أرجع (1u << (8 * Glyph_ڳڻپ_Type::static_سائيز)) - 1; }

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int () != 0; }

  منطقي sanitize_shallow (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخو.major == 1) &&
		  sharedTuples.sanitize (c, هذا, axis_ڳڻپ_2 * shared_Tuple_ڳڻپ) &&
		  (is_long_وٿي () ?
		     c->check_array (get_long_وٿي_array (), c->get_انگ_glyphs () + 1) :
		     c->check_array (get_short_وٿي_array (), c->get_انگ_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع sanitize_shallow (c); }

  منطقي decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_ڳڻپ_Type>& glyph_vars /*ٻاهريون*/) ثابت
  {
    hb_hashmap_t<hb_codepoint_t, hb_بائيٽون_t> new_gid_var_معلومات_map;
    خودڪار it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پهريون == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_معلومات_map.سيٽ (0, hb_بائيٽون_t ());
      it++;
    }

    لكل (خودڪار &_ : it)
    {
      hb_codepoint_t new_gid = _.پهريون;
      hb_codepoint_t old_gid = _.second;
      hb_بائيٽون_t var_معلومات_بائيٽون = get_glyph_var_معلومات_بائيٽون (c->ذريعو_blob, glyph_ڳڻپ_X, old_gid);
      new_gid_var_معلومات_map.سيٽ (new_gid, var_معلومات_بائيٽون);
    }

    إذا (new_gid_var_معلومات_map.اندريون_error ()) أرجع خطأ;

    hb_array_t<ثابت F2DOT14> shared_tuples = (هذا+sharedTuples).as_array ((بنانشان) shared_Tuple_ڳڻپ * (بنانشان) axis_ڳڻپ_2);
    أرجع glyph_vars.create_from_glyphs_var_معلومات (axis_ڳڻپ_2, shared_tuples, c->plan, new_gid_var_معلومات_map);
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت glyph_variations_t<Glyph_ڳڻپ_Type>& glyph_vars,
                  Iterator it,
                  بنانشان axis_ڳڻپ,
                  بنانشان انگ_glyphs,
                  منطقي force_long_offsets) ثابت
  {
    TRACE_SERIALIZE (هذا);
    gvar_GVAR *ٻاهريون = c->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->نسخو.major = 1;
    ٻاهريون->نسخو.minor = 0;
    ٻاهريون->axis_ڳڻپ_2 = axis_ڳڻپ;
    ٻاهريون->glyph_ڳڻپ_X = hb_min (glyph_ڳڻپ_max (), انگ_glyphs);

    بنانشان glyph_var_معلومات_سائيز = glyph_vars.compiled_byte_سائيز ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_وٿي = glyph_var_معلومات_سائيز > 0x1FFFEu || force_long_offsets;
    ٻاهريون->flags = long_وٿي ? 1 : 0;

    بنانشان glyph_var_معلومات_offsets_سائيز = (long_وٿي ? 4 : 2) * (انگ_glyphs + 1);
    HBUINT8 *glyph_var_معلومات_offsets = c->allocate_سائيز<HBUINT8> (glyph_var_معلومات_offsets_سائيز, خطأ);
    إذا (!glyph_var_معلومات_offsets) return_trace (خطأ);

    /* shared tuples */
    بنانشان shared_tuple_ڳڻپ = glyph_vars.compiled_shared_tuples_ڳڻپ ();
    ٻاهريون->shared_Tuple_ڳڻپ = shared_tuple_ڳڻپ;

    إذا (!shared_tuple_ڳڻپ)
      ٻاهريون->sharedTuples = (حرف *) glyph_var_معلومات_offsets + glyph_var_معلومات_offsets_سائيز - (حرف *) ٻاهريون;
    وإلا
    {
      hb_array_t<ثابت F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      إذا (!shared_tuples.arrayZ) return_trace (خطأ);
      ٻاهريون->sharedTuples = (ثابت حرف *) shared_tuples.arrayZ - (حرف *) ٻاهريون;
    }

    حرف *glyph_var_معلومات = c->شروعات_embed<حرف> ();
    إذا (!glyph_var_معلومات) return_trace (خطأ);
    ٻاهريون->معلومات_Z = glyph_var_معلومات - (حرف *) ٻاهريون;

    return_trace (glyph_vars.serialize_glyph_var_معلومات (c, it, long_وٿي, انگ_glyphs,
                                                       (حرف *) glyph_var_معلومات_offsets));
  }

  منطقي instantiate (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    glyph_variations_t<Glyph_ڳڻپ_Type> glyph_vars;
    إذا (!decompile_glyph_variations (c, glyph_vars))
      return_trace (خطأ);

    إذا (!glyph_vars.instantiate (c->plan)) return_trace (خطأ);
    إذا (!glyph_vars.compile_بائيٽون (c->plan->axes_اشاريو_map, c->plan->axes_old_اشاريو_ٽيگ_map))
      return_trace (خطأ);

    بنانشان axis_ڳڻپ = c->plan->axes_اشاريو_map.get_population ();
    بنانشان انگ_glyphs = c->plan->انگ_output_glyphs ();
    خودڪار it = hb_iter (c->plan->new_منزل_old_gid_list);

    منطقي force_long_offsets = خطأ;
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#جيڪڏهن_پورو
    return_trace (serialize (c->serializer, glyph_vars, it, axis_ڳڻپ, انگ_glyphs, force_long_offsets));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    إذا (c->plan->normalized_coords)
      return_trace (instantiate (c));

    بنانشان glyph_ڳڻپ = نسخو.منزل_int () ? c->plan->ذريعو->get_انگ_glyphs () : 0;

    gvar_GVAR *ٻاهريون = c->serializer->allocate_min<gvar_GVAR> ();
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->نسخو.major = 1;
    ٻاهريون->نسخو.minor = 0;
    ٻاهريون->axis_ڳڻپ_2 = axis_ڳڻپ_2;
    ٻاهريون->shared_Tuple_ڳڻپ = shared_Tuple_ڳڻپ;

    بنانشان صحيح انگ_glyphs = c->plan->انگ_output_glyphs ();
    ٻاهريون->glyph_ڳڻپ_X = hb_min (glyph_ڳڻپ_max (), انگ_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_ويڪٽر_t<hb_ويڪٽر_t<حرف>> culled_vars;
    hb_سيٽ_t culled_gids;
    إذا (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ثابت F2DOT14> shared_tuples_array =
	  (هذا+sharedTuples).as_array ((بنانشان) shared_Tuple_ڳڻپ * (بنانشان) axis_ڳڻپ_2);
      إذا (unlikely (!culled_vars.resize (انگ_glyphs))) return_trace (خطأ);
      خودڪار cull_it = hb_iter (c->plan->new_منزل_old_gid_list);
      إذا (cull_it->پهريون == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      لكل (خودڪار &_ : cull_it)
      {
	hb_بائيٽون_t var_معلومات_بائيٽون = get_glyph_var_معلومات_بائيٽون (c->ذريعو_blob, glyph_ڳڻپ, _.second);
	إذا (var_معلومات_بائيٽون.ڊيگهائي < Glyph_Variation_معلومات::min_سائيز) استمر;
	منطقي بدليو = خطأ;
	إذا (unlikely (!var_معلومات_بائيٽون.as<Glyph_Variation_معلومات> ()
		      ->cull_tuple_variations (var_معلومات_بائيٽون, axis_ڳڻپ_2,
					       shared_tuples_array,
					       &c->plan->axes_old_اشاريو_ٽيگ_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.پهريون], &بدليو)))
	  return_trace (خطأ);
	إذا (بدليو) culled_gids.جوڙيو (_.پهريون);
      }
      إذا (unlikely (culled_gids.اندريون_error ())) return_trace (خطأ);
    }

    خودڪار it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پهريون == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    بنانشان subset_معلومات_سائيز = 0;
    بنانشان padding_سائيز = 0;
    لكل (خودڪار &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      بنانشان glyph_معلومات_سائيز = culled_gids.has (_.پهريون)
				 ? culled_vars[_.پهريون].ڊيگهائي
				 : get_glyph_var_معلومات_بائيٽون (c->ذريعو_blob, glyph_ڳڻپ, old_gid).ڊيگهائي;
      إذا (glyph_معلومات_سائيز % 2)
      {
        glyph_معلومات_سائيز++;
        padding_سائيز++;
      }

      إذا (unlikely (hb_unsigned_جوڙيو_overflows (subset_معلومات_سائيز, glyph_معلومات_سائيز, &subset_معلومات_سائيز)))
	return_trace (خطأ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    منطقي long_وٿي = subset_معلومات_سائيز > 0x1FFFEu;
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    long_وٿي = long_وٿي || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#جيڪڏهن_پورو
    ٻاهريون->flags = long_وٿي ? 1 : 0;

    بنانشان subset_offsets_سائيز = (long_وٿي ? 4 : 2) * (انگ_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_سائيز<HBUINT8> (subset_offsets_سائيز, خطأ);
    إذا (!subset_offsets) return_trace (خطأ);

    /* shared tuples */
    إذا (!shared_Tuple_ڳڻپ || !sharedTuples)
      ٻاهريون->sharedTuples = (حرف *) subset_offsets + subset_offsets_سائيز - (حرف *) ٻاهريون;
    وإلا
    {
      بنانشان صحيح shared_tuple_سائيز = F2DOT14::static_سائيز * axis_ڳڻپ_2 * shared_Tuple_ڳڻپ;
      F2DOT14 *tuples = c->serializer->allocate_سائيز<F2DOT14> (shared_tuple_سائيز);
      إذا (!tuples) return_trace (خطأ);
      ٻاهريون->sharedTuples = (حرف *) tuples - (حرف *) ٻاهريون;
      hb_memcpy (tuples, هذا+sharedTuples, shared_tuple_سائيز);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    إذا (long_وٿي)
      subset_معلومات_سائيز -= padding_سائيز;
    حرف *subset_معلومات = c->serializer->allocate_سائيز<حرف> (subset_معلومات_سائيز, خطأ);
    إذا (!subset_معلومات) return_trace (خطأ);
    ٻاهريون->معلومات_Z = subset_معلومات - (حرف *) ٻاهريون;


    إذا (long_وٿي)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    وإلا
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    بنانشان صحيح glyph_وٿي = 0;

    hb_codepoint_t آخري = 0;
    it = hb_iter (c->plan->new_منزل_old_gid_list);
    إذا (it->پهريون == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    لكل (خودڪار &_ : it)
    {
      hb_codepoint_t gid = _.پهريون;
      hb_codepoint_t old_gid = _.second;

      إذا (long_وٿي)
	لكل (; آخري < gid; آخري++)
	  ((HBUINT32 *) subset_offsets)[آخري] = glyph_وٿي;
      وإلا
	لكل (; آخري < gid; آخري++)
	  ((HBUINT16 *) subset_offsets)[آخري] = glyph_وٿي / 2;

      hb_بائيٽون_t var_معلومات_بائيٽون = culled_gids.has (gid)
				  ? hb_بائيٽون_t (culled_vars[gid].arrayZ,
						culled_vars[gid].ڊيگهائي)
				  : get_glyph_var_معلومات_بائيٽون (c->ذريعو_blob,
							      glyph_ڳڻپ,
							      old_gid);

      hb_memcpy (subset_معلومات, var_معلومات_بائيٽون.arrayZ, var_معلومات_بائيٽون.ڊيگهائي);
      بنانشان glyph_معلومات_سائيز = var_معلومات_بائيٽون.ڊيگهائي;
      subset_معلومات += glyph_معلومات_سائيز;
      glyph_وٿي += glyph_معلومات_سائيز;

      إذا (!long_وٿي && (glyph_معلومات_سائيز % 2))
      {
        *subset_معلومات = 0;
        subset_معلومات++;
        glyph_وٿي++;
      }

      إذا (long_وٿي)
	((HBUINT32 *) subset_offsets)[gid] = glyph_وٿي;
      وإلا
	((HBUINT16 *) subset_offsets)[gid] = glyph_وٿي / 2;

      آخري++; // Skip over gid
    }

    إذا (long_وٿي)
      لكل (; آخري < انگ_glyphs; آخري++)
	((HBUINT32 *) subset_offsets)[آخري] = glyph_وٿي;
    وإلا
      لكل (; آخري < انگ_glyphs; آخري++)
	((HBUINT16 *) subset_offsets)[آخري] = glyph_وٿي / 2;

    return_trace (سچ);
  }

  محمي:
  ثابت hb_بائيٽون_t get_glyph_var_معلومات_بائيٽون (hb_blob_t *blob,
					     بنانشان glyph_ڳڻپ,
					     hb_codepoint_t glyph) ثابت
  {
    بنانشان شروعات_وٿي = get_وٿي (glyph_ڳڻپ, glyph);
    بنانشان پڇاڙي_وٿي = get_وٿي (glyph_ڳڻپ, glyph+1);
    إذا (unlikely (پڇاڙي_وٿي < شروعات_وٿي)) أرجع hb_بائيٽون_t ();
    بنانشان ڊيگهائي = پڇاڙي_وٿي - شروعات_وٿي;
    hb_بائيٽون_t var_معلومات = blob->as_بائيٽون ().sub_array (((بنانشان) معلومات_Z) + شروعات_وٿي, ڊيگهائي);
    أرجع likely (var_معلومات.ڊيگهائي >= Glyph_Variation_معلومات::min_سائيز) ? var_معلومات : hb_بائيٽون_t ();
  }

  منطقي is_long_وٿي () ثابت { أرجع flags & 1; }

  بنانشان get_وٿي (بنانشان glyph_ڳڻپ, بنانشان i) ثابت
  {
    إذا (unlikely (i > glyph_ڳڻپ)) أرجع 0;
    hb_barrier ();
    أرجع is_long_وٿي () ? get_long_وٿي_array ()[i] : get_short_وٿي_array ()[i] * 2;
  }

  ثابت HBUINT32 * get_long_وٿي_array () ثابت { أرجع (ثابت HBUINT32 *) &وٿي_Z; }
  ثابت HBUINT16 *get_short_وٿي_array () ثابت { أرجع (ثابت HBUINT16 *) &وٿي_Z; }

  عام:
  بنية accelerator_t
  {

    hb_scalar_cache_t *create_cache () ثابت
    {
      أرجع hb_scalar_cache_t::create (hb_min ((بنانشان) جدول->shared_Tuple_ڳڻپ,
						+Tuple_Variation_سرو::max_shared_tuple_ڳڻپ));
    }

    ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<gvar_GVAR> (face, T::جدول_ٽيگ);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_ڳڻپ_2 = جدول->نسخو.منزل_int () ? face->get_انگ_glyphs () : 0;
    }
    ~accelerator_t () { جدول.destroy (); }

    خاص:

    ساكن عائم infer_delta (ثابت hb_array_t<contour_نقطو_t> points,
			      ثابت hb_array_t<contour_نقطو_t> deltas,
			      بنانشان صحيح target, بنانشان صحيح prev, بنانشان صحيح next,
			      عائم contour_نقطو_t::*m)
    {
      عائم هدف_val = points.arrayZ[target].*m;
      عائم prev_val = points.arrayZ[prev].*m;
      عائم ايندڙ_val = points.arrayZ[next].*m;
      عائم prev_delta =  deltas.arrayZ[prev].*m;
      عائم ايندڙ_delta =  deltas.arrayZ[next].*m;

      إذا (prev_val == ايندڙ_val)
	أرجع (prev_delta == ايندڙ_delta) ? prev_delta : 0.f;
      وإلا إذا (هدف_val <= hb_min (prev_val, ايندڙ_val))
	أرجع (prev_val < ايندڙ_val) ? prev_delta : ايندڙ_delta;
      وإلا إذا (هدف_val >= hb_max (prev_val, ايندڙ_val))
	أرجع (prev_val > ايندڙ_val) ? prev_delta : ايندڙ_delta;

      /* linear interpolation */
      عائم r = (هدف_val - prev_val) / (ايندڙ_val - prev_val);
      أرجع prev_delta + r * (ايندڙ_delta - prev_delta);
    }

    ساكن بنانشان صحيح ايندڙ_اشاريو (بنانشان صحيح i, بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
    { أرجع (i >= پڇاڙي) ? شروعات : (i + 1); }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
    سانچو <منطقي is_x>
#جيڪڏهن_پورو
    ساكن منطقي decompile_deltas_جوڙيو_منزل_points (ثابت HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_نقطو_t> points,
						عائم scalar,
						ثابت HBUINT8 *پڇاڙي,
						بنانشان شروعات
#جيڪڏهن_وصف_ٿيل HB_OPTIMIZE_سائيز
						, منطقي is_x
#جيڪڏهن_پورو
						)
    {
      بنانشان i = 0;
      بنانشان ڳڻپ = points.ڊيگهائي;
      طالما (i < ڳڻپ)
      {
	إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
	بنانشان control = *p++;
	بنانشان لاڳيتو_ڳڻپ = (control & Tuple_قدرون::قدر_لاڳيتو_ڳڻپ_نقاب) + 1;
	بنانشان stop = i + لاڳيتو_ڳڻپ;
	إذا (unlikely (stop > ڳڻپ)) أرجع خطأ;

	بنانشان skip = i < شروعات ? hb_min (شروعات - i, لاڳيتو_ڳڻپ) : 0;
	i += skip;

	بدل (control & Tuple_قدرون::قدرون_سائيز_نقاب)
	{
	  حالة Tuple_قدرون::قدرون_ARE_ZEROS:
	    i = stop;
	    اكسر;
	  حالة Tuple_قدرون::قدرون_ARE_لفظ:
	  {
	    إذا (unlikely (p + لاڳيتو_ڳڻپ * HBINT16::static_سائيز > پڇاڙي)) أرجع خطأ;
	    p += skip * HBINT16::static_سائيز;
	    ثابت خودڪار *pp = (ثابت HBINT16 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_قدرون::قدرون_ARE_LONGS:
	  {
	    إذا (unlikely (p + لاڳيتو_ڳڻپ * HBINT32::static_سائيز > پڇاڙي)) أرجع خطأ;
	    p += skip * HBINT32::static_سائيز;
	    ثابت خودڪار *pp = (ثابت HBINT32 *) p;
	    لكل (; i < stop; i++)
	    {
	      عائم v = *pp++ * scalar;
	      إذا (is_x) points.arrayZ[i].x += v;
	      وإلا points.arrayZ[i].y += v;
	    }
	    p = (ثابت HBUINT8 *) pp;
	  }
	  اكسر;
	  حالة Tuple_قدرون::قدرون_ARE_بائيٽون:
	  {
	    إذا (unlikely (p + لاڳيتو_ڳڻپ > پڇاڙي)) أرجع خطأ;
	    p += skip * HBINT8::static_سائيز;
	    ثابت خودڪار *pp = (ثابت HBINT8 *) p;
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
				 ثابت hb_array_t<contour_نقطو_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالي_اشارو,
				 منطقي phantom_only = خطأ,
				 int64_t *budget = خالي_اشارو) ثابت
    {
      إذا (unlikely (glyph >= glyph_ڳڻپ_2)) أرجع سچ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_بائيٽون_t var_معلومات_بائيٽون = جدول->get_glyph_var_معلومات_بائيٽون (جدول.get_blob (), glyph_ڳڻپ_2, glyph);
      إذا (!var_معلومات_بائيٽون.as<Glyph_Variation_معلومات> ()->has_معلومات ()) أرجع سچ;

      خودڪار &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      قسم_نالو Glyph_Variation_معلومات::tuple_iterator_t iterator;
      إذا (!Glyph_Variation_معلومات::get_tuple_iterator (var_معلومات_بائيٽون, جدول->axis_ڳڻپ_2,
						   var_معلومات_بائيٽون.arrayZ,
						   shared_indices, &iterator))
	أرجع سچ; /* so isn't applied at all */

      منطقي any_ذاتي_points = خطأ;
      منطقي ذاتي_points_checked = خطأ;

      /* Save original points for inferred delta calculation */
      خودڪار &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      خودڪار orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      خودڪار &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      خودڪار deltas = deltas_vec.as_array ();

      بنانشان انگ_coords = جدول->axis_ڳڻپ_2;
      hb_array_t<ثابت F2DOT14> shared_tuples = (جدول+جدول->sharedTuples).as_array (جدول->shared_Tuple_ڳڻپ * انگ_coords);

      خودڪار &ذاتي_indices = scratch.ذاتي_indices;
      خودڪار &x_deltas = scratch.x_deltas;
      خودڪار &y_deltas = scratch.y_deltas;

      بنانشان ڳڻپ = points.ڊيگهائي;
      منطقي flush = خطأ;

      افعل
      {
	عائم scalar = iterator.current_tuple->calculate_scalar (coords, انگ_coords, shared_tuples,
								 scalar_cache);

	إذا (scalar == 0.f) استمر;

	إذا (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && ڳڻپ >= 4 ? 4 : ڳڻپ)))
	  أرجع خطأ;

	إذا (!ذاتي_points_checked)
	{
	  خودڪار جاچ = iterator;
	  افعل
	  {
	    إذا (جاچ.current_tuple->has_ذاتي_points ())
	    {
	      any_ذاتي_points = سچ;
	      اكسر;
	    }
	  } طالما (جاچ.move_منزل_ايندڙ ());
	  ذاتي_points_checked = سچ;
	}
	ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
	بنانشان صحيح ڊيگهائي = iterator.current_tuple->get_معلومات_سائيز ();
	إذا (unlikely (!iterator.var_معلومات_بائيٽون.check_range (p, ڊيگهائي)))
	  أرجع خطأ;

	إذا (!deltas)
	{
	  إذا (unlikely (!deltas_vec.resize_dirty  (ڳڻپ))) أرجع خطأ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? ڳڻپ - 4 : 0), 0,
		     (phantom_only ? 4 : ڳڻپ) * حجم (deltas[0]));
	}

	ثابت HBUINT8 *پڇاڙي = p + ڊيگهائي;

	منطقي has_ذاتي_points = iterator.current_tuple->has_ذاتي_points ();
	إذا (has_ذاتي_points &&
	    !Glyph_Variation_معلومات::decompile_points (p, ذاتي_indices, پڇاڙي))
	  أرجع خطأ;
	ثابت hb_array_t<بنانشان صحيح> &indices = has_ذاتي_points ? ذاتي_indices : shared_indices;

	منطقي apply_منزل_all = (indices.ڊيگهائي == 0);
	بنانشان انگ_deltas = apply_منزل_all ? points.ڊيگهائي : indices.ڊيگهائي;
	بنانشان شروعات_deltas = (apply_منزل_all && phantom_only && انگ_deltas >= 4 ? انگ_deltas - 4 : 0);

	إذا (apply_منزل_all && !any_ذاتي_points)
	{
#جيڪڏهن_وصف_ٿيل HB_OPTIMIZE_سائيز
	  إذا (unlikely (!decompile_deltas_جوڙيو_منزل_points (p, points, scalar, پڇاڙي, شروعات_deltas, سچ))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جوڙيو_منزل_points (p, points, scalar, پڇاڙي, شروعات_deltas, خطأ))) أرجع خطأ;
#وإلا
	  إذا (unlikely (!decompile_deltas_جوڙيو_منزل_points<سچ> (p, points, scalar, پڇاڙي, شروعات_deltas))) أرجع خطأ;
	  إذا (unlikely (!decompile_deltas_جوڙيو_منزل_points<خطأ> (p, points, scalar, پڇاڙي, شروعات_deltas))) أرجع خطأ;
#جيڪڏهن_پورو
	  استمر;
	}

	إذا (unlikely (!x_deltas.resize_dirty  (انگ_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_معلومات::decompile_deltas (p, x_deltas, پڇاڙي, خطأ, شروعات_deltas))) أرجع خطأ;
	إذا (unlikely (!y_deltas.resize_dirty  (انگ_deltas))) أرجع خطأ;
	إذا (unlikely (!Glyph_Variation_معلومات::decompile_deltas (p, y_deltas, پڇاڙي, خطأ, شروعات_deltas))) أرجع خطأ;

	إذا (!apply_منزل_all)
	{
	  إذا (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    إذا (unlikely (orig_points_vec.اندريون_error ())) أرجع خطأ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  إذا (flush)
	  {
	    لكل (بنانشان صحيح i = phantom_only ? ڳڻپ - 4 : 0; i < ڳڻپ; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? ڳڻپ - 4 : 0), 0,
		     (phantom_only ? 4 : ڳڻپ) * حجم (deltas[0]));
	}

	إذا (HB_OPTIMIZE_سائيز_VAL)
	{
	  لكل (بنانشان صحيح i = 0; i < انگ_deltas; i++)
	  {
	    بنانشان صحيح pt_اشاريو;
	    إذا (apply_منزل_all)
	      pt_اشاريو = i;
	    وإلا
	    {
	      pt_اشاريو = indices[i];
	      إذا (unlikely (pt_اشاريو >= deltas.ڊيگهائي)) استمر;
	    }
	    إذا (phantom_only && pt_اشاريو < ڳڻپ - 4) استمر;
	    خودڪار &delta = deltas.arrayZ[pt_اشاريو];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.جوڙيو_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	وإلا
	{
	  /* Ouch. Four cases... for optimization. */
	  إذا (scalar != 1.0f)
	  {
	    إذا (apply_منزل_all)
	      لكل (بنانشان صحيح i = phantom_only ? ڳڻپ - 4 : 0; i < ڳڻپ; i++)
	      {
		خودڪار &delta = deltas.arrayZ[i];
		delta.جوڙيو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    وإلا
	      لكل (بنانشان صحيح i = 0; i < انگ_deltas; i++)
	      {
		بنانشان صحيح pt_اشاريو = indices[i];
		إذا (unlikely (pt_اشاريو >= deltas.ڊيگهائي)) استمر;
		إذا (phantom_only && pt_اشاريو < ڳڻپ - 4) استمر;
		خودڪار &delta = deltas.arrayZ[pt_اشاريو];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڙيو_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  وإلا
	  {
	    إذا (apply_منزل_all)
	      لكل (بنانشان صحيح i = phantom_only ? ڳڻپ - 4 : 0; i < ڳڻپ; i++)
	      {
		خودڪار &delta = deltas.arrayZ[i];
		delta.جوڙيو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    وإلا
	      لكل (بنانشان صحيح i = 0; i < انگ_deltas; i++)
	      {
		بنانشان صحيح pt_اشاريو = indices[i];
		إذا (unlikely (pt_اشاريو >= deltas.ڊيگهائي)) استمر;
		إذا (phantom_only && pt_اشاريو < ڳڻپ - 4) استمر;
		خودڪار &delta = deltas.arrayZ[pt_اشاريو];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.جوڙيو_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	إذا (!apply_منزل_all && !phantom_only)
	{
	  بنانشان شروعات_نقطو_2 = 0;
	  بنانشان پڇاڙي_نقطو = 0;
	  طالما (سچ)
	  {
	    طالما (پڇاڙي_نقطو < ڳڻپ && !points.arrayZ[پڇاڙي_نقطو].is_پڇاڙي_نقطو)
	      پڇاڙي_نقطو++;
	    إذا (unlikely (پڇاڙي_نقطو == ڳڻپ)) اكسر;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    بنانشان unref_ڳڻپ = 0;
	    لكل (بنانشان i = شروعات_نقطو_2; i < پڇاڙي_نقطو + 1; i++)
	      unref_ڳڻپ += deltas.arrayZ[i].flag;
	    unref_ڳڻپ = (پڇاڙي_نقطو - شروعات_نقطو_2 + 1) - unref_ڳڻپ;

	    بنانشان j = شروعات_نقطو_2;
	    إذا (unref_ڳڻپ == 0 || unref_ڳڻپ > پڇاڙي_نقطو - شروعات_نقطو_2)
	      اذهب no_وڌيڪ_gaps;

	    لكل (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      بنانشان صحيح prev, next, i;
	      لكل (;;)
	      {
		i = j;
		j = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
		إذا (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) اكسر;
	      }
	      prev = j = i;
	      لكل (;;)
	      {
		i = j;
		j = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
		إذا (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) اكسر;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      لكل (;;)
	      {
		i = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
		إذا (i == next) اكسر;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطو_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_نقطو_t::y);
		إذا (--unref_ڳڻپ == 0) اذهب no_وڌيڪ_gaps;
	      }
	    }
	  no_وڌيڪ_gaps:
	    شروعات_نقطو_2 = پڇاڙي_نقطو = پڇاڙي_نقطو + 1;
	  }
	}

	flush = سچ;

      } طالما (iterator.move_منزل_ايندڙ ());

      إذا (flush)
      {
	لكل (بنانشان صحيح i = phantom_only ? ڳڻپ - 4 : 0; i < ڳڻپ; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      أرجع سچ;
    }

    بنانشان صحيح get_axis_ڳڻپ () ثابت { أرجع جدول->axis_ڳڻپ_2; }

    خاص:
    hb_blob_ptr_t<gvar_GVAR> جدول;
    بنانشان glyph_ڳڻپ_2;
  };

  محمي:
  Fixed_نسخو<>نسخو;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_ڳڻپ_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_ڳڻپ;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_منزل<Unsized_Array_جو<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_ڳڻپ_Type
		glyph_ڳڻپ_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_منزل<Glyph_Variation_معلومات>
		معلومات_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_جو<HBUINT8>
		وٿي_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  عام:
  DEFINE_سائيز_ARRAY (18 + Glyph_ڳڻپ_Type::static_سائيز, وٿي_Z);
};

بنية gvar : gvar_GVAR<gvar, SmallTypes>
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_gvar;
};

بنية GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_GVAR;
};

بنية gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ثابت
  {
    إذا (has_معلومات ()) أرجع HB_DUAL_GET (*هذا, create_cache ());
    أرجع خالي_اشارو;
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  منطقي apply_deltas_منزل_points (hb_codepoint_t glyph,
			       hb_array_t<ثابت صحيح> coords,
			       ثابت hb_array_t<contour_نقطو_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = خالي_اشارو,
			       منطقي phantom_only = خطأ,
			       int64_t *budget = خالي_اشارو) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, apply_deltas_منزل_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  بنانشان صحيح get_axis_ڳڻپ () ثابت
  {
    إذا (has_معلومات ()) أرجع HB_DUAL_GET (*هذا, get_axis_ڳڻپ ());
    أرجع 0;
  }
};

} /* namespace OT */

#جيڪڏهن_پورو /* HB_OT_VAR_GVAR_TABLE_HH */
