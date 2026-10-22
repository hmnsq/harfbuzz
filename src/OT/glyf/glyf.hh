#که_نه_تعریف_شوی OT_GLYF_GLYF_HH
#تعریف_کړه OT_GLYF_GLYF_HH


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
#تعریف_کړه HB_OT_نښان_glyf HB_نښان('g','l','y','f')
#تعریف_کړه HB_OT_نښان_GLYF HB_نښان('G','L','Y','F')

بنية glyf
{
  ملګری بنية glyf_accelerator_t;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_glyf;

  ساكن منطقي has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    أرجع سر_2.شاخص_منزل_Loc_Format <= 1 && سر_2.glyph_معلومات_Format <= 1;
  }

  ساكن منطقي has_extended_glyf (hb_face_t *face)
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_نښان_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_نښان_LOCA);
    منطقي ret = GLYF_جدول.get_اوږدتیا () && LOCA_جدول.get_اوږدتیا ();
    GLYF_جدول.destroy ();
    LOCA_جدول.destroy ();
    أرجع ret;
#وإلا
    أرجع خطأ;
#د_که_پای
  }

  منطقي sanitize (hb_sanitize_context_t *c HB_ناکارول_شوی) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (ریښتیا);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  کينډۍ <ډول_نوم Iterator>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (هذا);

    بینښه init_len = c->اوږدتیا ();
    لكل (خپلکاری &_ : it)
      إذا (unlikely (!_.serialize (c, use_short_loca, plan)))
        أرجع خطأ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    إذا (init_len == c->اوږدتیا ())
    {
      HBUINT8 تشه_byte;
      تشه_byte = 0;
      c->copy (تشه_byte);
    }
    return_trace (ریښتیا);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    منطقي extended = has_extended_glyf (c->plan->سرچینه);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended != (c->جدول_نښان_2 == HB_OT_نښان_GLYF))
      return_trace (خطأ);
