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

#جے_بنا_تعریف OT_COLOR_COLR_COLR_HH
#تعریف_کرو OT_COLOR_COLR_COLR_HH

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
#تعریف_کرو HB_OT_ٹیگ_COLR HB_ٹیگ('C','O','L','R')

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
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (ادھار); }

عام:
  ثابت فراغ *ادھار;
  hb_paint_funcs_t *funcs;
  فراغ *انکڑے;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  شے_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح ڈونگھائی_بایاں = HB_MAX_NESTING_LEVEL;
  صحيح کنارا_گنتی = HB_MAX_گراف_کنارا_گنتی;

  hb_paint_context_t (ثابت فراغ *ادھار_2,
		      hb_paint_funcs_t *funcs_,
		      فراغ *انکڑے_2,
                      hb_font_t *font_,
                      بنانشانی صحيح palette_,
                      hb_color_t foreground_,
		      شے_Var_Store_Instancer &instancer_) :
    ادھار (ادھار_2),
    funcs (funcs_),
    انکڑے (انکڑے_2),
    font (font_),
    palette (
#جے_بنا_تعریف HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->جدول.CPAL->get_palette_colors (palette_ < font->face->جدول.CPAL->get_palette_گنتی () ? palette_ : 0)
#جے_انت
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_سیٹ_synthetic_bold (font, 0, 0, سچ);
      hb_font_سیٹ_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساكن بنانشانی
  color_alpha (hb_color_t color, عائم alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    أرجع roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (بنانشانی صحيح color_اشاریہ, عائم alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = سچ;

    إذا (color_اشاریہ != 0xffff)
    {
      إذا (!funcs->custom_palette_color (انکڑے, color_اشاریہ, &color))
        color = palette[color_اشاریہ];

      *is_foreground = خطأ;
    }

    أرجع HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  مضمن فراغ recurse (ثابت Paint &paint);
};

بنية hb_colrv1_گھیرا_context_t :
       hb_dispatch_context_t<hb_colrv1_گھیرا_context_t>
{
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_بایاں == 0))
      أرجع hb_خالی_تھاواں_t ();

    إذا (paint_ویکھیا (&obj))
      أرجع hb_خالی_تھاواں_t ();

    nesting_level_بایاں--;
    obj.closurev1 (هذا);
    nesting_level_بایاں++;
    أرجع hb_خالی_تھاواں_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  منطقي paint_ویکھیا (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) ادھار);
    إذا (ویکھیا_paint.وچ_error() || ویکھیا_paint.has (delta))
      أرجع سچ;

    ویکھیا_paint.جوڑو (delta);
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (ادھار); }

  فراغ جوڑو_glyph (بنانشانی glyph_id)
  { glyphs->جوڑو (glyph_id); }

  فراغ جوڑو_layer_indices (بنانشانی پہلا_layer_اشاریہ, بنانشانی عدد_دا_layers)
  {
    إذا (عدد_دا_layers == 0) أرجع;
    layer_indices->جوڑو_range (پہلا_layer_اشاریہ, پہلا_layer_اشاریہ + عدد_دا_layers - 1);
  }

  فراغ جوڑو_palette_اشاریہ (بنانشانی palette_اشاریہ)
  { palette_indices->جوڑو (palette_اشاریہ); }

  فراغ جوڑو_var_idxes (بنانشانی پہلا_var_idx, بنانشانی عدد_idxes)
  {
    إذا (!عدد_idxes || پہلا_var_idx == VarIdx::NO_VARIATION) أرجع;
    variation_indices->جوڑو_range (پہلا_var_idx, پہلا_var_idx + عدد_idxes - 1);
  }

  عام:
  ثابت فراغ *ادھار;
  hb_سیٹ_t ویکھیا_paint;
  hb_سیٹ_t *glyphs;
  hb_سیٹ_t *layer_indices;
  hb_سیٹ_t *palette_indices;
  hb_سیٹ_t *variation_indices;
  بنانشانی عدد_var_idxes;
  بنانشانی nesting_level_بایاں;

  hb_colrv1_گھیرا_context_t (ثابت فراغ *ادھار_2,
                               hb_سیٹ_t *glyphs_,
                               hb_سیٹ_t *layer_indices_,
                               hb_سیٹ_t *palette_indices_,
                               hb_سیٹ_t *variation_indices_,
                               بنانشانی عدد_var_idxes_2 = 1,
                               بنانشانی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
                          ادھار (ادھار_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          عدد_var_idxes (عدد_var_idxes_2),
                          nesting_level_بایاں (nesting_level_بایاں_2)
  {}
};


بنية hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_بایاں == 0))
      أرجع hb_خالی_تھاواں_t ();

    إذا (paint_ویکھیا (&obj))
      أرجع hb_خالی_تھاواں_t ();

    nesting_level_بایاں--;
    obj.dependv1 (هذا);
    nesting_level_بایاں++;
    أرجع hb_خالی_تھاواں_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  منطقي paint_ویکھیا (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    إذا (ویکھیا_paint.وچ_error() || ویکھیا_paint.has (delta))
      أرجع سچ;

    ویکھیا_paint.جوڑو (delta);
    إذا (unlikely (ویکھیا_paint.وچ_error ()))
      depend_انکڑے->fail ();
    أرجع خطأ;
  }

  ثابت COLR* get_colr_جدول () ثابت
  { أرجع colr; }

  عام:
  ثابت COLR *colr;
  hb_depend_انکڑے_builder_t *depend_انکڑے;
  hb_سیٹ_t ویکھیا_paint;
  بنانشانی nesting_level_بایاں;
  hb_codepoint_t سروت_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_انکڑے_builder_t *depend_انکڑے_2,
                              بنانشانی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_انکڑے (depend_انکڑے_2),
                          nesting_level_بایاں (nesting_level_بایاں_2),
                          سروت_gid (0)
  {}
};

بنية Layer_ریکارڈ
{
  عامل hb_ot_color_layer_t () ثابت { أرجع {glyphId, colorIdx}; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
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
  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية ادھار_Glyph_ریکارڈ
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
  HBUINT16	پہلا_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	عدد_Layers;	/* Number of color layers
				 * associated with this glyph */
  عام:
  DEFINE_ناپ_STATIC (6);
};

سانچا <قسم_ناں T>
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

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    c->عدد_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->جوڑو_var_idxes (var_Idx_ادھار, c->عدد_var_idxes);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!value.subset (c, instancer, var_Idx_ادھار)) return_trace (خطأ);
    إذا (c->plan->all_axes_pinned)
      return_trace (سچ);

    VarIdx new_varidx;
    new_varidx = var_Idx_ادھار;
    إذا (var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      hb_pair_t<بنانشانی, صحيح> *new_varidx_delta;
      إذا (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_ادھار, &new_varidx_delta))
        return_trace (خطأ);

      new_varidx = hb_پہلا (*new_varidx_delta);
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
    value.paint_glyph (c, var_Idx_ادھار);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, var_Idx_ادھار, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, var_Idx_ادھار, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  محمي:
  T      value;
  عام:
  VarIdx var_Idx_ادھار;
  عام:
  DEFINE_ناپ_MIN (VarIdx::static_ناپ + T::min_ناپ);
};

سانچا <قسم_ناں T>
بنية NoVariable
{
  ساكن constexpr منطقي is_variable = خطأ;

  ثابت T& get_قدر () ثابت { أرجع value; }

  ساكن constexpr uint32_t var_Idx_ادھار = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (value.subset (c, instancer, var_Idx_ادھار));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, var_Idx_ادھار);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
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
  DEFINE_ناپ_MIN (T::min_ناپ);
};

// Color structures

سانچا <قسم_ناں Type, بنانشانی fraction_بٹ>
ساكن فراغ
سیٹ_fixed_clamped (HBFixed<Type, fraction_بٹ> &باہرلا, عائم value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  باہرلا.سیٹ_int (hb_clamp_منزل<قسم_ناں Type::type> (roundf ((value / HBFixed<Type, fraction_بٹ>::mult))));
}

