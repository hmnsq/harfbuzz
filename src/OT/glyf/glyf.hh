#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#شامل "../../hb-open-type.hh"
#شامل "../../hb-depend-data.hh"
#شامل "../../hb-ot-head-table.hh"
#شامل "../../hb-ot-hmtx-table.hh"
#شامل "../../hb-ot-var-gvar-table.hh"
#شامل "../../hb-draw.hh"
#شامل "../../hb-paint.hh"

#شامل "glyf-helpers.hh"
#شامل "Glyph.hh"
#شامل "SubsetGlyph.hh"
#شامل "loca.hh"
#شامل "path-builder.hh"


فضاینام OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

ساختار glyf
{
  friend ساختار glyf_accelerator_t;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  ایستا منطقی has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::head &head = *face->table.head;
    بازگشت head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  ایستا منطقی has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    منطقی ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    بازگشت ret;
#وگرنه
    بازگشت نادرست;
#endif
  }

  منطقی sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (این);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (درست);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقی use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (این);

    unsigned init_len = c->length ();
    برای (auto &_ : it)
      اگر (unlikely (!_.serialize (c, use_short_loca, plan)))
        بازگشت نادرست;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    اگر (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (درست);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    منطقی extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    اگر (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (نادرست);
#endif

    اگر (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_پیام (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (نادرست);
    }

    hb_font_t *font = nullptr;
    اگر (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      اگر (unlikely (!font))
	return_trace (نادرست);
    }

    hb_vector_t<unsigned> padded_offsets;
    اگر (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (نادرست);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    اگر (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (نادرست);
    }

    اگر (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    برای (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    منطقی use_short_loca = نادرست;
    اگر (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    اگر (!use_short_loca)
    {
      padded_offsets.clear ();
      برای (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_اول = c->serializer->start_embed <glyf> ();
    منطقی result = glyf_اول->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    اگر (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    اگر (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (نادرست);

    بازگشت result;
  }

  منطقی
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  تهی _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    برای (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  محافظتشده:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  عمومی:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
ساختار GLYF : glyf
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

ساختار glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = نادرست;
    short_offset = نادرست;
    رقم_glyphs = 0;
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
    اگر (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      بازگشت;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    اگر (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    وگرنه
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    اگر (!extended)
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

    رقم_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    رقم_glyphs = hb_min (رقم_glyphs, face->get_رقم_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    اگر (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  منطقی has_data () ثابت { بازگشت رقم_glyphs; }
  منطقی is_extended () ثابت { بازگشت extended; }

  تهی depend (hb_depend_data_builder_t *builder) ثابت
  {
    اگر (!has_data ()) بازگشت;
    برای (hb_codepoint_t gid = 0; gid < get_رقم_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      برای (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  محافظتشده:
  template<typename T>
  منطقی get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<ثابت صحیح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) ثابت
  {
    اگر (gid >= رقم_glyphs) بازگشت نادرست;

    int64_t پشته_budget = HB_BUDGET_GLYPH;
    اگر (!budget) budget = &پشته_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    منطقی phantom_only = !consumer.is_consuming_contour_points ();
    اگر (unlikely (!glyph_for_gid (gid).get_points (font, *این, all_points, scratch, nullptr, nullptr, nullptr, درست, درست, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      بازگشت نادرست;

    اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      بازگشت نادرست;

    unsigned شمار = all_points.length;
    أكد (شمار >= glyf_impl::PHANTOM_شمار);
    شمار -= glyf_impl::PHANTOM_شمار;

    اگر (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      اگر (نادرست)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	برای (unsigned i = 0; i < شمار; i++)
	{
	  consumer.consume_point (points[i]);
	  اگر (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      وگرنه
      {
	برای (unsigned i = 0; i < شمار; i++)
	{
	  // Start of a contour.
	  اگر (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    برای (; i < شمار; i++)
	    {
	      consumer.consume_point (points[i]);
	      اگر (points[i].is_end_point)
	      {
		consumer.contour_end ();
		بشکن;
	      }
	    }
	  }
	  وگرنه
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    برای (; i < شمار; i++)
	      اگر (points[i].is_end_point)
		بشکن;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    اگر (likely (end < شمار) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    وگرنه
#endif
	    اگر (likely (end < شمار))
	      consumer.consume_point (points[end]);

	    برای (i = start; i < limit; i++)
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
    اگر (phantoms)
      برای (unsigned i = 0; i < glyf_impl::PHANTOM_شمار; ++i)
	phantoms[i] = all_points.arrayZ[شمار + i];

    بازگشت درست;
  }

  عمومی:

  ساختار points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    منطقی scaled;

    ساختار contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      تهی add (ثابت contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقی empty () ثابت { بازگشت (min_x >= max_x) || (min_y >= max_y); }

      تهی get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقی scaled)
      {
	اگر (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  بازگشت;
	}
	{
	  دوگانه x_bearing = (دوگانه) roundf (min_x);
	  دوگانه y_bearing = (دوگانه) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((دوگانه) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((دوگانه) roundf (min_y) - y_bearing);

	  اگر (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محافظتشده:
      اعشاری min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, منطقی scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      اگر (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    تهی consume_point (ثابت contour_point_t &point) { bounds.add (point); }
    تهی contour_end () {}
    تهی points_end () { bounds.get_extents (font, extents, scaled); }

    منطقی is_consuming_contour_points () { بازگشت extents; }
    contour_point_t *get_phantoms_sink () { بازگشت phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقی is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) ثابت
  {
    اگر (unlikely (gid >= رقم_glyphs)) بازگشت 0;

    منطقی success = نادرست;

    contour_point_t phantoms[glyf_impl::PHANTOM_شمار];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, نادرست),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->رقم_coords : 0),
			  scratch, gvar_cache);
    اگر (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      بازگشت is_vertical ? upem : upem / 2;
    }

    اعشاری result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_بالاترین].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_راست].x - phantoms[glyf_impl::PHANTOM_چپ].x;
    بازگشت hb_clamp (roundf (result), 0.f, (اعشاری) UINT_MAX / 2);
  }
#endif

  اعشاری
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) ثابت
  {
    اگر (unlikely (gid >= رقم_glyphs)) بازگشت 0;

    منطقی success = نادرست;

    contour_point_t phantoms[glyf_impl::PHANTOM_شمار];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, نادرست),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->رقم_coords : 0),
			  scratch, gvar_cache);
    اگر (unlikely (!success))
    {
      بازگشت font->face->get_upem ();
    }

    بازگشت phantoms[glyf_impl::PHANTOM_بالاترین].y;
  }

  عمومی:

  منطقی get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { بازگشت get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->رقم_coords : 0)); }

  منطقی get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحیح> coords,
		       int64_t *budget = nullptr) ثابت
  {
    اگر (unlikely (gid >= رقم_glyphs)) بازگشت نادرست;

#ifndef HB_NO_VAR
    اگر (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) بازگشت نادرست;
      منطقی ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, درست),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      بازگشت ret;
    }
#endif
    بازگشت glyph_for_gid (gid).get_extents_without_var_scaled (font, *این, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقی needs_padding_removal = نادرست) ثابت
  {
    اگر (unlikely (gid >= رقم_glyphs)) بازگشت glyf_impl::Glyph ();

    unsigned صحیح start_offset, end_offset;

    اگر (short_offset)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    وگرنه
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    اگر (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      بازگشت glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((ثابت نویسه *) این->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    بازگشت needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقی
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) ثابت
  {
    اگر (!has_data ()) بازگشت نادرست;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    اگر (unlikely (!scratch)) بازگشت درست;

    منطقی ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->رقم_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    بازگشت ret;
  }

  منطقی
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحیح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) ثابت
  {
    اگر (!has_data ()) بازگشت نادرست;
    منطقی ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    بازگشت ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    اگر (!has_data ()) بازگشت nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, اندازه (hb_glyf_scratch_t));
      اگر (unlikely (!scratch))
	بازگشت nullptr;
    }
    بازگشت scratch;
  }
  تهی release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    اگر (!scratch)
      بازگشت;
    اگر (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned صحیح get_رقم_glyphs () ثابت { بازگشت رقم_glyphs; }
  hb_blob_t *reference_glyf_table () ثابت
  { بازگشت hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#endif
  ثابت hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#endif

  خصوصی:
  منطقی extended;
  منطقی short_offset;
  unsigned صحیح رقم_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


درونخطی منطقی
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) ثابت
{
  OT::glyf_accelerator_t glyf (plan->source);
  اگر (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) بازگشت نادرست;
  *source_glyf = glyf.reference_glyf_table ();

  برای (ثابت auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    اگر (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    وگرنه
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    اگر (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    وگرنه
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    اگر (font)
    {
      اگر (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        اگر (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        بازگشت نادرست;
      }
    }
  }
  بازگشت درست;
}

درونخطی hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->source);
  اگر (unlikely (font == hb_font_get_empty ())) بازگشت nullptr;

#ifndef HB_NO_VAR
  اگر (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    بازگشت font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  اگر (unlikely (!vars.alloc (plan->user_axes_location.get_population (), درست)))
  {
    hb_font_destroy (font);
    بازگشت nullptr;
  }

  برای (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.میانه;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  بازگشت font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
