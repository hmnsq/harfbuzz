/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2020  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Calder Kitagawa
 */

#اگر_غیر_تعریف_شدہ OT_COLOR_COLR_COLR_HH
#تعریف_کرو OT_COLOR_COLR_COLR_HH

#شامل "../../../hb.hh"
#شامل "../../../hb-decycler.hh"
#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-ot-var-common.hh"
#شامل "../../../hb-paint.hh"
#شامل "../../../hb-paint-bounded.hh"
#شامل "../../../hb-paint-extents.hh"
#شامل "../../../hb-depend-data.hh"

#شامل "../../../hb-ot-dual.hh"
#شامل "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#تعریف_کرو HB_OT_لیبل_COLR HB_لیبل('C','O','L','R')

نامخلا OT {
ساخت hb_paint_context_t;
}

ساخت hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

نامخلا OT {

ساخت COLR;

ساخت Paint;

ساخت hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ثابت حرف *get_name () { واپس "PAINT"; }
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  ثابت COLR* get_colr_جدول () ثابت
  { واپس reinterpret_cast<ثابت COLR *> (بنیاد); }

عوامی:
  ثابت خالی *بنیاد;
  hb_paint_funcs_t *funcs;
  خالی *ڈیٹا;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  شے_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحیح گہرائی_بایاں = HB_MAX_NESTING_LEVEL;
  صحیح کنارہ_گنتی = HB_MAX_گراف_کنارہ_گنتی;

  hb_paint_context_t (ثابت خالی *بنیاد_2,
		      hb_paint_funcs_t *funcs_,
		      خالی *ڈیٹا_2,
                      hb_font_t *font_,
                      غیرعلامتی صحیح palette_,
                      hb_color_t foreground_,
		      شے_Var_Store_Instancer &instancer_) :
    بنیاد (بنیاد_2),
    funcs (funcs_),
    ڈیٹا (ڈیٹا_2),
    font (font_),
    palette (
#اگر_غیر_تعریف_شدہ HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->جدول.CPAL->get_palette_colors (palette_ < font->face->جدول.CPAL->get_palette_گنتی () ? palette_ : 0)
#اگر_ختم
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    اگر (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_سیٹ_synthetic_bold (font, 0, 0, سچ);
      hb_font_سیٹ_synthetic_slant (font, 0);
    }
    ورنہ
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساکن غیرعلامتی
  color_alpha (hb_color_t color, اعشاری alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    واپس roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (غیرعلامتی صحیح color_اشاریہ, اعشاری alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = سچ;

    اگر (color_اشاریہ != 0xffff)
    {
      اگر (!funcs->custom_palette_color (ڈیٹا, color_اشاریہ, &color))
        color = palette[color_اشاریہ];

      *is_foreground = جھوٹ;
    }

    واپس HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  اندرونی خالی recurse (ثابت Paint &paint);
};

ساخت hb_colrv1_احاطہ_context_t :
       hb_dispatch_context_t<hb_colrv1_احاطہ_context_t>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj)
  {
    اگر (unlikely (nesting_level_بایاں == 0))
      واپس hb_خالی_خانے_t ();

    اگر (paint_دیکھا (&obj))
      واپس hb_خالی_خانے_t ();

    nesting_level_بایاں--;
    obj.closurev1 (یہ);
    nesting_level_بایاں++;
    واپس hb_خالی_خانے_t ();
  }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  منطقی paint_دیکھا (ثابت خالی *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) بنیاد);
    اگر (دیکھا_paint.اندر_error() || دیکھا_paint.has (delta))
      واپس سچ;

    دیکھا_paint.جوڑو (delta);
    واپس جھوٹ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { واپس reinterpret_cast<ثابت COLR *> (بنیاد); }

  خالی جوڑو_glyph (غیرعلامتی glyph_id)
  { glyphs->جوڑو (glyph_id); }

  خالی جوڑو_layer_indices (غیرعلامتی پہلا_layer_اشاریہ, غیرعلامتی عدد_کا_layers)
  {
    اگر (عدد_کا_layers == 0) واپس;
    layer_indices->جوڑو_range (پہلا_layer_اشاریہ, پہلا_layer_اشاریہ + عدد_کا_layers - 1);
  }

  خالی جوڑو_palette_اشاریہ (غیرعلامتی palette_اشاریہ)
  { palette_indices->جوڑو (palette_اشاریہ); }

  خالی جوڑو_var_idxes (غیرعلامتی پہلا_var_idx, غیرعلامتی عدد_idxes)
  {
    اگر (!عدد_idxes || پہلا_var_idx == VarIdx::NO_VARIATION) واپس;
    variation_indices->جوڑو_range (پہلا_var_idx, پہلا_var_idx + عدد_idxes - 1);
  }

  عوامی:
  ثابت خالی *بنیاد;
  hb_سیٹ_t دیکھا_paint;
  hb_سیٹ_t *glyphs;
  hb_سیٹ_t *layer_indices;
  hb_سیٹ_t *palette_indices;
  hb_سیٹ_t *variation_indices;
  غیرعلامتی عدد_var_idxes;
  غیرعلامتی nesting_level_بایاں;

  hb_colrv1_احاطہ_context_t (ثابت خالی *بنیاد_2,
                               hb_سیٹ_t *glyphs_,
                               hb_سیٹ_t *layer_indices_,
                               hb_سیٹ_t *palette_indices_,
                               hb_سیٹ_t *variation_indices_,
                               غیرعلامتی عدد_var_idxes_2 = 1,
                               غیرعلامتی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
                          بنیاد (بنیاد_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          عدد_var_idxes (عدد_var_idxes_2),
                          nesting_level_بایاں (nesting_level_بایاں_2)
  {}
};


ساخت hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj)
  {
    اگر (unlikely (nesting_level_بایاں == 0))
      واپس hb_خالی_خانے_t ();

    اگر (paint_دیکھا (&obj))
      واپس hb_خالی_خانے_t ();

    nesting_level_بایاں--;
    obj.dependv1 (یہ);
    nesting_level_بایاں++;
    واپس hb_خالی_خانے_t ();
  }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  منطقی paint_دیکھا (ثابت خالی *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    اگر (دیکھا_paint.اندر_error() || دیکھا_paint.has (delta))
      واپس سچ;

    دیکھا_paint.جوڑو (delta);
    اگر (unlikely (دیکھا_paint.اندر_error ()))
      depend_ڈیٹا->fail ();
    واپس جھوٹ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { واپس colr; }

  عوامی:
  ثابت COLR *colr;
  hb_depend_ڈیٹا_builder_t *depend_ڈیٹا;
  hb_سیٹ_t دیکھا_paint;
  غیرعلامتی nesting_level_بایاں;
  hb_codepoint_t ماخذ_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_ڈیٹا_builder_t *depend_ڈیٹا_2,
                              غیرعلامتی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_ڈیٹا (depend_ڈیٹا_2),
                          nesting_level_بایاں (nesting_level_بایاں_2),
                          ماخذ_gid (0)
  {}
};

ساخت Layer_ریکارڈ
{
  عامل hb_ot_color_layer_t () ثابت { واپس {glyphId, colorIdx}; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  اشاریہ		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت بنیاد_Glyph_ریکارڈ
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	پہلا_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	عدد_Layers;	/* Number of color layers
				 * associated with this glyph */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

سانچہ <قسم_نام T>
ساخت Variable
{
  ساکن constexpr منطقی is_variable = سچ;

  ثابت T& get_قدر () ثابت { واپس value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (یہ));
  }

  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    c->عدد_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->جوڑو_var_idxes (var_Idx_بنیاد, c->عدد_var_idxes);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!value.subset (c, instancer, var_Idx_بنیاد)) return_trace (جھوٹ);
    اگر (c->plan->all_axes_pinned)
      return_trace (سچ);

    VarIdx new_varidx;
    new_varidx = var_Idx_بنیاد;
    اگر (var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
      اگر (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_بنیاد, &new_varidx_delta))
        return_trace (جھوٹ);

      new_varidx = hb_پہلا (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c));
  }

  خالی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (یہ);
    value.paint_glyph (c, var_Idx_بنیاد);
  }

  خالی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, var_Idx_بنیاد, instancer);
  }

  خالی get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, var_Idx_بنیاد, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    واپس value.get_extend ();
  }

  محفوظ:
  T      value;
  عوامی:
  VarIdx var_Idx_بنیاد;
  عوامی:
  DEFINE_سائز_MIN (VarIdx::static_سائز + T::min_سائز);
};

سانچہ <قسم_نام T>
ساخت NoVariable
{
  ساکن constexpr منطقی is_variable = جھوٹ;

  ثابت T& get_قدر () ثابت { واپس value; }

  ساکن constexpr uint32_t var_Idx_بنیاد = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (یہ));
  }

  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace (value.subset (c, instancer, var_Idx_بنیاد));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c));
  }

  خالی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (یہ);
    value.paint_glyph (c, var_Idx_بنیاد);
  }

  خالی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  خالی get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    واپس value.get_extend ();
  }

  T      value;
  عوامی:
  DEFINE_سائز_MIN (T::min_سائز);
};

// Color structures

سانچہ <قسم_نام Type, غیرعلامتی fraction_بٹ>
ساکن خالی
سیٹ_fixed_clamped (HBFixed<Type, fraction_بٹ> &برآمد, اعشاری value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  برآمد.سیٹ_int (hb_clamp_مقصد<قسم_نام Type::type> (roundf ((value / HBFixed<Type, fraction_بٹ>::mult))));
}

