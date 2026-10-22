#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_GLYF_HH
#وصف_ڪريو OT_GLYF_GLYF_HH


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
#وصف_ڪريو HB_OT_ٽيگ_glyf HB_ٽيگ('g','l','y','f')
#وصف_ڪريو HB_OT_ٽيگ_GLYF HB_ٽيگ('G','L','Y','F')

بنية glyf
{
  دوست بنية glyf_accelerator_t;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_glyf;

  ساكن منطقي has_valid_glyf_format(ثابت hb_face_t* face)
  {
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    أرجع سر_2.اشاريو_منزل_Loc_Format <= 1 && سر_2.glyph_معلومات_Format <= 1;
  }

  ساكن منطقي has_extended_glyf (hb_face_t *face)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_ٽيگ_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_ٽيگ_LOCA);
    منطقي ret = GLYF_جدول.get_ڊيگهائي () && LOCA_جدول.get_ڊيگهائي ();
    GLYF_جدول.destroy ();
    LOCA_جدول.destroy ();
    أرجع ret;
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
  }

  منطقي sanitize (hb_sanitize_context_t *c HB_اڻ_استعمال_ٿيل) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (سچ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  سانچو <قسم_نالو Iterator>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
                  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (هذا);

    بنانشان init_len = c->ڊيگهائي ();
    لكل (خودڪار &_ : it)
      إذا (unlikely (!_.serialize (c, use_short_loca, plan)))
        أرجع خطأ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    إذا (init_len == c->ڊيگهائي ())
    {
      HBUINT8 خالي_جايون_byte;
      خالي_جايون_byte = 0;
      c->copy (خالي_جايون_byte);
    }
    return_trace (سچ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    منطقي extended = has_extended_glyf (c->plan->ذريعو);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended != (c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GLYF))
      return_trace (خطأ);
#جيڪڏهن_پورو

    إذا (!has_valid_glyf_format (c->plan->ذريعو)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_نياپو (SUBSET, خالي_اشارو,
                 "unkown glyf format, dropping from subset.");
      return_trace (خطأ);
    }

    hb_font_t *font = خالي_اشارو;
    إذا (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      إذا (unlikely (!font))
	return_trace (خطأ);
    }

    hb_ويڪٽر_t<بنانشان> padded_offsets;
    إذا (unlikely (!padded_offsets.alloc_exact (c->plan->new_منزل_old_gid_list.ڊيگهائي)))
    {
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    hb_ويڪٽر_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *ذريعو_glyf = خالي_اشارو;
    إذا (!_populate_subset_glyphs (c->plan, font, glyphs, &ذريعو_glyf))
    {
      hb_blob_destroy (ذريعو_glyf);
      hb_font_destroy (font);
      return_trace (خطأ);
    }

    إذا (font)
      hb_font_destroy (font);

    بنانشان max_وٿي = 0;
    لكل (خودڪار &g : glyphs)
    {
      بنانشان سائيز = g.padded_سائيز ();
      padded_offsets.push (سائيز);
      max_وٿي += سائيز;
    }

    منطقي use_short_loca = خطأ;
    إذا (likely (!c->plan->force_long_loca))
      use_short_loca = max_وٿي < 0x1FFFF;

    إذا (!use_short_loca)
    {
      padded_offsets.clear ();
      لكل (خودڪار &g : glyphs)
	padded_offsets.push (g.ڊيگهائي ());
    }

    خودڪار *glyf_مفرد = c->serializer->شروعات_embed <glyf> ();
    منطقي نتيجو = glyf_مفرد->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    إذا (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (ذريعو_glyf);

    إذا (unlikely (!c->serializer->check_success (glyf_impl::جوڙيو_loca_and_سر (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (خطأ);

    أرجع نتيجو;
  }

  منطقي
  _populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_ويڪٽر_t<glyf_impl::SubsetGlyph>& glyphs /*ٻاهريون*/,
			   hb_blob_t               **ذريعو_glyf /*ٻاهريون*/) ثابت;

  hb_font_t *
  _create_font_for_instancing (ثابت hb_subset_plan_t *plan) ثابت;

  فراغ _free_compiled_subset_glyphs (hb_ويڪٽر_t<glyf_impl::SubsetGlyph> &glyphs) ثابت
  {
    لكل (خودڪار &g : glyphs)
      g.free_compiled_بائيٽون ();
  }

  محمي:
  Unsized_Array_جو<HBUINT8>
		معلومات_Z;	/* Glyphs data. */
  عام:
  DEFINE_سائيز_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
بنية GLYF : glyf
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_GLYF;
};
#جيڪڏهن_پورو

بنية glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = خطأ;
    short_وٿي = خطأ;
    انگ_glyphs = 0;
    loca_جدول = خالي_اشارو;
    glyf_جدول = خالي_اشارو;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    gvar = خالي_اشارو;
#جيڪڏهن_پورو
    hmtx = خالي_اشارو;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
    vmtx = خالي_اشارو;
#جيڪڏهن_پورو
    ثابت OT::سر_2 &سر_2 = *face->جدول.سر_2;
    إذا (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      أرجع;
    short_وٿي = 0 == سر_2.اشاريو_منزل_Loc_Format;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face, HB_OT_ٽيگ_GLYF);
    hb_blob_ptr_t<loca> LOCA_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face, HB_OT_ٽيگ_LOCA);
    extended = GLYF_جدول.get_ڊيگهائي () && LOCA_جدول.get_ڊيگهائي ();
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
#جيڪڏهن_پورو
    إذا (!extended)
    {
      glyf_جدول = hb_sanitize_context_t ().reference_جدول<glyf> (face);
      loca_جدول = hb_sanitize_context_t ().reference_جدول<loca> (face);
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    gvar = face->جدول.gvar;
#جيڪڏهن_پورو
    hmtx = face->جدول.hmtx;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
    vmtx = face->جدول.vmtx;
#جيڪڏهن_پورو

    انگ_glyphs = hb_max (1u, loca_جدول.get_ڊيگهائي () / (short_وٿي ? 2 : 4)) - 1;
    انگ_glyphs = hb_min (انگ_glyphs, face->get_انگ_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    خودڪار *scratch = cached_scratch.get_relaxed ();
    إذا (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_جدول.destroy ();
    loca_جدول.destroy ();
  }

  منطقي has_معلومات () ثابت { أرجع انگ_glyphs; }
  منطقي is_extended () ثابت { أرجع extended; }

  فراغ depend (hb_depend_معلومات_builder_t *builder) ثابت
  {
    إذا (!has_معلومات ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < get_انگ_glyphs (); gid++)
    {
      خودڪار glyph = glyph_for_gid (gid);
      لكل (خودڪار &شي : glyph.get_مرکب_iterator ())
        builder->جوڙيو_depend (gid,
                             extended ? HB_OT_ٽيگ_GLYF : HB_OT_ٽيگ_glyf,
                             شي.get_gid (extended));
    }
  }

  محمي:
  سانچو<قسم_نالو T>
  منطقي get_points (hb_font_t *font, hb_codepoint_t gid, T استعمال_ڪندڙ,
		   hb_array_t<ثابت صحيح> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = خالي_اشارو,
		   int64_t *budget = خالي_اشارو) ثابت
  {
    إذا (gid >= انگ_glyphs) أرجع خطأ;

    int64_t ڍير_budget = HB_BUDGET_GLYPH;
    إذا (!budget) budget = &ڍير_budget;

    خودڪار &all_points = scratch.all_points;
    all_points.clear ();

    منطقي phantom_only = !استعمال_ڪندڙ.is_consuming_contour_points ();
    إذا (unlikely (!glyph_for_gid (gid).get_points (font, *هذا, all_points, scratch, خالي_اشارو, خالي_اشارو, خالي_اشارو, سچ, سچ, phantom_only, coords, gvar_cache, 0, خالي_اشارو, budget)))
      أرجع خطأ;

    إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.ڊيگهائي)))
      أرجع خطأ;

    بنانشان ڳڻپ = all_points.ڊيگهائي;
    أكد (ڳڻپ >= glyf_impl::PHANTOM_ڳڻپ);
    ڳڻپ -= glyf_impl::PHANTOM_ڳڻپ;

    إذا (استعمال_ڪندڙ.is_consuming_contour_points ())
    {
      خودڪار *points = all_points.arrayZ;

      إذا (خطأ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	لكل (بنانشان i = 0; i < ڳڻپ; i++)
	{
	  استعمال_ڪندڙ.consume_نقطو (points[i]);
	  إذا (points[i].is_پڇاڙي_نقطو)
	    استعمال_ڪندڙ.contour_پڇاڙي ();
	}
      }
      وإلا
      {
	لكل (بنانشان i = 0; i < ڳڻپ; i++)
	{
	  // Start of a contour.
	  إذا (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_وڪر)
	  {
	    // First point is on-curve. Draw the contour.
	    لكل (; i < ڳڻپ; i++)
	    {
	      استعمال_ڪندڙ.consume_نقطو (points[i]);
	      إذا (points[i].is_پڇاڙي_نقطو)
	      {
		استعمال_ڪندڙ.contour_پڇاڙي ();
		اكسر;
	      }
	    }
	  }
	  وإلا
	  {
	    بنانشان شروعات = i;

	    // Find end of the contour.
	    لكل (; i < ڳڻپ; i++)
	      إذا (points[i].is_پڇاڙي_نقطو)
		اكسر;

	    بنانشان پڇاڙي = i;

	    بنانشان حد_بندي = پڇاڙي;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    إذا (likely (پڇاڙي < ڳڻپ) &&
		(points[شروعات].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[پڇاڙي].flag & (glyf_impl::SimpleGlyph::FLAG_ON_وڪر |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      حد_بندي++;
	    وإلا
#جيڪڏهن_پورو
	    إذا (likely (پڇاڙي < ڳڻپ))
	      استعمال_ڪندڙ.consume_نقطو (points[پڇاڙي]);

	    لكل (i = شروعات; i < حد_بندي; i++)
	      استعمال_ڪندڙ.consume_نقطو (points[i]);
	    i = پڇاڙي;

	    استعمال_ڪندڙ.contour_پڇاڙي ();
	  }
	}
      }

      استعمال_ڪندڙ.points_پڇاڙي ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_نقطو_t *phantoms = استعمال_ڪندڙ.get_phantoms_sink ();
    إذا (phantoms)
      لكل (بنانشان i = 0; i < glyf_impl::PHANTOM_ڳڻپ; ++i)
	phantoms[i] = all_points.arrayZ[ڳڻپ + i];

    أرجع سچ;
  }

  عام:

  بنية points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_نقطو_t *phantoms;
    منطقي scaled;

    بنية contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      فراغ جوڙيو (ثابت contour_نقطو_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      منطقي خالي_جايون_2 () ثابت { أرجع (min_x >= max_x) || (min_y >= max_y); }

      فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scaled)
      {
	إذا (unlikely (خالي_جايون_2 ()))
	{
	  extents->ويڪر = 0;
	  extents->x_bearing = 0;
	  extents->اوچائي = 0;
	  extents->y_bearing = 0;
	  أرجع;
	}
	{
	  مزدوج x_bearing = (مزدوج) roundf (min_x);
	  مزدوج y_bearing = (مزدوج) roundf (max_y);
	  extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
	  extents->ويڪر = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
	  extents->اوچائي = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (min_y) - y_bearing);

	  إذا (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      محمي:
      عائم min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_نقطو_t *phantoms_, منطقي scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      إذا (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    فراغ consume_نقطو (ثابت contour_نقطو_t &نقطو) { bounds.جوڙيو (نقطو); }
    فراغ contour_پڇاڙي () {}
    فراغ points_پڇاڙي () { bounds.get_extents (font, extents, scaled); }

    منطقي is_consuming_contour_points () { أرجع extents; }
    contour_نقطو_t *get_phantoms_sink () { أرجع phantoms; }
  };

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  بنانشان
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 منطقي is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (gid >= انگ_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطو_t phantoms[glyf_impl::PHANTOM_ڳڻپ];
    success = get_points (font, gid, points_aggregator_t (font, خالي_اشارو, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->انگ_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      بنانشان upem = font->face->get_upem ();
      أرجع is_vertical ? upem : upem / 2;
    }

    عائم نتيجو = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_چوٽي].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ساڄو].x - phantoms[glyf_impl::PHANTOM_کاٻو].x;
    أرجع hb_clamp (roundf (نتيجو), 0.f, (عائم) UINT_MAX / 2);
  }
#جيڪڏهن_پورو

  عائم
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (gid >= انگ_glyphs)) أرجع 0;

    منطقي success = خطأ;

    contour_نقطو_t phantoms[glyf_impl::PHANTOM_ڳڻپ];
    success = get_points (font, gid, points_aggregator_t (font, خالي_اشارو, phantoms, خطأ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->انگ_coords : 0),
			  scratch, gvar_cache);
    إذا (unlikely (!success))
    {
      أرجع font->face->get_upem ();
    }

    أرجع phantoms[glyf_impl::PHANTOM_چوٽي].y;
  }

  عام:

  منطقي get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ثابت
  { أرجع get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->انگ_coords : 0)); }

  منطقي get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ثابت صحيح> coords,
		       int64_t *budget = خالي_اشارو) ثابت
  {
    إذا (unlikely (gid >= انگ_glyphs)) أرجع خطأ;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع خطأ;
      منطقي ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, خالي_اشارو, سچ),
			     coords,
			     *scratch,
			     خالي_اشارو,
			     budget);
      release_scratch (scratch);
      أرجع ret;
    }
