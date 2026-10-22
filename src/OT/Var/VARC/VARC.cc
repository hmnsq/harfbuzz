#اگر_غیر_تعریف_شدہ HB_OT_VAR_VARC_CC
#تعریف_کرو HB_OT_VAR_VARC_CC
#اگر_تعریف_شدہ HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#شامل "VARC.hh"

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES

#شامل "../../../hb-draw.hh"
#شامل "../../../hb-depend-data.hh"
#شامل "../../../hb-ot-layout-common.hh"
#شامل "../../../hb-ot-layout-gdef-table.hh"

نامخلا OT {

//namespace Var {

#تعریف_کرو VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_آغاز { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_انتہا

منطقی
VarComponent::decompile_ریکارڈ (ثابت VARC &varc,
				hb_ubytes_t کل_ریکارڈ,
				hb_سمتیہ_t<غیرعلامتی> *axis_indices,
				hb_سمتیہ_t<دوہرا> *axis_اقدار,
				ریکارڈ_t *decoded,
				int64_t *budget)
{
  ثابت غیرعلامتی حرف *آغاز = کل_ریکارڈ.arrayZ;
  ثابت غیرعلامتی حرف *ریکارڈ = آغاز;
  ثابت غیرعلامتی حرف *انتہا = آغاز + کل_ریکارڈ.لمبائی;

  اگر (axis_indices) axis_indices->clear ();
  اگر (axis_اقدار) axis_اقدار->clear ();

#تعریف_کرو READ_UINT32VAR(name) \
  HB_STMT_آغاز { \
    اگر (unlikely (غیرعلامتی (انتہا - ریکارڈ) < HBUINT32VAR::min_سائز)) واپس جھوٹ; \
    hb_barrier (); \
    خودکار &varint = * (ثابت HBUINT32VAR *) ریکارڈ; \
    غیرعلامتی سائز = varint.get_سائز (); \
    اگر (unlikely (غیرعلامتی (انتہا - ریکارڈ) < سائز)) واپس جھوٹ; \
    name = (uint32_t) varint; \
    ریکارڈ += سائز; \
  } HB_STMT_انتہا
#تعریف_کرو READ_UINT32_VAR_میدان(name, میدان) \
  HB_STMT_آغاز { \
    decoded->میدان##_offset = ریکارڈ - آغاز; \
    READ_UINT32VAR (name); \
    decoded->میدان##_size = ریکارڈ - آغاز - decoded->میدان##_offset; \
  } HB_STMT_انتہا

  READ_UINT32VAR (decoded->flags);

  decoded->gid_انحراف = ریکارڈ - آغاز;
  اگر (decoded->flags & (غیرعلامتی) flags_t::GID_IS_24_ثنائی_ہندسہ)
  {
    decoded->gid_سائز = HBGlyphID24::static_سائز;
    اگر (unlikely (غیرعلامتی (انتہا - ریکارڈ) < HBGlyphID24::static_سائز)) واپس جھوٹ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID24 *) ریکارڈ;
    ریکارڈ += HBGlyphID24::static_سائز;
  }
  ورنہ
  {
    decoded->gid_سائز = HBGlyphID16::static_سائز;
    اگر (unlikely (غیرعلامتی (انتہا - ریکارڈ) < HBGlyphID16::static_سائز)) واپس جھوٹ;
    hb_barrier ();
    decoded->gid = * (ثابت HBGlyphID16 *) ریکارڈ;
    ریکارڈ += HBGlyphID16::static_سائز;
  }

  اگر (decoded->flags & (غیرعلامتی) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_میدان (decoded->condition_اشاریہ, condition);

  اگر (decoded->flags & (غیرعلامتی) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_میدان (decoded->axis_indices_اشاریہ, axis_indices);
    غیرعلامتی axis_گنتی;
    اگر (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاریہ]);
      اگر (unlikely (axis_indices->اندر_error ())) واپس جھوٹ;
      axis_گنتی = axis_indices->لمبائی;
    }
    ورنہ
      axis_گنتی = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_اشاریہ]);
    اگر (unlikely (budget && axis_گنتی &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_گنتی)))
      واپس جھوٹ;
    اگر (axis_اقدار)
    {
      اگر (unlikely (!axis_اقدار->resize (axis_گنتی))) واپس جھوٹ;
      ثابت HBUINT8 *p = (ثابت HBUINT8 *) ریکارڈ;
      اگر (unlikely (!Tuple_اقدار::decompile (p, *axis_اقدار,
					    (ثابت HBUINT8 *) انتہا)))
	واپس جھوٹ;
      ریکارڈ = (ثابت غیرعلامتی حرف *) p;
    }
    ورنہ اگر (unlikely (!Tuple_اقدار::skip (ریکارڈ, انتہا, axis_گنتی)))
      واپس جھوٹ;
  }

  decoded->axis_اقدار_var_idx = VarIdx::NO_VARIATION;
  اگر (decoded->flags & (غیرعلامتی) flags_t::AXIS_اقدار_HAVE_VARIATION)
    READ_UINT32_VAR_میدان (decoded->axis_اقدار_var_idx, axis_اقدار_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  اگر (decoded->flags & (غیرعلامتی) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_میدان (decoded->transform_var_idx, transform_var);

#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(سرکاؤ, type, flag, name) \
  اگر (decoded->flags & (غیرعلامتی) flags_t::flag) \
  { \
    static_assert (type::static_سائز == HBINT16::static_سائز, ""); \
    اگر (unlikely (غیرعلامتی (انتہا - ریکارڈ) < HBINT16::static_سائز)) واپس جھوٹ; \
    hb_barrier (); \
    decoded->transform.name = * (ثابت HBINT16 *) ریکارڈ; \
    ریکارڈ += HBINT16::static_سائز; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_منسوخ PROCESS_TRANSFORM_COMPONENT

  غیرعلامتی reserved = decoded->flags & (غیرعلامتی) flags_t::RESERVED_نقاب;
  جب (reserved)
  {
    uint32_t discard HB_غیر_مستعمل;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->سائز = ریکارڈ - آغاز;
#تعریف_منسوخ READ_UINT32_VAR_میدان
#تعریف_منسوخ READ_UINT32VAR
  واپس سچ;
}

منطقی
VARC::احاطہ_glyphs (hb_سیٹ_t *glyphset) ثابت
{
  اگر (unlikely (glyphset->اندر_error ()))
    واپس جھوٹ;

  hb_سیٹ_t pending = *glyphset;
  اگر (unlikely (pending.اندر_error ()))
    واپس جھوٹ;

  جب (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    غیرعلامتی اشاریہ_2 = (یہ+coverage).get_coverage (gid);
    اگر (اشاریہ_2 == NOT_COVERED) جاری;

    hb_ubytes_t ریکارڈ = (یہ+glyph_اندراجات)[اشاریہ_2];
    جب (ریکارڈ)
    {
      VarComponent::ریکارڈ_t component;
      اگر (unlikely (!VarComponent::decompile_ریکارڈ (*یہ, ریکارڈ,
						     خالی_اشاریہ, خالی_اشاریہ, &component)))
	واپس جھوٹ;
      اگر (!glyphset->has (component.gid))
      {
	glyphset->جوڑو (component.gid);
	اگر (unlikely (glyphset->اندر_error ()))
	  واپس جھوٹ;
	pending.جوڑو (component.gid);
	اگر (unlikely (pending.اندر_error ()))
	  واپس جھوٹ;
      }
      ریکارڈ = ریکارڈ.sub_array (component.سائز);
    }
  }
  واپس سچ;
}

خالی
VARC::depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
{
  ثابت CFF2_اشاریہ &اندراجات = یہ+glyph_اندراجات;
  برائے (خودکار _ : + hb_zip (یہ+coverage,
			  hb_range ((غیرعلامتی) اندراجات.گنتی)))
  {
    hb_codepoint_t gid = _.پہلا;
    hb_ubytes_t ریکارڈ = اندراجات[_.second];
    جب (ریکارڈ)
    {
	VarComponent::ریکارڈ_t component;
	اگر (unlikely (!VarComponent::decompile_ریکارڈ (*یہ, ریکارڈ,
						       خالی_اشاریہ, خالی_اشاریہ, &component)))
	{
	  depend_ڈیٹا->fail ();
	  واپس;
	}
      depend_ڈیٹا->جوڑو_depend (gid, جدول_لیبل, component.gid);
      ریکارڈ = ریکارڈ.sub_array (component.سائز);
    }
  }
}


#اگر_غیر_تعریف_شدہ HB_NO_DRAW

ساخت hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  خالی *ڈیٹا;
  hb_draw_state_t *st;
  int64_t *budget_بقایا;
};

ساکن خالی
hb_transforming_pen_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			     خالی *ڈیٹا,
			     hb_draw_state_t *st,
			     اعشاری مقصد_x, اعشاری مقصد_y,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ڈیٹا;

  c->transform.transform_نقطہ (مقصد_x, مقصد_y);

  c->dfuncs->move_مقصد (c->ڈیٹا, *c->st, مقصد_x, مقصد_y);
}

ساکن خالی
hb_transforming_pen_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			     خالی *ڈیٹا,
			     hb_draw_state_t *st,
			     اعشاری مقصد_x, اعشاری مقصد_y,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ڈیٹا;

  c->transform.transform_نقطہ (مقصد_x, مقصد_y);

  c->dfuncs->سطر_مقصد (c->ڈیٹا, *c->st, مقصد_x, مقصد_y);
}

