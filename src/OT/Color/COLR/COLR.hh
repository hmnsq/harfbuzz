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

#اگر_تعریف_نشده OT_COLOR_COLR_COLR_HH
#تعریف_کن OT_COLOR_COLR_COLR_HH

#ضمن "../../../hb.hh"
#ضمن "../../../hb-decycler.hh"
#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-var-common.hh"
#ضمن "../../../hb-paint.hh"
#ضمن "../../../hb-paint-bounded.hh"
#ضمن "../../../hb-paint-extents.hh"
#ضمن "../../../hb-depend-data.hh"

#ضمن "../../../hb-ot-dual.hh"
#ضمن "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#تعریف_کن HB_OT_برچسب_COLR HB_برچسب('C','O','L','R')

نطاق OT {
بنية hb_paint_context_t;
}

بنية hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

نطاق OT {

بنية COLR;

بنية Paint;

بنية hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ثابت حرف *get_name () { أرجع "PAINT"; }
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (پایه); }

عام:
  ثابت فراغ *پایه;
  hb_paint_funcs_t *funcs;
  فراغ *داده;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  قلم_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح عمق_چپ = HB_MAX_NESTING_LEVEL;
  صحيح یال_شمار = HB_MAX_گراف_یال_شمار;

  hb_paint_context_t (ثابت فراغ *پایه_2,
		      hb_paint_funcs_t *funcs_,
		      فراغ *داده_2,
                      hb_font_t *font_,
                      بیعلامت صحيح palette_,
                      hb_color_t foreground_,
		      قلم_Var_Store_Instancer &instancer_) :
    پایه (پایه_2),
    funcs (funcs_),
    داده (داده_2),
    font (font_),
    palette (
#اگر_تعریف_نشده HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->جدول.CPAL->get_palette_colors (palette_ < font->face->جدول.CPAL->get_palette_شمار () ? palette_ : 0)
#پایان_اگر
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_مجموعه_synthetic_bold (font, 0, 0, درست);
      hb_font_مجموعه_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساكن بیعلامت
  color_alpha (hb_color_t color, عائم alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    أرجع roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (بیعلامت صحيح color_شاخص, عائم alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = درست;

    إذا (color_شاخص != 0xffff)
    {
      إذا (!funcs->custom_palette_color (داده, color_شاخص, &color))
        color = palette[color_شاخص];

      *is_foreground = خطأ;
    }

    أرجع HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  مضمن فراغ recurse (ثابت Paint &paint);
};

بنية hb_colrv1_بستگی_context_t :
       hb_dispatch_context_t<hb_colrv1_بستگی_context_t>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_چپ == 0))
      أرجع hb_خالی_t ();

    إذا (paint_دیدهشده (&obj))
      أرجع hb_خالی_t ();

    nesting_level_چپ--;
    obj.closurev1 (هذا);
    nesting_level_چپ++;
    أرجع hb_خالی_t ();
  }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  منطقي paint_دیدهشده (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) پایه);
    إذا (دیدهشده_paint.درون_error() || دیدهشده_paint.has (delta))
      أرجع درست;

    دیدهشده_paint.جمع_کن (delta);
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (پایه); }

  فراغ جمع_کن_glyph (بیعلامت glyph_id)
  { glyphs->جمع_کن (glyph_id); }

  فراغ جمع_کن_layer_indices (بیعلامت اولی_layer_شاخص, بیعلامت رقم_از_layers)
  {
    إذا (رقم_از_layers == 0) أرجع;
    layer_indices->جمع_کن_range (اولی_layer_شاخص, اولی_layer_شاخص + رقم_از_layers - 1);
  }

  فراغ جمع_کن_palette_شاخص (بیعلامت palette_شاخص)
  { palette_indices->جمع_کن (palette_شاخص); }

  فراغ جمع_کن_var_idxes (بیعلامت اولی_var_idx, بیعلامت رقم_idxes)
  {
    إذا (!رقم_idxes || اولی_var_idx == VarIdx::NO_VARIATION) أرجع;
    variation_indices->جمع_کن_range (اولی_var_idx, اولی_var_idx + رقم_idxes - 1);
  }

  عام:
  ثابت فراغ *پایه;
  hb_مجموعه_t دیدهشده_paint;
  hb_مجموعه_t *glyphs;
  hb_مجموعه_t *layer_indices;
  hb_مجموعه_t *palette_indices;
  hb_مجموعه_t *variation_indices;
  بیعلامت رقم_var_idxes;
  بیعلامت nesting_level_چپ;

  hb_colrv1_بستگی_context_t (ثابت فراغ *پایه_2,
                               hb_مجموعه_t *glyphs_,
                               hb_مجموعه_t *layer_indices_,
                               hb_مجموعه_t *palette_indices_,
                               hb_مجموعه_t *variation_indices_,
                               بیعلامت رقم_var_idxes_2 = 1,
                               بیعلامت nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
                          پایه (پایه_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          رقم_var_idxes (رقم_var_idxes_2),
                          nesting_level_چپ (nesting_level_چپ_2)
  {}
};


بنية hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_چپ == 0))
      أرجع hb_خالی_t ();

    إذا (paint_دیدهشده (&obj))
      أرجع hb_خالی_t ();

    nesting_level_چپ--;
    obj.dependv1 (هذا);
    nesting_level_چپ++;
    أرجع hb_خالی_t ();
  }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  منطقي paint_دیدهشده (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    إذا (دیدهشده_paint.درون_error() || دیدهشده_paint.has (delta))
      أرجع درست;

    دیدهشده_paint.جمع_کن (delta);
    إذا (unlikely (دیدهشده_paint.درون_error ()))
      depend_داده->fail ();
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع colr; }

  عام:
  ثابت COLR *colr;
  hb_depend_داده_builder_t *depend_داده;
  hb_مجموعه_t دیدهشده_paint;
  بیعلامت nesting_level_چپ;
  hb_codepoint_t منبع_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_داده_builder_t *depend_داده_2,
                              بیعلامت nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_داده (depend_داده_2),
                          nesting_level_چپ (nesting_level_چپ_2),
                          منبع_gid (0)
  {}
};

