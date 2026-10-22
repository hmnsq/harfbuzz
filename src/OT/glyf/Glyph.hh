#اگر_تعریف_نشده OT_GLYF_GLYPH_HH
#تعریف_کن OT_GLYF_GLYPH_HH


#ضمن "../../hb-open-type.hh"

#ضمن "GlyphHeader.hh"
#ضمن "SimpleGlyph.hh"
#ضمن "CompositeGlyph.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


تعداد phantom_نقطه_شاخص_t
{
  PHANTOM_چپ   = 0,
  PHANTOM_راست  = 1,
  PHANTOM_بالاترین    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_شمار  = 4
};

بنية Glyph
{
  تعداد glyph_type_t {
    خالی,
    SIMPLE,
    مرکب,
  };

  عام:
  مرکب_iter_t get_مرکب_iterator () ثابت
  {
    إذا (type != مرکب) أرجع مرکب_iter_t ();
    أرجع مرکب_Glyph (*سرآیند, بایتها, extended).iter ();
  }

  ثابت hb_بایتها_t trim_padding () ثابت
  {
    بدل (type) {
    حالة مرکب: أرجع مرکب_Glyph (*سرآیند, بایتها, extended).trim_padding ();
    حالة SIMPLE:    أرجع SimpleGlyph (*سرآیند, بایتها).trim_padding ();
    حالة خالی:     أرجع بایتها;
    افتراضي:        أرجع بایتها;
    }
  }

  فراغ drop_hints ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرآیند, بایتها, extended).drop_hints (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرآیند, بایتها).drop_hints (); أرجع;
    حالة خالی:
      إذا (بایتها.درازی >= Glyph_سرآیند::static_اندازه)
        SimpleGlyph (*سرآیند, بایتها).drop_hints ();
      أرجع;
    }
  }

  فراغ مجموعه_overlaps_flag ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرآیند, بایتها, extended).مجموعه_overlaps_flag (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرآیند, بایتها).مجموعه_overlaps_flag (); أرجع;
    حالة خالی:     أرجع;
    }
  }

  فراغ drop_hints_بایتها (hb_بایتها_t &dest_شروع, hb_بایتها_t &dest_انتها) ثابت
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرآیند, بایتها, extended).drop_hints_بایتها (dest_شروع); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرآیند, بایتها).drop_hints_بایتها (dest_شروع, dest_انتها); أرجع;
    حالة خالی:
      dest_شروع = بایتها;
      dest_انتها = hb_بایتها_t ();
      إذا (بایتها.درازی >= Glyph_سرآیند::static_اندازه &&
          SimpleGlyph (*سرآیند, بایتها).has_instructions_درازی ())
        SimpleGlyph (*سرآیند, بایتها).drop_hints_بایتها (dest_شروع, dest_انتها);
      أرجع;
    }
  }

  منطقي is_مرکب () ثابت
  { أرجع type == مرکب; }

  منطقي get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_نقطه_وکتور_t &points /*بیرون*/) ثابت
  {
    بدل (type) {
    حالة SIMPLE:
      إذا (unlikely (!SimpleGlyph (*سرآیند, بایتها).get_contour_points (points, extended)))
        أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودکار &قلم : get_مرکب_iterator ())
        إذا (unlikely (!قلم.get_points (points, extended))) أرجع خطأ;
      اكسر;
    }
    حالة خالی:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.درازی + PHANTOM_شمار))) أرجع خطأ;
    hb_array_t<contour_نقطه_t> phantoms = points.as_array ().sub_array (points.درازی - PHANTOM_شمار, PHANTOM_شمار);
    {
      // Duplicated code.
      صحيح lsb = 0;
      face->جدول.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرآیند->xMin - lsb;
      HB_استفادهنشده صحيح tsb = 0;
#اگر_تعریف_نشده HB_NO_VERTICAL
      face->جدول.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#پایان_اگر
      صحيح v_orig  = (صحيح) سرآیند->yMax + tsb;
      بیعلامت h_adv = face->جدول.hmtx->get_advance_without_var_unscaled (gid);
      بیعلامت v_adv =
#اگر_تعریف_نشده HB_NO_VERTICAL
                       face->جدول.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - face->get_upem ()
#پایان_اگر
                       ;
      phantoms[PHANTOM_چپ].x = h_delta;
      phantoms[PHANTOM_راست].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_بالاترین].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }
    أرجع درست;
  }

  فراغ update_mtx (ثابت hb_subset_plan_t *plan,
                   صحيح xMin, صحيح xMax,
                   صحيح yMin, صحيح yMax,
                   ثابت contour_نقطه_وکتور_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    إذا (!plan->new_gid_for_old_gid (gid, &new_gid))
      أرجع;

    إذا (type != خالی)
    {
      plan->bounds_عرض_vec[new_gid] = xMax - xMin;
      plan->bounds_ارتفاع_vec[new_gid] = yMax - yMin;
    }

    بیعلامت len = all_points.درازی;
    عائم چپ_Side_X = all_points[len - 4].x;
    عائم راست_Side_X = all_points[len - 3].x;
    عائم بالاترین_Side_Y = all_points[len - 2].y;
    عائم bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    علامتدار hori_aw = roundf (راست_Side_X - چپ_Side_X);
    إذا (hori_aw < 0) hori_aw = 0;
    صحيح lsb = roundf (xMin - چپ_Side_X);
    plan->hmtx_map.مجموعه_with_hash (new_gid, hash, hb_pair ((بیعلامت) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    إذا (type != خالی && lsb != xMin)
      plan->سر_maxp_info.allXMinIsLsb = خطأ;

    علامتدار vert_aw = roundf (بالاترین_Side_Y - bottomSideY);
    إذا (vert_aw < 0) vert_aw = 0;
    صحيح tsb = roundf (بالاترین_Side_Y - yMax);
    plan->vmtx_map.مجموعه_with_hash (new_gid, hash, hb_pair ((بیعلامت) vert_aw, tsb));
  }

  منطقي compile_سرآیند_بایتها (ثابت hb_subset_plan_t *plan,
                             ثابت contour_نقطه_وکتور_t &all_points,
                             hb_بایتها_t &dest_بایتها /*بیرون*/) ثابت
  {
    Glyph_سرآیند *glyph_سرآیند = اشاره_تهی;
    إذا (!plan->pinned_at_default && type != خالی && all_points.درازی >= 4)
    {
      glyph_سرآیند = (Glyph_سرآیند *) hb_calloc (1, Glyph_سرآیند::static_اندازه);
      إذا (unlikely (!glyph_سرآیند)) أرجع خطأ;
    }

    عائم xMin = 0, xMax = 0;
    عائم yMin = 0, yMax = 0;
    إذا (all_points.درازی > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      بیعلامت شمار = all_points.درازی - 4;
      لكل (بیعلامت i = 1; i < شمار; i++)
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

    إذا (type != خالی)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, rounded_xMin);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin, rounded_yMin);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax, rounded_xMax);
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    إذا (!glyph_سرآیند)
      أرجع درست;

    glyph_سرآیند->عدد_از_Contours = سرآیند->عدد_از_Contours;

    glyph_سرآیند->xMin = rounded_xMin;
    glyph_سرآیند->yMin = rounded_yMin;
    glyph_سرآیند->xMax = rounded_xMax;
    glyph_سرآیند->yMax = rounded_yMax;

    dest_بایتها = hb_بایتها_t ((ثابت حرف *)glyph_سرآیند, Glyph_سرآیند::static_اندازه);
    أرجع درست;
  }

  منطقي compile_بایتها_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_بایتها_t &dest_شروع,  /* IN/OUT */
                                  hb_بایتها_t &dest_انتها /*بیرون*/)
  {
    contour_نقطه_وکتور_t all_points, points_with_deltas;
    بیعلامت مرکب_contours = 0;
    سر_maxp_info_t *سر_maxp_info_p = &plan->سر_maxp_info;
    بیعلامت *مرکب_contours_p = &مرکب_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    إذا (type == خالی ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      سر_maxp_info_p = اشاره_تهی;
      مرکب_contours_p = اشاره_تهی;
    }

    hb_glyf_scratch_t scratch;
    إذا (!get_points (font, glyf, all_points, scratch, &points_with_deltas, سر_maxp_info_p, مرکب_contours_p, خطأ, خطأ))
      أرجع خطأ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    إذا (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = خالی;
      dest_شروع = hb_بایتها_t ();
      dest_انتها = hb_بایتها_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    إذا (!plan->pinned_at_default)
    {
      بدل (type)
      {
      حالة مرکب:
        إذا (!مرکب_Glyph (*سرآیند, بایتها, extended).compile_بایتها_with_deltas (dest_شروع,
										  points_with_deltas,
										  dest_انتها))
          أرجع خطأ;
        اكسر;
      حالة SIMPLE:
        إذا (!SimpleGlyph (*سرآیند, بایتها).compile_بایتها_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_انتها))
          أرجع خطأ;
        اكسر;
      حالة خالی:
        dest_شروع = hb_بایتها_t ();
        dest_انتها = hb_بایتها_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        إذا (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            بایتها.درازی >= Glyph_سرآیند::static_اندازه &&
            سرآیند->عدد_از_Contours == 0)
        {
          SimpleGlyph simple (*سرآیند, بایتها);
          بیعلامت درازی = simple.instructions_درازی ();
          إذا (درازی)
          {
            درازی = simple.درازی (درازی);
            حرف *p = (حرف *) hb_malloc (درازی);
            إذا (unlikely (!p)) أرجع خطأ;
            hb_memcpy (p, بایتها.arrayZ, درازی);
            dest_شروع = hb_بایتها_t (p, درازی);
          }
        }
        اكسر;
      }
    }

    إذا (!compile_سرآیند_بایتها (plan, all_points, dest_شروع))
    {
      dest_انتها.fini ();
      أرجع خطأ;
    }
    أرجع درست;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  قالب <نام_نوع accelerator_t>
  منطقي get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_نقطه_وکتور_t &all_points /*بیرون*/,
		   hb_glyf_scratch_t &scratch,
		   contour_نقطه_وکتور_t *points_with_deltas = اشاره_تهی, /*بیرون*/
		   سر_maxp_info_t * سر_maxp_info = اشاره_تهی, /*بیرون*/
		   بیعلامت *مرکب_contours = اشاره_تهی, /*بیرون*/
		   منطقي لغزش_points_hori = درست,
		   منطقي use_my_metrics = درست,
		   منطقي phantom_only = خطأ,
		   hb_array_t<ثابت صحيح> coords = hb_array_t<ثابت صحيح> (),
		   hb_scalar_cache_t *gvar_cache = اشاره_تهی,
		   بیعلامت صحيح عمق = 0,
		   بیعلامت *یال_شمار = اشاره_تهی,
		   int64_t *budget = اشاره_تهی) ثابت
  {
    إذا (unlikely (عمق > HB_MAX_NESTING_LEVEL)) أرجع خطأ;
    بیعلامت پشته_یال_شمار = 0;
    إذا (!یال_شمار) یال_شمار = &پشته_یال_شمار;
    إذا (unlikely (*یال_شمار > HB_MAX_گراف_یال_شمار)) أرجع خطأ;
    (*یال_شمار)++;

    int64_t پشته_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &پشته_budget;
    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) أرجع خطأ;

    إذا (سر_maxp_info)
    {
      سر_maxp_info->max_Component_عمق = hb_max (سر_maxp_info->max_Component_عمق, عمق);
    }

    إذا (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->رقم_coords);

    contour_نقطه_وکتور_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    بیعلامت old_درازی = points.درازی;

    بدل (type) {
    حالة SIMPLE:
      إذا (عمق == 0 && سر_maxp_info)
        سر_maxp_info->maxContours = hb_max (سر_maxp_info->maxContours, (بیعلامت) سرآیند->عدد_از_Contours);
      إذا (عمق > 0 && مرکب_contours)
        *مرکب_contours += (بیعلامت) سرآیند->عدد_از_Contours;
      إذا (unlikely (!SimpleGlyph (*سرآیند, بایتها).get_contour_points (all_points, extended, phantom_only, budget)))
	أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودکار &قلم : get_مرکب_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !قلم.get_points (points, extended)))
        {
          points.resize (old_درازی);
          أرجع خطأ;
        }
      }
      اكسر;
    }
    حالة خالی:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.درازی + PHANTOM_شمار))) أرجع خطأ;
    hb_array_t<contour_نقطه_t> phantoms = points.as_array ().sub_array (points.درازی - PHANTOM_شمار, PHANTOM_شمار);
    {
      // Duplicated code.
      صحيح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرآیند->xMin - lsb;
      HB_استفادهنشده صحيح tsb = 0;
#اگر_تعریف_نشده HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#پایان_اگر
      صحيح v_orig  = (صحيح) سرآیند->yMax + tsb;
      بیعلامت h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      بیعلامت v_adv =
#اگر_تعریف_نشده HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - font->face->get_upem ()
#پایان_اگر
                       ;
      phantoms[PHANTOM_چپ].x = h_delta;
      phantoms[PHANTOM_راست].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_بالاترین].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }

