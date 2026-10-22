#अगर_अघोषित OT_GLYF_GLYPH_HH
#घोषणा OT_GLYF_GLYPH_HH


#समावेश "../../hb-open-type.hh"

#समावेश "GlyphHeader.hh"
#समावेश "SimpleGlyph.hh"
#समावेश "CompositeGlyph.hh"


namespace OT {

काष्ठा glyf_accelerator_t;

namespace glyf_impl {


क्रमागत phantom_point_सूचकांक_t
{
  PHANTOM_बायाँ   = 0,
  PHANTOM_दायाँ  = 1,
  PHANTOM_शीर्ष    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_गिनती  = 4
};

काष्ठा Glyph
{
  क्रमागत glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  खुला:
  composite_iter_t get_composite_iterator () स्थिर
  {
    अगर (type != COMPOSITE) वापस composite_iter_t ();
    वापस CompositeGlyph (*header, bytes, extended).iter ();
  }

  स्थिर hb_bytes_t trim_padding () स्थिर
  {
    चयन (type) {
    हाल COMPOSITE: वापस CompositeGlyph (*header, bytes, extended).trim_padding ();
    हाल SIMPLE:    वापस SimpleGlyph (*header, bytes).trim_padding ();
    हाल EMPTY:     वापस bytes;
    शेष:        वापस bytes;
    }
  }

  व्योम drop_hints ()
  {
    चयन (type) {
    हाल COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); वापस;
    हाल SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); वापस;
    हाल EMPTY:
      अगर (bytes.लंबाई >= GlyphHeader::static_आकार)
        SimpleGlyph (*header, bytes).drop_hints ();
      वापस;
    }
  }

