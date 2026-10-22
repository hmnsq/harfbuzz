#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#அந்தர்பாவ "../../hb-open-type.hh"

#அந்தர்பாவ "GlyphHeader.hh"
#அந்தர்பாவ "SimpleGlyph.hh"
#அந்தர்பாவ "CompositeGlyph.hh"


நாமாகாஶ OT {

கட்டமைப்பு glyf_accelerator_t;

நாமாகாஶ glyf_impl {


எண்ணிக்கை phantom_point_index_t
{
  PHANTOM_இடது   = 0,
  PHANTOM_வலது  = 1,
  PHANTOM_உச்சி    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_மொத்தம்  = 4
};

கட்டமைப்பு Glyph
{
  எண்ணிக்கை glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  பொது:
  composite_iter_t get_composite_iterator () மாறிலி
  {
    எனில் (type != COMPOSITE) திரும்பு composite_iter_t ();
    திரும்பு CompositeGlyph (*header, bytes, extended).iter ();
  }

  மாறிலி hb_bytes_t trim_padding () மாறிலி
  {
    தேர்வு (type) {
    நிலை COMPOSITE: திரும்பு CompositeGlyph (*header, bytes, extended).trim_padding ();
    நிலை SIMPLE:    திரும்பு SimpleGlyph (*header, bytes).trim_padding ();
    நிலை EMPTY:     திரும்பு bytes;
    இயல்பு:        திரும்பு bytes;
    }
  }

  வெறுமை drop_hints ()
  {
    தேர்வு (type) {
    நிலை COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); திரும்பு;
    நிலை SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); திரும்பு;
    நிலை EMPTY:
      எனில் (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      திரும்பு;
    }
  }

  வெறுமை set_overlaps_flag ()
  {
    தேர்வு (type) {
    நிலை COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); திரும்பு;
    நிலை SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); திரும்பு;
    நிலை EMPTY:     திரும்பு;
    }
  }

  வெறுமை drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) மாறிலி
  {
    தேர்வு (type) {
    நிலை COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); திரும்பு;
    நிலை SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); திரும்பு;
    நிலை EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      எனில் (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      திரும்பு;
    }
  }

  தருக்க is_composite () மாறிலி
  { திரும்பு type == COMPOSITE; }

  தருக்க get_all_points_without_var (மாறிலி hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) மாறிலி
  {
    தேர்வு (type) {
    நிலை SIMPLE:
      எனில் (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        திரும்பு பொய்;
      நிறுத்து;
    நிலை COMPOSITE:
    {
      க்கு (auto &item : get_composite_iterator ())
        எனில் (unlikely (!item.get_points (points, extended))) திரும்பு பொய்;
      நிறுத்து;
    }
    நிலை EMPTY:
      நிறுத்து;
    }

    /* Init phantom points */
    எனில் (unlikely (!points.resize (points.length + PHANTOM_மொத்தம்))) திரும்பு பொய்;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_மொத்தம், PHANTOM_மொத்தம்);
    {
      // Duplicated code.
      முழு lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      முழு h_delta = (முழு) header->xMin - lsb;
      HB_UNUSED முழு tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      முழு v_orig  = (முழு) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#இல்லையெனில்
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_இடது].x = h_delta;
      phantoms[PHANTOM_வலது].x = (முழு) h_adv + h_delta;
      phantoms[PHANTOM_உச்சி].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (முழு) v_adv;
    }
    திரும்பு உண்மை;
  }

  வெறுமை update_mtx (மாறிலி hb_subset_plan_t *plan,
                   முழு xMin, முழு xMax,
                   முழு yMin, முழு yMax,
                   மாறிலி contour_point_vector_t &all_points) மாறிலி
  {
    hb_codepoint_t new_gid = 0;
    எனில் (!plan->new_gid_for_old_gid (gid, &new_gid))
      திரும்பு;

    எனில் (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    தசம இடது_Side_X = all_points[len - 4].x;
    தசம வலது_Side_X = all_points[len - 3].x;
    தசம உச்சி_Side_Y = all_points[len - 2].y;
    தசம bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (வலது_Side_X - இடது_Side_X);
    எனில் (hori_aw < 0) hori_aw = 0;
    முழு lsb = roundf (xMin - இடது_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    எனில் (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = பொய்;

    signed vert_aw = roundf (உச்சி_Side_Y - bottomSideY);
    எனில் (vert_aw < 0) vert_aw = 0;
    முழு tsb = roundf (உச்சி_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  தருக்க compile_header_bytes (மாறிலி hb_subset_plan_t *plan,
                             மாறிலி contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) மாறிலி
  {
    GlyphHeader *glyph_header = nullptr;
    எனில் (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      எனில் (unlikely (!glyph_header)) திரும்பு பொய்;
    }

    தசம xMin = 0, xMax = 0;
    தசம yMin = 0, yMax = 0;
    எனில் (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned மொத்தம் = all_points.length - 4;
      க்கு (unsigned i = 1; i < மொத்தம்; i++)
      {
	தசம x = all_points[i].x;
	தசம y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    முழு rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    முழு rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    முழு rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    முழு rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    எனில் (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    எனில் (!glyph_header)
      திரும்பு உண்மை;

    glyph_header->எண்_Of_Contours = header->எண்_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((மாறிலி எழுத்து *)glyph_header, GlyphHeader::static_size);
    திரும்பு உண்மை;
  }

  தருக்க compile_bytes_with_deltas (மாறிலி hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  மாறிலி glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    எனில் (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    எனில் (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, பொய், பொய்))
      திரும்பு பொய்;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    எனில் (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    எனில் (!plan->pinned_at_default)
    {
      தேர்வு (type)
      {
      நிலை COMPOSITE:
        எனில் (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          திரும்பு பொய்;
        நிறுத்து;
      நிலை SIMPLE:
        எனில் (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          திரும்பு பொய்;
        நிறுத்து;
      நிலை EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        எனில் (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->எண்_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          எனில் (length)
          {
            length = simple.length (length);
            எழுத்து *p = (எழுத்து *) hb_malloc (length);
            எனில் (unlikely (!p)) திரும்பு பொய்;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        நிறுத்து;
      }
    }

    எனில் (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  தருக்க get_points (hb_font_t *font, மாறிலி accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   தருக்க shift_points_hori = உண்மை,
		   தருக்க use_my_metrics = உண்மை,
		   தருக்க phantom_only = பொய்,
		   hb_array_t<மாறிலி முழு> coords = hb_array_t<மாறிலி முழு> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned முழு depth = 0,
		   unsigned *edge_மொத்தம் = nullptr,
		   int64_t *budget = nullptr) மாறிலி
  {
    எனில் (unlikely (depth > HB_MAX_NESTING_LEVEL)) திரும்பு பொய்;
    unsigned அடுக்கு_edge_மொத்தம் = 0;
    எனில் (!edge_மொத்தம்) edge_மொத்தம் = &அடுக்கு_edge_மொத்தம்;
    எனில் (unlikely (*edge_மொத்தம் > HB_MAX_GRAPH_EDGE_மொத்தம்)) திரும்பு பொய்;
    (*edge_மொத்தம்)++;

    int64_t அடுக்கு_budget = HB_BUDGET_GLYPH;
    எனில் (!budget) budget = &அடுக்கு_budget;
    எனில் (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) திரும்பு பொய்;

    எனில் (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    எனில் (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->இலக்கம்_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    தேர்வு (type) {
    நிலை SIMPLE:
      எனில் (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->எண்_Of_Contours);
      எனில் (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->எண்_Of_Contours;
      எனில் (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	திரும்பு பொய்;
      நிறுத்து;
    நிலை COMPOSITE:
    {
      க்கு (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        எனில் (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          திரும்பு பொய்;
        }
      }
      நிறுத்து;
    }
    நிலை EMPTY:
      நிறுத்து;
    }

    /* Init phantom points */
    எனில் (unlikely (!points.resize (points.length + PHANTOM_மொத்தம்))) திரும்பு பொய்;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_மொத்தம், PHANTOM_மொத்தம்);
    {
      // Duplicated code.
      முழு lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      முழு h_delta = (முழு) header->xMin - lsb;
      HB_UNUSED முழு tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      முழு v_orig  = (முழு) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#இல்லையெனில்
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_இடது].x = h_delta;
      phantoms[PHANTOM_வலது].x = (முழு) h_adv + h_delta;
      phantoms[PHANTOM_உச்சி].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (முழு) v_adv;
    }

#ifndef HB_NO_VAR
    எனில் (hb_any (coords))
    {
      எனில் (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        திரும்பு பொய்;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    எனில் (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      ப்ரதிஜ்ஞா (old_length == 0);
      *points_with_deltas = points;
    }

    தசம shift = 0;
    தேர்வு (type) {
    நிலை SIMPLE:
      எனில் (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_இடது].x;
      நிறுத்து;
    நிலை COMPOSITE:
    {
      hb_decycler_முனை_t decycler_முனை (scratch.decycler);

      unsigned முழு comp_index = 0;
      க்கு (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        எனில் (unlikely (!decycler_முனை.visit (item_gid)))
	{
	  comp_index++;
	  தொடர்;
	}

	unsigned old_மொத்தம் = all_points.length;

	எனில் (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_மொத்தம்,
						    budget)))
	{
	  points.resize (old_length);
	  திரும்பு பொய்;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_மொத்தம், PHANTOM_மொத்தம்);

	auto comp_points = all_points.as_array ().sub_array (old_மொத்தம்);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	எனில் (use_my_metrics && item.is_use_my_metrics ())
	  க்கு (unsigned முழு i = 0; i < PHANTOM_மொத்தம்; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_மொத்தம் + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	எனில் (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  திரும்பு பொய்;
	}

	எனில் (comp_points) // Empty in case of phantom_only
	{
	  தசம matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	எனில் (item.is_anchored () && !phantom_only)
	{
	  unsigned முழு p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  எனில் (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_மொத்தம்);

	எனில் (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  திரும்பு பொய்;
	}

	comp_index++;
      }

      எனில் (head_maxp_info && depth == 0)
      {
        எனில் (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_இடது].x;
      points.resize (old_length);
    } நிறுத்து;
    நிலை EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_இடது].x;
      points.resize (old_length);
      நிறுத்து;
    }

    எனில் (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      எனில் (shift)
        க்கு (auto &point : all_points)
	  point.x -= shift;
    }

    திரும்பு !all_points.in_error ();
  }

  தருக்க get_extents_without_var_scaled (hb_font_t *font, மாறிலி glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) மாறிலி
  {
    எனில் (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      திரும்பு உண்மை; /* Empty glyph; zero extents. */
    }
    திரும்பு header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () மாறிலி { திரும்பு bytes; }
  glyph_type_t get_type () மாறிலி { திரும்பு type; }
  மாறிலி GlyphHeader *get_header () மாறிலி { திரும்பு header; }
  தருக்க is_extended () மாறிலி { திரும்பு extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (பொய்)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 தருக்க extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    முழு இலக்கம்_contours = header->எண்_Of_Contours;
    எனில் (unlikely (இலக்கம்_contours == 0)) type = EMPTY;
    இல்லையெனில் எனில் (இலக்கம்_contours > 0) type = SIMPLE;
    இல்லையெனில் எனில் (இலக்கம்_contours <= -1) type = COMPOSITE;
    இல்லையெனில் type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  பாதுகாக்கப்பட்ட:
  hb_bytes_t bytes;
  மாறிலி GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  தருக்க extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
