#جے_بنا_تعریف OT_GLYF_GLYF_HH
#تعریف_کرو OT_GLYF_GLYF_HH


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
#تعریف_کرو HB_OT_ٹیگ_glyf HB_ٹیگ('g','l','y','f')
#تعریف_کرو HB_OT_ٹیگ_GLYF HB_ٹیگ('G','L','Y','F')

بنية glyf
{
  یار بنية glyf_accelerator_t;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_glyf;

  ساكن منطقي has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    أرجع سر_2.اشاریہ_منزل_Loc_Format <= 1 && سر_2.glyph_انکڑے_Format <= 1;
  }

  ساكن منطقي has_extended_glyf (hb_face_t *face)
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_ٹیگ_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_ٹیگ_LOCA);
    منطقي ret = GLYF_جدول.get_لمائی () && LOCA_جدول.get_لمائی ();
    GLYF_جدول.destroy ();
    LOCA_جدول.destroy ();
    أرجع ret;
#وإلا
    أرجع خطأ;
#جے_انت
  }

  منطقي sanitize (hb_sanitize_context_t *c HB_بن_ورتیا) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (سچ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  سانچا <قسم_ناں Iterator>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (هذا);

    بنانشانی init_len = c->لمائی ();
    لكل (خودکار &_ : it)
      إذا (unlikely (!_.serialize (c, use_short_loca, plan)))
        أرجع خطأ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    إذا (init_len == c->لمائی ())
    {
      HBUINT8 خالی_تھاواں_byte;
      خالی_تھاواں_byte = 0;
      c->copy (خالی_تھاواں_byte);
    }
    return_trace (سچ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    منطقي extended = has_extended_glyf (c->plan->سروت);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended != (c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GLYF))
      return_trace (خطأ);
#جے_انت

    إذا (!has_valid_glyf_format (c->plan->سروت)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_پیغام (SUBSET, خالی_سوچک,
                 "unkown glyf format, dropping from subset.");
      return_trace (خطأ);
    }

    hb_font_t *font = خالی_سوچک;
    إذا (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      إذا (unlikely (!font))
	return_trace (خطأ);
    }

    hb_سمتیہ_t<بنانشانی> padded_offsets;
    إذا (unlikely (!padded_offsets.alloc_exact (c->plan->new_منزل_old_gid_list.لمائی)))
    {
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    hb_سمتیہ_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *سروت_glyf = خالی_سوچک;
    إذا (!_populate_subset_glyphs (c->plan, font, glyphs, &سروت_glyf))
    {
      hb_blob_destroy (سروت_glyf);
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    إذا (font)
      hb_font_destroy (font);

    بنانشانی max_وتھ = 0;
    لكل (خودکار &g : glyphs)
    {
      بنانشانی ناپ = g.padded_ناپ ();
      padded_offsets.push (ناپ);
      max_وتھ += ناپ;
    }

    منطقي use_short_loca = خطأ;
    إذا (likely (!c->plan->force_long_loca))
      use_short_loca = max_وتھ < 0x1FFFF;

    إذا (!use_short_loca)
    {
      padded_offsets.clear ();
      لكل (خودکار &g : glyphs)
	padded_offsets.push (g.لمائی ());
    }

    خودکار *glyf_مفرد = c->serializer->شروع_embed <glyf> ();
    منطقي نتیجہ = glyf_مفرد->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    إذا (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (سروت_glyf);

    إذا (unlikely (!c->serializer->check_success (glyf_impl::جوڑو_loca_and_سر (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (خطأ);

    أرجع نتیجہ;
  }

  منطقي
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_سمتیہ_t<glyf_impl::SubsetGlyph>& glyphs /*باہرلا*/,
			   hb_blob_t               **سروت_glyf /*باہرلا*/) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  فراغ _free_compiled_subset_glyphs (hb_سمتیہ_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    لكل (خودکار &g : glyphs)
      g.free_compiled_بائٹاں ();
  }

  محمي:
  Unsized_Array_دا<HBUINT8>
		انکڑے_Z;	/* Glyphs data. */
  عام:
  DEFINE_ناپ_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#جے_بنا_تعریف HB_NO_BEYOND_64K
بنية GLYF : glyf
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_GLYF;
};
#جے_انت

بنية glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = خطأ;
    short_وتھ = خطأ;
    عدد_glyphs = 0;
    loca_جدول = خالی_سوچک;
    glyf_جدول = خالی_سوچک;
#جے_بنا_تعریف HB_NO_VAR
    gvar = خالی_سوچک;
#جے_انت
    hmtx = خالی_سوچک;
#جے_بنا_تعریف HB_NO_VERTICAL
    vmtx = خالی_سوچک;
#جے_انت
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    إذا (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      أرجع;
    short_وتھ = 0 == سر_2.اشاریہ_منزل_Loc_Format;

#جے_بنا_تعریف HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_ٹیگ_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_ٹیگ_LOCA);
    extended = GLYF_جدول.get_لمائی () && LOCA_جدول.get_لمائی ();
    إذا (extended)
    {
      glyf_جدول = std::move (GLYF_جدول);
      loca_جدول = std::move (LOCA_جدول);
    }
    وإلا
    {
      GLYF_جدول.destroy ();
      LOCA_جدول.destroy ();
    }
#جے_انت
    إذا (!extended)
    {
      glyf_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face);
      loca_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face);
    }
#جے_بنا_تعریف HB_NO_VAR
    gvar = face->جدول.gvar;
#جے_انت
    hmtx = face->جدول.hmtx;
#جے_بنا_تعریف HB_NO_VERTICAL
    vmtx = face->جدول.vmtx;
#جے_انت

    عدد_glyphs = hb_max (1u, loca_جدول.get_لمائی () / (short_وتھ ? 2 : 4)) - 1;
    عدد_glyphs = hb_min (عدد_glyphs, face->get_عدد_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    خودکار *scratch = cached_scratch.get_relaxed ();
    إذا (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_جدول.destroy ();
    loca_جدول.destroy ();
  }

  منطقي has_انکڑے () ثابت { أرجع عدد_glyphs; }
  منطقي is_extended () ثابت { أرجع extended; }

  فراغ depend (hb_depend_انکڑے_builder_t *builder) ثابت
  {
    إذا (!has_انکڑے ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < get_عدد_glyphs (); gid++)
    {
      خودکار glyph = glyph_for_gid (gid);
      لكل (خودکار &شے : glyph.get_مرکب_iterator ())
        builder->جوڑو_depend (gid,
                             extended ? HB_OT_ٹیگ_GLYF : HB_OT_ٹیگ_glyf,
                             شے.get_gid (extended));
    }
  }

  محمي:
  سانچا<قسم_ناں T>
  منطقي get_points (hb_font_t *font, hb_codepoint_t gid, T کھپتکار,
		   hb_array_t<ثابت صحيح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = خالی_سوچک,
		   int64_t *budget = خالی_سوچک) ثابت
  {
    إذا (gid >= عدد_glyphs) أرجع خطأ;

    int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ڈھیر_budget;

    خودکار &all_points = scratch.all_points;
    all_points.clear ();

    منطقي phantom_only = !کھپتکار.is_consuming_contour_points ();
    إذا (unlikely (!glyph_for_gid (gid).get_points (font, *هذا, all_points, scratch, خالی_سوچک, خالی_سوچک, خالی_سوچک, سچ, سچ, phantom_only, coords, gvar_cache, 0, خالی_سوچک, budget)))
      أرجع خطأ;

    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.لمائی)))
      أرجع خطأ;

    بنانشانی گنتی = all_points.لمائی;
    أكد (گنتی >= glyf_impl::PHANTOM_گنتی);
    گنتی -= glyf_impl::PHANTOM_گنتی;

    إذا (کھپتکار.is_consuming_contour_points ())
    {
      خودکار *points = all_points.arrayZ;

      إذا (خطأ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	لكل (بنانشانی i = 0; i < گنتی; i++)
	{
	  کھپتکار.consume_نقطہ (points[i]);
	  إذا (points[i].is_سرا_نقطہ)
	    کھپتکار.contour_سرا ();
	}
      }
      وإلا
      {
	لكل (بنانشانی i = 0; i < گنتی; i++)
	{
	  // Start of a contour.
	  إذا (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_ونگ)
	  {
	    // First point is on-curve. Draw the contour.
	    لكل (; i < گنتی; i++)
	    {
	      کھپتکار.consume_نقطہ (points[i]);
	      إذا (points[i].is_سرا_نقطہ)
	      {
		کھپتکار.contour_سرا ();
		اكسر;
	      }
	    }
	  }
	  وإلا
	  {
	    بنانشانی شروع = i;

	    // Find end of the contour.
	    لكل (; i < گنتی; i++)
	      إذا (points[i].is_سرا_نقطہ)
		اكسر;

	    بنانشانی سرا = i;

	    بنانشانی حد_بندی = سرا;
#جے_بنا_تعریف HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    إذا (likely (سرا < گنتی) &&
		(points[شروع].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[سرا].flag & (glyf_impl::SimpleGlyph::FLAG_ON_ونگ |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      حد_بندی++;
	    وإلا
#جے_انت
	    إذا (likely (سرا < گنتی))
	      کھپتکار.consume_نقطہ (points[سرا]);

	    لكل (i = شروع; i < حد_بندی; i++)
	      کھپتکار.consume_نقطہ (points[i]);
	    i = سرا;

	    کھپتکار.contour_سرا ();
	  }
	}
      }

      کھپتکار.points_سرا ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_نقطہ_t *phantoms = کھپتکار.get_phantoms_sink ();
    إذا (phantoms)
      لكل (بنانشانی i = 0; i < glyf_impl::PHANTOM_گنتی; ++i)
	phantoms[i] = all_points.arrayZ[گنتی + i];

    أرجع سچ;
  }

  عام:

  بنية points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_نقطہ_t *phantoms;
    منطقي scaled;

    بنية contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      فراغ جوڑو (ثابت contour_نقطہ_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقي خالی_تھاواں_2 () ثابت { أرجع (min_x >= max_x) || (min_y >= max_y); }

      فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scaled)
      {
	إذا (unlikely (خالی_تھاواں_2 ()))
	{
	  extents->چوڑائی = 0;
	  extents->x_bearing = 0;
	  extents->اچائی = 0;
	  extents->y_bearing = 0;
	  أرجع;
	}
	{
	  مزدوج x_bearing = (مزدوج) roundf (min_x);
	  مزدوج y_bearing = (مزدوج) roundf (max_y);
	  extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
	  extents->چوڑائی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
	  extents->اچائی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (min_y) - y_bearing);

	  إذا (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محمي:
      عائم min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_نقطہ_t *phantoms_, منطقي scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      إذا (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    فراغ consume_نقطہ (ثابت contour_نقطہ_t &نقطہ) { bounds.جوڑو (نقطہ); }
    فراغ contour_سرا () {}
    فراغ points_سرا () { bounds.get_extents (font, extents, scaled); }

    منطقي is_consuming_contour_points () { أرجع extents; }
    contour_نقطہ_t *get_phantoms_sink () { أرجع phantoms; }
  };

#جے_بنا_تعریف HB_NO_VAR
  بنانشانی
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقي is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطہ_t phantoms[glyf_impl::PHANTOM_گنتی];
    success = get_points (font, gid, points_aggregator_t (font, خالی_سوچک, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      بنانشانی upem = font->face->get_upem ();
      أرجع is_vertical ? upem : upem / 2;
    }

    عائم نتیجہ = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_چوٹی].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_دایاں].x - phantoms[glyf_impl::PHANTOM_بایاں].x;
    أرجع hb_clamp (roundf (نتیجہ), 0.f, (عائم) UINT_MAX / 2);
  }
#جے_انت

  عائم
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطہ_t phantoms[glyf_impl::PHANTOM_گنتی];
    success = get_points (font, gid, points_aggregator_t (font, خالی_سوچک, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      أرجع font->face->get_upem ();
    }

    أرجع phantoms[glyf_impl::PHANTOM_چوٹی].y;
  }

  عام:

  منطقي get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { أرجع get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->عدد_coords : 0)); }

  منطقي get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحيح> coords,
		       int64_t *budget = خالی_سوچک) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع خطأ;

#جے_بنا_تعریف HB_NO_VAR
    إذا (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع خطأ;
      منطقي ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, خالی_سوچک, سچ),
			     coords,
			     *scratch,
			     خالی_سوچک,
			     budget);
      release_scratch (scratch);
      أرجع ret;
    }
