#اگر_تعریف_نشده HB_OT_VAR_VARC_CC
#تعریف_کن HB_OT_VAR_VARC_CC
#اگر_تعریف_شده HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ضمن "VARC.hh"

#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES

#ضمن "../../../hb-draw.hh"
#ضمن "../../../hb-depend-data.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gdef-table.hh"

نطاق OT {

//namespace Var {

#تعریف_کن VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_شروع { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_انتها

منطقي
VarComponent::decompile_رکورد (ثابت VARC &varc,
				hb_ubytes_t مجموع_رکورد,
				hb_وکتور_t<بیعلامت> *axis_indices,
				hb_وکتور_t<مزدوج> *axis_مقادیر,
				رکورد_t *decoded,
				int64_t *budget)
{
  ثابت بیعلامت حرف *شروع = مجموع_رکورد.arrayZ;
  ثابت بیعلامت حرف *رکورد = شروع;
  ثابت بیعلامت حرف *انتها = شروع + مجموع_رکورد.درازی;

  إذا (axis_indices) axis_indices->clear ();
  إذا (axis_مقادیر) axis_مقادیر->clear ();

#تعریف_کن READ_UINT32VAR(name) \
  HB_STMT_شروع { \
    إذا (unlikely (بیعلامت (انتها - رکورد) < HBUINT32VAR::min_اندازه)) أرجع خطأ; \
    hb_barrier (); \
    خودکار &varint = * (ثابت HBUINT32VAR *) رکورد; \
    بیعلامت اندازه = varint.get_اندازه (); \
    إذا (unlikely (بیعلامت (انتها - رکورد) < اندازه)) أرجع خطأ; \
    name = (uint32_t) varint; \
    رکورد += اندازه; \
  } HB_STMT_انتها
#تعریف_کن READ_UINT32_VAR_فیلد(name, فیلد) \
  HB_STMT_شروع { \
    decoded->فیلد##_offset = رکورد - شروع; \
    READ_UINT32VAR (name); \
    decoded->فیلد##_size = رکورد - شروع - decoded->فیلد##_offset; \
  } HB_STMT_انتها

  READ_UINT32VAR (decoded->flags);

  decoded->gid_جابه_جایی = رکورد - شروع;
  إذا (decoded->flags & (بیعلامت) flags_t::GID_IS_24_بیت)
  {
    decoded->gid_اندازه = HBGlyphID24::static_اندازه;
    إذا (unlikely (بیعلامت (انتها - رکورد) < HBGlyphID24::static_اندازه)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) رکورد;
    رکورد += HBGlyphID24::static_اندازه;
  }
  وإلا
  {
    decoded->gid_اندازه = HBGlyphID16::static_اندازه;
    إذا (unlikely (بیعلامت (انتها - رکورد) < HBGlyphID16::static_اندازه)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) رکورد;
    رکورد += HBGlyphID16::static_اندازه;
  }

  إذا (decoded->flags & (بیعلامت) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_فیلد (decoded->condition_شاخص, condition);

  إذا (decoded->flags & (بیعلامت) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_فیلد (decoded->axis_indices_شاخص, axis_indices);
    بیعلامت axis_شمار;
    إذا (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_شاخص]);
      إذا (unlikely (axis_indices->درون_error ())) أرجع خطأ;
      axis_شمار = axis_indices->درازی;
    }
    وإلا
      axis_شمار = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_شاخص]);
    إذا (unlikely (budget && axis_شمار &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_شمار)))
      أرجع خطأ;
    إذا (axis_مقادیر)
    {
      إذا (unlikely (!axis_مقادیر->resize (axis_شمار))) أرجع خطأ;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) رکورد;
      إذا (unlikely (!Tuple_مقادیر::decompile (p, *axis_مقادیر,
					    (ثابت HBUINT8 *) انتها)))
	أرجع خطأ;
      رکورد = (ثابت بیعلامت حرف *) p;
    }
    وإلا إذا (unlikely (!Tuple_مقادیر::skip (رکورد, انتها, axis_شمار)))
      أرجع خطأ;
  }

  decoded->axis_مقادیر_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بیعلامت) flags_t::AXIS_مقادیر_HAVE_VARIATION)
    READ_UINT32_VAR_فیلد (decoded->axis_مقادیر_var_idx, axis_مقادیر_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بیعلامت) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_فیلد (decoded->transform_var_idx, transform_var);

