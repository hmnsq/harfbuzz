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
  PHANTOM_डावा   = 0,
  PHANTOM_उजवा  = 1,
  PHANTOM_शीर्ष    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_मोजणी  = 4
};

संरचना Glyph
{
  गणना glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  सार्वजनिक:
  composite_iter_t get_composite_iterator () स्थिरांक
  {
    जर (type != COMPOSITE) परत composite_iter_t ();
    परत CompositeGlyph (*header, bytes, extended).iter ();
  }

  स्थिरांक hb_bytes_t trim_padding () स्थिरांक
  {
    निवड (type) {
    स्थिती COMPOSITE: परत CompositeGlyph (*header, bytes, extended).trim_padding ();
    स्थिती SIMPLE:    परत SimpleGlyph (*header, bytes).trim_padding ();
    स्थिती EMPTY:     परत bytes;
    पूर्वनिर्धारित:        परत bytes;
    }
  }

  शून्य drop_hints ()
  {
    निवड (type) {
    स्थिती COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); परत;
    स्थिती SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); परत;
    स्थिती EMPTY:
      जर (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      परत;
    }
  }

  शून्य set_overlaps_flag ()
  {
    निवड (type) {
    स्थिती COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); परत;
    स्थिती SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); परत;
    स्थिती EMPTY:     परत;
    }
  }

  शून्य drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) स्थिरांक
  {
    निवड (type) {
    स्थिती COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); परत;
    स्थिती SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); परत;
    स्थिती EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      जर (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      परत;
    }
  }

  तार्किक is_composite () स्थिरांक
  { परत type == COMPOSITE; }

  तार्किक get_all_points_without_var (स्थिरांक hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) स्थिरांक
  {
    निवड (type) {
    स्थिती SIMPLE:
      जर (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        परत असत्य;
      थांबा;
    स्थिती COMPOSITE:
    {
      साठी (स्वतः &item : get_composite_iterator ())
        जर (unlikely (!item.get_points (points, extended))) परत असत्य;
      थांबा;
    }
    स्थिती EMPTY:
      थांबा;
    }

    /* Init phantom points */
    जर (unlikely (!points.resize (points.length + PHANTOM_मोजणी))) परत असत्य;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_मोजणी, PHANTOM_मोजणी);
    {
      // Duplicated code.
      पूर्णांक lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णांक h_delta = (पूर्णांक) header->xMin - lsb;
      HB_UNUSED पूर्णांक tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      पूर्णांक v_orig  = (पूर्णांक) header->yMax + tsb;
      अचिन्हित h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_डावा].x = h_delta;
      phantoms[PHANTOM_उजवा].x = (पूर्णांक) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णांक) v_adv;
    }
    परत सत्य;
  }

  शून्य update_mtx (स्थिरांक hb_subset_plan_t *plan,
                   पूर्णांक xMin, पूर्णांक xMax,
                   पूर्णांक yMin, पूर्णांक yMax,
                   स्थिरांक contour_point_vector_t &all_points) स्थिरांक
  {
    hb_codepoint_t new_gid = 0;
    जर (!plan->new_gid_for_old_gid (gid, &new_gid))
      परत;

    जर (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    अचिन्हित len = all_points.length;
    दशांश डावा_Side_X = all_points[len - 4].x;
    दशांश उजवा_Side_X = all_points[len - 3].x;
    दशांश शीर्ष_Side_Y = all_points[len - 2].y;
    दशांश bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    चिन्हित hori_aw = roundf (उजवा_Side_X - डावा_Side_X);
    जर (hori_aw < 0) hori_aw = 0;
    पूर्णांक lsb = roundf (xMin - डावा_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    जर (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = असत्य;

    चिन्हित vert_aw = roundf (शीर्ष_Side_Y - bottomSideY);
    जर (vert_aw < 0) vert_aw = 0;
    पूर्णांक tsb = roundf (शीर्ष_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((अचिन्हित) vert_aw, tsb));
  }

  तार्किक compile_header_bytes (स्थिरांक hb_subset_plan_t *plan,
                             स्थिरांक contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) स्थिरांक
  {
    GlyphHeader *glyph_header = nullptr;
    जर (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      जर (unlikely (!glyph_header)) परत असत्य;
    }

    दशांश xMin = 0, xMax = 0;
    दशांश yMin = 0, yMax = 0;
    जर (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      अचिन्हित मोजणी = all_points.length - 4;
      साठी (अचिन्हित i = 1; i < मोजणी; i++)
      {
	दशांश x = all_points[i].x;
	दशांश y = all_points[i].y;
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

    जर (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    जर (!glyph_header)
      परत सत्य;

    glyph_header->संख्या_Of_Contours = header->संख्या_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((स्थिरांक अक्षर *)glyph_header, GlyphHeader::static_size);
    परत सत्य;
  }

  तार्किक compile_bytes_with_deltas (स्थिरांक hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  स्थिरांक glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    अचिन्हित composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    अचिन्हित *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    जर (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    जर (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, असत्य, असत्य))
      परत असत्य;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    जर (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    जर (!plan->pinned_at_default)
    {
      निवड (type)
      {
      स्थिती COMPOSITE:
        जर (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          परत असत्य;
        थांबा;
      स्थिती SIMPLE:
        जर (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          परत असत्य;
        थांबा;
      स्थिती EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        जर (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->संख्या_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          अचिन्हित length = simple.instructions_length ();
          जर (length)
          {
            length = simple.length (length);
            अक्षर *p = (अक्षर *) hb_malloc (length);
            जर (unlikely (!p)) परत असत्य;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        थांबा;
      }
    }

    जर (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      परत असत्य;
    }
    परत सत्य;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  ढाँचा <typename accelerator_t>
  तार्किक get_points (hb_font_t *font, स्थिरांक accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   अचिन्हित *composite_contours = nullptr, /* OUT */
		   तार्किक shift_points_hori = सत्य,
		   तार्किक use_my_metrics = सत्य,
		   तार्किक phantom_only = असत्य,
		   hb_array_t<स्थिरांक पूर्णांक> coords = hb_array_t<स्थिरांक पूर्णांक> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   अचिन्हित पूर्णांक depth = 0,
		   अचिन्हित *edge_मोजणी = nullptr,
		   int64_t *budget = nullptr) स्थिरांक
  {
    जर (unlikely (depth > HB_MAX_NESTING_LEVEL)) परत असत्य;
    अचिन्हित चळत_edge_मोजणी = 0;
    जर (!edge_मोजणी) edge_मोजणी = &चळत_edge_मोजणी;
    जर (unlikely (*edge_मोजणी > HB_MAX_GRAPH_EDGE_मोजणी)) परत असत्य;
    (*edge_मोजणी)++;

    int64_t चळत_budget = HB_BUDGET_GLYPH;
    जर (!budget) budget = &चळत_budget;
    जर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) परत असत्य;

    जर (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    जर (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->अंक_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    अचिन्हित old_length = points.length;

    निवड (type) {
    स्थिती SIMPLE:
      जर (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (अचिन्हित) header->संख्या_Of_Contours);
      जर (depth > 0 && composite_contours)
        *composite_contours += (अचिन्हित) header->संख्या_Of_Contours;
      जर (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	परत असत्य;
      थांबा;
    स्थिती COMPOSITE:
    {
      साठी (स्वतः &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        जर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          परत असत्य;
        }
      }
      थांबा;
    }
    स्थिती EMPTY:
      थांबा;
    }

    /* Init phantom points */
    जर (unlikely (!points.resize (points.length + PHANTOM_मोजणी))) परत असत्य;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_मोजणी, PHANTOM_मोजणी);
    {
      // Duplicated code.
      पूर्णांक lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      पूर्णांक h_delta = (पूर्णांक) header->xMin - lsb;
      HB_UNUSED पूर्णांक tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      पूर्णांक v_orig  = (पूर्णांक) header->yMax + tsb;
      अचिन्हित h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      अचिन्हित v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#अन्यथा
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_डावा].x = h_delta;
      phantoms[PHANTOM_उजवा].x = (पूर्णांक) h_adv + h_delta;
      phantoms[PHANTOM_शीर्ष].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (पूर्णांक) v_adv;
    }

#ifndef HB_NO_VAR
    जर (hb_any (coords))
    {
      जर (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        परत असत्य;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    जर (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      प्रतिज्ञा (old_length == 0);
      *points_with_deltas = points;
    }

    दशांश shift = 0;
    निवड (type) {
    स्थिती SIMPLE:
      जर (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_डावा].x;
      थांबा;
    स्थिती COMPOSITE:
    {
      hb_decycler_गाठ_t decycler_गाठ (scratch.decycler);

      अचिन्हित पूर्णांक comp_index = 0;
      साठी (स्वतः &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        जर (unlikely (!decycler_गाठ.visit (item_gid)))
	{
	  comp_index++;
	  सुरूठेवा;
	}

	अचिन्हित old_मोजणी = all_points.length;

	जर (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_मोजणी,
						    budget)))
	{
	  points.resize (old_length);
	  परत असत्य;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_मोजणी, PHANTOM_मोजणी);

	स्वतः comp_points = all_points.as_array ().sub_array (old_मोजणी);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	जर (use_my_metrics && item.is_use_my_metrics ())
	  साठी (अचिन्हित पूर्णांक i = 0; i < PHANTOM_मोजणी; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_मोजणी + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	जर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  परत असत्य;
	}

	जर (comp_points) // Empty in case of phantom_only
	{
	  दशांश matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	जर (item.is_anchored () && !phantom_only)
	{
	  अचिन्हित पूर्णांक p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  जर (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_मोजणी);

	जर (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  परत असत्य;
	}

	comp_index++;
      }

      जर (head_maxp_info && depth == 0)
      {
        जर (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_डावा].x;
      points.resize (old_length);
    } थांबा;
    स्थिती EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_डावा].x;
      points.resize (old_length);
      थांबा;
    }

    जर (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      जर (shift)
        साठी (स्वतः &point : all_points)
	  point.x -= shift;
    }

    परत !all_points.in_error ();
  }

  तार्किक get_extents_without_var_scaled (hb_font_t *font, स्थिरांक glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) स्थिरांक
  {
    जर (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      परत सत्य; /* Empty glyph; zero extents. */
    }
    परत header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () स्थिरांक { परत bytes; }
  glyph_type_t get_type () स्थिरांक { परत type; }
  स्थिरांक GlyphHeader *get_header () स्थिरांक { परत header; }
  तार्किक is_extended () स्थिरांक { परत extended; }

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
    पूर्णांक अंक_contours = header->संख्या_Of_Contours;
    जर (unlikely (अंक_contours == 0)) type = EMPTY;
    अन्यथा जर (अंक_contours > 0) type = SIMPLE;
    अन्यथा जर (अंक_contours <= -1) type = COMPOSITE;
    अन्यथा type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  संरक्षित:
  hb_bytes_t bytes;
  स्थिरांक GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
