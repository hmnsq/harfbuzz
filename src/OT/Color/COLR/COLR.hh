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

#که_نه_تعریف_شوی OT_COLOR_COLR_COLR_HH
#تعریف_کړه OT_COLOR_COLR_COLR_HH

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
#تعریف_کړه HB_OT_نښان_COLR HB_نښان('C','O','L','R')

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
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (بنسټ); }

عام:
  ثابت فراغ *بنسټ;
  hb_paint_funcs_t *funcs;
  فراغ *معلومات;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  توکی_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح ژوروالی_کیڼ = HB_MAX_NESTING_LEVEL;
  صحيح څنډه_شمېر = HB_MAX_ګراف_څنډه_شمېر;

  hb_paint_context_t (ثابت فراغ *بنسټ_2,
		      hb_paint_funcs_t *funcs_,
		      فراغ *معلومات_2,
                      hb_font_t *font_,
                      بینښه صحيح palette_,
                      hb_color_t foreground_,
		      توکی_Var_Store_Instancer &instancer_) :
    بنسټ (بنسټ_2),
    funcs (funcs_),
    معلومات (معلومات_2),
    font (font_),
    palette (
#که_نه_تعریف_شوی HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->جدول.CPAL->get_palette_colors (palette_ < font->face->جدول.CPAL->get_palette_شمېر () ? palette_ : 0)
#د_که_پای
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_سېټ_synthetic_bold (font, 0, 0, ریښتیا);
      hb_font_سېټ_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساكن بینښه
  color_alpha (hb_color_t color, عائم alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    أرجع roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (بینښه صحيح color_شاخص, عائم alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = ریښتیا;

    إذا (color_شاخص != 0xffff)
    {
      إذا (!funcs->custom_palette_color (معلومات, color_شاخص, &color))
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

بنية hb_colrv1_تړون_context_t :
       hb_dispatch_context_t<hb_colrv1_تړون_context_t>
{
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_کیڼ == 0))
      أرجع hb_تشه_t ();

    إذا (paint_لیدل_شوی (&obj))
      أرجع hb_تشه_t ();

    nesting_level_کیڼ--;
    obj.closurev1 (هذا);
    nesting_level_کیڼ++;
    أرجع hb_تشه_t ();
  }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  منطقي paint_لیدل_شوی (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) بنسټ);
    إذا (لیدل_شوی_paint.دننه_error() || لیدل_شوی_paint.has (delta))
      أرجع ریښتیا;

    لیدل_شوی_paint.جمع_کړه (delta);
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (بنسټ); }

  فراغ جمع_کړه_glyph (بینښه glyph_id)
  { glyphs->جمع_کړه (glyph_id); }

  فراغ جمع_کړه_layer_indices (بینښه لومړی_layer_شاخص, بینښه عدد_د_layers)
  {
    إذا (عدد_د_layers == 0) أرجع;
    layer_indices->جمع_کړه_range (لومړی_layer_شاخص, لومړی_layer_شاخص + عدد_د_layers - 1);
  }

  فراغ جمع_کړه_palette_شاخص (بینښه palette_شاخص)
  { palette_indices->جمع_کړه (palette_شاخص); }

  فراغ جمع_کړه_var_idxes (بینښه لومړی_var_idx, بینښه عدد_idxes)
  {
    إذا (!عدد_idxes || لومړی_var_idx == VarIdx::NO_VARIATION) أرجع;
    variation_indices->جمع_کړه_range (لومړی_var_idx, لومړی_var_idx + عدد_idxes - 1);
  }

  عام:
  ثابت فراغ *بنسټ;
  hb_سېټ_t لیدل_شوی_paint;
  hb_سېټ_t *glyphs;
  hb_سېټ_t *layer_indices;
  hb_سېټ_t *palette_indices;
  hb_سېټ_t *variation_indices;
  بینښه عدد_var_idxes;
  بینښه nesting_level_کیڼ;

  hb_colrv1_تړون_context_t (ثابت فراغ *بنسټ_2,
                               hb_سېټ_t *glyphs_,
                               hb_سېټ_t *layer_indices_,
                               hb_سېټ_t *palette_indices_,
                               hb_سېټ_t *variation_indices_,
                               بینښه عدد_var_idxes_2 = 1,
                               بینښه nesting_level_کیڼ_2 = HB_MAX_NESTING_LEVEL) :
                          بنسټ (بنسټ_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          عدد_var_idxes (عدد_var_idxes_2),
                          nesting_level_کیڼ (nesting_level_کیڼ_2)
  {}
};


بنية hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_کیڼ == 0))
      أرجع hb_تشه_t ();

    إذا (paint_لیدل_شوی (&obj))
      أرجع hb_تشه_t ();

    nesting_level_کیڼ--;
    obj.dependv1 (هذا);
    nesting_level_کیڼ++;
    أرجع hb_تشه_t ();
  }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  منطقي paint_لیدل_شوی (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    إذا (لیدل_شوی_paint.دننه_error() || لیدل_شوی_paint.has (delta))
      أرجع ریښتیا;

    لیدل_شوی_paint.جمع_کړه (delta);
    إذا (unlikely (لیدل_شوی_paint.دننه_error ()))
      depend_معلومات->fail ();
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع colr; }

  عام:
  ثابت COLR *colr;
  hb_depend_معلومات_builder_t *depend_معلومات;
  hb_سېټ_t لیدل_شوی_paint;
  بینښه nesting_level_کیڼ;
  hb_codepoint_t سرچینه_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_معلومات_builder_t *depend_معلومات_2,
                              بینښه nesting_level_کیڼ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_معلومات (depend_معلومات_2),
                          nesting_level_کیڼ (nesting_level_کیڼ_2),
                          سرچینه_gid (0)
  {}
};

بنية Layer_ریکارډ
{
  چلوونکی hb_ot_color_layer_t () ثابت { أرجع {glyphId, colorIdx}; }

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

بنية بنسټ_Glyph_ریکارډ
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
  HBUINT16	لومړی_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	عدد_Layers;	/* Number of color layers
				 * associated with this glyph */
  عام:
  DEFINE_اندازه_STATIC (6);
};

کينډۍ <ډول_نوم T>
بنية Variable
{
  ساكن constexpr منطقي is_variable = ریښتیا;

  ثابت T& get_ارزښت () ثابت { أرجع value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    c->عدد_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->جمع_کړه_var_idxes (var_Idx_بنسټ, c->عدد_var_idxes);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!value.subset (c, instancer, var_Idx_بنسټ)) return_trace (خطأ);
    إذا (c->plan->all_axes_pinned)
      return_trace (ریښتیا);

    VarIdx new_varidx;
    new_varidx = var_Idx_بنسټ;
    إذا (var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      hb_pair_t<بینښه, صحيح> *new_varidx_delta;
      إذا (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_بنسټ, &new_varidx_delta))
        return_trace (خطأ);

      new_varidx = hb_لومړی (*new_varidx_delta);
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
    value.paint_glyph (c, var_Idx_بنسټ);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, var_Idx_بنسټ, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, var_Idx_بنسټ, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  محمي:
  T      value;
  عام:
  VarIdx var_Idx_بنسټ;
  عام:
  DEFINE_اندازه_MIN (VarIdx::static_اندازه + T::min_اندازه);
};