#تعریف_کن PROCESS_TRANSFORM_COMPONENT(لغزش, type, flag, name) \
  إذا (decoded->flags & (بیعلامت) flags_t::flag) \
  { \
    static_assert (type::static_اندازه == HBINT16::static_اندازه, ""); \
    إذا (unlikely (بیعلامت (انتها - رکورد) < HBINT16::static_اندازه)) أرجع خطأ; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) رکورد; \
    رکورد += HBINT16::static_اندازه; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#لغو_تعریف PROCESS_TRANSFORM_COMPONENT

  بیعلامت reserved = decoded->flags & (بیعلامت) flags_t::RESERVED_ماسک;
  طالما (reserved)
  {
    uint32_t discard HB_استفادهنشده;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->اندازه = رکورد - شروع;
#لغو_تعریف READ_UINT32_VAR_فیلد
#لغو_تعریف READ_UINT32VAR
  أرجع درست;
}

منطقي
VARC::بستگی_glyphs (hb_مجموعه_t *glyphset) ثابت
{
  إذا (unlikely (glyphset->درون_error ()))
    أرجع خطأ;

  hb_مجموعه_t pending = *glyphset;
  إذا (unlikely (pending.درون_error ()))
    أرجع خطأ;

  طالما (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    بیعلامت شاخص_2 = (هذا+coverage).get_coverage (gid);
    إذا (شاخص_2 == NOT_COVERED) استمر;

    hb_ubytes_t رکورد = (هذا+glyph_رکوردها)[شاخص_2];
    طالما (رکورد)
    {
      VarComponent::رکورد_t component;
      إذا (unlikely (!VarComponent::decompile_رکورد (*هذا, رکورد,
						     اشاره_تهی, اشاره_تهی, &component)))
	أرجع خطأ;
      إذا (!glyphset->has (component.gid))
      {
	glyphset->جمع_کن (component.gid);
	إذا (unlikely (glyphset->درون_error ()))
	  أرجع خطأ;
	pending.جمع_کن (component.gid);
	إذا (unlikely (pending.درون_error ()))
	  أرجع خطأ;
      }
      رکورد = رکورد.sub_array (component.اندازه);
    }
  }
  أرجع درست;
}

فراغ
VARC::depend (hb_depend_داده_builder_t *depend_داده) ثابت
{
  ثابت CFF2_شاخص &رکوردها = هذا+glyph_رکوردها;
  لكل (خودکار _ : + hb_zip (هذا+coverage,
			  hb_range ((بیعلامت) رکوردها.شمار)))
  {
    hb_codepoint_t gid = _.اولی;
    hb_ubytes_t رکورد = رکوردها[_.second];
    طالما (رکورد)
    {
	VarComponent::رکورد_t component;
	إذا (unlikely (!VarComponent::decompile_رکورد (*هذا, رکورد,
						       اشاره_تهی, اشاره_تهی, &component)))
	{
	  depend_داده->fail ();
	  أرجع;
	}
      depend_داده->جمع_کن_depend (gid, جدول_برچسب, component.gid);
      رکورد = رکورد.sub_array (component.اندازه);
    }
  }
}


#اگر_تعریف_نشده HB_NO_DRAW

بنية hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  فراغ *داده;
  hb_draw_state_t *st;
  int64_t *budget_مانده;
};

ساكن فراغ
hb_transforming_pen_move_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			     فراغ *داده,
			     hb_draw_state_t *st,
			     عائم مقصد_x, عائم مقصد_y,
			     فراغ *user_داده HB_استفادهنشده)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) داده;

  c->transform.transform_نقطه (مقصد_x, مقصد_y);

  c->dfuncs->move_مقصد (c->داده, *c->st, مقصد_x, مقصد_y);
}

ساكن فراغ
hb_transforming_pen_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			     فراغ *داده,
			     hb_draw_state_t *st,
			     عائم مقصد_x, عائم مقصد_y,
			     فراغ *user_داده HB_استفادهنشده)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) داده;

  c->transform.transform_نقطه (مقصد_x, مقصد_y);

  c->dfuncs->سطر_مقصد (c->داده, *c->st, مقصد_x, مقصد_y);
}

