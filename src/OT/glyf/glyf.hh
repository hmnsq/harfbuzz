#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"
#അന്തര്ഭാവ "../../hb-depend-data.hh"
#അന്തര്ഭാവ "../../hb-ot-head-table.hh"
#അന്തര്ഭാവ "../../hb-ot-hmtx-table.hh"
#അന്തര്ഭാവ "../../hb-ot-var-gvar-table.hh"
#അന്തര്ഭാവ "../../hb-draw.hh"
#അന്തര്ഭാവ "../../hb-paint.hh"

#അന്തര്ഭാവ "glyf-helpers.hh"
#അന്തര്ഭാവ "Glyph.hh"
#അന്തര്ഭാവ "SubsetGlyph.hh"
#അന്തര്ഭാവ "loca.hh"
#അന്തര്ഭാവ "path-builder.hh"


നാമാകാശ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

ഘടന glyf
{
  friend ഘടന glyf_accelerator_t;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  സ്ഥിതം തർക്കം has_valid_glyf_format(സ്ഥിരം hb_face_t* face)
  {
    സ്ഥിരം OT::head &head = *face->table.head;
    തിരികെ head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  സ്ഥിതം തർക്കം has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    തർക്കം ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    തിരികെ ret;
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
  }

  തർക്കം sanitize (hb_sanitize_context_t *c HB_UNUSED) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (സത്യം);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it,
                  തർക്കം use_short_loca,
		  സ്ഥിരം hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (ഇത്);

    unsigned init_len = c->length ();
    വേണ്ടി (auto &_ : it)
      എങ്കിൽ (unlikely (!_.serialize (c, use_short_loca, plan)))
        തിരികെ അസത്യം;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    എങ്കിൽ (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (സത്യം);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    തർക്കം extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (അസത്യം);
#endif

    എങ്കിൽ (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_സന്ദേശം (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (അസത്യം);
    }

    hb_font_t *font = nullptr;
    എങ്കിൽ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      എങ്കിൽ (unlikely (!font))
	return_trace (അസത്യം);
    }

    hb_vector_t<unsigned> padded_offsets;
    എങ്കിൽ (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (അസത്യം);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    എങ്കിൽ (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (അസത്യം);
    }

    എങ്കിൽ (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    വേണ്ടി (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    തർക്കം use_short_loca = അസത്യം;
    എങ്കിൽ (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    എങ്കിൽ (!use_short_loca)
    {
      padded_offsets.clear ();
      വേണ്ടി (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_അഭാജ്യം = c->serializer->start_embed <glyf> ();
    തർക്കം result = glyf_അഭാജ്യം->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    എങ്കിൽ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    എങ്കിൽ (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (അസത്യം);

    തിരികെ result;
  }

  തർക്കം
  _populate_subset_glyphs (സ്ഥിരം hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) സ്ഥിരം;

  hb_font_t *
  _create_font_for_instancing (സ്ഥിരം hb_subset_plan_t *plan) സ്ഥിരം;

  ശൂന്യം _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) സ്ഥിരം
  {
    വേണ്ടി (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  സംരക്ഷിത:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  പൊതു:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
ഘടന GLYF : glyf
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

ഘടന glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = അസത്യം;
    short_offset = അസത്യം;
    അക്കം_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    സ്ഥിരം OT::head &head = *face->table.head;
    എങ്കിൽ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      തിരികെ;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    എങ്കിൽ (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    അല്ലെങ്കിൽ
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    എങ്കിൽ (!extended)
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

    അക്കം_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    അക്കം_glyphs = hb_min (അക്കം_glyphs, face->get_അക്കം_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    എങ്കിൽ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  തർക്കം has_data () സ്ഥിരം { തിരികെ അക്കം_glyphs; }
  തർക്കം is_extended () സ്ഥിരം { തിരികെ extended; }

  ശൂന്യം depend (hb_depend_data_builder_t *builder) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ;
    വേണ്ടി (hb_codepoint_t gid = 0; gid < get_അക്കം_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      വേണ്ടി (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  സംരക്ഷിത:
  template<typename T>
  തർക്കം get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (gid >= അക്കം_glyphs) തിരികെ അസത്യം;

    int64_t അട്ടി_budget = HB_BUDGET_GLYPH;
    എങ്കിൽ (!budget) budget = &അട്ടി_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    തർക്കം phantom_only = !consumer.is_consuming_contour_points ();
    എങ്കിൽ (unlikely (!glyph_for_gid (gid).get_points (font, *ഇത്, all_points, scratch, nullptr, nullptr, nullptr, സത്യം, സത്യം, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      തിരികെ അസത്യം;

    എങ്കിൽ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      തിരികെ അസത്യം;

    unsigned എണ്ണം = all_points.length;
    പ്രതിജ്ഞാ (എണ്ണം >= glyf_impl::PHANTOM_എണ്ണം);
    എണ്ണം -= glyf_impl::PHANTOM_എണ്ണം;

    എങ്കിൽ (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      എങ്കിൽ (അസത്യം)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	{
	  consumer.consume_point (points[i]);
	  എങ്കിൽ (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      അല്ലെങ്കിൽ
      {
	വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	{
	  // Start of a contour.
	  എങ്കിൽ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    വേണ്ടി (; i < എണ്ണം; i++)
	    {
	      consumer.consume_point (points[i]);
	      എങ്കിൽ (points[i].is_end_point)
	      {
		consumer.contour_end ();
		നിർത്തുക;
	      }
	    }
	  }
	  അല്ലെങ്കിൽ
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    വേണ്ടി (; i < എണ്ണം; i++)
	      എങ്കിൽ (points[i].is_end_point)
		നിർത്തുക;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    എങ്കിൽ (likely (end < എണ്ണം) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    അല്ലെങ്കിൽ
#endif
	    എങ്കിൽ (likely (end < എണ്ണം))
	      consumer.consume_point (points[end]);

	    വേണ്ടി (i = start; i < limit; i++)
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
    എങ്കിൽ (phantoms)
      വേണ്ടി (unsigned i = 0; i < glyf_impl::PHANTOM_എണ്ണം; ++i)
	phantoms[i] = all_points.arrayZ[എണ്ണം + i];

    തിരികെ സത്യം;
  }

  പൊതു:

  ഘടന points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    തർക്കം scaled;

    ഘടന contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      ശൂന്യം add (സ്ഥിരം contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      തർക്കം empty () സ്ഥിരം { തിരികെ (min_x >= max_x) || (min_y >= max_y); }

      ശൂന്യം get_extents (hb_font_t *font, hb_glyph_extents_t *extents, തർക്കം scaled)
      {
	എങ്കിൽ (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  തിരികെ;
	}
	{
	  ഇരട്ട x_bearing = (ഇരട്ട) roundf (min_x);
	  ഇരട്ട y_bearing = (ഇരട്ട) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((ഇരട്ട) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((ഇരട്ട) roundf (min_y) - y_bearing);

	  എങ്കിൽ (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      സംരക്ഷിത:
      ദശാംശം min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, തർക്കം scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      എങ്കിൽ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    ശൂന്യം consume_point (സ്ഥിരം contour_point_t &point) { bounds.add (point); }
    ശൂന്യം contour_end () {}
    ശൂന്യം points_end () { bounds.get_extents (font, extents, scaled); }

    തർക്കം is_consuming_contour_points () { തിരികെ extents; }
    contour_point_t *get_phantoms_sink () { തിരികെ phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 തർക്കം is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (gid >= അക്കം_glyphs)) തിരികെ 0;

    തർക്കം success = അസത്യം;

    contour_point_t phantoms[glyf_impl::PHANTOM_എണ്ണം];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, അസത്യം),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->അക്കം_coords : 0),
			  scratch, gvar_cache);
    എങ്കിൽ (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      തിരികെ is_vertical ? upem : upem / 2;
    }

    ദശാംശം result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_മുകൾ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_വലത്].x - phantoms[glyf_impl::PHANTOM_ഇടത്].x;
    തിരികെ hb_clamp (roundf (result), 0.f, (ദശാംശം) UINT_MAX / 2);
  }
#endif

  ദശാംശം
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (gid >= അക്കം_glyphs)) തിരികെ 0;

    തർക്കം success = അസത്യം;

    contour_point_t phantoms[glyf_impl::PHANTOM_എണ്ണം];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, അസത്യം),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->അക്കം_coords : 0),
			  scratch, gvar_cache);
    എങ്കിൽ (unlikely (!success))
    {
      തിരികെ font->face->get_upem ();
    }

    തിരികെ phantoms[glyf_impl::PHANTOM_മുകൾ].y;
  }

  പൊതു:

  തർക്കം get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) സ്ഥിരം
  { തിരികെ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->അക്കം_coords : 0)); }

  തർക്കം get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
		       int64_t *budget = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (gid >= അക്കം_glyphs)) തിരികെ അസത്യം;

#ifndef HB_NO_VAR
    എങ്കിൽ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      എങ്കിൽ (unlikely (!scratch)) തിരികെ അസത്യം;
      തർക്കം ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, സത്യം),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      തിരികെ ret;
    }
#endif
    തിരികെ glyph_for_gid (gid).get_extents_without_var_scaled (font, *ഇത്, extents);
  }

  സ്ഥിരം glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, തർക്കം needs_padding_removal = അസത്യം) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (gid >= അക്കം_glyphs)) തിരികെ glyf_impl::Glyph ();

    unsigned പൂർണ്ണസംഖ്യ start_offset, end_offset;

    എങ്കിൽ (short_offset)
    {
      സ്ഥിരം HBUINT16 *offsets = (സ്ഥിരം HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    അല്ലെങ്കിൽ
    {
      സ്ഥിരം HBUINT32 *offsets = (സ്ഥിരം HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    എങ്കിൽ (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      തിരികെ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((സ്ഥിരം അക്ഷരം *) ഇത്->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    തിരികെ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  തർക്കം
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ അസത്യം;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    എങ്കിൽ (unlikely (!scratch)) തിരികെ സത്യം;

    തർക്കം ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->അക്കം_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    തിരികെ ret;
  }

  തർക്കം
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ അസത്യം;
    തർക്കം ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    തിരികെ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    എങ്കിൽ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, വലുപ്പം (hb_glyf_scratch_t));
      എങ്കിൽ (unlikely (!scratch))
	തിരികെ nullptr;
    }
    തിരികെ scratch;
  }
  ശൂന്യം release_scratch (hb_glyf_scratch_t *scratch) സ്ഥിരം
  {
    എങ്കിൽ (!scratch)
      തിരികെ;
    എങ്കിൽ (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned പൂർണ്ണസംഖ്യ get_അക്കം_glyphs () സ്ഥിരം { തിരികെ അക്കം_glyphs; }
  hb_blob_t *reference_glyf_table () സ്ഥിരം
  { തിരികെ hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  സ്ഥിരം gvar_accelerator_t *gvar;
#endif
  സ്ഥിരം hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  സ്ഥിരം vmtx_accelerator_t *vmtx;
#endif

  സ്വകാര്യ:
  തർക്കം extended;
  തർക്കം short_offset;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


വരിയിൽ തർക്കം
glyf::_populate_subset_glyphs (സ്ഥിരം hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) സ്ഥിരം
{
  OT::glyf_accelerator_t glyf (plan->source);
  എങ്കിൽ (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) തിരികെ അസത്യം;
  *source_glyf = glyf.reference_glyf_table ();

  വേണ്ടി (സ്ഥിരം auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    എങ്കിൽ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    അല്ലെങ്കിൽ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    എങ്കിൽ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    അല്ലെങ്കിൽ
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    എങ്കിൽ (font)
    {
      എങ്കിൽ (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        എങ്കിൽ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        തിരികെ അസത്യം;
      }
    }
  }
  തിരികെ സത്യം;
}

വരിയിൽ hb_font_t *
glyf::_create_font_for_instancing (സ്ഥിരം hb_subset_plan_t *plan) സ്ഥിരം
{
  hb_font_t *font = hb_font_create (plan->source);
  എങ്കിൽ (unlikely (font == hb_font_get_empty ())) തിരികെ nullptr;

#ifndef HB_NO_VAR
  എങ്കിൽ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    തിരികെ font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  എങ്കിൽ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), സത്യം)))
  {
    hb_font_destroy (font);
    തിരികെ nullptr;
  }

  വേണ്ടി (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.നടുവ്;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  തിരികെ font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
