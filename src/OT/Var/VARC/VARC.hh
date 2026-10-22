#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#অন্তর্ভাব "../../../hb-decycler.hh"
#অন্তর্ভাব "../../../hb-geometry.hh"
#অন্তর্ভাব "../../../hb-ot-layout-common.hh"
#অন্তর্ভাব "../../../hb-ot-glyf-table.hh"
#অন্তর্ভাব "../../../hb-ot-cff2-table.hh"
#অন্তর্ভাব "../../../hb-ot-cff1-table.hh"

#অন্তর্ভাব "coord-setter.hh"

সংরচনা hb_depend_data_builder_t;

নামাকাশ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

সংরচনা hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<দ্বিগুণ> axis_মানসমূহ_2;
  hb_glyf_scratch_t glyf_scratch;
};

সংরচনা hb_varc_context_t
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

সংরচনা VARC;

সংরচনা VarComponent
{
  গণনা বর্গ flags_t : uint32_t
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

  সংরচনা record_t
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
    uint32_t axis_মানসমূহ_var_idx;
    unsigned axis_মানসমূহ_var_offset;
    unsigned axis_মানসমূহ_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ধ্রুব hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ধ্রুব;

  HB_INTERNAL স্থির তার্কিক decompile_record (ধ্রুব VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<দ্বিগুণ> *axis_মানসমূহ,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

সংরচনা VarCompositeGlyph
{
  স্থির শূন্য
  get_path_at (ধ্রুব hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    যাবত্ (record)
    {
      ধ্রুব VarComponent &comp = * (ধ্রুব VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

সংরচনা VARC
{
  friend সংরচনা VarComponent;

  স্থির constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL তার্কিক
  get_path_at (ধ্রুব hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ধ্রুব;

  HB_INTERNAL তার্কিক closure_glyphs (hb_set_t *glyphset) ধ্রুব;
  HB_INTERNAL শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুব;
  HB_INTERNAL তার্কিক subset (hb_subset_context_t *c) ধ্রুব;

  তার্কিক
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ধ্রুব
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

    প্রত্যাগম get_path_at (c, gid,
			hb_array (font->coords, font->অঙ্ক_coords));
  }

  তার্কিক
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ধ্রুব
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

    প্রত্যাগম get_path_at (c, gid,
			hb_array (font->coords, font->অঙ্ক_coords));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, এতত্) &&
		  varStore.sanitize (c, এতত্) &&
		  conditionList.sanitize (c, এতত্) &&
		  axisIndicesList.sanitize (c, এতত্) &&
		  glyphRecords.sanitize (c, এতত্));
  }

  সংরচনা accelerator_t
  {
    friend সংরচনা VarComponent;

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

    তার্কিক has_data () ধ্রুব { প্রত্যাগম table->has_data (); }

    তার্কিক
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ধ্রুব
    {
      যদি (!table->has_data ()) প্রত্যাগম অসত্য;

      auto *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) প্রত্যাগম সত্য;
      তার্কিক ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      প্রত্যাগম ret;
    }

    তার্কিক closure_glyphs (hb_set_t *glyphset) ধ্রুব
    {
      যদি (table->has_data ())
	প্রত্যাগম table->closure_glyphs (glyphset);
      প্রত্যাগম সত্য;
    }

    শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুব
    {
      যদি (table->has_data ())
	table->depend (depend_data);
    }

    তার্কিক
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ধ্রুব
    {
#ifndef HB_NO_DRAW
      যদি (!table->has_data ()) প্রত্যাগম অসত্য;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) প্রত্যাগম সত্য;
      তার্কিক ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      যদি (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      প্রত্যাগম ret;
#অন্যথা
      প্রত্যাগম অসত্য;
#endif
    }

    গুপ্ত:

    hb_varc_scratch_t *acquire_scratch () ধ্রুব
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, পরিমাণ (hb_varc_scratch_t));
	যদি (unlikely (!scratch))
	  প্রত্যাগম nullptr;
      }

      প্রত্যাগম scratch;
    }
    শূন্য release_scratch (hb_varc_scratch_t *scratch) ধ্রুব
    {
      যদি (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    গুপ্ত:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.major != 0; }

  রক্ষিত:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (24);
};

সংরচনা VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
