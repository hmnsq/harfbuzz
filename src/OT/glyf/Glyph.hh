#که_نه_تعریف_شوی OT_GLYF_GLYPH_HH
#تعریف_کړه OT_GLYF_GLYPH_HH


#ضمن "../../hb-open-type.hh"

#ضمن "GlyphHeader.hh"
#ضمن "SimpleGlyph.hh"
#ضمن "CompositeGlyph.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


تعداد phantom_ټکی_شاخص_t
{
  PHANTOM_کیڼ   = 0,
  PHANTOM_ښی  = 1,
  PHANTOM_سر    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_شمېر  = 4
};

بنية Glyph
{
  تعداد glyph_type_t {
    تشه,
    SIMPLE,
    مرکب,
  };

  عام:
  مرکب_iter_t get_مرکب_iterator () ثابت
  {
    إذا (type != مرکب) أرجع مرکب_iter_t ();
    أرجع مرکب_Glyph (*سرلیک, بایټونه, extended).iter ();
  }

  ثابت hb_بایټونه_t trim_padding () ثابت
  {
    بدل (type) {
    حالة مرکب: أرجع مرکب_Glyph (*سرلیک, بایټونه, extended).trim_padding ();
    حالة SIMPLE:    أرجع SimpleGlyph (*سرلیک, بایټونه).trim_padding ();
    حالة تشه:     أرجع بایټونه;
    افتراضي:        أرجع بایټونه;
    }
  }

  فراغ drop_hints ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرلیک, بایټونه, extended).drop_hints (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرلیک, بایټونه).drop_hints (); أرجع;
    حالة تشه:
      إذا (بایټونه.اوږدتیا >= Glyph_سرلیک::static_اندازه)
        SimpleGlyph (*سرلیک, بایټونه).drop_hints ();
      أرجع;
    }
  }

  فراغ سېټ_overlaps_flag ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرلیک, بایټونه, extended).سېټ_overlaps_flag (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرلیک, بایټونه).سېټ_overlaps_flag (); أرجع;
    حالة تشه:     أرجع;
    }
  }

  فراغ drop_hints_بایټونه (hb_بایټونه_t &dest_پیل, hb_بایټونه_t &dest_پای_ټکی) ثابت
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرلیک, بایټونه, extended).drop_hints_بایټونه (dest_پیل); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرلیک, بایټونه).drop_hints_بایټونه (dest_پیل, dest_پای_ټکی); أرجع;
    حالة تشه:
      dest_پیل = بایټونه;
      dest_پای_ټکی = hb_بایټونه_t ();
      إذا (بایټونه.اوږدتیا >= Glyph_سرلیک::static_اندازه &&
          SimpleGlyph (*سرلیک, بایټونه).has_instructions_اوږدتیا ())
        SimpleGlyph (*سرلیک, بایټونه).drop_hints_بایټونه (dest_پیل, dest_پای_ټکی);
      أرجع;
    }
  }

  منطقي is_مرکب () ثابت
  { أرجع type == مرکب; }

  منطقي get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_ټکی_ویکټور_t &points /*بهر*/) ثابت
  {
    بدل (type) {
    حالة SIMPLE:
      إذا (unlikely (!SimpleGlyph (*سرلیک, بایټونه).get_contour_points (points, extended)))
        أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خپلکاری &توکی : get_مرکب_iterator ())
        إذا (unlikely (!توکی.get_points (points, extended))) أرجع خطأ;
      اكسر;
    }
    حالة تشه:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.اوږدتیا + PHANTOM_شمېر))) أرجع خطأ;
    hb_array_t<contour_ټکی_t> phantoms = points.as_array ().sub_array (points.اوږدتیا - PHANTOM_شمېر, PHANTOM_شمېر);
    {
      // Duplicated code.
      صحيح lsb = 0;
      face->جدول.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرلیک->xMin - lsb;
      HB_ناکارول_شوی صحيح tsb = 0;
#که_نه_تعریف_شوی HB_NO_VERTICAL
      face->جدول.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#د_که_پای
      صحيح v_orig  = (صحيح) سرلیک->yMax + tsb;
      بینښه h_adv = face->جدول.hmtx->get_advance_without_var_unscaled (gid);
      بینښه v_adv =
#که_نه_تعریف_شوی HB_NO_VERTICAL
                       face->جدول.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - face->get_upem ()
#د_که_پای
                       ;
      phantoms[PHANTOM_کیڼ].x = h_delta;
      phantoms[PHANTOM_ښی].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_سر].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }
    أرجع ریښتیا;
  }

  فراغ update_mtx (ثابت hb_subset_plan_t *plan,
                   صحيح xMin, صحيح xMax,
                   صحيح yMin, صحيح yMax,
                   ثابت contour_ټکی_ویکټور_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    إذا (!plan->new_gid_for_old_gid (gid, &new_gid))
      أرجع;

    إذا (type != تشه)
    {
      plan->bounds_پلنوالی_vec[new_gid] = xMax - xMin;
      plan->bounds_لوړوالی_vec[new_gid] = yMax - yMin;
    }

    بینښه len = all_points.اوږدتیا;
    عائم کیڼ_Side_X = all_points[len - 4].x;
    عائم ښی_Side_X = all_points[len - 3].x;
    عائم سر_Side_Y = all_points[len - 2].y;
    عائم bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    نښهدار hori_aw = roundf (ښی_Side_X - کیڼ_Side_X);
    إذا (hori_aw < 0) hori_aw = 0;
    صحيح lsb = roundf (xMin - کیڼ_Side_X);
    plan->hmtx_map.سېټ_with_hash (new_gid, hash, hb_pair ((بینښه) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    إذا (type != تشه && lsb != xMin)
      plan->سر_maxp_info.allXMinIsLsb = خطأ;

    نښهدار vert_aw = roundf (سر_Side_Y - bottomSideY);
    إذا (vert_aw < 0) vert_aw = 0;
    صحيح tsb = roundf (سر_Side_Y - yMax);
    plan->vmtx_map.سېټ_with_hash (new_gid, hash, hb_pair ((بینښه) vert_aw, tsb));
  }

  منطقي compile_سرلیک_بایټونه (ثابت hb_subset_plan_t *plan,
                             ثابت contour_ټکی_ویکټور_t &all_points,
                             hb_بایټونه_t &dest_بایټونه /*بهر*/) ثابت
  {
    Glyph_سرلیک *glyph_سرلیک = تش_اشاره;
    إذا (!plan->pinned_at_default && type != تشه && all_points.اوږدتیا >= 4)
    {
      glyph_سرلیک = (Glyph_سرلیک *) hb_calloc (1, Glyph_سرلیک::static_اندازه);
      إذا (unlikely (!glyph_سرلیک)) أرجع خطأ;
    }

    عائم xMin = 0, xMax = 0;
    عائم yMin = 0, yMax = 0;
    إذا (all_points.اوږدتیا > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      بینښه شمېر = all_points.اوږدتیا - 4;
      لكل (بینښه i = 1; i < شمېر; i++)
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

    إذا (type != تشه)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, rounded_xMin);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin, rounded_yMin);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax, rounded_xMax);
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    إذا (!glyph_سرلیک)
      أرجع ریښتیا;

    glyph_سرلیک->شمېره_د_Contours = سرلیک->شمېره_د_Contours;

    glyph_سرلیک->xMin = rounded_xMin;
    glyph_سرلیک->yMin = rounded_yMin;
    glyph_سرلیک->xMax = rounded_xMax;
    glyph_سرلیک->yMax = rounded_yMax;

    dest_بایټونه = hb_بایټونه_t ((ثابت حرف *)glyph_سرلیک, Glyph_سرلیک::static_اندازه);
    أرجع ریښتیا;
  }

  منطقي compile_بایټونه_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_بایټونه_t &dest_پیل,  /* IN/OUT */
                                  hb_بایټونه_t &dest_پای_ټکی /*بهر*/)
  {
    contour_ټکی_ویکټور_t all_points, points_with_deltas;
    بینښه مرکب_contours = 0;
    سر_maxp_info_t *سر_maxp_info_p = &plan->سر_maxp_info;
    بینښه *مرکب_contours_p = &مرکب_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    إذا (type == تشه ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      سر_maxp_info_p = تش_اشاره;
      مرکب_contours_p = تش_اشاره;
    }

    hb_glyf_scratch_t scratch;
    إذا (!get_points (font, glyf, all_points, scratch, &points_with_deltas, سر_maxp_info_p, مرکب_contours_p, خطأ, خطأ))
      أرجع خطأ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    إذا (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = تشه;
      dest_پیل = hb_بایټونه_t ();
      dest_پای_ټکی = hb_بایټونه_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    إذا (!plan->pinned_at_default)
    {
      بدل (type)
      {
      حالة مرکب:
        إذا (!مرکب_Glyph (*سرلیک, بایټونه, extended).compile_بایټونه_with_deltas (dest_پیل,
										  points_with_deltas,
										  dest_پای_ټکی))
          أرجع خطأ;
        اكسر;
      حالة SIMPLE:
        إذا (!SimpleGlyph (*سرلیک, بایټونه).compile_بایټونه_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_پای_ټکی))
          أرجع خطأ;
        اكسر;
      حالة تشه:
        dest_پیل = hb_بایټونه_t ();
        dest_پای_ټکی = hb_بایټونه_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        إذا (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            بایټونه.اوږدتیا >= Glyph_سرلیک::static_اندازه &&
            سرلیک->شمېره_د_Contours == 0)
        {
          SimpleGlyph simple (*سرلیک, بایټونه);
          بینښه اوږدتیا = simple.instructions_اوږدتیا ();
          إذا (اوږدتیا)
          {
            اوږدتیا = simple.اوږدتیا (اوږدتیا);
            حرف *p = (حرف *) hb_malloc (اوږدتیا);
            إذا (unlikely (!p)) أرجع خطأ;
            hb_memcpy (p, بایټونه.arrayZ, اوږدتیا);
            dest_پیل = hb_بایټونه_t (p, اوږدتیا);
          }
        }
        اكسر;
      }
    }

    إذا (!compile_سرلیک_بایټونه (plan, all_points, dest_پیل))
    {
      dest_پای_ټکی.fini ();
      أرجع خطأ;
    }
    أرجع ریښتیا;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  کينډۍ <ډول_نوم accelerator_t>
  منطقي get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_ټکی_ویکټور_t &all_points /*بهر*/,
		   hb_glyf_scratch_t &scratch,
		   contour_ټکی_ویکټور_t *points_with_deltas = تش_اشاره, /*بهر*/
		   سر_maxp_info_t * سر_maxp_info = تش_اشاره, /*بهر*/
		   بینښه *مرکب_contours = تش_اشاره, /*بهر*/
		   منطقي ښويدنه_points_hori = ریښتیا,
		   منطقي use_my_metrics = ریښتیا,
		   منطقي phantom_only = خطأ,
		   hb_array_t<ثابت صحيح> coords = hb_array_t<ثابت صحيح> (),
		   hb_scalar_cache_t *gvar_cache = تش_اشاره,
		   بینښه صحيح ژوروالی = 0,
		   بینښه *څنډه_شمېر = تش_اشاره,
		   int64_t *budget = تش_اشاره) ثابت
  {
    إذا (unlikely (ژوروالی > HB_MAX_NESTING_LEVEL)) أرجع خطأ;
    بینښه ډېری_څنډه_شمېر = 0;
    إذا (!څنډه_شمېر) څنډه_شمېر = &ډېری_څنډه_شمېر;
    إذا (unlikely (*څنډه_شمېر > HB_MAX_ګراف_څنډه_شمېر)) أرجع خطأ;
    (*څنډه_شمېر)++;

    int64_t ډېری_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ډېری_budget;
    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) أرجع خطأ;

    إذا (سر_maxp_info)
    {
      سر_maxp_info->max_Component_ژوروالی = hb_max (سر_maxp_info->max_Component_ژوروالی, ژوروالی);
    }

    إذا (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->عدد_coords);

    contour_ټکی_ویکټور_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    بینښه old_اوږدتیا = points.اوږدتیا;

    بدل (type) {
    حالة SIMPLE:
      إذا (ژوروالی == 0 && سر_maxp_info)
        سر_maxp_info->maxContours = hb_max (سر_maxp_info->maxContours, (بینښه) سرلیک->شمېره_د_Contours);
      إذا (ژوروالی > 0 && مرکب_contours)
        *مرکب_contours += (بینښه) سرلیک->شمېره_د_Contours;
      إذا (unlikely (!SimpleGlyph (*سرلیک, بایټونه).get_contour_points (all_points, extended, phantom_only, budget)))
	أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خپلکاری &توکی : get_مرکب_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !توکی.get_points (points, extended)))
        {
          points.resize (old_اوږدتیا);
          أرجع خطأ;
        }
      }
      اكسر;
    }
    حالة تشه:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.اوږدتیا + PHANTOM_شمېر))) أرجع خطأ;
    hb_array_t<contour_ټکی_t> phantoms = points.as_array ().sub_array (points.اوږدتیا - PHANTOM_شمېر, PHANTOM_شمېر);
    {
      // Duplicated code.
      صحيح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرلیک->xMin - lsb;
      HB_ناکارول_شوی صحيح tsb = 0;
#که_نه_تعریف_شوی HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#د_که_پای
      صحيح v_orig  = (صحيح) سرلیک->yMax + tsb;
      بینښه h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      بینښه v_adv =
#که_نه_تعریف_شوی HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - font->face->get_upem ()
#د_که_پای
                       ;
      phantoms[PHANTOM_کیڼ].x = h_delta;
      phantoms[PHANTOM_ښی].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_سر].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (hb_any (coords))
    {
      إذا (!glyf_accelerator.gvar->apply_deltas_منزل_points (gid,
							  coords,
							  points.as_array ().sub_array (old_اوږدتیا),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        أرجع خطأ;
    }
#د_که_پای

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    إذا (points_with_deltas != تش_اشاره && ژوروالی == 0 && type == مرکب)
    {
      أكد (old_اوږدتیا == 0);
      *points_with_deltas = points;
    }

    عائم ښويدنه = 0;
    بدل (type) {
    حالة SIMPLE:
      إذا (ژوروالی == 0 && سر_maxp_info)
        سر_maxp_info->maxPoints = hb_max (سر_maxp_info->maxPoints, all_points.اوږدتیا - old_اوږدتیا - 4);
      ښويدنه = phantoms[PHANTOM_کیڼ].x;
      اكسر;
    حالة مرکب:
    {
      hb_decycler_غوټه_t decycler_غوټه (scratch.decycler);

      بینښه صحيح comp_شاخص = 0;
      لكل (خپلکاری &توکی : get_مرکب_iterator ())
      {
	hb_codepoint_t توکی_gid = توکی.get_gid (extended);

        إذا (unlikely (!decycler_غوټه.لیدنه (توکی_gid)))
	{
	  comp_شاخص++;
	  استمر;
	}

	بینښه old_شمېر = all_points.اوږدتیا;

	إذا (unlikely ((!phantom_only || (use_my_metrics && توکی.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (توکی_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    سر_maxp_info,
						    مرکب_contours,
						    ښويدنه_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    ژوروالی + 1,
						    څنډه_شمېر,
						    budget)))
	{
	  points.resize (old_اوږدتیا);
	  أرجع خطأ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.اوږدتیا - PHANTOM_شمېر, PHANTOM_شمېر);

	خپلکاری comp_points = all_points.as_array ().sub_array (old_شمېر);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	إذا (use_my_metrics && توکی.is_use_my_metrics ())
	  لكل (بینښه صحيح i = 0; i < PHANTOM_شمېر; i++)
	    phantoms[i] = comp_points[comp_points.اوږدتیا - PHANTOM_شمېر + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.اوږدتیا)))
	{
	  points.resize (old_اوږدتیا);
	  أرجع خطأ;
	}

	إذا (comp_points) // Empty in case of phantom_only
	{
	  عائم ماتریکس[4];
	  contour_ټکی_t default_trans;
	  توکی.get_transformation (ماتریکس, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  توکی.transform_points (comp_points, ماتریکس, points[old_اوږدتیا + comp_شاخص]);
	}

	إذا (توکی.is_anchored () && !phantom_only)
	{
	  بینښه صحيح p1, p2;
	  توکی.get_anchor_points (p1, p2, extended);
	  إذا (likely (p1 < all_points.اوږدتیا && p2 < comp_points.اوږدتیا))
	  {
	    contour_ټکی_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    توکی.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.اوږدتیا - PHANTOM_شمېر);

	إذا (all_points.اوږدتیا > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_اوږدتیا);
	  أرجع خطأ;
	}

	comp_شاخص++;
      }

      إذا (سر_maxp_info && ژوروالی == 0)
      {
        إذا (مرکب_contours)
          سر_maxp_info->max_مرکب_Contours = hb_max (سر_maxp_info->max_مرکب_Contours, *مرکب_contours);
        سر_maxp_info->max_مرکب_Points = hb_max (سر_maxp_info->max_مرکب_Points, all_points.اوږدتیا);
        سر_maxp_info->maxComponentElements = hb_max (سر_maxp_info->maxComponentElements, comp_شاخص);
      }
      all_points.extend (phantoms);
      ښويدنه = phantoms[PHANTOM_کیڼ].x;
      points.resize (old_اوږدتیا);
    } اكسر;
    حالة تشه:
      all_points.extend (phantoms);
      ښويدنه = phantoms[PHANTOM_کیڼ].x;
      points.resize (old_اوږدتیا);
      اكسر;
    }

    إذا (ژوروالی == 0 && ښويدنه_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      إذا (ښويدنه)
        لكل (خپلکاری &ټکی : all_points)
	  ټکی.x -= ښويدنه;
    }

    أرجع !all_points.دننه_error ();
  }

  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    إذا (type == تشه)
    {
      *extents = {0, 0, 0, 0};
      أرجع ریښتیا; /* Empty glyph; zero extents. */
    }
    أرجع سرلیک->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_بایټونه_t get_بایټونه () ثابت { أرجع بایټونه; }
  glyph_type_t get_type () ثابت { أرجع type; }
  ثابت Glyph_سرلیک *get_سرلیک () ثابت { أرجع سرلیک; }
  منطقي is_extended () ثابت { أرجع extended; }

  Glyph () : بایټونه (),
             سرلیک (بایټونه.as<Glyph_سرلیک> ()),
             gid (-1),
             type(تشه),
	     extended (خطأ)
  {}

  Glyph (hb_بایټونه_t بایټونه_2,
	 hb_codepoint_t gid_,
	 منطقي extended_) : بایټونه (بایټونه_2),
			   سرلیک (بایټونه.as<Glyph_سرلیک> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحيح عدد_contours = سرلیک->شمېره_د_Contours;
    إذا (unlikely (عدد_contours == 0)) type = تشه;
    وإلا إذا (عدد_contours > 0) type = SIMPLE;
    وإلا إذا (عدد_contours <= -1) type = مرکب;
    وإلا type = تشه; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محمي:
  hb_بایټونه_t بایټونه;
  ثابت Glyph_سرلیک *سرلیک;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_GLYPH_HH */
