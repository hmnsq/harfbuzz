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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_COLR_COLR_HH
#وصف_ڪريو OT_COLOR_COLR_COLR_HH

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
#وصف_ڪريو HB_OT_ٽيگ_COLR HB_ٽيگ('C','O','L','R')

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
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (بنياد); }

عام:
  ثابت فراغ *بنياد;
  hb_paint_funcs_t *funcs;
  فراغ *معلومات;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  شي_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح اونهائي_کاٻو = HB_MAX_NESTING_LEVEL;
  صحيح ڪنارو_ڳڻپ = HB_MAX_گراف_ڪنارو_ڳڻپ;

  hb_paint_context_t (ثابت فراغ *بنياد_2,
		      hb_paint_funcs_t *funcs_,
		      فراغ *معلومات_2,
                      hb_font_t *font_,
                      بنانشان صحيح palette_,
                      hb_color_t foreground_,
		      شي_Var_Store_Instancer &instancer_) :
    بنياد (بنياد_2),
    funcs (funcs_),
    معلومات (معلومات_2),
    font (font_),
    palette (
#جيڪڏهن_وصف_نه_ٿيل HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->جدول.CPAL->get_palette_colors (palette_ < font->face->جدول.CPAL->get_palette_ڳڻپ () ? palette_ : 0)
#جيڪڏهن_پورو
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_سيٽ_synthetic_bold (font, 0, 0, سچ);
      hb_font_سيٽ_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساكن بنانشان
  color_alpha (hb_color_t color, عائم alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    أرجع roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (بنانشان صحيح color_اشاريو, عائم alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = سچ;

    إذا (color_اشاريو != 0xffff)
    {
      إذا (!funcs->custom_palette_color (معلومات, color_اشاريو, &color))
        color = palette[color_اشاريو];

      *is_foreground = خطأ;
    }

    أرجع HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  مضمن فراغ recurse (ثابت Paint &paint);
};

بنية hb_colrv1_گهيرو_context_t :
       hb_dispatch_context_t<hb_colrv1_گهيرو_context_t>
{
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_کاٻو == 0))
      أرجع hb_خالي_جايون_t ();

    إذا (paint_گهميل (&obj))
      أرجع hb_خالي_جايون_t ();

    nesting_level_کاٻو--;
    obj.closurev1 (هذا);
    nesting_level_کاٻو++;
    أرجع hb_خالي_جايون_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  منطقي paint_گهميل (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) بنياد);
    إذا (گهميل_paint.اندريون_error() || گهميل_paint.has (delta))
      أرجع سچ;

    گهميل_paint.جوڙيو (delta);
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (بنياد); }

  فراغ جوڙيو_glyph (بنانشان glyph_id)
  { glyphs->جوڙيو (glyph_id); }

  فراغ جوڙيو_layer_indices (بنانشان پهريون_layer_اشاريو, بنانشان انگ_جو_layers)
  {
    إذا (انگ_جو_layers == 0) أرجع;
    layer_indices->جوڙيو_range (پهريون_layer_اشاريو, پهريون_layer_اشاريو + انگ_جو_layers - 1);
  }

  فراغ جوڙيو_palette_اشاريو (بنانشان palette_اشاريو)
  { palette_indices->جوڙيو (palette_اشاريو); }

  فراغ جوڙيو_var_idxes (بنانشان پهريون_var_idx, بنانشان انگ_idxes)
  {
    إذا (!انگ_idxes || پهريون_var_idx == VarIdx::NO_VARIATION) أرجع;
    variation_indices->جوڙيو_range (پهريون_var_idx, پهريون_var_idx + انگ_idxes - 1);
  }

  عام:
  ثابت فراغ *بنياد;
  hb_سيٽ_t گهميل_paint;
  hb_سيٽ_t *glyphs;
  hb_سيٽ_t *layer_indices;
  hb_سيٽ_t *palette_indices;
  hb_سيٽ_t *variation_indices;
  بنانشان انگ_var_idxes;
  بنانشان nesting_level_کاٻو;

  hb_colrv1_گهيرو_context_t (ثابت فراغ *بنياد_2,
                               hb_سيٽ_t *glyphs_,
                               hb_سيٽ_t *layer_indices_,
                               hb_سيٽ_t *palette_indices_,
                               hb_سيٽ_t *variation_indices_,
                               بنانشان انگ_var_idxes_2 = 1,
                               بنانشان nesting_level_کاٻو_2 = HB_MAX_NESTING_LEVEL) :
                          بنياد (بنياد_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          انگ_var_idxes (انگ_var_idxes_2),
                          nesting_level_کاٻو (nesting_level_کاٻو_2)
  {}
};


بنية hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_کاٻو == 0))
      أرجع hb_خالي_جايون_t ();

    إذا (paint_گهميل (&obj))
      أرجع hb_خالي_جايون_t ();

    nesting_level_کاٻو--;
    obj.dependv1 (هذا);
    nesting_level_کاٻو++;
    أرجع hb_خالي_جايون_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  منطقي paint_گهميل (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    إذا (گهميل_paint.اندريون_error() || گهميل_paint.has (delta))
      أرجع سچ;

    گهميل_paint.جوڙيو (delta);
    إذا (unlikely (گهميل_paint.اندريون_error ()))
      depend_معلومات->fail ();
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع colr; }

  عام:
  ثابت COLR *colr;
  hb_depend_معلومات_builder_t *depend_معلومات;
  hb_سيٽ_t گهميل_paint;
  بنانشان nesting_level_کاٻو;
  hb_codepoint_t ذريعو_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_معلومات_builder_t *depend_معلومات_2,
                              بنانشان nesting_level_کاٻو_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_معلومات (depend_معلومات_2),
                          nesting_level_کاٻو (nesting_level_کاٻو_2),
                          ذريعو_gid (0)
  {}
};

بنية Layer_رڪارڊ
{
  عامل hb_ot_color_layer_t () ثابت { أرجع {glyphId, colorIdx}; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  اشاريو		colorIdx;	/* Index value to use with a
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
  DEFINE_سائيز_STATIC (4);
};

بنية بنياد_Glyph_رڪارڊ
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
  HBUINT16	پهريون_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	انگ_Layers;	/* Number of color layers
				 * associated with this glyph */
  عام:
  DEFINE_سائيز_STATIC (6);
};

سانچو <قسم_نالو T>
بنية Variable
{
  ساكن constexpr منطقي is_variable = سچ;

  ثابت T& get_قدر () ثابت { أرجع value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    c->انگ_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->جوڙيو_var_idxes (var_Idx_بنياد, c->انگ_var_idxes);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!value.subset (c, instancer, var_Idx_بنياد)) return_trace (خطأ);
    إذا (c->plan->all_axes_pinned)
      return_trace (سچ);

    VarIdx new_varidx;
    new_varidx = var_Idx_بنياد;
    إذا (var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      hb_pair_t<بنانشان, صحيح> *new_varidx_delta;
      إذا (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_بنياد, &new_varidx_delta))
        return_trace (خطأ);

      new_varidx = hb_پهريون (*new_varidx_delta);
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
    value.paint_glyph (c, var_Idx_بنياد);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, var_Idx_بنياد, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, var_Idx_بنياد, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  محمي:
  T      value;
  عام:
  VarIdx var_Idx_بنياد;
  عام:
  DEFINE_سائيز_MIN (VarIdx::static_سائيز + T::min_سائيز);
};

سانچو <قسم_نالو T>
بنية NoVariable
{
  ساكن constexpr منطقي is_variable = خطأ;

  ثابت T& get_قدر () ثابت { أرجع value; }

  ساكن constexpr uint32_t var_Idx_بنياد = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (value.subset (c, instancer, var_Idx_بنياد));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, var_Idx_بنياد);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شي_Var_Store_Instancer &instancer) ثابت
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
  DEFINE_سائيز_MIN (T::min_سائيز);
};

// Color structures

سانچو <قسم_نالو Type, بنانشان fraction_بٽ>
ساكن فراغ
سيٽ_fixed_clamped (HBFixed<Type, fraction_بٽ> &ٻاهريون, عائم value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  ٻاهريون.سيٽ_int (hb_clamp_منزل<قسم_نالو Type::type> (roundf ((value / HBFixed<Type, fraction_بٽ>::mult))));
}

