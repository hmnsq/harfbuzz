#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-depend-data.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-ot-head-table.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-ot-hmtx-table.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-ot-var-gvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-paint.hh"

#ଅନ୍ତର୍ଭାଵ "glyf-helpers.hh"
#ଅନ୍ତର୍ଭାଵ "Glyph.hh"
#ଅନ୍ତର୍ଭାଵ "SubsetGlyph.hh"
#ଅନ୍ତର୍ଭାଵ "loca.hh"
#ଅନ୍ତର୍ଭାଵ "path-builder.hh"


ନାମାକାଶ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

ସଂରଚନା glyf
{
  friend ସଂରଚନା glyf_accelerator_t;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  ସ୍ଥିର ତାର୍କିକ has_valid_glyf_format(ଅଚଳ hb_face_t* face)
  {
    ଅଚଳ OT::head &head = *face->table.head;
    ଫେରନ୍ତୁ head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  ସ୍ଥିର ତାର୍କିକ has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    ତାର୍କିକ ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ଫେରନ୍ତୁ ret;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c HB_UNUSED) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (ସତ୍ୟ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it,
                  ତାର୍କିକ use_short_loca,
		  ଅଚଳ hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (ଏହି);

    unsigned init_len = c->length ();
    ପାଇଁ (auto &_ : it)
      ଯଦି (unlikely (!_.serialize (c, use_short_loca, plan)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ଯଦି (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (ସତ୍ୟ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ତାର୍କିକ extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (ମିଥ୍ୟା);
#endif

    ଯଦି (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (ମିଥ୍ୟା);
    }

    hb_font_t *font = nullptr;
    ଯଦି (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ଯଦି (unlikely (!font))
	return_trace (ମିଥ୍ୟା);
    }

    hb_vector_t<unsigned> padded_offsets;
    ଯଦି (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (ମିଥ୍ୟା);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    ଯଦି (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    ପାଇଁ (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    ତାର୍କିକ use_short_loca = ମିଥ୍ୟା;
    ଯଦି (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    ଯଦି (!use_short_loca)
    {
      padded_offsets.clear ();
      ପାଇଁ (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_ମୌଳିକ = c->serializer->start_embed <glyf> ();
    ତାର୍କିକ result = glyf_ମୌଳିକ->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ଯଦି (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    ଯଦି (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (ମିଥ୍ୟା);

    ଫେରନ୍ତୁ result;
  }

  ତାର୍କିକ
  _populate_subset_glyphs (ଅଚଳ hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ଅଚଳ;

  hb_font_t *
  _create_font_for_instancing (ଅଚଳ hb_subset_plan_t *plan) ଅଚଳ;

  ଶୂନ୍ୟ _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ଅଚଳ
  {
    ପାଇଁ (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
ସଂରଚନା GLYF : glyf
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

ସଂରଚନା glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = ମିଥ୍ୟା;
    short_offset = ମିଥ୍ୟା;
    ଅଙ୍କ_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    ଅଚଳ OT::head &head = *face->table.head;
    ଯଦି (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ଫେରନ୍ତୁ;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    ଯଦି (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    ଅନ୍ୟଥା
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    ଯଦି (!extended)
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

    ଅଙ୍କ_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    ଅଙ୍କ_glyphs = hb_min (ଅଙ୍କ_glyphs, face->get_ଅଙ୍କ_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    ଯଦି (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_glyphs; }
  ତାର୍କିକ is_extended () ଅଚଳ { ଫେରନ୍ତୁ extended; }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *builder) ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ;
    ପାଇଁ (hb_codepoint_t gid = 0; gid < get_ଅଙ୍କ_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      ପାଇଁ (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  ସୁରକ୍ଷିତ:
  template<typename T>
  ତାର୍କିକ get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ଅଚଳ
  {
    ଯଦି (gid >= ଅଙ୍କ_glyphs) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    int64_t ଗଦା_budget = HB_BUDGET_GLYPH;
    ଯଦି (!budget) budget = &ଗଦା_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    ତାର୍କିକ phantom_only = !consumer.is_consuming_contour_points ();
    ଯଦି (unlikely (!glyph_for_gid (gid).get_points (font, *ଏହି, all_points, scratch, nullptr, nullptr, nullptr, ସତ୍ୟ, ସତ୍ୟ, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ଗଣନା_2 = all_points.length;
    ପ୍ରତିଜ୍ଞା (ଗଣନା_2 >= glyf_impl::PHANTOM_ଗଣନା);
    ଗଣନା_2 -= glyf_impl::PHANTOM_ଗଣନା;

    ଯଦି (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      ଯଦି (ମିଥ୍ୟା)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	{
	  consumer.consume_point (points[i]);
	  ଯଦି (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      ଅନ୍ୟଥା
      {
	ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	{
	  // Start of a contour.
	  ଯଦି (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    ପାଇଁ (; i < ଗଣନା_2; i++)
	    {
	      consumer.consume_point (points[i]);
	      ଯଦି (points[i].is_end_point)
	      {
		consumer.contour_end ();
		ଭାଙ୍ଗ;
	      }
	    }
	  }
	  ଅନ୍ୟଥା
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    ପାଇଁ (; i < ଗଣନା_2; i++)
	      ଯଦି (points[i].is_end_point)
		ଭାଙ୍ଗ;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ଯଦି (likely (end < ଗଣନା_2) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    ଅନ୍ୟଥା
#endif
	    ଯଦି (likely (end < ଗଣନା_2))
	      consumer.consume_point (points[end]);

	    ପାଇଁ (i = start; i < limit; i++)
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
    ଯଦି (phantoms)
      ପାଇଁ (unsigned i = 0; i < glyf_impl::PHANTOM_ଗଣନା; ++i)
	phantoms[i] = all_points.arrayZ[ଗଣନା_2 + i];

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସର୍ବସାଧାରଣ:

  ସଂରଚନା points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    ତାର୍କିକ scaled;

    ସଂରଚନା contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      ଶୂନ୍ୟ add (ଅଚଳ contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      ତାର୍କିକ empty () ଅଚଳ { ଫେରନ୍ତୁ (min_x >= max_x) || (min_y >= max_y); }

      ଶୂନ୍ୟ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ତାର୍କିକ scaled)
      {
	ଯଦି (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ଫେରନ୍ତୁ;
	}
	{
	  ଦ୍ୱିଗୁଣ x_bearing = (ଦ୍ୱିଗୁଣ) roundf (min_x);
	  ଦ୍ୱିଗୁଣ y_bearing = (ଦ୍ୱିଗୁଣ) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((ଦ୍ୱିଗୁଣ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((ଦ୍ୱିଗୁଣ) roundf (min_y) - y_bearing);

	  ଯଦି (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      ସୁରକ୍ଷିତ:
      ଦଶମିକ min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, ତାର୍କିକ scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      ଯଦି (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    ଶୂନ୍ୟ consume_point (ଅଚଳ contour_point_t &point) { bounds.add (point); }
    ଶୂନ୍ୟ contour_end () {}
    ଶୂନ୍ୟ points_end () { bounds.get_extents (font, extents, scaled); }

    ତାର୍କିକ is_consuming_contour_points () { ଫେରନ୍ତୁ extents; }
    contour_point_t *get_phantoms_sink () { ଫେରନ୍ତୁ phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 ତାର୍କିକ is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (gid >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ 0;

    ତାର୍କିକ success = ମିଥ୍ୟା;

    contour_point_t phantoms[glyf_impl::PHANTOM_ଗଣନା];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ମିଥ୍ୟା),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0),
			  scratch, gvar_cache);
    ଯଦି (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ଫେରନ୍ତୁ is_vertical ? upem : upem / 2;
    }

    ଦଶମିକ result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_ଶୀର୍ଷ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ଡାହାଣ].x - phantoms[glyf_impl::PHANTOM_ବାମ].x;
    ଫେରନ୍ତୁ hb_clamp (roundf (result), 0.f, (ଦଶମିକ) UINT_MAX / 2);
  }
#endif

  ଦଶମିକ
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (gid >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ 0;

    ତାର୍କିକ success = ମିଥ୍ୟା;

    contour_point_t phantoms[glyf_impl::PHANTOM_ଗଣନା];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ମିଥ୍ୟା),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0),
			  scratch, gvar_cache);
    ଯଦି (unlikely (!success))
    {
      ଫେରନ୍ତୁ font->face->get_upem ();
    }

    ଫେରନ୍ତୁ phantoms[glyf_impl::PHANTOM_ଶୀର୍ଷ].y;
  }

  ସର୍ବସାଧାରଣ:

  ତାର୍କିକ get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ଅଚଳ
  { ଫେରନ୍ତୁ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0)); }

  ତାର୍କିକ get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
		       int64_t *budget = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (gid >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

#ifndef HB_NO_VAR
    ଯଦି (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ତାର୍କିକ ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, ସତ୍ୟ),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ଫେରନ୍ତୁ ret;
    }
#endif
    ଫେରନ୍ତୁ glyph_for_gid (gid).get_extents_without_var_scaled (font, *ଏହି, extents);
  }

  ଅଚଳ glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, ତାର୍କିକ needs_padding_removal = ମିଥ୍ୟା) ଅଚଳ
  {
    ଯଦି (unlikely (gid >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ glyf_impl::Glyph ();

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset, end_offset;

    ଯଦି (short_offset)
    {
      ଅଚଳ HBUINT16 *offsets = (ଅଚଳ HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    ଅନ୍ୟଥା
    {
      ଅଚଳ HBUINT32 *offsets = (ଅଚଳ HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    ଯଦି (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      ଫେରନ୍ତୁ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *) ଏହି->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ଫେରନ୍ତୁ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  ତାର୍କିକ
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ସତ୍ୟ;

    ତାର୍କିକ ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ଫେରନ୍ତୁ ret;
  }

  ତାର୍କିକ
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ତାର୍କିକ ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ଫେରନ୍ତୁ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ଯଦି (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, ଆକାର (hb_glyf_scratch_t));
      ଯଦି (unlikely (!scratch))
	ଫେରନ୍ତୁ nullptr;
    }
    ଫେରନ୍ତୁ scratch;
  }
  ଶୂନ୍ୟ release_scratch (hb_glyf_scratch_t *scratch) ଅଚଳ
  {
    ଯଦି (!scratch)
      ଫେରନ୍ତୁ;
    ଯଦି (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_glyphs () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_glyphs; }
  hb_blob_t *reference_glyf_table () ଅଚଳ
  { ଫେରନ୍ତୁ hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ଅଚଳ gvar_accelerator_t *gvar;
#endif
  ଅଚଳ hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ଅଚଳ vmtx_accelerator_t *vmtx;
#endif

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ extended;
  ତାର୍କିକ short_offset;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


ଧାଡ଼ିଭିତରେ ତାର୍କିକ
glyf::_populate_subset_glyphs (ଅଚଳ hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ଅଚଳ
{
  OT::glyf_accelerator_t glyf (plan->source);
  ଯଦି (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  *source_glyf = glyf.reference_glyf_table ();

  ପାଇଁ (ଅଚଳ auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ଯଦି (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    ଅନ୍ୟଥା
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ଯଦି (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    ଅନ୍ୟଥା
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    ଯଦି (font)
    {
      ଯଦି (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ଯଦି (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ଧାଡ଼ିଭିତରେ hb_font_t *
glyf::_create_font_for_instancing (ଅଚଳ hb_subset_plan_t *plan) ଅଚଳ
{
  hb_font_t *font = hb_font_create (plan->source);
  ଯଦି (unlikely (font == hb_font_get_empty ())) ଫେରନ୍ତୁ nullptr;

#ifndef HB_NO_VAR
  ଯଦି (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    ଫେରନ୍ତୁ font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  ଯଦି (unlikely (!vars.alloc (plan->user_axes_location.get_population (), ସତ୍ୟ)))
  {
    hb_font_destroy (font);
    ଫେରନ୍ତୁ nullptr;
  }

  ପାଇଁ (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.ମଧ୍ୟ;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ଫେରନ୍ତୁ font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
