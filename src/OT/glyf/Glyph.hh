#ifndef OT_GLYF_GLYPH_HH
#define OT_GLYF_GLYPH_HH


#include "../../hb-open-type.hh"

#include "GlyphHeader.hh"
#include "SimpleGlyph.hh"
#include "CompositeGlyph.hh"


namespace OT {

sa_nracanaa glyf_accelerator_t;

namespace glyf_impl {


ga_nanaa phantom_point_index_t
{
  PHANTOM_LEFT   = 0,
  PHANTOM_RIGHT  = 1,
  PHANTOM_TOP    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_COUNT  = 4
};

sa_nracanaa Glyph
{
  ga_nanaa glyph_type_t {
    EMPTY,
    SIMPLE,
    COMPOSITE,
  };

  public:
  composite_iter_t get_composite_iterator () const
  {
    ប្រសិនបើ (type != COMPOSITE) ត្រឡប់ composite_iter_t ();
    ត្រឡប់ CompositeGlyph (*header, bytes, extended).iter ();
  }

  const hb_bytes_t trim_padding () const
  {
    switch (type) {
    pak_sa COMPOSITE: ត្រឡប់ CompositeGlyph (*header, bytes, extended).trim_padding ();
    pak_sa SIMPLE:    ត្រឡប់ SimpleGlyph (*header, bytes).trim_padding ();
    pak_sa EMPTY:     ត្រឡប់ bytes;
    default:        ត្រឡប់ bytes;
    }
  }

  void drop_hints ()
  {
    switch (type) {
    pak_sa COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints (); ត្រឡប់;
    pak_sa SIMPLE:    SimpleGlyph (*header, bytes).drop_hints (); ត្រឡប់;
    pak_sa EMPTY:
      ប្រសិនបើ (bytes.length >= GlyphHeader::static_size)
        SimpleGlyph (*header, bytes).drop_hints ();
      ត្រឡប់;
    }
  }

  void set_overlaps_flag ()
  {
    switch (type) {
    pak_sa COMPOSITE: CompositeGlyph (*header, bytes, extended).set_overlaps_flag (); ត្រឡប់;
    pak_sa SIMPLE:    SimpleGlyph (*header, bytes).set_overlaps_flag (); ត្រឡប់;
    pak_sa EMPTY:     ត្រឡប់;
    }
  }

  void drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) const
  {
    switch (type) {
    pak_sa COMPOSITE: CompositeGlyph (*header, bytes, extended).drop_hints_bytes (dest_start); ត្រឡប់;
    pak_sa SIMPLE:    SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end); ត្រឡប់;
    pak_sa EMPTY:
      dest_start = bytes;
      dest_end = hb_bytes_t ();
      ប្រសិនបើ (bytes.length >= GlyphHeader::static_size &&
          SimpleGlyph (*header, bytes).has_instructions_length ())
        SimpleGlyph (*header, bytes).drop_hints_bytes (dest_start, dest_end);
      ត្រឡប់;
    }
  }

  bool is_composite () const
  { ត្រឡប់ type == COMPOSITE; }

