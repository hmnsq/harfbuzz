#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#شامل "../../hb-open-type.hh"

#شامل "GlyphHeader.hh"
#شامل "SimpleGlyph.hh"
#شامل "CompositeGlyph.hh"


فضاینام OT {

ساختار glyf_accelerator_t;

فضاینام glyf_impl {


شمارش phantom_point_index_t
{
  PHANTOM_چپ   = 0,
  PHANTOM_راست  = 1,
  PHANTOM_بالاترین    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_شمار  = 4
};

ساختار Glyph
{
  شمارش glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  عمومی:
  composite_iter_t get_composite_iterator () ثابت
  {
    اگر (type != COMPOSITE) بازگشت composite_iter_t ();
    بازگشت CompositeGlyph (*header, bytes, extended).iter ();
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    انتخاب (type) {
    حالت COMPOSITE: بازگشت CompositeGlyph (*header, bytes, extended).trim_padding ();
    حالت SIMPLE:    بازگشت SimpleGlyph (*header, bytes).trim_padding ();
    حالت EMPTY:     بازگشت bytes;
    پیشفرض:        بازگشت bytes;
    }
  }

  تهی drop_hints ()
  {
    انتخاب (type) {
    حالت COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); بازگشت;
    حالت SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); بازگشت;
    حالت EMPTY:
      اگر (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      بازگشت;
    }
  }

  تهی set_overlaps_flag ()
  {
    انتخاب (type) {
    حالت COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); بازگشت;
    حالت SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); بازگشت;
    حالت EMPTY:     بازگشت;
    }
  }

  تهی drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ثابت
  {
    انتخاب (type) {
    حالت COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); بازگشت;
    حالت SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); بازگشت;
    حالت EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      اگر (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      بازگشت;
    }
  }

  منطقی is_composite () ثابت
  { بازگشت type == COMPOSITE; }

  منطقی get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ثابت
  {
    انتخاب (type) {
    حالت SIMPLE:
      اگر (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        بازگشت نادرست;
      بشکن;
    حالت COMPOSITE:
    {
      برای (auto &item : get_composite_iterator ())
        اگر (unlikely (!item.get_points (points, extended))) بازگشت نادرست;
      بشکن;
    }
    حالت EMPTY:
      بشکن;
    }

    /* Init phantom points */
    اگر (unlikely (!points.resize (points.length + PHANTOM_شمار))) بازگشت نادرست;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_شمار, PHANTOM_شمار);
    {
      // Duplicated code.
      صحیح lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحیح h_delta = (صحیح) header->xMin - lsb;
      HB_UNUSED صحیح tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      صحیح v_orig  = (صحیح) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#وگرنه
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_چپ].x = h_delta;
      phantoms[PHANTOM_راست].x = (صحیح) h_adv + h_delta;
      phantoms[PHANTOM_بالاترین].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحیح) v_adv;
    }
    بازگشت درست;
  }

  تهی update_mtx (ثابت hb_subset_plan_t *plan,
                   صحیح xMin, صحیح xMax,
                   صحیح yMin, صحیح yMax,
                   ثابت contour_point_vector_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    اگر (!plan->new_gid_for_old_gid (gid, &new_gid))
      بازگشت;

    اگر (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    اعشاری چپ_Side_X = all_points[len - 4].x;
    اعشاری راست_Side_X = all_points[len - 3].x;
    اعشاری بالاترین_Side_Y = all_points[len - 2].y;
    اعشاری bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (راست_Side_X - چپ_Side_X);
    اگر (hori_aw < 0) hori_aw = 0;
    صحیح lsb = roundf (xMin - چپ_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    اگر (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = نادرست;

    signed vert_aw = roundf (بالاترین_Side_Y - bottomSideY);
    اگر (vert_aw < 0) vert_aw = 0;
    صحیح tsb = roundf (بالاترین_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  منطقی compile_header_bytes (ثابت hb_subset_plan_t *plan,
                             ثابت contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ثابت
  {
    GlyphHeader *glyph_header = nullptr;
    اگر (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      اگر (unlikely (!glyph_header)) بازگشت نادرست;
    }

    اعشاری xMin = 0, xMax = 0;
    اعشاری yMin = 0, yMax = 0;
    اگر (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned شمار = all_points.length - 4;
      برای (unsigned i = 1; i < شمار; i++)
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

    اگر (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    اگر (!glyph_header)
      بازگشت درست;

    glyph_header->عدد_Of_Contours = header->عدد_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ثابت نویسه *)glyph_header, GlyphHeader::static_size);
    بازگشت درست;
  }

  منطقی compile_bytes_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    اگر (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    اگر (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, نادرست, نادرست))
      بازگشت نادرست;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    اگر (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    اگر (!plan->pinned_at_default)
    {
      انتخاب (type)
      {
      حالت COMPOSITE:
        اگر (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          بازگشت نادرست;
        بشکن;
      حالت SIMPLE:
        اگر (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          بازگشت نادرست;
        بشکن;
      حالت EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        اگر (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->عدد_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          اگر (length)
          {
            length = simple.length (length);
            نویسه *p = (نویسه *) hb_malloc (length);
            اگر (unlikely (!p)) بازگشت نادرست;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        بشکن;
      }
    }

    اگر (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      بازگشت نادرست;
    }
    بازگشت درست;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  منطقی get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   منطقی shift_points_hori = درست,
		   منطقی use_my_metrics = درست,
		   منطقی phantom_only = نادرست,
		   hb_array_t<ثابت صحیح> coords = hb_array_t<ثابت صحیح> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned صحیح depth = 0,
		   unsigned *edge_شمار = nullptr,
		   int64_t *budget = nullptr) ثابت
  {
    اگر (unlikely (depth > HB_MAX_NESTING_LEVEL)) بازگشت نادرست;
    unsigned پشته_edge_شمار = 0;
    اگر (!edge_شمار) edge_شمار = &پشته_edge_شمار;
    اگر (unlikely (*edge_شمار > HB_MAX_GRAPH_EDGE_شمار)) بازگشت نادرست;
    (*edge_شمار)++;

    int64_t پشته_budget = HB_BUDGET_GLYPH;
    اگر (!budget) budget = &پشته_budget;
    اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) بازگشت نادرست;

    اگر (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    اگر (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->رقم_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    انتخاب (type) {
    حالت SIMPLE:
      اگر (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->عدد_Of_Contours);
      اگر (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->عدد_Of_Contours;
      اگر (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	بازگشت نادرست;
      بشکن;
    حالت COMPOSITE:
    {
      برای (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          بازگشت نادرست;
        }
      }
      بشکن;
    }
    حالت EMPTY:
      بشکن;
    }

    /* Init phantom points */
    اگر (unlikely (!points.resize (points.length + PHANTOM_شمار))) بازگشت نادرست;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_شمار, PHANTOM_شمار);
    {
      // Duplicated code.
      صحیح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحیح h_delta = (صحیح) header->xMin - lsb;
      HB_UNUSED صحیح tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      صحیح v_orig  = (صحیح) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وگرنه
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_چپ].x = h_delta;
      phantoms[PHANTOM_راست].x = (صحیح) h_adv + h_delta;
      phantoms[PHANTOM_بالاترین].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحیح) v_adv;
    }

#ifndef HB_NO_VAR
    اگر (hb_any (coords))
    {
      اگر (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        بازگشت نادرست;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    اگر (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      أكد (old_length == 0);
      *points_with_deltas = points;
    }

    اعشاری shift = 0;
    انتخاب (type) {
    حالت SIMPLE:
      اگر (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_چپ].x;
      بشکن;
    حالت COMPOSITE:
    {
      hb_decycler_گره_t decycler_گره (scratch.decycler);

      unsigned صحیح comp_index = 0;
      برای (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        اگر (unlikely (!decycler_گره.visit (item_gid)))
	{
	  comp_index++;
	  ادامه;
	}

	unsigned old_شمار = all_points.length;

	اگر (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_شمار,
						    budget)))
	{
	  points.resize (old_length);
	  بازگشت نادرست;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_شمار, PHANTOM_شمار);

	auto comp_points = all_points.as_array ().sub_array (old_شمار);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	اگر (use_my_metrics && item.is_use_my_metrics ())
	  برای (unsigned صحیح i = 0; i < PHANTOM_شمار; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_شمار + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  بازگشت نادرست;
	}

	اگر (comp_points) // Empty in case of phantom_only
	{
	  اعشاری matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	اگر (item.is_anchored () && !phantom_only)
	{
	  unsigned صحیح p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  اگر (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_شمار);

	اگر (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  بازگشت نادرست;
	}

	comp_index++;
      }

      اگر (head_maxp_info && depth == 0)
      {
        اگر (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_چپ].x;
      points.resize (old_length);
    } بشکن;
    حالت EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_چپ].x;
      points.resize (old_length);
      بشکن;
    }

    اگر (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      اگر (shift)
        برای (auto &point : all_points)
	  point.x -= shift;
    }

    بازگشت !all_points.in_error ();
  }

  منطقی get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    اگر (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      بازگشت درست; /* Empty glyph; zero extents. */
    }
    بازگشت header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ثابت { بازگشت bytes; }
  glyph_type_t get_type () ثابت { بازگشت type; }
  ثابت GlyphHeader *get_header () ثابت { بازگشت header; }
  منطقی is_extended () ثابت { بازگشت extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (نادرست)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 منطقی extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحیح رقم_contours = header->عدد_Of_Contours;
    اگر (unlikely (رقم_contours == 0)) type = EMPTY;
    وگرنه اگر (رقم_contours > 0) type = SIMPLE;
    وگرنه اگر (رقم_contours <= -1) type = COMPOSITE;
    وگرنه type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محافظتشده:
  hb_bytes_t bytes;
  ثابت GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقی extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
