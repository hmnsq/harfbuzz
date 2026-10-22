#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ಅನ್ತರ್ಭಾವ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#ಅನ್ತರ್ಭಾವ "../../../hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-depend-data.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-gdef-table.hh"

ನಾಮಾಕಾಶ OT {

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

ತಾರ್ಕಿಕ
VarComponent::decompile_record (ಧ್ರುವ VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<ದ್ವಿಗುಣ> *axis_ಮೌಲ್ಯಗಳು,
				record_t *decoded,
				int64_t *budget)
{
  ಧ್ರುವ unsigned ವರ್ಣ *start = total_record.arrayZ;
  ಧ್ರುವ unsigned ವರ್ಣ *record = start;
  ಧ್ರುವ unsigned ವರ್ಣ *end = start + total_record.length;

  ಯದಿ (axis_indices) axis_indices->clear ();
  ಯದಿ (axis_ಮೌಲ್ಯಗಳು) axis_ಮೌಲ್ಯಗಳು->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    ಯದಿ (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
    hb_barrier (); \
    auto &varint = * (ಧ್ರುವ HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    ಯದಿ (unlikely (unsigned (end - record) < size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
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
  ಯದಿ (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    ಯದಿ (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_barrier ();
    decoded->gid = * (ಧ್ರುವ HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  ಅನ್ಯಥಾ
  {
    decoded->gid_size = HBGlyphID16::static_size;
    ಯದಿ (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_barrier ();
    decoded->gid = * (ಧ್ರುವ HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  ಯದಿ (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  ಯದಿ (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_ಎಣಿಕೆ;
    ಯದಿ (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      ಯದಿ (unlikely (axis_indices->in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      axis_ಎಣಿಕೆ = axis_indices->length;
    }
    ಅನ್ಯಥಾ
      axis_ಎಣಿಕೆ = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    ಯದಿ (unlikely (budget && axis_ಎಣಿಕೆ &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (axis_ಮೌಲ್ಯಗಳು)
    {
      ಯದಿ (unlikely (!axis_ಮೌಲ್ಯಗಳು->resize (axis_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಧ್ರುವ HBUINT8 *p = (ಧ್ರುವ HBUINT8 *) record;
      ಯದಿ (unlikely (!Tuple_ಮೌಲ್ಯಗಳು::decompile (p, *axis_ಮೌಲ್ಯಗಳು,
					    (ಧ್ರುವ HBUINT8 *) end)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      record = (ಧ್ರುವ unsigned ವರ್ಣ *) p;
    }
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!Tuple_ಮೌಲ್ಯಗಳು::skip (record, end, axis_ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  decoded->axis_ಮೌಲ್ಯಗಳು_var_idx = VarIdx::NO_VARIATION;
  ಯದಿ (decoded->flags & (unsigned) flags_t::AXIS_ಮೌಲ್ಯಗಳು_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_ಮೌಲ್ಯಗಳು_var_idx, axis_ಮೌಲ್ಯಗಳು_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  ಯದಿ (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  ಯದಿ (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    ಯದಿ (unlikely (unsigned (end - record) < HBINT16::static_size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
    hb_barrier (); \
    decoded->transform.name = * (ಧ್ರುವ HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  ಯಾವತ್ (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ
VARC::closure_glyphs (hb_set_t *glyphset) ಧ್ರುವ
{
  ಯದಿ (unlikely (glyphset->in_error ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_set_t pending = *glyphset;
  ಯದಿ (unlikely (pending.in_error ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯಾವತ್ (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (ಏತತ್+coverage).get_coverage (gid);
    ಯದಿ (index == NOT_COVERED) ಅನುವರ್ತ;

    hb_ubytes_t record = (ಏತತ್+glyphRecords)[index];
    ಯಾವತ್ (record)
    {
      VarComponent::record_t component;
      ಯದಿ (unlikely (!VarComponent::decompile_record (*ಏತತ್, record,
						     nullptr, nullptr, &component)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	ಯದಿ (unlikely (glyphset->in_error ()))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	pending.add (component.gid);
	ಯದಿ (unlikely (pending.in_error ()))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      record = record.sub_array (component.size);
    }
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಶೂನ್ಯ
VARC::depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
{
  ಧ್ರುವ CFF2Index &records = ಏತತ್+glyphRecords;
  ಪ್ರತಿ (auto _ : + hb_zip (ಏತತ್+coverage,
			  hb_range ((unsigned) records.ಎಣಿಕೆ)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    ಯಾವತ್ (record)
    {
	VarComponent::record_t component;
	ಯದಿ (unlikely (!VarComponent::decompile_record (*ಏತತ್, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  ಪ್ರತ್ಯಾಗಮ;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

ಸಂರಚನಾ hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  ಶೂನ್ಯ *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

ಸ್ಥಿರ ಶೂನ್ಯ
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ಶೂನ್ಯ *data,
			     hb_draw_state_t *st,
			     ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_transforming_pen_ಸಾಲು_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ಶೂನ್ಯ *data,
			     hb_draw_state_t *st,
			     ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->ಸಾಲು_to (c->data, *c->st, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ಶೂನ್ಯ *data,
				  hb_draw_state_t *st,
				  ಭಿನ್ನ control_x, ಭಿನ್ನ control_y,
				  ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      ಶೂನ್ಯ *data,
			      hb_draw_state_t *st,
			      ಭಿನ್ನ control1_x, ಭಿನ್ನ control1_y,
			      ಭಿನ್ನ control2_x, ಭಿನ್ನ control2_y,
			      ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			      ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				ಶೂನ್ಯ *data,
				hb_draw_state_t *st,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

ಸ್ಥಿರ int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, ಶೂನ್ಯ *draw_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_transforming_pen_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ಸಾಲು_to_func (funcs, hb_transforming_pen_ಸಾಲು_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_transforming_pen_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

ಸ್ಥಿರ hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ಧ್ರುವ hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) ಧ್ರುವ
{
  ಯದಿ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();

  ಧ್ರುವ unsigned ವರ್ಣ *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_ಮೌಲ್ಯಗಳು_2 = c.scratch.axis_ಮೌಲ್ಯಗಳು_2;
  record_t component;
  ಯದಿ (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_ಮೌಲ್ಯಗಳು_2, &component,
				   &c.budget)))
    ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ಧ್ರುವ unsigned ವರ್ಣ *record = total_record.arrayZ + component.size;

  // Condition
  ತಾರ್ಕಿಕ show = ಸತ್ಯ;
  ಯದಿ (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ಧ್ರುವ auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  ಯದಿ ((flags & (unsigned) flags_t::AXIS_ಮೌಲ್ಯಗಳು_HAVE_VARIATION) &&
      show && !axis_ಮೌಲ್ಯಗಳು_2.in_error ())
  {
    ಯದಿ (unlikely (!hb_budget_spend (c.budget, axis_ಮೌಲ್ಯಗಳು_2.length,
				    coords.length)))
      ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
    varStore.get_delta (component.axis_ಮೌಲ್ಯಗಳು_var_idx, coords,
			axis_ಮೌಲ್ಯಗಳು_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  ಯದಿ ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->ಅಂಕೆ_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  ಯದಿ (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    ಯದಿ (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<ಪೂರ್ಣಾಙ್ಕ> ());
    ಪ್ರತಿ (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (round (axis_ಮೌಲ್ಯಗಳು_2[i]));
    ಯದಿ (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    ಯದಿ (transformVarIdx != VarIdx::NO_VARIATION)
    {
      ದ್ವಿಗುಣ transform_ಮೌಲ್ಯಗಳು[9];
      unsigned ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ಯದಿ (flags & (unsigned) flags_t::flag) \
	    transform_ಮೌಲ್ಯಗಳು[ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು++] = (ದ್ವಿಗುಣ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      ಯದಿ (unlikely (!hb_budget_spend (c.budget, ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು,
				      coords.length)))
	ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_ಮೌಲ್ಯಗಳು, ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು), cache);
      ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ಯದಿ (flags & (unsigned) flags_t::flag) \
	    transform.name = (ಭಿನ್ನ) transform_ಮೌಲ್ಯಗಳು[ಅಂಕೆ_Transform_ಮೌಲ್ಯಗಳು++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  ಯದಿ (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    ಯದಿ (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    ತಾರ್ಕಿಕ same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_ಎಡ--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_ಎಡ++;
  }

  ಪ್ರತ್ಯಾಗಮ hb_ubytes_t (record, end - record);
}

ತಾರ್ಕಿಕ
VARC::get_path_at (ಧ್ರುವ hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ಧ್ರುವ
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (ಏತತ್+coverage).get_coverage (glyph);
  ಯದಿ (idx == NOT_COVERED)
  {
    ಯದಿ (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      ಯದಿ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

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

      ಯದಿ (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ifndef HB_NO_CFF
      ಯದಿ (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಯದಿ (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; // Doesn't have variations
#endif
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (c.extents)
    {
      ಯದಿ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      hb_glyph_extents_t glyph_extents;
      ಯದಿ (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      ಯದಿ (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      ಯದಿ (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಯದಿ (c.depth_ಎಡ <= 0)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  ಯದಿ (c.edges_ಎಡ <= 0)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  (c.edges_ಎಡ)--;

  hb_decycler_ಗಂಟು_t node (c.decycler);
  ಯದಿ (unlikely (!node.visit (glyph)))
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  hb_ubytes_t record = (ಏತತ್+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (ಏತತ್+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  ಯದಿ (cache != parent_cache)
    (ಏತತ್+varStore).destroy_cache (cache, &static_cache);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
