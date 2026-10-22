#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_VARC_CC
#وصف_ڪريو HB_OT_VAR_VARC_CC
#جيڪڏهن_وصف_ٿيل HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ضمن "VARC.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES

#ضمن "../../../hb-draw.hh"
#ضمن "../../../hb-depend-data.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gdef-table.hh"

نطاق OT {

//namespace Var {

#وصف_ڪريو VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_شروعات { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_پڇاڙي

منطقي
VarComponent::decompile_رڪارڊ (ثابت VARC &varc,
				hb_ubytes_t ڪل_رڪارڊ,
				hb_ويڪٽر_t<بنانشان> *axis_indices,
				hb_ويڪٽر_t<مزدوج> *axis_قدرون,
				رڪارڊ_t *decoded,
				int64_t *budget)
{
  ثابت بنانشان حرف *شروعات = ڪل_رڪارڊ.arrayZ;
  ثابت بنانشان حرف *رڪارڊ = شروعات;
  ثابت بنانشان حرف *پڇاڙي = شروعات + ڪل_رڪارڊ.ڊيگهائي;

  إذا (axis_indices) axis_indices->clear ();
  إذا (axis_قدرون) axis_قدرون->clear ();

#وصف_ڪريو READ_UINT32VAR(name) \
  HB_STMT_شروعات { \
    إذا (unlikely (بنانشان (پڇاڙي - رڪارڊ) < HBUINT32VAR::min_سائيز)) أرجع خطأ; \
    hb_barrier (); \
    خودڪار &varint = * (ثابت HBUINT32VAR *) رڪارڊ; \
    بنانشان سائيز = varint.get_سائيز (); \
    إذا (unlikely (بنانشان (پڇاڙي - رڪارڊ) < سائيز)) أرجع خطأ; \
    name = (uint32_t) varint; \
    رڪارڊ += سائيز; \
  } HB_STMT_پڇاڙي
#وصف_ڪريو READ_UINT32_VAR_ميدان(name, ميدان) \
  HB_STMT_شروعات { \
    decoded->ميدان##_offset = رڪارڊ - شروعات; \
    READ_UINT32VAR (name); \
    decoded->ميدان##_size = رڪارڊ - شروعات - decoded->ميدان##_offset; \
  } HB_STMT_پڇاڙي

  READ_UINT32VAR (decoded->flags);

  decoded->gid_وٿي = رڪارڊ - شروعات;
  إذا (decoded->flags & (بنانشان) flags_t::GID_IS_24_ٻٽو_انگ)
  {
    decoded->gid_سائيز = HBGlyphID24::static_سائيز;
    إذا (unlikely (بنانشان (پڇاڙي - رڪارڊ) < HBGlyphID24::static_سائيز)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) رڪارڊ;
    رڪارڊ += HBGlyphID24::static_سائيز;
  }
  وإلا
  {
    decoded->gid_سائيز = HBGlyphID16::static_سائيز;
    إذا (unlikely (بنانشان (پڇاڙي - رڪارڊ) < HBGlyphID16::static_سائيز)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) رڪارڊ;
    رڪارڊ += HBGlyphID16::static_سائيز;
  }

  إذا (decoded->flags & (بنانشان) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_ميدان (decoded->condition_اشاريو, condition);

  إذا (decoded->flags & (بنانشان) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_ميدان (decoded->axis_indices_اشاريو, axis_indices);
    بنانشان axis_ڳڻپ;
    إذا (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاريو]);
      إذا (unlikely (axis_indices->اندريون_error ())) أرجع خطأ;
      axis_ڳڻپ = axis_indices->ڊيگهائي;
    }
    وإلا
      axis_ڳڻپ = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاريو]);
    إذا (unlikely (budget && axis_ڳڻپ &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_ڳڻپ)))
      أرجع خطأ;
    إذا (axis_قدرون)
    {
      إذا (unlikely (!axis_قدرون->resize (axis_ڳڻپ))) أرجع خطأ;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) رڪارڊ;
      إذا (unlikely (!Tuple_قدرون::decompile (p, *axis_قدرون,
					    (ثابت HBUINT8 *) پڇاڙي)))
	أرجع خطأ;
      رڪارڊ = (ثابت بنانشان حرف *) p;
    }
    وإلا إذا (unlikely (!Tuple_قدرون::skip (رڪارڊ, پڇاڙي, axis_ڳڻپ)))
      أرجع خطأ;
  }

  decoded->axis_قدرون_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بنانشان) flags_t::AXIS_قدرون_HAVE_VARIATION)
    READ_UINT32_VAR_ميدان (decoded->axis_قدرون_var_idx, axis_قدرون_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بنانشان) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_ميدان (decoded->transform_var_idx, transform_var);