#د_که_پای

    إذا (!has_valid_glyf_format (c->plan->سرچینه)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_پیغام (SUBSET, تش_اشاره,
                 "unkown glyf format, dropping from subset.");
      return_trace (خطأ);
    }

    hb_font_t *font = تش_اشاره;
    إذا (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      إذا (unlikely (!font))
	return_trace (خطأ);
    }

    hb_ویکټور_t<بینښه> padded_offsets;
    إذا (unlikely (!padded_offsets.alloc_exact (c->plan->new_منزل_old_gid_list.اوږدتیا)))
    {
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    hb_ویکټور_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *سرچینه_glyf = تش_اشاره;
    إذا (!_populate_subset_glyphs (c->plan, font, glyphs, &سرچینه_glyf))
    {
      hb_blob_destroy (سرچینه_glyf);
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    إذا (font)
      hb_font_destroy (font);

    بینښه max_دننه_واټن = 0;
    لكل (خپلکاری &g : glyphs)
    {
      بینښه اندازه = g.padded_اندازه ();
      padded_offsets.push (اندازه);
      max_دننه_واټن += اندازه;
    }

    منطقي use_short_loca = خطأ;
    إذا (likely (!c->plan->force_long_loca))
      use_short_loca = max_دننه_واټن < 0x1FFFF;

    إذا (!use_short_loca)
    {
      padded_offsets.clear ();
      لكل (خپلکاری &g : glyphs)
	padded_offsets.push (g.اوږدتیا ());
    }

    خپلکاری *glyf_اولي = c->serializer->پیل_embed <glyf> ();
    منطقي پایله = glyf_اولي->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    إذا (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (سرچینه_glyf);

    إذا (unlikely (!c->serializer->check_success (glyf_impl::جمع_کړه_loca_and_سر (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (خطأ);

    أرجع پایله;
  }

  منطقي
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_ویکټور_t<glyf_impl::SubsetGlyph>& glyphs /*بهر*/,
			   hb_blob_t               **سرچینه_glyf /*بهر*/) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  فراغ _free_compiled_subset_glyphs (hb_ویکټور_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    لكل (خپلکاری &g : glyphs)
      g.free_compiled_بایټونه ();
  }

  محمي:
  Unsized_Array_د<HBUINT8>
		معلومات_Z;	/* Glyphs data. */
  عام:
  DEFINE_اندازه_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
بنية GLYF : glyf
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_GLYF;
};
#د_که_پای

بنية glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = خطأ;
    short_دننه_واټن = خطأ;
    عدد_glyphs = 0;
    loca_جدول = تش_اشاره;
    glyf_جدول = تش_اشاره;
#که_نه_تعریف_شوی HB_NO_VAR
    gvar = تش_اشاره;
#د_که_پای
    hmtx = تش_اشاره;
#که_نه_تعریف_شوی HB_NO_VERTICAL
    vmtx = تش_اشاره;
#د_که_پای
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    إذا (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      أرجع;
    short_دننه_واټن = 0 == سر_2.شاخص_منزل_Loc_Format;

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_نښان_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_نښان_LOCA);
    extended = GLYF_جدول.get_اوږدتیا () && LOCA_جدول.get_اوږدتیا ();
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
#د_که_پای
    إذا (!extended)
    {
      glyf_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face);
      loca_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face);
    }
#که_نه_تعریف_شوی HB_NO_VAR
    gvar = face->جدول.gvar;
#د_که_پای
    hmtx = face->جدول.hmtx;
#که_نه_تعریف_شوی HB_NO_VERTICAL
    vmtx = face->جدول.vmtx;
#د_که_پای

    عدد_glyphs = hb_max (1u, loca_جدول.get_اوږدتیا () / (short_دننه_واټن ? 2 : 4)) - 1;
    عدد_glyphs = hb_min (عدد_glyphs, face->get_عدد_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    خپلکاری *scratch = cached_scratch.get_relaxed ();
    إذا (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_جدول.destroy ();
    loca_جدول.destroy ();
  }

  منطقي has_معلومات () ثابت { أرجع عدد_glyphs; }
  منطقي is_extended () ثابت { أرجع extended; }

  فراغ depend (hb_depend_معلومات_builder_t *builder) ثابت
  {
    إذا (!has_معلومات ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < get_عدد_glyphs (); gid++)
    {
      خپلکاری glyph = glyph_for_gid (gid);
      لكل (خپلکاری &توکی : glyph.get_مرکب_iterator ())
        builder->جمع_کړه_depend (gid,
                             extended ? HB_OT_نښان_GLYF : HB_OT_نښان_glyf,
                             توکی.get_gid (extended));
    }
  }

  محمي:
  کينډۍ<ډول_نوم T>
  منطقي get_points (hb_font_t *font, hb_codepoint_t gid, T مصرفوونکی,
		   hb_array_t<ثابت صحيح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = تش_اشاره,
		   int64_t *budget = تش_اشاره) ثابت
  {
    إذا (gid >= عدد_glyphs) أرجع خطأ;

    int64_t ډېری_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ډېری_budget;

    خپلکاری &all_points = scratch.all_points;
    all_points.clear ();

    منطقي phantom_only = !مصرفوونکی.is_consuming_contour_points ();
    إذا (unlikely (!glyph_for_gid (gid).get_points (font, *هذا, all_points, scratch, تش_اشاره, تش_اشاره, تش_اشاره, ریښتیا, ریښتیا, phantom_only, coords, gvar_cache, 0, تش_اشاره, budget)))
      أرجع خطأ;

    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.اوږدتیا)))
      أرجع خطأ;

    بینښه شمېر = all_points.اوږدتیا;
    أكد (شمېر >= glyf_impl::PHANTOM_شمېر);
    شمېر -= glyf_impl::PHANTOM_شمېر;

    إذا (مصرفوونکی.is_consuming_contour_points ())
    {
      خپلکاری *points = all_points.arrayZ;

      إذا (خطأ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	لكل (بینښه i = 0; i < شمېر; i++)
	{
	  مصرفوونکی.consume_ټکی (points[i]);
	  إذا (points[i].is_پای_ټکی_ټکی)
	    مصرفوونکی.contour_پای_ټکی ();
	}
      }
      وإلا
      {
	لكل (بینښه i = 0; i < شمېر; i++)
	{
	  // Start of a contour.
	  إذا (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_منحني)
	  {
	    // First point is on-curve. Draw the contour.
	    لكل (; i < شمېر; i++)
	    {
	      مصرفوونکی.consume_ټکی (points[i]);
	      إذا (points[i].is_پای_ټکی_ټکی)
	      {
		مصرفوونکی.contour_پای_ټکی ();
		اكسر;
	      }
	    }
	  }
	  وإلا
	  {
	    بینښه پیل = i;

	    // Find end of the contour.
	    لكل (; i < شمېر; i++)
	      إذا (points[i].is_پای_ټکی_ټکی)
		اكسر;

	    بینښه پای_ټکی = i;

	    بینښه حد = پای_ټکی;
#که_نه_تعریف_شوی HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    إذا (likely (پای_ټکی < شمېر) &&
		(points[پیل].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[پای_ټکی].flag & (glyf_impl::SimpleGlyph::FLAG_ON_منحني |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      حد++;
	    وإلا
#د_که_پای
	    إذا (likely (پای_ټکی < شمېر))
	      مصرفوونکی.consume_ټکی (points[پای_ټکی]);

	    لكل (i = پیل; i < حد; i++)
	      مصرفوونکی.consume_ټکی (points[i]);
	    i = پای_ټکی;

	    مصرفوونکی.contour_پای_ټکی ();
	  }
	}
      }

      مصرفوونکی.points_پای_ټکی ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_ټکی_t *phantoms = مصرفوونکی.get_phantoms_sink ();
    إذا (phantoms)
      لكل (بینښه i = 0; i < glyf_impl::PHANTOM_شمېر; ++i)
	phantoms[i] = all_points.arrayZ[شمېر + i];

    أرجع ریښتیا;
  }

  عام:

  بنية points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_ټکی_t *phantoms;
    منطقي scaled;

    بنية contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      فراغ جمع_کړه (ثابت contour_ټکی_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقي تشه_2 () ثابت { أرجع (min_x >= max_x) || (min_y >= max_y); }

      فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scaled)
      {
	إذا (unlikely (تشه_2 ()))
	{
	  extents->پلنوالی = 0;
	  extents->x_bearing = 0;
	  extents->لوړوالی = 0;
	  extents->y_bearing = 0;
	  أرجع;
	}
	{
	  مزدوج x_bearing = (مزدوج) roundf (min_x);
	  مزدوج y_bearing = (مزدوج) roundf (max_y);
	  extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
	  extents->پلنوالی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
	  extents->لوړوالی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (min_y) - y_bearing);

	  إذا (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محمي:
      عائم min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_ټکی_t *phantoms_, منطقي scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      إذا (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    فراغ consume_ټکی (ثابت contour_ټکی_t &ټکی) { bounds.جمع_کړه (ټکی); }
    فراغ contour_پای_ټکی () {}
    فراغ points_پای_ټکی () { bounds.get_extents (font, extents, scaled); }

    منطقي is_consuming_contour_points () { أرجع extents; }
    contour_ټکی_t *get_phantoms_sink () { أرجع phantoms; }
  };

#که_نه_تعریف_شوی HB_NO_VAR
  بینښه
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقي is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = تش_اشاره) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_ټکی_t phantoms[glyf_impl::PHANTOM_شمېر];
    success = get_points (font, gid, points_aggregator_t (font, تش_اشاره, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      بینښه upem = font->face->get_upem ();
      أرجع is_vertical ? upem : upem / 2;
    }

    عائم پایله = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_سر].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ښی].x - phantoms[glyf_impl::PHANTOM_کیڼ].x;
    أرجع hb_clamp (roundf (پایله), 0.f, (عائم) UINT_MAX / 2);
  }
#د_که_پای

  عائم
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = تش_اشاره) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_ټکی_t phantoms[glyf_impl::PHANTOM_شمېر];
    success = get_points (font, gid, points_aggregator_t (font, تش_اشاره, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      أرجع font->face->get_upem ();
    }

    أرجع phantoms[glyf_impl::PHANTOM_سر].y;
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
		       int64_t *budget = تش_اشاره) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع خطأ;

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع خطأ;
      منطقي ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, تش_اشاره, ریښتیا),
			     coords,
			     *scratch,
			     تش_اشاره,
			     budget);
      release_scratch (scratch);
      أرجع ret;
    }
