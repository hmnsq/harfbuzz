#ifndef HB_OT_VAR_VARC_CC
#define HB_OT_VAR_VARC_CC
#ifdef HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#અન્તર્ભાવ "VARC.hh"

#ifndef HB_NO_VAR_COMPOSITES

#અન્તર્ભાવ "../../../hb-draw.hh"
#અન્તર્ભાવ "../../../hb-depend-data.hh"
#અન્તર્ભાવ "../../../hb-ot-layout-common.hh"
#અન્તર્ભાવ "../../../hb-ot-layout-gdef-table.hh"

નામાકાશ OT {

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

તાર્કિક
VarComponent::decompile_record (અચલ VARC &varc,
				hb_ubytes_t total_record,
				hb_vector_t<unsigned> *axis_indices,
				hb_vector_t<દ્વિગુણ> *axis_કિંમતો,
				record_t *decoded,
				int64_t *budget)
{
  અચલ unsigned અક્ષર *start = total_record.arrayZ;
  અચલ unsigned અક્ષર *record = start;
  અચલ unsigned અક્ષર *end = start + total_record.length;

  જો (axis_indices) axis_indices->clear ();
  જો (axis_કિંમતો) axis_કિંમતો->clear ();

#define READ_UINT32VAR(name) \
  HB_STMT_START { \
    જો (unlikely (unsigned (end - record) < HBUINT32VAR::min_size)) પાછા ખોટું; \
    hb_barrier (); \
    auto &varint = * (અચલ HBUINT32VAR *) record; \
    unsigned size = varint.get_size (); \
    જો (unlikely (unsigned (end - record) < size)) પાછા ખોટું; \
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
  જો (decoded->flags & (unsigned) flags_t::GID_IS_24BIT)
  {
    decoded->gid_size = HBGlyphID24::static_size;
    જો (unlikely (unsigned (end - record) < HBGlyphID24::static_size)) પાછા ખોટું;
    hb_barrier ();
    decoded->gid = * (અચલ HBGlyphID24 *) record;
    record += HBGlyphID24::static_size;
  }
  નહીંતર
  {
    decoded->gid_size = HBGlyphID16::static_size;
    જો (unlikely (unsigned (end - record) < HBGlyphID16::static_size)) પાછા ખોટું;
    hb_barrier ();
    decoded->gid = * (અચલ HBGlyphID16 *) record;
    record += HBGlyphID16::static_size;
  }

  જો (decoded->flags & (unsigned) flags_t::HAVE_CONDITION)
    READ_UINT32VAR_FIELD (decoded->condition_index, condition);

  જો (decoded->flags & (unsigned) flags_t::HAVE_AXES)
  {
    READ_UINT32VAR_FIELD (decoded->axis_indices_index, axis_indices);
    unsigned axis_ગણતરી;
    જો (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
      જો (unlikely (axis_indices->in_error ())) પાછા ખોટું;
      axis_ગણતરી = axis_indices->length;
    }
    નહીંતર
      axis_ગણતરી = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_index]);
    જો (unlikely (budget && axis_ગણતરી &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_ગણતરી)))
      પાછા ખોટું;
    જો (axis_કિંમતો)
    {
      જો (unlikely (!axis_કિંમતો->resize (axis_ગણતરી))) પાછા ખોટું;
      અચલ HBUINT8 *p = (અચલ HBUINT8 *) record;
      જો (unlikely (!Tuple_કિંમતો::decompile (p, *axis_કિંમતો,
					    (અચલ HBUINT8 *) end)))
	પાછા ખોટું;
      record = (અચલ unsigned અક્ષર *) p;
    }
    નહીંતર જો (unlikely (!Tuple_કિંમતો::skip (record, end, axis_ગણતરી)))
      પાછા ખોટું;
  }

  decoded->axis_કિંમતો_var_idx = VarIdx::NO_VARIATION;
  જો (decoded->flags & (unsigned) flags_t::AXIS_કિંમતો_HAVE_VARIATION)
    READ_UINT32VAR_FIELD (decoded->axis_કિંમતો_var_idx, axis_કિંમતો_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  જો (decoded->flags & (unsigned) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32VAR_FIELD (decoded->transform_var_idx, transform_var);

#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
  જો (decoded->flags & (unsigned) flags_t::flag) \
  { \
    static_assert (type::static_size == HBINT16::static_size, ""); \
    જો (unlikely (unsigned (end - record) < HBINT16::static_size)) પાછા ખોટું; \
    hb_barrier (); \
    decoded->transform.name = * (અચલ HBINT16 *) record; \
    record += HBINT16::static_size; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

  unsigned reserved = decoded->flags & (unsigned) flags_t::RESERVED_MASK;
  જ્યારે (reserved)
  {
    uint32_t discard HB_UNUSED;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->size = record - start;
#undef READ_UINT32VAR_FIELD
#undef READ_UINT32VAR
  પાછા સાચું;
}

તાર્કિક
VARC::closure_glyphs (hb_set_t *glyphset) અચલ
{
  જો (unlikely (glyphset->in_error ()))
    પાછા ખોટું;

  hb_set_t pending = *glyphset;
  જો (unlikely (pending.in_error ()))
    પાછા ખોટું;

  જ્યારે (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    unsigned index = (આ+coverage).get_coverage (gid);
    જો (index == NOT_COVERED) ચાલુ;

    hb_ubytes_t record = (આ+glyphRecords)[index];
    જ્યારે (record)
    {
      VarComponent::record_t component;
      જો (unlikely (!VarComponent::decompile_record (*આ, record,
						     nullptr, nullptr, &component)))
	પાછા ખોટું;
      જો (!glyphset->has (component.gid))
      {
	glyphset->add (component.gid);
	જો (unlikely (glyphset->in_error ()))
	  પાછા ખોટું;
	pending.add (component.gid);
	જો (unlikely (pending.in_error ()))
	  પાછા ખોટું;
      }
      record = record.sub_array (component.size);
    }
  }
  પાછા સાચું;
}

શૂન્ય
VARC::depend (hb_depend_data_builder_t *depend_data) અચલ
{
  અચલ CFF2Index &records = આ+glyphRecords;
  માટે (auto _ : + hb_zip (આ+coverage,
			  hb_range ((unsigned) records.ગણતરી)))
  {
    hb_codepoint_t gid = _.first;
    hb_ubytes_t record = records[_.second];
    જ્યારે (record)
    {
	VarComponent::record_t component;
	જો (unlikely (!VarComponent::decompile_record (*આ, record,
						       nullptr, nullptr, &component)))
	{
	  depend_data->fail ();
	  પાછા;
	}
      depend_data->add_depend (gid, tableTag, component.gid);
      record = record.sub_array (component.size);
    }
  }
}


#ifndef HB_NO_DRAW

સંરચના hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  શૂન્ય *data;
  hb_draw_state_t *st;
  int64_t *budget_remaining;
};

સ્થિર શૂન્ય
hb_transforming_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     શૂન્ય *data,
			     hb_draw_state_t *st,
			     દશાંશ to_x, દશાંશ to_y,
			     શૂન્ય *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->move_to (c->data, *c->st, to_x, to_y);
}

સ્થિર શૂન્ય
hb_transforming_pen_લીટી_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     શૂન્ય *data,
			     hb_draw_state_t *st,
			     દશાંશ to_x, દશાંશ to_y,
			     શૂન્ય *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (to_x, to_y);

  c->dfuncs->લીટી_to (c->data, *c->st, to_x, to_y);
}