بنية ColorStop
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    c->جوڙيو_palette_اشاريو (palette_اشاريو_2);
    c->انگ_var_idxes = 2;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->stop_وٿي, stop_وٿي.منزل_float(instancer (var_Idx_بنياد, 0)));
      عائم a = alpha.منزل_float (instancer (var_Idx_بنياد, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      سيٽ_fixed_clamped (ٻاهريون->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_سونپيو (ٻاهريون->palette_اشاريو_2, c->plan->colr_palettes.get (palette_اشاريو_2),
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *ٻاهريون,
		       uint32_t varIdx,
		       ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    ٻاهريون->وٿي = stop_وٿي.منزل_float(instancer (varIdx, 0));
    ٻاهريون->color = c->get_color (palette_اشاريو_2,
                               alpha.منزل_float (instancer (varIdx, 1)),
                               &ٻاهريون->is_foreground);
  }

  F2DOT14	stop_وٿي;
  HBUINT16	palette_اشاريو_2;
  F2DOT14	alpha;
  عام:
  DEFINE_سائيز_STATIC (2 + 2 * F2DOT14::static_سائيز);
};

بنية Extend : HBUINT8
{
  تعداد {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عام:
  DEFINE_سائيز_STATIC (1);
};

سانچو <سانچو<قسم_نالو> صنف Var>
بنية Color_سٽ
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    لكل (ثابت خودڪار &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    إذا (!c->serializer->check_سونپيو (ٻاهريون->extend, extend, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) return_trace (خطأ);
    إذا (!c->serializer->check_سونپيو (ٻاهريون->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي)) return_trace (خطأ);

    لكل (ثابت خودڪار& stop : stops.iter ())
    {
      إذا (!stop.subset (c, instancer)) return_trace (خطأ);
    }
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  بنانشان صحيح
  get_color_stops (hb_paint_context_t *c,
                   بنانشان صحيح شروعات,
		   بنانشان صحيح *ڳڻپ,
		   hb_color_stop_t *color_stops,
		   ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    بنانشان صحيح len = stops.len;

    إذا (ڳڻپ && color_stops)
    {
      بنانشان صحيح i;
      لكل (i = 0; i < *ڳڻپ && شروعات + i < len; i++)
        stops[شروعات + i].get_color_stop (c, &color_stops[i], instancer);
      *ڳڻپ = i;
    }

    أرجع len;
  }

  HB_اندروني ساكن بنانشان صحيح static_get_color_stops (hb_color_سٽ_t *color_سٽ,
							  فراغ *color_سٽ_معلومات,
							  بنانشان صحيح شروعات,
							  بنانشان صحيح *ڳڻپ,
							  hb_color_stop_t *color_stops,
							  فراغ *user_معلومات)
  {
    ثابت Color_سٽ *thiz = (ثابت Color_سٽ *) color_سٽ_معلومات;
    hb_paint_context_t *c = (hb_paint_context_t *) user_معلومات;
    أرجع thiz->get_color_stops (c, شروعات, ڳڻپ, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بدل ((بنانشان صحيح) extend)
    {
      حالة Extend::EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
      حالة Extend::EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
      حالة Extend::EXTEND_PAD:
      افتراضي:                     أرجع HB_PAINT_EXTEND_PAD;
    }
  }

  HB_اندروني ساكن hb_paint_extend_t static_get_extend (hb_color_سٽ_t *color_سٽ,
							  فراغ *color_سٽ_معلومات,
							  فراغ *user_معلومات)
  {
    ثابت Color_سٽ *thiz = (ثابت Color_سٽ *) color_سٽ_معلومات;
    أرجع thiz->get_extend ();
  }

  Extend	extend;
  Array16_جو<Var<ColorStop>>	stops;
  عام:
  DEFINE_سائيز_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
بنية مرکب_گهڻائي : HBUINT8
{
  عام:
  DEFINE_سائيز_STATIC (1);
};

بنية Affine2x3
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  { c->انگ_var_idxes = 6; }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      خودڪار fold = [&] (F16DOT16 &target, F16DOT16 ذريعو, بنانشان وٿي) {
        // Keep the stored integer precise while applying the float delta.
        target.سيٽ_int (hb_clamp_منزل<int32_t> (roundf ((مزدوج) ذريعو.منزل_int () +
                                                    (مزدوج) instancer (var_Idx_بنياد, وٿي))));
      };
      fold (ٻاهريون->xx, xx, 0);
      fold (ٻاهريون->yx, yx, 1);
      fold (ٻاهريون->xy, xy, 2);
      fold (ٻاهريون->yy, yy, 3);
      fold (ٻاهريون->dx, dx, 4);
      fold (ٻاهريون->dy, dy, 5);
    }
    return_trace (سچ);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    c->funcs->ڌڪو_transform (c->معلومات,
			      xx.منزل_float (c->instancer (var_Idx_بنياد, 0)),
			      yx.منزل_float (c->instancer (var_Idx_بنياد, 1)),
                              xy.منزل_float (c->instancer (var_Idx_بنياد, 2)),
			      yy.منزل_float (c->instancer (var_Idx_بنياد, 3)),
                              dx.منزل_float (c->instancer (var_Idx_بنياد, 4)),
			      dy.منزل_float (c->instancer (var_Idx_بنياد, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  عام:
  DEFINE_سائيز_STATIC (6 * F16DOT16::static_سائيز);
};

بنية PaintColrLayers
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer HB_اڻ_استعمال_ٿيل) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    uint32_t پهريون_layer_اشاريو = انگ_Layers ? c->plan->colrv1_layers.get (پهريون_Layer_اشاريو) : 0;
    return_trace (c->serializer->check_سونپيو (ٻاهريون->پهريون_Layer_اشاريو, پهريون_layer_اشاريو,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	انگ_Layers;
  HBUINT32	پهريون_Layer_اشاريو;  /* index into COLRv1::layerList */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية PaintSolid
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    c->جوڙيو_palette_اشاريو (palette_اشاريو_2);
    c->انگ_var_idxes = 1;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      عائم a = alpha.منزل_float (instancer (var_Idx_بنياد, 0));
      سيٽ_fixed_clamped (ٻاهريون->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    إذا (format == 3 && c->plan->all_axes_pinned)
        ٻاهريون->format = 2;

    return_trace (c->serializer->check_سونپيو (ٻاهريون->palette_اشاريو_2, c->plan->colr_palettes.get (palette_اشاريو_2),
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_بنياد,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (palette_اشاريو_2,
                           alpha.منزل_float (c->instancer (var_Idx_بنياد, 0)),
                           is_foreground);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_بنياد, &is_foreground, &color);
    c->funcs->color (c->معلومات, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_اشاريو_2;
  F2DOT14	alpha;
  عام:
  DEFINE_سائيز_STATIC (3 + F2DOT14::static_سائيز);
};

سانچو <سانچو<قسم_نالو> صنف Var>
بنية Paint_لڪيري_Gradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    (هذا+color_سٽ_2).closurev1 (c);
    c->انگ_var_idxes = 6;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      ٻاهريون->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
      ٻاهريون->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 3)));
      ٻاهريون->x2 = hb_clamp_منزل<int16_t> ((مزدوج) x2 + (مزدوج) roundf (instancer (var_Idx_بنياد, 4)));
      ٻاهريون->y2 = hb_clamp_منزل<int16_t> ((مزدوج) y2 + (مزدوج) roundf (instancer (var_Idx_بنياد, 5)));
    }

    إذا (format == 5 && c->plan->all_axes_pinned)
        ٻاهريون->format = 4;

    return_trace (ٻاهريون->color_سٽ_2.serialize_subset (c, color_سٽ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سٽ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سٽ_t cl = {
      (فراغ *) &(هذا+color_سٽ_2),
      (هذا+color_سٽ_2).static_get_color_stops, c,
      (هذا+color_سٽ_2).static_get_extend, خالي_اشارو
    };

    c->funcs->لڪيري_gradient (c->معلومات, &cl,
			       x0 + c->instancer (var_Idx_بنياد, 0),
			       y0 + c->instancer (var_Idx_بنياد, 1),
			       x1 + c->instancer (var_Idx_بنياد, 2),
			       y1 + c->instancer (var_Idx_بنياد, 3),
			       x2 + c->instancer (var_Idx_بنياد, 4),
			       y2 + c->instancer (var_Idx_بنياد, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_منزل<Color_سٽ<Var>>	color_سٽ_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  عام:
  DEFINE_سائيز_STATIC (4 + 6 * FWORD::static_سائيز);
};

سانچو <سانچو<قسم_نالو> صنف Var>
بنية PaintRadialGradient
{
  منطقي constant_after_pinning (hb_subset_context_t *c,
                               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    إذا (c->plan->has_avar2) أرجع خطأ;
    ثابت خودڪار &regions = instancer.varStore->get_region_list ();
    لكل (بنانشان i = 0; i < regions.region_ڳڻپ; i++)
    {
      hb_hashmap_t<hb_ٽيگ_t, Triple> axes;
      إذا (!regions.get_var_region (i, c->plan->axes_old_اشاريو_ٽيگ_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_ٻاهريون_جو_گنجائش);
        أرجع خطأ;
      }
      لكل (خودڪار axis : axes)
      {
        Triple *حد_بندي;
        إذا (!c->plan->user_axes_location.has (axis.پهريون, &حد_بندي) ||
            حد_بندي->minimum != حد_بندي->وڌ_کان_وڌ)
          أرجع خطأ;
      }
    }
    أرجع سچ;
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    (هذا+color_سٽ_2).closurev1 (c);
    c->انگ_var_idxes = 6;
  }

  منطقي subset_nonnegative (hb_subset_context_t *c,
                           ثابت شي_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_بنياد) ثابت
  {
    مزدوج ذريعو[6] = {
      x0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 0)),
      y0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)),
      radius0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)),
      x1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 3)),
      y1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 4)),
      radius1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 5))
    };
    خودڪار transparent = [&] {
      خودڪار *ٻاهريون = c->serializer->allocate_سائيز<PaintSolid> (5);
      إذا (!ٻاهريون) أرجع خطأ;
      ٻاهريون->format = 2; ٻاهريون->palette_اشاريو_2 = 0xFFFF; ٻاهريون->alpha.سيٽ_float (0);
      أرجع سچ;
    };
    ثابت خودڪار &سٽ = هذا+color_سٽ_2;
    إذا (ذريعو[2] == ذريعو[5] || !سٽ.stops.len) أرجع transparent ();
    بنية stop_t { مزدوج وٿي, alpha; بنانشان palette, ترتيب; };
    hb_ويڪٽر_t<stop_t> stops;
    إذا (!stops.alloc (سٽ.stops.len)) أرجع خطأ;
    بنانشان ترتيب = 0;
    لكل (ثابت خودڪار &داخلا : سٽ.stops.iter ())
    {
      ثابت خودڪار &stop = داخلا.get_قدر ();
      stops.push (stop_t {(مزدوج) roundf (stop.stop_وٿي.منزل_float (instancer (داخلا.var_Idx_بنياد, 0)) * 16384.f) / 16384.,
                   (مزدوج) hb_clamp (stop.alpha.منزل_float (instancer (داخلا.var_Idx_بنياد, 1)), 0.f, 1.f),
                   stop.palette_اشاريو_2, ترتيب++});
    }
    إذا (stops.اندريون_error ()) أرجع خطأ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      أرجع a.وٿي < b.وٿي ? -1 : a.وٿي > b.وٿي ? 1 :
             a.ترتيب < b.ترتيب ? -1 : a.ترتيب > b.ترتيب ? 1 : 0;
    });
    مزدوج dr = ذريعو[5] - ذريعو[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    بنانشان lattice = 16384;
    لكل (بنانشان j = 0; j < 3; j++)
    {
      مزدوج remainder = fmod (fabs (ذريعو[j + 3] - ذريعو[j]), 16384.);
      إذا (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
        أرجع خطأ;
      }
      بنانشان ساڄي = (بنانشان) remainder;
      طالما (ساڄي) { بنانشان r = lattice % ساڄي; lattice = ساڄي; ساڄي = r; }
    }
    لكل (بنانشان سرڪايو = 0; سرڪايو < 16; سرڪايو++)
    {
      مزدوج b = 1u << سرڪايو;
      مزدوج coarse = dr > 0 ? ceil (-ذريعو[2] / dr) : floor (ذريعو[2] / -dr) - b;
      مزدوج fine = dr > 0 ? ceil (-ذريعو[2] / dr * lattice) / lattice :
                            floor (ذريعو[2] / -dr * lattice) / lattice - b;
      مزدوج candidates[] = {coarse, fine};
      لكل (مزدوج a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        مزدوج geometry[6];
        منطقي سمائي = سچ;
        لكل (بنانشان j = 0; j < 3; j++)
        {
          مزدوج difference = ذريعو[j + 3] - ذريعو[j];
          geometry[j] = ذريعو[j] + a * difference;
          geometry[j + 3] = ذريعو[j] + (a + b) * difference;
          مزدوج هيٺيون = j == 2 ? 0 : -32768, مٿاهون = j == 2 ? 65535 : 32767;
          سمائي &= geometry[j] >= هيٺيون && geometry[j] <= مٿاهون &&
                  geometry[j + 3] >= هيٺيون && geometry[j + 3] <= مٿاهون;
        }
        إذا (!سمائي) استمر;
        بنانشان پهريون = 0, ڳڻپ = stops.ڊيگهائي;
        مزدوج min = (stops[0].وٿي - a) / b, max = (stops.پڇ ().وٿي - a) / b;
        مزدوج bias = 0;
        منطقي constant = خطأ;
        إذا (سٽ.extend == Extend::EXTEND_PAD)
        {
          مزدوج tip = -geometry[2] / (b * dr);
          إذا ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            پهريون = dr > 0 ? stops.ڊيگهائي - 1 : 0;
            ڳڻپ = 1; constant = سچ;
          }
        }
        وإلا
        {
          مزدوج period = max - min;
          إذا (سٽ.extend == Extend::EXTEND_REFLECT) period *= 2;
          إذا (period <= 0) أرجع transparent ();
          مزدوج n = ceil ((-2 - min) / period);
          إذا (n > floor ((32767. / 16384. - max) / period)) استمر;
          bias = n * period;
        }
        لكل (بنانشان j = 0; j < ڳڻپ; j++)
        {
          مزدوج بٽ = constant ? 0 : ((stops[پهريون + j].وٿي - a) / b + bias) * 16384;
          سمائي &= بٽ >= -32768 && بٽ <= 32767 && بٽ == round (بٽ);
        }
        إذا (!سمائي) استمر;
        خودڪار *ٻاهريون = c->serializer->allocate_سائيز<PaintRadialGradient<NoVariable>> (16);
        إذا (!ٻاهريون) أرجع خطأ;
        ٻاهريون->format = 6;
        ٻاهريون->x0 = (صحيح) geometry[0]; ٻاهريون->y0 = (صحيح) geometry[1]; ٻاهريون->radius0 = (بنانشان) geometry[2];
        ٻاهريون->x1 = (صحيح) geometry[3]; ٻاهريون->y1 = (صحيح) geometry[4]; ٻاهريون->radius1 = (بنانشان) geometry[5];
        c->serializer->push ();
        خودڪار *ٻاهريون_سٽ = c->serializer->allocate_سائيز<Color_سٽ<NoVariable>> (3 + ڳڻپ * ColorStop::static_سائيز);
        إذا (!ٻاهريون_سٽ) { c->serializer->ڪڍو_discard (); أرجع خطأ; }
        ٻاهريون_سٽ->extend = سٽ.extend;
        ٻاهريون_سٽ->stops.len = ڳڻپ;
        لكل (بنانشان j = 0; j < ڳڻپ; j++)
        {
          ثابت خودڪار &stop = stops[پهريون + j];
          خودڪار &ٻاهريون_stop = ٻاهريون_سٽ->stops[j].value;
          ٻاهريون_stop.stop_وٿي.سيٽ_float ((عائم) (constant ? 0 : (stop.وٿي - a) / b + bias));
          ٻاهريون_stop.alpha.سيٽ_float ((عائم) stop.alpha);
          إذا (!c->serializer->check_سونپيو (ٻاهريون_stop.palette_اشاريو_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
          { c->serializer->ڪڍو_discard (); أرجع خطأ; }
        }
        c->serializer->جوڙيو_link (ٻاهريون->color_سٽ_2, c->serializer->ڪڍو_pack ());
        أرجع !c->serializer->اندريون_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_بنياد != VarIdx::NO_VARIATION &&
        (radius0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)) < 0 ||
         radius1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_بنياد));
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      ٻاهريون->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->radius0 = hb_clamp_منزل<uint16_t> ((مزدوج) radius0 + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
      ٻاهريون->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 3)));
      ٻاهريون->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 4)));
      ٻاهريون->radius1 = hb_clamp_منزل<uint16_t> ((مزدوج) radius1 + (مزدوج) roundf (instancer (var_Idx_بنياد, 5)));
    }

    إذا (format == 7 && c->plan->all_axes_pinned)
        ٻاهريون->format = 6;

    return_trace (ٻاهريون->color_سٽ_2.serialize_subset (c, color_سٽ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سٽ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سٽ_t cl = {
      (فراغ *) &(هذا+color_سٽ_2),
      (هذا+color_سٽ_2).static_get_color_stops, c,
      (هذا+color_سٽ_2).static_get_extend, خالي_اشارو
    };

    c->funcs->radial_gradient (c->معلومات, &cl,
			       x0 + c->instancer (var_Idx_بنياد, 0),
			       y0 + c->instancer (var_Idx_بنياد, 1),
			       radius0 + c->instancer (var_Idx_بنياد, 2),
			       x1 + c->instancer (var_Idx_بنياد, 3),
			       y1 + c->instancer (var_Idx_بنياد, 4),
			       radius1 + c->instancer (var_Idx_بنياد, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_منزل<Color_سٽ<Var>>	color_سٽ_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  عام:
  DEFINE_سائيز_STATIC (4 + 6 * FWORD::static_سائيز);
};

سانچو <سانچو<قسم_نالو> صنف Var>
بنية PaintSweepGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    (هذا+color_سٽ_2).closurev1 (c);
    c->انگ_var_idxes = 4;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      ٻاهريون->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
      سيٽ_fixed_clamped (ٻاهريون->شروعات_Angle, شروعات_Angle.منزل_float (instancer (var_Idx_بنياد, 2)));
      سيٽ_fixed_clamped (ٻاهريون->پڇاڙي_Angle, پڇاڙي_Angle.منزل_float (instancer (var_Idx_بنياد, 3)));
    }

    إذا (format == 9 && c->plan->all_axes_pinned)
        ٻاهريون->format = 8;

    return_trace (ٻاهريون->color_سٽ_2.serialize_subset (c, color_سٽ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سٽ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سٽ_t cl = {
      (فراغ *) &(هذا+color_سٽ_2),
      (هذا+color_سٽ_2).static_get_color_stops, c,
      (هذا+color_سٽ_2).static_get_extend, خالي_اشارو
    };

    c->funcs->sweep_gradient (c->معلومات, &cl,
			      centerX + c->instancer (var_Idx_بنياد, 0),
			      centerY + c->instancer (var_Idx_بنياد, 1),
                              (شروعات_Angle.منزل_float (c->instancer (var_Idx_بنياد, 2)) + 1) * HB_PI,
                              (پڇاڙي_Angle.منزل_float   (c->instancer (var_Idx_بنياد, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_منزل<Color_سٽ<Var>>	color_سٽ_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		شروعات_Angle;
  F2DOT14		پڇاڙي_Angle;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * FWORD::static_سائيز + 2 * F2DOT14::static_سائيز);
};

// Paint a non-COLR glyph, filled as indicated by paint.
سانچو <قسم_نالو Types>
بنية PaintGlyph
{
  استعمال HBGlyphID = قسم_نالو Types::HBGlyphID;

  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (! c->serializer->check_سونپيو (ٻاهريون->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    return_trace (ٻاهريون->paint.serialize_subset (c, paint, هذا, instancer));
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
  DEFINE_سائيز_STATIC (4 + HBGlyphID::static_سائيز);
};

بنية PaintColrGlyph
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer HB_اڻ_استعمال_ٿيل) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    return_trace (c->serializer->check_سونپيو (ٻاهريون->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
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
  DEFINE_سائيز_STATIC (3);
};

سانچو <سانچو<قسم_نالو> صنف Var>
بنية PaintTransform
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    إذا (!ٻاهريون->transform.serialize_subset (c, transform, هذا, instancer)) return_trace (خطأ);
    إذا (format == 13 && c->plan->all_axes_pinned)
      ٻاهريون->format = 12;
    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
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
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_منزل<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_منزل<Var<Affine2x3>>	transform;
  عام:
  DEFINE_سائيز_STATIC (7);
};

بنية PaintTranslate
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      ٻاهريون->dx = hb_clamp_منزل<int16_t> ((مزدوج) dx + (مزدوج) roundf (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->dy = hb_clamp_منزل<int16_t> ((مزدوج) dy + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
    }

    إذا (format == 15 && c->plan->all_axes_pinned)
        ٻاهريون->format = 14;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم ddx = dx + c->instancer (var_Idx_بنياد, 0);
    عائم ddy = dy + c->instancer (var_Idx_بنياد, 1);

    c->funcs->ڌڪو_translate (c->معلومات, ddx, ddy);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * FWORD::static_سائيز);
};

سانچو <قسم_نالو ذريعو_2>
ساكن منطقي serialize_scale_transform (hb_subset_context_t *c,
                                       عائم sx, عائم sy, عائم dx, عائم dy,
                                       ثابت ذريعو_2 &ذريعو)
{
  خودڪار *ٻاهريون = c->serializer->allocate_سائيز<PaintTransform<NoVariable>> (7);
  إذا (unlikely (!ٻاهريون)) أرجع خطأ;
  ٻاهريون->format = 12;

  c->serializer->push ();
  خودڪار *transform = c->serializer->allocate_سائيز<Affine2x3> (Affine2x3::static_سائيز);
  إذا (unlikely (!transform))
  {
    c->serializer->ڪڍو_discard ();
    أرجع خطأ;
  }
  سيٽ_fixed_clamped (transform->xx, sx);
  transform->yx.سيٽ_float (0);
  transform->xy.سيٽ_float (0);
  سيٽ_fixed_clamped (transform->yy, sy);
  سيٽ_fixed_clamped (transform->dx, dx);
  سيٽ_fixed_clamped (transform->dy, dy);
  c->serializer->جوڙيو_link (ٻاهريون->transform, c->serializer->ڪڍو_pack ());

  c->serializer->push ();
  منطقي ret = ذريعو ();
  إذا (ret)
    c->serializer->جوڙيو_link (ٻاهريون->src, c->serializer->ڪڍو_pack ());
  وإلا
    c->serializer->ڪڍو_discard ();
  أرجع ret;
}

سانچو <قسم_نالو بنياد_3>
ساكن منطقي subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت بنياد_3 *بنياد,
                                      ثابت Offset24_منزل<Paint> &src,
                                      ثابت شي_Var_Store_Instancer &instancer,
                                      عائم sx, عائم sy,
                                      عائم cx = 0, عائم cy = 0)
{
  إذا (src.is_null ()) أرجع خطأ;
  خودڪار ذريعو = [&] { أرجع c->dispatch (بنياد+src, instancer); };
  عائم dx = cx * (1 - sx), dy = cy * (1 - sy);
  إذا (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    أرجع serialize_scale_transform (c, sx, sy, dx, dy, ذريعو);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  خودڪار inner = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, ذريعو);
  };
  خودڪار inverse = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  خودڪار scale = [&] {
    أرجع serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  خودڪار outer = [&] {
    أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

بنية PaintScale
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_بنياد, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_بنياد, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, sx, sy));
    }
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->scaleX, scaleX.منزل_float (instancer (var_Idx_بنياد, 0)));
      سيٽ_fixed_clamped (ٻاهريون->scaleY, scaleY.منزل_float (instancer (var_Idx_بنياد, 1)));
    }

    إذا (format == 17 && c->plan->all_axes_pinned)
        ٻاهريون->format = 16;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_بنياد, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_بنياد, 1));

    c->funcs->ڌڪو_scale (c->معلومات, sx, sy);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * F2DOT14::static_سائيز);
};