#د_که_پای
    أرجع glyph_for_gid (gid).get_extents_without_var_scaled (font, *هذا, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقي needs_padding_removal = خطأ) ثابت
  {
    إذا (unlikely (gid >= عدد_glyphs)) أرجع glyf_impl::Glyph ();

    بینښه صحيح پیل_دننه_واټن, پای_ټکی_دننه_واټن;

    إذا (short_دننه_واټن)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_جدول->معلومات_Z.arrayZ;
      پیل_دننه_واټن = 2 * offsets[gid];
      پای_ټکی_دننه_واټن   = 2 * offsets[gid + 1];
    }
    وإلا
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_جدول->معلومات_Z.arrayZ;
      پیل_دننه_واټن = offsets[gid];
      پای_ټکی_دننه_واټن   = offsets[gid + 1];
    }

    إذا (unlikely (پیل_دننه_واټن > پای_ټکی_دننه_واټن || پای_ټکی_دننه_واټن > glyf_جدول.get_اوږدتیا ()))
      أرجع glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_بایټونه_t ((ثابت حرف *) هذا->glyf_جدول + پیل_دننه_واټن,
			     پای_ټکی_دننه_واټن - پیل_دننه_واټن), gid, extended);
    أرجع needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقي
  get_لار (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = تش_اشاره,
	    int64_t *budget = تش_اشاره) ثابت
  {
    إذا (!has_معلومات ()) أرجع خطأ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    إذا (unlikely (!scratch)) أرجع ریښتیا;

    منطقي ret = get_points (font, gid, glyf_impl::لار_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->عدد_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    أرجع ret;
  }

  منطقي
  get_لار_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحيح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = تش_اشاره,
	       int64_t *budget = تش_اشاره) ثابت
  {
    إذا (!has_معلومات ()) أرجع خطأ;
    منطقي ret = get_points (font, gid, glyf_impl::لار_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    أرجع ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    إذا (!has_معلومات ()) أرجع تش_اشاره;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, تش_اشاره)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      إذا (unlikely (!scratch))
	أرجع تش_اشاره;
    }
    أرجع scratch;
  }
  فراغ release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    إذا (!scratch)
      أرجع;
    إذا (!cached_scratch.cmpexch (تش_اشاره, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  بینښه صحيح get_عدد_glyphs () ثابت { أرجع عدد_glyphs; }
  hb_blob_t *reference_glyf_جدول () ثابت
  { أرجع hb_blob_reference (glyf_جدول.get_blob ()); }

#که_نه_تعریف_شوی HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#د_که_پای
  ثابت hmtx_accelerator_t *hmtx;
#که_نه_تعریف_شوی HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#د_که_پای

  خاص:
  منطقي extended;
  منطقي short_دننه_واټن;
  بینښه صحيح عدد_glyphs;
  hb_blob_ptr_t<loca> loca_جدول;
  hb_blob_ptr_t<glyf> glyf_جدول;
  بدلیدونکی hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


مضمن منطقي
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_ویکټور_t<glyf_impl::SubsetGlyph>& glyphs /*بهر*/,
			       hb_blob_t **سرچینه_glyf /*بهر*/) ثابت
{
  OT::glyf_accelerator_t glyf (plan->سرچینه);
  إذا (!glyphs.alloc_exact (plan->new_منزل_old_gid_list.اوږدتیا)) أرجع خطأ;
  *سرچینه_glyf = glyf.reference_glyf_جدول ();

  لكل (ثابت خپلکاری &pair : plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.لومړی;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    إذا (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.سرچینه_glyph = glyf_impl::Glyph ();
    وإلا
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.سرچینه_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_بایټونه ();
    وإلا
      subset_glyph.dest_پیل = subset_glyph.سرچینه_glyph.get_بایټونه ();

    إذا (font)
    {
      إذا (unlikely (!subset_glyph.compile_بایټونه_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        إذا (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        أرجع خطأ;
      }
    }
  }
  أرجع ریښتیا;
}

مضمن hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->سرچینه);
  إذا (unlikely (font == hb_font_get_تشه ())) أرجع تش_اشاره;

#که_نه_تعریف_شوی HB_NO_VAR
  إذا (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_سېټ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.اوږدتیا);
    أرجع font;
  }
#د_که_پای

  hb_ویکټور_t<hb_variation_t> vars;
  إذا (unlikely (!vars.alloc (plan->user_axes_location.get_population (), ریښتیا)))
  {
    hb_font_destroy (font);
    أرجع تش_اشاره;
  }

  لكل (خپلکاری _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.نښان_2 = _.لومړی;
    var.value = _.second.منځ;
    vars.push (var);
  }

#که_نه_تعریف_شوی HB_NO_VAR
  hb_font_سېټ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#د_که_پای
  أرجع font;
}


} /* namespace OT */


#د_که_پای /* OT_GLYF_GLYF_HH */
