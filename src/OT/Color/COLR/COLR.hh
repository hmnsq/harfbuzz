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

#ଅନ୍ତର୍ଭାଵ "../../../hb.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-decycler.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-var-common.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint-bounded.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint-extents.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-depend-data.hh"

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

ନାମାକାଶ OT {
ସଂରଚନା hb_paint_context_t;
}

ସଂରଚନା hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

ନାମାକାଶ OT {

ସଂରଚନା COLR;

ସଂରଚନା Paint;

ସଂରଚନା hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "PAINT"; }
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.paint_glyph (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ଅଚଳ COLR* get_colr_table () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ COLR *> (base); }

ସର୍ବସାଧାରଣ:
  ଅଚଳ ଶୂନ୍ୟ *base;
  hb_paint_funcs_t *funcs;
  ଶୂନ୍ୟ *data;
  hb_font_t *font;
  hb_array_t<ଅଚଳ BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth_ବାମ = HB_MAX_NESTING_LEVEL;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge_ଗଣନା = HB_MAX_GRAPH_EDGE_ଗଣନା;

  hb_paint_context_t (ଅଚଳ ଶୂନ୍ୟ *base_,
		      hb_paint_funcs_t *funcs_,
		      ଶୂନ୍ୟ *data_,
                      hb_font_t *font_,
                      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_ଗଣନା () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    ଯଦି (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, ସତ୍ୟ);
      hb_font_set_synthetic_slant (font, 0);
    }
    ଅନ୍ୟଥା
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ସ୍ଥିର unsigned
  color_alpha (hb_color_t color, ଦଶମିକ alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    ଫେରନ୍ତୁ roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index, ଦଶମିକ alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = ସତ୍ୟ;

    ଯଦି (color_index != 0xffff)
    {
      ଯଦି (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ recurse (ଅଚଳ Paint &paint);
};

ସଂରଚନା hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj)
  {
    ଯଦି (unlikely (nesting_level_ବାମ == 0))
      ଫେରନ୍ତୁ hb_empty_t ();

    ଯଦି (paint_visited (&obj))
      ଫେରନ୍ତୁ hb_empty_t ();

    nesting_level_ବାମ--;
    obj.closurev1 (ଏହି);
    nesting_level_ବାମ++;
    ଫେରନ୍ତୁ hb_empty_t ();
  }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ତାର୍କିକ paint_visited (ଅଚଳ ଶୂନ୍ୟ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    ଯଦି (visited_paint.in_error() || visited_paint.has (delta))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    visited_paint.add (delta);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଅଚଳ COLR* get_colr_table () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ COLR *> (base); }

  ଶୂନ୍ୟ add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  ଶୂନ୍ୟ add_layer_indices (unsigned first_layer_index, unsigned ଅଙ୍କ_of_layers)
  {
    ଯଦି (ଅଙ୍କ_of_layers == 0) ଫେରନ୍ତୁ;
    layer_indices->add_range (first_layer_index, first_layer_index + ଅଙ୍କ_of_layers - 1);
  }

  ଶୂନ୍ୟ add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  ଶୂନ୍ୟ add_var_idxes (unsigned first_var_idx, unsigned ଅଙ୍କ_idxes)
  {
    ଯଦି (!ଅଙ୍କ_idxes || first_var_idx == VarIdx::NO_VARIATION) ଫେରନ୍ତୁ;
    variation_indices->add_range (first_var_idx, first_var_idx + ଅଙ୍କ_idxes - 1);
  }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ ଶୂନ୍ୟ *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned ଅଙ୍କ_var_idxes;
  unsigned nesting_level_ବାମ;

  hb_colrv1_closure_context_t (ଅଚଳ ଶୂନ୍ୟ *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned ଅଙ୍କ_var_idxes_2 = 1,
                               unsigned nesting_level_ବାମ_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          ଅଙ୍କ_var_idxes (ଅଙ୍କ_var_idxes_2),
                          nesting_level_ବାମ (nesting_level_ବାମ_2)
  {}
};


ସଂରଚନା hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj)
  {
    ଯଦି (unlikely (nesting_level_ବାମ == 0))
      ଫେରନ୍ତୁ hb_empty_t ();

    ଯଦି (paint_visited (&obj))
      ଫେରନ୍ତୁ hb_empty_t ();

    nesting_level_ବାମ--;
    obj.dependv1 (ଏହି);
    nesting_level_ବାମ++;
    ଫେରନ୍ତୁ hb_empty_t ();
  }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ତାର୍କିକ paint_visited (ଅଚଳ ଶୂନ୍ୟ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    ଯଦି (visited_paint.in_error() || visited_paint.has (delta))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    visited_paint.add (delta);
    ଯଦି (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଅଚଳ COLR* get_colr_table () ଅଚଳ
  { ଫେରନ୍ତୁ colr; }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_ବାମ;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ଅଚଳ COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_ବାମ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_ବାମ (nesting_level_ବାମ_2),
                          source_gid (0)
  {}
};

ସଂରଚନା LayerRecord
{
  operator hb_ot_color_layer_t () ଅଚଳ { ଫେରନ୍ତୁ {glyphId, colorIdx}; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା BaseGlyphRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	ଅଙ୍କ_Layers;	/* Number of color layers
				 * associated with this glyph */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ସଂରଚନା Variable
{
  ସ୍ଥିର constexpr ତାର୍କିକ is_variable = ସତ୍ୟ;

  ଅଚଳ T& get_ମୂଲ୍ୟ () ଅଚଳ { ଫେରନ୍ତୁ value; }

  Variable<T>* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (ଏହି));
  }

  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ
  { value.dependv1 (c); }

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    c->ଅଙ୍କ_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->ଅଙ୍କ_var_idxes);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!value.subset (c, instancer, varIdxBase)) return_trace (ମିଥ୍ୟା);
    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ସତ୍ୟ);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    ଯଦି (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
      ଯଦି (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (ମିଥ୍ୟା);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    value.paint_glyph (c, varIdxBase);
  }

  ଶୂନ୍ୟ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  ଶୂନ୍ୟ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ଅଚଳ
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ଅଚଳ
  {
    ଫେରନ୍ତୁ value.get_extend ();
  }

  ସୁରକ୍ଷିତ:
  T      value;
  ସର୍ବସାଧାରଣ:
  VarIdx varIdxBase;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
ସଂରଚନା NoVariable
{
  ସ୍ଥିର constexpr ତାର୍କିକ is_variable = ମିଥ୍ୟା;

  ଅଚଳ T& get_ମୂଲ୍ୟ () ଅଚଳ { ଫେରନ୍ତୁ value; }

  ସ୍ଥିର constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (ଏହି));
  }

  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ
  { value.dependv1 (c); }

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  { value.closurev1 (c); }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    value.paint_glyph (c, varIdxBase);
  }

  ଶୂନ୍ୟ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  ଶୂନ୍ୟ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ଅଚଳ
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ଅଚଳ
  {
    ଫେରନ୍ତୁ value.get_extend ();
  }

  T      value;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
