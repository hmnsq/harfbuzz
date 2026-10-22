#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#અન્તર્ભાવ "../../hb-open-type.hh"

#અન્તર્ભાવ "GlyphHeader.hh"
#અન્તર્ભાવ "SimpleGlyph.hh"
#અન્તર્ભાવ "CompositeGlyph.hh"


નામાકાશ OT {

સંરચના glyf_accelerator_t;

નામાકાશ glyf_impl {


ગણના phantom_point_index_t
{
  PHANTOM_ડાબું   = 0,
  PHANTOM_જમણું  = 1,
  PHANTOM_ટોચ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_ગણતરી  = 4
};

સંરચના Glyph
{
  ગણના glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  જાહેર:
  composite_iter_t get_composite_iterator () અચલ
  {
    જો (type != COMPOSITE) પાછા composite_iter_t ();
    પાછા CompositeGlyph (*header, bytes, extended).iter ();
  }

  અચલ hb_bytes_t trim_padding () અચલ
  {
    પસંદગી (type) {
    સ્થિતિ COMPOSITE: પાછા CompositeGlyph (*header, bytes, extended).trim_padding ();
    સ્થિતિ SIMPLE:    પાછા SimpleGlyph (*header, bytes).trim_padding ();
    સ્થિતિ EMPTY:     પાછા bytes;
    મૂળભૂત:        પાછા bytes;
    }
  }

  શૂન્ય drop_hints ()
  {
    પસંદગી (type) {
    સ્થિતિ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); પાછા;
    સ્થિતિ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); પાછા;
    સ્થિતિ EMPTY:
      જો (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      પાછા;
    }
  }

  શૂન્ય set_overlaps_flag ()
  {
    પસંદગી (type) {
    સ્થિતિ COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); પાછા;
    સ્થિતિ SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); પાછા;
    સ્થિતિ EMPTY:     પાછા;
    }
  }

  શૂન્ય drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) અચલ
  {
    પસંદગી (type) {
    સ્થિતિ COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); પાછા;
    સ્થિતિ SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); પાછા;
    સ્થિતિ EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      જો (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      પાછા;
    }
  }

  તાર્કિક is_composite () અચલ
  { પાછા type == COMPOSITE; }

  તાર્કિક get_all_points_without_var (અચલ hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) અચલ
  {
    પસંદગી (type) {
    સ્થિતિ SIMPLE:
      જો (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        પાછા ખોટું;
      તોડો;
    સ્થિતિ COMPOSITE:
    {
      માટે (auto &item : get_composite_iterator ())
        જો (unlikely (!item.get_points (points, extended))) પાછા ખોટું;
      તોડો;
    }
    સ્થિતિ EMPTY:
      તોડો;
    }

    /* Init phantom points */
    જો (unlikely (!points.resize (points.length + PHANTOM_ગણતરી))) પાછા ખોટું;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ગણતરી, PHANTOM_ગણતરી);
    {
      // Duplicated code.
      પૂર્ણાંક lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      પૂર્ણાંક h_delta = (પૂર્ણાંક) header->xMin - lsb;
      HB_UNUSED પૂર્ણાંક tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      પૂર્ણાંક v_orig  = (પૂર્ણાંક) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#નહીંતર
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ડાબું].x = h_delta;
      phantoms[PHANTOM_જમણું].x = (પૂર્ણાંક) h_adv + h_delta;
      phantoms[PHANTOM_ટોચ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (પૂર્ણાંક) v_adv;
    }
    પાછા સાચું;
  }

  શૂન્ય update_mtx (અચલ hb_subset_plan_t *plan,
                   પૂર્ણાંક xMin, પૂર્ણાંક xMax,
                   પૂર્ણાંક yMin, પૂર્ણાંક yMax,
                   અચલ contour_point_vector_t &all_points) અચલ
  {
    hb_codepoint_t new_gid = 0;
    જો (!plan->new_gid_for_old_gid (gid, &new_gid))
      પાછા;

    જો (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    દશાંશ ડાબું_Side_X = all_points[len - 4].x;
    દશાંશ જમણું_Side_X = all_points[len - 3].x;
    દશાંશ ટોચ_Side_Y = all_points[len - 2].y;
    દશાંશ bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (જમણું_Side_X - ડાબું_Side_X);
    જો (hori_aw < 0) hori_aw = 0;
    પૂર્ણાંક lsb = roundf (xMin - ડાબું_Side_X);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    જો (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = ખોટું;

    signed vert_aw = roundf (ટોચ_Side_Y - bottomSideY);
    જો (vert_aw < 0) vert_aw = 0;
    પૂર્ણાંક tsb = roundf (ટોચ_Side_Y - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  તાર્કિક compile_header_bytes (અચલ hb_subset_plan_t *plan,
                             અચલ contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) અચલ
  {
    GlyphHeader *glyph_header = nullptr;
    જો (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      જો (unlikely (!glyph_header)) પાછા ખોટું;
    }

    દશાંશ xMin = 0, xMax = 0;
    દશાંશ yMin = 0, yMax = 0;
    જો (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned ગણતરી = all_points.length - 4;
      માટે (unsigned i = 1; i < ગણતરી; i++)
      {
	દશાંશ x = all_points[i].x;
	દશાંશ y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    પૂર્ણાંક rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    પૂર્ણાંક rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    પૂર્ણાંક rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    પૂર્ણાંક rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    જો (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    જો (!glyph_header)
      પાછા સાચું;

    glyph_header->સંખ્યા_Of_Contours = header->સંખ્યા_Of_Contours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((અચલ અક્ષર *)glyph_header, GlyphHeader::static_size);
    પાછા સાચું;
  }

  તાર્કિક compile_bytes_with_deltas (અચલ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  અચલ glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    જો (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    જો (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, ખોટું, ખોટું))
      પાછા ખોટું;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    જો (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    જો (!plan->pinned_at_default)
    {
      પસંદગી (type)
      {
      સ્થિતિ COMPOSITE:
        જો (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          પાછા ખોટું;
        તોડો;
      સ્થિતિ SIMPLE:
        જો (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          પાછા ખોટું;
        તોડો;
      સ્થિતિ EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        જો (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->સંખ્યા_Of_Contours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          જો (length)
          {
            length = simple.length (length);
            અક્ષર *p = (અક્ષર *) hb_malloc (length);
            જો (unlikely (!p)) પાછા ખોટું;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        તોડો;
      }
    }

    જો (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      પાછા ખોટું;
    }
    પાછા સાચું;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  તાર્કિક get_points (hb_font_t *font, અચલ accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   તાર્કિક shift_points_hori = સાચું,
		   તાર્કિક use_my_metrics = સાચું,
		   તાર્કિક phantom_only = ખોટું,
		   hb_array_t<અચલ પૂર્ણાંક> coords = hb_array_t<અચલ પૂર્ણાંક> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned પૂર્ણાંક depth = 0,
		   unsigned *edge_ગણતરી = nullptr,
		   int64_t *budget = nullptr) અચલ
  {
    જો (unlikely (depth > HB_MAX_NESTING_LEVEL)) પાછા ખોટું;
    unsigned ઢગલો_edge_ગણતરી = 0;
    જો (!edge_ગણતરી) edge_ગણતરી = &ઢગલો_edge_ગણતરી;
    જો (unlikely (*edge_ગણતરી > HB_MAX_GRAPH_EDGE_ગણતરી)) પાછા ખોટું;
    (*edge_ગણતરી)++;

    int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
    જો (!budget) budget = &ઢગલો_budget;
    જો (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) પાછા ખોટું;

    જો (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    જો (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->અંક_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    પસંદગી (type) {
    સ્થિતિ SIMPLE:
      જો (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->સંખ્યા_Of_Contours);
      જો (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->સંખ્યા_Of_Contours;
      જો (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	પાછા ખોટું;
      તોડો;
    સ્થિતિ COMPOSITE:
    {
      માટે (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        જો (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          પાછા ખોટું;
        }
      }
      તોડો;
    }
    સ્થિતિ EMPTY:
      તોડો;
    }

    /* Init phantom points */
    જો (unlikely (!points.resize (points.length + PHANTOM_ગણતરી))) પાછા ખોટું;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_ગણતરી, PHANTOM_ગણતરી);
    {
      // Duplicated code.
      પૂર્ણાંક lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      પૂર્ણાંક h_delta = (પૂર્ણાંક) header->xMin - lsb;
      HB_UNUSED પૂર્ણાંક tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      પૂર્ણાંક v_orig  = (પૂર્ણાંક) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#નહીંતર
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_ડાબું].x = h_delta;
      phantoms[PHANTOM_જમણું].x = (પૂર્ણાંક) h_adv + h_delta;
      phantoms[PHANTOM_ટોચ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (પૂર્ણાંક) v_adv;
    }

#ifndef HB_NO_VAR
    જો (hb_any (coords))
    {
      જો (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        પાછા ખોટું;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    જો (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      પ્રતિજ્ઞા (old_length == 0);
      *points_with_deltas = points;
    }

    દશાંશ shift = 0;
    પસંદગી (type) {
    સ્થિતિ SIMPLE:
      જો (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_ડાબું].x;
      તોડો;
    સ્થિતિ COMPOSITE:
    {
      hb_decycler_ગાંઠ_t decycler_ગાંઠ (scratch.decycler);

      unsigned પૂર્ણાંક comp_index = 0;
      માટે (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        જો (unlikely (!decycler_ગાંઠ.visit (item_gid)))
	{
	  comp_index++;
	  ચાલુ;
	}

	unsigned old_ગણતરી = all_points.length;

	જો (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_ગણતરી,
						    budget)))
	{
	  points.resize (old_length);
	  પાછા ખોટું;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_ગણતરી, PHANTOM_ગણતરી);

	auto comp_points = all_points.as_array ().sub_array (old_ગણતરી);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	જો (use_my_metrics && item.is_use_my_metrics ())
	  માટે (unsigned પૂર્ણાંક i = 0; i < PHANTOM_ગણતરી; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_ગણતરી + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	જો (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  પાછા ખોટું;
	}

	જો (comp_points) // Empty in case of phantom_only
	{
	  દશાંશ matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	જો (item.is_anchored () && !phantom_only)
	{
	  unsigned પૂર્ણાંક p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  જો (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_ગણતરી);

	જો (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  પાછા ખોટું;
	}

	comp_index++;
      }

      જો (head_maxp_info && depth == 0)
      {
        જો (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ડાબું].x;
      points.resize (old_length);
    } તોડો;
    સ્થિતિ EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_ડાબું].x;
      points.resize (old_length);
      તોડો;
    }

    જો (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      જો (shift)
        માટે (auto &point : all_points)
	  point.x -= shift;
    }

    પાછા !all_points.in_error ();
  }

  તાર્કિક get_extents_without_var_scaled (hb_font_t *font, અચલ glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) અચલ
  {
    જો (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      પાછા સાચું; /* Empty glyph; zero extents. */
    }
    પાછા header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () અચલ { પાછા bytes; }
  glyph_type_t get_type () અચલ { પાછા type; }
  અચલ GlyphHeader *get_header () અચલ { પાછા header; }
  તાર્કિક is_extended () અચલ { પાછા extended; }

  Glyph () : bytes (),
             header (bytes.as<GlyphHeader> ()),
             gid (-1),
             type(EMPTY),
	     extended (ખોટું)
  {}

  Glyph (hb_bytes_t bytes_,
	 hb_codepoint_t gid_,
	 તાર્કિક extended_) : bytes (bytes_),
			   header (bytes.as<GlyphHeader> ()),
			   gid (gid_),
			   extended (extended_)
  {
    પૂર્ણાંક અંક_contours = header->સંખ્યા_Of_Contours;
    જો (unlikely (અંક_contours == 0)) type = EMPTY;
    નહીંતર જો (અંક_contours > 0) type = SIMPLE;
    નહીંતર જો (અંક_contours <= -1) type = COMPOSITE;
    નહીંતર type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  સુરક્ષિત:
  hb_bytes_t bytes;
  અચલ GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  તાર્કિક extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
