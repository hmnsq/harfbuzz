#నిర్వచితం_కాకపోతే OT_VAR_VARC_VARC_HH
#నిర్వచించు OT_VAR_VARC_VARC_HH

#అన్తర్భావ "../../../hb-decycler.hh"
#అన్తర్భావ "../../../hb-geometry.hh"
#అన్తర్భావ "../../../hb-ot-layout-common.hh"
#అన్తర్భావ "../../../hb-ot-glyf-table.hh"
#అన్తర్భావ "../../../hb-ot-cff2-table.hh"
#అన్తర్భావ "../../../hb-ot-cff1-table.hh"

#అన్తర్భావ "coord-setter.hh"

నిర్మాణం hb_depend_దత్తాంశం_builder_t;

నామాకాశ OT {

//namespace Var {

/*
 * VARC -- Variable Composites
 * https://github.com/harfbuzz/boring-expansion-spec/blob/main/VARC.md
 */

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES

నిర్మాణం hb_varc_scratch_t
{
  hb_సదిశ_t<గుర్తులేని> axisIndices;
  hb_సదిశ_t<ద్విగుణ> axis_విలువలు_2;
  hb_glyf_scratch_t glyf_scratch;
};

నిర్మాణం hb_varc_context_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;
  hb_extents_t<> *extents;
  మార్చదగిన hb_decycler_t decycler;
  మార్చదగిన గుర్తుగల అంచులు_ఎడమ;
  మార్చదగిన గుర్తుగల లోతు_ఎడమ;
  /* Renderer-owned live budget, shared by VARC record processing and
   * every leaf loaded from glyf/CFF/CFF2. */
  int64_t &budget;
  hb_varc_scratch_t &scratch;
};

నిర్మాణం VARC;

