#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-decycler.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-geometry.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-glyf-table.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-cff2-table.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-cff1-table.hh"

#ಅನ್ತರ್ಭಾವ "coord-setter.hh"

ಸಂರಚನಾ hb_depend_data_builder_t;

ನಾಮಾಕಾಶ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

ಸಂರಚನಾ hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<ದ್ವಿಗುಣ> axis_ಮೌಲ್ಯಗಳು_2;
  hb_glyf_scratch_t glyf_scratch;
};

ಸಂರಚನಾ hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_ಎಡ;
  mutable signed depth_ಎಡ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ಸಂರಚನಾ VARC;

ಸಂರಚನಾ VarComponent
{
  ಗಣನಾ ವರ್ಗ flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_ಮೌಲ್ಯಗಳು_HAVE_VARIATION	= 1u << 2,
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

  ಸಂರಚನಾ record_t
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
    uint32_t axis_ಮೌಲ್ಯಗಳು_var_idx;
    unsigned axis_ಮೌಲ್ಯಗಳು_var_offset;
    unsigned axis_ಮೌಲ್ಯಗಳು_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ಧ್ರುವ hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ;

  HB_INTERNAL ಸ್ಥಿರ ತಾರ್ಕಿಕ decompile_record (ಧ್ರುವ VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<ದ್ವಿಗುಣ> *axis_ಮೌಲ್ಯಗಳು,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

ಸಂರಚನಾ VarCompositeGlyph
{
  ಸ್ಥಿರ ಶೂನ್ಯ
  get_path_at (ಧ್ರುವ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    ಯಾವತ್ (record)
    {
      ಧ್ರುವ VarComponent &comp = * (ಧ್ರುವ VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ಸಂರಚನಾ VARC
{
  friend ಸಂರಚನಾ VarComponent;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL ತಾರ್ಕಿಕ
  get_path_at (ಧ್ರುವ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ಧ್ರುವ;

  HB_INTERNAL ತಾರ್ಕಿಕ closure_glyphs (hb_set_t *glyphset) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ;
  HB_INTERNAL ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ಧ್ರುವ
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ಎಣಿಕೆ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ಪ್ರತ್ಯಾಗಮ get_path_at (c, gid,
			hb_array (font->coords, font->ಅಂಕೆ_coords));
  }

  ತಾರ್ಕಿಕ
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ಧ್ರುವ
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ಎಣಿಕೆ,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ಪ್ರತ್ಯಾಗಮ get_path_at (c, gid,
			hb_array (font->coords, font->ಅಂಕೆ_coords));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, ಏತತ್) &&
		  varStore.sanitize (c, ಏತತ್) &&
		  conditionList.sanitize (c, ಏತತ್) &&
		  axisIndicesList.sanitize (c, ಏತತ್) &&
		  glyphRecords.sanitize (c, ಏತತ್));
  }

  ಸಂರಚನಾ accelerator_t
  {
    friend ಸಂರಚನಾ VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      ಯದಿ (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table->has_data (); }

    ತಾರ್ಕಿಕ
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ಧ್ರುವ
    {
      ಯದಿ (!table->has_data ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      auto *scratch = acquire_scratch ();
      ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ತಾರ್ಕಿಕ ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ತಾರ್ಕಿಕ closure_glyphs (hb_set_t *glyphset) ಧ್ರುವ
    {
      ಯದಿ (table->has_data ())
	ಪ್ರತ್ಯಾಗಮ table->closure_glyphs (glyphset);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
    {
      ಯದಿ (table->has_data ())
	table->depend (depend_data);
    }

    ತಾರ್ಕಿಕ
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ಧ್ರುವ
    {
#ifndef HB_NO_DRAW
      ಯದಿ (!table->has_data ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ತಾರ್ಕಿಕ ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      ಯದಿ (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      ಪ್ರತ್ಯಾಗಮ ret;
#ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
    }

    ಗುಪ್ತ:

    hb_varc_scratch_t *acquire_scratch () ಧ್ರುವ
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      ಯದಿ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_varc_scratch_t));
	ಯದಿ (unlikely (!scratch))
	  ಪ್ರತ್ಯಾಗಮ nullptr;
      }

      ಪ್ರತ್ಯಾಗಮ scratch;
    }
    ಶೂನ್ಯ release_scratch (hb_varc_scratch_t *scratch) ಧ್ರುವ
    {
      ಯದಿ (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    ಗುಪ್ತ:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.major != 0; }

  ರಕ್ಷಿತ:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (24);
};

ಸಂರಚನಾ VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