#اگر_تعریف_نشده HB_NO_VAR
    إذا (hb_any (coords))
    {
      إذا (!glyf_accelerator.gvar->apply_deltas_مقصد_points (gid,
							  coords,
							  points.as_array ().sub_array (old_درازی),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        أرجع خطأ;
    }
#پایان_اگر

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    إذا (points_with_deltas != اشاره_تهی && عمق == 0 && type == مرکب)
    {
      أكد (old_درازی == 0);
      *points_with_deltas = points;
    }

    عائم لغزش = 0;
    بدل (type) {
    حالة SIMPLE:
      إذا (عمق == 0 && سر_maxp_info)
        سر_maxp_info->maxPoints = hb_max (سر_maxp_info->maxPoints, all_points.درازی - old_درازی - 4);
      لغزش = phantoms[PHANTOM_چپ].x;
      اكسر;
    حالة مرکب:
    {
      hb_decycler_گره_t decycler_گره (scratch.decycler);

      بیعلامت صحيح comp_شاخص = 0;
      لكل (خودکار &قلم : get_مرکب_iterator ())
      {
	hb_codepoint_t قلم_gid = قلم.get_gid (extended);

        إذا (unlikely (!decycler_گره.بازدید (قلم_gid)))
	{
	  comp_شاخص++;
	  استمر;
	}

	بیعلامت old_شمار = all_points.درازی;

	إذا (unlikely ((!phantom_only || (use_my_metrics && قلم.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (قلم_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    سر_maxp_info,
						    مرکب_contours,
						    لغزش_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    عمق + 1,
						    یال_شمار,
						    budget)))
	{
	  points.resize (old_درازی);
	  أرجع خطأ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.درازی - PHANTOM_شمار, PHANTOM_شمار);

	خودکار comp_points = all_points.as_array ().sub_array (old_شمار);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	إذا (use_my_metrics && قلم.is_use_my_metrics ())
	  لكل (بیعلامت صحيح i = 0; i < PHANTOM_شمار; i++)
	    phantoms[i] = comp_points[comp_points.درازی - PHANTOM_شمار + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.درازی)))
	{
	  points.resize (old_درازی);
	  أرجع خطأ;
	}

	إذا (comp_points) // Empty in case of phantom_only
	{
	  عائم ماتریس[4];
	  contour_نقطه_t default_trans;
	  قلم.get_transformation (ماتریس, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  قلم.transform_points (comp_points, ماتریس, points[old_درازی + comp_شاخص]);
	}

	إذا (قلم.is_anchored () && !phantom_only)
	{
	  بیعلامت صحيح p1, p2;
	  قلم.get_anchor_points (p1, p2, extended);
	  إذا (likely (p1 < all_points.درازی && p2 < comp_points.درازی))
	  {
	    contour_نقطه_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    قلم.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.درازی - PHANTOM_شمار);

	إذا (all_points.درازی > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_درازی);
	  أرجع خطأ;
	}

	comp_شاخص++;
      }

      إذا (سر_maxp_info && عمق == 0)
      {
        إذا (مرکب_contours)
          سر_maxp_info->max_مرکب_Contours = hb_max (سر_maxp_info->max_مرکب_Contours, *مرکب_contours);
        سر_maxp_info->max_مرکب_Points = hb_max (سر_maxp_info->max_مرکب_Points, all_points.درازی);
        سر_maxp_info->maxComponentElements = hb_max (سر_maxp_info->maxComponentElements, comp_شاخص);
      }
      all_points.extend (phantoms);
      لغزش = phantoms[PHANTOM_چپ].x;
      points.resize (old_درازی);
    } اكسر;
    حالة خالی:
      all_points.extend (phantoms);
      لغزش = phantoms[PHANTOM_چپ].x;
      points.resize (old_درازی);
      اكسر;
    }

    إذا (عمق == 0 && لغزش_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      إذا (لغزش)
        لكل (خودکار &نقطه : all_points)
	  نقطه.x -= لغزش;
    }

    أرجع !all_points.درون_error ();
  }

  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    إذا (type == خالی)
    {
      *extents = {0, 0, 0, 0};
      أرجع درست; /* Empty glyph; zero extents. */
    }
    أرجع سرآیند->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_بایتها_t get_بایتها () ثابت { أرجع بایتها; }
  glyph_type_t get_type () ثابت { أرجع type; }
  ثابت Glyph_سرآیند *get_سرآیند () ثابت { أرجع سرآیند; }
  منطقي is_extended () ثابت { أرجع extended; }

  Glyph () : بایتها (),
             سرآیند (بایتها.as<Glyph_سرآیند> ()),
             gid (-1),
             type(خالی),
	     extended (خطأ)
  {}

  Glyph (hb_بایتها_t بایتها_2,
	 hb_codepoint_t gid_,
	 منطقي extended_) : بایتها (بایتها_2),
			   سرآیند (بایتها.as<Glyph_سرآیند> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحيح رقم_contours = سرآیند->عدد_از_Contours;
    إذا (unlikely (رقم_contours == 0)) type = خالی;
    وإلا إذا (رقم_contours > 0) type = SIMPLE;
    وإلا إذا (رقم_contours <= -1) type = مرکب;
    وإلا type = خالی; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محمي:
  hb_بایتها_t بایتها;
  ثابت Glyph_سرآیند *سرآیند;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_GLYPH_HH */
