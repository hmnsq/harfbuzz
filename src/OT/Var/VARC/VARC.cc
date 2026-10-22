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
VarComponent::decompile_record (ध्रुव VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<द्विगुण> *axis_मानानि,
				record_t *decoded,
				int64_t *budget)
{
  ध्रुव unsigned वर्ण *start = total_record.arrayZ;
  ध्रुव unsigned वर्ण *record = start;
  ध्रुव unsigned वर्ण *end = start + total_record.length;

  जइ (axis_indices) axis_indices->clear ();
  जइ (axis_मानानि) axis_मानानि->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    जइ (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) प्रत्यागम मिच्छ; \
    hb_barrier (); \
    auto &varint = * (ध्रुव HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    जइ (unlikely (unsigned (end - record) < size)) प्रत्यागम मिच्छ; \
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
  जइ (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    जइ (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) प्रत्यागम मिच्छ;
    hb_barrier ();
    decoded->gid = * (ध्रुव HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  अण्णहा
  {
    decoded->gid_size = HBGlyphID16::static_size;
    जइ (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) प्रत्यागम मिच्छ;
    hb_barrier ();
    decoded->gid = * (ध्रुव HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  जइ (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  जइ (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_गणनम्;
    जइ (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      जइ (unlikely (axis_indices->in_error ())) प्रत्यागम मिच्छ;
      axis_गणनम् = axis_indices->length;
    }
    अण्णहा
      axis_गणनम् = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    जइ (unlikely (budget && axis_गणनम् &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_गणनम्)))
      प्रत्यागम मिच्छ;
    जइ (axis_मानानि)
    {
      जइ (unlikely (!axis_मानानि->resize (axis_गणनम्))) प्रत्यागम मिच्छ;
      ध्रुव HBUINT8 *p = (ध्रुव HBUINT8 *) record;
      जइ (unlikely (!Tuple_मानानि::decompile (p, *axis_मानानि,
					    (ध्रुव HBUINT8 *) end)))
	प्रत्यागम मिच्छ;
      record = (ध्रुव unsigned वर्ण *) p;
    }
    अण्णहा जइ (unlikely (!Tuple_मानानि::skip (record, end, axis_गणनम्)))
      प्रत्यागम मिच्छ;
  }

  decoded->axis_मानानि_var_idx = VarIdx::NO_VARIATION;
  जइ (decoded->flags & (unsigned) flags_t::AXIS_मानानि_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_मानानि_var_idx, axis_मानानि_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  जइ (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  जइ (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    जइ (unlikely (unsigned (end - record) < HBINT16::static_size)) प्रत्यागम मिच्छ; \
    hb_barrier (); \
    decoded->transform.name = * (ध्रुव HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  जाव (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  प्रत्यागम सच्च;
}

तार्किक
VARC::closure_glyphs (hb_set_t *glyphset) ध्रुव
{
  जइ (unlikely (glyphset->in_error ()))
    प्रत्यागम मिच्छ;

  hb_set_t pending = *glyphset;
  जइ (unlikely (pending.in_error ()))
    प्रत्यागम मिच्छ;

  जाव (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (एतत्+coverage).get_coverage (gid);
    जइ (index == NOT_COVERED) अनुवर्त;

    hb_ubytes_t record = (एतत्+glyphRecords)[index];
    जाव (record)
    {
      VarComponent::record_t component;
      जइ (unlikely (!VarComponent::decompile_record (*एतत्, record,
						     nullptr, nullptr, &component)))
	प्रत्यागम मिच्छ;
      जइ (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	जइ (unlikely (glyphset->in_error ()))
	  प्रत्यागम मिच्छ;
	pending.add (component.gid);
	जइ (unlikely (pending.in_error ()))
	  प्रत्यागम मिच्छ;
      }
      record = record.sub_array (component.size);
    }
  }
  प्रत्यागम सच्च;
}

शून्य
VARC::depend (hb_depend_data_builder_t *depend_data) ध्रुव
{
  ध्रुव CFF2Index &records = एतत्+glyphRecords;
  प्रति (auto _ : + hb_zip (एतत्+coverage,
			  hb_range ((unsigned) records.गणनम्)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    जाव (record)
    {
	VarComponent::record_t component;
	जइ (unlikely (!VarComponent::decompile_record (*एतत्, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  प्रत्यागम;
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
			     भिन्न to_x, भिन्न to_y,
			     शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_पङ्क्तिः_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     शून्य *data,
			     hb_draw_state_t *st,
			     भिन्न to_x, भिन्न to_y,
			     शून्य *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->पङ्क्तिः_to (c->data, *c->st, to_x, to_y);
}

स्थिर शून्य
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  शून्य *data,
				  hb_draw_state_t *st,
				  भिन्न control_x, भिन्न control_y,
				  भिन्न to_x, भिन्न to_y,
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
			      भिन्न control1_x, भिन्न control1_y,
			      भिन्न control2_x, भिन्न control2_y,
			      भिन्न to_x, भिन्न to_y,
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
  प्रत्यागम ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

स्थिर अन्तःस्थ शून्य free_static_transforming_pen_funcs ();

स्थिर संरचना hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पङ्क्तिः_to_func (funcs, hb_transforming_pen_पङ्क्तिः_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    प्रत्यागम funcs;
  }
} static_transforming_pen_funcs;

स्थिर अन्तःस्थ
शून्य free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

स्थिर hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  प्रत्यागम static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (ध्रुव hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<ध्रुव पूर्णाङ्क> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) ध्रुव
{
  जइ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    प्रत्यागम hb_ubytes_t ();

  ध्रुव unsigned वर्ण *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_मानानि_2 = c.scratch.axis_मानानि_2;
  record_t component;
  जइ (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_मानानि_2, &component,
				   &c.budget)))
    प्रत्यागम hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ध्रुव unsigned वर्ण *record = total_record.arrayZ + component.size;

  // Condition
  तार्किक show = सच्च;
  जइ (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    ध्रुव auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  जइ ((flags & (unsigned) flags_t::AXIS_मानानि_HAVE_VARIATION) &&
      show && !axis_मानानि_2.in_error ())
  {
    जइ (unlikely (!hb_budget_spend (c.budget, axis_मानानि_2.length,
				    coords.length)))
      प्रत्यागम hb_ubytes_t ();
    varStore.get_delta (component.axis_मानानि_var_idx, coords,
			axis_मानानि_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  जइ ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->अङ्कः_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  जइ (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    जइ (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      प्रत्यागम hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<पूर्णाङ्क> ());
    प्रति (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<पूर्णाङ्क> (round (axis_मानानि_2[i]));
    जइ (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    जइ (transformVarIdx != VarIdx::NO_VARIATION)
    {
      द्विगुण transform_मानानि[9];
      unsigned अङ्कः_Transform_मानानि = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जइ (flags & (unsigned) flags_t::flag) \
	    transform_मानानि[अङ्कः_Transform_मानानि++] = (द्विगुण) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      जइ (unlikely (!hb_budget_spend (c.budget, अङ्कः_Transform_मानानि,
				      coords.length)))
	प्रत्यागम hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_मानानि, अङ्कः_Transform_मानानि), cache);
      अङ्कः_Transform_मानानि = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जइ (flags & (unsigned) flags_t::flag) \
	    transform.name = (भिन्न) transform_मानानि[अङ्कः_Transform_मानानि++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  जइ (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    जइ (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    तार्किक same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_वामः--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_वामः++;
  }

  प्रत्यागम hb_ubytes_t (record, end - record);
}

तार्किक
VARC::get_path_at (ध्रुव hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ध्रुव पूर्णाङ्क> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) ध्रुव
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (एतत्+coverage).get_coverage (glyph);
  जइ (idx == NOT_COVERED)
  {
    जइ (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      जइ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) प्रत्यागम सच्च;

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

      जइ (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) प्रत्यागम सच्च;
#ifndef HB_NO_CFF
      जइ (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) प्रत्यागम सच्च;
      जइ (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) प्रत्यागम सच्च; // Doesn't have variations
#endif
      प्रत्यागम मिच्छ;
    }
    अण्णहा जइ (c.extents)
    {
      जइ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) प्रत्यागम सच्च;

      hb_glyph_extents_t glyph_extents;
      जइ (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      जइ (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      जइ (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	प्रत्यागम मिच्छ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    प्रत्यागम सच्च;
  }

  जइ (c.depth_वामः <= 0)
    प्रत्यागम सच्च;

  जइ (c.edges_वामः <= 0)
    प्रत्यागम सच्च;
  (c.edges_वामः)--;

  hb_decycler_ग्रन्थिः_t node (c.decycler);
  जइ (unlikely (!node.visit (glyph)))
    प्रत्यागम सच्च;

  hb_ubytes_t record = (एतत्+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (एतत्+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  जइ (cache != parent_cache)
    (एतत्+varStore).destroy_cache (cache, &static_cache);

  प्रत्यागम सच्च;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