کينډۍ <ډول_نوم T>
بنية NoVariable
{
  ساكن constexpr منطقي is_variable = خطأ;

  ثابت T& get_ارزښت () ثابت { أرجع value; }

  ساكن constexpr uint32_t var_Idx_بنسټ = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (value.subset (c, instancer, var_Idx_بنسټ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, var_Idx_بنسټ);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت توکی_Var_Store_Instancer &instancer) ثابت
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

کينډۍ <ډول_نوم Type, بینښه fraction_بټونه>
ساكن فراغ
سېټ_fixed_clamped (HBFixed<Type, fraction_بټونه> &بهر, عائم value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  بهر.سېټ_int (hb_clamp_منزل<ډول_نوم Type::type> (roundf ((value / HBFixed<Type, fraction_بټونه>::mult))));
}

بنية ColorStop
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    c->جمع_کړه_palette_شاخص (palette_شاخص_2);
    c->عدد_var_idxes = 2;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->stop_دننه_واټن, stop_دننه_واټن.منزل_float(instancer (var_Idx_بنسټ, 0)));
      عائم a = alpha.منزل_float (instancer (var_Idx_بنسټ, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      سېټ_fixed_clamped (بهر->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_ټاکل (بهر->palette_شاخص_2, c->plan->colr_palettes.get (palette_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *بهر,
		       uint32_t varIdx,
		       ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    بهر->دننه_واټن = stop_دننه_واټن.منزل_float(instancer (varIdx, 0));
    بهر->color = c->get_color (palette_شاخص_2,
                               alpha.منزل_float (instancer (varIdx, 1)),
                               &بهر->is_foreground);
  }

  F2DOT14	stop_دننه_واټن;
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

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
بنية Color_کرښه
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    لكل (ثابت خپلکاری &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    إذا (!c->serializer->check_ټاکل (بهر->extend, extend, HB_SERIALIZE_ERROR_INT_ډکېدنه)) return_trace (خطأ);
    إذا (!c->serializer->check_ټاکل (بهر->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_ډکېدنه)) return_trace (خطأ);

    لكل (ثابت خپلکاری& stop : stops.iter ())
    {
      إذا (!stop.subset (c, instancer)) return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  بینښه صحيح
  get_color_stops (hb_paint_context_t *c,
                   بینښه صحيح پیل,
		   بینښه صحيح *شمېر,
		   hb_color_stop_t *color_stops,
		   ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    بینښه صحيح len = stops.len;

    إذا (شمېر && color_stops)
    {
      بینښه صحيح i;
      لكل (i = 0; i < *شمېر && پیل + i < len; i++)
        stops[پیل + i].get_color_stop (c, &color_stops[i], instancer);
      *شمېر = i;
    }

    أرجع len;
  }

  HB_دننی ساكن بینښه صحيح static_get_color_stops (hb_color_کرښه_t *color_کرښه,
							  فراغ *color_کرښه_معلومات,
							  بینښه صحيح پیل,
							  بینښه صحيح *شمېر,
							  hb_color_stop_t *color_stops,
							  فراغ *user_معلومات)
  {
    ثابت Color_کرښه *thiz = (ثابت Color_کرښه *) color_کرښه_معلومات;
    hb_paint_context_t *c = (hb_paint_context_t *) user_معلومات;
    أرجع thiz->get_color_stops (c, پیل, شمېر, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بدل ((بینښه صحيح) extend)
    {
      حالة Extend::EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
      حالة Extend::EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
      حالة Extend::EXTEND_PAD:
      افتراضي:                     أرجع HB_PAINT_EXTEND_PAD;
    }
  }

  HB_دننی ساكن hb_paint_extend_t static_get_extend (hb_color_کرښه_t *color_کرښه,
							  فراغ *color_کرښه_معلومات,
							  فراغ *user_معلومات)
  {
    ثابت Color_کرښه *thiz = (ثابت Color_کرښه *) color_کرښه_معلومات;
    أرجع thiz->get_extend ();
  }

  Extend	extend;
  Array16_د<Var<ColorStop>>	stops;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
بنية مرکب_ډېرپېښ : HBUINT8
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

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  { c->عدد_var_idxes = 6; }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      خپلکاری fold = [&] (F16DOT16 &target, F16DOT16 سرچینه, بینښه دننه_واټن) {
        // Keep the stored integer precise while applying the float delta.
        target.سېټ_int (hb_clamp_منزل<int32_t> (roundf ((مزدوج) سرچینه.منزل_int () +
                                                    (مزدوج) instancer (var_Idx_بنسټ, دننه_واټن))));
      };
      fold (بهر->xx, xx, 0);
      fold (بهر->yx, yx, 1);
      fold (بهر->xy, xy, 2);
      fold (بهر->yy, yy, 3);
      fold (بهر->dx, dx, 4);
      fold (بهر->dy, dy, 5);
    }
    return_trace (ریښتیا);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    c->funcs->ټېل_transform (c->معلومات,
			      xx.منزل_float (c->instancer (var_Idx_بنسټ, 0)),
			      yx.منزل_float (c->instancer (var_Idx_بنسټ, 1)),
                              xy.منزل_float (c->instancer (var_Idx_بنسټ, 2)),
			      yy.منزل_float (c->instancer (var_Idx_بنسټ, 3)),
                              dx.منزل_float (c->instancer (var_Idx_بنسټ, 4)),
			      dy.منزل_float (c->instancer (var_Idx_بنسټ, 5)));
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
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer HB_ناکارول_شوی) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    uint32_t لومړی_layer_شاخص = عدد_Layers ? c->plan->colrv1_layers.get (لومړی_Layer_شاخص) : 0;
    return_trace (c->serializer->check_ټاکل (بهر->لومړی_Layer_شاخص, لومړی_layer_شاخص,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	عدد_Layers;
  HBUINT32	لومړی_Layer_شاخص;  /* index into COLRv1::layerList */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية PaintSolid
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    c->جمع_کړه_palette_شاخص (palette_شاخص_2);
    c->عدد_var_idxes = 1;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      عائم a = alpha.منزل_float (instancer (var_Idx_بنسټ, 0));
      سېټ_fixed_clamped (بهر->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    إذا (format == 3 && c->plan->all_axes_pinned)
        بهر->format = 2;

    return_trace (c->serializer->check_ټاکل (بهر->palette_شاخص_2, c->plan->colr_palettes.get (palette_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_بنسټ,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (palette_شاخص_2,
                           alpha.منزل_float (c->instancer (var_Idx_بنسټ, 0)),
                           is_foreground);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_بنسټ, &is_foreground, &color);
    c->funcs->color (c->معلومات, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_شاخص_2;
  F2DOT14	alpha;
  عام:
  DEFINE_اندازه_STATIC (3 + F2DOT14::static_اندازه);
};

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
بنية Paint_خطي_Gradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    (هذا+color_کرښه_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      بهر->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 0)));
      بهر->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
      بهر->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
      بهر->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 3)));
      بهر->x2 = hb_clamp_منزل<int16_t> ((مزدوج) x2 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 4)));
      بهر->y2 = hb_clamp_منزل<int16_t> ((مزدوج) y2 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 5)));
    }

    إذا (format == 5 && c->plan->all_axes_pinned)
        بهر->format = 4;

    return_trace (بهر->color_کرښه_2.serialize_subset (c, color_کرښه_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_کرښه_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_کرښه_t cl = {
      (فراغ *) &(هذا+color_کرښه_2),
      (هذا+color_کرښه_2).static_get_color_stops, c,
      (هذا+color_کرښه_2).static_get_extend, تش_اشاره
    };

    c->funcs->خطي_gradient (c->معلومات, &cl,
			       x0 + c->instancer (var_Idx_بنسټ, 0),
			       y0 + c->instancer (var_Idx_بنسټ, 1),
			       x1 + c->instancer (var_Idx_بنسټ, 2),
			       y1 + c->instancer (var_Idx_بنسټ, 3),
			       x2 + c->instancer (var_Idx_بنسټ, 4),
			       y2 + c->instancer (var_Idx_بنسټ, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_منزل<Color_کرښه<Var>>	color_کرښه_2; /* Offset (from beginning of PaintLinearGradient
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

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
بنية PaintRadialGradient
{
  منطقي constant_after_pinning (hb_subset_context_t *c,
                               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    إذا (c->plan->has_avar2) أرجع خطأ;
    ثابت خپلکاری &regions = instancer.varStore->get_region_list ();
    لكل (بینښه i = 0; i < regions.region_شمېر; i++)
    {
      hb_hashmap_t<hb_نښان_t, Triple> axes;
      إذا (!regions.get_var_region (i, c->plan->axes_old_شاخص_نښان_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_بهر_د_ځای);
        أرجع خطأ;
      }
      لكل (خپلکاری axis : axes)
      {
        Triple *حد;
        إذا (!c->plan->user_axes_location.has (axis.لومړی, &حد) ||
            حد->minimum != حد->اعظمي)
          أرجع خطأ;
      }
    }
    أرجع ریښتیا;
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    (هذا+color_کرښه_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقي subset_nonnegative (hb_subset_context_t *c,
                           ثابت توکی_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_بنسټ) ثابت
  {
    مزدوج سرچینه[6] = {
      x0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 0)),
      y0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)),
      radius0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)),
      x1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 3)),
      y1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 4)),
      radius1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 5))
    };
    خپلکاری transparent = [&] {
      خپلکاری *بهر = c->serializer->allocate_اندازه<PaintSolid> (5);
      إذا (!بهر) أرجع خطأ;
      بهر->format = 2; بهر->palette_شاخص_2 = 0xFFFF; بهر->alpha.سېټ_float (0);
      أرجع ریښتیا;
    };
    ثابت خپلکاری &کرښه = هذا+color_کرښه_2;
    إذا (سرچینه[2] == سرچینه[5] || !کرښه.stops.len) أرجع transparent ();
    بنية stop_t { مزدوج دننه_واټن, alpha; بینښه palette, ترتیب; };
    hb_ویکټور_t<stop_t> stops;
    إذا (!stops.alloc (کرښه.stops.len)) أرجع خطأ;
    بینښه ترتیب = 0;
    لكل (ثابت خپلکاری &ثبت : کرښه.stops.iter ())
    {
      ثابت خپلکاری &stop = ثبت.get_ارزښت ();
      stops.push (stop_t {(مزدوج) roundf (stop.stop_دننه_واټن.منزل_float (instancer (ثبت.var_Idx_بنسټ, 0)) * 16384.f) / 16384.,
                   (مزدوج) hb_clamp (stop.alpha.منزل_float (instancer (ثبت.var_Idx_بنسټ, 1)), 0.f, 1.f),
                   stop.palette_شاخص_2, ترتیب++});
    }
    إذا (stops.دننه_error ()) أرجع خطأ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      أرجع a.دننه_واټن < b.دننه_واټن ? -1 : a.دننه_واټن > b.دننه_واټن ? 1 :
             a.ترتیب < b.ترتیب ? -1 : a.ترتیب > b.ترتیب ? 1 : 0;
    });
    مزدوج dr = سرچینه[5] - سرچینه[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    بینښه lattice = 16384;
    لكل (بینښه j = 0; j < 3; j++)
    {
      مزدوج remainder = fmod (fabs (سرچینه[j + 3] - سرچینه[j]), 16384.);
      إذا (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_ډکېدنه);
        أرجع خطأ;
      }
      بینښه ښی = (بینښه) remainder;
      طالما (ښی) { بینښه r = lattice % ښی; lattice = ښی; ښی = r; }
    }
    لكل (بینښه ښويدنه = 0; ښويدنه < 16; ښويدنه++)
    {
      مزدوج b = 1u << ښويدنه;
      مزدوج coarse = dr > 0 ? ceil (-سرچینه[2] / dr) : floor (سرچینه[2] / -dr) - b;
      مزدوج fine = dr > 0 ? ceil (-سرچینه[2] / dr * lattice) / lattice :
                            floor (سرچینه[2] / -dr * lattice) / lattice - b;
      مزدوج candidates[] = {coarse, fine};
      لكل (مزدوج a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        مزدوج geometry[6];
        منطقي ځاییږي = ریښتیا;
        لكل (بینښه j = 0; j < 3; j++)
        {
          مزدوج difference = سرچینه[j + 3] - سرچینه[j];
          geometry[j] = سرچینه[j] + a * difference;
          geometry[j + 3] = سرچینه[j] + (a + b) * difference;
          مزدوج ټیټ = j == 2 ? 0 : -32768, لوړ = j == 2 ? 65535 : 32767;
          ځاییږي &= geometry[j] >= ټیټ && geometry[j] <= لوړ &&
                  geometry[j + 3] >= ټیټ && geometry[j + 3] <= لوړ;
        }
        إذا (!ځاییږي) استمر;
        بینښه لومړی = 0, شمېر = stops.اوږدتیا;
        مزدوج min = (stops[0].دننه_واټن - a) / b, max = (stops.لکۍ ().دننه_واټن - a) / b;
        مزدوج bias = 0;
        منطقي constant = خطأ;
        إذا (کرښه.extend == Extend::EXTEND_PAD)
        {
          مزدوج tip = -geometry[2] / (b * dr);
          إذا ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            لومړی = dr > 0 ? stops.اوږدتیا - 1 : 0;
            شمېر = 1; constant = ریښتیا;
          }
        }
        وإلا
        {
          مزدوج period = max - min;
          إذا (کرښه.extend == Extend::EXTEND_REFLECT) period *= 2;
          إذا (period <= 0) أرجع transparent ();
          مزدوج n = ceil ((-2 - min) / period);
          إذا (n > floor ((32767. / 16384. - max) / period)) استمر;
          bias = n * period;
        }
        لكل (بینښه j = 0; j < شمېر; j++)
        {
          مزدوج بټونه = constant ? 0 : ((stops[لومړی + j].دننه_واټن - a) / b + bias) * 16384;
          ځاییږي &= بټونه >= -32768 && بټونه <= 32767 && بټونه == round (بټونه);
        }
        إذا (!ځاییږي) استمر;
        خپلکاری *بهر = c->serializer->allocate_اندازه<PaintRadialGradient<NoVariable>> (16);
        إذا (!بهر) أرجع خطأ;
        بهر->format = 6;
        بهر->x0 = (صحيح) geometry[0]; بهر->y0 = (صحيح) geometry[1]; بهر->radius0 = (بینښه) geometry[2];
        بهر->x1 = (صحيح) geometry[3]; بهر->y1 = (صحيح) geometry[4]; بهر->radius1 = (بینښه) geometry[5];
        c->serializer->push ();
        خپلکاری *بهر_کرښه = c->serializer->allocate_اندازه<Color_کرښه<NoVariable>> (3 + شمېر * ColorStop::static_اندازه);
        إذا (!بهر_کرښه) { c->serializer->راوباسه_discard (); أرجع خطأ; }
        بهر_کرښه->extend = کرښه.extend;
        بهر_کرښه->stops.len = شمېر;
        لكل (بینښه j = 0; j < شمېر; j++)
        {
          ثابت خپلکاری &stop = stops[لومړی + j];
          خپلکاری &بهر_stop = بهر_کرښه->stops[j].value;
          بهر_stop.stop_دننه_واټن.سېټ_float ((عائم) (constant ? 0 : (stop.دننه_واټن - a) / b + bias));
          بهر_stop.alpha.سېټ_float ((عائم) stop.alpha);
          إذا (!c->serializer->check_ټاکل (بهر_stop.palette_شاخص_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_ډکېدنه))
          { c->serializer->راوباسه_discard (); أرجع خطأ; }
        }
        c->serializer->جمع_کړه_link (بهر->color_کرښه_2, c->serializer->راوباسه_pack ());
        أرجع !c->serializer->دننه_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_ډکېدنه);
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_بنسټ != VarIdx::NO_VARIATION &&
        (radius0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)) < 0 ||
         radius1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_بنسټ));
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      بهر->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 0)));
      بهر->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
      بهر->radius0 = hb_clamp_منزل<uint16_t> ((مزدوج) radius0 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
      بهر->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 3)));
      بهر->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 4)));
      بهر->radius1 = hb_clamp_منزل<uint16_t> ((مزدوج) radius1 + (مزدوج) roundf (instancer (var_Idx_بنسټ, 5)));
    }

    إذا (format == 7 && c->plan->all_axes_pinned)
        بهر->format = 6;

    return_trace (بهر->color_کرښه_2.serialize_subset (c, color_کرښه_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_کرښه_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_کرښه_t cl = {
      (فراغ *) &(هذا+color_کرښه_2),
      (هذا+color_کرښه_2).static_get_color_stops, c,
      (هذا+color_کرښه_2).static_get_extend, تش_اشاره
    };

    c->funcs->radial_gradient (c->معلومات, &cl,
			       x0 + c->instancer (var_Idx_بنسټ, 0),
			       y0 + c->instancer (var_Idx_بنسټ, 1),
			       radius0 + c->instancer (var_Idx_بنسټ, 2),
			       x1 + c->instancer (var_Idx_بنسټ, 3),
			       y1 + c->instancer (var_Idx_بنسټ, 4),
			       radius1 + c->instancer (var_Idx_بنسټ, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_منزل<Color_کرښه<Var>>	color_کرښه_2; /* Offset (from beginning of PaintRadialGradient
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

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
بنية PaintSweepGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    (هذا+color_کرښه_2).closurev1 (c);
    c->عدد_var_idxes = 4;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      بهر->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنسټ, 0)));
      بهر->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
      سېټ_fixed_clamped (بهر->پیل_Angle, پیل_Angle.منزل_float (instancer (var_Idx_بنسټ, 2)));
      سېټ_fixed_clamped (بهر->پای_ټکی_Angle, پای_ټکی_Angle.منزل_float (instancer (var_Idx_بنسټ, 3)));
    }

    إذا (format == 9 && c->plan->all_axes_pinned)
        بهر->format = 8;

    return_trace (بهر->color_کرښه_2.serialize_subset (c, color_کرښه_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_کرښه_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_کرښه_t cl = {
      (فراغ *) &(هذا+color_کرښه_2),
      (هذا+color_کرښه_2).static_get_color_stops, c,
      (هذا+color_کرښه_2).static_get_extend, تش_اشاره
    };

    c->funcs->sweep_gradient (c->معلومات, &cl,
			      centerX + c->instancer (var_Idx_بنسټ, 0),
			      centerY + c->instancer (var_Idx_بنسټ, 1),
                              (پیل_Angle.منزل_float (c->instancer (var_Idx_بنسټ, 2)) + 1) * HB_PI,
                              (پای_ټکی_Angle.منزل_float   (c->instancer (var_Idx_بنسټ, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_منزل<Color_کرښه<Var>>	color_کرښه_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		پیل_Angle;
  F2DOT14		پای_ټکی_Angle;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * FWORD::static_اندازه + 2 * F2DOT14::static_اندازه);
};

// Paint a non-COLR glyph, filled as indicated by paint.
کينډۍ <ډول_نوم Types>
بنية PaintGlyph
{
  کارول HBGlyphID = ډول_نوم Types::HBGlyphID;

  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (! c->serializer->check_ټاکل (بهر->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    return_trace (بهر->paint.serialize_subset (c, paint, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && paint.sanitize (c, هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24_منزل<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  عام:
  DEFINE_اندازه_STATIC (4 + HBGlyphID::static_اندازه);
};

بنية PaintColrGlyph
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer HB_ناکارول_شوی) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    return_trace (c->serializer->check_ټاکل (بهر->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
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

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
بنية PaintTransform
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    إذا (!بهر->transform.serialize_subset (c, transform, هذا, instancer)) return_trace (خطأ);
    إذا (format == 13 && c->plan->all_axes_pinned)
      بهر->format = 12;
    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
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
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_منزل<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_منزل<Var<Affine2x3>>	transform;
  عام:
  DEFINE_اندازه_STATIC (7);
};

بنية PaintTranslate
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      بهر->dx = hb_clamp_منزل<int16_t> ((مزدوج) dx + (مزدوج) roundf (instancer (var_Idx_بنسټ, 0)));
      بهر->dy = hb_clamp_منزل<int16_t> ((مزدوج) dy + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
    }

    إذا (format == 15 && c->plan->all_axes_pinned)
        بهر->format = 14;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم ddx = dx + c->instancer (var_Idx_بنسټ, 0);
    عائم ddy = dy + c->instancer (var_Idx_بنسټ, 1);

    c->funcs->ټېل_translate (c->معلومات, ddx, ddy);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * FWORD::static_اندازه);
};

کينډۍ <ډول_نوم سرچینه_2>
ساكن منطقي serialize_scale_transform (hb_subset_context_t *c,
                                       عائم sx, عائم sy, عائم dx, عائم dy,
                                       ثابت سرچینه_2 &سرچینه)
{
  خپلکاری *بهر = c->serializer->allocate_اندازه<PaintTransform<NoVariable>> (7);
  إذا (unlikely (!بهر)) أرجع خطأ;
  بهر->format = 12;

  c->serializer->push ();
  خپلکاری *transform = c->serializer->allocate_اندازه<Affine2x3> (Affine2x3::static_اندازه);
  إذا (unlikely (!transform))
  {
    c->serializer->راوباسه_discard ();
    أرجع خطأ;
  }
  سېټ_fixed_clamped (transform->xx, sx);
  transform->yx.سېټ_float (0);
  transform->xy.سېټ_float (0);
  سېټ_fixed_clamped (transform->yy, sy);
  سېټ_fixed_clamped (transform->dx, dx);
  سېټ_fixed_clamped (transform->dy, dy);
  c->serializer->جمع_کړه_link (بهر->transform, c->serializer->راوباسه_pack ());

  c->serializer->push ();
  منطقي ret = سرچینه ();
  إذا (ret)
    c->serializer->جمع_کړه_link (بهر->src, c->serializer->راوباسه_pack ());
  وإلا
    c->serializer->راوباسه_discard ();
  أرجع ret;
}

کينډۍ <ډول_نوم بنسټ_3>
ساكن منطقي subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت بنسټ_3 *بنسټ,
                                      ثابت Offset24_منزل<Paint> &src,
                                      ثابت توکی_Var_Store_Instancer &instancer,
                                      عائم sx, عائم sy,
                                      عائم cx = 0, عائم cy = 0)
{
  إذا (src.is_null ()) أرجع خطأ;
  خپلکاری سرچینه = [&] { أرجع c->dispatch (بنسټ+src, instancer); };
  عائم dx = cx * (1 - sx), dy = cy * (1 - sy);
  إذا (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    أرجع serialize_scale_transform (c, sx, sy, dx, dy, سرچینه);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  خپلکاری inner = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, سرچینه);
  };
  خپلکاری inverse = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  خپلکاری scale = [&] {
    أرجع serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  خپلکاری outer = [&] {
    أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

بنية PaintScale
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_بنسټ, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_بنسټ, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, sx, sy));
    }
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->scaleX, scaleX.منزل_float (instancer (var_Idx_بنسټ, 0)));
      سېټ_fixed_clamped (بهر->scaleY, scaleY.منزل_float (instancer (var_Idx_بنسټ, 1)));
    }

    إذا (format == 17 && c->plan->all_axes_pinned)
        بهر->format = 16;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_بنسټ, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_بنسټ, 1));

    c->funcs->ټېل_scale (c->معلومات, sx, sy);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه);
};

بنية PaintScaleAroundCenter
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_بنسټ, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_بنسټ, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_بنسټ, 2)),
            centerY + roundf (instancer (var_Idx_بنسټ, 3))));
    }
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->scaleX, scaleX.منزل_float (instancer (var_Idx_بنسټ, 0)));
      سېټ_fixed_clamped (بهر->scaleY, scaleY.منزل_float (instancer (var_Idx_بنسټ, 1)));
      بهر->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
      بهر->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنسټ, 3)));
    }

    إذا (format == 19 && c->plan->all_axes_pinned)
        بهر->format = 18;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_بنسټ, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_بنسټ, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنسټ, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنسټ, 3);

    c->funcs->ټېل_scale_around_center (c->معلومات, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية Paint_Scale_یوشان
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_بنسټ, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, s, s));
    }
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
      سېټ_fixed_clamped (بهر->scale, scale.منزل_float (instancer (var_Idx_بنسټ, 0)));

    إذا (format == 21 && c->plan->all_axes_pinned)
        بهر->format = 20;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_بنسټ, 0));

    c->funcs->ټېل_scale (c->معلومات, s, s);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه);
};

بنية Paint_Scale_یوشان_Around_Center
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_بنسټ, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_بنسټ, 1)),
            centerY + roundf (instancer (var_Idx_بنسټ, 2))));
    }
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->scale, scale.منزل_float (instancer (var_Idx_بنسټ, 0)));
      بهر->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
      بهر->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
    }

    إذا (format == 23 && c->plan->all_axes_pinned)
        بهر->format = 22;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_بنسټ, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنسټ, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنسټ, 2);

    c->funcs->ټېل_scale_around_center (c->معلومات, s, s, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية PaintRotate
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
      سېټ_fixed_clamped (بهر->angle, angle.منزل_float (instancer (var_Idx_بنسټ, 0)));

    إذا (format == 25 && c->plan->all_axes_pinned)
      بهر->format = 24;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_بنسټ, 0));

    c->funcs->ټېل_rotate (c->معلومات, a);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه);
};

بنية PaintRotateAroundCenter
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->angle, angle.منزل_float (instancer (var_Idx_بنسټ, 0)));
      بهر->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنسټ, 1)));
      بهر->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
    }

    إذا (format ==27 && c->plan->all_axes_pinned)
        بهر->format = 26;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_بنسټ, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنسټ, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنسټ, 2);

    c->funcs->ټېل_rotate_around_center (c->معلومات, a, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية PaintSkew
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_بنسټ, 0)));
      سېټ_fixed_clamped (بهر->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_بنسټ, 1)));
    }

    إذا (format == 29 && c->plan->all_axes_pinned)
        بهر->format = 28;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_بنسټ, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_بنسټ, 1));

    c->funcs->ټېل_skew (c->معلومات, sx, sy);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه);
};

بنية PaintSkewAroundCenter
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      سېټ_fixed_clamped (بهر->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_بنسټ, 0)));
      سېټ_fixed_clamped (بهر->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_بنسټ, 1)));
      بهر->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنسټ, 2)));
      بهر->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنسټ, 3)));
    }

    إذا (format == 31 && c->plan->all_axes_pinned)
        بهر->format = 30;

    return_trace (بهر->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_بنسټ, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_بنسټ, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنسټ, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنسټ, 3);

    c->funcs->ټېل_skew_around_center (c->معلومات, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->راوباسه_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_اندازه_STATIC (4 + 2 * F2DOT14::static_اندازه + 2 * FWORD::static_اندازه);
};

بنية Paint_مرکب
{
  HB_دننی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_دننی فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= بهر->src.serialize_subset (c, src, هذا, instancer);
    ret |= بهر->backdrop.serialize_subset (c, backdrop, هذا, instancer);
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
    hb_paint_مرکب_ډېرپېښ_t مرکب_ډېرپېښ_2;
    بدل ((بینښه صحيح) ډېرپېښ)
    {
      حالة HB_PAINT_مرکب_ډېرپېښ_CLEAR:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_OVER:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_OVER:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_دننه:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_دننه:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_بهر:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_بهر:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_ATOP:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_ATOP:
      حالة HB_PAINT_مرکب_ډېرپېښ_XOR:
      حالة HB_PAINT_مرکب_ډېرپېښ_PLUS:
      حالة HB_PAINT_مرکب_ډېرپېښ_SCREEN:
      حالة HB_PAINT_مرکب_ډېرپېښ_OVERLAY:
      حالة HB_PAINT_مرکب_ډېرپېښ_DARKEN:
      حالة HB_PAINT_مرکب_ډېرپېښ_LIGHTEN:
      حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_DODGE:
      حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_BURN:
      حالة HB_PAINT_مرکب_ډېرپېښ_HARD_LIGHT:
      حالة HB_PAINT_مرکب_ډېرپېښ_SOFT_LIGHT:
      حالة HB_PAINT_مرکب_ډېرپېښ_DIFFERENCE:
      حالة HB_PAINT_مرکب_ډېرپېښ_EXCLUSION:
      حالة HB_PAINT_مرکب_ډېرپېښ_MULTIPLY:
      حالة HB_PAINT_مرکب_ډېرپېښ_HSL_HUE:
      حالة HB_PAINT_مرکب_ډېرپېښ_HSL_SATURATION:
      حالة HB_PAINT_مرکب_ډېرپېښ_HSL_COLOR:
      حالة HB_PAINT_مرکب_ډېرپېښ_HSL_LUMINOSITY:
	مرکب_ډېرپېښ_2 = (hb_paint_مرکب_ډېرپېښ_t) (بینښه صحيح) ډېرپېښ;
	اكسر;
      افتراضي:
	مرکب_ډېرپېښ_2 = HB_PAINT_مرکب_ډېرپېښ_CLEAR;
	اكسر;
    }

    c->funcs->ټېل_ډله_for (c->معلومات, HB_PAINT_مرکب_ډېرپېښ_SRC_OVER);
    c->recurse (هذا+backdrop);
    c->funcs->ټېل_ډله_for (c->معلومات, مرکب_ډېرپېښ_2);
    c->recurse (هذا+src);
    c->funcs->راوباسه_ډله (c->معلومات, مرکب_ډېرپېښ_2);
    c->funcs->راوباسه_ډله (c->معلومات, HB_PAINT_مرکب_ډېرپېښ_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  مرکب_ډېرپېښ		ډېرپېښ;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_منزل<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Clip_Box_معلومات
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

  فراغ get_clip_box (Clip_Box_معلومات &clip_box, ثابت توکی_Var_Store_Instancer &instancer HB_ناکارول_شوی) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنسټ != VarIdx::NO_VARIATION)
    {
      بهر->xMin = hb_clamp_منزل<int16_t> ((مزدوج) xMin + (مزدوج) floorf (instancer (var_Idx_بنسټ, 0)));
      بهر->yMin = hb_clamp_منزل<int16_t> ((مزدوج) yMin + (مزدوج) floorf (instancer (var_Idx_بنسټ, 1)));
      بهر->xMax = hb_clamp_منزل<int16_t> ((مزدوج) xMax + (مزدوج) ceilf (instancer (var_Idx_بنسټ, 2)));
      بهر->yMax = hb_clamp_منزل<int16_t> ((مزدوج) yMax + (مزدوج) ceilf (instancer (var_Idx_بنسټ, 3)));
    }

    إذا (format == 2 && c->plan->all_axes_pinned)
        بهر->format = 1;

    return_trace (ریښتیا);
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
  فراغ get_clip_box (Clip_Box_معلومات &clip_box, ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    إذا (instancer)
    {
      clip_box.xMin = hb_saturate_جمع_کړه (clip_box.xMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_بنسټ, 0))));
      clip_box.yMin = hb_saturate_جمع_کړه (clip_box.yMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_بنسټ, 1))));
      clip_box.xMax = hb_saturate_جمع_کړه (clip_box.xMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_بنسټ, 2))));
      clip_box.yMax = hb_saturate_جمع_کړه (clip_box.yMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_بنسټ, 3))));
    }
  }

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  { c->variation_indices->جمع_کړه_range (var_Idx_بنسټ, var_Idx_بنسټ + 3); }
};

بنية ClipBox
{
  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  فراغ closurev1 (hb_colrv1_تړون_context_t* c) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.format2.closurev1 (c); أرجع;
    افتراضي:أرجع;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
                    ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    Clip_Box_معلومات clip_box;
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
    extents->پلنوالی = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->لوړوالی = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    أرجع ریښتیا;
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

بنية Clip_ریکارډ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پیل_Glyph_ID ? -1 : g <= پای_ټکی_Glyph_ID ? 0 : +1; }

  فراغ closurev1 (hb_colrv1_تړون_context_t* c, ثابت فراغ *بنسټ) ثابت
  {
    إذا (!c->glyphs->intersects (پیل_Glyph_ID, پای_ټکی_Glyph_ID)) أرجع;
    (بنسټ+clipBox).closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنسټ,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    return_trace (بهر->clipBox.serialize_subset (c, clipBox, بنسټ, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && clipBox.sanitize (c, بنسټ));
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
		    ثابت فراغ *بنسټ,
		    ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    أرجع (بنسټ+clipBox).get_extents (extents, instancer);
  }

  عام:
  HBUINT16		پیل_Glyph_ID;  // first gid clip applies to
  HBUINT16		پای_ټکی_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_منزل<ClipBox>	clipBox;   // Box or VarBox
  عام:
  DEFINE_اندازه_STATIC (7);
};
DECLARE_NULL_NAMESPACE_بایټونه (OT, Clip_ریکارډ);

بنية ClipList
{
  بینښه serialize_clip_ریکارډونه (hb_subset_context_t *c,
                                   ثابت توکی_Var_Store_Instancer &instancer,
                                   ثابت hb_سېټ_t& gids,
                                   ثابت hb_map_t& gid_دننه_واټن_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (gids.is_empty () ||
        gid_دننه_واټن_map.get_population () != gids.get_population ())
      return_trace (0);

    بینښه شمېر  = 0;

    hb_codepoint_t پیل_gid= gids.get_min ();
    hb_codepoint_t prev_gid = پیل_gid;

    بینښه دننه_واټن = gid_دننه_واټن_map.get (پیل_gid);
    بینښه prev_دننه_واټن = دننه_واټن;
    لكل (ثابت hb_codepoint_t _ : gids.iter ())
    {
      إذا (_ == پیل_gid) استمر;

      دننه_واټن = gid_دننه_واټن_map.get (_);
      إذا (_ == prev_gid + 1 &&  دننه_واټن == prev_دننه_واټن)
      {
        prev_gid = _;
        استمر;
      }

      Clip_ریکارډ ریکارډ;
      ریکارډ.پیل_Glyph_ID = پیل_gid;
      ریکارډ.پای_ټکی_Glyph_ID = prev_gid;
      ریکارډ.clipBox = prev_دننه_واټن;

      إذا (!ریکارډ.subset (c, هذا, instancer)) return_trace (0);
      شمېر++;

      پیل_gid = _;
      prev_gid = _;
      prev_دننه_واټن = دننه_واټن;
    }

    //last one
    {
      Clip_ریکارډ ریکارډ;
      ریکارډ.پیل_Glyph_ID = پیل_gid;
      ریکارډ.پای_ټکی_Glyph_ID = prev_gid;
      ریکارډ.clipBox = prev_دننه_واټن;
      إذا (!ریکارډ.subset (c, هذا, instancer)) return_trace (0);
      شمېر++;
    }
    return_trace (شمېر);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    إذا (!c->serializer->check_ټاکل (بهر->format, format, HB_SERIALIZE_ERROR_INT_ډکېدنه)) return_trace (خطأ);

    ثابت hb_سېټ_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_دننه_واټن_map;
    hb_سېټ_t new_gids;
    لكل (ثابت Clip_ریکارډ& ریکارډ : clips.iter ())
    {
      بینښه پیل_gid = ریکارډ.پیل_Glyph_ID;
      بینښه پای_ټکی_gid = ریکارډ.پای_ټکی_Glyph_ID;
      لكل (بینښه gid = پیل_gid; gid <= پای_ټکی_gid; gid++)
      {
        إذا (!glyphset.has (gid) || !glyph_map.has (gid)) استمر;
        بینښه new_gid = glyph_map.get (gid);
        new_gid_دننه_واټن_map.سېټ (new_gid, ریکارډ.clipBox);
        new_gids.جمع_کړه (new_gid);
      }
    }

    بینښه شمېر = serialize_clip_ریکارډونه (c, instancer, new_gids, new_gid_دننه_واټن_map);
    إذا (!شمېر) return_trace (خطأ);
    return_trace (c->serializer->check_ټاکل (بهر->clips.len, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
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
	       ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    خپلکاری *rec = clips.as_array ().bsearch (gid);
    إذا (rec)
      أرجع rec->get_extents (extents, هذا, instancer);
    أرجع خطأ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_د<Clip_ریکارډ>	clips;  // Clip records, sorted by startGlyphID
  عام:
  DEFINE_اندازه_ARRAY_SIZED (5, clips);
};

بنية Paint
{

  کينډۍ <ډول_نوم ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_پیل_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ارزښت ());

    return_trace (c->پای_ټکی_recursion (هذا->dispatch (c, std::forward<Ts> (ds)...)));
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); أرجع ریښتیا;
    حالة 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); أرجع ریښتیا;
    افتراضي: أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<Paint_خطي_Gradient<NoVariable>>	paintformat4;
  Variable<Paint_خطي_Gradient<Variable>>	paintformat5;
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
  NoVariable<Paint_Scale_یوشان>			paintformat20;
  Variable<Paint_Scale_یوشان>			paintformat21;
  NoVariable<Paint_Scale_یوشان_Around_Center>	paintformat22;
  Variable<Paint_Scale_یوشان_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_مرکب				paintformat32;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_MIN (2);
};

بنية بنسټ_Glyph_Paint_ریکارډ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت فراغ* src_بنسټ, hb_subset_context_t *c,
                  ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = s->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    إذا (!s->check_ټاکل (بهر->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    return_trace (بهر->paint.serialize_subset (c, paint, src_بنسټ, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && paint.sanitize (c, بنسټ)));
  }

  عام:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_منزل<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية بنسټ_Glyph_List : Sorted_Array32_د<بنسټ_Glyph_Paint_ریکارډ>
{
  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر)))  return_trace (خطأ);
    ثابت hb_سېټ_t* glyphset = &c->plan->_glyphset_colred;

    لكل (ثابت خپلکاری& _ : as_array ())
    {
      بینښه gid = _.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      إذا (_.serialize (c->serializer, c->plan->glyph_map, هذا, c, instancer)) بهر->len++;
      وإلا return_trace (خطأ);
    }

    return_trace (بهر->len != 0);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Sorted_Array32_د<بنسټ_Glyph_Paint_ریکارډ>::sanitize (c, هذا));
  }
};

بنية LayerList : Array32_د_Offset32_منزل<Paint>
{
  ثابت Paint& get_paint (بینښه i) ثابت
  { أرجع هذا+(*هذا)[i]; }

  منطقي subset (hb_subset_context_t *c,
               ثابت توکی_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خپلکاری& _ : + hb_enumerate (*هذا)
                         | hb_filter (c->plan->colrv1_layers, hb_لومړی))

    {
      خپلکاری *o = بهر->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      ret |= o->serialize_subset (c, _.second, هذا, instancer);
    }
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Array32_د_Offset32_منزل<Paint>::sanitize (c, هذا));
  }
};

بنية delta_سېټ_شاخص_map_subset_plan_t
{
  بینښه get_inner_بټ_شمېر () ثابت { أرجع inner_بټ_شمېر; }
  بینښه get_پلنوالی ()           ثابت { أرجع ((outer_بټ_شمېر + inner_بټ_شمېر + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  delta_سېټ_شاخص_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_شمېر = 0;
    outer_بټ_شمېر = 0;
    inner_بټ_شمېر = 1;
    output_map.init ();

    /* search backwards */
    بینښه شمېر = new_deltaset_idx_varidx_map.get_population ();
    إذا (!شمېر) أرجع;

    بینښه وروستی_idx = (بینښه)-1;
    بینښه وروستی_varidx = (بینښه)-1;

    لكل (بینښه i = شمېر; i; i--)
    {
      بینښه delta_سېټ_idx = i - 1;
      بینښه var_idx = new_deltaset_idx_varidx_map.get (delta_سېټ_idx);
      إذا (i == شمېر)
      {
        وروستی_idx = delta_سېټ_idx;
        وروستی_varidx = var_idx;
        استمر;
      }
      إذا (var_idx != وروستی_varidx)
        اكسر;
      وروستی_idx = delta_سېټ_idx;
    }

    map_شمېر = وروستی_idx + 1;
  }

  منطقي remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_بټ_شمېر = 1;
    inner_بټ_شمېر = 1;

    إذا (unlikely (!output_map.resize_dirty (map_شمېر))) أرجع خطأ;

    لكل (بینښه idx = 0; idx < map_شمېر; idx++)
    {
      uint32_t *var_idx;
      إذا (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) أرجع خطأ;
      output_map.arrayZ[idx] = *var_idx;

      بینښه outer = (*var_idx) >> 16;
      بینښه بټ_شمېر = (outer == 0) ? 1 : hb_بټ_storage (outer);
      outer_بټ_شمېر = hb_max (بټ_شمېر, outer_بټ_شمېر);

      بینښه inner = (*var_idx) & 0xFFFF;
      بټ_شمېر = (inner == 0) ? 1 : hb_بټ_storage (inner);
      inner_بټ_شمېر = hb_max (بټ_شمېر, inner_بټ_شمېر);
    }
    أرجع ریښتیا;
  }

  خاص:
  بینښه map_شمېر;
  بینښه outer_بټ_شمېر;
  بینښه inner_بټ_شمېر;
  hb_ویکټور_t<uint32_t> output_map;
};

بنية COLR
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_COLR;

  منطقي has_معلومات () ثابت { أرجع has_v0_معلومات () || نسخه; }

  منطقي has_v0_معلومات () ثابت { أرجع عدد_بنسټ_Glyphs; }
  منطقي has_v1_معلومات () ثابت
  {
    إذا (نسخه < 1)
      أرجع خطأ;
    hb_barrier ();

    أرجع (هذا+بنسټ_Glyph_List_2).len > 0;
  }

  بینښه صحيح get_glyph_layers (hb_codepoint_t       glyph,
				 بینښه صحيح         پیل_دننه_واټن,
				 بینښه صحيح        *شمېر, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت بنسټ_Glyph_ریکارډ &ریکارډ = (هذا+بنسټ_Glyphs_Z).bsearch (عدد_بنسټ_Glyphs, glyph);

    hb_array_t<ثابت Layer_ریکارډ> all_layers = (هذا+layersZ).as_array (عدد_Layers);
    hb_array_t<ثابت Layer_ریکارډ> glyph_layers = all_layers.sub_array (ریکارډ.لومړی_Layer_Idx,
								       ریکارډ.عدد_Layers);
    إذا (شمېر && layers)
    {
      + glyph_layers.sub_array (پیل_دننه_واټن, شمېر)
      | hb_sink (hb_array (layers, *شمېر))
      ;
    }
    أرجع glyph_layers.اوږدتیا;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_جدول<COLR> (face); }

    ~accelerator_t ()
    {
      خپلکاری *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقي has_معلومات () ثابت { أرجع colr->has_معلومات (); }

#که_نه_تعریف_شوی HB_NO_PAINT
    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      إذا (unlikely (!has_معلومات ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع ریښتیا;
      منطقي ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, فراغ *معلومات,
		      بینښه صحيح palette_شاخص,
		      hb_color_t foreground,
		      منطقي clip = ریښتیا) ثابت
    {
      إذا (unlikely (!has_معلومات ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع ریښتیا;
      منطقي ret = colr->paint_glyph (font, glyph, funcs, معلومات, palette_شاخص, foreground, clip, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }
#د_که_پای

    منطقي is_valid () { أرجع colr.get_blob ()->اوږدتیا; }

    فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
    {
      إذا (!has_معلومات ()) أرجع;
      colr->depend (depend_معلومات);
    }

    فراغ تړون_glyphs (hb_codepoint_t glyph,
			 hb_سېټ_t *related_ids /*بهر*/) ثابت
    { colr->تړون_glyphs (glyph, related_ids); }

    فراغ تړون_V0palette_indices (ثابت hb_سېټ_t *glyphs,
				    hb_سېټ_t *palettes /*بهر*/) ثابت
    { colr->تړون_V0palette_indices (glyphs, palettes); }

    فراغ تړون_for_V1 (hb_سېټ_t *glyphset,
                        hb_سېټ_t *layer_indices,
                        hb_سېټ_t *palette_indices,
                        hb_سېټ_t *variation_indices,
                        hb_سېټ_t *delta_سېټ_indices) ثابت
    { colr->تړون_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_سېټ_indices); }

    منطقي has_var_store () ثابت
    { أرجع colr->has_var_store (); }

    ثابت توکی_Variation_Store &get_var_store () ثابت
    { أرجع colr->get_var_store (); }
    ثابت توکی_Variation_Store *get_var_store_ptr () ثابت
    { أرجع colr->get_var_store_ptr (); }

    منطقي has_delta_سېټ_شاخص_map () ثابت
    { أرجع colr->has_delta_سېټ_شاخص_map (); }

    ثابت Delta_سېټ_شاخص_Map &get_delta_سېټ_شاخص_map () ثابت
    { أرجع colr->get_delta_سېټ_شاخص_map (); }
    ثابت Delta_سېټ_شاخص_Map *get_delta_سېټ_شاخص_map_ptr () ثابت
    { أرجع colr->get_delta_سېټ_شاخص_map_ptr (); }

    خاص:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, تش_اشاره)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع تش_اشاره;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (تش_اشاره, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عام:
    hb_blob_ptr_t<COLR> colr;
    خاص:
    بدلیدونکی hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    // v0
    hb_array_t<ثابت بنسټ_Glyph_ریکارډ> بنسټ_Glyphs = (هذا+بنسټ_Glyphs_Z).as_array (عدد_بنسټ_Glyphs);
    لكل (ثابت بنسټ_Glyph_ریکارډ &ریکارډ : بنسټ_Glyphs)
    {
      خپلکاری glyph_layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (ریکارډ.لومړی_Layer_Idx,
                                                                         ریکارډ.عدد_Layers);
      لكل (ثابت Layer_ریکارډ layer : glyph_layers)
        depend_معلومات->جمع_کړه_depend(ریکارډ.glyphId, HB_OT_نښان_COLR, layer.glyphId);
    }

    إذا (!has_v1_معلومات())
      أرجع;

    // v1
    hb_colrv1_depend_context_t c (هذا, depend_معلومات);
    ثابت بنسټ_Glyph_List &baseglyph_paintrecords = هذا+بنسټ_Glyph_List_2;

    لكل (ثابت بنسټ_Glyph_Paint_ریکارډ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.سرچینه_gid = baseglyph_paintrecord.glyphId;
      c.لیدل_شوی_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  فراغ تړون_glyphs (hb_codepoint_t glyph,
		       hb_سېټ_t *related_ids /*بهر*/) ثابت
  {
    ثابت بنسټ_Glyph_ریکارډ *ریکارډ = get_بنسټ_glyph_ریکارډ (glyph);
    إذا (!ریکارډ) أرجع;

    خپلکاری glyph_layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (ریکارډ->لومړی_Layer_Idx,
								       ریکارډ->عدد_Layers);
    إذا (!glyph_layers.اوږدتیا) أرجع;
    related_ids->جمع_کړه_array (&glyph_layers[0].glyphId, glyph_layers.اوږدتیا, Layer_ریکارډ::min_اندازه);
  }

  فراغ تړون_V0palette_indices (ثابت hb_سېټ_t *glyphs,
				  hb_سېټ_t *palettes /*بهر*/) ثابت
  {
    إذا (!عدد_بنسټ_Glyphs || !عدد_Layers) أرجع;
    hb_array_t<ثابت بنسټ_Glyph_ریکارډ> بنسټ_Glyphs = (هذا+بنسټ_Glyphs_Z).as_array (عدد_بنسټ_Glyphs);
    hb_array_t<ثابت Layer_ریکارډ> all_layers = (هذا+layersZ).as_array (عدد_Layers);

    لكل (ثابت بنسټ_Glyph_ریکارډ ریکارډ : بنسټ_Glyphs)
    {
      إذا (!glyphs->has (ریکارډ.glyphId)) استمر;
      hb_array_t<ثابت Layer_ریکارډ> glyph_layers = all_layers.sub_array (ریکارډ.لومړی_Layer_Idx,
                                                                   ریکارډ.عدد_Layers);
      لكل (ثابت Layer_ریکارډ layer : glyph_layers)
        palettes->جمع_کړه (layer.colorIdx);
    }
  }

  فراغ تړون_for_V1 (hb_سېټ_t *glyphset,
                      hb_سېټ_t *layer_indices,
                      hb_سېټ_t *palette_indices,
                      hb_سېټ_t *variation_indices,
                      hb_سېټ_t *delta_سېټ_indices) ثابت
  {
    إذا (نسخه < 1) أرجع;
    hb_barrier ();

    hb_سېټ_t لیدل_شوی_glyphs;

    hb_colrv1_تړون_context_t c (هذا, &لیدل_شوی_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت بنسټ_Glyph_List &baseglyph_paintrecords = هذا+بنسټ_Glyph_List_2;

    لكل (ثابت بنسټ_Glyph_Paint_ریکارډ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      بینښه gid = baseglyph_paintrecord.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_سېټ_union (glyphset, &لیدل_شوی_glyphs);

    ثابت ClipList &cliplist = هذا+clipList;
    c.glyphs = glyphset;
    لكل (ثابت Clip_ریکارډ &clip_ریکارډ : cliplist.clips.iter())
      clip_ریکارډ.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    إذا (has_delta_سېټ_شاخص_map ())
    {
      ثابت Delta_سېټ_شاخص_Map &var_idx_map = هذا+varIdxMap;
      delta_سېټ_indices->سېټ (*variation_indices);
      variation_indices->clear ();
      لكل (بینښه delta_سېټ_idx : *delta_سېټ_indices)
        variation_indices->جمع_کړه (var_idx_map.map (delta_سېټ_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { أرجع (هذا+layerList); }

  ثابت بنسټ_Glyph_List& get_baseglyphList () ثابت
  { أرجع (هذا+بنسټ_Glyph_List_2); }

  منطقي has_var_store () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && varStore != 0; }

  منطقي has_delta_سېټ_شاخص_map () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقي has_clip_list () ثابت
  { أرجع نسخه >= 1 && hb_barrier () && clipList != 0; }

  ثابت Delta_سېټ_شاخص_Map &get_delta_سېټ_شاخص_map () ثابت
  { أرجع has_delta_سېټ_شاخص_map () && hb_barrier () ? هذا+varIdxMap : Null (Delta_سېټ_شاخص_Map); }
  ثابت Delta_سېټ_شاخص_Map *get_delta_سېټ_شاخص_map_ptr () ثابت
  { أرجع has_delta_سېټ_شاخص_map () && hb_barrier () ? &(هذا+varIdxMap) : تش_اشاره; }

  ثابت توکی_Variation_Store &get_var_store () ثابت
  { أرجع has_var_store () && hb_barrier () ? هذا+varStore : Null (توکی_Variation_Store); }
  ثابت توکی_Variation_Store *get_var_store_ptr () ثابت
  { أرجع has_var_store () && hb_barrier () ? &(هذا+varStore) : تش_اشاره; }

  ثابت ClipList &get_clip_list () ثابت
  { أرجع has_clip_list () && hb_barrier () ? هذا+clipList : Null (ClipList); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  (هذا+بنسټ_Glyphs_Z).sanitize (c, عدد_بنسټ_Glyphs) &&
                  (هذا+layersZ).sanitize (c, عدد_Layers) &&
                  (نسخه == 0 ||
		   (hb_barrier () &&
		    بنسټ_Glyph_List_2.sanitize (c, هذا) &&
		    layerList.sanitize (c, هذا) &&
		    clipList.sanitize (c, هذا) &&
		    varIdxMap.sanitize (c, هذا) &&
		    varStore.sanitize (c, هذا))));
  }

  کينډۍ<ډول_نوم بنسټ_Iterator, ډول_نوم LayerIterator,
	   hb_requires (hb_is_iterator (بنسټ_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقي serialize_V0 (hb_serialize_context_t *c,
		     بینښه نسخه,
		     بنسټ_Iterator بنسټ_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (بنسټ_it.len () != layer_it.len ()))
      return_trace (خطأ);

    هذا->نسخه = نسخه;
    عدد_Layers = 0;
    عدد_بنسټ_Glyphs = بنسټ_it.len ();
    إذا (عدد_بنسټ_Glyphs == 0)
    {
      بنسټ_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (ریښتیا);
    }

    c->push ();
    لكل (ثابت hb_توکی_type<بنسټ_Iterator> _ : + بنسټ_it.iter ())
    {
      خپلکاری* ریکارډ = c->embed (_);
      إذا (unlikely (!ریکارډ)) return_trace (خطأ);
      ریکارډ->لومړی_Layer_Idx = عدد_Layers;
      عدد_Layers += ریکارډ->عدد_Layers;
    }
    c->جمع_کړه_link (بنسټ_Glyphs_Z, c->راوباسه_pack ());

    c->push ();
    لكل (ثابت hb_توکی_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->جمع_کړه_link (layersZ, c->راوباسه_pack ());

    return_trace (ریښتیا);
  }

  ثابت بنسټ_Glyph_ریکارډ* get_بنسټ_glyph_ریکارډ (hb_codepoint_t gid) ثابت
  {
    ثابت بنسټ_Glyph_ریکارډ* ریکارډ = &(هذا+بنسټ_Glyphs_Z).bsearch (عدد_بنسټ_Glyphs, (بینښه صحيح) gid);
    إذا (ریکارډ == &Null (بنسټ_Glyph_ریکارډ) ||
        (ریکارډ && (hb_codepoint_t) ریکارډ->glyphId != gid))
      ریکارډ = تش_اشاره;
    أرجع ریکارډ;
  }

  ثابت بنسټ_Glyph_Paint_ریکارډ* get_بنسټ_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت بنسټ_Glyph_List &list = هذا+بنسټ_Glyph_List_2;
    بینښه صحيح i;
    إذا (!list.bfind ((بینښه) gid, &i))
      أرجع تش_اشاره;
    أرجع &list[i];
  }

  منطقي downgrade_منزل_V0 (ثابت hb_سېټ_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    لكل (ثابت بنسټ_Glyph_Paint_ریکارډ& _ : get_baseglyphList ())
      إذا (glyphset.has (_.glyphId))
        أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        COLR* بهر /*بهر*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (ریښتیا);

    ثابت توکی_Variation_Store& var_store = هذا+varStore;
    إذا (c->plan->normalized_coords)
    {
      توکی_variations_t توکی_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقي optimize = (varIdxMap != 0) ? ریښتیا : خطأ;
      إذا (!توکی_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (خطأ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      إذا (توکی_vars.get_region_list () &&
          توکی_vars.get_vardata_encodings () &&
          !بهر->varStore.serialize_serialize (c->serializer,
                                              توکی_vars.has_long_کلمه (),
                                              c->plan->axis_نښانونه,
                                              توکی_vars.get_region_list (),
                                              توکی_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      إذا (optimize)
      {
        ثابت hb_map_t &varidx_map = توکی_vars.get_varidx_map ();
        لكل (خپلکاری _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
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
      hb_سېټ_t dead_regions;
      إذا (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_شاخص_نښان_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      إذا (unlikely (!بهر->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : تش_اشاره)))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  منطقي subset_delta_سېټ_شاخص_map (hb_subset_context_t *c,
                                   COLR* بهر /*بهر*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (ریښتیا);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_سېټ_شاخص_map_subset_plan_t شاخص_map_plan (deltaset_idx_varidx_map);

    إذا (unlikely (!شاخص_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (خطأ);

    return_trace (بهر->varIdxMap.serialize_serialize (c->serializer, شاخص_map_plan));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &سرچپه_glyph_map = *c->plan->سرچپه_glyph_map;
    ثابت hb_سېټ_t& glyphset = c->plan->_glyphset_colred;

    خپلکاری بنسټ_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = سرچپه_glyph_map.get (new_gid);
		    إذا (glyphset.has (old_gid)) أرجع ریښتیا;
		    أرجع خطأ;
		 })
    | hb_map_retains_ترتیبونه ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = سرچپه_glyph_map.get (new_gid);

				ثابت بنسټ_Glyph_ریکارډ* old_ریکارډ = get_بنسټ_glyph_ریکارډ (old_gid);
				إذا (unlikely (!old_ریکارډ))
				  أرجع hb_pair_t<منطقي, بنسټ_Glyph_ریکارډ> (خطأ, Null (بنسټ_Glyph_ریکارډ));
				بنسټ_Glyph_ریکارډ new_ریکارډ = {};
				new_ریکارډ.glyphId = new_gid;
				new_ریکارډ.عدد_Layers = old_ریکارډ->عدد_Layers;
				أرجع hb_pair_t<منطقي, بنسټ_Glyph_ریکارډ> (ریښتیا, new_ریکارډ);
			      })
    | hb_filter (hb_لومړی)
    | hb_map_retains_ترتیبونه (hb_دوهم)
    ;

    خپلکاری layer_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_map (سرچپه_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_ترتیبونه ([&](hb_codepoint_t old_gid)
			      {
				ثابت بنسټ_Glyph_ریکارډ* old_ریکارډ = get_بنسټ_glyph_ریکارډ (old_gid);
				hb_ویکټور_t<Layer_ریکارډ> بهر_layers;

				إذا (unlikely (!old_ریکارډ ||
					      old_ریکارډ->لومړی_Layer_Idx >= عدد_Layers ||
					      old_ریکارډ->لومړی_Layer_Idx + old_ریکارډ->عدد_Layers > عدد_Layers))
				  أرجع hb_pair_t<منطقي, hb_ویکټور_t<Layer_ریکارډ>> (خطأ, بهر_layers);

				خپلکاری layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (old_ریکارډ->لومړی_Layer_Idx,
											     old_ریکارډ->عدد_Layers);
				بهر_layers.resize (layers.اوږدتیا);
				لكل (بینښه صحيح i = 0; i < layers.اوږدتیا; i++) {
				  بهر_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  إذا (unlikely (!c->plan->new_gid_for_old_gid (بهر_layers[i].glyphId, &new_gid)))
				    أرجع hb_pair_t<منطقي, hb_ویکټور_t<Layer_ریکارډ>> (خطأ, بهر_layers);
				  بهر_layers[i].glyphId = new_gid;
				  بهر_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				أرجع hb_pair_t<منطقي, hb_ویکټور_t<Layer_ریکارډ>> (ریښتیا, بهر_layers);
			      })
    | hb_filter (hb_لومړی)
    | hb_map_retains_ترتیبونه (hb_دوهم)
    ;

    إذا (نسخه == 0 && (!بنسټ_it || !layer_it))
      return_trace (خطأ);

    خپلکاری *colr_اولي = c->serializer->پیل_embed<COLR> ();
    إذا (unlikely (!c->serializer->extend_min (colr_اولي)))  return_trace (خطأ);

    إذا (نسخه == 0 || downgrade_منزل_V0 (glyphset))
      return_trace (colr_اولي->serialize_V0 (c->serializer, 0, بنسټ_it, layer_it));

    hb_barrier ();

    //start version 1
    إذا (!c->serializer->allocate_اندازه<فراغ> (5 * HBUINT32::static_اندازه)) return_trace (خطأ);
    إذا (!colr_اولي->serialize_V0 (c->serializer, نسخه, بنسټ_it, layer_it)) return_trace (خطأ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    إذا (!subset_varstore (c, colr_اولي)) return_trace (خطأ);

    توکی_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : تش_اشاره,
				     c->plan->normalized_coords ? get_delta_سېټ_شاخص_map_ptr () : تش_اشاره,
				     c->plan->normalized_coords.as_array ());

    إذا (!colr_اولي->بنسټ_Glyph_List_2.serialize_subset (c, بنسټ_Glyph_List_2, هذا, instancer))
      return_trace (خطأ);

    colr_اولي->layerList.serialize_subset (c, layerList, هذا, instancer);
    colr_اولي->clipList.serialize_subset (c, clipList, هذا, instancer);

    return_trace (subset_delta_سېټ_شاخص_map (c, colr_اولي));
  }

  ثابت Paint *get_بنسټ_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت بنسټ_Glyph_List &baseglyph_paintrecords = هذا+بنسټ_Glyph_List_2;
    ثابت بنسټ_Glyph_Paint_ریکارډ* ریکارډ = get_بنسټ_glyph_paintrecord (glyph);
    إذا (ریکارډ)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+ریکارډ->paint;
      أرجع &paint;
    }
    وإلا
      أرجع تش_اشاره;
  }

#که_نه_تعریف_شوی HB_NO_PAINT
  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    توکی_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_سېټ_شاخص_map_ptr (),
                                     hb_array (font->coords, font->عدد_coords));

    إذا (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      أرجع ریښتیا;
    }

    خپلکاری *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقي ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), ریښتیا, scratch);

    خپلکاری e = scratch.paint_extents.get_extents ();
    إذا (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->پلنوالی = 0;
      extents->لوړوالی = 0;
    }
    وإلا
      *extents = e.منزل_glyph_extents ();

    أرجع ret;
  }
#د_که_پای

  منطقي
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    إذا (نسخه >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنسټ_glyph_paint (glyph);

      أرجع paint != تش_اشاره;
    }

    أرجع خطأ;
  }

  منطقي get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت توکی_Var_Store_Instancer instancer) ثابت
  {
    أرجع get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#که_نه_تعریف_شوی HB_NO_PAINT
  منطقي
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, فراغ *معلومات,
	       بینښه صحيح palette_شاخص, hb_color_t foreground,
	       منطقي clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    توکی_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_سېټ_شاخص_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->عدد_coords : 0));
    hb_paint_context_t c (هذا, funcs, معلومات, font, palette_شاخص, foreground, instancer);

    hb_decycler_غوټه_t node (c.glyphs_decycler);
    node.لیدنه (glyph);

    إذا (نسخه >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنسټ_glyph_paint (glyph);
      إذا (paint)
      {
        // COLRv1 glyph

	منطقي is_bounded = ریښتیا;
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
	    خپلکاری *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_شاخص, foreground,
			 خطأ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ټېل_font_transform (c.معلومات, font);

	إذا (clip)
	  c.funcs->ټېل_clip_rectangle (c.معلومات,
					(عائم) extents.x_bearing,
					(عائم) extents.y_bearing + extents.لوړوالی,
					(عائم) extents.x_bearing + extents.پلنوالی,
					(عائم) extents.y_bearing);

	إذا (is_bounded)
	  c.recurse (*paint);

	إذا (clip)
	  c.funcs->راوباسه_clip (c.معلومات);

	c.funcs->راوباسه_transform (c.معلومات);

        أرجع ریښتیا;
      }
    }

    ثابت بنسټ_Glyph_ریکارډ *ریکارډ = get_بنسټ_glyph_ریکارډ (glyph);
    إذا (ریکارډ && ((hb_codepoint_t) ریکارډ->glyphId == glyph))
    {
      // COLRv0 glyph
      لكل (ثابت خپلکاری &r : (هذا+layersZ).as_array (عدد_Layers)
			   .sub_array (ریکارډ->لومړی_Layer_Idx, ریکارډ->عدد_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.معلومات, r.glyphId, c.font, is_foreground, color);
      }

      أرجع ریښتیا;
    }

    أرجع خطأ;
  }
#د_که_پای

  محمي:
  HBUINT16	نسخه;	/* Table version number (starts at 0). */
  HBUINT16	عدد_بنسټ_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_منزل<Sorted_Unsized_Array_د<بنسټ_Glyph_ریکارډ>>
		بنسټ_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_منزل<Unsized_Array_د<Layer_ریکارډ>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	عدد_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_منزل<بنسټ_Glyph_List>		بنسټ_Glyph_List_2;
  Offset32_منزل<LayerList>			layerList;
  Offset32_منزل<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_منزل<Delta_سېټ_شاخص_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_منزل<توکی_Variation_Store>	varStore;
  عام:
  DEFINE_اندازه_MIN (14);
};

بنية COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

فراغ
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  إذا (unlikely (ژوروالی_کیڼ <= 0 || څنډه_شمېر <= 0)) أرجع;
  ژوروالی_کیڼ--;
  څنډه_شمېر--;
  paint.dispatch (هذا);
  ژوروالی_کیڼ++;
}

کينډۍ <ډول_نوم Types>
فراغ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  إذا (likely (c->ژوروالی_کیڼ > 0 && c->څنډه_شمېر > 0) &&
      (هذا+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->څنډه_شمېر--;
    c->funcs->ټېل_inverse_font_transform (c->معلومات, c->font);
    c->funcs->fill_glyph (c->معلومات, gid, c->font, is_foreground, color);
    c->funcs->راوباسه_transform (c->معلومات);
    أرجع;
  }

  c->funcs->ټېل_inverse_font_transform (c->معلومات, c->font);
  c->funcs->ټېل_clip_glyph (c->معلومات, gid, c->font);
  c->funcs->ټېل_font_transform (c->معلومات, c->font);
  c->recurse (هذا+paint);
  c->funcs->راوباسه_transform (c->معلومات);
  c->funcs->راوباسه_clip (c->معلومات);
  c->funcs->راوباسه_transform (c->معلومات);
}

فراغ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);
  ثابت LayerList &paint_دننه_واټن_lists = c->get_colr_جدول ()->get_layerList ();
  hb_decycler_غوټه_t node (c->layers_decycler);
  لكل (بینښه i = لومړی_Layer_شاخص; i < لومړی_Layer_شاخص + عدد_Layers; i++)
  {
    إذا (unlikely (!node.لیدنه (i)))
      أرجع;

    ثابت Paint &paint = paint_دننه_واټن_lists.get_paint (i);
    c->recurse (paint);
  }
}

فراغ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_decycler_غوټه_t node (c->glyphs_decycler);
  إذا (unlikely (!node.لیدنه (gid)))
    أرجع;

  c->funcs->ټېل_inverse_font_transform (c->معلومات, c->font);
  إذا (c->funcs->color_glyph (c->معلومات, gid, c->font))
  {
    c->funcs->راوباسه_transform (c->معلومات);
    أرجع;
  }
  c->funcs->راوباسه_transform (c->معلومات);

  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت Paint *paint = colr_جدول->get_بنسټ_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقي has_clip_box = colr_جدول->get_clip (gid, &extents, c->instancer);

  إذا (has_clip_box)
    c->funcs->ټېل_clip_rectangle (c->معلومات,
				   (عائم) extents.x_bearing,
				   (عائم) extents.y_bearing + extents.لوړوالی,
				   (عائم) extents.x_bearing + extents.پلنوالی,
				   (عائم) extents.y_bearing);

  إذا (paint)
    c->recurse (*paint);

  إذا (has_clip_box)
    c->funcs->راوباسه_clip (c->معلومات);
}

} /* namespace OT */

#د_که_پای /* OT_COLOR_COLR_COLR_HH */