నిర్మాణం VarComponent
{
  గణన తరగతి flags_t : uint32_t
  {
    RESET_UNSPECIFIED_AXES	= 1u << 0,
    HAVE_AXES			= 1u << 1,
    AXIS_విలువలు_HAVE_VARIATION	= 1u << 2,
    TRANSFORM_HAS_VARIATION	= 1u << 3,
    HAVE_TRANSLATE_X		= 1u << 4,
    HAVE_TRANSLATE_Y		= 1u << 5,
    HAVE_ROTATION		= 1u << 6,
    HAVE_CONDITION		= 1u << 7,
    HAVE_SCALE_X		= 1u << 8,
    HAVE_SCALE_Y		= 1u << 9,
    HAVE_TCENTER_X		= 1u << 10,
    HAVE_TCENTER_Y		= 1u << 11,
    GID_IS_24_బిట్టు		= 1u << 12,
    HAVE_SKEW_X			= 1u << 13,
    HAVE_SKEW_Y			= 1u << 14,
    RESERVED_ముసుగు		= ~((1u << 15) - 1),
  };

  నిర్మాణం రికార్డు_t
  {
    uint32_t flags;
    hb_codepoint_t gid;
    గుర్తులేని gid_స్థానభ్రంశం;
    గుర్తులేని gid_కొలత;
    గుర్తులేని condition_సూచీ;
    గుర్తులేని condition_స్థానభ్రంశం;
    గుర్తులేని condition_కొలత;
    గుర్తులేని axis_indices_సూచీ;
    గుర్తులేని axis_indices_స్థానభ్రంశం;
    గుర్తులేని axis_indices_కొలత;
    uint32_t axis_విలువలు_var_idx;
    గుర్తులేని axis_విలువలు_var_స్థానభ్రంశం;
    గుర్తులేని axis_విలువలు_var_కొలత;
    uint32_t transform_var_idx;
    గుర్తులేని transform_var_స్థానభ్రంశం;
    గుర్తులేని transform_var_కొలత;
    hb_transform_decomposed_t<> transform;
    గుర్తులేని కొలత;
  };

  HB_అంతర్గత hb_ubytes_t
  get_మార్గం_at (స్థిరరాశి hb_varc_context_t &c,
	       hb_codepoint_t మాతృ_gid,
	       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t రికార్డు,
	       hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి;

  HB_అంతర్గత స్థిర తార్కిక decompile_రికార్డు (స్థిరరాశి VARC &varc,
					    hb_ubytes_t రికార్డు,
					    hb_సదిశ_t<గుర్తులేని> *axis_indices,
					    hb_సదిశ_t<ద్విగుణ> *axis_విలువలు,
					    రికార్డు_t *decoded /*బహిర్గతం*/,
					    int64_t *budget = శూన్యసూచిక);
};

నిర్మాణం Var_సంయుక్త_Glyph
{
  స్థిర శూన్య
  get_మార్గం_at (స్థిరరాశి hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
	       hb_transform_t<> transform,
	       hb_ubytes_t రికార్డు,
	       hb_scalar_cache_t *cache)
  {
    అయితే (రికార్డు)
    {
      స్థిరరాశి VarComponent &comp = * (స్థిరరాశి VarComponent *) (రికార్డు.arrayZ);
      రికార్డు = comp.get_మార్గం_at (c,
				 gid,
				 coords, transform,
				 రికార్డు,
				 cache);
    }
  }
};

HB_MARK_AS_FLAG_T (VarComponent::flags_t);

నిర్మాణం VARC
{
  మిత్ర నిర్మాణం VarComponent;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_ట్యాగ్ ('V', 'A', 'R', 'C');

  HB_అంతర్గత తార్కిక
  get_మార్గం_at (స్థిరరాశి hb_varc_context_t &c,
	       hb_codepoint_t gid,
	       hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
	       hb_transform_t<> transform = HB_TRANSFORM_తటస్థం,
	       hb_codepoint_t మాతృ_gid = HB_CODEPOINT_INVALID,
	       hb_scalar_cache_t *మాతృ_cache = శూన్యసూచిక) స్థిరరాశి;

  HB_అంతర్గత తార్కిక సంవృతం_glyphs (hb_సమితి_t *glyphset) స్థిరరాశి;
  HB_అంతర్గత శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి;
  HB_అంతర్గత తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి;

  తార్కిక
  get_మార్గం (hb_font_t *font,
	    hb_codepoint_t gid,
	    hb_draw_session_t &draw_session,
	    hb_varc_scratch_t &scratch) స్థిరరాశి
  {
    int64_t &budget = draw_session.get_budget ();
    hb_varc_context_t c {font,
			 &draw_session,
			 శూన్యసూచిక,
			 hb_decycler_t {},
			 HB_MAX_లేఖాచిత్రం_అంచు_లెక్క,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    తిరిగి get_మార్గం_at (c, gid,
			hb_array (font->coords, font->అంకె_coords));
  }

  తార్కిక
  get_extents (hb_font_t *font,
	       hb_codepoint_t gid,
	       hb_extents_t<> *extents,
	       hb_varc_scratch_t &scratch) స్థిరరాశి
  {
    int64_t budget = HB_BUDGET_GLYPH;
    hb_varc_context_t c {font,
			 శూన్యసూచిక,
			 extents,
			 hb_decycler_t {},
			 HB_MAX_లేఖాచిత్రం_అంచు_లెక్క,
			 HB_MAX_NESTING_LEVEL,
			 budget,
			 scratch};

    తిరిగి get_మార్గం_at (c, gid,
			hb_array (font->coords, font->అంకె_coords));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  సంస్కరణ.major == 1 &&
		  coverage.sanitize (c, ఇది) &&
		  varStore.sanitize (c, ఇది) &&
		  conditionList.sanitize (c, ఇది) &&
		  axisIndicesList.sanitize (c, ఇది) &&
		  glyph_రికార్డులు.sanitize (c, ఇది));
  }

  నిర్మాణం accelerator_t
  {
    మిత్ర నిర్మాణం VarComponent;

    accelerator_t (hb_face_t *face)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<VARC> (face);
    }
    ~accelerator_t ()
    {
      స్వయంచాలక *scratch = cached_scratch.get_relaxed ();
      ఒకవేళ (scratch)
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }

      పట్టిక.destroy ();
    }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక->has_దత్తాంశం (); }

    తార్కిక
    get_మార్గం (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session) స్థిరరాశి
    {
      ఒకవేళ (!పట్టిక->has_దత్తాంశం ()) తిరిగి అబద్ధం;

      స్వయంచాలక *scratch = acquire_scratch ();
      ఒకవేళ (unlikely (!scratch)) తిరిగి నిజం;
      తార్కిక ret = పట్టిక->get_మార్గం (font, gid, draw_session, *scratch);
      release_scratch (scratch);
      తిరిగి ret;
    }

    తార్కిక సంవృతం_glyphs (hb_సమితి_t *glyphset) స్థిరరాశి
    {
      ఒకవేళ (పట్టిక->has_దత్తాంశం ())
	తిరిగి పట్టిక->సంవృతం_glyphs (glyphset);
      తిరిగి నిజం;
    }

    శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
    {
      ఒకవేళ (పట్టిక->has_దత్తాంశం ())
	పట్టిక->depend (depend_దత్తాంశం);
    }

    తార్కిక
    get_extents (hb_font_t *font,
		 hb_codepoint_t gid,
		 hb_glyph_extents_t *extents) స్థిరరాశి
    {
#నిర్వచితం_కాకపోతే HB_NO_DRAW
      ఒకవేళ (!పట్టిక->has_దత్తాంశం ()) తిరిగి అబద్ధం;

      hb_extents_t<> f_extents;

      స్వయంచాలక *scratch = acquire_scratch ();
      ఒకవేళ (unlikely (!scratch)) తిరిగి నిజం;
      తార్కిక ret = పట్టిక->get_extents (font, gid, &f_extents, *scratch);
      release_scratch (scratch);

      ఒకవేళ (ret)
	*extents = f_extents.గమ్యం_glyph_extents (font->x_scale < 0, font->y_scale < 0);

      తిరిగి ret;
#లేకపోతే
      తిరిగి అబద్ధం;
#అయితే_ముగింపు
    }

    అంతరంగ:

    hb_varc_scratch_t *acquire_scratch () స్థిరరాశి
    {
      hb_varc_scratch_t *scratch = cached_scratch.get_acquire ();

      ఒకవేళ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, శూన్యసూచిక)))
      {
	scratch = (hb_varc_scratch_t *) hb_calloc (1, పరిమాణం (hb_varc_scratch_t));
	ఒకవేళ (unlikely (!scratch))
	  తిరిగి శూన్యసూచిక;
      }

      తిరిగి scratch;
    }
    శూన్య release_scratch (hb_varc_scratch_t *scratch) స్థిరరాశి
    {
      ఒకవేళ (!cached_scratch.cmpexch (శూన్యసూచిక, scratch))
      {
	scratch->~hb_varc_scratch_t ();
	hb_free (scratch);
      }
    }

    అంతరంగ:
    hb_blob_ptr_t<VARC> పట్టిక;
    మార్చదగిన hb_atomic_t<hb_varc_scratch_t *> cached_scratch;
  };

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.major != 0; }

  రక్షిత:
  Fixed_సంస్కరణ<> సంస్కరణ; /* Version identifier */
  Offset32_గమ్యం<Coverage> coverage;
  Offset32_గమ్యం<Multi_అంశం_Variation_Store> varStore;
  Offset32_గమ్యం<ConditionList> conditionList;
  Offset32_గమ్యం<TupleList> axisIndicesList;
  Offset32_గమ్యం<CFF2_సూచీ/*Of<VarCompositeGlyph>*/> glyph_రికార్డులు;
  బహిరంగ:
  DEFINE_కొలత_STATIC (24);
};

నిర్మాణం VARC_accelerator_t : VARC::accelerator_t {
  VARC_accelerator_t (hb_face_t *face) : VARC::accelerator_t (face) {}
};

#అయితే_ముగింపు

//}

}

#అయితే_ముగింపు  /* OT_VAR_VARC_VARC_HH */
