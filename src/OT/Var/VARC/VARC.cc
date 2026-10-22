#నిర్వచితం_కాకపోతే HB_OT_VAR_VARC_CC
#నిర్వచించు HB_OT_VAR_VARC_CC
#నిర్వచితమైతే HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#అన్తర్భావ "VARC.hh"

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES

#అన్తర్భావ "../../../hb-draw.hh"
#అన్తర్భావ "../../../hb-depend-data.hh"
#అన్తర్భావ "../../../hb-ot-layout-common.hh"
#అన్తర్భావ "../../../hb-ot-layout-gdef-table.hh"

నామాకాశ OT {

//namespace Var {

#నిర్వచించు VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_ప్రారంభం { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_చివర

తార్కిక
VarComponent::decompile_రికార్డు (స్థిరరాశి VARC &varc,
				hb_ubytes_t మొత్తం_రికార్డు,
				hb_సదిశ_t<గుర్తులేని> *axis_indices,
				hb_సదిశ_t<ద్విగుణ> *axis_విలువలు,
				రికార్డు_t *decoded,
				int64_t *budget)
{
  స్థిరరాశి గుర్తులేని అక్షరం *ప్రారంభం = మొత్తం_రికార్డు.arrayZ;
  స్థిరరాశి గుర్తులేని అక్షరం *రికార్డు = ప్రారంభం;
  స్థిరరాశి గుర్తులేని అక్షరం *చివర = ప్రారంభం + మొత్తం_రికార్డు.నిడివి;

  ఒకవేళ (axis_indices) axis_indices->clear ();
  ఒకవేళ (axis_విలువలు) axis_విలువలు->clear ();

#నిర్వచించు READ_UINT32VAR(name) \
  HB_STMT_ప్రారంభం { \
    ఒకవేళ (unlikely (గుర్తులేని (చివర - రికార్డు) < HBUINT32VAR::min_కొలత)) తిరిగి అబద్ధం; \
    hb_barrier (); \
    స్వయంచాలక &varint = * (స్థిరరాశి HBUINT32VAR *) రికార్డు; \
    గుర్తులేని కొలత = varint.get_కొలత (); \
    ఒకవేళ (unlikely (గుర్తులేని (చివర - రికార్డు) < కొలత)) తిరిగి అబద్ధం; \
    name = (uint32_t) varint; \
    రికార్డు += కొలత; \
  } HB_STMT_చివర
#నిర్వచించు READ_UINT32_VAR_క్షేత్రం(name, క్షేత్రం) \
  HB_STMT_ప్రారంభం { \
    decoded->క్షేత్రం##_offset = రికార్డు - ప్రారంభం; \
    READ_UINT32VAR (name); \
    decoded->క్షేత్రం##_size = రికార్డు - ప్రారంభం - decoded->క్షేత్రం##_offset; \
  } HB_STMT_చివర

  READ_UINT32VAR (decoded->flags);

  decoded->gid_స్థానభ్రంశం = రికార్డు - ప్రారంభం;
  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::GID_IS_24_బిట్టు)
  {
    decoded->gid_కొలత = HBGlyphID24::static_కొలత;
    ఒకవేళ (unlikely (గుర్తులేని (చివర - రికార్డు) < HBGlyphID24::static_కొలత)) తిరిగి అబద్ధం;
    hb_barrier ();
    decoded->gid = * (స్థిరరాశి HBGlyphID24 *) రికార్డు;
    రికార్డు += HBGlyphID24::static_కొలత;
  }
  లేకపోతే
  {
    decoded->gid_కొలత = HBGlyphID16::static_కొలత;
    ఒకవేళ (unlikely (గుర్తులేని (చివర - రికార్డు) < HBGlyphID16::static_కొలత)) తిరిగి అబద్ధం;
    hb_barrier ();
    decoded->gid = * (స్థిరరాశి HBGlyphID16 *) రికార్డు;
    రికార్డు += HBGlyphID16::static_కొలత;
  }

  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_క్షేత్రం (decoded->condition_సూచీ, condition);

  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_క్షేత్రం (decoded->axis_indices_సూచీ, axis_indices);
    గుర్తులేని axis_లెక్క;
    ఒకవేళ (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_సూచీ]);
      ఒకవేళ (unlikely (axis_indices->లోపల_error ())) తిరిగి అబద్ధం;
      axis_లెక్క = axis_indices->నిడివి;
    }
    లేకపోతే
      axis_లెక్క = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_సూచీ]);
    ఒకవేళ (unlikely (budget && axis_లెక్క &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_లెక్క)))
      తిరిగి అబద్ధం;
    ఒకవేళ (axis_విలువలు)
    {
      ఒకవేళ (unlikely (!axis_విలువలు->resize (axis_లెక్క))) తిరిగి అబద్ధం;
      స్థిరరాశి HBUINT8 *p = (స్థిరరాశి HBUINT8 *) రికార్డు;
      ఒకవేళ (unlikely (!Tuple_విలువలు::decompile (p, *axis_విలువలు,
					    (స్థిరరాశి HBUINT8 *) చివర)))
	తిరిగి అబద్ధం;
      రికార్డు = (స్థిరరాశి గుర్తులేని అక్షరం *) p;
    }
    లేకపోతే ఒకవేళ (unlikely (!Tuple_విలువలు::skip (రికార్డు, చివర, axis_లెక్క)))
      తిరిగి అబద్ధం;
  }

  decoded->axis_విలువలు_var_idx = VarIdx::NO_VARIATION;
  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::AXIS_విలువలు_HAVE_VARIATION)
    READ_UINT32_VAR_క్షేత్రం (decoded->axis_విలువలు_var_idx, axis_విలువలు_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_క్షేత్రం (decoded->transform_var_idx, transform_var);

#నిర్వచించు PROCESS_TRANSFORM_COMPONENT(జరుపు, type, flag, name) \
  ఒకవేళ (decoded->flags & (గుర్తులేని) flags_t::flag) \
  { \
    static_assert (type::static_కొలత == HBINT16::static_కొలత, ""); \
    ఒకవేళ (unlikely (గుర్తులేని (చివర - రికార్డు) < HBINT16::static_కొలత)) తిరిగి అబద్ధం; \
    hb_barrier (); \
    decoded->transform.name = * (స్థిరరాశి HBINT16 *) రికార్డు; \
    రికార్డు += HBINT16::static_కొలత; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#నిర్వచనం_రద్దు PROCESS_TRANSFORM_COMPONENT

  గుర్తులేని reserved = decoded->flags & (గుర్తులేని) flags_t::RESERVED_ముసుగు;
  అయితే (reserved)
  {
    uint32_t discard HB_వాడని;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->కొలత = రికార్డు - ప్రారంభం;
#నిర్వచనం_రద్దు READ_UINT32_VAR_క్షేత్రం
#నిర్వచనం_రద్దు READ_UINT32VAR
  తిరిగి నిజం;
}

తార్కిక
VARC::సంవృతం_glyphs (hb_సమితి_t *glyphset) స్థిరరాశి
{
  ఒకవేళ (unlikely (glyphset->లోపల_error ()))
    తిరిగి అబద్ధం;

  hb_సమితి_t pending = *glyphset;
  ఒకవేళ (unlikely (pending.లోపల_error ()))
    తిరిగి అబద్ధం;

  అయితే (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    గుర్తులేని సూచీ_2 = (ఇది+coverage).get_coverage (gid);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) కొనసాగు;

    hb_ubytes_t రికార్డు = (ఇది+glyph_రికార్డులు)[సూచీ_2];
    అయితే (రికార్డు)
    {
      VarComponent::రికార్డు_t component;
      ఒకవేళ (unlikely (!VarComponent::decompile_రికార్డు (*ఇది, రికార్డు,
						     శూన్యసూచిక, శూన్యసూచిక, &component)))
	తిరిగి అబద్ధం;
      ఒకవేళ (!glyphset->has (component.gid))
      {
	glyphset->కూడు (component.gid);
	ఒకవేళ (unlikely (glyphset->లోపల_error ()))
	  తిరిగి అబద్ధం;
	pending.కూడు (component.gid);
	ఒకవేళ (unlikely (pending.లోపల_error ()))
	  తిరిగి అబద్ధం;
      }
      రికార్డు = రికార్డు.sub_array (component.కొలత);
    }
  }
  తిరిగి నిజం;
}

శూన్య
VARC::depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
{
  స్థిరరాశి CFF2_సూచీ &రికార్డులు = ఇది+glyph_రికార్డులు;
  కొరకు (స్వయంచాలక _ : + hb_zip (ఇది+coverage,
			  hb_range ((గుర్తులేని) రికార్డులు.లెక్క)))
  {
    hb_codepoint_t gid = _.మొదటి;
    hb_ubytes_t రికార్డు = రికార్డులు[_.second];
    అయితే (రికార్డు)
    {
	VarComponent::రికార్డు_t component;
	ఒకవేళ (unlikely (!VarComponent::decompile_రికార్డు (*ఇది, రికార్డు,
						       శూన్యసూచిక, శూన్యసూచిక, &component)))
	{
	  depend_దత్తాంశం->fail ();
	  తిరిగి;
	}
      depend_దత్తాంశం->కూడు_depend (gid, పట్టిక_ట్యాగ్, component.gid);
      రికార్డు = రికార్డు.sub_array (component.కొలత);
    }
  }
}


#నిర్వచితం_కాకపోతే HB_NO_DRAW

నిర్మాణం hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  శూన్య *దత్తాంశం;
  hb_draw_state_t *st;
  int64_t *budget_మిగిలిన;
};

