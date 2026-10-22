#جے_بنا_تعریف OT_VAR_VARC_VARC_HH
#تعریف_کرو OT_VAR_VARC_VARC_HH

#ضمن "../../../hb-decycler.hh"
#ضمن "../../../hb-geometry.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-glyf-table.hh"
#ضمن "../../../hb-ot-cff2-table.hh"
#ضمن "../../../hb-ot-cff1-table.hh"

#ضمن "coord-setter.hh"

بنية hb_depend_انکڑے_builder_t;

نطاق OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#جے_بنا_تعریف HB_NO_VAR_COMPOSITES

بنية hb_varc_scratch_t
{
  hb_سمتیہ_t<بنانشانی> axisIndices;
  hb_سمتیہ_t<مزدوج> axis_اقدار_2;
  hb_glyf_scratch_t glyf_scratch;
};

بنية hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  بدلنجوگ hb_decycler_t decycler;
  بدلنجوگ نشانیوالا کنارے_بایاں;
  بدلنجوگ نشانیوالا ڈونگھائی_بایاں;
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
    AXIS_اقدار_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_ثنائی_ہندسہ		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_نقاب		= ~((1u << 15) - 1),
  };

  بنية ریکارڈ_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    بنانشانی gid_وتھ;
    بنانشانی gid_ناپ;
    بنانشانی condition_اشاریہ;
    بنانشانی condition_وتھ;
    بنانشانی condition_ناپ;
    بنانشانی axis_indices_اشاریہ;
    بنانشانی axis_indices_وتھ;
    بنانشانی axis_indices_ناپ;
    uint32_t axis_اقدار_var_idx;
    بنانشانی axis_اقدار_var_وتھ;
    بنانشانی axis_اقدار_var_ناپ;
    uint32_t transform_var_idx;
    بنانشانی transform_var_وتھ;
    بنانشانی transform_var_ناپ;
    hb_transform_decomposed_t<> transform;
    بنانشانی ناپ;
  };

  HB_اندرونی hb_ubytes_t
  get_رستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t ماپا_gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارڈ,
	       hb_scalar_cache_t *cache = خالی_سوچک) ثابت;

  HB_اندرونی ساكن منطقي decompile_ریکارڈ (ثابت VARC &varc,
					    hb_ubytes_t ریکارڈ,
					    hb_سمتیہ_t<بنانشانی> *axis_indices,
					    hb_سمتیہ_t<مزدوج> *axis_اقدار,
					    ریکارڈ_t *decoded /*باہرلا*/,
					    int64_t *budget = خالی_سوچک);
};

بنية Var_مرکب_Glyph
{
  ساكن فراغ
  get_رستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارڈ,
	       hb_scalar_cache_t *cache)
  {
    طالما (ریکارڈ)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (ریکارڈ.arrayZ);
      ریکارڈ = comp.get_رستہ_at (c,
				 gid,
				 coords, transform,
				 ریکارڈ,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

بنية VARC
{
  یار بنية VarComponent;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_ٹیگ ('V', 'A', 'R', 'C');

  HB_اندرونی منطقي
  get_رستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_پچھان_تت,
	       hb_codepoint_t ماپا_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *ماپا_cache = خالی_سوچک) ثابت;

  HB_اندرونی منطقي گھیرا_glyphs (hb_سیٹ_t *glyphset) ثابت;
  HB_اندرونی فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت;
  HB_اندرونی منطقي subset (hb_subset_context_t *c) ثابت;

  منطقي
  get_رستہ (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 خالی_سوچک,
			 hb_decycler_t {},
			 HB_MAX_گراف_کنارا_گنتی,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_رستہ_at (c, gid,
			hb_array (font->coords, font->عدد_coords));
  }

  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ثابت
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 خالی_سوچک,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_گراف_کنارا_گنتی,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_رستہ_at (c, gid,
			hb_array (font->coords, font->عدد_coords));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  نسخہ.major == 1 &&
		  coverage.sanitize (c, هذا) &&
		  varStore.sanitize (c, هذا) &&
		  conditionList.sanitize (c, هذا) &&
		  axisIndicesList.sanitize (c, هذا) &&
		  glyph_ریکارڈاں.sanitize (c, هذا));
  }

  بنية accelerator_t
  {
    یار بنية VarComponent;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<VARC> (face);
    }
    ~accelerator_t ()
    {
      خودکار *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      جدول.destroy ();
    }

    منطقي has_انکڑے () ثابت { أرجع جدول->has_انکڑے (); }

    منطقي
    get_رستہ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      إذا (!جدول->has_انکڑے ()) أرجع خطأ;

      خودکار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = جدول->get_رستہ (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي گھیرا_glyphs (hb_سیٹ_t *glyphset) ثابت
    {
      إذا (جدول->has_انکڑے ())
	أرجع جدول->گھیرا_glyphs (glyphset);
      أرجع سچ;
    }

    فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
    {
      إذا (جدول->has_انکڑے ())
	جدول->depend (depend_انکڑے);
    }

    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ثابت
    {
#جے_بنا_تعریف HB_NO_DRAW
      إذا (!جدول->has_انکڑے ()) أرجع خطأ;

      hb_extents_t<> f_extents;

      خودکار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = جدول->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      إذا (ret)
	*extents = f_extents.منزل_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      أرجع ret;
#وإلا
      أرجع خطأ;
#جے_انت
    }

    خاص:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_سوچک)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, حجم (hb_varc_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع خالی_سوچک;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (خالی_سوچک, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    خاص:
    hb_blob_ptr_t<VARC> جدول;
    بدلنجوگ hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقي has_انکڑے () ثابت { أرجع نسخہ.major != 0; }

  محمي:
  Fixed_نسخہ<> نسخہ; /* Version identifier */
  Offset32_منزل<Coverage> coverage;
  Offset32_منزل<Multi_شے_Variation_Store> varStore;
  Offset32_منزل<ConditionList> conditionList;
  Offset32_منزل<TupleList> axisIndicesList;
  Offset32_منزل<CFF2_اشاریہ/*Of<VarCompositeGlyph>*/> glyph_ریکارڈاں;
  عام:
  DEFINE_ناپ_STATIC (24);
};

بنية VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#جے_انت

//}

}

#جے_انت  /* OT_VAR_VARC_VARC_HH */
