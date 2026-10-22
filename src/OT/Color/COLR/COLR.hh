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

#ifndef OT_COLOR_COLR_COLR_HH
#define OT_COLOR_COLR_COLR_HH

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
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

فضاینام OT {
ساختار hb_paint_context_t;
}

ساختار hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

فضاینام OT {

ساختار COLR;

ساختار Paint;

ساختار hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ثابت نویسه *get_name () { بازگشت "PAINT"; }
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  ثابت COLR* get_colr_table () ثابت
  { بازگشت reinterpret_cast<ثابت COLR *> (base); }

عمومی:
  ثابت تهی *base;
  hb_paint_funcs_t *funcs;
  تهی *data;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحیح depth_چپ = HB_MAX_NESTING_LEVEL;
  صحیح edge_شمار = HB_MAX_GRAPH_EDGE_شمار;

  hb_paint_context_t (ثابت تهی *base_,
		      hb_paint_funcs_t *funcs_,
		      تهی *data_,
                      hb_font_t *font_,
                      unsigned صحیح palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_شمار () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    اگر (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, درست);
      hb_font_set_synthetic_slant (font, 0);
    }
    وگرنه
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ایستا unsigned
  color_alpha (hb_color_t color, اعشاری alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    بازگشت roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned صحیح color_index, اعشاری alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = درست;

    اگر (color_index != 0xffff)
    {
      اگر (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = نادرست;
    }

    بازگشت HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  درونخطی تهی recurse (ثابت Paint &paint);
};

ساختار hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    اگر (unlikely (nesting_level_چپ == 0))
      بازگشت hb_empty_t ();

    اگر (paint_visited (&obj))
      بازگشت hb_empty_t ();

    nesting_level_چپ--;
    obj.closurev1 (این);
    nesting_level_چپ++;
    بازگشت hb_empty_t ();
  }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  منطقی paint_visited (ثابت تهی *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    اگر (visited_paint.in_error() || visited_paint.has (delta))
      بازگشت درست;

    visited_paint.add (delta);
    بازگشت نادرست;
  }

  ثابت COLR* get_colr_table () ثابت
  { بازگشت reinterpret_cast<ثابت COLR *> (base); }

  تهی add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  تهی add_layer_indices (unsigned first_layer_index, unsigned رقم_of_layers)
  {
    اگر (رقم_of_layers == 0) بازگشت;
    layer_indices->add_range (first_layer_index, first_layer_index + رقم_of_layers - 1);
  }

  تهی add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  تهی add_var_idxes (unsigned first_var_idx, unsigned رقم_idxes)
  {
    اگر (!رقم_idxes || first_var_idx == VarIdx::NO_VARIATION) بازگشت;
    variation_indices->add_range (first_var_idx, first_var_idx + رقم_idxes - 1);
  }

  عمومی:
  ثابت تهی *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned رقم_var_idxes;
  unsigned nesting_level_چپ;

  hb_colrv1_closure_context_t (ثابت تهی *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned رقم_var_idxes_2 = 1,
                               unsigned nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          رقم_var_idxes (رقم_var_idxes_2),
                          nesting_level_چپ (nesting_level_چپ_2)
  {}
};


ساختار hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    اگر (unlikely (nesting_level_چپ == 0))
      بازگشت hb_empty_t ();

    اگر (paint_visited (&obj))
      بازگشت hb_empty_t ();

    nesting_level_چپ--;
    obj.dependv1 (این);
    nesting_level_چپ++;
    بازگشت hb_empty_t ();
  }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  منطقی paint_visited (ثابت تهی *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    اگر (visited_paint.in_error() || visited_paint.has (delta))
      بازگشت درست;

    visited_paint.add (delta);
    اگر (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    بازگشت نادرست;
  }

  ثابت COLR* get_colr_table () ثابت
  { بازگشت colr; }

  عمومی:
  ثابت COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_چپ;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_چپ (nesting_level_چپ_2),
                          source_gid (0)
  {}
};

ساختار LayerRecord
{
  operator hb_ot_color_layer_t () ثابت { بازگشت {glyphId, colorIdx}; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  Index		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار BaseGlyphRecord
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	رقم_Layers;	/* Number of color layers
				 * associated with this glyph */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ساختار Variable
{
  ایستا constexpr منطقی is_variable = درست;

  ثابت T& get_مقدار () ثابت { بازگشت value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (این));
  }

  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->رقم_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->رقم_var_idxes);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!value.subset (c, instancer, varIdxBase)) return_trace (نادرست);
    اگر (c->plan->all_axes_pinned)
      return_trace (درست);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    اگر (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, صحیح> *new_varidx_delta;
      اگر (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (نادرست);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c));
  }

  تهی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (این);
    value.paint_glyph (c, varIdxBase);
  }

  تهی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  تهی get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بازگشت value.get_extend ();
  }

  محافظتشده:
  T      value;
  عمومی:
  VarIdx varIdxBase;
  عمومی:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
ساختار NoVariable
{
  ایستا constexpr منطقی is_variable = نادرست;

  ثابت T& get_مقدار () ثابت { بازگشت value; }

  ایستا constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (این));
  }

  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c));
  }

  تهی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (این);
    value.paint_glyph (c, varIdxBase);
  }

  تهی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  تهی get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بازگشت value.get_extend ();
  }

  T      value;
  عمومی:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
ایستا تهی
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, اعشاری value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