ساخت ColorStop
{
  خالی dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    c->جوڑو_palette_اشاریہ (palette_اشاریہ_2);
    c->عدد_var_idxes = 2;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->stop_انحراف, stop_انحراف.مقصد_float(instancer (var_Idx_بنیاد, 0)));
      اعشاری a = alpha.مقصد_float (instancer (var_Idx_بنیاد, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      سیٹ_fixed_clamped (برآمد->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_تفویض (برآمد->palette_اشاریہ_2, c->plan->colr_palettes.get (palette_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *برآمد,
		       uint32_t varIdx,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    برآمد->انحراف = stop_انحراف.مقصد_float(instancer (varIdx, 0));
    برآمد->color = c->get_color (palette_اشاریہ_2,
                               alpha.مقصد_float (instancer (varIdx, 1)),
                               &برآمد->is_foreground);
  }

  F2DOT14	stop_انحراف;
  HBUINT16	palette_اشاریہ_2;
  F2DOT14	alpha;
  عوامی:
  DEFINE_سائز_STATIC (2 + 2 * F2DOT14::static_سائز);
};

ساخت Extend : HBUINT8
{
  شمار {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عوامی:
  DEFINE_سائز_STATIC (1);
};

سانچہ <سانچہ<قسم_نام> جماعت Var>
ساخت Color_سطر
{
  خالی dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    برائے (ثابت خودکار &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    اگر (!c->serializer->check_تفویض (برآمد->extend, extend, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) return_trace (جھوٹ);
    اگر (!c->serializer->check_تفویض (برآمد->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز)) return_trace (جھوٹ);

    برائے (ثابت خودکار& stop : stops.iter ())
    {
      اگر (!stop.subset (c, instancer)) return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  غیرعلامتی صحیح
  get_color_stops (hb_paint_context_t *c,
                   غیرعلامتی صحیح آغاز,
		   غیرعلامتی صحیح *گنتی,
		   hb_color_stop_t *color_stops,
		   ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    غیرعلامتی صحیح len = stops.len;

    اگر (گنتی && color_stops)
    {
      غیرعلامتی صحیح i;
      برائے (i = 0; i < *گنتی && آغاز + i < len; i++)
        stops[آغاز + i].get_color_stop (c, &color_stops[i], instancer);
      *گنتی = i;
    }

    واپس len;
  }

  HB_اندرونی ساکن غیرعلامتی صحیح static_get_color_stops (hb_color_سطر_t *color_سطر,
							  خالی *color_سطر_ڈیٹا,
							  غیرعلامتی صحیح آغاز,
							  غیرعلامتی صحیح *گنتی,
							  hb_color_stop_t *color_stops,
							  خالی *user_ڈیٹا)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_ڈیٹا;
    hb_paint_context_t *c = (hb_paint_context_t *) user_ڈیٹا;
    واپس thiz->get_color_stops (c, آغاز, گنتی, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    انتخاب ((غیرعلامتی صحیح) extend)
    {
      حالت Extend::EXTEND_REPEAT:  واپس HB_PAINT_EXTEND_REPEAT;
      حالت Extend::EXTEND_REFLECT: واپس HB_PAINT_EXTEND_REFLECT;
      حالت Extend::EXTEND_PAD:
      طےشدہ:                     واپس HB_PAINT_EXTEND_PAD;
    }
  }

  HB_اندرونی ساکن hb_paint_extend_t static_get_extend (hb_color_سطر_t *color_سطر,
							  خالی *color_سطر_ڈیٹا,
							  خالی *user_ڈیٹا)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_ڈیٹا;
    واپس thiz->get_extend ();
  }

  Extend	extend;
  Array16_کا<Var<ColorStop>>	stops;
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
ساخت مرکب_منوال : HBUINT8
{
  عوامی:
  DEFINE_سائز_STATIC (1);
};

ساخت Affine2x3
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  { c->عدد_var_idxes = 6; }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      خودکار fold = [&] (F16DOT16 &target, F16DOT16 ماخذ, غیرعلامتی انحراف) {
        // Keep the stored integer precise while applying the float delta.
        target.سیٹ_int (hb_clamp_مقصد<int32_t> (roundf ((دوہرا) ماخذ.مقصد_int () +
                                                    (دوہرا) instancer (var_Idx_بنیاد, انحراف))));
      };
      fold (برآمد->xx, xx, 0);
      fold (برآمد->yx, yx, 1);
      fold (برآمد->xy, xy, 2);
      fold (برآمد->yy, yy, 3);
      fold (برآمد->dx, dx, 4);
      fold (برآمد->dy, dy, 5);
    }
    return_trace (سچ);
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    c->funcs->ڈالو_transform (c->ڈیٹا,
			      xx.مقصد_float (c->instancer (var_Idx_بنیاد, 0)),
			      yx.مقصد_float (c->instancer (var_Idx_بنیاد, 1)),
                              xy.مقصد_float (c->instancer (var_Idx_بنیاد, 2)),
			      yy.مقصد_float (c->instancer (var_Idx_بنیاد, 3)),
                              dx.مقصد_float (c->instancer (var_Idx_بنیاد, 4)),
			      dy.مقصد_float (c->instancer (var_Idx_بنیاد, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  عوامی:
  DEFINE_سائز_STATIC (6 * F16DOT16::static_سائز);
};

ساخت PaintColrLayers
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer HB_غیر_مستعمل) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    uint32_t پہلا_layer_اشاریہ = عدد_Layers ? c->plan->colrv1_layers.get (پہلا_Layer_اشاریہ) : 0;
    return_trace (c->serializer->check_تفویض (برآمد->پہلا_Layer_اشاریہ, پہلا_layer_اشاریہ,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  اندرونی خالی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	عدد_Layers;
  HBUINT32	پہلا_Layer_اشاریہ;  /* index into COLRv1::layerList */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت PaintSolid
{
  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    c->جوڑو_palette_اشاریہ (palette_اشاریہ_2);
    c->عدد_var_idxes = 1;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      اعشاری a = alpha.مقصد_float (instancer (var_Idx_بنیاد, 0));
      سیٹ_fixed_clamped (برآمد->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    اگر (format == 3 && c->plan->all_axes_pinned)
        برآمد->format = 2;

    return_trace (c->serializer->check_تفویض (برآمد->palette_اشاریہ_2, c->plan->colr_palettes.get (palette_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_بنیاد,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (palette_اشاریہ_2,
                           alpha.مقصد_float (c->instancer (var_Idx_بنیاد, 0)),
                           is_foreground);
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_بنیاد, &is_foreground, &color);
    c->funcs->color (c->ڈیٹا, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_اشاریہ_2;
  F2DOT14	alpha;
  عوامی:
  DEFINE_سائز_STATIC (3 + F2DOT14::static_سائز);
};

سانچہ <سانچہ<قسم_نام> جماعت Var>
ساخت Paint_خطی_Gradient
{
  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    (یہ+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      برآمد->x0 = hb_clamp_مقصد<int16_t> ((دوہرا) x0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 0)));
      برآمد->y0 = hb_clamp_مقصد<int16_t> ((دوہرا) y0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
      برآمد->x1 = hb_clamp_مقصد<int16_t> ((دوہرا) x1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
      برآمد->y1 = hb_clamp_مقصد<int16_t> ((دوہرا) y1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 3)));
      برآمد->x2 = hb_clamp_مقصد<int16_t> ((دوہرا) x2 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 4)));
      برآمد->y2 = hb_clamp_مقصد<int16_t> ((دوہرا) y2 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 5)));
    }

    اگر (format == 5 && c->plan->all_axes_pinned)
        برآمد->format = 4;

    return_trace (برآمد->color_سطر_2.serialize_subset (c, color_سطر_2, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && color_سطر_2.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    hb_color_سطر_t cl = {
      (خالی *) &(یہ+color_سطر_2),
      (یہ+color_سطر_2).static_get_color_stops, c,
      (یہ+color_سطر_2).static_get_extend, خالی_اشاریہ
    };

    c->funcs->خطی_gradient (c->ڈیٹا, &cl,
			       x0 + c->instancer (var_Idx_بنیاد, 0),
			       y0 + c->instancer (var_Idx_بنیاد, 1),
			       x1 + c->instancer (var_Idx_بنیاد, 2),
			       y1 + c->instancer (var_Idx_بنیاد, 3),
			       x2 + c->instancer (var_Idx_بنیاد, 4),
			       y2 + c->instancer (var_Idx_بنیاد, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_مقصد<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  عوامی:
  DEFINE_سائز_STATIC (4 + 6 * FWORD::static_سائز);
};

سانچہ <سانچہ<قسم_نام> جماعت Var>
ساخت PaintRadialGradient
{
  منطقی constant_after_pinning (hb_subset_context_t *c,
                               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    اگر (c->plan->has_avar2) واپس جھوٹ;
    ثابت خودکار &regions = instancer.varStore->get_region_list ();
    برائے (غیرعلامتی i = 0; i < regions.region_گنتی; i++)
    {
      hb_hashmap_t<hb_لیبل_t, Triple> axes;
      اگر (!regions.get_var_region (i, c->plan->axes_old_اشاریہ_لیبل_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_برآمد_کا_گنجائش);
        واپس جھوٹ;
      }
      برائے (خودکار axis : axes)
      {
        Triple *حد_بندی;
        اگر (!c->plan->user_axes_location.has (axis.پہلا, &حد_بندی) ||
            حد_بندی->minimum != حد_بندی->زیادہ_سے_زیادہ)
          واپس جھوٹ;
      }
    }
    واپس سچ;
  }

  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    (یہ+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقی subset_nonnegative (hb_subset_context_t *c,
                           ثابت شے_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_بنیاد) ثابت
  {
    دوہرا ماخذ[6] = {
      x0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 0)),
      y0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)),
      radius0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)),
      x1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 3)),
      y1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 4)),
      radius1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 5))
    };
    خودکار transparent = [&] {
      خودکار *برآمد = c->serializer->allocate_سائز<PaintSolid> (5);
      اگر (!برآمد) واپس جھوٹ;
      برآمد->format = 2; برآمد->palette_اشاریہ_2 = 0xFFFF; برآمد->alpha.سیٹ_float (0);
      واپس سچ;
    };
    ثابت خودکار &سطر = یہ+color_سطر_2;
    اگر (ماخذ[2] == ماخذ[5] || !سطر.stops.len) واپس transparent ();
    ساخت stop_t { دوہرا انحراف, alpha; غیرعلامتی palette, ترتیب; };
    hb_سمتیہ_t<stop_t> stops;
    اگر (!stops.alloc (سطر.stops.len)) واپس جھوٹ;
    غیرعلامتی ترتیب = 0;
    برائے (ثابت خودکار &اندراج_2 : سطر.stops.iter ())
    {
      ثابت خودکار &stop = اندراج_2.get_قدر ();
      stops.push (stop_t {(دوہرا) roundf (stop.stop_انحراف.مقصد_float (instancer (اندراج_2.var_Idx_بنیاد, 0)) * 16384.f) / 16384.,
                   (دوہرا) hb_clamp (stop.alpha.مقصد_float (instancer (اندراج_2.var_Idx_بنیاد, 1)), 0.f, 1.f),
                   stop.palette_اشاریہ_2, ترتیب++});
    }
    اگر (stops.اندر_error ()) واپس جھوٹ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      واپس a.انحراف < b.انحراف ? -1 : a.انحراف > b.انحراف ? 1 :
             a.ترتیب < b.ترتیب ? -1 : a.ترتیب > b.ترتیب ? 1 : 0;
    });
    دوہرا dr = ماخذ[5] - ماخذ[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    غیرعلامتی lattice = 16384;
    برائے (غیرعلامتی j = 0; j < 3; j++)
    {
      دوہرا remainder = fmod (fabs (ماخذ[j + 3] - ماخذ[j]), 16384.);
      اگر (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
        واپس جھوٹ;
      }
      غیرعلامتی دائیں = (غیرعلامتی) remainder;
      جب (دائیں) { غیرعلامتی r = lattice % دائیں; lattice = دائیں; دائیں = r; }
    }
    برائے (غیرعلامتی سرکاؤ = 0; سرکاؤ < 16; سرکاؤ++)
    {
      دوہرا b = 1u << سرکاؤ;
      دوہرا coarse = dr > 0 ? ceil (-ماخذ[2] / dr) : floor (ماخذ[2] / -dr) - b;
      دوہرا fine = dr > 0 ? ceil (-ماخذ[2] / dr * lattice) / lattice :
                            floor (ماخذ[2] / -dr * lattice) / lattice - b;
      دوہرا candidates[] = {coarse, fine};
      برائے (دوہرا a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        دوہرا geometry[6];
        منطقی سماتا = سچ;
        برائے (غیرعلامتی j = 0; j < 3; j++)
        {
          دوہرا difference = ماخذ[j + 3] - ماخذ[j];
          geometry[j] = ماخذ[j] + a * difference;
          geometry[j + 3] = ماخذ[j] + (a + b) * difference;
          دوہرا کم = j == 2 ? 0 : -32768, بلند = j == 2 ? 65535 : 32767;
          سماتا &= geometry[j] >= کم && geometry[j] <= بلند &&
                  geometry[j + 3] >= کم && geometry[j + 3] <= بلند;
        }
        اگر (!سماتا) جاری;
        غیرعلامتی پہلا = 0, گنتی = stops.لمبائی;
        دوہرا min = (stops[0].انحراف - a) / b, max = (stops.دم ().انحراف - a) / b;
        دوہرا bias = 0;
        منطقی constant = جھوٹ;
        اگر (سطر.extend == Extend::EXTEND_PAD)
        {
          دوہرا tip = -geometry[2] / (b * dr);
          اگر ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            پہلا = dr > 0 ? stops.لمبائی - 1 : 0;
            گنتی = 1; constant = سچ;
          }
        }
        ورنہ
        {
          دوہرا period = max - min;
          اگر (سطر.extend == Extend::EXTEND_REFLECT) period *= 2;
          اگر (period <= 0) واپس transparent ();
          دوہرا n = ceil ((-2 - min) / period);
          اگر (n > floor ((32767. / 16384. - max) / period)) جاری;
          bias = n * period;
        }
        برائے (غیرعلامتی j = 0; j < گنتی; j++)
        {
          دوہرا بٹ = constant ? 0 : ((stops[پہلا + j].انحراف - a) / b + bias) * 16384;
          سماتا &= بٹ >= -32768 && بٹ <= 32767 && بٹ == round (بٹ);
        }
        اگر (!سماتا) جاری;
        خودکار *برآمد = c->serializer->allocate_سائز<PaintRadialGradient<NoVariable>> (16);
        اگر (!برآمد) واپس جھوٹ;
        برآمد->format = 6;
        برآمد->x0 = (صحیح) geometry[0]; برآمد->y0 = (صحیح) geometry[1]; برآمد->radius0 = (غیرعلامتی) geometry[2];
        برآمد->x1 = (صحیح) geometry[3]; برآمد->y1 = (صحیح) geometry[4]; برآمد->radius1 = (غیرعلامتی) geometry[5];
        c->serializer->push ();
        خودکار *برآمد_سطر = c->serializer->allocate_سائز<Color_سطر<NoVariable>> (3 + گنتی * ColorStop::static_سائز);
        اگر (!برآمد_سطر) { c->serializer->نکالو_discard (); واپس جھوٹ; }
        برآمد_سطر->extend = سطر.extend;
        برآمد_سطر->stops.len = گنتی;
        برائے (غیرعلامتی j = 0; j < گنتی; j++)
        {
          ثابت خودکار &stop = stops[پہلا + j];
          خودکار &برآمد_stop = برآمد_سطر->stops[j].value;
          برآمد_stop.stop_انحراف.سیٹ_float ((اعشاری) (constant ? 0 : (stop.انحراف - a) / b + bias));
          برآمد_stop.alpha.سیٹ_float ((اعشاری) stop.alpha);
          اگر (!c->serializer->check_تفویض (برآمد_stop.palette_اشاریہ_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
          { c->serializer->نکالو_discard (); واپس جھوٹ; }
        }
        c->serializer->جوڑو_link (برآمد->color_سطر_2, c->serializer->نکالو_pack ());
        واپس !c->serializer->اندر_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
    واپس جھوٹ;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_بنیاد != VarIdx::NO_VARIATION &&
        (radius0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)) < 0 ||
         radius1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_بنیاد));
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      برآمد->x0 = hb_clamp_مقصد<int16_t> ((دوہرا) x0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 0)));
      برآمد->y0 = hb_clamp_مقصد<int16_t> ((دوہرا) y0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
      برآمد->radius0 = hb_clamp_مقصد<uint16_t> ((دوہرا) radius0 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
      برآمد->x1 = hb_clamp_مقصد<int16_t> ((دوہرا) x1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 3)));
      برآمد->y1 = hb_clamp_مقصد<int16_t> ((دوہرا) y1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 4)));
      برآمد->radius1 = hb_clamp_مقصد<uint16_t> ((دوہرا) radius1 + (دوہرا) roundf (instancer (var_Idx_بنیاد, 5)));
    }

    اگر (format == 7 && c->plan->all_axes_pinned)
        برآمد->format = 6;

    return_trace (برآمد->color_سطر_2.serialize_subset (c, color_سطر_2, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && color_سطر_2.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    hb_color_سطر_t cl = {
      (خالی *) &(یہ+color_سطر_2),
      (یہ+color_سطر_2).static_get_color_stops, c,
      (یہ+color_سطر_2).static_get_extend, خالی_اشاریہ
    };

    c->funcs->radial_gradient (c->ڈیٹا, &cl,
			       x0 + c->instancer (var_Idx_بنیاد, 0),
			       y0 + c->instancer (var_Idx_بنیاد, 1),
			       radius0 + c->instancer (var_Idx_بنیاد, 2),
			       x1 + c->instancer (var_Idx_بنیاد, 3),
			       y1 + c->instancer (var_Idx_بنیاد, 4),
			       radius1 + c->instancer (var_Idx_بنیاد, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_مقصد<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  عوامی:
  DEFINE_سائز_STATIC (4 + 6 * FWORD::static_سائز);
};

سانچہ <سانچہ<قسم_نام> جماعت Var>
ساخت PaintSweepGradient
{
  خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    (یہ+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 4;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      برآمد->centerX = hb_clamp_مقصد<int16_t> ((دوہرا) centerX + (دوہرا) roundf (instancer (var_Idx_بنیاد, 0)));
      برآمد->centerY = hb_clamp_مقصد<int16_t> ((دوہرا) centerY + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
      سیٹ_fixed_clamped (برآمد->آغاز_Angle, آغاز_Angle.مقصد_float (instancer (var_Idx_بنیاد, 2)));
      سیٹ_fixed_clamped (برآمد->انتہا_Angle, انتہا_Angle.مقصد_float (instancer (var_Idx_بنیاد, 3)));
    }

    اگر (format == 9 && c->plan->all_axes_pinned)
        برآمد->format = 8;

    return_trace (برآمد->color_سطر_2.serialize_subset (c, color_سطر_2, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && color_سطر_2.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    hb_color_سطر_t cl = {
      (خالی *) &(یہ+color_سطر_2),
      (یہ+color_سطر_2).static_get_color_stops, c,
      (یہ+color_سطر_2).static_get_extend, خالی_اشاریہ
    };

    c->funcs->sweep_gradient (c->ڈیٹا, &cl,
			      centerX + c->instancer (var_Idx_بنیاد, 0),
			      centerY + c->instancer (var_Idx_بنیاد, 1),
                              (آغاز_Angle.مقصد_float (c->instancer (var_Idx_بنیاد, 2)) + 1) * HB_PI,
                              (انتہا_Angle.مقصد_float   (c->instancer (var_Idx_بنیاد, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_مقصد<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		آغاز_Angle;
  F2DOT14		انتہا_Angle;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * FWORD::static_سائز + 2 * F2DOT14::static_سائز);
};

// Paint a non-COLR glyph, filled as indicated by paint.
سانچہ <قسم_نام Types>
ساخت PaintGlyph
{
  استعمال HBGlyphID = قسم_نام Types::HBGlyphID;

  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (! c->serializer->check_تفویض (برآمد->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    return_trace (برآمد->paint.serialize_subset (c, paint, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && paint.sanitize (c, یہ));
  }

  اندرونی خالی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24_مقصد<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  عوامی:
  DEFINE_سائز_STATIC (4 + HBGlyphID::static_سائز);
};

ساخت PaintColrGlyph
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer HB_غیر_مستعمل) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    return_trace (c->serializer->check_تفویض (برآمد->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  اندرونی خالی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  عوامی:
  DEFINE_سائز_STATIC (3);
};

سانچہ <سانچہ<قسم_نام> جماعت Var>
ساخت PaintTransform
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    اگر (!برآمد->transform.serialize_subset (c, transform, یہ, instancer)) return_trace (جھوٹ);
    اگر (format == 13 && c->plan->all_axes_pinned)
      برآمد->format = 12;
    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  src.sanitize (c, یہ) &&
                  transform.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (یہ);
    (یہ+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_مقصد<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_مقصد<Var<Affine2x3>>	transform;
  عوامی:
  DEFINE_سائز_STATIC (7);
};

ساخت PaintTranslate
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      برآمد->dx = hb_clamp_مقصد<int16_t> ((دوہرا) dx + (دوہرا) roundf (instancer (var_Idx_بنیاد, 0)));
      برآمد->dy = hb_clamp_مقصد<int16_t> ((دوہرا) dy + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
    }

    اگر (format == 15 && c->plan->all_axes_pinned)
        برآمد->format = 14;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری ddx = dx + c->instancer (var_Idx_بنیاد, 0);
    اعشاری ddy = dy + c->instancer (var_Idx_بنیاد, 1);

    c->funcs->ڈالو_translate (c->ڈیٹا, ddx, ddy);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * FWORD::static_سائز);
};

سانچہ <قسم_نام ماخذ_2>
ساکن منطقی serialize_scale_transform (hb_subset_context_t *c,
                                       اعشاری sx, اعشاری sy, اعشاری dx, اعشاری dy,
                                       ثابت ماخذ_2 &ماخذ)
{
  خودکار *برآمد = c->serializer->allocate_سائز<PaintTransform<NoVariable>> (7);
  اگر (unlikely (!برآمد)) واپس جھوٹ;
  برآمد->format = 12;

  c->serializer->push ();
  خودکار *transform = c->serializer->allocate_سائز<Affine2x3> (Affine2x3::static_سائز);
  اگر (unlikely (!transform))
  {
    c->serializer->نکالو_discard ();
    واپس جھوٹ;
  }
  سیٹ_fixed_clamped (transform->xx, sx);
  transform->yx.سیٹ_float (0);
  transform->xy.سیٹ_float (0);
  سیٹ_fixed_clamped (transform->yy, sy);
  سیٹ_fixed_clamped (transform->dx, dx);
  سیٹ_fixed_clamped (transform->dy, dy);
  c->serializer->جوڑو_link (برآمد->transform, c->serializer->نکالو_pack ());

  c->serializer->push ();
  منطقی ret = ماخذ ();
  اگر (ret)
    c->serializer->جوڑو_link (برآمد->src, c->serializer->نکالو_pack ());
  ورنہ
    c->serializer->نکالو_discard ();
  واپس ret;
}

سانچہ <قسم_نام بنیاد_3>
ساکن منطقی subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت بنیاد_3 *بنیاد,
                                      ثابت Offset24_مقصد<Paint> &src,
                                      ثابت شے_Var_Store_Instancer &instancer,
                                      اعشاری sx, اعشاری sy,
                                      اعشاری cx = 0, اعشاری cy = 0)
{
  اگر (src.is_null ()) واپس جھوٹ;
  خودکار ماخذ = [&] { واپس c->dispatch (بنیاد+src, instancer); };
  اعشاری dx = cx * (1 - sx), dy = cy * (1 - sy);
  اگر (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    واپس serialize_scale_transform (c, sx, sy, dx, dy, ماخذ);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  خودکار inner = [&] {
    واپس serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, ماخذ);
  };
  خودکار inverse = [&] {
    واپس serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  خودکار scale = [&] {
    واپس serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  خودکار outer = [&] {
    واپس serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  واپس serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

ساخت PaintScale
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      اعشاری sx = roundf (scaleX.مقصد_float (instancer (var_Idx_بنیاد, 0)) * 16384) / 16384;
      اعشاری sy = roundf (scaleY.مقصد_float (instancer (var_Idx_بنیاد, 1)) * 16384) / 16384;
      اگر (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, یہ, src, instancer, sx, sy));
    }
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->scaleX, scaleX.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      سیٹ_fixed_clamped (برآمد->scaleY, scaleY.مقصد_float (instancer (var_Idx_بنیاد, 1)));
    }

    اگر (format == 17 && c->plan->all_axes_pinned)
        برآمد->format = 16;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری sx = scaleX.مقصد_float (c->instancer (var_Idx_بنیاد, 0));
    اعشاری sy = scaleY.مقصد_float (c->instancer (var_Idx_بنیاد, 1));

    c->funcs->ڈالو_scale (c->ڈیٹا, sx, sy);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * F2DOT14::static_سائز);
};

ساخت PaintScaleAroundCenter
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      اعشاری sx = roundf (scaleX.مقصد_float (instancer (var_Idx_بنیاد, 0)) * 16384) / 16384;
      اعشاری sy = roundf (scaleY.مقصد_float (instancer (var_Idx_بنیاد, 1)) * 16384) / 16384;
      اگر (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, یہ, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_بنیاد, 2)),
            centerY + roundf (instancer (var_Idx_بنیاد, 3))));
    }
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->scaleX, scaleX.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      سیٹ_fixed_clamped (برآمد->scaleY, scaleY.مقصد_float (instancer (var_Idx_بنیاد, 1)));
      برآمد->centerX = hb_clamp_مقصد<int16_t> ((دوہرا) centerX + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
      برآمد->centerY = hb_clamp_مقصد<int16_t> ((دوہرا) centerY + (دوہرا) roundf (instancer (var_Idx_بنیاد, 3)));
    }

    اگر (format == 19 && c->plan->all_axes_pinned)
        برآمد->format = 18;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری sx = scaleX.مقصد_float (c->instancer (var_Idx_بنیاد, 0));
    اعشاری sy = scaleY.مقصد_float (c->instancer (var_Idx_بنیاد, 1));
    اعشاری tCenterX = centerX + c->instancer (var_Idx_بنیاد, 2);
    اعشاری tCenterY = centerY + c->instancer (var_Idx_بنیاد, 3);

    c->funcs->ڈالو_scale_around_center (c->ڈیٹا, sx, sy, tCenterX, tCenterY);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * F2DOT14::static_سائز + 2 * FWORD::static_سائز);
};

ساخت Paint_Scale_یکساں
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      اعشاری s = roundf (scale.مقصد_float (instancer (var_Idx_بنیاد, 0)) * 16384) / 16384;
      اگر (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, یہ, src, instancer, s, s));
    }
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
      سیٹ_fixed_clamped (برآمد->scale, scale.مقصد_float (instancer (var_Idx_بنیاد, 0)));

    اگر (format == 21 && c->plan->all_axes_pinned)
        برآمد->format = 20;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری s = scale.مقصد_float (c->instancer (var_Idx_بنیاد, 0));

    c->funcs->ڈالو_scale (c->ڈیٹا, s, s);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عوامی:
  DEFINE_سائز_STATIC (4 + F2DOT14::static_سائز);
};

