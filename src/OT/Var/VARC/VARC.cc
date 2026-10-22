#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#অন্তর্ভাব "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#অন্তর্ভাব "../../../hb-draw.hh"
#অন্তর্ভাব "../../../hb-depend-data.hh"
#অন্তর্ভাব "../../../hb-ot-layout-common.hh"
#অন্তর্ভাব "../../../hb-ot-layout-gdef-table.hh"

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

যৌক্তিক
VarComponent::decompile_record (ধ্রুবক VARC &varc,
				hb_ubytes_t যোগ_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<দ্বিগুণ> *axis_মানসমূহ,
				record_t *decoded,
				int64_t *budget)
{
  ধ্রুবক unsigned অক্ষর *start = যোগ_record.arrayZ;
  ধ্রুবক unsigned অক্ষর *record = start;
  ধ্রুবক unsigned অক্ষর *end = start + যোগ_record.দৈর্ঘ্য;

  যদি (axis_indices) axis_indices->clear ();
  যদি (axis_মানসমূহ) axis_মানসমূহ->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    যদি (unlikely (unsigned (end - record) < HBUINT32VAR::min_আকার)) ফেরত মিথ্যা; \
    hb_barrier (); \
    auto &varint = * (ধ্রুবক HBUINT32VAR *) record; \
    unsigned আকার_2 = varint.get_আকার (); \
    যদি (unlikely (unsigned (end - record) < আকার_2)) ফেরত মিথ্যা; \
    name = (uint32_t) varint; \
    record += আকার_2; \
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
    decoded->gid_আকার = HBGlyphID24::static_আকার;
    যদি (unlikely (unsigned (end - record) < HBGlyphID24::static_আকার)) ফেরত মিথ্যা;
    hb_barrier ();
    decoded->gid = * (ধ্রুবক HBGlyphID24 *) record;
    record += HBGlyphID24::static_আকার;
  }
  অন্যথা
  {
    decoded->gid_আকার = HBGlyphID16::static_আকার;
    যদি (unlikely (unsigned (end - record) < HBGlyphID16::static_আকার)) ফেরত মিথ্যা;
    hb_barrier ();
    decoded->gid = * (ধ্রুবক HBGlyphID16 *) record;
    record += HBGlyphID16::static_আকার;
  }

  যদি (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_সূচক, condition);

  যদি (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_সূচক, axis_indices);
    unsigned axis_গুনতি;
    যদি (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_সূচক]);
      যদি (unlikely (axis_indices->in_error ())) ফেরত মিথ্যা;
      axis_গুনতি = axis_indices->দৈর্ঘ্য;
    }
    অন্যথা
      axis_গুনতি = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_সূচক]);
    যদি (unlikely (budget && axis_গুনতি &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_গুনতি)))
      ফেরত মিথ্যা;
    যদি (axis_মানসমূহ)
    {
      যদি (unlikely (!axis_মানসমূহ->resize (axis_গুনতি))) ফেরত মিথ্যা;
      ধ্রুবক HBUINT8 *p = (ধ্রুবক HBUINT8 *) record;
      যদি (unlikely (!Tuple_মানসমূহ::decompile (p, *axis_মানসমূহ,
					    (ধ্রুবক HBUINT8 *) end)))
	ফেরত মিথ্যা;
      record = (ধ্রুবক unsigned অক্ষর *) p;
    }
    অন্যথা যদি (unlikely (!Tuple_মানসমূহ::skip (record, end, axis_গুনতি)))
      ফেরত মিথ্যা;
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
    static_assert (type::static_আকার == HBINT16::static_আকার, ""); \
    যদি (unlikely (unsigned (end - record) < HBINT16::static_আকার)) ফেরত মিথ্যা; \
    hb_barrier (); \
    decoded->transform.name = * (ধ্রুবক HBINT16 *) record; \
    record += HBINT16::static_আকার; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  যখন (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->আকার_2 = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  ফেরত সত্য;
}

