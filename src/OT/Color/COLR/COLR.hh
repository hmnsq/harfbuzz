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

#अन्तर्भाव "../../../hb.hh"
#अन्तर्भाव "../../../hb-decycler.hh"
#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-ot-var-common.hh"
#अन्तर्भाव "../../../hb-paint.hh"
#अन्तर्भाव "../../../hb-paint-bounded.hh"
#अन्तर्भाव "../../../hb-paint-extents.hh"
#अन्तर्भाव "../../../hb-depend-data.hh"

#अन्तर्भाव "../../../hb-ot-dual.hh"
#अन्तर्भाव "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

नामाकाश OT {
संरचना hb_paint_context_t;
}

संरचना hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

नामाकाश OT {

संरचना COLR;

संरचना Paint;

संरचना hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ध्रुव वर्ण *get_name () { प्रत्यागम "PAINT"; }
  template <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.paint_glyph (एतत्); प्रत्यागम hb_empty_t (); }
  स्थिर return_t default_return_मानम् () { प्रत्यागम hb_empty_t (); }

  ध्रुव COLR* get_colr_table () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव COLR *> (base); }

सार्वजनिक:
  ध्रुव शून्य *base;
  hb_paint_funcs_t *funcs;
  शून्य *data;
  hb_font_t *font;
  hb_array_t<ध्रुव BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  पूर्णाङ्क depth_वामः = HB_MAX_NESTING_LEVEL;
  पूर्णाङ्क edge_गणनम् = HB_MAX_GRAPH_EDGE_गणनम्;

  hb_paint_context_t (ध्रुव शून्य *base_,
		      hb_paint_funcs_t *funcs_,
		      शून्य *data_,
                      hb_font_t *font_,
                      unsigned पूर्णाङ्क palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_गणनम् () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    जइ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, सच्च);
      hb_font_set_synthetic_slant (font, 0);
    }
    अण्णहा
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  स्थिर unsigned
  color_alpha (hb_color_t color, भिन्न alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    प्रत्यागम roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned पूर्णाङ्क color_index, भिन्न alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = सच्च;

    जइ (color_index != 0xffff)
    {
      जइ (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = मिच्छ;
    }

    प्रत्यागम HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  अन्तःस्थ शून्य recurse (ध्रुव Paint &paint);
};

संरचना hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ध्रुव T &obj)
  {
    जइ (unlikely (nesting_level_वामः == 0))
      प्रत्यागम hb_empty_t ();

    जइ (paint_visited (&obj))
      प्रत्यागम hb_empty_t ();

    nesting_level_वामः--;
    obj.closurev1 (एतत्);
    nesting_level_वामः++;
    प्रत्यागम hb_empty_t ();
  }
  स्थिर return_t default_return_मानम् () { प्रत्यागम hb_empty_t (); }

  तार्किक paint_visited (ध्रुव शून्य *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    जइ (visited_paint.in_error() || visited_paint.has (delta))
      प्रत्यागम सच्च;

    visited_paint.add (delta);
    प्रत्यागम मिच्छ;
  }

  ध्रुव COLR* get_colr_table () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव COLR *> (base); }

  शून्य add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  शून्य add_layer_indices (unsigned first_layer_index, unsigned अङ्कः_of_layers)
  {
    जइ (अङ्कः_of_layers == 0) प्रत्यागम;
    layer_indices->add_range (first_layer_index, first_layer_index + अङ्कः_of_layers - 1);
  }

  शून्य add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  शून्य add_var_idxes (unsigned first_var_idx, unsigned अङ्कः_idxes)
  {
    जइ (!अङ्कः_idxes || first_var_idx == VarIdx::NO_VARIATION) प्रत्यागम;
    variation_indices->add_range (first_var_idx, first_var_idx + अङ्कः_idxes - 1);
  }

  सार्वजनिक:
  ध्रुव शून्य *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned अङ्कः_var_idxes;
  unsigned nesting_level_वामः;

  hb_colrv1_closure_context_t (ध्रुव शून्य *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned अङ्कः_var_idxes_2 = 1,
                               unsigned nesting_level_वामः_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          अङ्कः_var_idxes (अङ्कः_var_idxes_2),
                          nesting_level_वामः (nesting_level_वामः_2)
  {}
};


संरचना hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ध्रुव T &obj)
  {
    जइ (unlikely (nesting_level_वामः == 0))
      प्रत्यागम hb_empty_t ();

    जइ (paint_visited (&obj))
      प्रत्यागम hb_empty_t ();

    nesting_level_वामः--;
    obj.dependv1 (एतत्);
    nesting_level_वामः++;
    प्रत्यागम hb_empty_t ();
  }
  स्थिर return_t default_return_मानम् () { प्रत्यागम hb_empty_t (); }

  तार्किक paint_visited (ध्रुव शून्य *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    जइ (visited_paint.in_error() || visited_paint.has (delta))
      प्रत्यागम सच्च;

    visited_paint.add (delta);
    जइ (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    प्रत्यागम मिच्छ;
  }

  ध्रुव COLR* get_colr_table () ध्रुव
  { प्रत्यागम colr; }

  सार्वजनिक:
  ध्रुव COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_वामः;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ध्रुव COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_वामः_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_वामः (nesting_level_वामः_2),
                          source_gid (0)
  {}
};