  व्योम set_overlaps_flag ()
  {
    चयन (type) {
    हाल COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); वापस;
    हाल SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); वापस;
    हाल EMPTY:     वापस;
    }
  }

  व्योम drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) स्थिर
  {
    चयन (type) {
    हाल COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); वापस;
    हाल SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); वापस;
    हाल EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      अगर (bytes.लंबाई >= GlyphHeader::static_आकार &&
          SimpleGlyph (*header, bytes).has_instructions_लंबाई ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      वापस;
    }
  }

  bool is_composite () स्थिर
  { वापस type == COMPOSITE; }

  bool get_all_points_without_var (स्थिर hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) स्थिर
  {
    चयन (type) {
    हाल SIMPLE:
      अगर (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        वापस false;
      अवरोध;
    हाल COMPOSITE:
    {
      क्रम (स्वतः &item : get_composite_iterator ())
        अगर (unlikely (!item.get_points (points, extended))) वापस false;
      अवरोध;
    }
    हाल EMPTY:
      अवरोध;
    }

    /* Init phantom points */
    अगर (unlikely (!points.resize (points.लंबाई + PHANTOM_गिनती))) वापस false;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.लंबाई - PHANTOM_गिनती, PHANTOM_गिनती);
    {
      // Duplicated code.
      पूर्णांक lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णांक h_delta = (पूर्णांक) header->xMin - lsb;
      HB_UNUSED पूर्णांक tsb = 0;
#अगर_अघोषित HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#पूर्ण_अगर
      पूर्णांक v_orig  = (पूर्णांक) header->yMax + tsb;
      अचिन्हित h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#अगर_अघोषित HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - face->get_upem ()
#पूर्ण_अगर
                       ;
      phantoms[PHANTOM_बायाँ].x = h_delta;
      phantoms[PHANTOM_दायाँ].x = (पूर्णांक) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णांक) v_adv;
    }
    वापस true;
  }

  व्योम update_mtx (स्थिर hb_subset_plan_t *plan,
                   पूर्णांक xMin, पूर्णांक xMax,
                   पूर्णांक yMin, पूर्णांक yMax,
                   स्थिर contour_point_vector_t &all_points) स्थिर
  {
    hb_codepoint_t new_gid = 0;
    अगर (!plan->new_gid_for_old_gid (gid, &new_gid))
      वापस;

    अगर (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    अचिन्हित len = all_points.लंबाई;
    भग्न बायाँ_Side_X = all_points[len - 4].x;
    भग्न दायाँ_Side_X = all_points[len - 3].x;
    भग्न शीर्ष_Side_Y = all_points[len - 2].y;
    भग्न bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    चिन्हित hori_aw = roundf (दायाँ_Side_X - बायाँ_Side_X);
    अगर (hori_aw < 0) hori_aw = 0;
    पूर्णांक lsb = roundf (xMin - बायाँ_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    अगर (type != EMPTY && lsb != xMin)
      plan->शीर्ष_maxp_info.allXMinIsLsb = false;

    चिन्हित vert_aw = roundf (शीर्ष_Side_Y - bottomSideY);
    अगर (vert_aw < 0) vert_aw = 0;
    पूर्णांक tsb = roundf (शीर्ष_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) vert_aw, tsb));
  }

  bool compile_header_bytes (स्थिर hb_subset_plan_t *plan,
                             स्थिर contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) स्थिर
  {
    GlyphHeader *glyph_header = nullptr;
    अगर (!plan->pinned_at_default && type != EMPTY && all_points.लंबाई >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_आकार);
      अगर (unlikely (!glyph_header)) वापस false;
    }

    भग्न xMin = 0, xMax = 0;
    भग्न yMin = 0, yMax = 0;
    अगर (all_points.लंबाई > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      अचिन्हित गिनती = all_points.लंबाई - 4;
      क्रम (अचिन्हित i = 1; i < गिनती; i++)
      {
	भग्न x = all_points[i].x;
	भग्न y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    पूर्णांक rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    पूर्णांक rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    पूर्णांक rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    पूर्णांक rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    अगर (type != EMPTY)
    {
      plan->शीर्ष_maxp_info.xMin = hb_min (plan->शीर्ष_maxp_info.xMin, rounded_xMin);
      plan->शीर्ष_maxp_info.yMin = hb_min (plan->शीर्ष_maxp_info.yMin, rounded_yMin);
      plan->शीर्ष_maxp_info.xMax = hb_max (plan->शीर्ष_maxp_info.xMax, rounded_xMax);
      plan->शीर्ष_maxp_info.yMax = hb_max (plan->शीर्ष_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    अगर (!glyph_header)
      वापस true;

    glyph_header->संख्या_Of_Contours = header->संख्या_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((स्थिर अक्षर *)glyph_header, GlyphHeader::static_आकार);
    वापस true;
  }

  bool compile_bytes_with_deltas (स्थिर hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  स्थिर glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    अचिन्हित composite_contours = 0;
    शीर्ष_maxp_info_t *शीर्ष_maxp_info_p = &plan->शीर्ष_maxp_info;
    अचिन्हित *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    अगर (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      शीर्ष_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    अगर (!get_points (font, glyf, all_points, scratch, &points_with_deltas, शीर्ष_maxp_info_p, composite_contours_p, false, false))
      वापस false;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    अगर (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    अगर (!plan->pinned_at_default)
    {
      चयन (type)
      {
      हाल COMPOSITE:
        अगर (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          वापस false;
        अवरोध;
      हाल SIMPLE:
        अगर (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          वापस false;
        अवरोध;
      हाल EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        अगर (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.लंबाई >= GlyphHeader::static_आकार &&
            header->संख्या_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          अचिन्हित लंबाई = simple.instructions_लंबाई ();
          अगर (लंबाई)
          {
            लंबाई = simple.लंबाई (लंबाई);
            अक्षर *p = (अक्षर *) hb_malloc (लंबाई);
            अगर (unlikely (!p)) वापस false;
            hb_memcpy (p, bytes.arrayZ, लंबाई);
            dest_start = hb_bytes_t (p, लंबाई);
          }
        }
        अवरोध;
      }
    }

    अगर (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      वापस false;
    }
    वापस true;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  ढाँचा <typename accelerator_t>
  bool get_points (hb_font_t *font, स्थिर accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   शीर्ष_maxp_info_t * शीर्ष_maxp_info = nullptr, /* OUT */
		   अचिन्हित *composite_contours = nullptr, /* OUT */
		   bool shift_points_hori = true,
		   bool use_my_metrics = true,
		   bool phantom_only = false,
		   hb_array_t<स्थिर पूर्णांक> coords = hb_array_t<स्थिर पूर्णांक> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   अचिन्हित पूर्णांक depth = 0,
		   अचिन्हित *edge_गिनती = nullptr,
		   int64_t *budget = nullptr) स्थिर
  {
    अगर (unlikely (depth > HB_MAX_NESTING_LEVEL)) वापस false;
    अचिन्हित ढेर_edge_गिनती = 0;
    अगर (!edge_गिनती) edge_गिनती = &ढेर_edge_गिनती;
    अगर (unlikely (*edge_गिनती > HB_MAX_GRAPH_EDGE_गिनती)) वापस false;
    (*edge_गिनती)++;

    int64_t ढेर_budget = HB_BUDGET_GLYPH;
    अगर (!budget) budget = &ढेर_budget;
    अगर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) वापस false;

    अगर (शीर्ष_maxp_info)
    {
      शीर्ष_maxp_info->maxComponentDepth = hb_max (शीर्ष_maxp_info->maxComponentDepth, depth);
    }

    अगर (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->अंक_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    अचिन्हित old_लंबाई = points.लंबाई;

    चयन (type) {
    हाल SIMPLE:
      अगर (depth == 0 && शीर्ष_maxp_info)
        शीर्ष_maxp_info->maxContours = hb_max (शीर्ष_maxp_info->maxContours, (अचिन्हित) header->संख्या_Of_Contours);
      अगर (depth > 0 && composite_contours)
        *composite_contours += (अचिन्हित) header->संख्या_Of_Contours;
      अगर (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	वापस false;
      अवरोध;
    हाल COMPOSITE:
    {
      क्रम (स्वतः &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        अगर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_लंबाई);
          वापस false;
        }
      }
      अवरोध;
    }
    हाल EMPTY:
      अवरोध;
    }

    /* Init phantom points */
    अगर (unlikely (!points.resize (points.लंबाई + PHANTOM_गिनती))) वापस false;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.लंबाई - PHANTOM_गिनती, PHANTOM_गिनती);
    {
      // Duplicated code.
      पूर्णांक lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णांक h_delta = (पूर्णांक) header->xMin - lsb;
      HB_UNUSED पूर्णांक tsb = 0;
#अगर_अघोषित HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#पूर्ण_अगर
      पूर्णांक v_orig  = (पूर्णांक) header->yMax + tsb;
      अचिन्हित h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#अगर_अघोषित HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - font->face->get_upem ()
#पूर्ण_अगर
                       ;
      phantoms[PHANTOM_बायाँ].x = h_delta;
      phantoms[PHANTOM_दायाँ].x = (पूर्णांक) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णांक) v_adv;
    }

#अगर_अघोषित HB_NO_VAR
    अगर (hb_any (coords))
    {
      अगर (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_लंबाई),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        वापस false;
    }
#पूर्ण_अगर

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    अगर (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      निश्चित (old_लंबाई == 0);
      *points_with_deltas = points;
    }

    भग्न shift = 0;
    चयन (type) {
    हाल SIMPLE:
      अगर (depth == 0 && शीर्ष_maxp_info)
        शीर्ष_maxp_info->maxPoints = hb_max (शीर्ष_maxp_info->maxPoints, all_points.लंबाई - old_लंबाई - 4);
      shift = phantoms[PHANTOM_बायाँ].x;
      अवरोध;
    हाल COMPOSITE:
    {
      hb_decycler_गाँठ_t decycler_गाँठ (scratch.decycler);

      अचिन्हित पूर्णांक comp_सूचकांक = 0;
      क्रम (स्वतः &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        अगर (unlikely (!decycler_गाँठ.visit (item_gid)))
	{
	  comp_सूचकांक++;
	  जारी;
	}

	अचिन्हित old_गिनती = all_points.लंबाई;

	अगर (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (item_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    शीर्ष_maxp_info,
						    composite_contours,
						    shift_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    depth + 1,
						    edge_गिनती,
						    budget)))
	{
	  points.resize (old_लंबाई);
	  वापस false;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.लंबाई - PHANTOM_गिनती, PHANTOM_गिनती);

	स्वतः comp_points = all_points.as_array ().sub_array (old_गिनती);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	अगर (use_my_metrics && item.is_use_my_metrics ())
	  क्रम (अचिन्हित पूर्णांक i = 0; i < PHANTOM_गिनती; i++)
	    phantoms[i] = comp_points[comp_points.लंबाई - PHANTOM_गिनती + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	अगर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.लंबाई)))
	{
	  points.resize (old_लंबाई);
	  वापस false;
	}

	अगर (comp_points) // Empty in case of phantom_only
	{
	  भग्न matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_लंबाई + comp_सूचकांक]);
	}

	अगर (item.is_anchored () && !phantom_only)
	{
	  अचिन्हित पूर्णांक p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  अगर (likely (p1 < all_points.लंबाई && p2 < comp_points.लंबाई))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.लंबाई - PHANTOM_गिनती);

	अगर (all_points.लंबाई > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_लंबाई);
	  वापस false;
	}

	comp_सूचकांक++;
      }

      अगर (शीर्ष_maxp_info && depth == 0)
      {
        अगर (composite_contours)
          शीर्ष_maxp_info->maxCompositeContours = hb_max (शीर्ष_maxp_info->maxCompositeContours, *composite_contours);
        शीर्ष_maxp_info->maxCompositePoints = hb_max (शीर्ष_maxp_info->maxCompositePoints, all_points.लंबाई);
        शीर्ष_maxp_info->maxComponentElements = hb_max (शीर्ष_maxp_info->maxComponentElements, comp_सूचकांक);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_बायाँ].x;
      points.resize (old_लंबाई);
    } अवरोध;
    हाल EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_बायाँ].x;
      points.resize (old_लंबाई);
      अवरोध;
    }

    अगर (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      अगर (shift)
        क्रम (स्वतः &point : all_points)
	  point.x -= shift;
    }

    वापस !all_points.in_error ();
  }

  bool get_extents_without_var_scaled (hb_font_t *font, स्थिर glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) स्थिर
  {
    अगर (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      वापस true; /* Empty glyph; zero extents. */
    }
    वापस header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () स्थिर { वापस bytes; }
  glyph_type_t get_type () स्थिर { वापस type; }
  स्थिर GlyphHeader *get_header () स्थिर { वापस header; }
  bool is_extended () स्थिर { वापस extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (false)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 bool extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    पूर्णांक अंक_contours = header->संख्या_Of_Contours;
    अगर (unlikely (अंक_contours == 0)) type = EMPTY;
    अन्यथा अगर (अंक_contours > 0) type = SIMPLE;
    अन्यथा अगर (अंक_contours <= -1) type = COMPOSITE;
    अन्यथा type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  रक्षित:
  hb_bytes_t bytes;
  स्थिर GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  bool extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_GLYPH_HH */
