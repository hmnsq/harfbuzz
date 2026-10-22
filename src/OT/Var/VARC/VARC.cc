#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ضمن "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#ضمن "../../../hb-draw.hh"
#ضمن "../../../hb-depend-data.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gdef-table.hh"

نطاق OT {

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

منطقي
VarComponent::decompile_record (ثابت VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<مزدوج> *axis_values,
				record_t *decoded,
				int64_t *budget)
{
  ثابت unsigned حرف *start = total_record.arrayZ;
  ثابت unsigned حرف *record = start;
  ثابت unsigned حرف *end = start + total_record.length;

  אם (axis_indices) axis_indices->clear ();
  אם (axis_values) axis_values->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    אם (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) החזר שקר; \
    hb_barrier (); \
    auto &varint = * (ثابت HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    אם (unlikely (unsigned (end - record) < size)) החזר שקר; \
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
  אם (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    אם (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) החזר שקר;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  אחרת
  {
    decoded->gid_size = HBGlyphID16::static_size;
    אם (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) החזר שקר;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  אם (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  אם (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_count;
    אם (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      אם (unlikely (axis_indices->in_error ())) החזר שקר;
      axis_count = axis_indices->length;
    }
    אחרת
      axis_count = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    אם (unlikely (budget && axis_count &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_count)))
      החזר שקר;
    אם (axis_values)
    {
      אם (unlikely (!axis_values->resize (axis_count))) החזר שקר;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) record;
      אם (unlikely (!TupleValues::decompile (p, *axis_values,
					    (ثابت HBUINT8 *) end)))
	החזר שקר;
      record = (ثابت unsigned حرف *) p;
    }
    אחרת אם (unlikely (!TupleValues::skip (record, end, axis_count)))
      החזר שקר;
  }

  decoded->axis_values_var_idx = VarIdx::NO_VARIATION;
  אם (decoded->flags & (unsigned) flags_t::AXIS_VALUES_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_values_var_idx, axis_values_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  אם (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  אם (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    אם (unlikely (unsigned (end - record) < HBINT16::static_size)) החזר שקר; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  כלעוד (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  החזר אמת;
}

منطقي
VARC::closure_glyphs (hb_set_t *glyphset) ثابت
{
  אם (unlikely (glyphset->in_error ()))
    החזר שקר;

  hb_set_t pending = *glyphset;
  אם (unlikely (pending.in_error ()))
    החזר שקר;

  כלעוד (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (هذا+coverage).get_coverage (gid);
    אם (index == NOT_COVERED) استمر;

    hb_ubytes_t record = (هذا+glyphRecords)[index];
    כלעוד (record)
    {
      VarComponent::record_t component;
      אם (unlikely (!VarComponent::decompile_record (*هذا, record,
						     nullptr, nullptr, &component)))
	החזר שקר;
      אם (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	אם (unlikely (glyphset->in_error ()))
	  החזר שקר;
	pending.add (component.gid);
	אם (unlikely (pending.in_error ()))
	  החזר שקר;
      }
      record = record.sub_array (component.size);
    }
  }
  החזר אמת;
}

فراغ
VARC::depend (hb_depend_data_builder_t *depend_data) ثابت
{
  ثابت CFF2Index &records = هذا+glyphRecords;
  עבור (auto _ : + hb_zip (هذا+coverage,
			  hb_range ((unsigned) records.count)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    כלעוד (record)
    {
	VarComponent::record_t component;
	אם (unlikely (!VarComponent::decompile_record (*هذا, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  החזר;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

بنية hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  فراغ *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

ساكن فراغ
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     فراغ *data,
			     hb_draw_state_t *st,
			     عائم to_x, عائم to_y,
			     فراغ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

ساكن فراغ
hb_transforming_pen_line_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     فراغ *data,
			     hb_draw_state_t *st,
			     عائم to_x, عائم to_y,
			     فراغ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->line_to (c->data, *c->st, to_x, to_y);
}

ساكن فراغ
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  فراغ *data,
				  hb_draw_state_t *st,
				  عائم control_x, عائم control_y,
				  عائم to_x, عائم to_y,
				  فراغ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

ساكن فراغ
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      فراغ *data,
			      hb_draw_state_t *st,
			      عائم control1_x, عائم control1_y,
			      عائم control2_x, عائم control2_y,
			      عائم to_x, عائم to_y,
			      فراغ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

ساكن فراغ
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				فراغ *data,
				hb_draw_state_t *st,
				فراغ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

ساكن int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, فراغ *draw_data, فراغ *)
{
  החזר ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

ساكن مضمن فراغ free_static_transforming_pen_funcs ();

ساكن بنية hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_line_to_func (funcs, hb_transforming_pen_line_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    החזר funcs;
  }
} static_transforming_pen_funcs;

ساكن مضمن
فراغ free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

ساكن hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  החזר static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ثابت صحيح> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) ثابت
{
  אם (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    החזר hb_ubytes_t ();

  ثابت unsigned حرف *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axisValues = c.scratch.axisValues;
  record_t component;
  אם (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axisValues, &component,
				   &c.budget)))
    החזר hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت unsigned حرف *record = total_record.arrayZ + component.size;

  // Condition
  منطقي show = אמת;
  אם (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ثابت auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  אם ((flags & (unsigned) flags_t::AXIS_VALUES_HAVE_VARIATION) &&
      show && !axisValues.in_error ())
  {
    אם (unlikely (!hb_budget_spend (c.budget, axisValues.length,
				    coords.length)))
      החזר hb_ubytes_t ();
    varStore.get_delta (component.axis_values_var_idx, coords,
			axisValues.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  אם ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->num_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  אם (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    אם (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      החזר hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحيح> ());
    עבור (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<صحيح> (round (axisValues[i]));
    אם (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    אם (transformVarIdx != VarIdx::NO_VARIATION)
    {
      مزدوج transformValues[9];
      unsigned numTransformValues = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  אם (flags & (unsigned) flags_t::flag) \
	    transformValues[numTransformValues++] = (مزدوج) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      אם (unlikely (!hb_budget_spend (c.budget, numTransformValues,
				      coords.length)))
	החזר hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transformValues, numTransformValues), cache);
      numTransformValues = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  אם (flags & (unsigned) flags_t::flag) \
	    transform.name = (عائم) transformValues[numTransformValues++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  אם (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    אם (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    منطقي same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_left--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_left++;
  }

  החזר hb_ubytes_t (record, end - record);
}

منطقي
VARC::get_path_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحيح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ثابت
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (هذا+coverage).get_coverage (glyph);
  אם (idx == NOT_COVERED)
  {
    אם (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      אם (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) החזר אמת;

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

      אם (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) החזר אמת;
#ifndef HB_NO_CFF
      אם (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) החזר אמת;
      אם (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) החזר אמת; // Doesn't have variations
#endif
      החזר שקר;
    }
    אחרת אם (c.extents)
    {
      אם (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) החזר אמת;

      hb_glyph_extents_t glyph_extents;
      אם (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      אם (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      אם (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	החזר שקר;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    החזר אמת;
  }

  אם (c.depth_left <= 0)
    החזר אמת;

  אם (c.edges_left <= 0)
    החזר אמת;
  (c.edges_left)--;

  hb_decycler_node_t node (c.decycler);
  אם (unlikely (!node.visit (glyph)))
    החזר אמת;

  hb_ubytes_t record = (هذا+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (هذا+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  אם (cache != parent_cache)
    (هذا+varStore).destroy_cache (cache, &static_cache);

  החזר אמת;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
