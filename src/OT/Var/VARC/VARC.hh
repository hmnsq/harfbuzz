#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_VAR_VARC_VARC_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_VAR_VARC_VARC_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-decycler.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-geometry.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-cff1-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "coord-setter.hh"

ਬਣਤਰ hb_depend_ਅੰਕੜੇ_builder_t;

ਨਾਮਾਕਾਸ਼ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES

ਬਣਤਰ hb_varc_scratch_t
{
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> axisIndices;
  hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> axis_ਮੁੱਲਾਂ_2;
  hb_glyf_scratch_t glyf_scratch;
};

ਬਣਤਰ hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  ਬਦਲਣਯੋਗ hb_decycler_t decycler;
  ਬਦਲਣਯੋਗ ਨਿਸ਼ਾਨੀਵਾਲਾ ਕਿਨਾਰੇ_ਖੱਬਾ;
  ਬਦਲਣਯੋਗ ਨਿਸ਼ਾਨੀਵਾਲਾ ਡੂੰਘਾਈ_ਖੱਬਾ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ਬਣਤਰ VARC;

ਬਣਤਰ VarComponent
{
  ਗਿਣਤੀ ਵਰਗ flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_ਮੁੱਲਾਂ_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_ਨਕਾਬ		= ~((1u << 15) - 1),
  };

  ਬਣਤਰ ਰਿਕਾਰਡ_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    ਬਿਨਾਨਿਸ਼ਾਨੀ gid_ਵਿੱਥ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ gid_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ condition_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ condition_ਵਿੱਥ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ condition_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_indices_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_indices_ਵਿੱਥ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_indices_ਮਾਪ;
    uint32_t axis_ਮੁੱਲਾਂ_var_idx;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਮੁੱਲਾਂ_var_ਵਿੱਥ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਮੁੱਲਾਂ_var_ਮਾਪ;
    uint32_t transform_var_idx;
    ਬਿਨਾਨਿਸ਼ਾਨੀ transform_var_ਵਿੱਥ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ transform_var_ਮਾਪ;
    hb_transform_decomposed_t<> transform;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ;
  };

  HB_ਅੰਦਰੂਨੀ hb_ubytes_t
  get_ਰਸਤਾ_at (ਅਚਲ hb_varc_context_t &c,
	       hb_codepoint_t ਮਾਪਾ_gid,
	       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ਰਿਕਾਰਡ,
	       hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਤਾਰਕਿਕ decompile_ਰਿਕਾਰਡ (ਅਚਲ VARC &varc,
					    hb_ubytes_t ਰਿਕਾਰਡ,
					    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *axis_indices,
					    hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> *axis_ਮੁੱਲਾਂ,
					    ਰਿਕਾਰਡ_t *decoded /*ਬਾਹਰਲਾ*/,
					    int64_t *budget = ਖਾਲੀ_ਸੂਚਕ);
};

