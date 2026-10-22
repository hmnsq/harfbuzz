#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_VARC_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_VARC_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_VARC_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "VARC.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-depend-data.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-gdef-table.hh"

ਨਾਮਾਕਾਸ਼ OT {

//namespace Var {

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VARC_PROCESS_TRANSFORM_COMPONENTS \
	HB_STMT_ਸ਼ੁਰੂ { \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_X, translateX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TRANSLATE_Y, translateY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_ROTATION, rotation); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_X, scaleX); \
	PROCESS_TRANSFORM_COMPONENT (10, F6DOT10, HAVE_SCALE_Y, scaleY); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_X, skewX); \
	PROCESS_TRANSFORM_COMPONENT (12, F4DOT12, HAVE_SKEW_Y, skewY); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_X, tCenterX); \
	PROCESS_TRANSFORM_COMPONENT ( 0, FWORD, HAVE_TCENTER_Y, tCenterY); \
	} HB_STMT_ਸਿਰਾ

ਤਾਰਕਿਕ
VarComponent::decompile_ਰਿਕਾਰਡ (ਅਚਲ VARC &varc,
				hb_ubytes_t ਕੁੱਲ_ਰਿਕਾਰਡ,
				hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *axis_indices,
				hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> *axis_ਮੁੱਲਾਂ,
				ਰਿਕਾਰਡ_t *decoded,
				int64_t *budget)
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਸ਼ੁਰੂ = ਕੁੱਲ_ਰਿਕਾਰਡ.arrayZ;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਰਿਕਾਰਡ = ਸ਼ੁਰੂ;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਸਿਰਾ = ਸ਼ੁਰੂ + ਕੁੱਲ_ਰਿਕਾਰਡ.ਲੰਮਾਈ;

  ਜੇ (axis_indices) axis_indices->clear ();
  ਜੇ (axis_ਮੁੱਲਾਂ) axis_ਮੁੱਲਾਂ->clear ();

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_UINT32VAR(name) \
  HB_STMT_ਸ਼ੁਰੂ { \
    ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - ਰਿਕਾਰਡ) < HBUINT32VAR::min_ਮਾਪ)) ਵਾਪਸ ਝੂਠ; \
    hb_barrier (); \
    ਸਵੈਚਾਲਿਤ &varint = * (ਅਚਲ HBUINT32VAR *) ਰਿਕਾਰਡ; \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = varint.get_ਮਾਪ (); \
    ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - ਰਿਕਾਰਡ) < ਮਾਪ)) ਵਾਪਸ ਝੂਠ; \
    name = (uint32_t) varint; \
    ਰਿਕਾਰਡ += ਮਾਪ; \
  } HB_STMT_ਸਿਰਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ READ_UINT32_VAR_ਖੇਤਰ(name, ਖੇਤਰ) \
  HB_STMT_ਸ਼ੁਰੂ { \
    decoded->ਖੇਤਰ##_offset = ਰਿਕਾਰਡ - ਸ਼ੁਰੂ; \
    READ_UINT32VAR (name); \
    decoded->ਖੇਤਰ##_size = ਰਿਕਾਰਡ - ਸ਼ੁਰੂ - decoded->ਖੇਤਰ##_offset; \
  } HB_STMT_ਸਿਰਾ

  READ_UINT32VAR (decoded->flags);

  decoded->gid_ਵਿੱਥ = ਰਿਕਾਰਡ - ਸ਼ੁਰੂ;
  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ)
  {
    decoded->gid_ਮਾਪ = HBGlyphID24::static_ਮਾਪ;
    ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - ਰਿਕਾਰਡ) < HBGlyphID24::static_ਮਾਪ)) ਵਾਪਸ ਝੂਠ;
    hb_barrier ();
    decoded->gid = * (ਅਚਲ HBGlyphID24 *) ਰਿਕਾਰਡ;
    ਰਿਕਾਰਡ += HBGlyphID24::static_ਮਾਪ;
  }
  ਨਹੀਂਤਾਂ
  {
    decoded->gid_ਮਾਪ = HBGlyphID16::static_ਮਾਪ;
    ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - ਰਿਕਾਰਡ) < HBGlyphID16::static_ਮਾਪ)) ਵਾਪਸ ਝੂਠ;
    hb_barrier ();
    decoded->gid = * (ਅਚਲ HBGlyphID16 *) ਰਿਕਾਰਡ;
    ਰਿਕਾਰਡ += HBGlyphID16::static_ਮਾਪ;
  }

  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::HAVE_CONDITION)
    READ_UINT32_VAR_ਖੇਤਰ (decoded->condition_ਸੂਚਕਾਂਕ, condition);

  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::HAVE_AXES)
  {
    READ_UINT32_VAR_ਖੇਤਰ (decoded->axis_indices_ਸੂਚਕਾਂਕ, axis_indices);
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ;
    ਜੇ (axis_indices)
    {
      axis_indices->extend ((&varc+varc.axisIndicesList)[decoded->axis_indices_ਸੂਚਕਾਂਕ]);
      ਜੇ (unlikely (axis_indices->ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
      axis_ਗਿਣਤੀ = axis_indices->ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
      axis_ਗਿਣਤੀ = hb_len ((&varc+varc.axisIndicesList)[decoded->axis_indices_ਸੂਚਕਾਂਕ]);
    ਜੇ (unlikely (budget && axis_ਗਿਣਤੀ &&
		  !hb_budget_spend (*budget, HB_BUDGET_1, axis_ਗਿਣਤੀ)))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (axis_ਮੁੱਲਾਂ)
    {
      ਜੇ (unlikely (!axis_ਮੁੱਲਾਂ->resize (axis_ਗਿਣਤੀ))) ਵਾਪਸ ਝੂਠ;
      ਅਚਲ HBUINT8 *p = (ਅਚਲ HBUINT8 *) ਰਿਕਾਰਡ;
      ਜੇ (unlikely (!Tuple_ਮੁੱਲਾਂ::decompile (p, *axis_ਮੁੱਲਾਂ,
					    (ਅਚਲ HBUINT8 *) ਸਿਰਾ)))
	ਵਾਪਸ ਝੂਠ;
      ਰਿਕਾਰਡ = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) p;
    }
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!Tuple_ਮੁੱਲਾਂ::skip (ਰਿਕਾਰਡ, ਸਿਰਾ, axis_ਗਿਣਤੀ)))
      ਵਾਪਸ ਝੂਠ;
  }

  decoded->axis_ਮੁੱਲਾਂ_var_idx = VarIdx::NO_VARIATION;
  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::AXIS_ਮੁੱਲਾਂ_HAVE_VARIATION)
    READ_UINT32_VAR_ਖੇਤਰ (decoded->axis_ਮੁੱਲਾਂ_var_idx, axis_ਮੁੱਲਾਂ_var);

  decoded->transform_var_idx = VarIdx::NO_VARIATION;
  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::TRANSFORM_HAS_VARIATION)
    READ_UINT32_VAR_ਖੇਤਰ (decoded->transform_var_idx, transform_var);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_TRANSFORM_COMPONENT(ਖਿਸਕਾਓ, type, flag, name) \
  ਜੇ (decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::flag) \
  { \
    static_assert (type::static_ਮਾਪ == HBINT16::static_ਮਾਪ, ""); \
    ਜੇ (unlikely (ਬਿਨਾਨਿਸ਼ਾਨੀ (ਸਿਰਾ - ਰਿਕਾਰਡ) < HBINT16::static_ਮਾਪ)) ਵਾਪਸ ਝੂਠ; \
    hb_barrier (); \
    decoded->transform.name = * (ਅਚਲ HBINT16 *) ਰਿਕਾਰਡ; \
    ਰਿਕਾਰਡ += HBINT16::static_ਮਾਪ; \
  }
  VARC_PROCESS_TRANSFORM_COMPONENTS;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_TRANSFORM_COMPONENT

  ਬਿਨਾਨਿਸ਼ਾਨੀ reserved = decoded->flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::RESERVED_ਨਕਾਬ;
  ਜਦੋਂ (reserved)
  {
    uint32_t discard HB_ਅਣਵਰਤਿਆ;
    READ_UINT32VAR (discard);
    reserved &= reserved - 1;
  }

  decoded->ਮਾਪ = ਰਿਕਾਰਡ - ਸ਼ੁਰੂ;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ READ_UINT32_VAR_ਖੇਤਰ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ READ_UINT32VAR
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
VARC::ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *glyphset) ਅਚਲ
{
  ਜੇ (unlikely (glyphset->ਵਿੱਚ_error ()))
    ਵਾਪਸ ਝੂਠ;

  hb_ਸੈੱਟ_t pending = *glyphset;
  ਜੇ (unlikely (pending.ਵਿੱਚ_error ()))
    ਵਾਪਸ ਝੂਠ;

  ਜਦੋਂ (pending)
  {
    hb_codepoint_t gid = pending.get_min ();
    pending.del (gid);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (gid);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) ਜਾਰੀ;

    hb_ubytes_t ਰਿਕਾਰਡ = (ਇਹ+glyph_ਰਿਕਾਰਡਾਂ)[ਸੂਚਕਾਂਕ_2];
    ਜਦੋਂ (ਰਿਕਾਰਡ)
    {
      VarComponent::ਰਿਕਾਰਡ_t component;
      ਜੇ (unlikely (!VarComponent::decompile_ਰਿਕਾਰਡ (*ਇਹ, ਰਿਕਾਰਡ,
						     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, &component)))
	ਵਾਪਸ ਝੂਠ;
      ਜੇ (!glyphset->has (component.gid))
      {
	glyphset->ਜੋੜੋ (component.gid);
	ਜੇ (unlikely (glyphset->ਵਿੱਚ_error ()))
	  ਵਾਪਸ ਝੂਠ;
	pending.ਜੋੜੋ (component.gid);
	ਜੇ (unlikely (pending.ਵਿੱਚ_error ()))
	  ਵਾਪਸ ਝੂਠ;
      }
      ਰਿਕਾਰਡ = ਰਿਕਾਰਡ.sub_array (component.ਮਾਪ);
    }
  }
  ਵਾਪਸ ਸੱਚ;
}