যৌক্তিক
VARC::closure_glyphs (hb_set_t *glyphset) ধ্রুবক
{
  যদি (unlikely (glyphset->in_error ()))
    ফেরত মিথ্যা;

  hb_set_t pending = *glyphset;
  যদি (unlikely (pending.in_error ()))
    ফেরত মিথ্যা;

  যখন (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned সূচক_2 = (এই+coverage).get_coverage (gid);
    যদি (সূচক_2 == NOT_COVERED) চালু;

    hb_ubytes_t record = (এই+glyphRecords)[সূচক_2];
    যখন (record)
    {
      VarComponent::record_t component;
      যদি (unlikely (!VarComponent::decompile_record (*এই, record,
						     nullptr, nullptr, &component)))
	ফেরত মিথ্যা;
      যদি (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	যদি (unlikely (glyphset->in_error ()))
	  ফেরত মিথ্যা;
	pending.add (component.gid);
	যদি (unlikely (pending.in_error ()))
	  ফেরত মিথ্যা;
      }
      record = record.sub_array (component.আকার_2);
    }
  }
  ফেরত সত্য;
}

শূন্য
VARC::depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
{
  ধ্রুবক CFF2_সূচক &records = এই+glyphRecords;
  জন্য (auto _ : + hb_zip (এই+coverage,
			  hb_range ((unsigned) records.গুনতি)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    যখন (record)
    {
	VarComponent::record_t component;
	যদি (unlikely (!VarComponent::decompile_record (*এই, record,
						       nullptr, nullptr, &component)))
	{
	  depend_তথ্য->fail ();
	  ফেরত;
	}
      depend_তথ্য->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.আকার_2);
    }
  }
}


#ifndef HB_NO_DRAW

কাঠামো hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  শূন্য *তথ্য;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

স্থির শূন্য
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     শূন্য *তথ্য,
			     hb_draw_state_t *st,
			     দশমিক to_x, দশমিক to_y,
			     শূন্য *user_তথ্য HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) তথ্য;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->তথ্য, *c->st, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_সারি_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     শূন্য *তথ্য,
			     hb_draw_state_t *st,
			     দশমিক to_x, দশমিক to_y,
			     শূন্য *user_তথ্য HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) তথ্য;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->সারি_to (c->তথ্য, *c->st, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  শূন্য *তথ্য,
				  hb_draw_state_t *st,
				  দশমিক control_x, দশমিক control_y,
				  দশমিক to_x, দশমিক to_y,
				  শূন্য *user_তথ্য HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) তথ্য;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->তথ্য, *c->st, control_x, control_y, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      শূন্য *তথ্য,
			      hb_draw_state_t *st,
			      দশমিক control1_x, দশমিক control1_y,
			      দশমিক control2_x, দশমিক control2_y,
			      দশমিক to_x, দশমিক to_y,
			      শূন্য *user_তথ্য HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) তথ্য;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->তথ্য, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

স্থির শূন্য
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				শূন্য *তথ্য,
				hb_draw_state_t *st,
				শূন্য *user_তথ্য HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) তথ্য;

  c->dfuncs->close_path (c->তথ্য, *c->st);
}

স্থির int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, শূন্য *draw_তথ্য, শূন্য *)
{
  ফেরত ((hb_transforming_pen_context_t *) draw_তথ্য)->budget_remaining;
}

স্থির অন্তঃরেখ শূন্য free_static_transforming_pen_funcs ();

স্থির কাঠামো hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  স্থির hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_সারি_to_func (funcs, hb_transforming_pen_সারি_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    ফেরত funcs;
  }
} static_transforming_pen_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

