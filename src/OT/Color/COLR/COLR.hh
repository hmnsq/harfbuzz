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
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

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
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.paint_glyph (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_بەها () { أرجع hb_empty_t (); }

  ثابت COLR* get_colr_table () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (base); }

عام:
  ثابت فراغ *base;
  hb_paint_funcs_t *funcs;
  فراغ *data;
  hb_font_t *font;
  hb_array_t<ثابت BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح depth_چەپ = HB_MAX_NESTING_LEVEL;
  صحيح edge_ژماردن = HB_MAX_GRAPH_EDGE_ژماردن;

  hb_paint_context_t (ثابت فراغ *base_,
		      hb_paint_funcs_t *funcs_,
		      فراغ *data_,
                      hb_font_t *font_,
                      unsigned صحيح palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_ژماردن () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, true);
      hb_font_set_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ساكن unsigned
  color_alpha (hb_color_t color, عائم alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    أرجع roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned صحيح color_index, عائم alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = true;

    إذا (color_index != 0xffff)
    {
      إذا (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = خطأ;
    }

    أرجع HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  مضمن فراغ recurse (ثابت Paint &paint);
};

بنية hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_چەپ == 0))
      أرجع hb_empty_t ();

    إذا (paint_visited (&obj))
      أرجع hb_empty_t ();

    nesting_level_چەپ--;
    obj.closurev1 (هذا);
    nesting_level_چەپ++;
    أرجع hb_empty_t ();
  }
  ساكن return_t default_return_بەها () { أرجع hb_empty_t (); }

  منطقي paint_visited (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    إذا (visited_paint.in_error() || visited_paint.has (delta))
      أرجع true;

    visited_paint.add (delta);
    أرجع خطأ;
  }

  ثابت COLR* get_colr_table () ثابت
  { أرجع reinterpret_cast<ثابت COLR *> (base); }

  فراغ add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  فراغ add_layer_indices (unsigned first_layer_index, unsigned ژمارە_of_layers)
  {
    إذا (ژمارە_of_layers == 0) أرجع;
    layer_indices->add_range (first_layer_index, first_layer_index + ژمارە_of_layers - 1);
  }

  فراغ add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  فراغ add_var_idxes (unsigned first_var_idx, unsigned ژمارە_idxes)
  {
    إذا (!ژمارە_idxes || first_var_idx == VarIdx::NO_VARIATION) أرجع;
    variation_indices->add_range (first_var_idx, first_var_idx + ژمارە_idxes - 1);
  }

  عام:
  ثابت فراغ *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned ژمارە_var_idxes;
  unsigned nesting_level_چەپ;

  hb_colrv1_closure_context_t (ثابت فراغ *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned ژمارە_var_idxes_2 = 1,
                               unsigned nesting_level_چەپ_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          ژمارە_var_idxes (ژمارە_var_idxes_2),
                          nesting_level_چەپ (nesting_level_چەپ_2)
  {}
};


بنية hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    إذا (unlikely (nesting_level_چەپ == 0))
      أرجع hb_empty_t ();

    إذا (paint_visited (&obj))
      أرجع hb_empty_t ();

    nesting_level_چەپ--;
    obj.dependv1 (هذا);
    nesting_level_چەپ++;
    أرجع hb_empty_t ();
  }
  ساكن return_t default_return_بەها () { أرجع hb_empty_t (); }

  منطقي paint_visited (ثابت فراغ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    إذا (visited_paint.in_error() || visited_paint.has (delta))
      أرجع true;

    visited_paint.add (delta);
    إذا (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    أرجع خطأ;
  }

  ثابت COLR* get_colr_table () ثابت
  { أرجع colr; }

  عام:
  ثابت COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_چەپ;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ثابت COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_چەپ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_چەپ (nesting_level_چەپ_2),
                          source_gid (0)
  {}
};