#وصف_ڪريو PROCESS_TRANSFORM_COMPONENT(سرڪايو, type, flag, name) \
  إذا (decoded->flags & (بنانشان) flags_t::flag) \
  { \
    static_assert (type::static_سائيز == HBINT16::static_سائيز, ""); \
    إذا (unlikely (بنانشان (پڇاڙي - رڪارڊ) < HBINT16::static_سائيز)) أرجع خطأ; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) رڪارڊ; \
    رڪارڊ += HBINT16::static_سائيز; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#وصف_رد PROCESS_TRANSFORM_COMPONENT

  بنانشان reserved = decoded->flags & (بنانشان) flags_t::RESERVED_نقاب;
  طالما (reserved)
  {
    uint32_t discard HB_اڻ_استعمال_ٿيل;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->سائيز = رڪارڊ - شروعات;
#وصف_رد READ_UINT32_VAR_ميدان
#وصف_رد READ_UINT32VAR
  أرجع سچ;
}

منطقي
VARC::گهيرو_glyphs (hb_سيٽ_t *glyphset) ثابت
{
  إذا (unlikely (glyphset->اندريون_error ()))
    أرجع خطأ;

  hb_سيٽ_t pending = *glyphset;
  إذا (unlikely (pending.اندريون_error ()))
    أرجع خطأ;

  طالما (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    بنانشان اشاريو_2 = (هذا+coverage).get_coverage (gid);
    إذا (اشاريو_2 == NOT_COVERED) استمر;

    hb_ubytes_t رڪارڊ = (هذا+glyph_رڪارڊون)[اشاريو_2];
    طالما (رڪارڊ)
    {
      VarComponent::رڪارڊ_t component;
      إذا (unlikely (!VarComponent::decompile_رڪارڊ (*هذا, رڪارڊ,
						     خالي_اشارو, خالي_اشارو, &component)))
	أرجع خطأ;
      إذا (!glyphset->has (component.gid))
      {
	glyphset->جوڙيو (component.gid);
	إذا (unlikely (glyphset->اندريون_error ()))
	  أرجع خطأ;
	pending.جوڙيو (component.gid);
	إذا (unlikely (pending.اندريون_error ()))
	  أرجع خطأ;
      }
      رڪارڊ = رڪارڊ.sub_array (component.سائيز);
    }
  }
  أرجع سچ;
}

فراغ
VARC::depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
{
  ثابت CFF2_اشاريو &رڪارڊون = هذا+glyph_رڪارڊون;
  لكل (خودڪار _ : + hb_zip (هذا+coverage,
			  hb_range ((بنانشان) رڪارڊون.ڳڻپ)))
  {
    hb_codepoint_t gid = _.پهريون;
    hb_ubytes_t رڪارڊ = رڪارڊون[_.second];
    طالما (رڪارڊ)
    {
	VarComponent::رڪارڊ_t component;
	إذا (unlikely (!VarComponent::decompile_رڪارڊ (*هذا, رڪارڊ,
						       خالي_اشارو, خالي_اشارو, &component)))
	{
	  depend_معلومات->fail ();
	  أرجع;
	}
      depend_معلومات->جوڙيو_depend (gid, جدول_ٽيگ, component.gid);
      رڪارڊ = رڪارڊ.sub_array (component.سائيز);
    }
  }
}


#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW

بنية hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  فراغ *معلومات;
  hb_draw_state_t *st;
  int64_t *budget_بچيل;
};