بنية Layer_رکورد
{
  عملگر hb_ot_color_layer_t () ثابت { أرجع {glyphId, colorIdx}; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  شاخص		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية پایه_Glyph_رکورد
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	اولی_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	رقم_Layers;	/* Number of color layers
				 * associated with this glyph */
  عام:
  DEFINE_اندازه_STATIC (6);
};

قالب <نام_نوع T>
بنية Variable
{
  ساكن constexpr منطقي is_variable = درست;

  ثابت T& get_مقدار () ثابت { أرجع value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    c->رقم_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->جمع_کن_var_idxes (var_Idx_پایه, c->رقم_var_idxes);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!value.subset (c, instancer, var_Idx_پایه)) return_trace (خطأ);
    إذا (c->plan->all_axes_pinned)
      return_trace (درست);

    VarIdx new_varidx;
    new_varidx = var_Idx_پایه;
    إذا (var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      hb_pair_t<بیعلامت, صحيح> *new_varidx_delta;
      إذا (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_پایه, &new_varidx_delta))
        return_trace (خطأ);

      new_varidx = hb_اولی (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, var_Idx_پایه);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, var_Idx_پایه, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, var_Idx_پایه, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  محمي:
  T      value;
  عام:
  VarIdx var_Idx_پایه;
  عام:
  DEFINE_اندازه_MIN (VarIdx::static_اندازه + T::min_اندازه);
};

قالب <نام_نوع T>
بنية NoVariable
{
  ساكن constexpr منطقي is_variable = خطأ;

  ثابت T& get_مقدار () ثابت { أرجع value; }

  ساكن constexpr uint32_t var_Idx_پایه = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (value.subset (c, instancer, var_Idx_پایه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, var_Idx_پایه);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  T      value;
  عام:
  DEFINE_اندازه_MIN (T::min_اندازه);
};

// Color structures

قالب <نام_نوع Type, بیعلامت fraction_بیتها>
ساكن فراغ
مجموعه_fixed_clamped (HBFixed<Type, fraction_بیتها> &بیرون, عائم value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  بیرون.مجموعه_int (hb_clamp_مقصد<نام_نوع Type::type> (roundf ((value / HBFixed<Type, fraction_بیتها>::mult))));
}

بنية ColorStop
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    c->جمع_کن_palette_شاخص (palette_شاخص_2);
    c->رقم_var_idxes = 2;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->stop_جابه_جایی, stop_جابه_جایی.مقصد_float(instancer (var_Idx_پایه, 0)));
      عائم a = alpha.مقصد_float (instancer (var_Idx_پایه, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      مجموعه_fixed_clamped (بیرون->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_تخصیص_بده (بیرون->palette_شاخص_2, c->plan->colr_palettes.get (palette_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *بیرون,
		       uint32_t varIdx,
		       ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    بیرون->جابه_جایی = stop_جابه_جایی.مقصد_float(instancer (varIdx, 0));
    بیرون->color = c->get_color (palette_شاخص_2,
                               alpha.مقصد_float (instancer (varIdx, 1)),
                               &بیرون->is_foreground);
  }

  F2DOT14	stop_جابه_جایی;
  HBUINT16	palette_شاخص_2;
  F2DOT14	alpha;
  عام:
  DEFINE_اندازه_STATIC (2 + 2 * F2DOT14::static_اندازه);
};

بنية Extend : HBUINT8
{
  تعداد {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عام:
  DEFINE_اندازه_STATIC (1);
};

قالب <قالب<نام_نوع> صنف Var>
بنية Color_سطر
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    لكل (ثابت خودکار &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    إذا (!c->serializer->check_تخصیص_بده (بیرون->extend, extend, HB_SERIALIZE_ERROR_INT_سرریز)) return_trace (خطأ);
    إذا (!c->serializer->check_تخصیص_بده (بیرون->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_سرریز)) return_trace (خطأ);

    لكل (ثابت خودکار& stop : stops.iter ())
    {
      إذا (!stop.subset (c, instancer)) return_trace (خطأ);
    }
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  بیعلامت صحيح
  get_color_stops (hb_paint_context_t *c,
                   بیعلامت صحيح شروع,
		   بیعلامت صحيح *شمار,
		   hb_color_stop_t *color_stops,
		   ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    بیعلامت صحيح len = stops.len;

    إذا (شمار && color_stops)
    {
      بیعلامت صحيح i;
      لكل (i = 0; i < *شمار && شروع + i < len; i++)
        stops[شروع + i].get_color_stop (c, &color_stops[i], instancer);
      *شمار = i;
    }

    أرجع len;
  }

  HB_داخلی ساكن بیعلامت صحيح static_get_color_stops (hb_color_سطر_t *color_سطر,
							  فراغ *color_سطر_داده,
							  بیعلامت صحيح شروع,
							  بیعلامت صحيح *شمار,
							  hb_color_stop_t *color_stops,
							  فراغ *user_داده)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_داده;
    hb_paint_context_t *c = (hb_paint_context_t *) user_داده;
    أرجع thiz->get_color_stops (c, شروع, شمار, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بدل ((بیعلامت صحيح) extend)
    {
      حالة Extend::EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
      حالة Extend::EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
      حالة Extend::EXTEND_PAD:
      افتراضي:                     أرجع HB_PAINT_EXTEND_PAD;
    }
  }

  HB_داخلی ساكن hb_paint_extend_t static_get_extend (hb_color_سطر_t *color_سطر,
							  فراغ *color_سطر_داده,
							  فراغ *user_داده)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_داده;
    أرجع thiz->get_extend ();
  }

  Extend	extend;
  Array16_از<Var<ColorStop>>	stops;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
بنية مرکب_نما : HBUINT8
{
  عام:
  DEFINE_اندازه_STATIC (1);
};

بنية Affine2x3
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  { c->رقم_var_idxes = 6; }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      خودکار fold = [&] (F16DOT16 &target, F16DOT16 منبع, بیعلامت جابه_جایی) {
        // Keep the stored integer precise while applying the float delta.
        target.مجموعه_int (hb_clamp_مقصد<int32_t> (roundf ((مزدوج) منبع.مقصد_int () +
                                                    (مزدوج) instancer (var_Idx_پایه, جابه_جایی))));
      };
      fold (بیرون->xx, xx, 0);
      fold (بیرون->yx, yx, 1);
      fold (بیرون->xy, xy, 2);
      fold (بیرون->yy, yy, 3);
      fold (بیرون->dx, dx, 4);
      fold (بیرون->dy, dy, 5);
    }
    return_trace (درست);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    c->funcs->افزودن_transform (c->داده,
			      xx.مقصد_float (c->instancer (var_Idx_پایه, 0)),
			      yx.مقصد_float (c->instancer (var_Idx_پایه, 1)),
                              xy.مقصد_float (c->instancer (var_Idx_پایه, 2)),
			      yy.مقصد_float (c->instancer (var_Idx_پایه, 3)),
                              dx.مقصد_float (c->instancer (var_Idx_پایه, 4)),
			      dy.مقصد_float (c->instancer (var_Idx_پایه, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  عام:
  DEFINE_اندازه_STATIC (6 * F16DOT16::static_اندازه);
};

بنية PaintColrLayers
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer HB_استفادهنشده) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    uint32_t اولی_layer_شاخص = رقم_Layers ? c->plan->colrv1_layers.get (اولی_Layer_شاخص) : 0;
    return_trace (c->serializer->check_تخصیص_بده (بیرون->اولی_Layer_شاخص, اولی_layer_شاخص,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	رقم_Layers;
  HBUINT32	اولی_Layer_شاخص;  /* index into COLRv1::layerList */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية PaintSolid
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    c->جمع_کن_palette_شاخص (palette_شاخص_2);
    c->رقم_var_idxes = 1;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      عائم a = alpha.مقصد_float (instancer (var_Idx_پایه, 0));
      مجموعه_fixed_clamped (بیرون->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    إذا (format == 3 && c->plan->all_axes_pinned)
        بیرون->format = 2;

    return_trace (c->serializer->check_تخصیص_بده (بیرون->palette_شاخص_2, c->plan->colr_palettes.get (palette_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_پایه,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (palette_شاخص_2,
                           alpha.مقصد_float (c->instancer (var_Idx_پایه, 0)),
                           is_foreground);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_پایه, &is_foreground, &color);
    c->funcs->color (c->داده, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_شاخص_2;
  F2DOT14	alpha;
  عام:
  DEFINE_اندازه_STATIC (3 + F2DOT14::static_اندازه);
};

قالب <قالب<نام_نوع> صنف Var>
بنية Paint_خطی_Gradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 6;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      بیرون->x0 = hb_clamp_مقصد<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_پایه, 0)));
      بیرون->y0 = hb_clamp_مقصد<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
      بیرون->x1 = hb_clamp_مقصد<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
      بیرون->y1 = hb_clamp_مقصد<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_پایه, 3)));
      بیرون->x2 = hb_clamp_مقصد<int16_t> ((مزدوج) x2 + (مزدوج) roundf (instancer (var_Idx_پایه, 4)));
      بیرون->y2 = hb_clamp_مقصد<int16_t> ((مزدوج) y2 + (مزدوج) roundf (instancer (var_Idx_پایه, 5)));
    }

    إذا (format == 5 && c->plan->all_axes_pinned)
        بیرون->format = 4;

    return_trace (بیرون->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, اشاره_تهی
    };

    c->funcs->خطی_gradient (c->داده, &cl,
			       x0 + c->instancer (var_Idx_پایه, 0),
			       y0 + c->instancer (var_Idx_پایه, 1),
			       x1 + c->instancer (var_Idx_پایه, 2),
			       y1 + c->instancer (var_Idx_پایه, 3),
			       x2 + c->instancer (var_Idx_پایه, 4),
			       y2 + c->instancer (var_Idx_پایه, 5));
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
  عام:
  DEFINE_اندازه_STATIC (4 + 6 * FWORD::static_اندازه);
};

قالب <قالب<نام_نوع> صنف Var>
بنية PaintRadialGradient
{
  منطقي constant_after_pinning (hb_subset_context_t *c,
                               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    إذا (c->plan->has_avar2) أرجع خطأ;
    ثابت خودکار &regions = instancer.varStore->get_region_list ();
    لكل (بیعلامت i = 0; i < regions.region_شمار; i++)
    {
      hb_hashmap_t<hb_برچسب_t, Triple> axes;
      إذا (!regions.get_var_region (i, c->plan->axes_old_شاخص_برچسب_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_بیرون_از_جا);
        أرجع خطأ;
      }
      لكل (خودکار axis : axes)
      {
        Triple *حد;
        إذا (!c->plan->user_axes_location.has (axis.اولی, &حد) ||
            حد->minimum != حد->بیشینه)
          أرجع خطأ;
      }
    }
    أرجع درست;
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 6;
  }

  منطقي subset_nonnegative (hb_subset_context_t *c,
                           ثابت قلم_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_پایه) ثابت
  {
    مزدوج منبع[6] = {
      x0 + (مزدوج) roundf (instancer (var_Idx_پایه, 0)),
      y0 + (مزدوج) roundf (instancer (var_Idx_پایه, 1)),
      radius0 + (مزدوج) roundf (instancer (var_Idx_پایه, 2)),
      x1 + (مزدوج) roundf (instancer (var_Idx_پایه, 3)),
      y1 + (مزدوج) roundf (instancer (var_Idx_پایه, 4)),
      radius1 + (مزدوج) roundf (instancer (var_Idx_پایه, 5))
    };
    خودکار transparent = [&] {
      خودکار *بیرون = c->serializer->allocate_اندازه<PaintSolid> (5);
      إذا (!بیرون) أرجع خطأ;
      بیرون->format = 2; بیرون->palette_شاخص_2 = 0xFFFF; بیرون->alpha.مجموعه_float (0);
      أرجع درست;
    };
    ثابت خودکار &سطر = هذا+color_سطر_2;
    إذا (منبع[2] == منبع[5] || !سطر.stops.len) أرجع transparent ();
    بنية stop_t { مزدوج جابه_جایی, alpha; بیعلامت palette, ترتیب; };
    hb_وکتور_t<stop_t> stops;
    إذا (!stops.alloc (سطر.stops.len)) أرجع خطأ;
    بیعلامت ترتیب = 0;
    لكل (ثابت خودکار &مدخل : سطر.stops.iter ())
    {
      ثابت خودکار &stop = مدخل.get_مقدار ();
      stops.push (stop_t {(مزدوج) roundf (stop.stop_جابه_جایی.مقصد_float (instancer (مدخل.var_Idx_پایه, 0)) * 16384.f) / 16384.,
                   (مزدوج) hb_clamp (stop.alpha.مقصد_float (instancer (مدخل.var_Idx_پایه, 1)), 0.f, 1.f),
                   stop.palette_شاخص_2, ترتیب++});
    }
    إذا (stops.درون_error ()) أرجع خطأ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      أرجع a.جابه_جایی < b.جابه_جایی ? -1 : a.جابه_جایی > b.جابه_جایی ? 1 :
             a.ترتیب < b.ترتیب ? -1 : a.ترتیب > b.ترتیب ? 1 : 0;
    });
    مزدوج dr = منبع[5] - منبع[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    بیعلامت lattice = 16384;
    لكل (بیعلامت j = 0; j < 3; j++)
    {
      مزدوج remainder = fmod (fabs (منبع[j + 3] - منبع[j]), 16384.);
      إذا (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_سرریز);
        أرجع خطأ;
      }
      بیعلامت راست = (بیعلامت) remainder;
      طالما (راست) { بیعلامت r = lattice % راست; lattice = راست; راست = r; }
    }
    لكل (بیعلامت لغزش = 0; لغزش < 16; لغزش++)
    {
      مزدوج b = 1u << لغزش;
      مزدوج coarse = dr > 0 ? ceil (-منبع[2] / dr) : floor (منبع[2] / -dr) - b;
      مزدوج fine = dr > 0 ? ceil (-منبع[2] / dr * lattice) / lattice :
                            floor (منبع[2] / -dr * lattice) / lattice - b;
      مزدوج candidates[] = {coarse, fine};
      لكل (مزدوج a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        مزدوج geometry[6];
        منطقي میگنجد = درست;
        لكل (بیعلامت j = 0; j < 3; j++)
        {
          مزدوج difference = منبع[j + 3] - منبع[j];
          geometry[j] = منبع[j] + a * difference;
          geometry[j + 3] = منبع[j] + (a + b) * difference;
          مزدوج کم = j == 2 ? 0 : -32768, بالا = j == 2 ? 65535 : 32767;
          میگنجد &= geometry[j] >= کم && geometry[j] <= بالا &&
                  geometry[j + 3] >= کم && geometry[j + 3] <= بالا;
        }
        إذا (!میگنجد) استمر;
        بیعلامت اولی = 0, شمار = stops.درازی;
        مزدوج min = (stops[0].جابه_جایی - a) / b, max = (stops.دم ().جابه_جایی - a) / b;
        مزدوج bias = 0;
        منطقي constant = خطأ;
        إذا (سطر.extend == Extend::EXTEND_PAD)
        {
          مزدوج tip = -geometry[2] / (b * dr);
          إذا ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            اولی = dr > 0 ? stops.درازی - 1 : 0;
            شمار = 1; constant = درست;
          }
        }
        وإلا
        {
          مزدوج period = max - min;
          إذا (سطر.extend == Extend::EXTEND_REFLECT) period *= 2;
          إذا (period <= 0) أرجع transparent ();
          مزدوج n = ceil ((-2 - min) / period);
          إذا (n > floor ((32767. / 16384. - max) / period)) استمر;
          bias = n * period;
        }
        لكل (بیعلامت j = 0; j < شمار; j++)
        {
          مزدوج بیتها = constant ? 0 : ((stops[اولی + j].جابه_جایی - a) / b + bias) * 16384;
          میگنجد &= بیتها >= -32768 && بیتها <= 32767 && بیتها == round (بیتها);
        }
        إذا (!میگنجد) استمر;
        خودکار *بیرون = c->serializer->allocate_اندازه<PaintRadialGradient<NoVariable>> (16);
        إذا (!بیرون) أرجع خطأ;
        بیرون->format = 6;
        بیرون->x0 = (صحيح) geometry[0]; بیرون->y0 = (صحيح) geometry[1]; بیرون->radius0 = (بیعلامت) geometry[2];
        بیرون->x1 = (صحيح) geometry[3]; بیرون->y1 = (صحيح) geometry[4]; بیرون->radius1 = (بیعلامت) geometry[5];
        c->serializer->push ();
        خودکار *بیرون_سطر = c->serializer->allocate_اندازه<Color_سطر<NoVariable>> (3 + شمار * ColorStop::static_اندازه);
        إذا (!بیرون_سطر) { c->serializer->برداشت_discard (); أرجع خطأ; }
        بیرون_سطر->extend = سطر.extend;
        بیرون_سطر->stops.len = شمار;
        لكل (بیعلامت j = 0; j < شمار; j++)
        {
          ثابت خودکار &stop = stops[اولی + j];
          خودکار &بیرون_stop = بیرون_سطر->stops[j].value;
          بیرون_stop.stop_جابه_جایی.مجموعه_float ((عائم) (constant ? 0 : (stop.جابه_جایی - a) / b + bias));
          بیرون_stop.alpha.مجموعه_float ((عائم) stop.alpha);
          إذا (!c->serializer->check_تخصیص_بده (بیرون_stop.palette_شاخص_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_سرریز))
          { c->serializer->برداشت_discard (); أرجع خطأ; }
        }
        c->serializer->جمع_کن_link (بیرون->color_سطر_2, c->serializer->برداشت_pack ());
        أرجع !c->serializer->درون_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_سرریز);
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_پایه != VarIdx::NO_VARIATION &&
        (radius0 + (مزدوج) roundf (instancer (var_Idx_پایه, 2)) < 0 ||
         radius1 + (مزدوج) roundf (instancer (var_Idx_پایه, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_پایه));
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      بیرون->x0 = hb_clamp_مقصد<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_پایه, 0)));
      بیرون->y0 = hb_clamp_مقصد<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
      بیرون->radius0 = hb_clamp_مقصد<uint16_t> ((مزدوج) radius0 + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
      بیرون->x1 = hb_clamp_مقصد<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_پایه, 3)));
      بیرون->y1 = hb_clamp_مقصد<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_پایه, 4)));
      بیرون->radius1 = hb_clamp_مقصد<uint16_t> ((مزدوج) radius1 + (مزدوج) roundf (instancer (var_Idx_پایه, 5)));
    }

    إذا (format == 7 && c->plan->all_axes_pinned)
        بیرون->format = 6;

    return_trace (بیرون->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, اشاره_تهی
    };

    c->funcs->radial_gradient (c->داده, &cl,
			       x0 + c->instancer (var_Idx_پایه, 0),
			       y0 + c->instancer (var_Idx_پایه, 1),
			       radius0 + c->instancer (var_Idx_پایه, 2),
			       x1 + c->instancer (var_Idx_پایه, 3),
			       y1 + c->instancer (var_Idx_پایه, 4),
			       radius1 + c->instancer (var_Idx_پایه, 5));
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
  عام:
  DEFINE_اندازه_STATIC (4 + 6 * FWORD::static_اندازه);
};

