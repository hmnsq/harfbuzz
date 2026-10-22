#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-depend-data.hh"
#ضمن "../../hb-ot-head-table.hh"
#ضمن "../../hb-ot-hmtx-table.hh"
#ضمن "../../hb-ot-var-gvar-table.hh"
#ضمن "../../hb-draw.hh"
#ضمن "../../hb-paint.hh"

#ضمن "glyf-helpers.hh"
#ضمن "Glyph.hh"
#ضمن "SubsetGlyph.hh"
#ضمن "loca.hh"
#ضمن "path-builder.hh"


نطاق OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

بنية glyf
{
  friend بنية glyf_accelerator_t;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  ساكن منطقي has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::head &head = *face->table.head;
    أرجع head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  ساكن منطقي has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    منطقي ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    أرجع ret;
#وإلا
    أرجع خطأ;
#endif
  }

  منطقي sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (true);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (هذا);

    unsigned init_len = c->length ();
    لكل (auto &_ : it)
      إذا (unlikely (!_.serialize (c, use_short_loca, plan)))
        أرجع خطأ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    إذا (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (true);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    منطقي extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    إذا (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (خطأ);
#endif

    إذا (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_ڤسنن (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (خطأ);
    }

    hb_font_t *font = nullptr;
    إذا (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      إذا (unlikely (!font))
	return_trace (خطأ);
    }

    hb_vector_t<unsigned> padded_offsets;
    إذا (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    إذا (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    إذا (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    لكل (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    منطقي use_short_loca = خطأ;
    إذا (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    إذا (!use_short_loca)
    {
      padded_offsets.clear ();
      لكل (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_ڤردان = c->serializer->start_embed <glyf> ();
    منطقي result = glyf_ڤردان->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    إذا (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    إذا (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (خطأ);

    أرجع result;
  }

  منطقي
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  فراغ _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    لكل (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  محمي:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  عام:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
بنية GLYF : glyf
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

بنية glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = خطأ;
    short_offset = خطأ;
    اڠک_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    ثابت OT::head &head = *face->table.head;
    إذا (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      أرجع;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    إذا (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    وإلا
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    إذا (!extended)
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

    اڠک_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    اڠک_glyphs = hb_min (اڠک_glyphs, face->get_اڠک_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    إذا (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  منطقي has_data () ثابت { أرجع اڠک_glyphs; }
  منطقي is_extended () ثابت { أرجع extended; }

  فراغ depend (hb_depend_data_builder_t *builder) ثابت
  {
    إذا (!has_data ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < get_اڠک_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      لكل (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  محمي:
  template<typename T>
  منطقي get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ثابت صحيح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ثابت
  {
    إذا (gid >= اڠک_glyphs) أرجع خطأ;

    int64_t تيندنن_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &تيندنن_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    منطقي phantom_only = !consumer.is_consuming_contour_points ();
    إذا (unlikely (!glyph_for_gid (gid).get_points (font, *هذا, all_points, scratch, nullptr, nullptr, nullptr, true, true, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      أرجع خطأ;

    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      أرجع خطأ;

    unsigned کيراءن = all_points.length;
    أكد (کيراءن >= glyf_impl::PHANTOM_کيراءن);
    کيراءن -= glyf_impl::PHANTOM_کيراءن;

    إذا (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      إذا (خطأ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	لكل (unsigned i = 0; i < کيراءن; i++)
	{
	  consumer.consume_point (points[i]);
	  إذا (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      وإلا
      {
	لكل (unsigned i = 0; i < کيراءن; i++)
	{
	  // Start of a contour.
	  إذا (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    لكل (; i < کيراءن; i++)
	    {
	      consumer.consume_point (points[i]);
	      إذا (points[i].is_end_point)
	      {
		consumer.contour_end ();
		اكسر;
	      }
	    }
	  }
	  وإلا
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    لكل (; i < کيراءن; i++)
	      إذا (points[i].is_end_point)
		اكسر;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    إذا (likely (end < کيراءن) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    وإلا
#endif
	    إذا (likely (end < کيراءن))
	      consumer.consume_point (points[end]);

	    لكل (i = start; i < limit; i++)
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
    إذا (phantoms)
      لكل (unsigned i = 0; i < glyf_impl::PHANTOM_کيراءن; ++i)
	phantoms[i] = all_points.arrayZ[کيراءن + i];

    أرجع true;
  }

  عام:

  بنية points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    منطقي scaled;

    بنية contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      فراغ add (ثابت contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقي empty () ثابت { أرجع (min_x >= max_x) || (min_y >= max_y); }

      فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scaled)
      {
	إذا (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  أرجع;
	}
	{
	  مزدوج x_bearing = (مزدوج) roundf (min_x);
	  مزدوج y_bearing = (مزدوج) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((مزدوج) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((مزدوج) roundf (min_y) - y_bearing);

	  إذا (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محمي:
      عائم min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, منطقي scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      إذا (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    فراغ consume_point (ثابت contour_point_t &point) { bounds.add (point); }
    فراغ contour_end () {}
    فراغ points_end () { bounds.get_extents (font, extents, scaled); }

    منطقي is_consuming_contour_points () { أرجع extents; }
    contour_point_t *get_phantoms_sink () { أرجع phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقي is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ثابت
  {
    إذا (unlikely (gid >= اڠک_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_point_t phantoms[glyf_impl::PHANTOM_کيراءن];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->اڠک_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      أرجع is_vertical ? upem : upem / 2;
    }

    عائم result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_اتس].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_کانن].x - phantoms[glyf_impl::PHANTOM_کيري].x;
    أرجع hb_clamp (roundf (result), 0.f, (عائم) UINT_MAX / 2);
  }
#endif

  عائم
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ثابت
  {
    إذا (unlikely (gid >= اڠک_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_point_t phantoms[glyf_impl::PHANTOM_کيراءن];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->اڠک_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      أرجع font->face->get_upem ();
    }

    أرجع phantoms[glyf_impl::PHANTOM_اتس].y;
  }

  عام:

  منطقي get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { أرجع get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->اڠک_coords : 0)); }

  منطقي get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحيح> coords,
		       int64_t *budget = nullptr) ثابت
  {
    إذا (unlikely (gid >= اڠک_glyphs)) أرجع خطأ;

#ifndef HB_NO_VAR
    إذا (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع خطأ;
      منطقي ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, true),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      أرجع ret;
    }
#endif
    أرجع glyph_for_gid (gid).get_extents_without_var_scaled (font, *هذا, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقي needs_padding_removal = خطأ) ثابت
  {
    إذا (unlikely (gid >= اڠک_glyphs)) أرجع glyf_impl::Glyph ();

    unsigned صحيح start_offset, end_offset;

    إذا (short_offset)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    وإلا
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    إذا (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      أرجع glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ثابت حرف *) هذا->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    أرجع needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقي
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ثابت
  {
    إذا (!has_data ()) أرجع خطأ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    إذا (unlikely (!scratch)) أرجع true;

    منطقي ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->اڠک_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    أرجع ret;
  }

  منطقي
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحيح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ثابت
  {
    إذا (!has_data ()) أرجع خطأ;
    منطقي ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    أرجع ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    إذا (!has_data ()) أرجع nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      إذا (unlikely (!scratch))
	أرجع nullptr;
    }
    أرجع scratch;
  }
  فراغ release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    إذا (!scratch)
      أرجع;
    إذا (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned صحيح get_اڠک_glyphs () ثابت { أرجع اڠک_glyphs; }
  hb_blob_t *reference_glyf_table () ثابت
  { أرجع hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#endif
  ثابت hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#endif

  خاص:
  منطقي extended;
  منطقي short_offset;
  unsigned صحيح اڠک_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


مضمن منطقي
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ثابت
{
  OT::glyf_accelerator_t glyf (plan->source);
  إذا (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) أرجع خطأ;
  *source_glyf = glyf.reference_glyf_table ();

  لكل (ثابت auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    إذا (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    وإلا
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    وإلا
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    إذا (font)
    {
      إذا (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        إذا (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        أرجع خطأ;
      }
    }
  }
  أرجع true;
}

مضمن hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->source);
  إذا (unlikely (font == hb_font_get_empty ())) أرجع nullptr;

#ifndef HB_NO_VAR
  إذا (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    أرجع font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  إذا (unlikely (!vars.alloc (plan->user_axes_location.get_population (), true)))
  {
    hb_font_destroy (font);
    أرجع nullptr;
  }

  لكل (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.تڠه;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  أرجع font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