ਖਾਲੀ
VARC::depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
{
  ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਰਿਕਾਰਡਾਂ = ਇਹ+glyph_ਰਿਕਾਰਡਾਂ;
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_zip (ਇਹ+coverage,
			  hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਰਿਕਾਰਡਾਂ.ਗਿਣਤੀ_2)))
  {
    hb_codepoint_t gid = _.ਪਹਿਲਾ;
    hb_ubytes_t ਰਿਕਾਰਡ = ਰਿਕਾਰਡਾਂ[_.second];
    ਜਦੋਂ (ਰਿਕਾਰਡ)
    {
	VarComponent::ਰਿਕਾਰਡ_t component;
	ਜੇ (unlikely (!VarComponent::decompile_ਰਿਕਾਰਡ (*ਇਹ, ਰਿਕਾਰਡ,
						       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, &component)))
	{
	  depend_ਅੰਕੜੇ->fail ();
	  ਵਾਪਸ;
	}
      depend_ਅੰਕੜੇ->ਜੋੜੋ_depend (gid, ਸਾਰਣੀ_ਟੈਗ, component.gid);
      ਰਿਕਾਰਡ = ਰਿਕਾਰਡ.sub_array (component.ਮਾਪ);
    }
  }
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW

ਬਣਤਰ hb_transforming_pen_context_t
{
  hb_transform_t<> transform;
  hb_draw_funcs_t *dfuncs;
  ਖਾਲੀ *ਅੰਕੜੇ;
  hb_draw_state_t *st;
  int64_t *budget_ਬਚਿਆ;
};

