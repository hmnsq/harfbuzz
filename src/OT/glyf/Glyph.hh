#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"

#অন্তর্ভাব "GlyphHeader.hh"
#অন্তর্ভাব "SimpleGlyph.hh"
#অন্তর্ভাব "CompositeGlyph.hh"


নামাকাশ OT {

কাঠামো glyf_accelerator_t;

নামাকাশ glyf_impl {


গণনা phantom_point_সূচক_t
{
  PHANTOM_বাম   = 0,
  PHANTOM_ডান  = 1,
  PHANTOM_শীর্ষ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_গুনতি  = 4
};

কাঠামো Glyph
{
  গণনা glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  সর্বজনীন:
  composite_iter_t get_composite_iterator () ধ্রুবক
  {
    যদি (type != COMPOSITE) ফেরত composite_iter_t ();
    ফেরত CompositeGlyph (*header, bytes, extended).iter ();
  }

  ধ্রুবক hb_bytes_t trim_padding () ধ্রুবক
  {
    নির্বাচন (type) {
    ক্ষেত্র COMPOSITE: ফেরত CompositeGlyph (*header, bytes, extended).trim_padding ();
    ক্ষেত্র SIMPLE:    ফেরত SimpleGlyph (*header, bytes).trim_padding ();
    ক্ষেত্র EMPTY:     ফেরত bytes;
    পূর্বনির্ধারিত:        ফেরত bytes;
    }
  }

  শূন্য drop_hints ()
  {
    নির্বাচন (type) {
    ক্ষেত্র COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); ফেরত;
    ক্ষেত্র SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); ফেরত;
    ক্ষেত্র EMPTY:
      যদি (bytes.দৈর্ঘ্য >= GlyphHeader::static_আকার)
        SimpleGlyph (*header, bytes).drop_hints ();
      ফেরত;
    }
  }

  শূন্য set_overlaps_flag ()
  {
    নির্বাচন (type) {
    ক্ষেত্র COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); ফেরত;
    ক্ষেত্র SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); ফেরত;
    ক্ষেত্র EMPTY:     ফেরত;
    }
  }

  শূন্য drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ধ্রুবক
  {
    নির্বাচন (type) {
    ক্ষেত্র COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); ফেরত;
    ক্ষেত্র SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); ফেরত;
    ক্ষেত্র EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      যদি (bytes.দৈর্ঘ্য >= GlyphHeader::static_আকার &&
          SimpleGlyph (*header, bytes).has_instructions_দৈর্ঘ্য ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      ফেরত;
    }
  }

  যৌক্তিক is_composite () ধ্রুবক
  { ফেরত type == COMPOSITE; }

  যৌক্তিক get_all_points_without_var (ধ্রুবক hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ধ্রুবক
  {
    নির্বাচন (type) {
    ক্ষেত্র SIMPLE:
      যদি (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        ফেরত মিথ্যা;
      বিরতি;
    ক্ষেত্র COMPOSITE:
    {
      জন্য (auto &item : get_composite_iterator ())
        যদি (unlikely (!item.get_points (points, extended))) ফেরত মিথ্যা;
      বিরতি;
    }
    ক্ষেত্র EMPTY:
      বিরতি;
    }

    /* Init phantom points */
    যদি (unlikely (!points.resize (points.দৈর্ঘ্য + PHANTOM_গুনতি))) ফেরত মিথ্যা;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.দৈর্ঘ্য - PHANTOM_গুনতি, PHANTOM_গুনতি);
    {
      // Duplicated code.
      পূর্ণসংখ্যা lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      পূর্ণসংখ্যা h_delta = (পূর্ণসংখ্যা) header->xMin - lsb;
      HB_UNUSED পূর্ণসংখ্যা tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      পূর্ণসংখ্যা v_orig  = (পূর্ণসংখ্যা) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#অন্যথা
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_বাম].x = h_delta;
      phantoms[PHANTOM_ডান].x = (পূর্ণসংখ্যা) h_adv + h_delta;
      phantoms[PHANTOM_শীর্ষ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (পূর্ণসংখ্যা) v_adv;
    }
    ফেরত সত্য;
  }

  শূন্য update_mtx (ধ্রুবক hb_subset_plan_t *plan,
                   পূর্ণসংখ্যা xMin, পূর্ণসংখ্যা xMax,
                   পূর্ণসংখ্যা yMin, পূর্ণসংখ্যা yMax,
                   ধ্রুবক contour_point_vector_t &all_points) ধ্রুবক
  {
    hb_codepoint_t new_gid = 0;
    যদি (!plan->new_gid_for_old_gid (gid, &new_gid))
      ফেরত;

    যদি (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.দৈর্ঘ্য;
    দশমিক বাম_Side_X = all_points[len - 4].x;
    দশমিক ডান_Side_X = all_points[len - 3].x;
    দশমিক শীর্ষ_Side_Y = all_points[len - 2].y;
    দশমিক bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (ডান_Side_X - বাম_Side_X);
    যদি (hori_aw < 0) hori_aw = 0;
    পূর্ণসংখ্যা lsb = roundf (xMin - বাম_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    যদি (type != EMPTY && lsb != xMin)
      plan->শীর্ষ_maxp_info.allXMinIsLsb = মিথ্যা;

    signed vert_aw = roundf (শীর্ষ_Side_Y - bottomSideY);
    যদি (vert_aw < 0) vert_aw = 0;
    পূর্ণসংখ্যা tsb = roundf (শীর্ষ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  যৌক্তিক compile_header_bytes (ধ্রুবক hb_subset_plan_t *plan,
                             ধ্রুবক contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ধ্রুবক
  {
    GlyphHeader *glyph_header = nullptr;
    যদি (!plan->pinned_at_default && type != EMPTY && all_points.দৈর্ঘ্য >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_আকার);
      যদি (unlikely (!glyph_header)) ফেরত মিথ্যা;
    }

    দশমিক xMin = 0, xMax = 0;
    দশমিক yMin = 0, yMax = 0;
    যদি (all_points.দৈর্ঘ্য > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned গুনতি = all_points.দৈর্ঘ্য - 4;
      জন্য (unsigned i = 1; i < গুনতি; i++)
      {
	দশমিক x = all_points[i].x;
	দশমিক y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    পূর্ণসংখ্যা rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    পূর্ণসংখ্যা rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    পূর্ণসংখ্যা rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    পূর্ণসংখ্যা rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    যদি (type != EMPTY)
    {
      plan->শীর্ষ_maxp_info.xMin = hb_min (plan->শীর্ষ_maxp_info.xMin, rounded_xMin);
      plan->শীর্ষ_maxp_info.yMin = hb_min (plan->শীর্ষ_maxp_info.yMin, rounded_yMin);
      plan->শীর্ষ_maxp_info.xMax = hb_max (plan->শীর্ষ_maxp_info.xMax, rounded_xMax);
      plan->শীর্ষ_maxp_info.yMax = hb_max (plan->শীর্ষ_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    যদি (!glyph_header)
      ফেরত সত্য;

    glyph_header->সংখ্যা_Of_Contours = header->সংখ্যা_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ধ্রুবক অক্ষর *)glyph_header, GlyphHeader::static_আকার);
    ফেরত সত্য;
  }

  যৌক্তিক compile_bytes_with_deltas (ধ্রুবক hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ধ্রুবক glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    শীর্ষ_maxp_info_t *শীর্ষ_maxp_info_p = &plan->শীর্ষ_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    যদি (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      শীর্ষ_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    যদি (!get_points (font, glyf, all_points, scratch, &points_with_deltas, শীর্ষ_maxp_info_p, composite_contours_p, মিথ্যা, মিথ্যা))
      ফেরত মিথ্যা;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    যদি (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    যদি (!plan->pinned_at_default)
    {
      নির্বাচন (type)
      {
      ক্ষেত্র COMPOSITE:
        যদি (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          ফেরত মিথ্যা;
        বিরতি;
      ক্ষেত্র SIMPLE:
        যদি (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          ফেরত মিথ্যা;
        বিরতি;
      ক্ষেত্র EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        যদি (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.দৈর্ঘ্য >= GlyphHeader::static_আকার &&
            header->সংখ্যা_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned দৈর্ঘ্য = simple.instructions_দৈর্ঘ্য ();
          যদি (দৈর্ঘ্য)
          {
            দৈর্ঘ্য = simple.দৈর্ঘ্য (দৈর্ঘ্য);
            অক্ষর *p = (অক্ষর *) hb_malloc (দৈর্ঘ্য);
            যদি (unlikely (!p)) ফেরত মিথ্যা;
            hb_memcpy (p, bytes.arrayZ, দৈর্ঘ্য);
            dest_start = hb_bytes_t (p, দৈর্ঘ্য);
          }
        }
        বিরতি;
      }
    }

    যদি (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  যৌক্তিক get_points (hb_font_t *font, ধ্রুবক accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   শীর্ষ_maxp_info_t * শীর্ষ_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   যৌক্তিক shift_points_hori = সত্য,
		   যৌক্তিক use_my_metrics = সত্য,
		   যৌক্তিক phantom_only = মিথ্যা,
		   hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords = hb_array_t<ধ্রুবক পূর্ণসংখ্যা> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned পূর্ণসংখ্যা depth = 0,
		   unsigned *edge_গুনতি = nullptr,
		   int64_t *budget = nullptr) ধ্রুবক
  {
    যদি (unlikely (depth > HB_MAX_NESTING_LEVEL)) ফেরত মিথ্যা;
    unsigned স্তূপ_edge_গুনতি = 0;
    যদি (!edge_গুনতি) edge_গুনতি = &স্তূপ_edge_গুনতি;
    যদি (unlikely (*edge_গুনতি > HB_MAX_GRAPH_EDGE_গুনতি)) ফেরত মিথ্যা;
    (*edge_গুনতি)++;

    int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
    যদি (!budget) budget = &স্তূপ_budget;
    যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) ফেরত মিথ্যা;

    যদি (শীর্ষ_maxp_info)
    {
      শীর্ষ_maxp_info->maxComponentDepth = hb_max (শীর্ষ_maxp_info->maxComponentDepth, depth);
    }

    যদি (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->অঙ্ক_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_দৈর্ঘ্য = points.দৈর্ঘ্য;

    নির্বাচন (type) {
    ক্ষেত্র SIMPLE:
      যদি (depth == 0 && শীর্ষ_maxp_info)
        শীর্ষ_maxp_info->maxContours = hb_max (শীর্ষ_maxp_info->maxContours, (unsigned) header->সংখ্যা_Of_Contours);
      যদি (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->সংখ্যা_Of_Contours;
      যদি (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	ফেরত মিথ্যা;
      বিরতি;
    ক্ষেত্র COMPOSITE:
    {
      জন্য (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_দৈর্ঘ্য);
          ফেরত মিথ্যা;
        }
      }
      বিরতি;
    }
    ক্ষেত্র EMPTY:
      বিরতি;
    }

    /* Init phantom points */
    যদি (unlikely (!points.resize (points.দৈর্ঘ্য + PHANTOM_গুনতি))) ফেরত মিথ্যা;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.দৈর্ঘ্য - PHANTOM_গুনতি, PHANTOM_গুনতি);
    {
      // Duplicated code.
      পূর্ণসংখ্যা lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      পূর্ণসংখ্যা h_delta = (পূর্ণসংখ্যা) header->xMin - lsb;
      HB_UNUSED পূর্ণসংখ্যা tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      পূর্ণসংখ্যা v_orig  = (পূর্ণসংখ্যা) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#অন্যথা
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_বাম].x = h_delta;
      phantoms[PHANTOM_ডান].x = (পূর্ণসংখ্যা) h_adv + h_delta;
      phantoms[PHANTOM_শীর্ষ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (পূর্ণসংখ্যা) v_adv;
    }

#ifndef HB_NO_VAR
    যদি (hb_any (coords))
    {
      যদি (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_দৈর্ঘ্য),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        ফেরত মিথ্যা;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    যদি (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      প্রতিজ্ঞা (old_দৈর্ঘ্য == 0);
      *points_with_deltas = points;
    }

    দশমিক shift = 0;
    নির্বাচন (type) {
    ক্ষেত্র SIMPLE:
      যদি (depth == 0 && শীর্ষ_maxp_info)
        শীর্ষ_maxp_info->maxPoints = hb_max (শীর্ষ_maxp_info->maxPoints, all_points.দৈর্ঘ্য - old_দৈর্ঘ্য - 4);
      shift = phantoms[PHANTOM_বাম].x;
      বিরতি;
    ক্ষেত্র COMPOSITE:
    {
      hb_decycler_গ্রন্থি_t decycler_গ্রন্থি (scratch.decycler);

      unsigned পূর্ণসংখ্যা comp_সূচক = 0;
      জন্য (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        যদি (unlikely (!decycler_গ্রন্থি.visit (item_gid)))
	{
	  comp_সূচক++;
	  চালু;
	}

	unsigned old_গুনতি = all_points.দৈর্ঘ্য;

	যদি (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (item_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    শীর্ষ_maxp_info,
						    composite_contours,
						    shift_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    depth + 1,
						    edge_গুনতি,
						    budget)))
	{
	  points.resize (old_দৈর্ঘ্য);
	  ফেরত মিথ্যা;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.দৈর্ঘ্য - PHANTOM_গুনতি, PHANTOM_গুনতি);

	auto comp_points = all_points.as_array ().sub_array (old_গুনতি);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	যদি (use_my_metrics && item.is_use_my_metrics ())
	  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < PHANTOM_গুনতি; i++)
	    phantoms[i] = comp_points[comp_points.দৈর্ঘ্য - PHANTOM_গুনতি + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.দৈর্ঘ্য)))
	{
	  points.resize (old_দৈর্ঘ্য);
	  ফেরত মিথ্যা;
	}

	যদি (comp_points) // Empty in case of phantom_only
	{
	  দশমিক matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_দৈর্ঘ্য + comp_সূচক]);
	}

	যদি (item.is_anchored () && !phantom_only)
	{
	  unsigned পূর্ণসংখ্যা p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  যদি (likely (p1 < all_points.দৈর্ঘ্য && p2 < comp_points.দৈর্ঘ্য))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.দৈর্ঘ্য - PHANTOM_গুনতি);

	যদি (all_points.দৈর্ঘ্য > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_দৈর্ঘ্য);
	  ফেরত মিথ্যা;
	}

	comp_সূচক++;
      }

      যদি (শীর্ষ_maxp_info && depth == 0)
      {
        যদি (composite_contours)
          শীর্ষ_maxp_info->maxCompositeContours = hb_max (শীর্ষ_maxp_info->maxCompositeContours, *composite_contours);
        শীর্ষ_maxp_info->maxCompositePoints = hb_max (শীর্ষ_maxp_info->maxCompositePoints, all_points.দৈর্ঘ্য);
        শীর্ষ_maxp_info->maxComponentElements = hb_max (শীর্ষ_maxp_info->maxComponentElements, comp_সূচক);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_বাম].x;
      points.resize (old_দৈর্ঘ্য);
    } বিরতি;
    ক্ষেত্র EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_বাম].x;
      points.resize (old_দৈর্ঘ্য);
      বিরতি;
    }

    যদি (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      যদি (shift)
        জন্য (auto &point : all_points)
	  point.x -= shift;
    }

    ফেরত !all_points.in_error ();
  }

  যৌক্তিক get_extents_without_var_scaled (hb_font_t *font, ধ্রুবক glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ধ্রুবক
  {
    যদি (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      ফেরত সত্য; /* Empty glyph; zero extents. */
    }
    ফেরত header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ধ্রুবক { ফেরত bytes; }
  glyph_type_t get_type () ধ্রুবক { ফেরত type; }
  ধ্রুবক GlyphHeader *get_header () ধ্রুবক { ফেরত header; }
  যৌক্তিক is_extended () ধ্রুবক { ফেরত extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (মিথ্যা)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 যৌক্তিক extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    পূর্ণসংখ্যা অঙ্ক_contours = header->সংখ্যা_Of_Contours;
    যদি (unlikely (অঙ্ক_contours == 0)) type = EMPTY;
    অন্যথা যদি (অঙ্ক_contours > 0) type = SIMPLE;
    অন্যথা যদি (অঙ্ক_contours <= -1) type = COMPOSITE;
    অন্যথা type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  সুরক্ষিত:
  hb_bytes_t bytes;
  ধ্রুবক GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  যৌক্তিক extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