قالب <قالب<نام_نوع> صنف Var>
بنية PaintSweepGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 4;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      بیرون->centerX = hb_clamp_مقصد<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_پایه, 0)));
      بیرون->centerY = hb_clamp_مقصد<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
      مجموعه_fixed_clamped (بیرون->شروع_Angle, شروع_Angle.مقصد_float (instancer (var_Idx_پایه, 2)));
      مجموعه_fixed_clamped (بیرون->انتها_Angle, انتها_Angle.مقصد_float (instancer (var_Idx_پایه, 3)));
    }

    إذا (format == 9 && c->plan->all_axes_pinned)
        بیرون->format = 8;

    return_trace (بیرون->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, اشاره_تهی
    };

    c->funcs->sweep_gradient (c->داده, &cl,
			      centerX + c->instancer (var_Idx_پایه, 0),
			      centerY + c->instancer (var_Idx_پایه, 1),
                              (شروع_Angle.مقصد_float (c->instancer (var_Idx_پایه, 2)) + 1) * HB_PI,
                              (انتها_Angle.مقصد_float   (c->instancer (var_Idx_پایه, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_مقصد<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		شروع_Angle;
  F2DOT14		انتها_Angle;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * FWORD::static_اندازه + 2 * F2DOT14::static_اندازه);
};

// Paint a non-COLR glyph, filled as indicated by paint.
قالب <نام_نوع Types>
بنية PaintGlyph
{
  استفاده HBGlyphID = نام_نوع Types::HBGlyphID;

  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (! c->serializer->check_تخصیص_بده (بیرون->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    return_trace (بیرون->paint.serialize_subset (c, paint, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && paint.sanitize (c, هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24_مقصد<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  عام:
  DEFINE_اندازه_STATIC (4 + HBGlyphID::static_اندازه);
};

بنية PaintColrGlyph
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer HB_استفادهنشده) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    return_trace (c->serializer->check_تخصیص_بده (بیرون->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  عام:
  DEFINE_اندازه_STATIC (3);
};

قالب <قالب<نام_نوع> صنف Var>
بنية PaintTransform
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    إذا (!بیرون->transform.serialize_subset (c, transform, هذا, instancer)) return_trace (خطأ);
    إذا (format == 13 && c->plan->all_axes_pinned)
      بیرون->format = 12;
    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  src.sanitize (c, هذا) &&
                  transform.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    (هذا+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_مقصد<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_مقصد<Var<Affine2x3>>	transform;
  عام:
  DEFINE_اندازه_STATIC (7);
};

بنية PaintTranslate
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      بیرون->dx = hb_clamp_مقصد<int16_t> ((مزدوج) dx + (مزدوج) roundf (instancer (var_Idx_پایه, 0)));
      بیرون->dy = hb_clamp_مقصد<int16_t> ((مزدوج) dy + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
    }

    إذا (format == 15 && c->plan->all_axes_pinned)
        بیرون->format = 14;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم ddx = dx + c->instancer (var_Idx_پایه, 0);
    عائم ddy = dy + c->instancer (var_Idx_پایه, 1);

    c->funcs->افزودن_translate (c->داده, ddx, ddy);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * FWORD::static_اندازه);
};

قالب <نام_نوع منبع_2>
ساكن منطقي serialize_scale_transform (hb_subset_context_t *c,
                                       عائم sx, عائم sy, عائم dx, عائم dy,
                                       ثابت منبع_2 &منبع)
{
  خودکار *بیرون = c->serializer->allocate_اندازه<PaintTransform<NoVariable>> (7);
  إذا (unlikely (!بیرون)) أرجع خطأ;
  بیرون->format = 12;

  c->serializer->push ();
  خودکار *transform = c->serializer->allocate_اندازه<Affine2x3> (Affine2x3::static_اندازه);
  إذا (unlikely (!transform))
  {
    c->serializer->برداشت_discard ();
    أرجع خطأ;
  }
  مجموعه_fixed_clamped (transform->xx, sx);
  transform->yx.مجموعه_float (0);
  transform->xy.مجموعه_float (0);
  مجموعه_fixed_clamped (transform->yy, sy);
  مجموعه_fixed_clamped (transform->dx, dx);
  مجموعه_fixed_clamped (transform->dy, dy);
  c->serializer->جمع_کن_link (بیرون->transform, c->serializer->برداشت_pack ());

  c->serializer->push ();
  منطقي ret = منبع ();
  إذا (ret)
    c->serializer->جمع_کن_link (بیرون->src, c->serializer->برداشت_pack ());
  وإلا
    c->serializer->برداشت_discard ();
  أرجع ret;
}

قالب <نام_نوع پایه_3>
ساكن منطقي subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت پایه_3 *پایه,
                                      ثابت Offset24_مقصد<Paint> &src,
                                      ثابت قلم_Var_Store_Instancer &instancer,
                                      عائم sx, عائم sy,
                                      عائم cx = 0, عائم cy = 0)
{
  إذا (src.is_null ()) أرجع خطأ;
  خودکار منبع = [&] { أرجع c->dispatch (پایه+src, instancer); };
  عائم dx = cx * (1 - sx), dy = cy * (1 - sy);
  إذا (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    أرجع serialize_scale_transform (c, sx, sy, dx, dy, منبع);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  خودکار inner = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, منبع);
  };
  خودکار inverse = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  خودکار scale = [&] {
    أرجع serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  خودکار outer = [&] {
    أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

بنية PaintScale
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.مقصد_float (instancer (var_Idx_پایه, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.مقصد_float (instancer (var_Idx_پایه, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, sx, sy));
    }
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->scaleX, scaleX.مقصد_float (instancer (var_Idx_پایه, 0)));
      مجموعه_fixed_clamped (بیرون->scaleY, scaleY.مقصد_float (instancer (var_Idx_پایه, 1)));
    }

    إذا (format == 17 && c->plan->all_axes_pinned)
        بیرون->format = 16;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.مقصد_float (c->instancer (var_Idx_پایه, 0));
    عائم sy = scaleY.مقصد_float (c->instancer (var_Idx_پایه, 1));

    c->funcs->افزودن_scale (c->داده, sx, sy);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه);
};

بنية PaintScaleAroundCenter
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.مقصد_float (instancer (var_Idx_پایه, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.مقصد_float (instancer (var_Idx_پایه, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_پایه, 2)),
            centerY + roundf (instancer (var_Idx_پایه, 3))));
    }
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->scaleX, scaleX.مقصد_float (instancer (var_Idx_پایه, 0)));
      مجموعه_fixed_clamped (بیرون->scaleY, scaleY.مقصد_float (instancer (var_Idx_پایه, 1)));
      بیرون->centerX = hb_clamp_مقصد<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
      بیرون->centerY = hb_clamp_مقصد<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_پایه, 3)));
    }

    إذا (format == 19 && c->plan->all_axes_pinned)
        بیرون->format = 18;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.مقصد_float (c->instancer (var_Idx_پایه, 0));
    عائم sy = scaleY.مقصد_float (c->instancer (var_Idx_پایه, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_پایه, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_پایه, 3);

    c->funcs->افزودن_scale_around_center (c->داده, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية Paint_Scale_یکنواخت
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.مقصد_float (instancer (var_Idx_پایه, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, s, s));
    }
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
      مجموعه_fixed_clamped (بیرون->scale, scale.مقصد_float (instancer (var_Idx_پایه, 0)));

    إذا (format == 21 && c->plan->all_axes_pinned)
        بیرون->format = 20;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.مقصد_float (c->instancer (var_Idx_پایه, 0));

    c->funcs->افزودن_scale (c->داده, s, s);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه);
};