#جيڪڏهن_پورو
    أرجع glyph_for_gid (gid).get_extents_without_var_scaled (font, *هذا, extents);
  }

  ثابت glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, منطقي needs_padding_removal = خطأ) ثابت
  {
    إذا (unlikely (gid >= انگ_glyphs)) أرجع glyf_impl::Glyph ();

    بنانشان صحيح شروعات_وٿي, پڇاڙي_وٿي;

    إذا (short_وٿي)
    {
      ثابت HBUINT16 *offsets = (ثابت HBUINT16 *) loca_جدول->معلومات_Z.arrayZ;
      شروعات_وٿي = 2 * offsets[gid];
      پڇاڙي_وٿي   = 2 * offsets[gid + 1];
    }
    وإلا
    {
      ثابت HBUINT32 *offsets = (ثابت HBUINT32 *) loca_جدول->معلومات_Z.arrayZ;
      شروعات_وٿي = offsets[gid];
      پڇاڙي_وٿي   = offsets[gid + 1];
    }

    إذا (unlikely (شروعات_وٿي > پڇاڙي_وٿي || پڇاڙي_وٿي > glyf_جدول.get_ڊيگهائي ()))
      أرجع glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_بائيٽون_t ((ثابت حرف *) هذا->glyf_جدول + شروعات_وٿي,
			     پڇاڙي_وٿي - شروعات_وٿي), gid, extended);
    أرجع needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  منطقي
  get_رستو (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = خالي_اشارو,
	    int64_t *budget = خالي_اشارو) ثابت
  {
    إذا (!has_معلومات ()) أرجع خطأ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    إذا (unlikely (!scratch)) أرجع سچ;

    منطقي ret = get_points (font, gid, glyf_impl::رستو_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->انگ_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    أرجع ret;
  }

  منطقي
  get_رستو_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ثابت صحيح> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = خالي_اشارو,
	       int64_t *budget = خالي_اشارو) ثابت
  {
    إذا (!has_معلومات ()) أرجع خطأ;
    منطقي ret = get_points (font, gid, glyf_impl::رستو_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    أرجع ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ثابت
  {
    إذا (!has_معلومات ()) أرجع خالي_اشارو;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالي_اشارو)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, حجم (hb_glyf_scratch_t));
      إذا (unlikely (!scratch))
	أرجع خالي_اشارو;
    }
    أرجع scratch;
  }
  فراغ release_scratch (hb_glyf_scratch_t *scratch) ثابت
  {
    إذا (!scratch)
      أرجع;
    إذا (!cached_scratch.cmpexch (خالي_اشارو, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  بنانشان صحيح get_انگ_glyphs () ثابت { أرجع انگ_glyphs; }
  hb_blob_t *reference_glyf_جدول () ثابت
  { أرجع hb_blob_reference (glyf_جدول.get_blob ()); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  ثابت gvar_accelerator_t *gvar;
#جيڪڏهن_پورو
  ثابت hmtx_accelerator_t *hmtx;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
  ثابت vmtx_accelerator_t *vmtx;
#جيڪڏهن_پورو

  خاص:
  منطقي extended;
  منطقي short_وٿي;
  بنانشان صحيح انگ_glyphs;
  hb_blob_ptr_t<loca> loca_جدول;
  hb_blob_ptr_t<glyf> glyf_جدول;
  بدلجندڙ hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


مضمن منطقي
glyf::_populate_subset_glyphs (ثابت hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_ويڪٽر_t<glyf_impl::SubsetGlyph>& glyphs /*ٻاهريون*/,
			       hb_blob_t **ذريعو_glyf /*ٻاهريون*/) ثابت
{
  OT::glyf_accelerator_t glyf (plan->ذريعو);
  إذا (!glyphs.alloc_exact (plan->new_منزل_old_gid_list.ڊيگهائي)) أرجع خطأ;
  *ذريعو_glyf = glyf.reference_glyf_جدول ();

  لكل (ثابت خودڪار &pair : plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.پهريون;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    إذا (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.ذريعو_glyph = glyf_impl::Glyph ();
    وإلا
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.ذريعو_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_بائيٽون ();
    وإلا
      subset_glyph.dest_شروعات = subset_glyph.ذريعو_glyph.get_بائيٽون ();

    إذا (font)
    {
      إذا (unlikely (!subset_glyph.compile_بائيٽون_with_deltas (plan, font, glyf)))
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
  hb_font_t *font = hb_font_create (plan->ذريعو);
  إذا (unlikely (font == hb_font_get_خالي_جايون ())) أرجع خالي_اشارو;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_سيٽ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.ڊيگهائي);
    أرجع font;
  }
#جيڪڏهن_پورو

  hb_ويڪٽر_t<hb_variation_t> vars;
  إذا (unlikely (!vars.alloc (plan->user_axes_location.get_population (), سچ)))
  {
    hb_font_destroy (font);
    أرجع خالي_اشارو;
  }

  لكل (خودڪار _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.ٽيگ_2 = _.پهريون;
    var.value = _.second.وچ;
    vars.push (var);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  hb_font_سيٽ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#جيڪڏهن_پورو
  أرجع font;
}


} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_GLYF_HH */