ਸਥਿਰ ਖਾਲੀ
hb_transforming_pen_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *ਅੰਕੜੇ,
			     hb_draw_state_t *st,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ਅੰਕੜੇ;

  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

  c->dfuncs->move_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, *c->st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_transforming_pen_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *ਅੰਕੜੇ,
			     hb_draw_state_t *st,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ਅੰਕੜੇ;

  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

  c->dfuncs->ਸਤਰ_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, *c->st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_transforming_pen_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *ਅੰਕੜੇ,
				  hb_draw_state_t *st,
				  ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
				  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ਅੰਕੜੇ;

  c->transform.transform_ਬਿੰਦੂ (control_x, control_y);
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

  c->dfuncs->quadratic_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, *c->st, control_x, control_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_transforming_pen_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *ਅੰਕੜੇ,
			      hb_draw_state_t *st,
			      ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
			      ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
			      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ਅੰਕੜੇ;

  c->transform.transform_ਬਿੰਦੂ (control1_x, control1_y);
  c->transform.transform_ਬਿੰਦੂ (control2_x, control2_y);
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);

  c->dfuncs->cubic_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, *c->st, control1_x, control1_y, control2_x, control2_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_transforming_pen_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *ਅੰਕੜੇ,
				hb_draw_state_t *st,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_transforming_pen_context_t *c = (hb_transforming_pen_context_t *) ਅੰਕੜੇ;

  c->dfuncs->close_ਰਸਤਾ (c->ਅੰਕੜੇ, *c->st);
}