  bool get_all_points_without_var (const hb_face_t *face,
                                   contour_point_vector_t &points /* OUT */) const
  {
    switch (type) {
    pak_sa SIMPLE:
      ប្រសិនបើ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (points, extended)))
        ត្រឡប់ false;
      bha_nga;
    pak_sa COMPOSITE:
    {
      សម្រាប់ (auto &item : get_composite_iterator ())
        ប្រសិនបើ (unlikely (!item.get_points (points, extended))) ត្រឡប់ false;
      bha_nga;
    }
    pak_sa EMPTY:
      bha_nga;
    }

    /* Init phantom points */
    ប្រសិនបើ (unlikely (!points.resize (points.length + PHANTOM_COUNT))) ត្រឡប់ false;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_COUNT, PHANTOM_COUNT);
    {
      // Duplicated code.
      ចំនួនគត់ lsb = 0;
      face->table.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ចំនួនគត់ h_delta = (ចំនួនគត់) header->xMin - lsb;
      HB_UNUSED ចំនួនគត់ tsb = 0;
#ifndef HB_NO_VERTICAL
      face->table.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ចំនួនគត់ v_orig  = (ចំនួនគត់) header->yMax + tsb;
      unsigned h_adv = face->table.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       face->table.vmtx->get_advance_without_var_unscaled (gid)
#បើមិនដូច្នេះ
                       - face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_LEFT].x = h_delta;
      phantoms[PHANTOM_RIGHT].x = (ចំនួនគត់) h_adv + h_delta;
      phantoms[PHANTOM_TOP].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ចំនួនគត់) v_adv;
    }
    ត្រឡប់ true;
  }

  void update_mtx (const hb_subset_plan_t *plan,
                   ចំនួនគត់ xMin, ចំនួនគត់ xMax,
                   ចំនួនគត់ yMin, ចំនួនគត់ yMax,
                   const contour_point_vector_t &all_points) const
  {
    hb_codepoint_t new_gid = 0;
    ប្រសិនបើ (!plan->new_gid_for_old_gid (gid, &new_gid))
      ត្រឡប់;

    ប្រសិនបើ (type != EMPTY)
    {
      plan->bounds_width_vec[new_gid] = xMax - xMin;
      plan->bounds_height_vec[new_gid] = yMax - yMin;
    }

    unsigned len = all_points.length;
    float leftSideX = all_points[len - 4].x;
    float rightSideX = all_points[len - 3].x;
    float topSideY = all_points[len - 2].y;
    float bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    signed hori_aw = roundf (rightSideX - leftSideX);
    ប្រសិនបើ (hori_aw < 0) hori_aw = 0;
    ចំនួនគត់ lsb = roundf (xMin - leftSideX);
    plan->hmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    ប្រសិនបើ (type != EMPTY && lsb != xMin)
      plan->head_maxp_info.allXMinIsLsb = false;

    signed vert_aw = roundf (topSideY - bottomSideY);
    ប្រសិនបើ (vert_aw < 0) vert_aw = 0;
    ចំនួនគត់ tsb = roundf (topSideY - yMax);
    plan->vmtx_map.set_with_hash (new_gid, hash, hb_pair ((unsigned) vert_aw, tsb));
  }

  bool compile_header_bytes (const hb_subset_plan_t *plan,
                             const contour_point_vector_t &all_points,
                             hb_bytes_t &dest_bytes /* OUT */) const
  {
    GlyphHeader *glyph_header = nullptr;
    ប្រសិនបើ (!plan->pinned_at_default && type != EMPTY && all_points.length >= 4)
    {
      glyph_header = (GlyphHeader *) hb_calloc (1, GlyphHeader::static_size);
      ប្រសិនបើ (unlikely (!glyph_header)) ត្រឡប់ false;
    }

    float xMin = 0, xMax = 0;
    float yMin = 0, yMax = 0;
    ប្រសិនបើ (all_points.length > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      unsigned count = all_points.length - 4;
      សម្រាប់ (unsigned i = 1; i < count; i++)
      {
	float x = all_points[i].x;
	float y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    ចំនួនគត់ rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    ចំនួនគត់ rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    ចំនួនគត់ rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    ចំនួនគត់ rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    ប្រសិនបើ (type != EMPTY)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, rounded_xMin);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin, rounded_yMin);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax, rounded_xMax);
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    ប្រសិនបើ (!glyph_header)
      ត្រឡប់ true;

    glyph_header->numberOfContours = header->numberOfContours;

    glyph_header->xMin = rounded_xMin;
    glyph_header->yMin = rounded_yMin;
    glyph_header->xMax = rounded_xMax;
    glyph_header->yMax = rounded_yMax;

    dest_bytes = hb_bytes_t ((const var_na *)glyph_header, GlyphHeader::static_size);
    ត្រឡប់ true;
  }

  bool compile_bytes_with_deltas (const hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  const glyf_accelerator_t &glyf,
                                  hb_bytes_t &dest_start,  /* IN/OUT */
                                  hb_bytes_t &dest_end /* OUT */)
  {
    contour_point_vector_t all_points, points_with_deltas;
    unsigned composite_contours = 0;
    head_maxp_info_t *head_maxp_info_p = &plan->head_maxp_info;
    unsigned *composite_contours_p = &composite_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    ប្រសិនបើ (type == EMPTY ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      head_maxp_info_p = nullptr;
      composite_contours_p = nullptr;
    }

    hb_glyf_scratch_t scratch;
    ប្រសិនបើ (!get_points (font, glyf, all_points, scratch, &points_with_deltas, head_maxp_info_p, composite_contours_p, false, false))
      ត្រឡប់ false;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    ប្រសិនបើ (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = EMPTY;
      dest_start = hb_bytes_t ();
      dest_end = hb_bytes_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    ប្រសិនបើ (!plan->pinned_at_default)
    {
      switch (type)
      {
      pak_sa COMPOSITE:
        ប្រសិនបើ (!CompositeGlyph (*header, bytes, extended).compile_bytes_with_deltas (dest_start,
										  points_with_deltas,
										  dest_end))
          ត្រឡប់ false;
        bha_nga;
      pak_sa SIMPLE:
        ប្រសិនបើ (!SimpleGlyph (*header, bytes).compile_bytes_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_end))
          ត្រឡប់ false;
        bha_nga;
      pak_sa EMPTY:
        dest_start = hb_bytes_t ();
        dest_end = hb_bytes_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        ប្រសិនបើ (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            bytes.length >= GlyphHeader::static_size &&
            header->numberOfContours == 0)
        {
          SimpleGlyph simple (*header, bytes);
          unsigned length = simple.instructions_length ();
          ប្រសិនបើ (length)
          {
            length = simple.length (length);
            var_na *p = (var_na *) hb_malloc (length);
            ប្រសិនបើ (unlikely (!p)) ត្រឡប់ false;
            hb_memcpy (p, bytes.arrayZ, length);
            dest_start = hb_bytes_t (p, length);
          }
        }
        bha_nga;
      }
    }

    ប្រសិនបើ (!compile_header_bytes (plan, all_points, dest_start))
    {
      dest_end.fini ();
      ត្រឡប់ false;
    }
    ត្រឡប់ true;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  template <typename accelerator_t>
  bool get_points (hb_font_t *font, const accelerator_t &glyf_accelerator,
		   contour_point_vector_t &all_points /* OUT */,
		   hb_glyf_scratch_t &scratch,
		   contour_point_vector_t *points_with_deltas = nullptr, /* OUT */
		   head_maxp_info_t * head_maxp_info = nullptr, /* OUT */
		   unsigned *composite_contours = nullptr, /* OUT */
		   bool shift_points_hori = true,
		   bool use_my_metrics = true,
		   bool phantom_only = false,
		   hb_array_t<const ចំនួនគត់> coords = hb_array_t<const ចំនួនគត់> (),
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   unsigned ចំនួនគត់ depth = 0,
		   unsigned *edge_count = nullptr,
		   int64_t *budget = nullptr) const
  {
    ប្រសិនបើ (unlikely (depth > HB_MAX_NESTING_LEVEL)) ត្រឡប់ false;
    unsigned stack_edge_count = 0;
    ប្រសិនបើ (!edge_count) edge_count = &stack_edge_count;
    ប្រសិនបើ (unlikely (*edge_count > HB_MAX_GRAPH_EDGE_COUNT)) ត្រឡប់ false;
    (*edge_count)++;

    int64_t stack_budget = HB_BUDGET_GLYPH;
    ប្រសិនបើ (!budget) budget = &stack_budget;
    ប្រសិនបើ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) ត្រឡប់ false;

    ប្រសិនបើ (head_maxp_info)
    {
      head_maxp_info->maxComponentDepth = hb_max (head_maxp_info->maxComponentDepth, depth);
    }

    ប្រសិនបើ (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->num_coords);

    contour_point_vector_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    unsigned old_length = points.length;

    switch (type) {
    pak_sa SIMPLE:
      ប្រសិនបើ (depth == 0 && head_maxp_info)
        head_maxp_info->maxContours = hb_max (head_maxp_info->maxContours, (unsigned) header->numberOfContours);
      ប្រសិនបើ (depth > 0 && composite_contours)
        *composite_contours += (unsigned) header->numberOfContours;
      ប្រសិនបើ (unlikely (!SimpleGlyph (*header, bytes).get_contour_points (all_points, extended, phantom_only, budget)))
	ត្រឡប់ false;
      bha_nga;
    pak_sa COMPOSITE:
    {
      សម្រាប់ (auto &item : get_composite_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        ប្រសិនបើ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !item.get_points (points, extended)))
        {
          points.resize (old_length);
          ត្រឡប់ false;
        }
      }
      bha_nga;
    }
    pak_sa EMPTY:
      bha_nga;
    }

    /* Init phantom points */
    ប្រសិនបើ (unlikely (!points.resize (points.length + PHANTOM_COUNT))) ត្រឡប់ false;
    hb_array_t<contour_point_t> phantoms = points.as_array ().sub_array (points.length - PHANTOM_COUNT, PHANTOM_COUNT);
    {
      // Duplicated code.
      ចំនួនគត់ lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ចំនួនគត់ h_delta = (ចំនួនគត់) header->xMin - lsb;
      HB_UNUSED ចំនួនគត់ tsb = 0;
#ifndef HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#endif
      ចំនួនគត់ v_orig  = (ចំនួនគត់) header->yMax + tsb;
      unsigned h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      unsigned v_adv =
#ifndef HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#បើមិនដូច្នេះ
                       - font->face->get_upem ()
#endif
                       ;
      phantoms[PHANTOM_LEFT].x = h_delta;
      phantoms[PHANTOM_RIGHT].x = (ចំនួនគត់) h_adv + h_delta;
      phantoms[PHANTOM_TOP].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ចំនួនគត់) v_adv;
    }

