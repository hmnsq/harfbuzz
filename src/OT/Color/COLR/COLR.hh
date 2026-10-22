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

#ಅನ್ತರ್ಭಾವ "../../../hb.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-decycler.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-var-common.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint-bounded.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint-extents.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-depend-data.hh"

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-dual.hh"
#ಅನ್ತರ್ಭಾವ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

ನಾಮಾಕಾಶ OT {
ಸಂರಚನಾ hb_paint_context_t;
}

ಸಂರಚನಾ hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

ನಾಮಾಕಾಶ OT {

ಸಂರಚನಾ COLR;

ಸಂರಚನಾ Paint;

ಸಂರಚನಾ hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "PAINT"; }
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.paint_glyph (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ಧ್ರುವ COLR* get_colr_table () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ COLR *> (base); }

ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ ಶೂನ್ಯ *base;
  hb_paint_funcs_t *funcs;
  ಶೂನ್ಯ *data;
  hb_font_t *font;
  hb_array_t<ಧ್ರುವ BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ಪೂರ್ಣಾಙ್ಕ depth_ಎಡ = HB_MAX_NESTING_LEVEL;
  ಪೂರ್ಣಾಙ್ಕ edge_ಎಣಿಕೆ = HB_MAX_GRAPH_EDGE_ಎಣಿಕೆ;

  hb_paint_context_t (ಧ್ರುವ ಶೂನ್ಯ *base_,
		      hb_paint_funcs_t *funcs_,
		      ಶೂನ್ಯ *data_,
                      hb_font_t *font_,
                      unsigned ಪೂರ್ಣಾಙ್ಕ palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_ಎಣಿಕೆ () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    ಯದಿ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, ಸತ್ಯ);
      hb_font_set_synthetic_slant (font, 0);
    }
    ಅನ್ಯಥಾ
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ಸ್ಥಿರ unsigned
  color_alpha (hb_color_t color, ಭಿನ್ನ alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    ಪ್ರತ್ಯಾಗಮ roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned ಪೂರ್ಣಾಙ್ಕ color_index, ಭಿನ್ನ alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = ಸತ್ಯ;

    ಯದಿ (color_index != 0xffff)
    {
      ಯದಿ (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ recurse (ಧ್ರುವ Paint &paint);
};

ಸಂರಚನಾ hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj)
  {
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0))
      ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

    ಯದಿ (paint_visited (&obj))
      ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

    nesting_level_ಎಡ--;
    obj.closurev1 (ಏತತ್);
    nesting_level_ಎಡ++;
    ಪ್ರತ್ಯಾಗಮ hb_empty_t ();
  }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ತಾರ್ಕಿಕ paint_visited (ಧ್ರುವ ಶೂನ್ಯ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    ಯದಿ (visited_paint.in_error() || visited_paint.has (delta))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    visited_paint.add (delta);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಧ್ರುವ COLR* get_colr_table () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ COLR *> (base); }

  ಶೂನ್ಯ add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  ಶೂನ್ಯ add_layer_indices (unsigned first_layer_index, unsigned ಅಂಕೆ_of_layers)
  {
    ಯದಿ (ಅಂಕೆ_of_layers == 0) ಪ್ರತ್ಯಾಗಮ;
    layer_indices->add_range (first_layer_index, first_layer_index + ಅಂಕೆ_of_layers - 1);
  }

  ಶೂನ್ಯ add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  ಶೂನ್ಯ add_var_idxes (unsigned first_var_idx, unsigned ಅಂಕೆ_idxes)
  {
    ಯದಿ (!ಅಂಕೆ_idxes || first_var_idx == VarIdx::NO_VARIATION) ಪ್ರತ್ಯಾಗಮ;
    variation_indices->add_range (first_var_idx, first_var_idx + ಅಂಕೆ_idxes - 1);
  }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ ಶೂನ್ಯ *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned ಅಂಕೆ_var_idxes;
  unsigned nesting_level_ಎಡ;

  hb_colrv1_closure_context_t (ಧ್ರುವ ಶೂನ್ಯ *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned ಅಂಕೆ_var_idxes_2 = 1,
                               unsigned nesting_level_ಎಡ_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          ಅಂಕೆ_var_idxes (ಅಂಕೆ_var_idxes_2),
                          nesting_level_ಎಡ (nesting_level_ಎಡ_2)
  {}
};


ಸಂರಚನಾ hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj)
  {
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0))
      ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

    ಯದಿ (paint_visited (&obj))
      ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

    nesting_level_ಎಡ--;
    obj.dependv1 (ಏತತ್);
    nesting_level_ಎಡ++;
    ಪ್ರತ್ಯಾಗಮ hb_empty_t ();
  }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ತಾರ್ಕಿಕ paint_visited (ಧ್ರುವ ಶೂನ್ಯ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    ಯದಿ (visited_paint.in_error() || visited_paint.has (delta))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    visited_paint.add (delta);
    ಯದಿ (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಧ್ರುವ COLR* get_colr_table () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ colr; }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_ಎಡ;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ಧ್ರುವ COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_ಎಡ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_ಎಡ (nesting_level_ಎಡ_2),
                          source_gid (0)
  {}
};

ಸಂರಚನಾ LayerRecord
{
  operator hb_ot_color_layer_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ {glyphId, colorIdx}; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ BaseGlyphRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	ಅಂಕೆ_Layers;	/* Number of color layers
				 * associated with this glyph */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ಸಂರಚನಾ Variable
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_variable = ಸತ್ಯ;

  ಧ್ರುವ T& get_ಮೌಲ್ಯ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value; }

  Variable<T>* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (ಏತತ್));
  }

  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
  { value.dependv1 (c); }

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    c->ಅಂಕೆ_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->ಅಂಕೆ_var_idxes);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!value.subset (c, instancer, varIdxBase)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಸತ್ಯ);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    ಯದಿ (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
      ಯದಿ (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (ಅಸತ್ಯ);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    value.paint_glyph (c, varIdxBase);
  }

  ಶೂನ್ಯ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  ಶೂನ್ಯ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ಧ್ರುವ
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ value.get_extend ();
  }

  ರಕ್ಷಿತ:
  T      value;
  ಸಾರ್ವಜನಿಕ:
  VarIdx varIdxBase;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
ಸಂರಚನಾ NoVariable
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_variable = ಅಸತ್ಯ;

  ಧ್ರುವ T& get_ಮೌಲ್ಯ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value; }

  ಸ್ಥಿರ constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (ಏತತ್));
  }

  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
  { value.dependv1 (c); }

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  { value.closurev1 (c); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    value.paint_glyph (c, varIdxBase);
  }

  ಶೂನ್ಯ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  ಶೂನ್ಯ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ಧ್ರುವ
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ value.get_extend ();
  }

  T      value;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