بنية ColorStop
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    c->جوڑو_palette_اشاریہ (palette_اشاریہ_2);
    c->عدد_var_idxes = 2;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->stop_وتھ, stop_وتھ.منزل_float(instancer (var_Idx_ادھار, 0)));
      عائم a = alpha.منزل_float (instancer (var_Idx_ادھار, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      سیٹ_fixed_clamped (باہرلا->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_سونپو (باہرلا->palette_اشاریہ_2, c->plan->colr_palettes.get (palette_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *باہرلا,
		       uint32_t varIdx,
		       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    باہرلا->وتھ = stop_وتھ.منزل_float(instancer (varIdx, 0));
    باہرلا->color = c->get_color (palette_اشاریہ_2,
                               alpha.منزل_float (instancer (varIdx, 1)),
                               &باہرلا->is_foreground);
  }

  F2DOT14	stop_وتھ;
  HBUINT16	palette_اشاریہ_2;
  F2DOT14	alpha;
  عام:
  DEFINE_ناپ_STATIC (2 + 2 * F2DOT14::static_ناپ);
};

بنية Extend : HBUINT8
{
  تعداد {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عام:
  DEFINE_ناپ_STATIC (1);
};

سانچا <سانچا<قسم_ناں> صنف Var>
بنية Color_سطر
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    لكل (ثابت خودکار &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    إذا (!c->serializer->check_سونپو (باہرلا->extend, extend, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) return_trace (خطأ);
    إذا (!c->serializer->check_سونپو (باہرلا->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ)) return_trace (خطأ);

    لكل (ثابت خودکار& stop : stops.iter ())
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
  بنانشانی صحيح
  get_color_stops (hb_paint_context_t *c,
                   بنانشانی صحيح شروع,
		   بنانشانی صحيح *گنتی,
		   hb_color_stop_t *color_stops,
		   ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    بنانشانی صحيح len = stops.len;

    إذا (گنتی && color_stops)
    {
      بنانشانی صحيح i;
      لكل (i = 0; i < *گنتی && شروع + i < len; i++)
        stops[شروع + i].get_color_stop (c, &color_stops[i], instancer);
      *گنتی = i;
    }

    أرجع len;
  }

  HB_اندرونی ساكن بنانشانی صحيح static_get_color_stops (hb_color_سطر_t *color_سطر,
							  فراغ *color_سطر_انکڑے,
							  بنانشانی صحيح شروع,
							  بنانشانی صحيح *گنتی,
							  hb_color_stop_t *color_stops,
							  فراغ *user_انکڑے)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_انکڑے;
    hb_paint_context_t *c = (hb_paint_context_t *) user_انکڑے;
    أرجع thiz->get_color_stops (c, شروع, گنتی, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بدل ((بنانشانی صحيح) extend)
    {
      حالة Extend::EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
      حالة Extend::EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
      حالة Extend::EXTEND_PAD:
      افتراضي:                     أرجع HB_PAINT_EXTEND_PAD;
    }
  }

  HB_اندرونی ساكن hb_paint_extend_t static_get_extend (hb_color_سطر_t *color_سطر,
							  فراغ *color_سطر_انکڑے,
							  فراغ *user_انکڑے)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_انکڑے;
    أرجع thiz->get_extend ();
  }

  Extend	extend;
  Array16_دا<Var<ColorStop>>	stops;
  عام:
  DEFINE_ناپ_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
بنية مرکب_منوال : HBUINT8
{
  عام:
  DEFINE_ناپ_STATIC (1);
};

بنية Affine2x3
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  { c->عدد_var_idxes = 6; }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      خودکار fold = [&] (F16DOT16 &target, F16DOT16 سروت, بنانشانی وتھ) {
        // Keep the stored integer precise while applying the float delta.
        target.سیٹ_int (hb_clamp_منزل<int32_t> (roundf ((مزدوج) سروت.منزل_int () +
                                                    (مزدوج) instancer (var_Idx_ادھار, وتھ))));
      };
      fold (باہرلا->xx, xx, 0);
      fold (باہرلا->yx, yx, 1);
      fold (باہرلا->xy, xy, 2);
      fold (باہرلا->yy, yy, 3);
      fold (باہرلا->dx, dx, 4);
      fold (باہرلا->dy, dy, 5);
    }
    return_trace (سچ);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    c->funcs->ڈالو_transform (c->انکڑے,
			      xx.منزل_float (c->instancer (var_Idx_ادھار, 0)),
			      yx.منزل_float (c->instancer (var_Idx_ادھار, 1)),
                              xy.منزل_float (c->instancer (var_Idx_ادھار, 2)),
			      yy.منزل_float (c->instancer (var_Idx_ادھار, 3)),
                              dx.منزل_float (c->instancer (var_Idx_ادھار, 4)),
			      dy.منزل_float (c->instancer (var_Idx_ادھار, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  عام:
  DEFINE_ناپ_STATIC (6 * F16DOT16::static_ناپ);
};

بنية PaintColrLayers
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer HB_بن_ورتیا) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    uint32_t پہلا_layer_اشاریہ = عدد_Layers ? c->plan->colrv1_layers.get (پہلا_Layer_اشاریہ) : 0;
    return_trace (c->serializer->check_سونپو (باہرلا->پہلا_Layer_اشاریہ, پہلا_layer_اشاریہ,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	عدد_Layers;
  HBUINT32	پہلا_Layer_اشاریہ;  /* index into COLRv1::layerList */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية PaintSolid
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    c->جوڑو_palette_اشاریہ (palette_اشاریہ_2);
    c->عدد_var_idxes = 1;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      عائم a = alpha.منزل_float (instancer (var_Idx_ادھار, 0));
      سیٹ_fixed_clamped (باہرلا->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    إذا (format == 3 && c->plan->all_axes_pinned)
        باہرلا->format = 2;

    return_trace (c->serializer->check_سونپو (باہرلا->palette_اشاریہ_2, c->plan->colr_palettes.get (palette_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_ادھار,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (palette_اشاریہ_2,
                           alpha.منزل_float (c->instancer (var_Idx_ادھار, 0)),
                           is_foreground);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_ادھار, &is_foreground, &color);
    c->funcs->color (c->انکڑے, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_اشاریہ_2;
  F2DOT14	alpha;
  عام:
  DEFINE_ناپ_STATIC (3 + F2DOT14::static_ناپ);
};

سانچا <سانچا<قسم_ناں> صنف Var>
بنية Paint_خطی_Gradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      باہرلا->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 0)));
      باہرلا->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
      باہرلا->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
      باہرلا->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 3)));
      باہرلا->x2 = hb_clamp_منزل<int16_t> ((مزدوج) x2 + (مزدوج) roundf (instancer (var_Idx_ادھار, 4)));
      باہرلا->y2 = hb_clamp_منزل<int16_t> ((مزدوج) y2 + (مزدوج) roundf (instancer (var_Idx_ادھار, 5)));
    }

    إذا (format == 5 && c->plan->all_axes_pinned)
        باہرلا->format = 4;

    return_trace (باہرلا->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, خالی_سوچک
    };

    c->funcs->خطی_gradient (c->انکڑے, &cl,
			       x0 + c->instancer (var_Idx_ادھار, 0),
			       y0 + c->instancer (var_Idx_ادھار, 1),
			       x1 + c->instancer (var_Idx_ادھار, 2),
			       y1 + c->instancer (var_Idx_ادھار, 3),
			       x2 + c->instancer (var_Idx_ادھار, 4),
			       y2 + c->instancer (var_Idx_ادھار, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_منزل<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  عام:
  DEFINE_ناپ_STATIC (4 + 6 * FWORD::static_ناپ);
};

سانچا <سانچا<قسم_ناں> صنف Var>
بنية PaintRadialGradient
{
  منطقي constant_after_pinning (hb_subset_context_t *c,
                               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    إذا (c->plan->has_avar2) أرجع خطأ;
    ثابت خودکار &regions = instancer.varStore->get_region_list ();
    لكل (بنانشانی i = 0; i < regions.region_گنتی; i++)
    {
      hb_hashmap_t<hb_ٹیگ_t, Triple> axes;
      إذا (!regions.get_var_region (i, c->plan->axes_old_اشاریہ_ٹیگ_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_باہرلا_دا_گنجائش);
        أرجع خطأ;
      }
      لكل (خودکار axis : axes)
      {
        Triple *حد_بندی;
        إذا (!c->plan->user_axes_location.has (axis.پہلا, &حد_بندی) ||
            حد_بندی->minimum != حد_بندی->ودھ_توں_ودھ)
          أرجع خطأ;
      }
    }
    أرجع سچ;
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 6;
  }

  منطقي subset_nonnegative (hb_subset_context_t *c,
                           ثابت شے_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_ادھار) ثابت
  {
    مزدوج سروت[6] = {
      x0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 0)),
      y0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)),
      radius0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)),
      x1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 3)),
      y1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 4)),
      radius1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 5))
    };
    خودکار transparent = [&] {
      خودکار *باہرلا = c->serializer->allocate_ناپ<PaintSolid> (5);
      إذا (!باہرلا) أرجع خطأ;
      باہرلا->format = 2; باہرلا->palette_اشاریہ_2 = 0xFFFF; باہرلا->alpha.سیٹ_float (0);
      أرجع سچ;
    };
    ثابت خودکار &سطر = هذا+color_سطر_2;
    إذا (سروت[2] == سروت[5] || !سطر.stops.len) أرجع transparent ();
    بنية stop_t { مزدوج وتھ, alpha; بنانشانی palette, ترتیب; };
    hb_سمتیہ_t<stop_t> stops;
    إذا (!stops.alloc (سطر.stops.len)) أرجع خطأ;
    بنانشانی ترتیب = 0;
    لكل (ثابت خودکار &اندراج : سطر.stops.iter ())
    {
      ثابت خودکار &stop = اندراج.get_قدر ();
      stops.push (stop_t {(مزدوج) roundf (stop.stop_وتھ.منزل_float (instancer (اندراج.var_Idx_ادھار, 0)) * 16384.f) / 16384.,
                   (مزدوج) hb_clamp (stop.alpha.منزل_float (instancer (اندراج.var_Idx_ادھار, 1)), 0.f, 1.f),
                   stop.palette_اشاریہ_2, ترتیب++});
    }
    إذا (stops.وچ_error ()) أرجع خطأ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      أرجع a.وتھ < b.وتھ ? -1 : a.وتھ > b.وتھ ? 1 :
             a.ترتیب < b.ترتیب ? -1 : a.ترتیب > b.ترتیب ? 1 : 0;
    });
    مزدوج dr = سروت[5] - سروت[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    بنانشانی lattice = 16384;
    لكل (بنانشانی j = 0; j < 3; j++)
    {
      مزدوج remainder = fmod (fabs (سروت[j + 3] - سروت[j]), 16384.);
      إذا (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
        أرجع خطأ;
      }
      بنانشانی سجے = (بنانشانی) remainder;
      طالما (سجے) { بنانشانی r = lattice % سجے; lattice = سجے; سجے = r; }
    }
    لكل (بنانشانی کھسکاؤ = 0; کھسکاؤ < 16; کھسکاؤ++)
    {
      مزدوج b = 1u << کھسکاؤ;
      مزدوج coarse = dr > 0 ? ceil (-سروت[2] / dr) : floor (سروت[2] / -dr) - b;
      مزدوج fine = dr > 0 ? ceil (-سروت[2] / dr * lattice) / lattice :
                            floor (سروت[2] / -dr * lattice) / lattice - b;
      مزدوج candidates[] = {coarse, fine};
      لكل (مزدوج a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        مزدوج geometry[6];
        منطقي سماندا = سچ;
        لكل (بنانشانی j = 0; j < 3; j++)
        {
          مزدوج difference = سروت[j + 3] - سروت[j];
          geometry[j] = سروت[j] + a * difference;
          geometry[j + 3] = سروت[j] + (a + b) * difference;
          مزدوج کم = j == 2 ? 0 : -32768, بلند = j == 2 ? 65535 : 32767;
          سماندا &= geometry[j] >= کم && geometry[j] <= بلند &&
                  geometry[j + 3] >= کم && geometry[j + 3] <= بلند;
        }
        إذا (!سماندا) استمر;
        بنانشانی پہلا = 0, گنتی = stops.لمائی;
        مزدوج min = (stops[0].وتھ - a) / b, max = (stops.پوچھ ().وتھ - a) / b;
        مزدوج bias = 0;
        منطقي constant = خطأ;
        إذا (سطر.extend == Extend::EXTEND_PAD)
        {
          مزدوج tip = -geometry[2] / (b * dr);
          إذا ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            پہلا = dr > 0 ? stops.لمائی - 1 : 0;
            گنتی = 1; constant = سچ;
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
        لكل (بنانشانی j = 0; j < گنتی; j++)
        {
          مزدوج بٹ = constant ? 0 : ((stops[پہلا + j].وتھ - a) / b + bias) * 16384;
          سماندا &= بٹ >= -32768 && بٹ <= 32767 && بٹ == round (بٹ);
        }
        إذا (!سماندا) استمر;
        خودکار *باہرلا = c->serializer->allocate_ناپ<PaintRadialGradient<NoVariable>> (16);
        إذا (!باہرلا) أرجع خطأ;
        باہرلا->format = 6;
        باہرلا->x0 = (صحيح) geometry[0]; باہرلا->y0 = (صحيح) geometry[1]; باہرلا->radius0 = (بنانشانی) geometry[2];
        باہرلا->x1 = (صحيح) geometry[3]; باہرلا->y1 = (صحيح) geometry[4]; باہرلا->radius1 = (بنانشانی) geometry[5];
        c->serializer->push ();
        خودکار *باہرلا_سطر = c->serializer->allocate_ناپ<Color_سطر<NoVariable>> (3 + گنتی * ColorStop::static_ناپ);
        إذا (!باہرلا_سطر) { c->serializer->نکالو_discard (); أرجع خطأ; }
        باہرلا_سطر->extend = سطر.extend;
        باہرلا_سطر->stops.len = گنتی;
        لكل (بنانشانی j = 0; j < گنتی; j++)
        {
          ثابت خودکار &stop = stops[پہلا + j];
          خودکار &باہرلا_stop = باہرلا_سطر->stops[j].value;
          باہرلا_stop.stop_وتھ.سیٹ_float ((عائم) (constant ? 0 : (stop.وتھ - a) / b + bias));
          باہرلا_stop.alpha.سیٹ_float ((عائم) stop.alpha);
          إذا (!c->serializer->check_سونپو (باہرلا_stop.palette_اشاریہ_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
          { c->serializer->نکالو_discard (); أرجع خطأ; }
        }
        c->serializer->جوڑو_link (باہرلا->color_سطر_2, c->serializer->نکالو_pack ());
        أرجع !c->serializer->وچ_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_ادھار != VarIdx::NO_VARIATION &&
        (radius0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)) < 0 ||
         radius1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_ادھار));
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      باہرلا->x0 = hb_clamp_منزل<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 0)));
      باہرلا->y0 = hb_clamp_منزل<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
      باہرلا->radius0 = hb_clamp_منزل<uint16_t> ((مزدوج) radius0 + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
      باہرلا->x1 = hb_clamp_منزل<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 3)));
      باہرلا->y1 = hb_clamp_منزل<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 4)));
      باہرلا->radius1 = hb_clamp_منزل<uint16_t> ((مزدوج) radius1 + (مزدوج) roundf (instancer (var_Idx_ادھار, 5)));
    }

    إذا (format == 7 && c->plan->all_axes_pinned)
        باہرلا->format = 6;

    return_trace (باہرلا->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, خالی_سوچک
    };

    c->funcs->radial_gradient (c->انکڑے, &cl,
			       x0 + c->instancer (var_Idx_ادھار, 0),
			       y0 + c->instancer (var_Idx_ادھار, 1),
			       radius0 + c->instancer (var_Idx_ادھار, 2),
			       x1 + c->instancer (var_Idx_ادھار, 3),
			       y1 + c->instancer (var_Idx_ادھار, 4),
			       radius1 + c->instancer (var_Idx_ادھار, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_منزل<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  عام:
  DEFINE_ناپ_STATIC (4 + 6 * FWORD::static_ناپ);
};

سانچا <سانچا<قسم_ناں> صنف Var>
بنية PaintSweepGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    (هذا+color_سطر_2).closurev1 (c);
    c->عدد_var_idxes = 4;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      باہرلا->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_ادھار, 0)));
      باہرلا->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
      سیٹ_fixed_clamped (باہرلا->شروع_Angle, شروع_Angle.منزل_float (instancer (var_Idx_ادھار, 2)));
      سیٹ_fixed_clamped (باہرلا->سرا_Angle, سرا_Angle.منزل_float (instancer (var_Idx_ادھار, 3)));
    }

    إذا (format == 9 && c->plan->all_axes_pinned)
        باہرلا->format = 8;

    return_trace (باہرلا->color_سطر_2.serialize_subset (c, color_سطر_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_سطر_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_سطر_t cl = {
      (فراغ *) &(هذا+color_سطر_2),
      (هذا+color_سطر_2).static_get_color_stops, c,
      (هذا+color_سطر_2).static_get_extend, خالی_سوچک
    };

    c->funcs->sweep_gradient (c->انکڑے, &cl,
			      centerX + c->instancer (var_Idx_ادھار, 0),
			      centerY + c->instancer (var_Idx_ادھار, 1),
                              (شروع_Angle.منزل_float (c->instancer (var_Idx_ادھار, 2)) + 1) * HB_PI,
                              (سرا_Angle.منزل_float   (c->instancer (var_Idx_ادھار, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_منزل<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		شروع_Angle;
  F2DOT14		سرا_Angle;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * FWORD::static_ناپ + 2 * F2DOT14::static_ناپ);
};

// Paint a non-COLR glyph, filled as indicated by paint.
سانچا <قسم_ناں Types>
بنية PaintGlyph
{
  ورتوں HBGlyphID = قسم_ناں Types::HBGlyphID;

  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (! c->serializer->check_سونپو (باہرلا->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    return_trace (باہرلا->paint.serialize_subset (c, paint, هذا, instancer));
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
  DEFINE_ناپ_STATIC (4 + HBGlyphID::static_ناپ);
};

بنية PaintColrGlyph
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer HB_بن_ورتیا) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    return_trace (c->serializer->check_سونپو (باہرلا->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
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
  DEFINE_ناپ_STATIC (3);
};

سانچا <سانچا<قسم_ناں> صنف Var>
بنية PaintTransform
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    إذا (!باہرلا->transform.serialize_subset (c, transform, هذا, instancer)) return_trace (خطأ);
    إذا (format == 13 && c->plan->all_axes_pinned)
      باہرلا->format = 12;
    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
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
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_منزل<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_منزل<Var<Affine2x3>>	transform;
  عام:
  DEFINE_ناپ_STATIC (7);
};

بنية PaintTranslate
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      باہرلا->dx = hb_clamp_منزل<int16_t> ((مزدوج) dx + (مزدوج) roundf (instancer (var_Idx_ادھار, 0)));
      باہرلا->dy = hb_clamp_منزل<int16_t> ((مزدوج) dy + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
    }

    إذا (format == 15 && c->plan->all_axes_pinned)
        باہرلا->format = 14;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم ddx = dx + c->instancer (var_Idx_ادھار, 0);
    عائم ddy = dy + c->instancer (var_Idx_ادھار, 1);

    c->funcs->ڈالو_translate (c->انکڑے, ddx, ddy);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * FWORD::static_ناپ);
};

سانچا <قسم_ناں سروت_2>
ساكن منطقي serialize_scale_transform (hb_subset_context_t *c,
                                       عائم sx, عائم sy, عائم dx, عائم dy,
                                       ثابت سروت_2 &سروت)
{
  خودکار *باہرلا = c->serializer->allocate_ناپ<PaintTransform<NoVariable>> (7);
  إذا (unlikely (!باہرلا)) أرجع خطأ;
  باہرلا->format = 12;

  c->serializer->push ();
  خودکار *transform = c->serializer->allocate_ناپ<Affine2x3> (Affine2x3::static_ناپ);
  إذا (unlikely (!transform))
  {
    c->serializer->نکالو_discard ();
    أرجع خطأ;
  }
  سیٹ_fixed_clamped (transform->xx, sx);
  transform->yx.سیٹ_float (0);
  transform->xy.سیٹ_float (0);
  سیٹ_fixed_clamped (transform->yy, sy);
  سیٹ_fixed_clamped (transform->dx, dx);
  سیٹ_fixed_clamped (transform->dy, dy);
  c->serializer->جوڑو_link (باہرلا->transform, c->serializer->نکالو_pack ());

  c->serializer->push ();
  منطقي ret = سروت ();
  إذا (ret)
    c->serializer->جوڑو_link (باہرلا->src, c->serializer->نکالو_pack ());
  وإلا
    c->serializer->نکالو_discard ();
  أرجع ret;
}

سانچا <قسم_ناں ادھار_3>
ساكن منطقي subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت ادھار_3 *ادھار,
                                      ثابت Offset24_منزل<Paint> &src,
                                      ثابت شے_Var_Store_Instancer &instancer,
                                      عائم sx, عائم sy,
                                      عائم cx = 0, عائم cy = 0)
{
  إذا (src.is_null ()) أرجع خطأ;
  خودکار سروت = [&] { أرجع c->dispatch (ادھار+src, instancer); };
  عائم dx = cx * (1 - sx), dy = cy * (1 - sy);
  إذا (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    أرجع serialize_scale_transform (c, sx, sy, dx, dy, سروت);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  خودکار inner = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, سروت);
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
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_ادھار, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_ادھار, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, sx, sy));
    }
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->scaleX, scaleX.منزل_float (instancer (var_Idx_ادھار, 0)));
      سیٹ_fixed_clamped (باہرلا->scaleY, scaleY.منزل_float (instancer (var_Idx_ادھار, 1)));
    }

    إذا (format == 17 && c->plan->all_axes_pinned)
        باہرلا->format = 16;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_ادھار, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_ادھار, 1));

    c->funcs->ڈالو_scale (c->انکڑے, sx, sy);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * F2DOT14::static_ناپ);
};

