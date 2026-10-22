#که_نه_تعریف_شوی HB_OT_VAR_VARC_CC
#تعریف_کړه HB_OT_VAR_VARC_CC
#که_تعریف_شوی HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ضمن "VARC.hh"

#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES

#ضمن "../../../hb-draw.hh"
#ضمن "../../../hb-depend-data.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gdef-table.hh"

نطاق OT {

//namespace Var {

#تعریف_کړه VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_پیل { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_پای_ټکی

منطقي
VarComponent::decompile_ریکارډ (ثابت VARC &varc,
				hb_ubytes_t ټول_ریکارډ,
				hb_ویکټور_t<بینښه> *axis_indices,
				hb_ویکټور_t<مزدوج> *axis_ارزښتونه,
				ریکارډ_t *decoded,
				int64_t *budget)
{
  ثابت بینښه حرف *پیل = ټول_ریکارډ.arrayZ;
  ثابت بینښه حرف *ریکارډ = پیل;
  ثابت بینښه حرف *پای_ټکی = پیل + ټول_ریکارډ.اوږدتیا;

  إذا (axis_indices) axis_indices->clear ();
  إذا (axis_ارزښتونه) axis_ارزښتونه->clear ();

#تعریف_کړه READ_UINT32VAR(name) \
  HB_STMT_پیل { \
    إذا (unlikely (بینښه (پای_ټکی - ریکارډ) < HBUINT32VAR::min_اندازه)) أرجع خطأ; \
    hb_barrier (); \
    خپلکاری &varint = * (ثابت HBUINT32VAR *) ریکارډ; \
    بینښه اندازه = varint.get_اندازه (); \
    إذا (unlikely (بینښه (پای_ټکی - ریکارډ) < اندازه)) أرجع خطأ; \
    name = (uint32_t) varint; \
    ریکارډ += اندازه; \
  } HB_STMT_پای_ټکی
#تعریف_کړه READ_UINT32_VAR_ډګر(name, ډګر) \
  HB_STMT_پیل { \
    decoded->ډګر##_offset = ریکارډ - پیل; \
    READ_UINT32VAR (name); \
    decoded->ډګر##_size = ریکارډ - پیل - decoded->ډګر##_offset; \
  } HB_STMT_پای_ټکی

  READ_UINT32VAR (decoded->flags);

  decoded->gid_دننه_واټن = ریکارډ - پیل;
  إذا (decoded->flags & (بینښه) flags_t::GID_IS_24_بټ)
  {
    decoded->gid_اندازه = HBGlyphID24::static_اندازه;
    إذا (unlikely (بینښه (پای_ټکی - ریکارډ) < HBGlyphID24::static_اندازه)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) ریکارډ;
    ریکارډ += HBGlyphID24::static_اندازه;
  }
  وإلا
  {
    decoded->gid_اندازه = HBGlyphID16::static_اندازه;
    إذا (unlikely (بینښه (پای_ټکی - ریکارډ) < HBGlyphID16::static_اندازه)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) ریکارډ;
    ریکارډ += HBGlyphID16::static_اندازه;
  }

  إذا (decoded->flags & (بینښه) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_ډګر (decoded->condition_شاخص, condition);

  إذا (decoded->flags & (بینښه) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_ډګر (decoded->axis_indices_شاخص, axis_indices);
    بینښه axis_شمېر;
    إذا (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_شاخص]);
      إذا (unlikely (axis_indices->دننه_error ())) أرجع خطأ;
      axis_شمېر = axis_indices->اوږدتیا;
    }
    وإلا
      axis_شمېر = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_شاخص]);
    إذا (unlikely (budget && axis_شمېر &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_شمېر)))
      أرجع خطأ;
    إذا (axis_ارزښتونه)
    {
      إذا (unlikely (!axis_ارزښتونه->resize (axis_شمېر))) أرجع خطأ;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) ریکارډ;
      إذا (unlikely (!Tuple_ارزښتونه::decompile (p, *axis_ارزښتونه,
					    (ثابت HBUINT8 *) پای_ټکی)))
	أرجع خطأ;
      ریکارډ = (ثابت بینښه حرف *) p;
    }
    وإلا إذا (unlikely (!Tuple_ارزښتونه::skip (ریکارډ, پای_ټکی, axis_شمېر)))
      أرجع خطأ;
  }

  decoded->axis_ارزښتونه_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بینښه) flags_t::AXIS_ارزښتونه_HAVE_VARIATION)
    READ_UINT32_VAR_ډګر (decoded->axis_ارزښتونه_var_idx, axis_ارزښتونه_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بینښه) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_ډګر (decoded->transform_var_idx, transform_var);

