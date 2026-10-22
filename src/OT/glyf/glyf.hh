#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_GLYF_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_GLYF_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-depend-data.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-ot-head-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-ot-hmtx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-ot-var-gvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-paint.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "glyf-helpers.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Glyph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SubsetGlyph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "loca.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "path-builder.hh"


ਨਾਮਾਕਾਸ਼ OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_glyf HB_ਟੈਗ('g','l','y','f')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_GLYF HB_ਟੈਗ('G','L','Y','F')

ਬਣਤਰ glyf
{
  ਮਿੱਤਰ ਬਣਤਰ glyf_accelerator_t;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_glyf;

  ਸਥਿਰ ਤਾਰਕਿਕ has_valid_glyf_format(ਅਚਲ hb_face_t* face)
  {
    ਅਚਲ OT::ਸਿਰ_2 &ਸਿਰ_2 = *face->ਸਾਰਣੀ.ਸਿਰ_2;
    ਵਾਪਸ ਸਿਰ_2.ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Loc_Format <= 1 && ਸਿਰ_2.glyph_ਅੰਕੜੇ_Format <= 1;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ has_extended_glyf (hb_face_t *face)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<glyf> (face, HB_OT_ਟੈਗ_GLYF);
    hb_blob_ptr_t<loca> LOCA_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<loca> (face, HB_OT_ਟੈਗ_LOCA);
    ਤਾਰਕਿਕ ret = GLYF_ਸਾਰਣੀ.get_ਲੰਮਾਈ () && LOCA_ਸਾਰਣੀ.get_ਲੰਮਾਈ ();
    GLYF_ਸਾਰਣੀ.destroy ();
    LOCA_ਸਾਰਣੀ.destroy ();
    ਵਾਪਸ ret;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (ਸੱਚ);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it,
                  ਤਾਰਕਿਕ use_short_loca,
		  ਅਚਲ hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ init_len = c->ਲੰਮਾਈ ();
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
      ਜੇ (unlikely (!_.serialize (c, use_short_loca, plan)))
        ਵਾਪਸ ਝੂਠ;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ਜੇ (init_len == c->ਲੰਮਾਈ ())
    {
      HBUINT8 ਖਾਲੀ_ਥਾਵਾਂ_byte;
      ਖਾਲੀ_ਥਾਵਾਂ_byte = 0;
      c->copy (ਖਾਲੀ_ਥਾਵਾਂ_byte);
    }
    return_trace (ਸੱਚ);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਤਾਰਕਿਕ extended = has_extended_glyf (c->plan->ਸਰੋਤ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended != (c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GLYF))
      return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    ਜੇ (!has_valid_glyf_format (c->plan->ਸਰੋਤ)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
                 "unkown glyf format, dropping from subset.");
      return_trace (ਝੂਠ);
    }

    hb_font_t *font = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ਜੇ (unlikely (!font))
	return_trace (ਝੂਠ);
    }

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> padded_offsets;
    ਜੇ (unlikely (!padded_offsets.alloc_exact (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ)))
    {
      hb_font_destroy (font);
      return_trace (ਝੂਠ);
    }

    hb_ਸਦਿਸ਼_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *ਸਰੋਤ_glyf = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!_populate_subset_glyphs (c->plan, font, glyphs, &ਸਰੋਤ_glyf))
    {
      hb_blob_destroy (ਸਰੋਤ_glyf);
      hb_font_destroy (font);
      return_trace (ਝੂਠ);
    }

    ਜੇ (font)
      hb_font_destroy (font);

    ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਵਿੱਥ = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ &g : glyphs)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = g.padded_ਮਾਪ ();
      padded_offsets.push (ਮਾਪ);
      max_ਵਿੱਥ += ਮਾਪ;
    }

    ਤਾਰਕਿਕ use_short_loca = ਝੂਠ;
    ਜੇ (likely (!c->plan->force_long_loca))
      use_short_loca = max_ਵਿੱਥ < 0x1FFFF;

    ਜੇ (!use_short_loca)
    {
      padded_offsets.clear ();
      ਲਈ (ਸਵੈਚਾਲਿਤ &g : glyphs)
	padded_offsets.push (g.ਲੰਮਾਈ ());
    }

    ਸਵੈਚਾਲਿਤ *glyf_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed <glyf> ();
    ਤਾਰਕਿਕ ਨਤੀਜਾ = glyf_ਅਭਾਜ->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ਜੇ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (ਸਰੋਤ_glyf);

    ਜੇ (unlikely (!c->serializer->check_success (glyf_impl::ਜੋੜੋ_loca_and_ਸਿਰ (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (ਝੂਠ);

    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਤਾਰਕਿਕ
  _populate_subset_glyphs (ਅਚਲ hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_ਸਦਿਸ਼_t<glyf_impl::SubsetGlyph>& glyphs /*ਬਾਹਰਲਾ*/,
			   hb_blob_t               **ਸਰੋਤ_glyf /*ਬਾਹਰਲਾ*/) ਅਚਲ;

  hb_font_t *
  _create_font_for_instancing (ਅਚਲ hb_subset_plan_t *plan) ਅਚਲ;

  ਖਾਲੀ _free_compiled_subset_glyphs (hb_ਸਦਿਸ਼_t<glyf_impl::SubsetGlyph> &glyphs) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &g : glyphs)
      g.free_compiled_ਬਾਈਟਾਂ ();
  }

  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<HBUINT8>
		ਅੰਕੜੇ_Z;	/* Glyphs data. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
ਬਣਤਰ GLYF : glyf
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_GLYF;
};
#ਜੇ_ਅੰਤ

ਬਣਤਰ glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = ਝੂਠ;
    short_ਵਿੱਥ = ਝੂਠ;
    ਅੰਕ_glyphs = 0;
    loca_ਸਾਰਣੀ = ਖਾਲੀ_ਸੂਚਕ;
    glyf_ਸਾਰਣੀ = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    gvar = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ
    hmtx = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    vmtx = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ
    ਅਚਲ OT::ਸਿਰ_2 &ਸਿਰ_2 = *face->ਸਾਰਣੀ.ਸਿਰ_2;
    ਜੇ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ਵਾਪਸ;
    short_ਵਿੱਥ = 0 == ਸਿਰ_2.ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Loc_Format;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<glyf> (face, HB_OT_ਟੈਗ_GLYF);
    hb_blob_ptr_t<loca> LOCA_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<loca> (face, HB_OT_ਟੈਗ_LOCA);
    extended = GLYF_ਸਾਰਣੀ.get_ਲੰਮਾਈ () && LOCA_ਸਾਰਣੀ.get_ਲੰਮਾਈ ();
    ਜੇ (extended)
    {
      glyf_ਸਾਰਣੀ = std::move (GLYF_ਸਾਰਣੀ);
      loca_ਸਾਰਣੀ = std::move (LOCA_ਸਾਰਣੀ);
    }
    ਨਹੀਂਤਾਂ
    {
      GLYF_ਸਾਰਣੀ.destroy ();
      LOCA_ਸਾਰਣੀ.destroy ();
    }
#ਜੇ_ਅੰਤ
    ਜੇ (!extended)
    {
      glyf_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<glyf> (face);
      loca_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<loca> (face);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    gvar = face->ਸਾਰਣੀ.gvar;
#ਜੇ_ਅੰਤ
    hmtx = face->ਸਾਰਣੀ.hmtx;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    vmtx = face->ਸਾਰਣੀ.vmtx;
#ਜੇ_ਅੰਤ

    ਅੰਕ_glyphs = hb_max (1u, loca_ਸਾਰਣੀ.get_ਲੰਮਾਈ () / (short_ਵਿੱਥ ? 2 : 4)) - 1;
    ਅੰਕ_glyphs = hb_min (ਅੰਕ_glyphs, face->get_ਅੰਕ_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    ਸਵੈਚਾਲਿਤ *scratch = cached_scratch.get_relaxed ();
    ਜੇ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_ਸਾਰਣੀ.destroy ();
    loca_ਸਾਰਣੀ.destroy ();
  }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਅੰਕ_glyphs; }
  ਤਾਰਕਿਕ is_extended () ਅਚਲ { ਵਾਪਸ extended; }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *builder) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ;
    ਲਈ (hb_codepoint_t gid = 0; gid < get_ਅੰਕ_glyphs (); gid++)
    {
      ਸਵੈਚਾਲਿਤ glyph = glyph_for_gid (gid);
      ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : glyph.get_ਸੰਯੁਕਤ_iterator ())
        builder->ਜੋੜੋ_depend (gid,
                             extended ? HB_OT_ਟੈਗ_GLYF : HB_OT_ਟੈਗ_glyf,
                             ਵਸਤੂ.get_gid (extended));
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ get_points (hb_font_t *font, hb_codepoint_t gid, T ਖਪਤਕਾਰ,
		   hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
		   int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (gid >= ਅੰਕ_glyphs) ਵਾਪਸ ਝੂਠ;

    int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
    ਜੇ (!budget) budget = &ਢੇਰ_budget;

    ਸਵੈਚਾਲਿਤ &all_points = scratch.all_points;
    all_points.clear ();

    ਤਾਰਕਿਕ phantom_only = !ਖਪਤਕਾਰ.is_consuming_contour_points ();
    ਜੇ (unlikely (!glyph_for_gid (gid).get_points (font, *ਇਹ, all_points, scratch, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਸੱਚ, ਸੱਚ, phantom_only, coords, gvar_cache, 0, ਖਾਲੀ_ਸੂਚਕ, budget)))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = all_points.ਲੰਮਾਈ;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਗਿਣਤੀ_2 >= glyf_impl::PHANTOM_ਗਿਣਤੀ);
    ਗਿਣਤੀ_2 -= glyf_impl::PHANTOM_ਗਿਣਤੀ;

    ਜੇ (ਖਪਤਕਾਰ.is_consuming_contour_points ())
    {
      ਸਵੈਚਾਲਿਤ *points = all_points.arrayZ;

      ਜੇ (ਝੂਠ)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	{
	  ਖਪਤਕਾਰ.consume_ਬਿੰਦੂ (points[i]);
	  ਜੇ (points[i].is_ਸਿਰਾ_ਬਿੰਦੂ)
	    ਖਪਤਕਾਰ.contour_ਸਿਰਾ ();
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	{
	  // Start of a contour.
	  ਜੇ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_ਵਕਰ)
	  {
	    // First point is on-curve. Draw the contour.
	    ਲਈ (; i < ਗਿਣਤੀ_2; i++)
	    {
	      ਖਪਤਕਾਰ.consume_ਬਿੰਦੂ (points[i]);
	      ਜੇ (points[i].is_ਸਿਰਾ_ਬਿੰਦੂ)
	      {
		ਖਪਤਕਾਰ.contour_ਸਿਰਾ ();
		ਰੋਕੋ;
	      }
	    }
	  }
	  ਨਹੀਂਤਾਂ
	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = i;

	    // Find end of the contour.
	    ਲਈ (; i < ਗਿਣਤੀ_2; i++)
	      ਜੇ (points[i].is_ਸਿਰਾ_ਬਿੰਦੂ)
		ਰੋਕੋ;

	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = i;

	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਹੱਦਬੰਦੀ = ਸਿਰਾ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ਜੇ (likely (ਸਿਰਾ < ਗਿਣਤੀ_2) &&
		(points[ਸ਼ੁਰੂ].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[ਸਿਰਾ].flag & (glyf_impl::SimpleGlyph::FLAG_ON_ਵਕਰ |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      ਹੱਦਬੰਦੀ++;
	    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
	    ਜੇ (likely (ਸਿਰਾ < ਗਿਣਤੀ_2))
	      ਖਪਤਕਾਰ.consume_ਬਿੰਦੂ (points[ਸਿਰਾ]);

	    ਲਈ (i = ਸ਼ੁਰੂ; i < ਹੱਦਬੰਦੀ; i++)
	      ਖਪਤਕਾਰ.consume_ਬਿੰਦੂ (points[i]);
	    i = ਸਿਰਾ;

	    ਖਪਤਕਾਰ.contour_ਸਿਰਾ ();
	  }
	}
      }

      ਖਪਤਕਾਰ.points_ਸਿਰਾ ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_ਬਿੰਦੂ_t *phantoms = ਖਪਤਕਾਰ.get_phantoms_sink ();
    ਜੇ (phantoms)
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyf_impl::PHANTOM_ਗਿਣਤੀ; ++i)
	phantoms[i] = all_points.arrayZ[ਗਿਣਤੀ_2 + i];

    ਵਾਪਸ ਸੱਚ;
  }

  ਜਨਤਕ:

  ਬਣਤਰ points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_ਬਿੰਦੂ_t *phantoms;
    ਤਾਰਕਿਕ scaled;

    ਬਣਤਰ contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      ਖਾਲੀ ਜੋੜੋ (ਅਚਲ contour_ਬਿੰਦੂ_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      ਤਾਰਕਿਕ ਖਾਲੀ_ਥਾਵਾਂ_2 () ਅਚਲ { ਵਾਪਸ (min_x >= max_x) || (min_y >= max_y); }

      ਖਾਲੀ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ਤਾਰਕਿਕ scaled)
      {
	ਜੇ (unlikely (ਖਾਲੀ_ਥਾਵਾਂ_2 ()))
	{
	  extents->ਚੌੜਾਈ = 0;
	  extents->x_bearing = 0;
	  extents->ਉਚਾਈ = 0;
	  extents->y_bearing = 0;
	  ਵਾਪਸ;
	}
	{
	  ਦੁੱਗਣਾ x_bearing = (ਦੁੱਗਣਾ) roundf (min_x);
	  ਦੁੱਗਣਾ y_bearing = (ਦੁੱਗਣਾ) roundf (max_y);
	  extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x_bearing);
	  extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (y_bearing);
	  extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (min_y) - y_bearing);

	  ਜੇ (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      ਸੁਰੱਖਿਅਤ:
      ਦਸ਼ਮਲਵ min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_ਬਿੰਦੂ_t *phantoms_, ਤਾਰਕਿਕ scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      ਜੇ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    ਖਾਲੀ consume_ਬਿੰਦੂ (ਅਚਲ contour_ਬਿੰਦੂ_t &ਬਿੰਦੂ) { bounds.ਜੋੜੋ (ਬਿੰਦੂ); }
    ਖਾਲੀ contour_ਸਿਰਾ () {}
    ਖਾਲੀ points_ਸਿਰਾ () { bounds.get_extents (font, extents, scaled); }

    ਤਾਰਕਿਕ is_consuming_contour_points () { ਵਾਪਸ extents; }
    contour_ਬਿੰਦੂ_t *get_phantoms_sink () { ਵਾਪਸ phantoms; }
  };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 ਤਾਰਕਿਕ is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (gid >= ਅੰਕ_glyphs)) ਵਾਪਸ 0;

    ਤਾਰਕਿਕ success = ਝੂਠ;

    contour_ਬਿੰਦੂ_t phantoms[glyf_impl::PHANTOM_ਗਿਣਤੀ];
    success = get_points (font, gid, points_aggregator_t (font, ਖਾਲੀ_ਸੂਚਕ, phantoms, ਝੂਠ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ਅੰਕ_coords : 0),
			  scratch, gvar_cache);
    ਜੇ (unlikely (!success))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ upem = font->face->get_upem ();
      ਵਾਪਸ is_vertical ? upem : upem / 2;
    }

    ਦਸ਼ਮਲਵ ਨਤੀਜਾ = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_ਸਿਖਰ].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_ਸੱਜਾ].x - phantoms[glyf_impl::PHANTOM_ਖੱਬਾ].x;
    ਵਾਪਸ hb_clamp (roundf (ਨਤੀਜਾ), 0.f, (ਦਸ਼ਮਲਵ) UINT_MAX / 2);
  }
#ਜੇ_ਅੰਤ

  ਦਸ਼ਮਲਵ
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (gid >= ਅੰਕ_glyphs)) ਵਾਪਸ 0;

    ਤਾਰਕਿਕ success = ਝੂਠ;

    contour_ਬਿੰਦੂ_t phantoms[glyf_impl::PHANTOM_ਗਿਣਤੀ];
    success = get_points (font, gid, points_aggregator_t (font, ਖਾਲੀ_ਸੂਚਕ, phantoms, ਝੂਠ),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->ਅੰਕ_coords : 0),
			  scratch, gvar_cache);
    ਜੇ (unlikely (!success))
    {
      ਵਾਪਸ font->face->get_upem ();
    }

    ਵਾਪਸ phantoms[glyf_impl::PHANTOM_ਸਿਖਰ].y;
  }

  ਜਨਤਕ:

  ਤਾਰਕਿਕ get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) ਅਚਲ
  { ਵਾਪਸ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->ਅੰਕ_coords : 0)); }

  ਤਾਰਕਿਕ get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
		       int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (gid >= ਅੰਕ_glyphs)) ਵਾਪਸ ਝੂਠ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਝੂਠ;
      ਤਾਰਕਿਕ ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, ਖਾਲੀ_ਸੂਚਕ, ਸੱਚ),
			     coords,
			     *scratch,
			     ਖਾਲੀ_ਸੂਚਕ,
			     budget);
      release_scratch (scratch);
      ਵਾਪਸ ret;
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ glyph_for_gid (gid).get_extents_without_var_scaled (font, *ਇਹ, extents);
  }

  ਅਚਲ glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, ਤਾਰਕਿਕ needs_padding_removal = ਝੂਠ) ਅਚਲ
  {
    ਜੇ (unlikely (gid >= ਅੰਕ_glyphs)) ਵਾਪਸ glyf_impl::Glyph ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਿਰਾ_ਵਿੱਥ;

    ਜੇ (short_ਵਿੱਥ)
    {
      ਅਚਲ HBUINT16 *offsets = (ਅਚਲ HBUINT16 *) loca_ਸਾਰਣੀ->ਅੰਕੜੇ_Z.arrayZ;
      ਸ਼ੁਰੂ_ਵਿੱਥ = 2 * offsets[gid];
      ਸਿਰਾ_ਵਿੱਥ   = 2 * offsets[gid + 1];
    }
    ਨਹੀਂਤਾਂ
    {
      ਅਚਲ HBUINT32 *offsets = (ਅਚਲ HBUINT32 *) loca_ਸਾਰਣੀ->ਅੰਕੜੇ_Z.arrayZ;
      ਸ਼ੁਰੂ_ਵਿੱਥ = offsets[gid];
      ਸਿਰਾ_ਵਿੱਥ   = offsets[gid + 1];
    }

    ਜੇ (unlikely (ਸ਼ੁਰੂ_ਵਿੱਥ > ਸਿਰਾ_ਵਿੱਥ || ਸਿਰਾ_ਵਿੱਥ > glyf_ਸਾਰਣੀ.get_ਲੰਮਾਈ ()))
      ਵਾਪਸ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *) ਇਹ->glyf_ਸਾਰਣੀ + ਸ਼ੁਰੂ_ਵਿੱਥ,
			     ਸਿਰਾ_ਵਿੱਥ - ਸ਼ੁਰੂ_ਵਿੱਥ), gid, extended);
    ਵਾਪਸ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  ਤਾਰਕਿਕ
  get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
	    int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ ਝੂਠ;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਸੱਚ;

    ਤਾਰਕਿਕ ret = get_points (font, gid, glyf_impl::ਰਸਤਾ_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->ਅੰਕ_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ਵਾਪਸ ret;
  }

  ਤਾਰਕਿਕ
  get_ਰਸਤਾ_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
	       int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ ਝੂਠ;
    ਤਾਰਕਿਕ ret = get_points (font, gid, glyf_impl::ਰਸਤਾ_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ਵਾਪਸ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ਜੇ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, ਖਾਲੀ_ਸੂਚਕ)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, ਆਕਾਰ (hb_glyf_scratch_t));
      ਜੇ (unlikely (!scratch))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ scratch;
  }
  ਖਾਲੀ release_scratch (hb_glyf_scratch_t *scratch) ਅਚਲ
  {
    ਜੇ (!scratch)
      ਵਾਪਸ;
    ਜੇ (!cached_scratch.cmpexch (ਖਾਲੀ_ਸੂਚਕ, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅੰਕ_glyphs () ਅਚਲ { ਵਾਪਸ ਅੰਕ_glyphs; }
  hb_blob_t *reference_glyf_ਸਾਰਣੀ () ਅਚਲ
  { ਵਾਪਸ hb_blob_reference (glyf_ਸਾਰਣੀ.get_blob ()); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਅਚਲ gvar_accelerator_t *gvar;
#ਜੇ_ਅੰਤ
  ਅਚਲ hmtx_accelerator_t *hmtx;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
  ਅਚਲ vmtx_accelerator_t *vmtx;
#ਜੇ_ਅੰਤ

  ਨਿਜੀ:
  ਤਾਰਕਿਕ extended;
  ਤਾਰਕਿਕ short_ਵਿੱਥ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  hb_blob_ptr_t<loca> loca_ਸਾਰਣੀ;
  hb_blob_ptr_t<glyf> glyf_ਸਾਰਣੀ;
  ਬਦਲਣਯੋਗ hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
glyf::_populate_subset_glyphs (ਅਚਲ hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_ਸਦਿਸ਼_t<glyf_impl::SubsetGlyph>& glyphs /*ਬਾਹਰਲਾ*/,
			       hb_blob_t **ਸਰੋਤ_glyf /*ਬਾਹਰਲਾ*/) ਅਚਲ
{
  OT::glyf_accelerator_t glyf (plan->ਸਰੋਤ);
  ਜੇ (!glyphs.alloc_exact (plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
  *ਸਰੋਤ_glyf = glyf.reference_glyf_ਸਾਰਣੀ ();

  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &pair : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.ਪਹਿਲਾ;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ਜੇ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.ਸਰੋਤ_glyph = glyf_impl::Glyph ();
    ਨਹੀਂਤਾਂ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.ਸਰੋਤ_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ਜੇ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_ਬਾਈਟਾਂ ();
    ਨਹੀਂਤਾਂ
      subset_glyph.dest_ਸ਼ੁਰੂ = subset_glyph.ਸਰੋਤ_glyph.get_ਬਾਈਟਾਂ ();

    ਜੇ (font)
    {
      ਜੇ (unlikely (!subset_glyph.compile_ਬਾਈਟਾਂ_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ਜੇ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ਵਾਪਸ ਝੂਠ;
      }
    }
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਤਰਅੰਦਰ hb_font_t *
glyf::_create_font_for_instancing (ਅਚਲ hb_subset_plan_t *plan) ਅਚਲ
{
  hb_font_t *font = hb_font_create (plan->ਸਰੋਤ);
  ਜੇ (unlikely (font == hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ())) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_ਸੈੱਟ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.ਲੰਮਾਈ);
    ਵਾਪਸ font;
  }
#ਜੇ_ਅੰਤ

  hb_ਸਦਿਸ਼_t<hb_variation_t> vars;
  ਜੇ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), ਸੱਚ)))
  {
    hb_font_destroy (font);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.ਟੈਗ_2 = _.ਪਹਿਲਾ;
    var.value = _.second.ਵਿਚਕਾਰ;
    vars.push (var);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  hb_font_ਸੈੱਟ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#ਜੇ_ਅੰਤ
  ਵਾਪਸ font;
}


} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_GLYF_HH */