ساکن خالی
hb_transforming_pen_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				  خالی *ڈیٹا,
				  hb_draw_state_t *st,
				  اعشاری control_x, اعشاری control_y,
				  اعشاری مقصد_x, اعشاری مقصد_y,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ڈیٹا;

  c->transform.transform_نقطہ (control_x, control_y);
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);

  c->dfuncs->quadratic_مقصد (c->ڈیٹا, *c->st, control_x, control_y, مقصد_x, مقصد_y);
}

ساکن خالی
hb_transforming_pen_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			      خالی *ڈیٹا,
			      hb_draw_state_t *st,
			      اعشاری control1_x, اعشاری control1_y,
			      اعشاری control2_x, اعشاری control2_y,
			      اعشاری مقصد_x, اعشاری مقصد_y,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ڈیٹا;

  c->transform.transform_نقطہ (control1_x, control1_y);
  c->transform.transform_نقطہ (control2_x, control2_y);
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);

  c->dfuncs->cubic_مقصد (c->ڈیٹا, *c->st, control1_x, control1_y, control2_x, control2_y, مقصد_x, مقصد_y);
}

ساکن خالی
hb_transforming_pen_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				خالی *ڈیٹا,
				hb_draw_state_t *st,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ڈیٹا;

  c->dfuncs->close_راستہ (c->ڈیٹا, *c->st);
}

