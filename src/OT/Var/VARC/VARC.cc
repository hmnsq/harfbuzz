#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#अन्तर्भाव "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#अन्तर्भाव "../../../hb-draw.hh"
#अन्तर्भाव "../../../hb-depend-data.hh"
#अन्तर्भाव "../../../hb-ot-layout-common.hh"
#अन्तर्भाव "../../../hb-ot-layout-gdef-table.hh"

नामाकाश OT {

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

तार्किक
VarComponent::decompile_record (स्थिरांक VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<अचिन्हित> *axis_indices,
				hb_vector_t<द्विगुण> *axis_मूल्ये,
				record_t *decoded,
				int64_t *budget)
{
  स्थिरांक अचिन्हित अक्षर *start = total_record.arrayZ;
  स्थिरांक अचिन्हित अक्षर *record = start;
  स्थिरांक अचिन्हित अक्षर *end = start + total_record.length;

  जर (axis_indices) axis_indices->clear ();
  जर (axis_मूल्ये) axis_मूल्ये->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    जर (unlikely (अचिन्हित (end - record) < HBUINT32VAR::min_size)) परत असत्य; \
    hb_barrier (); \
    स्वतः &varint = * (स्थिरांक HBUINT32VAR *) record; \
    अचिन्हित size = varint.get_size (); \
    जर (unlikely (अचिन्हित (end - record) < size)) परत असत्य; \
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
  जर (decoded->flags & (अचिन्हित) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    जर (unlikely (अचिन्हित (end - record) < HBGlyphID24::static_size)) परत असत्य;
    hb_barrier ();
    decoded->gid = * (स्थिरांक HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  अन्यथा
  {
    decoded->gid_size = HBGlyphID16::static_size;
    जर (unlikely (अचिन्हित (end - record) < HBGlyphID16::static_size)) परत असत्य;
    hb_barrier ();
    decoded->gid = * (स्थिरांक HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  जर (decoded->flags & (अचिन्हित) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  जर (decoded->flags & (अचिन्हित) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    अचिन्हित axis_मोजणी;
    जर (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      जर (unlikely (axis_indices->in_error ())) परत असत्य;
      axis_मोजणी = axis_indices->length;
    }
    अन्यथा
      axis_मोजणी = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    जर (unlikely (budget && axis_मोजणी &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_मोजणी)))
      परत असत्य;
    जर (axis_मूल्ये)
    {
      जर (unlikely (!axis_मूल्ये->resize (axis_मोजणी))) परत असत्य;
      स्थिरांक HBUINT8 *p = (स्थिरांक HBUINT8 *) record;
      जर (unlikely (!Tuple_मूल्ये::decompile (p, *axis_मूल्ये,
					    (स्थिरांक HBUINT8 *) end)))
	परत असत्य;
      record = (स्थिरांक अचिन्हित अक्षर *) p;
    }
    अन्यथा जर (unlikely (!Tuple_मूल्ये::skip (record, end, axis_मोजणी)))
      परत असत्य;
  }

  decoded->axis_मूल्ये_var_idx = VarIdx::NO_VARIATION;
  जर (decoded->flags & (अचिन्हित) flags_t::AXIS_मूल्ये_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_मूल्ये_var_idx, axis_मूल्ये_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  जर (decoded->flags & (अचिन्हित) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  जर (decoded->flags & (अचिन्हित) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    जर (unlikely (अचिन्हित (end - record) < HBINT16::static_size)) परत असत्य; \
    hb_barrier (); \
    decoded->transform.name = * (स्थिरांक HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  अचिन्हित reserved = decoded->flags & (अचिन्हित) flags_t::RESERVED_MASK;
  पर्यंत (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  परत सत्य;
}

तार्किक
VARC::closure_glyphs (hb_set_t *glyphset) स्थिरांक
{
  जर (unlikely (glyphset->in_error ()))
    परत असत्य;

  hb_set_t pending = *glyphset;
  जर (unlikely (pending.in_error ()))
    परत असत्य;

  पर्यंत (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    अचिन्हित index = (हे+coverage).get_coverage (gid);
    जर (index == NOT_COVERED) सुरूठेवा;

    hb_ubytes_t record = (हे+glyphRecords)[index];
    पर्यंत (record)
    {
      VarComponent::record_t component;
      जर (unlikely (!VarComponent::decompile_record (*हे, record,
						     nullptr, nullptr, &component)))
	परत असत्य;
      जर (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	जर (unlikely (glyphset->in_error ()))
	  परत असत्य;
	pending.add (component.gid);
	जर (unlikely (pending.in_error ()))
	  परत असत्य;
      }
      record = record.sub_array (component.size);
    }
  }
  परत सत्य;
}

शून्य
VARC::depend (hb_depend_data_builder_t *depend_data) स्थिरांक
{
  स्थिरांक CFF2Index &records = हे+glyphRecords;
  साठी (स्वतः _ : + hb_zip (हे+coverage,
			  hb_range ((अचिन्हित) records.मोजणी)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    पर्यंत (record)
    {
	VarComponent::record_t component;
	जर (unlikely (!VarComponent::decompile_record (*हे, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  परत;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

संरचना hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  शून्य *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

स्थिर शून्य
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     शून्य *data,
			     hb_draw_state_t *st,
			     दशांश to_x, दशांश to_y,
			     शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_ओळ_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     शून्य *data,
			     hb_draw_state_t *st,
			     दशांश to_x, दशांश to_y,
			     शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->ओळ_to (c->data, *c->st, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  शून्य *data,
				  hb_draw_state_t *st,
				  दशांश control_x, दशांश control_y,
				  दशांश to_x, दशांश to_y,
				  शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      शून्य *data,
			      hb_draw_state_t *st,
			      दशांश control1_x, दशांश control1_y,
			      दशांश control2_x, दशांश control2_y,
			      दशांश to_x, दशांश to_y,
			      शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				शून्य *data,
				hb_draw_state_t *st,
				शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

स्थिर int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, शून्य *draw_data, शून्य *)
{
  परत ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

स्थिर ओळीत शून्य free_static_transforming_pen_funcs ();

स्थिर संरचना hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func (funcs, hb_transforming_pen_ओळ_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    परत funcs;
  }
} static_transforming_pen_funcs;

स्थिर ओळीत
शून्य free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

स्थिर hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  परत static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (स्थिरांक hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<स्थिरांक पूर्णांक> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) स्थिरांक
{
  जर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    परत hb_ubytes_t ();

  स्थिरांक अचिन्हित अक्षर *end = total_record.arrayZ + total_record.length;
  स्वतः &VARC = *c.font->face->table.VARC->table;
  स्वतः &varStore = &VARC+VARC.varStore;

  स्वतः &axisIndices = c.scratch.axisIndices;
  स्वतः &axis_मूल्ये_2 = c.scratch.axis_मूल्ये_2;
  record_t component;
  जर (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_मूल्ये_2, &component,
				   &c.budget)))
    परत hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  स्थिरांक अचिन्हित अक्षर *record = total_record.arrayZ + component.size;

  // Condition
  तार्किक show = सत्य;
  जर (flags & (अचिन्हित) flags_t::HAVE_CONDITION)
  {
    स्थिरांक स्वतः &condition = (&VARC+VARC.conditionList)[component.condition_index];
    स्वतः instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  जर ((flags & (अचिन्हित) flags_t::AXIS_मूल्ये_HAVE_VARIATION) &&
      show && !axis_मूल्ये_2.in_error ())
  {
    जर (unlikely (!hb_budget_spend (c.budget, axis_मूल्ये_2.length,
				    coords.length)))
      परत hb_ubytes_t ();
    varStore.get_delta (component.axis_मूल्ये_var_idx, coords,
			axis_मूल्ये_2.as_array (), cache);
  }

  स्वतः component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  जर ((flags & (अचिन्हित) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->अंक_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  जर (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    जर (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      परत hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<पूर्णांक> ());
    साठी (अचिन्हित i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<पूर्णांक> (round (axis_मूल्ये_2[i]));
    जर (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    जर (transformVarIdx != VarIdx::NO_VARIATION)
    {
      द्विगुण transform_मूल्ये[9];
      अचिन्हित अंक_Transform_मूल्ये = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जर (flags & (अचिन्हित) flags_t::flag) \
	    transform_मूल्ये[अंक_Transform_मूल्ये++] = (द्विगुण) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      जर (unlikely (!hb_budget_spend (c.budget, अंक_Transform_मूल्ये,
				      coords.length)))
	परत hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_मूल्ये, अंक_Transform_मूल्ये), cache);
      अंक_Transform_मूल्ये = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जर (flags & (अचिन्हित) flags_t::flag) \
	    transform.name = (दशांश) transform_मूल्ये[अंक_Transform_मूल्ये++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जर (shift && (flags & (अचिन्हित) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    जर (!(flags & (अचिन्हित) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    तार्किक same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_डावा--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_डावा++;
  }

  परत hb_ubytes_t (record, end - record);
}

तार्किक
VARC::get_path_at (स्थिरांक hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<स्थिरांक पूर्णांक> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) स्थिरांक
{
  // Don't recurse on the same glyph.
  अचिन्हित idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (हे+coverage).get_coverage (glyph);
  जर (idx == NOT_COVERED)
  {
    जर (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      जर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) परत सत्य;

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

      जर (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) परत सत्य;
#ifndef HB_NO_CFF
      जर (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) परत सत्य;
      जर (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) परत सत्य; // Doesn't have variations
#endif
      परत असत्य;
    }
    अन्यथा जर (c.extents)
    {
      जर (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) परत सत्य;

      hb_glyph_extents_t glyph_extents;
      जर (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      जर (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      जर (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	परत असत्य;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    परत सत्य;
  }

  जर (c.depth_डावा <= 0)
    परत सत्य;

  जर (c.edges_डावा <= 0)
    परत सत्य;
  (c.edges_डावा)--;

  hb_decycler_गाठ_t node (c.decycler);
  जर (unlikely (!node.visit (glyph)))
    परत सत्य;

  hb_ubytes_t record = (हे+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (हे+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  जर (cache != parent_cache)
    (हे+varStore).destroy_cache (cache, &static_cache);

  परत सत्य;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