#تعریف_کړه PROCESS_TRANSFORM_COMPONENT(ښويدنه, type, flag, name) \
  إذا (decoded->flags & (بینښه) flags_t::flag) \
  { \
    static_assert (type::static_اندازه == HBINT16::static_اندازه, ""); \
    إذا (unlikely (بینښه (پای_ټکی - ریکارډ) < HBINT16::static_اندازه)) أرجع خطأ; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) ریکارډ; \
    ریکارډ += HBINT16::static_اندازه; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_لغوه PROCESS_TRANSFORM_COMPONENT

  بینښه reserved = decoded->flags & (بینښه) flags_t::RESERVED_ماسک;
  طالما (reserved)
  {
    uint32_t discard HB_ناکارول_شوی;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->اندازه = ریکارډ - پیل;
#تعریف_لغوه READ_UINT32_VAR_ډګر
#تعریف_لغوه READ_UINT32VAR
  أرجع ریښتیا;
}

منطقي
VARC::تړون_glyphs (hb_سېټ_t *glyphset) ثابت
{
  إذا (unlikely (glyphset->دننه_error ()))
    أرجع خطأ;

  hb_سېټ_t pending = *glyphset;
  إذا (unlikely (pending.دننه_error ()))
    أرجع خطأ;

  طالما (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    بینښه شاخص_2 = (هذا+coverage).get_coverage (gid);
    إذا (شاخص_2 == NOT_COVERED) استمر;

    hb_ubytes_t ریکارډ = (هذا+glyph_ریکارډونه)[شاخص_2];
    طالما (ریکارډ)
    {
      VarComponent::ریکارډ_t component;
      إذا (unlikely (!VarComponent::decompile_ریکارډ (*هذا, ریکارډ,
						     تش_اشاره, تش_اشاره, &component)))
	أرجع خطأ;
      إذا (!glyphset->has (component.gid))
      {
	glyphset->جمع_کړه (component.gid);
	إذا (unlikely (glyphset->دننه_error ()))
	  أرجع خطأ;
	pending.جمع_کړه (component.gid);
	إذا (unlikely (pending.دننه_error ()))
	  أرجع خطأ;
      }
      ریکارډ = ریکارډ.sub_array (component.اندازه);
    }
  }
  أرجع ریښتیا;
}

فراغ
VARC::depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
{
  ثابت CFF2_شاخص &ریکارډونه = هذا+glyph_ریکارډونه;
  لكل (خپلکاری _ : + hb_zip (هذا+coverage,
			  hb_range ((بینښه) ریکارډونه.شمېر)))
  {
    hb_codepoint_t gid = _.لومړی;
    hb_ubytes_t ریکارډ = ریکارډونه[_.second];
    طالما (ریکارډ)
    {
	VarComponent::ریکارډ_t component;
	إذا (unlikely (!VarComponent::decompile_ریکارډ (*هذا, ریکارډ,
						       تش_اشاره, تش_اشاره, &component)))
	{
	  depend_معلومات->fail ();
	  أرجع;
	}
      depend_معلومات->جمع_کړه_depend (gid, جدول_نښان, component.gid);
      ریکارډ = ریکارډ.sub_array (component.اندازه);
    }
  }
}


#که_نه_تعریف_شوی HB_NO_DRAW

بنية hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  فراغ *معلومات;
  hb_draw_state_t *st;
  int64_t *budget_پاتې_شوی;
};

