#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#அந்தர்பாவ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#அந்தர்பாவ "../../../hb-draw.hh"
#அந்தர்பாவ "../../../hb-depend-data.hh"
#அந்தர்பாவ "../../../hb-ot-layout-common.hh"
#அந்தர்பாவ "../../../hb-ot-layout-gdef-table.hh"

நாமாகாஶ OT {

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

தருக்க
VarComponent::decompile_record (மாறிலி VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<இரட்டை> *axis_மதிப்புகள்,
				record_t *decoded,
				int64_t *budget)
{
  மாறிலி unsigned எழுத்து *start = total_record.arrayZ;
  மாறிலி unsigned எழுத்து *record = start;
  மாறிலி unsigned எழுத்து *end = start + total_record.length;

  எனில் (axis_indices) axis_indices->clear ();
  எனில் (axis_மதிப்புகள்) axis_மதிப்புகள்->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    எனில் (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) திரும்பு பொய்; \
    hb_barrier (); \
    auto &varint = * (மாறிலி HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    எனில் (unlikely (unsigned (end - record) < size)) திரும்பு பொய்; \
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
  எனில் (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    எனில் (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) திரும்பு பொய்;
    hb_barrier ();
    decoded->gid = * (மாறிலி HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  இல்லையெனில்
  {
    decoded->gid_size = HBGlyphID16::static_size;
    எனில் (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) திரும்பு பொய்;
    hb_barrier ();
    decoded->gid = * (மாறிலி HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  எனில் (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  எனில் (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_மொத்தம்;
    எனில் (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      எனில் (unlikely (axis_indices->in_error ())) திரும்பு பொய்;
      axis_மொத்தம் = axis_indices->length;
    }
    இல்லையெனில்
      axis_மொத்தம் = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    எனில் (unlikely (budget && axis_மொத்தம் &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_மொத்தம்)))
      திரும்பு பொய்;
    எனில் (axis_மதிப்புகள்)
    {
      எனில் (unlikely (!axis_மதிப்புகள்->resize (axis_மொத்தம்))) திரும்பு பொய்;
      மாறிலி HBUINT8 *p = (மாறிலி HBUINT8 *) record;
      எனில் (unlikely (!Tuple_மதிப்புகள்::decompile (p, *axis_மதிப்புகள்,
					    (மாறிலி HBUINT8 *) end)))
	திரும்பு பொய்;
      record = (மாறிலி unsigned எழுத்து *) p;
    }
    இல்லையெனில் எனில் (unlikely (!Tuple_மதிப்புகள்::skip (record, end, axis_மொத்தம்)))
      திரும்பு பொய்;
  }

  decoded->axis_மதிப்புகள்_var_idx = VarIdx::NO_VARIATION;
  எனில் (decoded->flags & (unsigned) flags_t::AXIS_மதிப்புகள்_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_மதிப்புகள்_var_idx, axis_மதிப்புகள்_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  எனில் (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  எனில் (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    எனில் (unlikely (unsigned (end - record) < HBINT16::static_size)) திரும்பு பொய்; \
    hb_barrier (); \
    decoded->transform.name = * (மாறிலி HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  போது (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  திரும்பு உண்மை;
}

தருக்க
VARC::closure_glyphs (hb_set_t *glyphset) மாறிலி
{
  எனில் (unlikely (glyphset->in_error ()))
    திரும்பு பொய்;

  hb_set_t pending = *glyphset;
  எனில் (unlikely (pending.in_error ()))
    திரும்பு பொய்;

  போது (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (இது+coverage).get_coverage (gid);
    எனில் (index == NOT_COVERED) தொடர்;

    hb_ubytes_t record = (இது+glyphRecords)[index];
    போது (record)
    {
      VarComponent::record_t component;
      எனில் (unlikely (!VarComponent::decompile_record (*இது, record,
						     nullptr, nullptr, &component)))
	திரும்பு பொய்;
      எனில் (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	எனில் (unlikely (glyphset->in_error ()))
	  திரும்பு பொய்;
	pending.add (component.gid);
	எனில் (unlikely (pending.in_error ()))
	  திரும்பு பொய்;
      }
      record = record.sub_array (component.size);
    }
  }
  திரும்பு உண்மை;
}

வெறுமை
VARC::depend (hb_depend_data_builder_t *depend_data) மாறிலி
{
  மாறிலி CFF2Index &records = இது+glyphRecords;
  க்கு (auto _ : + hb_zip (இது+coverage,
			  hb_range ((unsigned) records.மொத்தம்)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    போது (record)
    {
	VarComponent::record_t component;
	எனில் (unlikely (!VarComponent::decompile_record (*இது, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  திரும்பு;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

கட்டமைப்பு hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  வெறுமை *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

நிலையான வெறுமை
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     வெறுமை *data,
			     hb_draw_state_t *st,
			     தசம to_x, தசம to_y,
			     வெறுமை *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

நிலையான வெறுமை
hb_transforming_pen_வரி_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     வெறுமை *data,
			     hb_draw_state_t *st,
			     தசம to_x, தசம to_y,
			     வெறுமை *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->வரி_to (c->data, *c->st, to_x, to_y);
}

நிலையான வெறுமை
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  வெறுமை *data,
				  hb_draw_state_t *st,
				  தசம control_x, தசம control_y,
				  தசம to_x, தசம to_y,
				  வெறுமை *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

நிலையான வெறுமை
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      வெறுமை *data,
			      hb_draw_state_t *st,
			      தசம control1_x, தசம control1_y,
			      தசம control2_x, தசம control2_y,
			      தசம to_x, தசம to_y,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

நிலையான வெறுமை
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				வெறுமை *data,
				hb_draw_state_t *st,
				வெறுமை *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

நிலையான int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, வெறுமை *draw_data, வெறுமை *)
{
  திரும்பு ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

நிலையான உள்வரி வெறுமை free_static_transforming_pen_funcs ();

நிலையான கட்டமைப்பு hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  நிலையான hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_வரி_to_func (funcs, hb_transforming_pen_வரி_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    திரும்பு funcs;
  }
} static_transforming_pen_funcs;

நிலையான உள்வரி
வெறுமை free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

நிலையான hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  திரும்பு static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (மாறிலி hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<மாறிலி முழு> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) மாறிலி
{
  எனில் (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    திரும்பு hb_ubytes_t ();

  மாறிலி unsigned எழுத்து *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_மதிப்புகள்_2 = c.scratch.axis_மதிப்புகள்_2;
  record_t component;
  எனில் (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_மதிப்புகள்_2, &component,
				   &c.budget)))
    திரும்பு hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  மாறிலி unsigned எழுத்து *record = total_record.arrayZ + component.size;

  // Condition
  தருக்க show = உண்மை;
  எனில் (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    மாறிலி auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  எனில் ((flags & (unsigned) flags_t::AXIS_மதிப்புகள்_HAVE_VARIATION) &&
      show && !axis_மதிப்புகள்_2.in_error ())
  {
    எனில் (unlikely (!hb_budget_spend (c.budget, axis_மதிப்புகள்_2.length,
				    coords.length)))
      திரும்பு hb_ubytes_t ();
    varStore.get_delta (component.axis_மதிப்புகள்_var_idx, coords,
			axis_மதிப்புகள்_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  எனில் ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->இலக்கம்_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  எனில் (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    எனில் (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      திரும்பு hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<முழு> ());
    க்கு (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<முழு> (round (axis_மதிப்புகள்_2[i]));
    எனில் (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    எனில் (transformVarIdx != VarIdx::NO_VARIATION)
    {
      இரட்டை transform_மதிப்புகள்[9];
      unsigned இலக்கம்_Transform_மதிப்புகள் = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  எனில் (flags & (unsigned) flags_t::flag) \
	    transform_மதிப்புகள்[இலக்கம்_Transform_மதிப்புகள்++] = (இரட்டை) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      எனில் (unlikely (!hb_budget_spend (c.budget, இலக்கம்_Transform_மதிப்புகள்,
				      coords.length)))
	திரும்பு hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_மதிப்புகள், இலக்கம்_Transform_மதிப்புகள்), cache);
      இலக்கம்_Transform_மதிப்புகள் = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  எனில் (flags & (unsigned) flags_t::flag) \
	    transform.name = (தசம) transform_மதிப்புகள்[இலக்கம்_Transform_மதிப்புகள்++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  எனில் (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    எனில் (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    தருக்க same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_இடது--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_இடது++;
  }

  திரும்பு hb_ubytes_t (record, end - record);
}

தருக்க
VARC::get_path_at (மாறிலி hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<மாறிலி முழு> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) மாறிலி
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (இது+coverage).get_coverage (glyph);
  எனில் (idx == NOT_COVERED)
  {
    எனில் (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      எனில் (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) திரும்பு உண்மை;

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

      எனில் (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) திரும்பு உண்மை;
#ifndef HB_NO_CFF
      எனில் (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) திரும்பு உண்மை;
      எனில் (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) திரும்பு உண்மை; // Doesn't have variations
#endif
      திரும்பு பொய்;
    }
    இல்லையெனில் எனில் (c.extents)
    {
      எனில் (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) திரும்பு உண்மை;

      hb_glyph_extents_t glyph_extents;
      எனில் (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      எனில் (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      எனில் (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	திரும்பு பொய்;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    திரும்பு உண்மை;
  }

  எனில் (c.depth_இடது <= 0)
    திரும்பு உண்மை;

  எனில் (c.edges_இடது <= 0)
    திரும்பு உண்மை;
  (c.edges_இடது)--;

  hb_decycler_முனை_t node (c.decycler);
  எனில் (unlikely (!node.visit (glyph)))
    திரும்பு உண்மை;

  hb_ubytes_t record = (இது+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (இது+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  எனில் (cache != parent_cache)
    (இது+varStore).destroy_cache (cache, &static_cache);

  திரும்பு உண்மை;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