#جے_انت
    أرجع glyph_for_gid (gid).get_extents_without_var_scaled (font, *هذا, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقي needs_padding_removal = خطأ) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع glyf_impl::Glyph ();

    بنانشانی صحيح شروع_وتھ, سرا_وتھ;

    إذا (short_وتھ)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_جدول->انکڑے_Z.arrayZ;
      شروع_وتھ = 2 * offsets[gid];
      سرا_وتھ   = 2 * offsets[gid + 1];
    }
    وإلا
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_جدول->انکڑے_Z.arrayZ;
      شروع_وتھ = offsets[gid];
      سرا_وتھ   = offsets[gid + 1];
    }

    إذا (unlikely (شروع_وتھ > سرا_وتھ || سرا_وتھ > glyf_جدول.get_لمائی ()))
      أرجع glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_بائٹاں_t ((ثابت حرف *) هذا->glyf_جدول + شروع_وتھ,
			     سرا_وتھ - شروع_وتھ), gid, extended);
    أرجع needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقي
  get_رستہ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = خالی_سوچک,
	    int64_t *budget = خالی_سوچک) ثابت
  {
    إذا (!has_انکڑے ()) أرجع خطأ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    إذا (unlikely (!scratch)) أرجع سچ;

    منطقي ret = get_points (font, gid, glyf_impl::رستہ_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->عدد_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    أرجع ret;
  }

  منطقي
  get_رستہ_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحيح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = خالی_سوچک,
	       int64_t *budget = خالی_سوچک) ثابت
  {
    إذا (!has_انکڑے ()) أرجع خطأ;
    منطقي ret = get_points (font, gid, glyf_impl::رستہ_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    أرجع ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    إذا (!has_انکڑے ()) أرجع خالی_سوچک;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_سوچک)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      إذا (unlikely (!scratch))
	أرجع خالی_سوچک;
    }
    أرجع scratch;
  }
  فراغ release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    إذا (!scratch)
      أرجع;
    إذا (!cached_scratch.cmpexch (خالی_سوچک, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  بنانشانی صحيح get_عدد_glyphs () ثابت { أرجع عدد_glyphs; }
  hb_blob_t *reference_glyf_جدول () ثابت
  { أرجع hb_blob_reference (glyf_جدول.get_blob ()); }

#جے_بنا_تعریف HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#جے_انت
  ثابت hmtx_accelerator_t *hmtx;
#جے_بنا_تعریف HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#جے_انت

  خاص:
  منطقي extended;
  منطقي short_وتھ;
  بنانشانی صحيح عدد_glyphs;
  hb_blob_ptr_t<loca> loca_جدول;
  hb_blob_ptr_t<glyf> glyf_جدول;
  بدلنجوگ hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


مضمن منطقي
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_سمتیہ_t<glyf_impl::SubsetGlyph>& glyphs /*باہرلا*/,
			       hb_blob_t **سروت_glyf /*باہرلا*/) ثابت
{
  OT::glyf_accelerator_t glyf (plan->سروت);
  إذا (!glyphs.alloc_exact (plan->new_منزل_old_gid_list.لمائی)) أرجع خطأ;
  *سروت_glyf = glyf.reference_glyf_جدول ();

  لكل (ثابت خودکار &pair : plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.پہلا;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    إذا (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.سروت_glyph = glyf_impl::Glyph ();
    وإلا
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.سروت_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_بائٹاں ();
    وإلا
      subset_glyph.dest_شروع = subset_glyph.سروت_glyph.get_بائٹاں ();

    إذا (font)
    {
      إذا (unlikely (!subset_glyph.compile_بائٹاں_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        إذا (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        أرجع خطأ;
      }
    }
  }
  أرجع سچ;
}

مضمن hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->سروت);
  إذا (unlikely (font == hb_font_get_خالی_تھاواں ())) أرجع خالی_سوچک;

#جے_بنا_تعریف HB_NO_VAR
  إذا (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_سیٹ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.لمائی);
    أرجع font;
  }
#جے_انت

  hb_سمتیہ_t<hb_variation_t> vars;
  إذا (unlikely (!vars.alloc (plan->user_axes_location.get_population (), سچ)))
  {
    hb_font_destroy (font);
    أرجع خالی_سوچک;
  }

  لكل (خودکار _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.ٹیگ_2 = _.پہلا;
    var.value = _.second.درمیان;
    vars.push (var);
  }

#جے_بنا_تعریف HB_NO_VAR
  hb_font_سیٹ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#جے_انت
  أرجع font;
}


} /* namespace OT */


#جے_انت /* OT_GLYF_GLYF_HH */
