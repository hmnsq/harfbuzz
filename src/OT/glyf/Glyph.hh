#جے_بنا_تعریف OT_GLYF_GLYPH_HH
#تعریف_کرو OT_GLYF_GLYPH_HH


#ضمن "../../hb-open-type.hh"

#ضمن "GlyphHeader.hh"
#ضمن "SimpleGlyph.hh"
#ضمن "CompositeGlyph.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


تعداد phantom_نقطہ_اشاریہ_t
{
  PHANTOM_بایاں   = 0,
  PHANTOM_دایاں  = 1,
  PHANTOM_چوٹی    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_گنتی  = 4
};

بنية Glyph
{
  تعداد glyph_type_t {
    خالی_تھاواں,
    SIMPLE,
    مرکب,
  };

  عام:
  مرکب_iter_t get_مرکب_iterator () ثابت
  {
    إذا (type != مرکب) أرجع مرکب_iter_t ();
    أرجع مرکب_Glyph (*سرخی, بائٹاں, extended).iter ();
  }

  ثابت hb_بائٹاں_t trim_padding () ثابت
  {
    بدل (type) {
    حالة مرکب: أرجع مرکب_Glyph (*سرخی, بائٹاں, extended).trim_padding ();
    حالة SIMPLE:    أرجع SimpleGlyph (*سرخی, بائٹاں).trim_padding ();
    حالة خالی_تھاواں:     أرجع بائٹاں;
    افتراضي:        أرجع بائٹاں;
    }
  }

  فراغ drop_hints ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرخی, بائٹاں, extended).drop_hints (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرخی, بائٹاں).drop_hints (); أرجع;
    حالة خالی_تھاواں:
      إذا (بائٹاں.لمائی >= Glyph_سرخی::static_ناپ)
        SimpleGlyph (*سرخی, بائٹاں).drop_hints ();
      أرجع;
    }
  }

  فراغ سیٹ_overlaps_flag ()
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرخی, بائٹاں, extended).سیٹ_overlaps_flag (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرخی, بائٹاں).سیٹ_overlaps_flag (); أرجع;
    حالة خالی_تھاواں:     أرجع;
    }
  }

  فراغ drop_hints_بائٹاں (hb_بائٹاں_t &dest_شروع, hb_بائٹاں_t &dest_سرا) ثابت
  {
    بدل (type) {
    حالة مرکب: مرکب_Glyph (*سرخی, بائٹاں, extended).drop_hints_بائٹاں (dest_شروع); أرجع;
    حالة SIMPLE:    SimpleGlyph (*سرخی, بائٹاں).drop_hints_بائٹاں (dest_شروع, dest_سرا); أرجع;
    حالة خالی_تھاواں:
      dest_شروع = بائٹاں;
      dest_سرا = hb_بائٹاں_t ();
      إذا (بائٹاں.لمائی >= Glyph_سرخی::static_ناپ &&
          SimpleGlyph (*سرخی, بائٹاں).has_instructions_لمائی ())
        SimpleGlyph (*سرخی, بائٹاں).drop_hints_بائٹاں (dest_شروع, dest_سرا);
      أرجع;
    }
  }

  منطقي is_مرکب () ثابت
  { أرجع type == مرکب; }

  منطقي get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_نقطہ_سمتیہ_t &points /*باہرلا*/) ثابت
  {
    بدل (type) {
    حالة SIMPLE:
      إذا (unlikely (!SimpleGlyph (*سرخی, بائٹاں).get_contour_points (points, extended)))
        أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودکار &شے : get_مرکب_iterator ())
        إذا (unlikely (!شے.get_points (points, extended))) أرجع خطأ;
      اكسر;
    }
    حالة خالی_تھاواں:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.لمائی + PHANTOM_گنتی))) أرجع خطأ;
    hb_array_t<contour_نقطہ_t> phantoms = points.as_array ().sub_array (points.لمائی - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحيح lsb = 0;
      face->جدول.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرخی->xMin - lsb;
      HB_بن_ورتیا صحيح tsb = 0;
#جے_بنا_تعریف HB_NO_VERTICAL
      face->جدول.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#جے_انت
      صحيح v_orig  = (صحيح) سرخی->yMax + tsb;
      بنانشانی h_adv = face->جدول.hmtx->get_advance_without_var_unscaled (gid);
      بنانشانی v_adv =
#جے_بنا_تعریف HB_NO_VERTICAL
                       face->جدول.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - face->get_upem ()
#جے_انت
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }
    أرجع سچ;
  }

  فراغ update_mtx (ثابت hb_subset_plan_t *plan,
                   صحيح xMin, صحيح xMax,
                   صحيح yMin, صحيح yMax,
                   ثابت contour_نقطہ_سمتیہ_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    إذا (!plan->new_gid_for_old_gid (gid, &new_gid))
      أرجع;

    إذا (type != خالی_تھاواں)
    {
      plan->bounds_چوڑائی_vec[new_gid] = xMax - xMin;
      plan->bounds_اچائی_vec[new_gid] = yMax - yMin;
    }

    بنانشانی len = all_points.لمائی;
    عائم بایاں_Side_X = all_points[len - 4].x;
    عائم دایاں_Side_X = all_points[len - 3].x;
    عائم چوٹی_Side_Y = all_points[len - 2].y;
    عائم bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    نشانیوالا hori_aw = roundf (دایاں_Side_X - بایاں_Side_X);
    إذا (hori_aw < 0) hori_aw = 0;
    صحيح lsb = roundf (xMin - بایاں_Side_X);
    plan->hmtx_map.سیٹ_with_hash (new_gid, hash, hb_pair ((بنانشانی) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    إذا (type != خالی_تھاواں && lsb != xMin)
      plan->سر_maxp_info.allXMinIsLsb = خطأ;

    نشانیوالا vert_aw = roundf (چوٹی_Side_Y - bottomSideY);
    إذا (vert_aw < 0) vert_aw = 0;
    صحيح tsb = roundf (چوٹی_Side_Y - yMax);
    plan->vmtx_map.سیٹ_with_hash (new_gid, hash, hb_pair ((بنانشانی) vert_aw, tsb));
  }

  منطقي compile_سرخی_بائٹاں (ثابت hb_subset_plan_t *plan,
                             ثابت contour_نقطہ_سمتیہ_t &all_points,
                             hb_بائٹاں_t &dest_بائٹاں /*باہرلا*/) ثابت
  {
    Glyph_سرخی *glyph_سرخی = خالی_سوچک;
    إذا (!plan->pinned_at_default && type != خالی_تھاواں && all_points.لمائی >= 4)
    {
      glyph_سرخی = (Glyph_سرخی *) hb_calloc (1, Glyph_سرخی::static_ناپ);
      إذا (unlikely (!glyph_سرخی)) أرجع خطأ;
    }

    عائم xMin = 0, xMax = 0;
    عائم yMin = 0, yMax = 0;
    إذا (all_points.لمائی > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      بنانشانی گنتی = all_points.لمائی - 4;
      لكل (بنانشانی i = 1; i < گنتی; i++)
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

    إذا (type != خالی_تھاواں)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, rounded_xMin);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin, rounded_yMin);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax, rounded_xMax);
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    إذا (!glyph_سرخی)
      أرجع سچ;

    glyph_سرخی->نمبر_دا_Contours = سرخی->نمبر_دا_Contours;

    glyph_سرخی->xMin = rounded_xMin;
    glyph_سرخی->yMin = rounded_yMin;
    glyph_سرخی->xMax = rounded_xMax;
    glyph_سرخی->yMax = rounded_yMax;

    dest_بائٹاں = hb_بائٹاں_t ((ثابت حرف *)glyph_سرخی, Glyph_سرخی::static_ناپ);
    أرجع سچ;
  }

  منطقي compile_بائٹاں_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_بائٹاں_t &dest_شروع,  /* IN/OUT */
                                  hb_بائٹاں_t &dest_سرا /*باہرلا*/)
  {
    contour_نقطہ_سمتیہ_t all_points, points_with_deltas;
    بنانشانی مرکب_contours = 0;
    سر_maxp_info_t *سر_maxp_info_p = &plan->سر_maxp_info;
    بنانشانی *مرکب_contours_p = &مرکب_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    إذا (type == خالی_تھاواں ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      سر_maxp_info_p = خالی_سوچک;
      مرکب_contours_p = خالی_سوچک;
    }

    hb_glyf_scratch_t scratch;
    إذا (!get_points (font, glyf, all_points, scratch, &points_with_deltas, سر_maxp_info_p, مرکب_contours_p, خطأ, خطأ))
      أرجع خطأ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    إذا (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = خالی_تھاواں;
      dest_شروع = hb_بائٹاں_t ();
      dest_سرا = hb_بائٹاں_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    إذا (!plan->pinned_at_default)
    {
      بدل (type)
      {
      حالة مرکب:
        إذا (!مرکب_Glyph (*سرخی, بائٹاں, extended).compile_بائٹاں_with_deltas (dest_شروع,
										  points_with_deltas,
										  dest_سرا))
          أرجع خطأ;
        اكسر;
      حالة SIMPLE:
        إذا (!SimpleGlyph (*سرخی, بائٹاں).compile_بائٹاں_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_سرا))
          أرجع خطأ;
        اكسر;
      حالة خالی_تھاواں:
        dest_شروع = hb_بائٹاں_t ();
        dest_سرا = hb_بائٹاں_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        إذا (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            بائٹاں.لمائی >= Glyph_سرخی::static_ناپ &&
            سرخی->نمبر_دا_Contours == 0)
        {
          SimpleGlyph simple (*سرخی, بائٹاں);
          بنانشانی لمائی = simple.instructions_لمائی ();
          إذا (لمائی)
          {
            لمائی = simple.لمائی (لمائی);
            حرف *p = (حرف *) hb_malloc (لمائی);
            إذا (unlikely (!p)) أرجع خطأ;
            hb_memcpy (p, بائٹاں.arrayZ, لمائی);
            dest_شروع = hb_بائٹاں_t (p, لمائی);
          }
        }
        اكسر;
      }
    }

    إذا (!compile_سرخی_بائٹاں (plan, all_points, dest_شروع))
    {
      dest_سرا.fini ();
      أرجع خطأ;
    }
    أرجع سچ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  سانچا <قسم_ناں accelerator_t>
  منطقي get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_نقطہ_سمتیہ_t &all_points /*باہرلا*/,
		   hb_glyf_scratch_t &scratch,
		   contour_نقطہ_سمتیہ_t *points_with_deltas = خالی_سوچک, /*باہرلا*/
		   سر_maxp_info_t * سر_maxp_info = خالی_سوچک, /*باہرلا*/
		   بنانشانی *مرکب_contours = خالی_سوچک, /*باہرلا*/
		   منطقي کھسکاؤ_points_hori = سچ,
		   منطقي use_my_metrics = سچ,
		   منطقي phantom_only = خطأ,
		   hb_array_t<ثابت صحيح> coords = hb_array_t<ثابت صحيح> (),
		   hb_scalar_cache_t *gvar_cache = خالی_سوچک,
		   بنانشانی صحيح ڈونگھائی = 0,
		   بنانشانی *کنارا_گنتی = خالی_سوچک,
		   int64_t *budget = خالی_سوچک) ثابت
  {
    إذا (unlikely (ڈونگھائی > HB_MAX_NESTING_LEVEL)) أرجع خطأ;
    بنانشانی ڈھیر_کنارا_گنتی = 0;
    إذا (!کنارا_گنتی) کنارا_گنتی = &ڈھیر_کنارا_گنتی;
    إذا (unlikely (*کنارا_گنتی > HB_MAX_گراف_کنارا_گنتی)) أرجع خطأ;
    (*کنارا_گنتی)++;

    int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ڈھیر_budget;
    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) أرجع خطأ;

    إذا (سر_maxp_info)
    {
      سر_maxp_info->max_Component_ڈونگھائی = hb_max (سر_maxp_info->max_Component_ڈونگھائی, ڈونگھائی);
    }

    إذا (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->عدد_coords);

    contour_نقطہ_سمتیہ_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    بنانشانی old_لمائی = points.لمائی;

    بدل (type) {
    حالة SIMPLE:
      إذا (ڈونگھائی == 0 && سر_maxp_info)
        سر_maxp_info->maxContours = hb_max (سر_maxp_info->maxContours, (بنانشانی) سرخی->نمبر_دا_Contours);
      إذا (ڈونگھائی > 0 && مرکب_contours)
        *مرکب_contours += (بنانشانی) سرخی->نمبر_دا_Contours;
      إذا (unlikely (!SimpleGlyph (*سرخی, بائٹاں).get_contour_points (all_points, extended, phantom_only, budget)))
	أرجع خطأ;
      اكسر;
    حالة مرکب:
    {
      لكل (خودکار &شے : get_مرکب_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !شے.get_points (points, extended)))
        {
          points.resize (old_لمائی);
          أرجع خطأ;
        }
      }
      اكسر;
    }
    حالة خالی_تھاواں:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.لمائی + PHANTOM_گنتی))) أرجع خطأ;
    hb_array_t<contour_نقطہ_t> phantoms = points.as_array ().sub_array (points.لمائی - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحيح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) سرخی->xMin - lsb;
      HB_بن_ورتیا صحيح tsb = 0;