ساخت Paint_Scale_یکساں_Around_Center
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      اعشاری s = roundf (scale.مقصد_float (instancer (var_Idx_بنیاد, 0)) * 16384) / 16384;
      اگر (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, یہ, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_بنیاد, 1)),
            centerY + roundf (instancer (var_Idx_بنیاد, 2))));
    }
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->scale, scale.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      برآمد->centerX = hb_clamp_مقصد<int16_t> ((دوہرا) centerX + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
      برآمد->centerY = hb_clamp_مقصد<int16_t> ((دوہرا) centerY + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
    }

    اگر (format == 23 && c->plan->all_axes_pinned)
        برآمد->format = 22;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری s = scale.مقصد_float (c->instancer (var_Idx_بنیاد, 0));
    اعشاری tCenterX = centerX + c->instancer (var_Idx_بنیاد, 1);
    اعشاری tCenterY = centerY + c->instancer (var_Idx_بنیاد, 2);

    c->funcs->ڈالو_scale_around_center (c->ڈیٹا, s, s, tCenterX, tCenterY);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عوامی:
  DEFINE_سائز_STATIC (4 + F2DOT14::static_سائز + 2 * FWORD::static_سائز);
};

ساخت PaintRotate
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
      سیٹ_fixed_clamped (برآمد->angle, angle.مقصد_float (instancer (var_Idx_بنیاد, 0)));

    اگر (format == 25 && c->plan->all_axes_pinned)
      برآمد->format = 24;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری a = angle.مقصد_float (c->instancer (var_Idx_بنیاد, 0));

    c->funcs->ڈالو_rotate (c->ڈیٹا, a);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عوامی:
  DEFINE_سائز_STATIC (4 + F2DOT14::static_سائز);
};

