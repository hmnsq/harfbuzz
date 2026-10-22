#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#અન્તર્ભાવ "../../hb-open-type.hh"
#અન્તર્ભાવ "../../hb-depend-data.hh"
#અન્તર્ભાવ "../../hb-ot-head-table.hh"
#અન્તર્ભાવ "../../hb-ot-hmtx-table.hh"
#અન્તર્ભાવ "../../hb-ot-var-gvar-table.hh"
#અન્તર્ભાવ "../../hb-draw.hh"
#અન્તર્ભાવ "../../hb-paint.hh"

#અન્તર્ભાવ "glyf-helpers.hh"
#અન્તર્ભાવ "Glyph.hh"
#અન્તર્ભાવ "SubsetGlyph.hh"
#અન્તર્ભાવ "loca.hh"
#અન્તર્ભાવ "path-builder.hh"


નામાકાશ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

સંરચના glyf
{
  friend સંરચના glyf_accelerator_t;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  સ્થિર તાર્કિક has_valid_glyf_format(અચલ hb_face_t* face)
  {
    અચલ OT::head &head = *face->table.head;
    પાછા head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  સ્થિર તાર્કિક has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    તાર્કિક ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    પાછા ret;
#નહીંતર
    પાછા ખોટું;
#endif
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c HB_UNUSED) અચલ
  {
    TRACE_SANITIZE (આ);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (સાચું);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it,
                  તાર્કિક use_short_loca,
		  અચલ hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (આ);

    unsigned init_len = c->length ();
    માટે (auto &_ : it)
      જો (unlikely (!_.serialize (c, use_short_loca, plan)))
        પાછા ખોટું;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    જો (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (સાચું);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    તાર્કિક extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    જો (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (ખોટું);
#endif

    જો (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_સંદેશ (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (ખોટું);
    }

    hb_font_t *font = nullptr;
    જો (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      જો (unlikely (!font))
	return_trace (ખોટું);
    }

    hb_vector_t<unsigned> padded_offsets;
    જો (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (ખોટું);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    જો (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (ખોટું);
    }

    જો (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    માટે (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    તાર્કિક use_short_loca = ખોટું;
    જો (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    જો (!use_short_loca)
    {
      padded_offsets.clear ();
      માટે (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_અવિભાજ્ય = c->serializer->start_embed <glyf> ();
    તાર્કિક result = glyf_અવિભાજ્ય->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    જો (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    જો (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (ખોટું);

    પાછા result;
  }

  તાર્કિક
  _populate_subset_glyphs (અચલ hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) અચલ;

  hb_font_t *
  _create_font_for_instancing (અચલ hb_subset_plan_t *plan) અચલ;

  શૂન્ય _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) અચલ
  {
    માટે (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  સુરક્ષિત:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  જાહેર:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
સંરચના GLYF : glyf
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

સંરચના glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = ખોટું;
    short_offset = ખોટું;
    અંક_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    અચલ OT::head &head = *face->table.head;
    જો (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      પાછા;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    જો (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    નહીંતર
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    જો (!extended)
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

    અંક_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    અંક_glyphs = hb_min (અંક_glyphs, face->get_અંક_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    જો (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  તાર્કિક has_data () અચલ { પાછા અંક_glyphs; }
  તાર્કિક is_extended () અચલ { પાછા extended; }

  શૂન્ય depend (hb_depend_data_builder_t *builder) અચલ
  {
    જો (!has_data ()) પાછા;
    માટે (hb_codepoint_t gid = 0; gid < get_અંક_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      માટે (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  સુરક્ષિત:
  template<typename T>
  તાર્કિક get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<અચલ પૂર્ણાંક> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) અચલ
  {
    જો (gid >= અંક_glyphs) પાછા ખોટું;

    int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
    જો (!budget) budget = &ઢગલો_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    તાર્કિક phantom_only = !consumer.is_consuming_contour_points ();
    જો (unlikely (!glyph_for_gid (gid).get_points (font, *આ, all_points, scratch, nullptr, nullptr, nullptr, સાચું, સાચું, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      પાછા ખોટું;

    જો (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      પાછા ખોટું;

    unsigned ગણતરી = all_points.length;
    પ્રતિજ્ઞા (ગણતરી >= glyf_impl::PHANTOM_ગણતરી);
    ગણતરી -= glyf_impl::PHANTOM_ગણતરી;

    જો (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      જો (ખોટું)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	માટે (unsigned i = 0; i < ગણતરી; i++)
	{
	  consumer.consume_point (points[i]);
	  જો (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      નહીંતર
      {
	માટે (unsigned i = 0; i < ગણતરી; i++)
	{
	  // Start of a contour.
	  જો (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    માટે (; i < ગણતરી; i++)
	    {
	      consumer.consume_point (points[i]);
	      જો (points[i].is_end_point)
	      {
		consumer.contour_end ();
		તોડો;
	      }
	    }
	  }
	  નહીંતર
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    માટે (; i < ગણતરી; i++)
	      જો (points[i].is_end_point)
		તોડો;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    જો (likely (end < ગણતરી) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    નહીંતર
#endif
	    જો (likely (end < ગણતરી))
	      consumer.consume_point (points[end]);

	    માટે (i = start; i < limit; i++)
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
    જો (phantoms)
      માટે (unsigned i = 0; i < glyf_impl::PHANTOM_ગણતરી; ++i)
	phantoms[i] = all_points.arrayZ[ગણતરી + i];

    પાછા સાચું;
  }

  જાહેર:

  સંરચના points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    તાર્કિક scaled;

    સંરચના contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      શૂન્ય add (અચલ contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      તાર્કિક empty () અચલ { પાછા (min_x >= max_x) || (min_y >= max_y); }

      શૂન્ય get_extents (hb_font_t *font, hb_glyph_extents_t *extents, તાર્કિક scaled)
      {
	જો (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  પાછા;
	}
	{
	  દ્વિગુણ x_bearing = (દ્વિગુણ) roundf (min_x);
	  દ્વિગુણ y_bearing = (દ્વિગુણ) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (min_y) - y_bearing);

	  જો (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      સુરક્ષિત:
      દશાંશ min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, તાર્કિક scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      જો (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    શૂન્ય consume_point (અચલ contour_point_t &point) { bounds.add (point); }
    શૂન્ય contour_end () {}
    શૂન્ય points_end () { bounds.get_extents (font, extents, scaled); }

    તાર્કિક is_consuming_contour_points () { પાછા extents; }
    contour_point_t *get_phantoms_sink () { પાછા phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 તાર્કિક is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) અચલ
  {
    જો (unlikely (gid >= અંક_glyphs)) પાછા 0;

    તાર્કિક success = ખોટું;

    contour_point_t phantoms[glyf_impl::PHANTOM_ગણતરી];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ખોટું),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->અંક_coords : 0),
			  scratch, gvar_cache);
    જો (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      પાછા is_vertical ? upem : upem / 2;
    }

    દશાંશ result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_ટોચ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_જમણું].x - phantoms[glyf_impl::PHANTOM_ડાબું].x;
    પાછા hb_clamp (roundf (result), 0.f, (દશાંશ) UINT_MAX / 2);
  }
#endif

  દશાંશ
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) અચલ
  {
    જો (unlikely (gid >= અંક_glyphs)) પાછા 0;

    તાર્કિક success = ખોટું;

    contour_point_t phantoms[glyf_impl::PHANTOM_ગણતરી];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, ખોટું),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->અંક_coords : 0),
			  scratch, gvar_cache);
    જો (unlikely (!success))
    {
      પાછા font->face->get_upem ();
    }

    પાછા phantoms[glyf_impl::PHANTOM_ટોચ].y;
  }

  જાહેર:

  તાર્કિક get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) અચલ
  { પાછા get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->અંક_coords : 0)); }

  તાર્કિક get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<અચલ પૂર્ણાંક> coords,
		       int64_t *budget = nullptr) અચલ
  {
    જો (unlikely (gid >= અંક_glyphs)) પાછા ખોટું;

#ifndef HB_NO_VAR
    જો (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      જો (unlikely (!scratch)) પાછા ખોટું;
      તાર્કિક ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, સાચું),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      પાછા ret;
    }
#endif
    પાછા glyph_for_gid (gid).get_extents_without_var_scaled (font, *આ, extents);
  }

  અચલ glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, તાર્કિક needs_padding_removal = ખોટું) અચલ
  {
    જો (unlikely (gid >= અંક_glyphs)) પાછા glyf_impl::Glyph ();

    unsigned પૂર્ણાંક start_offset, end_offset;

    જો (short_offset)
    {
      અચલ HBUINT16 *offsets = (અચલ HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    નહીંતર
    {
      અચલ HBUINT32 *offsets = (અચલ HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    જો (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      પાછા glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((અચલ અક્ષર *) આ->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    પાછા needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  તાર્કિક
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) અચલ
  {
    જો (!has_data ()) પાછા ખોટું;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    જો (unlikely (!scratch)) પાછા સાચું;

    તાર્કિક ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->અંક_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    પાછા ret;
  }

  તાર્કિક
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<અચલ પૂર્ણાંક> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) અચલ
  {
    જો (!has_data ()) પાછા ખોટું;
    તાર્કિક ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    પાછા ret;
  }


  hb_glyf_scratch_t *acquire_scratch () અચલ
  {
    જો (!has_data ()) પાછા nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    જો (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, કદ (hb_glyf_scratch_t));
      જો (unlikely (!scratch))
	પાછા nullptr;
    }
    પાછા scratch;
  }
  શૂન્ય release_scratch (hb_glyf_scratch_t *scratch) અચલ
  {
    જો (!scratch)
      પાછા;
    જો (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned પૂર્ણાંક get_અંક_glyphs () અચલ { પાછા અંક_glyphs; }
  hb_blob_t *reference_glyf_table () અચલ
  { પાછા hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  અચલ gvar_accelerator_t *gvar;
#endif
  અચલ hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  અચલ vmtx_accelerator_t *vmtx;
#endif

  ખાનગી:
  તાર્કિક extended;
  તાર્કિક short_offset;
  unsigned પૂર્ણાંક અંક_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


અંતઃરેખ તાર્કિક
glyf::_populate_subset_glyphs (અચલ hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) અચલ
{
  OT::glyf_accelerator_t glyf (plan->source);
  જો (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) પાછા ખોટું;
  *source_glyf = glyf.reference_glyf_table ();

  માટે (અચલ auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    જો (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    નહીંતર
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    જો (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    નહીંતર
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    જો (font)
    {
      જો (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        જો (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        પાછા ખોટું;
      }
    }
  }
  પાછા સાચું;
}

અંતઃરેખ hb_font_t *
glyf::_create_font_for_instancing (અચલ hb_subset_plan_t *plan) અચલ
{
  hb_font_t *font = hb_font_create (plan->source);
  જો (unlikely (font == hb_font_get_empty ())) પાછા nullptr;

#ifndef HB_NO_VAR
  જો (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    પાછા font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  જો (unlikely (!vars.alloc (plan->user_axes_location.get_population (), સાચું)))
  {
    hb_font_destroy (font);
    પાછા nullptr;
  }

  માટે (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.મધ્ય;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  પાછા font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