ସ୍ଥିର ଶୂନ୍ୟ
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, ଦଶମିକ value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

ସଂରଚନା ColorStop
{
  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t * c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    c->add_palette_index (paletteIndex);
    c->ଅଙ୍କ_var_idxes = 2;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      ଦଶମିକ a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

ସଂରଚନା Extend : HBUINT8
{
  ଗଣନା {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> ଶ୍ରେଣୀ Var>
ସଂରଚନା Color_ଧାଡ଼ି
{
  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t * c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ଯଦି (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto& stop : stops.iter ())
    {
      ଯଦି (!stop.subset (c, instancer)) return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
  get_color_stops (hb_paint_context_t *c,
                   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2,
		   hb_color_stop_t *color_stops,
		   ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = stops.len;

    ଯଦି (ଗଣନା_2 && color_stops)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
      ପାଇଁ (i = 0; i < *ଗଣନା_2 && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *ଗଣନା_2 = i;
    }

    ଫେରନ୍ତୁ len;
  }

  HB_INTERNAL ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା static_get_color_stops (hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
							  ଶୂନ୍ୟ *color_ଧାଡ଼ି_data,
							  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
							  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2,
							  hb_color_stop_t *color_stops,
							  ଶୂନ୍ୟ *user_data)
  {
    ଅଚଳ Color_ଧାଡ଼ି *thiz = (ଅଚଳ Color_ଧାଡ଼ି *) color_ଧାଡ଼ି_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    ଫେରନ୍ତୁ thiz->get_color_stops (c, start, ଗଣନା_2, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ଅଚଳ
  {
    ବାଛ ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) extend)
    {
      ସ୍ଥିତି Extend::EXTEND_REPEAT:  ଫେରନ୍ତୁ HB_PAINT_EXTEND_REPEAT;
      ସ୍ଥିତି Extend::EXTEND_REFLECT: ଫେରନ୍ତୁ HB_PAINT_EXTEND_REFLECT;
      ସ୍ଥିତି Extend::EXTEND_PAD:
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                     ଫେରନ୍ତୁ HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL ସ୍ଥିର hb_paint_extend_t static_get_extend (hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
							  ଶୂନ୍ୟ *color_ଧାଡ଼ି_data,
							  ଶୂନ୍ୟ *user_data)
  {
    ଅଚଳ Color_ଧାଡ଼ି *thiz = (ଅଚଳ Color_ଧାଡ଼ି *) color_ଧାଡ଼ି_data;
    ଫେରନ୍ତୁ thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
ସଂରଚନା CompositeMode : HBUINT8
{
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (1);
};

ସଂରଚନା Affine2x3
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  { c->ଅଙ୍କ_var_idxes = 6; }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((ଦ୍ୱିଗୁଣ) source.to_int () +
                                                    (ଦ୍ୱିଗୁଣ) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    c->funcs->ଠେଲ_transform (c->data,
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

ସଂରଚନା PaintColrLayers
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer HB_UNUSED) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    uint32_t first_layer_index = ଅଙ୍କ_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ;

  HBUINT8	format; /* format = 1 */
  HBUINT8	ଅଙ୍କ_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା PaintSolid
{
  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ {}
  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    c->add_palette_index (paletteIndex);
    c->ଅଙ୍କ_var_idxes = 1;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ଦଶମିକ a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    ଯଦି (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ଅଚଳ
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> ଶ୍ରେଣୀ Var>
ସଂରଚନା PaintLinearGradient
{
  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    (ଏହି+color_ଧାଡ଼ି_2).closurev1 (c);
    c->ଅଙ୍କ_var_idxes = 6;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) x0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) y0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) x1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) y1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) x2 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) y2 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 5)));
    }

    ଯଦି (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_ଧାଡ଼ି_2.serialize_subset (c, color_ଧାଡ଼ି_2, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && color_ଧାଡ଼ି_2.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    hb_color_ଧାଡ଼ି_t cl = {
      (ଶୂନ୍ୟ *) &(ଏହି+color_ଧାଡ଼ି_2),
      (ଏହି+color_ଧାଡ଼ି_2).static_get_color_stops, c,
      (ଏହି+color_ଧାଡ଼ି_2).static_get_extend, nullptr
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
  Offset24To<Color_ଧାଡ଼ି<Var>>	color_ଧାଡ଼ି_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> ଶ୍ରେଣୀ Var>
ସଂରଚନା PaintRadialGradient
{
  ତାର୍କିକ constant_after_pinning (hb_subset_context_t *c,
                               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    ଯଦି (c->plan->has_avar2) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ auto &regions = instancer.varStore->get_region_list ();
    ପାଇଁ (unsigned i = 0; i < regions.region_ଗଣନା; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      ଯଦି (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ପାଇଁ (auto axis : axes)
      {
        Triple *limit;
        ଯଦି (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    (ଏହି+color_ଧାଡ଼ି_2).closurev1 (c);
    c->ଅଙ୍କ_var_idxes = 6;
  }

  ତାର୍କିକ subset_nonnegative (hb_subset_context_t *c,
                           ଅଚଳ ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ source[6] = {
      x0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 0)),
      y0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)),
      radius0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)),
      x1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 3)),
      y1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 4)),
      radius1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      ଯଦି (!out) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    };
    ଅଚଳ auto &ଧାଡ଼ି = ଏହି+color_ଧାଡ଼ି_2;
    ଯଦି (source[2] == source[5] || !ଧାଡ଼ି.stops.len) ଫେରନ୍ତୁ transparent ();
    ସଂରଚନା stop_t { ଦ୍ୱିଗୁଣ offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    ଯଦି (!stops.alloc (ଧାଡ଼ି.stops.len)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned order = 0;
    ପାଇଁ (ଅଚଳ auto &entry : ଧାଡ଼ି.stops.iter ())
    {
      ଅଚଳ auto &stop = entry.get_ମୂଲ୍ୟ ();
      stops.push (stop_t {(ଦ୍ୱିଗୁଣ) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (ଦ୍ୱିଗୁଣ) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    ଯଦି (stops.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    stops.qsort ([] (ଅଚଳ stop_t &a, ଅଚଳ stop_t &b) {
      ଫେରନ୍ତୁ a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    ଦ୍ୱିଗୁଣ dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    ପାଇଁ (unsigned j = 0; j < 3; j++)
    {
      ଦ୍ୱିଗୁଣ remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      ଯଦି (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      unsigned ଡାହାଣ = (unsigned) remainder;
      ଯେତେବେଳେ (ଡାହାଣ) { unsigned r = lattice % ଡାହାଣ; lattice = ଡାହାଣ; ଡାହାଣ = r; }
    }
    ପାଇଁ (unsigned shift = 0; shift < 16; shift++)
    {
      ଦ୍ୱିଗୁଣ b = 1u << shift;
      ଦ୍ୱିଗୁଣ coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      ଦ୍ୱିଗୁଣ fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      ଦ୍ୱିଗୁଣ candidates[] = {coarse, fine};
      ପାଇଁ (ଦ୍ୱିଗୁଣ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        ଦ୍ୱିଗୁଣ geometry[6];
        ତାର୍କିକ fits = ସତ୍ୟ;
        ପାଇଁ (unsigned j = 0; j < 3; j++)
        {
          ଦ୍ୱିଗୁଣ difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          ଦ୍ୱିଗୁଣ ତଳ = j == 2 ? 0 : -32768, ଉପର = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= ତଳ && geometry[j] <= ଉପର &&
                  geometry[j + 3] >= ତଳ && geometry[j + 3] <= ଉପର;
        }
        ଯଦି (!fits) ଜାରି;
        unsigned first = 0, ଗଣନା_2 = stops.length;
        ଦ୍ୱିଗୁଣ min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        ଦ୍ୱିଗୁଣ bias = 0;
        ତାର୍କିକ constant = ମିଥ୍ୟା;
        ଯଦି (ଧାଡ଼ି.extend == Extend::EXTEND_PAD)
        {
          ଦ୍ୱିଗୁଣ tip = -geometry[2] / (b * dr);
          ଯଦି ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            ଗଣନା_2 = 1; constant = ସତ୍ୟ;
          }
        }
        ଅନ୍ୟଥା
        {
          ଦ୍ୱିଗୁଣ period = max - min;
          ଯଦି (ଧାଡ଼ି.extend == Extend::EXTEND_REFLECT) period *= 2;
          ଯଦି (period <= 0) ଫେରନ୍ତୁ transparent ();
          ଦ୍ୱିଗୁଣ n = ceil ((-2 - min) / period);
          ଯଦି (n > floor ((32767. / 16384. - max) / period)) ଜାରି;
          bias = n * period;
        }
        ପାଇଁ (unsigned j = 0; j < ଗଣନା_2; j++)
        {
          ଦ୍ୱିଗୁଣ bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        ଯଦି (!fits) ଜାରି;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        ଯଦି (!out) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        out->format = 6;
        out->x0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) geometry[0]; out->y0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) geometry[3]; out->y1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_ଧାଡ଼ି = c->serializer->allocate_size<Color_ଧାଡ଼ି<NoVariable>> (3 + ଗଣନା_2 * ColorStop::static_size);
        ଯଦି (!out_ଧାଡ଼ି) { c->serializer->କାଢ଼_discard (); ଫେରନ୍ତୁ ମିଥ୍ୟା; }
        out_ଧାଡ଼ି->extend = ଧାଡ଼ି.extend;
        out_ଧାଡ଼ି->stops.len = ଗଣନା_2;
        ପାଇଁ (unsigned j = 0; j < ଗଣନା_2; j++)
        {
          ଅଚଳ auto &stop = stops[first + j];
          auto &out_stop = out_ଧାଡ଼ି->stops[j].value;
          out_stop.stopOffset.set_float ((ଦଶମିକ) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((ଦଶମିକ) stop.alpha);
          ଯଦି (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->କାଢ଼_discard (); ଫେରନ୍ତୁ ମିଥ୍ୟା; }
        }
        c->serializer->add_link (out->color_ଧାଡ଼ି_2, c->serializer->କାଢ଼_pack ());
        ଫେରନ୍ତୁ !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) x0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) y0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((ଦ୍ୱିଗୁଣ) radius0 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) x1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) y1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((ଦ୍ୱିଗୁଣ) radius1 + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 5)));
    }

    ଯଦି (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_ଧାଡ଼ି_2.serialize_subset (c, color_ଧାଡ଼ି_2, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && color_ଧାଡ଼ି_2.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    hb_color_ଧାଡ଼ି_t cl = {
      (ଶୂନ୍ୟ *) &(ଏହି+color_ଧାଡ଼ି_2),
      (ଏହି+color_ଧାଡ଼ି_2).static_get_color_stops, c,
      (ଏହି+color_ଧାଡ଼ି_2).static_get_extend, nullptr
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
  Offset24To<Color_ଧାଡ଼ି<Var>>	color_ଧାଡ଼ି_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> ଶ୍ରେଣୀ Var>
ସଂରଚନା PaintSweepGradient
{
  ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ {}

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    (ଏହି+color_ଧାଡ଼ି_2).closurev1 (c);
    c->ଅଙ୍କ_var_idxes = 4;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerX + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerY + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    ଯଦି (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_ଧାଡ଼ି_2.serialize_subset (c, color_ଧାଡ଼ି_2, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && color_ଧାଡ଼ି_2.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    hb_color_ଧାଡ଼ି_t cl = {
      (ଶୂନ୍ୟ *) &(ଏହି+color_ଧାଡ଼ି_2),
      (ଏହି+color_ଧାଡ଼ି_2).static_get_color_stops, c,
      (ଏହି+color_ଧାଡ଼ି_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_ଧାଡ଼ି<Var>>	color_ଧାଡ଼ି_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
ସଂରଚନା PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->paint.serialize_subset (c, paint, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && paint.sanitize (c, ଏହି));
  }

  ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

ସଂରଚନା PaintColrGlyph
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer HB_UNUSED) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> ଶ୍ରେଣୀ Var>
ସଂରଚନା PaintTransform
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!out->transform.serialize_subset (c, transform, ଏହି, instancer)) return_trace (ମିଥ୍ୟା);
    ଯଦି (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  src.sanitize (c, ଏହି) &&
                  transform.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    (ଏହି+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (7);
};

ସଂରଚନା PaintTranslate
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) dx + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) dy + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
    }

    ଯଦି (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ ddx = dx + c->instancer (varIdxBase, 0);
    ଦଶମିକ ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ଠେଲ_translate (c->data, ddx, ddy);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
ସ୍ଥିର ତାର୍କିକ serialize_scale_transform (hb_subset_context_t *c,
                                       ଦଶମିକ sx, ଦଶମିକ sy, ଦଶମିକ dx, ଦଶମିକ dy,
                                       ଅଚଳ Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  ଯଦି (unlikely (!out)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  ଯଦି (unlikely (!transform))
  {
    c->serializer->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->କାଢ଼_pack ());

  c->serializer->push ();
  ତାର୍କିକ ret = source ();
  ଯଦି (ret)
    c->serializer->add_link (out->src, c->serializer->କାଢ଼_pack ());
  ଅନ୍ୟଥା
    c->serializer->କାଢ଼_discard ();
  ଫେରନ୍ତୁ ret;
}

template <typename Base>
ସ୍ଥିର ତାର୍କିକ subset_scale_as_transform (hb_subset_context_t *c,
                                      ଅଚଳ Base *base,
                                      ଅଚଳ Offset24To<Paint> &src,
                                      ଅଚଳ ItemVarStoreInstancer &instancer,
                                      ଦଶମିକ sx, ଦଶମିକ sy,
                                      ଦଶମିକ cx = 0, ଦଶମିକ cy = 0)
{
  ଯଦି (src.is_null ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  auto source = [&] { ଫେରନ୍ତୁ c->dispatch (base+src, instancer); };
  ଦଶମିକ dx = cx * (1 - sx), dy = cy * (1 - sy);
  ଯଦି (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    ଫେରନ୍ତୁ serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    ଫେରନ୍ତୁ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    ଫେରନ୍ତୁ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    ଫେରନ୍ତୁ serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    ଫେରନ୍ତୁ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  ଫେରନ୍ତୁ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

ସଂରଚନା PaintScale
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ଦଶମିକ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ଦଶମିକ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      ଯଦି (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, ଏହି, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    ଯଦି (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ଦଶମିକ sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ଠେଲ_scale (c->data, sx, sy);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ସଂରଚନା PaintScaleAroundCenter
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ଦଶମିକ sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ଦଶମିକ sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      ଯଦି (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, ଏହି, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerX + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerY + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 3)));
    }

    ଯଦି (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ଦଶମିକ sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    ଦଶମିକ tCenterX = centerX + c->instancer (varIdxBase, 2);
    ଦଶମିକ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ଠେଲ_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ସଂରଚନା PaintScaleUniform
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ଦଶମିକ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ଯଦି (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, ଏହି, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    ଯଦି (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ଠେଲ_scale (c->data, s, s);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ସଂରଚନା PaintScaleUniformAroundCenter
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ଦଶମିକ s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ଯଦି (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, ଏହି, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerX + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerY + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
    }

    ଯଦି (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ s = scale.to_float (c->instancer (varIdxBase, 0));
    ଦଶମିକ tCenterX = centerX + c->instancer (varIdxBase, 1);
    ଦଶମିକ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ଠେଲ_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ସଂରଚନା PaintRotate
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    ଯଦି (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ଠେଲ_rotate (c->data, a);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

ସଂରଚନା PaintRotateAroundCenter
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerX + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerY + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
    }

    ଯଦି (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ a = angle.to_float (c->instancer (varIdxBase, 0));
    ଦଶମିକ tCenterX = centerX + c->instancer (varIdxBase, 1);
    ଦଶମିକ tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ଠେଲ_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

ସଂରଚନା PaintSkew
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    ଯଦି (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ଦଶମିକ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ଠେଲ_skew (c->data, sx, sy);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

ସଂରଚନା PaintSkewAroundCenter
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerX + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) centerY + (ଦ୍ୱିଗୁଣ) roundf (instancer (varIdxBase, 3)));
    }

    ଯଦି (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, ଏହି, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && src.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    ଦଶମିକ sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ଦଶମିକ sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    ଦଶମିକ tCenterX = centerX + c->instancer (varIdxBase, 2);
    ଦଶମିକ tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ଠେଲ_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

ସଂରଚନା PaintComposite
{
  HB_INTERNAL ଶୂନ୍ୟ dependv1 (hb_colrv1_depend_context_t* c) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ;

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ret |= out->src.serialize_subset (c, src, ଏହି, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, ଏହି, instancer);
    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  c->check_ops (ଏହି->min_size) && // PainComposite can get exponential
                  src.sanitize (c, ଏହି) &&
                  backdrop.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ paint_glyph (hb_paint_context_t *c) ଅଚଳ
  {
    TRACE_PAINT (ଏହି);
    hb_paint_composite_mode_t composite_mode;
    ବାଛ ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode)
    {
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_CLEAR:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_IN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_XOR:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_PLUS:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SCREEN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_OVERLAY:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DARKEN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode;
	ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	ଭାଙ୍ଗ;
    }

    c->funcs->ଠେଲ_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (ଏହି+backdrop);
    c->funcs->ଠେଲ_group_for (c->data, composite_mode);
    c->recurse (ଏହି+src);
    c->funcs->କାଢ଼_group (c->data, composite_mode);
    c->funcs->କାଢ଼_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା ClipBoxData
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା xMin, yMin, xMax, yMax;
};

ସଂରଚନା ClipBoxFormat1
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_clip_box (ClipBoxData &clip_box, ଅଚଳ ItemVarStoreInstancer &instancer HB_UNUSED) ଅଚଳ
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) xMin + (ଦ୍ୱିଗୁଣ) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) yMin + (ଦ୍ୱିଗୁଣ) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) xMax + (ଦ୍ୱିଗୁଣ) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((ଦ୍ୱିଗୁଣ) yMax + (ଦ୍ୱିଗୁଣ) ceilf (instancer (varIdxBase, 3)));
    }

    ଯଦି (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

ସଂରଚନା ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  ଶୂନ୍ୟ get_clip_box (ClipBoxData &clip_box, ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    value.get_clip_box(clip_box, instancer);
    ଯଦି (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

ସଂରଚନା ClipBox
{
  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 2: hb_barrier (); u.format2.closurev1 (c); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ get_extents (hb_glyph_extents_t *extents,
                    ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    ClipBoxData clip_box;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

ସଂରଚନା ClipRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  ଶୂନ୍ୟ closurev1 (hb_colrv1_closure_context_t* c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (!c->glyphs->intersects (startGlyphID, endGlyphID)) ଫେରନ୍ତୁ;
    (base+clipBox).closurev1 (c);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ଶୂନ୍ୟ *base,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && clipBox.sanitize (c, base));
  }

  ତାର୍କିକ get_extents (hb_glyph_extents_t *extents,
		    ଅଚଳ ଶୂନ୍ୟ *base,
		    ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    ଫେରନ୍ତୁ (base+clipBox).get_extents (extents, instancer);
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

ସଂରଚନା ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ଅଚଳ ItemVarStoreInstancer &instancer,
                                   ଅଚଳ hb_set_t& gids,
                                   ଅଚଳ hb_map_t& gid_offset_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned ଗଣନା_2  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    ପାଇଁ (ଅଚଳ hb_codepoint_t _ : gids.iter ())
    {
      ଯଦି (_ == start_gid) ଜାରି;

      offset = gid_offset_map.get (_);
      ଯଦି (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        ଜାରି;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      ଯଦି (!record.subset (c, ଏହି, instancer)) return_trace (0);
      ଗଣନା_2++;

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
      ଯଦି (!record.subset (c, ଏହି, instancer)) return_trace (0);
      ଗଣନା_2++;
    }
    return_trace (ଗଣନା_2);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_set_t& glyphset = c->plan->_glyphset_colred;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    ପାଇଁ (ଅଚଳ ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      ପାଇଁ (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        ଯଦି (!glyphset.has (gid) || !glyph_map.has (gid)) ଜାରି;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned ଗଣନା_2 = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    ଯଦି (!ଗଣନା_2) return_trace (ମିଥ୍ୟା);
    return_trace (c->serializer->check_assign (out->clips.len, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (ଏହି) && clips.sanitize (c, ଏହି));
  }

  ତାର୍କିକ
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    auto *rec = clips.as_array ().bsearch (gid);
    ଯଦି (rec)
      ଫେରନ୍ତୁ rec->get_extents (extents, ଏହି, instancer);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

ସଂରଚନା Paint
{

  template <typename ...Ts>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ମୂଲ୍ୟ ());

    return_trace (c->end_recursion (ଏହି->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); ଫେରନ୍ତୁ ସତ୍ୟ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT8 v; }				format;
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2);
};

ସଂରଚନା BaseGlyphPaintRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ତାର୍କିକ serialize (hb_serialize_context_t *s, ଅଚଳ hb_map_t* glyph_map,
                  ଅଚଳ ଶୂନ୍ୟ* src_base, hb_subset_context_t *c,
                  ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = s->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) && paint.sanitize (c, base)));
  }

  ସର୍ବସାଧାରଣ:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out)))  return_trace (ମିଥ୍ୟା);
    ଅଚଳ hb_set_t* glyphset = &c->plan->_glyphset_colred;

    ପାଇଁ (ଅଚଳ auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      ଯଦି (!glyphset->has (gid)) ଜାରି;

      ଯଦି (_.serialize (c->serializer, c->plan->glyph_map, ଏହି, c, instancer)) out->len++;
      ଅନ୍ୟଥା return_trace (ମିଥ୍ୟା);
    }

    return_trace (out->len != 0);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, ଏହି));
  }
};

ସଂରଚନା LayerList : Array32OfOffset32To<Paint>
{
  ଅଚଳ Paint& get_paint (unsigned i) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+(*ଏହି)[i]; }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ItemVarStoreInstancer &instancer) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out)))  return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (*ଏହି)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!o)) return_trace (ମିଥ୍ୟା);
      ret |= o->serialize_subset (c, _.second, ଏହି, instancer);
    }
    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, ଏହି));
  }
};