ساختار ColorStop
{
  تهی dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->add_palette_index (paletteIndex);
    c->رقم_var_idxes = 2;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      اعشاری a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  عمومی:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

ساختار Extend : HBUINT8
{
  شمارش {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عمومی:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> رده Var>
ساختار Color_سطر
{
  تهی dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    برای (ثابت auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    اگر (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (نادرست);
    اگر (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (نادرست);

    برای (ثابت auto& stop : stops.iter ())
    {
      اگر (!stop.subset (c, instancer)) return_trace (نادرست);
    }
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned صحیح
  get_color_stops (hb_paint_context_t *c,
                   unsigned صحیح start,
		   unsigned صحیح *شمار,
		   hb_color_stop_t *color_stops,
		   ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    unsigned صحیح len = stops.len;

    اگر (شمار && color_stops)
    {
      unsigned صحیح i;
      برای (i = 0; i < *شمار && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *شمار = i;
    }

    بازگشت len;
  }

  HB_INTERNAL ایستا unsigned صحیح static_get_color_stops (hb_color_سطر_t *color_سطر,
							  تهی *color_سطر_data,
							  unsigned صحیح start,
							  unsigned صحیح *شمار,
							  hb_color_stop_t *color_stops,
							  تهی *user_data)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    بازگشت thiz->get_color_stops (c, start, شمار, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    انتخاب ((unsigned صحیح) extend)
    {
      حالت Extend::EXTEND_REPEAT:  بازگشت HB_PAINT_EXTEND_REPEAT;
      حالت Extend::EXTEND_REFLECT: بازگشت HB_PAINT_EXTEND_REFLECT;
      حالت Extend::EXTEND_PAD:
      پیشفرض:                     بازگشت HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL ایستا hb_paint_extend_t static_get_extend (hb_color_سطر_t *color_سطر,
							  تهی *color_سطر_data,
							  تهی *user_data)
  {
    ثابت Color_سطر *thiz = (ثابت Color_سطر *) color_سطر_data;
    بازگشت thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
ساختار CompositeMode : HBUINT8
{
  عمومی:
  DEFINE_SIZE_STATIC (1);
};

ساختار Affine2x3
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { c->رقم_var_idxes = 6; }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);
    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((دوگانه) source.to_int () +
                                                    (دوگانه) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (درست);
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    c->funcs->افزودن_transform (c->data,
			      xx.to_float (c->instancer (varIdxBase, 0)),
			      yx.to_float (c->instancer (varIdxBase, 1)),
                              xy.to_float (c->instancer (varIdxBase, 2)),
			      yy.to_float (c->instancer (varIdxBase, 3)),
                              dx.to_float (c->instancer (varIdxBase, 4)),
			      dy.to_float (c->instancer (varIdxBase, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  عمومی:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

ساختار PaintColrLayers
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    uint32_t first_layer_index = رقم_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  درونخطی تهی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	رقم_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار PaintSolid
{
  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->add_palette_index (paletteIndex);
    c->رقم_var_idxes = 1;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      اعشاری a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    اگر (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  عمومی:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> رده Var>
ساختار PaintLinearGradient
{
  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (این+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 6;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((دوگانه) x0 + (دوگانه) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((دوگانه) y0 + (دوگانه) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((دوگانه) x1 + (دوگانه) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((دوگانه) y1 + (دوگانه) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((دوگانه) x2 + (دوگانه) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((دوگانه) y2 + (دوگانه) roundf (instancer (varIdxBase, 5)));
    }

    اگر (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_سطر_2.serialize_subset (c, color_سطر_2, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && color_سطر_2.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    hb_color_سطر_t cl = {
      (تهی *) &(این+color_سطر_2),
      (این+color_سطر_2).static_get_color_stops, c,
      (این+color_سطر_2).static_get_extend, nullptr
    };

    c->funcs->linear_gradient (c->data, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       x1 + c->instancer (varIdxBase, 2),
			       y1 + c->instancer (varIdxBase, 3),
			       x2 + c->instancer (varIdxBase, 4),
			       y2 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24To<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> رده Var>
ساختار PaintRadialGradient
{
  منطقی constant_after_pinning (hb_subset_context_t *c,
                               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    اگر (c->plan->has_avar2) بازگشت نادرست;
    ثابت auto &regions = instancer.varStore->get_region_list ();
    برای (unsigned i = 0; i < regions.region_شمار; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      اگر (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        بازگشت نادرست;
      }
      برای (auto axis : axes)
      {
        Triple *limit;
        اگر (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          بازگشت نادرست;
      }
    }
    بازگشت درست;
  }

  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (این+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 6;
  }

  منطقی subset_nonnegative (hb_subset_context_t *c,
                           ثابت ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ثابت
  {
    دوگانه source[6] = {
      x0 + (دوگانه) roundf (instancer (varIdxBase, 0)),
      y0 + (دوگانه) roundf (instancer (varIdxBase, 1)),
      radius0 + (دوگانه) roundf (instancer (varIdxBase, 2)),
      x1 + (دوگانه) roundf (instancer (varIdxBase, 3)),
      y1 + (دوگانه) roundf (instancer (varIdxBase, 4)),
      radius1 + (دوگانه) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      اگر (!out) بازگشت نادرست;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      بازگشت درست;
    };
    ثابت auto &سطر = این+color_سطر_2;
    اگر (source[2] == source[5] || !سطر.stops.len) بازگشت transparent ();
    ساختار stop_t { دوگانه offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    اگر (!stops.alloc (سطر.stops.len)) بازگشت نادرست;
    unsigned order = 0;
    برای (ثابت auto &entry : سطر.stops.iter ())
    {
      ثابت auto &stop = entry.get_مقدار ();
      stops.push (stop_t {(دوگانه) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (دوگانه) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    اگر (stops.in_error ()) بازگشت نادرست;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      بازگشت a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    دوگانه dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    برای (unsigned j = 0; j < 3; j++)
    {
      دوگانه remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      اگر (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        بازگشت نادرست;
      }
      unsigned راست = (unsigned) remainder;
      وقتی (راست) { unsigned r = lattice % راست; lattice = راست; راست = r; }
    }
    برای (unsigned shift = 0; shift < 16; shift++)
    {
      دوگانه b = 1u << shift;
      دوگانه coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      دوگانه fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      دوگانه candidates[] = {coarse, fine};
      برای (دوگانه a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        دوگانه geometry[6];
        منطقی fits = درست;
        برای (unsigned j = 0; j < 3; j++)
        {
          دوگانه difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          دوگانه کم = j == 2 ? 0 : -32768, بالا = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= کم && geometry[j] <= بالا &&
                  geometry[j + 3] >= کم && geometry[j + 3] <= بالا;
        }
        اگر (!fits) ادامه;
        unsigned first = 0, شمار = stops.length;
        دوگانه min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        دوگانه bias = 0;
        منطقی constant = نادرست;
        اگر (سطر.extend == Extend::EXTEND_PAD)
        {
          دوگانه tip = -geometry[2] / (b * dr);
          اگر ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            شمار = 1; constant = درست;
          }
        }
        وگرنه
        {
          دوگانه period = max - min;
          اگر (سطر.extend == Extend::EXTEND_REFLECT) period *= 2;
          اگر (period <= 0) بازگشت transparent ();
          دوگانه n = ceil ((-2 - min) / period);
          اگر (n > floor ((32767. / 16384. - max) / period)) ادامه;
          bias = n * period;
        }
        برای (unsigned j = 0; j < شمار; j++)
        {
          دوگانه bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        اگر (!fits) ادامه;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        اگر (!out) بازگشت نادرست;
        out->format = 6;
        out->x0 = (صحیح) geometry[0]; out->y0 = (صحیح) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (صحیح) geometry[3]; out->y1 = (صحیح) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_سطر = c->serializer->allocate_size<Color_سطر<NoVariable>> (3 + شمار * ColorStop::static_size);
        اگر (!out_سطر) { c->serializer->برداشت_discard (); بازگشت نادرست; }
        out_سطر->extend = سطر.extend;
        out_سطر->stops.len = شمار;
        برای (unsigned j = 0; j < شمار; j++)
        {
          ثابت auto &stop = stops[first + j];
          auto &out_stop = out_سطر->stops[j].value;
          out_stop.stopOffset.set_float ((اعشاری) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((اعشاری) stop.alpha);
          اگر (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->برداشت_discard (); بازگشت نادرست; }
        }
        c->serializer->add_link (out->color_سطر_2, c->serializer->برداشت_pack ());
        بازگشت !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    بازگشت نادرست;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    اگر (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (دوگانه) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (دوگانه) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((دوگانه) x0 + (دوگانه) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((دوگانه) y0 + (دوگانه) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((دوگانه) radius0 + (دوگانه) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((دوگانه) x1 + (دوگانه) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((دوگانه) y1 + (دوگانه) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((دوگانه) radius1 + (دوگانه) roundf (instancer (varIdxBase, 5)));
    }

    اگر (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_سطر_2.serialize_subset (c, color_سطر_2, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && color_سطر_2.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    hb_color_سطر_t cl = {
      (تهی *) &(این+color_سطر_2),
      (این+color_سطر_2).static_get_color_stops, c,
      (این+color_سطر_2).static_get_extend, nullptr
    };

    c->funcs->radial_gradient (c->data, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       radius0 + c->instancer (varIdxBase, 2),
			       x1 + c->instancer (varIdxBase, 3),
			       y1 + c->instancer (varIdxBase, 4),
			       radius1 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24To<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> رده Var>
ساختار PaintSweepGradient
{
  تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (این+color_سطر_2).closurev1 (c);
    c->رقم_var_idxes = 4;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((دوگانه) centerX + (دوگانه) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((دوگانه) centerY + (دوگانه) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    اگر (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_سطر_2.serialize_subset (c, color_سطر_2, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && color_سطر_2.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    hb_color_سطر_t cl = {
      (تهی *) &(این+color_سطر_2),
      (این+color_سطر_2).static_get_color_stops, c,
      (این+color_سطر_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_سطر<Var>>	color_سطر_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
ساختار PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    return_trace (out->paint.serialize_subset (c, paint, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && paint.sanitize (c, این));
  }

  درونخطی تهی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  عمومی:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

ساختار PaintColrGlyph
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  درونخطی تهی paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  عمومی:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> رده Var>
ساختار PaintTransform
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);
    اگر (!out->transform.serialize_subset (c, transform, این, instancer)) return_trace (نادرست);
    اگر (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  src.sanitize (c, این) &&
                  transform.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (این);
    (این+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  عمومی:
  DEFINE_SIZE_STATIC (7);
};

ساختار PaintTranslate
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((دوگانه) dx + (دوگانه) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((دوگانه) dy + (دوگانه) roundf (instancer (varIdxBase, 1)));
    }

    اگر (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری ddx = dx + c->instancer (varIdxBase, 0);
    اعشاری ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->افزودن_translate (c->data, ddx, ddy);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
ایستا منطقی serialize_scale_transform (hb_subset_context_t *c,
                                       اعشاری sx, اعشاری sy, اعشاری dx, اعشاری dy,
                                       ثابت Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  اگر (unlikely (!out)) بازگشت نادرست;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  اگر (unlikely (!transform))
  {
    c->serializer->برداشت_discard ();
    بازگشت نادرست;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->برداشت_pack ());

  c->serializer->push ();
  منطقی ret = source ();
  اگر (ret)
    c->serializer->add_link (out->src, c->serializer->برداشت_pack ());
  وگرنه
    c->serializer->برداشت_discard ();
  بازگشت ret;
}

template <typename Base>
ایستا منطقی subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت Base *base,
                                      ثابت Offset24To<Paint> &src,
                                      ثابت ItemVarStoreInstancer &instancer,
                                      اعشاری sx, اعشاری sy,
                                      اعشاری cx = 0, اعشاری cy = 0)
{
  اگر (src.is_null ()) بازگشت نادرست;
  auto source = [&] { بازگشت c->dispatch (base+src, instancer); };
  اعشاری dx = cx * (1 - sx), dy = cy * (1 - sy);
  اگر (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    بازگشت serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    بازگشت serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    بازگشت serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    بازگشت serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    بازگشت serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  بازگشت serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

ساختار PaintScale
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      اعشاری sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      اعشاری sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      اگر (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, این, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    اگر (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    اعشاری sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->افزودن_scale (c->data, sx, sy);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ساختار PaintScaleAroundCenter
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      اعشاری sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      اعشاری sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      اگر (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, این, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((دوگانه) centerX + (دوگانه) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((دوگانه) centerY + (دوگانه) roundf (instancer (varIdxBase, 3)));
    }

    اگر (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    اعشاری sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    اعشاری tCenterX = centerX + c->instancer (varIdxBase, 2);
    اعشاری tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->افزودن_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ساختار PaintScaleUniform
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      اعشاری s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      اگر (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, این, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    اگر (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->افزودن_scale (c->data, s, s);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عمومی:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ساختار PaintScaleUniformAroundCenter
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      اعشاری s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      اگر (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, این, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((دوگانه) centerX + (دوگانه) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((دوگانه) centerY + (دوگانه) roundf (instancer (varIdxBase, 2)));
    }

    اگر (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری s = scale.to_float (c->instancer (varIdxBase, 0));
    اعشاری tCenterX = centerX + c->instancer (varIdxBase, 1);
    اعشاری tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->افزودن_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عمومی:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ساختار PaintRotate
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    اگر (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->افزودن_rotate (c->data, a);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عمومی:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ساختار PaintRotateAroundCenter
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((دوگانه) centerX + (دوگانه) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((دوگانه) centerY + (دوگانه) roundf (instancer (varIdxBase, 2)));
    }

    اگر (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری a = angle.to_float (c->instancer (varIdxBase, 0));
    اعشاری tCenterX = centerX + c->instancer (varIdxBase, 1);
    اعشاری tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->افزودن_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عمومی:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ساختار PaintSkew
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    اگر (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    اعشاری sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->افزودن_skew (c->data, sx, sy);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ساختار PaintSkewAroundCenter
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((دوگانه) centerX + (دوگانه) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((دوگانه) centerY + (دوگانه) roundf (instancer (varIdxBase, 3)));
    }

    اگر (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, این, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && src.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (این);
    اعشاری sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    اعشاری sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    اعشاری tCenterX = centerX + c->instancer (varIdxBase, 2);
    اعشاری tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->افزودن_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (این+src);
    c->funcs->برداشت_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عمومی:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ساختار PaintComposite
{
  HB_INTERNAL تهی dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    منطقی ret = نادرست;
    ret |= out->src.serialize_subset (c, src, این, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, این, instancer);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  c->check_ops (این->min_size) && // PainComposite can get exponential
                  src.sanitize (c, این) &&
                  backdrop.sanitize (c, این));
  }

  تهی paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (این);
    hb_paint_composite_mode_t composite_mode;
    انتخاب ((unsigned صحیح) mode)
    {
      حالت HB_PAINT_COMPOSITE_MODE_CLEAR:
      حالت HB_PAINT_COMPOSITE_MODE_SRC:
      حالت HB_PAINT_COMPOSITE_MODE_DEST:
      حالت HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      حالت HB_PAINT_COMPOSITE_MODE_SRC_IN:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_IN:
      حالت HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      حالت HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      حالت HB_PAINT_COMPOSITE_MODE_XOR:
      حالت HB_PAINT_COMPOSITE_MODE_PLUS:
      حالت HB_PAINT_COMPOSITE_MODE_SCREEN:
      حالت HB_PAINT_COMPOSITE_MODE_OVERLAY:
      حالت HB_PAINT_COMPOSITE_MODE_DARKEN:
      حالت HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      حالت HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      حالت HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      حالت HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      حالت HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      حالت HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      حالت HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      حالت HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      حالت HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      حالت HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      حالت HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      حالت HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned صحیح) mode;
	بشکن;
      پیشفرض:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	بشکن;
    }

    c->funcs->افزودن_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (این+backdrop);
    c->funcs->افزودن_group_for (c->data, composite_mode);
    c->recurse (این+src);
    c->funcs->برداشت_group (c->data, composite_mode);
    c->funcs->برداشت_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار ClipBoxData
{
  صحیح xMin, yMin, xMax, yMax;
};

ساختار ClipBoxFormat1
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_clip_box (ClipBoxData &clip_box, ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((دوگانه) xMin + (دوگانه) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((دوگانه) yMin + (دوگانه) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((دوگانه) xMax + (دوگانه) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((دوگانه) yMax + (دوگانه) ceilf (instancer (varIdxBase, 3)));
    }

    اگر (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (درست);
  }

  عمومی:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عمومی:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

ساختار ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  تهی get_clip_box (ClipBoxData &clip_box, ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    اگر (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<صحیح> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<صحیح> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<صحیح> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<صحیح> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

ساختار ClipBox
{
  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالت 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  تهی closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 2: hb_barrier (); u.format2.closurev1 (c); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  منطقی get_extents (hb_glyph_extents_t *extents,
                    ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    ClipBoxData clip_box;
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      بشکن;
    حالت 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      بشکن;
    پیشفرض:
      بازگشت نادرست;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    بازگشت درست;
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

ساختار ClipRecord
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  تهی closurev1 (hb_colrv1_closure_context_t* c, ثابت تهی *base) ثابت
  {
    اگر (!c->glyphs->intersects (startGlyphID, endGlyphID)) بازگشت;
    (base+clipBox).closurev1 (c);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت تهی *base,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && clipBox.sanitize (c, base));
  }

  منطقی get_extents (hb_glyph_extents_t *extents,
		    ثابت تهی *base,
		    ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    بازگشت (base+clipBox).get_extents (extents, instancer);
  }

  عمومی:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  عمومی:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

ساختار ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ثابت ItemVarStoreInstancer &instancer,
                                   ثابت hb_set_t& gids,
                                   ثابت hb_map_t& gid_offset_map) ثابت
  {
    TRACE_SERIALIZE (این);
    اگر (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned شمار  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    برای (ثابت hb_codepoint_t _ : gids.iter ())
    {
      اگر (_ == start_gid) ادامه;

      offset = gid_offset_map.get (_);
      اگر (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        ادامه;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      اگر (!record.subset (c, این, instancer)) return_trace (0);
      شمار++;

      start_gid = _;
      prev_gid = _;
      prev_offset = offset;
    }

    //last one
    {
      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;
      اگر (!record.subset (c, این, instancer)) return_trace (0);
      شمار++;
    }
    return_trace (شمار);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    اگر (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (نادرست);

    ثابت hb_set_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    برای (ثابت ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      برای (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        اگر (!glyphset.has (gid) || !glyph_map.has (gid)) ادامه;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned شمار = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    اگر (!شمار) return_trace (نادرست);
    return_trace (c->serializer->check_assign (out->clips.len, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (این) && clips.sanitize (c, این));
  }

  منطقی
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    auto *rec = clips.as_array ().bsearch (gid);
    اگر (rec)
      بازگشت rec->get_extents (extents, این, instancer);
    بازگشت نادرست;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

ساختار Paint
{

  template <typename ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_مقدار ());

    return_trace (c->end_recursion (این->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
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
#ifndef HB_NO_BEYOND_64K
    حالت 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  منطقی get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    انتخاب (u.format.v) {
    حالت 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); بازگشت درست;
    حالت 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); بازگشت درست;
    پیشفرض: بازگشت نادرست;
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<PaintLinearGradient<NoVariable>>	paintformat4;
  Variable<PaintLinearGradient<Variable>>	paintformat5;
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
  NoVariable<PaintScaleUniform>			paintformat20;
  Variable<PaintScaleUniform>			paintformat21;
  NoVariable<PaintScaleUniformAroundCenter>	paintformat22;
  Variable<PaintScaleUniformAroundCenter>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  PaintComposite				paintformat32;
#ifndef HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_MIN (2);
};

ساختار BaseGlyphPaintRecord
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقی serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت تهی* src_base, hb_subset_context_t *c,
                  ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = s->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);
    اگر (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) && paint.sanitize (c, base)));
  }

  عمومی:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out)))  return_trace (نادرست);
    ثابت hb_set_t* glyphset = &c->plan->_glyphset_colred;

    برای (ثابت auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      اگر (!glyphset->has (gid)) ادامه;

      اگر (_.serialize (c->serializer, c->plan->glyph_map, این, c, instancer)) out->len++;
      وگرنه return_trace (نادرست);
    }

    return_trace (out->len != 0);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, این));
  }
};

ساختار LayerList : Array32OfOffset32To<Paint>
{
  ثابت Paint& get_paint (unsigned i) ثابت
  { بازگشت این+(*این)[i]; }

  منطقی subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out)))  return_trace (نادرست);

    منطقی ret = نادرست;
    برای (ثابت auto& _ : + hb_enumerate (*این)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (نادرست);
      ret |= o->serialize_subset (c, _.second, این, instancer);
    }
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, این));
  }
};

ساختار delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_شمار () ثابت { بازگشت inner_bit_شمار; }
  unsigned get_width ()           ثابت { بازگشت ((outer_bit_شمار + inner_bit_شمار + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { بازگشت output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_شمار = 0;
    outer_bit_شمار = 0;
    inner_bit_شمار = 1;
    output_map.init ();

    /* search backwards */
    unsigned شمار = new_deltaset_idx_varidx_map.get_population ();
    اگر (!شمار) بازگشت;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    برای (unsigned i = شمار; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      اگر (i == شمار)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        ادامه;
      }
      اگر (var_idx != last_varidx)
        بشکن;
      last_idx = delta_set_idx;
    }

    map_شمار = last_idx + 1;
  }

  منطقی remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_شمار = 1;
    inner_bit_شمار = 1;

    اگر (unlikely (!output_map.resize_dirty (map_شمار))) بازگشت نادرست;

    برای (unsigned idx = 0; idx < map_شمار; idx++)
    {
      uint32_t *var_idx;
      اگر (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) بازگشت نادرست;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_شمار = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_شمار = hb_max (bit_شمار, outer_bit_شمار);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_شمار = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_شمار = hb_max (bit_شمار, inner_bit_شمار);
    }
    بازگشت درست;
  }

  خصوصی:
  unsigned map_شمار;
  unsigned outer_bit_شمار;
  unsigned inner_bit_شمار;
  hb_vector_t<uint32_t> output_map;
};

ساختار COLR
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  منطقی has_data () ثابت { بازگشت has_v0_data () || version; }

  منطقی has_v0_data () ثابت { بازگشت رقم_Base_Glyphs; }
  منطقی has_v1_data () ثابت
  {
    اگر (version < 1)
      بازگشت نادرست;
    hb_barrier ();

    بازگشت (این+baseGlyphList).len > 0;
  }

  unsigned صحیح get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned صحیح         start_offset,
				 unsigned صحیح        *شمار, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت BaseGlyphRecord &record = (این+baseGlyphsZ).bsearch (رقم_Base_Glyphs, glyph);

    hb_array_t<ثابت LayerRecord> all_layers = (این+layersZ).as_array (رقم_Layers);
    hb_array_t<ثابت LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.رقم_Layers);
    اگر (شمار && layers)
    {
      + glyph_layers.sub_array (start_offset, شمار)
      | hb_sink (hb_array (layers, *شمار))
      ;
    }
    بازگشت glyph_layers.length;
  }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      اگر (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقی has_data () ثابت { بازگشت colr->has_data (); }

#ifndef HB_NO_PAINT
    منطقی
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      اگر (unlikely (!has_data ())) بازگشت نادرست;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) بازگشت درست;
      منطقی ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      بازگشت ret;
    }

    منطقی paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, تهی *data,
		      unsigned صحیح palette_index,
		      hb_color_t foreground,
		      منطقی clip = درست) ثابت
    {
      اگر (unlikely (!has_data ())) بازگشت نادرست;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      اگر (unlikely (!scratch)) بازگشت درست;
      منطقی ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      بازگشت ret;
    }
#endif

    منطقی is_valid () { بازگشت colr.get_blob ()->length; }

    تهی depend (hb_depend_data_builder_t *depend_data) ثابت
    {
      اگر (!has_data ()) بازگشت;
      colr->depend (depend_data);
    }

    تهی closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ثابت
    { colr->closure_glyphs (glyph, related_ids); }

    تهی closure_V0palette_indices (ثابت hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ثابت
    { colr->closure_V0palette_indices (glyphs, palettes); }

    تهی closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ثابت
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    منطقی has_var_store () ثابت
    { بازگشت colr->has_var_store (); }

    ثابت ItemVariationStore &get_var_store () ثابت
    { بازگشت colr->get_var_store (); }
    ثابت ItemVariationStore *get_var_store_ptr () ثابت
    { بازگشت colr->get_var_store_ptr (); }

    منطقی has_delta_set_index_map () ثابت
    { بازگشت colr->has_delta_set_index_map (); }

    ثابت DeltaSetIndexMap &get_delta_set_index_map () ثابت
    { بازگشت colr->get_delta_set_index_map (); }
    ثابت DeltaSetIndexMap *get_delta_set_index_map_ptr () ثابت
    { بازگشت colr->get_delta_set_index_map_ptr (); }

    خصوصی:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      اگر (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, اندازه (hb_colr_scratch_t));
	اگر (unlikely (!scratch))
	  بازگشت nullptr;
      }

      بازگشت scratch;
    }
    تهی release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      اگر (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عمومی:
    hb_blob_ptr_t<COLR> colr;
    خصوصی:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  تهی depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    // v0
    hb_array_t<ثابت BaseGlyphRecord> baseGlyphs = (این+baseGlyphsZ).as_array (رقم_Base_Glyphs);
    برای (ثابت BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (این+layersZ).as_array (رقم_Layers).sub_array (record.firstLayerIdx,
                                                                         record.رقم_Layers);
      برای (ثابت LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    اگر (!has_v1_data())
      بازگشت;

    // v1
    hb_colrv1_depend_context_t c (این, depend_data);
    ثابت BaseGlyphList &baseglyph_paintrecords = این+baseGlyphList;

    برای (ثابت BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  تهی closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ثابت
  {
    ثابت BaseGlyphRecord *record = get_base_glyph_record (glyph);
    اگر (!record) بازگشت;

    auto glyph_layers = (این+layersZ).as_array (رقم_Layers).sub_array (record->firstLayerIdx,
								       record->رقم_Layers);
    اگر (!glyph_layers.length) بازگشت;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  تهی closure_V0palette_indices (ثابت hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ثابت
  {
    اگر (!رقم_Base_Glyphs || !رقم_Layers) بازگشت;
    hb_array_t<ثابت BaseGlyphRecord> baseGlyphs = (این+baseGlyphsZ).as_array (رقم_Base_Glyphs);
    hb_array_t<ثابت LayerRecord> all_layers = (این+layersZ).as_array (رقم_Layers);

    برای (ثابت BaseGlyphRecord record : baseGlyphs)
    {
      اگر (!glyphs->has (record.glyphId)) ادامه;
      hb_array_t<ثابت LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.رقم_Layers);
      برای (ثابت LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  تهی closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ثابت
  {
    اگر (version < 1) بازگشت;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (این, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت BaseGlyphList &baseglyph_paintrecords = این+baseGlyphList;

    برای (ثابت BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      اگر (!glyphset->has (gid)) ادامه;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ثابت ClipList &cliplist = این+clipList;
    c.glyphs = glyphset;
    برای (ثابت ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    اگر (has_delta_set_index_map ())
    {
      ثابت DeltaSetIndexMap &var_idx_map = این+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      برای (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { بازگشت (این+layerList); }

  ثابت BaseGlyphList& get_baseglyphList () ثابت
  { بازگشت (این+baseGlyphList); }

  منطقی has_var_store () ثابت
  { بازگشت version >= 1 && hb_barrier () && varStore != 0; }

  منطقی has_delta_set_index_map () ثابت
  { بازگشت version >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقی has_clip_list () ثابت
  { بازگشت version >= 1 && hb_barrier () && clipList != 0; }

  ثابت DeltaSetIndexMap &get_delta_set_index_map () ثابت
  { بازگشت has_delta_set_index_map () && hb_barrier () ? این+varIdxMap : Null (DeltaSetIndexMap); }
  ثابت DeltaSetIndexMap *get_delta_set_index_map_ptr () ثابت
  { بازگشت has_delta_set_index_map () && hb_barrier () ? &(این+varIdxMap) : nullptr; }

  ثابت ItemVariationStore &get_var_store () ثابت
  { بازگشت has_var_store () && hb_barrier () ? این+varStore : Null (ItemVariationStore); }
  ثابت ItemVariationStore *get_var_store_ptr () ثابت
  { بازگشت has_var_store () && hb_barrier () ? &(این+varStore) : nullptr; }

  ثابت ClipList &get_clip_list () ثابت
  { بازگشت has_clip_list () && hb_barrier () ? این+clipList : Null (ClipList); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
                  (این+baseGlyphsZ).sanitize (c, رقم_Base_Glyphs) &&
                  (این+layersZ).sanitize (c, رقم_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, این) &&
		    layerList.sanitize (c, این) &&
		    clipList.sanitize (c, این) &&
		    varIdxMap.sanitize (c, این) &&
		    varStore.sanitize (c, این))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقی serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (base_it.len () != layer_it.len ()))
      return_trace (نادرست);

    این->version = version;
    رقم_Layers = 0;
    رقم_Base_Glyphs = base_it.len ();
    اگر (رقم_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (درست);
    }

    c->push ();
    برای (ثابت hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      اگر (unlikely (!record)) return_trace (نادرست);
      record->firstLayerIdx = رقم_Layers;
      رقم_Layers += record->رقم_Layers;
    }
    c->add_link (baseGlyphsZ, c->برداشت_pack ());

    c->push ();
    برای (ثابت hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->برداشت_pack ());

    return_trace (درست);
  }

  ثابت BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ثابت
  {
    ثابت BaseGlyphRecord* record = &(این+baseGlyphsZ).bsearch (رقم_Base_Glyphs, (unsigned صحیح) gid);
    اگر (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    بازگشت record;
  }

  ثابت BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت BaseGlyphList &list = این+baseGlyphList;
    unsigned صحیح i;
    اگر (!list.bfind ((unsigned) gid, &i))
      بازگشت nullptr;
    بازگشت &list[i];
  }

  منطقی downgrade_to_V0 (ثابت hb_set_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    برای (ثابت BaseGlyphPaintRecord& _ : get_baseglyphList ())
      اگر (glyphset.has (_.glyphId))
        بازگشت نادرست;

    بازگشت درست;
  }

  منطقی subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (درست);

    ثابت ItemVariationStore& var_store = این+varStore;
    اگر (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقی optimize = (varIdxMap != 0) ? درست : نادرست;
      اگر (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (نادرست);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      اگر (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (نادرست);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      اگر (optimize)
      {
        ثابت hb_map_t &varidx_map = item_vars.get_varidx_map ();
        برای (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          اگر (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          وگرنه
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    وگرنه
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      اگر (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      اگر (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  منطقی subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (درست);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    اگر (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (نادرست);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ثابت hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->رقم_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    اگر (glyphset.has (old_gid)) بازگشت درست;
		    بازگشت نادرست;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ثابت BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				اگر (unlikely (!old_record))
				  بازگشت hb_pair_t<منطقی, BaseGlyphRecord> (نادرست, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.رقم_Layers = old_record->رقم_Layers;
				بازگشت hb_pair_t<منطقی, BaseGlyphRecord> (درست, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->رقم_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ثابت BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				اگر (unlikely (!old_record ||
					      old_record->firstLayerIdx >= رقم_Layers ||
					      old_record->firstLayerIdx + old_record->رقم_Layers > رقم_Layers))
				  بازگشت hb_pair_t<منطقی, hb_vector_t<LayerRecord>> (نادرست, out_layers);

				auto layers = (این+layersZ).as_array (رقم_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->رقم_Layers);
				out_layers.resize (layers.length);
				برای (unsigned صحیح i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  اگر (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    بازگشت hb_pair_t<منطقی, hb_vector_t<LayerRecord>> (نادرست, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				بازگشت hb_pair_t<منطقی, hb_vector_t<LayerRecord>> (درست, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    اگر (version == 0 && (!base_it || !layer_it))
      return_trace (نادرست);

    auto *colr_اول = c->serializer->start_embed<COLR> ();
    اگر (unlikely (!c->serializer->extend_min (colr_اول)))  return_trace (نادرست);

    اگر (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_اول->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    اگر (!c->serializer->allocate_size<تهی> (5 * HBUINT32::static_size)) return_trace (نادرست);
    اگر (!colr_اول->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (نادرست);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    اگر (!subset_varstore (c, colr_اول)) return_trace (نادرست);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    اگر (!colr_اول->baseGlyphList.serialize_subset (c, baseGlyphList, این, instancer))
      return_trace (نادرست);

    colr_اول->layerList.serialize_subset (c, layerList, این, instancer);
    colr_اول->clipList.serialize_subset (c, clipList, این, instancer);

    return_trace (subset_delta_set_index_map (c, colr_اول));
  }

  ثابت Paint *get_base_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت BaseGlyphList &baseglyph_paintrecords = این+baseGlyphList;
    ثابت BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    اگر (record)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+record->paint;
      بازگشت &paint;
    }
    وگرنه
      بازگشت nullptr;
  }

#ifndef HB_NO_PAINT
  منطقی
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->رقم_coords));

    اگر (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      بازگشت درست;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقی ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), درست, scratch);

    auto e = scratch.paint_extents.get_extents ();
    اگر (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    وگرنه
      *extents = e.to_glyph_extents ();

    بازگشت ret;
  }
#endif

  منطقی
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    اگر (version >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_base_glyph_paint (glyph);

      بازگشت paint != nullptr;
    }

    بازگشت نادرست;
  }

  منطقی get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت ItemVarStoreInstancer instancer) ثابت
  {
    بازگشت get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  منطقی
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, تهی *data,
	       unsigned صحیح palette_index, hb_color_t foreground,
	       منطقی clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->رقم_coords : 0));
    hb_paint_context_t c (این, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_گره_t node (c.glyphs_decycler);
    node.visit (glyph);

    اگر (version >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_base_glyph_paint (glyph);
      اگر (paint)
      {
        // COLRv1 glyph

	منطقی is_bounded = درست;
	hb_glyph_extents_t extents = {};
	اگر (clip)
	{
	  اگر (!get_clip (glyph, &extents, instancer))
	  {
	    clip = نادرست;
	    is_bounded = نادرست;
	  }

	  اگر (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 نادرست,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->افزودن_font_transform (c.data, font);

	اگر (clip)
	  c.funcs->افزودن_clip_rectangle (c.data,
					(اعشاری) extents.x_bearing,
					(اعشاری) extents.y_bearing + extents.height,
					(اعشاری) extents.x_bearing + extents.width,
					(اعشاری) extents.y_bearing);

	اگر (is_bounded)
	  c.recurse (*paint);

	اگر (clip)
	  c.funcs->برداشت_clip (c.data);

	c.funcs->برداشت_transform (c.data);

        بازگشت درست;
      }
    }

    ثابت BaseGlyphRecord *record = get_base_glyph_record (glyph);
    اگر (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      برای (ثابت auto &r : (این+layersZ).as_array (رقم_Layers)
			   .sub_array (record->firstLayerIdx, record->رقم_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      بازگشت درست;
    }

    بازگشت نادرست;
  }
#endif

  محافظتشده:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	رقم_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	رقم_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  عمومی:
  DEFINE_SIZE_MIN (14);
};

ساختار COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

تهی
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  اگر (unlikely (depth_چپ <= 0 || edge_شمار <= 0)) بازگشت;
  depth_چپ--;
  edge_شمار--;
  paint.dispatch (این);
  depth_چپ++;
}

template <typename Types>
تهی PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (این);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  اگر (likely (c->depth_چپ > 0 && c->edge_شمار > 0) &&
      (این+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_شمار--;
    c->funcs->افزودن_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->برداشت_transform (c->data);
    بازگشت;
  }

  c->funcs->افزودن_inverse_font_transform (c->data, c->font);
  c->funcs->افزودن_clip_glyph (c->data, gid, c->font);
  c->funcs->افزودن_font_transform (c->data, c->font);
  c->recurse (این+paint);
  c->funcs->برداشت_transform (c->data);
  c->funcs->برداشت_clip (c->data);
  c->funcs->برداشت_transform (c->data);
}

تهی PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (این);
  ثابت LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_گره_t node (c->layers_decycler);
  برای (unsigned i = firstLayerIndex; i < firstLayerIndex + رقم_Layers; i++)
  {
    اگر (unlikely (!node.visit (i)))
      بازگشت;

    ثابت Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

تهی PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (این);

  hb_decycler_گره_t node (c->glyphs_decycler);
  اگر (unlikely (!node.visit (gid)))
    بازگشت;

  c->funcs->افزودن_inverse_font_transform (c->data, c->font);
  اگر (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->برداشت_transform (c->data);
    بازگشت;
  }
  c->funcs->برداشت_transform (c->data);

  ثابت COLR *colr_table = c->get_colr_table ();
  ثابت Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقی has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  اگر (has_clip_box)
    c->funcs->افزودن_clip_rectangle (c->data,
				   (اعشاری) extents.x_bearing,
				   (اعشاری) extents.y_bearing + extents.height,
				   (اعشاری) extents.x_bearing + extents.width,
				   (اعشاری) extents.y_bearing);

  اگر (paint)
    c->recurse (*paint);

  اگر (has_clip_box)
    c->funcs->برداشت_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