ಸ್ಥಿರ ಶೂನ್ಯ
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, ಭಿನ್ನ value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

ಸಂರಚನಾ ColorStop
{
  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t * c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    c->add_palette_index (paletteIndex);
    c->ಅಂಕೆ_var_idxes = 2;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      ಭಿನ್ನ a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

ಸಂರಚನಾ Extend : HBUINT8
{
  ಗಣನಾ {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> ವರ್ಗ Var>
ಸಂರಚನಾ Color_ಸಾಲು
{
  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t * c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto& stop : stops.iter ())
    {
      ಯದಿ (!stop.subset (c, instancer)) return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned ಪೂರ್ಣಾಙ್ಕ
  get_color_stops (hb_paint_context_t *c,
                   unsigned ಪೂರ್ಣಾಙ್ಕ start,
		   unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ,
		   hb_color_stop_t *color_stops,
		   ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ len = stops.len;

    ಯದಿ (ಎಣಿಕೆ && color_stops)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ i;
      ಪ್ರತಿ (i = 0; i < *ಎಣಿಕೆ && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *ಎಣಿಕೆ = i;
    }

    ಪ್ರತ್ಯಾಗಮ len;
  }

  HB_INTERNAL ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ static_get_color_stops (hb_color_ಸಾಲು_t *color_ಸಾಲು,
							  ಶೂನ್ಯ *color_ಸಾಲು_data,
							  unsigned ಪೂರ್ಣಾಙ್ಕ start,
							  unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ,
							  hb_color_stop_t *color_stops,
							  ಶೂನ್ಯ *user_data)
  {
    ಧ್ರುವ Color_ಸಾಲು *thiz = (ಧ್ರುವ Color_ಸಾಲು *) color_ಸಾಲು_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    ಪ್ರತ್ಯಾಗಮ thiz->get_color_stops (c, start, ಎಣಿಕೆ, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ ((unsigned ಪೂರ್ಣಾಙ್ಕ) extend)
    {
      ಪಕ್ಷ Extend::EXTEND_REPEAT:  ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_REPEAT;
      ಪಕ್ಷ Extend::EXTEND_REFLECT: ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_REFLECT;
      ಪಕ್ಷ Extend::EXTEND_PAD:
      ಸ್ವಭಾವ:                     ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL ಸ್ಥಿರ hb_paint_extend_t static_get_extend (hb_color_ಸಾಲು_t *color_ಸಾಲು,
							  ಶೂನ್ಯ *color_ಸಾಲು_data,
							  ಶೂನ್ಯ *user_data)
  {
    ಧ್ರುವ Color_ಸಾಲು *thiz = (ಧ್ರುವ Color_ಸಾಲು *) color_ಸಾಲು_data;
    ಪ್ರತ್ಯಾಗಮ thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
ಸಂರಚನಾ CompositeMode : HBUINT8
{
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (1);
};

ಸಂರಚನಾ Affine2x3
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  { c->ಅಂಕೆ_var_idxes = 6; }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((ದ್ವಿಗುಣ) source.to_int () +
                                                    (ದ್ವಿಗುಣ) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    c->funcs->ತಳ್ಳು_transform (c->data,
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

ಸಂರಚನಾ PaintColrLayers
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer HB_UNUSED) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    uint32_t first_layer_index = ಅಂಕೆ_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ;

  HBUINT8	format; /* format = 1 */
  HBUINT8	ಅಂಕೆ_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ PaintSolid
{
  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ {}
  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    c->add_palette_index (paletteIndex);
    c->ಅಂಕೆ_var_idxes = 1;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ಭಿನ್ನ a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    ಯದಿ (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ಧ್ರುವ
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> ವರ್ಗ Var>
ಸಂರಚನಾ PaintLinearGradient
{
  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    (ಏತತ್+color_ಸಾಲು_2).closurev1 (c);
    c->ಅಂಕೆ_var_idxes = 6;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) x0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) y0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) x1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) y1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) x2 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) y2 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 5)));
    }

    ಯದಿ (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_ಸಾಲು_2.serialize_subset (c, color_ಸಾಲು_2, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && color_ಸಾಲು_2.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    hb_color_ಸಾಲು_t cl = {
      (ಶೂನ್ಯ *) &(ಏತತ್+color_ಸಾಲು_2),
      (ಏತತ್+color_ಸಾಲು_2).static_get_color_stops, c,
      (ಏತತ್+color_ಸಾಲು_2).static_get_extend, nullptr
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
  Offset24To<Color_ಸಾಲು<Var>>	color_ಸಾಲು_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> ವರ್ಗ Var>
ಸಂರಚನಾ PaintRadialGradient
{
  ತಾರ್ಕಿಕ constant_after_pinning (hb_subset_context_t *c,
                               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    ಯದಿ (c->plan->has_avar2) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ auto &regions = instancer.varStore->get_region_list ();
    ಪ್ರತಿ (unsigned i = 0; i < regions.region_ಎಣಿಕೆ; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      ಯದಿ (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಪ್ರತಿ (auto axis : axes)
      {
        Triple *limit;
        ಯದಿ (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    (ಏತತ್+color_ಸಾಲು_2).closurev1 (c);
    c->ಅಂಕೆ_var_idxes = 6;
  }

  ತಾರ್ಕಿಕ subset_nonnegative (hb_subset_context_t *c,
                           ಧ್ರುವ ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ source[6] = {
      x0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 0)),
      y0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)),
      radius0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)),
      x1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 3)),
      y1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 4)),
      radius1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      ಯದಿ (!out) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    };
    ಧ್ರುವ auto &ಸಾಲು = ಏತತ್+color_ಸಾಲು_2;
    ಯದಿ (source[2] == source[5] || !ಸಾಲು.stops.len) ಪ್ರತ್ಯಾಗಮ transparent ();
    ಸಂರಚನಾ stop_t { ದ್ವಿಗುಣ offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    ಯದಿ (!stops.alloc (ಸಾಲು.stops.len)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned order = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : ಸಾಲು.stops.iter ())
    {
      ಧ್ರುವ auto &stop = entry.get_ಮೌಲ್ಯ ();
      stops.push (stop_t {(ದ್ವಿಗುಣ) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (ದ್ವಿಗುಣ) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    ಯದಿ (stops.in_error ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    stops.qsort ([] (ಧ್ರುವ stop_t &a, ಧ್ರುವ stop_t &b) {
      ಪ್ರತ್ಯಾಗಮ a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    ದ್ವಿಗುಣ dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    ಪ್ರತಿ (unsigned j = 0; j < 3; j++)
    {
      ದ್ವಿಗುಣ remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      ಯದಿ (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      unsigned ಬಲ = (unsigned) remainder;
      ಯಾವತ್ (ಬಲ) { unsigned r = lattice % ಬಲ; lattice = ಬಲ; ಬಲ = r; }
    }
    ಪ್ರತಿ (unsigned shift = 0; shift < 16; shift++)
    {
      ದ್ವಿಗುಣ b = 1u << shift;
      ದ್ವಿಗುಣ coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      ದ್ವಿಗುಣ fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      ದ್ವಿಗುಣ candidates[] = {coarse, fine};
      ಪ್ರತಿ (ದ್ವಿಗುಣ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        ದ್ವಿಗುಣ geometry[6];
        ತಾರ್ಕಿಕ fits = ಸತ್ಯ;
        ಪ್ರತಿ (unsigned j = 0; j < 3; j++)
        {
          ದ್ವಿಗುಣ difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          ದ್ವಿಗುಣ ಕೆಳಗಿನ = j == 2 ? 0 : -32768, ಮೇಲಿನ = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= ಕೆಳಗಿನ && geometry[j] <= ಮೇಲಿನ &&
                  geometry[j + 3] >= ಕೆಳಗಿನ && geometry[j + 3] <= ಮೇಲಿನ;
        }
        ಯದಿ (!fits) ಅನುವರ್ತ;
        unsigned first = 0, ಎಣಿಕೆ = stops.length;
        ದ್ವಿಗುಣ min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        ದ್ವಿಗುಣ bias = 0;
        ತಾರ್ಕಿಕ constant = ಅಸತ್ಯ;
        ಯದಿ (ಸಾಲು.extend == Extend::EXTEND_PAD)
        {
          ದ್ವಿಗುಣ tip = -geometry[2] / (b * dr);
          ಯದಿ ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            ಎಣಿಕೆ = 1; constant = ಸತ್ಯ;
          }
        }
        ಅನ್ಯಥಾ
        {
          ದ್ವಿಗುಣ period = max - min;
          ಯದಿ (ಸಾಲು.extend == Extend::EXTEND_REFLECT) period *= 2;
          ಯದಿ (period <= 0) ಪ್ರತ್ಯಾಗಮ transparent ();
          ದ್ವಿಗುಣ n = ceil ((-2 - min) / period);
          ಯದಿ (n > floor ((32767. / 16384. - max) / period)) ಅನುವರ್ತ;
          bias = n * period;
        }
        ಪ್ರತಿ (unsigned j = 0; j < ಎಣಿಕೆ; j++)
        {
          ದ್ವಿಗುಣ bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        ಯದಿ (!fits) ಅನುವರ್ತ;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        ಯದಿ (!out) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        out->format = 6;
        out->x0 = (ಪೂರ್ಣಾಙ್ಕ) geometry[0]; out->y0 = (ಪೂರ್ಣಾಙ್ಕ) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (ಪೂರ್ಣಾಙ್ಕ) geometry[3]; out->y1 = (ಪೂರ್ಣಾಙ್ಕ) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_ಸಾಲು = c->serializer->allocate_size<Color_ಸಾಲು<NoVariable>> (3 + ಎಣಿಕೆ * ColorStop::static_size);
        ಯದಿ (!out_ಸಾಲು) { c->serializer->ತೆಗೆ_discard (); ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
        out_ಸಾಲು->extend = ಸಾಲು.extend;
        out_ಸಾಲು->stops.len = ಎಣಿಕೆ;
        ಪ್ರತಿ (unsigned j = 0; j < ಎಣಿಕೆ; j++)
        {
          ಧ್ರುವ auto &stop = stops[first + j];
          auto &out_stop = out_ಸಾಲು->stops[j].value;
          out_stop.stopOffset.set_float ((ಭಿನ್ನ) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((ಭಿನ್ನ) stop.alpha);
          ಯದಿ (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->ತೆಗೆ_discard (); ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
        }
        c->serializer->add_link (out->color_ಸಾಲು_2, c->serializer->ತೆಗೆ_pack ());
        ಪ್ರತ್ಯಾಗಮ !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) x0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) y0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((ದ್ವಿಗುಣ) radius0 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) x1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) y1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((ದ್ವಿಗುಣ) radius1 + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 5)));
    }

    ಯದಿ (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_ಸಾಲು_2.serialize_subset (c, color_ಸಾಲು_2, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && color_ಸಾಲು_2.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    hb_color_ಸಾಲು_t cl = {
      (ಶೂನ್ಯ *) &(ಏತತ್+color_ಸಾಲು_2),
      (ಏತತ್+color_ಸಾಲು_2).static_get_color_stops, c,
      (ಏತತ್+color_ಸಾಲು_2).static_get_extend, nullptr
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
  Offset24To<Color_ಸಾಲು<Var>>	color_ಸಾಲು_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> ವರ್ಗ Var>
ಸಂರಚನಾ PaintSweepGradient
{
  ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ {}

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    (ಏತತ್+color_ಸಾಲು_2).closurev1 (c);
    c->ಅಂಕೆ_var_idxes = 4;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerX + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerY + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    ಯದಿ (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_ಸಾಲು_2.serialize_subset (c, color_ಸಾಲು_2, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && color_ಸಾಲು_2.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    hb_color_ಸಾಲು_t cl = {
      (ಶೂನ್ಯ *) &(ಏತತ್+color_ಸಾಲು_2),
      (ಏತತ್+color_ಸಾಲು_2).static_get_color_stops, c,
      (ಏತತ್+color_ಸಾಲು_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_ಸಾಲು<Var>>	color_ಸಾಲು_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
ಸಂರಚನಾ PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->paint.serialize_subset (c, paint, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && paint.sanitize (c, ಏತತ್));
  }

  ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

ಸಂರಚನಾ PaintColrGlyph
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer HB_UNUSED) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> ವರ್ಗ Var>
ಸಂರಚನಾ PaintTransform
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!out->transform.serialize_subset (c, transform, ಏತತ್, instancer)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  src.sanitize (c, ಏತತ್) &&
                  transform.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    (ಏತತ್+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (7);
};

ಸಂರಚನಾ PaintTranslate
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) dx + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) dy + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
    }

    ಯದಿ (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ ddx = dx + c->instancer (varIdxBase, 0);
    ಭಿನ್ನ ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ತಳ್ಳು_translate (c->data, ddx, ddy);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_scale_transform (hb_subset_context_t *c,
                                       ಭಿನ್ನ sx, ಭಿನ್ನ sy, ಭಿನ್ನ dx, ಭಿನ್ನ dy,
                                       ಧ್ರುವ Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  ಯದಿ (unlikely (!out)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  ಯದಿ (unlikely (!transform))
  {
    c->serializer->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->ತೆಗೆ_pack ());

  c->serializer->push ();
  ತಾರ್ಕಿಕ ret = source ();
  ಯದಿ (ret)
    c->serializer->add_link (out->src, c->serializer->ತೆಗೆ_pack ());
  ಅನ್ಯಥಾ
    c->serializer->ತೆಗೆ_discard ();
  ಪ್ರತ್ಯಾಗಮ ret;
}

template <typename Base>
ಸ್ಥಿರ ತಾರ್ಕಿಕ subset_scale_as_transform (hb_subset_context_t *c,
                                      ಧ್ರುವ Base *base,
                                      ಧ್ರುವ Offset24To<Paint> &src,
                                      ಧ್ರುವ ItemVarStoreInstancer &instancer,
                                      ಭಿನ್ನ sx, ಭಿನ್ನ sy,
                                      ಭಿನ್ನ cx = 0, ಭಿನ್ನ cy = 0)
{
  ಯದಿ (src.is_null ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  auto source = [&] { ಪ್ರತ್ಯಾಗಮ c->dispatch (base+src, instancer); };
  ಭಿನ್ನ dx = cx * (1 - sx), dy = cy * (1 - sy);
  ಯದಿ (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  ಪ್ರತ್ಯಾಗಮ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

ಸಂರಚನಾ PaintScale
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ಭಿನ್ನ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ಭಿನ್ನ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      ಯದಿ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, ಏತತ್, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    ಯದಿ (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ಭಿನ್ನ sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ತಳ್ಳು_scale (c->data, sx, sy);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ಸಂರಚನಾ PaintScaleAroundCenter
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ಭಿನ್ನ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ಭಿನ್ನ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      ಯದಿ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, ಏತತ್, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerX + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerY + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 3)));
    }

    ಯದಿ (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ಭಿನ್ನ sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    ಭಿನ್ನ tCenterX = centerX + c->instancer (varIdxBase, 2);
    ಭಿನ್ನ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ತಳ್ಳು_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ಸಂರಚನಾ PaintScaleUniform
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ಭಿನ್ನ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ಯದಿ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, ಏತತ್, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    ಯದಿ (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ತಳ್ಳು_scale (c->data, s, s);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ಸಂರಚನಾ PaintScaleUniformAroundCenter
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ಭಿನ್ನ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ಯದಿ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, ಏತತ್, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerX + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerY + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
    }

    ಯದಿ (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ s = scale.to_float (c->instancer (varIdxBase, 0));
    ಭಿನ್ನ tCenterX = centerX + c->instancer (varIdxBase, 1);
    ಭಿನ್ನ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ತಳ್ಳು_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ಸಂರಚನಾ PaintRotate
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    ಯದಿ (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ತಳ್ಳು_rotate (c->data, a);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ಸಂರಚನಾ PaintRotateAroundCenter
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerX + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerY + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
    }

    ಯದಿ (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ a = angle.to_float (c->instancer (varIdxBase, 0));
    ಭಿನ್ನ tCenterX = centerX + c->instancer (varIdxBase, 1);
    ಭಿನ್ನ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ತಳ್ಳು_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ಸಂರಚನಾ PaintSkew
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    ಯದಿ (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ಭಿನ್ನ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ತಳ್ಳು_skew (c->data, sx, sy);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ಸಂರಚನಾ PaintSkewAroundCenter
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerX + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) centerY + (ದ್ವಿಗುಣ) roundf (instancer (varIdxBase, 3)));
    }

    ಯದಿ (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, ಏತತ್, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && src.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    ಭಿನ್ನ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ಭಿನ್ನ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    ಭಿನ್ನ tCenterX = centerX + c->instancer (varIdxBase, 2);
    ಭಿನ್ನ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ತಳ್ಳು_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ಸಂರಚನಾ PaintComposite
{
  HB_INTERNAL ಶೂನ್ಯ dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ret |= out->src.serialize_subset (c, src, ಏತತ್, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, ಏತತ್, instancer);
    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  c->check_ops (ಏತತ್->min_size) && // PainComposite can get exponential
                  src.sanitize (c, ಏತತ್) &&
                  backdrop.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
  {
    TRACE_PAINT (ಏತತ್);
    hb_paint_composite_mode_t composite_mode;
    ವಿಕಲ್ಪ ((unsigned ಪೂರ್ಣಾಙ್ಕ) mode)
    {
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_CLEAR:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_IN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_XOR:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_PLUS:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SCREEN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_OVERLAY:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DARKEN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) mode;
	ಭಙ್ಗ;
      ಸ್ವಭಾವ:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	ಭಙ್ಗ;
    }

    c->funcs->ತಳ್ಳು_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (ಏತತ್+backdrop);
    c->funcs->ತಳ್ಳು_group_for (c->data, composite_mode);
    c->recurse (ಏತತ್+src);
    c->funcs->ತೆಗೆ_group (c->data, composite_mode);
    c->funcs->ತೆಗೆ_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ ClipBoxData
{
  ಪೂರ್ಣಾಙ್ಕ xMin, yMin, xMax, yMax;
};

ಸಂರಚನಾ ClipBoxFormat1
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_clip_box (ClipBoxData &clip_box, ಧ್ರುವ ItemVarStoreInstancer &instancer HB_UNUSED) ಧ್ರುವ
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) xMin + (ದ್ವಿಗುಣ) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) yMin + (ದ್ವಿಗುಣ) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) xMax + (ದ್ವಿಗುಣ) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((ದ್ವಿಗುಣ) yMax + (ದ್ವಿಗುಣ) ceilf (instancer (varIdxBase, 3)));
    }

    ಯದಿ (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

ಸಂರಚನಾ ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  ಶೂನ್ಯ get_clip_box (ClipBoxData &clip_box, ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    value.get_clip_box(clip_box, instancer);
    ಯದಿ (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

ಸಂರಚನಾ ClipBox
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 2: hb_barrier (); u.format2.closurev1 (c); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ get_extents (hb_glyph_extents_t *extents,
                    ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    ClipBoxData clip_box;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      ಭಙ್ಗ;
    ಪಕ್ಷ 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

ಸಂರಚನಾ ClipRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  ಶೂನ್ಯ closurev1 (hb_colrv1_closure_context_t* c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (!c->glyphs->intersects (startGlyphID, endGlyphID)) ಪ್ರತ್ಯಾಗಮ;
    (base+clipBox).closurev1 (c);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ಶೂನ್ಯ *base,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && clipBox.sanitize (c, base));
  }

  ತಾರ್ಕಿಕ get_extents (hb_glyph_extents_t *extents,
		    ಧ್ರುವ ಶೂನ್ಯ *base,
		    ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (base+clipBox).get_extents (extents, instancer);
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

ಸಂರಚನಾ ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ಧ್ರುವ ItemVarStoreInstancer &instancer,
                                   ಧ್ರುವ hb_set_t& gids,
                                   ಧ್ರುವ hb_map_t& gid_offset_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned ಎಣಿಕೆ  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    ಪ್ರತಿ (ಧ್ರುವ hb_codepoint_t _ : gids.iter ())
    {
      ಯದಿ (_ == start_gid) ಅನುವರ್ತ;

      offset = gid_offset_map.get (_);
      ಯದಿ (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        ಅನುವರ್ತ;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      ಯದಿ (!record.subset (c, ಏತತ್, instancer)) return_trace (0);
      ಎಣಿಕೆ++;

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
      ಯದಿ (!record.subset (c, ಏತತ್, instancer)) return_trace (0);
      ಎಣಿಕೆ++;
    }
    return_trace (ಎಣಿಕೆ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_set_t& glyphset = c->plan->_glyphset_colred;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    ಪ್ರತಿ (ಧ್ರುವ ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      ಪ್ರತಿ (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        ಯದಿ (!glyphset.has (gid) || !glyph_map.has (gid)) ಅನುವರ್ತ;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned ಎಣಿಕೆ = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    ಯದಿ (!ಎಣಿಕೆ) return_trace (ಅಸತ್ಯ);
    return_trace (c->serializer->check_assign (out->clips.len, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (ಏತತ್) && clips.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    auto *rec = clips.as_array ().bsearch (gid);
    ಯದಿ (rec)
      ಪ್ರತ್ಯಾಗಮ rec->get_extents (extents, ಏತತ್, instancer);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

ಸಂರಚನಾ Paint
{

  template <typename ...Ts>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ಮೌಲ್ಯ ());

    return_trace (c->end_recursion (ಏತತ್->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT8 v; }				format;
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2);
};

ಸಂರಚನಾ BaseGlyphPaintRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *s, ಧ್ರುವ hb_map_t* glyph_map,
                  ಧ್ರುವ ಶೂನ್ಯ* src_base, hb_subset_context_t *c,
                  ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = s->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) && paint.sanitize (c, base)));
  }

  ಸಾರ್ವಜನಿಕ:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out)))  return_trace (ಅಸತ್ಯ);
    ಧ್ರುವ hb_set_t* glyphset = &c->plan->_glyphset_colred;

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      ಯದಿ (!glyphset->has (gid)) ಅನುವರ್ತ;

      ಯದಿ (_.serialize (c->serializer, c->plan->glyph_map, ಏತತ್, c, instancer)) out->len++;
      ಅನ್ಯಥಾ return_trace (ಅಸತ್ಯ);
    }

    return_trace (out->len != 0);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, ಏತತ್));
  }
};

ಸಂರಚನಾ LayerList : Array32OfOffset32To<Paint>
{
  ಧ್ರುವ Paint& get_paint (unsigned i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+(*ಏತತ್)[i]; }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ItemVarStoreInstancer &instancer) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out)))  return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : + hb_enumerate (*ಏತತ್)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!o)) return_trace (ಅಸತ್ಯ);
      ret |= o->serialize_subset (c, _.second, ಏತತ್, instancer);
    }
    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, ಏತತ್));
  }
};

