#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#அந்தர்பாவ "../../hb-open-type.hh"
#அந்தர்பாவ "../../hb-depend-data.hh"
#அந்தர்பாவ "../../hb-ot-head-table.hh"
#அந்தர்பாவ "../../hb-ot-hmtx-table.hh"
#அந்தர்பாவ "../../hb-ot-var-gvar-table.hh"
#அந்தர்பாவ "../../hb-draw.hh"
#அந்தர்பாவ "../../hb-paint.hh"

#அந்தர்பாவ "glyf-helpers.hh"
#அந்தர்பாவ "Glyph.hh"
#அந்தர்பாவ "SubsetGlyph.hh"
#அந்தர்பாவ "loca.hh"
#அந்தர்பாவ "path-builder.hh"


நாமாகாஶ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

கட்டமைப்பு glyf
{
  friend கட்டமைப்பு glyf_accelerator_t;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  நிலையான தருக்க has_valid_glyf_format(மாறிலி hb_face_t* face)
  {
    மாறிலி OT::head &head = *face->table.head;
    திரும்பு head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  நிலையான தருக்க has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    தருக்க ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    திரும்பு ret;
#இல்லையெனில்
    திரும்பு பொய்;
#endif
  }

  தருக்க sanitize (hb_sanitize_context_t *c HB_UNUSED) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (உண்மை);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it,
                  தருக்க use_short_loca,
		  மாறிலி hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (இது);

    unsigned init_len = c->length ();
    க்கு (auto &_ : it)
      எனில் (unlikely (!_.serialize (c, use_short_loca, plan)))
        திரும்பு பொய்;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    எனில் (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (உண்மை);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    தருக்க extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    எனில் (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (பொய்);
#endif

    எனில் (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_செய்தி (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (பொய்);
    }

    hb_font_t *font = nullptr;
    எனில் (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      எனில் (unlikely (!font))
	return_trace (பொய்);
    }

    hb_vector_t<unsigned> padded_offsets;
    எனில் (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (பொய்);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    எனில் (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (பொய்);
    }

    எனில் (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    க்கு (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    தருக்க use_short_loca = பொய்;
    எனில் (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    எனில் (!use_short_loca)
    {
      padded_offsets.clear ();
      க்கு (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_பகாஎண் = c->serializer->start_embed <glyf> ();
    தருக்க result = glyf_பகாஎண்->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    எனில் (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    எனில் (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (பொய்);

    திரும்பு result;
  }

  தருக்க
  _populate_subset_glyphs (மாறிலி hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) மாறிலி;

  hb_font_t *
  _create_font_for_instancing (மாறிலி hb_subset_plan_t *plan) மாறிலி;

  வெறுமை _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) மாறிலி
  {
    க்கு (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  பொது:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
கட்டமைப்பு GLYF : glyf
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

கட்டமைப்பு glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = பொய்;
    short_offset = பொய்;
    இலக்கம்_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    மாறிலி OT::head &head = *face->table.head;
    எனில் (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      திரும்பு;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    எனில் (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    இல்லையெனில்
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    எனில் (!extended)
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

    இலக்கம்_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    இலக்கம்_glyphs = hb_min (இலக்கம்_glyphs, face->get_இலக்கம்_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    எனில் (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  தருக்க has_data () மாறிலி { திரும்பு இலக்கம்_glyphs; }
  தருக்க is_extended () மாறிலி { திரும்பு extended; }

  வெறுமை depend (hb_depend_data_builder_t *builder) மாறிலி
  {
    எனில் (!has_data ()) திரும்பு;
    க்கு (hb_codepoint_t gid = 0; gid < get_இலக்கம்_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      க்கு (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  பாதுகாக்கப்பட்ட:
  template<typename T>
  தருக்க get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<மாறிலி முழு> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) மாறிலி
  {
    எனில் (gid >= இலக்கம்_glyphs) திரும்பு பொய்;

    int64_t அடுக்கு_budget = HB_BUDGET_GLYPH;
    எனில் (!budget) budget = &அடுக்கு_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    தருக்க phantom_only = !consumer.is_consuming_contour_points ();
    எனில் (unlikely (!glyph_for_gid (gid).get_points (font, *இது, all_points, scratch, nullptr, nullptr, nullptr, உண்மை, உண்மை, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      திரும்பு பொய்;

    எனில் (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      திரும்பு பொய்;

    unsigned மொத்தம் = all_points.length;
    ப்ரதிஜ்ஞா (மொத்தம் >= glyf_impl::PHANTOM_மொத்தம்);
    மொத்தம் -= glyf_impl::PHANTOM_மொத்தம்;

    எனில் (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      எனில் (பொய்)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	க்கு (unsigned i = 0; i < மொத்தம்; i++)
	{
	  consumer.consume_point (points[i]);
	  எனில் (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      இல்லையெனில்
      {
	க்கு (unsigned i = 0; i < மொத்தம்; i++)
	{
	  // Start of a contour.
	  எனில் (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    க்கு (; i < மொத்தம்; i++)
	    {
	      consumer.consume_point (points[i]);
	      எனில் (points[i].is_end_point)
	      {
		consumer.contour_end ();
		நிறுத்து;
	      }
	    }
	  }
	  இல்லையெனில்
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    க்கு (; i < மொத்தம்; i++)
	      எனில் (points[i].is_end_point)
		நிறுத்து;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    எனில் (likely (end < மொத்தம்) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    இல்லையெனில்
#endif
	    எனில் (likely (end < மொத்தம்))
	      consumer.consume_point (points[end]);

	    க்கு (i = start; i < limit; i++)
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
    எனில் (phantoms)
      க்கு (unsigned i = 0; i < glyf_impl::PHANTOM_மொத்தம்; ++i)
	phantoms[i] = all_points.arrayZ[மொத்தம் + i];

    திரும்பு உண்மை;
  }

  பொது:

  கட்டமைப்பு points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    தருக்க scaled;

    கட்டமைப்பு contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      வெறுமை add (மாறிலி contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      தருக்க empty () மாறிலி { திரும்பு (min_x >= max_x) || (min_y >= max_y); }

      வெறுமை get_extents (hb_font_t *font, hb_glyph_extents_t *extents, தருக்க scaled)
      {
	எனில் (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  திரும்பு;
	}
	{
	  இரட்டை x_bearing = (இரட்டை) roundf (min_x);
	  இரட்டை y_bearing = (இரட்டை) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((இரட்டை) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((இரட்டை) roundf (min_y) - y_bearing);

	  எனில் (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      பாதுகாக்கப்பட்ட:
      தசம min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, தருக்க scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      எனில் (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    வெறுமை consume_point (மாறிலி contour_point_t &point) { bounds.add (point); }
    வெறுமை contour_end () {}
    வெறுமை points_end () { bounds.get_extents (font, extents, scaled); }

    தருக்க is_consuming_contour_points () { திரும்பு extents; }
    contour_point_t *get_phantoms_sink () { திரும்பு phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 தருக்க is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) மாறிலி
  {
    எனில் (unlikely (gid >= இலக்கம்_glyphs)) திரும்பு 0;

    தருக்க success = பொய்;

    contour_point_t phantoms[glyf_impl::PHANTOM_மொத்தம்];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, பொய்),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->இலக்கம்_coords : 0),
			  scratch, gvar_cache);
    எனில் (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      திரும்பு is_vertical ? upem : upem / 2;
    }

    தசம result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_உச்சி].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_வலது].x - phantoms[glyf_impl::PHANTOM_இடது].x;
    திரும்பு hb_clamp (roundf (result), 0.f, (தசம) UINT_MAX / 2);
  }
#endif

  தசம
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) மாறிலி
  {
    எனில் (unlikely (gid >= இலக்கம்_glyphs)) திரும்பு 0;

    தருக்க success = பொய்;

    contour_point_t phantoms[glyf_impl::PHANTOM_மொத்தம்];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, பொய்),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->இலக்கம்_coords : 0),
			  scratch, gvar_cache);
    எனில் (unlikely (!success))
    {
      திரும்பு font->face->get_upem ();
    }

    திரும்பு phantoms[glyf_impl::PHANTOM_உச்சி].y;
  }

  பொது:

  தருக்க get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) மாறிலி
  { திரும்பு get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->இலக்கம்_coords : 0)); }

  தருக்க get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<மாறிலி முழு> coords,
		       int64_t *budget = nullptr) மாறிலி
  {
    எனில் (unlikely (gid >= இலக்கம்_glyphs)) திரும்பு பொய்;

#ifndef HB_NO_VAR
    எனில் (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      எனில் (unlikely (!scratch)) திரும்பு பொய்;
      தருக்க ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, உண்மை),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      திரும்பு ret;
    }
#endif
    திரும்பு glyph_for_gid (gid).get_extents_without_var_scaled (font, *இது, extents);
  }

  மாறிலி glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, தருக்க needs_padding_removal = பொய்) மாறிலி
  {
    எனில் (unlikely (gid >= இலக்கம்_glyphs)) திரும்பு glyf_impl::Glyph ();

    unsigned முழு start_offset, end_offset;

    எனில் (short_offset)
    {
      மாறிலி HBUINT16 *offsets = (மாறிலி HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    இல்லையெனில்
    {
      மாறிலி HBUINT32 *offsets = (மாறிலி HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    எனில் (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      திரும்பு glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((மாறிலி எழுத்து *) இது->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    திரும்பு needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  தருக்க
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) மாறிலி
  {
    எனில் (!has_data ()) திரும்பு பொய்;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    எனில் (unlikely (!scratch)) திரும்பு உண்மை;

    தருக்க ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->இலக்கம்_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    திரும்பு ret;
  }

  தருக்க
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<மாறிலி முழு> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) மாறிலி
  {
    எனில் (!has_data ()) திரும்பு பொய்;
    தருக்க ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    திரும்பு ret;
  }


  hb_glyf_scratch_t *acquire_scratch () மாறிலி
  {
    எனில் (!has_data ()) திரும்பு nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    எனில் (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, அளவு (hb_glyf_scratch_t));
      எனில் (unlikely (!scratch))
	திரும்பு nullptr;
    }
    திரும்பு scratch;
  }
  வெறுமை release_scratch (hb_glyf_scratch_t *scratch) மாறிலி
  {
    எனில் (!scratch)
      திரும்பு;
    எனில் (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned முழு get_இலக்கம்_glyphs () மாறிலி { திரும்பு இலக்கம்_glyphs; }
  hb_blob_t *reference_glyf_table () மாறிலி
  { திரும்பு hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  மாறிலி gvar_accelerator_t *gvar;
#endif
  மாறிலி hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  மாறிலி vmtx_accelerator_t *vmtx;
#endif

  தனிப்பட்ட:
  தருக்க extended;
  தருக்க short_offset;
  unsigned முழு இலக்கம்_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


உள்வரி தருக்க
glyf::_populate_subset_glyphs (மாறிலி hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) மாறிலி
{
  OT::glyf_accelerator_t glyf (plan->source);
  எனில் (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) திரும்பு பொய்;
  *source_glyf = glyf.reference_glyf_table ();

  க்கு (மாறிலி auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    எனில் (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    இல்லையெனில்
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    எனில் (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    இல்லையெனில்
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    எனில் (font)
    {
      எனில் (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        எனில் (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        திரும்பு பொய்;
      }
    }
  }
  திரும்பு உண்மை;
}

உள்வரி hb_font_t *
glyf::_create_font_for_instancing (மாறிலி hb_subset_plan_t *plan) மாறிலி
{
  hb_font_t *font = hb_font_create (plan->source);
  எனில் (unlikely (font == hb_font_get_empty ())) திரும்பு nullptr;

#ifndef HB_NO_VAR
  எனில் (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    திரும்பு font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  எனில் (unlikely (!vars.alloc (plan->user_axes_location.get_population (), உண்மை)))
  {
    hb_font_destroy (font);
    திரும்பு nullptr;
  }

  க்கு (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.நடு;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  திரும்பு font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
