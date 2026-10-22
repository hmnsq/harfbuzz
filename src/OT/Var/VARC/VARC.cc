#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ଅନ୍ତର୍ଭାଵ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#ଅନ୍ତର୍ଭାଵ "../../../hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-depend-data.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-gdef-table.hh"

ନାମାକାଶ OT {

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

ତାର୍କିକ
VarComponent::decompile_record (ଅଚଳ VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<ଦ୍ୱିଗୁଣ> *axis_ମୂଲ୍ୟଗୁଡ଼ିକ,
				record_t *decoded,
				int64_t *budget)
{
  ଅଚଳ unsigned ଅକ୍ଷର *start = total_record.arrayZ;
  ଅଚଳ unsigned ଅକ୍ଷର *record = start;
  ଅଚଳ unsigned ଅକ୍ଷର *end = start + total_record.length;

  ଯଦି (axis_indices) axis_indices->clear ();
  ଯଦି (axis_ମୂଲ୍ୟଗୁଡ଼ିକ) axis_ମୂଲ୍ୟଗୁଡ଼ିକ->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    ଯଦି (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) ଫେରନ୍ତୁ ମିଥ୍ୟା; \
    hb_barrier (); \
    auto &varint = * (ଅଚଳ HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    ଯଦି (unlikely (unsigned (end - record) < size)) ଫେରନ୍ତୁ ମିଥ୍ୟା; \
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
  ଯଦି (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    ଯଦି (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_barrier ();
    decoded->gid = * (ଅଚଳ HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  ଅନ୍ୟଥା
  {
    decoded->gid_size = HBGlyphID16::static_size;
    ଯଦି (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_barrier ();
    decoded->gid = * (ଅଚଳ HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  ଯଦି (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  ଯଦି (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_ଗଣନା;
    ଯଦି (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      ଯଦି (unlikely (axis_indices->in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      axis_ଗଣନା = axis_indices->length;
    }
    ଅନ୍ୟଥା
      axis_ଗଣନା = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    ଯଦି (unlikely (budget && axis_ଗଣନା &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_ଗଣନା)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (axis_ମୂଲ୍ୟଗୁଡ଼ିକ)
    {
      ଯଦି (unlikely (!axis_ମୂଲ୍ୟଗୁଡ଼ିକ->resize (axis_ଗଣନା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଅଚଳ HBUINT8 *p = (ଅଚଳ HBUINT8 *) record;
      ଯଦି (unlikely (!Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::decompile (p, *axis_ମୂଲ୍ୟଗୁଡ଼ିକ,
					    (ଅଚଳ HBUINT8 *) end)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      record = (ଅଚଳ unsigned ଅକ୍ଷର *) p;
    }
    ଅନ୍ୟଥା ଯଦି (unlikely (!Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::skip (record, end, axis_ଗଣନା)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  decoded->axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_idx = VarIdx::NO_VARIATION;
  ଯଦି (decoded->flags & (unsigned) flags_t::AXIS_ମୂଲ୍ୟଗୁଡ଼ିକ_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_idx, axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  ଯଦି (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  ଯଦି (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    ଯଦି (unlikely (unsigned (end - record) < HBINT16::static_size)) ଫେରନ୍ତୁ ମିଥ୍ୟା; \
    hb_barrier (); \
    decoded->transform.name = * (ଅଚଳ HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  ଯେତେବେଳେ (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ
VARC::closure_glyphs (hb_set_t *glyphset) ଅଚଳ
{
  ଯଦି (unlikely (glyphset->in_error ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_set_t pending = *glyphset;
  ଯଦି (unlikely (pending.in_error ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯେତେବେଳେ (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (ଏହି+coverage).get_coverage (gid);
    ଯଦି (index == NOT_COVERED) ଜାରି;

    hb_ubytes_t record = (ଏହି+glyphRecords)[index];
    ଯେତେବେଳେ (record)
    {
      VarComponent::record_t component;
      ଯଦି (unlikely (!VarComponent::decompile_record (*ଏହି, record,
						     nullptr, nullptr, &component)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	ଯଦି (unlikely (glyphset->in_error ()))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	pending.add (component.gid);
	ଯଦି (unlikely (pending.in_error ()))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      record = record.sub_array (component.size);
    }
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ଶୂନ୍ୟ
VARC::depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
{
  ଅଚଳ CFF2Index &records = ଏହି+glyphRecords;
  ପାଇଁ (auto _ : + hb_zip (ଏହି+coverage,
			  hb_range ((unsigned) records.ଗଣନା_2)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    ଯେତେବେଳେ (record)
    {
	VarComponent::record_t component;
	ଯଦି (unlikely (!VarComponent::decompile_record (*ଏହି, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  ଫେରନ୍ତୁ;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

ସଂରଚନା hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  ଶୂନ୍ୟ *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

ସ୍ଥିର ଶୂନ୍ୟ
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ଶୂନ୍ୟ *data,
			     hb_draw_state_t *st,
			     ଦଶମିକ to_x, ଦଶମିକ to_y,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_transforming_pen_ଧାଡ଼ି_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ଶୂନ୍ୟ *data,
			     hb_draw_state_t *st,
			     ଦଶମିକ to_x, ଦଶମିକ to_y,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->ଧାଡ଼ି_to (c->data, *c->st, to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ଶୂନ୍ୟ *data,
				  hb_draw_state_t *st,
				  ଦଶମିକ control_x, ଦଶମିକ control_y,
				  ଦଶମିକ to_x, ଦଶମିକ to_y,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      ଶୂନ୍ୟ *data,
			      hb_draw_state_t *st,
			      ଦଶମିକ control1_x, ଦଶମିକ control1_y,
			      ଦଶମିକ control2_x, ଦଶମିକ control2_y,
			      ଦଶମିକ to_x, ଦଶମିକ to_y,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				ଶୂନ୍ୟ *data,
				hb_draw_state_t *st,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

ସ୍ଥିର int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, ଶୂନ୍ୟ *draw_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_transforming_pen_funcs ();

ସ୍ଥିର ସଂରଚନା hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ଧାଡ଼ି_to_func (funcs, hb_transforming_pen_ଧାଡ଼ି_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_transforming_pen_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

ସ୍ଥିର hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  ଫେରନ୍ତୁ static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ଅଚଳ hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) ଅଚଳ
{
  ଯଦି (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    ଫେରନ୍ତୁ hb_ubytes_t ();

  ଅଚଳ unsigned ଅକ୍ଷର *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2 = c.scratch.axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
  record_t component;
  ଯଦି (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2, &component,
				   &c.budget)))
    ଫେରନ୍ତୁ hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ଅଚଳ unsigned ଅକ୍ଷର *record = total_record.arrayZ + component.size;

  // Condition
  ତାର୍କିକ show = ସତ୍ୟ;
  ଯଦି (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ଅଚଳ auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  ଯଦି ((flags & (unsigned) flags_t::AXIS_ମୂଲ୍ୟଗୁଡ଼ିକ_HAVE_VARIATION) &&
      show && !axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.in_error ())
  {
    ଯଦି (unlikely (!hb_budget_spend (c.budget, axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.length,
				    coords.length)))
      ଫେରନ୍ତୁ hb_ubytes_t ();
    varStore.get_delta (component.axis_ମୂଲ୍ୟଗୁଡ଼ିକ_var_idx, coords,
			axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  ଯଦି ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->ଅଙ୍କ_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  ଯଦି (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    ଯଦି (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      ଫେରନ୍ତୁ hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ());
    ପାଇଁ (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (round (axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2[i]));
    ଯଦି (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    ଯଦି (transformVarIdx != VarIdx::NO_VARIATION)
    {
      ଦ୍ୱିଗୁଣ transform_ମୂଲ୍ୟଗୁଡ଼ିକ[9];
      unsigned ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ଯଦି (flags & (unsigned) flags_t::flag) \
	    transform_ମୂଲ୍ୟଗୁଡ଼ିକ[ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ++] = (ଦ୍ୱିଗୁଣ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      ଯଦି (unlikely (!hb_budget_spend (c.budget, ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ,
				      coords.length)))
	ଫେରନ୍ତୁ hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_ମୂଲ୍ୟଗୁଡ଼ିକ, ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ), cache);
      ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ଯଦି (flags & (unsigned) flags_t::flag) \
	    transform.name = (ଦଶମିକ) transform_ମୂଲ୍ୟଗୁଡ଼ିକ[ଅଙ୍କ_Transform_ମୂଲ୍ୟଗୁଡ଼ିକ++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ଯଦି (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    ଯଦି (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    ତାର୍କିକ same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_ବାମ--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_ବାମ++;
  }

  ଫେରନ୍ତୁ hb_ubytes_t (record, end - record);
}

ତାର୍କିକ
VARC::get_path_at (ଅଚଳ hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ଅଚଳ
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (ଏହି+coverage).get_coverage (glyph);
  ଯଦି (idx == NOT_COVERED)
  {
    ଯଦି (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      ଯଦି (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ଫେରନ୍ତୁ ସତ୍ୟ;

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

      ଯଦି (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_CFF
      ଯଦି (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ;
      ଯଦି (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ; // Doesn't have variations
#endif
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା ଯଦି (c.extents)
    {
      ଯଦି (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ଫେରନ୍ତୁ ସତ୍ୟ;

      hb_glyph_extents_t glyph_extents;
      ଯଦି (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      ଯଦି (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      ଯଦି (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଯଦି (c.depth_ବାମ <= 0)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  ଯଦି (c.edges_ବାମ <= 0)
    ଫେରନ୍ତୁ ସତ୍ୟ;
  (c.edges_ବାମ)--;

  hb_decycler_ଗଣ୍ଠି_t node (c.decycler);
  ଯଦି (unlikely (!node.visit (glyph)))
    ଫେରନ୍ତୁ ସତ୍ୟ;

  hb_ubytes_t record = (ଏହି+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (ଏହି+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  ଯଦି (cache != parent_cache)
    (ଏହି+varStore).destroy_cache (cache, &static_cache);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
