#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#অন্তর্ভাব "../../hb-open-type.hh"
#অন্তর্ভাব "../../hb-depend-data.hh"
#অন্তর্ভাব "../../hb-ot-head-table.hh"
#অন্তর্ভাব "../../hb-ot-hmtx-table.hh"
#অন্তর্ভাব "../../hb-ot-var-gvar-table.hh"
#অন্তর্ভাব "../../hb-draw.hh"
#অন্তর্ভাব "../../hb-paint.hh"

#অন্তর্ভাব "glyf-helpers.hh"
#অন্তর্ভাব "Glyph.hh"
#অন্তর্ভাব "SubsetGlyph.hh"
#অন্তর্ভাব "loca.hh"
#অন্তর্ভাব "path-builder.hh"


নামাকাশ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

কাঠামো glyf
{
  friend কাঠামো glyf_accelerator_t;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  স্থির যৌক্তিক has_valid_glyf_format(ধ্রুবক hb_face_t* face)
  {
    ধ্রুবক OT::শীর্ষ_2 &শীর্ষ_2 = *face->table.শীর্ষ_2;
    ফেরত শীর্ষ_2.সূচক_To_Loc_Format <= 1 && শীর্ষ_2.glyph_তথ্য_Format <= 1;
  }

  স্থির যৌক্তিক has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    যৌক্তিক ret = GLYF_table.get_দৈর্ঘ্য () && LOCA_table.get_দৈর্ঘ্য ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ফেরত ret;
#অন্যথা
    ফেরত মিথ্যা;
#endif
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c HB_UNUSED) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (সত্য);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it,
                  যৌক্তিক use_short_loca,
		  ধ্রুবক hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (এই);

    unsigned init_len = c->দৈর্ঘ্য ();
    জন্য (auto &_ : it)
      যদি (unlikely (!_.serialize (c, use_short_loca, plan)))
        ফেরত মিথ্যা;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    যদি (init_len == c->দৈর্ঘ্য ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (সত্য);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    যৌক্তিক extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    যদি (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (মিথ্যা);
#endif

    যদি (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_বার্তা (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (মিথ্যা);
    }

    hb_font_t *font = nullptr;
    যদি (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      যদি (unlikely (!font))
	return_trace (মিথ্যা);
    }

    hb_vector_t<unsigned> padded_offsets;
    যদি (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.দৈর্ঘ্য)))
    {
      hb_font_destroy (font);
      return_trace (মিথ্যা);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    যদি (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (মিথ্যা);
    }

    যদি (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    জন্য (auto &g : glyphs)
    {
      unsigned আকার_2 = g.padded_আকার ();
      padded_offsets.push (আকার_2);
      max_offset += আকার_2;
    }

    যৌক্তিক use_short_loca = মিথ্যা;
    যদি (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    যদি (!use_short_loca)
    {
      padded_offsets.clear ();
      জন্য (auto &g : glyphs)
	padded_offsets.push (g.দৈর্ঘ্য ());
    }

    auto *glyf_মৌলিক = c->serializer->start_embed <glyf> ();
    যৌক্তিক ফলাফল = glyf_মৌলিক->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    যদি (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    যদি (unlikely (!c->serializer->check_success (glyf_impl::add_loca_and_শীর্ষ (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (মিথ্যা);

    ফেরত ফলাফল;
  }

  যৌক্তিক
  _populate_subset_glyphs (ধ্রুবক hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ধ্রুবক;

  hb_font_t *
  _create_font_for_instancing (ধ্রুবক hb_subset_plan_t *plan) ধ্রুবক;

  শূন্য _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ধ্রুবক
  {
    জন্য (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  সুরক্ষিত:
  UnsizedArrayOf<HBUINT8>
		তথ্য_Z;	/* Glyphs data. */
  সর্বজনীন:
  DEFINE_আকার_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
কাঠামো GLYF : glyf
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

কাঠামো glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = মিথ্যা;
    short_offset = মিথ্যা;
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
    ধ্রুবক OT::শীর্ষ_2 &শীর্ষ_2 = *face->table.শীর্ষ_2;
    যদি (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ফেরত;
    short_offset = 0 == শীর্ষ_2.সূচক_To_Loc_Format;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_দৈর্ঘ্য () && LOCA_table.get_দৈর্ঘ্য ();
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

    অঙ্ক_glyphs = hb_max (1u, loca_table.get_দৈর্ঘ্য () / (short_offset ? 2 : 4)) - 1;
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

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত অঙ্ক_glyphs; }
  যৌক্তিক is_extended () ধ্রুবক { ফেরত extended; }

  শূন্য depend (hb_depend_তথ্য_builder_t *builder) ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত;
    জন্য (hb_codepoint_t gid = 0; gid < get_অঙ্ক_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      জন্য (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  সুরক্ষিত:
  template<typename T>
  যৌক্তিক get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ধ্রুবক
  {
    যদি (gid >= অঙ্ক_glyphs) ফেরত মিথ্যা;

    int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
    যদি (!budget) budget = &স্তূপ_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    যৌক্তিক phantom_only = !consumer.is_consuming_contour_points ();
    যদি (unlikely (!glyph_for_gid (gid).get_points (font, *এই, all_points, scratch, nullptr, nullptr, nullptr, সত্য, সত্য, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ফেরত মিথ্যা;

    যদি (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.দৈর্ঘ্য)))
      ফেরত মিথ্যা;

    unsigned গুনতি = all_points.দৈর্ঘ্য;
    প্রতিজ্ঞা (গুনতি >= glyf_impl::PHANTOM_গুনতি);
    গুনতি -= glyf_impl::PHANTOM_গুনতি;

    যদি (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      যদি (মিথ্যা)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	জন্য (unsigned i = 0; i < গুনতি; i++)
	{
	  consumer.consume_point (points[i]);
	  যদি (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      অন্যথা
      {
	জন্য (unsigned i = 0; i < গুনতি; i++)
	{
	  // Start of a contour.
	  যদি (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    জন্য (; i < গুনতি; i++)
	    {
	      consumer.consume_point (points[i]);
	      যদি (points[i].is_end_point)
	      {
		consumer.contour_end ();
		বিরতি;
	      }
	    }
	  }
	  অন্যথা
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    জন্য (; i < গুনতি; i++)
	      যদি (points[i].is_end_point)
		বিরতি;

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

	    জন্য (i = start; i < limit; i++)
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
      জন্য (unsigned i = 0; i < glyf_impl::PHANTOM_গুনতি; ++i)
	phantoms[i] = all_points.arrayZ[গুনতি + i];

    ফেরত সত্য;
  }

  সর্বজনীন:

  কাঠামো points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    যৌক্তিক scaled;

    কাঠামো contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      শূন্য add (ধ্রুবক contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      যৌক্তিক empty () ধ্রুবক { ফেরত (min_x >= max_x) || (min_y >= max_y); }

      শূন্য get_extents (hb_font_t *font, hb_glyph_extents_t *extents, যৌক্তিক scaled)
      {
	যদি (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ফেরত;
	}
	{
	  দ্বিগুণ x_bearing = (দ্বিগুণ) roundf (min_x);
	  দ্বিগুণ y_bearing = (দ্বিগুণ) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((দ্বিগুণ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((দ্বিগুণ) roundf (min_y) - y_bearing);

	  যদি (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      সুরক্ষিত:
      দশমিক min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, যৌক্তিক scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      যদি (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    শূন্য consume_point (ধ্রুবক contour_point_t &point) { bounds.add (point); }
    শূন্য contour_end () {}
    শূন্য points_end () { bounds.get_extents (font, extents, scaled); }

    যৌক্তিক is_consuming_contour_points () { ফেরত extents; }
    contour_point_t *get_phantoms_sink () { ফেরত phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 যৌক্তিক is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ফেরত 0;

    যৌক্তিক success = মিথ্যা;

    contour_point_t phantoms[glyf_impl::PHANTOM_গুনতি];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, মিথ্যা),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			  scratch, gvar_cache);
    যদি (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ফেরত is_vertical ? upem : upem / 2;
    }

    দশমিক ফলাফল = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_শীর্ষ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ডান].x - phantoms[glyf_impl::PHANTOM_বাম].x;
    ফেরত hb_clamp (roundf (ফলাফল), 0.f, (দশমিক) UINT_MAX / 2);
  }
#endif

  দশমিক
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ফেরত 0;

    যৌক্তিক success = মিথ্যা;

    contour_point_t phantoms[glyf_impl::PHANTOM_গুনতি];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, মিথ্যা),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			  scratch, gvar_cache);
    যদি (unlikely (!success))
    {
      ফেরত font->face->get_upem ();
    }

    ফেরত phantoms[glyf_impl::PHANTOM_শীর্ষ].y;
  }

  সর্বজনীন:

  যৌক্তিক get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ধ্রুবক
  { ফেরত get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->অঙ্ক_coords : 0)); }

  যৌক্তিক get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
		       int64_t *budget = nullptr) ধ্রুবক
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ফেরত মিথ্যা;

#ifndef HB_NO_VAR
    যদি (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ফেরত মিথ্যা;
      যৌক্তিক ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, সত্য),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ফেরত ret;
    }
#endif
    ফেরত glyph_for_gid (gid).get_extents_without_var_scaled (font, *এই, extents);
  }

  ধ্রুবক glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, যৌক্তিক needs_padding_removal = মিথ্যা) ধ্রুবক
  {
    যদি (unlikely (gid >= অঙ্ক_glyphs)) ফেরত glyf_impl::Glyph ();

    unsigned পূর্ণসংখ্যা start_offset, end_offset;

    যদি (short_offset)
    {
      ধ্রুবক HBUINT16 *offsets = (ধ্রুবক HBUINT16 *) loca_table->তথ্য_Z.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    অন্যথা
    {
      ধ্রুবক HBUINT32 *offsets = (ধ্রুবক HBUINT32 *) loca_table->তথ্য_Z.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    যদি (unlikely (start_offset > end_offset || end_offset > glyf_table.get_দৈর্ঘ্য ()))
      ফেরত glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ধ্রুবক অক্ষর *) এই->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ফেরত needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  যৌক্তিক
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত মিথ্যা;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    যদি (unlikely (!scratch)) ফেরত সত্য;

    যৌক্তিক ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ফেরত ret;
  }

  যৌক্তিক
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত মিথ্যা;
    যৌক্তিক ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ফেরত ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, আকার (hb_glyf_scratch_t));
      যদি (unlikely (!scratch))
	ফেরত nullptr;
    }
    ফেরত scratch;
  }
  শূন্য release_scratch (hb_glyf_scratch_t *scratch) ধ্রুবক
  {
    যদি (!scratch)
      ফেরত;
    যদি (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned পূর্ণসংখ্যা get_অঙ্ক_glyphs () ধ্রুবক { ফেরত অঙ্ক_glyphs; }
  hb_blob_t *reference_glyf_table () ধ্রুবক
  { ফেরত hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ধ্রুবক gvar_accelerator_t *gvar;
#endif
  ধ্রুবক hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ধ্রুবক vmtx_accelerator_t *vmtx;
#endif

  ব্যক্তিগত:
  যৌক্তিক extended;
  যৌক্তিক short_offset;
  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


অন্তঃরেখ যৌক্তিক
glyf::_populate_subset_glyphs (ধ্রুবক hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ধ্রুবক
{
  OT::glyf_accelerator_t glyf (plan->source);
  যদি (!glyphs.alloc_exact (plan->new_to_old_gid_list.দৈর্ঘ্য)) ফেরত মিথ্যা;
  *source_glyf = glyf.reference_glyf_table ();

  জন্য (ধ্রুবক auto &pair : plan->new_to_old_gid_list)
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
        ফেরত মিথ্যা;
      }
    }
  }
  ফেরত সত্য;
}

অন্তঃরেখ hb_font_t *
glyf::_create_font_for_instancing (ধ্রুবক hb_subset_plan_t *plan) ধ্রুবক
{
  hb_font_t *font = hb_font_create (plan->source);
  যদি (unlikely (font == hb_font_get_empty ())) ফেরত nullptr;

#ifndef HB_NO_VAR
  যদি (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.দৈর্ঘ্য);
    ফেরত font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  যদি (unlikely (!vars.alloc (plan->user_axes_location.get_population (), সত্য)))
  {
    hb_font_destroy (font);
    ফেরত nullptr;
  }

  জন্য (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.মধ্য;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ফেরত font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