بنية PaintScaleAroundCenter
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.منزل_float (instancer (var_Idx_ادھار, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.منزل_float (instancer (var_Idx_ادھار, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_ادھار, 2)),
            centerY + roundf (instancer (var_Idx_ادھار, 3))));
    }
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->scaleX, scaleX.منزل_float (instancer (var_Idx_ادھار, 0)));
      سیٹ_fixed_clamped (باہرلا->scaleY, scaleY.منزل_float (instancer (var_Idx_ادھار, 1)));
      باہرلا->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
      باہرلا->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_ادھار, 3)));
    }

    إذا (format == 19 && c->plan->all_axes_pinned)
        باہرلا->format = 18;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.منزل_float (c->instancer (var_Idx_ادھار, 0));
    عائم sy = scaleY.منزل_float (c->instancer (var_Idx_ادھار, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_ادھار, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_ادھار, 3);

    c->funcs->ڈالو_scale_around_center (c->انکڑے, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * F2DOT14::static_ناپ + 2 * FWORD::static_ناپ);
};

بنية Paint_Scale_اکسار
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_ادھار, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, s, s));
    }
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
      سیٹ_fixed_clamped (باہرلا->scale, scale.منزل_float (instancer (var_Idx_ادھار, 0)));

    إذا (format == 21 && c->plan->all_axes_pinned)
        باہرلا->format = 20;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_ادھار, 0));

    c->funcs->ڈالو_scale (c->انکڑے, s, s);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عام:
  DEFINE_ناپ_STATIC (4 + F2DOT14::static_ناپ);
};

