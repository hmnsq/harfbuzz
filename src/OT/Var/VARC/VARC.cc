#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#അന്തര്ഭാവ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#അന്തര്ഭാവ "../../../hb-draw.hh"
#അന്തര്ഭാവ "../../../hb-depend-data.hh"
#അന്തര്ഭാവ "../../../hb-ot-layout-common.hh"
#അന്തര്ഭാവ "../../../hb-ot-layout-gdef-table.hh"

നാമാകാശ OT {

//namespace Var {

#define VARC_PROCESS_TRANSFORM_COMPONENTS \
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

തർക്കം
VarComponent::decompile_record (സ്ഥിരം VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<ഇരട്ട> *axis_മൂല്യങ്ങൾ,
				record_t *decoded,
				int64_t *budget)
{
  സ്ഥിരം unsigned അക്ഷരം *start = total_record.arrayZ;
  സ്ഥിരം unsigned അക്ഷരം *record = start;
  സ്ഥിരം unsigned അക്ഷരം *end = start + total_record.length;

  എങ്കിൽ (axis_indices) axis_indices->clear ();
  എങ്കിൽ (axis_മൂല്യങ്ങൾ) axis_മൂല്യങ്ങൾ->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    എങ്കിൽ (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) തിരികെ അസത്യം; \
    hb_barrier (); \
    auto &varint = * (സ്ഥിരം HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    എങ്കിൽ (unlikely (unsigned (end - record) < size)) തിരികെ അസത്യം; \
    name = (uint32_t) varint; \
    record += size; \
  } HB_STMT_END
#define READ_UINT32VAR_FIELD(name, field) \
  HB_STMT_START { \
    decoded->field##_offset = record - start; \
    READ_UINT32VAR (name); \
    decoded->field##_size = record - start - decoded->field##_offset; \
  } HB_STMT_END

  READ_UINT32VAR (decoded->flags);

  decoded->gid_offset = record - start;
  എങ്കിൽ (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    എങ്കിൽ (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) തിരികെ അസത്യം;
    hb_barrier ();
    decoded->gid = * (സ്ഥിരം HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  അല്ലെങ്കിൽ
  {
    decoded->gid_size = HBGlyphID16::static_size;
    എങ്കിൽ (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) തിരികെ അസത്യം;
    hb_barrier ();
    decoded->gid = * (സ്ഥിരം HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  എങ്കിൽ (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  എങ്കിൽ (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_എണ്ണം;
    എങ്കിൽ (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      എങ്കിൽ (unlikely (axis_indices->in_error ())) തിരികെ അസത്യം;
      axis_എണ്ണം = axis_indices->length;
    }
    അല്ലെങ്കിൽ
      axis_എണ്ണം = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    എങ്കിൽ (unlikely (budget && axis_എണ്ണം &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_എണ്ണം)))
      തിരികെ അസത്യം;
    എങ്കിൽ (axis_മൂല്യങ്ങൾ)
    {
      എങ്കിൽ (unlikely (!axis_മൂല്യങ്ങൾ->resize (axis_എണ്ണം))) തിരികെ അസത്യം;
      സ്ഥിരം HBUINT8 *p = (സ്ഥിരം HBUINT8 *) record;
      എങ്കിൽ (unlikely (!Tuple_മൂല്യങ്ങൾ::decompile (p, *axis_മൂല്യങ്ങൾ,
					    (സ്ഥിരം HBUINT8 *) end)))
	തിരികെ അസത്യം;
      record = (സ്ഥിരം unsigned അക്ഷരം *) p;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!Tuple_മൂല്യങ്ങൾ::skip (record, end, axis_എണ്ണം)))
      തിരികെ അസത്യം;
  }

  decoded->axis_മൂല്യങ്ങൾ_var_idx = VarIdx::NO_VARIATION;
  എങ്കിൽ (decoded->flags & (unsigned) flags_t::AXIS_മൂല്യങ്ങൾ_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_മൂല്യങ്ങൾ_var_idx, axis_മൂല്യങ്ങൾ_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  എങ്കിൽ (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  എങ്കിൽ (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    എങ്കിൽ (unlikely (unsigned (end - record) < HBINT16::static_size)) തിരികെ അസത്യം; \
    hb_barrier (); \
    decoded->transform.name = * (സ്ഥിരം HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  ആകുമ്പോൾ (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  തിരികെ സത്യം;
}

തർക്കം
VARC::closure_glyphs (hb_set_t *glyphset) സ്ഥിരം
{
  എങ്കിൽ (unlikely (glyphset->in_error ()))
    തിരികെ അസത്യം;

  hb_set_t pending = *glyphset;
  എങ്കിൽ (unlikely (pending.in_error ()))
    തിരികെ അസത്യം;

  ആകുമ്പോൾ (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (ഇത്+coverage).get_coverage (gid);
    എങ്കിൽ (index == NOT_COVERED) തുടരുക;

    hb_ubytes_t record = (ഇത്+glyphRecords)[index];
    ആകുമ്പോൾ (record)
    {
      VarComponent::record_t component;
      എങ്കിൽ (unlikely (!VarComponent::decompile_record (*ഇത്, record,
						     nullptr, nullptr, &component)))
	തിരികെ അസത്യം;
      എങ്കിൽ (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	എങ്കിൽ (unlikely (glyphset->in_error ()))
	  തിരികെ അസത്യം;
	pending.add (component.gid);
	എങ്കിൽ (unlikely (pending.in_error ()))
	  തിരികെ അസത്യം;
      }
      record = record.sub_array (component.size);
    }
  }
  തിരികെ സത്യം;
}

ശൂന്യം
VARC::depend (hb_depend_data_builder_t *depend_data) സ്ഥിരം
{
  സ്ഥിരം CFF2Index &records = ഇത്+glyphRecords;
  വേണ്ടി (auto _ : + hb_zip (ഇത്+coverage,
			  hb_range ((unsigned) records.എണ്ണം)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    ആകുമ്പോൾ (record)
    {
	VarComponent::record_t component;
	എങ്കിൽ (unlikely (!VarComponent::decompile_record (*ഇത്, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  തിരികെ;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

ഘടന hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  ശൂന്യം *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

സ്ഥിതം ശൂന്യം
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ശൂന്യം *data,
			     hb_draw_state_t *st,
			     ദശാംശം to_x, ദശാംശം to_y,
			     ശൂന്യം *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_transforming_pen_വരി_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ശൂന്യം *data,
			     hb_draw_state_t *st,
			     ദശാംശം to_x, ദശാംശം to_y,
			     ശൂന്യം *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->വരി_to (c->data, *c->st, to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ശൂന്യം *data,
				  hb_draw_state_t *st,
				  ദശാംശം control_x, ദശാംശം control_y,
				  ദശാംശം to_x, ദശാംശം to_y,
				  ശൂന്യം *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      ശൂന്യം *data,
			      hb_draw_state_t *st,
			      ദശാംശം control1_x, ദശാംശം control1_y,
			      ദശാംശം control2_x, ദശാംശം control2_y,
			      ദശാംശം to_x, ദശാംശം to_y,
			      ശൂന്യം *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				ശൂന്യം *data,
				hb_draw_state_t *st,
				ശൂന്യം *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

സ്ഥിതം int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, ശൂന്യം *draw_data, ശൂന്യം *)
{
  തിരികെ ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_transforming_pen_funcs ();

സ്ഥിതം ഘടന hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_വരി_to_func (funcs, hb_transforming_pen_വരി_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    തിരികെ funcs;
  }
} static_transforming_pen_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

സ്ഥിതം hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  തിരികെ static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (സ്ഥിരം hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) സ്ഥിരം
{
  എങ്കിൽ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    തിരികെ hb_ubytes_t ();

  സ്ഥിരം unsigned അക്ഷരം *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_മൂല്യങ്ങൾ_2 = c.scratch.axis_മൂല്യങ്ങൾ_2;
  record_t component;
  എങ്കിൽ (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_മൂല്യങ്ങൾ_2, &component,
				   &c.budget)))
    തിരികെ hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  സ്ഥിരം unsigned അക്ഷരം *record = total_record.arrayZ + component.size;

  // Condition
  തർക്കം show = സത്യം;
  എങ്കിൽ (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    സ്ഥിരം auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  എങ്കിൽ ((flags & (unsigned) flags_t::AXIS_മൂല്യങ്ങൾ_HAVE_VARIATION) &&
      show && !axis_മൂല്യങ്ങൾ_2.in_error ())
  {
    എങ്കിൽ (unlikely (!hb_budget_spend (c.budget, axis_മൂല്യങ്ങൾ_2.length,
				    coords.length)))
      തിരികെ hb_ubytes_t ();
    varStore.get_delta (component.axis_മൂല്യങ്ങൾ_var_idx, coords,
			axis_മൂല്യങ്ങൾ_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  എങ്കിൽ ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->അക്കം_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  എങ്കിൽ (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    എങ്കിൽ (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      തിരികെ hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<പൂർണ്ണസംഖ്യ> ());
    വേണ്ടി (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<പൂർണ്ണസംഖ്യ> (round (axis_മൂല്യങ്ങൾ_2[i]));
    എങ്കിൽ (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    എങ്കിൽ (transformVarIdx != VarIdx::NO_VARIATION)
    {
      ഇരട്ട transform_മൂല്യങ്ങൾ[9];
      unsigned അക്കം_Transform_മൂല്യങ്ങൾ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  എങ്കിൽ (flags & (unsigned) flags_t::flag) \
	    transform_മൂല്യങ്ങൾ[അക്കം_Transform_മൂല്യങ്ങൾ++] = (ഇരട്ട) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      എങ്കിൽ (unlikely (!hb_budget_spend (c.budget, അക്കം_Transform_മൂല്യങ്ങൾ,
				      coords.length)))
	തിരികെ hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_മൂല്യങ്ങൾ, അക്കം_Transform_മൂല്യങ്ങൾ), cache);
      അക്കം_Transform_മൂല്യങ്ങൾ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  എങ്കിൽ (flags & (unsigned) flags_t::flag) \
	    transform.name = (ദശാംശം) transform_മൂല്യങ്ങൾ[അക്കം_Transform_മൂല്യങ്ങൾ++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  എങ്കിൽ (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    എങ്കിൽ (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    തർക്കം same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_ഇടത്--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_ഇടത്++;
  }

  തിരികെ hb_ubytes_t (record, end - record);
}

തർക്കം
VARC::get_path_at (സ്ഥിരം hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) സ്ഥിരം
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (ഇത്+coverage).get_coverage (glyph);
  എങ്കിൽ (idx == NOT_COVERED)
  {
    എങ്കിൽ (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      എങ്കിൽ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) തിരികെ സത്യം;

      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {leaf_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_data,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = leaf_transform.is_identity () ? *c.draw_session : transformer_session;

      എങ്കിൽ (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) തിരികെ സത്യം;
#ifndef HB_NO_CFF
      എങ്കിൽ (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) തിരികെ സത്യം;
      എങ്കിൽ (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) തിരികെ സത്യം; // Doesn't have variations
#endif
      തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (c.extents)
    {
      എങ്കിൽ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) തിരികെ സത്യം;

      hb_glyph_extents_t glyph_extents;
      എങ്കിൽ (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      എങ്കിൽ (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      എങ്കിൽ (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	തിരികെ അസത്യം;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    തിരികെ സത്യം;
  }

  എങ്കിൽ (c.depth_ഇടത് <= 0)
    തിരികെ സത്യം;

  എങ്കിൽ (c.edges_ഇടത് <= 0)
    തിരികെ സത്യം;
  (c.edges_ഇടത്)--;

  hb_decycler_കണ്ണി_t node (c.decycler);
  എങ്കിൽ (unlikely (!node.visit (glyph)))
    തിരികെ സത്യം;

  hb_ubytes_t record = (ഇത്+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (ഇത്+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  എങ്കിൽ (cache != parent_cache)
    (ഇത്+varStore).destroy_cache (cache, &static_cache);

  തിരികെ സത്യം;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
