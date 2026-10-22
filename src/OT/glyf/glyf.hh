#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#অন্তর্ভাৱ "../../hb-open-type.hh"
#অন্তর্ভাৱ "../../hb-depend-data.hh"
#অন্তর্ভাৱ "../../hb-ot-head-table.hh"
#অন্তর্ভাৱ "../../hb-ot-hmtx-table.hh"
#অন্তর্ভাৱ "../../hb-ot-var-gvar-table.hh"
#অন্তর্ভাৱ "../../hb-draw.hh"
#অন্তর্ভাৱ "../../hb-paint.hh"

#অন্তর্ভাৱ "glyf-helpers.hh"
#অন্তর্ভাৱ "Glyph.hh"
#অন্তর্ভাৱ "SubsetGlyph.hh"
#অন্তর্ভাৱ "loca.hh"
#অন্তর্ভাৱ "path-builder.hh"


নামাকাশ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

সংরচনা glyf
{
  friend সংরচনা glyf_accelerator_t;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  স্থির তার্কিক has_valid_glyf_format(ধ্রুৱ hb_face_t* face)
  {
    ধ্রুৱ OT::head &head = *face->table.head;
    ঘূৰাই head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  স্থির তার্কিক has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    তার্কিক ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ঘূৰাই ret;
#অন্যথা
    ঘূৰাই মিছা;
#endif
  }

  তার্কিক sanitize (hb_sanitize_context_t *c HB_UNUSED) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (সঁচা);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it,
                  তার্কিক use_short_loca,
		  ধ্রুৱ hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (এতত্);

    unsigned init_len = c->length ();
    বাবে (auto &_ : it)
      যদি (unlikely (!_.serialize (c, use_short_loca, plan)))
        ঘূৰাই মিছা;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    যদি (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (সঁচা);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    তার্কিক extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    যদি (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (মিছা);
#endif

    যদি (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_বাৰ্তা (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (মিছা);
    }

    hb_font_t *font = nullptr;
    যদি (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      যদি (unlikely (!font))
	return_trace (মিছা);
    }

    hb_vector_t<unsigned> padded_offsets;
    যদি (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (মিছা);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    যদি (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (মিছা);
    }

    যদি (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    বাবে (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    তার্কিক use_short_loca = মিছা;
    যদি (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    যদি (!use_short_loca)
    {
      padded_offsets.clear ();
      বাবে (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_মৌলিক = c->serializer->start_embed <glyf> ();
    তার্কিক result = glyf_মৌলিক->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    যদি (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    যদি (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (মিছা);

    ঘূৰাই result;
  }

  তার্কিক
  _populate_subset_glyphs (ধ্রুৱ hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ধ্রুৱ;

  hb_font_t *
  _create_font_for_instancing (ধ্রুৱ hb_subset_plan_t *plan) ধ্রুৱ;

  শূন্য _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ধ্রুৱ
  {
    বাবে (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  রক্ষিত:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
সংরচনা GLYF : glyf
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

সংরচনা glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = মিছা;
    short_offset = মিছা;
    অঙ্ক_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    ধ্রুৱ OT::head &head = *face->table.head;
    যদি (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ঘূৰাই;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    যদি (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    অন্যথা
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    যদি (!extended)
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

    অঙ্ক_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    অঙ্ক_glyphs = hb_min (অঙ্ক_glyphs, face->get_অঙ্ক_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    যদি (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই অঙ্ক_glyphs; }
  তার্কিক is_extended () ধ্রুৱ { ঘূৰাই extended; }

  শূন্য depend (hb_depend_data_builder_t *builder) ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই;
    বাবে (hb_codepoint_t gid = 0; gid < get_অঙ্ক_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      বাবে (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  রক্ষিত:
  template<typename T>
  তার্কিক get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ধ্রুৱ
  {
    যদি (gid >= অঙ্ক_glyphs) ঘূৰাই মিছা;

    int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
    যদি (!budget) budget = &স্তূপ_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    তার্কিক phantom_only = !consumer.is_consuming_contour_points ();
    যদি (unlikely (!glyph_for_gid (gid).get_points (font, *এতত্, all_points, scratch, nullptr, nullptr, nullptr, সঁচা, সঁচা, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ঘূৰাই মিছা;

    যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      ঘূৰাই মিছা;

    unsigned গুনতি = all_points.length;
    প্রতিজ্ঞা (গুনতি >= glyf_impl::PHANTOM_গুনতি);
    গুনতি -= glyf_impl::PHANTOM_গুনতি;

    যদি (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      যদি (মিছা)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	বাবে (unsigned i = 0; i < গুনতি; i++)
	{
	  consumer.consume_point (points[i]);
	  যদি (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      অন্যথা
      {
	বাবে (unsigned i = 0; i < গুনতি; i++)
	{
	  // Start of a contour.
	  যদি (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    বাবে (; i < গুনতি; i++)
	    {
	      consumer.consume_point (points[i]);
	      যদি (points[i].is_end_point)
	      {
		consumer.contour_end ();
		ভঙ্গ;
	      }
	    }
	  }
	  অন্যথা
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    বাবে (; i < গুনতি; i++)
	      যদি (points[i].is_end_point)
		ভঙ্গ;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    যদি (likely (end < গুনতি) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    অন্যথা
#endif
	    যদি (likely (end < গুনতি))
	      consumer.consume_point (points[end]);

	    বাবে (i = start; i < limit; i++)
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
    যদি (phantoms)
      বাবে (unsigned i = 0; i < glyf_impl::PHANTOM_গুনতি; ++i)
	phantoms[i] = all_points.arrayZ[গুনতি + i];

    ঘূৰাই সঁচা;
  }

  সার্ৱজনিক:

  সংরচনা points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    তার্কিক scaled;

    সংরচনা contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      শূন্য add (ধ্রুৱ contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      তার্কিক empty () ধ্রুৱ { ঘূৰাই (min_x >= max_x) || (min_y >= max_y); }

      শূন্য get_extents (hb_font_t *font, hb_glyph_extents_t *extents, তার্কিক scaled)
      {
	যদি (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ঘূৰাই;
	}
	{
	  দ্ৱিগুণ x_bearing = (দ্ৱিগুণ) roundf (min_x);
	  দ্ৱিগুণ y_bearing = (দ্ৱিগুণ) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((দ্ৱিগুণ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((দ্ৱিগুণ) roundf (min_y) - y_bearing);

	  যদি (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      রক্ষিত:
      ভিন্ন min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, তার্কিক scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      যদি (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    শূন্য consume_point (ধ্রুৱ contour_point_t &point) { bounds.add (point); }
    শূন্য contour_end () {}
    শূন্য points_end () { bounds.get_extents (font, extents, scaled); }

    তার্কিক is_consuming_contour_points () { ঘূৰাই extents; }
    contour_point_t *get_phantoms_sink () { ঘূৰাই phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 তার্কিক is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ঘূৰাই 0;

    তার্কিক success = মিছা;

    contour_point_t phantoms[glyf_impl::PHANTOM_গুনতি];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, মিছা),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			  scratch, gvar_cache);
    যদি (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ঘূৰাই is_vertical ? upem : upem / 2;
    }

    ভিন্ন result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_শীৰ্ষ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ডান].x - phantoms[glyf_impl::PHANTOM_বাম].x;
    ঘূৰাই hb_clamp (roundf (result), 0.f, (ভিন্ন) UINT_MAX / 2);
  }
#endif

  ভিন্ন
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ঘূৰাই 0;

    তার্কিক success = মিছা;

    contour_point_t phantoms[glyf_impl::PHANTOM_গুনতি];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, মিছা),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			  scratch, gvar_cache);
    যদি (unlikely (!success))
    {
      ঘূৰাই font->face->get_upem ();
    }

    ঘূৰাই phantoms[glyf_impl::PHANTOM_শীৰ্ষ].y;
  }

  সার্ৱজনিক:

  তার্কিক get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ধ্রুৱ
  { ঘূৰাই get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->অঙ্ক_coords : 0)); }

  তার্কিক get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
		       int64_t *budget = nullptr) ধ্রুৱ
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ঘূৰাই মিছা;

#ifndef HB_NO_VAR
    যদি (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ঘূৰাই মিছা;
      তার্কিক ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, সঁচা),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ঘূৰাই ret;
    }
#endif
    ঘূৰাই glyph_for_gid (gid).get_extents_without_var_scaled (font, *এতত্, extents);
  }

  ধ্রুৱ glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, তার্কিক needs_padding_removal = মিছা) ধ্রুৱ
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ঘূৰাই glyf_impl::Glyph ();

    unsigned পূর্ণাঙ্ক start_offset, end_offset;

    যদি (short_offset)
    {
      ধ্রুৱ HBUINT16 *offsets = (ধ্রুৱ HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    অন্যথা
    {
      ধ্রুৱ HBUINT32 *offsets = (ধ্রুৱ HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    যদি (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      ঘূৰাই glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ধ্রুৱ ৱর্ণ *) এতত্->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ঘূৰাই needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  তার্কিক
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই মিছা;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    যদি (unlikely (!scratch)) ঘূৰাই সঁচা;

    তার্কিক ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ঘূৰাই ret;
  }

  তার্কিক
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই মিছা;
    তার্কিক ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ঘূৰাই ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, পরিমাণ (hb_glyf_scratch_t));
      যদি (unlikely (!scratch))
	ঘূৰাই nullptr;
    }
    ঘূৰাই scratch;
  }
  শূন্য release_scratch (hb_glyf_scratch_t *scratch) ধ্রুৱ
  {
    যদি (!scratch)
      ঘূৰাই;
    যদি (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned পূর্ণাঙ্ক get_অঙ্ক_glyphs () ধ্রুৱ { ঘূৰাই অঙ্ক_glyphs; }
  hb_blob_t *reference_glyf_table () ধ্রুৱ
  { ঘূৰাই hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ধ্রুৱ gvar_accelerator_t *gvar;
#endif
  ধ্রুৱ hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ধ্রুৱ vmtx_accelerator_t *vmtx;
#endif

  গুপ্ত:
  তার্কিক extended;
  তার্কিক short_offset;
  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


অন্তঃস্থ তার্কিক
glyf::_populate_subset_glyphs (ধ্রুৱ hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ধ্রুৱ
{
  OT::glyf_accelerator_t glyf (plan->source);
  যদি (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) ঘূৰাই মিছা;
  *source_glyf = glyf.reference_glyf_table ();

  বাবে (ধ্রুৱ auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    যদি (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    অন্যথা
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    যদি (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    অন্যথা
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    যদি (font)
    {
      যদি (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        যদি (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ঘূৰাই মিছা;
      }
    }
  }
  ঘূৰাই সঁচা;
}

অন্তঃস্থ hb_font_t *
glyf::_create_font_for_instancing (ধ্রুৱ hb_subset_plan_t *plan) ধ্রুৱ
{
  hb_font_t *font = hb_font_create (plan->source);
  যদি (unlikely (font == hb_font_get_empty ())) ঘূৰাই nullptr;

#ifndef HB_NO_VAR
  যদি (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    ঘূৰাই font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  যদি (unlikely (!vars.alloc (plan->user_axes_location.get_population (), সঁচা)))
  {
    hb_font_destroy (font);
    ঘূৰাই nullptr;
  }

  বাবে (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.মধ্য;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ঘূৰাই font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
