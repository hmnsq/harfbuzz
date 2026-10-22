#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#شامل "../../../hb-decycler.hh"
#شامل "../../../hb-geometry.hh"
#شامل "../../../hb-ot-layout-common.hh"
#شامل "../../../hb-ot-glyf-table.hh"
#شامل "../../../hb-ot-cff2-table.hh"
#شامل "../../../hb-ot-cff1-table.hh"

#شامل "coord-setter.hh"

ساختار hb_depend_data_builder_t;

فضاینام OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

ساختار hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<دوگانه> axis_مقادیر_2;
  hb_glyf_scratch_t glyf_scratch;
};

ساختار hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_چپ;
  mutable signed depth_چپ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ساختار VARC;

ساختار VarComponent
{
  شمارش رده flags_t : uint32_t
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
    GID_IS_24BIT		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_MASK		= ~((1u << 15) - 1),
  };

  ساختار record_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    unsigned gid_offset;
    unsigned gid_size;
    unsigned condition_index;
    unsigned condition_offset;
    unsigned condition_size;
    unsigned axis_indices_index;
    unsigned axis_indices_offset;
    unsigned axis_indices_size;
    uint32_t axis_مقادیر_var_idx;
    unsigned axis_مقادیر_var_offset;
    unsigned axis_مقادیر_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ثابت;

  HB_INTERNAL ایستا منطقی decompile_record (ثابت VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<دوگانه> *axis_مقادیر,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

ساختار VarCompositeGlyph
{
  ایستا تهی
  get_path_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    وقتی (record)
    {
      ثابت VarComponent &comp = * (ثابت VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ساختار VARC
{
  friend ساختار VarComponent;

  ایستا constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL منطقی
  get_path_at (ثابت hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ثابت صحیح> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ثابت;

  HB_INTERNAL منطقی closure_glyphs (hb_set_t *glyphset) ثابت;
  HB_INTERNAL تهی depend (hb_depend_data_builder_t *depend_data) ثابت;
  HB_INTERNAL منطقی subset (hb_subset_context_t *c) ثابت;

  منطقی
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ثابت
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_شمار,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    بازگشت get_path_at (c, gid,
			hb_array (font->coords, font->رقم_coords));
  }

  منطقی
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ثابت
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_شمار,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    بازگشت get_path_at (c, gid,
			hb_array (font->coords, font->رقم_coords));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, این) &&
		  varStore.sanitize (c, این) &&
		  conditionList.sanitize (c, این) &&
		  axisIndicesList.sanitize (c, این) &&
		  glyphRecords.sanitize (c, این));
  }

  ساختار accelerator_t
  {
    friend ساختار VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      اگر (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    منطقی has_data () ثابت { بازگشت table->has_data (); }

    منطقی
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ثابت
    {
      اگر (!table->has_data ()) بازگشت نادرست;

      auto *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) بازگشت درست;
      منطقی ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      بازگشت ret;
    }

    منطقی closure_glyphs (hb_set_t *glyphset) ثابت
    {
      اگر (table->has_data ())
	بازگشت table->closure_glyphs (glyphset);
      بازگشت درست;
    }

    تهی depend (hb_depend_data_builder_t *depend_data) ثابت
    {
      اگر (table->has_data ())
	table->depend (depend_data);
    }

    منطقی
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ثابت
    {
#ifndef HB_NO_DRAW
      اگر (!table->has_data ()) بازگشت نادرست;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) بازگشت درست;
      منطقی ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      اگر (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      بازگشت ret;
#وگرنه
      بازگشت نادرست;
#endif
    }

    خصوصی:

    hb_varc_scratch_t *acquire_scratch () ثابت
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, اندازه (hb_varc_scratch_t));
	اگر (unlikely (!scratch))
	  بازگشت nullptr;
      }

      بازگشت scratch;
    }
    تهی release_scratch (hb_varc_scratch_t *scratch) ثابت
    {
      اگر (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    خصوصی:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  منطقی has_data () ثابت { بازگشت version.major != 0; }

  محافظتشده:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  عمومی:
  DEFINE_SIZE_STATIC (24);
};

ساختار VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