ساكن فراغ
hb_transforming_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
			     فراغ *معلومات,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_نقطو (منزل_x, منزل_y);

  c->dfuncs->move_منزل (c->معلومات, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_سٽ_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
			     فراغ *معلومات,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_نقطو (منزل_x, منزل_y);

  c->dfuncs->سٽ_منزل (c->معلومات, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم control_x, عائم control_y,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_نقطو (control_x, control_y);
  c->transform.transform_نقطو (منزل_x, منزل_y);

  c->dfuncs->quadratic_منزل (c->معلومات, *c->st, control_x, control_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
			      فراغ *معلومات,
			      hb_draw_state_t *st,
			      عائم control1_x, عائم control1_y,
			      عائم control2_x, عائم control2_y,
			      عائم منزل_x, عائم منزل_y,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->transform.transform_نقطو (control1_x, control1_y);
  c->transform.transform_نقطو (control2_x, control2_y);
  c->transform.transform_نقطو (منزل_x, منزل_y);

  c->dfuncs->cubic_منزل (c->معلومات, *c->st, control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_close_رستو (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				فراغ *معلومات,
				hb_draw_state_t *st,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) معلومات;

  c->dfuncs->close_رستو (c->معلومات, *c->st);
}

ساكن int64_t *
hb_transforming_pen_get_budget_بچيل (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_transforming_pen_context_t *) draw_معلومات)->budget_بچيل;
}

ساكن مضمن فراغ free_static_transforming_pen_funcs ();

ساكن بنية hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سيٽ_move_منزل_func (funcs, hb_transforming_pen_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, hb_transforming_pen_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, hb_transforming_pen_quadratic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, hb_transforming_pen_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, hb_transforming_pen_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_transforming_pen_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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
VarComponent::get_رستو_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t والد_gid,
			   hb_array_t<ثابت صحيح> coords,
			   hb_transform_t<> ڪل_transform,
			   hb_ubytes_t ڪل_رڪارڊ,
			   hb_scalar_cache_t *cache) ثابت
{
  إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    أرجع hb_ubytes_t ();

  ثابت بنانشان حرف *پڇاڙي = ڪل_رڪارڊ.arrayZ + ڪل_رڪارڊ.ڊيگهائي;
  خودڪار &VARC = *c.font->face->جدول.VARC->جدول;
  خودڪار &varStore = &VARC+VARC.varStore;

  خودڪار &axisIndices = c.scratch.axisIndices;
  خودڪار &axis_قدرون_2 = c.scratch.axis_قدرون_2;
  رڪارڊ_t component;
  إذا (unlikely (!decompile_رڪارڊ (VARC, ڪل_رڪارڊ,
				   &axisIndices, &axis_قدرون_2, &component,
				   &c.budget)))
    أرجع hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت بنانشان حرف *رڪارڊ = ڪل_رڪارڊ.arrayZ + component.سائيز;

  // Condition
  منطقي show = سچ;
  إذا (flags & (بنانشان) flags_t::HAVE_CONDITION)
  {
    ثابت خودڪار &condition = (&VARC+VARC.conditionList)[component.condition_اشاريو];
    خودڪار instancer = Multi_شي_Var_Store_Instancer(&varStore, خالي_اشارو, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.ڊيگهائي, &instancer);
  }

  // Axis values

  // Apply variations if any
  إذا ((flags & (بنانشان) flags_t::AXIS_قدرون_HAVE_VARIATION) &&
      show && !axis_قدرون_2.اندريون_error ())
  {
    إذا (unlikely (!hb_budget_spend (c.budget, axis_قدرون_2.ڊيگهائي,
				    coords.ڊيگهائي)))
      أرجع hb_ubytes_t ();
    varStore.get_delta (component.axis_قدرون_var_idx, coords,
			axis_قدرون_2.as_array (), cache);
  }

  خودڪار component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  إذا ((flags & (بنانشان) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.ڊيگهائي > HB_VAR_مرکب_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->انگ_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  إذا (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    إذا (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.ڊيگهائي)))
      أرجع hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحيح> ());
    لكل (بنانشان i = 0; i < axisIndices.ڊيگهائي; i++)
      coord_setter[axisIndices[i]] = hb_clamp_منزل<صحيح> (round (axis_قدرون_2[i]));
    إذا (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    إذا (transformVarIdx != VarIdx::NO_VARIATION)
    {
      مزدوج transform_قدرون[9];
      بنانشان انگ_Transform_قدرون = 0;
#وصف_ڪريو PROCESS_TRANSFORM_COMPONENT(سرڪايو, type, flag, name) \
	  إذا (flags & (بنانشان) flags_t::flag) \
	    transform_قدرون[انگ_Transform_قدرون++] = (مزدوج) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#وصف_رد PROCESS_TRANSFORM_COMPONENT
      إذا (unlikely (!hb_budget_spend (c.budget, انگ_Transform_قدرون,
				      coords.ڊيگهائي)))
	أرجع hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_قدرون, انگ_Transform_قدرون), cache);
      انگ_Transform_قدرون = 0;
#وصف_ڪريو PROCESS_TRANSFORM_COMPONENT(سرڪايو, type, flag, name) \
	  إذا (flags & (بنانشان) flags_t::flag) \
	    transform.name = (عائم) transform_قدرون[انگ_Transform_قدرون++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#وصف_رد PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#وصف_ڪريو PROCESS_TRANSFORM_COMPONENT(سرڪايو, type, flag, name) \
	  إذا (سرڪايو && (flags & (بنانشان) flags_t::flag)) \
	     transform.name *= 1.f / (1 << سرڪايو);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#وصف_رد PROCESS_TRANSFORM_COMPONENT

    إذا (!(flags & (بنانشان) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    ڪل_transform.transform (transform.منزل_transform ());

    منطقي same_coords = component_coords.ڊيگهائي == coords.ڊيگهائي &&
		       component_coords.arrayZ == coords.arrayZ;

    c.اونهائي_کاٻو--;
    VARC.get_رستو_at (c, gid,
		      component_coords, ڪل_transform,
		      والد_gid,
		      same_coords ? cache : خالي_اشارو);
    c.اونهائي_کاٻو++;
  }

  أرجع hb_ubytes_t (رڪارڊ, پڇاڙي - رڪارڊ);
}

منطقي
VARC::get_رستو_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحيح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t والد_glyph,
		   hb_scalar_cache_t *والد_cache) ثابت
{
  // Don't recurse on the same glyph.
  بنانشان idx = glyph == والد_glyph ?
		 NOT_COVERED :
		 (هذا+coverage).get_coverage (glyph);
  إذا (idx == NOT_COVERED)
  {
    إذا (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع سچ;

      hb_transform_t<> پن_transform = transform;
      پن_transform.x0 *= c.font->x_multf;
      پن_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {پن_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_معلومات,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = پن_transform.is_سڃاڻپ_عنصر () ? *c.draw_session : transformer_session;

      إذا (c.font->face->جدول.glyf->get_رستو_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, خالي_اشارو, &shape_draw_session.get_budget ())) أرجع سچ;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_CFF
      إذا (c.font->face->جدول.cff2->get_رستو_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) أرجع سچ;
      إذا (c.font->face->جدول.cff1->get_رستو (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) أرجع سچ; // Doesn't have variations
#جيڪڏهن_پورو
      أرجع خطأ;
    }
    وإلا إذا (c.extents)
    {
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع سچ;

      hb_glyph_extents_t glyph_extents;
      إذا (!c.font->face->جدول.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#جيڪڏهن_وصف_نه_ٿيل HB_NO_CFF
      إذا (!c.font->face->جدول.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      إذا (!c.font->face->جدول.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#جيڪڏهن_پورو
	أرجع خطأ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> پن_transform = transform;
      پن_transform.x0 *= c.font->x_multf;
      پن_transform.y0 *= c.font->y_multf;
      پن_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    أرجع سچ;
  }

  إذا (c.اونهائي_کاٻو <= 0)
    أرجع سچ;

  إذا (c.ڪنارا_کاٻو <= 0)
    أرجع سچ;
  (c.ڪنارا_کاٻو)--;

  hb_decycler_ڳنڍ_t node (c.decycler);
  إذا (unlikely (!node.گهمو (glyph)))
    أرجع سچ;

  hb_ubytes_t رڪارڊ = (هذا+glyph_رڪارڊون)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = والد_cache ?
				  والد_cache :
				  (هذا+varStore).create_cache (&static_cache);

  Var_مرکب_Glyph::get_رستو_at (c,
				  glyph,
				  coords, transform,
				  رڪارڊ,
				  cache);

  إذا (cache != والد_cache)
    (هذا+varStore).destroy_cache (cache, &static_cache);

  أرجع سچ;
}

#جيڪڏهن_پورو

#وصف_رد VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#جيڪڏهن_پورو

#جيڪڏهن_پورو /* HB_OT_VAR_VARC_CC (pacify clang) */
#جيڪڏهن_پورو /* HB_OT_VAR_VARC_CC */