بنية Paint_Scale_اکسار_Around_Center
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.منزل_float (instancer (var_Idx_ادھار, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_ادھار, 1)),
            centerY + roundf (instancer (var_Idx_ادھار, 2))));
    }
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->scale, scale.منزل_float (instancer (var_Idx_ادھار, 0)));
      باہرلا->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
      باہرلا->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
    }

    إذا (format == 23 && c->plan->all_axes_pinned)
        باہرلا->format = 22;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.منزل_float (c->instancer (var_Idx_ادھار, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_ادھار, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_ادھار, 2);

    c->funcs->ڈالو_scale_around_center (c->انکڑے, s, s, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_ناپ_STATIC (4 + F2DOT14::static_ناپ + 2 * FWORD::static_ناپ);
};

بنية PaintRotate
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
      سیٹ_fixed_clamped (باہرلا->angle, angle.منزل_float (instancer (var_Idx_ادھار, 0)));

    إذا (format == 25 && c->plan->all_axes_pinned)
      باہرلا->format = 24;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_ادھار, 0));

    c->funcs->ڈالو_rotate (c->انکڑے, a);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عام:
  DEFINE_ناپ_STATIC (4 + F2DOT14::static_ناپ);
};

بنية PaintRotateAroundCenter
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->angle, angle.منزل_float (instancer (var_Idx_ادھار, 0)));
      باہرلا->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_ادھار, 1)));
      باہرلا->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
    }

    إذا (format ==27 && c->plan->all_axes_pinned)
        باہرلا->format = 26;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.منزل_float (c->instancer (var_Idx_ادھار, 0));
    عائم tCenterX = centerX + c->instancer (var_Idx_ادھار, 1);
    عائم tCenterY = centerY + c->instancer (var_Idx_ادھار, 2);

    c->funcs->ڈالو_rotate_around_center (c->انکڑے, a, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_ناپ_STATIC (4 + F2DOT14::static_ناپ + 2 * FWORD::static_ناپ);
};

بنية PaintSkew
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_ادھار, 0)));
      سیٹ_fixed_clamped (باہرلا->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_ادھار, 1)));
    }

    إذا (format == 29 && c->plan->all_axes_pinned)
        باہرلا->format = 28;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_ادھار, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_ادھار, 1));

    c->funcs->ڈالو_skew (c->انکڑے, sx, sy);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * F2DOT14::static_ناپ);
};

بنية PaintSkewAroundCenter
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      سیٹ_fixed_clamped (باہرلا->xSkewAngle, xSkewAngle.منزل_float (instancer (var_Idx_ادھار, 0)));
      سیٹ_fixed_clamped (باہرلا->ySkewAngle, ySkewAngle.منزل_float (instancer (var_Idx_ادھار, 1)));
      باہرلا->centerX = hb_clamp_منزل<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (var_Idx_ادھار, 2)));
      باہرلا->centerY = hb_clamp_منزل<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (var_Idx_ادھار, 3)));
    }

    إذا (format == 31 && c->plan->all_axes_pinned)
        باہرلا->format = 30;

    return_trace (باہرلا->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.منزل_float(c->instancer (var_Idx_ادھار, 0));
    عائم sy = ySkewAngle.منزل_float(c->instancer (var_Idx_ادھار, 1));
    عائم tCenterX = centerX + c->instancer (var_Idx_ادھار, 2);
    عائم tCenterY = centerY + c->instancer (var_Idx_ادھار, 3);

    c->funcs->ڈالو_skew_around_center (c->انکڑے, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->نکالو_transform (c->انکڑے);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_ناپ_STATIC (4 + 2 * F2DOT14::static_ناپ + 2 * FWORD::static_ناپ);
};

بنية Paint_مرکب
{
  HB_اندرونی فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_اندرونی فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= باہرلا->src.serialize_subset (c, src, هذا, instancer);
    ret |= باہرلا->backdrop.serialize_subset (c, backdrop, هذا, instancer);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  c->check_ops (هذا->min_ناپ) && // PainComposite can get exponential
                  src.sanitize (c, هذا) &&
                  backdrop.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    hb_paint_مرکب_منوال_t مرکب_منوال_2;
    بدل ((بنانشانی صحيح) منوال)
    {
      حالة HB_PAINT_مرکب_منوال_CLEAR:
      حالة HB_PAINT_مرکب_منوال_SRC:
      حالة HB_PAINT_مرکب_منوال_DEST:
      حالة HB_PAINT_مرکب_منوال_SRC_OVER:
      حالة HB_PAINT_مرکب_منوال_DEST_OVER:
      حالة HB_PAINT_مرکب_منوال_SRC_وچ:
      حالة HB_PAINT_مرکب_منوال_DEST_وچ:
      حالة HB_PAINT_مرکب_منوال_SRC_باہرلا:
      حالة HB_PAINT_مرکب_منوال_DEST_باہرلا:
      حالة HB_PAINT_مرکب_منوال_SRC_ATOP:
      حالة HB_PAINT_مرکب_منوال_DEST_ATOP:
      حالة HB_PAINT_مرکب_منوال_XOR:
      حالة HB_PAINT_مرکب_منوال_PLUS:
      حالة HB_PAINT_مرکب_منوال_SCREEN:
      حالة HB_PAINT_مرکب_منوال_OVERLAY:
      حالة HB_PAINT_مرکب_منوال_DARKEN:
      حالة HB_PAINT_مرکب_منوال_LIGHTEN:
      حالة HB_PAINT_مرکب_منوال_COLOR_DODGE:
      حالة HB_PAINT_مرکب_منوال_COLOR_BURN:
      حالة HB_PAINT_مرکب_منوال_HARD_LIGHT:
      حالة HB_PAINT_مرکب_منوال_SOFT_LIGHT:
      حالة HB_PAINT_مرکب_منوال_DIFFERENCE:
      حالة HB_PAINT_مرکب_منوال_EXCLUSION:
      حالة HB_PAINT_مرکب_منوال_MULTIPLY:
      حالة HB_PAINT_مرکب_منوال_HSL_HUE:
      حالة HB_PAINT_مرکب_منوال_HSL_SATURATION:
      حالة HB_PAINT_مرکب_منوال_HSL_COLOR:
      حالة HB_PAINT_مرکب_منوال_HSL_LUMINOSITY:
	مرکب_منوال_2 = (hb_paint_مرکب_منوال_t) (بنانشانی صحيح) منوال;
	اكسر;
      افتراضي:
	مرکب_منوال_2 = HB_PAINT_مرکب_منوال_CLEAR;
	اكسر;
    }

    c->funcs->ڈالو_گروہ_for (c->انکڑے, HB_PAINT_مرکب_منوال_SRC_OVER);
    c->recurse (هذا+backdrop);
    c->funcs->ڈالو_گروہ_for (c->انکڑے, مرکب_منوال_2);
    c->recurse (هذا+src);
    c->funcs->نکالو_گروہ (c->انکڑے, مرکب_منوال_2);
    c->funcs->نکالو_گروہ (c->انکڑے, HB_PAINT_مرکب_منوال_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_منزل<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  مرکب_منوال		منوال;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_منزل<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية Clip_Box_انکڑے
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

  فراغ get_clip_box (Clip_Box_انکڑے &clip_box, ثابت شے_Var_Store_Instancer &instancer HB_بن_ورتیا) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && var_Idx_ادھار != VarIdx::NO_VARIATION)
    {
      باہرلا->xMin = hb_clamp_منزل<int16_t> ((مزدوج) xMin + (مزدوج) floorf (instancer (var_Idx_ادھار, 0)));
      باہرلا->yMin = hb_clamp_منزل<int16_t> ((مزدوج) yMin + (مزدوج) floorf (instancer (var_Idx_ادھار, 1)));
      باہرلا->xMax = hb_clamp_منزل<int16_t> ((مزدوج) xMax + (مزدوج) ceilf (instancer (var_Idx_ادھار, 2)));
      باہرلا->yMax = hb_clamp_منزل<int16_t> ((مزدوج) yMax + (مزدوج) ceilf (instancer (var_Idx_ادھار, 3)));
    }

    إذا (format == 2 && c->plan->all_axes_pinned)
        باہرلا->format = 1;

    return_trace (سچ);
  }

  عام:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عام:
  DEFINE_ناپ_STATIC (1 + 4 * FWORD::static_ناپ);
};

