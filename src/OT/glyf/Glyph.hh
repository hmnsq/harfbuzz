#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_GLYPH_HH
#وصف_ڪريو OT_GLYF_GLYPH_HH


#ضمن "../../hb-open-type.hh"

#ضمن "GlyphHeader.hh"
#ضمن "SimpleGlyph.hh"
#ضمن "CompositeGlyph.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


تعداد phantom_نقطو_اشاريو_t
{
  PHANTOM_کاٻو   = 0,
  PHANTOM_ساڄو  = 1,
  PHANTOM_چوٽي    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_ڳڻپ  = 4
};

بنية Glyph
{
  تعداد glyph_type_t {
    خالي_جايون,
    SIMPLE,
    مرکب,
  };

  عام:
  مرکب_iter_t get_مرکب_iterator () ثابت
  {
    إذا (type != مرکب) أرجع مرکب_iter_t ();
    أرجع مرکب_Glyph (*سرو, بائيٽون, extended).iter ();
  }

  ثابت hb_بائيٽون_t trim_padding () ثابت
  {
    بدل (type) {
    حالة مرکب: أرجع مرکب_Glyph (*سرو, بائيٽون, extended).trim_padding ();
    حالة SIMPLE:    أرجع SimpleGlyph (*سرو, بائيٽون).trim_padding ();
    حالة خالي_جايون:     أرجع بائيٽون;
    افتراضي:        أرجع بائيٽون;
    }
  }

  فراغ drop_hints ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرو, بائيٽون, extended).drop_hints (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرو, بائيٽون).drop_hints (); أرجع;
    حالة خالي_جايون:
      إذا (بائيٽون.ڊيگهائي >= Glyph_سرو::static_سائيز)
        SimpleGlyph (*سرو, بائيٽون).drop_hints ();
      أرجع;
    }
  }

  فراغ سيٽ_overlaps_flag ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرو, بائيٽون, extended).سيٽ_overlaps_flag (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرو, بائيٽون).سيٽ_overlaps_flag (); أرجع;
    حالة خالي_جايون:     أرجع;
    }
  }

  فراغ drop_hints_بائيٽون (hb_بائيٽون_t &dest_شروعات, hb_بائيٽون_t &dest_پڇاڙي) ثابت
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرو, بائيٽون, extended).drop_hints_بائيٽون (dest_شروعات); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرو, بائيٽون).drop_hints_بائيٽون (dest_شروعات, dest_پڇاڙي); أرجع;
    حالة خالي_جايون:
      dest_شروعات = بائيٽون;
      dest_پڇاڙي = hb_بائيٽون_t ();
      إذا (بائيٽون.ڊيگهائي >= Glyph_سرو::static_سائيز &&
          SimpleGlyph (*سرو, بائيٽون).has_instructions_ڊيگهائي ())
        SimpleGlyph (*سرو, بائيٽون).drop_hints_بائيٽون (dest_شروعات, dest_پڇاڙي);
      أرجع;
    }
  }

  منطقي is_مرکب () ثابت
  { أرجع type == مرکب; }

  منطقي get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_نقطو_ويڪٽر_t &points /*ٻاهريون*/) ثابت
  {
    بدل (type) {
    حالة SIMPLE:
      إذا (unlikely (!SimpleGlyph (*سرو, بائيٽون).get_contour_points (points, extended)))
        أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودڪار &شي : get_مرکب_iterator ())
        إذا (unlikely (!شي.get_points (points, extended))) أرجع خطأ;
      اكسر;
    }
    حالة خالي_جايون:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.ڊيگهائي + PHANTOM_ڳڻپ))) أرجع خطأ;
    hb_array_t<contour_نقطو_t> phantoms = points.as_array ().sub_array (points.ڊيگهائي - PHANTOM_ڳڻپ, PHANTOM_ڳڻپ);
    {
      // Duplicated code.
      صحيح lsb = 0;
      face->جدول.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرو->xMin - lsb;
      HB_اڻ_استعمال_ٿيل صحيح tsb = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
      face->جدول.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#جيڪڏهن_پورو
      صحيح v_orig  = (صحيح) سرو->yMax + tsb;
      بنانشان h_adv = face->جدول.hmtx->get_advance_without_var_unscaled (gid);
      بنانشان v_adv =
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
                       face->جدول.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - face->get_upem ()
#جيڪڏهن_پورو
                       ;
      phantoms[PHANTOM_کاٻو].x = h_delta;
      phantoms[PHANTOM_ساڄو].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٽي].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }
    أرجع سچ;
  }

  فراغ update_mtx (ثابت hb_subset_plan_t *plan,
                   صحيح xMin, صحيح xMax,
                   صحيح yMin, صحيح yMax,
                   ثابت contour_نقطو_ويڪٽر_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    إذا (!plan->new_gid_for_old_gid (gid, &new_gid))
      أرجع;

    إذا (type != خالي_جايون)
    {
      plan->bounds_ويڪر_vec[new_gid] = xMax - xMin;
      plan->bounds_اوچائي_vec[new_gid] = yMax - yMin;
    }

    بنانشان len = all_points.ڊيگهائي;
    عائم کاٻو_Side_X = all_points[len - 4].x;
    عائم ساڄو_Side_X = all_points[len - 3].x;
    عائم چوٽي_Side_Y = all_points[len - 2].y;
    عائم bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    نشانوارو hori_aw = roundf (ساڄو_Side_X - کاٻو_Side_X);
    إذا (hori_aw < 0) hori_aw = 0;
    صحيح lsb = roundf (xMin - کاٻو_Side_X);
    plan->hmtx_map.سيٽ_with_hash (new_gid, hash, hb_pair ((بنانشان) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    إذا (type != خالي_جايون && lsb != xMin)
      plan->سر_maxp_info.allXMinIsLsb = خطأ;

    نشانوارو vert_aw = roundf (چوٽي_Side_Y - bottomSideY);
    إذا (vert_aw < 0) vert_aw = 0;
    صحيح tsb = roundf (چوٽي_Side_Y - yMax);
    plan->vmtx_map.سيٽ_with_hash (new_gid, hash, hb_pair ((بنانشان) vert_aw, tsb));
  }

  منطقي compile_سرو_بائيٽون (ثابت hb_subset_plan_t *plan,
                             ثابت contour_نقطو_ويڪٽر_t &all_points,
                             hb_بائيٽون_t &dest_بائيٽون /*ٻاهريون*/) ثابت
  {
    Glyph_سرو *glyph_سرو = خالي_اشارو;
    إذا (!plan->pinned_at_default && type != خالي_جايون && all_points.ڊيگهائي >= 4)
    {
      glyph_سرو = (Glyph_سرو *) hb_calloc (1, Glyph_سرو::static_سائيز);
      إذا (unlikely (!glyph_سرو)) أرجع خطأ;
    }

    عائم xMin = 0, xMax = 0;
    عائم yMin = 0, yMax = 0;
    إذا (all_points.ڊيگهائي > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      بنانشان ڳڻپ = all_points.ڊيگهائي - 4;
      لكل (بنانشان i = 1; i < ڳڻپ; i++)
      {
	عائم x = all_points[i].x;
	عائم y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    صحيح rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    صحيح rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    صحيح rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    صحيح rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    إذا (type != خالي_جايون)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, rounded_xMin);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin, rounded_yMin);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax, rounded_xMax);
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    إذا (!glyph_سرو)
      أرجع سچ;

    glyph_سرو->عدد_جو_Contours = سرو->عدد_جو_Contours;

    glyph_سرو->xMin = rounded_xMin;
    glyph_سرو->yMin = rounded_yMin;
    glyph_سرو->xMax = rounded_xMax;
    glyph_سرو->yMax = rounded_yMax;

    dest_بائيٽون = hb_بائيٽون_t ((ثابت حرف *)glyph_سرو, Glyph_سرو::static_سائيز);
    أرجع سچ;
  }

  منطقي compile_بائيٽون_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_بائيٽون_t &dest_شروعات,  /* IN/OUT */
                                  hb_بائيٽون_t &dest_پڇاڙي /*ٻاهريون*/)
  {
    contour_نقطو_ويڪٽر_t all_points, points_with_deltas;
    بنانشان مرکب_contours = 0;
    سر_maxp_info_t *سر_maxp_info_p = &plan->سر_maxp_info;
    بنانشان *مرکب_contours_p = &مرکب_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    إذا (type == خالي_جايون ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      سر_maxp_info_p = خالي_اشارو;
      مرکب_contours_p = خالي_اشارو;
    }

    hb_glyf_scratch_t scratch;
    إذا (!get_points (font, glyf, all_points, scratch, &points_with_deltas, سر_maxp_info_p, مرکب_contours_p, خطأ, خطأ))
      أرجع خطأ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    إذا (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = خالي_جايون;
      dest_شروعات = hb_بائيٽون_t ();
      dest_پڇاڙي = hb_بائيٽون_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    إذا (!plan->pinned_at_default)
    {
      بدل (type)
      {
      حالة مرکب:
        إذا (!مرکب_Glyph (*سرو, بائيٽون, extended).compile_بائيٽون_with_deltas (dest_شروعات,
										  points_with_deltas,
										  dest_پڇاڙي))
          أرجع خطأ;
        اكسر;
      حالة SIMPLE:
        إذا (!SimpleGlyph (*سرو, بائيٽون).compile_بائيٽون_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_پڇاڙي))
          أرجع خطأ;
        اكسر;
      حالة خالي_جايون:
        dest_شروعات = hb_بائيٽون_t ();
        dest_پڇاڙي = hb_بائيٽون_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        إذا (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            بائيٽون.ڊيگهائي >= Glyph_سرو::static_سائيز &&
            سرو->عدد_جو_Contours == 0)
        {
          SimpleGlyph simple (*سرو, بائيٽون);
          بنانشان ڊيگهائي = simple.instructions_ڊيگهائي ();
          إذا (ڊيگهائي)
          {
            ڊيگهائي = simple.ڊيگهائي (ڊيگهائي);
            حرف *p = (حرف *) hb_malloc (ڊيگهائي);
            إذا (unlikely (!p)) أرجع خطأ;
            hb_memcpy (p, بائيٽون.arrayZ, ڊيگهائي);
            dest_شروعات = hb_بائيٽون_t (p, ڊيگهائي);
          }
        }
        اكسر;
      }
    }

    إذا (!compile_سرو_بائيٽون (plan, all_points, dest_شروعات))
    {
      dest_پڇاڙي.fini ();
      أرجع خطأ;
    }
    أرجع سچ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  سانچو <قسم_نالو accelerator_t>
  منطقي get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_نقطو_ويڪٽر_t &all_points /*ٻاهريون*/,
		   hb_glyf_scratch_t &scratch,
		   contour_نقطو_ويڪٽر_t *points_with_deltas = خالي_اشارو, /*ٻاهريون*/
		   سر_maxp_info_t * سر_maxp_info = خالي_اشارو, /*ٻاهريون*/
		   بنانشان *مرکب_contours = خالي_اشارو, /*ٻاهريون*/
		   منطقي سرڪايو_points_hori = سچ,
		   منطقي use_my_metrics = سچ,
		   منطقي phantom_only = خطأ,
		   hb_array_t<ثابت صحيح> coords = hb_array_t<ثابت صحيح> (),
		   hb_scalar_cache_t *gvar_cache = خالي_اشارو,
		   بنانشان صحيح اونهائي = 0,
		   بنانشان *ڪنارو_ڳڻپ = خالي_اشارو,
		   int64_t *budget = خالي_اشارو) ثابت
  {
    إذا (unlikely (اونهائي > HB_MAX_NESTING_LEVEL)) أرجع خطأ;
    بنانشان ڍير_ڪنارو_ڳڻپ = 0;
    إذا (!ڪنارو_ڳڻپ) ڪنارو_ڳڻپ = &ڍير_ڪنارو_ڳڻپ;
    إذا (unlikely (*ڪنارو_ڳڻپ > HB_MAX_گراف_ڪنارو_ڳڻپ)) أرجع خطأ;
    (*ڪنارو_ڳڻپ)++;

    int64_t ڍير_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ڍير_budget;
    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) أرجع خطأ;

    إذا (سر_maxp_info)
    {
      سر_maxp_info->max_Component_اونهائي = hb_max (سر_maxp_info->max_Component_اونهائي, اونهائي);
    }

    إذا (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->انگ_coords);

    contour_نقطو_ويڪٽر_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    بنانشان old_ڊيگهائي = points.ڊيگهائي;

    بدل (type) {
    حالة SIMPLE:
      إذا (اونهائي == 0 && سر_maxp_info)
        سر_maxp_info->maxContours = hb_max (سر_maxp_info->maxContours, (بنانشان) سرو->عدد_جو_Contours);
      إذا (اونهائي > 0 && مرکب_contours)
        *مرکب_contours += (بنانشان) سرو->عدد_جو_Contours;
      إذا (unlikely (!SimpleGlyph (*سرو, بائيٽون).get_contour_points (all_points, extended, phantom_only, budget)))
	أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودڪار &شي : get_مرکب_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !شي.get_points (points, extended)))
        {
          points.resize (old_ڊيگهائي);
          أرجع خطأ;
        }
      }
      اكسر;
    }
    حالة خالي_جايون:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.ڊيگهائي + PHANTOM_ڳڻپ))) أرجع خطأ;
    hb_array_t<contour_نقطو_t> phantoms = points.as_array ().sub_array (points.ڊيگهائي - PHANTOM_ڳڻپ, PHANTOM_ڳڻپ);
    {
      // Duplicated code.
      صحيح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرو->xMin - lsb;
      HB_اڻ_استعمال_ٿيل صحيح tsb = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#جيڪڏهن_پورو
      صحيح v_orig  = (صحيح) سرو->yMax + tsb;
      بنانشان h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      بنانشان v_adv =
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - font->face->get_upem ()
#جيڪڏهن_پورو
                       ;
      phantoms[PHANTOM_کاٻو].x = h_delta;
      phantoms[PHANTOM_ساڄو].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٽي].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (hb_any (coords))
    {
      إذا (!glyf_accelerator.gvar->apply_deltas_منزل_points (gid,
							  coords,
							  points.as_array ().sub_array (old_ڊيگهائي),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        أرجع خطأ;
    }
#جيڪڏهن_پورو

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    إذا (points_with_deltas != خالي_اشارو && اونهائي == 0 && type == مرکب)
    {
      أكد (old_ڊيگهائي == 0);
      *points_with_deltas = points;
    }

    عائم سرڪايو = 0;
    بدل (type) {
    حالة SIMPLE:
      إذا (اونهائي == 0 && سر_maxp_info)
        سر_maxp_info->maxPoints = hb_max (سر_maxp_info->maxPoints, all_points.ڊيگهائي - old_ڊيگهائي - 4);
      سرڪايو = phantoms[PHANTOM_کاٻو].x;
      اكسر;
    حالة مرکب:
    {
      hb_decycler_ڳنڍ_t decycler_ڳنڍ (scratch.decycler);

      بنانشان صحيح comp_اشاريو = 0;
      لكل (خودڪار &شي : get_مرکب_iterator ())
      {
	hb_codepoint_t شي_gid = شي.get_gid (extended);

        إذا (unlikely (!decycler_ڳنڍ.گهمو (شي_gid)))
	{
	  comp_اشاريو++;
	  استمر;
	}

	بنانشان old_ڳڻپ = all_points.ڊيگهائي;

	إذا (unlikely ((!phantom_only || (use_my_metrics && شي.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (شي_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    سر_maxp_info,
						    مرکب_contours,
						    سرڪايو_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    اونهائي + 1,
						    ڪنارو_ڳڻپ,
						    budget)))
	{
	  points.resize (old_ڊيگهائي);
	  أرجع خطأ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.ڊيگهائي - PHANTOM_ڳڻپ, PHANTOM_ڳڻپ);

	خودڪار comp_points = all_points.as_array ().sub_array (old_ڳڻپ);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	إذا (use_my_metrics && شي.is_use_my_metrics ())
	  لكل (بنانشان صحيح i = 0; i < PHANTOM_ڳڻپ; i++)
	    phantoms[i] = comp_points[comp_points.ڊيگهائي - PHANTOM_ڳڻپ + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.ڊيگهائي)))
	{
	  points.resize (old_ڊيگهائي);
	  أرجع خطأ;
	}

	إذا (comp_points) // Empty in case of phantom_only
	{
	  عائم ميٽرڪس[4];
	  contour_نقطو_t default_trans;
	  شي.get_transformation (ميٽرڪس, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  شي.transform_points (comp_points, ميٽرڪس, points[old_ڊيگهائي + comp_اشاريو]);
	}

	إذا (شي.is_anchored () && !phantom_only)
	{
	  بنانشان صحيح p1, p2;
	  شي.get_anchor_points (p1, p2, extended);
	  إذا (likely (p1 < all_points.ڊيگهائي && p2 < comp_points.ڊيگهائي))
	  {
	    contour_نقطو_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    شي.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.ڊيگهائي - PHANTOM_ڳڻپ);

	إذا (all_points.ڊيگهائي > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_ڊيگهائي);
	  أرجع خطأ;
	}

	comp_اشاريو++;
      }

      إذا (سر_maxp_info && اونهائي == 0)
      {
        إذا (مرکب_contours)
          سر_maxp_info->max_مرکب_Contours = hb_max (سر_maxp_info->max_مرکب_Contours, *مرکب_contours);
        سر_maxp_info->max_مرکب_Points = hb_max (سر_maxp_info->max_مرکب_Points, all_points.ڊيگهائي);
        سر_maxp_info->maxComponentElements = hb_max (سر_maxp_info->maxComponentElements, comp_اشاريو);
      }
      all_points.extend (phantoms);
      سرڪايو = phantoms[PHANTOM_کاٻو].x;
      points.resize (old_ڊيگهائي);
    } اكسر;
    حالة خالي_جايون:
      all_points.extend (phantoms);
      سرڪايو = phantoms[PHANTOM_کاٻو].x;
      points.resize (old_ڊيگهائي);
      اكسر;
    }

    إذا (اونهائي == 0 && سرڪايو_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      إذا (سرڪايو)
        لكل (خودڪار &نقطو : all_points)
	  نقطو.x -= سرڪايو;
    }

    أرجع !all_points.اندريون_error ();
  }

  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    إذا (type == خالي_جايون)
    {
      *extents = {0, 0, 0, 0};
      أرجع سچ; /* Empty glyph; zero extents. */
    }
    أرجع سرو->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_بائيٽون_t get_بائيٽون () ثابت { أرجع بائيٽون; }
  glyph_type_t get_type () ثابت { أرجع type; }
  ثابت Glyph_سرو *get_سرو () ثابت { أرجع سرو; }
  منطقي is_extended () ثابت { أرجع extended; }

  Glyph () : بائيٽون (),
             سرو (بائيٽون.as<Glyph_سرو> ()),
             gid (-1),
             type(خالي_جايون),
	     extended (خطأ)
  {}

  Glyph (hb_بائيٽون_t بائيٽون_2,
	 hb_codepoint_t gid_,
	 منطقي extended_) : بائيٽون (بائيٽون_2),
			   سرو (بائيٽون.as<Glyph_سرو> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحيح انگ_contours = سرو->عدد_جو_Contours;
    إذا (unlikely (انگ_contours == 0)) type = خالي_جايون;
    وإلا إذا (انگ_contours > 0) type = SIMPLE;
    وإلا إذا (انگ_contours <= -1) type = مرکب;
    وإلا type = خالي_جايون; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محمي:
  hb_بائيٽون_t بائيٽون;
  ثابت Glyph_سرو *سرو;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_GLYPH_HH */