સ્થિર શૂન્ય
hb_transforming_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  શૂન્ય *data,
				  hb_draw_state_t *st,
				  દશાંશ control_x, દશાંશ control_y,
				  દશાંશ to_x, દશાંશ to_y,
				  શૂન્ય *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control_x, control_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->quadratic_to (c->data, *c->st, control_x, control_y, to_x, to_y);
}

સ્થિર શૂન્ય
hb_transforming_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      શૂન્ય *data,
			      hb_draw_state_t *st,
			      દશાંશ control1_x, દશાંશ control1_y,
			      દશાંશ control2_x, દશાંશ control2_y,
			      દશાંશ to_x, દશાંશ to_y,
			      શૂન્ય *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->transform.transform_point (control1_x, control1_y);
  c->transform.transform_point (control2_x, control2_y);
  c->transform.transform_point (to_x, to_y);

  c->dfuncs->cubic_to (c->data, *c->st, control1_x, control1_y, control2_x, control2_y, to_x, to_y);
}

સ્થિર શૂન્ય
hb_transforming_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				શૂન્ય *data,
				hb_draw_state_t *st,
				શૂન્ય *user_data HB_UNUSED)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) data;

  c->dfuncs->close_path (c->data, *c->st);
}

સ્થિર int64_t *
hb_transforming_pen_get_budget_remaining (hb_draw_funcs_t *, શૂન્ય *draw_data, શૂન્ય *)
{
  પાછા ((hb_transforming_pen_context_t *) draw_data)->budget_remaining;
}