ಸಂರಚನಾ delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ inner_bit_ಎಣಿಕೆ; }
  unsigned get_width ()           ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ((outer_bit_ಎಣಿಕೆ + inner_bit_ಎಣಿಕೆ + 7) / 8); }
  hb_array_t<ಧ್ರುವ uint32_t> get_output_map () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ಧ್ರುವ hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ಎಣಿಕೆ = 0;
    outer_bit_ಎಣಿಕೆ = 0;
    inner_bit_ಎಣಿಕೆ = 1;
    output_map.init ();

    /* search backwards */
    unsigned ಎಣಿಕೆ = new_deltaset_idx_varidx_map.get_population ();
    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    ಪ್ರತಿ (unsigned i = ಎಣಿಕೆ; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      ಯದಿ (i == ಎಣಿಕೆ)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        ಅನುವರ್ತ;
      }
      ಯದಿ (var_idx != last_varidx)
        ಭಙ್ಗ;
      last_idx = delta_set_idx;
    }

    map_ಎಣಿಕೆ = last_idx + 1;
  }

  ತಾರ್ಕಿಕ remap (ಧ್ರುವ hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_ಎಣಿಕೆ = 1;
    inner_bit_ಎಣಿಕೆ = 1;

    ಯದಿ (unlikely (!output_map.resize_dirty (map_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned idx = 0; idx < map_ಎಣಿಕೆ; idx++)
    {
      uint32_t *var_idx;
      ಯದಿ (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_ಎಣಿಕೆ = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ಎಣಿಕೆ = hb_max (bit_ಎಣಿಕೆ, outer_bit_ಎಣಿಕೆ);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_ಎಣಿಕೆ = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ಎಣಿಕೆ = hb_max (bit_ಎಣಿಕೆ, inner_bit_ಎಣಿಕೆ);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಗುಪ್ತ:
  unsigned map_ಎಣಿಕೆ;
  unsigned outer_bit_ಎಣಿಕೆ;
  unsigned inner_bit_ಎಣಿಕೆ;
  hb_vector_t<uint32_t> output_map;
};

ಸಂರಚನಾ COLR
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_v0_data () || version; }

  ತಾರ್ಕಿಕ has_v0_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Base_Glyphs; }
  ತಾರ್ಕಿಕ has_v1_data () ಧ್ರುವ
  {
    ಯದಿ (version < 1)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_barrier ();

    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+baseGlyphList).len > 0;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned ಪೂರ್ಣಾಙ್ಕ         start_offset,
				 unsigned ಪೂರ್ಣಾಙ್ಕ        *ಎಣಿಕೆ, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ಧ್ರುವ
  {
    ಧ್ರುವ BaseGlyphRecord &record = (ಏತತ್+baseGlyphsZ).bsearch (ಅಂಕೆ_Base_Glyphs, glyph);

    hb_array_t<ಧ್ರುವ LayerRecord> all_layers = (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers);
    hb_array_t<ಧ್ರುವ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.ಅಂಕೆ_Layers);
    ಯದಿ (ಎಣಿಕೆ && layers)
    {
      + glyph_layers.sub_array (start_offset, ಎಣಿಕೆ)
      | hb_sink (hb_array (layers, *ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ glyph_layers.length;
  }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      ಯದಿ (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ colr->has_data (); }

#ifndef HB_NO_PAINT
    ತಾರ್ಕಿಕ
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ಧ್ರುವ
    {
      ಯದಿ (unlikely (!has_data ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ತಾರ್ಕಿಕ ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ತಾರ್ಕಿಕ paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, ಶೂನ್ಯ *data,
		      unsigned ಪೂರ್ಣಾಙ್ಕ palette_index,
		      hb_color_t foreground,
		      ತಾರ್ಕಿಕ clip = ಸತ್ಯ) ಧ್ರುವ
    {
      ಯದಿ (unlikely (!has_data ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ಯದಿ (unlikely (!scratch)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ತಾರ್ಕಿಕ ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      ಪ್ರತ್ಯಾಗಮ ret;
    }
#endif

    ತಾರ್ಕಿಕ is_valid () { ಪ್ರತ್ಯಾಗಮ colr.get_blob ()->length; }

    ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
    {
      ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ;
      colr->depend (depend_data);
    }

    ಶೂನ್ಯ closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ಧ್ರುವ
    { colr->closure_glyphs (glyph, related_ids); }

    ಶೂನ್ಯ closure_V0palette_indices (ಧ್ರುವ hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ಧ್ರುವ
    { colr->closure_V0palette_indices (glyphs, palettes); }

    ಶೂನ್ಯ closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ಧ್ರುವ
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    ತಾರ್ಕಿಕ has_var_store () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->has_var_store (); }

    ಧ್ರುವ ItemVariationStore &get_var_store () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->get_var_store (); }
    ಧ್ರುವ ItemVariationStore *get_var_store_ptr () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->get_var_store_ptr (); }

    ತಾರ್ಕಿಕ has_delta_set_index_map () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->has_delta_set_index_map (); }

    ಧ್ರುವ DeltaSetIndexMap &get_delta_set_index_map () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->get_delta_set_index_map (); }
    ಧ್ರುವ DeltaSetIndexMap *get_delta_set_index_map_ptr () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ colr->get_delta_set_index_map_ptr (); }

    ಗುಪ್ತ:

    hb_colr_scratch_t *acquire_scratch () ಧ್ರುವ
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      ಯದಿ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_colr_scratch_t));
	ಯದಿ (unlikely (!scratch))
	  ಪ್ರತ್ಯಾಗಮ nullptr;
      }

      ಪ್ರತ್ಯಾಗಮ scratch;
    }
    ಶೂನ್ಯ release_scratch (hb_colr_scratch_t *scratch) ಧ್ರುವ
    {
      ಯದಿ (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    ಸಾರ್ವಜನಿಕ:
    hb_blob_ptr_t<COLR> colr;
    ಗುಪ್ತ:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
  {
    // v0
    hb_array_t<ಧ್ರುವ BaseGlyphRecord> baseGlyphs = (ಏತತ್+baseGlyphsZ).as_array (ಅಂಕೆ_Base_Glyphs);
    ಪ್ರತಿ (ಧ್ರುವ BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers).sub_array (record.firstLayerIdx,
                                                                         record.ಅಂಕೆ_Layers);
      ಪ್ರತಿ (ಧ್ರುವ LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    ಯದಿ (!has_v1_data())
      ಪ್ರತ್ಯಾಗಮ;

    // v1
    hb_colrv1_depend_context_t c (ಏತತ್, depend_data);
    ಧ್ರುವ BaseGlyphList &baseglyph_paintrecords = ಏತತ್+baseGlyphList;

    ಪ್ರತಿ (ಧ್ರುವ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ಧ್ರುವ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  ಶೂನ್ಯ closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ಧ್ರುವ
  {
    ಧ್ರುವ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    ಯದಿ (!record) ಪ್ರತ್ಯಾಗಮ;

    auto glyph_layers = (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers).sub_array (record->firstLayerIdx,
								       record->ಅಂಕೆ_Layers);
    ಯದಿ (!glyph_layers.length) ಪ್ರತ್ಯಾಗಮ;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  ಶೂನ್ಯ closure_V0palette_indices (ಧ್ರುವ hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (!ಅಂಕೆ_Base_Glyphs || !ಅಂಕೆ_Layers) ಪ್ರತ್ಯಾಗಮ;
    hb_array_t<ಧ್ರುವ BaseGlyphRecord> baseGlyphs = (ಏತತ್+baseGlyphsZ).as_array (ಅಂಕೆ_Base_Glyphs);
    hb_array_t<ಧ್ರುವ LayerRecord> all_layers = (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers);

    ಪ್ರತಿ (ಧ್ರುವ BaseGlyphRecord record : baseGlyphs)
    {
      ಯದಿ (!glyphs->has (record.glyphId)) ಅನುವರ್ತ;
      hb_array_t<ಧ್ರುವ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.ಅಂಕೆ_Layers);
      ಪ್ರತಿ (ಧ್ರುವ LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  ಶೂನ್ಯ closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ಧ್ರುವ
  {
    ಯದಿ (version < 1) ಪ್ರತ್ಯಾಗಮ;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (ಏತತ್, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ಧ್ರುವ BaseGlyphList &baseglyph_paintrecords = ಏತತ್+baseGlyphList;

    ಪ್ರತಿ (ಧ್ರುವ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      ಯದಿ (!glyphset->has (gid)) ಅನುವರ್ತ;

      ಧ್ರುವ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ಧ್ರುವ ClipList &cliplist = ಏತತ್+clipList;
    c.glyphs = glyphset;
    ಪ್ರತಿ (ಧ್ರುವ ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    ಯದಿ (has_delta_set_index_map ())
    {
      ಧ್ರುವ DeltaSetIndexMap &var_idx_map = ಏತತ್+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      ಪ್ರತಿ (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ಧ್ರುವ LayerList& get_layerList () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+layerList); }

  ಧ್ರುವ BaseGlyphList& get_baseglyphList () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+baseGlyphList); }

  ತಾರ್ಕಿಕ has_var_store () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version >= 1 && hb_barrier () && varStore != 0; }

  ತಾರ್ಕಿಕ has_delta_set_index_map () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version >= 1 && hb_barrier () && varIdxMap != 0; }

  ತಾರ್ಕಿಕ has_clip_list () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version >= 1 && hb_barrier () && clipList != 0; }

  ಧ್ರುವ DeltaSetIndexMap &get_delta_set_index_map () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_delta_set_index_map () && hb_barrier () ? ಏತತ್+varIdxMap : Null (DeltaSetIndexMap); }
  ಧ್ರುವ DeltaSetIndexMap *get_delta_set_index_map_ptr () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_delta_set_index_map () && hb_barrier () ? &(ಏತತ್+varIdxMap) : nullptr; }

  ಧ್ರುವ ItemVariationStore &get_var_store () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_var_store () && hb_barrier () ? ಏತತ್+varStore : Null (ItemVariationStore); }
  ಧ್ರುವ ItemVariationStore *get_var_store_ptr () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_var_store () && hb_barrier () ? &(ಏತತ್+varStore) : nullptr; }

  ಧ್ರುವ ClipList &get_clip_list () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_clip_list () && hb_barrier () ? ಏತತ್+clipList : Null (ClipList); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
                  (ಏತತ್+baseGlyphsZ).sanitize (c, ಅಂಕೆ_Base_Glyphs) &&
                  (ಏತತ್+layersZ).sanitize (c, ಅಂಕೆ_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, ಏತತ್) &&
		    layerList.sanitize (c, ಏತತ್) &&
		    clipList.sanitize (c, ಏತತ್) &&
		    varIdxMap.sanitize (c, ಏತತ್) &&
		    varStore.sanitize (c, ಏತತ್))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  ತಾರ್ಕಿಕ serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (base_it.len () != layer_it.len ()))
      return_trace (ಅಸತ್ಯ);

    ಏತತ್->version = version;
    ಅಂಕೆ_Layers = 0;
    ಅಂಕೆ_Base_Glyphs = base_it.len ();
    ಯದಿ (ಅಂಕೆ_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (ಸತ್ಯ);
    }

    c->push ();
    ಪ್ರತಿ (ಧ್ರುವ hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      ಯದಿ (unlikely (!record)) return_trace (ಅಸತ್ಯ);
      record->firstLayerIdx = ಅಂಕೆ_Layers;
      ಅಂಕೆ_Layers += record->ಅಂಕೆ_Layers;
    }
    c->add_link (baseGlyphsZ, c->ತೆಗೆ_pack ());

    c->push ();
    ಪ್ರತಿ (ಧ್ರುವ hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->ತೆಗೆ_pack ());

    return_trace (ಸತ್ಯ);
  }

  ಧ್ರುವ BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ಧ್ರುವ
  {
    ಧ್ರುವ BaseGlyphRecord* record = &(ಏತತ್+baseGlyphsZ).bsearch (ಅಂಕೆ_Base_Glyphs, (unsigned ಪೂರ್ಣಾಙ್ಕ) gid);
    ಯದಿ (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    ಪ್ರತ್ಯಾಗಮ record;
  }

  ಧ್ರುವ BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ಧ್ರುವ
  {
    ಧ್ರುವ BaseGlyphList &list = ಏತತ್+baseGlyphList;
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ (!list.bfind ((unsigned) gid, &i))
      ಪ್ರತ್ಯಾಗಮ nullptr;
    ಪ್ರತ್ಯಾಗಮ &list[i];
  }

  ತಾರ್ಕಿಕ downgrade_to_V0 (ಧ್ರುವ hb_set_t &glyphset) ಧ್ರುವ
  {
    //no more COLRv1 glyphs, downgrade to version 0
    ಪ್ರತಿ (ಧ್ರುವ BaseGlyphPaintRecord& _ : get_baseglyphList ())
      ಯದಿ (glyphset.has (_.glyphId))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (ಸತ್ಯ);

    ಧ್ರುವ ItemVariationStore& var_store = ಏತತ್+varStore;
    ಯದಿ (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      ತಾರ್ಕಿಕ optimize = (varIdxMap != 0) ? ಸತ್ಯ : ಅಸತ್ಯ;
      ಯದಿ (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (ಅಸತ್ಯ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      ಯದಿ (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ಅಸತ್ಯ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      ಯದಿ (optimize)
      {
        ಧ್ರುವ hb_map_t &varidx_map = item_vars.get_varidx_map ();
        ಪ್ರತಿ (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          ಯದಿ (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          ಅನ್ಯಥಾ
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    ಅನ್ಯಥಾ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      ಯದಿ (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      ಯದಿ (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (ಸತ್ಯ);

    ಧ್ರುವ hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    ಯದಿ (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ಧ್ರುವ hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->ಅಂಕೆ_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    ಯದಿ (glyphset.has (old_gid)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
		    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ಧ್ರುವ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				ಯದಿ (unlikely (!old_record))
				  ಪ್ರತ್ಯಾಗಮ hb_pair_t<ತಾರ್ಕಿಕ, BaseGlyphRecord> (ಅಸತ್ಯ, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.ಅಂಕೆ_Layers = old_record->ಅಂಕೆ_Layers;
				ಪ್ರತ್ಯಾಗಮ hb_pair_t<ತಾರ್ಕಿಕ, BaseGlyphRecord> (ಸತ್ಯ, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->ಅಂಕೆ_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ಧ್ರುವ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				ಯದಿ (unlikely (!old_record ||
					      old_record->firstLayerIdx >= ಅಂಕೆ_Layers ||
					      old_record->firstLayerIdx + old_record->ಅಂಕೆ_Layers > ಅಂಕೆ_Layers))
				  ಪ್ರತ್ಯಾಗಮ hb_pair_t<ತಾರ್ಕಿಕ, hb_vector_t<LayerRecord>> (ಅಸತ್ಯ, out_layers);

				auto layers = (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->ಅಂಕೆ_Layers);
				out_layers.resize (layers.length);
				ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  ಯದಿ (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    ಪ್ರತ್ಯಾಗಮ hb_pair_t<ತಾರ್ಕಿಕ, hb_vector_t<LayerRecord>> (ಅಸತ್ಯ, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				ಪ್ರತ್ಯಾಗಮ hb_pair_t<ತಾರ್ಕಿಕ, hb_vector_t<LayerRecord>> (ಸತ್ಯ, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    ಯದಿ (version == 0 && (!base_it || !layer_it))
      return_trace (ಅಸತ್ಯ);

    auto *colr_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<COLR> ();
    ಯದಿ (unlikely (!c->serializer->extend_min (colr_ಅವಿಭಾಜ್ಯ)))  return_trace (ಅಸತ್ಯ);

    ಯದಿ (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_ಅವಿಭಾಜ್ಯ->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    ಯದಿ (!c->serializer->allocate_size<ಶೂನ್ಯ> (5 * HBUINT32::static_size)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!colr_ಅವಿಭಾಜ್ಯ->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (ಅಸತ್ಯ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    ಯದಿ (!subset_varstore (c, colr_ಅವಿಭಾಜ್ಯ)) return_trace (ಅಸತ್ಯ);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    ಯದಿ (!colr_ಅವಿಭಾಜ್ಯ->baseGlyphList.serialize_subset (c, baseGlyphList, ಏತತ್, instancer))
      return_trace (ಅಸತ್ಯ);

    colr_ಅವಿಭಾಜ್ಯ->layerList.serialize_subset (c, layerList, ಏತತ್, instancer);
    colr_ಅವಿಭಾಜ್ಯ->clipList.serialize_subset (c, clipList, ಏತತ್, instancer);

    return_trace (subset_delta_set_index_map (c, colr_ಅವಿಭಾಜ್ಯ));
  }

  ಧ್ರುವ Paint *get_base_glyph_paint (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಧ್ರುವ BaseGlyphList &baseglyph_paintrecords = ಏತತ್+baseGlyphList;
    ಧ್ರುವ BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    ಯದಿ (record)
    {
      ಧ್ರುವ Paint &paint = &baseglyph_paintrecords+record->paint;
      ಪ್ರತ್ಯಾಗಮ &paint;
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ nullptr;
  }

#ifndef HB_NO_PAINT
  ತಾರ್ಕಿಕ
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ಧ್ರುವ
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->ಅಂಕೆ_coords));

    ಯದಿ (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    ತಾರ್ಕಿಕ ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), ಸತ್ಯ, scratch);

    auto e = scratch.paint_extents.get_extents ();
    ಯದಿ (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    ಅನ್ಯಥಾ
      *extents = e.to_glyph_extents ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }
#endif

  ತಾರ್ಕಿಕ
  has_paint_for_glyph (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಯದಿ (version >= 1)
    {
      hb_barrier ();

      ಧ್ರುವ Paint *paint = get_base_glyph_paint (glyph);

      ಪ್ರತ್ಯಾಗಮ paint != nullptr;
    }

    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ಧ್ರುವ ItemVarStoreInstancer instancer) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  ತಾರ್ಕಿಕ
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, ಶೂನ್ಯ *data,
	       unsigned ಪೂರ್ಣಾಙ್ಕ palette_index, hb_color_t foreground,
	       ತಾರ್ಕಿಕ clip,
	       hb_colr_scratch_t &scratch) ಧ್ರುವ
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->ಅಂಕೆ_coords : 0));
    hb_paint_context_t c (ಏತತ್, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_ಗಂಟು_t node (c.glyphs_decycler);
    node.visit (glyph);

    ಯದಿ (version >= 1)
    {
      hb_barrier ();

      ಧ್ರುವ Paint *paint = get_base_glyph_paint (glyph);
      ಯದಿ (paint)
      {
        // COLRv1 glyph

	ತಾರ್ಕಿಕ is_bounded = ಸತ್ಯ;
	hb_glyph_extents_t extents = {};
	ಯದಿ (clip)
	{
	  ಯದಿ (!get_clip (glyph, &extents, instancer))
	  {
	    clip = ಅಸತ್ಯ;
	    is_bounded = ಅಸತ್ಯ;
	  }

	  ಯದಿ (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 ಅಸತ್ಯ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ತಳ್ಳು_font_transform (c.data, font);

	ಯದಿ (clip)
	  c.funcs->ತಳ್ಳು_clip_rectangle (c.data,
					(ಭಿನ್ನ) extents.x_bearing,
					(ಭಿನ್ನ) extents.y_bearing + extents.height,
					(ಭಿನ್ನ) extents.x_bearing + extents.width,
					(ಭಿನ್ನ) extents.y_bearing);

	ಯದಿ (is_bounded)
	  c.recurse (*paint);

	ಯದಿ (clip)
	  c.funcs->ತೆಗೆ_clip (c.data);

	c.funcs->ತೆಗೆ_transform (c.data);

        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }

    ಧ್ರುವ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    ಯದಿ (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      ಪ್ರತಿ (ಧ್ರುವ auto &r : (ಏತತ್+layersZ).as_array (ಅಂಕೆ_Layers)
			   .sub_array (record->firstLayerIdx, record->ಅಂಕೆ_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
#endif

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	ಅಂಕೆ_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	ಅಂಕೆ_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (14);
};

ಸಂರಚನಾ COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

ಶೂನ್ಯ
hb_paint_context_t::recurse (ಧ್ರುವ Paint &paint)
{
  ಯದಿ (unlikely (depth_ಎಡ <= 0 || edge_ಎಣಿಕೆ <= 0)) ಪ್ರತ್ಯಾಗಮ;
  depth_ಎಡ--;
  edge_ಎಣಿಕೆ--;
  paint.dispatch (ಏತತ್);
  depth_ಎಡ++;
}

template <typename Types>
ಶೂನ್ಯ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
{
  TRACE_PAINT (ಏತತ್);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  ಯದಿ (likely (c->depth_ಎಡ > 0 && c->edge_ಎಣಿಕೆ > 0) &&
      (ಏತತ್+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_ಎಣಿಕೆ--;
    c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->ತೆಗೆ_transform (c->data);
    ಪ್ರತ್ಯಾಗಮ;
  }

  c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
  c->funcs->ತಳ್ಳು_clip_glyph (c->data, gid, c->font);
  c->funcs->ತಳ್ಳು_font_transform (c->data, c->font);
  c->recurse (ಏತತ್+paint);
  c->funcs->ತೆಗೆ_transform (c->data);
  c->funcs->ತೆಗೆ_clip (c->data);
  c->funcs->ತೆಗೆ_transform (c->data);
}

ಶೂನ್ಯ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
{
  TRACE_PAINT (ಏತತ್);
  ಧ್ರುವ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_ಗಂಟು_t node (c->layers_decycler);
  ಪ್ರತಿ (unsigned i = firstLayerIndex; i < firstLayerIndex + ಅಂಕೆ_Layers; i++)
  {
    ಯದಿ (unlikely (!node.visit (i)))
      ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

ಶೂನ್ಯ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ಧ್ರುವ
{
  TRACE_PAINT (ಏತತ್);

  hb_decycler_ಗಂಟು_t node (c->glyphs_decycler);
  ಯದಿ (unlikely (!node.visit (gid)))
    ಪ್ರತ್ಯಾಗಮ;

  c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
  ಯದಿ (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->ತೆಗೆ_transform (c->data);
    ಪ್ರತ್ಯಾಗಮ;
  }
  c->funcs->ತೆಗೆ_transform (c->data);

  ಧ್ರುವ COLR *colr_table = c->get_colr_table ();
  ಧ್ರುವ Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  ತಾರ್ಕಿಕ has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  ಯದಿ (has_clip_box)
    c->funcs->ತಳ್ಳು_clip_rectangle (c->data,
				   (ಭಿನ್ನ) extents.x_bearing,
				   (ಭಿನ್ನ) extents.y_bearing + extents.height,
				   (ಭಿನ್ನ) extents.x_bearing + extents.width,
				   (ಭಿನ್ನ) extents.y_bearing);

  ಯದಿ (paint)
    c->recurse (*paint);

  ಯದಿ (has_clip_box)
    c->funcs->ತೆಗೆ_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