بنية ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  فراغ get_clip_box (Clip_Box_انکڑے &clip_box, ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    إذا (instancer)
    {
      clip_box.xMin = hb_saturate_جوڑو (clip_box.xMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_ادھار, 0))));
      clip_box.yMin = hb_saturate_جوڑو (clip_box.yMin, hb_clamp_منزل<صحيح> (floorf (instancer (var_Idx_ادھار, 1))));
      clip_box.xMax = hb_saturate_جوڑو (clip_box.xMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_ادھار, 2))));
      clip_box.yMax = hb_saturate_جوڑو (clip_box.yMax, hb_clamp_منزل<صحيح> (ceilf (instancer (var_Idx_ادھار, 3))));
    }
  }

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  { c->variation_indices->جوڑو_range (var_Idx_ادھار, var_Idx_ادھار + 3); }
};

بنية ClipBox
{
  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.format2.closurev1 (c); أرجع;
    افتراضي:أرجع;
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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
                    ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    Clip_Box_انکڑے clip_box;
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
    extents->چوڑائی = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->اچائی = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    أرجع سچ;
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

بنية Clip_ریکارڈ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروع_Glyph_ID ? -1 : g <= سرا_Glyph_ID ? 0 : +1; }

  فراغ closurev1 (hb_colrv1_گھیرا_context_t* c, ثابت فراغ *ادھار) ثابت
  {
    إذا (!c->glyphs->intersects (شروع_Glyph_ID, سرا_Glyph_ID)) أرجع;
    (ادھار+clipBox).closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *ادھار,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    return_trace (باہرلا->clipBox.serialize_subset (c, clipBox, ادھار, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && clipBox.sanitize (c, ادھار));
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
		    ثابت فراغ *ادھار,
		    ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    أرجع (ادھار+clipBox).get_extents (extents, instancer);
  }

  عام:
  HBUINT16		شروع_Glyph_ID;  // first gid clip applies to
  HBUINT16		سرا_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_منزل<ClipBox>	clipBox;   // Box or VarBox
  عام:
  DEFINE_ناپ_STATIC (7);
};
DECLARE_NULL_NAMESPACE_بائٹاں (OT, Clip_ریکارڈ);

بنية ClipList
{
  بنانشانی serialize_clip_ریکارڈاں (hb_subset_context_t *c,
                                   ثابت شے_Var_Store_Instancer &instancer,
                                   ثابت hb_سیٹ_t& gids,
                                   ثابت hb_map_t& gid_وتھ_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (gids.is_empty () ||
        gid_وتھ_map.get_population () != gids.get_population ())
      return_trace (0);

    بنانشانی گنتی  = 0;

    hb_codepoint_t شروع_gid= gids.get_min ();
    hb_codepoint_t prev_gid = شروع_gid;

    بنانشانی وتھ = gid_وتھ_map.get (شروع_gid);
    بنانشانی prev_وتھ = وتھ;
    لكل (ثابت hb_codepoint_t _ : gids.iter ())
    {
      إذا (_ == شروع_gid) استمر;

      وتھ = gid_وتھ_map.get (_);
      إذا (_ == prev_gid + 1 &&  وتھ == prev_وتھ)
      {
        prev_gid = _;
        استمر;
      }

      Clip_ریکارڈ ریکارڈ;
      ریکارڈ.شروع_Glyph_ID = شروع_gid;
      ریکارڈ.سرا_Glyph_ID = prev_gid;
      ریکارڈ.clipBox = prev_وتھ;

      إذا (!ریکارڈ.subset (c, هذا, instancer)) return_trace (0);
      گنتی++;

      شروع_gid = _;
      prev_gid = _;
      prev_وتھ = وتھ;
    }

    //last one
    {
      Clip_ریکارڈ ریکارڈ;
      ریکارڈ.شروع_Glyph_ID = شروع_gid;
      ریکارڈ.سرا_Glyph_ID = prev_gid;
      ریکارڈ.clipBox = prev_وتھ;
      إذا (!ریکارڈ.subset (c, هذا, instancer)) return_trace (0);
      گنتی++;
    }
    return_trace (گنتی);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    إذا (!c->serializer->check_سونپو (باہرلا->format, format, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) return_trace (خطأ);

    ثابت hb_سیٹ_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_وتھ_map;
    hb_سیٹ_t new_gids;
    لكل (ثابت Clip_ریکارڈ& ریکارڈ : clips.iter ())
    {
      بنانشانی شروع_gid = ریکارڈ.شروع_Glyph_ID;
      بنانشانی سرا_gid = ریکارڈ.سرا_Glyph_ID;
      لكل (بنانشانی gid = شروع_gid; gid <= سرا_gid; gid++)
      {
        إذا (!glyphset.has (gid) || !glyph_map.has (gid)) استمر;
        بنانشانی new_gid = glyph_map.get (gid);
        new_gid_وتھ_map.سیٹ (new_gid, ریکارڈ.clipBox);
        new_gids.جوڑو (new_gid);
      }
    }

    بنانشانی گنتی = serialize_clip_ریکارڈاں (c, instancer, new_gids, new_gid_وتھ_map);
    إذا (!گنتی) return_trace (خطأ);
    return_trace (c->serializer->check_سونپو (باہرلا->clips.len, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
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
	       ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    خودکار *rec = clips.as_array ().bsearch (gid);
    إذا (rec)
      أرجع rec->get_extents (extents, هذا, instancer);
    أرجع خطأ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_دا<Clip_ریکارڈ>	clips;  // Clip records, sorted by startGlyphID
  عام:
  DEFINE_ناپ_ARRAY_SIZED (5, clips);
};

بنية Paint
{

  سانچا <قسم_ناں ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_شروع_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->سرا_recursion (هذا->dispatch (c, std::forward<Ts> (ds)...)));
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#جے_انت
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
  NoVariable<Paint_Scale_اکسار>			paintformat20;
  Variable<Paint_Scale_اکسار>			paintformat21;
  NoVariable<Paint_Scale_اکسار_Around_Center>	paintformat22;
  Variable<Paint_Scale_اکسار_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_مرکب				paintformat32;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_MIN (2);
};

بنية ادھار_Glyph_Paint_ریکارڈ
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت فراغ* src_ادھار, hb_subset_context_t *c,
                  ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = s->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    إذا (!s->check_سونپو (باہرلا->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    return_trace (باہرلا->paint.serialize_subset (c, paint, src_ادھار, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && paint.sanitize (c, ادھار)));
  }

  عام:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_منزل<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية ادھار_Glyph_List : Sorted_Array32_دا<ادھار_Glyph_Paint_ریکارڈ>
{
  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا)))  return_trace (خطأ);
    ثابت hb_سیٹ_t* glyphset = &c->plan->_glyphset_colred;

    لكل (ثابت خودکار& _ : as_array ())
    {
      بنانشانی gid = _.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      إذا (_.serialize (c->serializer, c->plan->glyph_map, هذا, c, instancer)) باہرلا->len++;
      وإلا return_trace (خطأ);
    }

    return_trace (باہرلا->len != 0);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Sorted_Array32_دا<ادھار_Glyph_Paint_ریکارڈ>::sanitize (c, هذا));
  }
};

بنية LayerList : Array32_دا_Offset32_منزل<Paint>
{
  ثابت Paint& get_paint (بنانشانی i) ثابت
  { أرجع هذا+(*هذا)[i]; }

  منطقي subset (hb_subset_context_t *c,
               ثابت شے_Var_Store_Instancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودکار& _ : + hb_enumerate (*هذا)
                         | hb_filter (c->plan->colrv1_layers, hb_پہلا))

    {
      خودکار *o = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      ret |= o->serialize_subset (c, _.second, هذا, instancer);
    }
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Array32_دا_Offset32_منزل<Paint>::sanitize (c, هذا));
  }
};