بنية LayerRecord
{
  operator hb_ot_color_layer_t () ثابت { أرجع {glyphId, colorIdx}; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
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
  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية BaseGlyphRecord
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
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	ژمارە_Layers;	/* Number of color layers
				 * associated with this glyph */
  عام:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
بنية Variable
{
  ساكن constexpr منطقي is_variable = true;

  ثابت T& get_بەها () ثابت { أرجع value; }

  Variable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->ژمارە_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->ژمارە_var_idxes);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!value.subset (c, instancer, varIdxBase)) return_trace (خطأ);
    إذا (c->plan->all_axes_pinned)
      return_trace (true);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    إذا (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, صحيح> *new_varidx_delta;
      إذا (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (خطأ);

      new_varidx = hb_first (*new_varidx_delta);
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
    value.paint_glyph (c, varIdxBase);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  فراغ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    أرجع value.get_extend ();
  }

  محمي:
  T      value;
  عام:
  VarIdx varIdxBase;
  عام:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
بنية NoVariable
{
  ساكن constexpr منطقي is_variable = خطأ;

  ثابت T& get_بەها () ثابت { أرجع value; }

  ساكن constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت
  { value.dependv1 (c); }

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { value.closurev1 (c); }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    value.paint_glyph (c, varIdxBase);
  }

  فراغ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ثابت ItemVarStoreInstancer &instancer) ثابت
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
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
ساكن فراغ
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, عائم value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

بنية ColorStop
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->add_palette_index (paletteIndex);
    c->ژمارە_var_idxes = 2;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      عائم a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_color_stop (hb_paint_context_t *c,
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
  عام:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

بنية Extend : HBUINT8
{
  تعداد {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  عام:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> صنف Var>
بنية Color_هێڵ
{
  فراغ dependv1 (hb_colrv1_depend_context_t * c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    لكل (ثابت auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    إذا (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (خطأ);
    إذا (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (خطأ);

    لكل (ثابت auto& stop : stops.iter ())
    {
      إذا (!stop.subset (c, instancer)) return_trace (خطأ);
    }
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned صحيح
  get_color_stops (hb_paint_context_t *c,
                   unsigned صحيح start,
		   unsigned صحيح *ژماردن,
		   hb_color_stop_t *color_stops,
		   ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    unsigned صحيح len = stops.len;

    إذا (ژماردن && color_stops)
    {
      unsigned صحيح i;
      لكل (i = 0; i < *ژماردن && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *ژماردن = i;
    }

    أرجع len;
  }

  HB_INTERNAL ساكن unsigned صحيح static_get_color_stops (hb_color_هێڵ_t *color_هێڵ,
							  فراغ *color_هێڵ_data,
							  unsigned صحيح start,
							  unsigned صحيح *ژماردن,
							  hb_color_stop_t *color_stops,
							  فراغ *user_data)
  {
    ثابت Color_هێڵ *thiz = (ثابت Color_هێڵ *) color_هێڵ_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    أرجع thiz->get_color_stops (c, start, ژماردن, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ثابت
  {
    بدل ((unsigned صحيح) extend)
    {
      حالة Extend::EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
      حالة Extend::EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
      حالة Extend::EXTEND_PAD:
      افتراضي:                     أرجع HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL ساكن hb_paint_extend_t static_get_extend (hb_color_هێڵ_t *color_هێڵ,
							  فراغ *color_هێڵ_data,
							  فراغ *user_data)
  {
    ثابت Color_هێڵ *thiz = (ثابت Color_هێڵ *) color_هێڵ_data;
    أرجع thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  عام:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
بنية CompositeMode : HBUINT8
{
  عام:
  DEFINE_SIZE_STATIC (1);
};

بنية Affine2x3
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { c->ژمارە_var_idxes = 6; }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((مزدوج) source.to_int () +
                                                    (مزدوج) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (true);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    c->funcs->پاڵنان_transform (c->data,
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
  عام:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

بنية PaintColrLayers
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    uint32_t first_layer_index = ژمارە_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8	format; /* format = 1 */
  HBUINT8	ژمارە_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية PaintSolid
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}
  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    c->add_palette_index (paletteIndex);
    c->ژمارە_var_idxes = 1;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      عائم a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    إذا (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  عام:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> صنف Var>
بنية PaintLinearGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (هذا+color_هێڵ_2).closurev1 (c);
    c->ژمارە_var_idxes = 6;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((مزدوج) x2 + (مزدوج) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((مزدوج) y2 + (مزدوج) roundf (instancer (varIdxBase, 5)));
    }

    إذا (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_هێڵ_2.serialize_subset (c, color_هێڵ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_هێڵ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_هێڵ_t cl = {
      (فراغ *) &(هذا+color_هێڵ_2),
      (هذا+color_هێڵ_2).static_get_color_stops, c,
      (هذا+color_هێڵ_2).static_get_extend, nullptr
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
  Offset24To<Color_هێڵ<Var>>	color_هێڵ_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  عام:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> صنف Var>
بنية PaintRadialGradient
{
  منطقي constant_after_pinning (hb_subset_context_t *c,
                               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    إذا (c->plan->has_avar2) أرجع خطأ;
    ثابت auto &regions = instancer.varStore->get_region_list ();
    لكل (unsigned i = 0; i < regions.region_ژماردن; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      إذا (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        أرجع خطأ;
      }
      لكل (auto axis : axes)
      {
        Triple *limit;
        إذا (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          أرجع خطأ;
      }
    }
    أرجع true;
  }

  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (هذا+color_هێڵ_2).closurev1 (c);
    c->ژمارە_var_idxes = 6;
  }

  منطقي subset_nonnegative (hb_subset_context_t *c,
                           ثابت ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ثابت
  {
    مزدوج source[6] = {
      x0 + (مزدوج) roundf (instancer (varIdxBase, 0)),
      y0 + (مزدوج) roundf (instancer (varIdxBase, 1)),
      radius0 + (مزدوج) roundf (instancer (varIdxBase, 2)),
      x1 + (مزدوج) roundf (instancer (varIdxBase, 3)),
      y1 + (مزدوج) roundf (instancer (varIdxBase, 4)),
      radius1 + (مزدوج) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      إذا (!out) أرجع خطأ;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      أرجع true;
    };
    ثابت auto &هێڵ = هذا+color_هێڵ_2;
    إذا (source[2] == source[5] || !هێڵ.stops.len) أرجع transparent ();
    بنية stop_t { مزدوج offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    إذا (!stops.alloc (هێڵ.stops.len)) أرجع خطأ;
    unsigned order = 0;
    لكل (ثابت auto &entry : هێڵ.stops.iter ())
    {
      ثابت auto &stop = entry.get_بەها ();
      stops.push (stop_t {(مزدوج) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (مزدوج) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    إذا (stops.in_error ()) أرجع خطأ;
    stops.qsort ([] (ثابت stop_t &a, ثابت stop_t &b) {
      أرجع a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    مزدوج dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    لكل (unsigned j = 0; j < 3; j++)
    {
      مزدوج remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      إذا (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        أرجع خطأ;
      }
      unsigned ڕاست = (unsigned) remainder;
      طالما (ڕاست) { unsigned r = lattice % ڕاست; lattice = ڕاست; ڕاست = r; }
    }
    لكل (unsigned shift = 0; shift < 16; shift++)
    {
      مزدوج b = 1u << shift;
      مزدوج coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      مزدوج fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      مزدوج candidates[] = {coarse, fine};
      لكل (مزدوج a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        مزدوج geometry[6];
        منطقي fits = true;
        لكل (unsigned j = 0; j < 3; j++)
        {
          مزدوج difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          مزدوج نزم = j == 2 ? 0 : -32768, بەرز = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= نزم && geometry[j] <= بەرز &&
                  geometry[j + 3] >= نزم && geometry[j + 3] <= بەرز;
        }
        إذا (!fits) استمر;
        unsigned first = 0, ژماردن = stops.length;
        مزدوج min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        مزدوج bias = 0;
        منطقي constant = خطأ;
        إذا (هێڵ.extend == Extend::EXTEND_PAD)
        {
          مزدوج tip = -geometry[2] / (b * dr);
          إذا ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            ژماردن = 1; constant = true;
          }
        }
        وإلا
        {
          مزدوج period = max - min;
          إذا (هێڵ.extend == Extend::EXTEND_REFLECT) period *= 2;
          إذا (period <= 0) أرجع transparent ();
          مزدوج n = ceil ((-2 - min) / period);
          إذا (n > floor ((32767. / 16384. - max) / period)) استمر;
          bias = n * period;
        }
        لكل (unsigned j = 0; j < ژماردن; j++)
        {
          مزدوج bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        إذا (!fits) استمر;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        إذا (!out) أرجع خطأ;
        out->format = 6;
        out->x0 = (صحيح) geometry[0]; out->y0 = (صحيح) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (صحيح) geometry[3]; out->y1 = (صحيح) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_هێڵ = c->serializer->allocate_size<Color_هێڵ<NoVariable>> (3 + ژماردن * ColorStop::static_size);
        إذا (!out_هێڵ) { c->serializer->دەرهێنان_discard (); أرجع خطأ; }
        out_هێڵ->extend = هێڵ.extend;
        out_هێڵ->stops.len = ژماردن;
        لكل (unsigned j = 0; j < ژماردن; j++)
        {
          ثابت auto &stop = stops[first + j];
          auto &out_stop = out_هێڵ->stops[j].value;
          out_stop.stopOffset.set_float ((عائم) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((عائم) stop.alpha);
          إذا (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->دەرهێنان_discard (); أرجع خطأ; }
        }
        c->serializer->add_link (out->color_هێڵ_2, c->serializer->دەرهێنان_pack ());
        أرجع !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (مزدوج) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (مزدوج) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((مزدوج) x0 + (مزدوج) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((مزدوج) y0 + (مزدوج) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((مزدوج) radius0 + (مزدوج) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((مزدوج) x1 + (مزدوج) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((مزدوج) y1 + (مزدوج) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((مزدوج) radius1 + (مزدوج) roundf (instancer (varIdxBase, 5)));
    }

    إذا (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_هێڵ_2.serialize_subset (c, color_هێڵ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_هێڵ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_هێڵ_t cl = {
      (فراغ *) &(هذا+color_هێڵ_2),
      (هذا+color_هێڵ_2).static_get_color_stops, c,
      (هذا+color_هێڵ_2).static_get_extend, nullptr
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
  Offset24To<Color_هێڵ<Var>>	color_هێڵ_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  عام:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> صنف Var>
بنية PaintSweepGradient
{
  فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت {}

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    (هذا+color_هێڵ_2).closurev1 (c);
    c->ژمارە_var_idxes = 4;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    إذا (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_هێڵ_2.serialize_subset (c, color_هێڵ_2, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && color_هێڵ_2.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    hb_color_هێڵ_t cl = {
      (فراغ *) &(هذا+color_هێڵ_2),
      (هذا+color_هێڵ_2).static_get_color_stops, c,
      (هذا+color_هێڵ_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_هێڵ<Var>>	color_هێڵ_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
بنية PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    return_trace (out->paint.serialize_subset (c, paint, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && paint.sanitize (c, هذا));
  }

  مضمن فراغ paint_glyph (hb_paint_context_t *c) ثابت;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  عام:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

بنية PaintColrGlyph
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
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
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> صنف Var>
بنية PaintTransform
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    إذا (!out->transform.serialize_subset (c, transform, هذا, instancer)) return_trace (خطأ);
    إذا (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
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
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  عام:
  DEFINE_SIZE_STATIC (7);
};

بنية PaintTranslate
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((مزدوج) dx + (مزدوج) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((مزدوج) dy + (مزدوج) roundf (instancer (varIdxBase, 1)));
    }

    إذا (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم ddx = dx + c->instancer (varIdxBase, 0);
    عائم ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->پاڵنان_translate (c->data, ddx, ddy);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
ساكن منطقي serialize_scale_transform (hb_subset_context_t *c,
                                       عائم sx, عائم sy, عائم dx, عائم dy,
                                       ثابت Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  إذا (unlikely (!out)) أرجع خطأ;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  إذا (unlikely (!transform))
  {
    c->serializer->دەرهێنان_discard ();
    أرجع خطأ;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->دەرهێنان_pack ());

  c->serializer->push ();
  منطقي ret = source ();
  إذا (ret)
    c->serializer->add_link (out->src, c->serializer->دەرهێنان_pack ());
  وإلا
    c->serializer->دەرهێنان_discard ();
  أرجع ret;
}

template <typename Base>
ساكن منطقي subset_scale_as_transform (hb_subset_context_t *c,
                                      ثابت Base *base,
                                      ثابت Offset24To<Paint> &src,
                                      ثابت ItemVarStoreInstancer &instancer,
                                      عائم sx, عائم sy,
                                      عائم cx = 0, عائم cy = 0)
{
  إذا (src.is_null ()) أرجع خطأ;
  auto source = [&] { أرجع c->dispatch (base+src, instancer); };
  عائم dx = cx * (1 - sx), dy = cy * (1 - sy);
  إذا (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    أرجع serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    أرجع serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    أرجع serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  أرجع serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

بنية PaintScale
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    إذا (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    عائم sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->پاڵنان_scale (c->data, sx, sy);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

بنية PaintScaleAroundCenter
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      عائم sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      عائم sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      إذا (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (varIdxBase, 3)));
    }

    إذا (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    عائم sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    عائم tCenterX = centerX + c->instancer (varIdxBase, 2);
    عائم tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->پاڵنان_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

بنية PaintScaleUniform
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, هذا, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    إذا (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->پاڵنان_scale (c->data, s, s);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  عام:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

بنية PaintScaleUniformAroundCenter
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      عائم s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      إذا (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, هذا, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (varIdxBase, 2)));
    }

    إذا (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم s = scale.to_float (c->instancer (varIdxBase, 0));
    عائم tCenterX = centerX + c->instancer (varIdxBase, 1);
    عائم tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->پاڵنان_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

بنية PaintRotate
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    إذا (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->پاڵنان_rotate (c->data, a);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  عام:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

بنية PaintRotateAroundCenter
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (varIdxBase, 2)));
    }

    إذا (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم a = angle.to_float (c->instancer (varIdxBase, 0));
    عائم tCenterX = centerX + c->instancer (varIdxBase, 1);
    عائم tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->پاڵنان_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

بنية PaintSkew
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    إذا (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    عائم sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->پاڵنان_skew (c->data, sx, sy);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

بنية PaintSkewAroundCenter
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((مزدوج) centerX + (مزدوج) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((مزدوج) centerY + (مزدوج) roundf (instancer (varIdxBase, 3)));
    }

    إذا (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, هذا, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && src.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ثابت
  {
    TRACE_PAINT (هذا);
    عائم sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    عائم sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    عائم tCenterX = centerX + c->instancer (varIdxBase, 2);
    عائم tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->پاڵنان_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  عام:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

بنية PaintComposite
{
  HB_INTERNAL فراغ dependv1 (hb_colrv1_depend_context_t* c) ثابت;
  HB_INTERNAL فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت;

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= out->src.serialize_subset (c, src, هذا, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, هذا, instancer);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  c->check_ops (هذا->min_size) && // PainComposite can get exponential
                  src.sanitize (c, هذا) &&
                  backdrop.sanitize (c, هذا));
  }

  فراغ paint_glyph (hb_paint_context_t *c) ثابت
  {
    TRACE_PAINT (هذا);
    hb_paint_composite_mode_t composite_mode;
    بدل ((unsigned صحيح) mode)
    {
      حالة HB_PAINT_COMPOSITE_MODE_CLEAR:
      حالة HB_PAINT_COMPOSITE_MODE_SRC:
      حالة HB_PAINT_COMPOSITE_MODE_DEST:
      حالة HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      حالة HB_PAINT_COMPOSITE_MODE_SRC_IN:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_IN:
      حالة HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      حالة HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      حالة HB_PAINT_COMPOSITE_MODE_XOR:
      حالة HB_PAINT_COMPOSITE_MODE_PLUS:
      حالة HB_PAINT_COMPOSITE_MODE_SCREEN:
      حالة HB_PAINT_COMPOSITE_MODE_OVERLAY:
      حالة HB_PAINT_COMPOSITE_MODE_DARKEN:
      حالة HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      حالة HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      حالة HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      حالة HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      حالة HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      حالة HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      حالة HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      حالة HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      حالة HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      حالة HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      حالة HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      حالة HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned صحيح) mode;
	اكسر;
      افتراضي:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	اكسر;
    }

    c->funcs->پاڵنان_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (هذا+backdrop);
    c->funcs->پاڵنان_group_for (c->data, composite_mode);
    c->recurse (هذا+src);
    c->funcs->دەرهێنان_group (c->data, composite_mode);
    c->funcs->دەرهێنان_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية ClipBoxData
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

  فراغ get_clip_box (ClipBoxData &clip_box, ثابت ItemVarStoreInstancer &instancer HB_UNUSED) ثابت
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((مزدوج) xMin + (مزدوج) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((مزدوج) yMin + (مزدوج) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((مزدوج) xMax + (مزدوج) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((مزدوج) yMax + (مزدوج) ceilf (instancer (varIdxBase, 3)));
    }

    إذا (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (true);
  }

  عام:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  عام:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

بنية ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  فراغ get_clip_box (ClipBoxData &clip_box, ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    value.get_clip_box(clip_box, instancer);
    إذا (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<صحيح> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<صحيح> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<صحيح> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<صحيح> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

بنية ClipBox
{
  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    افتراضي:return_trace (c->default_return_بەها ());
    }
  }

  فراغ closurev1 (hb_colrv1_closure_context_t* c) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.format2.closurev1 (c); أرجع;
    افتراضي:أرجع;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_بەها ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_بەها ());
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
                    ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    ClipBoxData clip_box;
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
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    أرجع true;
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

بنية ClipRecord
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  فراغ closurev1 (hb_colrv1_closure_context_t* c, ثابت فراغ *base) ثابت
  {
    إذا (!c->glyphs->intersects (startGlyphID, endGlyphID)) أرجع;
    (base+clipBox).closurev1 (c);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *base,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && clipBox.sanitize (c, base));
  }

  منطقي get_extents (hb_glyph_extents_t *extents,
		    ثابت فراغ *base,
		    ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    أرجع (base+clipBox).get_extents (extents, instancer);
  }

  عام:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  عام:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

بنية ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ثابت ItemVarStoreInstancer &instancer,
                                   ثابت hb_set_t& gids,
                                   ثابت hb_map_t& gid_offset_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned ژماردن  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    لكل (ثابت hb_codepoint_t _ : gids.iter ())
    {
      إذا (_ == start_gid) استمر;

      offset = gid_offset_map.get (_);
      إذا (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        استمر;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      إذا (!record.subset (c, هذا, instancer)) return_trace (0);
      ژماردن++;

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
      إذا (!record.subset (c, هذا, instancer)) return_trace (0);
      ژماردن++;
    }
    return_trace (ژماردن);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    إذا (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (خطأ);

    ثابت hb_set_t& glyphset = c->plan->_glyphset_colred;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    لكل (ثابت ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      لكل (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        إذا (!glyphset.has (gid) || !glyph_map.has (gid)) استمر;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned ژماردن = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    إذا (!ژماردن) return_trace (خطأ);
    return_trace (c->serializer->check_assign (out->clips.len, ژماردن, HB_SERIALIZE_ERROR_INT_OVERFLOW));
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
	       ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    auto *rec = clips.as_array ().bsearch (gid);
    إذا (rec)
      أرجع rec->get_extents (extents, هذا, instancer);
    أرجع خطأ;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  عام:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

بنية Paint
{

  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_بەها ());

    return_trace (c->end_recursion (هذا->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_بەها ();
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
#ifndef HB_NO_BEYOND_64K
    حالة 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_بەها ());
    }
  }

  منطقي get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ثابت
  {
    بدل (u.format.v) {
    حالة 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); أرجع true;
    حالة 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); أرجع true;
    افتراضي: أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }				format;
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
  عام:
  DEFINE_SIZE_MIN (2);
};

بنية BaseGlyphPaintRecord
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  منطقي serialize (hb_serialize_context_t *s, ثابت hb_map_t* glyph_map,
                  ثابت فراغ* src_base, hb_subset_context_t *c,
                  ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = s->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    إذا (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && paint.sanitize (c, base)));
  }

  عام:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out)))  return_trace (خطأ);
    ثابت hb_set_t* glyphset = &c->plan->_glyphset_colred;

    لكل (ثابت auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      إذا (_.serialize (c->serializer, c->plan->glyph_map, هذا, c, instancer)) out->len++;
      وإلا return_trace (خطأ);
    }

    return_trace (out->len != 0);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, هذا));
  }
};

بنية LayerList : Array32OfOffset32To<Paint>
{
  ثابت Paint& get_paint (unsigned i) ثابت
  { أرجع هذا+(*هذا)[i]; }

  منطقي subset (hb_subset_context_t *c,
               ثابت ItemVarStoreInstancer &instancer) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت auto& _ : + hb_enumerate (*هذا)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      ret |= o->serialize_subset (c, _.second, هذا, instancer);
    }
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, هذا));
  }
};

بنية delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_ژماردن () ثابت { أرجع inner_bit_ژماردن; }
  unsigned get_width ()           ثابت { أرجع ((outer_bit_ژماردن + inner_bit_ژماردن + 7) / 8); }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ژماردن = 0;
    outer_bit_ژماردن = 0;
    inner_bit_ژماردن = 1;
    output_map.init ();

    /* search backwards */
    unsigned ژماردن = new_deltaset_idx_varidx_map.get_population ();
    إذا (!ژماردن) أرجع;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    لكل (unsigned i = ژماردن; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      إذا (i == ژماردن)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        استمر;
      }
      إذا (var_idx != last_varidx)
        اكسر;
      last_idx = delta_set_idx;
    }

    map_ژماردن = last_idx + 1;
  }

  منطقي remap (ثابت hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_ژماردن = 1;
    inner_bit_ژماردن = 1;

    إذا (unlikely (!output_map.resize_dirty (map_ژماردن))) أرجع خطأ;

    لكل (unsigned idx = 0; idx < map_ژماردن; idx++)
    {
      uint32_t *var_idx;
      إذا (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) أرجع خطأ;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_ژماردن = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ژماردن = hb_max (bit_ژماردن, outer_bit_ژماردن);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_ژماردن = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ژماردن = hb_max (bit_ژماردن, inner_bit_ژماردن);
    }
    أرجع true;
  }

  خاص:
  unsigned map_ژماردن;
  unsigned outer_bit_ژماردن;
  unsigned inner_bit_ژماردن;
  hb_vector_t<uint32_t> output_map;
};

بنية COLR
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  منطقي has_data () ثابت { أرجع has_v0_data () || version; }

  منطقي has_v0_data () ثابت { أرجع ژمارە_Base_Glyphs; }
  منطقي has_v1_data () ثابت
  {
    إذا (version < 1)
      أرجع خطأ;
    hb_barrier ();

    أرجع (هذا+baseGlyphList).len > 0;
  }

  unsigned صحيح get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned صحيح         start_offset,
				 unsigned صحيح        *ژماردن, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ثابت
  {
    ثابت BaseGlyphRecord &record = (هذا+baseGlyphsZ).bsearch (ژمارە_Base_Glyphs, glyph);

    hb_array_t<ثابت LayerRecord> all_layers = (هذا+layersZ).as_array (ژمارە_Layers);
    hb_array_t<ثابت LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.ژمارە_Layers);
    إذا (ژماردن && layers)
    {
      + glyph_layers.sub_array (start_offset, ژماردن)
      | hb_sink (hb_array (layers, *ژماردن))
      ;
    }
    أرجع glyph_layers.length;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      إذا (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    منطقي has_data () ثابت { أرجع colr->has_data (); }

#ifndef HB_NO_PAINT
    منطقي
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ثابت
    {
      إذا (unlikely (!has_data ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع true;
      منطقي ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }

    منطقي paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, فراغ *data,
		      unsigned صحيح palette_index,
		      hb_color_t foreground,
		      منطقي clip = true) ثابت
    {
      إذا (unlikely (!has_data ())) أرجع خطأ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      إذا (unlikely (!scratch)) أرجع true;
      منطقي ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      أرجع ret;
    }
#endif

    منطقي is_valid () { أرجع colr.get_blob ()->length; }

    فراغ depend (hb_depend_data_builder_t *depend_data) ثابت
    {
      إذا (!has_data ()) أرجع;
      colr->depend (depend_data);
    }

    فراغ closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ثابت
    { colr->closure_glyphs (glyph, related_ids); }

    فراغ closure_V0palette_indices (ثابت hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ثابت
    { colr->closure_V0palette_indices (glyphs, palettes); }

    فراغ closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ثابت
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    منطقي has_var_store () ثابت
    { أرجع colr->has_var_store (); }

    ثابت ItemVariationStore &get_var_store () ثابت
    { أرجع colr->get_var_store (); }
    ثابت ItemVariationStore *get_var_store_ptr () ثابت
    { أرجع colr->get_var_store_ptr (); }

    منطقي has_delta_set_index_map () ثابت
    { أرجع colr->has_delta_set_index_map (); }

    ثابت DeltaSetIndexMap &get_delta_set_index_map () ثابت
    { أرجع colr->get_delta_set_index_map (); }
    ثابت DeltaSetIndexMap *get_delta_set_index_map_ptr () ثابت
    { أرجع colr->get_delta_set_index_map_ptr (); }

    خاص:

    hb_colr_scratch_t *acquire_scratch () ثابت
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      إذا (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, حجم (hb_colr_scratch_t));
	إذا (unlikely (!scratch))
	  أرجع nullptr;
      }

      أرجع scratch;
    }
    فراغ release_scratch (hb_colr_scratch_t *scratch) ثابت
    {
      إذا (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    عام:
    hb_blob_ptr_t<COLR> colr;
    خاص:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  فراغ depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    // v0
    hb_array_t<ثابت BaseGlyphRecord> baseGlyphs = (هذا+baseGlyphsZ).as_array (ژمارە_Base_Glyphs);
    لكل (ثابت BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (هذا+layersZ).as_array (ژمارە_Layers).sub_array (record.firstLayerIdx,
                                                                         record.ژمارە_Layers);
      لكل (ثابت LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    إذا (!has_v1_data())
      أرجع;

    // v1
    hb_colrv1_depend_context_t c (هذا, depend_data);
    ثابت BaseGlyphList &baseglyph_paintrecords = هذا+baseGlyphList;

    لكل (ثابت BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  فراغ closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ثابت
  {
    ثابت BaseGlyphRecord *record = get_base_glyph_record (glyph);
    إذا (!record) أرجع;

    auto glyph_layers = (هذا+layersZ).as_array (ژمارە_Layers).sub_array (record->firstLayerIdx,
								       record->ژمارە_Layers);
    إذا (!glyph_layers.length) أرجع;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  فراغ closure_V0palette_indices (ثابت hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ثابت
  {
    إذا (!ژمارە_Base_Glyphs || !ژمارە_Layers) أرجع;
    hb_array_t<ثابت BaseGlyphRecord> baseGlyphs = (هذا+baseGlyphsZ).as_array (ژمارە_Base_Glyphs);
    hb_array_t<ثابت LayerRecord> all_layers = (هذا+layersZ).as_array (ژمارە_Layers);

    لكل (ثابت BaseGlyphRecord record : baseGlyphs)
    {
      إذا (!glyphs->has (record.glyphId)) استمر;
      hb_array_t<ثابت LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.ژمارە_Layers);
      لكل (ثابت LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  فراغ closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ثابت
  {
    إذا (version < 1) أرجع;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (هذا, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ثابت BaseGlyphList &baseglyph_paintrecords = هذا+baseGlyphList;

    لكل (ثابت BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      إذا (!glyphset->has (gid)) استمر;

      ثابت Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ثابت ClipList &cliplist = هذا+clipList;
    c.glyphs = glyphset;
    لكل (ثابت ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    إذا (has_delta_set_index_map ())
    {
      ثابت DeltaSetIndexMap &var_idx_map = هذا+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      لكل (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ثابت LayerList& get_layerList () ثابت
  { أرجع (هذا+layerList); }

  ثابت BaseGlyphList& get_baseglyphList () ثابت
  { أرجع (هذا+baseGlyphList); }

  منطقي has_var_store () ثابت
  { أرجع version >= 1 && hb_barrier () && varStore != 0; }

  منطقي has_delta_set_index_map () ثابت
  { أرجع version >= 1 && hb_barrier () && varIdxMap != 0; }

  منطقي has_clip_list () ثابت
  { أرجع version >= 1 && hb_barrier () && clipList != 0; }

  ثابت DeltaSetIndexMap &get_delta_set_index_map () ثابت
  { أرجع has_delta_set_index_map () && hb_barrier () ? هذا+varIdxMap : Null (DeltaSetIndexMap); }
  ثابت DeltaSetIndexMap *get_delta_set_index_map_ptr () ثابت
  { أرجع has_delta_set_index_map () && hb_barrier () ? &(هذا+varIdxMap) : nullptr; }

  ثابت ItemVariationStore &get_var_store () ثابت
  { أرجع has_var_store () && hb_barrier () ? هذا+varStore : Null (ItemVariationStore); }
  ثابت ItemVariationStore *get_var_store_ptr () ثابت
  { أرجع has_var_store () && hb_barrier () ? &(هذا+varStore) : nullptr; }

  ثابت ClipList &get_clip_list () ثابت
  { أرجع has_clip_list () && hb_barrier () ? هذا+clipList : Null (ClipList); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  (هذا+baseGlyphsZ).sanitize (c, ژمارە_Base_Glyphs) &&
                  (هذا+layersZ).sanitize (c, ژمارە_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, هذا) &&
		    layerList.sanitize (c, هذا) &&
		    clipList.sanitize (c, هذا) &&
		    varIdxMap.sanitize (c, هذا) &&
		    varStore.sanitize (c, هذا))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  منطقي serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (base_it.len () != layer_it.len ()))
      return_trace (خطأ);

    هذا->version = version;
    ژمارە_Layers = 0;
    ژمارە_Base_Glyphs = base_it.len ();
    إذا (ژمارە_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (true);
    }

    c->push ();
    لكل (ثابت hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      إذا (unlikely (!record)) return_trace (خطأ);
      record->firstLayerIdx = ژمارە_Layers;
      ژمارە_Layers += record->ژمارە_Layers;
    }
    c->add_link (baseGlyphsZ, c->دەرهێنان_pack ());

    c->push ();
    لكل (ثابت hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->دەرهێنان_pack ());

    return_trace (true);
  }

  ثابت BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ثابت
  {
    ثابت BaseGlyphRecord* record = &(هذا+baseGlyphsZ).bsearch (ژمارە_Base_Glyphs, (unsigned صحيح) gid);
    إذا (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    أرجع record;
  }

  ثابت BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ثابت
  {
    ثابت BaseGlyphList &list = هذا+baseGlyphList;
    unsigned صحيح i;
    إذا (!list.bfind ((unsigned) gid, &i))
      أرجع nullptr;
    أرجع &list[i];
  }

  منطقي downgrade_to_V0 (ثابت hb_set_t &glyphset) ثابت
  {
    //no more COLRv1 glyphs, downgrade to version 0
    لكل (ثابت BaseGlyphPaintRecord& _ : get_baseglyphList ())
      إذا (glyphset.has (_.glyphId))
        أرجع خطأ;

    أرجع true;
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (true);

    ثابت ItemVariationStore& var_store = هذا+varStore;
    إذا (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      منطقي optimize = (varIdxMap != 0) ? true : خطأ;
      إذا (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (خطأ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      إذا (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      إذا (optimize)
      {
        ثابت hb_map_t &varidx_map = item_vars.get_varidx_map ();
        لكل (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
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
      hb_set_t dead_regions;
      إذا (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      إذا (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  منطقي subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (true);

    ثابت hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    إذا (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (خطأ);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ثابت hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->ژمارە_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    إذا (glyphset.has (old_gid)) أرجع true;
		    أرجع خطأ;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ثابت BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				إذا (unlikely (!old_record))
				  أرجع hb_pair_t<منطقي, BaseGlyphRecord> (خطأ, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.ژمارە_Layers = old_record->ژمارە_Layers;
				أرجع hb_pair_t<منطقي, BaseGlyphRecord> (true, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->ژمارە_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ثابت BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				إذا (unlikely (!old_record ||
					      old_record->firstLayerIdx >= ژمارە_Layers ||
					      old_record->firstLayerIdx + old_record->ژمارە_Layers > ژمارە_Layers))
				  أرجع hb_pair_t<منطقي, hb_vector_t<LayerRecord>> (خطأ, out_layers);

				auto layers = (هذا+layersZ).as_array (ژمارە_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->ژمارە_Layers);
				out_layers.resize (layers.length);
				لكل (unsigned صحيح i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  إذا (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    أرجع hb_pair_t<منطقي, hb_vector_t<LayerRecord>> (خطأ, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				أرجع hb_pair_t<منطقي, hb_vector_t<LayerRecord>> (true, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    إذا (version == 0 && (!base_it || !layer_it))
      return_trace (خطأ);

    auto *colr_سەرەتایی = c->serializer->start_embed<COLR> ();
    إذا (unlikely (!c->serializer->extend_min (colr_سەرەتایی)))  return_trace (خطأ);

    إذا (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_سەرەتایی->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    إذا (!c->serializer->allocate_size<فراغ> (5 * HBUINT32::static_size)) return_trace (خطأ);
    إذا (!colr_سەرەتایی->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (خطأ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    إذا (!subset_varstore (c, colr_سەرەتایی)) return_trace (خطأ);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    إذا (!colr_سەرەتایی->baseGlyphList.serialize_subset (c, baseGlyphList, هذا, instancer))
      return_trace (خطأ);

    colr_سەرەتایی->layerList.serialize_subset (c, layerList, هذا, instancer);
    colr_سەرەتایی->clipList.serialize_subset (c, clipList, هذا, instancer);

    return_trace (subset_delta_set_index_map (c, colr_سەرەتایی));
  }

  ثابت Paint *get_base_glyph_paint (hb_codepoint_t glyph) ثابت
  {
    ثابت BaseGlyphList &baseglyph_paintrecords = هذا+baseGlyphList;
    ثابت BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    إذا (record)
    {
      ثابت Paint &paint = &baseglyph_paintrecords+record->paint;
      أرجع &paint;
    }
    وإلا
      أرجع nullptr;
  }

#ifndef HB_NO_PAINT
  منطقي
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ثابت
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->ژمارە_coords));

    إذا (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      أرجع true;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    منطقي ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), true, scratch);

    auto e = scratch.paint_extents.get_extents ();
    إذا (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    وإلا
      *extents = e.to_glyph_extents ();

    أرجع ret;
  }
#endif

  منطقي
  has_paint_for_glyph (hb_codepoint_t glyph) ثابت
  {
    إذا (version >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_base_glyph_paint (glyph);

      أرجع paint != nullptr;
    }

    أرجع خطأ;
  }

  منطقي get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ثابت ItemVarStoreInstancer instancer) ثابت
  {
    أرجع get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  منطقي
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, فراغ *data,
	       unsigned صحيح palette_index, hb_color_t foreground,
	       منطقي clip,
	       hb_colr_scratch_t &scratch) ثابت
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->ژمارە_coords : 0));
    hb_paint_context_t c (هذا, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_گرێ_t node (c.glyphs_decycler);
    node.visit (glyph);

    إذا (version >= 1)
    {
      hb_barrier ();

      ثابت Paint *paint = get_base_glyph_paint (glyph);
      إذا (paint)
      {
        // COLRv1 glyph

	منطقي is_bounded = true;
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
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 خطأ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->پاڵنان_font_transform (c.data, font);

	إذا (clip)
	  c.funcs->پاڵنان_clip_rectangle (c.data,
					(عائم) extents.x_bearing,
					(عائم) extents.y_bearing + extents.height,
					(عائم) extents.x_bearing + extents.width,
					(عائم) extents.y_bearing);

	إذا (is_bounded)
	  c.recurse (*paint);

	إذا (clip)
	  c.funcs->دەرهێنان_clip (c.data);

	c.funcs->دەرهێنان_transform (c.data);

        أرجع true;
      }
    }

    ثابت BaseGlyphRecord *record = get_base_glyph_record (glyph);
    إذا (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      لكل (ثابت auto &r : (هذا+layersZ).as_array (ژمارە_Layers)
			   .sub_array (record->firstLayerIdx, record->ژمارە_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      أرجع true;
    }

    أرجع خطأ;
  }
#endif

  محمي:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	ژمارە_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	ژمارە_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  عام:
  DEFINE_SIZE_MIN (14);
};

بنية COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

فراغ
hb_paint_context_t::recurse (ثابت Paint &paint)
{
  إذا (unlikely (depth_چەپ <= 0 || edge_ژماردن <= 0)) أرجع;
  depth_چەپ--;
  edge_ژماردن--;
  paint.dispatch (هذا);
  depth_چەپ++;
}

template <typename Types>
فراغ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  إذا (likely (c->depth_چەپ > 0 && c->edge_ژماردن > 0) &&
      (هذا+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_ژماردن--;
    c->funcs->پاڵنان_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->دەرهێنان_transform (c->data);
    أرجع;
  }

  c->funcs->پاڵنان_inverse_font_transform (c->data, c->font);
  c->funcs->پاڵنان_clip_glyph (c->data, gid, c->font);
  c->funcs->پاڵنان_font_transform (c->data, c->font);
  c->recurse (هذا+paint);
  c->funcs->دەرهێنان_transform (c->data);
  c->funcs->دەرهێنان_clip (c->data);
  c->funcs->دەرهێنان_transform (c->data);
}

فراغ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);
  ثابت LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_گرێ_t node (c->layers_decycler);
  لكل (unsigned i = firstLayerIndex; i < firstLayerIndex + ژمارە_Layers; i++)
  {
    إذا (unlikely (!node.visit (i)))
      أرجع;

    ثابت Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

فراغ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ثابت
{
  TRACE_PAINT (هذا);

  hb_decycler_گرێ_t node (c->glyphs_decycler);
  إذا (unlikely (!node.visit (gid)))
    أرجع;

  c->funcs->پاڵنان_inverse_font_transform (c->data, c->font);
  إذا (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->دەرهێنان_transform (c->data);
    أرجع;
  }
  c->funcs->دەرهێنان_transform (c->data);

  ثابت COLR *colr_table = c->get_colr_table ();
  ثابت Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  منطقي has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  إذا (has_clip_box)
    c->funcs->پاڵنان_clip_rectangle (c->data,
				   (عائم) extents.x_bearing,
				   (عائم) extents.y_bearing + extents.height,
				   (عائم) extents.x_bearing + extents.width,
				   (عائم) extents.y_bearing);

  إذا (paint)
    c->recurse (*paint);

  إذا (has_clip_box)
    c->funcs->دەرهێنان_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
