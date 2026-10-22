#నిర్వచితం_కాకపోతే OT_GLYF_GLYF_HH
#నిర్వచించు OT_GLYF_GLYF_HH


#అన్తర్భావ "../../hb-open-type.hh"
#అన్తర్భావ "../../hb-depend-data.hh"
#అన్తర్భావ "../../hb-ot-head-table.hh"
#అన్తర్భావ "../../hb-ot-hmtx-table.hh"
#అన్తర్భావ "../../hb-ot-var-gvar-table.hh"
#అన్తర్భావ "../../hb-draw.hh"
#అన్తర్భావ "../../hb-paint.hh"

#అన్తర్భావ "glyf-helpers.hh"
#అన్తర్భావ "Glyph.hh"
#అన్తర్భావ "SubsetGlyph.hh"
#అన్తర్భావ "loca.hh"
#అన్తర్భావ "path-builder.hh"


నామాకాశ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#నిర్వచించు HB_OT_ట్యాగ్_glyf HB_ట్యాగ్('g','l','y','f')
#నిర్వచించు HB_OT_ట్యాగ్_GLYF HB_ట్యాగ్('G','L','Y','F')

నిర్మాణం glyf
{
  మిత్ర నిర్మాణం glyf_accelerator_t;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_glyf;

  స్థిర తార్కిక has_valid_glyf_format(స్థిరరాశి hb_face_t* face)
  {
    స్థిరరాశి OT::తల_2 &తల_2 = *face->పట్టిక.తల_2;
    తిరిగి తల_2.సూచీ_గమ్యం_Loc_Format <= 1 && తల_2.glyph_దత్తాంశం_Format <= 1;
  }

  స్థిర తార్కిక has_extended_glyf (hb_face_t *face)
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<glyf> (face, HB_OT_ట్యాగ్_GLYF);
    hb_blob_ptr_t<loca> LOCA_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<loca> (face, HB_OT_ట్యాగ్_LOCA);
    తార్కిక ret = GLYF_పట్టిక.get_నిడివి () && LOCA_పట్టిక.get_నిడివి ();
    GLYF_పట్టిక.destroy ();
    LOCA_పట్టిక.destroy ();
    తిరిగి ret;
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
  }

  తార్కిక sanitize (hb_sanitize_context_t *c HB_వాడని) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (నిజం);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  మూస <రకం_పేరు Iterator>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it,
                  తార్కిక use_short_loca,
		  స్థిరరాశి hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (ఇది);

    గుర్తులేని init_len = c->నిడివి ();
    కొరకు (స్వయంచాలక &_ : it)
      ఒకవేళ (unlikely (!_.serialize (c, use_short_loca, plan)))
        తిరిగి అబద్ధం;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ఒకవేళ (init_len == c->నిడివి ())
    {
      HBUINT8 ఖాళీలు_byte;
      ఖాళీలు_byte = 0;
      c->copy (ఖాళీలు_byte);
    }
    return_trace (నిజం);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    తార్కిక extended = has_extended_glyf (c->plan->ఆధారం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended != (c->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GLYF))
      return_trace (అబద్ధం);
