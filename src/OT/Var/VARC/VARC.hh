#अगर_अघोषित OT_VAR_VARC_VARC_HH
#घोषणा OT_VAR_VARC_VARC_HH

#समावेश "../../../hb-decycler.hh"
#समावेश "../../../hb-geometry.hh"
#समावेश "../../../hb-ot-layout-common.hh"
#समावेश "../../../hb-ot-glyf-table.hh"
#समावेश "../../../hb-ot-cff2-table.hh"
#समावेश "../../../hb-ot-cff1-table.hh"

#समावेश "coord-setter.hh"

काष्ठा hb_depend_तथ्य_builder_t;

namespace OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#अगर_अघोषित HB_NO_VAR_COMPOSITES

काष्ठा hb_varc_scratch_t
{
  hb_vector_t<अचिन्हित> axisIndices;
  hb_vector_t<द्विगुन> axis_मूल्यावली_2;
  hb_glyf_scratch_t glyf_scratch;
};

काष्ठा hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable चिन्हित edges_बायाँ;
  mutable चिन्हित depth_बायाँ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

काष्ठा VARC;

काष्ठा VarComponent
{
  क्रमागत श्रेणी flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_मूल्यावली_HAVE_VARIATION	= 1u << 2,
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

  काष्ठा record_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    अचिन्हित gid_offset;
    अचिन्हित gid_आकार;
    अचिन्हित condition_सूचकांक;
    अचिन्हित condition_offset;
    अचिन्हित condition_आकार;
    अचिन्हित axis_indices_सूचकांक;
    अचिन्हित axis_indices_offset;
    अचिन्हित axis_indices_आकार;
    uint32_t axis_मूल्यावली_var_idx;
    अचिन्हित axis_मूल्यावली_var_offset;
    अचिन्हित axis_मूल्यावली_var_आकार;
    uint32_t transform_var_idx;
    अचिन्हित transform_var_offset;
    अचिन्हित transform_var_आकार;
    hb_transform_decomposed_t<> transform;
    अचिन्हित आकार;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (स्थिर hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<स्थिर पूर्णांक> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) स्थिर;

  HB_INTERNAL अटल bool decompile_record (स्थिर VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<अचिन्हित> *axis_indices,
					    hb_vector_t<द्विगुन> *axis_मूल्यावली,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

काष्ठा VarCompositeGlyph
{
  अटल व्योम
  get_path_at (स्थिर hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<स्थिर पूर्णांक> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    जबतक (record)
    {
      स्थिर VarComponent &comp = * (स्थिर VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

काष्ठा VARC
{
  मित्र काष्ठा VarComponent;

  अटल constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL bool
  get_path_at (स्थिर hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<स्थिर पूर्णांक> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) स्थिर;

  HB_INTERNAL bool closure_glyphs (hb_set_t *glyphset) स्थिर;
  HB_INTERNAL व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर;
  HB_INTERNAL bool subset (hb_subset_context_t *c) स्थिर;

  bool
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) स्थिर
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_गिनती,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    वापस get_path_at (c, gid,
			hb_array (font->coords, font->अंक_coords));
  }

  bool
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) स्थिर
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_गिनती,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    वापस get_path_at (c, gid,
			hb_array (font->coords, font->अंक_coords));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, यह) &&
		  varStore.sanitize (c, यह) &&
		  conditionList.sanitize (c, यह) &&
		  axisIndicesList.sanitize (c, यह) &&
		  glyphRecords.sanitize (c, यह));
  }

  काष्ठा accelerator_t
  {
    मित्र काष्ठा VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      स्वतः *scratch = cached_scratch.get_relaxed ();
      अगर (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    bool has_तथ्य () स्थिर { वापस table->has_तथ्य (); }

    bool
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) स्थिर
    {
      अगर (!table->has_तथ्य ()) वापस false;

      स्वतः *scratch = acquire_scratch ();
      अगर (unlikely (!scratch)) वापस true;
      bool ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      वापस ret;
    }

    bool closure_glyphs (hb_set_t *glyphset) स्थिर
    {
      अगर (table->has_तथ्य ())
	वापस table->closure_glyphs (glyphset);
      वापस true;
    }

    व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
    {
      अगर (table->has_तथ्य ())
	table->depend (depend_तथ्य);
    }

    bool
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) स्थिर
    {
#अगर_अघोषित HB_NO_DRAW
      अगर (!table->has_तथ्य ()) वापस false;

      hb_extents_t<> f_extents;

      स्वतः *scratch = acquire_scratch ();
      अगर (unlikely (!scratch)) वापस true;
      bool ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      अगर (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      वापस ret;
#अन्यथा
      वापस false;
#पूर्ण_अगर
    }

    निजी:

    hb_varc_scratch_t *acquire_scratch () स्थिर
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      अगर (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, माप (hb_varc_scratch_t));
	अगर (unlikely (!scratch))
	  वापस nullptr;
      }

      वापस scratch;
    }
    व्योम release_scratch (hb_varc_scratch_t *scratch) स्थिर
    {
      अगर (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    निजी:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  bool has_तथ्य () स्थिर { वापस version.major != 0; }

  रक्षित:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2_सूचकांक/*Of<VarCompositeGlyph>*/> glyphRecords;
  खुला:
  DEFINE_आकार_STATIC (24);
};

काष्ठा VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#पूर्ण_अगर

//}

}

#पूर्ण_अगर  /* OT_VAR_VARC_VARC_HH */
