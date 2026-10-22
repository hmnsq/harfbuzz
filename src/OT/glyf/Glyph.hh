#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"

#അന്തര്ഭാവ "GlyphHeader.hh"
#അന്തര്ഭാവ "SimpleGlyph.hh"
#അന്തര്ഭാവ "CompositeGlyph.hh"


നാമാകാശ OT {

ഘടന glyf_accelerator_t;

നാമാകാശ glyf_impl {


എണ്ണൽ phantom_point_index_t
{
  PHANTOM_ഇടത്   = 0,
  PHANTOM_വലത്  = 1,
  PHANTOM_മുകൾ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_എണ്ണം  = 4
};

ഘടന Glyph
{
  എണ്ണൽ glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  പൊതു:
  composite_iter_t get_composite_iterator () സ്ഥിരം
  {
    എങ്കിൽ (type != COMPOSITE) തിരികെ composite_iter_t ();
    തിരികെ CompositeGlyph (*header, bytes, extended).iter ();
  }

  സ്ഥിരം hb_bytes_t trim_padding () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ COMPOSITE: തിരികെ CompositeGlyph (*header, bytes, extended).trim_padding ();
    അവസ്ഥ SIMPLE:    തിരികെ SimpleGlyph (*header, bytes).trim_padding ();
    അവസ്ഥ EMPTY:     തിരികെ bytes;
    സ്ഥിരസ്ഥിതി:        തിരികെ bytes;
    }
  }

  ശൂന്യം drop_hints ()
  {
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); തിരികെ;
    അവസ്ഥ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); തിരികെ;
    അവസ്ഥ EMPTY:
      എങ്കിൽ (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      തിരികെ;
    }
  }

  ശൂന്യം set_overlaps_flag ()
  {
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); തിരികെ;
    അവസ്ഥ SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); തിരികെ;
    അവസ്ഥ EMPTY:     തിരികെ;
    }
  }

  ശൂന്യം drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); തിരികെ;
    അവസ്ഥ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); തിരികെ;
    അവസ്ഥ EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      എങ്കിൽ (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      തിരികെ;
    }
  }

  തർക്കം is_composite () സ്ഥിരം
  { തിരികെ type == COMPOSITE; }

  തർക്കം get_all_points_without_var (സ്ഥിരം hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ SIMPLE:
      എങ്കിൽ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        തിരികെ അസത്യം;
      നിർത്തുക;
    അവസ്ഥ COMPOSITE:
    {
      വേണ്ടി (auto &item : get_composite_iterator ())
        എങ്കിൽ (unlikely (!item.get_points (points, extended))) തിരികെ അസത്യം;
      നിർത്തുക;
    }
    അവസ്ഥ EMPTY:
      നിർത്തുക;
    }

    /* Init phantom points */
    എങ്കിൽ (unlikely (!points.resize (points.length + PHANTOM_എണ്ണം))) തിരികെ അസത്യം;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_എണ്ണം, PHANTOM_എണ്ണം);
    {
      // Duplicated code.
      പൂർണ്ണസംഖ്യ lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      പൂർണ്ണസംഖ്യ h_delta = (പൂർണ്ണസംഖ്യ) header->xMin - lsb;
      HB_UNUSED പൂർണ്ണസംഖ്യ tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      പൂർണ്ണസംഖ്യ v_orig  = (പൂർണ്ണസംഖ്യ) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#അല്ലെങ്കിൽ
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ഇടത്].x = h_delta;
      phantoms[PHANTOM_വലത്].x = (പൂർണ്ണസംഖ്യ) h_adv + h_delta;
      phantoms[PHANTOM_മുകൾ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (പൂർണ്ണസംഖ്യ) v_adv;
    }
    തിരികെ സത്യം;
  }

  ശൂന്യം update_mtx (സ്ഥിരം hb_subset_plan_t *plan,
                   പൂർണ്ണസംഖ്യ xMin, പൂർണ്ണസംഖ്യ xMax,
                   പൂർണ്ണസംഖ്യ yMin, പൂർണ്ണസംഖ്യ yMax,
                   സ്ഥിരം contour_point_vector_t &all_points) സ്ഥിരം
  {
    hb_codepoint_t new_gid = 0;
    എങ്കിൽ (!plan->new_gid_for_old_gid (gid, &new_gid))
      തിരികെ;

    എങ്കിൽ (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    ദശാംശം ഇടത്_Side_X = all_points[len - 4].x;
    ദശാംശം വലത്_Side_X = all_points[len - 3].x;
    ദശാംശം മുകൾ_Side_Y = all_points[len - 2].y;
    ദശാംശം bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (വലത്_Side_X - ഇടത്_Side_X);
    എങ്കിൽ (hori_aw < 0) hori_aw = 0;
    പൂർണ്ണസംഖ്യ lsb = roundf (xMin - ഇടത്_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    എങ്കിൽ (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = അസത്യം;

    signed vert_aw = roundf (മുകൾ_Side_Y - bottomSideY);
    എങ്കിൽ (vert_aw < 0) vert_aw = 0;
    പൂർണ്ണസംഖ്യ tsb = roundf (മുകൾ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  തർക്കം compile_header_bytes (സ്ഥിരം hb_subset_plan_t *plan,
                             സ്ഥിരം contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) സ്ഥിരം
  {
    GlyphHeader *glyph_header = nullptr;
    എങ്കിൽ (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      എങ്കിൽ (unlikely (!glyph_header)) തിരികെ അസത്യം;
    }

    ദശാംശം xMin = 0, xMax = 0;
    ദശാംശം yMin = 0, yMax = 0;
    എങ്കിൽ (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned എണ്ണം = all_points.length - 4;
      വേണ്ടി (unsigned i = 1; i < എണ്ണം; i++)
      {
	ദശാംശം x = all_points[i].x;
	ദശാംശം y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    പൂർണ്ണസംഖ്യ rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    പൂർണ്ണസംഖ്യ rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    പൂർണ്ണസംഖ്യ rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    പൂർണ്ണസംഖ്യ rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    എങ്കിൽ (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    എങ്കിൽ (!glyph_header)
      തിരികെ സത്യം;

    glyph_header->സംഖ്യ_Of_Contours = header->സംഖ്യ_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((സ്ഥിരം അക്ഷരം *)glyph_header, GlyphHeader::static_size);
    തിരികെ സത്യം;
  }

  തർക്കം compile_bytes_with_deltas (സ്ഥിരം hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  സ്ഥിരം glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    എങ്കിൽ (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    എങ്കിൽ (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, അസത്യം, അസത്യം))
      തിരികെ അസത്യം;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    എങ്കിൽ (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    എങ്കിൽ (!plan->pinned_at_default)
    {
      തിരഞ്ഞെടുപ്പ് (type)
      {
      അവസ്ഥ COMPOSITE:
        എങ്കിൽ (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          തിരികെ അസത്യം;
        നിർത്തുക;
      അവസ്ഥ SIMPLE:
        എങ്കിൽ (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          തിരികെ അസത്യം;
        നിർത്തുക;
      അവസ്ഥ EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        എങ്കിൽ (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->സംഖ്യ_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          എങ്കിൽ (length)
          {
            length = simple.length (length);
            അക്ഷരം *p = (അക്ഷരം *) hb_malloc (length);
            എങ്കിൽ (unlikely (!p)) തിരികെ അസത്യം;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        നിർത്തുക;
      }
    }

    എങ്കിൽ (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  തർക്കം get_points (hb_font_t *font, സ്ഥിരം accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   തർക്കം shift_points_hori = സത്യം,
		   തർക്കം use_my_metrics = സത്യം,
		   തർക്കം phantom_only = അസത്യം,
		   hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords = hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned പൂർണ്ണസംഖ്യ depth = 0,
		   unsigned *edge_എണ്ണം = nullptr,
		   int64_t *budget = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (depth > HB_MAX_NESTING_LEVEL)) തിരികെ അസത്യം;
    unsigned അട്ടി_edge_എണ്ണം = 0;
    എങ്കിൽ (!edge_എണ്ണം) edge_എണ്ണം = &അട്ടി_edge_എണ്ണം;
    എങ്കിൽ (unlikely (*edge_എണ്ണം > HB_MAX_GRAPH_EDGE_എണ്ണം)) തിരികെ അസത്യം;
    (*edge_എണ്ണം)++;

    int64_t അട്ടി_budget = HB_BUDGET_GLYPH;
    എങ്കിൽ (!budget) budget = &അട്ടി_budget;
    എങ്കിൽ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) തിരികെ അസത്യം;

    എങ്കിൽ (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    എങ്കിൽ (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->അക്കം_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ SIMPLE:
      എങ്കിൽ (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->സംഖ്യ_Of_Contours);
      എങ്കിൽ (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->സംഖ്യ_Of_Contours;
      എങ്കിൽ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	തിരികെ അസത്യം;
      നിർത്തുക;
    അവസ്ഥ COMPOSITE:
    {
      വേണ്ടി (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        എങ്കിൽ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          തിരികെ അസത്യം;
        }
      }
      നിർത്തുക;
    }
    അവസ്ഥ EMPTY:
      നിർത്തുക;
    }

    /* Init phantom points */
    എങ്കിൽ (unlikely (!points.resize (points.length + PHANTOM_എണ്ണം))) തിരികെ അസത്യം;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_എണ്ണം, PHANTOM_എണ്ണം);
    {
      // Duplicated code.
      പൂർണ്ണസംഖ്യ lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      പൂർണ്ണസംഖ്യ h_delta = (പൂർണ്ണസംഖ്യ) header->xMin - lsb;
      HB_UNUSED പൂർണ്ണസംഖ്യ tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      പൂർണ്ണസംഖ്യ v_orig  = (പൂർണ്ണസംഖ്യ) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#അല്ലെങ്കിൽ
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ഇടത്].x = h_delta;
      phantoms[PHANTOM_വലത്].x = (പൂർണ്ണസംഖ്യ) h_adv + h_delta;
      phantoms[PHANTOM_മുകൾ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (പൂർണ്ണസംഖ്യ) v_adv;
    }

#ifndef HB_NO_VAR
    എങ്കിൽ (hb_any (coords))
    {
      എങ്കിൽ (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        തിരികെ അസത്യം;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    എങ്കിൽ (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      പ്രതിജ്ഞാ (old_length == 0);
      *points_with_deltas = points;
    }

    ദശാംശം shift = 0;
    തിരഞ്ഞെടുപ്പ് (type) {
    അവസ്ഥ SIMPLE:
      എങ്കിൽ (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_ഇടത്].x;
      നിർത്തുക;
    അവസ്ഥ COMPOSITE:
    {
      hb_decycler_കണ്ണി_t decycler_കണ്ണി (scratch.decycler);

      unsigned പൂർണ്ണസംഖ്യ comp_index = 0;
      വേണ്ടി (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        എങ്കിൽ (unlikely (!decycler_കണ്ണി.visit (item_gid)))
	{
	  comp_index++;
	  തുടരുക;
	}

	unsigned old_എണ്ണം = all_points.length;

	എങ്കിൽ (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_എണ്ണം,
						    budget)))
	{
	  points.resize (old_length);
	  തിരികെ അസത്യം;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_എണ്ണം, PHANTOM_എണ്ണം);

	auto comp_points = all_points.as_array ().sub_array (old_എണ്ണം);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	എങ്കിൽ (use_my_metrics && item.is_use_my_metrics ())
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < PHANTOM_എണ്ണം; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_എണ്ണം + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	എങ്കിൽ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  തിരികെ അസത്യം;
	}

	എങ്കിൽ (comp_points) // Empty in case of phantom_only
	{
	  ദശാംശം matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	എങ്കിൽ (item.is_anchored () && !phantom_only)
	{
	  unsigned പൂർണ്ണസംഖ്യ p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  എങ്കിൽ (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_എണ്ണം);

	എങ്കിൽ (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  തിരികെ അസത്യം;
	}

	comp_index++;
      }

      എങ്കിൽ (head_maxp_info && depth == 0)
      {
        എങ്കിൽ (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ഇടത്].x;
      points.resize (old_length);
    } നിർത്തുക;
    അവസ്ഥ EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ഇടത്].x;
      points.resize (old_length);
      നിർത്തുക;
    }

    എങ്കിൽ (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      എങ്കിൽ (shift)
        വേണ്ടി (auto &point : all_points)
	  point.x -= shift;
    }

    തിരികെ !all_points.in_error ();
  }

  തർക്കം get_extents_without_var_scaled (hb_font_t *font, സ്ഥിരം glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) സ്ഥിരം
  {
    എങ്കിൽ (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      തിരികെ സത്യം; /* Empty glyph; zero extents. */
    }
    തിരികെ header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () സ്ഥിരം { തിരികെ bytes; }
  glyph_type_t get_type () സ്ഥിരം { തിരികെ type; }
  സ്ഥിരം GlyphHeader *get_header () സ്ഥിരം { തിരികെ header; }
  തർക്കം is_extended () സ്ഥിരം { തിരികെ extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (അസത്യം)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 തർക്കം extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    പൂർണ്ണസംഖ്യ അക്കം_contours = header->സംഖ്യ_Of_Contours;
    എങ്കിൽ (unlikely (അക്കം_contours == 0)) type = EMPTY;
    അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_contours > 0) type = SIMPLE;
    അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_contours <= -1) type = COMPOSITE;
    അല്ലെങ്കിൽ type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  സംരക്ഷിത:
  hb_bytes_t bytes;
  സ്ഥിരം GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  തർക്കം extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
