#اگر_تعریف_نشده OT_VAR_VARC_VARC_HH
#تعریف_کن OT_VAR_VARC_VARC_HH

#ضمن "../../../hb-decycler.hh"
#ضمن "../../../hb-geometry.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-glyf-table.hh"
#ضمن "../../../hb-ot-cff2-table.hh"
#ضمن "../../../hb-ot-cff1-table.hh"

#ضمن "coord-setter.hh"

بنية hb_depend_داده_builder_t;

نطاق OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES

بنية hb_varc_scratch_t
{
  hb_وکتور_t<بیعلامت> axisIndices;
  hb_وکتور_t<مزدوج> axis_مقادیر_2;
  hb_glyf_scratch_t glyf_scratch;
};

بنية hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  تغییرپذیر hb_decycler_t decycler;
  تغییرپذیر علامتدار یالها_چپ;
  تغییرپذیر علامتدار عمق_چپ;
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
    AXIS_مقادیر_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_بیت		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_ماسک		= ~((1u << 15) - 1),
  };

  بنية رکورد_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    بیعلامت gid_جابه_جایی;
    بیعلامت gid_اندازه;
    بیعلامت condition_شاخص;
    بیعلامت condition_جابه_جایی;
    بیعلامت condition_اندازه;
    بیعلامت axis_indices_شاخص;
    بیعلامت axis_indices_جابه_جایی;
    بیعلامت axis_indices_اندازه;
    uint32_t axis_مقادیر_var_idx;
    بیعلامت axis_مقادیر_var_جابه_جایی;
    بیعلامت axis_مقادیر_var_اندازه;
    uint32_t transform_var_idx;
    بیعلامت transform_var_جابه_جایی;
    بیعلامت transform_var_اندازه;
    hb_transform_decomposed_t<> transform;
    بیعلامت اندازه;
  };

  HB_داخلی hb_ubytes_t
  get_مسیر_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t والد_gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t رکورد,
	       hb_scalar_cache_t *cache = اشاره_تهی) ثابت;

  HB_داخلی ساكن منطقي decompile_رکورد (ثابت VARC &varc,
					    hb_ubytes_t رکورد,
					    hb_وکتور_t<بیعلامت> *axis_indices,
					    hb_وکتور_t<مزدوج> *axis_مقادیر,
					    رکورد_t *decoded /*بیرون*/,
					    int64_t *budget = اشاره_تهی);
};

بنية Var_مرکب_Glyph
{
  ساكن فراغ
  get_مسیر_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t رکورد,
	       hb_scalar_cache_t *cache)
  {
    طالما (رکورد)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (رکورد.arrayZ);
      رکورد = comp.get_مسیر_at (c,
				 gid,
				 coords, transform,
				 رکورد,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

بنية VARC
{
  دوست بنية VarComponent;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_برچسب ('V', 'A', 'R', 'C');

  HB_داخلی منطقي
  get_مسیر_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحيح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_عنصر_خنثی,
	       hb_codepoint_t والد_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *والد_cache = اشاره_تهی) ثابت;

  HB_داخلی منطقي بستگی_glyphs (hb_مجموعه_t *glyphset) ثابت;
  HB_داخلی فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت;
  HB_داخلی منطقي subset (hb_subset_context_t *c) ثابت;

  منطقي
  get_مسیر (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 اشاره_تهی,
			 hb_decycler_t {},
			 HB_MAX_گراف_یال_شمار,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_مسیر_at (c, gid,
			hb_array (font->coords, font->رقم_coords));
  }

  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ثابت
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 اشاره_تهی,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_گراف_یال_شمار,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    أرجع get_مسیر_at (c, gid,
			hb_array (font->coords, font->رقم_coords));
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
		  glyph_رکوردها.sanitize (c, هذا));
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
      خودکار *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      جدول.destroy ();
    }

    منطقي has_داده () ثابت { أرجع جدول->has_داده (); }

    منطقي
    get_مسیر (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      إذا (!جدول->has_داده ()) أرجع خطأ;

      خودکار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع درست;
      منطقي ret = جدول->get_مسیر (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي بستگی_glyphs (hb_مجموعه_t *glyphset) ثابت
    {
      إذا (جدول->has_داده ())
	أرجع جدول->بستگی_glyphs (glyphset);
      أرجع درست;
    }

    فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت
    {
      إذا (جدول->has_داده ())
	جدول->depend (depend_داده);
    }

    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ثابت
    {
#اگر_تعریف_نشده HB_NO_DRAW
      إذا (!جدول->has_داده ()) أرجع خطأ;

      hb_extents_t<> f_extents;

      خودکار *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع درست;
      منطقي ret = جدول->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      إذا (ret)
	*extents = f_extents.مقصد_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      أرجع ret;
#وإلا
      أرجع خطأ;
#پایان_اگر
    }

    خاص:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, اشاره_تهی)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, حجم (hb_varc_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع اشاره_تهی;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (اشاره_تهی, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    خاص:
    hb_blob_ptr_t<VARC> جدول;
    تغییرپذیر hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقي has_داده () ثابت { أرجع نسخه.major != 0; }

  محمي:
  Fixed_نسخه<> نسخه; /* Version identifier */
  Offset32_مقصد<Coverage> coverage;
  Offset32_مقصد<Multi_قلم_Variation_Store> varStore;
  Offset32_مقصد<ConditionList> conditionList;
  Offset32_مقصد<TupleList> axisIndicesList;
  Offset32_مقصد<CFF2_شاخص/*Of<VarCompositeGlyph>*/> glyph_رکوردها;
  عام:
  DEFINE_اندازه_STATIC (24);
};

بنية VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#پایان_اگر

//}

}

#پایان_اگر  /* OT_VAR_VARC_VARC_HH */
