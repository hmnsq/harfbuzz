#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#ଅନ୍ତର୍ଭାଵ "../../../hb-decycler.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-geometry.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-cff1-table.hh"

#ଅନ୍ତର୍ଭାଵ "coord-setter.hh"

ସଂରଚନା hb_depend_data_builder_t;

ନାମାକାଶ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

ସଂରଚନା hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<ଦ୍ୱିଗୁଣ> axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
  hb_glyf_scratch_t glyf_scratch;
};

ସଂରଚନା hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_ବାମ;
  mutable signed depth_ବାମ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

ସଂରଚନା VARC;

ସଂରଚନା VarComponent
{
  ଗଣନା ଶ୍ରେଣୀ flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_ମୂଲ୍ୟଗୁଡ଼ିକ_HAVE_VARIATION	= 1u << 2,
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

  ସଂରଚନା record_t
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
    uint32_t axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_idx;
    unsigned axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_offset;
    unsigned axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ଅଚଳ hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ଅଚଳ;

  HB_INTERNAL ସ୍ଥିର ତାର୍କିକ decompile_record (ଅଚଳ VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<ଦ୍ୱିଗୁଣ> *axis_ମୂଲ୍ୟଗୁଡ଼ିକ,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

ସଂରଚନା VarCompositeGlyph
{
  ସ୍ଥିର ଶୂନ୍ୟ
  get_path_at (ଅଚଳ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    ଯେତେବେଳେ (record)
    {
      ଅଚଳ VarComponent &comp = * (ଅଚଳ VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

ସଂରଚନା VARC
{
  friend ସଂରଚନା VarComponent;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL ତାର୍କିକ
  get_path_at (ଅଚଳ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ଅଚଳ;

  HB_INTERNAL ତାର୍କିକ closure_glyphs (hb_set_t *glyphset) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ;
  HB_INTERNAL ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ;

  ତାର୍କିକ
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ଅଚଳ
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ଗଣନା,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ଫେରନ୍ତୁ get_path_at (c, gid,
			hb_array (font->coords, font->ଅଙ୍କ_coords));
  }

  ତାର୍କିକ
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ଅଚଳ
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ଗଣନା,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    ଫେରନ୍ତୁ get_path_at (c, gid,
			hb_array (font->coords, font->ଅଙ୍କ_coords));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, ଏହି) &&
		  varStore.sanitize (c, ଏହି) &&
		  conditionList.sanitize (c, ଏହି) &&
		  axisIndicesList.sanitize (c, ଏହି) &&
		  glyphRecords.sanitize (c, ଏହି));
  }

  ସଂରଚନା accelerator_t
  {
    friend ସଂରଚନା VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      ଯଦି (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ table->has_data (); }

    ତାର୍କିକ
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ଅଚଳ
    {
      ଯଦି (!table->has_data ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      auto *scratch = acquire_scratch ();
      ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ତାର୍କିକ ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      ଫେରନ୍ତୁ ret;
    }

    ତାର୍କିକ closure_glyphs (hb_set_t *glyphset) ଅଚଳ
    {
      ଯଦି (table->has_data ())
	ଫେରନ୍ତୁ table->closure_glyphs (glyphset);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
    {
      ଯଦି (table->has_data ())
	table->depend (depend_data);
    }

    ତାର୍କିକ
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ଅଚଳ
    {
#ifndef HB_NO_DRAW
      ଯଦି (!table->has_data ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ତାର୍କିକ ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      ଯଦି (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      ଫେରନ୍ତୁ ret;
#ଅନ୍ୟଥା
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
    }

    ବ୍ୟକ୍ତିଗତ:

    hb_varc_scratch_t *acquire_scratch () ଅଚଳ
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      ଯଦି (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, ଆକାର (hb_varc_scratch_t));
	ଯଦି (unlikely (!scratch))
	  ଫେରନ୍ତୁ nullptr;
      }

      ଫେରନ୍ତୁ scratch;
    }
    ଶୂନ୍ୟ release_scratch (hb_varc_scratch_t *scratch) ଅଚଳ
    {
      ଯଦି (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    ବ୍ୟକ୍ତିଗତ:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.major != 0; }

  ସୁରକ୍ଷିତ:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (24);
};

ସଂରଚନା VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
