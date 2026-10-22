#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"

#অন্তর্ভাব "GlyphHeader.hh"
#অন্তর্ভাব "SimpleGlyph.hh"
#অন্তর্ভাব "CompositeGlyph.hh"


নামাকাশ OT {

সংরচনা glyf_accelerator_t;

নামাকাশ glyf_impl {


গণনা phantom_point_index_t
{
  PHANTOM_বাম   = 0,
  PHANTOM_ডান  = 1,
  PHANTOM_শীর্ষ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_গুনতি  = 4
};

সংরচনা Glyph
{
  গণনা glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  সার্বজনিক:
  composite_iter_t get_composite_iterator () ধ্রুব
  {
    যদি (type != COMPOSITE) প্রত্যাগম composite_iter_t ();
    প্রত্যাগম CompositeGlyph (*header, bytes, extended).iter ();
  }

  ধ্রুব hb_bytes_t trim_padding () ধ্রুব
  {
    বিকল্প (type) {
    পক্ষ COMPOSITE: প্রত্যাগম CompositeGlyph (*header, bytes, extended).trim_padding ();
    পক্ষ SIMPLE:    প্রত্যাগম SimpleGlyph (*header, bytes).trim_padding ();
    পক্ষ EMPTY:     প্রত্যাগম bytes;
    স্বভাব:        প্রত্যাগম bytes;
    }
  }

  শূন্য drop_hints ()
  {
    বিকল্প (type) {
    পক্ষ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); প্রত্যাগম;
    পক্ষ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); প্রত্যাগম;
    পক্ষ EMPTY:
      যদি (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      প্রত্যাগম;
    }
  }

  শূন্য set_overlaps_flag ()
  {
    বিকল্প (type) {
    পক্ষ COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); প্রত্যাগম;
    পক্ষ SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); প্রত্যাগম;
    পক্ষ EMPTY:     প্রত্যাগম;
    }
  }

  শূন্য drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ধ্রুব
  {
    বিকল্প (type) {
    পক্ষ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); প্রত্যাগম;
    পক্ষ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); প্রত্যাগম;
    পক্ষ EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      যদি (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      প্রত্যাগম;
    }
  }

  তার্কিক is_composite () ধ্রুব
  { প্রত্যাগম type == COMPOSITE; }

  তার্কিক get_all_points_without_var (ধ্রুব hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ধ্রুব
  {
    বিকল্প (type) {
    পক্ষ SIMPLE:
      যদি (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        প্রত্যাগম অসত্য;
      ভঙ্গ;
    পক্ষ COMPOSITE:
    {
      প্রতি (auto &item : get_composite_iterator ())
        যদি (unlikely (!item.get_points (points, extended))) প্রত্যাগম অসত্য;
      ভঙ্গ;
    }
    পক্ষ EMPTY:
      ভঙ্গ;
    }

    /* Init phantom points */
    যদি (unlikely (!points.resize (points.length + PHANTOM_গুনতি))) প্রত্যাগম অসত্য;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_গুনতি, PHANTOM_গুনতি);
    {
      // Duplicated code.
      পূর্ণাঙ্ক lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      পূর্ণাঙ্ক h_delta = (পূর্ণাঙ্ক) header->xMin - lsb;
      HB_UNUSED পূর্ণাঙ্ক tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      পূর্ণাঙ্ক v_orig  = (পূর্ণাঙ্ক) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#অন্যথা
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_বাম].x = h_delta;
      phantoms[PHANTOM_ডান].x = (পূর্ণাঙ্ক) h_adv + h_delta;
      phantoms[PHANTOM_শীর্ষ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (পূর্ণাঙ্ক) v_adv;
    }
    প্রত্যাগম সত্য;
  }

  শূন্য update_mtx (ধ্রুব hb_subset_plan_t *plan,
                   পূর্ণাঙ্ক xMin, পূর্ণাঙ্ক xMax,
                   পূর্ণাঙ্ক yMin, পূর্ণাঙ্ক yMax,
                   ধ্রুব contour_point_vector_t &all_points) ধ্রুব
  {
    hb_codepoint_t new_gid = 0;
    যদি (!plan->new_gid_for_old_gid (gid, &new_gid))
      প্রত্যাগম;

    যদি (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    ভিন্ন বাম_Side_X = all_points[len - 4].x;
    ভিন্ন ডান_Side_X = all_points[len - 3].x;
    ভিন্ন শীর্ষ_Side_Y = all_points[len - 2].y;
    ভিন্ন bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (ডান_Side_X - বাম_Side_X);
    যদি (hori_aw < 0) hori_aw = 0;
    পূর্ণাঙ্ক lsb = roundf (xMin - বাম_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    যদি (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = অসত্য;

    signed vert_aw = roundf (শীর্ষ_Side_Y - bottomSideY);
    যদি (vert_aw < 0) vert_aw = 0;
    পূর্ণাঙ্ক tsb = roundf (শীর্ষ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  তার্কিক compile_header_bytes (ধ্রুব hb_subset_plan_t *plan,
                             ধ্রুব contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ধ্রুব
  {
    GlyphHeader *glyph_header = nullptr;
    যদি (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      যদি (unlikely (!glyph_header)) প্রত্যাগম অসত্য;
    }

    ভিন্ন xMin = 0, xMax = 0;
    ভিন্ন yMin = 0, yMax = 0;
    যদি (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned গুনতি = all_points.length - 4;
      প্রতি (unsigned i = 1; i < গুনতি; i++)
      {
	ভিন্ন x = all_points[i].x;
	ভিন্ন y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    পূর্ণাঙ্ক rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    পূর্ণাঙ্ক rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    পূর্ণাঙ্ক rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    পূর্ণাঙ্ক rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    যদি (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    যদি (!glyph_header)
      প্রত্যাগম সত্য;

    glyph_header->সংখ্যা_Of_Contours = header->সংখ্যা_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ধ্রুব বর্ণ *)glyph_header, GlyphHeader::static_size);
    প্রত্যাগম সত্য;
  }

  তার্কিক compile_bytes_with_deltas (ধ্রুব hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ধ্রুব glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    যদি (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    যদি (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, অসত্য, অসত্য))
      প্রত্যাগম অসত্য;

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
      বিকল্প (type)
      {
      পক্ষ COMPOSITE:
        যদি (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          প্রত্যাগম অসত্য;
        ভঙ্গ;
      পক্ষ SIMPLE:
        যদি (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          প্রত্যাগম অসত্য;
        ভঙ্গ;
      পক্ষ EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        যদি (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->সংখ্যা_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          যদি (length)
          {
            length = simple.length (length);
            বর্ণ *p = (বর্ণ *) hb_malloc (length);
            যদি (unlikely (!p)) প্রত্যাগম অসত্য;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        ভঙ্গ;
      }
    }

    যদি (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      প্রত্যাগম অসত্য;
    }
    প্রত্যাগম সত্য;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  তার্কিক get_points (hb_font_t *font, ধ্রুব accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   তার্কিক shift_points_hori = সত্য,
		   তার্কিক use_my_metrics = সত্য,
		   তার্কিক phantom_only = অসত্য,
		   hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords = hb_array_t<ধ্রুব পূর্ণাঙ্ক> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned পূর্ণাঙ্ক depth = 0,
		   unsigned *edge_গুনতি = nullptr,
		   int64_t *budget = nullptr) ধ্রুব
  {
    যদি (unlikely (depth > HB_MAX_NESTING_LEVEL)) প্রত্যাগম অসত্য;
    unsigned স্তূপ_edge_গুনতি = 0;
    যদি (!edge_গুনতি) edge_গুনতি = &স্তূপ_edge_গুনতি;
    যদি (unlikely (*edge_গুনতি > HB_MAX_GRAPH_EDGE_গুনতি)) প্রত্যাগম অসত্য;
    (*edge_গুনতি)++;

    int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
    যদি (!budget) budget = &স্তূপ_budget;
    যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) প্রত্যাগম অসত্য;

    যদি (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    যদি (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->অঙ্ক_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    বিকল্প (type) {
    পক্ষ SIMPLE:
      যদি (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->সংখ্যা_Of_Contours);
      যদি (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->সংখ্যা_Of_Contours;
      যদি (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	প্রত্যাগম অসত্য;
      ভঙ্গ;
    পক্ষ COMPOSITE:
    {
      প্রতি (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          প্রত্যাগম অসত্য;
        }
      }
      ভঙ্গ;
    }
    পক্ষ EMPTY:
      ভঙ্গ;
    }

    /* Init phantom points */
    যদি (unlikely (!points.resize (points.length + PHANTOM_গুনতি))) প্রত্যাগম অসত্য;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_গুনতি, PHANTOM_গুনতি);
    {
      // Duplicated code.
      পূর্ণাঙ্ক lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      পূর্ণাঙ্ক h_delta = (পূর্ণাঙ্ক) header->xMin - lsb;
      HB_UNUSED পূর্ণাঙ্ক tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      পূর্ণাঙ্ক v_orig  = (পূর্ণাঙ্ক) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#অন্যথা
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_বাম].x = h_delta;
      phantoms[PHANTOM_ডান].x = (পূর্ণাঙ্ক) h_adv + h_delta;
      phantoms[PHANTOM_শীর্ষ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (পূর্ণাঙ্ক) v_adv;
    }

#ifndef HB_NO_VAR
    যদি (hb_any (coords))
    {
      যদি (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        প্রত্যাগম অসত্য;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    যদি (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      প্রতিজ্ঞা (old_length == 0);
      *points_with_deltas = points;
    }

    ভিন্ন shift = 0;
    বিকল্প (type) {
    পক্ষ SIMPLE:
      যদি (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_বাম].x;
      ভঙ্গ;
    পক্ষ COMPOSITE:
    {
      hb_decycler_গ্রন্থি_t decycler_গ্রন্থি (scratch.decycler);

      unsigned পূর্ণাঙ্ক comp_index = 0;
      প্রতি (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        যদি (unlikely (!decycler_গ্রন্থি.visit (item_gid)))
	{
	  comp_index++;
	  অনুবর্ত;
	}

	unsigned old_গুনতি = all_points.length;

	যদি (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_গুনতি,
						    budget)))
	{
	  points.resize (old_length);
	  প্রত্যাগম অসত্য;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_গুনতি, PHANTOM_গুনতি);

	auto comp_points = all_points.as_array ().sub_array (old_গুনতি);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	যদি (use_my_metrics && item.is_use_my_metrics ())
	  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < PHANTOM_গুনতি; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_গুনতি + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  প্রত্যাগম অসত্য;
	}

	যদি (comp_points) // Empty in case of phantom_only
	{
	  ভিন্ন matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	যদি (item.is_anchored () && !phantom_only)
	{
	  unsigned পূর্ণাঙ্ক p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  যদি (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_গুনতি);

	যদি (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  প্রত্যাগম অসত্য;
	}

	comp_index++;
      }

      যদি (head_maxp_info && depth == 0)
      {
        যদি (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_বাম].x;
      points.resize (old_length);
    } ভঙ্গ;
    পক্ষ EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_বাম].x;
      points.resize (old_length);
      ভঙ্গ;
    }

    যদি (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      যদি (shift)
        প্রতি (auto &point : all_points)
	  point.x -= shift;
    }

    প্রত্যাগম !all_points.in_error ();
  }

  তার্কিক get_extents_without_var_scaled (hb_font_t *font, ধ্রুব glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ধ্রুব
  {
    যদি (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      প্রত্যাগম সত্য; /* Empty glyph; zero extents. */
    }
    প্রত্যাগম header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ধ্রুব { প্রত্যাগম bytes; }
  glyph_type_t get_type () ধ্রুব { প্রত্যাগম type; }
  ধ্রুব GlyphHeader *get_header () ধ্রুব { প্রত্যাগম header; }
  তার্কিক is_extended () ধ্রুব { প্রত্যাগম extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (অসত্য)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 তার্কিক extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    পূর্ণাঙ্ক অঙ্ক_contours = header->সংখ্যা_Of_Contours;
    যদি (unlikely (অঙ্ক_contours == 0)) type = EMPTY;
    অন্যথা যদি (অঙ্ক_contours > 0) type = SIMPLE;
    অন্যথা যদি (অঙ্ক_contours <= -1) type = COMPOSITE;
    অন্যথা type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  রক্ষিত:
  hb_bytes_t bytes;
  ধ্রুব GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  তার্কিক extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
