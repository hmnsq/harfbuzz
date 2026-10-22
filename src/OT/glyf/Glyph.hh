#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"

#ଅନ୍ତର୍ଭାଵ "GlyphHeader.hh"
#ଅନ୍ତର୍ଭାଵ "SimpleGlyph.hh"
#ଅନ୍ତର୍ଭାଵ "CompositeGlyph.hh"


ନାମାକାଶ OT {

ସଂରଚନା glyf_accelerator_t;

ନାମାକାଶ glyf_impl {


ଗଣନା phantom_point_index_t
{
  PHANTOM_ବାମ   = 0,
  PHANTOM_ଡାହାଣ  = 1,
  PHANTOM_ଶୀର୍ଷ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_ଗଣନା  = 4
};

ସଂରଚନା Glyph
{
  ଗଣନା glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  ସର୍ବସାଧାରଣ:
  composite_iter_t get_composite_iterator () ଅଚଳ
  {
    ଯଦି (type != COMPOSITE) ଫେରନ୍ତୁ composite_iter_t ();
    ଫେରନ୍ତୁ CompositeGlyph (*header, bytes, extended).iter ();
  }

  ଅଚଳ hb_bytes_t trim_padding () ଅଚଳ
  {
    ବାଛ (type) {
    ସ୍ଥିତି COMPOSITE: ଫେରନ୍ତୁ CompositeGlyph (*header, bytes, extended).trim_padding ();
    ସ୍ଥିତି SIMPLE:    ଫେରନ୍ତୁ SimpleGlyph (*header, bytes).trim_padding ();
    ସ୍ଥିତି EMPTY:     ଫେରନ୍ତୁ bytes;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:        ଫେରନ୍ତୁ bytes;
    }
  }

  ଶୂନ୍ୟ drop_hints ()
  {
    ବାଛ (type) {
    ସ୍ଥିତି COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); ଫେରନ୍ତୁ;
    ସ୍ଥିତି SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); ଫେରନ୍ତୁ;
    ସ୍ଥିତି EMPTY:
      ଯଦି (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ set_overlaps_flag ()
  {
    ବାଛ (type) {
    ସ୍ଥିତି COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); ଫେରନ୍ତୁ;
    ସ୍ଥିତି SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); ଫେରନ୍ତୁ;
    ସ୍ଥିତି EMPTY:     ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ଅଚଳ
  {
    ବାଛ (type) {
    ସ୍ଥିତି COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); ଫେରନ୍ତୁ;
    ସ୍ଥିତି SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); ଫେରନ୍ତୁ;
    ସ୍ଥିତି EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      ଯଦି (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ is_composite () ଅଚଳ
  { ଫେରନ୍ତୁ type == COMPOSITE; }

  ତାର୍କିକ get_all_points_without_var (ଅଚଳ hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ଅଚଳ
  {
    ବାଛ (type) {
    ସ୍ଥିତି SIMPLE:
      ଯଦି (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି COMPOSITE:
    {
      ପାଇଁ (auto &item : get_composite_iterator ())
        ଯଦି (unlikely (!item.get_points (points, extended))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    }
    ସ୍ଥିତି EMPTY:
      ଭାଙ୍ଗ;
    }

    /* Init phantom points */
    ଯଦି (unlikely (!points.resize (points.length + PHANTOM_ଗଣନା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ଗଣନା, PHANTOM_ଗଣନା);
    {
      // Duplicated code.
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) header->xMin - lsb;
      HB_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v_orig  = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#ଅନ୍ୟଥା
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ବାମ].x = h_delta;
      phantoms[PHANTOM_ଡାହାଣ].x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) h_adv + h_delta;
      phantoms[PHANTOM_ଶୀର୍ଷ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) v_adv;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ update_mtx (ଅଚଳ hb_subset_plan_t *plan,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା xMin, ପୂର୍ଣ୍ଣସଂଖ୍ୟା xMax,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା yMin, ପୂର୍ଣ୍ଣସଂଖ୍ୟା yMax,
                   ଅଚଳ contour_point_vector_t &all_points) ଅଚଳ
  {
    hb_codepoint_t new_gid = 0;
    ଯଦି (!plan->new_gid_for_old_gid (gid, &new_gid))
      ଫେରନ୍ତୁ;

    ଯଦି (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    ଦଶମିକ ବାମ_Side_X = all_points[len - 4].x;
    ଦଶମିକ ଡାହାଣ_Side_X = all_points[len - 3].x;
    ଦଶମିକ ଶୀର୍ଷ_Side_Y = all_points[len - 2].y;
    ଦଶମିକ bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (ଡାହାଣ_Side_X - ବାମ_Side_X);
    ଯଦି (hori_aw < 0) hori_aw = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = roundf (xMin - ବାମ_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    ଯଦି (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = ମିଥ୍ୟା;

    signed vert_aw = roundf (ଶୀର୍ଷ_Side_Y - bottomSideY);
    ଯଦି (vert_aw < 0) vert_aw = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tsb = roundf (ଶୀର୍ଷ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  ତାର୍କିକ compile_header_bytes (ଅଚଳ hb_subset_plan_t *plan,
                             ଅଚଳ contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ଅଚଳ
  {
    GlyphHeader *glyph_header = nullptr;
    ଯଦି (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      ଯଦି (unlikely (!glyph_header)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଦଶମିକ xMin = 0, xMax = 0;
    ଦଶମିକ yMin = 0, yMax = 0;
    ଯଦି (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned ଗଣନା_2 = all_points.length - 4;
      ପାଇଁ (unsigned i = 1; i < ଗଣନା_2; i++)
      {
	ଦଶମିକ x = all_points[i].x;
	ଦଶମିକ y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    ଯଦି (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    ଯଦି (!glyph_header)
      ଫେରନ୍ତୁ ସତ୍ୟ;

    glyph_header->ସଂଖ୍ୟା_Of_Contours = header->ସଂଖ୍ୟା_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *)glyph_header, GlyphHeader::static_size);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ compile_bytes_with_deltas (ଅଚଳ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ଅଚଳ glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    ଯଦି (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    ଯଦି (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, ମିଥ୍ୟା, ମିଥ୍ୟା))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    ଯଦି (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    ଯଦି (!plan->pinned_at_default)
    {
      ବାଛ (type)
      {
      ସ୍ଥିତି COMPOSITE:
        ଯଦି (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି SIMPLE:
        ଯଦି (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଭାଙ୍ଗ;
      ସ୍ଥିତି EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        ଯଦି (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->ସଂଖ୍ୟା_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          ଯଦି (length)
          {
            length = simple.length (length);
            ଅକ୍ଷର *p = (ଅକ୍ଷର *) hb_malloc (length);
            ଯଦି (unlikely (!p)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        ଭାଙ୍ଗ;
      }
    }

    ଯଦି (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  ତାର୍କିକ get_points (hb_font_t *font, ଅଚଳ accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   ତାର୍କିକ shift_points_hori = ସତ୍ୟ,
		   ତାର୍କିକ use_my_metrics = ସତ୍ୟ,
		   ତାର୍କିକ phantom_only = ମିଥ୍ୟା,
		   hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords = hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0,
		   unsigned *edge_ଗଣନା = nullptr,
		   int64_t *budget = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (depth > HB_MAX_NESTING_LEVEL)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned ଗଦା_edge_ଗଣନା = 0;
    ଯଦି (!edge_ଗଣନା) edge_ଗଣନା = &ଗଦା_edge_ଗଣନା;
    ଯଦି (unlikely (*edge_ଗଣନା > HB_MAX_GRAPH_EDGE_ଗଣନା)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    (*edge_ଗଣନା)++;

    int64_t ଗଦା_budget = HB_BUDGET_GLYPH;
    ଯଦି (!budget) budget = &ଗଦା_budget;
    ଯଦି (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    ଯଦି (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->ଅଙ୍କ_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    ବାଛ (type) {
    ସ୍ଥିତି SIMPLE:
      ଯଦି (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->ସଂଖ୍ୟା_Of_Contours);
      ଯଦି (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->ସଂଖ୍ୟା_Of_Contours;
      ଯଦି (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି COMPOSITE:
    {
      ପାଇଁ (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        ଯଦି (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
      }
      ଭାଙ୍ଗ;
    }
    ସ୍ଥିତି EMPTY:
      ଭାଙ୍ଗ;
    }

    /* Init phantom points */
    ଯଦି (unlikely (!points.resize (points.length + PHANTOM_ଗଣନା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ଗଣନା, PHANTOM_ଗଣନା);
    {
      // Duplicated code.
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) header->xMin - lsb;
      HB_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v_orig  = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#ଅନ୍ୟଥା
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ବାମ].x = h_delta;
      phantoms[PHANTOM_ଡାହାଣ].x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) h_adv + h_delta;
      phantoms[PHANTOM_ଶୀର୍ଷ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) v_adv;
    }

#ifndef HB_NO_VAR
    ଯଦି (hb_any (coords))
    {
      ଯଦି (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    ଯଦି (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      ପ୍ରତିଜ୍ଞା (old_length == 0);
      *points_with_deltas = points;
    }

    ଦଶମିକ shift = 0;
    ବାଛ (type) {
    ସ୍ଥିତି SIMPLE:
      ଯଦି (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_ବାମ].x;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି COMPOSITE:
    {
      hb_decycler_ଗଣ୍ଠି_t decycler_ଗଣ୍ଠି (scratch.decycler);

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp_index = 0;
      ପାଇଁ (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        ଯଦି (unlikely (!decycler_ଗଣ୍ଠି.visit (item_gid)))
	{
	  comp_index++;
	  ଜାରି;
	}

	unsigned old_ଗଣନା = all_points.length;

	ଯଦି (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_ଗଣନା,
						    budget)))
	{
	  points.resize (old_length);
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_ଗଣନା, PHANTOM_ଗଣନା);

	auto comp_points = all_points.as_array ().sub_array (old_ଗଣନା);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	ଯଦି (use_my_metrics && item.is_use_my_metrics ())
	  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < PHANTOM_ଗଣନା; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_ଗଣନା + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	ଯଦି (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}

	ଯଦି (comp_points) // Empty in case of phantom_only
	{
	  ଦଶମିକ matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	ଯଦି (item.is_anchored () && !phantom_only)
	{
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  ଯଦି (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_ଗଣନା);

	ଯଦି (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}

	comp_index++;
      }

      ଯଦି (head_maxp_info && depth == 0)
      {
        ଯଦି (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ବାମ].x;
      points.resize (old_length);
    } ଭାଙ୍ଗ;
    ସ୍ଥିତି EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ବାମ].x;
      points.resize (old_length);
      ଭାଙ୍ଗ;
    }

    ଯଦି (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      ଯଦି (shift)
        ପାଇଁ (auto &point : all_points)
	  point.x -= shift;
    }

    ଫେରନ୍ତୁ !all_points.in_error ();
  }

  ତାର୍କିକ get_extents_without_var_scaled (hb_font_t *font, ଅଚଳ glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ଅଚଳ
  {
    ଯଦି (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      ଫେରନ୍ତୁ ସତ୍ୟ; /* Empty glyph; zero extents. */
    }
    ଫେରନ୍ତୁ header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ଅଚଳ { ଫେରନ୍ତୁ bytes; }
  glyph_type_t get_type () ଅଚଳ { ଫେରନ୍ତୁ type; }
  ଅଚଳ GlyphHeader *get_header () ଅଚଳ { ଫେରନ୍ତୁ header; }
  ତାର୍କିକ is_extended () ଅଚଳ { ଫେରନ୍ତୁ extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (ମିଥ୍ୟା)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 ତାର୍କିକ extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_contours = header->ସଂଖ୍ୟା_Of_Contours;
    ଯଦି (unlikely (ଅଙ୍କ_contours == 0)) type = EMPTY;
    ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_contours > 0) type = SIMPLE;
    ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_contours <= -1) type = COMPOSITE;
    ଅନ୍ୟଥା type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  ସୁରକ୍ଷିତ:
  hb_bytes_t bytes;
  ଅଚଳ GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  ତାର୍କିକ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