ساكن فراغ
hb_transforming_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			     فراغ *معلومات,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_ټکی (منزل_x, منزل_y);

  c->dfuncs->move_منزل (c->معلومات, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_کرښه_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			     فراغ *معلومات,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_ټکی (منزل_x, منزل_y);

  c->dfuncs->کرښه_منزل (c->معلومات, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم control_x, عائم control_y,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_ټکی (control_x, control_y);
  c->transform.transform_ټکی (منزل_x, منزل_y);

  c->dfuncs->quadratic_منزل (c->معلومات, *c->st, control_x, control_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			      فراغ *معلومات,
			      hb_draw_state_t *st,
			      عائم control1_x, عائم control1_y,
			      عائم control2_x, عائم control2_y,
			      عائم منزل_x, عائم منزل_y,
			      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_ټکی (control1_x, control1_y);
  c->transform.transform_ټکی (control2_x, control2_y);
  c->transform.transform_ټکی (منزل_x, منزل_y);

  c->dfuncs->cubic_منزل (c->معلومات, *c->st, control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_close_لار (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				فراغ *معلومات,
				hb_draw_state_t *st,
				فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->dfuncs->close_لار (c->معلومات, *c->st);
}

ساكن int64_t *
hb_transforming_pen_get_budget_پاتې_شوی (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_transforming_pen_context_t *) draw_معلومات)->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_transforming_pen_funcs ();

ساكن بنية hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سېټ_move_منزل_func (funcs, hb_transforming_pen_move_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, hb_transforming_pen_کرښه_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, hb_transforming_pen_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func (funcs, hb_transforming_pen_cubic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func (funcs, hb_transforming_pen_close_لار, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_transforming_pen_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    أرجع funcs;
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
  أرجع static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_لار_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t پلار_gid,
			   hb_array_t<ثابت صحيح> coords,
			   hb_transform_t<> ټول_transform,
			   hb_ubytes_t ټول_ریکارډ,
			   hb_scalar_cache_t *cache) ثابت
{
  إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    أرجع hb_ubytes_t ();

  ثابت بینښه حرف *پای_ټکی = ټول_ریکارډ.arrayZ + ټول_ریکارډ.اوږدتیا;
  خپلکاری &VARC = *c.font->face->جدول.VARC->جدول;
  خپلکاری &varStore = &VARC+VARC.varStore;

  خپلکاری &axisIndices = c.scratch.axisIndices;
  خپلکاری &axis_ارزښتونه_2 = c.scratch.axis_ارزښتونه_2;
  ریکارډ_t component;
  إذا (unlikely (!decompile_ریکارډ (VARC, ټول_ریکارډ,
				   &axisIndices, &axis_ارزښتونه_2, &component,
				   &c.budget)))
    أرجع hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت بینښه حرف *ریکارډ = ټول_ریکارډ.arrayZ + component.اندازه;

  // Condition
  منطقي show = ریښتیا;
  إذا (flags & (بینښه) flags_t::HAVE_CONDITION)
  {
    ثابت خپلکاری &condition = (&VARC+VARC.conditionList)[component.condition_شاخص];
    خپلکاری instancer = Multi_توکی_Var_Store_Instancer(&varStore, تش_اشاره, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.اوږدتیا, &instancer);
  }

  // Axis values

  // Apply variations if any
  إذا ((flags & (بینښه) flags_t::AXIS_ارزښتونه_HAVE_VARIATION) &&
      show && !axis_ارزښتونه_2.دننه_error ())
  {
    إذا (unlikely (!hb_budget_spend (c.budget, axis_ارزښتونه_2.اوږدتیا,
				    coords.اوږدتیا)))
      أرجع hb_ubytes_t ();
    varStore.get_delta (component.axis_ارزښتونه_var_idx, coords,
			axis_ارزښتونه_2.as_array (), cache);
  }

  خپلکاری component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  إذا ((flags & (بینښه) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.اوږدتیا > HB_VAR_مرکب_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->عدد_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  إذا (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    إذا (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.اوږدتیا)))
      أرجع hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحيح> ());
    لكل (بینښه i = 0; i < axisIndices.اوږدتیا; i++)
      coord_setter[axisIndices[i]] = hb_clamp_منزل<صحيح> (round (axis_ارزښتونه_2[i]));
    إذا (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    إذا (transformVarIdx != VarIdx::NO_VARIATION)
    {
      مزدوج transform_ارزښتونه[9];
      بینښه عدد_Transform_ارزښتونه = 0;
#تعریف_کړه PROCESS_TRANSFORM_COMPONENT(ښويدنه, type, flag, name) \
	  إذا (flags & (بینښه) flags_t::flag) \
	    transform_ارزښتونه[عدد_Transform_ارزښتونه++] = (مزدوج) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_لغوه PROCESS_TRANSFORM_COMPONENT
      إذا (unlikely (!hb_budget_spend (c.budget, عدد_Transform_ارزښتونه,
				      coords.اوږدتیا)))
	أرجع hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_ارزښتونه, عدد_Transform_ارزښتونه), cache);
      عدد_Transform_ارزښتونه = 0;
#تعریف_کړه PROCESS_TRANSFORM_COMPONENT(ښويدنه, type, flag, name) \
	  إذا (flags & (بینښه) flags_t::flag) \
	    transform.name = (عائم) transform_ارزښتونه[عدد_Transform_ارزښتونه++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_لغوه PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#تعریف_کړه PROCESS_TRANSFORM_COMPONENT(ښويدنه, type, flag, name) \
	  إذا (ښويدنه && (flags & (بینښه) flags_t::flag)) \
	     transform.name *= 1.f / (1 << ښويدنه);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_لغوه PROCESS_TRANSFORM_COMPONENT

    إذا (!(flags & (بینښه) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    ټول_transform.transform (transform.منزل_transform ());

    منطقي same_coords = component_coords.اوږدتیا == coords.اوږدتیا &&
		       component_coords.arrayZ == coords.arrayZ;

    c.ژوروالی_کیڼ--;
    VARC.get_لار_at (c, gid,
		      component_coords, ټول_transform,
		      پلار_gid,
		      same_coords ? cache : تش_اشاره);
    c.ژوروالی_کیڼ++;
  }

  أرجع hb_ubytes_t (ریکارډ, پای_ټکی - ریکارډ);
}

منطقي
VARC::get_لار_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحيح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t پلار_glyph,
		   hb_scalar_cache_t *پلار_cache) ثابت
{
  // Don't recurse on the same glyph.
  بینښه idx = glyph == پلار_glyph ?
		 NOT_COVERED :
		 (هذا+coverage).get_coverage (glyph);
  إذا (idx == NOT_COVERED)
  {
    إذا (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع ریښتیا;

      hb_transform_t<> ونې_پاڼه_transform = transform;
      ونې_پاڼه_transform.x0 *= c.font->x_multf;
      ونې_پاڼه_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {ونې_پاڼه_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_معلومات,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = ونې_پاڼه_transform.is_بې_اغېزه_عنصر () ? *c.draw_session : transformer_session;

      إذا (c.font->face->جدول.glyf->get_لار_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, تش_اشاره, &shape_draw_session.get_budget ())) أرجع ریښتیا;
#که_نه_تعریف_شوی HB_NO_CFF
      إذا (c.font->face->جدول.cff2->get_لار_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) أرجع ریښتیا;
      إذا (c.font->face->جدول.cff1->get_لار (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) أرجع ریښتیا; // Doesn't have variations
#د_که_پای
      أرجع خطأ;
    }
    وإلا إذا (c.extents)
    {
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع ریښتیا;

      hb_glyph_extents_t glyph_extents;
      إذا (!c.font->face->جدول.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#که_نه_تعریف_شوی HB_NO_CFF
      إذا (!c.font->face->جدول.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      إذا (!c.font->face->جدول.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#د_که_پای
	أرجع خطأ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> ونې_پاڼه_transform = transform;
      ونې_پاڼه_transform.x0 *= c.font->x_multf;
      ونې_پاڼه_transform.y0 *= c.font->y_multf;
      ونې_پاڼه_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    أرجع ریښتیا;
  }

  إذا (c.ژوروالی_کیڼ <= 0)
    أرجع ریښتیا;

  إذا (c.څنډې_کیڼ <= 0)
    أرجع ریښتیا;
  (c.څنډې_کیڼ)--;

  hb_decycler_غوټه_t node (c.decycler);
  إذا (unlikely (!node.لیدنه (glyph)))
    أرجع ریښتیا;

  hb_ubytes_t ریکارډ = (هذا+glyph_ریکارډونه)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = پلار_cache ?
				  پلار_cache :
				  (هذا+varStore).create_cache (&static_cache);

  Var_مرکب_Glyph::get_لار_at (c,
				  glyph,
				  coords, transform,
				  ریکارډ,
				  cache);

  إذا (cache != پلار_cache)
    (هذا+varStore).destroy_cache (cache, &static_cache);

  أرجع ریښتیا;
}

#د_که_پای

#تعریف_لغوه VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#د_که_پای

#د_که_پای /* HB_OT_VAR_VARC_CC (pacify clang) */
#د_که_پای /* HB_OT_VAR_VARC_CC */
