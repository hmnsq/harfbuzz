#جے_بنا_تعریف HB_OT_VAR_VARC_CC
#تعریف_کرو HB_OT_VAR_VARC_CC
#جے_تعریف_شدہ HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ضمن "VARC.hh"

#جے_بنا_تعریف HB_NO_VAR_COMPOSITES

#ضمن "../../../hb-draw.hh"
#ضمن "../../../hb-depend-data.hh"
#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gdef-table.hh"

نطاق OT {

//namespace Var {

#تعریف_کرو VARC_PROCESS_TRANSFORM_COMPONENTS \
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
	} HB_STMT_سرا

منطقي
VarComponent::decompile_ریکارڈ (ثابت VARC &varc,
				hb_ubytes_t کل_ریکارڈ,
				hb_سمتیہ_t<بنانشانی> *axis_indices,
				hb_سمتیہ_t<مزدوج> *axis_اقدار,
				ریکارڈ_t *decoded,
				int64_t *budget)
{
  ثابت بنانشانی حرف *شروع = کل_ریکارڈ.arrayZ;
  ثابت بنانشانی حرف *ریکارڈ = شروع;
  ثابت بنانشانی حرف *سرا = شروع + کل_ریکارڈ.لمائی;

  إذا (axis_indices) axis_indices->clear ();
  إذا (axis_اقدار) axis_اقدار->clear ();

#تعریف_کرو READ_UINT32VAR(name) \
  HB_STMT_شروع { \
    إذا (unlikely (بنانشانی (سرا - ریکارڈ) < HBUINT32VAR::min_ناپ)) أرجع خطأ; \
    hb_barrier (); \
    خودکار &varint = * (ثابت HBUINT32VAR *) ریکارڈ; \
    بنانشانی ناپ = varint.get_ناپ (); \
    إذا (unlikely (بنانشانی (سرا - ریکارڈ) < ناپ)) أرجع خطأ; \
    name = (uint32_t) varint; \
    ریکارڈ += ناپ; \
  } HB_STMT_سرا
#تعریف_کرو READ_UINT32_VAR_میدان(name, میدان) \
  HB_STMT_شروع { \
    decoded->میدان##_offset = ریکارڈ - شروع; \
    READ_UINT32VAR (name); \
    decoded->میدان##_size = ریکارڈ - شروع - decoded->میدان##_offset; \
  } HB_STMT_سرا

  READ_UINT32VAR (decoded->flags);

  decoded->gid_وتھ = ریکارڈ - شروع;
  إذا (decoded->flags & (بنانشانی) flags_t::GID_IS_24_ثنائی_ہندسہ)
  {
    decoded->gid_ناپ = HBGlyphID24::static_ناپ;
    إذا (unlikely (بنانشانی (سرا - ریکارڈ) < HBGlyphID24::static_ناپ)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) ریکارڈ;
    ریکارڈ += HBGlyphID24::static_ناپ;
  }
  وإلا
  {
    decoded->gid_ناپ = HBGlyphID16::static_ناپ;
    إذا (unlikely (بنانشانی (سرا - ریکارڈ) < HBGlyphID16::static_ناپ)) أرجع خطأ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) ریکارڈ;
    ریکارڈ += HBGlyphID16::static_ناپ;
  }

  إذا (decoded->flags & (بنانشانی) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_میدان (decoded->condition_اشاریہ, condition);

  إذا (decoded->flags & (بنانشانی) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_میدان (decoded->axis_indices_اشاریہ, axis_indices);
    بنانشانی axis_گنتی;
    إذا (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاریہ]);
      إذا (unlikely (axis_indices->وچ_error ())) أرجع خطأ;
      axis_گنتی = axis_indices->لمائی;
    }
    وإلا
      axis_گنتی = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاریہ]);
    إذا (unlikely (budget && axis_گنتی &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_گنتی)))
      أرجع خطأ;
    إذا (axis_اقدار)
    {
      إذا (unlikely (!axis_اقدار->resize (axis_گنتی))) أرجع خطأ;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) ریکارڈ;
      إذا (unlikely (!Tuple_اقدار::decompile (p, *axis_اقدار,
					    (ثابت HBUINT8 *) سرا)))
	أرجع خطأ;
      ریکارڈ = (ثابت بنانشانی حرف *) p;
    }
    وإلا إذا (unlikely (!Tuple_اقدار::skip (ریکارڈ, سرا, axis_گنتی)))
      أرجع خطأ;
  }

  decoded->axis_اقدار_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بنانشانی) flags_t::AXIS_اقدار_HAVE_VARIATION)
    READ_UINT32_VAR_میدان (decoded->axis_اقدار_var_idx, axis_اقدار_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  إذا (decoded->flags & (بنانشانی) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_میدان (decoded->transform_var_idx, transform_var);

#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(کھسکاؤ, type, flag, name) \
  إذا (decoded->flags & (بنانشانی) flags_t::flag) \
  { \
    static_assert (type::static_ناپ == HBINT16::static_ناپ, ""); \
    إذا (unlikely (بنانشانی (سرا - ریکارڈ) < HBINT16::static_ناپ)) أرجع خطأ; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) ریکارڈ; \
    ریکارڈ += HBINT16::static_ناپ; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_رد PROCESS_TRANSFORM_COMPONENT

  بنانشانی reserved = decoded->flags & (بنانشانی) flags_t::RESERVED_نقاب;
  طالما (reserved)
  {
    uint32_t discard HB_بن_ورتیا;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->ناپ = ریکارڈ - شروع;
#تعریف_رد READ_UINT32_VAR_میدان
#تعریف_رد READ_UINT32VAR
  أرجع سچ;
}

منطقي
VARC::گھیرا_glyphs (hb_سیٹ_t *glyphset) ثابت
{
  إذا (unlikely (glyphset->وچ_error ()))
    أرجع خطأ;

  hb_سیٹ_t pending = *glyphset;
  إذا (unlikely (pending.وچ_error ()))
    أرجع خطأ;

  طالما (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    بنانشانی اشاریہ_2 = (هذا+coverage).get_coverage (gid);
    إذا (اشاریہ_2 == NOT_COVERED) استمر;

    hb_ubytes_t ریکارڈ = (هذا+glyph_ریکارڈاں)[اشاریہ_2];
    طالما (ریکارڈ)
    {
      VarComponent::ریکارڈ_t component;
      إذا (unlikely (!VarComponent::decompile_ریکارڈ (*هذا, ریکارڈ,
						     خالی_سوچک, خالی_سوچک, &component)))
	أرجع خطأ;
      إذا (!glyphset->has (component.gid))
      {
	glyphset->جوڑو (component.gid);
	إذا (unlikely (glyphset->وچ_error ()))
	  أرجع خطأ;
	pending.جوڑو (component.gid);
	إذا (unlikely (pending.وچ_error ()))
	  أرجع خطأ;
      }
      ریکارڈ = ریکارڈ.sub_array (component.ناپ);
    }
  }
  أرجع سچ;
}

فراغ
VARC::depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
{
  ثابت CFF2_اشاریہ &ریکارڈاں = هذا+glyph_ریکارڈاں;
  لكل (خودکار _ : + hb_zip (هذا+coverage,
			  hb_range ((بنانشانی) ریکارڈاں.گنتی)))
  {
    hb_codepoint_t gid = _.پہلا;
    hb_ubytes_t ریکارڈ = ریکارڈاں[_.second];
    طالما (ریکارڈ)
    {
	VarComponent::ریکارڈ_t component;
	إذا (unlikely (!VarComponent::decompile_ریکارڈ (*هذا, ریکارڈ,
						       خالی_سوچک, خالی_سوچک, &component)))
	{
	  depend_انکڑے->fail ();
	  أرجع;
	}
      depend_انکڑے->جوڑو_depend (gid, جدول_ٹیگ, component.gid);
      ریکارڈ = ریکارڈ.sub_array (component.ناپ);
    }
  }
}


#جے_بنا_تعریف HB_NO_DRAW

بنية hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  فراغ *انکڑے;
  hb_draw_state_t *st;
  int64_t *budget_بچیا;
};

ساكن فراغ
hb_transforming_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			     فراغ *انکڑے,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) انکڑے;

  c->transform.transform_نقطہ (منزل_x, منزل_y);

  c->dfuncs->move_منزل (c->انکڑے, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			     فراغ *انکڑے,
			     hb_draw_state_t *st,
			     عائم منزل_x, عائم منزل_y,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) انکڑے;

  c->transform.transform_نقطہ (منزل_x, منزل_y);

  c->dfuncs->سطر_منزل (c->انکڑے, *c->st, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				  فراغ *انکڑے,
				  hb_draw_state_t *st,
				  عائم control_x, عائم control_y,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) انکڑے;

  c->transform.transform_نقطہ (control_x, control_y);
  c->transform.transform_نقطہ (منزل_x, منزل_y);

  c->dfuncs->quadratic_منزل (c->انکڑے, *c->st, control_x, control_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			      فراغ *انکڑے,
			      hb_draw_state_t *st,
			      عائم control1_x, عائم control1_y,
			      عائم control2_x, عائم control2_y,
			      عائم منزل_x, عائم منزل_y,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) انکڑے;

  c->transform.transform_نقطہ (control1_x, control1_y);
  c->transform.transform_نقطہ (control2_x, control2_y);
  c->transform.transform_نقطہ (منزل_x, منزل_y);

  c->dfuncs->cubic_منزل (c->انکڑے, *c->st, control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_transforming_pen_close_رستہ (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				فراغ *انکڑے,
				hb_draw_state_t *st,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) انکڑے;

  c->dfuncs->close_رستہ (c->انکڑے, *c->st);
}

