#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#अन्तर्भाव "../../hb-open-type.hh"

#अन्तर्भाव "GlyphHeader.hh"
#अन्तर्भाव "SimpleGlyph.hh"
#अन्तर्भाव "CompositeGlyph.hh"


नामाकाश OT {

संरचना glyf_accelerator_t;

नामाकाश glyf_impl {


गणना phantom_point_index_t
{
  PHANTOM_वामः   = 0,
  PHANTOM_दक्षिणः  = 1,
  PHANTOM_शिखरम्    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_गणनम्  = 4
};

संरचना Glyph
{
  गणना glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  सार्वजनिक:
  composite_iter_t get_composite_iterator () ध्रुव
  {
    सचे (type != COMPOSITE) पटिनिवत्त composite_iter_t ();
    पटिनिवत्त CompositeGlyph (*header, bytes, extended).iter ();
  }

  ध्रुव hb_bytes_t trim_padding () ध्रुव
  {
    विकल्प (type) {
    पक्ष COMPOSITE: पटिनिवत्त CompositeGlyph (*header, bytes, extended).trim_padding ();
    पक्ष SIMPLE:    पटिनिवत्त SimpleGlyph (*header, bytes).trim_padding ();
    पक्ष EMPTY:     पटिनिवत्त bytes;
    स्वभाव:        पटिनिवत्त bytes;
    }
  }

  शून्य drop_hints ()
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); पटिनिवत्त;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); पटिनिवत्त;
    पक्ष EMPTY:
      सचे (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      पटिनिवत्त;
    }
  }

  शून्य set_overlaps_flag ()
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); पटिनिवत्त;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); पटिनिवत्त;
    पक्ष EMPTY:     पटिनिवत्त;
    }
  }

  शून्य drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ध्रुव
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); पटिनिवत्त;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); पटिनिवत्त;
    पक्ष EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      सचे (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      पटिनिवत्त;
    }
  }

  तार्किक is_composite () ध्रुव
  { पटिनिवत्त type == COMPOSITE; }

  तार्किक get_all_points_without_var (ध्रुव hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ध्रुव
  {
    विकल्प (type) {
    पक्ष SIMPLE:
      सचे (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        पटिनिवत्त मिच्छा;
      भङ्ग;
    पक्ष COMPOSITE:
    {
      अत्थाय (auto &item : get_composite_iterator ())
        सचे (unlikely (!item.get_points (points, extended))) पटिनिवत्त मिच्छा;
      भङ्ग;
    }
    पक्ष EMPTY:
      भङ्ग;
    }

    /* Init phantom points */
    सचे (unlikely (!points.resize (points.length + PHANTOM_गणनम्))) पटिनिवत्त मिच्छा;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_गणनम्, PHANTOM_गणनम्);
    {
      // Duplicated code.
      पूर्णाङ्क lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णाङ्क h_delta = (पूर्णाङ्क) header->xMin - lsb;
      HB_UNUSED पूर्णाङ्क tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      पूर्णाङ्क v_orig  = (पूर्णाङ्क) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#अञ्ञथा
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_वामः].x = h_delta;
      phantoms[PHANTOM_दक्षिणः].x = (पूर्णाङ्क) h_adv + h_delta;
      phantoms[PHANTOM_शिखरम्].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णाङ्क) v_adv;
    }
    पटिनिवत्त सच्च;
  }

  शून्य update_mtx (ध्रुव hb_subset_plan_t *plan,
                   पूर्णाङ्क xMin, पूर्णाङ्क xMax,
                   पूर्णाङ्क yMin, पूर्णाङ्क yMax,
                   ध्रुव contour_point_vector_t &all_points) ध्रुव
  {
    hb_codepoint_t new_gid = 0;
    सचे (!plan->new_gid_for_old_gid (gid, &new_gid))
      पटिनिवत्त;

    सचे (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    भिन्न वामः_Side_X = all_points[len - 4].x;
    भिन्न दक्षिणः_Side_X = all_points[len - 3].x;
    भिन्न शिखरम्_Side_Y = all_points[len - 2].y;
    भिन्न bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (दक्षिणः_Side_X - वामः_Side_X);
    सचे (hori_aw < 0) hori_aw = 0;
    पूर्णाङ्क lsb = roundf (xMin - वामः_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    सचे (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = मिच्छा;

    signed vert_aw = roundf (शिखरम्_Side_Y - bottomSideY);
    सचे (vert_aw < 0) vert_aw = 0;
    पूर्णाङ्क tsb = roundf (शिखरम्_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  तार्किक compile_header_bytes (ध्रुव hb_subset_plan_t *plan,
                             ध्रुव contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ध्रुव
  {
    GlyphHeader *glyph_header = nullptr;
    सचे (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      सचे (unlikely (!glyph_header)) पटिनिवत्त मिच्छा;
    }

    भिन्न xMin = 0, xMax = 0;
    भिन्न yMin = 0, yMax = 0;
    सचे (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned गणनम् = all_points.length - 4;
      अत्थाय (unsigned i = 1; i < गणनम्; i++)
      {
	भिन्न x = all_points[i].x;
	भिन्न y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    पूर्णाङ्क rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    पूर्णाङ्क rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    पूर्णाङ्क rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    पूर्णाङ्क rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    सचे (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    सचे (!glyph_header)
      पटिनिवत्त सच्च;

    glyph_header->सङ्ख्या_Of_Contours = header->सङ्ख्या_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ध्रुव वर्ण *)glyph_header, GlyphHeader::static_size);
    पटिनिवत्त सच्च;
  }

  तार्किक compile_bytes_with_deltas (ध्रुव hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ध्रुव glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    सचे (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    सचे (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, मिच्छा, मिच्छा))
      पटिनिवत्त मिच्छा;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    सचे (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    सचे (!plan->pinned_at_default)
    {
      विकल्प (type)
      {
      पक्ष COMPOSITE:
        सचे (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          पटिनिवत्त मिच्छा;
        भङ्ग;
      पक्ष SIMPLE:
        सचे (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          पटिनिवत्त मिच्छा;
        भङ्ग;
      पक्ष EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        सचे (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->सङ्ख्या_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          सचे (length)
          {
            length = simple.length (length);
            वर्ण *p = (वर्ण *) hb_malloc (length);
            सचे (unlikely (!p)) पटिनिवत्त मिच्छा;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        भङ्ग;
      }
    }

    सचे (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  तार्किक get_points (hb_font_t *font, ध्रुव accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   तार्किक shift_points_hori = सच्च,
		   तार्किक use_my_metrics = सच्च,
		   तार्किक phantom_only = मिच्छा,
		   hb_array_t<ध्रुव पूर्णाङ्क> coords = hb_array_t<ध्रुव पूर्णाङ्क> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned पूर्णाङ्क depth = 0,
		   unsigned *edge_गणनम् = nullptr,
		   int64_t *budget = nullptr) ध्रुव
  {
    सचे (unlikely (depth > HB_MAX_NESTING_LEVEL)) पटिनिवत्त मिच्छा;
    unsigned राशिः_edge_गणनम् = 0;
    सचे (!edge_गणनम्) edge_गणनम् = &राशिः_edge_गणनम्;
    सचे (unlikely (*edge_गणनम् > HB_MAX_GRAPH_EDGE_गणनम्)) पटिनिवत्त मिच्छा;
    (*edge_गणनम्)++;

    int64_t राशिः_budget = HB_BUDGET_GLYPH;
    सचे (!budget) budget = &राशिः_budget;
    सचे (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) पटिनिवत्त मिच्छा;

    सचे (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    सचे (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->अङ्कः_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    विकल्प (type) {
    पक्ष SIMPLE:
      सचे (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->सङ्ख्या_Of_Contours);
      सचे (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->सङ्ख्या_Of_Contours;
      सचे (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	पटिनिवत्त मिच्छा;
      भङ्ग;
    पक्ष COMPOSITE:
    {
      अत्थाय (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        सचे (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          पटिनिवत्त मिच्छा;
        }
      }
      भङ्ग;
    }
    पक्ष EMPTY:
      भङ्ग;
    }

    /* Init phantom points */
    सचे (unlikely (!points.resize (points.length + PHANTOM_गणनम्))) पटिनिवत्त मिच्छा;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_गणनम्, PHANTOM_गणनम्);
    {
      // Duplicated code.
      पूर्णाङ्क lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णाङ्क h_delta = (पूर्णाङ्क) header->xMin - lsb;
      HB_UNUSED पूर्णाङ्क tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      पूर्णाङ्क v_orig  = (पूर्णाङ्क) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#अञ्ञथा
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_वामः].x = h_delta;
      phantoms[PHANTOM_दक्षिणः].x = (पूर्णाङ्क) h_adv + h_delta;
      phantoms[PHANTOM_शिखरम्].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णाङ्क) v_adv;
    }

#ifndef HB_NO_VAR
    सचे (hb_any (coords))
    {
      सचे (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        पटिनिवत्त मिच्छा;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    सचे (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      प्रतिज्ञा (old_length == 0);
      *points_with_deltas = points;
    }

    भिन्न shift = 0;
    विकल्प (type) {
    पक्ष SIMPLE:
      सचे (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_वामः].x;
      भङ्ग;
    पक्ष COMPOSITE:
    {
      hb_decycler_ग्रन्थिः_t decycler_ग्रन्थिः (scratch.decycler);

      unsigned पूर्णाङ्क comp_index = 0;
      अत्थाय (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        सचे (unlikely (!decycler_ग्रन्थिः.visit (item_gid)))
	{
	  comp_index++;
	  अनुवर्त;
	}

	unsigned old_गणनम् = all_points.length;

	सचे (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (item_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    head_maxp_info,
						    composite_contours,
						    shift_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    depth + 1,
						    edge_गणनम्,
						    budget)))
	{
	  points.resize (old_length);
	  पटिनिवत्त मिच्छा;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_गणनम्, PHANTOM_गणनम्);

	auto comp_points = all_points.as_array ().sub_array (old_गणनम्);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	सचे (use_my_metrics && item.is_use_my_metrics ())
	  अत्थाय (unsigned पूर्णाङ्क i = 0; i < PHANTOM_गणनम्; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_गणनम् + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	सचे (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  पटिनिवत्त मिच्छा;
	}

	सचे (comp_points) // Empty in case of phantom_only
	{
	  भिन्न matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	सचे (item.is_anchored () && !phantom_only)
	{
	  unsigned पूर्णाङ्क p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  सचे (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_गणनम्);

	सचे (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  पटिनिवत्त मिच्छा;
	}

	comp_index++;
      }

      सचे (head_maxp_info && depth == 0)
      {
        सचे (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_वामः].x;
      points.resize (old_length);
    } भङ्ग;
    पक्ष EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_वामः].x;
      points.resize (old_length);
      भङ्ग;
    }

    सचे (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      सचे (shift)
        अत्थाय (auto &point : all_points)
	  point.x -= shift;
    }

    पटिनिवत्त !all_points.in_error ();
  }

  तार्किक get_extents_without_var_scaled (hb_font_t *font, ध्रुव glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ध्रुव
  {
    सचे (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      पटिनिवत्त सच्च; /* Empty glyph; zero extents. */
    }
    पटिनिवत्त header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ध्रुव { पटिनिवत्त bytes; }
  glyph_type_t get_type () ध्रुव { पटिनिवत्त type; }
  ध्रुव GlyphHeader *get_header () ध्रुव { पटिनिवत्त header; }
  तार्किक is_extended () ध्रुव { पटिनिवत्त extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (मिच्छा)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 तार्किक extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    पूर्णाङ्क अङ्कः_contours = header->सङ्ख्या_Of_Contours;
    सचे (unlikely (अङ्कः_contours == 0)) type = EMPTY;
    अञ्ञथा सचे (अङ्कः_contours > 0) type = SIMPLE;
    अञ्ञथा सचे (अङ्कः_contours <= -1) type = COMPOSITE;
    अञ्ञथा type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  रक्षित:
  hb_bytes_t bytes;
  ध्रुव GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
