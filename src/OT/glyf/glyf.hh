#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-depend-data.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-ot-head-table.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-ot-hmtx-table.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-ot-var-gvar-table.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-paint.hh"

#ಅನ್ತರ್ಭಾವ "glyf-helpers.hh"
#ಅನ್ತರ್ಭಾವ "Glyph.hh"
#ಅನ್ತರ್ಭಾವ "SubsetGlyph.hh"
#ಅನ್ತರ್ಭಾವ "loca.hh"
#ಅನ್ತರ್ಭಾವ "path-builder.hh"


ನಾಮಾಕಾಶ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

ಸಂರಚನಾ glyf
{
  friend ಸಂರಚನಾ glyf_accelerator_t;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  ಸ್ಥಿರ ತಾರ್ಕಿಕ has_valid_glyf_format(ಧ್ರುವ hb_face_t* face)
  {
    ಧ್ರುವ OT::head &head = *face->table.head;
    ಪ್ರತ್ಯಾಗಮ head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    ತಾರ್ಕಿಕ ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ಪ್ರತ್ಯಾಗಮ ret;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c HB_UNUSED) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (ಸತ್ಯ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it,
                  ತಾರ್ಕಿಕ use_short_loca,
		  ಧ್ರುವ hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (ಏತತ್);

    unsigned init_len = c->length ();
    ಪ್ರತಿ (auto &_ : it)
      ಯದಿ (unlikely (!_.serialize (c, use_short_loca, plan)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ಯದಿ (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (ಸತ್ಯ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ತಾರ್ಕಿಕ extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (ಅಸತ್ಯ);
#endif

    ಯದಿ (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (ಅಸತ್ಯ);
    }

    hb_font_t *font = nullptr;
    ಯದಿ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ಯದಿ (unlikely (!font))
	return_trace (ಅಸತ್ಯ);
    }

    hb_vector_t<unsigned> padded_offsets;
    ಯದಿ (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (ಅಸತ್ಯ);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    ಯದಿ (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    ಪ್ರತಿ (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    ತಾರ್ಕಿಕ use_short_loca = ಅಸತ್ಯ;
    ಯದಿ (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    ಯದಿ (!use_short_loca)
    {
      padded_offsets.clear ();
      ಪ್ರತಿ (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed <glyf> ();
    ತಾರ್ಕಿಕ result = glyf_ಅವಿಭಾಜ್ಯ->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ಯದಿ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    ಯದಿ (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (ಅಸತ್ಯ);

    ಪ್ರತ್ಯಾಗಮ result;
  }

  ತಾರ್ಕಿಕ
  _populate_subset_glyphs (ಧ್ರುವ hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ಧ್ರುವ;

  hb_font_t *
  _create_font_for_instancing (ಧ್ರುವ hb_subset_plan_t *plan) ಧ್ರುವ;

  ಶೂನ್ಯ _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  ರಕ್ಷಿತ:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
ಸಂರಚನಾ GLYF : glyf
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

ಸಂರಚನಾ glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = ಅಸತ್ಯ;
    short_offset = ಅಸತ್ಯ;
    ಅಂಕೆ_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    ಧ್ರುವ OT::head &head = *face->table.head;
    ಯದಿ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ಪ್ರತ್ಯಾಗಮ;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    ಯದಿ (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    ಅನ್ಯಥಾ
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    ಯದಿ (!extended)
    {
      glyf_table = hb_sanitize_context_t ().reference_table<glyf> (face);
      loca_table = hb_sanitize_context_t ().reference_table<loca> (face);
    }
#ifndef HB_NO_VAR
    gvar = face->table.gvar;
#endif
    hmtx = face->table.hmtx;
#ifndef HB_NO_VERTICAL
    vmtx = face->table.vmtx;
#endif

    ಅಂಕೆ_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    ಅಂಕೆ_glyphs = hb_min (ಅಂಕೆ_glyphs, face->get_ಅಂಕೆ_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    ಯದಿ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_glyphs; }
  ತಾರ್ಕಿಕ is_extended () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ extended; }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *builder) ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (hb_codepoint_t gid = 0; gid < get_ಅಂಕೆ_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      ಪ್ರತಿ (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  ರಕ್ಷಿತ:
  template<typename T>
  ತಾರ್ಕಿಕ get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಯದಿ (gid >= ಅಂಕೆ_glyphs) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    int64_t ರಾಶಿ_budget = HB_BUDGET_GLYPH;
    ಯದಿ (!budget) budget = &ರಾಶಿ_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    ತಾರ್ಕಿಕ phantom_only = !consumer.is_consuming_contour_points ();
    ಯದಿ (unlikely (!glyph_for_gid (gid).get_points (font, *ಏತತ್, all_points, scratch, nullptr, nullptr, nullptr, ಸತ್ಯ, ಸತ್ಯ, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಎಣಿಕೆ = all_points.length;
    ಪ್ರತಿಜ್ಞಾ (ಎಣಿಕೆ >= glyf_impl::PHANTOM_ಎಣಿಕೆ);
    ಎಣಿಕೆ -= glyf_impl::PHANTOM_ಎಣಿಕೆ;

    ಯದಿ (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      ಯದಿ (ಅಸತ್ಯ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	{
	  consumer.consume_point (points[i]);
	  ಯದಿ (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      ಅನ್ಯಥಾ
      {
	ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	{
	  // Start of a contour.
	  ಯದಿ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    ಪ್ರತಿ (; i < ಎಣಿಕೆ; i++)
	    {
	      consumer.consume_point (points[i]);
	      ಯದಿ (points[i].is_end_point)
	      {
		consumer.contour_end ();
		ಭಙ್ಗ;
	      }
	    }
	  }
	  ಅನ್ಯಥಾ
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    ಪ್ರತಿ (; i < ಎಣಿಕೆ; i++)
	      ಯದಿ (points[i].is_end_point)
		ಭಙ್ಗ;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ಯದಿ (likely (end < ಎಣಿಕೆ) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    ಅನ್ಯಥಾ
#endif
	    ಯದಿ (likely (end < ಎಣಿಕೆ))
	      consumer.consume_point (points[end]);

	    ಪ್ರತಿ (i = start; i < limit; i++)
	      consumer.consume_point (points[i]);
	    i = end;

	    consumer.contour_end ();
	  }
	}
      }

      consumer.points_end ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_point_t *phantoms = consumer.get_phantoms_sink ();
    ಯದಿ (phantoms)
      ಪ್ರತಿ (unsigned i = 0; i < glyf_impl::PHANTOM_ಎಣಿಕೆ; ++i)
	phantoms[i] = all_points.arrayZ[ಎಣಿಕೆ + i];

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸಾರ್ವಜನಿಕ:

  ಸಂರಚನಾ points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    ತಾರ್ಕಿಕ scaled;

    ಸಂರಚನಾ contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      ಶೂನ್ಯ add (ಧ್ರುವ contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      ತಾರ್ಕಿಕ empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (min_x >= max_x) || (min_y >= max_y); }

      ಶೂನ್ಯ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ತಾರ್ಕಿಕ scaled)
      {
	ಯದಿ (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ಪ್ರತ್ಯಾಗಮ;
	}
	{
	  ದ್ವಿಗುಣ x_bearing = (ದ್ವಿಗುಣ) roundf (min_x);
	  ದ್ವಿಗುಣ y_bearing = (ದ್ವಿಗುಣ) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) roundf (min_y) - y_bearing);

	  ಯದಿ (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      ರಕ್ಷಿತ:
      ಭಿನ್ನ min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, ತಾರ್ಕಿಕ scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      ಯದಿ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    ಶೂನ್ಯ consume_point (ಧ್ರುವ contour_point_t &point) { bounds.add (point); }
    ಶೂನ್ಯ contour_end () {}
    ಶೂನ್ಯ points_end () { bounds.get_extents (font, extents, scaled); }

    ತಾರ್ಕಿಕ is_consuming_contour_points () { ಪ್ರತ್ಯಾಗಮ extents; }
    contour_point_t *get_phantoms_sink () { ಪ್ರತ್ಯಾಗಮ phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 ತಾರ್ಕಿಕ is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (gid >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ 0;

    ತಾರ್ಕಿಕ success = ಅಸತ್ಯ;

    contour_point_t phantoms[glyf_impl::PHANTOM_ಎಣಿಕೆ];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ಅಸತ್ಯ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0),
			  scratch, gvar_cache);
    ಯದಿ (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ಪ್ರತ್ಯಾಗಮ is_vertical ? upem : upem / 2;
    }

    ಭಿನ್ನ result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_ತುದಿ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ಬಲ].x - phantoms[glyf_impl::PHANTOM_ಎಡ].x;
    ಪ್ರತ್ಯಾಗಮ hb_clamp (roundf (result), 0.f, (ಭಿನ್ನ) UINT_MAX / 2);
  }
#endif

  ಭಿನ್ನ
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (gid >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ 0;

    ತಾರ್ಕಿಕ success = ಅಸತ್ಯ;

    contour_point_t phantoms[glyf_impl::PHANTOM_ಎಣಿಕೆ];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ಅಸತ್ಯ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0),
			  scratch, gvar_cache);
    ಯದಿ (unlikely (!success))
    {
      ಪ್ರತ್ಯಾಗಮ font->face->get_upem ();
    }

    ಪ್ರತ್ಯಾಗಮ phantoms[glyf_impl::PHANTOM_ತುದಿ].y;
  }

  ಸಾರ್ವಜನಿಕ:

  ತಾರ್ಕಿಕ get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0)); }

  ತಾರ್ಕಿಕ get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
		       int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (gid >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

#ifndef HB_NO_VAR
    ಯದಿ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ತಾರ್ಕಿಕ ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, ಸತ್ಯ),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ಪ್ರತ್ಯಾಗಮ ret;
    }
#endif
    ಪ್ರತ್ಯಾಗಮ glyph_for_gid (gid).get_extents_without_var_scaled (font, *ಏತತ್, extents);
  }

  ಧ್ರುವ glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, ತಾರ್ಕಿಕ needs_padding_removal = ಅಸತ್ಯ) ಧ್ರುವ
  {
    ಯದಿ (unlikely (gid >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ glyf_impl::Glyph ();

    unsigned ಪೂರ್ಣಾಙ್ಕ start_offset, end_offset;

    ಯದಿ (short_offset)
    {
      ಧ್ರುವ HBUINT16 *offsets = (ಧ್ರುವ HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    ಅನ್ಯಥಾ
    {
      ಧ್ರುವ HBUINT32 *offsets = (ಧ್ರುವ HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    ಯದಿ (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      ಪ್ರತ್ಯಾಗಮ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *) ಏತತ್->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ಪ್ರತ್ಯಾಗಮ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  ತಾರ್ಕಿಕ
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ತಾರ್ಕಿಕ ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ತಾರ್ಕಿಕ
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ಪ್ರತ್ಯಾಗಮ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ಯದಿ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_glyf_scratch_t));
      ಯದಿ (unlikely (!scratch))
	ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ scratch;
  }
  ಶೂನ್ಯ release_scratch (hb_glyf_scratch_t *scratch) ಧ್ರುವ
  {
    ಯದಿ (!scratch)
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_glyphs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_glyphs; }
  hb_blob_t *reference_glyf_table () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ಧ್ರುವ gvar_accelerator_t *gvar;
#endif
  ಧ್ರುವ hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ಧ್ರುವ vmtx_accelerator_t *vmtx;
#endif

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ extended;
  ತಾರ್ಕಿಕ short_offset;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
glyf::_populate_subset_glyphs (ಧ್ರುವ hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ಧ್ರುವ
{
  OT::glyf_accelerator_t glyf (plan->source);
  ಯದಿ (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  *source_glyf = glyf.reference_glyf_table ();

  ಪ್ರತಿ (ಧ್ರುವ auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ಯದಿ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    ಅನ್ಯಥಾ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ಯದಿ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    ಅನ್ಯಥಾ
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    ಯದಿ (font)
    {
      ಯದಿ (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ಯದಿ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಅನ್ತಃಸ್ಥ hb_font_t *
glyf::_create_font_for_instancing (ಧ್ರುವ hb_subset_plan_t *plan) ಧ್ರುವ
{
  hb_font_t *font = hb_font_create (plan->source);
  ಯದಿ (unlikely (font == hb_font_get_empty ())) ಪ್ರತ್ಯಾಗಮ nullptr;

#ifndef HB_NO_VAR
  ಯದಿ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    ಪ್ರತ್ಯಾಗಮ font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  ಯದಿ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), ಸತ್ಯ)))
  {
    hb_font_destroy (font);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತಿ (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.ಮಧ್ಯ;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ಪ್ರತ್ಯಾಗಮ font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
