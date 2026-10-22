#ifndef OT_VAR_VARC_VARC_HH
#define OT_VAR_VARC_VARC_HH

#अन्तर्भाव "../../../hb-decycler.hh"
#अन्तर्भाव "../../../hb-geometry.hh"
#अन्तर्भाव "../../../hb-ot-layout-common.hh"
#अन्तर्भाव "../../../hb-ot-glyf-table.hh"
#अन्तर्भाव "../../../hb-ot-cff2-table.hh"
#अन्तर्भाव "../../../hb-ot-cff1-table.hh"

#अन्तर्भाव "coord-setter.hh"

संरचना hb_depend_data_builder_t;

नामाकाश OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#ifndef HB_NO_VAR_COMPOSITES

संरचना hb_varc_scratch_t
{
  hb_vector_t<unsigned> axisIndices;
  hb_vector_t<द्विगुण> axis_मूल्ये_2;
  hb_glyf_scratch_t glyf_scratch;
};

संरचना hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  mutable hb_decycler_t decycler;
  mutable signed edges_डावा;
  mutable signed depth_डावा;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

संरचना VARC;

संरचना VarComponent
{
  गणना वर्ग flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_मूल्ये_HAVE_VARIATION	= 1u << 2,
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

  संरचना record_t
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
    uint32_t axis_मूल्ये_var_idx;
    unsigned axis_मूल्ये_var_offset;
    unsigned axis_मूल्ये_var_size;
    uint32_t transform_var_idx;
    unsigned transform_var_offset;
    unsigned transform_var_size;
    hb_transform_decomposed_t<> transform;
    unsigned size;
  };

  HB_INTERNAL hb_ubytes_t
  get_path_at (ध्रुव hb_varc_context_t &c,
	       hb_codepoint_t parent_gid,
	       hb_array_t<ध्रुव पूर्णाङ्क> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache = nullptr) ध्रुव;

  HB_INTERNAL स्थिर तार्किक decompile_record (ध्रुव VARC &varc,
					    hb_ubytes_t record,
					    hb_vector_t<unsigned> *axis_indices,
					    hb_vector_t<द्विगुण> *axis_मूल्ये,
					    record_t *decoded /* OUT */,
					    int64_t *budget = nullptr);
};

संरचना VarCompositeGlyph
{
  स्थिर शून्य
  get_path_at (ध्रुव hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ध्रुव पूर्णाङ्क> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t record,
	       hb_scalar_cache_t *cache)
  {
    यावत् (record)
    {
      ध्रुव VarComponent &comp = * (ध्रुव VarComponent *) (record.arrayZ);
      record = comp.get_path_at (c,
				 gid,
				 coords, transform,
				 record,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

संरचना VARC
{
  friend संरचना VarComponent;

  स्थिर constexpr hb_tag_t tableTag = HB_TAG ('V', 'A', 'R', 'C');

  HB_INTERNAL तार्किक
  get_path_at (ध्रुव hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<ध्रुव पूर्णाङ्क> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_IDENTITY,
	       hb_codepoint_t parent_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *parent_cache = nullptr) ध्रुव;

  HB_INTERNAL तार्किक closure_glyphs (hb_set_t *glyphset) ध्रुव;
  HB_INTERNAL शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव;
  HB_INTERNAL तार्किक subset (hb_subset_context_t *c) ध्रुव;

  तार्किक
  get_path (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) ध्रुव
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 nullptr,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_मोजणी,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    प्रत्यागम get_path_at (c, gid,
			hb_array (font->coords, font->अंक_coords));
  }

  तार्किक
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) ध्रुव
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 nullptr,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_GRAPH_EDGE_मोजणी,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    प्रत्यागम get_path_at (c, gid,
			hb_array (font->coords, font->अंक_coords));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  version.major == 1 &&
		  coverage.sanitize (c, एतत्) &&
		  varStore.sanitize (c, एतत्) &&
		  conditionList.sanitize (c, एतत्) &&
		  axisIndicesList.sanitize (c, एतत्) &&
		  glyphRecords.sanitize (c, एतत्));
  }

  संरचना accelerator_t
  {
    friend संरचना VarComponent;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<VARC> (face);
    }
    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      यदि (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      table.destroy ();
    }

    तार्किक has_data () ध्रुव { प्रत्यागम table->has_data (); }

    तार्किक
    get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) ध्रुव
    {
      यदि (!table->has_data ()) प्रत्यागम असत्य;

      auto *scratch = acquire_scratch ();
      यदि (unlikely (!scratch)) प्रत्यागम सत्य;
      तार्किक ret = table->get_path (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      प्रत्यागम ret;
    }

    तार्किक closure_glyphs (hb_set_t *glyphset) ध्रुव
    {
      यदि (table->has_data ())
	प्रत्यागम table->closure_glyphs (glyphset);
      प्रत्यागम सत्य;
    }

    शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव
    {
      यदि (table->has_data ())
	table->depend (depend_data);
    }

    तार्किक
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) ध्रुव
    {
#ifndef HB_NO_DRAW
      यदि (!table->has_data ()) प्रत्यागम असत्य;

      hb_extents_t<> f_extents;

      auto *scratch = acquire_scratch ();
      यदि (unlikely (!scratch)) प्रत्यागम सत्य;
      तार्किक ret = table->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      यदि (ret)
	*extents = f_extents.to_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      प्रत्यागम ret;
#अन्यथा
      प्रत्यागम असत्य;
#endif
    }

    गुप्त:

    hb_varc_scratch_t *acquire_scratch () ध्रुव
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      यदि (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, परिमाण (hb_varc_scratch_t));
	यदि (unlikely (!scratch))
	  प्रत्यागम nullptr;
      }

      प्रत्यागम scratch;
    }
    शून्य release_scratch (hb_varc_scratch_t *scratch) ध्रुव
    {
      यदि (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    गुप्त:
    hb_blob_ptr_t<VARC> table;
    mutable hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  तार्किक has_data () ध्रुव { प्रत्यागम version.major != 0; }

  रक्षित:
  FixedVersion<> version; /* Version identifier */
  Offset32To<Coverage> coverage;
  Offset32To<MultiItemVariationStore> varStore;
  Offset32To<ConditionList> conditionList;
  Offset32To<TupleList> axisIndicesList;
  Offset32To<CFF2Index/*Of<VarCompositeGlyph>*/> glyphRecords;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (24);
};

संरचना VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#endif

//}

}

#endif  /* OT_VAR_VARC_VARC_HH */
