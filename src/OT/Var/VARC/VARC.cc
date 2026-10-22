#अगर_अघोषित HB_OT_VAR_VARC_CC
#घोषणा HB_OT_VAR_VARC_CC
#अगर_घोषित HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#समावेश "VARC.hh"

#अगर_अघोषित HB_NO_VAR_COMPOSITES

#समावेश "../../../hb-draw.hh"
#समावेश "../../../hb-depend-data.hh"
#समावेश "../../../hb-ot-layout-common.hh"
#समावेश "../../../hb-ot-layout-gdef-table.hh"

namespace OT {

//namespace Var {

#घोषणा VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_START { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_END

bool
VarComponent::decompile_record (स्थिर VARC &varc,
				hb_ubytes_t योग_record,
				hb_vector_t<अचिन्हित> *axis_indices,
				hb_vector_t<द्विगुन> *axis_मूल्यावली,
				record_t *decoded,
				int64_t *budget)
{
  स्थिर अचिन्हित अक्षर *start = योग_record.arrayZ;
  स्थिर अचिन्हित अक्षर *record = start;
  स्थिर अचिन्हित अक्षर *end = start + योग_record.लंबाई;

  अगर (axis_indices) axis_indices->सफा ();
  अगर (axis_मूल्यावली) axis_मूल्यावली->सफा ();

#घोषणा READ_UINT32VAR(name) \
  HB_STMT_START { \
    अगर (unlikely (अचिन्हित (end - record) < HBUINT32VAR::min_आकार)) वापस false; \
    hb_barrier (); \
    स्वतः &varint = * (स्थिर HBUINT32VAR *) record; \
    अचिन्हित आकार = varint.get_आकार (); \
    अगर (unlikely (अचिन्हित (end - record) < आकार)) वापस false; \
    name = (uint32_t) varint; \
    record += आकार; \
  } HB_STMT_END
#घोषणा READ_UINT32VAR_FIELD(name, field) \
  HB_STMT_START { \
    decoded->field##_offset = record - start; \
    READ_UINT32VAR (name); \
    decoded->field##_size = record - start - decoded->field##_offset; \
  } HB_STMT_END

  READ_UINT32VAR (decoded->flags);

  decoded->gid_offset = record - start;
  अगर (decoded->flags & (अचिन्हित) flags_t::GID_IS_24BIT)
  {
    decoded->gid_आकार = HBGlyphID24::static_आकार;
    अगर (unlikely (अचिन्हित (end - record) < HBGlyphID24::static_आकार)) वापस false;
    hb_barrier ();
    decoded->gid = * (स्थिर HBGlyphID24 *) record;
    record += HBGlyphID24::static_आकार;
  }
  अन्यथा
  {
    decoded->gid_आकार = HBGlyphID16::static_आकार;
    अगर (unlikely (अचिन्हित (end - record) < HBGlyphID16::static_आकार)) वापस false;
    hb_barrier ();
    decoded->gid = * (स्थिर HBGlyphID16 *) record;
    record += HBGlyphID16::static_आकार;
  }

  अगर (decoded->flags & (अचिन्हित) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_सूचकांक, condition);

  अगर (decoded->flags & (अचिन्हित) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_सूचकांक, axis_indices);
    अचिन्हित axis_गिनती;
    अगर (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_सूचकांक]);
      अगर (unlikely (axis_indices->in_error ())) वापस false;
      axis_गिनती = axis_indices->लंबाई;
    }
    अन्यथा
      axis_गिनती = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_सूचकांक]);
    अगर (unlikely (budget && axis_गिनती &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_गिनती)))
      वापस false;
    अगर (axis_मूल्यावली)
    {
      अगर (unlikely (!axis_मूल्यावली->resize (axis_गिनती))) वापस false;
      स्थिर HBUINT8 *p = (स्थिर HBUINT8 *) record;
      अगर (unlikely (!Tuple_मूल्यावली::decompile (p, *axis_मूल्यावली,
					    (स्थिर HBUINT8 *) end)))
	वापस false;
      record = (स्थिर अचिन्हित अक्षर *) p;
    }
    अन्यथा अगर (unlikely (!Tuple_मूल्यावली::skip (record, end, axis_गिनती)))
      वापस false;
  }

  decoded->axis_मूल्यावली_var_idx = VarIdx::NO_VARIATION;
  अगर (decoded->flags & (अचिन्हित) flags_t::AXIS_मूल्यावली_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_मूल्यावली_var_idx, axis_मूल्यावली_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  अगर (decoded->flags & (अचिन्हित) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#घोषणा PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  अगर (decoded->flags & (अचिन्हित) flags_t::flag) \
  { \
    static_assert (type::static_आकार == HBINT16::static_आकार, ""); \
    अगर (unlikely (अचिन्हित (end - record) < HBINT16::static_आकार)) वापस false; \
    hb_barrier (); \
    decoded->transform.name = * (स्थिर HBINT16 *) record; \
    record += HBINT16::static_आकार; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#अघोषित PROCESS_TRANSFORM_COMPONENT

  अचिन्हित reserved = decoded->flags & (अचिन्हित) flags_t::RESERVED_MASK;
  जबतक (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->आकार = record - start;
#अघोषित READ_UINT32VAR_FIELD
#अघोषित READ_UINT32VAR
  वापस true;
}

bool
VARC::closure_glyphs (hb_set_t *glyphset) स्थिर
{
  अगर (unlikely (glyphset->in_error ()))
    वापस false;

  hb_set_t pending = *glyphset;
  अगर (unlikely (pending.in_error ()))
    वापस false;

  जबतक (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    अचिन्हित सूचकांक_2 = (यह+coverage).get_coverage (gid);
    अगर (सूचकांक_2 == NOT_COVERED) जारी;

    hb_ubytes_t record = (यह+glyphRecords)[सूचकांक_2];
    जबतक (record)
    {
      VarComponent::record_t component;
      अगर (unlikely (!VarComponent::decompile_record (*यह, record,
						     nullptr, nullptr, &component)))
	वापस false;
      अगर (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	अगर (unlikely (glyphset->in_error ()))
	  वापस false;
	pending.add (component.gid);
	अगर (unlikely (pending.in_error ()))
	  वापस false;
      }
      record = record.sub_array (component.आकार);
    }
  }
  वापस true;
}

व्योम
VARC::depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
{
  स्थिर CFF2_सूचकांक &records = यह+glyphRecords;
  क्रम (स्वतः _ : + hb_zip (यह+coverage,
			  hb_range ((अचिन्हित) records.गिनती)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    जबतक (record)
    {
	VarComponent::record_t component;
	अगर (unlikely (!VarComponent::decompile_record (*यह, record,
						       nullptr, nullptr, &component)))
	{
	  depend_तथ्य->हार ();
	  वापस;
	}
      depend_तथ्य->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.आकार);
    }
  }
}


#अगर_अघोषित HB_NO_DRAW

काष्ठा hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  व्योम *तथ्य;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

अटल व्योम
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     व्योम *तथ्य,
			     hb_draw_state_t *st,
			     भग्न to_x, भग्न to_y,
			     व्योम *user_तथ्य HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) तथ्य;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->तथ्य, *c->st, to_x, to_y);
}