بنية Paint_Scale_یکنواخت_Around_Center
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.مقصد_float (instancer (var_Idx_پایه, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_پایه, 1)),
            centerY + roundf (instancer (var_Idx_پایه, 2))));
    }
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->scale, scale.مقصد_float (instancer (var_Idx_پایه, 0)));
      بیرون->centerX = hb_clamp_مقصد<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
      بیرون->centerY = hb_clamp_مقصد<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
    }

    إذا (format == 23 && c->plan->all_axes_pinned)
        بیرون->format = 22;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.مقصد_float (c->instancer (var_Idx_پایه, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_پایه, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_پایه, 2);

    c->funcs->افزودن_scale_around_center (c->داده, s, s, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية PaintRotate
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
      مجموعه_fixed_clamped (بیرون->angle, angle.مقصد_float (instancer (var_Idx_پایه, 0)));

    إذا (format == 25 && c->plan->all_axes_pinned)
      بیرون->format = 24;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.مقصد_float (c->instancer (var_Idx_پایه, 0));

    c->funcs->افزودن_rotate (c->داده, a);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه);
};

بنية PaintRotateAroundCenter
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->angle, angle.مقصد_float (instancer (var_Idx_پایه, 0)));
      بیرون->centerX = hb_clamp_مقصد<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_پایه, 1)));
      بیرون->centerY = hb_clamp_مقصد<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
    }

    إذا (format ==27 && c->plan->all_axes_pinned)
        بیرون->format = 26;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.مقصد_float (c->instancer (var_Idx_پایه, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_پایه, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_پایه, 2);

    c->funcs->افزودن_rotate_around_center (c->داده, a, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية PaintSkew
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->xSkewAngle, xSkewAngle.مقصد_float (instancer (var_Idx_پایه, 0)));
      مجموعه_fixed_clamped (بیرون->ySkewAngle, ySkewAngle.مقصد_float (instancer (var_Idx_پایه, 1)));
    }

    إذا (format == 29 && c->plan->all_axes_pinned)
        بیرون->format = 28;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.مقصد_float(c->instancer (var_Idx_پایه, 0));
    عائم sy = ySkewAngle.مقصد_float(c->instancer (var_Idx_پایه, 1));

    c->funcs->افزودن_skew (c->داده, sx, sy);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه);
};