ਸਥਿਰ int64_t *
hb_transforming_pen_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_transforming_pen_context_t *) draw_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_transforming_pen_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_transforming_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_transforming_pen_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, hb_transforming_pen_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, hb_transforming_pen_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_transforming_pen_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, hb_transforming_pen_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, hb_transforming_pen_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_transforming_pen_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_transforming_pen_funcs);

    ਵਾਪਸ funcs;
  }
} static_transforming_pen_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_transforming_pen_funcs ()
{
  static_transforming_pen_funcs.free_instance ();
}

ਸਥਿਰ hb_draw_funcs_t *
hb_transforming_pen_get_funcs ()
{
  ਵਾਪਸ static_transforming_pen_funcs.get_unconst ();
}

hb_ubytes_t
VarComponent::get_ਰਸਤਾ_at (ਅਚਲ hb_varc_context_t &c,
			   hb_codepoint_t ਮਾਪਾ_gid,
			   hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
			   hb_transform_t<> ਕੁੱਲ_transform,
			   hb_ubytes_t ਕੁੱਲ_ਰਿਕਾਰਡ,
			   hb_scalar_cache_t *cache) ਅਚਲ
{
  ਜੇ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_4)))
    ਵਾਪਸ hb_ubytes_t ();

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਸਿਰਾ = ਕੁੱਲ_ਰਿਕਾਰਡ.arrayZ + ਕੁੱਲ_ਰਿਕਾਰਡ.ਲੰਮਾਈ;
  ਸਵੈਚਾਲਿਤ &VARC = *c.font->face->ਸਾਰਣੀ.VARC->ਸਾਰਣੀ;
  ਸਵੈਚਾਲਿਤ &varStore = &VARC+VARC.varStore;

  ਸਵੈਚਾਲਿਤ &axisIndices = c.scratch.axisIndices;
  ਸਵੈਚਾਲਿਤ &axis_ਮੁੱਲਾਂ_2 = c.scratch.axis_ਮੁੱਲਾਂ_2;
  ਰਿਕਾਰਡ_t component;
  ਜੇ (unlikely (!decompile_ਰਿਕਾਰਡ (VARC, ਕੁੱਲ_ਰਿਕਾਰਡ,
				   &axisIndices, &axis_ਮੁੱਲਾਂ_2, &component,
				   &c.budget)))
    ਵਾਪਸ hb_ubytes_t ();

  uint32_t flags = component.flags;
  hb_codepoint_t gid = component.gid;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਰਿਕਾਰਡ = ਕੁੱਲ_ਰਿਕਾਰਡ.arrayZ + component.ਮਾਪ;

  // Condition
  ਤਾਰਕਿਕ show = ਸੱਚ;
  ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::HAVE_CONDITION)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &condition = (&VARC+VARC.conditionList)[component.condition_ਸੂਚਕਾਂਕ];
    ਸਵੈਚਾਲਿਤ instancer = Multi_ਵਸਤੂ_Var_Store_Instancer(&varStore, ਖਾਲੀ_ਸੂਚਕ, coords, cache);
    show = condition.evaluate (coords.arrayZ, coords.ਲੰਮਾਈ, &instancer);
  }

  // Axis values

  // Apply variations if any
  ਜੇ ((flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::AXIS_ਮੁੱਲਾਂ_HAVE_VARIATION) &&
      show && !axis_ਮੁੱਲਾਂ_2.ਵਿੱਚ_error ())
  {
    ਜੇ (unlikely (!hb_budget_spend (c.budget, axis_ਮੁੱਲਾਂ_2.ਲੰਮਾਈ,
				    coords.ਲੰਮਾਈ)))
      ਵਾਪਸ hb_ubytes_t ();
    varStore.get_delta (component.axis_ਮੁੱਲਾਂ_var_idx, coords,
			axis_ਮੁੱਲਾਂ_2.as_array (), cache);
  }

  ਸਵੈਚਾਲਿਤ component_coords = coords;
  /* Copying coords is expensive; so we have put an arbitrary
   * limit on the max number of coords for now. */
  ਜੇ ((flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::RESET_UNSPECIFIED_AXES) ||
      coords.ਲੰਮਾਈ > HB_VAR_ਸੰਯੁਕਤ_MAX_AXES)
    component_coords = hb_array (c.font->coords, c.font->ਅੰਕ_coords);

  uint32_t transformVarIdx = component.transform_var_idx;
  hb_transform_decomposed_t<> transform = component.transform;

  ਜੇ (show)
  {
    // Only use coord_setter if there's actually any axis overrides.
    ਜੇ (unlikely (axisIndices &&
		  !hb_budget_spend (c.budget, HB_BUDGET_1,
				    component_coords.ਲੰਮਾਈ)))
      ਵਾਪਸ hb_ubytes_t ();
    coord_setter_t coord_setter (axisIndices ? component_coords : hb_array<ਪੂਰਨ> ());
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axisIndices.ਲੰਮਾਈ; i++)
      coord_setter[axisIndices[i]] = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (round (axis_ਮੁੱਲਾਂ_2[i]));
    ਜੇ (axisIndices)
      component_coords = coord_setter.get_coords ();

    // Apply transform variations if any
    ਜੇ (transformVarIdx != VarIdx::NO_VARIATION)
    {
      ਦੁੱਗਣਾ transform_ਮੁੱਲਾਂ[9];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_Transform_ਮੁੱਲਾਂ = 0;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_TRANSFORM_COMPONENT(ਖਿਸਕਾਓ, type, flag, name) \
	  ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::flag) \
	    transform_ਮੁੱਲਾਂ[ਅੰਕ_Transform_ਮੁੱਲਾਂ++] = (ਦੁੱਗਣਾ) transform.name;
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_TRANSFORM_COMPONENT
      ਜੇ (unlikely (!hb_budget_spend (c.budget, ਅੰਕ_Transform_ਮੁੱਲਾਂ,
				      coords.ਲੰਮਾਈ)))
	ਵਾਪਸ hb_ubytes_t ();
      varStore.get_delta (transformVarIdx, coords, hb_array (transform_ਮੁੱਲਾਂ, ਅੰਕ_Transform_ਮੁੱਲਾਂ), cache);
      ਅੰਕ_Transform_ਮੁੱਲਾਂ = 0;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_TRANSFORM_COMPONENT(ਖਿਸਕਾਓ, type, flag, name) \
	  ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::flag) \
	    transform.name = (ਦਸ਼ਮਲਵ) transform_ਮੁੱਲਾਂ[ਅੰਕ_Transform_ਮੁੱਲਾਂ++];
      VARC_PROCESS_TRANSFORM_COMPONENTS;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_TRANSFORM_COMPONENT
    }

    // Divide them by their divisors
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_TRANSFORM_COMPONENT(ਖਿਸਕਾਓ, type, flag, name) \
	  ਜੇ (ਖਿਸਕਾਓ && (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::flag)) \
	     transform.name *= 1.f / (1 << ਖਿਸਕਾਓ);
    VARC_PROCESS_TRANSFORM_COMPONENTS;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_TRANSFORM_COMPONENT

    ਜੇ (!(flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) flags_t::HAVE_SCALE_Y))
      transform.scaleY = transform.scaleX;

    transform.rotation *= HB_PI;
    transform.skewX *= HB_PI;
    transform.skewY *= HB_PI;

    ਕੁੱਲ_transform.transform (transform.ਮੰਜ਼ਿਲ_transform ());

    ਤਾਰਕਿਕ same_coords = component_coords.ਲੰਮਾਈ == coords.ਲੰਮਾਈ &&
		       component_coords.arrayZ == coords.arrayZ;

    c.ਡੂੰਘਾਈ_ਖੱਬਾ--;
    VARC.get_ਰਸਤਾ_at (c, gid,
		      component_coords, ਕੁੱਲ_transform,
		      ਮਾਪਾ_gid,
		      same_coords ? cache : ਖਾਲੀ_ਸੂਚਕ);
    c.ਡੂੰਘਾਈ_ਖੱਬਾ++;
  }

  ਵਾਪਸ hb_ubytes_t (ਰਿਕਾਰਡ, ਸਿਰਾ - ਰਿਕਾਰਡ);
}

