#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#અન્તર્ભાવ "../../../hb-decycler.hh"
#અન્તર્ભાવ "../../../hb-geometry.hh"
#અન્તર્ભાવ "../../../hb-ot-layout-common.hh"
#અન્તર્ભાવ "../../../hb-ot-glyf-table.hh"
#અન્તર્ભાવ "../../../hb-ot-cff2-table.hh"
#અન્તર્ભાવ "../../../hb-ot-cff1-table.hh"

#અન્તર્ભાવ "coord-setter.hh"

સંરચના hb_depend_data_builder_t;

નામાકાશ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

સંરચના hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<દ્વિગુણ> axis_કિંમતો_2;
  hb_glyf_scratch_t glyf_scratch;
};

સંરચના hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_ડાબું;
  mutable signed depth_ડાબું;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

સંરચના VARC;

સંરચના VarComponent
{
  ગણના વર્ગ flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_કિંમતો_HAVE_VARIATION	= 1u << 2,
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

  સંરચના record_t
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
    uint32_t axis_કિંમતો_var_idx;
    unsigned axis_કિંમતો_var_offset;
    unsigned axis_કિંમતો_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (અચલ hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<અચલ પૂર્ણાંક> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) અચલ;

  HB_INTERNAL સ્થિર તાર્કિક decompile_record (અચલ VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<દ્વિગુણ> *axis_કિંમતો,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

સંરચના VarCompositeGlyph
{
  સ્થિર શૂન્ય
  get_path_at (અચલ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<અચલ પૂર્ણાંક> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    જ્યારે (record)
    {
      અચલ VarComponent &comp = * (અચલ VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

સંરચના VARC
{
  friend સંરચના VarComponent;

  સ્થિર constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL તાર્કિક
  get_path_at (અચલ hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<અચલ પૂર્ણાંક> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) અચલ;

  HB_INTERNAL તાર્કિક closure_glyphs (hb_set_t *glyphset) અચલ;
  HB_INTERNAL શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ;
  HB_INTERNAL તાર્કિક subset (hb_subset_context_t *c) અચલ;

  તાર્કિક
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) અચલ
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ગણતરી,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    પાછા get_path_at (c, gid,
			hb_array (font->coords, font->અંક_coords));
  }

  તાર્કિક
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) અચલ
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_ગણતરી,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    પાછા get_path_at (c, gid,
			hb_array (font->coords, font->અંક_coords));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, આ) &&
		  varStore.sanitize (c, આ) &&
		  conditionList.sanitize (c, આ) &&
		  axisIndicesList.sanitize (c, આ) &&
		  glyphRecords.sanitize (c, આ));
  }

  સંરચના accelerator_t
  {
    friend સંરચના VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      જો (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    તાર્કિક has_data () અચલ { પાછા table->has_data (); }

    તાર્કિક
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) અચલ
    {
      જો (!table->has_data ()) પાછા ખોટું;

      auto *scratch = acquire_scratch ();
      જો (unlikely (!scratch)) પાછા સાચું;
      તાર્કિક ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      પાછા ret;
    }

    તાર્કિક closure_glyphs (hb_set_t *glyphset) અચલ
    {
      જો (table->has_data ())
	પાછા table->closure_glyphs (glyphset);
      પાછા સાચું;
    }

    શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ
    {
      જો (table->has_data ())
	table->depend (depend_data);
    }

    તાર્કિક
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) અચલ
    {
#ifndef HB_NO_DRAW
      જો (!table->has_data ()) પાછા ખોટું;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      જો (unlikely (!scratch)) પાછા સાચું;
      તાર્કિક ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      જો (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      પાછા ret;
#નહીંતર
      પાછા ખોટું;
#endif
    }

    ખાનગી:

    hb_varc_scratch_t *acquire_scratch () અચલ
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      જો (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, કદ (hb_varc_scratch_t));
	જો (unlikely (!scratch))
	  પાછા nullptr;
      }

      પાછા scratch;
    }
    શૂન્ય release_scratch (hb_varc_scratch_t *scratch) અચલ
    {
      જો (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    ખાનગી:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  તાર્કિક has_data () અચલ { પાછા version.major != 0; }

  સુરક્ષિત:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  જાહેર:
  DEFINE_SIZE_STATIC (24);
};

સંરચના VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
