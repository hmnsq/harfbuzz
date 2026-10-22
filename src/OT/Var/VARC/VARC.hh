#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#അന്തര്ഭാവ "../../../hb-decycler.hh"
#അന്തര്ഭാവ "../../../hb-geometry.hh"
#അന്തര്ഭാവ "../../../hb-ot-layout-common.hh"
#അന്തര്ഭാവ "../../../hb-ot-glyf-table.hh"
#അന്തര്ഭാവ "../../../hb-ot-cff2-table.hh"
#അന്തര്ഭാവ "../../../hb-ot-cff1-table.hh"

#അന്തര്ഭാവ "coord-setter.hh"

ഘടന hb_depend_data_builder_t;

നാമാകാശ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

ഘടന hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<ഇരട്ട> axis_മൂല്യങ്ങൾ_2;
  hb_glyf_scratch_t glyf_scratch;
};

ഘടന hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_ഇടത്;
  mutable signed depth_ഇടത്;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ഘടന VARC;

ഘടന VarComponent
{
  എണ്ണൽ വർഗ്ഗം flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_മൂല്യങ്ങൾ_HAVE_VARIATION	= 1u << 2,
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

  ഘടന record_t
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
    uint32_t axis_മൂല്യങ്ങൾ_var_idx;
    unsigned axis_മൂല്യങ്ങൾ_var_offset;
    unsigned axis_മൂല്യങ്ങൾ_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (സ്ഥിരം hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) സ്ഥിരം;

  HB_INTERNAL സ്ഥിതം തർക്കം decompile_record (സ്ഥിരം VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<ഇരട്ട> *axis_മൂല്യങ്ങൾ,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

ഘടന VarCompositeGlyph
{
  സ്ഥിതം ശൂന്യം
  get_path_at (സ്ഥിരം hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    ആകുമ്പോൾ (record)
    {
      സ്ഥിരം VarComponent &comp = * (സ്ഥിരം VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ഘടന VARC
{
  friend ഘടന VarComponent;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL തർക്കം
  get_path_at (സ്ഥിരം hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) സ്ഥിരം;

  HB_INTERNAL തർക്കം closure_glyphs (hb_set_t *glyphset) സ്ഥിരം;
  HB_INTERNAL ശൂന്യം depend (hb_depend_data_builder_t *depend_data) സ്ഥിരം;
  HB_INTERNAL തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം;

  തർക്കം
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) സ്ഥിരം
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_എണ്ണം,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    തിരികെ get_path_at (c, gid,
			hb_array (font->coords, font->അക്കം_coords));
  }

  തർക്കം
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) സ്ഥിരം
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_എണ്ണം,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    തിരികെ get_path_at (c, gid,
			hb_array (font->coords, font->അക്കം_coords));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, ഇത്) &&
		  varStore.sanitize (c, ഇത്) &&
		  conditionList.sanitize (c, ഇത്) &&
		  axisIndicesList.sanitize (c, ഇത്) &&
		  glyphRecords.sanitize (c, ഇത്));
  }

  ഘടന accelerator_t
  {
    friend ഘടന VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      എങ്കിൽ (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    തർക്കം has_data () സ്ഥിരം { തിരികെ table->has_data (); }

    തർക്കം
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) സ്ഥിരം
    {
      എങ്കിൽ (!table->has_data ()) തിരികെ അസത്യം;

      auto *scratch = acquire_scratch ();
      എങ്കിൽ (unlikely (!scratch)) തിരികെ സത്യം;
      തർക്കം ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      തിരികെ ret;
    }

    തർക്കം closure_glyphs (hb_set_t *glyphset) സ്ഥിരം
    {
      എങ്കിൽ (table->has_data ())
	തിരികെ table->closure_glyphs (glyphset);
      തിരികെ സത്യം;
    }

    ശൂന്യം depend (hb_depend_data_builder_t *depend_data) സ്ഥിരം
    {
      എങ്കിൽ (table->has_data ())
	table->depend (depend_data);
    }

    തർക്കം
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) സ്ഥിരം
    {
#ifndef HB_NO_DRAW
      എങ്കിൽ (!table->has_data ()) തിരികെ അസത്യം;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      എങ്കിൽ (unlikely (!scratch)) തിരികെ സത്യം;
      തർക്കം ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      എങ്കിൽ (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      തിരികെ ret;
#അല്ലെങ്കിൽ
      തിരികെ അസത്യം;
#endif
    }

    സ്വകാര്യ:

    hb_varc_scratch_t *acquire_scratch () സ്ഥിരം
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      എങ്കിൽ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, വലുപ്പം (hb_varc_scratch_t));
	എങ്കിൽ (unlikely (!scratch))
	  തിരികെ nullptr;
      }

      തിരികെ scratch;
    }
    ശൂന്യം release_scratch (hb_varc_scratch_t *scratch) സ്ഥിരം
    {
      എങ്കിൽ (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    സ്വകാര്യ:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.major != 0; }

  സംരക്ഷിത:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  പൊതു:
  DEFINE_SIZE_STATIC (24);
};

ഘടന VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
