#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"

#ಅನ್ತರ್ಭಾವ "GlyphHeader.hh"
#ಅನ್ತರ್ಭಾವ "SimpleGlyph.hh"
#ಅನ್ತರ್ಭಾವ "CompositeGlyph.hh"


ನಾಮಾಕಾಶ OT {

ಸಂರಚನಾ glyf_accelerator_t;

ನಾಮಾಕಾಶ glyf_impl {


ಗಣನಾ phantom_point_index_t
{
  PHANTOM_ಎಡ   = 0,
  PHANTOM_ಬಲ  = 1,
  PHANTOM_ತುದಿ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_ಎಣಿಕೆ  = 4
};

ಸಂರಚನಾ Glyph
{
  ಗಣನಾ glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  ಸಾರ್ವಜನಿಕ:
  composite_iter_t get_composite_iterator () ಧ್ರುವ
  {
    ಯದಿ (type != COMPOSITE) ಪ್ರತ್ಯಾಗಮ composite_iter_t ();
    ಪ್ರತ್ಯಾಗಮ CompositeGlyph (*header, bytes, extended).iter ();
  }

  ಧ್ರುವ hb_bytes_t trim_padding () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ COMPOSITE: ಪ್ರತ್ಯಾಗಮ CompositeGlyph (*header, bytes, extended).trim_padding ();
    ಪಕ್ಷ SIMPLE:    ಪ್ರತ್ಯಾಗಮ SimpleGlyph (*header, bytes).trim_padding ();
    ಪಕ್ಷ EMPTY:     ಪ್ರತ್ಯಾಗಮ bytes;
    ಸ್ವಭಾವ:        ಪ್ರತ್ಯಾಗಮ bytes;
    }
  }

  ಶೂನ್ಯ drop_hints ()
  {
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ EMPTY:
      ಯದಿ (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ set_overlaps_flag ()
  {
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ EMPTY:     ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      ಯದಿ (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ is_composite () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ type == COMPOSITE; }

  ತಾರ್ಕಿಕ get_all_points_without_var (ಧ್ರುವ hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ SIMPLE:
      ಯದಿ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಭಙ್ಗ;
    ಪಕ್ಷ COMPOSITE:
    {
      ಪ್ರತಿ (auto &item : get_composite_iterator ())
        ಯದಿ (unlikely (!item.get_points (points, extended))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಭಙ್ಗ;
    }
    ಪಕ್ಷ EMPTY:
      ಭಙ್ಗ;
    }

    /* Init phantom points */
    ಯದಿ (unlikely (!points.resize (points.length + PHANTOM_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ಎಣಿಕೆ, PHANTOM_ಎಣಿಕೆ);
    {
      // Duplicated code.
      ಪೂರ್ಣಾಙ್ಕ lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ಪೂರ್ಣಾಙ್ಕ h_delta = (ಪೂರ್ಣಾಙ್ಕ) header->xMin - lsb;
      HB_UNUSED ಪೂರ್ಣಾಙ್ಕ tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ಪೂರ್ಣಾಙ್ಕ v_orig  = (ಪೂರ್ಣಾಙ್ಕ) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#ಅನ್ಯಥಾ
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ಎಡ].x = h_delta;
      phantoms[PHANTOM_ಬಲ].x = (ಪೂರ್ಣಾಙ್ಕ) h_adv + h_delta;
      phantoms[PHANTOM_ತುದಿ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ಪೂರ್ಣಾಙ್ಕ) v_adv;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ update_mtx (ಧ್ರುವ hb_subset_plan_t *plan,
                   ಪೂರ್ಣಾಙ್ಕ xMin, ಪೂರ್ಣಾಙ್ಕ xMax,
                   ಪೂರ್ಣಾಙ್ಕ yMin, ಪೂರ್ಣಾಙ್ಕ yMax,
                   ಧ್ರುವ contour_point_vector_t &all_points) ಧ್ರುವ
  {
    hb_codepoint_t new_gid = 0;
    ಯದಿ (!plan->new_gid_for_old_gid (gid, &new_gid))
      ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    ಭಿನ್ನ ಎಡ_Side_X = all_points[len - 4].x;
    ಭಿನ್ನ ಬಲ_Side_X = all_points[len - 3].x;
    ಭಿನ್ನ ತುದಿ_Side_Y = all_points[len - 2].y;
    ಭಿನ್ನ bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (ಬಲ_Side_X - ಎಡ_Side_X);
    ಯದಿ (hori_aw < 0) hori_aw = 0;
    ಪೂರ್ಣಾಙ್ಕ lsb = roundf (xMin - ಎಡ_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    ಯದಿ (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = ಅಸತ್ಯ;

    signed vert_aw = roundf (ತುದಿ_Side_Y - bottomSideY);
    ಯದಿ (vert_aw < 0) vert_aw = 0;
    ಪೂರ್ಣಾಙ್ಕ tsb = roundf (ತುದಿ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  ತಾರ್ಕಿಕ compile_header_bytes (ಧ್ರುವ hb_subset_plan_t *plan,
                             ಧ್ರುವ contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) ಧ್ರುವ
  {
    GlyphHeader *glyph_header = nullptr;
    ಯದಿ (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      ಯದಿ (unlikely (!glyph_header)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಭಿನ್ನ xMin = 0, xMax = 0;
    ಭಿನ್ನ yMin = 0, yMax = 0;
    ಯದಿ (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned ಎಣಿಕೆ = all_points.length - 4;
      ಪ್ರತಿ (unsigned i = 1; i < ಎಣಿಕೆ; i++)
      {
	ಭಿನ್ನ x = all_points[i].x;
	ಭಿನ್ನ y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    ಪೂರ್ಣಾಙ್ಕ rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    ಪೂರ್ಣಾಙ್ಕ rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    ಪೂರ್ಣಾಙ್ಕ rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    ಪೂರ್ಣಾಙ್ಕ rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    ಯದಿ (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    ಯದಿ (!glyph_header)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    glyph_header->ಸಂಖ್ಯೆ_Of_Contours = header->ಸಂಖ್ಯೆ_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *)glyph_header, GlyphHeader::static_size);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ compile_bytes_with_deltas (ಧ್ರುವ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ಧ್ರುವ glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    ಯದಿ (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    ಯದಿ (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, ಅಸತ್ಯ, ಅಸತ್ಯ))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    ಯದಿ (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    ಯದಿ (!plan->pinned_at_default)
    {
      ವಿಕಲ್ಪ (type)
      {
      ಪಕ್ಷ COMPOSITE:
        ಯದಿ (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಭಙ್ಗ;
      ಪಕ್ಷ SIMPLE:
        ಯದಿ (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಭಙ್ಗ;
      ಪಕ್ಷ EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        ಯದಿ (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->ಸಂಖ್ಯೆ_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          ಯದಿ (length)
          {
            length = simple.length (length);
            ವರ್ಣ *p = (ವರ್ಣ *) hb_malloc (length);
            ಯದಿ (unlikely (!p)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        ಭಙ್ಗ;
      }
    }

    ಯದಿ (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  ತಾರ್ಕಿಕ get_points (hb_font_t *font, ಧ್ರುವ accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   ತಾರ್ಕಿಕ shift_points_hori = ಸತ್ಯ,
		   ತಾರ್ಕಿಕ use_my_metrics = ಸತ್ಯ,
		   ತಾರ್ಕಿಕ phantom_only = ಅಸತ್ಯ,
		   hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords = hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned ಪೂರ್ಣಾಙ್ಕ depth = 0,
		   unsigned *edge_ಎಣಿಕೆ = nullptr,
		   int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (depth > HB_MAX_NESTING_LEVEL)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ರಾಶಿ_edge_ಎಣಿಕೆ = 0;
    ಯದಿ (!edge_ಎಣಿಕೆ) edge_ಎಣಿಕೆ = &ರಾಶಿ_edge_ಎಣಿಕೆ;
    ಯದಿ (unlikely (*edge_ಎಣಿಕೆ > HB_MAX_GRAPH_EDGE_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    (*edge_ಎಣಿಕೆ)++;

    int64_t ರಾಶಿ_budget = HB_BUDGET_GLYPH;
    ಯದಿ (!budget) budget = &ರಾಶಿ_budget;
    ಯದಿ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    ಯದಿ (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->ಅಂಕೆ_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ SIMPLE:
      ಯದಿ (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->ಸಂಖ್ಯೆ_Of_Contours);
      ಯದಿ (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->ಸಂಖ್ಯೆ_Of_Contours;
      ಯದಿ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಭಙ್ಗ;
    ಪಕ್ಷ COMPOSITE:
    {
      ಪ್ರತಿ (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        ಯದಿ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
      }
      ಭಙ್ಗ;
    }
    ಪಕ್ಷ EMPTY:
      ಭಙ್ಗ;
    }

    /* Init phantom points */
    ಯದಿ (unlikely (!points.resize (points.length + PHANTOM_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ಎಣಿಕೆ, PHANTOM_ಎಣಿಕೆ);
    {
      // Duplicated code.
      ಪೂರ್ಣಾಙ್ಕ lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ಪೂರ್ಣಾಙ್ಕ h_delta = (ಪೂರ್ಣಾಙ್ಕ) header->xMin - lsb;
      HB_UNUSED ಪೂರ್ಣಾಙ್ಕ tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ಪೂರ್ಣಾಙ್ಕ v_orig  = (ಪೂರ್ಣಾಙ್ಕ) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#ಅನ್ಯಥಾ
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ಎಡ].x = h_delta;
      phantoms[PHANTOM_ಬಲ].x = (ಪೂರ್ಣಾಙ್ಕ) h_adv + h_delta;
      phantoms[PHANTOM_ತುದಿ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ಪೂರ್ಣಾಙ್ಕ) v_adv;
    }

#ifndef HB_NO_VAR
    ಯದಿ (hb_any (coords))
    {
      ಯದಿ (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    ಯದಿ (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      ಪ್ರತಿಜ್ಞಾ (old_length == 0);
      *points_with_deltas = points;
    }

    ಭಿನ್ನ shift = 0;
    ವಿಕಲ್ಪ (type) {
    ಪಕ್ಷ SIMPLE:
      ಯದಿ (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_ಎಡ].x;
      ಭಙ್ಗ;
    ಪಕ್ಷ COMPOSITE:
    {
      hb_decycler_ಗಂಟು_t decycler_ಗಂಟು (scratch.decycler);

      unsigned ಪೂರ್ಣಾಙ್ಕ comp_index = 0;
      ಪ್ರತಿ (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        ಯದಿ (unlikely (!decycler_ಗಂಟು.visit (item_gid)))
	{
	  comp_index++;
	  ಅನುವರ್ತ;
	}

	unsigned old_ಎಣಿಕೆ = all_points.length;

	ಯದಿ (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_ಎಣಿಕೆ,
						    budget)))
	{
	  points.resize (old_length);
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_ಎಣಿಕೆ, PHANTOM_ಎಣಿಕೆ);

	auto comp_points = all_points.as_array ().sub_array (old_ಎಣಿಕೆ);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	ಯದಿ (use_my_metrics && item.is_use_my_metrics ())
	  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < PHANTOM_ಎಣಿಕೆ; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_ಎಣಿಕೆ + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	ಯದಿ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}

	ಯದಿ (comp_points) // Empty in case of phantom_only
	{
	  ಭಿನ್ನ matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	ಯದಿ (item.is_anchored () && !phantom_only)
	{
	  unsigned ಪೂರ್ಣಾಙ್ಕ p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  ಯದಿ (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_ಎಣಿಕೆ);

	ಯದಿ (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}

	comp_index++;
      }

      ಯದಿ (head_maxp_info && depth == 0)
      {
        ಯದಿ (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ಎಡ].x;
      points.resize (old_length);
    } ಭಙ್ಗ;
    ಪಕ್ಷ EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ಎಡ].x;
      points.resize (old_length);
      ಭಙ್ಗ;
    }

    ಯದಿ (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      ಯದಿ (shift)
        ಪ್ರತಿ (auto &point : all_points)
	  point.x -= shift;
    }

    ಪ್ರತ್ಯಾಗಮ !all_points.in_error ();
  }

  ತಾರ್ಕಿಕ get_extents_without_var_scaled (hb_font_t *font, ಧ್ರುವ glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ಧ್ರುವ
  {
    ಯದಿ (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* Empty glyph; zero extents. */
    }
    ಪ್ರತ್ಯಾಗಮ header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ bytes; }
  glyph_type_t get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ type; }
  ಧ್ರುವ GlyphHeader *get_header () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ header; }
  ತಾರ್ಕಿಕ is_extended () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (ಅಸತ್ಯ)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 ತಾರ್ಕಿಕ extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_contours = header->ಸಂಖ್ಯೆ_Of_Contours;
    ಯದಿ (unlikely (ಅಂಕೆ_contours == 0)) type = EMPTY;
    ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_contours > 0) type = SIMPLE;
    ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_contours <= -1) type = COMPOSITE;
    ಅನ್ಯಥಾ type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  ರಕ್ಷಿತ:
  hb_bytes_t bytes;
  ಧ್ರುವ GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  ತಾರ್ಕಿಕ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