స్థిర శూన్య
hb_transforming_pen_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			     శూన్య *దత్తాంశం,
			     hb_draw_state_t *st,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) దత్తాంశం;

  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);

  c->dfuncs->move_గమ్యం (c->దత్తాంశం, *c->st, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_transforming_pen_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			     శూన్య *దత్తాంశం,
			     hb_draw_state_t *st,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) దత్తాంశం;

  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);

  c->dfuncs->పంక్తి_గమ్యం (c->దత్తాంశం, *c->st, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_transforming_pen_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
				  శూన్య *దత్తాంశం,
				  hb_draw_state_t *st,
				  దశాంశం control_x, దశాంశం control_y,
				  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) దత్తాంశం;

  c->transform.transform_బిందువు (control_x, control_y);
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);

  c->dfuncs->quadratic_గమ్యం (c->దత్తాంశం, *c->st, control_x, control_y, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_transforming_pen_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			      శూన్య *దత్తాంశం,
			      hb_draw_state_t *st,
			      దశాంశం control1_x, దశాంశం control1_y,
			      దశాంశం control2_x, దశాంశం control2_y,
			      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) దత్తాంశం;

  c->transform.transform_బిందువు (control1_x, control1_y);
  c->transform.transform_బిందువు (control2_x, control2_y);
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);

  c->dfuncs->cubic_గమ్యం (c->దత్తాంశం, *c->st, control1_x, control1_y, control2_x, control2_y, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_transforming_pen_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
				శూన్య *దత్తాంశం,
				hb_draw_state_t *st,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) దత్తాంశం;

  c->dfuncs->close_మార్గం (c->దత్తాంశం, *c->st);
}

