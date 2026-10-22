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
  PHANTOM_बायाँ   = 0,
  PHANTOM_दायाँ  = 1,
  PHANTOM_शीर्ष    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_गन्ती  = 4
};

संरचना Glyph
{
  गणना glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  सार्वजनिक:
  composite_iter_t get_composite_iterator () अचर
  {
    यदि (type != COMPOSITE) फर्काऊ composite_iter_t ();
    फर्काऊ CompositeGlyph (*header, bytes, extended).iter ();
  }

  अचर hb_bytes_t trim_padding () अचर
  {
    विकल्प (type) {
    पक्ष COMPOSITE: फर्काऊ CompositeGlyph (*header, bytes, extended).trim_padding ();
    पक्ष SIMPLE:    फर्काऊ SimpleGlyph (*header, bytes).trim_padding ();
    पक्ष EMPTY:     फर्काऊ bytes;
    स्वभाव:        फर्काऊ bytes;
    }
  }

  शून्य drop_hints ()
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); फर्काऊ;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); फर्काऊ;
    पक्ष EMPTY:
      यदि (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      फर्काऊ;
    }
  }

  शून्य set_overlaps_flag ()
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); फर्काऊ;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); फर्काऊ;
    पक्ष EMPTY:     फर्काऊ;
    }
  }

  शून्य drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) अचर
  {
    विकल्प (type) {
    पक्ष COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); फर्काऊ;
    पक्ष SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); फर्काऊ;
    पक्ष EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      यदि (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      फर्काऊ;
    }
  }

  तार्किक is_composite () अचर
  { फर्काऊ type == COMPOSITE; }

  तार्किक get_all_points_without_var (अचर hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) अचर
  {
    विकल्प (type) {
    पक्ष SIMPLE:
      यदि (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        फर्काऊ असत्य;
      रोक;
    पक्ष COMPOSITE:
    {
      लागि (स्वतः &item : get_composite_iterator ())
        यदि (unlikely (!item.get_points (points, extended))) फर्काऊ असत्य;
      रोक;
    }
    पक्ष EMPTY:
      रोक;
    }

    /* Init phantom points */
    यदि (unlikely (!points.resize (points.length + PHANTOM_गन्ती))) फर्काऊ असत्य;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_गन्ती, PHANTOM_गन्ती);
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
      अचिन्हित h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_बायाँ].x = h_delta;
      phantoms[PHANTOM_दायाँ].x = (पूर्णाङ्क) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णाङ्क) v_adv;
    }
    फर्काऊ सत्य;
  }

  शून्य update_mtx (अचर hb_subset_plan_t *plan,
                   पूर्णाङ्क xMin, पूर्णाङ्क xMax,
                   पूर्णाङ्क yMin, पूर्णाङ्क yMax,
                   अचर contour_point_vector_t &all_points) अचर
  {
    hb_codepoint_t new_gid = 0;
    यदि (!plan->new_gid_for_old_gid (gid, &new_gid))
      फर्काऊ;

    यदि (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    अचिन्हित len = all_points.length;
    भिन्न बायाँ_Side_X = all_points[len - 4].x;
    भिन्न दायाँ_Side_X = all_points[len - 3].x;
    भिन्न शीर्ष_Side_Y = all_points[len - 2].y;
    भिन्न bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    चिन्हित hori_aw = roundf (दायाँ_Side_X - बायाँ_Side_X);
    यदि (hori_aw < 0) hori_aw = 0;
    पूर्णाङ्क lsb = roundf (xMin - बायाँ_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    यदि (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = असत्य;

    चिन्हित vert_aw = roundf (शीर्ष_Side_Y - bottomSideY);
    यदि (vert_aw < 0) vert_aw = 0;
    पूर्णाङ्क tsb = roundf (शीर्ष_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) vert_aw, tsb));
  }

  तार्किक compile_header_bytes (अचर hb_subset_plan_t *plan,
                             अचर contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) अचर
  {
    GlyphHeader *glyph_header = nullptr;
    यदि (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      यदि (unlikely (!glyph_header)) फर्काऊ असत्य;
    }

    भिन्न xMin = 0, xMax = 0;
    भिन्न yMin = 0, yMax = 0;
    यदि (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      अचिन्हित गन्ती = all_points.length - 4;
      लागि (अचिन्हित i = 1; i < गन्ती; i++)
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

    यदि (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    यदि (!glyph_header)
      फर्काऊ सत्य;

    glyph_header->सङ्ख्या_Of_Contours = header->सङ्ख्या_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((अचर वर्ण *)glyph_header, GlyphHeader::static_size);
    फर्काऊ सत्य;
  }

  तार्किक compile_bytes_with_deltas (अचर hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  अचर glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    अचिन्हित composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    अचिन्हित *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    यदि (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    यदि (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, असत्य, असत्य))
      फर्काऊ असत्य;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    यदि (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    यदि (!plan->pinned_at_default)
    {
      विकल्प (type)
      {
      पक्ष COMPOSITE:
        यदि (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          फर्काऊ असत्य;
        रोक;
      पक्ष SIMPLE:
        यदि (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          फर्काऊ असत्य;
        रोक;
      पक्ष EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        यदि (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->सङ्ख्या_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          अचिन्हित length = simple.instructions_length ();
          यदि (length)
          {
            length = simple.length (length);
            वर्ण *p = (वर्ण *) hb_malloc (length);
            यदि (unlikely (!p)) फर्काऊ असत्य;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        रोक;
      }
    }

    यदि (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  ढाँचा <typename accelerator_t>
  तार्किक get_points (hb_font_t *font, अचर accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   अचिन्हित *composite_contours = nullptr, /* OUT */
		   तार्किक shift_points_hori = सत्य,
		   तार्किक use_my_metrics = सत्य,
		   तार्किक phantom_only = असत्य,
		   hb_array_t<अचर पूर्णाङ्क> coords = hb_array_t<अचर पूर्णाङ्क> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   अचिन्हित पूर्णाङ्क depth = 0,
		   अचिन्हित *edge_गन्ती = nullptr,
		   int64_t *budget = nullptr) अचर
  {
    यदि (unlikely (depth > HB_MAX_NESTING_LEVEL)) फर्काऊ असत्य;
    अचिन्हित थाक_edge_गन्ती = 0;
    यदि (!edge_गन्ती) edge_गन्ती = &थाक_edge_गन्ती;
    यदि (unlikely (*edge_गन्ती > HB_MAX_GRAPH_EDGE_गन्ती)) फर्काऊ असत्य;
    (*edge_गन्ती)++;

    int64_t थाक_budget = HB_BUDGET_GLYPH;
    यदि (!budget) budget = &थाक_budget;
    यदि (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) फर्काऊ असत्य;

    यदि (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    यदि (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->अङ्क_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    अचिन्हित old_length = points.length;

    विकल्प (type) {
    पक्ष SIMPLE:
      यदि (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (अचिन्हित) header->सङ्ख्या_Of_Contours);
      यदि (depth > 0 && composite_contours)
        *composite_contours += (अचिन्हित) header->सङ्ख्या_Of_Contours;
      यदि (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	फर्काऊ असत्य;
      रोक;
    पक्ष COMPOSITE:
    {
      लागि (स्वतः &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        यदि (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          फर्काऊ असत्य;
        }
      }
      रोक;
    }
    पक्ष EMPTY:
      रोक;
    }

    /* Init phantom points */
    यदि (unlikely (!points.resize (points.length + PHANTOM_गन्ती))) फर्काऊ असत्य;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_गन्ती, PHANTOM_गन्ती);
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
      अचिन्हित h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_बायाँ].x = h_delta;
      phantoms[PHANTOM_दायाँ].x = (पूर्णाङ्क) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णाङ्क) v_adv;
    }

#ifndef HB_NO_VAR
    यदि (hb_any (coords))
    {
      यदि (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        फर्काऊ असत्य;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    यदि (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      प्रतिज्ञा (old_length == 0);
      *points_with_deltas = points;
    }

    भिन्न shift = 0;
    विकल्प (type) {
    पक्ष SIMPLE:
      यदि (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_बायाँ].x;
      रोक;
    पक्ष COMPOSITE:
    {
      hb_decycler_गाँठो_t decycler_गाँठो (scratch.decycler);

      अचिन्हित पूर्णाङ्क comp_index = 0;
      लागि (स्वतः &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        यदि (unlikely (!decycler_गाँठो.visit (item_gid)))
	{
	  comp_index++;
	  जारी;
	}

	अचिन्हित old_गन्ती = all_points.length;

	यदि (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_गन्ती,
						    budget)))
	{
	  points.resize (old_length);
	  फर्काऊ असत्य;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_गन्ती, PHANTOM_गन्ती);

	स्वतः comp_points = all_points.as_array ().sub_array (old_गन्ती);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	यदि (use_my_metrics && item.is_use_my_metrics ())
	  लागि (अचिन्हित पूर्णाङ्क i = 0; i < PHANTOM_गन्ती; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_गन्ती + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	यदि (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  फर्काऊ असत्य;
	}

	यदि (comp_points) // Empty in case of phantom_only
	{
	  भिन्न matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	यदि (item.is_anchored () && !phantom_only)
	{
	  अचिन्हित पूर्णाङ्क p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  यदि (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_गन्ती);

	यदि (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  फर्काऊ असत्य;
	}

	comp_index++;
      }

      यदि (head_maxp_info && depth == 0)
      {
        यदि (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_बायाँ].x;
      points.resize (old_length);
    } रोक;
    पक्ष EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_बायाँ].x;
      points.resize (old_length);
      रोक;
    }

    यदि (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      यदि (shift)
        लागि (स्वतः &point : all_points)
	  point.x -= shift;
    }

    फर्काऊ !all_points.in_error ();
  }

  तार्किक get_extents_without_var_scaled (hb_font_t *font, अचर glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) अचर
  {
    यदि (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      फर्काऊ सत्य; /* Empty glyph; zero extents. */
    }
    फर्काऊ header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () अचर { फर्काऊ bytes; }
  glyph_type_t get_type () अचर { फर्काऊ type; }
  अचर GlyphHeader *get_header () अचर { फर्काऊ header; }
  तार्किक is_extended () अचर { फर्काऊ extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (असत्य)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 तार्किक extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    पूर्णाङ्क अङ्क_contours = header->सङ्ख्या_Of_Contours;
    यदि (unlikely (अङ्क_contours == 0)) type = EMPTY;
    अन्यथा यदि (अङ्क_contours > 0) type = SIMPLE;
    अन्यथा यदि (अङ्क_contours <= -1) type = COMPOSITE;
    अन्यथा type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  रक्षित:
  hb_bytes_t bytes;
  अचर GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
