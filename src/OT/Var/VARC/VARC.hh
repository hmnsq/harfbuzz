#اگر_غیر_تعریف_شدہ OT_VAR_VARC_VARC_HH
#تعریف_کرو OT_VAR_VARC_VARC_HH

#شامل "../../../hb-decycler.hh"
#شامل "../../../hb-geometry.hh"
#شامل "../../../hb-ot-layout-common.hh"
#شامل "../../../hb-ot-glyf-table.hh"
#شامل "../../../hb-ot-cff2-table.hh"
#شامل "../../../hb-ot-cff1-table.hh"

#شامل "coord-setter.hh"

ساخت hb_depend_ڈیٹا_builder_t;

نامخلا OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES

ساخت hb_varc_scratch_t
{
  hb_سمتیہ_t<غیرعلامتی> axisIndices;
  hb_سمتیہ_t<دوہرا> axis_اقدار_2;
  hb_glyf_scratch_t glyf_scratch;
};

ساخت hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  تغیرپذیر hb_decycler_t decycler;
  تغیرپذیر علامتی کنارے_بایاں;
  تغیرپذیر علامتی گہرائی_بایاں;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ساخت VARC;

ساخت VarComponent
{
  شمار جماعت flags_t : uint32_t
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

  ساخت ریکارڈ_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    غیرعلامتی gid_انحراف;
    غیرعلامتی gid_سائز;
    غیرعلامتی condition_اشاریہ;
    غیرعلامتی condition_انحراف;
    غیرعلامتی condition_سائز;
    غیرعلامتی axis_indices_اشاریہ;
    غیرعلامتی axis_indices_انحراف;
    غیرعلامتی axis_indices_سائز;
    uint32_t axis_اقدار_var_idx;
    غیرعلامتی axis_اقدار_var_انحراف;
    غیرعلامتی axis_اقدار_var_سائز;
    uint32_t transform_var_idx;
    غیرعلامتی transform_var_انحراف;
    غیرعلامتی transform_var_سائز;
    hb_transform_decomposed_t<> transform;
    غیرعلامتی سائز;
  };

  HB_اندرونی hb_ubytes_t
  get_راستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t والد_gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارڈ,
	       hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت;

  HB_اندرونی ساکن منطقی decompile_ریکارڈ (ثابت VARC &varc,
					    hb_ubytes_t ریکارڈ,
					    hb_سمتیہ_t<غیرعلامتی> *axis_indices,
					    hb_سمتیہ_t<دوہرا> *axis_اقدار,
					    ریکارڈ_t *decoded /*برآمد*/,
					    int64_t *budget = خالی_اشاریہ);
};

ساخت Var_مرکب_Glyph
{
  ساکن خالی
  get_راستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t ریکارڈ,
	       hb_scalar_cache_t *cache)
  {
    جب (ریکارڈ)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (ریکارڈ.arrayZ);
      ریکارڈ = comp.get_راستہ_at (c,
				 gid,
				 coords, transform,
				 ریکارڈ,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ساخت VARC
{
  دوست ساخت VarComponent;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_لیبل ('V', 'A', 'R', 'C');

  HB_اندرونی منطقی
  get_راستہ_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_شناختی_عنصر,
	       hb_codepoint_t والد_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *والد_cache = خالی_اشاریہ) ثابت;

  HB_اندرونی منطقی احاطہ_glyphs (hb_سیٹ_t *glyphset) ثابت;
  HB_اندرونی خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت;
  HB_اندرونی منطقی subset (hb_subset_context_t *c) ثابت;

  منطقی
  get_راستہ (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 خالی_اشاریہ,
			 hb_decycler_t {},
			 HB_MAX_گراف_کنارہ_گنتی,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    واپس get_راستہ_at (c, gid,
			hb_array (font->coords, font->عدد_coords));
  }

  منطقی
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ثابت
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 خالی_اشاریہ,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_گراف_کنارہ_گنتی,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    واپس get_راستہ_at (c, gid,
			hb_array (font->coords, font->عدد_coords));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  نسخہ.major == 1 &&
		  coverage.sanitize (c, یہ) &&
		  varStore.sanitize (c, یہ) &&
		  conditionList.sanitize (c, یہ) &&
		  axisIndicesList.sanitize (c, یہ) &&
		  glyph_اندراجات.sanitize (c, یہ));
  }

  ساخت accelerator_t
  {
    دوست ساخت VarComponent;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<VARC> (face);
    }
    ~accelerator_t ()
    {
      خودکار *scratch = cached_scratch.get_relaxed ();
      اگر (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      جدول.destroy ();
    }

    منطقی has_ڈیٹا () ثابت { واپس جدول->has_ڈیٹا (); }

    منطقی
    get_راستہ (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      اگر (!جدول->has_ڈیٹا ()) واپس جھوٹ;

      خودکار *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) واپس سچ;
      منطقی ret = جدول->get_راستہ (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      واپس ret;
    }

    منطقی احاطہ_glyphs (hb_سیٹ_t *glyphset) ثابت
    {
      اگر (جدول->has_ڈیٹا ())
	واپس جدول->احاطہ_glyphs (glyphset);
      واپس سچ;
    }

    خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
    {
      اگر (جدول->has_ڈیٹا ())
	جدول->depend (depend_ڈیٹا);
    }

    منطقی
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ثابت
    {
#اگر_غیر_تعریف_شدہ HB_NO_DRAW
      اگر (!جدول->has_ڈیٹا ()) واپس جھوٹ;

      hb_extents_t<> f_extents;

      خودکار *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) واپس سچ;
      منطقی ret = جدول->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      اگر (ret)
	*extents = f_extents.مقصد_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      واپس ret;
#ورنہ
      واپس جھوٹ;
#اگر_ختم
    }

    نجی:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_اشاریہ)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, حجم (hb_varc_scratch_t));
	اگر (unlikely (!scratch))
	  واپس خالی_اشاریہ;
      }

      واپس scratch;
    }
    خالی release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      اگر (!cached_scratch.cmpexch (خالی_اشاریہ, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    نجی:
    hb_blob_ptr_t<VARC> جدول;
    تغیرپذیر hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.major != 0; }

  محفوظ:
  Fixed_نسخہ<> نسخہ; /* Version identifier */
  Offset32_مقصد<Coverage> coverage;
  Offset32_مقصد<Multi_شے_Variation_Store> varStore;
  Offset32_مقصد<ConditionList> conditionList;
  Offset32_مقصد<TupleList> axisIndicesList;
  Offset32_مقصد<CFF2_اشاریہ/*Of<VarCompositeGlyph>*/> glyph_اندراجات;
  عوامی:
  DEFINE_سائز_STATIC (24);
};

ساخت VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#اگر_ختم

//}

}

#اگر_ختم  /* OT_VAR_VARC_VARC_HH */