بنية PaintSkewAroundCenter
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      مجموعه_fixed_clamped (بیرون->xSkewAngle, xSkewAngle.مقصد_float (instancer (var_Idx_پایه, 0)));
      مجموعه_fixed_clamped (بیرون->ySkewAngle, ySkewAngle.مقصد_float (instancer (var_Idx_پایه, 1)));
      بیرون->centerX = hb_clamp_مقصد<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_پایه, 2)));
      بیرون->centerY = hb_clamp_مقصد<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_پایه, 3)));
    }

    إذا (format == 31 && c->plan->all_axes_pinned)
        بیرون->format = 30;

    return_trace (بیرون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_پایه) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.مقصد_float(c->instancer (var_Idx_پایه, 0));
    عائم sy = ySkewAngle.مقصد_float(c->instancer (var_Idx_پایه, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_پایه, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_پایه, 3);

    c->funcs->افزودن_skew_around_center (c->داده, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->برداشت_transform (c->داده);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية Paint_مرکب
{
  HB_داخلی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_داخلی فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= بیرون->src.serialize_subset (c, src, هذا, instancer);
    ret |= بیرون->backdrop.serialize_subset (c, backdrop, هذا, instancer);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  c->check_ops (هذا->min_اندازه) && // PainComposite can get exponential
                  src.sanitize (c, هذا) &&
                  backdrop.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    hb_paint_مرکب_نما_t مرکب_نما_2;
    بدل ((بیعلامت صحيح) نما)
    {
      حالة HB_PAINT_مرکب_نما_CLEAR:
      حالة HB_PAINT_مرکب_نما_SRC:
      حالة HB_PAINT_مرکب_نما_DEST:
      حالة HB_PAINT_مرکب_نما_SRC_OVER:
      حالة HB_PAINT_مرکب_نما_DEST_OVER:
      حالة HB_PAINT_مرکب_نما_SRC_درون:
      حالة HB_PAINT_مرکب_نما_DEST_درون:
      حالة HB_PAINT_مرکب_نما_SRC_بیرون:
      حالة HB_PAINT_مرکب_نما_DEST_بیرون:
      حالة HB_PAINT_مرکب_نما_SRC_ATOP:
      حالة HB_PAINT_مرکب_نما_DEST_ATOP:
      حالة HB_PAINT_مرکب_نما_XOR:
      حالة HB_PAINT_مرکب_نما_PLUS:
      حالة HB_PAINT_مرکب_نما_SCREEN:
      حالة HB_PAINT_مرکب_نما_OVERLAY:
      حالة HB_PAINT_مرکب_نما_DARKEN:
      حالة HB_PAINT_مرکب_نما_LIGHTEN:
      حالة HB_PAINT_مرکب_نما_COLOR_DODGE:
      حالة HB_PAINT_مرکب_نما_COLOR_BURN:
      حالة HB_PAINT_مرکب_نما_HARD_LIGHT:
      حالة HB_PAINT_مرکب_نما_SOFT_LIGHT:
      حالة HB_PAINT_مرکب_نما_DIFFERENCE:
      حالة HB_PAINT_مرکب_نما_EXCLUSION:
      حالة HB_PAINT_مرکب_نما_MULTIPLY:
      حالة HB_PAINT_مرکب_نما_HSL_HUE:
      حالة HB_PAINT_مرکب_نما_HSL_SATURATION:
      حالة HB_PAINT_مرکب_نما_HSL_COLOR:
      حالة HB_PAINT_مرکب_نما_HSL_LUMINOSITY:
	مرکب_نما_2 = (hb_paint_مرکب_نما_t) (بیعلامت صحيح) نما;
	اكسر;
      افتراضي:
	مرکب_نما_2 = HB_PAINT_مرکب_نما_CLEAR;
	اكسر;
    }

    c->funcs->افزودن_گروه_for (c->داده, HB_PAINT_مرکب_نما_SRC_OVER);
    c->recurse (هذا+backdrop);
    c->funcs->افزودن_گروه_for (c->داده, مرکب_نما_2);
    c->recurse (هذا+src);
    c->funcs->برداشت_گروه (c->داده, مرکب_نما_2);
    c->funcs->برداشت_گروه (c->داده, HB_PAINT_مرکب_نما_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_مقصد<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  مرکب_نما		نما;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_مقصد<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Clip_Box_داده
{
  صحيح xMin, yMin, xMax, yMax;
};

بنية ClipBoxFormat1
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_clip_box (Clip_Box_داده &clip_box, ثابت قلم_Var_Store_Instancer &instancer HB_استفادهنشده) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer,
               uint32_t var_Idx_پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_پایه != VarIdx::NO_VARIATION)
    {
      بیرون->xMin = hb_clamp_مقصد<int16_t> ((مزدوج) xMin + (مزدوج) floorf (instancer (var_Idx_پایه, 0)));
      بیرون->yMin = hb_clamp_مقصد<int16_t> ((مزدوج) yMin + (مزدوج) floorf (instancer (var_Idx_پایه, 1)));
      بیرون->xMax = hb_clamp_مقصد<int16_t> ((مزدوج) xMax + (مزدوج) ceilf (instancer (var_Idx_پایه, 2)));
      بیرون->yMax = hb_clamp_مقصد<int16_t> ((مزدوج) yMax + (مزدوج) ceilf (instancer (var_Idx_پایه, 3)));
    }

    إذا (format == 2 && c->plan->all_axes_pinned)
        بیرون->format = 1;

    return_trace (درست);
  }

  عام:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عام:
  DEFINE_اندازه_STATIC (1 + 4 * FWORD::static_اندازه);
};

بنية ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  فراغ get_clip_box (Clip_Box_داده &clip_box, ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    إذا (instancer)
    {
      clip_box.xMin = hb_saturate_جمع_کن (clip_box.xMin, hb_clamp_مقصد<صحيح> (floorf (instancer (var_Idx_پایه, 0))));
      clip_box.yMin = hb_saturate_جمع_کن (clip_box.yMin, hb_clamp_مقصد<صحيح> (floorf (instancer (var_Idx_پایه, 1))));
      clip_box.xMax = hb_saturate_جمع_کن (clip_box.xMax, hb_clamp_مقصد<صحيح> (ceilf (instancer (var_Idx_پایه, 2))));
      clip_box.yMax = hb_saturate_جمع_کن (clip_box.yMax, hb_clamp_مقصد<صحيح> (ceilf (instancer (var_Idx_پایه, 3))));
    }
  }

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  { c->variation_indices->جمع_کن_range (var_Idx_پایه, var_Idx_پایه + 3); }
};

بنية ClipBox
{
  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.format2.closurev1 (c); أرجع;
    افتراضي:أرجع;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
                    ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    Clip_Box_داده clip_box;
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      اكسر;
    حالة 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      اكسر;
    افتراضي:
      أرجع خطأ;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->عرض = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->ارتفاع = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    أرجع درست;
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

بنية Clip_رکورد
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروع_Glyph_ID ? -1 : g <= انتها_Glyph_ID ? 0 : +1; }

  فراغ closurev1 (hb_colrv1_بستگی_context_t* c, ثابت فراغ *پایه) ثابت
  {
    إذا (!c->glyphs->intersects (شروع_Glyph_ID, انتها_Glyph_ID)) أرجع;
    (پایه+clipBox).closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *پایه,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    return_trace (بیرون->clipBox.serialize_subset (c, clipBox, پایه, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && clipBox.sanitize (c, پایه));
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
		    ثابت فراغ *پایه,
		    ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    أرجع (پایه+clipBox).get_extents (extents, instancer);
  }

  عام:
  HBUINT16		شروع_Glyph_ID;  // first gid clip applies to
  HBUINT16		انتها_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_مقصد<ClipBox>	clipBox;   // Box or VarBox
  عام:
  DEFINE_اندازه_STATIC (7);
};
DECLARE_NULL_NAMESPACE_بایتها (OT, Clip_رکورد);

بنية ClipList
{
  بیعلامت serialize_clip_رکوردها (hb_subset_context_t *c,
                                   ثابت قلم_Var_Store_Instancer &instancer,
                                   ثابت hb_مجموعه_t& gids,
                                   ثابت hb_map_t& gid_جابه_جایی_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (gids.is_empty () ||
        gid_جابه_جایی_map.get_population () != gids.get_population ())
      return_trace (0);

    بیعلامت شمار  = 0;

    hb_codepoint_t شروع_gid= gids.get_min ();
    hb_codepoint_t prev_gid = شروع_gid;

    بیعلامت جابه_جایی = gid_جابه_جایی_map.get (شروع_gid);
    بیعلامت prev_جابه_جایی = جابه_جایی;
    لكل (ثابت hb_codepoint_t _ : gids.iter ())
    {
      إذا (_ == شروع_gid) استمر;

      جابه_جایی = gid_جابه_جایی_map.get (_);
      إذا (_ == prev_gid + 1 &&  جابه_جایی == prev_جابه_جایی)
      {
        prev_gid = _;
        استمر;
      }

      Clip_رکورد رکورد;
      رکورد.شروع_Glyph_ID = شروع_gid;
      رکورد.انتها_Glyph_ID = prev_gid;
      رکورد.clipBox = prev_جابه_جایی;

      إذا (!رکورد.subset (c, هذا, instancer)) return_trace (0);
      شمار++;

      شروع_gid = _;
      prev_gid = _;
      prev_جابه_جایی = جابه_جایی;
    }

    //last one
    {
      Clip_رکورد رکورد;
      رکورد.شروع_Glyph_ID = شروع_gid;
      رکورد.انتها_Glyph_ID = prev_gid;
      رکورد.clipBox = prev_جابه_جایی;
      إذا (!رکورد.subset (c, هذا, instancer)) return_trace (0);
      شمار++;
    }
    return_trace (شمار);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    إذا (!c->serializer->check_تخصیص_بده (بیرون->format, format, HB_SERIALIZE_ERROR_INT_سرریز)) return_trace (خطأ);

    ثابت hb_مجموعه_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_جابه_جایی_map;
    hb_مجموعه_t new_gids;
    لكل (ثابت Clip_رکورد& رکورد : clips.iter ())
    {
      بیعلامت شروع_gid = رکورد.شروع_Glyph_ID;
      بیعلامت انتها_gid = رکورد.انتها_Glyph_ID;
      لكل (بیعلامت gid = شروع_gid; gid <= انتها_gid; gid++)
      {
        إذا (!glyphset.has (gid) || !glyph_map.has (gid)) استمر;
        بیعلامت new_gid = glyph_map.get (gid);
        new_gid_جابه_جایی_map.مجموعه (new_gid, رکورد.clipBox);
        new_gids.جمع_کن (new_gid);
      }
    }

    بیعلامت شمار = serialize_clip_رکوردها (c, instancer, new_gids, new_gid_جابه_جایی_map);
    إذا (!شمار) return_trace (خطأ);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->clips.len, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (هذا) && clips.sanitize (c, هذا));
  }

  منطقي
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    خودکار *rec = clips.as_array ().bsearch (gid);
    إذا (rec)
      أرجع rec->get_extents (extents, هذا, instancer);
    أرجع خطأ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_از<Clip_رکورد>	clips;  // Clip records, sorted by startGlyphID
  عام:
  DEFINE_اندازه_ARRAY_SIZED (5, clips);
};

بنية Paint
{

  قالب <نام_نوع ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_شروع_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_مقدار ());

    return_trace (c->انتها_recursion (هذا->dispatch (c, std::forward<Ts> (ds)...)));
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    حالة 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    حالة 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    حالة 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    حالة 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    حالة 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    حالة 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    حالة 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    حالة 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    حالة 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    حالة 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    حالة 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    حالة 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    حالة 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    حالة 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    حالة 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    حالة 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    حالة 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    حالة 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    حالة 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    حالة 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    حالة 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    حالة 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    حالة 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    حالة 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    حالة 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    حالة 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  منطقي get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); أرجع درست;
    حالة 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); أرجع درست;
    افتراضي: أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }				format;
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
  NoVariable<Paint_Scale_یکنواخت>			paintformat20;
  Variable<Paint_Scale_یکنواخت>			paintformat21;
  NoVariable<Paint_Scale_یکنواخت_Around_Center>	paintformat22;
  Variable<Paint_Scale_یکنواخت_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_مرکب				paintformat32;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_MIN (2);
};