ਤਾਰਕਿਕ
VARC::get_ਰਸਤਾ_at (ਅਚਲ hb_varc_context_t &c,
		   hb_codepoint_t glyph,
		   hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
		   hb_transform_t<> transform,
		   hb_codepoint_t ਮਾਪਾ_glyph,
		   hb_scalar_cache_t *ਮਾਪਾ_cache) ਅਚਲ
{
  // Don't recurse on the same glyph.
  ਬਿਨਾਨਿਸ਼ਾਨੀ idx = glyph == ਮਾਪਾ_glyph ?
		 NOT_COVERED :
		 (ਇਹ+coverage).get_coverage (glyph);
  ਜੇ (idx == NOT_COVERED)
  {
    ਜੇ (c.draw_session)
    {
      /* Out of budget: draw nothing, but signal success so remaining
       * leaves are skipped instead of falling back per-glyph. */
      ਜੇ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ਵਾਪਸ ਸੱਚ;

      hb_transform_t<> ਪੱਤੀ_transform = transform;
      ਪੱਤੀ_transform.x0 *= c.font->x_multf;
      ਪੱਤੀ_transform.y0 *= c.font->y_multf;

      // Build a transforming pen to apply the transform.
      hb_draw_funcs_t *transformer_funcs = hb_transforming_pen_get_funcs ();
      hb_transforming_pen_context_t context {ਪੱਤੀ_transform,
					     c.draw_session->funcs,
					     c.draw_session->draw_ਅੰਕੜੇ,
					     &c.draw_session->st,
					     &c.budget};
      hb_draw_session_t transformer_session {transformer_funcs, &context};
      hb_draw_session_t &shape_draw_session = ਪੱਤੀ_transform.is_ਪਛਾਣ_ਤੱਤ () ? *c.draw_session : transformer_session;

      ਜੇ (c.font->face->ਸਾਰਣੀ.glyf->get_ਰਸਤਾ_at (c.font, glyph, shape_draw_session, coords, c.scratch.glyf_scratch, ਖਾਲੀ_ਸੂਚਕ, &shape_draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF
      ਜੇ (c.font->face->ਸਾਰਣੀ.cff2->get_ਰਸਤਾ_at (c.font, glyph, shape_draw_session, coords, &shape_draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ;
      ਜੇ (c.font->face->ਸਾਰਣੀ.cff1->get_ਰਸਤਾ (c.font, glyph, shape_draw_session, &shape_draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ; // Doesn't have variations
#ਜੇ_ਅੰਤ
      ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (c.extents)
    {
      ਜੇ (unlikely (!hb_budget_spend (c.budget, HB_BUDGET_1))) ਵਾਪਸ ਸੱਚ;

      hb_glyph_extents_t glyph_extents;
      ਜੇ (!c.font->face->ਸਾਰਣੀ.glyf->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF
      ਜੇ (!c.font->face->ਸਾਰਣੀ.cff2->get_extents_at (c.font, glyph, &glyph_extents, coords, &c.budget))
      ਜੇ (!c.font->face->ਸਾਰਣੀ.cff1->get_extents (c.font, glyph, &glyph_extents, &c.budget)) // Doesn't have variations
#ਜੇ_ਅੰਤ
	ਵਾਪਸ ਝੂਠ;

      hb_extents_t<> comp_extents (glyph_extents);
      hb_transform_t<> ਪੱਤੀ_transform = transform;
      ਪੱਤੀ_transform.x0 *= c.font->x_multf;
      ਪੱਤੀ_transform.y0 *= c.font->y_multf;
      ਪੱਤੀ_transform.transform_extents (comp_extents);
      c.extents->union_ (comp_extents);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਜੇ (c.ਡੂੰਘਾਈ_ਖੱਬਾ <= 0)
    ਵਾਪਸ ਸੱਚ;

  ਜੇ (c.ਕਿਨਾਰੇ_ਖੱਬਾ <= 0)
    ਵਾਪਸ ਸੱਚ;
  (c.ਕਿਨਾਰੇ_ਖੱਬਾ)--;

  hb_decycler_ਗੰਢ_t node (c.decycler);
  ਜੇ (unlikely (!node.ਦੇਖੋ (glyph)))
    ਵਾਪਸ ਸੱਚ;

  hb_ubytes_t ਰਿਕਾਰਡ = (ਇਹ+glyph_ਰਿਕਾਰਡਾਂ)[idx];

  hb_scalar_cache_t static_cache;
  hb_scalar_cache_t *cache = ਮਾਪਾ_cache ?
				  ਮਾਪਾ_cache :
				  (ਇਹ+varStore).create_cache (&static_cache);

  Var_ਸੰਯੁਕਤ_Glyph::get_ਰਸਤਾ_at (c,
				  glyph,
				  coords, transform,
				  ਰਿਕਾਰਡ,
				  cache);

  ਜੇ (cache != ਮਾਪਾ_cache)
    (ਇਹ+varStore).destroy_cache (cache, &static_cache);

  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VARC_PROCESS_TRANSFORM_COMPONENTS

//} // namespace Var
} // namespace OT

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_OT_VAR_VARC_CC (pacify clang) */
#ਜੇ_ਅੰਤ /* HB_OT_VAR_VARC_CC */