بنية PaintScaleAroundCenter
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_بنياد, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_بنياد, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_بنياد, 2)),
            centerY + roundf (instancer (var_Idx_بنياد, 3))));
    }
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->scaleX, scaleX.منزل_float (instancer (var_Idx_بنياد, 0)));
      سيٽ_fixed_clamped (ٻاهريون->scaleY, scaleY.منزل_float (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
      ٻاهريون->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنياد, 3)));
    }

    إذا (format == 19 && c->plan->all_axes_pinned)
        ٻاهريون->format = 18;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_بنياد, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_بنياد, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنياد, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنياد, 3);

    c->funcs->ڌڪو_scale_around_center (c->معلومات, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * F2DOT14::static_سائيز + 2 * FWORD::static_سائيز);
};

بنية Paint_Scale_هڪجهڙو
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_بنياد, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, s, s));
    }
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
      سيٽ_fixed_clamped (ٻاهريون->scale, scale.منزل_float (instancer (var_Idx_بنياد, 0)));

    إذا (format == 21 && c->plan->all_axes_pinned)
        ٻاهريون->format = 20;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_بنياد, 0));

    c->funcs->ڌڪو_scale (c->معلومات, s, s);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عام:
  DEFINE_سائيز_STATIC (4 + F2DOT14::static_سائيز);
};

