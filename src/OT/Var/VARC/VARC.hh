#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#অন্তর্ভাব "../../../hb-decycler.hh"
#অন্তর্ভাব "../../../hb-geometry.hh"
#অন্তর্ভাব "../../../hb-ot-layout-common.hh"
#অন্তর্ভাব "../../../hb-ot-glyf-table.hh"
#অন্তর্ভাব "../../../hb-ot-cff2-table.hh"
#অন্তর্ভাব "../../../hb-ot-cff1-table.hh"

#অন্তর্ভাব "coord-setter.hh"

কাঠামো hb_depend_তথ্য_builder_t;

নামাকাশ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

কাঠামো hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<দ্বিগুণ> axis_মানসমূহ_2;
  hb_glyf_scratch_t glyf_scratch;
};

কাঠামো hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_বাম;
  mutable signed depth_বাম;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

কাঠামো VARC;

কাঠামো VarComponent
{
  গণনা শ্রেণী flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_মানসমূহ_HAVE_VARIATION	= 1u << 2,
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

  কাঠামো record_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    unsigned gid_offset;
    unsigned gid_আকার;
    unsigned condition_সূচক;
    unsigned condition_offset;
    unsigned condition_আকার;
    unsigned axis_indices_সূচক;
    unsigned axis_indices_offset;
    unsigned axis_indices_আকার;
    uint32_t axis_মানসমূহ_var_idx;
    unsigned axis_মানসমূহ_var_offset;
    unsigned axis_মানসমূহ_var_আকার;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_আকার;
    hb_transform_decomposed_t<> transform;
    unsigned আকার_2;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ধ্রুবক hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ধ্রুবক;

  HB_INTERNAL স্থির যৌক্তিক decompile_record (ধ্রুবক VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<দ্বিগুণ> *axis_মানসমূহ,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

কাঠামো VarCompositeGlyph
{
  স্থির শূন্য
  get_path_at (ধ্রুবক hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    যখন (record)
    {
      ধ্রুবক VarComponent &comp = * (ধ্রুবক VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

কাঠামো VARC
{
  friend কাঠামো VarComponent;

  স্থির constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL যৌক্তিক
  get_path_at (ধ্রুবক hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ধ্রুবক;

  HB_INTERNAL যৌক্তিক closure_glyphs (hb_set_t *glyphset) ধ্রুবক;
  HB_INTERNAL শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক;
  HB_INTERNAL যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক;

  যৌক্তিক
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ধ্রুবক
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_গুনতি,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ফেরত get_path_at (c, gid,
			hb_array (font->coords, font->অঙ্ক_coords));
  }

  যৌক্তিক
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ধ্রুবক
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_গুনতি,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ফেরত get_path_at (c, gid,
			hb_array (font->coords, font->অঙ্ক_coords));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, এই) &&
		  varStore.sanitize (c, এই) &&
		  conditionList.sanitize (c, এই) &&
		  axisIndicesList.sanitize (c, এই) &&
		  glyphRecords.sanitize (c, এই));
  }

  কাঠামো accelerator_t
  {
    friend কাঠামো VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      যদি (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত table->has_তথ্য (); }

    যৌক্তিক
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ধ্রুবক
    {
      যদি (!table->has_তথ্য ()) ফেরত মিথ্যা;

      auto *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ফেরত সত্য;
      যৌক্তিক ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      ফেরত ret;
    }

    যৌক্তিক closure_glyphs (hb_set_t *glyphset) ধ্রুবক
    {
      যদি (table->has_তথ্য ())
	ফেরত table->closure_glyphs (glyphset);
      ফেরত সত্য;
    }

    শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
    {
      যদি (table->has_তথ্য ())
	table->depend (depend_তথ্য);
    }

    যৌক্তিক
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ধ্রুবক
    {
#ifndef HB_NO_DRAW
      যদি (!table->has_তথ্য ()) ফেরত মিথ্যা;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ফেরত সত্য;
      যৌক্তিক ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      যদি (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      ফেরত ret;
#অন্যথা
      ফেরত মিথ্যা;
#endif
    }

    ব্যক্তিগত:

    hb_varc_scratch_t *acquire_scratch () ধ্রুবক
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, আকার (hb_varc_scratch_t));
	যদি (unlikely (!scratch))
	  ফেরত nullptr;
      }

      ফেরত scratch;
    }
    শূন্য release_scratch (hb_varc_scratch_t *scratch) ধ্রুবক
    {
      যদি (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    ব্যক্তিগত:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.major != 0; }

  সুরক্ষিত:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2_সূচক/*Of<VarCompositeGlyph>*/> glyphRecords;
  সর্বজনীন:
  DEFINE_আকার_STATIC (24);
};

কাঠামো VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
