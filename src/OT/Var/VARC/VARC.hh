#جيڪڏهن_وصف_نه_ٿيل OT_VAR_VARC_VARC_HH
#وصف_ڪريو OT_VAR_VARC_VARC_HH

#ضمن "../../../hb-decycler.hh"
#ضمن "../../../hb-geometry.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-glyf-table.hh"
#ضمن "../../../hb-ot-cff2-table.hh"
#ضمن "../../../hb-ot-cff1-table.hh"

#ضمن "coord-setter.hh"

بنية hb_depend_معلومات_builder_t;

نطاق OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES

بنية hb_varc_scratch_t
{
  hb_ويڪٽر_t<بنانشان> axisIndices;
  hb_ويڪٽر_t<مزدوج> axis_قدرون_2;
  hb_glyf_scratch_t glyf_scratch;
};

بنية hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  بدلجندڙ hb_decycler_t decycler;
  بدلجندڙ نشانوارو ڪنارا_کاٻو;
  بدلجندڙ نشانوارو اونهائي_کاٻو;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

بنية VARC;

بنية VarComponent
{
  تعداد صنف flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_قدرون_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_ٻٽو_انگ		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_نقاب		= ~((1u << 15) - 1),
  };

  بنية رڪارڊ_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    بنانشان gid_وٿي;
    بنانشان gid_سائيز;
    بنانشان condition_اشاريو;
    بنانشان condition_وٿي;
    بنانشان condition_سائيز;
    بنانشان axis_indices_اشاريو;
    بنانشان axis_indices_وٿي;
    بنانشان axis_indices_سائيز;
    uint32_t axis_قدرون_var_idx;
    بنانشان axis_قدرون_var_وٿي;
    بنانشان axis_قدرون_var_سائيز;
    uint32_t transform_var_idx;
    بنانشان transform_var_وٿي;
    بنانشان transform_var_سائيز;
    hb_transform_decomposed_t<> transform;
    بنانشان سائيز;
  };

  HB_اندروني hb_ubytes_t
  get_رستو_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t والد_gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t رڪارڊ,
	       hb_scalar_cache_t *cache = خالي_اشارو) ثابت;

  HB_اندروني ساكن منطقي decompile_رڪارڊ (ثابت VARC &varc,
					    hb_ubytes_t رڪارڊ,
					    hb_ويڪٽر_t<بنانشان> *axis_indices,
					    hb_ويڪٽر_t<مزدوج> *axis_قدرون,
					    رڪارڊ_t *decoded /*ٻاهريون*/,
					    int64_t *budget = خالي_اشارو);
};

بنية Var_مرکب_Glyph
{
  ساكن فراغ
  get_رستو_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t رڪارڊ,
	       hb_scalar_cache_t *cache)
  {
    طالما (رڪارڊ)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (رڪارڊ.arrayZ);
      رڪارڊ = comp.get_رستو_at (c,
				 gid,
				 coords, transform,
				 رڪارڊ,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

بنية VARC
{
  دوست بنية VarComponent;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_ٽيگ ('V', 'A', 'R', 'C');

  HB_اندروني منطقي
  get_رستو_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_سڃاڻپ_عنصر,
	       hb_codepoint_t والد_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *والد_cache = خالي_اشارو) ثابت;

  HB_اندروني منطقي گهيرو_glyphs (hb_سيٽ_t *glyphset) ثابت;
  HB_اندروني فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت;
  HB_اندروني منطقي subset (hb_subset_context_t *c) ثابت;

  منطقي
  get_رستو (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 خالي_اشارو,
			 hb_decycler_t {},
			 HB_MAX_گراف_ڪنارو_ڳڻپ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_رستو_at (c, gid,
			hb_array (font->coords, font->انگ_coords));
  }

  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ثابت
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 خالي_اشارو,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_گراف_ڪنارو_ڳڻپ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_رستو_at (c, gid,
			hb_array (font->coords, font->انگ_coords));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  نسخو.major == 1 &&
		  coverage.sanitize (c, هذا) &&
		  varStore.sanitize (c, هذا) &&
		  conditionList.sanitize (c, هذا) &&
		  axisIndicesList.sanitize (c, هذا) &&
		  glyph_رڪارڊون.sanitize (c, هذا));
  }

  بنية accelerator_t
  {
    دوست بنية VarComponent;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<VARC> (face);
    }
    ~accelerator_t ()
    {
      خودڪار *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      جدول.destroy ();
    }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    منطقي
    get_رستو (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      إذا (!جدول->has_معلومات ()) أرجع خطأ;

      خودڪار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = جدول->get_رستو (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي گهيرو_glyphs (hb_سيٽ_t *glyphset) ثابت
    {
      إذا (جدول->has_معلومات ())
	أرجع جدول->گهيرو_glyphs (glyphset);
      أرجع سچ;
    }

    فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
    {
      إذا (جدول->has_معلومات ())
	جدول->depend (depend_معلومات);
    }

    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ثابت
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW
      إذا (!جدول->has_معلومات ()) أرجع خطأ;

      hb_extents_t<> f_extents;

      خودڪار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = جدول->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      إذا (ret)
	*extents = f_extents.منزل_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      أرجع ret;
#وإلا
      أرجع خطأ;
#جيڪڏهن_پورو
    }

    خاص:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالي_اشارو)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, حجم (hb_varc_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع خالي_اشارو;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (خالي_اشارو, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    خاص:
    hb_blob_ptr_t<VARC> جدول;
    بدلجندڙ hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقي has_معلومات () ثابت { أرجع نسخو.major != 0; }

  محمي:
  Fixed_نسخو<> نسخو; /* Version identifier */
  Offset32_منزل<Coverage> coverage;
  Offset32_منزل<Multi_شي_Variation_Store> varStore;
  Offset32_منزل<ConditionList> conditionList;
  Offset32_منزل<TupleList> axisIndicesList;
  Offset32_منزل<CFF2_اشاريو/*Of<VarCompositeGlyph>*/> glyph_رڪارڊون;
  عام:
  DEFINE_سائيز_STATIC (24);
};

بنية VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#جيڪڏهن_پورو

//}

}

#جيڪڏهن_پورو  /* OT_VAR_VARC_VARC_HH */
