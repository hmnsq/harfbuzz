#اگر_غیر_تعریف_شدہ OT_GLYF_GLYF_HH
#تعریف_کرو OT_GLYF_GLYF_HH


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


نامخلا OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#تعریف_کرو HB_OT_لیبل_glyf HB_لیبل('g','l','y','f')
#تعریف_کرو HB_OT_لیبل_GLYF HB_لیبل('G','L','Y','F')

ساخت glyf
{
  دوست ساخت glyf_accelerator_t;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_glyf;

  ساکن منطقی has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    واپس سر_2.اشاریہ_مقصد_Loc_Format <= 1 && سر_2.glyph_ڈیٹا_Format <= 1;
  }

  ساکن منطقی has_extended_glyf (hb_face_t *face)
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_لیبل_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_لیبل_LOCA);
    منطقی ret = GLYF_جدول.get_لمبائی () && LOCA_جدول.get_لمبائی ();
    GLYF_جدول.destroy ();
    LOCA_جدول.destroy ();
    واپس ret;
#ورنہ
    واپس جھوٹ;
#اگر_ختم
  }

  منطقی sanitize (hb_sanitize_context_t *c HB_غیر_مستعمل) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (سچ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  سانچہ <قسم_نام Iterator>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقی use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (یہ);

    غیرعلامتی init_len = c->لمبائی ();
    برائے (خودکار &_ : it)
      اگر (unlikely (!_.serialize (c, use_short_loca, plan)))
        واپس جھوٹ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    اگر (init_len == c->لمبائی ())
    {
      HBUINT8 خالی_خانے_byte;
      خالی_خانے_byte = 0;
      c->copy (خالی_خانے_byte);
    }
    return_trace (سچ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    منطقی extended = has_extended_glyf (c->plan->ماخذ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended != (c->جدول_لیبل_2 == HB_OT_لیبل_GLYF))
      return_trace (جھوٹ);
#اگر_ختم

    اگر (!has_valid_glyf_format (c->plan->ماخذ)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_پیغام (SUBSET, خالی_اشاریہ,
                 "unkown glyf format, dropping from subset.");
      return_trace (جھوٹ);
    }

    hb_font_t *font = خالی_اشاریہ;
    اگر (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      اگر (unlikely (!font))
	return_trace (جھوٹ);
    }

    hb_سمتیہ_t<غیرعلامتی> padded_offsets;
    اگر (unlikely (!padded_offsets.alloc_exact (c->plan->new_مقصد_old_gid_list.لمبائی)))
    {
      hb_font_destroy (font);
      return_trace (جھوٹ);
    }

    hb_سمتیہ_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *ماخذ_glyf = خالی_اشاریہ;
    اگر (!_populate_subset_glyphs (c->plan, font, glyphs, &ماخذ_glyf))
    {
      hb_blob_destroy (ماخذ_glyf);
      hb_font_destroy (font);
      return_trace (جھوٹ);
    }

    اگر (font)
      hb_font_destroy (font);

    غیرعلامتی max_انحراف = 0;
    برائے (خودکار &g : glyphs)
    {
      غیرعلامتی سائز = g.padded_سائز ();
      padded_offsets.push (سائز);
      max_انحراف += سائز;
    }

    منطقی use_short_loca = جھوٹ;
    اگر (likely (!c->plan->force_long_loca))
      use_short_loca = max_انحراف < 0x1FFFF;

    اگر (!use_short_loca)
    {
      padded_offsets.clear ();
      برائے (خودکار &g : glyphs)
	padded_offsets.push (g.لمبائی ());
    }

    خودکار *glyf_مفرد = c->serializer->آغاز_embed <glyf> ();
    منطقی نتیجہ = glyf_مفرد->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    اگر (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (ماخذ_glyf);

    اگر (unlikely (!c->serializer->check_success (glyf_impl::جوڑو_loca_and_سر (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (جھوٹ);

    واپس نتیجہ;
  }

  منطقی
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_سمتیہ_t<glyf_impl::SubsetGlyph>& glyphs /*برآمد*/,
			   hb_blob_t               **ماخذ_glyf /*برآمد*/) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  خالی _free_compiled_subset_glyphs (hb_سمتیہ_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    برائے (خودکار &g : glyphs)
      g.free_compiled_بائٹیں ();
  }

  محفوظ:
  Unsized_Array_کا<HBUINT8>
		ڈیٹا_Z;	/* Glyphs data. */
  عوامی:
  DEFINE_سائز_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
ساخت GLYF : glyf
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_GLYF;
};
#اگر_ختم

ساخت glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = جھوٹ;
    short_انحراف = جھوٹ;
    عدد_glyphs = 0;
    loca_جدول = خالی_اشاریہ;
    glyf_جدول = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    gvar = خالی_اشاریہ;
#اگر_ختم
    hmtx = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    vmtx = خالی_اشاریہ;
#اگر_ختم
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    اگر (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      واپس;
    short_انحراف = 0 == سر_2.اشاریہ_مقصد_Loc_Format;

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_لیبل_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_لیبل_LOCA);
    extended = GLYF_جدول.get_لمبائی () && LOCA_جدول.get_لمبائی ();
    اگر (extended)
    {
      glyf_جدول = std::move (GLYF_جدول);
      loca_جدول = std::move (LOCA_جدول);
    }
    ورنہ
    {
      GLYF_جدول.destroy ();
      LOCA_جدول.destroy ();
    }
#اگر_ختم
    اگر (!extended)
    {
      glyf_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face);
      loca_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face);
    }
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    gvar = face->جدول.gvar;
#اگر_ختم
    hmtx = face->جدول.hmtx;
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    vmtx = face->جدول.vmtx;
#اگر_ختم

    عدد_glyphs = hb_max (1u, loca_جدول.get_لمبائی () / (short_انحراف ? 2 : 4)) - 1;
    عدد_glyphs = hb_min (عدد_glyphs, face->get_عدد_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    خودکار *scratch = cached_scratch.get_relaxed ();
    اگر (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_جدول.destroy ();
    loca_جدول.destroy ();
  }

  منطقی has_ڈیٹا () ثابت { واپس عدد_glyphs; }
  منطقی is_extended () ثابت { واپس extended; }

  خالی depend (hb_depend_ڈیٹا_builder_t *builder) ثابت
  {
    اگر (!has_ڈیٹا ()) واپس;
    برائے (hb_codepoint_t gid = 0; gid < get_عدد_glyphs (); gid++)
    {
      خودکار glyph = glyph_for_gid (gid);
      برائے (خودکار &شے : glyph.get_مرکب_iterator ())
        builder->جوڑو_depend (gid,
                             extended ? HB_OT_لیبل_GLYF : HB_OT_لیبل_glyf,
                             شے.get_gid (extended));
    }
  }

  محفوظ:
  سانچہ<قسم_نام T>
  منطقی get_points (hb_font_t *font, hb_codepoint_t gid, T صارف,
		   hb_array_t<ثابت صحیح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
		   int64_t *budget = خالی_اشاریہ) ثابت
  {
    اگر (gid >= عدد_glyphs) واپس جھوٹ;

    int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
    اگر (!budget) budget = &ڈھیر_budget;

    خودکار &all_points = scratch.all_points;
    all_points.clear ();

    منطقی phantom_only = !صارف.is_consuming_contour_points ();
    اگر (unlikely (!glyph_for_gid (gid).get_points (font, *یہ, all_points, scratch, خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ, سچ, سچ, phantom_only, coords, gvar_cache, 0, خالی_اشاریہ, budget)))
      واپس جھوٹ;

    اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.لمبائی)))
      واپس جھوٹ;

    غیرعلامتی گنتی = all_points.لمبائی;
    أكد (گنتی >= glyf_impl::PHANTOM_گنتی);
    گنتی -= glyf_impl::PHANTOM_گنتی;

    اگر (صارف.is_consuming_contour_points ())
    {
      خودکار *points = all_points.arrayZ;

      اگر (جھوٹ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	برائے (غیرعلامتی i = 0; i < گنتی; i++)
	{
	  صارف.consume_نقطہ (points[i]);
	  اگر (points[i].is_انتہا_نقطہ)
	    صارف.contour_انتہا ();
	}
      }
      ورنہ
      {
	برائے (غیرعلامتی i = 0; i < گنتی; i++)
	{
	  // Start of a contour.
	  اگر (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_خم)
	  {
	    // First point is on-curve. Draw the contour.
	    برائے (; i < گنتی; i++)
	    {
	      صارف.consume_نقطہ (points[i]);
	      اگر (points[i].is_انتہا_نقطہ)
	      {
		صارف.contour_انتہا ();
		توڑو;
	      }
	    }
	  }
	  ورنہ
	  {
	    غیرعلامتی آغاز = i;

	    // Find end of the contour.
	    برائے (; i < گنتی; i++)
	      اگر (points[i].is_انتہا_نقطہ)
		توڑو;

	    غیرعلامتی انتہا = i;

	    غیرعلامتی حد_بندی = انتہا;
#اگر_غیر_تعریف_شدہ HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    اگر (likely (انتہا < گنتی) &&
		(points[آغاز].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[انتہا].flag & (glyf_impl::SimpleGlyph::FLAG_ON_خم |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      حد_بندی++;
	    ورنہ
#اگر_ختم
	    اگر (likely (انتہا < گنتی))
	      صارف.consume_نقطہ (points[انتہا]);

	    برائے (i = آغاز; i < حد_بندی; i++)
	      صارف.consume_نقطہ (points[i]);
	    i = انتہا;

	    صارف.contour_انتہا ();
	  }
	}
      }

      صارف.points_انتہا ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_نقطہ_t *phantoms = صارف.get_phantoms_sink ();
    اگر (phantoms)
      برائے (غیرعلامتی i = 0; i < glyf_impl::PHANTOM_گنتی; ++i)
	phantoms[i] = all_points.arrayZ[گنتی + i];

    واپس سچ;
  }

  عوامی:

  ساخت points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_نقطہ_t *phantoms;
    منطقی scaled;

    ساخت contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      خالی جوڑو (ثابت contour_نقطہ_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقی خالی_خانے_2 () ثابت { واپس (min_x >= max_x) || (min_y >= max_y); }

      خالی get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقی scaled)
      {
	اگر (unlikely (خالی_خانے_2 ()))
	{
	  extents->چوڑائی = 0;
	  extents->x_bearing = 0;
	  extents->اونچائی = 0;
	  extents->y_bearing = 0;
	  واپس;
	}
	{
	  دوہرا x_bearing = (دوہرا) roundf (min_x);
	  دوہرا y_bearing = (دوہرا) roundf (max_y);
	  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x_bearing);
	  extents->چوڑائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
	  extents->اونچائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (min_y) - y_bearing);

	  اگر (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محفوظ:
      اعشاری min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_نقطہ_t *phantoms_, منطقی scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      اگر (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    خالی consume_نقطہ (ثابت contour_نقطہ_t &نقطہ) { bounds.جوڑو (نقطہ); }
    خالی contour_انتہا () {}
    خالی points_انتہا () { bounds.get_extents (font, extents, scaled); }

    منطقی is_consuming_contour_points () { واپس extents; }
    contour_نقطہ_t *get_phantoms_sink () { واپس phantoms; }
  };

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  غیرعلامتی
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقی is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (gid >= عدد_glyphs)) واپس 0;

    منطقی success = جھوٹ;

    contour_نقطہ_t phantoms[glyf_impl::PHANTOM_گنتی];
    success = get_points (font, gid, points_aggregator_t (font, خالی_اشاریہ, phantoms, جھوٹ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    اگر (unlikely (!success))
    {
      غیرعلامتی upem = font->face->get_upem ();
      واپس is_vertical ? upem : upem / 2;
    }

    اعشاری نتیجہ = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_چوٹی].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_دایاں].x - phantoms[glyf_impl::PHANTOM_بایاں].x;
    واپس hb_clamp (roundf (نتیجہ), 0.f, (اعشاری) UINT_MAX / 2);
  }
#اگر_ختم

  اعشاری
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (gid >= عدد_glyphs)) واپس 0;

    منطقی success = جھوٹ;

    contour_نقطہ_t phantoms[glyf_impl::PHANTOM_گنتی];
    success = get_points (font, gid, points_aggregator_t (font, خالی_اشاریہ, phantoms, جھوٹ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->عدد_coords : 0),
			  scratch, gvar_cache);
    اگر (unlikely (!success))
    {
      واپس font->face->get_upem ();
    }

    واپس phantoms[glyf_impl::PHANTOM_چوٹی].y;
  }

  عوامی:

  منطقی get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { واپس get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->عدد_coords : 0)); }

  منطقی get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحیح> coords,
		       int64_t *budget = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (gid >= عدد_glyphs)) واپس جھوٹ;

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) واپس جھوٹ;
      منطقی ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, خالی_اشاریہ, سچ),
			     coords,
			     *scratch,
			     خالی_اشاریہ,
			     budget);
      release_scratch (scratch);
      واپس ret;
    }
