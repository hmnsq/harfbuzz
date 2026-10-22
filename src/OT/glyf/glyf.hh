#اگر_تعریف_نشده OT_GLYF_GLYF_HH
#تعریف_کن OT_GLYF_GLYF_HH


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
#تعریف_کن HB_OT_برچسب_glyf HB_برچسب('g','l','y','f')
#تعریف_کن HB_OT_برچسب_GLYF HB_برچسب('G','L','Y','F')

بنية glyf
{
  دوست بنية glyf_accelerator_t;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_glyf;

  ساكن منطقي has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    أرجع سر_2.شاخص_مقصد_Loc_Format <= 1 && سر_2.glyph_داده_Format <= 1;
  }

  ساكن منطقي has_extended_glyf (hb_face_t *face)
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_برچسب_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_برچسب_LOCA);
    منطقي ret = GLYF_جدول.get_درازی () && LOCA_جدول.get_درازی ();
    GLYF_جدول.destroy ();
    LOCA_جدول.destroy ();
    أرجع ret;
#وإلا
    أرجع خطأ;
#پایان_اگر
  }

  منطقي sanitize (hb_sanitize_context_t *c HB_استفادهنشده) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (درست);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  قالب <نام_نوع Iterator>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (هذا);

    بیعلامت init_len = c->درازی ();
    لكل (خودکار &_ : it)
      إذا (unlikely (!_.serialize (c, use_short_loca, plan)))
        أرجع خطأ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    إذا (init_len == c->درازی ())
    {
      HBUINT8 خالی_byte;
      خالی_byte = 0;
      c->copy (خالی_byte);
    }
    return_trace (درست);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    منطقي extended = has_extended_glyf (c->plan->منبع);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended != (c->جدول_برچسب_2 == HB_OT_برچسب_GLYF))
      return_trace (خطأ);