ساخت PaintRotateAroundCenter
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->angle, angle.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      برآمد->centerX = hb_clamp_مقصد<int16_t> ((دوہرا) centerX + (دوہرا) roundf (instancer (var_Idx_بنیاد, 1)));
      برآمد->centerY = hb_clamp_مقصد<int16_t> ((دوہرا) centerY + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
    }

    اگر (format ==27 && c->plan->all_axes_pinned)
        برآمد->format = 26;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری a = angle.مقصد_float (c->instancer (var_Idx_بنیاد, 0));
    اعشاری tCenterX = centerX + c->instancer (var_Idx_بنیاد, 1);
    اعشاری tCenterY = centerY + c->instancer (var_Idx_بنیاد, 2);

    c->funcs->ڈالو_rotate_around_center (c->ڈیٹا, a, tCenterX, tCenterY);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عوامی:
  DEFINE_سائز_STATIC (4 + F2DOT14::static_سائز + 2 * FWORD::static_سائز);
};

ساخت PaintSkew
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->xSkewAngle, xSkewAngle.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      سیٹ_fixed_clamped (برآمد->ySkewAngle, ySkewAngle.مقصد_float (instancer (var_Idx_بنیاد, 1)));
    }

    اگر (format == 29 && c->plan->all_axes_pinned)
        برآمد->format = 28;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری sx = xSkewAngle.مقصد_float(c->instancer (var_Idx_بنیاد, 0));
    اعشاری sy = ySkewAngle.مقصد_float(c->instancer (var_Idx_بنیاد, 1));

    c->funcs->ڈالو_skew (c->ڈیٹا, sx, sy);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * F2DOT14::static_سائز);
};