#అయితే_ముగింపు

    ఒకవేళ (!has_valid_glyf_format (c->plan->ఆధారం)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
                 "unkown glyf format, dropping from subset.");
      return_trace (అబద్ధం);
    }

    hb_font_t *font = శూన్యసూచిక;
    ఒకవేళ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ఒకవేళ (unlikely (!font))
	return_trace (అబద్ధం);
    }

    hb_సదిశ_t<గుర్తులేని> padded_offsets;
    ఒకవేళ (unlikely (!padded_offsets.alloc_exact (c->plan->new_గమ్యం_old_gid_list.నిడివి)))
    {
      hb_font_destroy (font);
      return_trace (అబద్ధం);
    }

    hb_సదిశ_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *ఆధారం_glyf = శూన్యసూచిక;
    ఒకవేళ (!_populate_subset_glyphs (c->plan, font, glyphs, &ఆధారం_glyf))
    {
      hb_blob_destroy (ఆధారం_glyf);
      hb_font_destroy (font);
      return_trace (అబద్ధం);
    }

    ఒకవేళ (font)
      hb_font_destroy (font);

    గుర్తులేని max_స్థానభ్రంశం = 0;
    కొరకు (స్వయంచాలక &g : glyphs)
    {
      గుర్తులేని కొలత = g.padded_కొలత ();
      padded_offsets.push (కొలత);
      max_స్థానభ్రంశం += కొలత;
    }

    తార్కిక use_short_loca = అబద్ధం;
    ఒకవేళ (likely (!c->plan->force_long_loca))
      use_short_loca = max_స్థానభ్రంశం < 0x1FFFF;

    ఒకవేళ (!use_short_loca)
    {
      padded_offsets.clear ();
      కొరకు (స్వయంచాలక &g : glyphs)
	padded_offsets.push (g.నిడివి ());
    }

    స్వయంచాలక *glyf_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed <glyf> ();
    తార్కిక ఫలితం = glyf_ప్రధానసంఖ్య->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ఒకవేళ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (ఆధారం_glyf);

    ఒకవేళ (unlikely (!c->serializer->check_success (glyf_impl::కూడు_loca_and_తల (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (అబద్ధం);

    తిరిగి ఫలితం;
  }

  తార్కిక
  _populate_subset_glyphs (స్థిరరాశి hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_సదిశ_t<glyf_impl::SubsetGlyph>& glyphs /*బహిర్గతం*/,
			   hb_blob_t               **ఆధారం_glyf /*బహిర్గతం*/) స్థిరరాశి;

  hb_font_t *
  _create_font_for_instancing (స్థిరరాశి hb_subset_plan_t *plan) స్థిరరాశి;

  శూన్య _free_compiled_subset_glyphs (hb_సదిశ_t<glyf_impl::SubsetGlyph> &glyphs) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక &g : glyphs)
      g.free_compiled_బైట్లు ();
  }

  రక్షిత:
  Unsized_Array_యొక్క<HBUINT8>
		దత్తాంశం_Z;	/* Glyphs data. */
  బహిరంగ:
  DEFINE_కొలత_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
నిర్మాణం GLYF : glyf
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_GLYF;
};
#అయితే_ముగింపు

నిర్మాణం glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = అబద్ధం;
    short_స్థానభ్రంశం = అబద్ధం;
    అంకె_glyphs = 0;
    loca_పట్టిక = శూన్యసూచిక;
    glyf_పట్టిక = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_VAR
    gvar = శూన్యసూచిక;
#అయితే_ముగింపు
    hmtx = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    vmtx = శూన్యసూచిక;
#అయితే_ముగింపు
    స్థిరరాశి OT::తల_2 &తల_2 = *face->పట్టిక.తల_2;
    ఒకవేళ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      తిరిగి;
    short_స్థానభ్రంశం = 0 == తల_2.సూచీ_గమ్యం_Loc_Format;

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<glyf> (face, HB_OT_ట్యాగ్_GLYF);
    hb_blob_ptr_t<loca> LOCA_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<loca> (face, HB_OT_ట్యాగ్_LOCA);
    extended = GLYF_పట్టిక.get_నిడివి () && LOCA_పట్టిక.get_నిడివి ();
    ఒకవేళ (extended)
    {
      glyf_పట్టిక = std::move (GLYF_పట్టిక);
      loca_పట్టిక = std::move (LOCA_పట్టిక);
    }
    లేకపోతే
    {
      GLYF_పట్టిక.destroy ();
      LOCA_పట్టిక.destroy ();
    }
#అయితే_ముగింపు
    ఒకవేళ (!extended)
    {
      glyf_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<glyf> (face);
      loca_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<loca> (face);
    }
#నిర్వచితం_కాకపోతే HB_NO_VAR
    gvar = face->పట్టిక.gvar;
