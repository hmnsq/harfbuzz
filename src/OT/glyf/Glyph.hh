#اگر_غیر_تعریف_شدہ OT_GLYF_GLYPH_HH
#تعریف_کرو OT_GLYF_GLYPH_HH


#شامل "../../hb-open-type.hh"

#شامل "GlyphHeader.hh"
#شامل "SimpleGlyph.hh"
#شامل "CompositeGlyph.hh"


نامخلا OT {

ساخت glyf_accelerator_t;

نامخلا glyf_impl {


شمار phantom_نقطہ_اشاریہ_t
{
  PHANTOM_بایاں   = 0,
  PHANTOM_دایاں  = 1,
  PHANTOM_چوٹی    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_گنتی  = 4
};

ساخت Glyph
{
  شمار glyph_type_t {
    خالی_خانے,
    SIMPLE,
    مرکب,
  };

  عوامی:
  مرکب_iter_t get_مرکب_iterator () ثابت
  {
    اگر (type != مرکب) واپس مرکب_iter_t ();
    واپس مرکب_Glyph (*سرخی, بائٹیں, extended).iter ();
  }

  ثابت hb_بائٹیں_t trim_padding () ثابت
  {
    انتخاب (type) {
    حالت مرکب: واپس مرکب_Glyph (*سرخی, بائٹیں, extended).trim_padding ();
    حالت SIMPLE:    واپس SimpleGlyph (*سرخی, بائٹیں).trim_padding ();
    حالت خالی_خانے:     واپس بائٹیں;
    طےشدہ:        واپس بائٹیں;
    }
  }

  خالی drop_hints ()
  {
    انتخاب (type) {
    حالت مرکب: مرکب_Glyph (*سرخی, بائٹیں, extended).drop_hints (); واپس;
    حالت SIMPLE:    SimpleGlyph (*سرخی, بائٹیں).drop_hints (); واپس;
    حالت خالی_خانے:
      اگر (بائٹیں.لمبائی >= Glyph_سرخی::static_سائز)
        SimpleGlyph (*سرخی, بائٹیں).drop_hints ();
      واپس;
    }
  }

  خالی سیٹ_overlaps_flag ()
  {
    انتخاب (type) {
    حالت مرکب: مرکب_Glyph (*سرخی, بائٹیں, extended).سیٹ_overlaps_flag (); واپس;
    حالت SIMPLE:    SimpleGlyph (*سرخی, بائٹیں).سیٹ_overlaps_flag (); واپس;
    حالت خالی_خانے:     واپس;
    }
  }

  خالی drop_hints_بائٹیں (hb_بائٹیں_t &dest_آغاز, hb_بائٹیں_t &dest_انتہا) ثابت
  {
    انتخاب (type) {
    حالت مرکب: مرکب_Glyph (*سرخی, بائٹیں, extended).drop_hints_بائٹیں (dest_آغاز); واپس;
    حالت SIMPLE:    SimpleGlyph (*سرخی, بائٹیں).drop_hints_بائٹیں (dest_آغاز, dest_انتہا); واپس;
    حالت خالی_خانے:
      dest_آغاز = بائٹیں;
      dest_انتہا = hb_بائٹیں_t ();
      اگر (بائٹیں.لمبائی >= Glyph_سرخی::static_سائز &&
          SimpleGlyph (*سرخی, بائٹیں).has_instructions_لمبائی ())
        SimpleGlyph (*سرخی, بائٹیں).drop_hints_بائٹیں (dest_آغاز, dest_انتہا);
      واپس;
    }
  }

  منطقی is_مرکب () ثابت
  { واپس type == مرکب; }

  منطقی get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_نقطہ_سمتیہ_t &points /*برآمد*/) ثابت
  {
    انتخاب (type) {
    حالت SIMPLE:
      اگر (unlikely (!SimpleGlyph (*سرخی, بائٹیں).get_contour_points (points, extended)))
        واپس جھوٹ;
      توڑو;
    حالت مرکب:
    {
      برائے (خودکار &شے : get_مرکب_iterator ())
        اگر (unlikely (!شے.get_points (points, extended))) واپس جھوٹ;
      توڑو;
    }
    حالت خالی_خانے:
      توڑو;
    }

    /* Init phantom points */
    اگر (unlikely (!points.resize (points.لمبائی + PHANTOM_گنتی))) واپس جھوٹ;
    hb_array_t<contour_نقطہ_t> phantoms = points.as_array ().sub_array (points.لمبائی - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحیح lsb = 0;
      face->جدول.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحیح h_delta = (صحیح) سرخی->xMin - lsb;
      HB_غیر_مستعمل صحیح tsb = 0;
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
      face->جدول.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#اگر_ختم
      صحیح v_orig  = (صحیح) سرخی->yMax + tsb;
      غیرعلامتی h_adv = face->جدول.hmtx->get_advance_without_var_unscaled (gid);
      غیرعلامتی v_adv =
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
                       face->جدول.vmtx->get_advance_without_var_unscaled (gid)
#ورنہ
                       - face->get_upem ()
#اگر_ختم
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحیح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحیح) v_adv;
    }
    واپس سچ;
  }

  خالی update_mtx (ثابت hb_subset_plan_t *plan,
                   صحیح xMin, صحیح xMax,
                   صحیح yMin, صحیح yMax,
                   ثابت contour_نقطہ_سمتیہ_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    اگر (!plan->new_gid_for_old_gid (gid, &new_gid))
      واپس;

    اگر (type != خالی_خانے)
    {
      plan->bounds_چوڑائی_vec[new_gid] = xMax - xMin;
      plan->bounds_اونچائی_vec[new_gid] = yMax - yMin;
    }

    غیرعلامتی len = all_points.لمبائی;
    اعشاری بایاں_Side_X = all_points[len - 4].x;
    اعشاری دایاں_Side_X = all_points[len - 3].x;
    اعشاری چوٹی_Side_Y = all_points[len - 2].y;
    اعشاری bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    علامتی hori_aw = roundf (دایاں_Side_X - بایاں_Side_X);
    اگر (hori_aw < 0) hori_aw = 0;
    صحیح lsb = roundf (xMin - بایاں_Side_X);
    plan->hmtx_map.سیٹ_with_hash (new_gid, hash, hb_pair ((غیرعلامتی) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    اگر (type != خالی_خانے && lsb != xMin)
      plan->سر_maxp_info.allXMinIsLsb = جھوٹ;

    علامتی vert_aw = roundf (چوٹی_Side_Y - bottomSideY);
    اگر (vert_aw < 0) vert_aw = 0;
    صحیح tsb = roundf (چوٹی_Side_Y - yMax);
    plan->vmtx_map.سیٹ_with_hash (new_gid, hash, hb_pair ((غیرعلامتی) vert_aw, tsb));
  }

  منطقی compile_سرخی_بائٹیں (ثابت hb_subset_plan_t *plan,
                             ثابت contour_نقطہ_سمتیہ_t &all_points,
                             hb_بائٹیں_t &dest_بائٹیں /*برآمد*/) ثابت
  {
    Glyph_سرخی *glyph_سرخی = خالی_اشاریہ;
    اگر (!plan->pinned_at_default && type != خالی_خانے && all_points.لمبائی >= 4)
    {
      glyph_سرخی = (Glyph_سرخی *) hb_calloc (1, Glyph_سرخی::static_سائز);
      اگر (unlikely (!glyph_سرخی)) واپس جھوٹ;
    }

    اعشاری xMin = 0, xMax = 0;
    اعشاری yMin = 0, yMax = 0;
    اگر (all_points.لمبائی > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      غیرعلامتی گنتی = all_points.لمبائی - 4;
      برائے (غیرعلامتی i = 1; i < گنتی; i++)
      {
	اعشاری x = all_points[i].x;
	اعشاری y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    صحیح rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    صحیح rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    صحیح rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    صحیح rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    اگر (type != خالی_خانے)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, rounded_xMin);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin, rounded_yMin);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax, rounded_xMax);
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    اگر (!glyph_سرخی)
      واپس سچ;

    glyph_سرخی->نمبر_کا_Contours = سرخی->نمبر_کا_Contours;

    glyph_سرخی->xMin = rounded_xMin;
    glyph_سرخی->yMin = rounded_yMin;
    glyph_سرخی->xMax = rounded_xMax;
    glyph_سرخی->yMax = rounded_yMax;

    dest_بائٹیں = hb_بائٹیں_t ((ثابت حرف *)glyph_سرخی, Glyph_سرخی::static_سائز);
    واپس سچ;
  }

  منطقی compile_بائٹیں_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_بائٹیں_t &dest_آغاز,  /* IN/OUT */
                                  hb_بائٹیں_t &dest_انتہا /*برآمد*/)
  {
    contour_نقطہ_سمتیہ_t all_points, points_with_deltas;
    غیرعلامتی مرکب_contours = 0;
    سر_maxp_info_t *سر_maxp_info_p = &plan->سر_maxp_info;
    غیرعلامتی *مرکب_contours_p = &مرکب_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    اگر (type == خالی_خانے ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      سر_maxp_info_p = خالی_اشاریہ;
      مرکب_contours_p = خالی_اشاریہ;
    }

    hb_glyf_scratch_t scratch;
    اگر (!get_points (font, glyf, all_points, scratch, &points_with_deltas, سر_maxp_info_p, مرکب_contours_p, جھوٹ, جھوٹ))
      واپس جھوٹ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    اگر (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = خالی_خانے;
      dest_آغاز = hb_بائٹیں_t ();
      dest_انتہا = hb_بائٹیں_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    اگر (!plan->pinned_at_default)
    {
      انتخاب (type)
      {
      حالت مرکب:
        اگر (!مرکب_Glyph (*سرخی, بائٹیں, extended).compile_بائٹیں_with_deltas (dest_آغاز,
										  points_with_deltas,
										  dest_انتہا))
          واپس جھوٹ;
        توڑو;
      حالت SIMPLE:
        اگر (!SimpleGlyph (*سرخی, بائٹیں).compile_بائٹیں_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_انتہا))
          واپس جھوٹ;
        توڑو;
      حالت خالی_خانے:
        dest_آغاز = hb_بائٹیں_t ();
        dest_انتہا = hb_بائٹیں_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        اگر (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            بائٹیں.لمبائی >= Glyph_سرخی::static_سائز &&
            سرخی->نمبر_کا_Contours == 0)
        {
          SimpleGlyph simple (*سرخی, بائٹیں);
          غیرعلامتی لمبائی = simple.instructions_لمبائی ();
          اگر (لمبائی)
          {
            لمبائی = simple.لمبائی (لمبائی);
            حرف *p = (حرف *) hb_malloc (لمبائی);
            اگر (unlikely (!p)) واپس جھوٹ;
            hb_memcpy (p, بائٹیں.arrayZ, لمبائی);
            dest_آغاز = hb_بائٹیں_t (p, لمبائی);
          }
        }
        توڑو;
      }
    }

    اگر (!compile_سرخی_بائٹیں (plan, all_points, dest_آغاز))
    {
      dest_انتہا.fini ();
      واپس جھوٹ;
    }
    واپس سچ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  سانچہ <قسم_نام accelerator_t>
  منطقی get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_نقطہ_سمتیہ_t &all_points /*برآمد*/,
		   hb_glyf_scratch_t &scratch,
		   contour_نقطہ_سمتیہ_t *points_with_deltas = خالی_اشاریہ, /*برآمد*/
		   سر_maxp_info_t * سر_maxp_info = خالی_اشاریہ, /*برآمد*/
		   غیرعلامتی *مرکب_contours = خالی_اشاریہ, /*برآمد*/
		   منطقی سرکاؤ_points_hori = سچ,
		   منطقی use_my_metrics = سچ,
		   منطقی phantom_only = جھوٹ,
		   hb_array_t<ثابت صحیح> coords = hb_array_t<ثابت صحیح> (),
		   hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
		   غیرعلامتی صحیح گہرائی = 0,
		   غیرعلامتی *کنارہ_گنتی = خالی_اشاریہ,
		   int64_t *budget = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (گہرائی > HB_MAX_NESTING_LEVEL)) واپس جھوٹ;
    غیرعلامتی ڈھیر_کنارہ_گنتی = 0;
    اگر (!کنارہ_گنتی) کنارہ_گنتی = &ڈھیر_کنارہ_گنتی;
    اگر (unlikely (*کنارہ_گنتی > HB_MAX_گراف_کنارہ_گنتی)) واپس جھوٹ;
    (*کنارہ_گنتی)++;

    int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
    اگر (!budget) budget = &ڈھیر_budget;
    اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) واپس جھوٹ;

    اگر (سر_maxp_info)
    {
      سر_maxp_info->max_Component_گہرائی = hb_max (سر_maxp_info->max_Component_گہرائی, گہرائی);
    }

    اگر (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->عدد_coords);

    contour_نقطہ_سمتیہ_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    غیرعلامتی old_لمبائی = points.لمبائی;

    انتخاب (type) {
    حالت SIMPLE:
      اگر (گہرائی == 0 && سر_maxp_info)
        سر_maxp_info->maxContours = hb_max (سر_maxp_info->maxContours, (غیرعلامتی) سرخی->نمبر_کا_Contours);
      اگر (گہرائی > 0 && مرکب_contours)
        *مرکب_contours += (غیرعلامتی) سرخی->نمبر_کا_Contours;
      اگر (unlikely (!SimpleGlyph (*سرخی, بائٹیں).get_contour_points (all_points, extended, phantom_only, budget)))
	واپس جھوٹ;
      توڑو;
    حالت مرکب:
    {
      برائے (خودکار &شے : get_مرکب_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !شے.get_points (points, extended)))
        {
          points.resize (old_لمبائی);
          واپس جھوٹ;
        }
      }
      توڑو;
    }
    حالت خالی_خانے:
      توڑو;
    }

    /* Init phantom points */
    اگر (unlikely (!points.resize (points.لمبائی + PHANTOM_گنتی))) واپس جھوٹ;
    hb_array_t<contour_نقطہ_t> phantoms = points.as_array ().sub_array (points.لمبائی - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحیح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحیح h_delta = (صحیح) سرخی->xMin - lsb;
      HB_غیر_مستعمل صحیح tsb = 0;
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#اگر_ختم
      صحیح v_orig  = (صحیح) سرخی->yMax + tsb;
      غیرعلامتی h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      غیرعلامتی v_adv =
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#ورنہ
                       - font->face->get_upem ()
#اگر_ختم
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحیح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحیح) v_adv;
    }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (hb_any (coords))
    {
      اگر (!glyf_accelerator.gvar->apply_deltas_مقصد_points (gid,
							  coords,
							  points.as_array ().sub_array (old_لمبائی),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        واپس جھوٹ;
    }
#اگر_ختم

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    اگر (points_with_deltas != خالی_اشاریہ && گہرائی == 0 && type == مرکب)
    {
      أكد (old_لمبائی == 0);
      *points_with_deltas = points;
    }

    اعشاری سرکاؤ = 0;
    انتخاب (type) {
    حالت SIMPLE:
      اگر (گہرائی == 0 && سر_maxp_info)
        سر_maxp_info->maxPoints = hb_max (سر_maxp_info->maxPoints, all_points.لمبائی - old_لمبائی - 4);
      سرکاؤ = phantoms[PHANTOM_بایاں].x;
      توڑو;
    حالت مرکب:
    {
      hb_decycler_گرہ_t decycler_گرہ (scratch.decycler);

      غیرعلامتی صحیح comp_اشاریہ = 0;
      برائے (خودکار &شے : get_مرکب_iterator ())
      {
	hb_codepoint_t شے_gid = شے.get_gid (extended);

        اگر (unlikely (!decycler_گرہ.دورہ (شے_gid)))
	{
	  comp_اشاریہ++;
	  جاری;
	}

	غیرعلامتی old_گنتی = all_points.لمبائی;

	اگر (unlikely ((!phantom_only || (use_my_metrics && شے.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (شے_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    سر_maxp_info,
						    مرکب_contours,
						    سرکاؤ_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    گہرائی + 1,
						    کنارہ_گنتی,
						    budget)))
	{
	  points.resize (old_لمبائی);
	  واپس جھوٹ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.لمبائی - PHANTOM_گنتی, PHANTOM_گنتی);

	خودکار comp_points = all_points.as_array ().sub_array (old_گنتی);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	اگر (use_my_metrics && شے.is_use_my_metrics ())
	  برائے (غیرعلامتی صحیح i = 0; i < PHANTOM_گنتی; i++)
	    phantoms[i] = comp_points[comp_points.لمبائی - PHANTOM_گنتی + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.لمبائی)))
	{
	  points.resize (old_لمبائی);
	  واپس جھوٹ;
	}

	اگر (comp_points) // Empty in case of phantom_only
	{
	  اعشاری میٹرکس[4];
	  contour_نقطہ_t default_trans;
	  شے.get_transformation (میٹرکس, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  شے.transform_points (comp_points, میٹرکس, points[old_لمبائی + comp_اشاریہ]);
	}

	اگر (شے.is_anchored () && !phantom_only)
	{
	  غیرعلامتی صحیح p1, p2;
	  شے.get_anchor_points (p1, p2, extended);
	  اگر (likely (p1 < all_points.لمبائی && p2 < comp_points.لمبائی))
	  {
	    contour_نقطہ_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    شے.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.لمبائی - PHANTOM_گنتی);

	اگر (all_points.لمبائی > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_لمبائی);
	  واپس جھوٹ;
	}

	comp_اشاریہ++;
      }

      اگر (سر_maxp_info && گہرائی == 0)
      {
        اگر (مرکب_contours)
          سر_maxp_info->max_مرکب_Contours = hb_max (سر_maxp_info->max_مرکب_Contours, *مرکب_contours);
        سر_maxp_info->max_مرکب_Points = hb_max (سر_maxp_info->max_مرکب_Points, all_points.لمبائی);
        سر_maxp_info->maxComponentElements = hb_max (سر_maxp_info->maxComponentElements, comp_اشاریہ);
      }
      all_points.extend (phantoms);
      سرکاؤ = phantoms[PHANTOM_بایاں].x;
      points.resize (old_لمبائی);
    } توڑو;
    حالت خالی_خانے:
      all_points.extend (phantoms);
      سرکاؤ = phantoms[PHANTOM_بایاں].x;
      points.resize (old_لمبائی);
      توڑو;
    }

    اگر (گہرائی == 0 && سرکاؤ_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      اگر (سرکاؤ)
        برائے (خودکار &نقطہ : all_points)
	  نقطہ.x -= سرکاؤ;
    }

    واپس !all_points.اندر_error ();
  }

  منطقی get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    اگر (type == خالی_خانے)
    {
      *extents = {0, 0, 0, 0};
      واپس سچ; /* Empty glyph; zero extents. */
    }
    واپس سرخی->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_بائٹیں_t get_بائٹیں () ثابت { واپس بائٹیں; }
  glyph_type_t get_type () ثابت { واپس type; }
  ثابت Glyph_سرخی *get_سرخی () ثابت { واپس سرخی; }
  منطقی is_extended () ثابت { واپس extended; }

  Glyph () : بائٹیں (),
             سرخی (بائٹیں.as<Glyph_سرخی> ()),
             gid (-1),
             type(خالی_خانے),
	     extended (جھوٹ)
  {}

  Glyph (hb_بائٹیں_t بائٹیں_2,
	 hb_codepoint_t gid_,
	 منطقی extended_) : بائٹیں (بائٹیں_2),
			   سرخی (بائٹیں.as<Glyph_سرخی> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحیح عدد_contours = سرخی->نمبر_کا_Contours;
    اگر (unlikely (عدد_contours == 0)) type = خالی_خانے;
    ورنہ اگر (عدد_contours > 0) type = SIMPLE;
    ورنہ اگر (عدد_contours <= -1) type = مرکب;
    ورنہ type = خالی_خانے; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محفوظ:
  hb_بائٹیں_t بائٹیں;
  ثابت Glyph_سرخی *سرخی;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقی extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_GLYPH_HH */