ساخت PaintSkewAroundCenter
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (برآمد->xSkewAngle, xSkewAngle.مقصد_float (instancer (var_Idx_بنیاد, 0)));
      سیٹ_fixed_clamped (برآمد->ySkewAngle, ySkewAngle.مقصد_float (instancer (var_Idx_بنیاد, 1)));
      برآمد->centerX = hb_clamp_مقصد<int16_t> ((دوہرا) centerX + (دوہرا) roundf (instancer (var_Idx_بنیاد, 2)));
      برآمد->centerY = hb_clamp_مقصد<int16_t> ((دوہرا) centerY + (دوہرا) roundf (instancer (var_Idx_بنیاد, 3)));
    }

    اگر (format == 31 && c->plan->all_axes_pinned)
        برآمد->format = 30;

    return_trace (برآمد->src.serialize_subset (c, src, یہ, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && src.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_PAINT (یہ);
    اعشاری sx = xSkewAngle.مقصد_float(c->instancer (var_Idx_بنیاد, 0));
    اعشاری sy = ySkewAngle.مقصد_float(c->instancer (var_Idx_بنیاد, 1));
    اعشاری tCenterX = centerX + c->instancer (var_Idx_بنیاد, 2);
    اعشاری tCenterY = centerY + c->instancer (var_Idx_بنیاد, 3);

    c->funcs->ڈالو_skew_around_center (c->ڈیٹا, sx, sy, tCenterX, tCenterY);
    c->recurse (یہ+src);
    c->funcs->نکالو_transform (c->ڈیٹا);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عوامی:
  DEFINE_سائز_STATIC (4 + 2 * F2DOT14::static_سائز + 2 * FWORD::static_سائز);
};

ساخت Paint_مرکب
{
  HB_اندرونی خالی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    منطقی ret = جھوٹ;
    ret |= برآمد->src.serialize_subset (c, src, یہ, instancer);
    ret |= برآمد->backdrop.serialize_subset (c, backdrop, یہ, instancer);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  c->check_ops (یہ->min_سائز) && // PainComposite can get exponential
                  src.sanitize (c, یہ) &&
                  backdrop.sanitize (c, یہ));
  }

  خالی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (یہ);
    hb_paint_مرکب_منوال_t مرکب_منوال_2;
    انتخاب ((غیرعلامتی صحیح) منوال)
    {
      حالت HB_PAINT_مرکب_منوال_CLEAR:
      حالت HB_PAINT_مرکب_منوال_SRC:
      حالت HB_PAINT_مرکب_منوال_DEST:
      حالت HB_PAINT_مرکب_منوال_SRC_OVER:
      حالت HB_PAINT_مرکب_منوال_DEST_OVER:
      حالت HB_PAINT_مرکب_منوال_SRC_اندر:
      حالت HB_PAINT_مرکب_منوال_DEST_اندر:
      حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
      حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
      حالت HB_PAINT_مرکب_منوال_SRC_ATOP:
      حالت HB_PAINT_مرکب_منوال_DEST_ATOP:
      حالت HB_PAINT_مرکب_منوال_XOR:
      حالت HB_PAINT_مرکب_منوال_PLUS:
      حالت HB_PAINT_مرکب_منوال_SCREEN:
      حالت HB_PAINT_مرکب_منوال_OVERLAY:
      حالت HB_PAINT_مرکب_منوال_DARKEN:
      حالت HB_PAINT_مرکب_منوال_LIGHTEN:
      حالت HB_PAINT_مرکب_منوال_COLOR_DODGE:
      حالت HB_PAINT_مرکب_منوال_COLOR_BURN:
      حالت HB_PAINT_مرکب_منوال_HARD_LIGHT:
      حالت HB_PAINT_مرکب_منوال_SOFT_LIGHT:
      حالت HB_PAINT_مرکب_منوال_DIFFERENCE:
      حالت HB_PAINT_مرکب_منوال_EXCLUSION:
      حالت HB_PAINT_مرکب_منوال_MULTIPLY:
      حالت HB_PAINT_مرکب_منوال_HSL_HUE:
      حالت HB_PAINT_مرکب_منوال_HSL_SATURATION:
      حالت HB_PAINT_مرکب_منوال_HSL_COLOR:
      حالت HB_PAINT_مرکب_منوال_HSL_LUMINOSITY:
	مرکب_منوال_2 = (hb_paint_مرکب_منوال_t) (غیرعلامتی صحیح) منوال;
	توڑو;
      طےشدہ:
	مرکب_منوال_2 = HB_PAINT_مرکب_منوال_CLEAR;
	توڑو;
    }

    c->funcs->ڈالو_گروہ_for (c->ڈیٹا, HB_PAINT_مرکب_منوال_SRC_OVER);
    c->recurse (یہ+backdrop);
    c->funcs->ڈالو_گروہ_for (c->ڈیٹا, مرکب_منوال_2);
    c->recurse (یہ+src);
    c->funcs->نکالو_گروہ (c->ڈیٹا, مرکب_منوال_2);
    c->funcs->نکالو_گروہ (c->ڈیٹا, HB_PAINT_مرکب_منوال_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  مرکب_منوال		منوال;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_مقصد<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Clip_Box_ڈیٹا
{
  صحیح xMin, yMin, xMax, yMax;
};

ساخت ClipBoxFormat1
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_clip_box (Clip_Box_ڈیٹا &clip_box, ثابت شے_Var_Store_Instancer &instancer HB_غیر_مستعمل) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (instancer && !c->plan->pinned_at_default && var_Idx_بنیاد != VarIdx::NO_VARIATION)
    {
      برآمد->xMin = hb_clamp_مقصد<int16_t> ((دوہرا) xMin + (دوہرا) floorf (instancer (var_Idx_بنیاد, 0)));
      برآمد->yMin = hb_clamp_مقصد<int16_t> ((دوہرا) yMin + (دوہرا) floorf (instancer (var_Idx_بنیاد, 1)));
      برآمد->xMax = hb_clamp_مقصد<int16_t> ((دوہرا) xMax + (دوہرا) ceilf (instancer (var_Idx_بنیاد, 2)));
      برآمد->yMax = hb_clamp_مقصد<int16_t> ((دوہرا) yMax + (دوہرا) ceilf (instancer (var_Idx_بنیاد, 3)));
    }

    اگر (format == 2 && c->plan->all_axes_pinned)
        برآمد->format = 1;

    return_trace (سچ);
  }

  عوامی:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عوامی:
  DEFINE_سائز_STATIC (1 + 4 * FWORD::static_سائز);
};

ساخت ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  خالی get_clip_box (Clip_Box_ڈیٹا &clip_box, ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    اگر (instancer)
    {
      clip_box.xMin = hb_saturate_جوڑو (clip_box.xMin, hb_clamp_مقصد<صحیح> (floorf (instancer (var_Idx_بنیاد, 0))));
      clip_box.yMin = hb_saturate_جوڑو (clip_box.yMin, hb_clamp_مقصد<صحیح> (floorf (instancer (var_Idx_بنیاد, 1))));
      clip_box.xMax = hb_saturate_جوڑو (clip_box.xMax, hb_clamp_مقصد<صحیح> (ceilf (instancer (var_Idx_بنیاد, 2))));
      clip_box.yMax = hb_saturate_جوڑو (clip_box.yMax, hb_clamp_مقصد<صحیح> (ceilf (instancer (var_Idx_بنیاد, 3))));
    }
  }

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  { c->variation_indices->جوڑو_range (var_Idx_بنیاد, var_Idx_بنیاد + 3); }
};

ساخت ClipBox
{
  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالت 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 2: hb_barrier (); u.format2.closurev1 (c); واپس;
    طےشدہ:واپس;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  منطقی get_extents (hb_glyph_extents_t *extents,
                    ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    Clip_Box_ڈیٹا clip_box;
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      توڑو;
    حالت 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      توڑو;
    طےشدہ:
      واپس جھوٹ;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->چوڑائی = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->اونچائی = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    واپس سچ;
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

ساخت Clip_ریکارڈ
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < آغاز_Glyph_ID ? -1 : g <= انتہا_Glyph_ID ? 0 : +1; }

  خالی closurev1 (hb_colrv1_احاطہ_context_t* c, ثابت خالی *بنیاد) ثابت
  {
    اگر (!c->glyphs->intersects (آغاز_Glyph_ID, انتہا_Glyph_ID)) واپس;
    (بنیاد+clipBox).closurev1 (c);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت خالی *بنیاد,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    return_trace (برآمد->clipBox.serialize_subset (c, clipBox, بنیاد, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && clipBox.sanitize (c, بنیاد));
  }

  منطقی get_extents (hb_glyph_extents_t *extents,
		    ثابت خالی *بنیاد,
		    ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    واپس (بنیاد+clipBox).get_extents (extents, instancer);
  }

  عوامی:
  HBUINT16		آغاز_Glyph_ID;  // first gid clip applies to
  HBUINT16		انتہا_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_مقصد<ClipBox>	clipBox;   // Box or VarBox
  عوامی:
  DEFINE_سائز_STATIC (7);
};
DECLARE_NULL_NAMESPACE_بائٹیں (OT, Clip_ریکارڈ);

ساخت ClipList
{
  غیرعلامتی serialize_clip_اندراجات (hb_subset_context_t *c,
                                   ثابت شے_Var_Store_Instancer &instancer,
                                   ثابت hb_سیٹ_t& gids,
                                   ثابت hb_map_t& gid_انحراف_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    اگر (gids.is_empty () ||
        gid_انحراف_map.get_population () != gids.get_population ())
      return_trace (0);

    غیرعلامتی گنتی  = 0;

    hb_codepoint_t آغاز_gid= gids.get_min ();
    hb_codepoint_t پچھلا_gid = آغاز_gid;

    غیرعلامتی انحراف = gid_انحراف_map.get (آغاز_gid);
    غیرعلامتی پچھلا_انحراف = انحراف;
    برائے (ثابت hb_codepoint_t _ : gids.iter ())
    {
      اگر (_ == آغاز_gid) جاری;

      انحراف = gid_انحراف_map.get (_);
      اگر (_ == پچھلا_gid + 1 &&  انحراف == پچھلا_انحراف)
      {
        پچھلا_gid = _;
        جاری;
      }

      Clip_ریکارڈ ریکارڈ;
      ریکارڈ.آغاز_Glyph_ID = آغاز_gid;
      ریکارڈ.انتہا_Glyph_ID = پچھلا_gid;
      ریکارڈ.clipBox = پچھلا_انحراف;

      اگر (!ریکارڈ.subset (c, یہ, instancer)) return_trace (0);
      گنتی++;

      آغاز_gid = _;
      پچھلا_gid = _;
      پچھلا_انحراف = انحراف;
    }

    //last one
    {
      Clip_ریکارڈ ریکارڈ;
      ریکارڈ.آغاز_Glyph_ID = آغاز_gid;
      ریکارڈ.انتہا_Glyph_ID = پچھلا_gid;
      ریکارڈ.clipBox = پچھلا_انحراف;
      اگر (!ریکارڈ.subset (c, یہ, instancer)) return_trace (0);
      گنتی++;
    }
    return_trace (گنتی);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    اگر (!c->serializer->check_تفویض (برآمد->format, format, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) return_trace (جھوٹ);

    ثابت hb_سیٹ_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_انحراف_map;
    hb_سیٹ_t new_gids;
    برائے (ثابت Clip_ریکارڈ& ریکارڈ : clips.iter ())
    {
      غیرعلامتی آغاز_gid = ریکارڈ.آغاز_Glyph_ID;
      غیرعلامتی انتہا_gid = ریکارڈ.انتہا_Glyph_ID;
      برائے (غیرعلامتی gid = آغاز_gid; gid <= انتہا_gid; gid++)
      {
        اگر (!glyphset.has (gid) || !glyph_map.has (gid)) جاری;
        غیرعلامتی new_gid = glyph_map.get (gid);
        new_gid_انحراف_map.سیٹ (new_gid, ریکارڈ.clipBox);
        new_gids.جوڑو (new_gid);
      }
    }

    غیرعلامتی گنتی = serialize_clip_اندراجات (c, instancer, new_gids, new_gid_انحراف_map);
    اگر (!گنتی) return_trace (جھوٹ);
    return_trace (c->serializer->check_تفویض (برآمد->clips.len, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (یہ) && clips.sanitize (c, یہ));
  }

  منطقی
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    خودکار *rec = clips.as_array ().bsearch (gid);
    اگر (rec)
      واپس rec->get_extents (extents, یہ, instancer);
    واپس جھوٹ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_کا<Clip_ریکارڈ>	clips;  // Clip records, sorted by startGlyphID
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (5, clips);
};

ساخت Paint
{

  سانچہ <قسم_نام ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (unlikely (!c->check_آغاز_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->انتہا_recursion (یہ->dispatch (c, std::forward<Ts> (ds)...)));
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    حالت 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    حالت 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    حالت 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    حالت 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    حالت 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    حالت 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    حالت 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    حالت 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    حالت 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    حالت 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    حالت 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    حالت 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    حالت 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    حالت 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    حالت 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    حالت 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    حالت 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    حالت 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    حالت 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    حالت 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    حالت 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    حالت 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    حالت 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    حالت 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    حالت 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    حالت 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    حالت 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  منطقی get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    انتخاب (u.format.v) {
    حالت 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); واپس سچ;
    حالت 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); واپس سچ;
    طےشدہ: واپس جھوٹ;
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<Paint_خطی_Gradient<NoVariable>>	paintformat4;
  Variable<Paint_خطی_Gradient<Variable>>	paintformat5;
  NoVariable<PaintRadialGradient<NoVariable>>	paintformat6;
  Variable<PaintRadialGradient<Variable>>	paintformat7;
  NoVariable<PaintSweepGradient<NoVariable>>	paintformat8;
  Variable<PaintSweepGradient<Variable>>	paintformat9;
  PaintGlyph<Layout::SmallTypes>		paintformat10;
  PaintColrGlyph				paintformat11;
  PaintTransform<NoVariable>			paintformat12;
  PaintTransform<Variable>			paintformat13;
  NoVariable<PaintTranslate>			paintformat14;
  Variable<PaintTranslate>			paintformat15;
  NoVariable<PaintScale>			paintformat16;
  Variable<PaintScale>				paintformat17;
  NoVariable<PaintScaleAroundCenter>		paintformat18;
  Variable<PaintScaleAroundCenter>		paintformat19;
  NoVariable<Paint_Scale_یکساں>			paintformat20;
  Variable<Paint_Scale_یکساں>			paintformat21;
  NoVariable<Paint_Scale_یکساں_Around_Center>	paintformat22;
  Variable<Paint_Scale_یکساں_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_مرکب				paintformat32;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_MIN (2);
};

ساخت بنیاد_Glyph_Paint_ریکارڈ
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقی serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت خالی* src_بنیاد, hb_subset_context_t *c,
                  ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = s->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    اگر (!s->check_تفویض (برآمد->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    return_trace (برآمد->paint.serialize_subset (c, paint, src_بنیاد, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) && paint.sanitize (c, بنیاد)));
  }

  عوامی:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_مقصد<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت بنیاد_Glyph_List : Sorted_Array32_کا<بنیاد_Glyph_Paint_ریکارڈ>
{
  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد)))  return_trace (جھوٹ);
    ثابت hb_سیٹ_t* glyphset = &c->plan->_glyphset_colred;

    برائے (ثابت خودکار& _ : as_array ())
    {
      غیرعلامتی gid = _.glyphId;
      اگر (!glyphset->has (gid)) جاری;

      اگر (_.serialize (c->serializer, c->plan->glyph_map, یہ, c, instancer)) برآمد->len++;
      ورنہ return_trace (جھوٹ);
    }

    return_trace (برآمد->len != 0);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (Sorted_Array32_کا<بنیاد_Glyph_Paint_ریکارڈ>::sanitize (c, یہ));
  }
};