#అయితే_ముగింపు
    hmtx = face->పట్టిక.hmtx;
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    vmtx = face->పట్టిక.vmtx;
#అయితే_ముగింపు

    అంకె_glyphs = hb_max (1u, loca_పట్టిక.get_నిడివి () / (short_స్థానభ్రంశం ? 2 : 4)) - 1;
    అంకె_glyphs = hb_min (అంకె_glyphs, face->get_అంకె_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    స్వయంచాలక *scratch = cached_scratch.get_relaxed ();
    ఒకవేళ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_పట్టిక.destroy ();
    loca_పట్టిక.destroy ();
  }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి అంకె_glyphs; }
  తార్కిక is_extended () స్థిరరాశి { తిరిగి extended; }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *builder) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి;
    కొరకు (hb_codepoint_t gid = 0; gid < get_అంకె_glyphs (); gid++)
    {
      స్వయంచాలక glyph = glyph_for_gid (gid);
      కొరకు (స్వయంచాలక &అంశం : glyph.get_సంయుక్త_iterator ())
        builder->కూడు_depend (gid,
                             extended ? HB_OT_ట్యాగ్_GLYF : HB_OT_ట్యాగ్_glyf,
                             అంశం.get_gid (extended));
    }
  }

  రక్షిత:
  మూస<రకం_పేరు T>
  తార్కిక get_points (hb_font_t *font, hb_codepoint_t gid, T వినియోగదారు,
		   hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
		   int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (gid >= అంకె_glyphs) తిరిగి అబద్ధం;

    int64_t దొంతర_budget = HB_BUDGET_GLYPH;
    ఒకవేళ (!budget) budget = &దొంతర_budget;

    స్వయంచాలక &all_points = scratch.all_points;
    all_points.clear ();

    తార్కిక phantom_only = !వినియోగదారు.is_consuming_contour_points ();
    ఒకవేళ (unlikely (!glyph_for_gid (gid).get_points (font, *ఇది, all_points, scratch, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక, నిజం, నిజం, phantom_only, coords, gvar_cache, 0, శూన్యసూచిక, budget)))
      తిరిగి అబద్ధం;

    ఒకవేళ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.నిడివి)))
      తిరిగి అబద్ధం;

    గుర్తులేని లెక్క = all_points.నిడివి;
    ప్రతిజ్ఞా (లెక్క >= glyf_impl::PHANTOM_లెక్క);
    లెక్క -= glyf_impl::PHANTOM_లెక్క;

    ఒకవేళ (వినియోగదారు.is_consuming_contour_points ())
    {
      స్వయంచాలక *points = all_points.arrayZ;

      ఒకవేళ (అబద్ధం)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	{
	  వినియోగదారు.consume_బిందువు (points[i]);
	  ఒకవేళ (points[i].is_చివర_బిందువు)
	    వినియోగదారు.contour_చివర ();
	}
      }
      లేకపోతే
      {
	కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	{
	  // Start of a contour.
	  ఒకవేళ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_వక్రం)
	  {
	    // First point is on-curve. Draw the contour.
	    కొరకు (; i < లెక్క; i++)
	    {
	      వినియోగదారు.consume_బిందువు (points[i]);
	      ఒకవేళ (points[i].is_చివర_బిందువు)
	      {
		వినియోగదారు.contour_చివర ();
		ఆపు;
	      }
	    }
	  }
	  లేకపోతే
	  {
	    గుర్తులేని ప్రారంభం = i;

	    // Find end of the contour.
	    కొరకు (; i < లెక్క; i++)
	      ఒకవేళ (points[i].is_చివర_బిందువు)
		ఆపు;

	    గుర్తులేని చివర = i;

	    గుర్తులేని హద్దు = చివర;
#నిర్వచితం_కాకపోతే HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ఒకవేళ (likely (చివర < లెక్క) &&
		(points[ప్రారంభం].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[చివర].flag & (glyf_impl::SimpleGlyph::FLAG_ON_వక్రం |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      హద్దు++;
	    లేకపోతే
#అయితే_ముగింపు
	    ఒకవేళ (likely (చివర < లెక్క))
	      వినియోగదారు.consume_బిందువు (points[చివర]);

	    కొరకు (i = ప్రారంభం; i < హద్దు; i++)
	      వినియోగదారు.consume_బిందువు (points[i]);
	    i = చివర;

	    వినియోగదారు.contour_చివర ();
	  }
	}
      }

      వినియోగదారు.points_చివర ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_బిందువు_t *phantoms = వినియోగదారు.get_phantoms_sink ();
    ఒకవేళ (phantoms)
      కొరకు (గుర్తులేని i = 0; i < glyf_impl::PHANTOM_లెక్క; ++i)
	phantoms[i] = all_points.arrayZ[లెక్క + i];

    తిరిగి నిజం;
  }

  బహిరంగ:

  నిర్మాణం points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_బిందువు_t *phantoms;
    తార్కిక scaled;

    నిర్మాణం contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      శూన్య కూడు (స్థిరరాశి contour_బిందువు_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      తార్కిక ఖాళీలు_2 () స్థిరరాశి { తిరిగి (min_x >= max_x) || (min_y >= max_y); }

      శూన్య get_extents (hb_font_t *font, hb_glyph_extents_t *extents, తార్కిక scaled)
      {
	ఒకవేళ (unlikely (ఖాళీలు_2 ()))
	{
	  extents->వెడల్పు = 0;
	  extents->x_bearing = 0;
	  extents->ఎత్తు = 0;
	  extents->y_bearing = 0;
	  తిరిగి;
	}
	{
	  ద్విగుణ x_bearing = (ద్విగుణ) roundf (min_x);
	  ద్విగుణ y_bearing = (ద్విగుణ) roundf (max_y);
	  extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (x_bearing);
	  extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (y_bearing);
	  extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) roundf (min_y) - y_bearing);

	  ఒకవేళ (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      రక్షిత:
      దశాంశం min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_బిందువు_t *phantoms_, తార్కిక scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      ఒకవేళ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    శూన్య consume_బిందువు (స్థిరరాశి contour_బిందువు_t &బిందువు) { bounds.కూడు (బిందువు); }
    శూన్య contour_చివర () {}
    శూన్య points_చివర () { bounds.get_extents (font, extents, scaled); }

    తార్కిక is_consuming_contour_points () { తిరిగి extents; }
    contour_బిందువు_t *get_phantoms_sink () { తిరిగి phantoms; }
  };

#నిర్వచితం_కాకపోతే HB_NO_VAR
  గుర్తులేని
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 తార్కిక is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (gid >= అంకె_glyphs)) తిరిగి 0;

    తార్కిక success = అబద్ధం;

    contour_బిందువు_t phantoms[glyf_impl::PHANTOM_లెక్క];
    success = get_points (font, gid, points_aggregator_t (font, శూన్యసూచిక, phantoms, అబద్ధం),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->అంకె_coords : 0),
			  scratch, gvar_cache);
    ఒకవేళ (unlikely (!success))
    {
      గుర్తులేని upem = font->face->get_upem ();
      తిరిగి is_vertical ? upem : upem / 2;
    }

    దశాంశం ఫలితం = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_శిఖరం].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_కుడి].x - phantoms[glyf_impl::PHANTOM_ఎడమ].x;
    తిరిగి hb_clamp (roundf (ఫలితం), 0.f, (దశాంశం) UINT_MAX / 2);
  }