ساکن int64_t *
hb_transforming_pen_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_transforming_pen_context_t *) draw_ڈیٹا)->budget_بقایا;
}

ساکن اندرونی خالی free_static_transforming_pen_funcs ();

ساکن ساخت hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, hb_transforming_pen_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, hb_transforming_pen_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_transforming_pen_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, hb_transforming_pen_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, hb_transforming_pen_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_transforming_pen_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    واپس funcs;
  }
} static_transforming_pen_funcs;

ساکن اندرونی
خالی free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

ساکن hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  واپس static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_راستہ_at (ثابت hb_varc_context_t &c,
			   hb_codepoint_t والد_gid,
			   hb_array_t<ثابت صحیح> coords,
			   hb_transform_t<> کل_transform,
			   hb_ubytes_t کل_ریکارڈ,
			   hb_scalar_cache_t *cache) ثابت
{
  اگر (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    واپس hb_ubytes_t ();

  ثابت غیرعلامتی حرف *انتہا = کل_ریکارڈ.arrayZ + کل_ریکارڈ.لمبائی;
  خودکار &VARC = *c.font->face->جدول.VARC->جدول;
  خودکار &varStore = &VARC+VARC.varStore;

  خودکار &axisIndices = c.scratch.axisIndices;
  خودکار &axis_اقدار_2 = c.scratch.axis_اقدار_2;
  ریکارڈ_t component;
  اگر (unlikely (!decompile_ریکارڈ (VARC, کل_ریکارڈ,
				   &axisIndices, &axis_اقدار_2, &component,
				   &c.budget)))
    واپس hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ثابت غیرعلامتی حرف *ریکارڈ = کل_ریکارڈ.arrayZ + component.سائز;

  // Condition
  منطقی show = سچ;
  اگر (flags & (غیرعلامتی) flags_t::HAVE_CONDITION)
  {
    ثابت خودکار &condition = (&VARC+VARC.conditionList)[component.condition_اشاریہ];
    خودکار instancer = Multi_شے_Var_Store_Instancer(&varStore, خالی_اشاریہ, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.لمبائی, &instancer);
  }

  // Axis values

  // Apply variations if any
  اگر ((flags & (غیرعلامتی) flags_t::AXIS_اقدار_HAVE_VARIATION) &&
      show && !axis_اقدار_2.اندر_error ())
  {
    اگر (unlikely (!hb_budget_spend (c.budget, axis_اقدار_2.لمبائی,
				    coords.لمبائی)))
      واپس hb_ubytes_t ();
    varStore.get_delta (component.axis_اقدار_var_idx, coords,
			axis_اقدار_2.as_array (), cache);
  }

  خودکار component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  اگر ((flags & (غیرعلامتی) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.لمبائی > HB_VAR_مرکب_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->عدد_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  اگر (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    اگر (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.لمبائی)))
      واپس hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<صحیح> ());
    برائے (غیرعلامتی i = 0; i < axisIndices.لمبائی; i++)
      coord_setter[axisIndices[i]] = hb_clamp_مقصد<صحیح> (round (axis_اقدار_2[i]));
    اگر (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    اگر (transformVarIdx != VarIdx::NO_VARIATION)
    {
      دوہرا transform_اقدار[9];
      غیرعلامتی عدد_Transform_اقدار = 0;
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(سرکاؤ, type, flag, name) \
	  اگر (flags & (غیرعلامتی) flags_t::flag) \
	    transform_اقدار[عدد_Transform_اقدار++] = (دوہرا) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_منسوخ PROCESS_TRANSFORM_COMPONENT
      اگر (unlikely (!hb_budget_spend (c.budget, عدد_Transform_اقدار,
				      coords.لمبائی)))
	واپس hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_اقدار, عدد_Transform_اقدار), cache);
      عدد_Transform_اقدار = 0;
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(سرکاؤ, type, flag, name) \
	  اگر (flags & (غیرعلامتی) flags_t::flag) \
	    transform.name = (اعشاری) transform_اقدار[عدد_Transform_اقدار++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_منسوخ PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#تعریف_کرو PROCESS_TRANSFORM_COMPONENT(سرکاؤ, type, flag, name) \
	  اگر (سرکاؤ && (flags & (غیرعلامتی) flags_t::flag)) \
	     transform.name *= 1.f / (1 << سرکاؤ);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#تعریف_منسوخ PROCESS_TRANSFORM_COMPONENT

    اگر (!(flags & (غیرعلامتی) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    کل_transform.transform (transform.مقصد_transform ());

    منطقی same_coords = component_coords.لمبائی == coords.لمبائی &&
		       component_coords.arrayZ == coords.arrayZ;

    c.گہرائی_بایاں--;
    VARC.get_راستہ_at (c, gid,
		      component_coords, کل_transform,
		      والد_gid,
		      same_coords ? cache : خالی_اشاریہ);
    c.گہرائی_بایاں++;
  }

  واپس hb_ubytes_t (ریکارڈ, انتہا - ریکارڈ);
}

منطقی
VARC::get_راستہ_at (ثابت hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ثابت صحیح> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t والد_glyph,
		   hb_scalar_cache_t *والد_cache) ثابت
{
  // Don't recurse on the same glyph.
  غیرعلامتی idx = glyph == والد_glyph ?
		 NOT_COVERED :
		 (یہ+coverage).get_coverage (glyph);
  اگر (idx == NOT_COVERED)
  {
    اگر (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      اگر (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) واپس سچ;

      hb_transform_t<> برگ_transform = transform;
      برگ_transform.x0 *= c.font->x_multf;
      برگ_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {برگ_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_ڈیٹا,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = برگ_transform.is_شناختی_عنصر () ? *c.draw_session : transformer_session;

      اگر (c.font->face->جدول.glyf->get_راستہ_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, خالی_اشاریہ, &shape_draw_session.get_budget ())) واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_CFF
      اگر (c.font->face->جدول.cff2->get_راستہ_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) واپس سچ;
      اگر (c.font->face->جدول.cff1->get_راستہ (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) واپس سچ; // Doesn't have variations
#اگر_ختم
      واپس جھوٹ;
    }
    ورنہ اگر (c.extents)
    {
      اگر (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) واپس سچ;

      hb_glyph_extents_t glyph_extents;
      اگر (!c.font->face->جدول.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#اگر_غیر_تعریف_شدہ HB_NO_CFF
      اگر (!c.font->face->جدول.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      اگر (!c.font->face->جدول.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#اگر_ختم
	واپس جھوٹ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> برگ_transform = transform;
      برگ_transform.x0 *= c.font->x_multf;
      برگ_transform.y0 *= c.font->y_multf;
      برگ_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    واپس سچ;
  }

  اگر (c.گہرائی_بایاں <= 0)
    واپس سچ;

  اگر (c.کنارے_بایاں <= 0)
    واپس سچ;
  (c.کنارے_بایاں)--;

  hb_decycler_گرہ_t node (c.decycler);
  اگر (unlikely (!node.دورہ (glyph)))
    واپس سچ;

  hb_ubytes_t ریکارڈ = (یہ+glyph_اندراجات)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = والد_cache ?
				  والد_cache :
				  (یہ+varStore).create_cache (&static_cache);

  Var_مرکب_Glyph::get_راستہ_at (c,
				  glyph,
				  coords, transform,
				  ریکارڈ,
				  cache);

  اگر (cache != والد_cache)
    (یہ+varStore).destroy_cache (cache, &static_cache);

  واپس سچ;
}

#اگر_ختم

#تعریف_منسوخ VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#اگر_ختم

#اگر_ختم /* HB_OT_VAR_VARC_CC (pacify clang) */
#اگر_ختم /* HB_OT_VAR_VARC_CC */