ساخت LayerList : Array32_کا_Offset32_مقصد<Paint>
{
  ثابت Paint& get_paint (غیرعلامتی i) ثابت
  { واپس یہ+(*یہ)[i]; }

  منطقی subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد)))  return_trace (جھوٹ);

    منطقی ret = جھوٹ;
    برائے (ثابت خودکار& _ : + hb_enumerate (*یہ)
                         | hb_filter (c->plan->colrv1_layers, hb_پہلا))

    {
      خودکار *o = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (جھوٹ);
      ret |= o->serialize_subset (c, _.second, یہ, instancer);
    }
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (Array32_کا_Offset32_مقصد<Paint>::sanitize (c, یہ));
  }
};

ساخت delta_سیٹ_اشاریہ_map_subset_plan_t
{
  غیرعلامتی get_inner_ثنائی_ہندسہ_گنتی () ثابت { واپس inner_ثنائی_ہندسہ_گنتی; }
  غیرعلامتی get_چوڑائی ()           ثابت { واپس ((outer_ثنائی_ہندسہ_گنتی + inner_ثنائی_ہندسہ_گنتی + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { واپس output_map.as_array (); }

  delta_سیٹ_اشاریہ_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_گنتی = 0;
    outer_ثنائی_ہندسہ_گنتی = 0;
    inner_ثنائی_ہندسہ_گنتی = 1;
    output_map.init ();

    /* search backwards */
    غیرعلامتی گنتی = new_deltaset_idx_varidx_map.get_population ();
    اگر (!گنتی) واپس;

    غیرعلامتی آخری_idx = (غیرعلامتی)-1;
    غیرعلامتی آخری_varidx = (غیرعلامتی)-1;

    برائے (غیرعلامتی i = گنتی; i; i--)
    {
      غیرعلامتی delta_سیٹ_idx = i - 1;
      غیرعلامتی var_idx = new_deltaset_idx_varidx_map.get (delta_سیٹ_idx);
      اگر (i == گنتی)
      {
        آخری_idx = delta_سیٹ_idx;
        آخری_varidx = var_idx;
        جاری;
      }
      اگر (var_idx != آخری_varidx)
        توڑو;
      آخری_idx = delta_سیٹ_idx;
    }

    map_گنتی = آخری_idx + 1;
  }

  منطقی remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    اگر (unlikely (!output_map.resize_dirty (map_گنتی))) واپس جھوٹ;

    برائے (غیرعلامتی idx = 0; idx < map_گنتی; idx++)
    {
      uint32_t *var_idx;
      اگر (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) واپس جھوٹ;
      output_map.arrayZ[idx] = *var_idx;

      غیرعلامتی outer = (*var_idx) >> 16;
      غیرعلامتی ثنائی_ہندسہ_گنتی = (outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      غیرعلامتی inner = (*var_idx) & 0xFFFF;
      ثنائی_ہندسہ_گنتی = (inner == 0) ? 1 : hb_ثنائی_ہندسہ_storage (inner);
      inner_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, inner_ثنائی_ہندسہ_گنتی);
    }
    واپس سچ;
  }

  نجی:
  غیرعلامتی map_گنتی;
  غیرعلامتی outer_ثنائی_ہندسہ_گنتی;
  غیرعلامتی inner_ثنائی_ہندسہ_گنتی;
  hb_سمتیہ_t<uint32_t> output_map;
};

ساخت COLR
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_COLR;

  منطقی has_ڈیٹا () ثابت { واپس has_v0_ڈیٹا () || نسخہ; }

  منطقی has_v0_ڈیٹا () ثابت { واپس عدد_بنیاد_Glyphs; }
  منطقی has_v1_ڈیٹا () ثابت
  {
    اگر (نسخہ < 1)
      واپس جھوٹ;
    hb_barrier ();

    واپس (یہ+بنیاد_Glyph_List_2).len > 0;
  }

  غیرعلامتی صحیح get_glyph_layers (hb_codepoint_t       glyph,
				 غیرعلامتی صحیح         آغاز_انحراف,
				 غیرعلامتی صحیح        *گنتی, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت بنیاد_Glyph_ریکارڈ &ریکارڈ = (یہ+بنیاد_Glyphs_Z).bsearch (عدد_بنیاد_Glyphs, glyph);

    hb_array_t<ثابت Layer_ریکارڈ> all_layers = (یہ+layersZ).as_array (عدد_Layers);
    hb_array_t<ثابت Layer_ریکارڈ> glyph_layers = all_layers.sub_array (ریکارڈ.پہلا_Layer_Idx,
								       ریکارڈ.عدد_Layers);
    اگر (گنتی && layers)
    {
      + glyph_layers.sub_array (آغاز_انحراف, گنتی)
      | hb_sink (hb_array (layers, *گنتی))
      ;
    }
    واپس glyph_layers.لمبائی;
  }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_جدول<COLR> (face); }

    ~accelerator_t ()
    {
      خودکار *scratch = cached_scratch.get_relaxed ();
      اگر (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقی has_ڈیٹا () ثابت { واپس colr->has_ڈیٹا (); }

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
    منطقی
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      اگر (unlikely (!has_ڈیٹا ())) واپس جھوٹ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) واپس سچ;
      منطقی ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      واپس ret;
    }

    منطقی paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, خالی *ڈیٹا,
		      غیرعلامتی صحیح palette_اشاریہ,
		      hb_color_t foreground,
		      منطقی clip = سچ) ثابت
    {
      اگر (unlikely (!has_ڈیٹا ())) واپس جھوٹ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) واپس سچ;
      منطقی ret = colr->paint_glyph (font, glyph, funcs, ڈیٹا, palette_اشاریہ, foreground, clip, *scratch);
      release_scratch (scratch);
      واپس ret;
    }