#اگر_ختم
    واپس glyph_for_gid (gid).get_extents_without_var_scaled (font, *یہ, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقی needs_padding_removal = جھوٹ) ثابت
  {
    اگر (unlikely (gid >= عدد_glyphs)) واپس glyf_impl::Glyph ();

    غیرعلامتی صحیح آغاز_انحراف, انتہا_انحراف;

    اگر (short_انحراف)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_جدول->ڈیٹا_Z.arrayZ;
      آغاز_انحراف = 2 * offsets[gid];
      انتہا_انحراف   = 2 * offsets[gid + 1];
    }
    ورنہ
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_جدول->ڈیٹا_Z.arrayZ;
      آغاز_انحراف = offsets[gid];
      انتہا_انحراف   = offsets[gid + 1];
    }

    اگر (unlikely (آغاز_انحراف > انتہا_انحراف || انتہا_انحراف > glyf_جدول.get_لمبائی ()))
      واپس glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_بائٹیں_t ((ثابت حرف *) یہ->glyf_جدول + آغاز_انحراف,
			     انتہا_انحراف - آغاز_انحراف), gid, extended);
    واپس needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقی
  get_راستہ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
	    int64_t *budget = خالی_اشاریہ) ثابت
  {
    اگر (!has_ڈیٹا ()) واپس جھوٹ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    اگر (unlikely (!scratch)) واپس سچ;

    منطقی ret = get_points (font, gid, glyf_impl::راستہ_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->عدد_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    واپس ret;
  }

  منطقی
  get_راستہ_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحیح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = خالی_اشاریہ,
	       int64_t *budget = خالی_اشاریہ) ثابت
  {
    اگر (!has_ڈیٹا ()) واپس جھوٹ;
    منطقی ret = get_points (font, gid, glyf_impl::راستہ_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    واپس ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    اگر (!has_ڈیٹا ()) واپس خالی_اشاریہ;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_اشاریہ)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      اگر (unlikely (!scratch))
	واپس خالی_اشاریہ;
    }
    واپس scratch;
  }
  خالی release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    اگر (!scratch)
      واپس;
    اگر (!cached_scratch.cmpexch (خالی_اشاریہ, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  غیرعلامتی صحیح get_عدد_glyphs () ثابت { واپس عدد_glyphs; }
  hb_blob_t *reference_glyf_جدول () ثابت
  { واپس hb_blob_reference (glyf_جدول.get_blob ()); }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#اگر_ختم
  ثابت hmtx_accelerator_t *hmtx;
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#اگر_ختم

  نجی:
  منطقی extended;
  منطقی short_انحراف;
  غیرعلامتی صحیح عدد_glyphs;
  hb_blob_ptr_t<loca> loca_جدول;
  hb_blob_ptr_t<glyf> glyf_جدول;
  تغیرپذیر hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


اندرونی منطقی
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_سمتیہ_t<glyf_impl::SubsetGlyph>& glyphs /*برآمد*/,
			       hb_blob_t **ماخذ_glyf /*برآمد*/) ثابت
{
  OT::glyf_accelerator_t glyf (plan->ماخذ);
  اگر (!glyphs.alloc_exact (plan->new_مقصد_old_gid_list.لمبائی)) واپس جھوٹ;
  *ماخذ_glyf = glyf.reference_glyf_جدول ();

  برائے (ثابت خودکار &pair : plan->new_مقصد_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.پہلا;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    اگر (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.ماخذ_glyph = glyf_impl::Glyph ();
    ورنہ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.ماخذ_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    اگر (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_بائٹیں ();
    ورنہ
      subset_glyph.dest_آغاز = subset_glyph.ماخذ_glyph.get_بائٹیں ();

    اگر (font)
    {
      اگر (unlikely (!subset_glyph.compile_بائٹیں_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        اگر (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        واپس جھوٹ;
      }
    }
  }
  واپس سچ;
}

اندرونی hb_font_t *
glyf::_create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت
{
  hb_font_t *font = hb_font_create (plan->ماخذ);
  اگر (unlikely (font == hb_font_get_خالی_خانے ())) واپس خالی_اشاریہ;

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_سیٹ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.لمبائی);
    واپس font;
  }
#اگر_ختم

  hb_سمتیہ_t<hb_variation_t> vars;
  اگر (unlikely (!vars.alloc (plan->user_axes_location.get_population (), سچ)))
  {
    hb_font_destroy (font);
    واپس خالی_اشاریہ;
  }

  برائے (خودکار _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.لیبل_2 = _.پہلا;
    var.value = _.second.درمیان;
    vars.push (var);
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  hb_font_سیٹ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#اگر_ختم
  واپس font;
}


} /* namespace OT */


#اگر_ختم /* OT_GLYF_GLYF_HH */
