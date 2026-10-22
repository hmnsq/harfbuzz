#నిర్వచితం_కాకపోతే OT_GLYF_GLYPH_HH
#నిర్వచించు OT_GLYF_GLYPH_HH


#అన్తర్భావ "../../hb-open-type.hh"

#అన్తర్భావ "GlyphHeader.hh"
#అన్తర్భావ "SimpleGlyph.hh"
#అన్తర్భావ "CompositeGlyph.hh"


నామాకాశ OT {

నిర్మాణం glyf_accelerator_t;

నామాకాశ glyf_impl {


గణన phantom_బిందువు_సూచీ_t
{
  PHANTOM_ఎడమ   = 0,
  PHANTOM_కుడి  = 1,
  PHANTOM_శిఖరం    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_లెక్క  = 4
};

నిర్మాణం Glyph
{
  గణన glyph_type_t {
    ఖాళీలు,
    SIMPLE,
    సంయుక్త,
  };

  బహిరంగ:
  సంయుక్త_iter_t get_సంయుక్త_iterator () స్థిరరాశి
  {
    ఒకవేళ (type != సంయుక్త) తిరిగి సంయుక్త_iter_t ();
    తిరిగి సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).iter ();
  }

  స్థిరరాశి hb_బైట్లు_t trim_padding () స్థిరరాశి
  {
    ఎంపిక (type) {
    స్థితి సంయుక్త: తిరిగి సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).trim_padding ();
    స్థితి SIMPLE:    తిరిగి SimpleGlyph (*శీర్షిక, బైట్లు).trim_padding ();
    స్థితి ఖాళీలు:     తిరిగి బైట్లు;
    అప్రమేయ:        తిరిగి బైట్లు;
    }
  }

  శూన్య drop_hints ()
  {
    ఎంపిక (type) {
    స్థితి సంయుక్త: సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).drop_hints (); తిరిగి;
    స్థితి SIMPLE:    SimpleGlyph (*శీర్షిక, బైట్లు).drop_hints (); తిరిగి;
    స్థితి ఖాళీలు:
      ఒకవేళ (బైట్లు.నిడివి >= Glyph_శీర్షిక::static_కొలత)
        SimpleGlyph (*శీర్షిక, బైట్లు).drop_hints ();
      తిరిగి;
    }
  }

  శూన్య సమితి_overlaps_flag ()
  {
    ఎంపిక (type) {
    స్థితి సంయుక్త: సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).సమితి_overlaps_flag (); తిరిగి;
    స్థితి SIMPLE:    SimpleGlyph (*శీర్షిక, బైట్లు).సమితి_overlaps_flag (); తిరిగి;
    స్థితి ఖాళీలు:     తిరిగి;
    }
  }

  శూన్య drop_hints_బైట్లు (hb_బైట్లు_t &dest_ప్రారంభం, hb_బైట్లు_t &dest_చివర) స్థిరరాశి
  {
    ఎంపిక (type) {
    స్థితి సంయుక్త: సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).drop_hints_బైట్లు (dest_ప్రారంభం); తిరిగి;
    స్థితి SIMPLE:    SimpleGlyph (*శీర్షిక, బైట్లు).drop_hints_బైట్లు (dest_ప్రారంభం, dest_చివర); తిరిగి;
    స్థితి ఖాళీలు:
      dest_ప్రారంభం = బైట్లు;
      dest_చివర = hb_బైట్లు_t ();
      ఒకవేళ (బైట్లు.నిడివి >= Glyph_శీర్షిక::static_కొలత &&
          SimpleGlyph (*శీర్షిక, బైట్లు).has_instructions_నిడివి ())
        SimpleGlyph (*శీర్షిక, బైట్లు).drop_hints_బైట్లు (dest_ప్రారంభం, dest_చివర);
      తిరిగి;
    }
  }

  తార్కిక is_సంయుక్త () స్థిరరాశి
  { తిరిగి type == సంయుక్త; }

  తార్కిక get_all_points_without_var (స్థిరరాశి hb_face_t *face,
                                   contour_బిందువు_సదిశ_t &points /*బహిర్గతం*/) స్థిరరాశి
  {
    ఎంపిక (type) {
    స్థితి SIMPLE:
      ఒకవేళ (unlikely (!SimpleGlyph (*శీర్షిక, బైట్లు).get_contour_points (points, extended)))
        తిరిగి అబద్ధం;
      ఆపు;
    స్థితి సంయుక్త:
    {
      కొరకు (స్వయంచాలక &అంశం : get_సంయుక్త_iterator ())
        ఒకవేళ (unlikely (!అంశం.get_points (points, extended))) తిరిగి అబద్ధం;
      ఆపు;
    }
    స్థితి ఖాళీలు:
      ఆపు;
    }

    /* Init phantom points */
    ఒకవేళ (unlikely (!points.resize (points.నిడివి + PHANTOM_లెక్క))) తిరిగి అబద్ధం;
    hb_array_t<contour_బిందువు_t> phantoms = points.as_array ().sub_array (points.నిడివి - PHANTOM_లెక్క, PHANTOM_లెక్క);
    {
      // Duplicated code.
      పూర్ణాంకం lsb = 0;
      face->పట్టిక.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      పూర్ణాంకం h_delta = (పూర్ణాంకం) శీర్షిక->xMin - lsb;
      HB_వాడని పూర్ణాంకం tsb = 0;
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
      face->పట్టిక.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#అయితే_ముగింపు
      పూర్ణాంకం v_orig  = (పూర్ణాంకం) శీర్షిక->yMax + tsb;
      గుర్తులేని h_adv = face->పట్టిక.hmtx->get_advance_without_var_unscaled (gid);
      గుర్తులేని v_adv =
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
                       face->పట్టిక.vmtx->get_advance_without_var_unscaled (gid)
#లేకపోతే
                       - face->get_upem ()
#అయితే_ముగింపు
                       ;
      phantoms[PHANTOM_ఎడమ].x = h_delta;
      phantoms[PHANTOM_కుడి].x = (పూర్ణాంకం) h_adv + h_delta;
      phantoms[PHANTOM_శిఖరం].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (పూర్ణాంకం) v_adv;
    }
    తిరిగి నిజం;
  }

  శూన్య update_mtx (స్థిరరాశి hb_subset_plan_t *plan,
                   పూర్ణాంకం xMin, పూర్ణాంకం xMax,
                   పూర్ణాంకం yMin, పూర్ణాంకం yMax,
                   స్థిరరాశి contour_బిందువు_సదిశ_t &all_points) స్థిరరాశి
  {
    hb_codepoint_t new_gid = 0;
    ఒకవేళ (!plan->new_gid_for_old_gid (gid, &new_gid))
      తిరిగి;

    ఒకవేళ (type != ఖాళీలు)
    {
      plan->bounds_వెడల్పు_vec[new_gid] = xMax - xMin;
      plan->bounds_ఎత్తు_vec[new_gid] = yMax - yMin;
    }

    గుర్తులేని len = all_points.నిడివి;
    దశాంశం ఎడమ_Side_X = all_points[len - 4].x;
    దశాంశం కుడి_Side_X = all_points[len - 3].x;
    దశాంశం శిఖరం_Side_Y = all_points[len - 2].y;
    దశాంశం bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    గుర్తుగల hori_aw = roundf (కుడి_Side_X - ఎడమ_Side_X);
    ఒకవేళ (hori_aw < 0) hori_aw = 0;
    పూర్ణాంకం lsb = roundf (xMin - ఎడమ_Side_X);
    plan->hmtx_map.సమితి_with_hash (new_gid, hash, hb_pair ((గుర్తులేని) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    ఒకవేళ (type != ఖాళీలు && lsb != xMin)
      plan->తల_maxp_info.allXMinIsLsb = అబద్ధం;

    గుర్తుగల vert_aw = roundf (శిఖరం_Side_Y - bottomSideY);
    ఒకవేళ (vert_aw < 0) vert_aw = 0;
    పూర్ణాంకం tsb = roundf (శిఖరం_Side_Y - yMax);
    plan->vmtx_map.సమితి_with_hash (new_gid, hash, hb_pair ((గుర్తులేని) vert_aw, tsb));
  }

  తార్కిక compile_శీర్షిక_బైట్లు (స్థిరరాశి hb_subset_plan_t *plan,
                             స్థిరరాశి contour_బిందువు_సదిశ_t &all_points,
                             hb_బైట్లు_t &dest_బైట్లు /*బహిర్గతం*/) స్థిరరాశి
  {
    Glyph_శీర్షిక *glyph_శీర్షిక = శూన్యసూచిక;
    ఒకవేళ (!plan->pinned_at_default && type != ఖాళీలు && all_points.నిడివి >= 4)
    {
      glyph_శీర్షిక = (Glyph_శీర్షిక *) hb_calloc (1, Glyph_శీర్షిక::static_కొలత);
      ఒకవేళ (unlikely (!glyph_శీర్షిక)) తిరిగి అబద్ధం;
    }

    దశాంశం xMin = 0, xMax = 0;
    దశాంశం yMin = 0, yMax = 0;
    ఒకవేళ (all_points.నిడివి > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      గుర్తులేని లెక్క = all_points.నిడివి - 4;
      కొరకు (గుర్తులేని i = 1; i < లెక్క; i++)
      {
	దశాంశం x = all_points[i].x;
	దశాంశం y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    పూర్ణాంకం rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    పూర్ణాంకం rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    పూర్ణాంకం rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    పూర్ణాంకం rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    ఒకవేళ (type != ఖాళీలు)
    {
      plan->తల_maxp_info.xMin = hb_min (plan->తల_maxp_info.xMin, rounded_xMin);
      plan->తల_maxp_info.yMin = hb_min (plan->తల_maxp_info.yMin, rounded_yMin);
      plan->తల_maxp_info.xMax = hb_max (plan->తల_maxp_info.xMax, rounded_xMax);
      plan->తల_maxp_info.yMax = hb_max (plan->తల_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    ఒకవేళ (!glyph_శీర్షిక)
      తిరిగి నిజం;

    glyph_శీర్షిక->సంఖ్య_యొక్క_Contours = శీర్షిక->సంఖ్య_యొక్క_Contours;

    glyph_శీర్షిక->xMin = rounded_xMin;
    glyph_శీర్షిక->yMin = rounded_yMin;
    glyph_శీర్షిక->xMax = rounded_xMax;
    glyph_శీర్షిక->yMax = rounded_yMax;

    dest_బైట్లు = hb_బైట్లు_t ((స్థిరరాశి అక్షరం *)glyph_శీర్షిక, Glyph_శీర్షిక::static_కొలత);
    తిరిగి నిజం;
  }

  తార్కిక compile_బైట్లు_with_deltas (స్థిరరాశి hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  స్థిరరాశి glyf_accelerator_t &glyf,
                                  hb_బైట్లు_t &dest_ప్రారంభం,  /* IN/OUT */
                                  hb_బైట్లు_t &dest_చివర /*బహిర్గతం*/)
  {
    contour_బిందువు_సదిశ_t all_points, points_with_deltas;
    గుర్తులేని సంయుక్త_contours = 0;
    తల_maxp_info_t *తల_maxp_info_p = &plan->తల_maxp_info;
    గుర్తులేని *సంయుక్త_contours_p = &సంయుక్త_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    ఒకవేళ (type == ఖాళీలు ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      తల_maxp_info_p = శూన్యసూచిక;
      సంయుక్త_contours_p = శూన్యసూచిక;
    }

    hb_glyf_scratch_t scratch;
    ఒకవేళ (!get_points (font, glyf, all_points, scratch, &points_with_deltas, తల_maxp_info_p, సంయుక్త_contours_p, అబద్ధం, అబద్ధం))
      తిరిగి అబద్ధం;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    ఒకవేళ (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = ఖాళీలు;
      dest_ప్రారంభం = hb_బైట్లు_t ();
      dest_చివర = hb_బైట్లు_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    ఒకవేళ (!plan->pinned_at_default)
    {
      ఎంపిక (type)
      {
      స్థితి సంయుక్త:
        ఒకవేళ (!సంయుక్త_Glyph (*శీర్షిక, బైట్లు, extended).compile_బైట్లు_with_deltas (dest_ప్రారంభం,
										  points_with_deltas,
										  dest_చివర))
          తిరిగి అబద్ధం;
        ఆపు;
      స్థితి SIMPLE:
        ఒకవేళ (!SimpleGlyph (*శీర్షిక, బైట్లు).compile_బైట్లు_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_చివర))
          తిరిగి అబద్ధం;
        ఆపు;
      స్థితి ఖాళీలు:
        dest_ప్రారంభం = hb_బైట్లు_t ();
        dest_చివర = hb_బైట్లు_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        ఒకవేళ (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            బైట్లు.నిడివి >= Glyph_శీర్షిక::static_కొలత &&
            శీర్షిక->సంఖ్య_యొక్క_Contours == 0)
        {
          SimpleGlyph simple (*శీర్షిక, బైట్లు);
          గుర్తులేని నిడివి = simple.instructions_నిడివి ();
          ఒకవేళ (నిడివి)
          {
            నిడివి = simple.నిడివి (నిడివి);
            అక్షరం *p = (అక్షరం *) hb_malloc (నిడివి);
            ఒకవేళ (unlikely (!p)) తిరిగి అబద్ధం;
            hb_memcpy (p, బైట్లు.arrayZ, నిడివి);
            dest_ప్రారంభం = hb_బైట్లు_t (p, నిడివి);
          }
        }
        ఆపు;
      }
    }

    ఒకవేళ (!compile_శీర్షిక_బైట్లు (plan, all_points, dest_ప్రారంభం))
    {
      dest_చివర.fini ();
      తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  మూస <రకం_పేరు accelerator_t>
  తార్కిక get_points (hb_font_t *font, స్థిరరాశి accelerator_t &glyf_accelerator,
		   contour_బిందువు_సదిశ_t &all_points /*బహిర్గతం*/,
		   hb_glyf_scratch_t &scratch,
		   contour_బిందువు_సదిశ_t *points_with_deltas = శూన్యసూచిక, /*బహిర్గతం*/
		   తల_maxp_info_t * తల_maxp_info = శూన్యసూచిక, /*బహిర్గతం*/
		   గుర్తులేని *సంయుక్త_contours = శూన్యసూచిక, /*బహిర్గతం*/
		   తార్కిక జరుపు_points_hori = నిజం,
		   తార్కిక use_my_metrics = నిజం,
		   తార్కిక phantom_only = అబద్ధం,
		   hb_array_t<స్థిరరాశి పూర్ణాంకం> coords = hb_array_t<స్థిరరాశి పూర్ణాంకం> (),
		   hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
		   గుర్తులేని పూర్ణాంకం లోతు = 0,
		   గుర్తులేని *అంచు_లెక్క = శూన్యసూచిక,
		   int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (లోతు > HB_MAX_NESTING_LEVEL)) తిరిగి అబద్ధం;
    గుర్తులేని దొంతర_అంచు_లెక్క = 0;
    ఒకవేళ (!అంచు_లెక్క) అంచు_లెక్క = &దొంతర_అంచు_లెక్క;
    ఒకవేళ (unlikely (*అంచు_లెక్క > HB_MAX_లేఖాచిత్రం_అంచు_లెక్క)) తిరిగి అబద్ధం;
    (*అంచు_లెక్క)++;

    int64_t దొంతర_budget = HB_BUDGET_GLYPH;
    ఒకవేళ (!budget) budget = &దొంతర_budget;
    ఒకవేళ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) తిరిగి అబద్ధం;

    ఒకవేళ (తల_maxp_info)
    {
      తల_maxp_info->max_Component_లోతు = hb_max (తల_maxp_info->max_Component_లోతు, లోతు);
    }

    ఒకవేళ (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->అంకె_coords);

    contour_బిందువు_సదిశ_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    గుర్తులేని old_నిడివి = points.నిడివి;

    ఎంపిక (type) {
    స్థితి SIMPLE:
      ఒకవేళ (లోతు == 0 && తల_maxp_info)
        తల_maxp_info->maxContours = hb_max (తల_maxp_info->maxContours, (గుర్తులేని) శీర్షిక->సంఖ్య_యొక్క_Contours);
      ఒకవేళ (లోతు > 0 && సంయుక్త_contours)
        *సంయుక్త_contours += (గుర్తులేని) శీర్షిక->సంఖ్య_యొక్క_Contours;
      ఒకవేళ (unlikely (!SimpleGlyph (*శీర్షిక, బైట్లు).get_contour_points (all_points, extended, phantom_only, budget)))
	తిరిగి అబద్ధం;
      ఆపు;
    స్థితి సంయుక్త:
    {
      కొరకు (స్వయంచాలక &అంశం : get_సంయుక్త_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        ఒకవేళ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !అంశం.get_points (points, extended)))
        {
          points.resize (old_నిడివి);
          తిరిగి అబద్ధం;
        }
      }
      ఆపు;
    }
    స్థితి ఖాళీలు:
      ఆపు;
    }

    /* Init phantom points */
    ఒకవేళ (unlikely (!points.resize (points.నిడివి + PHANTOM_లెక్క))) తిరిగి అబద్ధం;
    hb_array_t<contour_బిందువు_t> phantoms = points.as_array ().sub_array (points.నిడివి - PHANTOM_లెక్క, PHANTOM_లెక్క);
    {
      // Duplicated code.
      పూర్ణాంకం lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      పూర్ణాంకం h_delta = (పూర్ణాంకం) శీర్షిక->xMin - lsb;
      HB_వాడని పూర్ణాంకం tsb = 0;
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#అయితే_ముగింపు
      పూర్ణాంకం v_orig  = (పూర్ణాంకం) శీర్షిక->yMax + tsb;
      గుర్తులేని h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      గుర్తులేని v_adv =
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#లేకపోతే
                       - font->face->get_upem ()
#అయితే_ముగింపు
                       ;
      phantoms[PHANTOM_ఎడమ].x = h_delta;
      phantoms[PHANTOM_కుడి].x = (పూర్ణాంకం) h_adv + h_delta;
      phantoms[PHANTOM_శిఖరం].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (పూర్ణాంకం) v_adv;
    }

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (hb_any (coords))
    {
      ఒకవేళ (!glyf_accelerator.gvar->apply_deltas_గమ్యం_points (gid,
							  coords,
							  points.as_array ().sub_array (old_నిడివి),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        తిరిగి అబద్ధం;
    }
#అయితే_ముగింపు

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    ఒకవేళ (points_with_deltas != శూన్యసూచిక && లోతు == 0 && type == సంయుక్త)
    {
      ప్రతిజ్ఞా (old_నిడివి == 0);
      *points_with_deltas = points;
    }

    దశాంశం జరుపు = 0;
    ఎంపిక (type) {
    స్థితి SIMPLE:
      ఒకవేళ (లోతు == 0 && తల_maxp_info)
        తల_maxp_info->maxPoints = hb_max (తల_maxp_info->maxPoints, all_points.నిడివి - old_నిడివి - 4);
      జరుపు = phantoms[PHANTOM_ఎడమ].x;
      ఆపు;
    స్థితి సంయుక్త:
    {
      hb_decycler_కణుపు_t decycler_కణుపు (scratch.decycler);

      గుర్తులేని పూర్ణాంకం comp_సూచీ = 0;
      కొరకు (స్వయంచాలక &అంశం : get_సంయుక్త_iterator ())
      {
	hb_codepoint_t అంశం_gid = అంశం.get_gid (extended);

        ఒకవేళ (unlikely (!decycler_కణుపు.సందర్శించు (అంశం_gid)))
	{
	  comp_సూచీ++;
	  కొనసాగు;
	}

	గుర్తులేని old_లెక్క = all_points.నిడివి;

	ఒకవేళ (unlikely ((!phantom_only || (use_my_metrics && అంశం.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (అంశం_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    తల_maxp_info,
						    సంయుక్త_contours,
						    జరుపు_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    లోతు + 1,
						    అంచు_లెక్క,
						    budget)))
	{
	  points.resize (old_నిడివి);
	  తిరిగి అబద్ధం;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.నిడివి - PHANTOM_లెక్క, PHANTOM_లెక్క);

	స్వయంచాలక comp_points = all_points.as_array ().sub_array (old_లెక్క);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	ఒకవేళ (use_my_metrics && అంశం.is_use_my_metrics ())
	  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < PHANTOM_లెక్క; i++)
	    phantoms[i] = comp_points[comp_points.నిడివి - PHANTOM_లెక్క + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	ఒకవేళ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.నిడివి)))
	{
	  points.resize (old_నిడివి);
	  తిరిగి అబద్ధం;
	}

	ఒకవేళ (comp_points) // Empty in case of phantom_only
	{
	  దశాంశం మాతృక[4];
	  contour_బిందువు_t default_trans;
	  అంశం.get_transformation (మాతృక, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  అంశం.transform_points (comp_points, మాతృక, points[old_నిడివి + comp_సూచీ]);
	}

	ఒకవేళ (అంశం.is_anchored () && !phantom_only)
	{
	  గుర్తులేని పూర్ణాంకం p1, p2;
	  అంశం.get_anchor_points (p1, p2, extended);
	  ఒకవేళ (likely (p1 < all_points.నిడివి && p2 < comp_points.నిడివి))
	  {
	    contour_బిందువు_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    అంశం.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.నిడివి - PHANTOM_లెక్క);

	ఒకవేళ (all_points.నిడివి > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_నిడివి);
	  తిరిగి అబద్ధం;
	}

	comp_సూచీ++;
      }

      ఒకవేళ (తల_maxp_info && లోతు == 0)
      {
        ఒకవేళ (సంయుక్త_contours)
          తల_maxp_info->max_సంయుక్త_Contours = hb_max (తల_maxp_info->max_సంయుక్త_Contours, *సంయుక్త_contours);
        తల_maxp_info->max_సంయుక్త_Points = hb_max (తల_maxp_info->max_సంయుక్త_Points, all_points.నిడివి);
        తల_maxp_info->maxComponentElements = hb_max (తల_maxp_info->maxComponentElements, comp_సూచీ);
      }
      all_points.extend (phantoms);
      జరుపు = phantoms[PHANTOM_ఎడమ].x;
      points.resize (old_నిడివి);
    } ఆపు;
    స్థితి ఖాళీలు:
      all_points.extend (phantoms);
      జరుపు = phantoms[PHANTOM_ఎడమ].x;
      points.resize (old_నిడివి);
      ఆపు;
    }

    ఒకవేళ (లోతు == 0 && జరుపు_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      ఒకవేళ (జరుపు)
        కొరకు (స్వయంచాలక &బిందువు : all_points)
	  బిందువు.x -= జరుపు;
    }

    తిరిగి !all_points.లోపల_error ();
  }

  తార్కిక get_extents_without_var_scaled (hb_font_t *font, స్థిరరాశి glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) స్థిరరాశి
  {
    ఒకవేళ (type == ఖాళీలు)
    {
      *extents = {0, 0, 0, 0};
      తిరిగి నిజం; /* Empty glyph; zero extents. */
    }
    తిరిగి శీర్షిక->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_బైట్లు_t get_బైట్లు () స్థిరరాశి { తిరిగి బైట్లు; }
  glyph_type_t get_type () స్థిరరాశి { తిరిగి type; }
  స్థిరరాశి Glyph_శీర్షిక *get_శీర్షిక () స్థిరరాశి { తిరిగి శీర్షిక; }
  తార్కిక is_extended () స్థిరరాశి { తిరిగి extended; }

  Glyph () : బైట్లు (),
             శీర్షిక (బైట్లు.as<Glyph_శీర్షిక> ()),
             gid (-1),
             type(ఖాళీలు),
	     extended (అబద్ధం)
  {}

  Glyph (hb_బైట్లు_t బైట్లు_2,
	 hb_codepoint_t gid_,
	 తార్కిక extended_) : బైట్లు (బైట్లు_2),
			   శీర్షిక (బైట్లు.as<Glyph_శీర్షిక> ()),
			   gid (gid_),
			   extended (extended_)
  {
    పూర్ణాంకం అంకె_contours = శీర్షిక->సంఖ్య_యొక్క_Contours;
    ఒకవేళ (unlikely (అంకె_contours == 0)) type = ఖాళీలు;
    లేకపోతే ఒకవేళ (అంకె_contours > 0) type = SIMPLE;
    లేకపోతే ఒకవేళ (అంకె_contours <= -1) type = సంయుక్త;
    లేకపోతే type = ఖాళీలు; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  రక్షిత:
  hb_బైట్లు_t బైట్లు;
  స్థిరరాశి Glyph_శీర్షిక *శీర్షిక;
  hb_codepoint_t gid;
  glyph_type_t type;
  తార్కిక extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_GLYPH_HH */
