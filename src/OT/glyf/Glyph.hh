#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#ضمن "../../hb-open-type.hh"

#ضمن "GlyphHeader.hh"
#ضمن "SimpleGlyph.hh"
#ضمن "CompositeGlyph.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


تعداد phantom_point_index_t
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
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  عام:
  composite_iter_t get_composite_iterator () ثابت
  {
    إذا (type != COMPOSITE) أرجع composite_iter_t ();
    أرجع CompositeGlyph (*header, bytes, extended).iter ();
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    بدل (type) {
    حالة COMPOSITE: أرجع CompositeGlyph (*header, bytes, extended).trim_padding ();
    حالة SIMPLE:    أرجع SimpleGlyph (*header, bytes).trim_padding ();
    حالة EMPTY:     أرجع bytes;
    افتراضي:        أرجع bytes;
    }
  }

  فراغ drop_hints ()
  {
    بدل (type) {
    حالة COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); أرجع;
    حالة EMPTY:
      إذا (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      أرجع;
    }
  }

  فراغ set_overlaps_flag ()
  {
    بدل (type) {
    حالة COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); أرجع;
    حالة SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); أرجع;
    حالة EMPTY:     أرجع;
    }
  }

  فراغ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ثابت
  {
    بدل (type) {
    حالة COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); أرجع;
    حالة SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); أرجع;
    حالة EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      إذا (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      أرجع;
    }
  }

  منطقي is_composite () ثابت
  { أرجع type == COMPOSITE; }

  منطقي get_all_points_without_var (ثابت hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ثابت
  {
    بدل (type) {
    حالة SIMPLE:
      إذا (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        أرجع خطأ;
      اكسر;
    حالة COMPOSITE:
    {
      لكل (auto &item : get_composite_iterator ())
        إذا (unlikely (!item.get_points (points, extended))) أرجع خطأ;
      اكسر;
    }
    حالة EMPTY:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.length + PHANTOM_گنتی))) أرجع خطأ;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحيح lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) header->xMin - lsb;
      HB_UNUSED صحيح tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      صحيح v_orig  = (صحيح) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }
    أرجع true;
  }

  فراغ update_mtx (ثابت hb_subset_plan_t *plan,
                   صحيح xMin, صحيح xMax,
                   صحيح yMin, صحيح yMax,
                   ثابت contour_point_vector_t &all_points) ثابت
  {
    hb_codepoint_t new_gid = 0;
    إذا (!plan->new_gid_for_old_gid (gid, &new_gid))
      أرجع;

    إذا (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    عائم بایاں_Side_X = all_points[len - 4].x;
    عائم دایاں_Side_X = all_points[len - 3].x;
    عائم چوٹی_Side_Y = all_points[len - 2].y;
    عائم bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (دایاں_Side_X - بایاں_Side_X);
    إذا (hori_aw < 0) hori_aw = 0;
    صحيح lsb = roundf (xMin - بایاں_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    إذا (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = خطأ;

    signed vert_aw = roundf (چوٹی_Side_Y - bottomSideY);
    إذا (vert_aw < 0) vert_aw = 0;
    صحيح tsb = roundf (چوٹی_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  منطقي compile_header_bytes (ثابت hb_subset_plan_t *plan,
                             ثابت contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ثابت
  {
    GlyphHeader *glyph_header = nullptr;
    إذا (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      إذا (unlikely (!glyph_header)) أرجع خطأ;
    }

    عائم xMin = 0, xMax = 0;
    عائم yMin = 0, yMax = 0;
    إذا (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned گنتی = all_points.length - 4;
      لكل (unsigned i = 1; i < گنتی; i++)
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

    إذا (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    إذا (!glyph_header)
      أرجع true;

    glyph_header->نمبر_Of_Contours = header->نمبر_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ثابت حرف *)glyph_header, GlyphHeader::static_size);
    أرجع true;
  }

  منطقي compile_bytes_with_deltas (ثابت hb_subset_plan_t *plan,
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
    إذا (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    إذا (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, خطأ, خطأ))
      أرجع خطأ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    إذا (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    إذا (!plan->pinned_at_default)
    {
      بدل (type)
      {
      حالة COMPOSITE:
        إذا (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          أرجع خطأ;
        اكسر;
      حالة SIMPLE:
        إذا (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          أرجع خطأ;
        اكسر;
      حالة EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        إذا (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->نمبر_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          إذا (length)
          {
            length = simple.length (length);
            حرف *p = (حرف *) hb_malloc (length);
            إذا (unlikely (!p)) أرجع خطأ;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        اكسر;
      }
    }

    إذا (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      أرجع خطأ;
    }
    أرجع true;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  منطقي get_points (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   منطقي shift_points_hori = true,
		   منطقي use_my_metrics = true,
		   منطقي phantom_only = خطأ,
		   hb_array_t<ثابت صحيح> coords = hb_array_t<ثابت صحيح> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned صحيح depth = 0,
		   unsigned *edge_گنتی = nullptr,
		   int64_t *budget = nullptr) ثابت
  {
    إذا (unlikely (depth > HB_MAX_NESTING_LEVEL)) أرجع خطأ;
    unsigned ڈھیر_edge_گنتی = 0;
    إذا (!edge_گنتی) edge_گنتی = &ڈھیر_edge_گنتی;
    إذا (unlikely (*edge_گنتی > HB_MAX_GRAPH_EDGE_گنتی)) أرجع خطأ;
    (*edge_گنتی)++;

    int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ڈھیر_budget;
    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) أرجع خطأ;

    إذا (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    إذا (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->عدد_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    بدل (type) {
    حالة SIMPLE:
      إذا (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->نمبر_Of_Contours);
      إذا (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->نمبر_Of_Contours;
      إذا (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	أرجع خطأ;
      اكسر;
    حالة COMPOSITE:
    {
      لكل (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          أرجع خطأ;
        }
      }
      اكسر;
    }
    حالة EMPTY:
      اكسر;
    }

    /* Init phantom points */
    إذا (unlikely (!points.resize (points.length + PHANTOM_گنتی))) أرجع خطأ;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_گنتی, PHANTOM_گنتی);
    {
      // Duplicated code.
      صحيح lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      صحيح h_delta = (صحيح) header->xMin - lsb;
      HB_UNUSED صحيح tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      صحيح v_orig  = (صحيح) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#وإلا
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_بایاں].x = h_delta;
      phantoms[PHANTOM_دایاں].x = (صحيح) h_adv + h_delta;
      phantoms[PHANTOM_چوٹی].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (صحيح) v_adv;
    }

#ifndef HB_NO_VAR
    إذا (hb_any (coords))
    {
      إذا (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        أرجع خطأ;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    إذا (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      أكد (old_length == 0);
      *points_with_deltas = points;
    }

    عائم shift = 0;
    بدل (type) {
    حالة SIMPLE:
      إذا (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_بایاں].x;
      اكسر;
    حالة COMPOSITE:
    {
      hb_decycler_گرہ_t decycler_گرہ (scratch.decycler);

      unsigned صحيح comp_index = 0;
      لكل (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        إذا (unlikely (!decycler_گرہ.visit (item_gid)))
	{
	  comp_index++;
	  استمر;
	}

	unsigned old_گنتی = all_points.length;

	إذا (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_گنتی,
						    budget)))
	{
	  points.resize (old_length);
	  أرجع خطأ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_گنتی, PHANTOM_گنتی);

	auto comp_points = all_points.as_array ().sub_array (old_گنتی);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	إذا (use_my_metrics && item.is_use_my_metrics ())
	  لكل (unsigned صحيح i = 0; i < PHANTOM_گنتی; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_گنتی + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  أرجع خطأ;
	}

	إذا (comp_points) // Empty in case of phantom_only
	{
	  عائم matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	إذا (item.is_anchored () && !phantom_only)
	{
	  unsigned صحيح p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  إذا (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_گنتی);

	إذا (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  أرجع خطأ;
	}

	comp_index++;
      }

      إذا (head_maxp_info && depth == 0)
      {
        إذا (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_بایاں].x;
      points.resize (old_length);
    } اكسر;
    حالة EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_بایاں].x;
      points.resize (old_length);
      اكسر;
    }

    إذا (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      إذا (shift)
        لكل (auto &point : all_points)
	  point.x -= shift;
    }

    أرجع !all_points.in_error ();
  }

  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ثابت
  {
    إذا (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      أرجع true; /* Empty glyph; zero extents. */
    }
    أرجع header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ثابت { أرجع bytes; }
  glyph_type_t get_type () ثابت { أرجع type; }
  ثابت GlyphHeader *get_header () ثابت { أرجع header; }
  منطقي is_extended () ثابت { أرجع extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (خطأ)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 منطقي extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    صحيح عدد_contours = header->نمبر_Of_Contours;
    إذا (unlikely (عدد_contours == 0)) type = EMPTY;
    وإلا إذا (عدد_contours > 0) type = SIMPLE;
    وإلا إذا (عدد_contours <= -1) type = COMPOSITE;
    وإلا type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  محمي:
  hb_bytes_t bytes;
  ثابت GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