#అయితే_ముగింపు

  దశాంశం
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (gid >= అంకె_glyphs)) తిరిగి 0;

    తార్కిక success = అబద్ధం;

    contour_బిందువు_t phantoms[glyf_impl::PHANTOM_లెక్క];
    success = get_points (font, gid, points_aggregator_t (font, శూన్యసూచిక, phantoms, అబద్ధం),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->అంకె_coords : 0),
			  scratch, gvar_cache);
    ఒకవేళ (unlikely (!success))
    {
      తిరిగి font->face->get_upem ();
    }

    తిరిగి phantoms[glyf_impl::PHANTOM_శిఖరం].y;
  }

  బహిరంగ:

  తార్కిక get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) స్థిరరాశి
  { తిరిగి get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->అంకె_coords : 0)); }

  తార్కిక get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
		       int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (gid >= అంకె_glyphs)) తిరిగి అబద్ధం;

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ఒకవేళ (unlikely (!scratch)) తిరిగి అబద్ధం;
      తార్కిక ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, శూన్యసూచిక, నిజం),
			     coords,
			     *scratch,
			     శూన్యసూచిక,
			     budget);
      release_scratch (scratch);
      తిరిగి ret;
    }
#అయితే_ముగింపు
    తిరిగి glyph_for_gid (gid).get_extents_without_var_scaled (font, *ఇది, extents);
  }

  స్థిరరాశి glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, తార్కిక needs_padding_removal = అబద్ధం) స్థిరరాశి
  {
    ఒకవేళ (unlikely (gid >= అంకె_glyphs)) తిరిగి glyf_impl::Glyph ();

    గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం, చివర_స్థానభ్రంశం;

    ఒకవేళ (short_స్థానభ్రంశం)
    {
      స్థిరరాశి HBUINT16 *offsets = (స్థిరరాశి HBUINT16 *) loca_పట్టిక->దత్తాంశం_Z.arrayZ;
      ప్రారంభం_స్థానభ్రంశం = 2 * offsets[gid];
      చివర_స్థానభ్రంశం   = 2 * offsets[gid + 1];
    }
    లేకపోతే
    {
      స్థిరరాశి HBUINT32 *offsets = (స్థిరరాశి HBUINT32 *) loca_పట్టిక->దత్తాంశం_Z.arrayZ;
      ప్రారంభం_స్థానభ్రంశం = offsets[gid];
      చివర_స్థానభ్రంశం   = offsets[gid + 1];
    }

    ఒకవేళ (unlikely (ప్రారంభం_స్థానభ్రంశం > చివర_స్థానభ్రంశం || చివర_స్థానభ్రంశం > glyf_పట్టిక.get_నిడివి ()))
      తిరిగి glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_బైట్లు_t ((స్థిరరాశి అక్షరం *) ఇది->glyf_పట్టిక + ప్రారంభం_స్థానభ్రంశం,
			     చివర_స్థానభ్రంశం - ప్రారంభం_స్థానభ్రంశం), gid, extended);
    తిరిగి needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  తార్కిక
  get_మార్గం (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
	    int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి అబద్ధం;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ఒకవేళ (unlikely (!scratch)) తిరిగి నిజం;

    తార్కిక ret = get_points (font, gid, glyf_impl::మార్గం_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->అంకె_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    తిరిగి ret;
  }

  తార్కిక
  get_మార్గం_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = శూన్యసూచిక,
	       int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి అబద్ధం;
    తార్కిక ret = get_points (font, gid, glyf_impl::మార్గం_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    తిరిగి ret;
  }


  hb_glyf_scratch_t *acquire_scratch () స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి శూన్యసూచిక;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ఒకవేళ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, శూన్యసూచిక)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, పరిమాణం (hb_glyf_scratch_t));
      ఒకవేళ (unlikely (!scratch))
	తిరిగి శూన్యసూచిక;
    }
    తిరిగి scratch;
  }
  శూన్య release_scratch (hb_glyf_scratch_t *scratch) స్థిరరాశి
  {
    ఒకవేళ (!scratch)
      తిరిగి;
    ఒకవేళ (!cached_scratch.cmpexch (శూన్యసూచిక, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  గుర్తులేని పూర్ణాంకం get_అంకె_glyphs () స్థిరరాశి { తిరిగి అంకె_glyphs; }
  hb_blob_t *reference_glyf_పట్టిక () స్థిరరాశి
  { తిరిగి hb_blob_reference (glyf_పట్టిక.get_blob ()); }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  స్థిరరాశి gvar_accelerator_t *gvar;
#అయితే_ముగింపు
  స్థిరరాశి hmtx_accelerator_t *hmtx;
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
  స్థిరరాశి vmtx_accelerator_t *vmtx;
#అయితే_ముగింపు

  అంతరంగ:
  తార్కిక extended;
  తార్కిక short_స్థానభ్రంశం;
  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  hb_blob_ptr_t<loca> loca_పట్టిక;
  hb_blob_ptr_t<glyf> glyf_పట్టిక;
  మార్చదగిన hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


అంతఃవరుస తార్కిక
glyf::_populate_subset_glyphs (స్థిరరాశి hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_సదిశ_t<glyf_impl::SubsetGlyph>& glyphs /*బహిర్గతం*/,
			       hb_blob_t **ఆధారం_glyf /*బహిర్గతం*/) స్థిరరాశి
{
  OT::glyf_accelerator_t glyf (plan->ఆధారం);
  ఒకవేళ (!glyphs.alloc_exact (plan->new_గమ్యం_old_gid_list.నిడివి)) తిరిగి అబద్ధం;
  *ఆధారం_glyf = glyf.reference_glyf_పట్టిక ();

  కొరకు (స్థిరరాశి స్వయంచాలక &pair : plan->new_గమ్యం_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.మొదటి;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ఒకవేళ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.ఆధారం_glyph = glyf_impl::Glyph ();
    లేకపోతే
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.ఆధారం_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ఒకవేళ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_బైట్లు ();
    లేకపోతే
      subset_glyph.dest_ప్రారంభం = subset_glyph.ఆధారం_glyph.get_బైట్లు ();

    ఒకవేళ (font)
    {
      ఒకవేళ (unlikely (!subset_glyph.compile_బైట్లు_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ఒకవేళ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        తిరిగి అబద్ధం;
      }
    }
  }
  తిరిగి నిజం;
}

అంతఃవరుస hb_font_t *
glyf::_create_font_for_instancing (స్థిరరాశి hb_subset_plan_t *plan) స్థిరరాశి
{
  hb_font_t *font = hb_font_create (plan->ఆధారం);
  ఒకవేళ (unlikely (font == hb_font_get_ఖాళీలు ())) తిరిగి శూన్యసూచిక;

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_సమితి_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.నిడివి);
    తిరిగి font;
  }
#అయితే_ముగింపు

  hb_సదిశ_t<hb_variation_t> vars;
  ఒకవేళ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), నిజం)))
  {
    hb_font_destroy (font);
    తిరిగి శూన్యసూచిక;
  }

  కొరకు (స్వయంచాలక _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.ట్యాగ్_2 = _.మొదటి;
    var.value = _.second.మధ్య;
    vars.push (var);
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  hb_font_సమితి_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#అయితే_ముగింపు
  తిరిగి font;
}


} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_GLYF_HH */
