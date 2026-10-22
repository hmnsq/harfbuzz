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

#અન્તર્ભાવ "../../../hb.hh"
#અન્તર્ભાવ "../../../hb-decycler.hh"
#અન્તર્ભાવ "../../../hb-open-type.hh"
#અન્તર્ભાવ "../../../hb-ot-var-common.hh"
#અન્તર્ભાવ "../../../hb-paint.hh"
#અન્તર્ભાવ "../../../hb-paint-bounded.hh"
#અન્તર્ભાવ "../../../hb-paint-extents.hh"
#અન્તર્ભાવ "../../../hb-depend-data.hh"

#અન્તર્ભાવ "../../../hb-ot-dual.hh"
#અન્તર્ભાવ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

નામાકાશ OT {
સંરચના hb_paint_context_t;
}

સંરચના hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

નામાકાશ OT {

સંરચના COLR;

સંરચના Paint;

સંરચના hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  અચલ અક્ષર *get_name () { પાછા "PAINT"; }
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.paint_glyph (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  અચલ COLR* get_colr_table () અચલ
  { પાછા reinterpret_cast<અચલ COLR *> (base); }

જાહેર:
  અચલ શૂન્ય *base;
  hb_paint_funcs_t *funcs;
  શૂન્ય *data;
  hb_font_t *font;
  hb_array_t<અચલ BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  પૂર્ણાંક depth_ડાબું = HB_MAX_NESTING_LEVEL;
  પૂર્ણાંક edge_ગણતરી = HB_MAX_GRAPH_EDGE_ગણતરી;

  hb_paint_context_t (અચલ શૂન્ય *base_,
		      hb_paint_funcs_t *funcs_,
		      શૂન્ય *data_,
                      hb_font_t *font_,
                      unsigned પૂર્ણાંક palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_ગણતરી () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    જો (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, સાચું);
      hb_font_set_synthetic_slant (font, 0);
    }
    નહીંતર
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  સ્થિર unsigned
  color_alpha (hb_color_t color, દશાંશ alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    પાછા roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned પૂર્ણાંક color_index, દશાંશ alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = સાચું;

    જો (color_index != 0xffff)
    {
      જો (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = ખોટું;
    }

    પાછા HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  અંતઃરેખ શૂન્ય recurse (અચલ Paint &paint);
};

સંરચના hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (અચલ T &obj)
  {
    જો (unlikely (nesting_level_ડાબું == 0))
      પાછા hb_empty_t ();

    જો (paint_visited (&obj))
      પાછા hb_empty_t ();

    nesting_level_ડાબું--;
    obj.closurev1 (આ);
    nesting_level_ડાબું++;
    પાછા hb_empty_t ();
  }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  તાર્કિક paint_visited (અચલ શૂન્ય *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    જો (visited_paint.in_error() || visited_paint.has (delta))
      પાછા સાચું;

    visited_paint.add (delta);
    પાછા ખોટું;
  }

  અચલ COLR* get_colr_table () અચલ
  { પાછા reinterpret_cast<અચલ COLR *> (base); }

  શૂન્ય add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  શૂન્ય add_layer_indices (unsigned first_layer_index, unsigned અંક_of_layers)
  {
    જો (અંક_of_layers == 0) પાછા;
    layer_indices->add_range (first_layer_index, first_layer_index + અંક_of_layers - 1);
  }

  શૂન્ય add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  શૂન્ય add_var_idxes (unsigned first_var_idx, unsigned અંક_idxes)
  {
    જો (!અંક_idxes || first_var_idx == VarIdx::NO_VARIATION) પાછા;
    variation_indices->add_range (first_var_idx, first_var_idx + અંક_idxes - 1);
  }

  જાહેર:
  અચલ શૂન્ય *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned અંક_var_idxes;
  unsigned nesting_level_ડાબું;

  hb_colrv1_closure_context_t (અચલ શૂન્ય *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned અંક_var_idxes_2 = 1,
                               unsigned nesting_level_ડાબું_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          અંક_var_idxes (અંક_var_idxes_2),
                          nesting_level_ડાબું (nesting_level_ડાબું_2)
  {}
};


સંરચના hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (અચલ T &obj)
  {
    જો (unlikely (nesting_level_ડાબું == 0))
      પાછા hb_empty_t ();

    જો (paint_visited (&obj))
      પાછા hb_empty_t ();

    nesting_level_ડાબું--;
    obj.dependv1 (આ);
    nesting_level_ડાબું++;
    પાછા hb_empty_t ();
  }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  તાર્કિક paint_visited (અચલ શૂન્ય *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    જો (visited_paint.in_error() || visited_paint.has (delta))
      પાછા સાચું;

    visited_paint.add (delta);
    જો (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    પાછા ખોટું;
  }

  અચલ COLR* get_colr_table () અચલ
  { પાછા colr; }

  જાહેર:
  અચલ COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_ડાબું;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (અચલ COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_ડાબું_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_ડાબું (nesting_level_ડાબું_2),
                          source_gid (0)
  {}
};

સંરચના LayerRecord
{
  operator hb_ot_color_layer_t () અચલ { પાછા {glyphId, colorIdx}; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
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
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના BaseGlyphRecord
{
  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	અંક_Layers;	/* Number of color layers
				 * associated with this glyph */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
સંરચના Variable
{
  સ્થિર constexpr તાર્કિક is_variable = સાચું;

  અચલ T& get_કિંમત () અચલ { પાછા value; }

  Variable<T>* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (આ));
  }

  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ
  { value.dependv1 (c); }

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    c->અંક_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->અંક_var_idxes);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!value.subset (c, instancer, varIdxBase)) return_trace (ખોટું);
    જો (c->plan->all_axes_pinned)
      return_trace (સાચું);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    જો (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
      જો (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (ખોટું);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ
  {
    TRACE_PAINT (આ);
    value.paint_glyph (c, varIdxBase);
  }

  શૂન્ય get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  શૂન્ય get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) અચલ
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () અચલ
  {
    પાછા value.get_extend ();
  }

  સુરક્ષિત:
  T      value;
  જાહેર:
  VarIdx varIdxBase;
  જાહેર:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
સંરચના NoVariable
{
  સ્થિર constexpr તાર્કિક is_variable = ખોટું;

  અચલ T& get_કિંમત () અચલ { પાછા value; }

  સ્થિર constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (આ));
  }

  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ
  { value.dependv1 (c); }

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  { value.closurev1 (c); }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ
  {
    TRACE_PAINT (આ);
    value.paint_glyph (c, varIdxBase);
  }

  શૂન્ય get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  શૂન્ય get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) અચલ
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () અચલ
  {
    પાછા value.get_extend ();
  }

  T      value;
  જાહેર:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
સ્થિર શૂન્ય
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, દશાંશ value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

સંરચના ColorStop
{
  શૂન્ય dependv1 (hb_colrv1_depend_context_t * c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    c->add_palette_index (paletteIndex);
    c->અંક_var_idxes = 2;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      દશાંશ a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  જાહેર:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

સંરચના Extend : HBUINT8
{
  ગણના {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  જાહેર:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> વર્ગ Var>
સંરચના Color_લીટી
{
  શૂન્ય dependv1 (hb_colrv1_depend_context_t * c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    માટે (અચલ auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    જો (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ખોટું);
    જો (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (ખોટું);

    માટે (અચલ auto& stop : stops.iter ())
    {
      જો (!stop.subset (c, instancer)) return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned પૂર્ણાંક
  get_color_stops (hb_paint_context_t *c,
                   unsigned પૂર્ણાંક start,
		   unsigned પૂર્ણાંક *ગણતરી,
		   hb_color_stop_t *color_stops,
		   અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    unsigned પૂર્ણાંક len = stops.len;

    જો (ગણતરી && color_stops)
    {
      unsigned પૂર્ણાંક i;
      માટે (i = 0; i < *ગણતરી && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *ગણતરી = i;
    }

    પાછા len;
  }

  HB_INTERNAL સ્થિર unsigned પૂર્ણાંક static_get_color_stops (hb_color_લીટી_t *color_લીટી,
							  શૂન્ય *color_લીટી_data,
							  unsigned પૂર્ણાંક start,
							  unsigned પૂર્ણાંક *ગણતરી,
							  hb_color_stop_t *color_stops,
							  શૂન્ય *user_data)
  {
    અચલ Color_લીટી *thiz = (અચલ Color_લીટી *) color_લીટી_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    પાછા thiz->get_color_stops (c, start, ગણતરી, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () અચલ
  {
    પસંદગી ((unsigned પૂર્ણાંક) extend)
    {
      સ્થિતિ Extend::EXTEND_REPEAT:  પાછા HB_PAINT_EXTEND_REPEAT;
      સ્થિતિ Extend::EXTEND_REFLECT: પાછા HB_PAINT_EXTEND_REFLECT;
      સ્થિતિ Extend::EXTEND_PAD:
      મૂળભૂત:                     પાછા HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL સ્થિર hb_paint_extend_t static_get_extend (hb_color_લીટી_t *color_લીટી,
							  શૂન્ય *color_લીટી_data,
							  શૂન્ય *user_data)
  {
    અચલ Color_લીટી *thiz = (અચલ Color_લીટી *) color_લીટી_data;
    પાછા thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
સંરચના CompositeMode : HBUINT8
{
  જાહેર:
  DEFINE_SIZE_STATIC (1);
};

સંરચના Affine2x3
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  { c->અંક_var_idxes = 6; }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((દ્વિગુણ) source.to_int () +
                                                    (દ્વિગુણ) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (સાચું);
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    c->funcs->ધકેલો_transform (c->data,
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
  જાહેર:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

સંરચના PaintColrLayers
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer HB_UNUSED) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    uint32_t first_layer_index = અંક_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  અંતઃરેખ શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ;

  HBUINT8	format; /* format = 1 */
  HBUINT8	અંક_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના PaintSolid
{
  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ {}
  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    c->add_palette_index (paletteIndex);
    c->અંક_var_idxes = 1;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      દશાંશ a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    જો (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) અચલ
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  જાહેર:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> વર્ગ Var>
સંરચના PaintLinearGradient
{
  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    (આ+color_લીટી_2).closurev1 (c);
    c->અંક_var_idxes = 6;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((દ્વિગુણ) x0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((દ્વિગુણ) y0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((દ્વિગુણ) x1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((દ્વિગુણ) y1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((દ્વિગુણ) x2 + (દ્વિગુણ) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((દ્વિગુણ) y2 + (દ્વિગુણ) roundf (instancer (varIdxBase, 5)));
    }

    જો (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_લીટી_2.serialize_subset (c, color_લીટી_2, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && color_લીટી_2.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    hb_color_લીટી_t cl = {
      (શૂન્ય *) &(આ+color_લીટી_2),
      (આ+color_લીટી_2).static_get_color_stops, c,
      (આ+color_લીટી_2).static_get_extend, nullptr
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
  Offset24To<Color_લીટી<Var>>	color_લીટી_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> વર્ગ Var>
સંરચના PaintRadialGradient
{
  તાર્કિક constant_after_pinning (hb_subset_context_t *c,
                               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    જો (c->plan->has_avar2) પાછા ખોટું;
    અચલ auto &regions = instancer.varStore->get_region_list ();
    માટે (unsigned i = 0; i < regions.region_ગણતરી; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      જો (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        પાછા ખોટું;
      }
      માટે (auto axis : axes)
      {
        Triple *limit;
        જો (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          પાછા ખોટું;
      }
    }
    પાછા સાચું;
  }

  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    (આ+color_લીટી_2).closurev1 (c);
    c->અંક_var_idxes = 6;
  }

  તાર્કિક subset_nonnegative (hb_subset_context_t *c,
                           અચલ ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) અચલ
  {
    દ્વિગુણ source[6] = {
      x0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 0)),
      y0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)),
      radius0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)),
      x1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 3)),
      y1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 4)),
      radius1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      જો (!out) પાછા ખોટું;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      પાછા સાચું;
    };
    અચલ auto &લીટી = આ+color_લીટી_2;
    જો (source[2] == source[5] || !લીટી.stops.len) પાછા transparent ();
    સંરચના stop_t { દ્વિગુણ offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    જો (!stops.alloc (લીટી.stops.len)) પાછા ખોટું;
    unsigned order = 0;
    માટે (અચલ auto &entry : લીટી.stops.iter ())
    {
      અચલ auto &stop = entry.get_કિંમત ();
      stops.push (stop_t {(દ્વિગુણ) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (દ્વિગુણ) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    જો (stops.in_error ()) પાછા ખોટું;
    stops.qsort ([] (અચલ stop_t &a, અચલ stop_t &b) {
      પાછા a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    દ્વિગુણ dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    માટે (unsigned j = 0; j < 3; j++)
    {
      દ્વિગુણ remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      જો (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        પાછા ખોટું;
      }
      unsigned જમણું = (unsigned) remainder;
      જ્યારે (જમણું) { unsigned r = lattice % જમણું; lattice = જમણું; જમણું = r; }
    }
    માટે (unsigned shift = 0; shift < 16; shift++)
    {
      દ્વિગુણ b = 1u << shift;
      દ્વિગુણ coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      દ્વિગુણ fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      દ્વિગુણ candidates[] = {coarse, fine};
      માટે (દ્વિગુણ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        દ્વિગુણ geometry[6];
        તાર્કિક fits = સાચું;
        માટે (unsigned j = 0; j < 3; j++)
        {
          દ્વિગુણ difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          દ્વિગુણ નીચલું = j == 2 ? 0 : -32768, ઉપલું = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= નીચલું && geometry[j] <= ઉપલું &&
                  geometry[j + 3] >= નીચલું && geometry[j + 3] <= ઉપલું;
        }
        જો (!fits) ચાલુ;
        unsigned first = 0, ગણતરી = stops.length;
        દ્વિગુણ min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        દ્વિગુણ bias = 0;
        તાર્કિક constant = ખોટું;
        જો (લીટી.extend == Extend::EXTEND_PAD)
        {
          દ્વિગુણ tip = -geometry[2] / (b * dr);
          જો ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            ગણતરી = 1; constant = સાચું;
          }
        }
        નહીંતર
        {
          દ્વિગુણ period = max - min;
          જો (લીટી.extend == Extend::EXTEND_REFLECT) period *= 2;
          જો (period <= 0) પાછા transparent ();
          દ્વિગુણ n = ceil ((-2 - min) / period);
          જો (n > floor ((32767. / 16384. - max) / period)) ચાલુ;
          bias = n * period;
        }
        માટે (unsigned j = 0; j < ગણતરી; j++)
        {
          દ્વિગુણ bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        જો (!fits) ચાલુ;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        જો (!out) પાછા ખોટું;
        out->format = 6;
        out->x0 = (પૂર્ણાંક) geometry[0]; out->y0 = (પૂર્ણાંક) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (પૂર્ણાંક) geometry[3]; out->y1 = (પૂર્ણાંક) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_લીટી = c->serializer->allocate_size<Color_લીટી<NoVariable>> (3 + ગણતરી * ColorStop::static_size);
        જો (!out_લીટી) { c->serializer->કાઢો_discard (); પાછા ખોટું; }
        out_લીટી->extend = લીટી.extend;
        out_લીટી->stops.len = ગણતરી;
        માટે (unsigned j = 0; j < ગણતરી; j++)
        {
          અચલ auto &stop = stops[first + j];
          auto &out_stop = out_લીટી->stops[j].value;
          out_stop.stopOffset.set_float ((દશાંશ) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((દશાંશ) stop.alpha);
          જો (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->કાઢો_discard (); પાછા ખોટું; }
        }
        c->serializer->add_link (out->color_લીટી_2, c->serializer->કાઢો_pack ());
        પાછા !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    પાછા ખોટું;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    જો (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((દ્વિગુણ) x0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((દ્વિગુણ) y0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((દ્વિગુણ) radius0 + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((દ્વિગુણ) x1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((દ્વિગુણ) y1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((દ્વિગુણ) radius1 + (દ્વિગુણ) roundf (instancer (varIdxBase, 5)));
    }

    જો (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_લીટી_2.serialize_subset (c, color_લીટી_2, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && color_લીટી_2.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    hb_color_લીટી_t cl = {
      (શૂન્ય *) &(આ+color_લીટી_2),
      (આ+color_લીટી_2).static_get_color_stops, c,
      (આ+color_લીટી_2).static_get_extend, nullptr
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
  Offset24To<Color_લીટી<Var>>	color_લીટી_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> વર્ગ Var>
સંરચના PaintSweepGradient
{
  શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ {}

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    (આ+color_લીટી_2).closurev1 (c);
    c->અંક_var_idxes = 4;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((દ્વિગુણ) centerX + (દ્વિગુણ) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((દ્વિગુણ) centerY + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    જો (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_લીટી_2.serialize_subset (c, color_લીટી_2, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && color_લીટી_2.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    hb_color_લીટી_t cl = {
      (શૂન્ય *) &(આ+color_લીટી_2),
      (આ+color_લીટી_2).static_get_color_stops, c,
      (આ+color_લીટી_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_લીટી<Var>>	color_લીટી_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
સંરચના PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    return_trace (out->paint.serialize_subset (c, paint, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && paint.sanitize (c, આ));
  }

  અંતઃરેખ શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

સંરચના PaintColrGlyph
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer HB_UNUSED) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  અંતઃરેખ શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  જાહેર:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> વર્ગ Var>
સંરચના PaintTransform
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    જો (!out->transform.serialize_subset (c, transform, આ, instancer)) return_trace (ખોટું);
    જો (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  src.sanitize (c, આ) &&
                  transform.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ
  {
    TRACE_PAINT (આ);
    (આ+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  જાહેર:
  DEFINE_SIZE_STATIC (7);
};

સંરચના PaintTranslate
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((દ્વિગુણ) dx + (દ્વિગુણ) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((દ્વિગુણ) dy + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
    }

    જો (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ ddx = dx + c->instancer (varIdxBase, 0);
    દશાંશ ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ધકેલો_translate (c->data, ddx, ddy);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
સ્થિર તાર્કિક serialize_scale_transform (hb_subset_context_t *c,
                                       દશાંશ sx, દશાંશ sy, દશાંશ dx, દશાંશ dy,
                                       અચલ Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  જો (unlikely (!out)) પાછા ખોટું;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  જો (unlikely (!transform))
  {
    c->serializer->કાઢો_discard ();
    પાછા ખોટું;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->કાઢો_pack ());

  c->serializer->push ();
  તાર્કિક ret = source ();
  જો (ret)
    c->serializer->add_link (out->src, c->serializer->કાઢો_pack ());
  નહીંતર
    c->serializer->કાઢો_discard ();
  પાછા ret;
}

template <typename Base>
સ્થિર તાર્કિક subset_scale_as_transform (hb_subset_context_t *c,
                                      અચલ Base *base,
                                      અચલ Offset24To<Paint> &src,
                                      અચલ ItemVarStoreInstancer &instancer,
                                      દશાંશ sx, દશાંશ sy,
                                      દશાંશ cx = 0, દશાંશ cy = 0)
{
  જો (src.is_null ()) પાછા ખોટું;
  auto source = [&] { પાછા c->dispatch (base+src, instancer); };
  દશાંશ dx = cx * (1 - sx), dy = cy * (1 - sy);
  જો (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    પાછા serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    પાછા serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    પાછા serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    પાછા serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    પાછા serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  પાછા serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

સંરચના PaintScale
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      દશાંશ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      દશાંશ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      જો (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, આ, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    જો (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    દશાંશ sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ધકેલો_scale (c->data, sx, sy);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

સંરચના PaintScaleAroundCenter
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      દશાંશ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      દશાંશ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      જો (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, આ, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((દ્વિગુણ) centerX + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((દ્વિગુણ) centerY + (દ્વિગુણ) roundf (instancer (varIdxBase, 3)));
    }

    જો (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    દશાંશ sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    દશાંશ tCenterX = centerX + c->instancer (varIdxBase, 2);
    દશાંશ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ધકેલો_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

સંરચના PaintScaleUniform
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      દશાંશ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      જો (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, આ, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    જો (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ધકેલો_scale (c->data, s, s);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

સંરચના PaintScaleUniformAroundCenter
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      દશાંશ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      જો (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, આ, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((દ્વિગુણ) centerX + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((દ્વિગુણ) centerY + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
    }

    જો (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ s = scale.to_float (c->instancer (varIdxBase, 0));
    દશાંશ tCenterX = centerX + c->instancer (varIdxBase, 1);
    દશાંશ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ધકેલો_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

સંરચના PaintRotate
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    જો (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ધકેલો_rotate (c->data, a);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

સંરચના PaintRotateAroundCenter
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((દ્વિગુણ) centerX + (દ્વિગુણ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((દ્વિગુણ) centerY + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
    }

    જો (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ a = angle.to_float (c->instancer (varIdxBase, 0));
    દશાંશ tCenterX = centerX + c->instancer (varIdxBase, 1);
    દશાંશ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ધકેલો_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

સંરચના PaintSkew
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    જો (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    દશાંશ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ધકેલો_skew (c->data, sx, sy);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

સંરચના PaintSkewAroundCenter
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((દ્વિગુણ) centerX + (દ્વિગુણ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((દ્વિગુણ) centerY + (દ્વિગુણ) roundf (instancer (varIdxBase, 3)));
    }

    જો (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, આ, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && src.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) અચલ
  {
    TRACE_PAINT (આ);
    દશાંશ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    દશાંશ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    દશાંશ tCenterX = centerX + c->instancer (varIdxBase, 2);
    દશાંશ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ધકેલો_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (આ+src);
    c->funcs->કાઢો_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

સંરચના PaintComposite
{
  HB_INTERNAL શૂન્ય dependv1 (hb_colrv1_depend_context_t* c) અચલ;
  HB_INTERNAL શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ;

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    તાર્કિક ret = ખોટું;
    ret |= out->src.serialize_subset (c, src, આ, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, આ, instancer);
    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  c->check_ops (આ->min_size) && // PainComposite can get exponential
                  src.sanitize (c, આ) &&
                  backdrop.sanitize (c, આ));
  }

  શૂન્ય paint_glyph (hb_paint_context_t *c) અચલ
  {
    TRACE_PAINT (આ);
    hb_paint_composite_mode_t composite_mode;
    પસંદગી ((unsigned પૂર્ણાંક) mode)
    {
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_CLEAR:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_IN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_XOR:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_PLUS:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SCREEN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_OVERLAY:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DARKEN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned પૂર્ણાંક) mode;
	તોડો;
      મૂળભૂત:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	તોડો;
    }

    c->funcs->ધકેલો_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (આ+backdrop);
    c->funcs->ધકેલો_group_for (c->data, composite_mode);
    c->recurse (આ+src);
    c->funcs->કાઢો_group (c->data, composite_mode);
    c->funcs->કાઢો_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના ClipBoxData
{
  પૂર્ણાંક xMin, yMin, xMax, yMax;
};

સંરચના ClipBoxFormat1
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_clip_box (ClipBoxData &clip_box, અચલ ItemVarStoreInstancer &instancer HB_UNUSED) અચલ
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((દ્વિગુણ) xMin + (દ્વિગુણ) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((દ્વિગુણ) yMin + (દ્વિગુણ) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((દ્વિગુણ) xMax + (દ્વિગુણ) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((દ્વિગુણ) yMax + (દ્વિગુણ) ceilf (instancer (varIdxBase, 3)));
    }

    જો (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (સાચું);
  }

  જાહેર:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  જાહેર:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

સંરચના ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  શૂન્ય get_clip_box (ClipBoxData &clip_box, અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    value.get_clip_box(clip_box, instancer);
    જો (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<પૂર્ણાંક> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<પૂર્ણાંક> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<પૂર્ણાંક> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<પૂર્ણાંક> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

સંરચના ClipBox
{
  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 2: hb_barrier (); u.format2.closurev1 (c); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક get_extents (hb_glyph_extents_t *extents,
                    અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    ClipBoxData clip_box;
    પસંદગી (u.format.v) {
    સ્થિતિ 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      તોડો;
    સ્થિતિ 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      તોડો;
    મૂળભૂત:
      પાછા ખોટું;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    પાછા સાચું;
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

સંરચના ClipRecord
{
  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  શૂન્ય closurev1 (hb_colrv1_closure_context_t* c, અચલ શૂન્ય *base) અચલ
  {
    જો (!c->glyphs->intersects (startGlyphID, endGlyphID)) પાછા;
    (base+clipBox).closurev1 (c);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ શૂન્ય *base,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && clipBox.sanitize (c, base));
  }

  તાર્કિક get_extents (hb_glyph_extents_t *extents,
		    અચલ શૂન્ય *base,
		    અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    પાછા (base+clipBox).get_extents (extents, instancer);
  }

  જાહેર:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  જાહેર:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

સંરચના ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   અચલ ItemVarStoreInstancer &instancer,
                                   અચલ hb_set_t& gids,
                                   અચલ hb_map_t& gid_offset_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    જો (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned ગણતરી  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    માટે (અચલ hb_codepoint_t _ : gids.iter ())
    {
      જો (_ == start_gid) ચાલુ;

      offset = gid_offset_map.get (_);
      જો (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        ચાલુ;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      જો (!record.subset (c, આ, instancer)) return_trace (0);
      ગણતરી++;

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
      જો (!record.subset (c, આ, instancer)) return_trace (0);
      ગણતરી++;
    }
    return_trace (ગણતરી);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    જો (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ખોટું);

    અચલ hb_set_t& glyphset = c->plan->_glyphset_colred;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    માટે (અચલ ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      માટે (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        જો (!glyphset.has (gid) || !glyph_map.has (gid)) ચાલુ;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned ગણતરી = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    જો (!ગણતરી) return_trace (ખોટું);
    return_trace (c->serializer->check_assign (out->clips.len, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (આ) && clips.sanitize (c, આ));
  }

  તાર્કિક
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    auto *rec = clips.as_array ().bsearch (gid);
    જો (rec)
      પાછા rec->get_extents (extents, આ, instancer);
    પાછા ખોટું;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

સંરચના Paint
{

  template <typename ...Ts>
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_કિંમત ());

    return_trace (c->end_recursion (આ->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    સ્થિતિ 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    સ્થિતિ 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    સ્થિતિ 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    સ્થિતિ 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    સ્થિતિ 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    સ્થિતિ 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    સ્થિતિ 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    સ્થિતિ 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    સ્થિતિ 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    સ્થિતિ 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    સ્થિતિ 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    સ્થિતિ 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    સ્થિતિ 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    સ્થિતિ 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    સ્થિતિ 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    સ્થિતિ 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    સ્થિતિ 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    સ્થિતિ 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    સ્થિતિ 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    સ્થિતિ 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    સ્થિતિ 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    સ્થિતિ 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    સ્થિતિ 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    સ્થિતિ 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    સ્થિતિ 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    સ્થિતિ 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    સ્થિતિ 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    સ્થિતિ 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); પાછા સાચું;
    સ્થિતિ 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); પાછા સાચું;
    મૂળભૂત: પાછા ખોટું;
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT8 v; }				format;
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
  જાહેર:
  DEFINE_SIZE_MIN (2);
};

સંરચના BaseGlyphPaintRecord
{
  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  તાર્કિક serialize (hb_serialize_context_t *s, અચલ hb_map_t* glyph_map,
                  અચલ શૂન્ય* src_base, hb_subset_context_t *c,
                  અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = s->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    જો (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) && paint.sanitize (c, base)));
  }

  જાહેર:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out)))  return_trace (ખોટું);
    અચલ hb_set_t* glyphset = &c->plan->_glyphset_colred;

    માટે (અચલ auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      જો (!glyphset->has (gid)) ચાલુ;

      જો (_.serialize (c->serializer, c->plan->glyph_map, આ, c, instancer)) out->len++;
      નહીંતર return_trace (ખોટું);
    }

    return_trace (out->len != 0);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, આ));
  }
};

સંરચના LayerList : Array32OfOffset32To<Paint>
{
  અચલ Paint& get_paint (unsigned i) અચલ
  { પાછા આ+(*આ)[i]; }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ ItemVarStoreInstancer &instancer) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out)))  return_trace (ખોટું);

    તાર્કિક ret = ખોટું;
    માટે (અચલ auto& _ : + hb_enumerate (*આ)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      જો (unlikely (!o)) return_trace (ખોટું);
      ret |= o->serialize_subset (c, _.second, આ, instancer);
    }
    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, આ));
  }
};

સંરચના delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_ગણતરી () અચલ { પાછા inner_bit_ગણતરી; }
  unsigned get_width ()           અચલ { પાછા ((outer_bit_ગણતરી + inner_bit_ગણતરી + 7) / 8); }
  hb_array_t<અચલ uint32_t> get_output_map () અચલ { પાછા output_map.as_array (); }

  delta_set_index_map_subset_plan_t (અચલ hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ગણતરી = 0;
    outer_bit_ગણતરી = 0;
    inner_bit_ગણતરી = 1;
    output_map.init ();

    /* search backwards */
    unsigned ગણતરી = new_deltaset_idx_varidx_map.get_population ();
    જો (!ગણતરી) પાછા;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    માટે (unsigned i = ગણતરી; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      જો (i == ગણતરી)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        ચાલુ;
      }
      જો (var_idx != last_varidx)
        તોડો;
      last_idx = delta_set_idx;
    }

    map_ગણતરી = last_idx + 1;
  }

  તાર્કિક remap (અચલ hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_ગણતરી = 1;
    inner_bit_ગણતરી = 1;

    જો (unlikely (!output_map.resize_dirty (map_ગણતરી))) પાછા ખોટું;

    માટે (unsigned idx = 0; idx < map_ગણતરી; idx++)
    {
      uint32_t *var_idx;
      જો (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) પાછા ખોટું;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_ગણતરી = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ગણતરી = hb_max (bit_ગણતરી, outer_bit_ગણતરી);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_ગણતરી = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ગણતરી = hb_max (bit_ગણતરી, inner_bit_ગણતરી);
    }
    પાછા સાચું;
  }

  ખાનગી:
  unsigned map_ગણતરી;
  unsigned outer_bit_ગણતરી;
  unsigned inner_bit_ગણતરી;
  hb_vector_t<uint32_t> output_map;
};

સંરચના COLR
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  તાર્કિક has_data () અચલ { પાછા has_v0_data () || version; }

  તાર્કિક has_v0_data () અચલ { પાછા અંક_Base_Glyphs; }
  તાર્કિક has_v1_data () અચલ
  {
    જો (version < 1)
      પાછા ખોટું;
    hb_barrier ();

    પાછા (આ+baseGlyphList).len > 0;
  }

  unsigned પૂર્ણાંક get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned પૂર્ણાંક         start_offset,
				 unsigned પૂર્ણાંક        *ગણતરી, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) અચલ
  {
    અચલ BaseGlyphRecord &record = (આ+baseGlyphsZ).bsearch (અંક_Base_Glyphs, glyph);

    hb_array_t<અચલ LayerRecord> all_layers = (આ+layersZ).as_array (અંક_Layers);
    hb_array_t<અચલ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.અંક_Layers);
    જો (ગણતરી && layers)
    {
      + glyph_layers.sub_array (start_offset, ગણતરી)
      | hb_sink (hb_array (layers, *ગણતરી))
      ;
    }
    પાછા glyph_layers.length;
  }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      જો (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    તાર્કિક has_data () અચલ { પાછા colr->has_data (); }

#ifndef HB_NO_PAINT
    તાર્કિક
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) અચલ
    {
      જો (unlikely (!has_data ())) પાછા ખોટું;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      જો (unlikely (!scratch)) પાછા સાચું;
      તાર્કિક ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      પાછા ret;
    }

    તાર્કિક paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, શૂન્ય *data,
		      unsigned પૂર્ણાંક palette_index,
		      hb_color_t foreground,
		      તાર્કિક clip = સાચું) અચલ
    {
      જો (unlikely (!has_data ())) પાછા ખોટું;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      જો (unlikely (!scratch)) પાછા સાચું;
      તાર્કિક ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      પાછા ret;
    }
#endif

    તાર્કિક is_valid () { પાછા colr.get_blob ()->length; }

    શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ
    {
      જો (!has_data ()) પાછા;
      colr->depend (depend_data);
    }

    શૂન્ય closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) અચલ
    { colr->closure_glyphs (glyph, related_ids); }

    શૂન્ય closure_V0palette_indices (અચલ hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) અચલ
    { colr->closure_V0palette_indices (glyphs, palettes); }

    શૂન્ય closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) અચલ
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    તાર્કિક has_var_store () અચલ
    { પાછા colr->has_var_store (); }

    અચલ ItemVariationStore &get_var_store () અચલ
    { પાછા colr->get_var_store (); }
    અચલ ItemVariationStore *get_var_store_ptr () અચલ
    { પાછા colr->get_var_store_ptr (); }

    તાર્કિક has_delta_set_index_map () અચલ
    { પાછા colr->has_delta_set_index_map (); }

    અચલ DeltaSetIndexMap &get_delta_set_index_map () અચલ
    { પાછા colr->get_delta_set_index_map (); }
    અચલ DeltaSetIndexMap *get_delta_set_index_map_ptr () અચલ
    { પાછા colr->get_delta_set_index_map_ptr (); }

    ખાનગી:

    hb_colr_scratch_t *acquire_scratch () અચલ
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      જો (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, કદ (hb_colr_scratch_t));
	જો (unlikely (!scratch))
	  પાછા nullptr;
      }

      પાછા scratch;
    }
    શૂન્ય release_scratch (hb_colr_scratch_t *scratch) અચલ
    {
      જો (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    જાહેર:
    hb_blob_ptr_t<COLR> colr;
    ખાનગી:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ
  {
    // v0
    hb_array_t<અચલ BaseGlyphRecord> baseGlyphs = (આ+baseGlyphsZ).as_array (અંક_Base_Glyphs);
    માટે (અચલ BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (આ+layersZ).as_array (અંક_Layers).sub_array (record.firstLayerIdx,
                                                                         record.અંક_Layers);
      માટે (અચલ LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    જો (!has_v1_data())
      પાછા;

    // v1
    hb_colrv1_depend_context_t c (આ, depend_data);
    અચલ BaseGlyphList &baseglyph_paintrecords = આ+baseGlyphList;

    માટે (અચલ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      અચલ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  શૂન્ય closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) અચલ
  {
    અચલ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    જો (!record) પાછા;

    auto glyph_layers = (આ+layersZ).as_array (અંક_Layers).sub_array (record->firstLayerIdx,
								       record->અંક_Layers);
    જો (!glyph_layers.length) પાછા;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  શૂન્ય closure_V0palette_indices (અચલ hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) અચલ
  {
    જો (!અંક_Base_Glyphs || !અંક_Layers) પાછા;
    hb_array_t<અચલ BaseGlyphRecord> baseGlyphs = (આ+baseGlyphsZ).as_array (અંક_Base_Glyphs);
    hb_array_t<અચલ LayerRecord> all_layers = (આ+layersZ).as_array (અંક_Layers);

    માટે (અચલ BaseGlyphRecord record : baseGlyphs)
    {
      જો (!glyphs->has (record.glyphId)) ચાલુ;
      hb_array_t<અચલ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.અંક_Layers);
      માટે (અચલ LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  શૂન્ય closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) અચલ
  {
    જો (version < 1) પાછા;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (આ, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    અચલ BaseGlyphList &baseglyph_paintrecords = આ+baseGlyphList;

    માટે (અચલ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      જો (!glyphset->has (gid)) ચાલુ;

      અચલ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    અચલ ClipList &cliplist = આ+clipList;
    c.glyphs = glyphset;
    માટે (અચલ ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    જો (has_delta_set_index_map ())
    {
      અચલ DeltaSetIndexMap &var_idx_map = આ+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      માટે (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  અચલ LayerList& get_layerList () અચલ
  { પાછા (આ+layerList); }

  અચલ BaseGlyphList& get_baseglyphList () અચલ
  { પાછા (આ+baseGlyphList); }

  તાર્કિક has_var_store () અચલ
  { પાછા version >= 1 && hb_barrier () && varStore != 0; }

  તાર્કિક has_delta_set_index_map () અચલ
  { પાછા version >= 1 && hb_barrier () && varIdxMap != 0; }

  તાર્કિક has_clip_list () અચલ
  { પાછા version >= 1 && hb_barrier () && clipList != 0; }

  અચલ DeltaSetIndexMap &get_delta_set_index_map () અચલ
  { પાછા has_delta_set_index_map () && hb_barrier () ? આ+varIdxMap : Null (DeltaSetIndexMap); }
  અચલ DeltaSetIndexMap *get_delta_set_index_map_ptr () અચલ
  { પાછા has_delta_set_index_map () && hb_barrier () ? &(આ+varIdxMap) : nullptr; }

  અચલ ItemVariationStore &get_var_store () અચલ
  { પાછા has_var_store () && hb_barrier () ? આ+varStore : Null (ItemVariationStore); }
  અચલ ItemVariationStore *get_var_store_ptr () અચલ
  { પાછા has_var_store () && hb_barrier () ? &(આ+varStore) : nullptr; }

  અચલ ClipList &get_clip_list () અચલ
  { પાછા has_clip_list () && hb_barrier () ? આ+clipList : Null (ClipList); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
                  (આ+baseGlyphsZ).sanitize (c, અંક_Base_Glyphs) &&
                  (આ+layersZ).sanitize (c, અંક_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, આ) &&
		    layerList.sanitize (c, આ) &&
		    clipList.sanitize (c, આ) &&
		    varIdxMap.sanitize (c, આ) &&
		    varStore.sanitize (c, આ))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  તાર્કિક serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (base_it.len () != layer_it.len ()))
      return_trace (ખોટું);

    આ->version = version;
    અંક_Layers = 0;
    અંક_Base_Glyphs = base_it.len ();
    જો (અંક_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (સાચું);
    }

    c->push ();
    માટે (અચલ hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      જો (unlikely (!record)) return_trace (ખોટું);
      record->firstLayerIdx = અંક_Layers;
      અંક_Layers += record->અંક_Layers;
    }
    c->add_link (baseGlyphsZ, c->કાઢો_pack ());

    c->push ();
    માટે (અચલ hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->કાઢો_pack ());

    return_trace (સાચું);
  }

  અચલ BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) અચલ
  {
    અચલ BaseGlyphRecord* record = &(આ+baseGlyphsZ).bsearch (અંક_Base_Glyphs, (unsigned પૂર્ણાંક) gid);
    જો (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    પાછા record;
  }

  અચલ BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) અચલ
  {
    અચલ BaseGlyphList &list = આ+baseGlyphList;
    unsigned પૂર્ણાંક i;
    જો (!list.bfind ((unsigned) gid, &i))
      પાછા nullptr;
    પાછા &list[i];
  }

  તાર્કિક downgrade_to_V0 (અચલ hb_set_t &glyphset) અચલ
  {
    //no more COLRv1 glyphs, downgrade to version 0
    માટે (અચલ BaseGlyphPaintRecord& _ : get_baseglyphList ())
      જો (glyphset.has (_.glyphId))
        પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (સાચું);

    અચલ ItemVariationStore& var_store = આ+varStore;
    જો (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      તાર્કિક optimize = (varIdxMap != 0) ? સાચું : ખોટું;
      જો (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (ખોટું);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      જો (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ખોટું);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      જો (optimize)
      {
        અચલ hb_map_t &varidx_map = item_vars.get_varidx_map ();
        માટે (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          જો (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          નહીંતર
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    નહીંતર
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      જો (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      જો (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  તાર્કિક subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (સાચું);

    અચલ hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    જો (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (ખોટું);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    અચલ hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->અંક_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    જો (glyphset.has (old_gid)) પાછા સાચું;
		    પાછા ખોટું;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				અચલ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				જો (unlikely (!old_record))
				  પાછા hb_pair_t<તાર્કિક, BaseGlyphRecord> (ખોટું, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.અંક_Layers = old_record->અંક_Layers;
				પાછા hb_pair_t<તાર્કિક, BaseGlyphRecord> (સાચું, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->અંક_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				અચલ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				જો (unlikely (!old_record ||
					      old_record->firstLayerIdx >= અંક_Layers ||
					      old_record->firstLayerIdx + old_record->અંક_Layers > અંક_Layers))
				  પાછા hb_pair_t<તાર્કિક, hb_vector_t<LayerRecord>> (ખોટું, out_layers);

				auto layers = (આ+layersZ).as_array (અંક_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->અંક_Layers);
				out_layers.resize (layers.length);
				માટે (unsigned પૂર્ણાંક i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  જો (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    પાછા hb_pair_t<તાર્કિક, hb_vector_t<LayerRecord>> (ખોટું, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				પાછા hb_pair_t<તાર્કિક, hb_vector_t<LayerRecord>> (સાચું, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    જો (version == 0 && (!base_it || !layer_it))
      return_trace (ખોટું);

    auto *colr_અવિભાજ્ય = c->serializer->start_embed<COLR> ();
    જો (unlikely (!c->serializer->extend_min (colr_અવિભાજ્ય)))  return_trace (ખોટું);

    જો (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_અવિભાજ્ય->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    જો (!c->serializer->allocate_size<શૂન્ય> (5 * HBUINT32::static_size)) return_trace (ખોટું);
    જો (!colr_અવિભાજ્ય->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (ખોટું);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    જો (!subset_varstore (c, colr_અવિભાજ્ય)) return_trace (ખોટું);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    જો (!colr_અવિભાજ્ય->baseGlyphList.serialize_subset (c, baseGlyphList, આ, instancer))
      return_trace (ખોટું);

    colr_અવિભાજ્ય->layerList.serialize_subset (c, layerList, આ, instancer);
    colr_અવિભાજ્ય->clipList.serialize_subset (c, clipList, આ, instancer);

    return_trace (subset_delta_set_index_map (c, colr_અવિભાજ્ય));
  }

  અચલ Paint *get_base_glyph_paint (hb_codepoint_t glyph) અચલ
  {
    અચલ BaseGlyphList &baseglyph_paintrecords = આ+baseGlyphList;
    અચલ BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    જો (record)
    {
      અચલ Paint &paint = &baseglyph_paintrecords+record->paint;
      પાછા &paint;
    }
    નહીંતર
      પાછા nullptr;
  }

#ifndef HB_NO_PAINT
  તાર્કિક
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) અચલ
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->અંક_coords));

    જો (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      પાછા સાચું;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    તાર્કિક ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), સાચું, scratch);

    auto e = scratch.paint_extents.get_extents ();
    જો (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    નહીંતર
      *extents = e.to_glyph_extents ();

    પાછા ret;
  }
#endif

  તાર્કિક
  has_paint_for_glyph (hb_codepoint_t glyph) અચલ
  {
    જો (version >= 1)
    {
      hb_barrier ();

      અચલ Paint *paint = get_base_glyph_paint (glyph);

      પાછા paint != nullptr;
    }

    પાછા ખોટું;
  }

  તાર્કિક get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 અચલ ItemVarStoreInstancer instancer) અચલ
  {
    પાછા get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  તાર્કિક
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, શૂન્ય *data,
	       unsigned પૂર્ણાંક palette_index, hb_color_t foreground,
	       તાર્કિક clip,
	       hb_colr_scratch_t &scratch) અચલ
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->અંક_coords : 0));
    hb_paint_context_t c (આ, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_ગાંઠ_t node (c.glyphs_decycler);
    node.visit (glyph);

    જો (version >= 1)
    {
      hb_barrier ();

      અચલ Paint *paint = get_base_glyph_paint (glyph);
      જો (paint)
      {
        // COLRv1 glyph

	તાર્કિક is_bounded = સાચું;
	hb_glyph_extents_t extents = {};
	જો (clip)
	{
	  જો (!get_clip (glyph, &extents, instancer))
	  {
	    clip = ખોટું;
	    is_bounded = ખોટું;
	  }

	  જો (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 ખોટું,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ધકેલો_font_transform (c.data, font);

	જો (clip)
	  c.funcs->ધકેલો_clip_rectangle (c.data,
					(દશાંશ) extents.x_bearing,
					(દશાંશ) extents.y_bearing + extents.height,
					(દશાંશ) extents.x_bearing + extents.width,
					(દશાંશ) extents.y_bearing);

	જો (is_bounded)
	  c.recurse (*paint);

	જો (clip)
	  c.funcs->કાઢો_clip (c.data);

	c.funcs->કાઢો_transform (c.data);

        પાછા સાચું;
      }
    }

    અચલ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    જો (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      માટે (અચલ auto &r : (આ+layersZ).as_array (અંક_Layers)
			   .sub_array (record->firstLayerIdx, record->અંક_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      પાછા સાચું;
    }

    પાછા ખોટું;
  }
#endif

  સુરક્ષિત:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	અંક_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	અંક_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  જાહેર:
  DEFINE_SIZE_MIN (14);
};

સંરચના COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

શૂન્ય
hb_paint_context_t::recurse (અચલ Paint &paint)
{
  જો (unlikely (depth_ડાબું <= 0 || edge_ગણતરી <= 0)) પાછા;
  depth_ડાબું--;
  edge_ગણતરી--;
  paint.dispatch (આ);
  depth_ડાબું++;
}

template <typename Types>
શૂન્ય PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) અચલ
{
  TRACE_PAINT (આ);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  જો (likely (c->depth_ડાબું > 0 && c->edge_ગણતરી > 0) &&
      (આ+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_ગણતરી--;
    c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->કાઢો_transform (c->data);
    પાછા;
  }

  c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
  c->funcs->ધકેલો_clip_glyph (c->data, gid, c->font);
  c->funcs->ધકેલો_font_transform (c->data, c->font);
  c->recurse (આ+paint);
  c->funcs->કાઢો_transform (c->data);
  c->funcs->કાઢો_clip (c->data);
  c->funcs->કાઢો_transform (c->data);
}

શૂન્ય PaintColrLayers::paint_glyph (hb_paint_context_t *c) અચલ
{
  TRACE_PAINT (આ);
  અચલ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_ગાંઠ_t node (c->layers_decycler);
  માટે (unsigned i = firstLayerIndex; i < firstLayerIndex + અંક_Layers; i++)
  {
    જો (unlikely (!node.visit (i)))
      પાછા;

    અચલ Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

શૂન્ય PaintColrGlyph::paint_glyph (hb_paint_context_t *c) અચલ
{
  TRACE_PAINT (આ);

  hb_decycler_ગાંઠ_t node (c->glyphs_decycler);
  જો (unlikely (!node.visit (gid)))
    પાછા;

  c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
  જો (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->કાઢો_transform (c->data);
    પાછા;
  }
  c->funcs->કાઢો_transform (c->data);

  અચલ COLR *colr_table = c->get_colr_table ();
  અચલ Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  તાર્કિક has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  જો (has_clip_box)
    c->funcs->ધકેલો_clip_rectangle (c->data,
				   (દશાંશ) extents.x_bearing,
				   (દશાંશ) extents.y_bearing + extents.height,
				   (દશાંશ) extents.x_bearing + extents.width,
				   (દશાંશ) extents.y_bearing);

  જો (paint)
    c->recurse (*paint);

  જો (has_clip_box)
    c->funcs->કાઢો_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