સ્થિર અંતઃરેખ શૂન્ય free_static_transforming_pen_funcs ();

સ્થિર સંરચના hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  સ્થિર hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_transforming_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_લીટી_to_func (funcs, hb_transforming_pen_લીટી_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_transforming_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_transforming_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_transforming_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_transforming_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    પાછા funcs;
  }
} static_transforming_pen_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

સ્થિર hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  પાછા static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_path_at (અચલ hb_varc_context_t &c,
			   hb_codepoint_t parent_gid,
			   hb_array_t<અચલ પૂર્ણાંક> coords,
			   hb_transform_t<> total_transform,
			   hb_ubytes_t total_record,
			   hb_scalar_cache_t *cache) અચલ
{
  જો (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    પાછા hb_ubytes_t ();

  અચલ unsigned અક્ષર *end = total_record.arrayZ + total_record.length;
  auto &VARC = *c.font->face->table.VARC->table;
  auto &varStore = &VARC+VARC.varStore;

  auto &axisIndices = c.scratch.axisIndices;
  auto &axis_કિંમતો_2 = c.scratch.axis_કિંમતો_2;
  record_t component;
  જો (unlikely (!decompile_record (VARC, total_record,
				   &axisIndices, &axis_કિંમતો_2, &component,
				   &c.budget)))
    પાછા hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  અચલ unsigned અક્ષર *record = total_record.arrayZ + component.size;

  // Condition
  તાર્કિક show = સાચું;
  જો (flags & (unsigned) flags_t::HAVE_CONDITION)
  {
    અચલ auto &condition = (&VARC+VARC.conditionList)[component.condition_index];
    auto instancer = MultiItemVarStoreInstancer(&varStore, nullptr, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.length, &instancer);
  }

  // Axis values

  // Apply variations if any
  જો ((flags & (unsigned) flags_t::AXIS_કિંમતો_HAVE_VARIATION) &&
      show && !axis_કિંમતો_2.in_error ())
  {
    જો (unlikely (!hb_budget_spend (c.budget, axis_કિંમતો_2.length,
				    coords.length)))
      પાછા hb_ubytes_t ();
    varStore.get_delta (component.axis_કિંમતો_var_idx, coords,
			axis_કિંમતો_2.as_array (), cache);
  }

  auto component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  જો ((flags & (unsigned) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.length > HB_VAR_COMPOSITE_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->અંક_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  જો (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    જો (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.length)))
      પાછા hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<પૂર્ણાંક> ());
    માટે (unsigned i = 0; i < axisIndices.length; i++)
      coord_setter[axisIndices[i]] = hb_clamp_to<પૂર્ણાંક> (round (axis_કિંમતો_2[i]));
    જો (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    જો (transformVarIdx != VarIdx::NO_VARIATION)
    {
      દ્વિગુણ transform_કિંમતો[9];
      unsigned અંક_Transform_કિંમતો = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  જો (flags & (unsigned) flags_t::flag) \
	    transform_કિંમતો[અંક_Transform_કિંમતો++] = (દ્વિગુણ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
      જો (unlikely (!hb_budget_spend (c.budget, અંક_Transform_કિંમતો,
				      coords.length)))
	પાછા hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_કિંમતો, અંક_Transform_કિંમતો), cache);
      અંક_Transform_કિંમતો = 0;
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  જો (flags & (unsigned) flags_t::flag) \
	    transform.name = (દશાંશ) transform_કિંમતો[અંક_Transform_કિંમતો++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#define PROCESS_TRANSFORM_COMPONENT(shift, type, flag, name) \
	  જો (shift && (flags & (unsigned) flags_t::flag)) \
	     transform.name *= 1.f / (1 << shift);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#undef PROCESS_TRANSFORM_COMPONENT

    જો (!(flags & (unsigned) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    total_transform.transform (transform.to_transform ());

    તાર્કિક same_coords = component_coords.length == coords.length &&
		       component_coords.arrayZ == coords.arrayZ;

    c.depth_ડાબું--;
    VARC.get_path_at (c, gid,
		      component_coords, total_transform,
		      parent_gid,
		      same_coords ? cache : nullptr);
    c.depth_ડાબું++;
  }

  પાછા hb_ubytes_t (record, end - record);
}

તાર્કિક
VARC::get_path_at (અચલ hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<અચલ પૂર્ણાંક> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t parent_glyph,
		   hb_scalar_cache_t *parent_cache) અચલ
{
  // Don't recurse on the same glyph.
  unsigned idx = glyph == parent_glyph ?
		 NOT_COVERED :
		 (આ+coverage).get_coverage (glyph);
  જો (idx == NOT_COVERED)
  {
    જો (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      જો (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) પાછા સાચું;

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

      જો (c.font->face->table.glyf->get_path_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, nullptr, &shape_draw_session.get_budget ())) પાછા સાચું;
#ifndef HB_NO_CFF
      જો (c.font->face->table.cff2->get_path_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) પાછા સાચું;
      જો (c.font->face->table.cff1->get_path (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) પાછા સાચું; // Doesn't have variations
#endif
      પાછા ખોટું;
    }
    નહીંતર જો (c.extents)
    {
      જો (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) પાછા સાચું;

      hb_glyph_extents_t glyph_extents;
      જો (!c.font->face->table.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ifndef HB_NO_CFF
      જો (!c.font->face->table.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      જો (!c.font->face->table.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#endif
	પાછા ખોટું;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> leaf_transform = transform;
      leaf_transform.x0 *= c.font->x_multf;
      leaf_transform.y0 *= c.font->y_multf;
      leaf_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    પાછા સાચું;
  }

  જો (c.depth_ડાબું <= 0)
    પાછા સાચું;

  જો (c.edges_ડાબું <= 0)
    પાછા સાચું;
  (c.edges_ડાબું)--;

  hb_decycler_ગાંઠ_t node (c.decycler);
  જો (unlikely (!node.visit (glyph)))
    પાછા સાચું;

  hb_ubytes_t record = (આ+glyphRecords)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = parent_cache ?
				  parent_cache :
				  (આ+varStore).create_cache (&static_cache);

  VarCompositeGlyph::get_path_at (c,
				  glyph,
				  coords, transform,
				  record,
				  cache);

  જો (cache != parent_cache)
    (આ+varStore).destroy_cache (cache, &static_cache);

  પાછા સાચું;
}

#endif

#undef VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#endif

#endif /* HB_OT_VAR_VARC_CC (pacify clang) */
#endif /* HB_OT_VAR_VARC_CC */