#پایان_اگر

    إذا (!has_valid_glyf_format (c->plan->منبع)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_پیام (SUBSET, اشاره_تهی,
                 "unkown glyf format, dropping from subset.");
      return_trace (خطأ);
    }

    hb_font_t *font = اشاره_تهی;
    إذا (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      إذا (unlikely (!font))
	return_trace (خطأ);
    }

    hb_وکتور_t<بیعلامت> padded_offsets;
    إذا (unlikely (!padded_offsets.alloc_exact (c->plan->new_مقصد_old_gid_list.درازی)))
    {
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    hb_وکتور_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *منبع_glyf = اشاره_تهی;
    إذا (!_populate_subset_glyphs (c->plan, font, glyphs, &منبع_glyf))
    {
      hb_blob_destroy (منبع_glyf);
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    إذا (font)
      hb_font_destroy (font);

    بیعلامت max_جابه_جایی = 0;
    لكل (خودکار &g : glyphs)
    {
      بیعلامت اندازه = g.padded_اندازه ();
      padded_offsets.push (اندازه);
      max_جابه_جایی += اندازه;
    }

    منطقي use_short_loca = خطأ;
    إذا (likely (!c->plan->force_long_loca))
      use_short_loca = max_جابه_جایی < 0x1FFFF;

    إذا (!use_short_loca)
    {
      padded_offsets.clear ();
      لكل (خودکار &g : glyphs)
	padded_offsets.push (g.درازی ());
    }

    خودکار *glyf_اول = c->serializer->شروع_embed <glyf> ();
    منطقي نتیجه = glyf_اول->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    إذا (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (منبع_glyf);

    إذا (unlikely (!c->serializer->check_success (glyf_impl::جمع_کن_loca_and_سر (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (خطأ);

    أرجع نتیجه;
  }

  منطقي
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_وکتور_t<glyf_impl::SubsetGlyph>& glyphs /*بیرون*/,
			   hb_blob_t               **منبع_glyf /*بیرون*/) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  فراغ _free_compiled_subset_glyphs (hb_وکتور_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    لكل (خودکار &g : glyphs)
      g.free_compiled_بایتها ();
  }

  محمي:
  Unsized_Array_از<HBUINT8>
		داده_Z;	/* Glyphs data. */
  عام:
  DEFINE_اندازه_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#اگر_تعریف_نشده HB_NO_BEYOND_64K
بنية GLYF : glyf
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_GLYF;
};
#پایان_اگر

بنية glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = خطأ;
    short_جابه_جایی = خطأ;
    رقم_glyphs = 0;
    loca_جدول = اشاره_تهی;
    glyf_جدول = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_VAR
    gvar = اشاره_تهی;
#پایان_اگر
    hmtx = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_VERTICAL
    vmtx = اشاره_تهی;
#پایان_اگر
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    إذا (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      أرجع;
    short_جابه_جایی = 0 == سر_2.شاخص_مقصد_Loc_Format;

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_برچسب_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_برچسب_LOCA);
    extended = GLYF_جدول.get_درازی () && LOCA_جدول.get_درازی ();
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
#پایان_اگر
    إذا (!extended)
    {
      glyf_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face);
      loca_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face);
    }
#اگر_تعریف_نشده HB_NO_VAR
    gvar = face->جدول.gvar;
#پایان_اگر
    hmtx = face->جدول.hmtx;
#اگر_تعریف_نشده HB_NO_VERTICAL
    vmtx = face->جدول.vmtx;
#پایان_اگر

    رقم_glyphs = hb_max (1u, loca_جدول.get_درازی () / (short_جابه_جایی ? 2 : 4)) - 1;
    رقم_glyphs = hb_min (رقم_glyphs, face->get_رقم_glyphs ());
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

  منطقي has_داده () ثابت { أرجع رقم_glyphs; }
  منطقي is_extended () ثابت { أرجع extended; }

  فراغ depend (hb_depend_داده_builder_t *builder) ثابت
  {
    إذا (!has_داده ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < get_رقم_glyphs (); gid++)
    {
      خودکار glyph = glyph_for_gid (gid);
      لكل (خودکار &قلم : glyph.get_مرکب_iterator ())
        builder->جمع_کن_depend (gid,
                             extended ? HB_OT_برچسب_GLYF : HB_OT_برچسب_glyf,
                             قلم.get_gid (extended));
    }
  }

  محمي:
  قالب<نام_نوع T>
  منطقي get_points (hb_font_t *font, hb_codepoint_t gid, T مصرفکننده,
		   hb_array_t<ثابت صحيح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = اشاره_تهی,
		   int64_t *budget = اشاره_تهی) ثابت
  {
    إذا (gid >= رقم_glyphs) أرجع خطأ;

    int64_t پشته_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &پشته_budget;

    خودکار &all_points = scratch.all_points;
    all_points.clear ();

    منطقي phantom_only = !مصرفکننده.is_consuming_contour_points ();
    إذا (unlikely (!glyph_for_gid (gid).get_points (font, *هذا, all_points, scratch, اشاره_تهی, اشاره_تهی, اشاره_تهی, درست, درست, phantom_only, coords, gvar_cache, 0, اشاره_تهی, budget)))
      أرجع خطأ;

    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.درازی)))
      أرجع خطأ;

    بیعلامت شمار = all_points.درازی;
    أكد (شمار >= glyf_impl::PHANTOM_شمار);
    شمار -= glyf_impl::PHANTOM_شمار;

    إذا (مصرفکننده.is_consuming_contour_points ())
    {
      خودکار *points = all_points.arrayZ;

      إذا (خطأ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	لكل (بیعلامت i = 0; i < شمار; i++)
	{
	  مصرفکننده.consume_نقطه (points[i]);
	  إذا (points[i].is_انتها_نقطه)
	    مصرفکننده.contour_انتها ();
	}
      }
      وإلا
      {
	لكل (بیعلامت i = 0; i < شمار; i++)
	{
	  // Start of a contour.
	  إذا (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_منحنی)
	  {
	    // First point is on-curve. Draw the contour.
	    لكل (; i < شمار; i++)
	    {
	      مصرفکننده.consume_نقطه (points[i]);
	      إذا (points[i].is_انتها_نقطه)
	      {
		مصرفکننده.contour_انتها ();
		اكسر;
	      }
	    }
	  }
	  وإلا
	  {
	    بیعلامت شروع = i;

	    // Find end of the contour.
	    لكل (; i < شمار; i++)
	      إذا (points[i].is_انتها_نقطه)
		اكسر;

	    بیعلامت انتها = i;

	    بیعلامت حد = انتها;
#اگر_تعریف_نشده HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    إذا (likely (انتها < شمار) &&
		(points[شروع].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[انتها].flag & (glyf_impl::SimpleGlyph::FLAG_ON_منحنی |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      حد++;
	    وإلا
#پایان_اگر
	    إذا (likely (انتها < شمار))
	      مصرفکننده.consume_نقطه (points[انتها]);

	    لكل (i = شروع; i < حد; i++)
	      مصرفکننده.consume_نقطه (points[i]);
	    i = انتها;

	    مصرفکننده.contour_انتها ();
	  }
	}
      }

      مصرفکننده.points_انتها ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_نقطه_t *phantoms = مصرفکننده.get_phantoms_sink ();
    إذا (phantoms)
      لكل (بیعلامت i = 0; i < glyf_impl::PHANTOM_شمار; ++i)
	phantoms[i] = all_points.arrayZ[شمار + i];

    أرجع درست;
  }

  عام:

  بنية points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_نقطه_t *phantoms;
    منطقي scaled;

    بنية contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      فراغ جمع_کن (ثابت contour_نقطه_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقي خالی_2 () ثابت { أرجع (min_x >= max_x) || (min_y >= max_y); }

      فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scaled)
      {
	إذا (unlikely (خالی_2 ()))
	{
	  extents->عرض = 0;
	  extents->x_bearing = 0;
	  extents->ارتفاع = 0;
	  extents->y_bearing = 0;
	  أرجع;
	}
	{
	  مزدوج x_bearing = (مزدوج) roundf (min_x);
	  مزدوج y_bearing = (مزدوج) roundf (max_y);
	  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x_bearing);
	  extents->عرض = hb_clamp_مقصد<hb_position_t> ((مزدوج) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
	  extents->ارتفاع = hb_clamp_مقصد<hb_position_t> ((مزدوج) roundf (min_y) - y_bearing);

	  إذا (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محمي:
      عائم min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_نقطه_t *phantoms_, منطقي scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      إذا (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    فراغ consume_نقطه (ثابت contour_نقطه_t &نقطه) { bounds.جمع_کن (نقطه); }
    فراغ contour_انتها () {}
    فراغ points_انتها () { bounds.get_extents (font, extents, scaled); }

    منطقي is_consuming_contour_points () { أرجع extents; }
    contour_نقطه_t *get_phantoms_sink () { أرجع phantoms; }
  };

#اگر_تعریف_نشده HB_NO_VAR
  بیعلامت
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقي is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (gid >= رقم_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطه_t phantoms[glyf_impl::PHANTOM_شمار];
    success = get_points (font, gid, points_aggregator_t (font, اشاره_تهی, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->رقم_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      بیعلامت upem = font->face->get_upem ();
      أرجع is_vertical ? upem : upem / 2;
    }

    عائم نتیجه = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_بالاترین].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_راست].x - phantoms[glyf_impl::PHANTOM_چپ].x;
    أرجع hb_clamp (roundf (نتیجه), 0.f, (عائم) UINT_MAX / 2);
  }
#پایان_اگر

  عائم
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (gid >= رقم_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطه_t phantoms[glyf_impl::PHANTOM_شمار];
    success = get_points (font, gid, points_aggregator_t (font, اشاره_تهی, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->رقم_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      أرجع font->face->get_upem ();
    }

    أرجع phantoms[glyf_impl::PHANTOM_بالاترین].y;
  }

  عام:

  منطقي get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { أرجع get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->رقم_coords : 0)); }

  منطقي get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحيح> coords,
		       int64_t *budget = اشاره_تهی) ثابت
  {
    إذا (unlikely (gid >= رقم_glyphs)) أرجع خطأ;

#اگر_تعریف_نشده HB_NO_VAR
    إذا (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع خطأ;
      منطقي ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, اشاره_تهی, درست),
			     coords,
			     *scratch,
			     اشاره_تهی,
			     budget);
      release_scratch (scratch);
      أرجع ret;
    }
#پایان_اگر
    أرجع glyph_for_gid (gid).get_extents_without_var_scaled (font, *هذا, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقي needs_padding_removal = خطأ) ثابت
  {
    إذا (unlikely (gid >= رقم_glyphs)) أرجع glyf_impl::Glyph ();

    بیعلامت صحيح شروع_جابه_جایی, انتها_جابه_جایی;

    إذا (short_جابه_جایی)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_جدول->داده_Z.arrayZ;
      شروع_جابه_جایی = 2 * offsets[gid];
      انتها_جابه_جایی   = 2 * offsets[gid + 1];
    }
    وإلا
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_جدول->داده_Z.arrayZ;
      شروع_جابه_جایی = offsets[gid];
      انتها_جابه_جایی   = offsets[gid + 1];
    }

    إذا (unlikely (شروع_جابه_جایی > انتها_جابه_جایی || انتها_جابه_جایی > glyf_جدول.get_درازی ()))
      أرجع glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_بایتها_t ((ثابت حرف *) هذا->glyf_جدول + شروع_جابه_جایی,
			     انتها_جابه_جایی - شروع_جابه_جایی), gid, extended);
    أرجع needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقي
  get_مسیر (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = اشاره_تهی,
	    int64_t *budget = اشاره_تهی) ثابت
  {
    إذا (!has_داده ()) أرجع خطأ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    إذا (unlikely (!scratch)) أرجع درست;

    منطقي ret = get_points (font, gid, glyf_impl::مسیر_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->رقم_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    أرجع ret;
  }

  منطقي
  get_مسیر_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحيح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = اشاره_تهی,
	       int64_t *budget = اشاره_تهی) ثابت
  {
    إذا (!has_داده ()) أرجع خطأ;
    منطقي ret = get_points (font, gid, glyf_impl::مسیر_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    أرجع ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    إذا (!has_داده ()) أرجع اشاره_تهی;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, اشاره_تهی)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      إذا (unlikely (!scratch))
	أرجع اشاره_تهی;
    }
    أرجع scratch;
  }
  فراغ release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    إذا (!scratch)
      أرجع;
    إذا (!cached_scratch.cmpexch (اشاره_تهی, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  بیعلامت صحيح get_رقم_glyphs () ثابت { أرجع رقم_glyphs; }
  hb_blob_t *reference_glyf_جدول () ثابت
  { أرجع hb_blob_reference (glyf_جدول.get_blob ()); }

#اگر_تعریف_نشده HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#پایان_اگر
  ثابت hmtx_accelerator_t *hmtx;
#اگر_تعریف_نشده HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#پایان_اگر

  خاص:
  منطقي extended;
  منطقي short_جابه_جایی;
  بیعلامت صحيح رقم_glyphs;
  hb_blob_ptr_t<loca> loca_جدول;
  hb_blob_ptr_t<glyf> glyf_جدول;
  تغییرپذیر hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


مضمن منطقي
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_وکتور_t<glyf_impl::SubsetGlyph>& glyphs /*بیرون*/,
			       hb_blob_t **منبع_glyf /*بیرون*/) ثابت
{
  OT::glyf_accelerator_t glyf (plan->منبع);
  إذا (!glyphs.alloc_exact (plan->new_مقصد_old_gid_list.درازی)) أرجع خطأ;
  *منبع_glyf = glyf.reference_glyf_جدول ();

  لكل (ثابت خودکار &pair : plan->new_مقصد_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.اولی;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    إذا (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.منبع_glyph = glyf_impl::Glyph ();
    وإلا
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.منبع_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_بایتها ();
    وإلا
      subset_glyph.dest_شروع = subset_glyph.منبع_glyph.get_بایتها ();

    إذا (font)
    {
      إذا (unlikely (!subset_glyph.compile_بایتها_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        إذا (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        أرجع خطأ;
      }
    }
  }
  أرجع درست;
}

مضمن hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->منبع);
  إذا (unlikely (font == hb_font_get_خالی ())) أرجع اشاره_تهی;

#اگر_تعریف_نشده HB_NO_VAR
  إذا (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_مجموعه_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.درازی);
    أرجع font;
  }
#پایان_اگر

  hb_وکتور_t<hb_variation_t> vars;
  إذا (unlikely (!vars.alloc (plan->user_axes_location.get_population (), درست)))
  {
    hb_font_destroy (font);
    أرجع اشاره_تهی;
  }

  لكل (خودکار _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.برچسب_2 = _.اولی;
    var.value = _.second.میانه;
    vars.push (var);
  }

#اگر_تعریف_نشده HB_NO_VAR
  hb_font_مجموعه_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#پایان_اگر
  أرجع font;
}


} /* namespace OT */


#پایان_اگر /* OT_GLYF_GLYF_HH */