ساكن فراغ
hb_transforming_pen_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				  فراغ *داده,
				  hb_draw_state_t *st,
				  عائم control_x, عائم control_y,
				  عائم مقصد_x, عائم مقصد_y,
				  فراغ *user_داده HB_استفادهنشده)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) داده;

  c->transform.transform_نقطه (control_x, control_y);
  c->transform.transform_نقطه (مقصد_x, مقصد_y);

  c->dfuncs->quadratic_مقصد (c->داده, *c->st, control_x, control_y, مقصد_x, مقصد_y);
}

ساكن فراغ
hb_transforming_pen_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			      فراغ *داده,
			      hb_draw_state_t *st,
			      عائم control1_x, عائم control1_y,
			      عائم control2_x, عائم control2_y,
			      عائم مقصد_x, عائم مقصد_y,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) داده;

  c->transform.transform_نقطه (control1_x, control1_y);
  c->transform.transform_نقطه (control2_x, control2_y);
  c->transform.transform_نقطه (مقصد_x, مقصد_y);

  c->dfuncs->cubic_مقصد (c->داده, *c->st, control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y);
}

ساكن فراغ
hb_transforming_pen_close_مسیر (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				فراغ *داده,
				hb_draw_state_t *st,
				فراغ *user_داده HB_استفادهنشده)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) داده;

  c->dfuncs->close_مسیر (c->داده, *c->st);
}

ساكن int64_t *
hb_transforming_pen_get_budget_مانده (hb_draw_funcs_t *, فراغ *draw_داده, فراغ *)
{
  أرجع ((hb_transforming_pen_context_t *) draw_داده)->budget_مانده;
}

ساكن مضمن فراغ free_static_transforming_pen_funcs ();