بنية Paint_Scale_هڪجهڙو_Around_Center
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_بنياد, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_بنياد, 1)),
            centerY + roundf (instancer (var_Idx_بنياد, 2))));
    }
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->scale, scale.منزل_float (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
    }

    إذا (format == 23 && c->plan->all_axes_pinned)
        ٻاهريون->format = 22;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_بنياد, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنياد, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنياد, 2);

    c->funcs->ڌڪو_scale_around_center (c->معلومات, s, s, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_سائيز_STATIC (4 + F2DOT14::static_سائيز + 2 * FWORD::static_سائيز);
};

بنية PaintRotate
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
      سيٽ_fixed_clamped (ٻاهريون->angle, angle.منزل_float (instancer (var_Idx_بنياد, 0)));

    إذا (format == 25 && c->plan->all_axes_pinned)
      ٻاهريون->format = 24;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_بنياد, 0));

    c->funcs->ڌڪو_rotate (c->معلومات, a);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عام:
  DEFINE_سائيز_STATIC (4 + F2DOT14::static_سائيز);
};

بنية PaintRotateAroundCenter
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->angle, angle.منزل_float (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
    }

    إذا (format ==27 && c->plan->all_axes_pinned)
        ٻاهريون->format = 26;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_بنياد, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنياد, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنياد, 2);

    c->funcs->ڌڪو_rotate_around_center (c->معلومات, a, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_سائيز_STATIC (4 + F2DOT14::static_سائيز + 2 * FWORD::static_سائيز);
};

بنية PaintSkew
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_بنياد, 0)));
      سيٽ_fixed_clamped (ٻاهريون->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_بنياد, 1)));
    }

    إذا (format == 29 && c->plan->all_axes_pinned)
        ٻاهريون->format = 28;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_بنياد, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_بنياد, 1));

    c->funcs->ڌڪو_skew (c->معلومات, sx, sy);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * F2DOT14::static_سائيز);
};

بنية PaintSkewAroundCenter
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      سيٽ_fixed_clamped (ٻاهريون->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_بنياد, 0)));
      سيٽ_fixed_clamped (ٻاهريون->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_بنياد, 2)));
      ٻاهريون->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_بنياد, 3)));
    }

    إذا (format == 31 && c->plan->all_axes_pinned)
        ٻاهريون->format = 30;

    return_trace (ٻاهريون->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_بنياد, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_بنياد, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_بنياد, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_بنياد, 3);

    c->funcs->ڌڪو_skew_around_center (c->معلومات, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_transform (c->معلومات);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_سائيز_STATIC (4 + 2 * F2DOT14::static_سائيز + 2 * FWORD::static_سائيز);
};

بنية Paint_مرکب
{
  HB_اندروني فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندروني فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= ٻاهريون->src.serialize_subset (c, src, هذا, instancer);
    ret |= ٻاهريون->backdrop.serialize_subset (c, backdrop, هذا, instancer);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  c->check_ops (هذا->min_سائيز) && // PainComposite can get exponential
                  src.sanitize (c, هذا) &&
                  backdrop.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    hb_paint_مرکب_گهڻائي_t مرکب_گهڻائي_2;
    بدل ((بنانشان صحيح) گهڻائي)
    {
      حالة HB_PAINT_مرکب_گهڻائي_CLEAR:
      حالة HB_PAINT_مرکب_گهڻائي_SRC:
      حالة HB_PAINT_مرکب_گهڻائي_DEST:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_OVER:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_OVER:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_ATOP:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_ATOP:
      حالة HB_PAINT_مرکب_گهڻائي_XOR:
      حالة HB_PAINT_مرکب_گهڻائي_PLUS:
      حالة HB_PAINT_مرکب_گهڻائي_SCREEN:
      حالة HB_PAINT_مرکب_گهڻائي_OVERLAY:
      حالة HB_PAINT_مرکب_گهڻائي_DARKEN:
      حالة HB_PAINT_مرکب_گهڻائي_LIGHTEN:
      حالة HB_PAINT_مرکب_گهڻائي_COLOR_DODGE:
      حالة HB_PAINT_مرکب_گهڻائي_COLOR_BURN:
      حالة HB_PAINT_مرکب_گهڻائي_HARD_LIGHT:
      حالة HB_PAINT_مرکب_گهڻائي_SOFT_LIGHT:
      حالة HB_PAINT_مرکب_گهڻائي_DIFFERENCE:
      حالة HB_PAINT_مرکب_گهڻائي_EXCLUSION:
      حالة HB_PAINT_مرکب_گهڻائي_MULTIPLY:
      حالة HB_PAINT_مرکب_گهڻائي_HSL_HUE:
      حالة HB_PAINT_مرکب_گهڻائي_HSL_SATURATION:
      حالة HB_PAINT_مرکب_گهڻائي_HSL_COLOR:
      حالة HB_PAINT_مرکب_گهڻائي_HSL_LUMINOSITY:
	مرکب_گهڻائي_2 = (hb_paint_مرکب_گهڻائي_t) (بنانشان صحيح) گهڻائي;
	اكسر;
      افتراضي:
	مرکب_گهڻائي_2 = HB_PAINT_مرکب_گهڻائي_CLEAR;
	اكسر;
    }

    c->funcs->ڌڪو_گروهه_for (c->معلومات, HB_PAINT_مرکب_گهڻائي_SRC_OVER);
    c->recurse (هذا+backdrop);
    c->funcs->ڌڪو_گروهه_for (c->معلومات, مرکب_گهڻائي_2);
    c->recurse (هذا+src);
    c->funcs->ڪڍو_گروهه (c->معلومات, مرکب_گهڻائي_2);
    c->funcs->ڪڍو_گروهه (c->معلومات, HB_PAINT_مرکب_گهڻائي_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  مرکب_گهڻائي		گهڻائي;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_منزل<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عام:
  DEFINE_سائيز_STATIC (8);
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

  فراغ get_clip_box (Clip_Box_معلومات &clip_box, ثابت شي_Var_Store_Instancer &instancer HB_اڻ_استعمال_ٿيل) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer,
               uint32_t var_Idx_بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_بنياد != VarIdx::NO_VARIATION)
    {
      ٻاهريون->xMin = hb_clamp_منزل<int16_t> ((مزدوج) xMin + (مزدوج) floorf (instancer (var_Idx_بنياد, 0)));
      ٻاهريون->yMin = hb_clamp_منزل<int16_t> ((مزدوج) yMin + (مزدوج) floorf (instancer (var_Idx_بنياد, 1)));
      ٻاهريون->xMax = hb_clamp_منزل<int16_t> ((مزدوج) xMax + (مزدوج) ceilf (instancer (var_Idx_بنياد, 2)));
      ٻاهريون->yMax = hb_clamp_منزل<int16_t> ((مزدوج) yMax + (مزدوج) ceilf (instancer (var_Idx_بنياد, 3)));
    }

    إذا (format == 2 && c->plan->all_axes_pinned)
        ٻاهريون->format = 1;

    return_trace (سچ);
  }

  عام:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عام:
  DEFINE_سائيز_STATIC (1 + 4 * FWORD::static_سائيز);
};

