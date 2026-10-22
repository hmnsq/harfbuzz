#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#অন্তর্ভাৱ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#অন্তর্ভাৱ "../../../hb-draw.hh"
#অন্তর্ভাৱ "../../../hb-depend-data.hh"
#অন্তর্ভাৱ "../../../hb-ot-layout-common.hh"
#অন্তর্ভাৱ "../../../hb-ot-layout-gdef-table.hh"

নামাকাশ OT {

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

তার্কিক
VarComponent::decompile_record (ধ্রুৱ VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<দ্ৱিগুণ> *axis_মানসমূহ,
				record_t *decoded,
				int64_t *budget)
{
  ধ্রুৱ unsigned ৱর্ণ *start = total_record.arrayZ;
  ধ্রুৱ unsigned ৱর্ণ *record = start;
  ধ্রুৱ unsigned ৱর্ণ *end = start + total_record.length;

  যদি (axis_indices) axis_indices->clear ();
  যদি (axis_মানসমূহ) axis_মানসমূহ->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    যদি (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) ঘূৰাই মিছা; \
    hb_barrier (); \
    auto &varint = * (ধ্রুৱ HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    যদি (unlikely (unsigned (end - record) < size)) ঘূৰাই মিছা; \
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
  যদি (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    যদি (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) ঘূৰাই মিছা;
    hb_barrier ();
    decoded->gid = * (ধ্রুৱ HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  অন্যথা
  {
    decoded->gid_size = HBGlyphID16::static_size;
    যদি (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) ঘূৰাই মিছা;
    hb_barrier ();
    decoded->gid = * (ধ্রুৱ HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  যদি (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  যদি (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_গুনতি;
    যদি (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      যদি (unlikely (axis_indices->in_error ())) ঘূৰাই মিছা;
      axis_গুনতি = axis_indices->length;
    }
    অন্যথা
      axis_গুনতি = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    যদি (unlikely (budget && axis_গুনতি &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_গুনতি)))
      ঘূৰাই মিছা;
    যদি (axis_মানসমূহ)
    {
      যদি (unlikely (!axis_মানসমূহ->resize (axis_গুনতি))) ঘূৰাই মিছা;
      ধ্রুৱ HBUINT8 *p = (ধ্রুৱ HBUINT8 *) record;
      যদি (unlikely (!Tuple_মানসমূহ::decompile (p, *axis_মানসমূহ,
					    (ধ্রুৱ HBUINT8 *) end)))
	ঘূৰাই মিছা;
      record = (ধ্রুৱ unsigned ৱর্ণ *) p;
    }
    অন্যথা যদি (unlikely (!Tuple_মানসমূহ::skip (record, end, axis_গুনতি)))
      ঘূৰাই মিছা;
  }

  decoded->axis_মানসমূহ_var_idx = VarIdx::NO_VARIATION;
  যদি (decoded->flags & (unsigned) flags_t::AXIS_মানসমূহ_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_মানসমূহ_var_idx, axis_মানসমূহ_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  যদি (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  যদি (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    যদি (unlikely (unsigned (end - record) < HBINT16::static_size)) ঘূৰাই মিছা; \
    hb_barrier (); \
    decoded->transform.name = * (ধ্রুৱ HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  যেতিয়ালৈ (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  ঘূৰাই সঁচা;
}

তার্কিক
VARC::closure_glyphs (hb_set_t *glyphset) ধ্রুৱ
{
  যদি (unlikely (glyphset->in_error ()))
    ঘূৰাই মিছা;

  hb_set_t pending = *glyphset;
  যদি (unlikely (pending.in_error ()))
    ঘূৰাই মিছা;

  যেতিয়ালৈ (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (এতত্+coverage).get_coverage (gid);
    যদি (index == NOT_COVERED) অনুৱর্ত;

    hb_ubytes_t record = (এতত্+glyphRecords)[index];
    যেতিয়ালৈ (record)
    {
      VarComponent::record_t component;
      যদি (unlikely (!VarComponent::decompile_record (*এতত্, record,
						     nullptr, nullptr, &component)))
	ঘূৰাই মিছা;
      যদি (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	যদি (unlikely (glyphset->in_error ()))
	  ঘূৰাই মিছা;
	pending.add (component.gid);
	যদি (unlikely (pending.in_error ()))
	  ঘূৰাই মিছা;
      }
      record = record.sub_array (component.size);
    }
  }
  ঘূৰাই সঁচা;
}

শূন্য
VARC::depend (hb_depend_data_builder_t *depend_data) ধ্রুৱ
{
  ধ্রুৱ CFF2Index &records = এতত্+glyphRecords;
  বাবে (auto _ : + hb_zip (এতত্+coverage,
			  hb_range ((unsigned) records.গুনতি)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    যেতিয়ালৈ (record)
    {
	VarComponent::record_t component;
	যদি (unlikely (!VarComponent::decompile_record (*এতত্, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  ঘূৰাই;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

সংরচনা hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  শূন্য *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

স্থির শূন্য
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     শূন্য *data,
			     hb_draw_state_t *st,
			     ভিন্ন to_x, ভিন্ন to_y,
			     শূন্য *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_সাৰি_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     শূন্য *data,
			     hb_draw_state_t *st,
			     ভিন্ন to_x, ভিন্ন to_y,
			     শূন্য *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->সাৰি_to (c->data, *c->st, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  শূন্য *data,
				  hb_draw_state_t *st,
				  ভিন্ন control_x, ভিন্ন control_y,
				  ভিন্ন to_x, ভিন্ন to_y,
				  শূন্য *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      শূন্য *data,
			      hb_draw_state_t *st,
			      ভিন্ন control1_x, ভিন্ন control1_y,
			      ভিন্ন control2_x, ভিন্ন control2_y,
			      ভিন্ন to_x, ভিন্ন to_y,
			      শূন্য *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				শূন্য *data,
				hb_draw_state_t *st,
				শূন্য *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

স্থির int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, শূন্য *draw_data, শূন্য *)
{
  ঘূৰাই ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

স্থির অন্তঃস্থ শূন্য free_static_transforming_pen_funcs ();

স্থির সংরচনা hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  স্থির hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_সাৰি_to_func (funcs, hb_transforming_pen_সাৰি_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    ঘূৰাই funcs;
  }
} static_transforming_pen_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

স্থির hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  ঘূৰাই static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ধ্রুৱ hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) ধ্রুৱ
{
  যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    ঘূৰাই hb_ubytes_t ();

  ধ্রুৱ unsigned ৱর্ণ *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_মানসমূহ_2 = c.scratch.axis_মানসমূহ_2;
  record_t component;
  যদি (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_মানসমূহ_2, &component,
				   &c.budget)))
    ঘূৰাই hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ধ্রুৱ unsigned ৱর্ণ *record = total_record.arrayZ + component.size;

  // Condition
  তার্কিক show = সঁচা;
  যদি (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ধ্রুৱ auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  যদি ((flags & (unsigned) flags_t::AXIS_মানসমূহ_HAVE_VARIATION) &&
      show && !axis_মানসমূহ_2.in_error ())
  {
    যদি (unlikely (!hb_budget_spend (c.budget, axis_মানসমূহ_2.length,
				    coords.length)))
      ঘূৰাই hb_ubytes_t ();
    varStore.get_delta (component.axis_মানসমূহ_var_idx, coords,
			axis_মানসমূহ_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  যদি ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->অঙ্ক_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  যদি (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    যদি (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      ঘূৰাই hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<পূর্ণাঙ্ক> ());
    বাবে (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<পূর্ণাঙ্ক> (round (axis_মানসমূহ_2[i]));
    যদি (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    যদি (transformVarIdx != VarIdx::NO_VARIATION)
    {
      দ্ৱিগুণ transform_মানসমূহ[9];
      unsigned অঙ্ক_Transform_মানসমূহ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  যদি (flags & (unsigned) flags_t::flag) \
	    transform_মানসমূহ[অঙ্ক_Transform_মানসমূহ++] = (দ্ৱিগুণ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      যদি (unlikely (!hb_budget_spend (c.budget, অঙ্ক_Transform_মানসমূহ,
				      coords.length)))
	ঘূৰাই hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_মানসমূহ, অঙ্ক_Transform_মানসমূহ), cache);
      অঙ্ক_Transform_মানসমূহ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  যদি (flags & (unsigned) flags_t::flag) \
	    transform.name = (ভিন্ন) transform_মানসমূহ[অঙ্ক_Transform_মানসমূহ++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  যদি (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    যদি (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    তার্কিক same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_বাম--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_বাম++;
  }

  ঘূৰাই hb_ubytes_t (record, end - record);
}

তার্কিক
VARC::get_path_at (ধ্রুৱ hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ধ্রুৱ
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (এতত্+coverage).get_coverage (glyph);
  যদি (idx == NOT_COVERED)
  {
    যদি (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ঘূৰাই সঁচা;

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

      যদি (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) ঘূৰাই সঁচা;
#ifndef HB_NO_CFF
      যদি (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) ঘূৰাই সঁচা;
      যদি (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) ঘূৰাই সঁচা; // Doesn't have variations
#endif
      ঘূৰাই মিছা;
    }
    অন্যথা যদি (c.extents)
    {
      যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ঘূৰাই সঁচা;

      hb_glyph_extents_t glyph_extents;
      যদি (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      যদি (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      যদি (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	ঘূৰাই মিছা;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    ঘূৰাই সঁচা;
  }

  যদি (c.depth_বাম <= 0)
    ঘূৰাই সঁচা;

  যদি (c.edges_বাম <= 0)
    ঘূৰাই সঁচা;
  (c.edges_বাম)--;

  hb_decycler_গ্ৰন্থি_t node (c.decycler);
  যদি (unlikely (!node.visit (glyph)))
    ঘূৰাই সঁচা;

  hb_ubytes_t record = (এতত্+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (এতত্+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  যদি (cache != parent_cache)
    (এতত্+varStore).destroy_cache (cache, &static_cache);

  ঘূৰাই সঁচা;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