بنية delta_سیٹ_اشاریہ_map_subset_plan_t
{
  بنانشانی get_inner_ثنائی_ہندسہ_گنتی () ثابت { أرجع inner_ثنائی_ہندسہ_گنتی; }
  بنانشانی get_چوڑائی ()           ثابت { أرجع ((outer_ثنائی_ہندسہ_گنتی + inner_ثنائی_ہندسہ_گنتی + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  delta_سیٹ_اشاریہ_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_گنتی = 0;
    outer_ثنائی_ہندسہ_گنتی = 0;
    inner_ثنائی_ہندسہ_گنتی = 1;
    output_map.init ();

    /* search backwards */
    بنانشانی گنتی = new_deltaset_idx_varidx_map.get_population ();
    إذا (!گنتی) أرجع;

    بنانشانی آخری_idx = (بنانشانی)-1;
    بنانشانی آخری_varidx = (بنانشانی)-1;

    لكل (بنانشانی i = گنتی; i; i--)
    {
      بنانشانی delta_سیٹ_idx = i - 1;
      بنانشانی var_idx = new_deltaset_idx_varidx_map.get (delta_سیٹ_idx);
      إذا (i == گنتی)
      {
        آخری_idx = delta_سیٹ_idx;
        آخری_varidx = var_idx;
        استمر;
      }
      إذا (var_idx != آخری_varidx)
        اكسر;
      آخری_idx = delta_سیٹ_idx;
    }

    map_گنتی = آخری_idx + 1;
  }

  منطقي remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    إذا (unlikely (!output_map.resize_dirty (map_گنتی))) أرجع خطأ;

    لكل (بنانشانی idx = 0; idx < map_گنتی; idx++)
    {
      uint32_t *var_idx;
      إذا (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) أرجع خطأ;
      output_map.arrayZ[idx] = *var_idx;

      بنانشانی outer = (*var_idx) >> 16;
      بنانشانی ثنائی_ہندسہ_گنتی = (outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      بنانشانی inner = (*var_idx) & 0xFFFF;
      ثنائی_ہندسہ_گنتی = (inner == 0) ? 1 : hb_ثنائی_ہندسہ_storage (inner);
      inner_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, inner_ثنائی_ہندسہ_گنتی);
    }
    أرجع سچ;
  }

  خاص:
  بنانشانی map_گنتی;
  بنانشانی outer_ثنائی_ہندسہ_گنتی;
  بنانشانی inner_ثنائی_ہندسہ_گنتی;
  hb_سمتیہ_t<uint32_t> output_map;
};

بنية COLR
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_COLR;

  منطقي has_انکڑے () ثابت { أرجع has_v0_انکڑے () || نسخہ; }

  منطقي has_v0_انکڑے () ثابت { أرجع عدد_ادھار_Glyphs; }
  منطقي has_v1_انکڑے () ثابت
  {
    إذا (نسخہ < 1)
      أرجع خطأ;
    hb_barrier ();

    أرجع (هذا+ادھار_Glyph_List_2).len > 0;
  }

  بنانشانی صحيح get_glyph_layers (hb_codepoint_t       glyph,
				 بنانشانی صحيح         شروع_وتھ,
				 بنانشانی صحيح        *گنتی, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت ادھار_Glyph_ریکارڈ &ریکارڈ = (هذا+ادھار_Glyphs_Z).bsearch (عدد_ادھار_Glyphs, glyph);

    hb_array_t<ثابت Layer_ریکارڈ> all_layers = (هذا+layersZ).as_array (عدد_Layers);
    hb_array_t<ثابت Layer_ریکارڈ> glyph_layers = all_layers.sub_array (ریکارڈ.پہلا_Layer_Idx,
								       ریکارڈ.عدد_Layers);
    إذا (گنتی && layers)
    {
      + glyph_layers.sub_array (شروع_وتھ, گنتی)
      | hb_sink (hb_array (layers, *گنتی))
      ;
    }
    أرجع glyph_layers.لمائی;
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


    منطقي has_انکڑے () ثابت { أرجع colr->has_انکڑے (); }

#جے_بنا_تعریف HB_NO_PAINT
    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      إذا (unlikely (!has_انکڑے ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, فراغ *انکڑے,
		      بنانشانی صحيح palette_اشاریہ,
		      hb_color_t foreground,
		      منطقي clip = سچ) ثابت
    {
      إذا (unlikely (!has_انکڑے ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع سچ;
      منطقي ret = colr->paint_glyph (font, glyph, funcs, انکڑے, palette_اشاریہ, foreground, clip, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }
#جے_انت

    منطقي is_valid () { أرجع colr.get_blob ()->لمائی; }

    فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
    {
      إذا (!has_انکڑے ()) أرجع;
      colr->depend (depend_انکڑے);
    }

    فراغ گھیرا_glyphs (hb_codepoint_t glyph,
			 hb_سیٹ_t *related_ids /*باہرلا*/) ثابت
    { colr->گھیرا_glyphs (glyph, related_ids); }

    فراغ گھیرا_V0palette_indices (ثابت hb_سیٹ_t *glyphs,
				    hb_سیٹ_t *palettes /*باہرلا*/) ثابت
    { colr->گھیرا_V0palette_indices (glyphs, palettes); }

    فراغ گھیرا_for_V1 (hb_سیٹ_t *glyphset,
                        hb_سیٹ_t *layer_indices,
                        hb_سیٹ_t *palette_indices,
                        hb_سیٹ_t *variation_indices,
                        hb_سیٹ_t *delta_سیٹ_indices) ثابت
    { colr->گھیرا_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_سیٹ_indices); }

    منطقي has_var_store () ثابت
    { أرجع colr->has_var_store (); }

    ثابت شے_Variation_Store &get_var_store () ثابت
    { أرجع colr->get_var_store (); }
    ثابت شے_Variation_Store *get_var_store_ptr () ثابت
    { أرجع colr->get_var_store_ptr (); }

    منطقي has_delta_سیٹ_اشاریہ_map () ثابت
    { أرجع colr->has_delta_سیٹ_اشاریہ_map (); }

    ثابت Delta_سیٹ_اشاریہ_Map &get_delta_سیٹ_اشاریہ_map () ثابت
    { أرجع colr->get_delta_سیٹ_اشاریہ_map (); }
    ثابت Delta_سیٹ_اشاریہ_Map *get_delta_سیٹ_اشاریہ_map_ptr () ثابت
    { أرجع colr->get_delta_سیٹ_اشاریہ_map_ptr (); }

    خاص:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, خالی_سوچک)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع خالی_سوچک;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (خالی_سوچک, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عام:
    hb_blob_ptr_t<COLR> colr;
    خاص:
    بدلنجوگ hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
  {
    // v0
    hb_array_t<ثابت ادھار_Glyph_ریکارڈ> ادھار_Glyphs = (هذا+ادھار_Glyphs_Z).as_array (عدد_ادھار_Glyphs);
    لكل (ثابت ادھار_Glyph_ریکارڈ &ریکارڈ : ادھار_Glyphs)
    {
      خودکار glyph_layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (ریکارڈ.پہلا_Layer_Idx,
                                                                         ریکارڈ.عدد_Layers);
      لكل (ثابت Layer_ریکارڈ layer : glyph_layers)
        depend_انکڑے->جوڑو_depend(ریکارڈ.glyphId, HB_OT_ٹیگ_COLR, layer.glyphId);
    }

    إذا (!has_v1_انکڑے())
      أرجع;

    // v1
    hb_colrv1_depend_context_t c (هذا, depend_انکڑے);
    ثابت ادھار_Glyph_List &baseglyph_paintrecords = هذا+ادھار_Glyph_List_2;

    لكل (ثابت ادھار_Glyph_Paint_ریکارڈ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.سروت_gid = baseglyph_paintrecord.glyphId;
      c.ویکھیا_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  فراغ گھیرا_glyphs (hb_codepoint_t glyph,
		       hb_سیٹ_t *related_ids /*باہرلا*/) ثابت
  {
    ثابت ادھار_Glyph_ریکارڈ *ریکارڈ = get_ادھار_glyph_ریکارڈ (glyph);
    إذا (!ریکارڈ) أرجع;

    خودکار glyph_layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (ریکارڈ->پہلا_Layer_Idx,
								       ریکارڈ->عدد_Layers);
    إذا (!glyph_layers.لمائی) أرجع;
    related_ids->جوڑو_array (&glyph_layers[0].glyphId, glyph_layers.لمائی, Layer_ریکارڈ::min_ناپ);
  }

  فراغ گھیرا_V0palette_indices (ثابت hb_سیٹ_t *glyphs,
				  hb_سیٹ_t *palettes /*باہرلا*/) ثابت
  {
    إذا (!عدد_ادھار_Glyphs || !عدد_Layers) أرجع;
    hb_array_t<ثابت ادھار_Glyph_ریکارڈ> ادھار_Glyphs = (هذا+ادھار_Glyphs_Z).as_array (عدد_ادھار_Glyphs);
    hb_array_t<ثابت Layer_ریکارڈ> all_layers = (هذا+layersZ).as_array (عدد_Layers);

    لكل (ثابت ادھار_Glyph_ریکارڈ ریکارڈ : ادھار_Glyphs)
    {
      إذا (!glyphs->has (ریکارڈ.glyphId)) استمر;
      hb_array_t<ثابت Layer_ریکارڈ> glyph_layers = all_layers.sub_array (ریکارڈ.پہلا_Layer_Idx,
                                                                   ریکارڈ.عدد_Layers);
      لكل (ثابت Layer_ریکارڈ layer : glyph_layers)
        palettes->جوڑو (layer.colorIdx);
    }
  }

  فراغ گھیرا_for_V1 (hb_سیٹ_t *glyphset,
                      hb_سیٹ_t *layer_indices,
                      hb_سیٹ_t *palette_indices,
                      hb_سیٹ_t *variation_indices,
                      hb_سیٹ_t *delta_سیٹ_indices) ثابت
  {
    إذا (نسخہ < 1) أرجع;
    hb_barrier ();

    hb_سیٹ_t ویکھیا_glyphs;

    hb_colrv1_گھیرا_context_t c (هذا, &ویکھیا_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت ادھار_Glyph_List &baseglyph_paintrecords = هذا+ادھار_Glyph_List_2;

    لكل (ثابت ادھار_Glyph_Paint_ریکارڈ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      بنانشانی gid = baseglyph_paintrecord.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_سیٹ_union (glyphset, &ویکھیا_glyphs);

    ثابت ClipList &cliplist = هذا+clipList;
    c.glyphs = glyphset;
    لكل (ثابت Clip_ریکارڈ &clip_ریکارڈ : cliplist.clips.iter())
      clip_ریکارڈ.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    إذا (has_delta_سیٹ_اشاریہ_map ())
    {
      ثابت Delta_سیٹ_اشاریہ_Map &var_idx_map = هذا+varIdxMap;
      delta_سیٹ_indices->سیٹ (*variation_indices);
      variation_indices->clear ();
      لكل (بنانشانی delta_سیٹ_idx : *delta_سیٹ_indices)
        variation_indices->جوڑو (var_idx_map.map (delta_سیٹ_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { أرجع (هذا+layerList); }

  ثابت ادھار_Glyph_List& get_baseglyphList () ثابت
  { أرجع (هذا+ادھار_Glyph_List_2); }

  منطقي has_var_store () ثابت
  { أرجع نسخہ >= 1 && hb_barrier () && varStore != 0; }

  منطقي has_delta_سیٹ_اشاریہ_map () ثابت
  { أرجع نسخہ >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقي has_clip_list () ثابت
  { أرجع نسخہ >= 1 && hb_barrier () && clipList != 0; }

  ثابت Delta_سیٹ_اشاریہ_Map &get_delta_سیٹ_اشاریہ_map () ثابت
  { أرجع has_delta_سیٹ_اشاریہ_map () && hb_barrier () ? هذا+varIdxMap : Null (Delta_سیٹ_اشاریہ_Map); }
  ثابت Delta_سیٹ_اشاریہ_Map *get_delta_سیٹ_اشاریہ_map_ptr () ثابت
  { أرجع has_delta_سیٹ_اشاریہ_map () && hb_barrier () ? &(هذا+varIdxMap) : خالی_سوچک; }

  ثابت شے_Variation_Store &get_var_store () ثابت
  { أرجع has_var_store () && hb_barrier () ? هذا+varStore : Null (شے_Variation_Store); }
  ثابت شے_Variation_Store *get_var_store_ptr () ثابت
  { أرجع has_var_store () && hb_barrier () ? &(هذا+varStore) : خالی_سوچک; }

  ثابت ClipList &get_clip_list () ثابت
  { أرجع has_clip_list () && hb_barrier () ? هذا+clipList : Null (ClipList); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  (هذا+ادھار_Glyphs_Z).sanitize (c, عدد_ادھار_Glyphs) &&
                  (هذا+layersZ).sanitize (c, عدد_Layers) &&
                  (نسخہ == 0 ||
		   (hb_barrier () &&
		    ادھار_Glyph_List_2.sanitize (c, هذا) &&
		    layerList.sanitize (c, هذا) &&
		    clipList.sanitize (c, هذا) &&
		    varIdxMap.sanitize (c, هذا) &&
		    varStore.sanitize (c, هذا))));
  }

  سانچا<قسم_ناں ادھار_Iterator, قسم_ناں LayerIterator,
	   hb_requires (hb_is_iterator (ادھار_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقي serialize_V0 (hb_serialize_context_t *c,
		     بنانشانی نسخہ,
		     ادھار_Iterator ادھار_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (ادھار_it.len () != layer_it.len ()))
      return_trace (خطأ);

    هذا->نسخہ = نسخہ;
    عدد_Layers = 0;
    عدد_ادھار_Glyphs = ادھار_it.len ();
    إذا (عدد_ادھار_Glyphs == 0)
    {
      ادھار_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (سچ);
    }

    c->push ();
    لكل (ثابت hb_شے_type<ادھار_Iterator> _ : + ادھار_it.iter ())
    {
      خودکار* ریکارڈ = c->embed (_);
      إذا (unlikely (!ریکارڈ)) return_trace (خطأ);
      ریکارڈ->پہلا_Layer_Idx = عدد_Layers;
      عدد_Layers += ریکارڈ->عدد_Layers;
    }
    c->جوڑو_link (ادھار_Glyphs_Z, c->نکالو_pack ());

    c->push ();
    لكل (ثابت hb_شے_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->جوڑو_link (layersZ, c->نکالو_pack ());

    return_trace (سچ);
  }

  ثابت ادھار_Glyph_ریکارڈ* get_ادھار_glyph_ریکارڈ (hb_codepoint_t gid) ثابت
  {
    ثابت ادھار_Glyph_ریکارڈ* ریکارڈ = &(هذا+ادھار_Glyphs_Z).bsearch (عدد_ادھار_Glyphs, (بنانشانی صحيح) gid);
    إذا (ریکارڈ == &Null (ادھار_Glyph_ریکارڈ) ||
        (ریکارڈ && (hb_codepoint_t) ریکارڈ->glyphId != gid))
      ریکارڈ = خالی_سوچک;
    أرجع ریکارڈ;
  }

  ثابت ادھار_Glyph_Paint_ریکارڈ* get_ادھار_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت ادھار_Glyph_List &list = هذا+ادھار_Glyph_List_2;
    بنانشانی صحيح i;
    إذا (!list.bfind ((بنانشانی) gid, &i))
      أرجع خالی_سوچک;
    أرجع &list[i];
  }

  منطقي downgrade_منزل_V0 (ثابت hb_سیٹ_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    لكل (ثابت ادھار_Glyph_Paint_ریکارڈ& _ : get_baseglyphList ())
      إذا (glyphset.has (_.glyphId))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        COLR* باہرلا /*باہرلا*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (سچ);

    ثابت شے_Variation_Store& var_store = هذا+varStore;
    إذا (c->plan->normalized_coords)
    {
      شے_variations_t شے_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقي optimize = (varIdxMap != 0) ? سچ : خطأ;
      إذا (!شے_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (خطأ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      إذا (شے_vars.get_region_list () &&
          شے_vars.get_vardata_encodings () &&
          !باہرلا->varStore.serialize_serialize (c->serializer,
                                              شے_vars.has_long_لفظ (),
                                              c->plan->axis_ٹیگ_سوچی,
                                              شے_vars.get_region_list (),
                                              شے_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      إذا (optimize)
      {
        ثابت hb_map_t &varidx_map = شے_vars.get_varidx_map ();
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
      hb_سیٹ_t dead_regions;
      إذا (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_اشاریہ_ٹیگ_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      إذا (unlikely (!باہرلا->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : خالی_سوچک)))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  منطقي subset_delta_سیٹ_اشاریہ_map (hb_subset_context_t *c,
                                   COLR* باہرلا /*باہرلا*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (سچ);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_سیٹ_اشاریہ_map_subset_plan_t اشاریہ_map_plan (deltaset_idx_varidx_map);

    إذا (unlikely (!اشاریہ_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (خطأ);

    return_trace (باہرلا->varIdxMap.serialize_serialize (c->serializer, اشاریہ_map_plan));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &الٹ_glyph_map = *c->plan->الٹ_glyph_map;
    ثابت hb_سیٹ_t& glyphset = c->plan->_glyphset_colred;

    خودکار ادھار_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = الٹ_glyph_map.get (new_gid);
		    إذا (glyphset.has (old_gid)) أرجع سچ;
		    أرجع خطأ;
		 })
    | hb_map_retains_چھانٹی ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = الٹ_glyph_map.get (new_gid);

				ثابت ادھار_Glyph_ریکارڈ* old_ریکارڈ = get_ادھار_glyph_ریکارڈ (old_gid);
				إذا (unlikely (!old_ریکارڈ))
				  أرجع hb_pair_t<منطقي, ادھار_Glyph_ریکارڈ> (خطأ, Null (ادھار_Glyph_ریکارڈ));
				ادھار_Glyph_ریکارڈ new_ریکارڈ = {};
				new_ریکارڈ.glyphId = new_gid;
				new_ریکارڈ.عدد_Layers = old_ریکارڈ->عدد_Layers;
				أرجع hb_pair_t<منطقي, ادھار_Glyph_ریکارڈ> (سچ, new_ریکارڈ);
			      })
    | hb_filter (hb_پہلا)
    | hb_map_retains_چھانٹی (hb_دوجا)
    ;

    خودکار layer_it =
    + hb_range (c->plan->عدد_output_glyphs ())
    | hb_map (الٹ_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_چھانٹی ([&](hb_codepoint_t old_gid)
			      {
				ثابت ادھار_Glyph_ریکارڈ* old_ریکارڈ = get_ادھار_glyph_ریکارڈ (old_gid);
				hb_سمتیہ_t<Layer_ریکارڈ> باہرلا_layers;

				إذا (unlikely (!old_ریکارڈ ||
					      old_ریکارڈ->پہلا_Layer_Idx >= عدد_Layers ||
					      old_ریکارڈ->پہلا_Layer_Idx + old_ریکارڈ->عدد_Layers > عدد_Layers))
				  أرجع hb_pair_t<منطقي, hb_سمتیہ_t<Layer_ریکارڈ>> (خطأ, باہرلا_layers);

				خودکار layers = (هذا+layersZ).as_array (عدد_Layers).sub_array (old_ریکارڈ->پہلا_Layer_Idx,
											     old_ریکارڈ->عدد_Layers);
				باہرلا_layers.resize (layers.لمائی);
				لكل (بنانشانی صحيح i = 0; i < layers.لمائی; i++) {
				  باہرلا_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  إذا (unlikely (!c->plan->new_gid_for_old_gid (باہرلا_layers[i].glyphId, &new_gid)))
				    أرجع hb_pair_t<منطقي, hb_سمتیہ_t<Layer_ریکارڈ>> (خطأ, باہرلا_layers);
				  باہرلا_layers[i].glyphId = new_gid;
				  باہرلا_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				أرجع hb_pair_t<منطقي, hb_سمتیہ_t<Layer_ریکارڈ>> (سچ, باہرلا_layers);
			      })
    | hb_filter (hb_پہلا)
    | hb_map_retains_چھانٹی (hb_دوجا)
    ;

    إذا (نسخہ == 0 && (!ادھار_it || !layer_it))
      return_trace (خطأ);

    خودکار *colr_مفرد = c->serializer->شروع_embed<COLR> ();
    إذا (unlikely (!c->serializer->extend_min (colr_مفرد)))  return_trace (خطأ);

    إذا (نسخہ == 0 || downgrade_منزل_V0 (glyphset))
      return_trace (colr_مفرد->serialize_V0 (c->serializer, 0, ادھار_it, layer_it));

    hb_barrier ();

    //start version 1
    إذا (!c->serializer->allocate_ناپ<فراغ> (5 * HBUINT32::static_ناپ)) return_trace (خطأ);
    إذا (!colr_مفرد->serialize_V0 (c->serializer, نسخہ, ادھار_it, layer_it)) return_trace (خطأ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    إذا (!subset_varstore (c, colr_مفرد)) return_trace (خطأ);

    شے_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : خالی_سوچک,
				     c->plan->normalized_coords ? get_delta_سیٹ_اشاریہ_map_ptr () : خالی_سوچک,
				     c->plan->normalized_coords.as_array ());

    إذا (!colr_مفرد->ادھار_Glyph_List_2.serialize_subset (c, ادھار_Glyph_List_2, هذا, instancer))
      return_trace (خطأ);

    colr_مفرد->layerList.serialize_subset (c, layerList, هذا, instancer);
    colr_مفرد->clipList.serialize_subset (c, clipList, هذا, instancer);

    return_trace (subset_delta_سیٹ_اشاریہ_map (c, colr_مفرد));
  }

  ثابت Paint *get_ادھار_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت ادھار_Glyph_List &baseglyph_paintrecords = هذا+ادھار_Glyph_List_2;
    ثابت ادھار_Glyph_Paint_ریکارڈ* ریکارڈ = get_ادھار_glyph_paintrecord (glyph);
    إذا (ریکارڈ)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+ریکارڈ->paint;
      أرجع &paint;
    }
    وإلا
      أرجع خالی_سوچک;
  }

#جے_بنا_تعریف HB_NO_PAINT
  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    شے_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_سیٹ_اشاریہ_map_ptr (),
                                     hb_array (font->coords, font->عدد_coords));

    إذا (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      أرجع سچ;
    }

    خودکار *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقي ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), سچ, scratch);

    خودکار e = scratch.paint_extents.get_extents ();
    إذا (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->چوڑائی = 0;
      extents->اچائی = 0;
    }
    وإلا
      *extents = e.منزل_glyph_extents ();

    أرجع ret;
  }
#جے_انت

  منطقي
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    إذا (نسخہ >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_ادھار_glyph_paint (glyph);

      أرجع paint != خالی_سوچک;
    }

    أرجع خطأ;
  }

  منطقي get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت شے_Var_Store_Instancer instancer) ثابت
  {
    أرجع get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#جے_بنا_تعریف HB_NO_PAINT
  منطقي
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, فراغ *انکڑے,
	       بنانشانی صحيح palette_اشاریہ, hb_color_t foreground,
	       منطقي clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    شے_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_سیٹ_اشاریہ_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->عدد_coords : 0));
    hb_paint_context_t c (هذا, funcs, انکڑے, font, palette_اشاریہ, foreground, instancer);

    hb_decycler_گرہ_t node (c.glyphs_decycler);
    node.ویکھو (glyph);

    إذا (نسخہ >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_ادھار_glyph_paint (glyph);
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
	    خودکار *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_اشاریہ, foreground,
			 خطأ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ڈالو_font_transform (c.انکڑے, font);

	إذا (clip)
	  c.funcs->ڈالو_clip_rectangle (c.انکڑے,
					(عائم) extents.x_bearing,
					(عائم) extents.y_bearing + extents.اچائی,
					(عائم) extents.x_bearing + extents.چوڑائی,
					(عائم) extents.y_bearing);

	إذا (is_bounded)
	  c.recurse (*paint);

	إذا (clip)
	  c.funcs->نکالو_clip (c.انکڑے);

	c.funcs->نکالو_transform (c.انکڑے);

        أرجع سچ;
      }
    }

    ثابت ادھار_Glyph_ریکارڈ *ریکارڈ = get_ادھار_glyph_ریکارڈ (glyph);
    إذا (ریکارڈ && ((hb_codepoint_t) ریکارڈ->glyphId == glyph))
    {
      // COLRv0 glyph
      لكل (ثابت خودکار &r : (هذا+layersZ).as_array (عدد_Layers)
			   .sub_array (ریکارڈ->پہلا_Layer_Idx, ریکارڈ->عدد_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.انکڑے, r.glyphId, c.font, is_foreground, color);
      }

      أرجع سچ;
    }

    أرجع خطأ;
  }
#جے_انت

  محمي:
  HBUINT16	نسخہ;	/* Table version number (starts at 0). */
  HBUINT16	عدد_ادھار_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_منزل<Sorted_Unsized_Array_دا<ادھار_Glyph_ریکارڈ>>
		ادھار_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_منزل<Unsized_Array_دا<Layer_ریکارڈ>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	عدد_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_منزل<ادھار_Glyph_List>		ادھار_Glyph_List_2;
  Offset32_منزل<LayerList>			layerList;
  Offset32_منزل<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_منزل<Delta_سیٹ_اشاریہ_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_منزل<شے_Variation_Store>	varStore;
  عام:
  DEFINE_ناپ_MIN (14);
};

بنية COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

فراغ
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  إذا (unlikely (ڈونگھائی_بایاں <= 0 || کنارا_گنتی <= 0)) أرجع;
  ڈونگھائی_بایاں--;
  کنارا_گنتی--;
  paint.dispatch (هذا);
  ڈونگھائی_بایاں++;
}

سانچا <قسم_ناں Types>
فراغ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  إذا (likely (c->ڈونگھائی_بایاں > 0 && c->کنارا_گنتی > 0) &&
      (هذا+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->کنارا_گنتی--;
    c->funcs->ڈالو_inverse_font_transform (c->انکڑے, c->font);
    c->funcs->fill_glyph (c->انکڑے, gid, c->font, is_foreground, color);
    c->funcs->نکالو_transform (c->انکڑے);
    أرجع;
  }

  c->funcs->ڈالو_inverse_font_transform (c->انکڑے, c->font);
  c->funcs->ڈالو_clip_glyph (c->انکڑے, gid, c->font);
  c->funcs->ڈالو_font_transform (c->انکڑے, c->font);
  c->recurse (هذا+paint);
  c->funcs->نکالو_transform (c->انکڑے);
  c->funcs->نکالو_clip (c->انکڑے);
  c->funcs->نکالو_transform (c->انکڑے);
}

فراغ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);
  ثابت LayerList &paint_وتھ_lists = c->get_colr_جدول ()->get_layerList ();
  hb_decycler_گرہ_t node (c->layers_decycler);
  لكل (بنانشانی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    إذا (unlikely (!node.ویکھو (i)))
      أرجع;

    ثابت Paint &paint = paint_وتھ_lists.get_paint (i);
    c->recurse (paint);
  }
}

فراغ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_decycler_گرہ_t node (c->glyphs_decycler);
  إذا (unlikely (!node.ویکھو (gid)))
    أرجع;

  c->funcs->ڈالو_inverse_font_transform (c->انکڑے, c->font);
  إذا (c->funcs->color_glyph (c->انکڑے, gid, c->font))
  {
    c->funcs->نکالو_transform (c->انکڑے);
    أرجع;
  }
  c->funcs->نکالو_transform (c->انکڑے);

  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت Paint *paint = colr_جدول->get_ادھار_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقي has_clip_box = colr_جدول->get_clip (gid, &extents, c->instancer);

  إذا (has_clip_box)
    c->funcs->ڈالو_clip_rectangle (c->انکڑے,
				   (عائم) extents.x_bearing,
				   (عائم) extents.y_bearing + extents.اچائی,
				   (عائم) extents.x_bearing + extents.چوڑائی,
				   (عائم) extents.y_bearing);

  إذا (paint)
    c->recurse (*paint);

  إذا (has_clip_box)
    c->funcs->نکالو_clip (c->انکڑے);
}

} /* namespace OT */

#جے_انت /* OT_COLOR_COLR_COLR_HH */