بنية ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  فراغ get_clip_box (Clip_Box_معلومات &clip_box, ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    إذا (instancer)
    {
      clip_box.xMin = hb_saturate_جوڙيو (clip_box.xMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_بنياد, 0))));
      clip_box.yMin = hb_saturate_جوڙيو (clip_box.yMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_بنياد, 1))));
      clip_box.xMax = hb_saturate_جوڙيو (clip_box.xMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_بنياد, 2))));
      clip_box.yMax = hb_saturate_جوڙيو (clip_box.yMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_بنياد, 3))));
    }
  }

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  { c->variation_indices->جوڙيو_range (var_Idx_بنياد, var_Idx_بنياد + 3); }
};

بنية ClipBox
{
  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.format2.closurev1 (c); أرجع;
    افتراضي:أرجع;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
                    ثابت شي_Var_Store_Instancer &instancer) ثابت
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
    extents->ويڪر = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->اوچائي = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    أرجع سچ;
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

بنية Clip_رڪارڊ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروعات_Glyph_ID ? -1 : g <= پڇاڙي_Glyph_ID ? 0 : +1; }

  فراغ closurev1 (hb_colrv1_گهيرو_context_t* c, ثابت فراغ *بنياد) ثابت
  {
    إذا (!c->glyphs->intersects (شروعات_Glyph_ID, پڇاڙي_Glyph_ID)) أرجع;
    (بنياد+clipBox).closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنياد,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    return_trace (ٻاهريون->clipBox.serialize_subset (c, clipBox, بنياد, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && clipBox.sanitize (c, بنياد));
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
		    ثابت فراغ *بنياد,
		    ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    أرجع (بنياد+clipBox).get_extents (extents, instancer);
  }

  عام:
  HBUINT16		شروعات_Glyph_ID;  // first gid clip applies to
  HBUINT16		پڇاڙي_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_منزل<ClipBox>	clipBox;   // Box or VarBox
  عام:
  DEFINE_سائيز_STATIC (7);
};
DECLARE_NULL_NAMESPACE_بائيٽون (OT, Clip_رڪارڊ);

بنية ClipList
{
  بنانشان serialize_clip_رڪارڊون (hb_subset_context_t *c,
                                   ثابت شي_Var_Store_Instancer &instancer,
                                   ثابت hb_سيٽ_t& gids,
                                   ثابت hb_map_t& gid_وٿي_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (gids.is_empty () ||
        gid_وٿي_map.get_population () != gids.get_population ())
      return_trace (0);

    بنانشان ڳڻپ  = 0;

    hb_codepoint_t شروعات_gid= gids.get_min ();
    hb_codepoint_t prev_gid = شروعات_gid;

    بنانشان وٿي = gid_وٿي_map.get (شروعات_gid);
    بنانشان prev_وٿي = وٿي;
    لكل (ثابت hb_codepoint_t _ : gids.iter ())
    {
      إذا (_ == شروعات_gid) استمر;

      وٿي = gid_وٿي_map.get (_);
      إذا (_ == prev_gid + 1 &&  وٿي == prev_وٿي)
      {
        prev_gid = _;
        استمر;
      }

      Clip_رڪارڊ رڪارڊ;
      رڪارڊ.شروعات_Glyph_ID = شروعات_gid;
      رڪارڊ.پڇاڙي_Glyph_ID = prev_gid;
      رڪارڊ.clipBox = prev_وٿي;

      إذا (!رڪارڊ.subset (c, هذا, instancer)) return_trace (0);
      ڳڻپ++;

      شروعات_gid = _;
      prev_gid = _;
      prev_وٿي = وٿي;
    }

    //last one
    {
      Clip_رڪارڊ رڪارڊ;
      رڪارڊ.شروعات_Glyph_ID = شروعات_gid;
      رڪارڊ.پڇاڙي_Glyph_ID = prev_gid;
      رڪارڊ.clipBox = prev_وٿي;
      إذا (!رڪارڊ.subset (c, هذا, instancer)) return_trace (0);
      ڳڻپ++;
    }
    return_trace (ڳڻپ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    إذا (!c->serializer->check_سونپيو (ٻاهريون->format, format, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) return_trace (خطأ);

    ثابت hb_سيٽ_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_وٿي_map;
    hb_سيٽ_t new_gids;
    لكل (ثابت Clip_رڪارڊ& رڪارڊ : clips.iter ())
    {
      بنانشان شروعات_gid = رڪارڊ.شروعات_Glyph_ID;
      بنانشان پڇاڙي_gid = رڪارڊ.پڇاڙي_Glyph_ID;
      لكل (بنانشان gid = شروعات_gid; gid <= پڇاڙي_gid; gid++)
      {
        إذا (!glyphset.has (gid) || !glyph_map.has (gid)) استمر;
        بنانشان new_gid = glyph_map.get (gid);
        new_gid_وٿي_map.سيٽ (new_gid, رڪارڊ.clipBox);
        new_gids.جوڙيو (new_gid);
      }
    }

    بنانشان ڳڻپ = serialize_clip_رڪارڊون (c, instancer, new_gids, new_gid_وٿي_map);
    إذا (!ڳڻپ) return_trace (خطأ);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->clips.len, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
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
	       ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    خودڪار *rec = clips.as_array ().bsearch (gid);
    إذا (rec)
      أرجع rec->get_extents (extents, هذا, instancer);
    أرجع خطأ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_جو<Clip_رڪارڊ>	clips;  // Clip records, sorted by startGlyphID
  عام:
  DEFINE_سائيز_ARRAY_SIZED (5, clips);
};

بنية Paint
{

  سانچو <قسم_نالو ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_شروعات_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->پڇاڙي_recursion (هذا->dispatch (c, std::forward<Ts> (ds)...)));
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  منطقي get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); أرجع سچ;
    حالة 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); أرجع سچ;
    افتراضي: أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<Paint_لڪيري_Gradient<NoVariable>>	paintformat4;
  Variable<Paint_لڪيري_Gradient<Variable>>	paintformat5;
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
  NoVariable<Paint_Scale_هڪجهڙو>			paintformat20;
  Variable<Paint_Scale_هڪجهڙو>			paintformat21;
  NoVariable<Paint_Scale_هڪجهڙو_Around_Center>	paintformat22;
  Variable<Paint_Scale_هڪجهڙو_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_مرکب				paintformat32;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_MIN (2);
};

بنية بنياد_Glyph_Paint_رڪارڊ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت فراغ* src_بنياد, hb_subset_context_t *c,
                  ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = s->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    إذا (!s->check_سونپيو (ٻاهريون->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    return_trace (ٻاهريون->paint.serialize_subset (c, paint, src_بنياد, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && paint.sanitize (c, بنياد)));
  }

  عام:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_منزل<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية بنياد_Glyph_List : Sorted_Array32_جو<بنياد_Glyph_Paint_رڪارڊ>
{
  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون)))  return_trace (خطأ);
    ثابت hb_سيٽ_t* glyphset = &c->plan->_glyphset_colred;

    لكل (ثابت خودڪار& _ : as_array ())
    {
      بنانشان gid = _.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      إذا (_.serialize (c->serializer, c->plan->glyph_map, هذا, c, instancer)) ٻاهريون->len++;
      وإلا return_trace (خطأ);
    }

    return_trace (ٻاهريون->len != 0);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Sorted_Array32_جو<بنياد_Glyph_Paint_رڪارڊ>::sanitize (c, هذا));
  }
};

بنية LayerList : Array32_جو_Offset32_منزل<Paint>
{
  ثابت Paint& get_paint (بنانشان i) ثابت
  { أرجع هذا+(*هذا)[i]; }

  منطقي subset (hb_subset_context_t *c,
               ثابت شي_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودڪار& _ : + hb_enumerate (*هذا)
                         | hb_filter (c->plan->colrv1_layers, hb_پهريون))

    {
      خودڪار *o = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      ret |= o->serialize_subset (c, _.second, هذا, instancer);
    }
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Array32_جو_Offset32_منزل<Paint>::sanitize (c, هذا));
  }
};