ساكن بنية hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, hb_transforming_pen_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, hb_transforming_pen_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, hb_transforming_pen_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, hb_transforming_pen_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, hb_transforming_pen_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_transforming_pen_get_budget_مانده, اشاره_تهی, اشاره_تهی);

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
VarComponent::get_مسیر_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t والد_gid,
			   hb_array_t<ثابت صحيح> coords,
			   hb_transform_t<> مجموع_transform,
			   hb_ubytes_t مجموع_رکورد,
			   hb_scalar_cache_t *cache) ثابت
{
  إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    أرجع hb_ubytes_t ();

  ثابت بیعلامت حرف *انتها = مجموع_رکورد.arrayZ + مجموع_رکورد.درازی;
  خودکار &VARC = *c.font->face->جدول.VARC->جدول;
  خودکار &varStore = &VARC+VARC.varStore;

  خودکار &axisIndices = c.scratch.axisIndices;
  خودکار &axis_مقادیر_2 = c.scratch.axis_مقادیر_2;
  رکورد_t component;
  إذا (unlikely (!decompile_رکورد (VARC, مجموع_رکورد,
				   &axisIndices, &axis_مقادیر_2, &component,
				   &c.budget)))
    أرجع hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت بیعلامت حرف *رکورد = مجموع_رکورد.arrayZ + component.اندازه;

  // Condition
  منطقي show = درست;
  إذا (flags & (بیعلامت) flags_t::HAVE_CONDITION)
  {
    ثابت خودکار &condition = (&VARC+VARC.conditionList)[component.condition_شاخص];
    خودکار instancer = Multi_قلم_Var_Store_Instancer(&varStore, اشاره_تهی, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.درازی, &instancer);
  }

  // Axis values

  // Apply variations if any
  إذا ((flags & (بیعلامت) flags_t::AXIS_مقادیر_HAVE_VARIATION) &&
      show && !axis_مقادیر_2.درون_error ())
  {
    إذا (unlikely (!hb_budget_spend (c.budget, axis_مقادیر_2.درازی,
				    coords.درازی)))
      أرجع hb_ubytes_t ();
    varStore.get_delta (component.axis_مقادیر_var_idx, coords,
			axis_مقادیر_2.as_array (), cache);
  }

  خودکار component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  إذا ((flags & (بیعلامت) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.درازی > HB_VAR_مرکب_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->رقم_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  إذا (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    إذا (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.درازی)))
      أرجع hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحيح> ());
    لكل (بیعلامت i = 0; i < axisIndices.درازی; i++)
      coord_setter[axisIndices[i]] = hb_clamp_مقصد<صحيح> (round (axis_مقادیر_2[i]));
    إذا (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    إذا (transformVarIdx != VarIdx::NO_VARIATION)
    {
      مزدوج transform_مقادیر[9];
      بیعلامت رقم_Transform_مقادیر = 0;
#تعریف_کن PROCESS_TRANSFORM_COMPONENT(لغزش, type, flag, name) \
	  إذا (flags & (بیعلامت) flags_t::flag) \
	    transform_مقادیر[رقم_Transform_مقادیر++] = (مزدوج) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#لغو_تعریف PROCESS_TRANSFORM_COMPONENT
      إذا (unlikely (!hb_budget_spend (c.budget, رقم_Transform_مقادیر,
				      coords.درازی)))
	أرجع hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_مقادیر, رقم_Transform_مقادیر), cache);
      رقم_Transform_مقادیر = 0;
#تعریف_کن PROCESS_TRANSFORM_COMPONENT(لغزش, type, flag, name) \
	  إذا (flags & (بیعلامت) flags_t::flag) \
	    transform.name = (عائم) transform_مقادیر[رقم_Transform_مقادیر++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#لغو_تعریف PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#تعریف_کن PROCESS_TRANSFORM_COMPONENT(لغزش, type, flag, name) \
	  إذا (لغزش && (flags & (بیعلامت) flags_t::flag)) \
	     transform.name *= 1.f / (1 << لغزش);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#لغو_تعریف PROCESS_TRANSFORM_COMPONENT

    إذا (!(flags & (بیعلامت) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    مجموع_transform.transform (transform.مقصد_transform ());

    منطقي same_coords = component_coords.درازی == coords.درازی &&
		       component_coords.arrayZ == coords.arrayZ;

    c.عمق_چپ--;
    VARC.get_مسیر_at (c, gid,
		      component_coords, مجموع_transform,
		      والد_gid,
		      same_coords ? cache : اشاره_تهی);
    c.عمق_چپ++;
  }

  أرجع hb_ubytes_t (رکورد, انتها - رکورد);
}

منطقي
VARC::get_مسیر_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحيح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t والد_glyph,
		   hb_scalar_cache_t *والد_cache) ثابت
{
  // Don't recurse on the same glyph.
  بیعلامت idx = glyph == والد_glyph ?
		 NOT_COVERED :
		 (هذا+coverage).get_coverage (glyph);
  إذا (idx == NOT_COVERED)
  {
    إذا (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع درست;

      hb_transform_t<> برگ_transform = transform;
      برگ_transform.x0 *= c.font->x_multf;
      برگ_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {برگ_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_داده,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = برگ_transform.is_عنصر_خنثی () ? *c.draw_session : transformer_session;

      إذا (c.font->face->جدول.glyf->get_مسیر_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, اشاره_تهی, &shape_draw_session.get_budget ())) أرجع درست;
#اگر_تعریف_نشده HB_NO_CFF
      إذا (c.font->face->جدول.cff2->get_مسیر_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) أرجع درست;
      إذا (c.font->face->جدول.cff1->get_مسیر (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) أرجع درست; // Doesn't have variations
#پایان_اگر
      أرجع خطأ;
    }
    وإلا إذا (c.extents)
    {
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع درست;

      hb_glyph_extents_t glyph_extents;
      إذا (!c.font->face->جدول.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#اگر_تعریف_نشده HB_NO_CFF
      إذا (!c.font->face->جدول.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      إذا (!c.font->face->جدول.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#پایان_اگر
	أرجع خطأ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> برگ_transform = transform;
      برگ_transform.x0 *= c.font->x_multf;
      برگ_transform.y0 *= c.font->y_multf;
      برگ_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    أرجع درست;
  }

  إذا (c.عمق_چپ <= 0)
    أرجع درست;

  إذا (c.یالها_چپ <= 0)
    أرجع درست;
  (c.یالها_چپ)--;

  hb_decycler_گره_t node (c.decycler);
  إذا (unlikely (!node.بازدید (glyph)))
    أرجع درست;

  hb_ubytes_t رکورد = (هذا+glyph_رکوردها)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = والد_cache ?
				  والد_cache :
				  (هذا+varStore).create_cache (&static_cache);

  Var_مرکب_Glyph::get_مسیر_at (c,
				  glyph,
				  coords, transform,
				  رکورد,
				  cache);

  إذا (cache != والد_cache)
    (هذا+varStore).destroy_cache (cache, &static_cache);

  أرجع درست;
}

#پایان_اگر

#لغو_تعریف VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#پایان_اگر

#پایان_اگر /* HB_OT_VAR_VARC_CC (pacify clang) */
#پایان_اگر /* HB_OT_VAR_VARC_CC */
