#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#அந்தர்பாவ "../../../hb-decycler.hh"
#அந்தர்பாவ "../../../hb-geometry.hh"
#அந்தர்பாவ "../../../hb-ot-layout-common.hh"
#அந்தர்பாவ "../../../hb-ot-glyf-table.hh"
#அந்தர்பாவ "../../../hb-ot-cff2-table.hh"
#அந்தர்பாவ "../../../hb-ot-cff1-table.hh"

#அந்தர்பாவ "coord-setter.hh"

கட்டமைப்பு hb_depend_data_builder_t;

நாமாகாஶ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

கட்டமைப்பு hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<இரட்டை> axis_மதிப்புகள்_2;
  hb_glyf_scratch_t glyf_scratch;
};

கட்டமைப்பு hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_இடது;
  mutable signed depth_இடது;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

கட்டமைப்பு VARC;

கட்டமைப்பு VarComponent
{
  எண்ணிக்கை வகுப்பு flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_மதிப்புகள்_HAVE_VARIATION	= 1u << 2,
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

  கட்டமைப்பு record_t
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
    uint32_t axis_மதிப்புகள்_var_idx;
    unsigned axis_மதிப்புகள்_var_offset;
    unsigned axis_மதிப்புகள்_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (மாறிலி hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<மாறிலி முழு> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) மாறிலி;

  HB_INTERNAL நிலையான தருக்க decompile_record (மாறிலி VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<இரட்டை> *axis_மதிப்புகள்,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

கட்டமைப்பு VarCompositeGlyph
{
  நிலையான வெறுமை
  get_path_at (மாறிலி hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<மாறிலி முழு> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    போது (record)
    {
      மாறிலி VarComponent &comp = * (மாறிலி VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

கட்டமைப்பு VARC
{
  friend கட்டமைப்பு VarComponent;

  நிலையான constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL தருக்க
  get_path_at (மாறிலி hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<மாறிலி முழு> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) மாறிலி;

  HB_INTERNAL தருக்க closure_glyphs (hb_set_t *glyphset) மாறிலி;
  HB_INTERNAL வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி;
  HB_INTERNAL தருக்க subset (hb_subset_context_t *c) மாறிலி;

  தருக்க
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) மாறிலி
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_மொத்தம்,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    திரும்பு get_path_at (c, gid,
			hb_array (font->coords, font->இலக்கம்_coords));
  }

  தருக்க
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) மாறிலி
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_மொத்தம்,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    திரும்பு get_path_at (c, gid,
			hb_array (font->coords, font->இலக்கம்_coords));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, இது) &&
		  varStore.sanitize (c, இது) &&
		  conditionList.sanitize (c, இது) &&
		  axisIndicesList.sanitize (c, இது) &&
		  glyphRecords.sanitize (c, இது));
  }

  கட்டமைப்பு accelerator_t
  {
    friend கட்டமைப்பு VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      எனில் (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    தருக்க has_data () மாறிலி { திரும்பு table->has_data (); }

    தருக்க
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) மாறிலி
    {
      எனில் (!table->has_data ()) திரும்பு பொய்;

      auto *scratch = acquire_scratch ();
      எனில் (unlikely (!scratch)) திரும்பு உண்மை;
      தருக்க ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      திரும்பு ret;
    }

    தருக்க closure_glyphs (hb_set_t *glyphset) மாறிலி
    {
      எனில் (table->has_data ())
	திரும்பு table->closure_glyphs (glyphset);
      திரும்பு உண்மை;
    }

    வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி
    {
      எனில் (table->has_data ())
	table->depend (depend_data);
    }

    தருக்க
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) மாறிலி
    {
#ifndef HB_NO_DRAW
      எனில் (!table->has_data ()) திரும்பு பொய்;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      எனில் (unlikely (!scratch)) திரும்பு உண்மை;
      தருக்க ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      எனில் (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      திரும்பு ret;
#இல்லையெனில்
      திரும்பு பொய்;
#endif
    }

    தனிப்பட்ட:

    hb_varc_scratch_t *acquire_scratch () மாறிலி
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      எனில் (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, அளவு (hb_varc_scratch_t));
	எனில் (unlikely (!scratch))
	  திரும்பு nullptr;
      }

      திரும்பு scratch;
    }
    வெறுமை release_scratch (hb_varc_scratch_t *scratch) மாறிலி
    {
      எனில் (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    தனிப்பட்ட:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  தருக்க has_data () மாறிலி { திரும்பு version.major != 0; }

  பாதுகாக்கப்பட்ட:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  பொது:
  DEFINE_SIZE_STATIC (24);
};

கட்டமைப்பு VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