ساكن int64_t *
hb_transforming_pen_get_budget_بچیا (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_transforming_pen_context_t *) draw_انکڑے)->budget_بچیا;
}

ساكن مضمن فراغ free_static_transforming_pen_funcs ();

ساكن بنية hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_منزل_func (funcs, hb_transforming_pen_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, hb_transforming_pen_سطر_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, hb_transforming_pen_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, hb_transforming_pen_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func (funcs, hb_transforming_pen_close_رستہ, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_transforming_pen_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

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
VarComponent::get_رستہ_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t ماپا_gid,
			   hb_array_t<ثابت صحيح> coords,
			   hb_transform_t<> کل_transform,
			   hb_ubytes_t کل_ریکارڈ,
			   hb_scalar_cache_t *cache) ثابت
{
  إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    أرجع hb_ubytes_t ();

  ثابت بنانشانی حرف *سرا = کل_ریکارڈ.arrayZ + کل_ریکارڈ.لمائی;
  خودکار &VARC = *c.font->face->جدول.VARC->جدول;
  خودکار &varStore = &VARC+VARC.varStore;

  خودکار &axisIndices = c.scratch.axisIndices;
  خودکار &axis_اقدار_2 = c.scratch.axis_اقدار_2;
  ریکارڈ_t component;
  إذا (unlikely (!decompile_ریکارڈ (VARC, کل_ریکارڈ,
				   &axisIndices, &axis_اقدار_2, &component,
				   &c.budget)))
    أرجع hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت بنانشانی حرف *ریکارڈ = کل_ریکارڈ.arrayZ + component.ناپ;

  // Condition
  منطقي show = سچ;
  إذا (flags & (بنانشانی) flags_t::HAVE_CONDITION)
  {
    ثابت خودکار &condition = (&VARC+VARC.conditionList)[component.condition_اشاریہ];
    خودکار instancer = Multi_شے_Var_Store_Instancer(&varStore, خالی_سوچک, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.لمائی, &instancer);
  }

  // Axis values

  // Apply variations if any
  إذا ((flags & (بنانشانی) flags_t::AXIS_اقدار_HAVE_VARIATION) &&
      show && !axis_اقدار_2.وچ_error ())
  {
    إذا (unlikely (!hb_budget_spend (c.budget, axis_اقدار_2.لمائی,
				    coords.لمائی)))
      أرجع hb_ubytes_t ();
    varStore.get_delta (component.axis_اقدار_var_idx, coords,
			axis_اقدار_2.as_array (), cache);
  }

  خودکار component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  إذا ((flags & (بنانشانی) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.لمائی > HB_VAR_مرکب_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->عدد_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  إذا (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    إذا (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.لمائی)))
      أرجع hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحيح> ());
    لكل (بنانشانی i = 0; i < axisIndices.لمائی; i++)
      coord_setter[axisIndices[i]] = hb_clamp_منزل<صحيح> (round (axis_اقدار_2[i]));
    إذا (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    إذا (transformVarIdx != VarIdx::NO_VARIATION)
    {
      مزدوج transform_اقدار[9];
      بنانشانی عدد_Transform_اقدار = 0;
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(کھسکاؤ, type, flag, name) \
	  إذا (flags & (بنانشانی) flags_t::flag) \
	    transform_اقدار[عدد_Transform_اقدار++] = (مزدوج) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_رد PROCESS_TRANSFORM_COMPONENT
      إذا (unlikely (!hb_budget_spend (c.budget, عدد_Transform_اقدار,
				      coords.لمائی)))
	أرجع hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_اقدار, عدد_Transform_اقدار), cache);
      عدد_Transform_اقدار = 0;
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(کھسکاؤ, type, flag, name) \
	  إذا (flags & (بنانشانی) flags_t::flag) \
	    transform.name = (عائم) transform_اقدار[عدد_Transform_اقدار++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_رد PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(کھسکاؤ, type, flag, name) \
	  إذا (کھسکاؤ && (flags & (بنانشانی) flags_t::flag)) \
	     transform.name *= 1.f / (1 << کھسکاؤ);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_رد PROCESS_TRANSFORM_COMPONENT

    إذا (!(flags & (بنانشانی) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    کل_transform.transform (transform.منزل_transform ());

    منطقي same_coords = component_coords.لمائی == coords.لمائی &&
		       component_coords.arrayZ == coords.arrayZ;

    c.ڈونگھائی_بایاں--;
    VARC.get_رستہ_at (c, gid,
		      component_coords, کل_transform,
		      ماپا_gid,
		      same_coords ? cache : خالی_سوچک);
    c.ڈونگھائی_بایاں++;
  }

  أرجع hb_ubytes_t (ریکارڈ, سرا - ریکارڈ);
}

منطقي
VARC::get_رستہ_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحيح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t ماپا_glyph,
		   hb_scalar_cache_t *ماپا_cache) ثابت
{
  // Don't recurse on the same glyph.
  بنانشانی idx = glyph == ماپا_glyph ?
		 NOT_COVERED :
		 (هذا+coverage).get_coverage (glyph);
  إذا (idx == NOT_COVERED)
  {
    إذا (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع سچ;

      hb_transform_t<> پتی_transform = transform;
      پتی_transform.x0 *= c.font->x_multf;
      پتی_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {پتی_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_انکڑے,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = پتی_transform.is_پچھان_تت () ? *c.draw_session : transformer_session;

      إذا (c.font->face->جدول.glyf->get_رستہ_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, خالی_سوچک, &shape_draw_session.get_budget ())) أرجع سچ;
#جے_بنا_تعریف HB_NO_CFF
      إذا (c.font->face->جدول.cff2->get_رستہ_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) أرجع سچ;
      إذا (c.font->face->جدول.cff1->get_رستہ (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) أرجع سچ; // Doesn't have variations
#جے_انت
      أرجع خطأ;
    }
    وإلا إذا (c.extents)
    {
      إذا (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) أرجع سچ;

      hb_glyph_extents_t glyph_extents;
      إذا (!c.font->face->جدول.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#جے_بنا_تعریف HB_NO_CFF
      إذا (!c.font->face->جدول.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      إذا (!c.font->face->جدول.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#جے_انت
	أرجع خطأ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> پتی_transform = transform;
      پتی_transform.x0 *= c.font->x_multf;
      پتی_transform.y0 *= c.font->y_multf;
      پتی_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    أرجع سچ;
  }

  إذا (c.ڈونگھائی_بایاں <= 0)
    أرجع سچ;

  إذا (c.کنارے_بایاں <= 0)
    أرجع سچ;
  (c.کنارے_بایاں)--;

  hb_decycler_گرہ_t node (c.decycler);
  إذا (unlikely (!node.ویکھو (glyph)))
    أرجع سچ;

  hb_ubytes_t ریکارڈ = (هذا+glyph_ریکارڈاں)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = ماپا_cache ?
				  ماپا_cache :
				  (هذا+varStore).create_cache (&static_cache);

  Var_مرکب_Glyph::get_رستہ_at (c,
				  glyph,
				  coords, transform,
				  ریکارڈ,
				  cache);

  إذا (cache != ماپا_cache)
    (هذا+varStore).destroy_cache (cache, &static_cache);

  أرجع سچ;
}

#جے_انت

#تعریف_رد VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#جے_انت

#جے_انت /* HB_OT_VAR_VARC_CC (pacify clang) */
#جے_انت /* HB_OT_VAR_VARC_CC */