अटल व्योम
hb_transforming_pen_पंक्ति_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     व्योम *तथ्य,
			     hb_draw_state_t *st,
			     भग्न to_x, भग्न to_y,
			     व्योम *user_तथ्य HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) तथ्य;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->पंक्ति_to (c->तथ्य, *c->st, to_x, to_y);
}

अटल व्योम
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  व्योम *तथ्य,
				  hb_draw_state_t *st,
				  भग्न control_x, भग्न control_y,
				  भग्न to_x, भग्न to_y,
				  व्योम *user_तथ्य HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) तथ्य;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->तथ्य, *c->st, control_x, control_y, to_x, to_y);
}

अटल व्योम
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      व्योम *तथ्य,
			      hb_draw_state_t *st,
			      भग्न control1_x, भग्न control1_y,
			      भग्न control2_x, भग्न control2_y,
			      भग्न to_x, भग्न to_y,
			      व्योम *user_तथ्य HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) तथ्य;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->तथ्य, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

अटल व्योम
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				व्योम *तथ्य,
				hb_draw_state_t *st,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) तथ्य;

  c->dfuncs->close_path (c->तथ्य, *c->st);
}

अटल int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, व्योम *draw_तथ्य, व्योम *)
{
  वापस ((hb_transforming_pen_context_t *) draw_तथ्य)->budget_remaining;
}

अटल अंतरभूत व्योम free_static_transforming_pen_funcs ();

अटल काष्ठा hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  अटल hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पंक्ति_to_func (funcs, hb_transforming_pen_पंक्ति_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    वापस funcs;
  }
} static_transforming_pen_funcs;

अटल अंतरभूत
व्योम free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