#اگر_ختم

    منطقی is_valid () { واپس colr.get_blob ()->لمبائی; }

    خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
    {
      اگر (!has_ڈیٹا ()) واپس;
      colr->depend (depend_ڈیٹا);
    }

    خالی احاطہ_glyphs (hb_codepoint_t glyph,
			 hb_سیٹ_t *related_ids /*برآمد*/) ثابت
    { colr->احاطہ_glyphs (glyph, related_ids); }

    خالی احاطہ_V0palette_indices (ثابت hb_سیٹ_t *glyphs,
				    hb_سیٹ_t *palettes /*برآمد*/) ثابت
    { colr->احاطہ_V0palette_indices (glyphs, palettes); }

    خالی احاطہ_for_V1 (hb_سیٹ_t *glyphset,
                        hb_سیٹ_t *layer_indices,
                        hb_سیٹ_t *palette_indices,
                        hb_سیٹ_t *variation_indices,
                        hb_سیٹ_t *delta_سیٹ_indices) ثابت
    { colr->احاطہ_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_سیٹ_indices); }

    منطقی has_var_store () ثابت
    { واپس colr->has_var_store (); }

    ثابت شے_Variation_Store &get_var_store () ثابت
    { واپس colr->get_var_store (); }
    ثابت شے_Variation_Store *get_var_store_ptr () ثابت
    { واپس colr->get_var_store_ptr (); }

    منطقی has_delta_سیٹ_اشاریہ_map () ثابت
    { واپس colr->has_delta_سیٹ_اشاریہ_map (); }

    ثابت Delta_سیٹ_اشاریہ_Map &get_delta_سیٹ_اشاریہ_map () ثابت
    { واپس colr->get_delta_سیٹ_اشاریہ_map (); }
    ثابت Delta_سیٹ_اشاریہ_Map *get_delta_سیٹ_اشاریہ_map_ptr () ثابت
    { واپس colr->get_delta_سیٹ_اشاریہ_map_ptr (); }

    نجی:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_اشاریہ)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	اگر (unlikely (!scratch))
	  واپس خالی_اشاریہ;
      }

      واپس scratch;
    }
    خالی release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      اگر (!cached_scratch.cmpexch (خالی_اشاریہ, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عوامی:
    hb_blob_ptr_t<COLR> colr;
    نجی:
    تغیرپذیر hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
  {
    // v0
    hb_array_t<ثابت بنیاد_Glyph_ریکارڈ> بنیاد_Glyphs = (یہ+بنیاد_Glyphs_Z).as_array (عدد_بنیاد_Glyphs);
    برائے (ثابت بنیاد_Glyph_ریکارڈ &ریکارڈ : بنیاد_Glyphs)
    {
      خودکار glyph_layers = (یہ+layersZ).as_array (عدد_Layers).sub_array (ریکارڈ.پہلا_Layer_Idx,
                                                                         ریکارڈ.عدد_Layers);
      برائے (ثابت Layer_ریکارڈ layer : glyph_layers)
        depend_ڈیٹا->جوڑو_depend(ریکارڈ.glyphId, HB_OT_لیبل_COLR, layer.glyphId);
    }

    اگر (!has_v1_ڈیٹا())
      واپس;

    // v1
    hb_colrv1_depend_context_t c (یہ, depend_ڈیٹا);
    ثابت بنیاد_Glyph_List &baseglyph_paintrecords = یہ+بنیاد_Glyph_List_2;

    برائے (ثابت بنیاد_Glyph_Paint_ریکارڈ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.ماخذ_gid = baseglyph_paintrecord.glyphId;
      c.دیکھا_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  خالی احاطہ_glyphs (hb_codepoint_t glyph,
		       hb_سیٹ_t *related_ids /*برآمد*/) ثابت
  {
    ثابت بنیاد_Glyph_ریکارڈ *ریکارڈ = get_بنیاد_glyph_ریکارڈ (glyph);
    اگر (!ریکارڈ) واپس;

    خودکار glyph_layers = (یہ+layersZ).as_array (عدد_Layers).sub_array (ریکارڈ->پہلا_Layer_Idx,
								       ریکارڈ->عدد_Layers);
    اگر (!glyph_layers.لمبائی) واپس;
    related_ids->جوڑو_array (&glyph_layers[0].glyphId, glyph_layers.لمبائی, Layer_ریکارڈ::min_سائز);
  }

  خالی احاطہ_V0palette_indices (ثابت hb_سیٹ_t *glyphs,
				  hb_سیٹ_t *palettes /*برآمد*/) ثابت
  {
    اگر (!عدد_بنیاد_Glyphs || !عدد_Layers) واپس;
    hb_array_t<ثابت بنیاد_Glyph_ریکارڈ> بنیاد_Glyphs = (یہ+بنیاد_Glyphs_Z).as_array (عدد_بنیاد_Glyphs);
    hb_array_t<ثابت Layer_ریکارڈ> all_layers = (یہ+layersZ).as_array (عدد_Layers);

    برائے (ثابت بنیاد_Glyph_ریکارڈ ریکارڈ : بنیاد_Glyphs)
    {
      اگر (!glyphs->has (ریکارڈ.glyphId)) جاری;
      hb_array_t<ثابت Layer_ریکارڈ> glyph_layers = all_layers.sub_array (ریکارڈ.پہلا_Layer_Idx,
                                                                   ریکارڈ.عدد_Layers);
      برائے (ثابت Layer_ریکارڈ layer : glyph_layers)
        palettes->جوڑو (layer.colorIdx);
    }
  }

  خالی احاطہ_for_V1 (hb_سیٹ_t *glyphset,
                      hb_سیٹ_t *layer_indices,
                      hb_سیٹ_t *palette_indices,
                      hb_سیٹ_t *variation_indices,
                      hb_سیٹ_t *delta_سیٹ_indices) ثابت
  {
    اگر (نسخہ < 1) واپس;
    hb_barrier ();

    hb_سیٹ_t دیکھا_glyphs;

    hb_colrv1_احاطہ_context_t c (یہ, &دیکھا_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت بنیاد_Glyph_List &baseglyph_paintrecords = یہ+بنیاد_Glyph_List_2;

    برائے (ثابت بنیاد_Glyph_Paint_ریکارڈ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      غیرعلامتی gid = baseglyph_paintrecord.glyphId;
      اگر (!glyphset->has (gid)) جاری;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_سیٹ_union (glyphset, &دیکھا_glyphs);

    ثابت ClipList &cliplist = یہ+clipList;
    c.glyphs = glyphset;
    برائے (ثابت Clip_ریکارڈ &clip_ریکارڈ : cliplist.clips.iter())
      clip_ریکارڈ.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    اگر (has_delta_سیٹ_اشاریہ_map ())
    {
      ثابت Delta_سیٹ_اشاریہ_Map &var_idx_map = یہ+varIdxMap;
      delta_سیٹ_indices->سیٹ (*variation_indices);
      variation_indices->clear ();
      برائے (غیرعلامتی delta_سیٹ_idx : *delta_سیٹ_indices)
        variation_indices->جوڑو (var_idx_map.map (delta_سیٹ_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { واپس (یہ+layerList); }

  ثابت بنیاد_Glyph_List& get_baseglyphList () ثابت
  { واپس (یہ+بنیاد_Glyph_List_2); }

  منطقی has_var_store () ثابت
  { واپس نسخہ >= 1 && hb_barrier () && varStore != 0; }

  منطقی has_delta_سیٹ_اشاریہ_map () ثابت
  { واپس نسخہ >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقی has_clip_list () ثابت
  { واپس نسخہ >= 1 && hb_barrier () && clipList != 0; }

  ثابت Delta_سیٹ_اشاریہ_Map &get_delta_سیٹ_اشاریہ_map () ثابت
  { واپس has_delta_سیٹ_اشاریہ_map () && hb_barrier () ? یہ+varIdxMap : Null (Delta_سیٹ_اشاریہ_Map); }
  ثابت Delta_سیٹ_اشاریہ_Map *get_delta_سیٹ_اشاریہ_map_ptr () ثابت
  { واپس has_delta_سیٹ_اشاریہ_map () && hb_barrier () ? &(یہ+varIdxMap) : خالی_اشاریہ; }

  ثابت شے_Variation_Store &get_var_store () ثابت
  { واپس has_var_store () && hb_barrier () ? یہ+varStore : Null (شے_Variation_Store); }
  ثابت شے_Variation_Store *get_var_store_ptr () ثابت
  { واپس has_var_store () && hb_barrier () ? &(یہ+varStore) : خالی_اشاریہ; }

  ثابت ClipList &get_clip_list () ثابت
  { واپس has_clip_list () && hb_barrier () ? یہ+clipList : Null (ClipList); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
                  (یہ+بنیاد_Glyphs_Z).sanitize (c, عدد_بنیاد_Glyphs) &&
                  (یہ+layersZ).sanitize (c, عدد_Layers) &&
                  (نسخہ == 0 ||
		   (hb_barrier () &&
		    بنیاد_Glyph_List_2.sanitize (c, یہ) &&
		    layerList.sanitize (c, یہ) &&
		    clipList.sanitize (c, یہ) &&
		    varIdxMap.sanitize (c, یہ) &&
		    varStore.sanitize (c, یہ))));
  }

  سانچہ<قسم_نام بنیاد_Iterator, قسم_نام LayerIterator,
	   hb_requires (hb_is_iterator (بنیاد_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقی serialize_V0 (hb_serialize_context_t *c,
		     غیرعلامتی نسخہ,
		     بنیاد_Iterator بنیاد_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (بنیاد_it.len () != layer_it.len ()))
      return_trace (جھوٹ);

    یہ->نسخہ = نسخہ;
    عدد_Layers = 0;
    عدد_بنیاد_Glyphs = بنیاد_it.len ();
    اگر (عدد_بنیاد_Glyphs == 0)
    {
      بنیاد_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (سچ);
    }

    c->push ();
    برائے (ثابت hb_شے_type<بنیاد_Iterator> _ : + بنیاد_it.iter ())
    {
      خودکار* ریکارڈ = c->embed (_);
      اگر (unlikely (!ریکارڈ)) return_trace (جھوٹ);
      ریکارڈ->پہلا_Layer_Idx = عدد_Layers;
      عدد_Layers += ریکارڈ->عدد_Layers;
    }
    c->جوڑو_link (بنیاد_Glyphs_Z, c->نکالو_pack ());

    c->push ();
    برائے (ثابت hb_شے_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->جوڑو_link (layersZ, c->نکالو_pack ());

    return_trace (سچ);
  }

  ثابت بنیاد_Glyph_ریکارڈ* get_بنیاد_glyph_ریکارڈ (hb_codepoint_t gid) ثابت
  {
    ثابت بنیاد_Glyph_ریکارڈ* ریکارڈ = &(یہ+بنیاد_Glyphs_Z).bsearch (عدد_بنیاد_Glyphs, (غیرعلامتی صحیح) gid);
    اگر (ریکارڈ == &Null (بنیاد_Glyph_ریکارڈ) ||
        (ریکارڈ && (hb_codepoint_t) ریکارڈ->glyphId != gid))
      ریکارڈ = خالی_اشاریہ;
    واپس ریکارڈ;
  }

  ثابت بنیاد_Glyph_Paint_ریکارڈ* get_بنیاد_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت بنیاد_Glyph_List &list = یہ+بنیاد_Glyph_List_2;
    غیرعلامتی صحیح i;
    اگر (!list.bfind ((غیرعلامتی) gid, &i))
      واپس خالی_اشاریہ;
    واپس &list[i];
  }

  منطقی downgrade_مقصد_V0 (ثابت hb_سیٹ_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    برائے (ثابت بنیاد_Glyph_Paint_ریکارڈ& _ : get_baseglyphList ())
      اگر (glyphset.has (_.glyphId))
        واپس جھوٹ;

    واپس سچ;
  }

  منطقی subset_varstore (hb_subset_context_t *c,
                        COLR* برآمد /*برآمد*/) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (سچ);

    ثابت شے_Variation_Store& var_store = یہ+varStore;
    اگر (c->plan->normalized_coords)
    {
      شے_variations_t شے_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقی optimize = (varIdxMap != 0) ? سچ : جھوٹ;
      اگر (!شے_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (جھوٹ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      اگر (شے_vars.get_region_list () &&
          شے_vars.get_vardata_encodings () &&
          !برآمد->varStore.serialize_serialize (c->serializer,
                                              شے_vars.has_long_لفظ (),
                                              c->plan->axis_لیبل_فہرست,
                                              شے_vars.get_region_list (),
                                              شے_vars.get_vardata_encodings ()))
        return_trace (جھوٹ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      اگر (optimize)
      {
        ثابت hb_map_t &varidx_map = شے_vars.get_varidx_map ();
        برائے (خودکار _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          اگر (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          ورنہ
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    ورنہ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_سیٹ_t dead_regions;
      اگر (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_اشاریہ_لیبل_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      اگر (unlikely (!برآمد->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : خالی_اشاریہ)))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  منطقی subset_delta_سیٹ_اشاریہ_map (hb_subset_context_t *c,
                                   COLR* برآمد /*برآمد*/) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (سچ);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_سیٹ_اشاریہ_map_subset_plan_t اشاریہ_map_plan (deltaset_idx_varidx_map);

    اگر (unlikely (!اشاریہ_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (جھوٹ);

    return_trace (برآمد->varIdxMap.serialize_serialize (c->serializer, اشاریہ_map_plan));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_map_t &الٹ_glyph_map = *c->plan->الٹ_glyph_map;
    ثابت hb_سیٹ_t& glyphset = c->plan->_glyphset_colred;

    خودکار بنیاد_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = الٹ_glyph_map.get (new_gid);
		    اگر (glyphset.has (old_gid)) واپس سچ;
		    واپس جھوٹ;
		 })
    | hb_map_retains_ترتیب_کاری ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = الٹ_glyph_map.get (new_gid);

				ثابت بنیاد_Glyph_ریکارڈ* old_ریکارڈ = get_بنیاد_glyph_ریکارڈ (old_gid);
				اگر (unlikely (!old_ریکارڈ))
				  واپس hb_pair_t<منطقی, بنیاد_Glyph_ریکارڈ> (جھوٹ, Null (بنیاد_Glyph_ریکارڈ));
				بنیاد_Glyph_ریکارڈ new_ریکارڈ = {};
				new_ریکارڈ.glyphId = new_gid;
				new_ریکارڈ.عدد_Layers = old_ریکارڈ->عدد_Layers;
				واپس hb_pair_t<منطقی, بنیاد_Glyph_ریکارڈ> (سچ, new_ریکارڈ);
			      })
    | hb_filter (hb_پہلا)
    | hb_map_retains_ترتیب_کاری (hb_دوسرا)
    ;

    خودکار layer_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_map (الٹ_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_ترتیب_کاری ([&](hb_codepoint_t old_gid)
			      {
				ثابت بنیاد_Glyph_ریکارڈ* old_ریکارڈ = get_بنیاد_glyph_ریکارڈ (old_gid);
				hb_سمتیہ_t<Layer_ریکارڈ> برآمد_layers;

				اگر (unlikely (!old_ریکارڈ ||
					      old_ریکارڈ->پہلا_Layer_Idx >= عدد_Layers ||
					      old_ریکارڈ->پہلا_Layer_Idx + old_ریکارڈ->عدد_Layers > عدد_Layers))
				  واپس hb_pair_t<منطقی, hb_سمتیہ_t<Layer_ریکارڈ>> (جھوٹ, برآمد_layers);

				خودکار layers = (یہ+layersZ).as_array (عدد_Layers).sub_array (old_ریکارڈ->پہلا_Layer_Idx,
											     old_ریکارڈ->عدد_Layers);
				برآمد_layers.resize (layers.لمبائی);
				برائے (غیرعلامتی صحیح i = 0; i < layers.لمبائی; i++) {
				  برآمد_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  اگر (unlikely (!c->plan->new_gid_for_old_gid (برآمد_layers[i].glyphId, &new_gid)))
				    واپس hb_pair_t<منطقی, hb_سمتیہ_t<Layer_ریکارڈ>> (جھوٹ, برآمد_layers);
				  برآمد_layers[i].glyphId = new_gid;
				  برآمد_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				واپس hb_pair_t<منطقی, hb_سمتیہ_t<Layer_ریکارڈ>> (سچ, برآمد_layers);
			      })
    | hb_filter (hb_پہلا)
    | hb_map_retains_ترتیب_کاری (hb_دوسرا)
    ;

    اگر (نسخہ == 0 && (!بنیاد_it || !layer_it))
      return_trace (جھوٹ);

    خودکار *colr_مفرد = c->serializer->آغاز_embed<COLR> ();
    اگر (unlikely (!c->serializer->extend_min (colr_مفرد)))  return_trace (جھوٹ);

    اگر (نسخہ == 0 || downgrade_مقصد_V0 (glyphset))
      return_trace (colr_مفرد->serialize_V0 (c->serializer, 0, بنیاد_it, layer_it));

    hb_barrier ();

    //start version 1
    اگر (!c->serializer->allocate_سائز<خالی> (5 * HBUINT32::static_سائز)) return_trace (جھوٹ);
    اگر (!colr_مفرد->serialize_V0 (c->serializer, نسخہ, بنیاد_it, layer_it)) return_trace (جھوٹ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    اگر (!subset_varstore (c, colr_مفرد)) return_trace (جھوٹ);

    شے_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : خالی_اشاریہ,
				     c->plan->normalized_coords ? get_delta_سیٹ_اشاریہ_map_ptr () : خالی_اشاریہ,
				     c->plan->normalized_coords.as_array ());

    اگر (!colr_مفرد->بنیاد_Glyph_List_2.serialize_subset (c, بنیاد_Glyph_List_2, یہ, instancer))
      return_trace (جھوٹ);

    colr_مفرد->layerList.serialize_subset (c, layerList, یہ, instancer);
    colr_مفرد->clipList.serialize_subset (c, clipList, یہ, instancer);

    return_trace (subset_delta_سیٹ_اشاریہ_map (c, colr_مفرد));
  }

  ثابت Paint *get_بنیاد_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت بنیاد_Glyph_List &baseglyph_paintrecords = یہ+بنیاد_Glyph_List_2;
    ثابت بنیاد_Glyph_Paint_ریکارڈ* ریکارڈ = get_بنیاد_glyph_paintrecord (glyph);
    اگر (ریکارڈ)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+ریکارڈ->paint;
      واپس &paint;
    }
    ورنہ
      واپس خالی_اشاریہ;
  }

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
  منطقی
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    شے_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_سیٹ_اشاریہ_map_ptr (),
                                     hb_array (font->coords, font->عدد_coords));

    اگر (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      واپس سچ;
    }

    خودکار *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقی ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), سچ, scratch);

    خودکار e = scratch.paint_extents.get_extents ();
    اگر (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->چوڑائی = 0;
      extents->اونچائی = 0;
    }
    ورنہ
      *extents = e.مقصد_glyph_extents ();

    واپس ret;
  }
#اگر_ختم

  منطقی
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    اگر (نسخہ >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنیاد_glyph_paint (glyph);

      واپس paint != خالی_اشاریہ;
    }

    واپس جھوٹ;
  }

  منطقی get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت شے_Var_Store_Instancer instancer) ثابت
  {
    واپس get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
  منطقی
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, خالی *ڈیٹا,
	       غیرعلامتی صحیح palette_اشاریہ, hb_color_t foreground,
	       منطقی clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    شے_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_سیٹ_اشاریہ_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->عدد_coords : 0));
    hb_paint_context_t c (یہ, funcs, ڈیٹا, font, palette_اشاریہ, foreground, instancer);

    hb_decycler_گرہ_t node (c.glyphs_decycler);
    node.دورہ (glyph);

    اگر (نسخہ >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنیاد_glyph_paint (glyph);
      اگر (paint)
      {
        // COLRv1 glyph

	منطقی is_bounded = سچ;
	hb_glyph_extents_t extents = {};
	اگر (clip)
	{
	  اگر (!get_clip (glyph, &extents, instancer))
	  {
	    clip = جھوٹ;
	    is_bounded = جھوٹ;
	  }

	  اگر (!is_bounded)
	  {
	    خودکار *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_اشاریہ, foreground,
			 جھوٹ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ڈالو_font_transform (c.ڈیٹا, font);

	اگر (clip)
	  c.funcs->ڈالو_clip_rectangle (c.ڈیٹا,
					(اعشاری) extents.x_bearing,
					(اعشاری) extents.y_bearing + extents.اونچائی,
					(اعشاری) extents.x_bearing + extents.چوڑائی,
					(اعشاری) extents.y_bearing);

	اگر (is_bounded)
	  c.recurse (*paint);

	اگر (clip)
	  c.funcs->نکالو_clip (c.ڈیٹا);

	c.funcs->نکالو_transform (c.ڈیٹا);

        واپس سچ;
      }
    }

    ثابت بنیاد_Glyph_ریکارڈ *ریکارڈ = get_بنیاد_glyph_ریکارڈ (glyph);
    اگر (ریکارڈ && ((hb_codepoint_t) ریکارڈ->glyphId == glyph))
    {
      // COLRv0 glyph
      برائے (ثابت خودکار &r : (یہ+layersZ).as_array (عدد_Layers)
			   .sub_array (ریکارڈ->پہلا_Layer_Idx, ریکارڈ->عدد_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.ڈیٹا, r.glyphId, c.font, is_foreground, color);
      }

      واپس سچ;
    }

    واپس جھوٹ;
  }
#اگر_ختم

  محفوظ:
  HBUINT16	نسخہ;	/* Table version number (starts at 0). */
  HBUINT16	عدد_بنیاد_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_مقصد<Sorted_Unsized_Array_کا<بنیاد_Glyph_ریکارڈ>>
		بنیاد_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_مقصد<Unsized_Array_کا<Layer_ریکارڈ>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	عدد_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_مقصد<بنیاد_Glyph_List>		بنیاد_Glyph_List_2;
  Offset32_مقصد<LayerList>			layerList;
  Offset32_مقصد<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_مقصد<شے_Variation_Store>	varStore;
  عوامی:
  DEFINE_سائز_MIN (14);
};

ساخت COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

خالی
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  اگر (unlikely (گہرائی_بایاں <= 0 || کنارہ_گنتی <= 0)) واپس;
  گہرائی_بایاں--;
  کنارہ_گنتی--;
  paint.dispatch (یہ);
  گہرائی_بایاں++;
}

سانچہ <قسم_نام Types>
خالی PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (یہ);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  اگر (likely (c->گہرائی_بایاں > 0 && c->کنارہ_گنتی > 0) &&
      (یہ+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->کنارہ_گنتی--;
    c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
    c->funcs->fill_glyph (c->ڈیٹا, gid, c->font, is_foreground, color);
    c->funcs->نکالو_transform (c->ڈیٹا);
    واپس;
  }

  c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
  c->funcs->ڈالو_clip_glyph (c->ڈیٹا, gid, c->font);
  c->funcs->ڈالو_font_transform (c->ڈیٹا, c->font);
  c->recurse (یہ+paint);
  c->funcs->نکالو_transform (c->ڈیٹا);
  c->funcs->نکالو_clip (c->ڈیٹا);
  c->funcs->نکالو_transform (c->ڈیٹا);
}

خالی PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (یہ);
  ثابت LayerList &paint_انحراف_lists = c->get_colr_جدول ()->get_layerList ();
  hb_decycler_گرہ_t node (c->layers_decycler);
  برائے (غیرعلامتی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    اگر (unlikely (!node.دورہ (i)))
      واپس;

    ثابت Paint &paint = paint_انحراف_lists.get_paint (i);
    c->recurse (paint);
  }
}

خالی PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (یہ);

  hb_decycler_گرہ_t node (c->glyphs_decycler);
  اگر (unlikely (!node.دورہ (gid)))
    واپس;

  c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
  اگر (c->funcs->color_glyph (c->ڈیٹا, gid, c->font))
  {
    c->funcs->نکالو_transform (c->ڈیٹا);
    واپس;
  }
  c->funcs->نکالو_transform (c->ڈیٹا);

  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت Paint *paint = colr_جدول->get_بنیاد_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقی has_clip_box = colr_جدول->get_clip (gid, &extents, c->instancer);

  اگر (has_clip_box)
    c->funcs->ڈالو_clip_rectangle (c->ڈیٹا,
				   (اعشاری) extents.x_bearing,
				   (اعشاری) extents.y_bearing + extents.اونچائی,
				   (اعشاری) extents.x_bearing + extents.چوڑائی,
				   (اعشاری) extents.y_bearing);

  اگر (paint)
    c->recurse (*paint);

  اگر (has_clip_box)
    c->funcs->نکالو_clip (c->ڈیٹا);
}

} /* namespace OT */

#اگر_ختم /* OT_COLOR_COLR_COLR_HH */
