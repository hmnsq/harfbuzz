#अगर_अघोषित OT_GLYF_GLYF_HH
#घोषणा OT_GLYF_GLYF_HH


#समावेश "../../hb-open-type.hh"
#समावेश "../../hb-depend-data.hh"
#समावेश "../../hb-ot-head-table.hh"
#समावेश "../../hb-ot-hmtx-table.hh"
#समावेश "../../hb-ot-var-gvar-table.hh"
#समावेश "../../hb-draw.hh"
#समावेश "../../hb-paint.hh"

#समावेश "glyf-helpers.hh"
#समावेश "Glyph.hh"
#समावेश "SubsetGlyph.hh"
#समावेश "loca.hh"
#समावेश "path-builder.hh"


namespace OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#घोषणा HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#घोषणा HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

काष्ठा glyf
{
  मित्र काष्ठा glyf_accelerator_t;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  अटल bool has_valid_glyf_format(स्थिर hb_face_t* face)
  {
    स्थिर OT::शीर्ष_2 &शीर्ष_2 = *face->table.शीर्ष_2;
    वापस शीर्ष_2.सूचकांक_To_Loc_Format <= 1 && शीर्ष_2.glyph_तथ्य_Format <= 1;
  }

  अटल bool has_extended_glyf (hb_face_t *face)
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    bool ret = GLYF_table.get_लंबाई () && LOCA_table.get_लंबाई ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    वापस ret;
#अन्यथा
    वापस false;
#पूर्ण_अगर
  }

  bool sanitize (hb_sanitize_context_t *c HB_UNUSED) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (true);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  ढाँचा <typename Iterator>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
                  bool use_short_loca,
		  स्थिर hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (यह);

    अचिन्हित init_len = c->लंबाई ();
    क्रम (स्वतः &_ : it)
      अगर (unlikely (!_.serialize (c, use_short_loca, plan)))
        वापस false;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    अगर (init_len == c->लंबाई ())
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
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    bool extended = has_extended_glyf (c->plan->source);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (false);
#पूर्ण_अगर

    अगर (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_संदेश (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (false);
    }

    hb_font_t *font = nullptr;
    अगर (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      अगर (unlikely (!font))
	return_trace (false);
    }

    hb_vector_t<अचिन्हित> padded_offsets;
    अगर (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.लंबाई)))
    {
      hb_font_destroy (font);
      return_trace (false);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    अगर (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (false);
    }

    अगर (font)
      hb_font_destroy (font);

    अचिन्हित max_offset = 0;
    क्रम (स्वतः &g : glyphs)
    {
      अचिन्हित आकार = g.padded_आकार ();
      padded_offsets.push (आकार);
      max_offset += आकार;
    }

    bool use_short_loca = false;
    अगर (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    अगर (!use_short_loca)
    {
      padded_offsets.सफा ();
      क्रम (स्वतः &g : glyphs)
	padded_offsets.push (g.लंबाई ());
    }

    स्वतः *glyf_अभाज्य = c->serializer->start_embed <glyf> ();
    bool परिणाम = glyf_अभाज्य->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    अगर (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    अगर (unlikely (!c->serializer->check_success (glyf_impl::add_loca_and_शीर्ष (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (false);

    वापस परिणाम;
  }

  bool
  _populate_subset_glyphs (स्थिर hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) स्थिर;

  hb_font_t *
  _create_font_for_instancing (स्थिर hb_subset_plan_t *plan) स्थिर;

  व्योम _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) स्थिर
  {
    क्रम (स्वतः &g : glyphs)
      g.free_compiled_bytes ();
  }

  रक्षित:
  UnsizedArrayOf<HBUINT8>
		तथ्य_Z;	/* Glyphs data. */
  खुला:
  DEFINE_आकार_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#अगर_अघोषित HB_NO_BEYOND_64K
काष्ठा GLYF : glyf
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#पूर्ण_अगर

काष्ठा glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = false;
    short_offset = false;
    अंक_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#अगर_अघोषित HB_NO_VAR
    gvar = nullptr;
#पूर्ण_अगर
    hmtx = nullptr;
#अगर_अघोषित HB_NO_VERTICAL
    vmtx = nullptr;
#पूर्ण_अगर
    स्थिर OT::शीर्ष_2 &शीर्ष_2 = *face->table.शीर्ष_2;
    अगर (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      वापस;
    short_offset = 0 == शीर्ष_2.सूचकांक_To_Loc_Format;

#अगर_अघोषित HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_लंबाई () && LOCA_table.get_लंबाई ();
    अगर (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    अन्यथा
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#पूर्ण_अगर
    अगर (!extended)
    {
      glyf_table = hb_sanitize_context_t ().reference_table<glyf> (face);
      loca_table = hb_sanitize_context_t ().reference_table<loca> (face);
    }
#अगर_अघोषित HB_NO_VAR
    gvar = face->table.gvar;
#पूर्ण_अगर
    hmtx = face->table.hmtx;
#अगर_अघोषित HB_NO_VERTICAL
    vmtx = face->table.vmtx;
#पूर्ण_अगर

    अंक_glyphs = hb_max (1u, loca_table.get_लंबाई () / (short_offset ? 2 : 4)) - 1;
    अंक_glyphs = hb_min (अंक_glyphs, face->get_अंक_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    स्वतः *scratch = cached_scratch.get_relaxed ();
    अगर (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  bool has_तथ्य () स्थिर { वापस अंक_glyphs; }
  bool is_extended () स्थिर { वापस extended; }

  व्योम depend (hb_depend_तथ्य_builder_t *builder) स्थिर
  {
    अगर (!has_तथ्य ()) वापस;
    क्रम (hb_codepoint_t gid = 0; gid < get_अंक_glyphs (); gid++)
    {
      स्वतः glyph = glyph_for_gid (gid);
      क्रम (स्वतः &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  रक्षित:
  ढाँचा<typename T>
  bool get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<स्थिर पूर्णांक> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) स्थिर
  {
    अगर (gid >= अंक_glyphs) वापस false;

    int64_t ढेर_budget = HB_BUDGET_GLYPH;
    अगर (!budget) budget = &ढेर_budget;

    स्वतः &all_points = scratch.all_points;
    all_points.सफा ();

    bool phantom_only = !consumer.is_consuming_contour_points ();
    अगर (unlikely (!glyph_for_gid (gid).get_points (font, *यह, all_points, scratch, nullptr, nullptr, nullptr, true, true, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      वापस false;

    अगर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.लंबाई)))
      वापस false;

    अचिन्हित गिनती = all_points.लंबाई;
    निश्चित (गिनती >= glyf_impl::PHANTOM_गिनती);
    गिनती -= glyf_impl::PHANTOM_गिनती;

    अगर (consumer.is_consuming_contour_points ())
    {
      स्वतः *points = all_points.arrayZ;

      अगर (false)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	क्रम (अचिन्हित i = 0; i < गिनती; i++)
	{
	  consumer.consume_point (points[i]);
	  अगर (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      अन्यथा
      {
	क्रम (अचिन्हित i = 0; i < गिनती; i++)
	{
	  // Start of a contour.
	  अगर (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    क्रम (; i < गिनती; i++)
	    {
	      consumer.consume_point (points[i]);
	      अगर (points[i].is_end_point)
	      {
		consumer.contour_end ();
		अवरोध;
	      }
	    }
	  }
	  अन्यथा
	  {
	    अचिन्हित start = i;

	    // Find end of the contour.
	    क्रम (; i < गिनती; i++)
	      अगर (points[i].is_end_point)
		अवरोध;

	    अचिन्हित end = i;

	    अचिन्हित limit = end;
#अगर_अघोषित HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    अगर (likely (end < गिनती) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    अन्यथा
#पूर्ण_अगर
	    अगर (likely (end < गिनती))
	      consumer.consume_point (points[end]);

	    क्रम (i = start; i < limit; i++)
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
    अगर (phantoms)
      क्रम (अचिन्हित i = 0; i < glyf_impl::PHANTOM_गिनती; ++i)
	phantoms[i] = all_points.arrayZ[गिनती + i];

    वापस true;
  }

  खुला:

  काष्ठा points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    bool scaled;

    काष्ठा contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = भग्न_उच्च; max_x = max_y = -भग्न_उच्च; }

      व्योम add (स्थिर contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      bool empty () स्थिर { वापस (min_x >= max_x) || (min_y >= max_y); }

      व्योम get_extents (hb_font_t *font, hb_glyph_extents_t *extents, bool scaled)
      {
	अगर (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  वापस;
	}
	{
	  द्विगुन x_bearing = (द्विगुन) roundf (min_x);
	  द्विगुन y_bearing = (द्विगुन) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((द्विगुन) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((द्विगुन) roundf (min_y) - y_bearing);

	  अगर (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      रक्षित:
      भग्न min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, bool scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      अगर (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    व्योम consume_point (स्थिर contour_point_t &point) { bounds.add (point); }
    व्योम contour_end () {}
    व्योम points_end () { bounds.get_extents (font, extents, scaled); }

    bool is_consuming_contour_points () { वापस extents; }
    contour_point_t *get_phantoms_sink () { वापस phantoms; }
  };

#अगर_अघोषित HB_NO_VAR
  अचिन्हित
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 bool is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) स्थिर
  {
    अगर (unlikely (gid >= अंक_glyphs)) वापस 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_गिनती];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->अंक_coords : 0),
			  scratch, gvar_cache);
    अगर (unlikely (!success))
    {
      अचिन्हित upem = font->face->get_upem ();
      वापस is_vertical ? upem : upem / 2;
    }

    भग्न परिणाम = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_शीर्ष].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_दायाँ].x - phantoms[glyf_impl::PHANTOM_बायाँ].x;
    वापस hb_clamp (roundf (परिणाम), 0.f, (भग्न) अच_पूर्णांक_उच्च / 2);
  }
#पूर्ण_अगर

  भग्न
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) स्थिर
  {
    अगर (unlikely (gid >= अंक_glyphs)) वापस 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_गिनती];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->अंक_coords : 0),
			  scratch, gvar_cache);
    अगर (unlikely (!success))
    {
      वापस font->face->get_upem ();
    }

    वापस phantoms[glyf_impl::PHANTOM_शीर्ष].y;
  }

  खुला:

  bool get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) स्थिर
  { वापस get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->अंक_coords : 0)); }

  bool get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<स्थिर पूर्णांक> coords,
		       int64_t *budget = nullptr) स्थिर
  {
    अगर (unlikely (gid >= अंक_glyphs)) वापस false;

#अगर_अघोषित HB_NO_VAR
    अगर (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      अगर (unlikely (!scratch)) वापस false;
      bool ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, true),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      वापस ret;
    }
#पूर्ण_अगर
    वापस glyph_for_gid (gid).get_extents_without_var_scaled (font, *यह, extents);
  }

  स्थिर glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, bool needs_padding_removal = false) स्थिर
  {
    अगर (unlikely (gid >= अंक_glyphs)) वापस glyf_impl::Glyph ();

    अचिन्हित पूर्णांक start_offset, end_offset;

    अगर (short_offset)
    {
      स्थिर HBUINT16 *offsets = (स्थिर HBUINT16 *) loca_table->तथ्य_Z.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    अन्यथा
    {
      स्थिर HBUINT32 *offsets = (स्थिर HBUINT32 *) loca_table->तथ्य_Z.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    अगर (unlikely (start_offset > end_offset || end_offset > glyf_table.get_लंबाई ()))
      वापस glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((स्थिर अक्षर *) यह->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    वापस needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  bool
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) स्थिर
  {
    अगर (!has_तथ्य ()) वापस false;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    अगर (unlikely (!scratch)) वापस true;

    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->अंक_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    वापस ret;
  }

  bool
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<स्थिर पूर्णांक> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) स्थिर
  {
    अगर (!has_तथ्य ()) वापस false;
    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    वापस ret;
  }


  hb_glyf_scratch_t *acquire_scratch () स्थिर
  {
    अगर (!has_तथ्य ()) वापस nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    अगर (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, माप (hb_glyf_scratch_t));
      अगर (unlikely (!scratch))
	वापस nullptr;
    }
    वापस scratch;
  }
  व्योम release_scratch (hb_glyf_scratch_t *scratch) स्थिर
  {
    अगर (!scratch)
      वापस;
    अगर (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  अचिन्हित पूर्णांक get_अंक_glyphs () स्थिर { वापस अंक_glyphs; }
  hb_blob_t *reference_glyf_table () स्थिर
  { वापस hb_blob_reference (glyf_table.get_blob ()); }

#अगर_अघोषित HB_NO_VAR
  स्थिर gvar_accelerator_t *gvar;
#पूर्ण_अगर
  स्थिर hmtx_accelerator_t *hmtx;
#अगर_अघोषित HB_NO_VERTICAL
  स्थिर vmtx_accelerator_t *vmtx;
#पूर्ण_अगर

  निजी:
  bool extended;
  bool short_offset;
  अचिन्हित पूर्णांक अंक_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


अंतरभूत bool
glyf::_populate_subset_glyphs (स्थिर hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) स्थिर
{
  OT::glyf_accelerator_t glyf (plan->source);
  अगर (!glyphs.alloc_exact (plan->new_to_old_gid_list.लंबाई)) वापस false;
  *source_glyf = glyf.reference_glyf_table ();

  क्रम (स्थिर स्वतः &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    अगर (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    अन्यथा
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    अगर (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    अन्यथा
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    अगर (font)
    {
      अगर (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        अगर (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        वापस false;
      }
    }
  }
  वापस true;
}

अंतरभूत hb_font_t *
glyf::_create_font_for_instancing (स्थिर hb_subset_plan_t *plan) स्थिर
{
  hb_font_t *font = hb_font_create (plan->source);
  अगर (unlikely (font == hb_font_get_empty ())) वापस nullptr;

#अगर_अघोषित HB_NO_VAR
  अगर (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.लंबाई);
    वापस font;
  }
#पूर्ण_अगर

  hb_vector_t<hb_variation_t> vars;
  अगर (unlikely (!vars.alloc (plan->user_axes_location.get_population (), true)))
  {
    hb_font_destroy (font);
    वापस nullptr;
  }

  क्रम (स्वतः _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.मध्य;
    vars.push (var);
  }

#अगर_अघोषित HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#पूर्ण_अगर
  वापस font;
}


} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_GLYF_HH */