#ifndef HB_NO_VAR
    ប្រសិនបើ (hb_any (coords))
    {
      ប្រសិនបើ (!glyf_accelerator.gvar->apply_deltas_to_points (gid,
							  coords,
							  points.as_array ().sub_array (old_length),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        ត្រឡប់ false;
    }
#endif

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    ប្រសិនបើ (points_with_deltas != nullptr && depth == 0 && type == COMPOSITE)
    {
      assert (old_length == 0);
      *points_with_deltas = points;
    }

    float shift = 0;
    switch (type) {
    pak_sa SIMPLE:
      ប្រសិនបើ (depth == 0 && head_maxp_info)
        head_maxp_info->maxPoints = hb_max (head_maxp_info->maxPoints, all_points.length - old_length - 4);
      shift = phantoms[PHANTOM_LEFT].x;
      bha_nga;
    pak_sa COMPOSITE:
    {
      hb_decycler_node_t decycler_node (scratch.decycler);

      unsigned ចំនួនគត់ comp_index = 0;
      សម្រាប់ (auto &item : get_composite_iterator ())
      {
	hb_codepoint_t item_gid = item.get_gid (extended);

        ប្រសិនបើ (unlikely (!decycler_node.visit (item_gid)))
	{
	  comp_index++;
	  continue;
	}

	unsigned old_count = all_points.length;

	ប្រសិនបើ (unlikely ((!phantom_only || (use_my_metrics && item.is_use_my_metrics ())) &&
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
						    edge_count,
						    budget)))
	{
	  points.resize (old_length);
	  ត្រឡប់ false;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.length - PHANTOM_COUNT, PHANTOM_COUNT);

	auto comp_points = all_points.as_array ().sub_array (old_count);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	ប្រសិនបើ (use_my_metrics && item.is_use_my_metrics ())
	  សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < PHANTOM_COUNT; i++)
	    phantoms[i] = comp_points[comp_points.length - PHANTOM_COUNT + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	ប្រសិនបើ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.length)))
	{
	  points.resize (old_length);
	  ត្រឡប់ false;
	}

	ប្រសិនបើ (comp_points) // Empty in case of phantom_only
	{
	  float matrix[4];
	  contour_point_t default_trans;
	  item.get_transformation (matrix, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  item.transform_points (comp_points, matrix, points[old_length + comp_index]);
	}

	ប្រសិនបើ (item.is_anchored () && !phantom_only)
	{
	  unsigned ចំនួនគត់ p1, p2;
	  item.get_anchor_points (p1, p2, extended);
	  ប្រសិនបើ (likely (p1 < all_points.length && p2 < comp_points.length))
	  {
	    contour_point_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    item.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.length - PHANTOM_COUNT);

	ប្រសិនបើ (all_points.length > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_length);
	  ត្រឡប់ false;
	}

	comp_index++;
      }

      ប្រសិនបើ (head_maxp_info && depth == 0)
      {
        ប្រសិនបើ (composite_contours)
          head_maxp_info->maxCompositeContours = hb_max (head_maxp_info->maxCompositeContours, *composite_contours);
        head_maxp_info->maxCompositePoints = hb_max (head_maxp_info->maxCompositePoints, all_points.length);
        head_maxp_info->maxComponentElements = hb_max (head_maxp_info->maxComponentElements, comp_index);
      }
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_LEFT].x;
      points.resize (old_length);
    } bha_nga;
    pak_sa EMPTY:
      all_points.extend (phantoms);
      shift = phantoms[PHANTOM_LEFT].x;
      points.resize (old_length);
      bha_nga;
    }

    ប្រសិនបើ (depth == 0 && shift_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      ប្រសិនបើ (shift)
        សម្រាប់ (auto &point : all_points)
	  point.x -= shift;
    }

    ត្រឡប់ !all_points.in_error ();
  }

  bool get_extents_without_var_scaled (hb_font_t *font, const glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) const
  {
    ប្រសិនបើ (type == EMPTY)
    {
      *extents = {0, 0, 0, 0};
      ត្រឡប់ true; /* Empty glyph; zero extents. */
    }
    ត្រឡប់ header->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_bytes_t get_bytes () const { ត្រឡប់ bytes; }
  glyph_type_t get_type () const { ត្រឡប់ type; }
  const GlyphHeader *get_header () const { ត្រឡប់ header; }
  bool is_extended () const { ត្រឡប់ extended; }

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
    ចំនួនគត់ num_contours = header->numberOfContours;
    ប្រសិនបើ (unlikely (num_contours == 0)) type = EMPTY;
    បើមិនដូច្នេះ ប្រសិនបើ (num_contours > 0) type = SIMPLE;
    បើមិនដូច្នេះ ប្រសិនបើ (num_contours <= -1) type = COMPOSITE;
    បើមិនដូច្នេះ type = EMPTY; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  rak_sita:
  hb_bytes_t bytes;
  const GlyphHeader *header;
  hb_codepoint_t gid;
  glyph_type_t type;
  bool extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPH_HH */