संरचना LayerRecord
{
  operator hb_ot_color_layer_t () ध्रुव { प्रत्यागम {glyphId, colorIdx}; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना BaseGlyphRecord
{
  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	अङ्कः_Layers;	/* Number of color layers
				 * associated with this glyph */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
संरचना Variable
{
  स्थिर constexpr तार्किक is_variable = सच्च;

  ध्रुव T& get_मानम् () ध्रुव { प्रत्यागम value; }

  Variable<T>* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
  { value.dependv1 (c); }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    c->अङ्कः_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->अङ्कः_var_idxes);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (!value.subset (c, instancer, varIdxBase)) return_trace (मिच्छ);
    जइ (c->plan->all_axes_pinned)
      return_trace (सच्च);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    जइ (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, पूर्णाङ्क> *new_varidx_delta;
      जइ (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (मिच्छ);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }

  शून्य paint_glyph (hb_paint_context_t *c) ध्रुव
  {
    TRACE_PAINT (एतत्);
    value.paint_glyph (c, varIdxBase);
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  शून्य get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ध्रुव
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ध्रुव
  {
    प्रत्यागम value.get_extend ();
  }

  रक्षित:
  T      value;
  सार्वजनिक:
  VarIdx varIdxBase;
  सार्वजनिक:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
संरचना NoVariable
{
  स्थिर constexpr तार्किक is_variable = मिच्छ;

  ध्रुव T& get_मानम् () ध्रुव { प्रत्यागम value; }

  स्थिर constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
  { value.dependv1 (c); }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  { value.closurev1 (c); }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }

  शून्य paint_glyph (hb_paint_context_t *c) ध्रुव
  {
    TRACE_PAINT (एतत्);
    value.paint_glyph (c, varIdxBase);
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  शून्य get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ध्रुव
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ध्रुव
  {
    प्रत्यागम value.get_extend ();
  }

  T      value;
  सार्वजनिक:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
स्थिर शून्य
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, भिन्न value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

संरचना ColorStop
{
  शून्य dependv1 (hb_colrv1_depend_context_t * c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    c->add_palette_index (paletteIndex);
    c->अङ्कः_var_idxes = 2;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      भिन्न a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

संरचना Extend : HBUINT8
{
  गणना {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  सार्वजनिक:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> वर्ग Var>
संरचना Color_पङ्क्तिः
{
  शून्य dependv1 (hb_colrv1_depend_context_t * c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    प्रति (ध्रुव auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    जइ (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (मिच्छ);
    जइ (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (मिच्छ);

    प्रति (ध्रुव auto& stop : stops.iter ())
    {
      जइ (!stop.subset (c, instancer)) return_trace (मिच्छ);
    }
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned पूर्णाङ्क
  get_color_stops (hb_paint_context_t *c,
                   unsigned पूर्णाङ्क start,
		   unsigned पूर्णाङ्क *गणनम्,
		   hb_color_stop_t *color_stops,
		   ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    unsigned पूर्णाङ्क len = stops.len;

    जइ (गणनम् && color_stops)
    {
      unsigned पूर्णाङ्क i;
      प्रति (i = 0; i < *गणनम् && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *गणनम् = i;
    }

    प्रत्यागम len;
  }

  HB_INTERNAL स्थिर unsigned पूर्णाङ्क static_get_color_stops (hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
							  शून्य *color_पङ्क्तिः_data,
							  unsigned पूर्णाङ्क start,
							  unsigned पूर्णाङ्क *गणनम्,
							  hb_color_stop_t *color_stops,
							  शून्य *user_data)
  {
    ध्रुव Color_पङ्क्तिः *thiz = (ध्रुव Color_पङ्क्तिः *) color_पङ्क्तिः_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    प्रत्यागम thiz->get_color_stops (c, start, गणनम्, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ध्रुव
  {
    विकल्प ((unsigned पूर्णाङ्क) extend)
    {
      पक्ष Extend::EXTEND_REPEAT:  प्रत्यागम HB_PAINT_EXTEND_REPEAT;
      पक्ष Extend::EXTEND_REFLECT: प्रत्यागम HB_PAINT_EXTEND_REFLECT;
      पक्ष Extend::EXTEND_PAD:
      स्वभाव:                     प्रत्यागम HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL स्थिर hb_paint_extend_t static_get_extend (hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
							  शून्य *color_पङ्क्तिः_data,
							  शून्य *user_data)
  {
    ध्रुव Color_पङ्क्तिः *thiz = (ध्रुव Color_पङ्क्तिः *) color_पङ्क्तिः_data;
    प्रत्यागम thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
संरचना CompositeMode : HBUINT8
{
  सार्वजनिक:
  DEFINE_SIZE_STATIC (1);
};

संरचना Affine2x3
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  { c->अङ्कः_var_idxes = 6; }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);
    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((द्विगुण) source.to_int () +
                                                    (द्विगुण) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (सच्च);
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    c->funcs->प्रक्षिप_transform (c->data,
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

संरचना PaintColrLayers
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer HB_UNUSED) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    uint32_t first_layer_index = अङ्कः_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  अन्तःस्थ शून्य paint_glyph (hb_paint_context_t *c) ध्रुव;

  HBUINT8	format; /* format = 1 */
  HBUINT8	अङ्कः_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना PaintSolid
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव {}
  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    c->add_palette_index (paletteIndex);
    c->अङ्कः_var_idxes = 1;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भिन्न a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    जइ (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ध्रुव
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> वर्ग Var>
संरचना PaintLinearGradient
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    (एतत्+color_पङ्क्तिः_2).closurev1 (c);
    c->अङ्कः_var_idxes = 6;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुण) x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुण) y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुण) x1 + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुण) y1 + (द्विगुण) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((द्विगुण) x2 + (द्विगुण) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((द्विगुण) y2 + (द्विगुण) roundf (instancer (varIdxBase, 5)));
    }

    जइ (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_पङ्क्तिः_2.serialize_subset (c, color_पङ्क्तिः_2, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && color_पङ्क्तिः_2.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    hb_color_पङ्क्तिः_t cl = {
      (शून्य *) &(एतत्+color_पङ्क्तिः_2),
      (एतत्+color_पङ्क्तिः_2).static_get_color_stops, c,
      (एतत्+color_पङ्क्तिः_2).static_get_extend, nullptr
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
  Offset24To<Color_पङ्क्तिः<Var>>	color_पङ्क्तिः_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> वर्ग Var>
संरचना PaintRadialGradient
{
  तार्किक constant_after_pinning (hb_subset_context_t *c,
                               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    जइ (c->plan->has_avar2) प्रत्यागम मिच्छ;
    ध्रुव auto &regions = instancer.varStore->get_region_list ();
    प्रति (unsigned i = 0; i < regions.region_गणनम्; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      जइ (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        प्रत्यागम मिच्छ;
      }
      प्रति (auto axis : axes)
      {
        Triple *limit;
        जइ (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          प्रत्यागम मिच्छ;
      }
    }
    प्रत्यागम सच्च;
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    (एतत्+color_पङ्क्तिः_2).closurev1 (c);
    c->अङ्कः_var_idxes = 6;
  }

  तार्किक subset_nonnegative (hb_subset_context_t *c,
                           ध्रुव ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ध्रुव
  {
    द्विगुण source[6] = {
      x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)),
      y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)),
      radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)),
      x1 + (द्विगुण) roundf (instancer (varIdxBase, 3)),
      y1 + (द्विगुण) roundf (instancer (varIdxBase, 4)),
      radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      जइ (!out) प्रत्यागम मिच्छ;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      प्रत्यागम सच्च;
    };
    ध्रुव auto &पङ्क्तिः = एतत्+color_पङ्क्तिः_2;
    जइ (source[2] == source[5] || !पङ्क्तिः.stops.len) प्रत्यागम transparent ();
    संरचना stop_t { द्विगुण offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    जइ (!stops.alloc (पङ्क्तिः.stops.len)) प्रत्यागम मिच्छ;
    unsigned order = 0;
    प्रति (ध्रुव auto &entry : पङ्क्तिः.stops.iter ())
    {
      ध्रुव auto &stop = entry.get_मानम् ();
      stops.push (stop_t {(द्विगुण) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (द्विगुण) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    जइ (stops.in_error ()) प्रत्यागम मिच्छ;
    stops.qsort ([] (ध्रुव stop_t &a, ध्रुव stop_t &b) {
      प्रत्यागम a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    द्विगुण dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    प्रति (unsigned j = 0; j < 3; j++)
    {
      द्विगुण remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      जइ (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        प्रत्यागम मिच्छ;
      }
      unsigned दक्षिणः = (unsigned) remainder;
      जाव (दक्षिणः) { unsigned r = lattice % दक्षिणः; lattice = दक्षिणः; दक्षिणः = r; }
    }
    प्रति (unsigned shift = 0; shift < 16; shift++)
    {
      द्विगुण b = 1u << shift;
      द्विगुण coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      द्विगुण fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      द्विगुण candidates[] = {coarse, fine};
      प्रति (द्विगुण a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        द्विगुण geometry[6];
        तार्किक fits = सच्च;
        प्रति (unsigned j = 0; j < 3; j++)
        {
          द्विगुण difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          द्विगुण नीचः = j == 2 ? 0 : -32768, उच्चः = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= नीचः && geometry[j] <= उच्चः &&
                  geometry[j + 3] >= नीचः && geometry[j + 3] <= उच्चः;
        }
        जइ (!fits) अनुवर्त;
        unsigned first = 0, गणनम् = stops.length;
        द्विगुण min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        द्विगुण bias = 0;
        तार्किक constant = मिच्छ;
        जइ (पङ्क्तिः.extend == Extend::EXTEND_PAD)
        {
          द्विगुण tip = -geometry[2] / (b * dr);
          जइ ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            गणनम् = 1; constant = सच्च;
          }
        }
        अण्णहा
        {
          द्विगुण period = max - min;
          जइ (पङ्क्तिः.extend == Extend::EXTEND_REFLECT) period *= 2;
          जइ (period <= 0) प्रत्यागम transparent ();
          द्विगुण n = ceil ((-2 - min) / period);
          जइ (n > floor ((32767. / 16384. - max) / period)) अनुवर्त;
          bias = n * period;
        }
        प्रति (unsigned j = 0; j < गणनम्; j++)
        {
          द्विगुण bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        जइ (!fits) अनुवर्त;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        जइ (!out) प्रत्यागम मिच्छ;
        out->format = 6;
        out->x0 = (पूर्णाङ्क) geometry[0]; out->y0 = (पूर्णाङ्क) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (पूर्णाङ्क) geometry[3]; out->y1 = (पूर्णाङ्क) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_पङ्क्तिः = c->serializer->allocate_size<Color_पङ्क्तिः<NoVariable>> (3 + गणनम् * ColorStop::static_size);
        जइ (!out_पङ्क्तिः) { c->serializer->निष्कासय_discard (); प्रत्यागम मिच्छ; }
        out_पङ्क्तिः->extend = पङ्क्तिः.extend;
        out_पङ्क्तिः->stops.len = गणनम्;
        प्रति (unsigned j = 0; j < गणनम्; j++)
        {
          ध्रुव auto &stop = stops[first + j];
          auto &out_stop = out_पङ्क्तिः->stops[j].value;
          out_stop.stopOffset.set_float ((भिन्न) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((भिन्न) stop.alpha);
          जइ (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->निष्कासय_discard (); प्रत्यागम मिच्छ; }
        }
        c->serializer->add_link (out->color_पङ्क्तिः_2, c->serializer->निष्कासय_pack ());
        प्रत्यागम !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    प्रत्यागम मिच्छ;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुण) x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुण) y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((द्विगुण) radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुण) x1 + (द्विगुण) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुण) y1 + (द्विगुण) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((द्विगुण) radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5)));
    }

    जइ (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_पङ्क्तिः_2.serialize_subset (c, color_पङ्क्तिः_2, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && color_पङ्क्तिः_2.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    hb_color_पङ्क्तिः_t cl = {
      (शून्य *) &(एतत्+color_पङ्क्तिः_2),
      (एतत्+color_पङ्क्तिः_2).static_get_color_stops, c,
      (एतत्+color_पङ्क्तिः_2).static_get_extend, nullptr
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
  Offset24To<Color_पङ्क्तिः<Var>>	color_पङ्क्तिः_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> वर्ग Var>
संरचना PaintSweepGradient
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    (एतत्+color_पङ्क्तिः_2).closurev1 (c);
    c->अङ्कः_var_idxes = 4;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    जइ (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_पङ्क्तिः_2.serialize_subset (c, color_पङ्क्तिः_2, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && color_पङ्क्तिः_2.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    hb_color_पङ्क्तिः_t cl = {
      (शून्य *) &(एतत्+color_पङ्क्तिः_2),
      (एतत्+color_पङ्क्तिः_2).static_get_color_stops, c,
      (एतत्+color_पङ्क्तिः_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_पङ्क्तिः<Var>>	color_पङ्क्तिः_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
संरचना PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);

    return_trace (out->paint.serialize_subset (c, paint, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && paint.sanitize (c, एतत्));
  }

  अन्तःस्थ शून्य paint_glyph (hb_paint_context_t *c) ध्रुव;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

संरचना PaintColrGlyph
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer HB_UNUSED) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  अन्तःस्थ शून्य paint_glyph (hb_paint_context_t *c) ध्रुव;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> वर्ग Var>
संरचना PaintTransform
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);
    जइ (!out->transform.serialize_subset (c, transform, एतत्, instancer)) return_trace (मिच्छ);
    जइ (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  src.sanitize (c, एतत्) &&
                  transform.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c) ध्रुव
  {
    TRACE_PAINT (एतत्);
    (एतत्+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (7);
};

संरचना PaintTranslate
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((द्विगुण) dx + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((द्विगुण) dy + (द्विगुण) roundf (instancer (varIdxBase, 1)));
    }

    जइ (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न ddx = dx + c->instancer (varIdxBase, 0);
    भिन्न ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->प्रक्षिप_translate (c->data, ddx, ddy);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
स्थिर तार्किक serialize_scale_transform (hb_subset_context_t *c,
                                       भिन्न sx, भिन्न sy, भिन्न dx, भिन्न dy,
                                       ध्रुव Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  जइ (unlikely (!out)) प्रत्यागम मिच्छ;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  जइ (unlikely (!transform))
  {
    c->serializer->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->निष्कासय_pack ());

  c->serializer->push ();
  तार्किक ret = source ();
  जइ (ret)
    c->serializer->add_link (out->src, c->serializer->निष्कासय_pack ());
  अण्णहा
    c->serializer->निष्कासय_discard ();
  प्रत्यागम ret;
}

template <typename Base>
स्थिर तार्किक subset_scale_as_transform (hb_subset_context_t *c,
                                      ध्रुव Base *base,
                                      ध्रुव Offset24To<Paint> &src,
                                      ध्रुव ItemVarStoreInstancer &instancer,
                                      भिन्न sx, भिन्न sy,
                                      भिन्न cx = 0, भिन्न cy = 0)
{
  जइ (src.is_null ()) प्रत्यागम मिच्छ;
  auto source = [&] { प्रत्यागम c->dispatch (base+src, instancer); };
  भिन्न dx = cx * (1 - sx), dy = cy * (1 - sy);
  जइ (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    प्रत्यागम serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    प्रत्यागम serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    प्रत्यागम serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    प्रत्यागम serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    प्रत्यागम serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  प्रत्यागम serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

संरचना PaintScale
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भिन्न sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      भिन्न sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      जइ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, एतत्, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    जइ (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    भिन्न sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->प्रक्षिप_scale (c->data, sx, sy);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

संरचना PaintScaleAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भिन्न sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      भिन्न sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      जइ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, एतत्, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 3)));
    }

    जइ (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    भिन्न sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    भिन्न tCenterX = centerX + c->instancer (varIdxBase, 2);
    भिन्न tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->प्रक्षिप_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

संरचना PaintScaleUniform
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भिन्न s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      जइ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, एतत्, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    जइ (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->प्रक्षिप_scale (c->data, s, s);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

संरचना PaintScaleUniformAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भिन्न s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      जइ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, एतत्, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 2)));
    }

    जइ (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न s = scale.to_float (c->instancer (varIdxBase, 0));
    भिन्न tCenterX = centerX + c->instancer (varIdxBase, 1);
    भिन्न tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->प्रक्षिप_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

संरचना PaintRotate
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    जइ (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->प्रक्षिप_rotate (c->data, a);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

संरचना PaintRotateAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 2)));
    }

    जइ (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न a = angle.to_float (c->instancer (varIdxBase, 0));
    भिन्न tCenterX = centerX + c->instancer (varIdxBase, 1);
    भिन्न tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->प्रक्षिप_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

संरचना PaintSkew
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    जइ (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    भिन्न sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->प्रक्षिप_skew (c->data, sx, sy);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

संरचना PaintSkewAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 3)));
    }

    जइ (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, एतत्, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && src.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ध्रुव
  {
    TRACE_PAINT (एतत्);
    भिन्न sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    भिन्न sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    भिन्न tCenterX = centerX + c->instancer (varIdxBase, 2);
    भिन्न tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->प्रक्षिप_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

संरचना PaintComposite
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) ध्रुव;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव;

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    तार्किक ret = मिच्छ;
    ret |= out->src.serialize_subset (c, src, एतत्, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, एतत्, instancer);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  c->check_ops (एतत्->min_size) && // PainComposite can get exponential
                  src.sanitize (c, एतत्) &&
                  backdrop.sanitize (c, एतत्));
  }

  शून्य paint_glyph (hb_paint_context_t *c) ध्रुव
  {
    TRACE_PAINT (एतत्);
    hb_paint_composite_mode_t composite_mode;
    विकल्प ((unsigned पूर्णाङ्क) mode)
    {
      पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      पक्ष HB_PAINT_COMPOSITE_MODE_XOR:
      पक्ष HB_PAINT_COMPOSITE_MODE_PLUS:
      पक्ष HB_PAINT_COMPOSITE_MODE_SCREEN:
      पक्ष HB_PAINT_COMPOSITE_MODE_OVERLAY:
      पक्ष HB_PAINT_COMPOSITE_MODE_DARKEN:
      पक्ष HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      पक्ष HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      पक्ष HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      पक्ष HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      पक्ष HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      पक्ष HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      पक्ष HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      पक्ष HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      पक्ष HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      पक्ष HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned पूर्णाङ्क) mode;
	भङ्ग;
      स्वभाव:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	भङ्ग;
    }

    c->funcs->प्रक्षिप_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (एतत्+backdrop);
    c->funcs->प्रक्षिप_group_for (c->data, composite_mode);
    c->recurse (एतत्+src);
    c->funcs->निष्कासय_group (c->data, composite_mode);
    c->funcs->निष्कासय_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ClipBoxData
{
  पूर्णाङ्क xMin, yMin, xMax, yMax;
};

संरचना ClipBoxFormat1
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_clip_box (ClipBoxData &clip_box, ध्रुव ItemVarStoreInstancer &instancer HB_UNUSED) ध्रुव
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    जइ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((द्विगुण) xMin + (द्विगुण) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((द्विगुण) yMin + (द्विगुण) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((द्विगुण) xMax + (द्विगुण) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((द्विगुण) yMax + (द्विगुण) ceilf (instancer (varIdxBase, 3)));
    }

    जइ (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (सच्च);
  }

  सार्वजनिक:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

संरचना ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  शून्य get_clip_box (ClipBoxData &clip_box, ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    value.get_clip_box(clip_box, instancer);
    जइ (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<पूर्णाङ्क> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<पूर्णाङ्क> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<पूर्णाङ्क> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<पूर्णाङ्क> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

संरचना ClipBox
{
  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    पक्ष 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 2: hb_barrier (); u.format2.closurev1 (c); प्रत्यागम;
    स्वभाव:प्रत्यागम;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    जइ (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  तार्किक get_extents (hb_glyph_extents_t *extents,
                    ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    ClipBoxData clip_box;
    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      भङ्ग;
    पक्ष 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      भङ्ग;
    स्वभाव:
      प्रत्यागम मिच्छ;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    प्रत्यागम सच्च;
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

संरचना ClipRecord
{
  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  शून्य closurev1 (hb_colrv1_closure_context_t* c, ध्रुव शून्य *base) ध्रुव
  {
    जइ (!c->glyphs->intersects (startGlyphID, endGlyphID)) प्रत्यागम;
    (base+clipBox).closurev1 (c);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव शून्य *base,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && clipBox.sanitize (c, base));
  }

  तार्किक get_extents (hb_glyph_extents_t *extents,
		    ध्रुव शून्य *base,
		    ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    प्रत्यागम (base+clipBox).get_extents (extents, instancer);
  }

  सार्वजनिक:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  सार्वजनिक:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

संरचना ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ध्रुव ItemVarStoreInstancer &instancer,
                                   ध्रुव hb_set_t& gids,
                                   ध्रुव hb_map_t& gid_offset_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    जइ (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned गणनम्  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    प्रति (ध्रुव hb_codepoint_t _ : gids.iter ())
    {
      जइ (_ == start_gid) अनुवर्त;

      offset = gid_offset_map.get (_);
      जइ (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        अनुवर्त;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      जइ (!record.subset (c, एतत्, instancer)) return_trace (0);
      गणनम्++;

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
      जइ (!record.subset (c, एतत्, instancer)) return_trace (0);
      गणनम्++;
    }
    return_trace (गणनम्);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    जइ (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (मिच्छ);

    ध्रुव hb_set_t& glyphset = c->plan->_glyphset_colred;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    प्रति (ध्रुव ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      प्रति (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        जइ (!glyphset.has (gid) || !glyph_map.has (gid)) अनुवर्त;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned गणनम् = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    जइ (!गणनम्) return_trace (मिच्छ);
    return_trace (c->serializer->check_assign (out->clips.len, गणनम्, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (एतत्) && clips.sanitize (c, एतत्));
  }

  तार्किक
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    auto *rec = clips.as_array ().bsearch (gid);
    जइ (rec)
      प्रत्यागम rec->get_extents (extents, एतत्, instancer);
    प्रत्यागम मिच्छ;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

संरचना Paint
{

  template <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    जइ (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मानम् ());

    return_trace (c->end_recursion (एतत्->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    जइ (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    पक्ष 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    पक्ष 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    पक्ष 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    पक्ष 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    पक्ष 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    पक्ष 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    पक्ष 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    पक्ष 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    पक्ष 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    पक्ष 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    पक्ष 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    पक्ष 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    पक्ष 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    पक्ष 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    पक्ष 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    पक्ष 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    पक्ष 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    पक्ष 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    पक्ष 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    पक्ष 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    पक्ष 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    पक्ष 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    पक्ष 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    पक्ष 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    पक्ष 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    पक्ष 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    पक्ष 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    पक्ष 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  तार्किक get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); प्रत्यागम सच्च;
    पक्ष 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); प्रत्यागम सच्च;
    स्वभाव: प्रत्यागम मिच्छ;
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT8 v; }				format;
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

संरचना BaseGlyphPaintRecord
{
  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  तार्किक serialize (hb_serialize_context_t *s, ध्रुव hb_map_t* glyph_map,
                  ध्रुव शून्य* src_base, hb_subset_context_t *c,
                  ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = s->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);
    जइ (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) && paint.sanitize (c, base)));
  }

  सार्वजनिक:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    जइ (unlikely (!c->serializer->extend_min (out)))  return_trace (मिच्छ);
    ध्रुव hb_set_t* glyphset = &c->plan->_glyphset_colred;

    प्रति (ध्रुव auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      जइ (!glyphset->has (gid)) अनुवर्त;

      जइ (_.serialize (c->serializer, c->plan->glyph_map, एतत्, c, instancer)) out->len++;
      अण्णहा return_trace (मिच्छ);
    }

    return_trace (out->len != 0);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, एतत्));
  }
};

संरचना LayerList : Array32OfOffset32To<Paint>
{
  ध्रुव Paint& get_paint (unsigned i) ध्रुव
  { प्रत्यागम एतत्+(*एतत्)[i]; }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव ItemVarStoreInstancer &instancer) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    जइ (unlikely (!c->serializer->extend_min (out)))  return_trace (मिच्छ);

    तार्किक ret = मिच्छ;
    प्रति (ध्रुव auto& _ : + hb_enumerate (*एतत्)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      जइ (unlikely (!o)) return_trace (मिच्छ);
      ret |= o->serialize_subset (c, _.second, एतत्, instancer);
    }
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, एतत्));
  }
};

संरचना delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_गणनम् () ध्रुव { प्रत्यागम inner_bit_गणनम्; }
  unsigned get_width ()           ध्रुव { प्रत्यागम ((outer_bit_गणनम् + inner_bit_गणनम् + 7) / 8); }
  hb_array_t<ध्रुव uint32_t> get_output_map () ध्रुव { प्रत्यागम output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ध्रुव hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_गणनम् = 0;
    outer_bit_गणनम् = 0;
    inner_bit_गणनम् = 1;
    output_map.init ();

    /* search backwards */
    unsigned गणनम् = new_deltaset_idx_varidx_map.get_population ();
    जइ (!गणनम्) प्रत्यागम;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    प्रति (unsigned i = गणनम्; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      जइ (i == गणनम्)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        अनुवर्त;
      }
      जइ (var_idx != last_varidx)
        भङ्ग;
      last_idx = delta_set_idx;
    }

    map_गणनम् = last_idx + 1;
  }

  तार्किक remap (ध्रुव hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_गणनम् = 1;
    inner_bit_गणनम् = 1;

    जइ (unlikely (!output_map.resize_dirty (map_गणनम्))) प्रत्यागम मिच्छ;

    प्रति (unsigned idx = 0; idx < map_गणनम्; idx++)
    {
      uint32_t *var_idx;
      जइ (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) प्रत्यागम मिच्छ;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_गणनम् = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_गणनम् = hb_max (bit_गणनम्, outer_bit_गणनम्);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_गणनम् = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_गणनम् = hb_max (bit_गणनम्, inner_bit_गणनम्);
    }
    प्रत्यागम सच्च;
  }

  गुप्त:
  unsigned map_गणनम्;
  unsigned outer_bit_गणनम्;
  unsigned inner_bit_गणनम्;
  hb_vector_t<uint32_t> output_map;
};

संरचना COLR
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  तार्किक has_data () ध्रुव { प्रत्यागम has_v0_data () || version; }

  तार्किक has_v0_data () ध्रुव { प्रत्यागम अङ्कः_Base_Glyphs; }
  तार्किक has_v1_data () ध्रुव
  {
    जइ (version < 1)
      प्रत्यागम मिच्छ;
    hb_barrier ();

    प्रत्यागम (एतत्+baseGlyphList).len > 0;
  }

  unsigned पूर्णाङ्क get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned पूर्णाङ्क         start_offset,
				 unsigned पूर्णाङ्क        *गणनम्, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ध्रुव
  {
    ध्रुव BaseGlyphRecord &record = (एतत्+baseGlyphsZ).bsearch (अङ्कः_Base_Glyphs, glyph);

    hb_array_t<ध्रुव LayerRecord> all_layers = (एतत्+layersZ).as_array (अङ्कः_Layers);
    hb_array_t<ध्रुव LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.अङ्कः_Layers);
    जइ (गणनम् && layers)
    {
      + glyph_layers.sub_array (start_offset, गणनम्)
      | hb_sink (hb_array (layers, *गणनम्))
      ;
    }
    प्रत्यागम glyph_layers.length;
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      जइ (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    तार्किक has_data () ध्रुव { प्रत्यागम colr->has_data (); }

#ifndef HB_NO_PAINT
    तार्किक
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ध्रुव
    {
      जइ (unlikely (!has_data ())) प्रत्यागम मिच्छ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      जइ (unlikely (!scratch)) प्रत्यागम सच्च;
      तार्किक ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      प्रत्यागम ret;
    }

    तार्किक paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, शून्य *data,
		      unsigned पूर्णाङ्क palette_index,
		      hb_color_t foreground,
		      तार्किक clip = सच्च) ध्रुव
    {
      जइ (unlikely (!has_data ())) प्रत्यागम मिच्छ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      जइ (unlikely (!scratch)) प्रत्यागम सच्च;
      तार्किक ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      प्रत्यागम ret;
    }
#endif

    तार्किक is_valid () { प्रत्यागम colr.get_blob ()->length; }

    शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव
    {
      जइ (!has_data ()) प्रत्यागम;
      colr->depend (depend_data);
    }

    शून्य closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ध्रुव
    { colr->closure_glyphs (glyph, related_ids); }

    शून्य closure_V0palette_indices (ध्रुव hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ध्रुव
    { colr->closure_V0palette_indices (glyphs, palettes); }

    शून्य closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ध्रुव
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    तार्किक has_var_store () ध्रुव
    { प्रत्यागम colr->has_var_store (); }

    ध्रुव ItemVariationStore &get_var_store () ध्रुव
    { प्रत्यागम colr->get_var_store (); }
    ध्रुव ItemVariationStore *get_var_store_ptr () ध्रुव
    { प्रत्यागम colr->get_var_store_ptr (); }

    तार्किक has_delta_set_index_map () ध्रुव
    { प्रत्यागम colr->has_delta_set_index_map (); }

    ध्रुव DeltaSetIndexMap &get_delta_set_index_map () ध्रुव
    { प्रत्यागम colr->get_delta_set_index_map (); }
    ध्रुव DeltaSetIndexMap *get_delta_set_index_map_ptr () ध्रुव
    { प्रत्यागम colr->get_delta_set_index_map_ptr (); }

    गुप्त:

    hb_colr_scratch_t *acquire_scratch () ध्रुव
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      जइ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, परिमाण (hb_colr_scratch_t));
	जइ (unlikely (!scratch))
	  प्रत्यागम nullptr;
      }

      प्रत्यागम scratch;
    }
    शून्य release_scratch (hb_colr_scratch_t *scratch) ध्रुव
    {
      जइ (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    सार्वजनिक:
    hb_blob_ptr_t<COLR> colr;
    गुप्त:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव
  {
    // v0
    hb_array_t<ध्रुव BaseGlyphRecord> baseGlyphs = (एतत्+baseGlyphsZ).as_array (अङ्कः_Base_Glyphs);
    प्रति (ध्रुव BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (एतत्+layersZ).as_array (अङ्कः_Layers).sub_array (record.firstLayerIdx,
                                                                         record.अङ्कः_Layers);
      प्रति (ध्रुव LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    जइ (!has_v1_data())
      प्रत्यागम;

    // v1
    hb_colrv1_depend_context_t c (एतत्, depend_data);
    ध्रुव BaseGlyphList &baseglyph_paintrecords = एतत्+baseGlyphList;

    प्रति (ध्रुव BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ध्रुव Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  शून्य closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ध्रुव
  {
    ध्रुव BaseGlyphRecord *record = get_base_glyph_record (glyph);
    जइ (!record) प्रत्यागम;

    auto glyph_layers = (एतत्+layersZ).as_array (अङ्कः_Layers).sub_array (record->firstLayerIdx,
								       record->अङ्कः_Layers);
    जइ (!glyph_layers.length) प्रत्यागम;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  शून्य closure_V0palette_indices (ध्रुव hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ध्रुव
  {
    जइ (!अङ्कः_Base_Glyphs || !अङ्कः_Layers) प्रत्यागम;
    hb_array_t<ध्रुव BaseGlyphRecord> baseGlyphs = (एतत्+baseGlyphsZ).as_array (अङ्कः_Base_Glyphs);
    hb_array_t<ध्रुव LayerRecord> all_layers = (एतत्+layersZ).as_array (अङ्कः_Layers);

    प्रति (ध्रुव BaseGlyphRecord record : baseGlyphs)
    {
      जइ (!glyphs->has (record.glyphId)) अनुवर्त;
      hb_array_t<ध्रुव LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.अङ्कः_Layers);
      प्रति (ध्रुव LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  शून्य closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ध्रुव
  {
    जइ (version < 1) प्रत्यागम;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (एतत्, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ध्रुव BaseGlyphList &baseglyph_paintrecords = एतत्+baseGlyphList;

    प्रति (ध्रुव BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      जइ (!glyphset->has (gid)) अनुवर्त;

      ध्रुव Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ध्रुव ClipList &cliplist = एतत्+clipList;
    c.glyphs = glyphset;
    प्रति (ध्रुव ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    जइ (has_delta_set_index_map ())
    {
      ध्रुव DeltaSetIndexMap &var_idx_map = एतत्+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      प्रति (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ध्रुव LayerList& get_layerList () ध्रुव
  { प्रत्यागम (एतत्+layerList); }

  ध्रुव BaseGlyphList& get_baseglyphList () ध्रुव
  { प्रत्यागम (एतत्+baseGlyphList); }

  तार्किक has_var_store () ध्रुव
  { प्रत्यागम version >= 1 && hb_barrier () && varStore != 0; }

  तार्किक has_delta_set_index_map () ध्रुव
  { प्रत्यागम version >= 1 && hb_barrier () && varIdxMap != 0; }

  तार्किक has_clip_list () ध्रुव
  { प्रत्यागम version >= 1 && hb_barrier () && clipList != 0; }

  ध्रुव DeltaSetIndexMap &get_delta_set_index_map () ध्रुव
  { प्रत्यागम has_delta_set_index_map () && hb_barrier () ? एतत्+varIdxMap : Null (DeltaSetIndexMap); }
  ध्रुव DeltaSetIndexMap *get_delta_set_index_map_ptr () ध्रुव
  { प्रत्यागम has_delta_set_index_map () && hb_barrier () ? &(एतत्+varIdxMap) : nullptr; }

  ध्रुव ItemVariationStore &get_var_store () ध्रुव
  { प्रत्यागम has_var_store () && hb_barrier () ? एतत्+varStore : Null (ItemVariationStore); }
  ध्रुव ItemVariationStore *get_var_store_ptr () ध्रुव
  { प्रत्यागम has_var_store () && hb_barrier () ? &(एतत्+varStore) : nullptr; }

  ध्रुव ClipList &get_clip_list () ध्रुव
  { प्रत्यागम has_clip_list () && hb_barrier () ? एतत्+clipList : Null (ClipList); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
                  (एतत्+baseGlyphsZ).sanitize (c, अङ्कः_Base_Glyphs) &&
                  (एतत्+layersZ).sanitize (c, अङ्कः_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, एतत्) &&
		    layerList.sanitize (c, एतत्) &&
		    clipList.sanitize (c, एतत्) &&
		    varIdxMap.sanitize (c, एतत्) &&
		    varStore.sanitize (c, एतत्))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  तार्किक serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (base_it.len () != layer_it.len ()))
      return_trace (मिच्छ);

    एतत्->version = version;
    अङ्कः_Layers = 0;
    अङ्कः_Base_Glyphs = base_it.len ();
    जइ (अङ्कः_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (सच्च);
    }

    c->push ();
    प्रति (ध्रुव hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      जइ (unlikely (!record)) return_trace (मिच्छ);
      record->firstLayerIdx = अङ्कः_Layers;
      अङ्कः_Layers += record->अङ्कः_Layers;
    }
    c->add_link (baseGlyphsZ, c->निष्कासय_pack ());

    c->push ();
    प्रति (ध्रुव hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->निष्कासय_pack ());

    return_trace (सच्च);
  }

  ध्रुव BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ध्रुव
  {
    ध्रुव BaseGlyphRecord* record = &(एतत्+baseGlyphsZ).bsearch (अङ्कः_Base_Glyphs, (unsigned पूर्णाङ्क) gid);
    जइ (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    प्रत्यागम record;
  }

  ध्रुव BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ध्रुव
  {
    ध्रुव BaseGlyphList &list = एतत्+baseGlyphList;
    unsigned पूर्णाङ्क i;
    जइ (!list.bfind ((unsigned) gid, &i))
      प्रत्यागम nullptr;
    प्रत्यागम &list[i];
  }

  तार्किक downgrade_to_V0 (ध्रुव hb_set_t &glyphset) ध्रुव
  {
    //no more COLRv1 glyphs, downgrade to version 0
    प्रति (ध्रुव BaseGlyphPaintRecord& _ : get_baseglyphList ())
      जइ (glyphset.has (_.glyphId))
        प्रत्यागम मिच्छ;

    प्रत्यागम सच्च;
  }

  तार्किक subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (सच्च);

    ध्रुव ItemVariationStore& var_store = एतत्+varStore;
    जइ (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      तार्किक optimize = (varIdxMap != 0) ? सच्च : मिच्छ;
      जइ (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (मिच्छ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      जइ (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (मिच्छ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      जइ (optimize)
      {
        ध्रुव hb_map_t &varidx_map = item_vars.get_varidx_map ();
        प्रति (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          जइ (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          अण्णहा
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    अण्णहा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      जइ (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      जइ (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }

  तार्किक subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    जइ (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (सच्च);

    ध्रुव hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    जइ (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (मिच्छ);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ध्रुव hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->अङ्कः_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    जइ (glyphset.has (old_gid)) प्रत्यागम सच्च;
		    प्रत्यागम मिच्छ;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ध्रुव BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				जइ (unlikely (!old_record))
				  प्रत्यागम hb_pair_t<तार्किक, BaseGlyphRecord> (मिच्छ, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.अङ्कः_Layers = old_record->अङ्कः_Layers;
				प्रत्यागम hb_pair_t<तार्किक, BaseGlyphRecord> (सच्च, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->अङ्कः_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ध्रुव BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				जइ (unlikely (!old_record ||
					      old_record->firstLayerIdx >= अङ्कः_Layers ||
					      old_record->firstLayerIdx + old_record->अङ्कः_Layers > अङ्कः_Layers))
				  प्रत्यागम hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (मिच्छ, out_layers);

				auto layers = (एतत्+layersZ).as_array (अङ्कः_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->अङ्कः_Layers);
				out_layers.resize (layers.length);
				प्रति (unsigned पूर्णाङ्क i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  जइ (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    प्रत्यागम hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (मिच्छ, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				प्रत्यागम hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (सच्च, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    जइ (version == 0 && (!base_it || !layer_it))
      return_trace (मिच्छ);

    auto *colr_अविभाज्यः = c->serializer->start_embed<COLR> ();
    जइ (unlikely (!c->serializer->extend_min (colr_अविभाज्यः)))  return_trace (मिच्छ);

    जइ (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_अविभाज्यः->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    जइ (!c->serializer->allocate_size<शून्य> (5 * HBUINT32::static_size)) return_trace (मिच्छ);
    जइ (!colr_अविभाज्यः->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (मिच्छ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    जइ (!subset_varstore (c, colr_अविभाज्यः)) return_trace (मिच्छ);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    जइ (!colr_अविभाज्यः->baseGlyphList.serialize_subset (c, baseGlyphList, एतत्, instancer))
      return_trace (मिच्छ);

    colr_अविभाज्यः->layerList.serialize_subset (c, layerList, एतत्, instancer);
    colr_अविभाज्यः->clipList.serialize_subset (c, clipList, एतत्, instancer);

    return_trace (subset_delta_set_index_map (c, colr_अविभाज्यः));
  }

  ध्रुव Paint *get_base_glyph_paint (hb_codepoint_t glyph) ध्रुव
  {
    ध्रुव BaseGlyphList &baseglyph_paintrecords = एतत्+baseGlyphList;
    ध्रुव BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    जइ (record)
    {
      ध्रुव Paint &paint = &baseglyph_paintrecords+record->paint;
      प्रत्यागम &paint;
    }
    अण्णहा
      प्रत्यागम nullptr;
  }

#ifndef HB_NO_PAINT
  तार्किक
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ध्रुव
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->अङ्कः_coords));

    जइ (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      प्रत्यागम सच्च;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    तार्किक ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), सच्च, scratch);

    auto e = scratch.paint_extents.get_extents ();
    जइ (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    अण्णहा
      *extents = e.to_glyph_extents ();

    प्रत्यागम ret;
  }
#endif

  तार्किक
  has_paint_for_glyph (hb_codepoint_t glyph) ध्रुव
  {
    जइ (version >= 1)
    {
      hb_barrier ();

      ध्रुव Paint *paint = get_base_glyph_paint (glyph);

      प्रत्यागम paint != nullptr;
    }

    प्रत्यागम मिच्छ;
  }

  तार्किक get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ध्रुव ItemVarStoreInstancer instancer) ध्रुव
  {
    प्रत्यागम get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  तार्किक
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, शून्य *data,
	       unsigned पूर्णाङ्क palette_index, hb_color_t foreground,
	       तार्किक clip,
	       hb_colr_scratch_t &scratch) ध्रुव
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->अङ्कः_coords : 0));
    hb_paint_context_t c (एतत्, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_ग्रन्थिः_t node (c.glyphs_decycler);
    node.visit (glyph);

    जइ (version >= 1)
    {
      hb_barrier ();

      ध्रुव Paint *paint = get_base_glyph_paint (glyph);
      जइ (paint)
      {
        // COLRv1 glyph

	तार्किक is_bounded = सच्च;
	hb_glyph_extents_t extents = {};
	जइ (clip)
	{
	  जइ (!get_clip (glyph, &extents, instancer))
	  {
	    clip = मिच्छ;
	    is_bounded = मिच्छ;
	  }

	  जइ (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 मिच्छ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->प्रक्षिप_font_transform (c.data, font);

	जइ (clip)
	  c.funcs->प्रक्षिप_clip_rectangle (c.data,
					(भिन्न) extents.x_bearing,
					(भिन्न) extents.y_bearing + extents.height,
					(भिन्न) extents.x_bearing + extents.width,
					(भिन्न) extents.y_bearing);

	जइ (is_bounded)
	  c.recurse (*paint);

	जइ (clip)
	  c.funcs->निष्कासय_clip (c.data);

	c.funcs->निष्कासय_transform (c.data);

        प्रत्यागम सच्च;
      }
    }

    ध्रुव BaseGlyphRecord *record = get_base_glyph_record (glyph);
    जइ (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      प्रति (ध्रुव auto &r : (एतत्+layersZ).as_array (अङ्कः_Layers)
			   .sub_array (record->firstLayerIdx, record->अङ्कः_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      प्रत्यागम सच्च;
    }

    प्रत्यागम मिच्छ;
  }
#endif

  रक्षित:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	अङ्कः_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	अङ्कः_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  सार्वजनिक:
  DEFINE_SIZE_MIN (14);
};

संरचना COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

शून्य
hb_paint_context_t::recurse (ध्रुव Paint &paint)
{
  जइ (unlikely (depth_वामः <= 0 || edge_गणनम् <= 0)) प्रत्यागम;
  depth_वामः--;
  edge_गणनम्--;
  paint.dispatch (एतत्);
  depth_वामः++;
}

template <typename Types>
शून्य PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ध्रुव
{
  TRACE_PAINT (एतत्);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  जइ (likely (c->depth_वामः > 0 && c->edge_गणनम् > 0) &&
      (एतत्+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_गणनम्--;
    c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->निष्कासय_transform (c->data);
    प्रत्यागम;
  }

  c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
  c->funcs->प्रक्षिप_clip_glyph (c->data, gid, c->font);
  c->funcs->प्रक्षिप_font_transform (c->data, c->font);
  c->recurse (एतत्+paint);
  c->funcs->निष्कासय_transform (c->data);
  c->funcs->निष्कासय_clip (c->data);
  c->funcs->निष्कासय_transform (c->data);
}

शून्य PaintColrLayers::paint_glyph (hb_paint_context_t *c) ध्रुव
{
  TRACE_PAINT (एतत्);
  ध्रुव LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_ग्रन्थिः_t node (c->layers_decycler);
  प्रति (unsigned i = firstLayerIndex; i < firstLayerIndex + अङ्कः_Layers; i++)
  {
    जइ (unlikely (!node.visit (i)))
      प्रत्यागम;

    ध्रुव Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

शून्य PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ध्रुव
{
  TRACE_PAINT (एतत्);

  hb_decycler_ग्रन्थिः_t node (c->glyphs_decycler);
  जइ (unlikely (!node.visit (gid)))
    प्रत्यागम;

  c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
  जइ (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->निष्कासय_transform (c->data);
    प्रत्यागम;
  }
  c->funcs->निष्कासय_transform (c->data);

  ध्रुव COLR *colr_table = c->get_colr_table ();
  ध्रुव Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  तार्किक has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  जइ (has_clip_box)
    c->funcs->प्रक्षिप_clip_rectangle (c->data,
				   (भिन्न) extents.x_bearing,
				   (भिन्न) extents.y_bearing + extents.height,
				   (भिन्न) extents.x_bearing + extents.width,
				   (भिन्न) extents.y_bearing);

  जइ (paint)
    c->recurse (*paint);

  जइ (has_clip_box)
    c->funcs->निष्कासय_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