بنية پایه_Glyph_Paint_رکورد
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت فراغ* src_پایه, hb_subset_context_t *c,
                  ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = s->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    إذا (!s->check_تخصیص_بده (بیرون->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    return_trace (بیرون->paint.serialize_subset (c, paint, src_پایه, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && paint.sanitize (c, پایه)));
  }

  عام:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_مقصد<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية پایه_Glyph_List : Sorted_Array32_از<پایه_Glyph_Paint_رکورد>
{
  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون)))  return_trace (خطأ);
    ثابت hb_مجموعه_t* glyphset = &c->plan->_glyphset_colred;

    لكل (ثابت خودکار& _ : as_array ())
    {
      بیعلامت gid = _.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      إذا (_.serialize (c->serializer, c->plan->glyph_map, هذا, c, instancer)) بیرون->len++;
      وإلا return_trace (خطأ);
    }

    return_trace (بیرون->len != 0);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Sorted_Array32_از<پایه_Glyph_Paint_رکورد>::sanitize (c, هذا));
  }
};

بنية LayerList : Array32_از_Offset32_مقصد<Paint>
{
  ثابت Paint& get_paint (بیعلامت i) ثابت
  { أرجع هذا+(*هذا)[i]; }

  منطقي subset (hb_subset_context_t *c,
               ثابت قلم_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودکار& _ : + hb_enumerate (*هذا)
                         | hb_filter (c->plan->colrv1_layers, hb_اولی))

    {
      خودکار *o = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      ret |= o->serialize_subset (c, _.second, هذا, instancer);
    }
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Array32_از_Offset32_مقصد<Paint>::sanitize (c, هذا));
  }
};