ସଂରଚନା delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ inner_bit_ଗଣନା; }
  unsigned get_width ()           ଅଚଳ { ଫେରନ୍ତୁ ((outer_bit_ଗଣନା + inner_bit_ଗଣନା + 7) / 8); }
  hb_array_t<ଅଚଳ uint32_t> get_output_map () ଅଚଳ { ଫେରନ୍ତୁ output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ଅଚଳ hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ଗଣନା = 0;
    outer_bit_ଗଣନା = 0;
    inner_bit_ଗଣନା = 1;
    output_map.init ();

    /* search backwards */
    unsigned ଗଣନା_2 = new_deltaset_idx_varidx_map.get_population ();
    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    ପାଇଁ (unsigned i = ଗଣନା_2; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      ଯଦି (i == ଗଣନା_2)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        ଜାରି;
      }
      ଯଦି (var_idx != last_varidx)
        ଭାଙ୍ଗ;
      last_idx = delta_set_idx;
    }

    map_ଗଣନା = last_idx + 1;
  }

  ତାର୍କିକ remap (ଅଚଳ hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_ଗଣନା = 1;
    inner_bit_ଗଣନା = 1;

    ଯଦି (unlikely (!output_map.resize_dirty (map_ଗଣନା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned idx = 0; idx < map_ଗଣନା; idx++)
    {
      uint32_t *var_idx;
      ଯଦି (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_ଗଣନା = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ଗଣନା = hb_max (bit_ଗଣନା, outer_bit_ଗଣନା);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_ଗଣନା = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ଗଣନା = hb_max (bit_ଗଣନା, inner_bit_ଗଣନା);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:
  unsigned map_ଗଣନା;
  unsigned outer_bit_ଗଣନା;
  unsigned inner_bit_ଗଣନା;
  hb_vector_t<uint32_t> output_map;
};

ସଂରଚନା COLR
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ has_v0_data () || version; }

  ତାର୍କିକ has_v0_data () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Base_Glyphs; }
  ତାର୍କିକ has_v1_data () ଅଚଳ
  {
    ଯଦି (version < 1)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_barrier ();

    ଫେରନ୍ତୁ (ଏହି+baseGlyphList).len > 0;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା         start_offset,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        *ଗଣନା_2, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ଅଚଳ
  {
    ଅଚଳ BaseGlyphRecord &record = (ଏହି+baseGlyphsZ).bsearch (ଅଙ୍କ_Base_Glyphs, glyph);

    hb_array_t<ଅଚଳ LayerRecord> all_layers = (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers);
    hb_array_t<ଅଚଳ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.ଅଙ୍କ_Layers);
    ଯଦି (ଗଣନା_2 && layers)
    {
      + glyph_layers.sub_array (start_offset, ଗଣନା_2)
      | hb_sink (hb_array (layers, *ଗଣନା_2))
      ;
    }
    ଫେରନ୍ତୁ glyph_layers.length;
  }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      ଯଦି (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ colr->has_data (); }

#ifndef HB_NO_PAINT
    ତାର୍କିକ
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ଅଚଳ
    {
      ଯଦି (unlikely (!has_data ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ତାର୍କିକ ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      ଫେରନ୍ତୁ ret;
    }

    ତାର୍କିକ paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, ଶୂନ୍ୟ *data,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index,
		      hb_color_t foreground,
		      ତାର୍କିକ clip = ସତ୍ୟ) ଅଚଳ
    {
      ଯଦି (unlikely (!has_data ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ଯଦି (unlikely (!scratch)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ତାର୍କିକ ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      ଫେରନ୍ତୁ ret;
    }
#endif

    ତାର୍କିକ is_valid () { ଫେରନ୍ତୁ colr.get_blob ()->length; }

    ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
    {
      ଯଦି (!has_data ()) ଫେରନ୍ତୁ;
      colr->depend (depend_data);
    }

    ଶୂନ୍ୟ closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ଅଚଳ
    { colr->closure_glyphs (glyph, related_ids); }

    ଶୂନ୍ୟ closure_V0palette_indices (ଅଚଳ hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ଅଚଳ
    { colr->closure_V0palette_indices (glyphs, palettes); }

    ଶୂନ୍ୟ closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ଅଚଳ
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    ତାର୍କିକ has_var_store () ଅଚଳ
    { ଫେରନ୍ତୁ colr->has_var_store (); }

    ଅଚଳ ItemVariationStore &get_var_store () ଅଚଳ
    { ଫେରନ୍ତୁ colr->get_var_store (); }
    ଅଚଳ ItemVariationStore *get_var_store_ptr () ଅଚଳ
    { ଫେରନ୍ତୁ colr->get_var_store_ptr (); }

    ତାର୍କିକ has_delta_set_index_map () ଅଚଳ
    { ଫେରନ୍ତୁ colr->has_delta_set_index_map (); }

    ଅଚଳ DeltaSetIndexMap &get_delta_set_index_map () ଅଚଳ
    { ଫେରନ୍ତୁ colr->get_delta_set_index_map (); }
    ଅଚଳ DeltaSetIndexMap *get_delta_set_index_map_ptr () ଅଚଳ
    { ଫେରନ୍ତୁ colr->get_delta_set_index_map_ptr (); }

    ବ୍ୟକ୍ତିଗତ:

    hb_colr_scratch_t *acquire_scratch () ଅଚଳ
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      ଯଦି (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, ଆକାର (hb_colr_scratch_t));
	ଯଦି (unlikely (!scratch))
	  ଫେରନ୍ତୁ nullptr;
      }

      ଫେରନ୍ତୁ scratch;
    }
    ଶୂନ୍ୟ release_scratch (hb_colr_scratch_t *scratch) ଅଚଳ
    {
      ଯଦି (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    ସର୍ବସାଧାରଣ:
    hb_blob_ptr_t<COLR> colr;
    ବ୍ୟକ୍ତିଗତ:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
  {
    // v0
    hb_array_t<ଅଚଳ BaseGlyphRecord> baseGlyphs = (ଏହି+baseGlyphsZ).as_array (ଅଙ୍କ_Base_Glyphs);
    ପାଇଁ (ଅଚଳ BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers).sub_array (record.firstLayerIdx,
                                                                         record.ଅଙ୍କ_Layers);
      ପାଇଁ (ଅଚଳ LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    ଯଦି (!has_v1_data())
      ଫେରନ୍ତୁ;

    // v1
    hb_colrv1_depend_context_t c (ଏହି, depend_data);
    ଅଚଳ BaseGlyphList &baseglyph_paintrecords = ଏହି+baseGlyphList;

    ପାଇଁ (ଅଚଳ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ଅଚଳ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  ଶୂନ୍ୟ closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ଅଚଳ
  {
    ଅଚଳ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    ଯଦି (!record) ଫେରନ୍ତୁ;

    auto glyph_layers = (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers).sub_array (record->firstLayerIdx,
								       record->ଅଙ୍କ_Layers);
    ଯଦି (!glyph_layers.length) ଫେରନ୍ତୁ;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  ଶୂନ୍ୟ closure_V0palette_indices (ଅଚଳ hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ଅଚଳ
  {
    ଯଦି (!ଅଙ୍କ_Base_Glyphs || !ଅଙ୍କ_Layers) ଫେରନ୍ତୁ;
    hb_array_t<ଅଚଳ BaseGlyphRecord> baseGlyphs = (ଏହି+baseGlyphsZ).as_array (ଅଙ୍କ_Base_Glyphs);
    hb_array_t<ଅଚଳ LayerRecord> all_layers = (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers);

    ପାଇଁ (ଅଚଳ BaseGlyphRecord record : baseGlyphs)
    {
      ଯଦି (!glyphs->has (record.glyphId)) ଜାରି;
      hb_array_t<ଅଚଳ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.ଅଙ୍କ_Layers);
      ପାଇଁ (ଅଚଳ LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  ଶୂନ୍ୟ closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ଅଚଳ
  {
    ଯଦି (version < 1) ଫେରନ୍ତୁ;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (ଏହି, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ଅଚଳ BaseGlyphList &baseglyph_paintrecords = ଏହି+baseGlyphList;

    ପାଇଁ (ଅଚଳ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      ଯଦି (!glyphset->has (gid)) ଜାରି;

      ଅଚଳ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ଅଚଳ ClipList &cliplist = ଏହି+clipList;
    c.glyphs = glyphset;
    ପାଇଁ (ଅଚଳ ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    ଯଦି (has_delta_set_index_map ())
    {
      ଅଚଳ DeltaSetIndexMap &var_idx_map = ଏହି+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      ପାଇଁ (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ଅଚଳ LayerList& get_layerList () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+layerList); }

  ଅଚଳ BaseGlyphList& get_baseglyphList () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+baseGlyphList); }

  ତାର୍କିକ has_var_store () ଅଚଳ
  { ଫେରନ୍ତୁ version >= 1 && hb_barrier () && varStore != 0; }

  ତାର୍କିକ has_delta_set_index_map () ଅଚଳ
  { ଫେରନ୍ତୁ version >= 1 && hb_barrier () && varIdxMap != 0; }

  ତାର୍କିକ has_clip_list () ଅଚଳ
  { ଫେରନ୍ତୁ version >= 1 && hb_barrier () && clipList != 0; }

  ଅଚଳ DeltaSetIndexMap &get_delta_set_index_map () ଅଚଳ
  { ଫେରନ୍ତୁ has_delta_set_index_map () && hb_barrier () ? ଏହି+varIdxMap : Null (DeltaSetIndexMap); }
  ଅଚଳ DeltaSetIndexMap *get_delta_set_index_map_ptr () ଅଚଳ
  { ଫେରନ୍ତୁ has_delta_set_index_map () && hb_barrier () ? &(ଏହି+varIdxMap) : nullptr; }

  ଅଚଳ ItemVariationStore &get_var_store () ଅଚଳ
  { ଫେରନ୍ତୁ has_var_store () && hb_barrier () ? ଏହି+varStore : Null (ItemVariationStore); }
  ଅଚଳ ItemVariationStore *get_var_store_ptr () ଅଚଳ
  { ଫେରନ୍ତୁ has_var_store () && hb_barrier () ? &(ଏହି+varStore) : nullptr; }

  ଅଚଳ ClipList &get_clip_list () ଅଚଳ
  { ଫେରନ୍ତୁ has_clip_list () && hb_barrier () ? ଏହି+clipList : Null (ClipList); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
                  (ଏହି+baseGlyphsZ).sanitize (c, ଅଙ୍କ_Base_Glyphs) &&
                  (ଏହି+layersZ).sanitize (c, ଅଙ୍କ_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, ଏହି) &&
		    layerList.sanitize (c, ଏହି) &&
		    clipList.sanitize (c, ଏହି) &&
		    varIdxMap.sanitize (c, ଏହି) &&
		    varStore.sanitize (c, ଏହି))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  ତାର୍କିକ serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (base_it.len () != layer_it.len ()))
      return_trace (ମିଥ୍ୟା);

    ଏହି->version = version;
    ଅଙ୍କ_Layers = 0;
    ଅଙ୍କ_Base_Glyphs = base_it.len ();
    ଯଦି (ଅଙ୍କ_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (ସତ୍ୟ);
    }

    c->push ();
    ପାଇଁ (ଅଚଳ hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      ଯଦି (unlikely (!record)) return_trace (ମିଥ୍ୟା);
      record->firstLayerIdx = ଅଙ୍କ_Layers;
      ଅଙ୍କ_Layers += record->ଅଙ୍କ_Layers;
    }
    c->add_link (baseGlyphsZ, c->କାଢ଼_pack ());

    c->push ();
    ପାଇଁ (ଅଚଳ hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->କାଢ଼_pack ());

    return_trace (ସତ୍ୟ);
  }

  ଅଚଳ BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ଅଚଳ
  {
    ଅଚଳ BaseGlyphRecord* record = &(ଏହି+baseGlyphsZ).bsearch (ଅଙ୍କ_Base_Glyphs, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) gid);
    ଯଦି (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    ଫେରନ୍ତୁ record;
  }

  ଅଚଳ BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ଅଚଳ
  {
    ଅଚଳ BaseGlyphList &list = ଏହି+baseGlyphList;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (!list.bfind ((unsigned) gid, &i))
      ଫେରନ୍ତୁ nullptr;
    ଫେରନ୍ତୁ &list[i];
  }

  ତାର୍କିକ downgrade_to_V0 (ଅଚଳ hb_set_t &glyphset) ଅଚଳ
  {
    //no more COLRv1 glyphs, downgrade to version 0
    ପାଇଁ (ଅଚଳ BaseGlyphPaintRecord& _ : get_baseglyphList ())
      ଯଦି (glyphset.has (_.glyphId))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (ସତ୍ୟ);

    ଅଚଳ ItemVariationStore& var_store = ଏହି+varStore;
    ଯଦି (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      ତାର୍କିକ optimize = (varIdxMap != 0) ? ସତ୍ୟ : ମିଥ୍ୟା;
      ଯଦି (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (ମିଥ୍ୟା);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      ଯଦି (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ମିଥ୍ୟା);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      ଯଦି (optimize)
      {
        ଅଚଳ hb_map_t &varidx_map = item_vars.get_varidx_map ();
        ପାଇଁ (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          ଯଦି (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          ଅନ୍ୟଥା
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    ଅନ୍ୟଥା
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      ଯଦି (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      ଯଦି (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (ସତ୍ୟ);

    ଅଚଳ hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    ଯଦି (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ଅଚଳ hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->ଅଙ୍କ_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    ଯଦି (glyphset.has (old_gid)) ଫେରନ୍ତୁ ସତ୍ୟ;
		    ଫେରନ୍ତୁ ମିଥ୍ୟା;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ଅଚଳ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				ଯଦି (unlikely (!old_record))
				  ଫେରନ୍ତୁ hb_pair_t<ତାର୍କିକ, BaseGlyphRecord> (ମିଥ୍ୟା, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.ଅଙ୍କ_Layers = old_record->ଅଙ୍କ_Layers;
				ଫେରନ୍ତୁ hb_pair_t<ତାର୍କିକ, BaseGlyphRecord> (ସତ୍ୟ, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->ଅଙ୍କ_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ଅଚଳ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				ଯଦି (unlikely (!old_record ||
					      old_record->firstLayerIdx >= ଅଙ୍କ_Layers ||
					      old_record->firstLayerIdx + old_record->ଅଙ୍କ_Layers > ଅଙ୍କ_Layers))
				  ଫେରନ୍ତୁ hb_pair_t<ତାର୍କିକ, hb_vector_t<LayerRecord>> (ମିଥ୍ୟା, out_layers);

				auto layers = (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->ଅଙ୍କ_Layers);
				out_layers.resize (layers.length);
				ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  ଯଦି (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    ଫେରନ୍ତୁ hb_pair_t<ତାର୍କିକ, hb_vector_t<LayerRecord>> (ମିଥ୍ୟା, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				ଫେରନ୍ତୁ hb_pair_t<ତାର୍କିକ, hb_vector_t<LayerRecord>> (ସତ୍ୟ, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    ଯଦି (version == 0 && (!base_it || !layer_it))
      return_trace (ମିଥ୍ୟା);

    auto *colr_ମୌଳିକ = c->serializer->start_embed<COLR> ();
    ଯଦି (unlikely (!c->serializer->extend_min (colr_ମୌଳିକ)))  return_trace (ମିଥ୍ୟା);

    ଯଦି (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_ମୌଳିକ->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    ଯଦି (!c->serializer->allocate_size<ଶୂନ୍ୟ> (5 * HBUINT32::static_size)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!colr_ମୌଳିକ->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (ମିଥ୍ୟା);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    ଯଦି (!subset_varstore (c, colr_ମୌଳିକ)) return_trace (ମିଥ୍ୟା);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    ଯଦି (!colr_ମୌଳିକ->baseGlyphList.serialize_subset (c, baseGlyphList, ଏହି, instancer))
      return_trace (ମିଥ୍ୟା);

    colr_ମୌଳିକ->layerList.serialize_subset (c, layerList, ଏହି, instancer);
    colr_ମୌଳିକ->clipList.serialize_subset (c, clipList, ଏହି, instancer);

    return_trace (subset_delta_set_index_map (c, colr_ମୌଳିକ));
  }

  ଅଚଳ Paint *get_base_glyph_paint (hb_codepoint_t glyph) ଅଚଳ
  {
    ଅଚଳ BaseGlyphList &baseglyph_paintrecords = ଏହି+baseGlyphList;
    ଅଚଳ BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    ଯଦି (record)
    {
      ଅଚଳ Paint &paint = &baseglyph_paintrecords+record->paint;
      ଫେରନ୍ତୁ &paint;
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ nullptr;
  }

#ifndef HB_NO_PAINT
  ତାର୍କିକ
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ଅଚଳ
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->ଅଙ୍କ_coords));

    ଯଦି (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    ତାର୍କିକ ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), ସତ୍ୟ, scratch);

    auto e = scratch.paint_extents.get_extents ();
    ଯଦି (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    ଅନ୍ୟଥା
      *extents = e.to_glyph_extents ();

    ଫେରନ୍ତୁ ret;
  }
#endif

  ତାର୍କିକ
  has_paint_for_glyph (hb_codepoint_t glyph) ଅଚଳ
  {
    ଯଦି (version >= 1)
    {
      hb_barrier ();

      ଅଚଳ Paint *paint = get_base_glyph_paint (glyph);

      ଫେରନ୍ତୁ paint != nullptr;
    }

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ଅଚଳ ItemVarStoreInstancer instancer) ଅଚଳ
  {
    ଫେରନ୍ତୁ get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  ତାର୍କିକ
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, ଶୂନ୍ୟ *data,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index, hb_color_t foreground,
	       ତାର୍କିକ clip,
	       hb_colr_scratch_t &scratch) ଅଚଳ
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0));
    hb_paint_context_t c (ଏହି, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_ଗଣ୍ଠି_t node (c.glyphs_decycler);
    node.visit (glyph);

    ଯଦି (version >= 1)
    {
      hb_barrier ();

      ଅଚଳ Paint *paint = get_base_glyph_paint (glyph);
      ଯଦି (paint)
      {
        // COLRv1 glyph

	ତାର୍କିକ is_bounded = ସତ୍ୟ;
	hb_glyph_extents_t extents = {};
	ଯଦି (clip)
	{
	  ଯଦି (!get_clip (glyph, &extents, instancer))
	  {
	    clip = ମିଥ୍ୟା;
	    is_bounded = ମିଥ୍ୟା;
	  }

	  ଯଦି (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 ମିଥ୍ୟା,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ଠେଲ_font_transform (c.data, font);

	ଯଦି (clip)
	  c.funcs->ଠେଲ_clip_rectangle (c.data,
					(ଦଶମିକ) extents.x_bearing,
					(ଦଶମିକ) extents.y_bearing + extents.height,
					(ଦଶମିକ) extents.x_bearing + extents.width,
					(ଦଶମିକ) extents.y_bearing);

	ଯଦି (is_bounded)
	  c.recurse (*paint);

	ଯଦି (clip)
	  c.funcs->କାଢ଼_clip (c.data);

	c.funcs->କାଢ଼_transform (c.data);

        ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }

    ଅଚଳ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    ଯଦି (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      ପାଇଁ (ଅଚଳ auto &r : (ଏହି+layersZ).as_array (ଅଙ୍କ_Layers)
			   .sub_array (record->firstLayerIdx, record->ଅଙ୍କ_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
#endif

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	ଅଙ୍କ_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	ଅଙ୍କ_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (14);
};

ସଂରଚନା COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

ଶୂନ୍ୟ
hb_paint_context_t::recurse (ଅଚଳ Paint &paint)
{
  ଯଦି (unlikely (depth_ବାମ <= 0 || edge_ଗଣନା <= 0)) ଫେରନ୍ତୁ;
  depth_ବାମ--;
  edge_ଗଣନା--;
  paint.dispatch (ଏହି);
  depth_ବାମ++;
}

template <typename Types>
ଶୂନ୍ୟ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ଅଚଳ
{
  TRACE_PAINT (ଏହି);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  ଯଦି (likely (c->depth_ବାମ > 0 && c->edge_ଗଣନା > 0) &&
      (ଏହି+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_ଗଣନା--;
    c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->କାଢ଼_transform (c->data);
    ଫେରନ୍ତୁ;
  }

  c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
  c->funcs->ଠେଲ_clip_glyph (c->data, gid, c->font);
  c->funcs->ଠେଲ_font_transform (c->data, c->font);
  c->recurse (ଏହି+paint);
  c->funcs->କାଢ଼_transform (c->data);
  c->funcs->କାଢ଼_clip (c->data);
  c->funcs->କାଢ଼_transform (c->data);
}

ଶୂନ୍ୟ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ଅଚଳ
{
  TRACE_PAINT (ଏହି);
  ଅଚଳ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_ଗଣ୍ଠି_t node (c->layers_decycler);
  ପାଇଁ (unsigned i = firstLayerIndex; i < firstLayerIndex + ଅଙ୍କ_Layers; i++)
  {
    ଯଦି (unlikely (!node.visit (i)))
      ଫେରନ୍ତୁ;

    ଅଚଳ Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

ଶୂନ୍ୟ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ଅଚଳ
{
  TRACE_PAINT (ଏହି);

  hb_decycler_ଗଣ୍ଠି_t node (c->glyphs_decycler);
  ଯଦି (unlikely (!node.visit (gid)))
    ଫେରନ୍ତୁ;

  c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
  ଯଦି (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->କାଢ଼_transform (c->data);
    ଫେରନ୍ତୁ;
  }
  c->funcs->କାଢ଼_transform (c->data);

  ଅଚଳ COLR *colr_table = c->get_colr_table ();
  ଅଚଳ Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  ତାର୍କିକ has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  ଯଦି (has_clip_box)
    c->funcs->ଠେଲ_clip_rectangle (c->data,
				   (ଦଶମିକ) extents.x_bearing,
				   (ଦଶମିକ) extents.y_bearing + extents.height,
				   (ଦଶମିକ) extents.x_bearing + extents.width,
				   (ଦଶମିକ) extents.y_bearing);

  ଯଦି (paint)
    c->recurse (*paint);

  ଯଦି (has_clip_box)
    c->funcs->କାଢ଼_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