ਬਣਤਰ Var_ਸੰਯੁਕਤ_Glyph
{
  ਸਥਿਰ ਖਾਲੀ
  get_ਰਸਤਾ_at (ਅਚਲ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ਰਿਕਾਰਡ,
	       hb_scalar_cache_t *cache)
  {
    ਜਦੋਂ (ਰਿਕਾਰਡ)
    {
      ਅਚਲ VarComponent &comp = * (ਅਚਲ VarComponent *) (ਰਿਕਾਰਡ.arrayZ);
      ਰਿਕਾਰਡ = comp.get_ਰਸਤਾ_at (c,
				 gid,
				 coords, transform,
				 ਰਿਕਾਰਡ,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ਬਣਤਰ VARC
{
  ਮਿੱਤਰ ਬਣਤਰ VarComponent;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_ਟੈਗ ('V', 'A', 'R', 'C');

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
  get_ਰਸਤਾ_at (ਅਚਲ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_ਪਛਾਣ_ਤੱਤ,
	       hb_codepoint_t ਮਾਪਾ_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *ਮਾਪਾ_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *glyphset) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ;

  ਤਾਰਕਿਕ
  get_ਰਸਤਾ (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ਅਚਲ
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 ਖਾਲੀ_ਸੂਚਕ,
			 hb_decycler_t {},
			 HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ਵਾਪਸ get_ਰਸਤਾ_at (c, gid,
			hb_array (font->coords, font->ਅੰਕ_coords));
  }

  ਤਾਰਕਿਕ
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ਅਚਲ
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 ਖਾਲੀ_ਸੂਚਕ,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ਵਾਪਸ get_ਰਸਤਾ_at (c, gid,
			hb_array (font->coords, font->ਅੰਕ_coords));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  ਸੰਸਕਰਣ.major == 1 &&
		  coverage.sanitize (c, ਇਹ) &&
		  varStore.sanitize (c, ਇਹ) &&
		  conditionList.sanitize (c, ਇਹ) &&
		  axisIndicesList.sanitize (c, ਇਹ) &&
		  glyph_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ));
  }

  ਬਣਤਰ accelerator_t
  {
    ਮਿੱਤਰ ਬਣਤਰ VarComponent;

    accelerator_t (hb_face_t *face)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<VARC> (face);
    }
    ~accelerator_t ()
    {
      ਸਵੈਚਾਲਿਤ *scratch = cached_scratch.get_relaxed ();
      ਜੇ (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      ਸਾਰਣੀ.destroy ();
    }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->has_ਅੰਕੜੇ (); }

    ਤਾਰਕਿਕ
    get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ਅਚਲ
    {
      ਜੇ (!ਸਾਰਣੀ->has_ਅੰਕੜੇ ()) ਵਾਪਸ ਝੂਠ;

      ਸਵੈਚਾਲਿਤ *scratch = acquire_scratch ();
      ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਸੱਚ;
      ਤਾਰਕਿਕ ret = ਸਾਰਣੀ->get_ਰਸਤਾ (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      ਵਾਪਸ ret;
    }

    ਤਾਰਕਿਕ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *glyphset) ਅਚਲ
    {
      ਜੇ (ਸਾਰਣੀ->has_ਅੰਕੜੇ ())
	ਵਾਪਸ ਸਾਰਣੀ->ਘੇਰਾ_glyphs (glyphset);
      ਵਾਪਸ ਸੱਚ;
    }

    ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
    {
      ਜੇ (ਸਾਰਣੀ->has_ਅੰਕੜੇ ())
	ਸਾਰਣੀ->depend (depend_ਅੰਕੜੇ);
    }

    ਤਾਰਕਿਕ
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ਅਚਲ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
      ਜੇ (!ਸਾਰਣੀ->has_ਅੰਕੜੇ ()) ਵਾਪਸ ਝੂਠ;

      hb_extents_t<> f_extents;

      ਸਵੈਚਾਲਿਤ *scratch = acquire_scratch ();
      ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਸੱਚ;
      ਤਾਰਕਿਕ ret = ਸਾਰਣੀ->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      ਜੇ (ret)
	*extents = f_extents.ਮੰਜ਼ਿਲ_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      ਵਾਪਸ ret;
#ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    }

    ਨਿਜੀ:

    hb_varc_scratch_t *acquire_scratch () ਅਚਲ
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      ਜੇ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, ਖਾਲੀ_ਸੂਚਕ)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, ਆਕਾਰ (hb_varc_scratch_t));
	ਜੇ (unlikely (!scratch))
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      }

      ਵਾਪਸ scratch;
    }
    ਖਾਲੀ release_scratch (hb_varc_scratch_t *scratch) ਅਚਲ
    {
      ਜੇ (!cached_scratch.cmpexch (ਖਾਲੀ_ਸੂਚਕ, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    ਨਿਜੀ:
    hb_blob_ptr_t<VARC> ਸਾਰਣੀ;
    ਬਦਲਣਯੋਗ hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.major != 0; }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<> ਸੰਸਕਰਣ; /* Version identifier */
  Offset32_ਮੰਜ਼ਿਲ<Coverage> coverage;
  Offset32_ਮੰਜ਼ਿਲ<Multi_ਵਸਤੂ_Variation_Store> varStore;
  Offset32_ਮੰਜ਼ਿਲ<ConditionList> conditionList;
  Offset32_ਮੰਜ਼ਿਲ<TupleList> axisIndicesList;
  Offset32_ਮੰਜ਼ਿਲ<CFF2_ਸੂਚਕਾਂਕ/*Of<VarCompositeGlyph>*/> glyph_ਰਿਕਾਰਡਾਂ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (24);
};

ਬਣਤਰ VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#ਜੇ_ਅੰਤ

//}

}

#ਜੇ_ਅੰਤ  /* OT_VAR_VARC_VARC_HH */