بنية delta_مجموعه_شاخص_map_subset_plan_t
{
  بیعلامت get_inner_بیت_شمار () ثابت { أرجع inner_بیت_شمار; }
  بیعلامت get_عرض ()           ثابت { أرجع ((outer_بیت_شمار + inner_بیت_شمار + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  delta_مجموعه_شاخص_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_شمار = 0;
    outer_بیت_شمار = 0;
    inner_بیت_شمار = 1;
    output_map.init ();

    /* search backwards */
    بیعلامت شمار = new_deltaset_idx_varidx_map.get_population ();
    إذا (!شمار) أرجع;

    بیعلامت آخرین_idx = (بیعلامت)-1;
    بیعلامت آخرین_varidx = (بیعلامت)-1;

    لكل (بیعلامت i = شمار; i; i--)
    {
      بیعلامت delta_مجموعه_idx = i - 1;
      بیعلامت var_idx = new_deltaset_idx_varidx_map.get (delta_مجموعه_idx);
      إذا (i == شمار)
      {
        آخرین_idx = delta_مجموعه_idx;
        آخرین_varidx = var_idx;
        استمر;
      }
      إذا (var_idx != آخرین_varidx)
        اكسر;
      آخرین_idx = delta_مجموعه_idx;
    }

    map_شمار = آخرین_idx + 1;
  }

  منطقي remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_بیت_شمار = 1;
    inner_بیت_شمار = 1;

    إذا (unlikely (!output_map.resize_dirty (map_شمار))) أرجع خطأ;

    لكل (بیعلامت idx = 0; idx < map_شمار; idx++)
    {
      uint32_t *var_idx;
      إذا (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) أرجع خطأ;
      output_map.arrayZ[idx] = *var_idx;

      بیعلامت outer = (*var_idx) >> 16;
      بیعلامت بیت_شمار = (outer == 0) ? 1 : hb_بیت_storage (outer);
      outer_بیت_شمار = hb_max (بیت_شمار, outer_بیت_شمار);

      بیعلامت inner = (*var_idx) & 0xFFFF;
      بیت_شمار = (inner == 0) ? 1 : hb_بیت_storage (inner);
      inner_بیت_شمار = hb_max (بیت_شمار, inner_بیت_شمار);
    }
    أرجع درست;
  }

  خاص:
  بیعلامت map_شمار;
  بیعلامت outer_بیت_شمار;
  بیعلامت inner_بیت_شمار;
  hb_وکتور_t<uint32_t> output_map;
};

بنية COLR
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_COLR;

  منطقي has_داده () ثابت { أرجع has_v0_داده () || نسخه; }

  منطقي has_v0_داده () ثابت { أرجع رقم_پایه_Glyphs; }
  منطقي has_v1_داده () ثابت
  {
    إذا (نسخه < 1)
      أرجع خطأ;
    hb_barrier ();

    أرجع (هذا+پایه_Glyph_List_2).len > 0;
  }

  بیعلامت صحيح get_glyph_layers (hb_codepoint_t       glyph,
				 بیعلامت صحيح         شروع_جابه_جایی,
				 بیعلامت صحيح        *شمار, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت پایه_Glyph_رکورد &رکورد = (هذا+پایه_Glyphs_Z).bsearch (رقم_پایه_Glyphs, glyph);

    hb_array_t<ثابت Layer_رکورد> all_layers = (هذا+layersZ).as_array (رقم_Layers);
    hb_array_t<ثابت Layer_رکورد> glyph_layers = all_layers.sub_array (رکورد.اولی_Layer_Idx,
								       رکورد.رقم_Layers);
    إذا (شمار && layers)
    {
      + glyph_layers.sub_array (شروع_جابه_جایی, شمار)
      | hb_sink (hb_array (layers, *شمار))
      ;
    }
    أرجع glyph_layers.درازی;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_جدول<COLR> (face); }

    ~accelerator_t ()
    {
      خودکار *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقي has_داده () ثابت { أرجع colr->has_داده (); }

#اگر_تعریف_نشده HB_NO_PAINT
    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      إذا (unlikely (!has_داده ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع درست;
      منطقي ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, فراغ *داده,
		      بیعلامت صحيح palette_شاخص,
		      hb_color_t foreground,
		      منطقي clip = درست) ثابت
    {
      إذا (unlikely (!has_داده ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع درست;
      منطقي ret = colr->paint_glyph (font, glyph, funcs, داده, palette_شاخص, foreground, clip, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }
#پایان_اگر

    منطقي is_valid () { أرجع colr.get_blob ()->درازی; }

    فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت
    {
      إذا (!has_داده ()) أرجع;
      colr->depend (depend_داده);
    }

    فراغ بستگی_glyphs (hb_codepoint_t glyph,
			 hb_مجموعه_t *related_ids /*بیرون*/) ثابت
    { colr->بستگی_glyphs (glyph, related_ids); }

    فراغ بستگی_V0palette_indices (ثابت hb_مجموعه_t *glyphs,
				    hb_مجموعه_t *palettes /*بیرون*/) ثابت
    { colr->بستگی_V0palette_indices (glyphs, palettes); }

    فراغ بستگی_for_V1 (hb_مجموعه_t *glyphset,
                        hb_مجموعه_t *layer_indices,
                        hb_مجموعه_t *palette_indices,
                        hb_مجموعه_t *variation_indices,
                        hb_مجموعه_t *delta_مجموعه_indices) ثابت
    { colr->بستگی_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_مجموعه_indices); }

    منطقي has_var_store () ثابت
    { أرجع colr->has_var_store (); }

    ثابت قلم_Variation_Store &get_var_store () ثابت
    { أرجع colr->get_var_store (); }
    ثابت قلم_Variation_Store *get_var_store_ptr () ثابت
    { أرجع colr->get_var_store_ptr (); }

    منطقي has_delta_مجموعه_شاخص_map () ثابت
    { أرجع colr->has_delta_مجموعه_شاخص_map (); }

    ثابت Delta_مجموعه_شاخص_Map &get_delta_مجموعه_شاخص_map () ثابت
    { أرجع colr->get_delta_مجموعه_شاخص_map (); }
    ثابت Delta_مجموعه_شاخص_Map *get_delta_مجموعه_شاخص_map_ptr () ثابت
    { أرجع colr->get_delta_مجموعه_شاخص_map_ptr (); }

    خاص:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, اشاره_تهی)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع اشاره_تهی;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (اشاره_تهی, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عام:
    hb_blob_ptr_t<COLR> colr;
    خاص:
    تغییرپذیر hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت
  {
    // v0
    hb_array_t<ثابت پایه_Glyph_رکورد> پایه_Glyphs = (هذا+پایه_Glyphs_Z).as_array (رقم_پایه_Glyphs);
    لكل (ثابت پایه_Glyph_رکورد &رکورد : پایه_Glyphs)
    {
      خودکار glyph_layers = (هذا+layersZ).as_array (رقم_Layers).sub_array (رکورد.اولی_Layer_Idx,
                                                                         رکورد.رقم_Layers);
      لكل (ثابت Layer_رکورد layer : glyph_layers)
        depend_داده->جمع_کن_depend(رکورد.glyphId, HB_OT_برچسب_COLR, layer.glyphId);
    }

    إذا (!has_v1_داده())
      أرجع;

    // v1
    hb_colrv1_depend_context_t c (هذا, depend_داده);
    ثابت پایه_Glyph_List &baseglyph_paintrecords = هذا+پایه_Glyph_List_2;

    لكل (ثابت پایه_Glyph_Paint_رکورد &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.منبع_gid = baseglyph_paintrecord.glyphId;
      c.دیدهشده_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  فراغ بستگی_glyphs (hb_codepoint_t glyph,
		       hb_مجموعه_t *related_ids /*بیرون*/) ثابت
  {
    ثابت پایه_Glyph_رکورد *رکورد = get_پایه_glyph_رکورد (glyph);
    إذا (!رکورد) أرجع;

    خودکار glyph_layers = (هذا+layersZ).as_array (رقم_Layers).sub_array (رکورد->اولی_Layer_Idx,
								       رکورد->رقم_Layers);
    إذا (!glyph_layers.درازی) أرجع;
    related_ids->جمع_کن_array (&glyph_layers[0].glyphId, glyph_layers.درازی, Layer_رکورد::min_اندازه);
  }

  فراغ بستگی_V0palette_indices (ثابت hb_مجموعه_t *glyphs,
				  hb_مجموعه_t *palettes /*بیرون*/) ثابت
  {
    إذا (!رقم_پایه_Glyphs || !رقم_Layers) أرجع;
    hb_array_t<ثابت پایه_Glyph_رکورد> پایه_Glyphs = (هذا+پایه_Glyphs_Z).as_array (رقم_پایه_Glyphs);
    hb_array_t<ثابت Layer_رکورد> all_layers = (هذا+layersZ).as_array (رقم_Layers);

    لكل (ثابت پایه_Glyph_رکورد رکورد : پایه_Glyphs)
    {
      إذا (!glyphs->has (رکورد.glyphId)) استمر;
      hb_array_t<ثابت Layer_رکورد> glyph_layers = all_layers.sub_array (رکورد.اولی_Layer_Idx,
                                                                   رکورد.رقم_Layers);
      لكل (ثابت Layer_رکورد layer : glyph_layers)
        palettes->جمع_کن (layer.colorIdx);
    }
  }

  فراغ بستگی_for_V1 (hb_مجموعه_t *glyphset,
                      hb_مجموعه_t *layer_indices,
                      hb_مجموعه_t *palette_indices,
                      hb_مجموعه_t *variation_indices,
                      hb_مجموعه_t *delta_مجموعه_indices) ثابت
  {
    إذا (نسخه < 1) أرجع;
    hb_barrier ();

    hb_مجموعه_t دیدهشده_glyphs;

    hb_colrv1_بستگی_context_t c (هذا, &دیدهشده_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت پایه_Glyph_List &baseglyph_paintrecords = هذا+پایه_Glyph_List_2;

    لكل (ثابت پایه_Glyph_Paint_رکورد &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      بیعلامت gid = baseglyph_paintrecord.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_مجموعه_union (glyphset, &دیدهشده_glyphs);

    ثابت ClipList &cliplist = هذا+clipList;
    c.glyphs = glyphset;
    لكل (ثابت Clip_رکورد &clip_رکورد : cliplist.clips.iter())
      clip_رکورد.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    إذا (has_delta_مجموعه_شاخص_map ())
    {
      ثابت Delta_مجموعه_شاخص_Map &var_idx_map = هذا+varIdxMap;
      delta_مجموعه_indices->مجموعه (*variation_indices);
      variation_indices->clear ();
      لكل (بیعلامت delta_مجموعه_idx : *delta_مجموعه_indices)
        variation_indices->جمع_کن (var_idx_map.map (delta_مجموعه_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { أرجع (هذا+layerList); }

  ثابت پایه_Glyph_List& get_baseglyphList () ثابت
  { أرجع (هذا+پایه_Glyph_List_2); }

  منطقي has_var_store () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && varStore != 0; }

  منطقي has_delta_مجموعه_شاخص_map () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقي has_clip_list () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && clipList != 0; }

  ثابت Delta_مجموعه_شاخص_Map &get_delta_مجموعه_شاخص_map () ثابت
  { أرجع has_delta_مجموعه_شاخص_map () && hb_barrier () ? هذا+varIdxMap : Null (Delta_مجموعه_شاخص_Map); }
  ثابت Delta_مجموعه_شاخص_Map *get_delta_مجموعه_شاخص_map_ptr () ثابت
  { أرجع has_delta_مجموعه_شاخص_map () && hb_barrier () ? &(هذا+varIdxMap) : اشاره_تهی; }

  ثابت قلم_Variation_Store &get_var_store () ثابت
  { أرجع has_var_store () && hb_barrier () ? هذا+varStore : Null (قلم_Variation_Store); }
  ثابت قلم_Variation_Store *get_var_store_ptr () ثابت
  { أرجع has_var_store () && hb_barrier () ? &(هذا+varStore) : اشاره_تهی; }

  ثابت ClipList &get_clip_list () ثابت
  { أرجع has_clip_list () && hb_barrier () ? هذا+clipList : Null (ClipList); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  (هذا+پایه_Glyphs_Z).sanitize (c, رقم_پایه_Glyphs) &&
                  (هذا+layersZ).sanitize (c, رقم_Layers) &&
                  (نسخه == 0 ||
		   (hb_barrier () &&
		    پایه_Glyph_List_2.sanitize (c, هذا) &&
		    layerList.sanitize (c, هذا) &&
		    clipList.sanitize (c, هذا) &&
		    varIdxMap.sanitize (c, هذا) &&
		    varStore.sanitize (c, هذا))));
  }

  قالب<نام_نوع پایه_Iterator, نام_نوع LayerIterator,
	   hb_requires (hb_is_iterator (پایه_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقي serialize_V0 (hb_serialize_context_t *c,
		     بیعلامت نسخه,
		     پایه_Iterator پایه_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (پایه_it.len () != layer_it.len ()))
      return_trace (خطأ);

    هذا->نسخه = نسخه;
    رقم_Layers = 0;
    رقم_پایه_Glyphs = پایه_it.len ();
    إذا (رقم_پایه_Glyphs == 0)
    {
      پایه_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (درست);
    }

    c->push ();
    لكل (ثابت hb_قلم_type<پایه_Iterator> _ : + پایه_it.iter ())
    {
      خودکار* رکورد = c->embed (_);
      إذا (unlikely (!رکورد)) return_trace (خطأ);
      رکورد->اولی_Layer_Idx = رقم_Layers;
      رقم_Layers += رکورد->رقم_Layers;
    }
    c->جمع_کن_link (پایه_Glyphs_Z, c->برداشت_pack ());

    c->push ();
    لكل (ثابت hb_قلم_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->جمع_کن_link (layersZ, c->برداشت_pack ());

    return_trace (درست);
  }

  ثابت پایه_Glyph_رکورد* get_پایه_glyph_رکورد (hb_codepoint_t gid) ثابت
  {
    ثابت پایه_Glyph_رکورد* رکورد = &(هذا+پایه_Glyphs_Z).bsearch (رقم_پایه_Glyphs, (بیعلامت صحيح) gid);
    إذا (رکورد == &Null (پایه_Glyph_رکورد) ||
        (رکورد && (hb_codepoint_t) رکورد->glyphId != gid))
      رکورد = اشاره_تهی;
    أرجع رکورد;
  }

  ثابت پایه_Glyph_Paint_رکورد* get_پایه_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت پایه_Glyph_List &list = هذا+پایه_Glyph_List_2;
    بیعلامت صحيح i;
    إذا (!list.bfind ((بیعلامت) gid, &i))
      أرجع اشاره_تهی;
    أرجع &list[i];
  }

  منطقي downgrade_مقصد_V0 (ثابت hb_مجموعه_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    لكل (ثابت پایه_Glyph_Paint_رکورد& _ : get_baseglyphList ())
      إذا (glyphset.has (_.glyphId))
        أرجع خطأ;

    أرجع درست;
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        COLR* بیرون /*بیرون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (درست);

    ثابت قلم_Variation_Store& var_store = هذا+varStore;
    إذا (c->plan->normalized_coords)
    {
      قلم_variations_t قلم_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقي optimize = (varIdxMap != 0) ? درست : خطأ;
      إذا (!قلم_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (خطأ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      إذا (قلم_vars.get_region_list () &&
          قلم_vars.get_vardata_encodings () &&
          !بیرون->varStore.serialize_serialize (c->serializer,
                                              قلم_vars.has_long_کلمه (),
                                              c->plan->axis_برچسبها,
                                              قلم_vars.get_region_list (),
                                              قلم_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      إذا (optimize)
      {
        ثابت hb_map_t &varidx_map = قلم_vars.get_varidx_map ();
        لكل (خودکار _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          إذا (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          وإلا
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_مجموعه_t dead_regions;
      إذا (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_شاخص_برچسب_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      إذا (unlikely (!بیرون->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : اشاره_تهی)))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  منطقي subset_delta_مجموعه_شاخص_map (hb_subset_context_t *c,
                                   COLR* بیرون /*بیرون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (درست);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_مجموعه_شاخص_map_subset_plan_t شاخص_map_plan (deltaset_idx_varidx_map);

    إذا (unlikely (!شاخص_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (خطأ);

    return_trace (بیرون->varIdxMap.serialize_serialize (c->serializer, شاخص_map_plan));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &معکوس_glyph_map = *c->plan->معکوس_glyph_map;
    ثابت hb_مجموعه_t& glyphset = c->plan->_glyphset_colred;

    خودکار پایه_it =
    + hb_range (c->plan->رقم_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = معکوس_glyph_map.get (new_gid);
		    إذا (glyphset.has (old_gid)) أرجع درست;
		    أرجع خطأ;
		 })
    | hb_map_retains_ترتیبدهی ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = معکوس_glyph_map.get (new_gid);

				ثابت پایه_Glyph_رکورد* old_رکورد = get_پایه_glyph_رکورد (old_gid);
				إذا (unlikely (!old_رکورد))
				  أرجع hb_pair_t<منطقي, پایه_Glyph_رکورد> (خطأ, Null (پایه_Glyph_رکورد));
				پایه_Glyph_رکورد new_رکورد = {};
				new_رکورد.glyphId = new_gid;
				new_رکورد.رقم_Layers = old_رکورد->رقم_Layers;
				أرجع hb_pair_t<منطقي, پایه_Glyph_رکورد> (درست, new_رکورد);
			      })
    | hb_filter (hb_اولی)
    | hb_map_retains_ترتیبدهی (hb_دومی)
    ;

    خودکار layer_it =
    + hb_range (c->plan->رقم_output_glyphs ())
    | hb_map (معکوس_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_ترتیبدهی ([&](hb_codepoint_t old_gid)
			      {
				ثابت پایه_Glyph_رکورد* old_رکورد = get_پایه_glyph_رکورد (old_gid);
				hb_وکتور_t<Layer_رکورد> بیرون_layers;

				إذا (unlikely (!old_رکورد ||
					      old_رکورد->اولی_Layer_Idx >= رقم_Layers ||
					      old_رکورد->اولی_Layer_Idx + old_رکورد->رقم_Layers > رقم_Layers))
				  أرجع hb_pair_t<منطقي, hb_وکتور_t<Layer_رکورد>> (خطأ, بیرون_layers);

				خودکار layers = (هذا+layersZ).as_array (رقم_Layers).sub_array (old_رکورد->اولی_Layer_Idx,
											     old_رکورد->رقم_Layers);
				بیرون_layers.resize (layers.درازی);
				لكل (بیعلامت صحيح i = 0; i < layers.درازی; i++) {
				  بیرون_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  إذا (unlikely (!c->plan->new_gid_for_old_gid (بیرون_layers[i].glyphId, &new_gid)))
				    أرجع hb_pair_t<منطقي, hb_وکتور_t<Layer_رکورد>> (خطأ, بیرون_layers);
				  بیرون_layers[i].glyphId = new_gid;
				  بیرون_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				أرجع hb_pair_t<منطقي, hb_وکتور_t<Layer_رکورد>> (درست, بیرون_layers);
			      })
    | hb_filter (hb_اولی)
    | hb_map_retains_ترتیبدهی (hb_دومی)
    ;

    إذا (نسخه == 0 && (!پایه_it || !layer_it))
      return_trace (خطأ);

    خودکار *colr_اول = c->serializer->شروع_embed<COLR> ();
    إذا (unlikely (!c->serializer->extend_min (colr_اول)))  return_trace (خطأ);

    إذا (نسخه == 0 || downgrade_مقصد_V0 (glyphset))
      return_trace (colr_اول->serialize_V0 (c->serializer, 0, پایه_it, layer_it));

    hb_barrier ();

    //start version 1
    إذا (!c->serializer->allocate_اندازه<فراغ> (5 * HBUINT32::static_اندازه)) return_trace (خطأ);
    إذا (!colr_اول->serialize_V0 (c->serializer, نسخه, پایه_it, layer_it)) return_trace (خطأ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    إذا (!subset_varstore (c, colr_اول)) return_trace (خطأ);

    قلم_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : اشاره_تهی,
				     c->plan->normalized_coords ? get_delta_مجموعه_شاخص_map_ptr () : اشاره_تهی,
				     c->plan->normalized_coords.as_array ());

    إذا (!colr_اول->پایه_Glyph_List_2.serialize_subset (c, پایه_Glyph_List_2, هذا, instancer))
      return_trace (خطأ);

    colr_اول->layerList.serialize_subset (c, layerList, هذا, instancer);
    colr_اول->clipList.serialize_subset (c, clipList, هذا, instancer);

    return_trace (subset_delta_مجموعه_شاخص_map (c, colr_اول));
  }

  ثابت Paint *get_پایه_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت پایه_Glyph_List &baseglyph_paintrecords = هذا+پایه_Glyph_List_2;
    ثابت پایه_Glyph_Paint_رکورد* رکورد = get_پایه_glyph_paintrecord (glyph);
    إذا (رکورد)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+رکورد->paint;
      أرجع &paint;
    }
    وإلا
      أرجع اشاره_تهی;
  }

#اگر_تعریف_نشده HB_NO_PAINT
  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    قلم_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_مجموعه_شاخص_map_ptr (),
                                     hb_array (font->coords, font->رقم_coords));

    إذا (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      أرجع درست;
    }

    خودکار *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقي ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), درست, scratch);

    خودکار e = scratch.paint_extents.get_extents ();
    إذا (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->عرض = 0;
      extents->ارتفاع = 0;
    }
    وإلا
      *extents = e.مقصد_glyph_extents ();

    أرجع ret;
  }
#پایان_اگر

  منطقي
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    إذا (نسخه >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_پایه_glyph_paint (glyph);

      أرجع paint != اشاره_تهی;
    }

    أرجع خطأ;
  }

  منطقي get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت قلم_Var_Store_Instancer instancer) ثابت
  {
    أرجع get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#اگر_تعریف_نشده HB_NO_PAINT
  منطقي
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, فراغ *داده,
	       بیعلامت صحيح palette_شاخص, hb_color_t foreground,
	       منطقي clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    قلم_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_مجموعه_شاخص_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->رقم_coords : 0));
    hb_paint_context_t c (هذا, funcs, داده, font, palette_شاخص, foreground, instancer);

    hb_decycler_گره_t node (c.glyphs_decycler);
    node.بازدید (glyph);

    إذا (نسخه >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_پایه_glyph_paint (glyph);
      إذا (paint)
      {
        // COLRv1 glyph

	منطقي is_bounded = درست;
	hb_glyph_extents_t extents = {};
	إذا (clip)
	{
	  إذا (!get_clip (glyph, &extents, instancer))
	  {
	    clip = خطأ;
	    is_bounded = خطأ;
	  }

	  إذا (!is_bounded)
	  {
	    خودکار *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_شاخص, foreground,
			 خطأ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->افزودن_font_transform (c.داده, font);

	إذا (clip)
	  c.funcs->افزودن_clip_rectangle (c.داده,
					(عائم) extents.x_bearing,
					(عائم) extents.y_bearing + extents.ارتفاع,
					(عائم) extents.x_bearing + extents.عرض,
					(عائم) extents.y_bearing);

	إذا (is_bounded)
	  c.recurse (*paint);

	إذا (clip)
	  c.funcs->برداشت_clip (c.داده);

	c.funcs->برداشت_transform (c.داده);

        أرجع درست;
      }
    }

    ثابت پایه_Glyph_رکورد *رکورد = get_پایه_glyph_رکورد (glyph);
    إذا (رکورد && ((hb_codepoint_t) رکورد->glyphId == glyph))
    {
      // COLRv0 glyph
      لكل (ثابت خودکار &r : (هذا+layersZ).as_array (رقم_Layers)
			   .sub_array (رکورد->اولی_Layer_Idx, رکورد->رقم_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.داده, r.glyphId, c.font, is_foreground, color);
      }

      أرجع درست;
    }

    أرجع خطأ;
  }
#پایان_اگر

  محمي:
  HBUINT16	نسخه;	/* Table version number (starts at 0). */
  HBUINT16	رقم_پایه_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_مقصد<Sorted_Unsized_Array_از<پایه_Glyph_رکورد>>
		پایه_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_مقصد<Unsized_Array_از<Layer_رکورد>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	رقم_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_مقصد<پایه_Glyph_List>		پایه_Glyph_List_2;
  Offset32_مقصد<LayerList>			layerList;
  Offset32_مقصد<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_مقصد<Delta_مجموعه_شاخص_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_مقصد<قلم_Variation_Store>	varStore;
  عام:
  DEFINE_اندازه_MIN (14);
};

بنية COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

فراغ
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  إذا (unlikely (عمق_چپ <= 0 || یال_شمار <= 0)) أرجع;
  عمق_چپ--;
  یال_شمار--;
  paint.dispatch (هذا);
  عمق_چپ++;
}

قالب <نام_نوع Types>
فراغ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  إذا (likely (c->عمق_چپ > 0 && c->یال_شمار > 0) &&
      (هذا+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->یال_شمار--;
    c->funcs->افزودن_inverse_font_transform (c->داده, c->font);
    c->funcs->fill_glyph (c->داده, gid, c->font, is_foreground, color);
    c->funcs->برداشت_transform (c->داده);
    أرجع;
  }

  c->funcs->افزودن_inverse_font_transform (c->داده, c->font);
  c->funcs->افزودن_clip_glyph (c->داده, gid, c->font);
  c->funcs->افزودن_font_transform (c->داده, c->font);
  c->recurse (هذا+paint);
  c->funcs->برداشت_transform (c->داده);
  c->funcs->برداشت_clip (c->داده);
  c->funcs->برداشت_transform (c->داده);
}

فراغ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);
  ثابت LayerList &paint_جابه_جایی_lists = c->get_colr_جدول ()->get_layerList ();
  hb_decycler_گره_t node (c->layers_decycler);
  لكل (بیعلامت i = اولی_Layer_شاخص; i < اولی_Layer_شاخص + رقم_Layers; i++)
  {
    إذا (unlikely (!node.بازدید (i)))
      أرجع;

    ثابت Paint &paint = paint_جابه_جایی_lists.get_paint (i);
    c->recurse (paint);
  }
}

فراغ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_decycler_گره_t node (c->glyphs_decycler);
  إذا (unlikely (!node.بازدید (gid)))
    أرجع;

  c->funcs->افزودن_inverse_font_transform (c->داده, c->font);
  إذا (c->funcs->color_glyph (c->داده, gid, c->font))
  {
    c->funcs->برداشت_transform (c->داده);
    أرجع;
  }
  c->funcs->برداشت_transform (c->داده);

  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت Paint *paint = colr_جدول->get_پایه_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقي has_clip_box = colr_جدول->get_clip (gid, &extents, c->instancer);

  إذا (has_clip_box)
    c->funcs->افزودن_clip_rectangle (c->داده,
				   (عائم) extents.x_bearing,
				   (عائم) extents.y_bearing + extents.ارتفاع,
				   (عائم) extents.x_bearing + extents.عرض,
				   (عائم) extents.y_bearing);

  إذا (paint)
    c->recurse (*paint);

  إذا (has_clip_box)
    c->funcs->برداشت_clip (c->داده);
}

} /* namespace OT */

#پایان_اگر /* OT_COLOR_COLR_COLR_HH */