స్థిర int64_t *
hb_transforming_pen_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_transforming_pen_context_t *) draw_దత్తాంశం)->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_transforming_pen_funcs ();

స్థిర నిర్మాణం hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, hb_transforming_pen_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, hb_transforming_pen_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_transforming_pen_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, hb_transforming_pen_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, hb_transforming_pen_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_transforming_pen_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    తిరిగి funcs;
  }
} static_transforming_pen_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

స్థిర hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  తిరిగి static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_మార్గం_at (స్థిరరాశి hb_varc_context_t &c,
			   hb_codepoint_t మాతృ_gid,
			   hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
			   hb_transform_t<> మొత్తం_transform,
			   hb_ubytes_t మొత్తం_రికార్డు,
			   hb_scalar_cache_t *cache) స్థిరరాశి
{
  ఒకవేళ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    తిరిగి hb_ubytes_t ();

  స్థిరరాశి గుర్తులేని అక్షరం *చివర = మొత్తం_రికార్డు.arrayZ + మొత్తం_రికార్డు.నిడివి;
  స్వయంచాలక &VARC = *c.font->face->పట్టిక.VARC->పట్టిక;
  స్వయంచాలక &varStore = &VARC+VARC.varStore;

  స్వయంచాలక &axisIndices = c.scratch.axisIndices;
  స్వయంచాలక &axis_విలువలు_2 = c.scratch.axis_విలువలు_2;
  రికార్డు_t component;
  ఒకవేళ (unlikely (!decompile_రికార్డు (VARC, మొత్తం_రికార్డు,
				   &axisIndices, &axis_విలువలు_2, &component,
				   &c.budget)))
    తిరిగి hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  స్థిరరాశి గుర్తులేని అక్షరం *రికార్డు = మొత్తం_రికార్డు.arrayZ + component.కొలత;

  // Condition
  తార్కిక show = నిజం;
  ఒకవేళ (flags & (గుర్తులేని) flags_t::HAVE_CONDITION)
  {
    స్థిరరాశి స్వయంచాలక &condition = (&VARC+VARC.conditionList)[component.condition_సూచీ];
    స్వయంచాలక instancer = Multi_అంశం_Var_Store_Instancer(&varStore, శూన్యసూచిక, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.నిడివి, &instancer);
  }

  // Axis values

  // Apply variations if any
  ఒకవేళ ((flags & (గుర్తులేని) flags_t::AXIS_విలువలు_HAVE_VARIATION) &&
      show && !axis_విలువలు_2.లోపల_error ())
  {
    ఒకవేళ (unlikely (!hb_budget_spend (c.budget, axis_విలువలు_2.నిడివి,
				    coords.నిడివి)))
      తిరిగి hb_ubytes_t ();
    varStore.get_delta (component.axis_విలువలు_var_idx, coords,
			axis_విలువలు_2.as_array (), cache);
  }

  స్వయంచాలక component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  ఒకవేళ ((flags & (గుర్తులేని) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.నిడివి > HB_VAR_సంయుక్త_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->అంకె_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  ఒకవేళ (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    ఒకవేళ (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.నిడివి)))
      తిరిగి hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<పూర్ణాంకం> ());
    కొరకు (గుర్తులేని i = 0; i < axisIndices.నిడివి; i++)
      coord_setter[axisIndices[i]] = hb_clamp_గమ్యం<పూర్ణాంకం> (round (axis_విలువలు_2[i]));
    ఒకవేళ (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    ఒకవేళ (transformVarIdx != VarIdx::NO_VARIATION)
    {
      ద్విగుణ transform_విలువలు[9];
      గుర్తులేని అంకె_Transform_విలువలు = 0;
#నిర్వచించు PROCESS_TRANSFORM_COMPONENT(జరుపు, type, flag, name) \
	  ఒకవేళ (flags & (గుర్తులేని) flags_t::flag) \
	    transform_విలువలు[అంకె_Transform_విలువలు++] = (ద్విగుణ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#నిర్వచనం_రద్దు PROCESS_TRANSFORM_COMPONENT
      ఒకవేళ (unlikely (!hb_budget_spend (c.budget, అంకె_Transform_విలువలు,
				      coords.నిడివి)))
	తిరిగి hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_విలువలు, అంకె_Transform_విలువలు), cache);
      అంకె_Transform_విలువలు = 0;
#నిర్వచించు PROCESS_TRANSFORM_COMPONENT(జరుపు, type, flag, name) \
	  ఒకవేళ (flags & (గుర్తులేని) flags_t::flag) \
	    transform.name = (దశాంశం) transform_విలువలు[అంకె_Transform_విలువలు++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#నిర్వచనం_రద్దు PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#నిర్వచించు PROCESS_TRANSFORM_COMPONENT(జరుపు, type, flag, name) \
	  ఒకవేళ (జరుపు && (flags & (గుర్తులేని) flags_t::flag)) \
	     transform.name *= 1.f / (1 << జరుపు);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#నిర్వచనం_రద్దు PROCESS_TRANSFORM_COMPONENT

    ఒకవేళ (!(flags & (గుర్తులేని) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    మొత్తం_transform.transform (transform.గమ్యం_transform ());

    తార్కిక same_coords = component_coords.నిడివి == coords.నిడివి &&
		       component_coords.arrayZ == coords.arrayZ;

    c.లోతు_ఎడమ--;
    VARC.get_మార్గం_at (c, gid,
		      component_coords, మొత్తం_transform,
		      మాతృ_gid,
		      same_coords ? cache : శూన్యసూచిక);
    c.లోతు_ఎడమ++;
  }

  తిరిగి hb_ubytes_t (రికార్డు, చివర - రికార్డు);
}

తార్కిక
VARC::get_మార్గం_at (స్థిరరాశి hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t మాతృ_glyph,
		   hb_scalar_cache_t *మాతృ_cache) స్థిరరాశి
{
  // Don't recurse on the same glyph.
  గుర్తులేని idx = glyph == మాతృ_glyph ?
		 NOT_COVERED :
		 (ఇది+coverage).get_coverage (glyph);
  ఒకవేళ (idx == NOT_COVERED)
  {
    ఒకవేళ (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      ఒకవేళ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) తిరిగి నిజం;

      hb_transform_t<> ఆకు_transform = transform;
      ఆకు_transform.x0 *= c.font->x_multf;
      ఆకు_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {ఆకు_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_దత్తాంశం,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = ఆకు_transform.is_తటస్థం () ? *c.draw_session : transformer_session;

      ఒకవేళ (c.font->face->పట్టిక.glyf->get_మార్గం_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, శూన్యసూచిక, &shape_draw_session.get_budget ())) తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_CFF
      ఒకవేళ (c.font->face->పట్టిక.cff2->get_మార్గం_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) తిరిగి నిజం;
      ఒకవేళ (c.font->face->పట్టిక.cff1->get_మార్గం (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) తిరిగి నిజం; // Doesn't have variations
#అయితే_ముగింపు
      తిరిగి అబద్ధం;
    }
    లేకపోతే ఒకవేళ (c.extents)
    {
      ఒకవేళ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) తిరిగి నిజం;

      hb_glyph_extents_t glyph_extents;
      ఒకవేళ (!c.font->face->పట్టిక.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#నిర్వచితం_కాకపోతే HB_NO_CFF
      ఒకవేళ (!c.font->face->పట్టిక.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      ఒకవేళ (!c.font->face->పట్టిక.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#అయితే_ముగింపు
	తిరిగి అబద్ధం;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> ఆకు_transform = transform;
      ఆకు_transform.x0 *= c.font->x_multf;
      ఆకు_transform.y0 *= c.font->y_multf;
      ఆకు_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    తిరిగి నిజం;
  }

  ఒకవేళ (c.లోతు_ఎడమ <= 0)
    తిరిగి నిజం;

  ఒకవేళ (c.అంచులు_ఎడమ <= 0)
    తిరిగి నిజం;
  (c.అంచులు_ఎడమ)--;

  hb_decycler_కణుపు_t node (c.decycler);
  ఒకవేళ (unlikely (!node.సందర్శించు (glyph)))
    తిరిగి నిజం;

  hb_ubytes_t రికార్డు = (ఇది+glyph_రికార్డులు)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = మాతృ_cache ?
				  మాతృ_cache :
				  (ఇది+varStore).create_cache (&static_cache);

  Var_సంయుక్త_Glyph::get_మార్గం_at (c,
				  glyph,
				  coords, transform,
				  రికార్డు,
				  cache);

  ఒకవేళ (cache != మాతృ_cache)
    (ఇది+varStore).destroy_cache (cache, &static_cache);

  తిరిగి నిజం;
}

#అయితే_ముగింపు

#నిర్వచనం_రద్దు VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_OT_VAR_VARC_CC (pacify clang) */
#అయితే_ముగింపు /* HB_OT_VAR_VARC_CC */