#جے_بنا_تعریف HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#جے_انت
      صحيح v_orig  = (صحيح) سرخی->yMax + tsb;
      بنانشانی h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      بنانشانی v_adv =
#جے_بنا_تعریف HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - font->face->get_upem ()
#جے_انت
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }

#جے_بنا_تعریف HB_NO_VAR
    إذا (hb_any (coords))
    {
      إذا (!glyf_accelerator.gvar->apply_deltas_منزل_points (gid,
							  coords,
							  points.as_array ().sub_array (old_لمائی),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        أرجع خطأ;
    }
#جے_انت

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    إذا (points_with_deltas != خالی_سوچک && ڈونگھائی == 0 && type == مرکب)
    {
      أكد (old_لمائی == 0);
      *points_with_deltas = points;
    }

    عائم کھسکاؤ = 0;
    بدل (type) {
    حالة SIMPLE:
      إذا (ڈونگھائی == 0 && سر_maxp_info)
        سر_maxp_info->maxPoints = hb_max (سر_maxp_info->maxPoints, all_points.لمائی - old_لمائی - 4);
      کھسکاؤ = phantoms[PHANTOM_بایاں].x;
      اكسر;
    حالة مرکب:
    {
      hb_decycler_گرہ_t decycler_گرہ (scratch.decycler);

      بنانشانی صحيح comp_اشاریہ = 0;
      لكل (خودکار &شے : get_مرکب_iterator ())
      {
	hb_codepoint_t شے_gid = شے.get_gid (extended);

        إذا (unlikely (!decycler_گرہ.ویکھو (شے_gid)))
	{
	  comp_اشاریہ++;
	  استمر;
	}

	بنانشانی old_گنتی = all_points.لمائی;

	إذا (unlikely ((!phantom_only || (use_my_metrics && شے.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (شے_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    سر_maxp_info,
						    مرکب_contours,
						    کھسکاؤ_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    ڈونگھائی + 1,
						    کنارا_گنتی,
						    budget)))
	{
	  points.resize (old_لمائی);
	  أرجع خطأ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.لمائی - PHANTOM_گنتی, PHANTOM_گنتی);

	خودکار comp_points = all_points.as_array ().sub_array (old_گنتی);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	إذا (use_my_metrics && شے.is_use_my_metrics ())
	  لكل (بنانشانی صحيح i = 0; i < PHANTOM_گنتی; i++)
	    phantoms[i] = comp_points[comp_points.لمائی - PHANTOM_گنتی + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.لمائی)))
	{
	  points.resize (old_لمائی);
	  أرجع خطأ;
	}

	إذا (comp_points) // Empty in case of phantom_only
	{
	  عائم میٹرکس[4];
	  contour_نقطہ_t default_trans;
	  شے.get_transformation (میٹرکس, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  شے.transform_points (comp_points, میٹرکس, points[old_لمائی + comp_اشاریہ]);
	}

	إذا (شے.is_anchored () && !phantom_only)
	{
	  بنانشانی صحيح p1, p2;
	  شے.get_anchor_points (p1, p2, extended);
	  إذا (likely (p1 < all_points.لمائی && p2 < comp_points.لمائی))
	  {
	    contour_نقطہ_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    شے.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.لمائی - PHANTOM_گنتی);

	إذا (all_points.لمائی > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_لمائی);
	  أرجع خطأ;
	}

	comp_اشاریہ++;
      }

      إذا (سر_maxp_info && ڈونگھائی == 0)
      {
        إذا (مرکب_contours)
          سر_maxp_info->max_مرکب_Contours = hb_max (سر_maxp_info->max_مرکب_Contours, *مرکب_contours);
        سر_maxp_info->max_مرکب_Points = hb_max (سر_maxp_info->max_مرکب_Points, all_points.لمائی);
        سر_maxp_info->maxComponentElements = hb_max (سر_maxp_info->maxComponentElements, comp_اشاریہ);
      }
      all_points.extend (phantoms);
      کھسکاؤ = phantoms[PHANTOM_بایاں].x;
      points.resize (old_لمائی);
    } اكسر;
    حالة خالی_تھاواں:
      all_points.extend (phantoms);
      کھسکاؤ = phantoms[PHANTOM_بایاں].x;
      points.resize (old_لمائی);
      اكسر;
    }

    إذا (ڈونگھائی == 0 && کھسکاؤ_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      إذا (کھسکاؤ)
        لكل (خودکار &نقطہ : all_points)
	  نقطہ.x -= کھسکاؤ;
    }

    أرجع !all_points.وچ_error ();
  }

  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    إذا (type == خالی_تھاواں)
    {
      *extents = {0, 0, 0, 0};
      أرجع سچ; /* Empty glyph; zero extents. */
    }
    أرجع سرخی->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_بائٹاں_t get_بائٹاں () ثابت { أرجع بائٹاں; }
  glyph_type_t get_type () ثابت { أرجع type; }
  ثابت Glyph_سرخی *get_سرخی () ثابت { أرجع سرخی; }
  منطقي is_extended () ثابت { أرجع extended; }

  Glyph () : بائٹاں (),
             سرخی (بائٹاں.as<Glyph_سرخی> ()),
             gid (-1),
             type(خالی_تھاواں),
	     extended (خطأ)
  {}

  Glyph (hb_بائٹاں_t بائٹاں_2,
	 hb_codepoint_t gid_,
	 منطقي extended_) : بائٹاں (بائٹاں_2),
			   سرخی (بائٹاں.as<Glyph_سرخی> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحيح عدد_contours = سرخی->نمبر_دا_Contours;
    إذا (unlikely (عدد_contours == 0)) type = خالی_تھاواں;
    وإلا إذا (عدد_contours > 0) type = SIMPLE;
    وإلا إذا (عدد_contours <= -1) type = مرکب;
    وإلا type = خالی_تھاواں; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محمي:
  hb_بائٹاں_t بائٹاں;
  ثابت Glyph_سرخی *سرخی;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_GLYPH_HH */