अटल hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  वापस static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (स्थिर hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<स्थिर पूर्णांक> coords,
			   hb_transform_t<> योग_transform,
			   hb_ubytes_t योग_record,
			   hb_scalar_cache_t *cache) स्थिर
{
  अगर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    वापस hb_ubytes_t ();

  स्थिर अचिन्हित अक्षर *end = योग_record.arrayZ + योग_record.लंबाई;
  स्वतः &VARC = *c.font->face->table.VARC->table;
  स्वतः &varStore = &VARC+VARC.varStore;

  स्वतः &axisIndices = c.scratch.axisIndices;
  स्वतः &axis_मूल्यावली_2 = c.scratch.axis_मूल्यावली_2;
  record_t component;
  अगर (unlikely (!decompile_record (VARC, योग_record,
				   &axisIndices, &axis_मूल्यावली_2, &component,
				   &c.budget)))
    वापस hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  स्थिर अचिन्हित अक्षर *record = योग_record.arrayZ + component.आकार;

  // Condition
  bool show = true;
  अगर (flags & (अचिन्हित) flags_t::HAVE_CONDITION)
  {
    स्थिर स्वतः &condition = (&VARC+VARC.conditionList)[component.condition_सूचकांक];
    स्वतः instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.लंबाई, &instancer);
  }

  // Axis values

  // Apply variations if any
  अगर ((flags & (अचिन्हित) flags_t::AXIS_मूल्यावली_HAVE_VARIATION) &&
      show && !axis_मूल्यावली_2.in_error ())
  {
    अगर (unlikely (!hb_budget_spend (c.budget, axis_मूल्यावली_2.लंबाई,
				    coords.लंबाई)))
      वापस hb_ubytes_t ();
    varStore.get_delta (component.axis_मूल्यावली_var_idx, coords,
			axis_मूल्यावली_2.as_array (), cache);
  }

  स्वतः component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  अगर ((flags & (अचिन्हित) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.लंबाई > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->अंक_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  अगर (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    अगर (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.लंबाई)))
      वापस hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<पूर्णांक> ());
    क्रम (अचिन्हित i = 0; i < axisIndices.लंबाई; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<पूर्णांक> (round (axis_मूल्यावली_2[i]));
    अगर (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    अगर (transformVarIdx != VarIdx::NO_VARIATION)
    {
      द्विगुन transform_मूल्यावली[9];
      अचिन्हित अंक_Transform_मूल्यावली = 0;
#घोषणा PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  अगर (flags & (अचिन्हित) flags_t::flag) \
	    transform_मूल्यावली[अंक_Transform_मूल्यावली++] = (द्विगुन) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#अघोषित PROCESS_TRANSFORM_COMPONENT
      अगर (unlikely (!hb_budget_spend (c.budget, अंक_Transform_मूल्यावली,
				      coords.लंबाई)))
	वापस hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_मूल्यावली, अंक_Transform_मूल्यावली), cache);
      अंक_Transform_मूल्यावली = 0;
#घोषणा PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  अगर (flags & (अचिन्हित) flags_t::flag) \
	    transform.name = (भग्न) transform_मूल्यावली[अंक_Transform_मूल्यावली++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#अघोषित PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#घोषणा PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  अगर (shift && (flags & (अचिन्हित) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#अघोषित PROCESS_TRANSFORM_COMPONENT

    अगर (!(flags & (अचिन्हित) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    योग_transform.transform (transform.to_transform ());

    bool same_coords = component_coords.लंबाई == coords.लंबाई &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_बायाँ--;
    VARC.get_path_at (c, gid,
		      component_coords, योग_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_बायाँ++;
  }

  वापस hb_ubytes_t (record, end - record);
}

bool
VARC::get_path_at (स्थिर hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<स्थिर पूर्णांक> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) स्थिर
{
  // Don't recurse on the same glyph.
  अचिन्हित idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (यह+coverage).get_coverage (glyph);
  अगर (idx == NOT_COVERED)
  {
    अगर (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      अगर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) वापस true;

      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {leaf_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_तथ्य,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = leaf_transform.is_identity () ? *c.draw_session : transformer_session;

      अगर (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) वापस true;
#अगर_अघोषित HB_NO_CFF
      अगर (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) वापस true;
      अगर (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) वापस true; // Doesn't have variations
#पूर्ण_अगर
      वापस false;
    }
    अन्यथा अगर (c.extents)
    {
      अगर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) वापस true;

      hb_glyph_extents_t glyph_extents;
      अगर (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#अगर_अघोषित HB_NO_CFF
      अगर (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      अगर (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#पूर्ण_अगर
	वापस false;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    वापस true;
  }

  अगर (c.depth_बायाँ <= 0)
    वापस true;

  अगर (c.edges_बायाँ <= 0)
    वापस true;
  (c.edges_बायाँ)--;

  hb_decycler_गाँठ_t node (c.decycler);
  अगर (unlikely (!node.visit (glyph)))
    वापस true;

  hb_ubytes_t record = (यह+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (यह+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  अगर (cache != parent_cache)
    (यह+varStore).destroy_cache (cache, &static_cache);

  वापस true;
}

#पूर्ण_अगर

#अघोषित VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#पूर्ण_अगर

#पूर्ण_अगर /* HB_OT_VAR_VARC_CC (pacify clang) */
#पूर्ण_अगर /* HB_OT_VAR_VARC_CC */