স্থির hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  ফেরত static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ধ্রুবক hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
			   hb_transform_t<> যোগ_transform,
			   hb_ubytes_t যোগ_record,
			   hb_scalar_cache_t *cache) ধ্রুবক
{
  যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    ফেরত hb_ubytes_t ();

  ধ্রুবক unsigned অক্ষর *end = যোগ_record.arrayZ + যোগ_record.দৈর্ঘ্য;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_মানসমূহ_2 = c.scratch.axis_মানসমূহ_2;
  record_t component;
  যদি (unlikely (!decompile_record (VARC, যোগ_record,
				   &axisIndices, &axis_মানসমূহ_2, &component,
				   &c.budget)))
    ফেরত hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ধ্রুবক unsigned অক্ষর *record = যোগ_record.arrayZ + component.আকার_2;

  // Condition
  যৌক্তিক show = সত্য;
  যদি (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ধ্রুবক auto &condition = (&VARC+VARC.conditionList)[component.condition_সূচক];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.দৈর্ঘ্য, &instancer);
  }

  // Axis values

  // Apply variations if any
  যদি ((flags & (unsigned) flags_t::AXIS_মানসমূহ_HAVE_VARIATION) &&
      show && !axis_মানসমূহ_2.in_error ())
  {
    যদি (unlikely (!hb_budget_spend (c.budget, axis_মানসমূহ_2.দৈর্ঘ্য,
				    coords.দৈর্ঘ্য)))
      ফেরত hb_ubytes_t ();
    varStore.get_delta (component.axis_মানসমূহ_var_idx, coords,
			axis_মানসমূহ_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  যদি ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.দৈর্ঘ্য > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->অঙ্ক_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  যদি (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    যদি (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.দৈর্ঘ্য)))
      ফেরত hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<পূর্ণসংখ্যা> ());
    জন্য (unsigned i = 0; i < axisIndices.দৈর্ঘ্য; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<পূর্ণসংখ্যা> (round (axis_মানসমূহ_2[i]));
    যদি (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    যদি (transformVarIdx != VarIdx::NO_VARIATION)
    {
      দ্বিগুণ transform_মানসমূহ[9];
      unsigned অঙ্ক_Transform_মানসমূহ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  যদি (flags & (unsigned) flags_t::flag) \
	    transform_মানসমূহ[অঙ্ক_Transform_মানসমূহ++] = (দ্বিগুণ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      যদি (unlikely (!hb_budget_spend (c.budget, অঙ্ক_Transform_মানসমূহ,
				      coords.দৈর্ঘ্য)))
	ফেরত hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_মানসমূহ, অঙ্ক_Transform_মানসমূহ), cache);
      অঙ্ক_Transform_মানসমূহ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  যদি (flags & (unsigned) flags_t::flag) \
	    transform.name = (দশমিক) transform_মানসমূহ[অঙ্ক_Transform_মানসমূহ++];
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

    যোগ_transform.transform (transform.to_transform ());

    যৌক্তিক same_coords = component_coords.দৈর্ঘ্য == coords.দৈর্ঘ্য &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_বাম--;
    VARC.get_path_at (c, gid,
		      component_coords, যোগ_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_বাম++;
  }

  ফেরত hb_ubytes_t (record, end - record);
}

যৌক্তিক
VARC::get_path_at (ধ্রুবক hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ধ্রুবক
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (এই+coverage).get_coverage (glyph);
  যদি (idx == NOT_COVERED)
  {
    যদি (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ফেরত সত্য;

      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {leaf_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_তথ্য,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = leaf_transform.is_identity () ? *c.draw_session : transformer_session;

      যদি (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) ফেরত সত্য;
#ifndef HB_NO_CFF
      যদি (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) ফেরত সত্য;
      যদি (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) ফেরত সত্য; // Doesn't have variations
#endif
      ফেরত মিথ্যা;
    }
    অন্যথা যদি (c.extents)
    {
      যদি (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ফেরত সত্য;

      hb_glyph_extents_t glyph_extents;
      যদি (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      যদি (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      যদি (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	ফেরত মিথ্যা;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    ফেরত সত্য;
  }

  যদি (c.depth_বাম <= 0)
    ফেরত সত্য;

  যদি (c.edges_বাম <= 0)
    ফেরত সত্য;
  (c.edges_বাম)--;

  hb_decycler_গ্রন্থি_t node (c.decycler);
  যদি (unlikely (!node.visit (glyph)))
    ফেরত সত্য;

  hb_ubytes_t record = (এই+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (এই+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  যদি (cache != parent_cache)
    (এই+varStore).destroy_cache (cache, &static_cache);

  ফেরত সত্য;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