بنية delta_سيٽ_اشاريو_map_subset_plan_t
{
  بنانشان get_inner_ٻٽو_انگ_ڳڻپ () ثابت { أرجع inner_ٻٽو_انگ_ڳڻپ; }
  بنانشان get_ويڪر ()           ثابت { أرجع ((outer_ٻٽو_انگ_ڳڻپ + inner_ٻٽو_انگ_ڳڻپ + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  delta_سيٽ_اشاريو_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ڳڻپ = 0;
    outer_ٻٽو_انگ_ڳڻپ = 0;
    inner_ٻٽو_انگ_ڳڻپ = 1;
    output_map.init ();

    /* search backwards */
    بنانشان ڳڻپ = new_deltaset_idx_varidx_map.get_population ();
    إذا (!ڳڻپ) أرجع;

    بنانشان آخري_idx = (بنانشان)-1;
    بنانشان آخري_varidx = (بنانشان)-1;

    لكل (بنانشان i = ڳڻپ; i; i--)
    {
      بنانشان delta_سيٽ_idx = i - 1;
      بنانشان var_idx = new_deltaset_idx_varidx_map.get (delta_سيٽ_idx);
      إذا (i == ڳڻپ)
      {
        آخري_idx = delta_سيٽ_idx;
        آخري_varidx = var_idx;
        استمر;
      }
      إذا (var_idx != آخري_varidx)
        اكسر;
      آخري_idx = delta_سيٽ_idx;
    }

    map_ڳڻپ = آخري_idx + 1;
  }

  منطقي remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_ٻٽو_انگ_ڳڻپ = 1;
    inner_ٻٽو_انگ_ڳڻپ = 1;

    إذا (unlikely (!output_map.resize_dirty (map_ڳڻپ))) أرجع خطأ;

    لكل (بنانشان idx = 0; idx < map_ڳڻپ; idx++)
    {
      uint32_t *var_idx;
      إذا (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) أرجع خطأ;
      output_map.arrayZ[idx] = *var_idx;

      بنانشان outer = (*var_idx) >> 16;
      بنانشان ٻٽو_انگ_ڳڻپ = (outer == 0) ? 1 : hb_ٻٽو_انگ_storage (outer);
      outer_ٻٽو_انگ_ڳڻپ = hb_max (ٻٽو_انگ_ڳڻپ, outer_ٻٽو_انگ_ڳڻپ);

      بنانشان inner = (*var_idx) & 0xFFFF;
      ٻٽو_انگ_ڳڻپ = (inner == 0) ? 1 : hb_ٻٽو_انگ_storage (inner);
      inner_ٻٽو_انگ_ڳڻپ = hb_max (ٻٽو_انگ_ڳڻپ, inner_ٻٽو_انگ_ڳڻپ);
    }
    أرجع سچ;
  }

  خاص:
  بنانشان map_ڳڻپ;
  بنانشان outer_ٻٽو_انگ_ڳڻپ;
  بنانشان inner_ٻٽو_انگ_ڳڻپ;
  hb_ويڪٽر_t<uint32_t> output_map;
};

بنية COLR
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_COLR;

  منطقي has_معلومات () ثابت { أرجع has_v0_معلومات () || نسخو; }

  منطقي has_v0_معلومات () ثابت { أرجع انگ_بنياد_Glyphs; }
  منطقي has_v1_معلومات () ثابت
  {
    إذا (نسخو < 1)
      أرجع خطأ;
    hb_barrier ();

    أرجع (هذا+بنياد_Glyph_List_2).len > 0;
  }

  بنانشان صحيح get_glyph_layers (hb_codepoint_t       glyph,
				 بنانشان صحيح         شروعات_وٿي,
				 بنانشان صحيح        *ڳڻپ, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت بنياد_Glyph_رڪارڊ &رڪارڊ = (هذا+بنياد_Glyphs_Z).bsearch (انگ_بنياد_Glyphs, glyph);

    hb_array_t<ثابت Layer_رڪارڊ> all_layers = (هذا+layersZ).as_array (انگ_Layers);
    hb_array_t<ثابت Layer_رڪارڊ> glyph_layers = all_layers.sub_array (رڪارڊ.پهريون_Layer_Idx,
								       رڪارڊ.انگ_Layers);
    إذا (ڳڻپ && layers)
    {
      + glyph_layers.sub_array (شروعات_وٿي, ڳڻپ)
      | hb_sink (hb_array (layers, *ڳڻپ))
      ;
    }
    أرجع glyph_layers.ڊيگهائي;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_جدول<COLR> (face); }

    ~accelerator_t ()
    {
      خودڪار *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقي has_معلومات () ثابت { أرجع colr->has_معلومات (); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT
    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      إذا (unlikely (!has_معلومات ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, فراغ *معلومات,
		      بنانشان صحيح palette_اشاريو,
		      hb_color_t foreground,
		      منطقي clip = سچ) ثابت
    {
      إذا (unlikely (!has_معلومات ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = colr->paint_glyph (font, glyph, funcs, معلومات, palette_اشاريو, foreground, clip, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }
#جيڪڏهن_پورو

    منطقي is_valid () { أرجع colr.get_blob ()->ڊيگهائي; }

    فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
    {
      إذا (!has_معلومات ()) أرجع;
      colr->depend (depend_معلومات);
    }

    فراغ گهيرو_glyphs (hb_codepoint_t glyph,
			 hb_سيٽ_t *related_ids /*ٻاهريون*/) ثابت
    { colr->گهيرو_glyphs (glyph, related_ids); }

    فراغ گهيرو_V0palette_indices (ثابت hb_سيٽ_t *glyphs,
				    hb_سيٽ_t *palettes /*ٻاهريون*/) ثابت
    { colr->گهيرو_V0palette_indices (glyphs, palettes); }

    فراغ گهيرو_for_V1 (hb_سيٽ_t *glyphset,
                        hb_سيٽ_t *layer_indices,
                        hb_سيٽ_t *palette_indices,
                        hb_سيٽ_t *variation_indices,
                        hb_سيٽ_t *delta_سيٽ_indices) ثابت
    { colr->گهيرو_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_سيٽ_indices); }

    منطقي has_var_store () ثابت
    { أرجع colr->has_var_store (); }

    ثابت شي_Variation_Store &get_var_store () ثابت
    { أرجع colr->get_var_store (); }
    ثابت شي_Variation_Store *get_var_store_ptr () ثابت
    { أرجع colr->get_var_store_ptr (); }

    منطقي has_delta_سيٽ_اشاريو_map () ثابت
    { أرجع colr->has_delta_سيٽ_اشاريو_map (); }

    ثابت Delta_سيٽ_اشاريو_Map &get_delta_سيٽ_اشاريو_map () ثابت
    { أرجع colr->get_delta_سيٽ_اشاريو_map (); }
    ثابت Delta_سيٽ_اشاريو_Map *get_delta_سيٽ_اشاريو_map_ptr () ثابت
    { أرجع colr->get_delta_سيٽ_اشاريو_map_ptr (); }

    خاص:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالي_اشارو)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع خالي_اشارو;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (خالي_اشارو, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عام:
    hb_blob_ptr_t<COLR> colr;
    خاص:
    بدلجندڙ hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    // v0
    hb_array_t<ثابت بنياد_Glyph_رڪارڊ> بنياد_Glyphs = (هذا+بنياد_Glyphs_Z).as_array (انگ_بنياد_Glyphs);
    لكل (ثابت بنياد_Glyph_رڪارڊ &رڪارڊ : بنياد_Glyphs)
    {
      خودڪار glyph_layers = (هذا+layersZ).as_array (انگ_Layers).sub_array (رڪارڊ.پهريون_Layer_Idx,
                                                                         رڪارڊ.انگ_Layers);
      لكل (ثابت Layer_رڪارڊ layer : glyph_layers)
        depend_معلومات->جوڙيو_depend(رڪارڊ.glyphId, HB_OT_ٽيگ_COLR, layer.glyphId);
    }

    إذا (!has_v1_معلومات())
      أرجع;

    // v1
    hb_colrv1_depend_context_t c (هذا, depend_معلومات);
    ثابت بنياد_Glyph_List &baseglyph_paintrecords = هذا+بنياد_Glyph_List_2;

    لكل (ثابت بنياد_Glyph_Paint_رڪارڊ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.ذريعو_gid = baseglyph_paintrecord.glyphId;
      c.گهميل_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  فراغ گهيرو_glyphs (hb_codepoint_t glyph,
		       hb_سيٽ_t *related_ids /*ٻاهريون*/) ثابت
  {
    ثابت بنياد_Glyph_رڪارڊ *رڪارڊ = get_بنياد_glyph_رڪارڊ (glyph);
    إذا (!رڪارڊ) أرجع;

    خودڪار glyph_layers = (هذا+layersZ).as_array (انگ_Layers).sub_array (رڪارڊ->پهريون_Layer_Idx,
								       رڪارڊ->انگ_Layers);
    إذا (!glyph_layers.ڊيگهائي) أرجع;
    related_ids->جوڙيو_array (&glyph_layers[0].glyphId, glyph_layers.ڊيگهائي, Layer_رڪارڊ::min_سائيز);
  }

  فراغ گهيرو_V0palette_indices (ثابت hb_سيٽ_t *glyphs,
				  hb_سيٽ_t *palettes /*ٻاهريون*/) ثابت
  {
    إذا (!انگ_بنياد_Glyphs || !انگ_Layers) أرجع;
    hb_array_t<ثابت بنياد_Glyph_رڪارڊ> بنياد_Glyphs = (هذا+بنياد_Glyphs_Z).as_array (انگ_بنياد_Glyphs);
    hb_array_t<ثابت Layer_رڪارڊ> all_layers = (هذا+layersZ).as_array (انگ_Layers);

    لكل (ثابت بنياد_Glyph_رڪارڊ رڪارڊ : بنياد_Glyphs)
    {
      إذا (!glyphs->has (رڪارڊ.glyphId)) استمر;
      hb_array_t<ثابت Layer_رڪارڊ> glyph_layers = all_layers.sub_array (رڪارڊ.پهريون_Layer_Idx,
                                                                   رڪارڊ.انگ_Layers);
      لكل (ثابت Layer_رڪارڊ layer : glyph_layers)
        palettes->جوڙيو (layer.colorIdx);
    }
  }

  فراغ گهيرو_for_V1 (hb_سيٽ_t *glyphset,
                      hb_سيٽ_t *layer_indices,
                      hb_سيٽ_t *palette_indices,
                      hb_سيٽ_t *variation_indices,
                      hb_سيٽ_t *delta_سيٽ_indices) ثابت
  {
    إذا (نسخو < 1) أرجع;
    hb_barrier ();

    hb_سيٽ_t گهميل_glyphs;

    hb_colrv1_گهيرو_context_t c (هذا, &گهميل_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت بنياد_Glyph_List &baseglyph_paintrecords = هذا+بنياد_Glyph_List_2;

    لكل (ثابت بنياد_Glyph_Paint_رڪارڊ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      بنانشان gid = baseglyph_paintrecord.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_سيٽ_union (glyphset, &گهميل_glyphs);

    ثابت ClipList &cliplist = هذا+clipList;
    c.glyphs = glyphset;
    لكل (ثابت Clip_رڪارڊ &clip_رڪارڊ : cliplist.clips.iter())
      clip_رڪارڊ.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    إذا (has_delta_سيٽ_اشاريو_map ())
    {
      ثابت Delta_سيٽ_اشاريو_Map &var_idx_map = هذا+varIdxMap;
      delta_سيٽ_indices->سيٽ (*variation_indices);
      variation_indices->clear ();
      لكل (بنانشان delta_سيٽ_idx : *delta_سيٽ_indices)
        variation_indices->جوڙيو (var_idx_map.map (delta_سيٽ_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { أرجع (هذا+layerList); }

  ثابت بنياد_Glyph_List& get_baseglyphList () ثابت
  { أرجع (هذا+بنياد_Glyph_List_2); }

  منطقي has_var_store () ثابت
  { أرجع نسخو >= 1 && hb_barrier () && varStore != 0; }

  منطقي has_delta_سيٽ_اشاريو_map () ثابت
  { أرجع نسخو >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقي has_clip_list () ثابت
  { أرجع نسخو >= 1 && hb_barrier () && clipList != 0; }

  ثابت Delta_سيٽ_اشاريو_Map &get_delta_سيٽ_اشاريو_map () ثابت
  { أرجع has_delta_سيٽ_اشاريو_map () && hb_barrier () ? هذا+varIdxMap : Null (Delta_سيٽ_اشاريو_Map); }
  ثابت Delta_سيٽ_اشاريو_Map *get_delta_سيٽ_اشاريو_map_ptr () ثابت
  { أرجع has_delta_سيٽ_اشاريو_map () && hb_barrier () ? &(هذا+varIdxMap) : خالي_اشارو; }

  ثابت شي_Variation_Store &get_var_store () ثابت
  { أرجع has_var_store () && hb_barrier () ? هذا+varStore : Null (شي_Variation_Store); }
  ثابت شي_Variation_Store *get_var_store_ptr () ثابت
  { أرجع has_var_store () && hb_barrier () ? &(هذا+varStore) : خالي_اشارو; }

  ثابت ClipList &get_clip_list () ثابت
  { أرجع has_clip_list () && hb_barrier () ? هذا+clipList : Null (ClipList); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  (هذا+بنياد_Glyphs_Z).sanitize (c, انگ_بنياد_Glyphs) &&
                  (هذا+layersZ).sanitize (c, انگ_Layers) &&
                  (نسخو == 0 ||
		   (hb_barrier () &&
		    بنياد_Glyph_List_2.sanitize (c, هذا) &&
		    layerList.sanitize (c, هذا) &&
		    clipList.sanitize (c, هذا) &&
		    varIdxMap.sanitize (c, هذا) &&
		    varStore.sanitize (c, هذا))));
  }

  سانچو<قسم_نالو بنياد_Iterator, قسم_نالو LayerIterator,
	   hb_requires (hb_is_iterator (بنياد_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقي serialize_V0 (hb_serialize_context_t *c,
		     بنانشان نسخو,
		     بنياد_Iterator بنياد_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (بنياد_it.len () != layer_it.len ()))
      return_trace (خطأ);

    هذا->نسخو = نسخو;
    انگ_Layers = 0;
    انگ_بنياد_Glyphs = بنياد_it.len ();
    إذا (انگ_بنياد_Glyphs == 0)
    {
      بنياد_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (سچ);
    }

    c->push ();
    لكل (ثابت hb_شي_type<بنياد_Iterator> _ : + بنياد_it.iter ())
    {
      خودڪار* رڪارڊ = c->embed (_);
      إذا (unlikely (!رڪارڊ)) return_trace (خطأ);
      رڪارڊ->پهريون_Layer_Idx = انگ_Layers;
      انگ_Layers += رڪارڊ->انگ_Layers;
    }
    c->جوڙيو_link (بنياد_Glyphs_Z, c->ڪڍو_pack ());

    c->push ();
    لكل (ثابت hb_شي_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->جوڙيو_link (layersZ, c->ڪڍو_pack ());

    return_trace (سچ);
  }

  ثابت بنياد_Glyph_رڪارڊ* get_بنياد_glyph_رڪارڊ (hb_codepoint_t gid) ثابت
  {
    ثابت بنياد_Glyph_رڪارڊ* رڪارڊ = &(هذا+بنياد_Glyphs_Z).bsearch (انگ_بنياد_Glyphs, (بنانشان صحيح) gid);
    إذا (رڪارڊ == &Null (بنياد_Glyph_رڪارڊ) ||
        (رڪارڊ && (hb_codepoint_t) رڪارڊ->glyphId != gid))
      رڪارڊ = خالي_اشارو;
    أرجع رڪارڊ;
  }

  ثابت بنياد_Glyph_Paint_رڪارڊ* get_بنياد_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت بنياد_Glyph_List &list = هذا+بنياد_Glyph_List_2;
    بنانشان صحيح i;
    إذا (!list.bfind ((بنانشان) gid, &i))
      أرجع خالي_اشارو;
    أرجع &list[i];
  }

  منطقي downgrade_منزل_V0 (ثابت hb_سيٽ_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    لكل (ثابت بنياد_Glyph_Paint_رڪارڊ& _ : get_baseglyphList ())
      إذا (glyphset.has (_.glyphId))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        COLR* ٻاهريون /*ٻاهريون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (سچ);

    ثابت شي_Variation_Store& var_store = هذا+varStore;
    إذا (c->plan->normalized_coords)
    {
      شي_variations_t شي_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقي optimize = (varIdxMap != 0) ? سچ : خطأ;
      إذا (!شي_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (خطأ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      إذا (شي_vars.get_region_list () &&
          شي_vars.get_vardata_encodings () &&
          !ٻاهريون->varStore.serialize_serialize (c->serializer,
                                              شي_vars.has_long_لفظ (),
                                              c->plan->axis_ٽيگ_فهرست,
                                              شي_vars.get_region_list (),
                                              شي_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      إذا (optimize)
      {
        ثابت hb_map_t &varidx_map = شي_vars.get_varidx_map ();
        لكل (خودڪار _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
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
      hb_سيٽ_t dead_regions;
      إذا (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_اشاريو_ٽيگ_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      إذا (unlikely (!ٻاهريون->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : خالي_اشارو)))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  منطقي subset_delta_سيٽ_اشاريو_map (hb_subset_context_t *c,
                                   COLR* ٻاهريون /*ٻاهريون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (سچ);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_سيٽ_اشاريو_map_subset_plan_t اشاريو_map_plan (deltaset_idx_varidx_map);

    إذا (unlikely (!اشاريو_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (خطأ);

    return_trace (ٻاهريون->varIdxMap.serialize_serialize (c->serializer, اشاريو_map_plan));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &ابتو_glyph_map = *c->plan->ابتو_glyph_map;
    ثابت hb_سيٽ_t& glyphset = c->plan->_glyphset_colred;

    خودڪار بنياد_it =
    + hb_range (c->plan->انگ_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = ابتو_glyph_map.get (new_gid);
		    إذا (glyphset.has (old_gid)) أرجع سچ;
		    أرجع خطأ;
		 })
    | hb_map_retains_ترتيب_ڏيڻ ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = ابتو_glyph_map.get (new_gid);

				ثابت بنياد_Glyph_رڪارڊ* old_رڪارڊ = get_بنياد_glyph_رڪارڊ (old_gid);
				إذا (unlikely (!old_رڪارڊ))
				  أرجع hb_pair_t<منطقي, بنياد_Glyph_رڪارڊ> (خطأ, Null (بنياد_Glyph_رڪارڊ));
				بنياد_Glyph_رڪارڊ new_رڪارڊ = {};
				new_رڪارڊ.glyphId = new_gid;
				new_رڪارڊ.انگ_Layers = old_رڪارڊ->انگ_Layers;
				أرجع hb_pair_t<منطقي, بنياد_Glyph_رڪارڊ> (سچ, new_رڪارڊ);
			      })
    | hb_filter (hb_پهريون)
    | hb_map_retains_ترتيب_ڏيڻ (hb_ٻيو)
    ;

    خودڪار layer_it =
    + hb_range (c->plan->انگ_output_glyphs ())
    | hb_map (ابتو_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_ترتيب_ڏيڻ ([&](hb_codepoint_t old_gid)
			      {
				ثابت بنياد_Glyph_رڪارڊ* old_رڪارڊ = get_بنياد_glyph_رڪارڊ (old_gid);
				hb_ويڪٽر_t<Layer_رڪارڊ> ٻاهريون_layers;

				إذا (unlikely (!old_رڪارڊ ||
					      old_رڪارڊ->پهريون_Layer_Idx >= انگ_Layers ||
					      old_رڪارڊ->پهريون_Layer_Idx + old_رڪارڊ->انگ_Layers > انگ_Layers))
				  أرجع hb_pair_t<منطقي, hb_ويڪٽر_t<Layer_رڪارڊ>> (خطأ, ٻاهريون_layers);

				خودڪار layers = (هذا+layersZ).as_array (انگ_Layers).sub_array (old_رڪارڊ->پهريون_Layer_Idx,
											     old_رڪارڊ->انگ_Layers);
				ٻاهريون_layers.resize (layers.ڊيگهائي);
				لكل (بنانشان صحيح i = 0; i < layers.ڊيگهائي; i++) {
				  ٻاهريون_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  إذا (unlikely (!c->plan->new_gid_for_old_gid (ٻاهريون_layers[i].glyphId, &new_gid)))
				    أرجع hb_pair_t<منطقي, hb_ويڪٽر_t<Layer_رڪارڊ>> (خطأ, ٻاهريون_layers);
				  ٻاهريون_layers[i].glyphId = new_gid;
				  ٻاهريون_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				أرجع hb_pair_t<منطقي, hb_ويڪٽر_t<Layer_رڪارڊ>> (سچ, ٻاهريون_layers);
			      })
    | hb_filter (hb_پهريون)
    | hb_map_retains_ترتيب_ڏيڻ (hb_ٻيو)
    ;

    إذا (نسخو == 0 && (!بنياد_it || !layer_it))
      return_trace (خطأ);

    خودڪار *colr_مفرد = c->serializer->شروعات_embed<COLR> ();
    إذا (unlikely (!c->serializer->extend_min (colr_مفرد)))  return_trace (خطأ);

    إذا (نسخو == 0 || downgrade_منزل_V0 (glyphset))
      return_trace (colr_مفرد->serialize_V0 (c->serializer, 0, بنياد_it, layer_it));

    hb_barrier ();

    //start version 1
    إذا (!c->serializer->allocate_سائيز<فراغ> (5 * HBUINT32::static_سائيز)) return_trace (خطأ);
    إذا (!colr_مفرد->serialize_V0 (c->serializer, نسخو, بنياد_it, layer_it)) return_trace (خطأ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    إذا (!subset_varstore (c, colr_مفرد)) return_trace (خطأ);

    شي_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : خالي_اشارو,
				     c->plan->normalized_coords ? get_delta_سيٽ_اشاريو_map_ptr () : خالي_اشارو,
				     c->plan->normalized_coords.as_array ());

    إذا (!colr_مفرد->بنياد_Glyph_List_2.serialize_subset (c, بنياد_Glyph_List_2, هذا, instancer))
      return_trace (خطأ);

    colr_مفرد->layerList.serialize_subset (c, layerList, هذا, instancer);
    colr_مفرد->clipList.serialize_subset (c, clipList, هذا, instancer);

    return_trace (subset_delta_سيٽ_اشاريو_map (c, colr_مفرد));
  }

  ثابت Paint *get_بنياد_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت بنياد_Glyph_List &baseglyph_paintrecords = هذا+بنياد_Glyph_List_2;
    ثابت بنياد_Glyph_Paint_رڪارڊ* رڪارڊ = get_بنياد_glyph_paintrecord (glyph);
    إذا (رڪارڊ)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+رڪارڊ->paint;
      أرجع &paint;
    }
    وإلا
      أرجع خالي_اشارو;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT
  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    شي_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_سيٽ_اشاريو_map_ptr (),
                                     hb_array (font->coords, font->انگ_coords));

    إذا (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      أرجع سچ;
    }

    خودڪار *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقي ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), سچ, scratch);

    خودڪار e = scratch.paint_extents.get_extents ();
    إذا (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->ويڪر = 0;
      extents->اوچائي = 0;
    }
    وإلا
      *extents = e.منزل_glyph_extents ();

    أرجع ret;
  }
#جيڪڏهن_پورو

  منطقي
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    إذا (نسخو >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنياد_glyph_paint (glyph);

      أرجع paint != خالي_اشارو;
    }

    أرجع خطأ;
  }

  منطقي get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت شي_Var_Store_Instancer instancer) ثابت
  {
    أرجع get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT
  منطقي
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, فراغ *معلومات,
	       بنانشان صحيح palette_اشاريو, hb_color_t foreground,
	       منطقي clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    شي_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_سيٽ_اشاريو_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->انگ_coords : 0));
    hb_paint_context_t c (هذا, funcs, معلومات, font, palette_اشاريو, foreground, instancer);

    hb_decycler_ڳنڍ_t node (c.glyphs_decycler);
    node.گهمو (glyph);

    إذا (نسخو >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_بنياد_glyph_paint (glyph);
      إذا (paint)
      {
        // COLRv1 glyph

	منطقي is_bounded = سچ;
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
	    خودڪار *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_اشاريو, foreground,
			 خطأ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ڌڪو_font_transform (c.معلومات, font);

	إذا (clip)
	  c.funcs->ڌڪو_clip_rectangle (c.معلومات,
					(عائم) extents.x_bearing,
					(عائم) extents.y_bearing + extents.اوچائي,
					(عائم) extents.x_bearing + extents.ويڪر,
					(عائم) extents.y_bearing);

	إذا (is_bounded)
	  c.recurse (*paint);

	إذا (clip)
	  c.funcs->ڪڍو_clip (c.معلومات);

	c.funcs->ڪڍو_transform (c.معلومات);

        أرجع سچ;
      }
    }

    ثابت بنياد_Glyph_رڪارڊ *رڪارڊ = get_بنياد_glyph_رڪارڊ (glyph);
    إذا (رڪارڊ && ((hb_codepoint_t) رڪارڊ->glyphId == glyph))
    {
      // COLRv0 glyph
      لكل (ثابت خودڪار &r : (هذا+layersZ).as_array (انگ_Layers)
			   .sub_array (رڪارڊ->پهريون_Layer_Idx, رڪارڊ->انگ_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.معلومات, r.glyphId, c.font, is_foreground, color);
      }

      أرجع سچ;
    }

    أرجع خطأ;
  }
#جيڪڏهن_پورو

  محمي:
  HBUINT16	نسخو;	/* Table version number (starts at 0). */
  HBUINT16	انگ_بنياد_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_منزل<Sorted_Unsized_Array_جو<بنياد_Glyph_رڪارڊ>>
		بنياد_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_منزل<Unsized_Array_جو<Layer_رڪارڊ>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	انگ_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_منزل<بنياد_Glyph_List>		بنياد_Glyph_List_2;
  Offset32_منزل<LayerList>			layerList;
  Offset32_منزل<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_منزل<Delta_سيٽ_اشاريو_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_منزل<شي_Variation_Store>	varStore;
  عام:
  DEFINE_سائيز_MIN (14);
};

بنية COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

فراغ
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  إذا (unlikely (اونهائي_کاٻو <= 0 || ڪنارو_ڳڻپ <= 0)) أرجع;
  اونهائي_کاٻو--;
  ڪنارو_ڳڻپ--;
  paint.dispatch (هذا);
  اونهائي_کاٻو++;
}

سانچو <قسم_نالو Types>
فراغ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  إذا (likely (c->اونهائي_کاٻو > 0 && c->ڪنارو_ڳڻپ > 0) &&
      (هذا+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->ڪنارو_ڳڻپ--;
    c->funcs->ڌڪو_inverse_font_transform (c->معلومات, c->font);
    c->funcs->fill_glyph (c->معلومات, gid, c->font, is_foreground, color);
    c->funcs->ڪڍو_transform (c->معلومات);
    أرجع;
  }

  c->funcs->ڌڪو_inverse_font_transform (c->معلومات, c->font);
  c->funcs->ڌڪو_clip_glyph (c->معلومات, gid, c->font);
  c->funcs->ڌڪو_font_transform (c->معلومات, c->font);
  c->recurse (هذا+paint);
  c->funcs->ڪڍو_transform (c->معلومات);
  c->funcs->ڪڍو_clip (c->معلومات);
  c->funcs->ڪڍو_transform (c->معلومات);
}

فراغ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);
  ثابت LayerList &paint_وٿي_lists = c->get_colr_جدول ()->get_layerList ();
  hb_decycler_ڳنڍ_t node (c->layers_decycler);
  لكل (بنانشان i = پهريون_Layer_اشاريو; i < پهريون_Layer_اشاريو + انگ_Layers; i++)
  {
    إذا (unlikely (!node.گهمو (i)))
      أرجع;

    ثابت Paint &paint = paint_وٿي_lists.get_paint (i);
    c->recurse (paint);
  }
}

فراغ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_decycler_ڳنڍ_t node (c->glyphs_decycler);
  إذا (unlikely (!node.گهمو (gid)))
    أرجع;

  c->funcs->ڌڪو_inverse_font_transform (c->معلومات, c->font);
  إذا (c->funcs->color_glyph (c->معلومات, gid, c->font))
  {
    c->funcs->ڪڍو_transform (c->معلومات);
    أرجع;
  }
  c->funcs->ڪڍو_transform (c->معلومات);

  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت Paint *paint = colr_جدول->get_بنياد_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقي has_clip_box = colr_جدول->get_clip (gid, &extents, c->instancer);

  إذا (has_clip_box)
    c->funcs->ڌڪو_clip_rectangle (c->معلومات,
				   (عائم) extents.x_bearing,
				   (عائم) extents.y_bearing + extents.اوچائي,
				   (عائم) extents.x_bearing + extents.ويڪر,
				   (عائم) extents.y_bearing);

  إذا (paint)
    c->recurse (*paint);

  إذا (has_clip_box)
    c->funcs->ڪڍو_clip (c->معلومات);
}

} /* namespace OT */

#جيڪڏهن_پورو /* OT_COLOR_COLR_COLR_HH */
