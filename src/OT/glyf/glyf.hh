#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "../../hb-depend-data.hh"
#अन्तर्भाव "../../hb-ot-head-table.hh"
#अन्तर्भाव "../../hb-ot-hmtx-table.hh"
#अन्तर्भाव "../../hb-ot-var-gvar-table.hh"
#अन्तर्भाव "../../hb-draw.hh"
#अन्तर्भाव "../../hb-paint.hh"

#अन्तर्भाव "glyf-helpers.hh"
#अन्तर्भाव "Glyph.hh"
#अन्तर्भाव "SubsetGlyph.hh"
#अन्तर्भाव "loca.hh"
#अन्तर्भाव "path-builder.hh"


नामाकाश OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

संरचना glyf
{
  friend संरचना glyf_accelerator_t;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  स्थिर तार्किक has_valid_glyf_format(ध्रुव hb_face_t* face)
  {
    ध्रुव OT::head &head = *face->table.head;
    पटिनिवत्त head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  स्थिर तार्किक has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    तार्किक ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    पटिनिवत्त ret;
#अञ्ञथा
    पटिनिवत्त मिच्छा;
#endif
  }

  तार्किक sanitize (hb_sanitize_context_t *c HB_UNUSED) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (सच्च);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
                  तार्किक use_short_loca,
		  ध्रुव hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (एतत्);

    unsigned init_len = c->length ();
    अत्थाय (auto &_ : it)
      सचे (unlikely (!_.serialize (c, use_short_loca, plan)))
        पटिनिवत्त मिच्छा;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    सचे (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (सच्च);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    तार्किक extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    सचे (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (मिच्छा);
#endif

    सचे (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_सन्देशः (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (मिच्छा);
    }

    hb_font_t *font = nullptr;
    सचे (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      सचे (unlikely (!font))
	return_trace (मिच्छा);
    }

    hb_vector_t<unsigned> padded_offsets;
    सचे (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (मिच्छा);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    सचे (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (मिच्छा);
    }

    सचे (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    अत्थाय (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    तार्किक use_short_loca = मिच्छा;
    सचे (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    सचे (!use_short_loca)
    {
      padded_offsets.clear ();
      अत्थाय (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_अविभाज्यः = c->serializer->start_embed <glyf> ();
    तार्किक result = glyf_अविभाज्यः->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    सचे (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    सचे (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (मिच्छा);

    पटिनिवत्त result;
  }

  तार्किक
  _populate_subset_glyphs (ध्रुव hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ध्रुव;

  hb_font_t *
  _create_font_for_instancing (ध्रुव hb_subset_plan_t *plan) ध्रुव;

  शून्य _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ध्रुव
  {
    अत्थाय (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  रक्षित:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
संरचना GLYF : glyf
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

संरचना glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = मिच्छा;
    short_offset = मिच्छा;
    अङ्कः_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    ध्रुव OT::head &head = *face->table.head;
    सचे (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      पटिनिवत्त;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    सचे (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    अञ्ञथा
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    सचे (!extended)
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

    अङ्कः_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    अङ्कः_glyphs = hb_min (अङ्कः_glyphs, face->get_अङ्कः_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    सचे (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  तार्किक has_data () ध्रुव { पटिनिवत्त अङ्कः_glyphs; }
  तार्किक is_extended () ध्रुव { पटिनिवत्त extended; }

  शून्य depend (hb_depend_data_builder_t *builder) ध्रुव
  {
    सचे (!has_data ()) पटिनिवत्त;
    अत्थाय (hb_codepoint_t gid = 0; gid < get_अङ्कः_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      अत्थाय (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  रक्षित:
  template<typename T>
  तार्किक get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ध्रुव पूर्णाङ्क> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ध्रुव
  {
    सचे (gid >= अङ्कः_glyphs) पटिनिवत्त मिच्छा;

    int64_t राशिः_budget = HB_BUDGET_GLYPH;
    सचे (!budget) budget = &राशिः_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    तार्किक phantom_only = !consumer.is_consuming_contour_points ();
    सचे (unlikely (!glyph_for_gid (gid).get_points (font, *एतत्, all_points, scratch, nullptr, nullptr, nullptr, सच्च, सच्च, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      पटिनिवत्त मिच्छा;

    सचे (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      पटिनिवत्त मिच्छा;

    unsigned गणनम् = all_points.length;
    प्रतिज्ञा (गणनम् >= glyf_impl::PHANTOM_गणनम्);
    गणनम् -= glyf_impl::PHANTOM_गणनम्;

    सचे (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      सचे (मिच्छा)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	अत्थाय (unsigned i = 0; i < गणनम्; i++)
	{
	  consumer.consume_point (points[i]);
	  सचे (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      अञ्ञथा
      {
	अत्थाय (unsigned i = 0; i < गणनम्; i++)
	{
	  // Start of a contour.
	  सचे (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    अत्थाय (; i < गणनम्; i++)
	    {
	      consumer.consume_point (points[i]);
	      सचे (points[i].is_end_point)
	      {
		consumer.contour_end ();
		भङ्ग;
	      }
	    }
	  }
	  अञ्ञथा
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    अत्थाय (; i < गणनम्; i++)
	      सचे (points[i].is_end_point)
		भङ्ग;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    सचे (likely (end < गणनम्) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    अञ्ञथा
#endif
	    सचे (likely (end < गणनम्))
	      consumer.consume_point (points[end]);

	    अत्थाय (i = start; i < limit; i++)
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
    सचे (phantoms)
      अत्थाय (unsigned i = 0; i < glyf_impl::PHANTOM_गणनम्; ++i)
	phantoms[i] = all_points.arrayZ[गणनम् + i];

    पटिनिवत्त सच्च;
  }

  सार्वजनिक:

  संरचना points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    तार्किक scaled;

    संरचना contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      शून्य add (ध्रुव contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      तार्किक empty () ध्रुव { पटिनिवत्त (min_x >= max_x) || (min_y >= max_y); }

      शून्य get_extents (hb_font_t *font, hb_glyph_extents_t *extents, तार्किक scaled)
      {
	सचे (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  पटिनिवत्त;
	}
	{
	  द्विगुण x_bearing = (द्विगुण) roundf (min_x);
	  द्विगुण y_bearing = (द्विगुण) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((द्विगुण) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((द्विगुण) roundf (min_y) - y_bearing);

	  सचे (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      रक्षित:
      भिन्न min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, तार्किक scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      सचे (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    शून्य consume_point (ध्रुव contour_point_t &point) { bounds.add (point); }
    शून्य contour_end () {}
    शून्य points_end () { bounds.get_extents (font, extents, scaled); }

    तार्किक is_consuming_contour_points () { पटिनिवत्त extents; }
    contour_point_t *get_phantoms_sink () { पटिनिवत्त phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 तार्किक is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ध्रुव
  {
    सचे (unlikely (gid >= अङ्कः_glyphs)) पटिनिवत्त 0;

    तार्किक success = मिच्छा;

    contour_point_t phantoms[glyf_impl::PHANTOM_गणनम्];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, मिच्छा),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->अङ्कः_coords : 0),
			  scratch, gvar_cache);
    सचे (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      पटिनिवत्त is_vertical ? upem : upem / 2;
    }

    भिन्न result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_शिखरम्].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_दक्षिणः].x - phantoms[glyf_impl::PHANTOM_वामः].x;
    पटिनिवत्त hb_clamp (roundf (result), 0.f, (भिन्न) UINT_MAX / 2);
  }
#endif

  भिन्न
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ध्रुव
  {
    सचे (unlikely (gid >= अङ्कः_glyphs)) पटिनिवत्त 0;

    तार्किक success = मिच्छा;

    contour_point_t phantoms[glyf_impl::PHANTOM_गणनम्];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, मिच्छा),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->अङ्कः_coords : 0),
			  scratch, gvar_cache);
    सचे (unlikely (!success))
    {
      पटिनिवत्त font->face->get_upem ();
    }

    पटिनिवत्त phantoms[glyf_impl::PHANTOM_शिखरम्].y;
  }

  सार्वजनिक:

  तार्किक get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ध्रुव
  { पटिनिवत्त get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->अङ्कः_coords : 0)); }

  तार्किक get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ध्रुव पूर्णाङ्क> coords,
		       int64_t *budget = nullptr) ध्रुव
  {
    सचे (unlikely (gid >= अङ्कः_glyphs)) पटिनिवत्त मिच्छा;

#ifndef HB_NO_VAR
    सचे (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      सचे (unlikely (!scratch)) पटिनिवत्त मिच्छा;
      तार्किक ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, सच्च),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      पटिनिवत्त ret;
    }
#endif
    पटिनिवत्त glyph_for_gid (gid).get_extents_without_var_scaled (font, *एतत्, extents);
  }

  ध्रुव glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, तार्किक needs_padding_removal = मिच्छा) ध्रुव
  {
    सचे (unlikely (gid >= अङ्कः_glyphs)) पटिनिवत्त glyf_impl::Glyph ();

    unsigned पूर्णाङ्क start_offset, end_offset;

    सचे (short_offset)
    {
      ध्रुव HBUINT16 *offsets = (ध्रुव HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    अञ्ञथा
    {
      ध्रुव HBUINT32 *offsets = (ध्रुव HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    सचे (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      पटिनिवत्त glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ध्रुव वर्ण *) एतत्->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    पटिनिवत्त needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  तार्किक
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ध्रुव
  {
    सचे (!has_data ()) पटिनिवत्त मिच्छा;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    सचे (unlikely (!scratch)) पटिनिवत्त सच्च;

    तार्किक ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->अङ्कः_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    पटिनिवत्त ret;
  }

  तार्किक
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ध्रुव पूर्णाङ्क> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ध्रुव
  {
    सचे (!has_data ()) पटिनिवत्त मिच्छा;
    तार्किक ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    पटिनिवत्त ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ध्रुव
  {
    सचे (!has_data ()) पटिनिवत्त nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    सचे (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, परिमाण (hb_glyf_scratch_t));
      सचे (unlikely (!scratch))
	पटिनिवत्त nullptr;
    }
    पटिनिवत्त scratch;
  }
  शून्य release_scratch (hb_glyf_scratch_t *scratch) ध्रुव
  {
    सचे (!scratch)
      पटिनिवत्त;
    सचे (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned पूर्णाङ्क get_अङ्कः_glyphs () ध्रुव { पटिनिवत्त अङ्कः_glyphs; }
  hb_blob_t *reference_glyf_table () ध्रुव
  { पटिनिवत्त hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ध्रुव gvar_accelerator_t *gvar;
#endif
  ध्रुव hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ध्रुव vmtx_accelerator_t *vmtx;
#endif

  गुप्त:
  तार्किक extended;
  तार्किक short_offset;
  unsigned पूर्णाङ्क अङ्कः_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


अन्तःस्थ तार्किक
glyf::_populate_subset_glyphs (ध्रुव hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ध्रुव
{
  OT::glyf_accelerator_t glyf (plan->source);
  सचे (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) पटिनिवत्त मिच्छा;
  *source_glyf = glyf.reference_glyf_table ();

  अत्थाय (ध्रुव auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    सचे (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    अञ्ञथा
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    सचे (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    अञ्ञथा
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    सचे (font)
    {
      सचे (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        सचे (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        पटिनिवत्त मिच्छा;
      }
    }
  }
  पटिनिवत्त सच्च;
}

अन्तःस्थ hb_font_t *
glyf::_create_font_for_instancing (ध्रुव hb_subset_plan_t *plan) ध्रुव
{
  hb_font_t *font = hb_font_create (plan->source);
  सचे (unlikely (font == hb_font_get_empty ())) पटिनिवत्त nullptr;

#ifndef HB_NO_VAR
  सचे (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    पटिनिवत्त font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  सचे (unlikely (!vars.alloc (plan->user_axes_location.get_population (), सच्च)))
  {
    hb_font_destroy (font);
    पटिनिवत्त nullptr;
  }

  अत्थाय (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.मध्यः;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  पटिनिवत्त font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
