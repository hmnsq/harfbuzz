#که_نه_تعریف_شوی OT_VAR_VARC_VARC_HH
#تعریف_کړه OT_VAR_VARC_VARC_HH

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

#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES

بنية hb_varc_scratch_t
{
  hb_ویکټور_t<بینښه> axisIndices;
  hb_ویکټور_t<مزدوج> axis_ارزښتونه_2;
  hb_glyf_scratch_t glyf_scratch;
};

بنية hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  بدلیدونکی hb_decycler_t decycler;
  بدلیدونکی نښهدار څنډې_کیڼ;
  بدلیدونکی نښهدار ژوروالی_کیڼ;
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
    AXIS_ارزښتونه_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_بټ		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_ماسک		= ~((1u << 15) - 1),
  };

  بنية ریکارډ_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    بینښه gid_دننه_واټن;
    بینښه gid_اندازه;
    بینښه condition_شاخص;
    بینښه condition_دننه_واټن;
    بینښه condition_اندازه;
    بینښه axis_indices_شاخص;
    بینښه axis_indices_دننه_واټن;
    بینښه axis_indices_اندازه;
    uint32_t axis_ارزښتونه_var_idx;
    بینښه axis_ارزښتونه_var_دننه_واټن;
    بینښه axis_ارزښتونه_var_اندازه;
    uint32_t transform_var_idx;
    بینښه transform_var_دننه_واټن;
    بینښه transform_var_اندازه;
    hb_transform_decomposed_t<> transform;
    بینښه اندازه;
  };

  HB_دننی hb_ubytes_t
  get_لار_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t پلار_gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارډ,
	       hb_scalar_cache_t *cache = تش_اشاره) ثابت;

  HB_دننی ساكن منطقي decompile_ریکارډ (ثابت VARC &varc,
					    hb_ubytes_t ریکارډ,
					    hb_ویکټور_t<بینښه> *axis_indices,
					    hb_ویکټور_t<مزدوج> *axis_ارزښتونه,
					    ریکارډ_t *decoded /*بهر*/,
					    int64_t *budget = تش_اشاره);
};

بنية Var_مرکب_Glyph
{
  ساكن فراغ
  get_لار_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارډ,
	       hb_scalar_cache_t *cache)
  {
    طالما (ریکارډ)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (ریکارډ.arrayZ);
      ریکارډ = comp.get_لار_at (c,
				 gid,
				 coords, transform,
				 ریکارډ,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

بنية VARC
{
  ملګری بنية VarComponent;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_نښان ('V', 'A', 'R', 'C');

  HB_دننی منطقي
  get_لار_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_بې_اغېزه_عنصر,
	       hb_codepoint_t پلار_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *پلار_cache = تش_اشاره) ثابت;

  HB_دننی منطقي تړون_glyphs (hb_سېټ_t *glyphset) ثابت;
  HB_دننی فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت;
  HB_دننی منطقي subset (hb_subset_context_t *c) ثابت;

  منطقي
  get_لار (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 تش_اشاره,
			 hb_decycler_t {},
			 HB_MAX_ګراف_څنډه_شمېر,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_لار_at (c, gid,
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
			 تش_اشاره,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_ګراف_څنډه_شمېر,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_لار_at (c, gid,
			hb_array (font->coords, font->عدد_coords));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  نسخه.major == 1 &&
		  coverage.sanitize (c, هذا) &&
		  varStore.sanitize (c, هذا) &&
		  conditionList.sanitize (c, هذا) &&
		  axisIndicesList.sanitize (c, هذا) &&
		  glyph_ریکارډونه.sanitize (c, هذا));
  }

  بنية accelerator_t
  {
    ملګری بنية VarComponent;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<VARC> (face);
    }
    ~accelerator_t ()
    {
      خپلکاری *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      جدول.destroy ();
    }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    منطقي
    get_لار (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      إذا (!جدول->has_معلومات ()) أرجع خطأ;

      خپلکاری *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع ریښتیا;
      منطقي ret = جدول->get_لار (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي تړون_glyphs (hb_سېټ_t *glyphset) ثابت
    {
      إذا (جدول->has_معلومات ())
	أرجع جدول->تړون_glyphs (glyphset);
      أرجع ریښتیا;
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
#که_نه_تعریف_شوی HB_NO_DRAW
      إذا (!جدول->has_معلومات ()) أرجع خطأ;

      hb_extents_t<> f_extents;

      خپلکاری *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع ریښتیا;
      منطقي ret = جدول->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      إذا (ret)
	*extents = f_extents.منزل_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      أرجع ret;
#وإلا
      أرجع خطأ;
#د_که_پای
    }

    خاص:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, تش_اشاره)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, حجم (hb_varc_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع تش_اشاره;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (تش_اشاره, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    خاص:
    hb_blob_ptr_t<VARC> جدول;
    بدلیدونکی hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقي has_معلومات () ثابت { أرجع نسخه.major != 0; }

  محمي:
  Fixed_نسخه<> نسخه; /* Version identifier */
  Offset32_منزل<Coverage> coverage;
  Offset32_منزل<Multi_توکی_Variation_Store> varStore;
  Offset32_منزل<ConditionList> conditionList;
  Offset32_منزل<TupleList> axisIndicesList;
  Offset32_منزل<CFF2_شاخص/*Of<VarCompositeGlyph>*/> glyph_ریکارډونه;
  عام:
  DEFINE_اندازه_STATIC (24);
};

بنية VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#د_که_پای

//}

}

#د_که_پای  /* OT_VAR_VARC_VARC_HH */
