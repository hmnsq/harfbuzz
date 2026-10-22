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

#অন্তর্ভাৱ "../../../hb.hh"
#অন্তর্ভাৱ "../../../hb-decycler.hh"
#অন্তর্ভাৱ "../../../hb-open-type.hh"
#অন্তর্ভাৱ "../../../hb-ot-var-common.hh"
#অন্তর্ভাৱ "../../../hb-paint.hh"
#অন্তর্ভাৱ "../../../hb-paint-bounded.hh"
#অন্তর্ভাৱ "../../../hb-paint-extents.hh"
#অন্তর্ভাৱ "../../../hb-depend-data.hh"

#অন্তর্ভাৱ "../../../hb-ot-dual.hh"
#অন্তর্ভাৱ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

নামাকাশ OT {
সংরচনা hb_paint_context_t;
}

সংরচনা hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

নামাকাশ OT {

সংরচনা COLR;

সংরচনা Paint;

সংরচনা hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ধ্রুৱ ৱর্ণ *get_name () { ঘূৰাই "PAINT"; }
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.paint_glyph (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  ধ্রুৱ COLR* get_colr_table () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ COLR *> (base); }

সার্ৱজনিক:
  ধ্রুৱ শূন্য *base;
  hb_paint_funcs_t *funcs;
  শূন্য *data;
  hb_font_t *font;
  hb_array_t<ধ্রুৱ BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  পূর্ণাঙ্ক depth_বাম = HB_MAX_NESTING_LEVEL;
  পূর্ণাঙ্ক edge_গুনতি = HB_MAX_GRAPH_EDGE_গুনতি;

  hb_paint_context_t (ধ্রুৱ শূন্য *base_,
		      hb_paint_funcs_t *funcs_,
		      শূন্য *data_,
                      hb_font_t *font_,
                      unsigned পূর্ণাঙ্ক palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_গুনতি () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    যদি (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, সঁচা);
      hb_font_set_synthetic_slant (font, 0);
    }
    অন্যথা
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  স্থির unsigned
  color_alpha (hb_color_t color, ভিন্ন alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    ঘূৰাই roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned পূর্ণাঙ্ক color_index, ভিন্ন alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = সঁচা;

    যদি (color_index != 0xffff)
    {
      যদি (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = মিছা;
    }

    ঘূৰাই HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  অন্তঃস্থ শূন্য recurse (ধ্রুৱ Paint &paint);
};

সংরচনা hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj)
  {
    যদি (unlikely (nesting_level_বাম == 0))
      ঘূৰাই hb_empty_t ();

    যদি (paint_visited (&obj))
      ঘূৰাই hb_empty_t ();

    nesting_level_বাম--;
    obj.closurev1 (এতত্);
    nesting_level_বাম++;
    ঘূৰাই hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  তার্কিক paint_visited (ধ্রুৱ শূন্য *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    যদি (visited_paint.in_error() || visited_paint.has (delta))
      ঘূৰাই সঁচা;

    visited_paint.add (delta);
    ঘূৰাই মিছা;
  }

  ধ্রুৱ COLR* get_colr_table () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ COLR *> (base); }

  শূন্য add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  শূন্য add_layer_indices (unsigned first_layer_index, unsigned অঙ্ক_of_layers)
  {
    যদি (অঙ্ক_of_layers == 0) ঘূৰাই;
    layer_indices->add_range (first_layer_index, first_layer_index + অঙ্ক_of_layers - 1);
  }

  শূন্য add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  শূন্য add_var_idxes (unsigned first_var_idx, unsigned অঙ্ক_idxes)
  {
    যদি (!অঙ্ক_idxes || first_var_idx == VarIdx::NO_VARIATION) ঘূৰাই;
    variation_indices->add_range (first_var_idx, first_var_idx + অঙ্ক_idxes - 1);
  }

  সার্ৱজনিক:
  ধ্রুৱ শূন্য *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned অঙ্ক_var_idxes;
  unsigned nesting_level_বাম;

  hb_colrv1_closure_context_t (ধ্রুৱ শূন্য *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned অঙ্ক_var_idxes_2 = 1,
                               unsigned nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          অঙ্ক_var_idxes (অঙ্ক_var_idxes_2),
                          nesting_level_বাম (nesting_level_বাম_2)
  {}
};


সংরচনা hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj)
  {
    যদি (unlikely (nesting_level_বাম == 0))
      ঘূৰাই hb_empty_t ();

    যদি (paint_visited (&obj))
      ঘূৰাই hb_empty_t ();

    nesting_level_বাম--;
    obj.dependv1 (এতত্);
    nesting_level_বাম++;
    ঘূৰাই hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  তার্কিক paint_visited (ধ্রুৱ শূন্য *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    যদি (visited_paint.in_error() || visited_paint.has (delta))
      ঘূৰাই সঁচা;

    visited_paint.add (delta);
    যদি (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    ঘূৰাই মিছা;
  }

  ধ্রুৱ COLR* get_colr_table () ধ্রুৱ
  { ঘূৰাই colr; }

  সার্ৱজনিক:
  ধ্রুৱ COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_বাম;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ধ্রুৱ COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_বাম (nesting_level_বাম_2),
                          source_gid (0)
  {}
};

সংরচনা LayerRecord
{
  operator hb_ot_color_layer_t () ধ্রুৱ { ঘূৰাই {glyphId, colorIdx}; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা BaseGlyphRecord
{
  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	অঙ্ক_Layers;	/* Number of color layers
				 * associated with this glyph */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
সংরচনা Variable
{
  স্থির constexpr তার্কিক is_variable = সঁচা;

  ধ্রুৱ T& get_মান () ধ্রুৱ { ঘূৰাই value; }

  Variable<T>* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (এতত্));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ
  { value.dependv1 (c); }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    c->অঙ্ক_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->অঙ্ক_var_idxes);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!value.subset (c, instancer, varIdxBase)) return_trace (মিছা);
    যদি (c->plan->all_axes_pinned)
      return_trace (সঁচা);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    যদি (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
      যদি (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (মিছা);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    value.paint_glyph (c, varIdxBase);
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  শূন্য get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুৱ
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ধ্রুৱ
  {
    ঘূৰাই value.get_extend ();
  }

  রক্ষিত:
  T      value;
  সার্ৱজনিক:
  VarIdx varIdxBase;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
সংরচনা NoVariable
{
  স্থির constexpr তার্কিক is_variable = মিছা;

  ধ্রুৱ T& get_মান () ধ্রুৱ { ঘূৰাই value; }

  স্থির constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (এতত্));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ
  { value.dependv1 (c); }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  { value.closurev1 (c); }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    value.paint_glyph (c, varIdxBase);
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  শূন্য get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুৱ
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ধ্রুৱ
  {
    ঘূৰাই value.get_extend ();
  }

  T      value;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
স্থির শূন্য
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, ভিন্ন value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

সংরচনা ColorStop
{
  শূন্য dependv1 (hb_colrv1_depend_context_t * c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    c->add_palette_index (paletteIndex);
    c->অঙ্ক_var_idxes = 2;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      ভিন্ন a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

সংরচনা Extend : HBUINT8
{
  গণনা {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> শ্ৰেণী Var>
সংরচনা Color_সাৰি
{
  শূন্য dependv1 (hb_colrv1_depend_context_t * c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    যদি (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিছা);
    যদি (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (মিছা);

    বাবে (ধ্রুৱ auto& stop : stops.iter ())
    {
      যদি (!stop.subset (c, instancer)) return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned পূর্ণাঙ্ক
  get_color_stops (hb_paint_context_t *c,
                   unsigned পূর্ণাঙ্ক start,
		   unsigned পূর্ণাঙ্ক *গুনতি,
		   hb_color_stop_t *color_stops,
		   ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক len = stops.len;

    যদি (গুনতি && color_stops)
    {
      unsigned পূর্ণাঙ্ক i;
      বাবে (i = 0; i < *গুনতি && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *গুনতি = i;
    }

    ঘূৰাই len;
  }

  HB_INTERNAL স্থির unsigned পূর্ণাঙ্ক static_get_color_stops (hb_color_সাৰি_t *color_সাৰি,
							  শূন্য *color_সাৰি_data,
							  unsigned পূর্ণাঙ্ক start,
							  unsigned পূর্ণাঙ্ক *গুনতি,
							  hb_color_stop_t *color_stops,
							  শূন্য *user_data)
  {
    ধ্রুৱ Color_সাৰি *thiz = (ধ্রুৱ Color_সাৰি *) color_সাৰি_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    ঘূৰাই thiz->get_color_stops (c, start, গুনতি, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ধ্রুৱ
  {
    ৱিকল্প ((unsigned পূর্ণাঙ্ক) extend)
    {
      পক্ষ Extend::EXTEND_REPEAT:  ঘূৰাই HB_PAINT_EXTEND_REPEAT;
      পক্ষ Extend::EXTEND_REFLECT: ঘূৰাই HB_PAINT_EXTEND_REFLECT;
      পক্ষ Extend::EXTEND_PAD:
      স্ৱভাৱ:                     ঘূৰাই HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL স্থির hb_paint_extend_t static_get_extend (hb_color_সাৰি_t *color_সাৰি,
							  শূন্য *color_সাৰি_data,
							  শূন্য *user_data)
  {
    ধ্রুৱ Color_সাৰি *thiz = (ধ্রুৱ Color_সাৰি *) color_সাৰি_data;
    ঘূৰাই thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
সংরচনা CompositeMode : HBUINT8
{
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (1);
};

সংরচনা Affine2x3
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  { c->অঙ্ক_var_idxes = 6; }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);
    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((দ্ৱিগুণ) source.to_int () +
                                                    (দ্ৱিগুণ) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (সঁচা);
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    c->funcs->ঠেলো_transform (c->data,
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

সংরচনা PaintColrLayers
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    uint32_t first_layer_index = অঙ্ক_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  অন্তঃস্থ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ;

  HBUINT8	format; /* format = 1 */
  HBUINT8	অঙ্ক_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা PaintSolid
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ {}
  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    c->add_palette_index (paletteIndex);
    c->অঙ্ক_var_idxes = 1;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ভিন্ন a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    যদি (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুৱ
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> শ্ৰেণী Var>
সংরচনা PaintLinearGradient
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    (এতত্+color_সাৰি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 6;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) x0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) y0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) x1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) y1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) x2 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) y2 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 5)));
    }

    যদি (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_সাৰি_2.serialize_subset (c, color_সাৰি_2, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && color_সাৰি_2.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    hb_color_সাৰি_t cl = {
      (শূন্য *) &(এতত্+color_সাৰি_2),
      (এতত্+color_সাৰি_2).static_get_color_stops, c,
      (এতত্+color_সাৰি_2).static_get_extend, nullptr
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
  Offset24To<Color_সাৰি<Var>>	color_সাৰি_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> শ্ৰেণী Var>
সংরচনা PaintRadialGradient
{
  তার্কিক constant_after_pinning (hb_subset_context_t *c,
                               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    যদি (c->plan->has_avar2) ঘূৰাই মিছা;
    ধ্রুৱ auto &regions = instancer.varStore->get_region_list ();
    বাবে (unsigned i = 0; i < regions.region_গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      যদি (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        ঘূৰাই মিছা;
      }
      বাবে (auto axis : axes)
      {
        Triple *limit;
        যদি (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          ঘূৰাই মিছা;
      }
    }
    ঘূৰাই সঁচা;
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    (এতত্+color_সাৰি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 6;
  }

  তার্কিক subset_nonnegative (hb_subset_context_t *c,
                           ধ্রুৱ ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ধ্রুৱ
  {
    দ্ৱিগুণ source[6] = {
      x0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 0)),
      y0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)),
      radius0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)),
      x1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 3)),
      y1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 4)),
      radius1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      যদি (!out) ঘূৰাই মিছা;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      ঘূৰাই সঁচা;
    };
    ধ্রুৱ auto &সাৰি = এতত্+color_সাৰি_2;
    যদি (source[2] == source[5] || !সাৰি.stops.len) ঘূৰাই transparent ();
    সংরচনা stop_t { দ্ৱিগুণ offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    যদি (!stops.alloc (সাৰি.stops.len)) ঘূৰাই মিছা;
    unsigned order = 0;
    বাবে (ধ্রুৱ auto &entry : সাৰি.stops.iter ())
    {
      ধ্রুৱ auto &stop = entry.get_মান ();
      stops.push (stop_t {(দ্ৱিগুণ) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (দ্ৱিগুণ) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    যদি (stops.in_error ()) ঘূৰাই মিছা;
    stops.qsort ([] (ধ্রুৱ stop_t &a, ধ্রুৱ stop_t &b) {
      ঘূৰাই a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    দ্ৱিগুণ dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    বাবে (unsigned j = 0; j < 3; j++)
    {
      দ্ৱিগুণ remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      যদি (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        ঘূৰাই মিছা;
      }
      unsigned ডান = (unsigned) remainder;
      যেতিয়ালৈ (ডান) { unsigned r = lattice % ডান; lattice = ডান; ডান = r; }
    }
    বাবে (unsigned shift = 0; shift < 16; shift++)
    {
      দ্ৱিগুণ b = 1u << shift;
      দ্ৱিগুণ coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      দ্ৱিগুণ fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      দ্ৱিগুণ candidates[] = {coarse, fine};
      বাবে (দ্ৱিগুণ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        দ্ৱিগুণ geometry[6];
        তার্কিক fits = সঁচা;
        বাবে (unsigned j = 0; j < 3; j++)
        {
          দ্ৱিগুণ difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          দ্ৱিগুণ নিচেৰ = j == 2 ? 0 : -32768, উপৰেৰ = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= নিচেৰ && geometry[j] <= উপৰেৰ &&
                  geometry[j + 3] >= নিচেৰ && geometry[j + 3] <= উপৰেৰ;
        }
        যদি (!fits) অনুৱর্ত;
        unsigned first = 0, গুনতি = stops.length;
        দ্ৱিগুণ min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        দ্ৱিগুণ bias = 0;
        তার্কিক constant = মিছা;
        যদি (সাৰি.extend == Extend::EXTEND_PAD)
        {
          দ্ৱিগুণ tip = -geometry[2] / (b * dr);
          যদি ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            গুনতি = 1; constant = সঁচা;
          }
        }
        অন্যথা
        {
          দ্ৱিগুণ period = max - min;
          যদি (সাৰি.extend == Extend::EXTEND_REFLECT) period *= 2;
          যদি (period <= 0) ঘূৰাই transparent ();
          দ্ৱিগুণ n = ceil ((-2 - min) / period);
          যদি (n > floor ((32767. / 16384. - max) / period)) অনুৱর্ত;
          bias = n * period;
        }
        বাবে (unsigned j = 0; j < গুনতি; j++)
        {
          দ্ৱিগুণ bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        যদি (!fits) অনুৱর্ত;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        যদি (!out) ঘূৰাই মিছা;
        out->format = 6;
        out->x0 = (পূর্ণাঙ্ক) geometry[0]; out->y0 = (পূর্ণাঙ্ক) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (পূর্ণাঙ্ক) geometry[3]; out->y1 = (পূর্ণাঙ্ক) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_সাৰি = c->serializer->allocate_size<Color_সাৰি<NoVariable>> (3 + গুনতি * ColorStop::static_size);
        যদি (!out_সাৰি) { c->serializer->বেৰ_কৰো_discard (); ঘূৰাই মিছা; }
        out_সাৰি->extend = সাৰি.extend;
        out_সাৰি->stops.len = গুনতি;
        বাবে (unsigned j = 0; j < গুনতি; j++)
        {
          ধ্রুৱ auto &stop = stops[first + j];
          auto &out_stop = out_সাৰি->stops[j].value;
          out_stop.stopOffset.set_float ((ভিন্ন) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((ভিন্ন) stop.alpha);
          যদি (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->বেৰ_কৰো_discard (); ঘূৰাই মিছা; }
        }
        c->serializer->add_link (out->color_সাৰি_2, c->serializer->বেৰ_কৰো_pack ());
        ঘূৰাই !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    ঘূৰাই মিছা;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) x0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) y0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((দ্ৱিগুণ) radius0 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) x1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((দ্ৱিগুণ) y1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((দ্ৱিগুণ) radius1 + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 5)));
    }

    যদি (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_সাৰি_2.serialize_subset (c, color_সাৰি_2, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && color_সাৰি_2.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    hb_color_সাৰি_t cl = {
      (শূন্য *) &(এতত্+color_সাৰি_2),
      (এতত্+color_সাৰি_2).static_get_color_stops, c,
      (এতত্+color_সাৰি_2).static_get_extend, nullptr
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
  Offset24To<Color_সাৰি<Var>>	color_সাৰি_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> শ্ৰেণী Var>
সংরচনা PaintSweepGradient
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    (এতত্+color_সাৰি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 4;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerX + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerY + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    যদি (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_সাৰি_2.serialize_subset (c, color_সাৰি_2, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && color_সাৰি_2.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    hb_color_সাৰি_t cl = {
      (শূন্য *) &(এতত্+color_সাৰি_2),
      (এতত্+color_সাৰি_2).static_get_color_stops, c,
      (এতত্+color_সাৰি_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_সাৰি<Var>>	color_সাৰি_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
সংরচনা PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    return_trace (out->paint.serialize_subset (c, paint, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && paint.sanitize (c, এতত্));
  }

  অন্তঃস্থ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

সংরচনা PaintColrGlyph
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  অন্তঃস্থ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> শ্ৰেণী Var>
সংরচনা PaintTransform
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);
    যদি (!out->transform.serialize_subset (c, transform, এতত্, instancer)) return_trace (মিছা);
    যদি (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  src.sanitize (c, এতত্) &&
                  transform.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    (এতত্+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (7);
};

সংরচনা PaintTranslate
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((দ্ৱিগুণ) dx + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((দ্ৱিগুণ) dy + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
    }

    যদি (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন ddx = dx + c->instancer (varIdxBase, 0);
    ভিন্ন ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ঠেলো_translate (c->data, ddx, ddy);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
স্থির তার্কিক serialize_scale_transform (hb_subset_context_t *c,
                                       ভিন্ন sx, ভিন্ন sy, ভিন্ন dx, ভিন্ন dy,
                                       ধ্রুৱ Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  যদি (unlikely (!out)) ঘূৰাই মিছা;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  যদি (unlikely (!transform))
  {
    c->serializer->বেৰ_কৰো_discard ();
    ঘূৰাই মিছা;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->বেৰ_কৰো_pack ());

  c->serializer->push ();
  তার্কিক ret = source ();
  যদি (ret)
    c->serializer->add_link (out->src, c->serializer->বেৰ_কৰো_pack ());
  অন্যথা
    c->serializer->বেৰ_কৰো_discard ();
  ঘূৰাই ret;
}

template <typename Base>
স্থির তার্কিক subset_scale_as_transform (hb_subset_context_t *c,
                                      ধ্রুৱ Base *base,
                                      ধ্রুৱ Offset24To<Paint> &src,
                                      ধ্রুৱ ItemVarStoreInstancer &instancer,
                                      ভিন্ন sx, ভিন্ন sy,
                                      ভিন্ন cx = 0, ভিন্ন cy = 0)
{
  যদি (src.is_null ()) ঘূৰাই মিছা;
  auto source = [&] { ঘূৰাই c->dispatch (base+src, instancer); };
  ভিন্ন dx = cx * (1 - sx), dy = cy * (1 - sy);
  যদি (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    ঘূৰাই serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    ঘূৰাই serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    ঘূৰাই serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    ঘূৰাই serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    ঘূৰাই serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  ঘূৰাই serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

সংরচনা PaintScale
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ভিন্ন sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ভিন্ন sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      যদি (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, এতত্, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    যদি (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ভিন্ন sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ঠেলো_scale (c->data, sx, sy);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

সংরচনা PaintScaleAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ভিন্ন sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      ভিন্ন sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      যদি (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, এতত্, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerX + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerY + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 3)));
    }

    যদি (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    ভিন্ন sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    ভিন্ন tCenterX = centerX + c->instancer (varIdxBase, 2);
    ভিন্ন tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ঠেলো_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

সংরচনা PaintScaleUniform
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ভিন্ন s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      যদি (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, এতত্, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    যদি (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ঠেলো_scale (c->data, s, s);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

সংরচনা PaintScaleUniformAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      ভিন্ন s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      যদি (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, এতত্, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerX + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerY + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
    }

    যদি (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন s = scale.to_float (c->instancer (varIdxBase, 0));
    ভিন্ন tCenterX = centerX + c->instancer (varIdxBase, 1);
    ভিন্ন tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ঠেলো_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

সংরচনা PaintRotate
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    যদি (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ঠেলো_rotate (c->data, a);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

সংরচনা PaintRotateAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerX + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerY + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
    }

    যদি (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন a = angle.to_float (c->instancer (varIdxBase, 0));
    ভিন্ন tCenterX = centerX + c->instancer (varIdxBase, 1);
    ভিন্ন tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ঠেলো_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

সংরচনা PaintSkew
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    যদি (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ভিন্ন sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ঠেলো_skew (c->data, sx, sy);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

সংরচনা PaintSkewAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerX + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((দ্ৱিগুণ) centerY + (দ্ৱিগুণ) roundf (instancer (varIdxBase, 3)));
    }

    যদি (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, এতত্, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && src.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    ভিন্ন sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    ভিন্ন sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    ভিন্ন tCenterX = centerX + c->instancer (varIdxBase, 2);
    ভিন্ন tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ঠেলো_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

সংরচনা PaintComposite
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুৱ;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    তার্কিক ret = মিছা;
    ret |= out->src.serialize_subset (c, src, এতত্, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, এতত্, instancer);
    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  c->check_ops (এতত্->min_size) && // PainComposite can get exponential
                  src.sanitize (c, এতত্) &&
                  backdrop.sanitize (c, এতত্));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুৱ
  {
    TRACE_PAINT (এতত্);
    hb_paint_composite_mode_t composite_mode;
    ৱিকল্প ((unsigned পূর্ণাঙ্ক) mode)
    {
      পক্ষ HB_PAINT_COMPOSITE_MODE_CLEAR:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_IN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      পক্ষ HB_PAINT_COMPOSITE_MODE_XOR:
      পক্ষ HB_PAINT_COMPOSITE_MODE_PLUS:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SCREEN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_OVERLAY:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DARKEN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      পক্ষ HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      পক্ষ HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned পূর্ণাঙ্ক) mode;
	ভঙ্গ;
      স্ৱভাৱ:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	ভঙ্গ;
    }

    c->funcs->ঠেলো_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (এতত্+backdrop);
    c->funcs->ঠেলো_group_for (c->data, composite_mode);
    c->recurse (এতত্+src);
    c->funcs->বেৰ_কৰো_group (c->data, composite_mode);
    c->funcs->বেৰ_কৰো_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা ClipBoxData
{
  পূর্ণাঙ্ক xMin, yMin, xMax, yMax;
};

সংরচনা ClipBoxFormat1
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_clip_box (ClipBoxData &clip_box, ধ্রুৱ ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুৱ
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((দ্ৱিগুণ) xMin + (দ্ৱিগুণ) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((দ্ৱিগুণ) yMin + (দ্ৱিগুণ) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((দ্ৱিগুণ) xMax + (দ্ৱিগুণ) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((দ্ৱিগুণ) yMax + (দ্ৱিগুণ) ceilf (instancer (varIdxBase, 3)));
    }

    যদি (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (সঁচা);
  }

  সার্ৱজনিক:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

সংরচনা ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  শূন্য get_clip_box (ClipBoxData &clip_box, ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    value.get_clip_box(clip_box, instancer);
    যদি (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<পূর্ণাঙ্ক> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<পূর্ণাঙ্ক> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<পূর্ণাঙ্ক> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<পূর্ণাঙ্ক> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

সংরচনা ClipBox
{
  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 2: hb_barrier (); u.format2.closurev1 (c); ঘূৰাই;
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  তার্কিক get_extents (hb_glyph_extents_t *extents,
                    ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    ClipBoxData clip_box;
    ৱিকল্প (u.format.v) {
    পক্ষ 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      ভঙ্গ;
    পক্ষ 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      ভঙ্গ;
    স্ৱভাৱ:
      ঘূৰাই মিছা;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    ঘূৰাই সঁচা;
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

সংরচনা ClipRecord
{
  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    যদি (!c->glyphs->intersects (startGlyphID, endGlyphID)) ঘূৰাই;
    (base+clipBox).closurev1 (c);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ শূন্য *base,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && clipBox.sanitize (c, base));
  }

  তার্কিক get_extents (hb_glyph_extents_t *extents,
		    ধ্রুৱ শূন্য *base,
		    ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    ঘূৰাই (base+clipBox).get_extents (extents, instancer);
  }

  সার্ৱজনিক:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

সংরচনা ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ধ্রুৱ ItemVarStoreInstancer &instancer,
                                   ধ্রুৱ hb_set_t& gids,
                                   ধ্রুৱ hb_map_t& gid_offset_map) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    যদি (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned গুনতি  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    বাবে (ধ্রুৱ hb_codepoint_t _ : gids.iter ())
    {
      যদি (_ == start_gid) অনুৱর্ত;

      offset = gid_offset_map.get (_);
      যদি (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        অনুৱর্ত;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      যদি (!record.subset (c, এতত্, instancer)) return_trace (0);
      গুনতি++;

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
      যদি (!record.subset (c, এতত্, instancer)) return_trace (0);
      গুনতি++;
    }
    return_trace (গুনতি);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    যদি (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিছা);

    ধ্রুৱ hb_set_t& glyphset = c->plan->_glyphset_colred;
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    বাবে (ধ্রুৱ ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      বাবে (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        যদি (!glyphset.has (gid) || !glyph_map.has (gid)) অনুৱর্ত;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned গুনতি = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    যদি (!গুনতি) return_trace (মিছা);
    return_trace (c->serializer->check_assign (out->clips.len, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (এতত্) && clips.sanitize (c, এতত্));
  }

  তার্কিক
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    auto *rec = clips.as_array ().bsearch (gid);
    যদি (rec)
      ঘূৰাই rec->get_extents (extents, এতত্, instancer);
    ঘূৰাই মিছা;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

সংরচনা Paint
{

  template <typename ...Ts>
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    যদি (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_মান ());

    return_trace (c->end_recursion (এতত্->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    পক্ষ 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    পক্ষ 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    পক্ষ 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    পক্ষ 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    পক্ষ 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    পক্ষ 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    পক্ষ 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    পক্ষ 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    পক্ষ 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    পক্ষ 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    পক্ষ 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    পক্ষ 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    পক্ষ 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    পক্ষ 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    পক্ষ 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    পক্ষ 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    পক্ষ 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    পক্ষ 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    পক্ষ 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    পক্ষ 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    পক্ষ 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    পক্ষ 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    পক্ষ 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    পক্ষ 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    পক্ষ 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    পক্ষ 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    পক্ষ 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    পক্ষ 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  তার্কিক get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); ঘূৰাই সঁচা;
    পক্ষ 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); ঘূৰাই সঁচা;
    স্ৱভাৱ: ঘূৰাই মিছা;
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT8 v; }				format;
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
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (2);
};

সংরচনা BaseGlyphPaintRecord
{
  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  তার্কিক serialize (hb_serialize_context_t *s, ধ্রুৱ hb_map_t* glyph_map,
                  ধ্রুৱ শূন্য* src_base, hb_subset_context_t *c,
                  ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = s->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);
    যদি (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) && paint.sanitize (c, base)));
  }

  সার্ৱজনিক:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিছা);
    ধ্রুৱ hb_set_t* glyphset = &c->plan->_glyphset_colred;

    বাবে (ধ্রুৱ auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      যদি (!glyphset->has (gid)) অনুৱর্ত;

      যদি (_.serialize (c->serializer, c->plan->glyph_map, এতত্, c, instancer)) out->len++;
      অন্যথা return_trace (মিছা);
    }

    return_trace (out->len != 0);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, এতত্));
  }
};

সংরচনা LayerList : Array32OfOffset32To<Paint>
{
  ধ্রুৱ Paint& get_paint (unsigned i) ধ্রুৱ
  { ঘূৰাই এতত্+(*এতত্)[i]; }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ ItemVarStoreInstancer &instancer) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিছা);

    তার্কিক ret = মিছা;
    বাবে (ধ্রুৱ auto& _ : + hb_enumerate (*এতত্)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (মিছা);
      ret |= o->serialize_subset (c, _.second, এতত্, instancer);
    }
    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, এতত্));
  }
};

সংরচনা delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_গুনতি () ধ্রুৱ { ঘূৰাই inner_bit_গুনতি; }
  unsigned get_width ()           ধ্রুৱ { ঘূৰাই ((outer_bit_গুনতি + inner_bit_গুনতি + 7) / 8); }
  hb_array_t<ধ্রুৱ uint32_t> get_output_map () ধ্রুৱ { ঘূৰাই output_map.as_array (); }

  delta_set_index_map_subset_plan_t (ধ্রুৱ hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_গুনতি = 0;
    outer_bit_গুনতি = 0;
    inner_bit_গুনতি = 1;
    output_map.init ();

    /* search backwards */
    unsigned গুনতি = new_deltaset_idx_varidx_map.get_population ();
    যদি (!গুনতি) ঘূৰাই;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    বাবে (unsigned i = গুনতি; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      যদি (i == গুনতি)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        অনুৱর্ত;
      }
      যদি (var_idx != last_varidx)
        ভঙ্গ;
      last_idx = delta_set_idx;
    }

    map_গুনতি = last_idx + 1;
  }

  তার্কিক remap (ধ্রুৱ hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    যদি (unlikely (!output_map.resize_dirty (map_গুনতি))) ঘূৰাই মিছা;

    বাবে (unsigned idx = 0; idx < map_গুনতি; idx++)
    {
      uint32_t *var_idx;
      যদি (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) ঘূৰাই মিছা;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_গুনতি = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_গুনতি = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_গুনতি = hb_max (bit_গুনতি, inner_bit_গুনতি);
    }
    ঘূৰাই সঁচা;
  }

  গুপ্ত:
  unsigned map_গুনতি;
  unsigned outer_bit_গুনতি;
  unsigned inner_bit_গুনতি;
  hb_vector_t<uint32_t> output_map;
};

সংরচনা COLR
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই has_v0_data () || version; }

  তার্কিক has_v0_data () ধ্রুৱ { ঘূৰাই অঙ্ক_Base_Glyphs; }
  তার্কিক has_v1_data () ধ্রুৱ
  {
    যদি (version < 1)
      ঘূৰাই মিছা;
    hb_barrier ();

    ঘূৰাই (এতত্+baseGlyphList).len > 0;
  }

  unsigned পূর্ণাঙ্ক get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned পূর্ণাঙ্ক         start_offset,
				 unsigned পূর্ণাঙ্ক        *গুনতি, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ধ্রুৱ
  {
    ধ্রুৱ BaseGlyphRecord &record = (এতত্+baseGlyphsZ).bsearch (অঙ্ক_Base_Glyphs, glyph);

    hb_array_t<ধ্রুৱ LayerRecord> all_layers = (এতত্+layersZ).as_array (অঙ্ক_Layers);
    hb_array_t<ধ্রুৱ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.অঙ্ক_Layers);
    যদি (গুনতি && layers)
    {
      + glyph_layers.sub_array (start_offset, গুনতি)
      | hb_sink (hb_array (layers, *গুনতি))
      ;
    }
    ঘূৰাই glyph_layers.length;
  }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      যদি (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    তার্কিক has_data () ধ্রুৱ { ঘূৰাই colr->has_data (); }

#ifndef HB_NO_PAINT
    তার্কিক
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ধ্রুৱ
    {
      যদি (unlikely (!has_data ())) ঘূৰাই মিছা;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ঘূৰাই সঁচা;
      তার্কিক ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      ঘূৰাই ret;
    }

    তার্কিক paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, শূন্য *data,
		      unsigned পূর্ণাঙ্ক palette_index,
		      hb_color_t foreground,
		      তার্কিক clip = সঁচা) ধ্রুৱ
    {
      যদি (unlikely (!has_data ())) ঘূৰাই মিছা;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ঘূৰাই সঁচা;
      তার্কিক ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      ঘূৰাই ret;
    }
#endif

    তার্কিক is_valid () { ঘূৰাই colr.get_blob ()->length; }

    শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুৱ
    {
      যদি (!has_data ()) ঘূৰাই;
      colr->depend (depend_data);
    }

    শূন্য closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ধ্রুৱ
    { colr->closure_glyphs (glyph, related_ids); }

    শূন্য closure_V0palette_indices (ধ্রুৱ hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ধ্রুৱ
    { colr->closure_V0palette_indices (glyphs, palettes); }

    শূন্য closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ধ্রুৱ
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    তার্কিক has_var_store () ধ্রুৱ
    { ঘূৰাই colr->has_var_store (); }

    ধ্রুৱ ItemVariationStore &get_var_store () ধ্রুৱ
    { ঘূৰাই colr->get_var_store (); }
    ধ্রুৱ ItemVariationStore *get_var_store_ptr () ধ্রুৱ
    { ঘূৰাই colr->get_var_store_ptr (); }

    তার্কিক has_delta_set_index_map () ধ্রুৱ
    { ঘূৰাই colr->has_delta_set_index_map (); }

    ধ্রুৱ DeltaSetIndexMap &get_delta_set_index_map () ধ্রুৱ
    { ঘূৰাই colr->get_delta_set_index_map (); }
    ধ্রুৱ DeltaSetIndexMap *get_delta_set_index_map_ptr () ধ্রুৱ
    { ঘূৰাই colr->get_delta_set_index_map_ptr (); }

    গুপ্ত:

    hb_colr_scratch_t *acquire_scratch () ধ্রুৱ
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, পরিমাণ (hb_colr_scratch_t));
	যদি (unlikely (!scratch))
	  ঘূৰাই nullptr;
      }

      ঘূৰাই scratch;
    }
    শূন্য release_scratch (hb_colr_scratch_t *scratch) ধ্রুৱ
    {
      যদি (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    সার্ৱজনিক:
    hb_blob_ptr_t<COLR> colr;
    গুপ্ত:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুৱ
  {
    // v0
    hb_array_t<ধ্রুৱ BaseGlyphRecord> baseGlyphs = (এতত্+baseGlyphsZ).as_array (অঙ্ক_Base_Glyphs);
    বাবে (ধ্রুৱ BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (এতত্+layersZ).as_array (অঙ্ক_Layers).sub_array (record.firstLayerIdx,
                                                                         record.অঙ্ক_Layers);
      বাবে (ধ্রুৱ LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    যদি (!has_v1_data())
      ঘূৰাই;

    // v1
    hb_colrv1_depend_context_t c (এতত্, depend_data);
    ধ্রুৱ BaseGlyphList &baseglyph_paintrecords = এতত্+baseGlyphList;

    বাবে (ধ্রুৱ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ধ্রুৱ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  শূন্য closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ধ্রুৱ
  {
    ধ্রুৱ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    যদি (!record) ঘূৰাই;

    auto glyph_layers = (এতত্+layersZ).as_array (অঙ্ক_Layers).sub_array (record->firstLayerIdx,
								       record->অঙ্ক_Layers);
    যদি (!glyph_layers.length) ঘূৰাই;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  শূন্য closure_V0palette_indices (ধ্রুৱ hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ধ্রুৱ
  {
    যদি (!অঙ্ক_Base_Glyphs || !অঙ্ক_Layers) ঘূৰাই;
    hb_array_t<ধ্রুৱ BaseGlyphRecord> baseGlyphs = (এতত্+baseGlyphsZ).as_array (অঙ্ক_Base_Glyphs);
    hb_array_t<ধ্রুৱ LayerRecord> all_layers = (এতত্+layersZ).as_array (অঙ্ক_Layers);

    বাবে (ধ্রুৱ BaseGlyphRecord record : baseGlyphs)
    {
      যদি (!glyphs->has (record.glyphId)) অনুৱর্ত;
      hb_array_t<ধ্রুৱ LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.অঙ্ক_Layers);
      বাবে (ধ্রুৱ LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  শূন্য closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ধ্রুৱ
  {
    যদি (version < 1) ঘূৰাই;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (এতত্, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ধ্রুৱ BaseGlyphList &baseglyph_paintrecords = এতত্+baseGlyphList;

    বাবে (ধ্রুৱ BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      যদি (!glyphset->has (gid)) অনুৱর্ত;

      ধ্রুৱ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ধ্রুৱ ClipList &cliplist = এতত্+clipList;
    c.glyphs = glyphset;
    বাবে (ধ্রুৱ ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    যদি (has_delta_set_index_map ())
    {
      ধ্রুৱ DeltaSetIndexMap &var_idx_map = এতত্+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      বাবে (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ধ্রুৱ LayerList& get_layerList () ধ্রুৱ
  { ঘূৰাই (এতত্+layerList); }

  ধ্রুৱ BaseGlyphList& get_baseglyphList () ধ্রুৱ
  { ঘূৰাই (এতত্+baseGlyphList); }

  তার্কিক has_var_store () ধ্রুৱ
  { ঘূৰাই version >= 1 && hb_barrier () && varStore != 0; }

  তার্কিক has_delta_set_index_map () ধ্রুৱ
  { ঘূৰাই version >= 1 && hb_barrier () && varIdxMap != 0; }

  তার্কিক has_clip_list () ধ্রুৱ
  { ঘূৰাই version >= 1 && hb_barrier () && clipList != 0; }

  ধ্রুৱ DeltaSetIndexMap &get_delta_set_index_map () ধ্রুৱ
  { ঘূৰাই has_delta_set_index_map () && hb_barrier () ? এতত্+varIdxMap : Null (DeltaSetIndexMap); }
  ধ্রুৱ DeltaSetIndexMap *get_delta_set_index_map_ptr () ধ্রুৱ
  { ঘূৰাই has_delta_set_index_map () && hb_barrier () ? &(এতত্+varIdxMap) : nullptr; }

  ধ্রুৱ ItemVariationStore &get_var_store () ধ্রুৱ
  { ঘূৰাই has_var_store () && hb_barrier () ? এতত্+varStore : Null (ItemVariationStore); }
  ধ্রুৱ ItemVariationStore *get_var_store_ptr () ধ্রুৱ
  { ঘূৰাই has_var_store () && hb_barrier () ? &(এতত্+varStore) : nullptr; }

  ধ্রুৱ ClipList &get_clip_list () ধ্রুৱ
  { ঘূৰাই has_clip_list () && hb_barrier () ? এতত্+clipList : Null (ClipList); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
                  (এতত্+baseGlyphsZ).sanitize (c, অঙ্ক_Base_Glyphs) &&
                  (এতত্+layersZ).sanitize (c, অঙ্ক_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, এতত্) &&
		    layerList.sanitize (c, এতত্) &&
		    clipList.sanitize (c, এতত্) &&
		    varIdxMap.sanitize (c, এতত্) &&
		    varStore.sanitize (c, এতত্))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  তার্কিক serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (base_it.len () != layer_it.len ()))
      return_trace (মিছা);

    এতত্->version = version;
    অঙ্ক_Layers = 0;
    অঙ্ক_Base_Glyphs = base_it.len ();
    যদি (অঙ্ক_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (সঁচা);
    }

    c->push ();
    বাবে (ধ্রুৱ hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      যদি (unlikely (!record)) return_trace (মিছা);
      record->firstLayerIdx = অঙ্ক_Layers;
      অঙ্ক_Layers += record->অঙ্ক_Layers;
    }
    c->add_link (baseGlyphsZ, c->বেৰ_কৰো_pack ());

    c->push ();
    বাবে (ধ্রুৱ hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->বেৰ_কৰো_pack ());

    return_trace (সঁচা);
  }

  ধ্রুৱ BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ধ্রুৱ
  {
    ধ্রুৱ BaseGlyphRecord* record = &(এতত্+baseGlyphsZ).bsearch (অঙ্ক_Base_Glyphs, (unsigned পূর্ণাঙ্ক) gid);
    যদি (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    ঘূৰাই record;
  }

  ধ্রুৱ BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ধ্রুৱ
  {
    ধ্রুৱ BaseGlyphList &list = এতত্+baseGlyphList;
    unsigned পূর্ণাঙ্ক i;
    যদি (!list.bfind ((unsigned) gid, &i))
      ঘূৰাই nullptr;
    ঘূৰাই &list[i];
  }

  তার্কিক downgrade_to_V0 (ধ্রুৱ hb_set_t &glyphset) ধ্রুৱ
  {
    //no more COLRv1 glyphs, downgrade to version 0
    বাবে (ধ্রুৱ BaseGlyphPaintRecord& _ : get_baseglyphList ())
      যদি (glyphset.has (_.glyphId))
        ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  তার্কিক subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (সঁচা);

    ধ্রুৱ ItemVariationStore& var_store = এতত্+varStore;
    যদি (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      তার্কিক optimize = (varIdxMap != 0) ? সঁচা : মিছা;
      যদি (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (মিছা);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      যদি (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (মিছা);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      যদি (optimize)
      {
        ধ্রুৱ hb_map_t &varidx_map = item_vars.get_varidx_map ();
        বাবে (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          যদি (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          অন্যথা
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    অন্যথা
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      যদি (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      যদি (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  তার্কিক subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (সঁচা);

    ধ্রুৱ hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    যদি (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (মিছা);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ধ্রুৱ hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->অঙ্ক_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    যদি (glyphset.has (old_gid)) ঘূৰাই সঁচা;
		    ঘূৰাই মিছা;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ধ্রুৱ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				যদি (unlikely (!old_record))
				  ঘূৰাই hb_pair_t<তার্কিক, BaseGlyphRecord> (মিছা, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.অঙ্ক_Layers = old_record->অঙ্ক_Layers;
				ঘূৰাই hb_pair_t<তার্কিক, BaseGlyphRecord> (সঁচা, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->অঙ্ক_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				ধ্রুৱ BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				যদি (unlikely (!old_record ||
					      old_record->firstLayerIdx >= অঙ্ক_Layers ||
					      old_record->firstLayerIdx + old_record->অঙ্ক_Layers > অঙ্ক_Layers))
				  ঘূৰাই hb_pair_t<তার্কিক, hb_vector_t<LayerRecord>> (মিছা, out_layers);

				auto layers = (এতত্+layersZ).as_array (অঙ্ক_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->অঙ্ক_Layers);
				out_layers.resize (layers.length);
				বাবে (unsigned পূর্ণাঙ্ক i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  যদি (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    ঘূৰাই hb_pair_t<তার্কিক, hb_vector_t<LayerRecord>> (মিছা, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				ঘূৰাই hb_pair_t<তার্কিক, hb_vector_t<LayerRecord>> (সঁচা, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    যদি (version == 0 && (!base_it || !layer_it))
      return_trace (মিছা);

    auto *colr_মৌলিক = c->serializer->start_embed<COLR> ();
    যদি (unlikely (!c->serializer->extend_min (colr_মৌলিক)))  return_trace (মিছা);

    যদি (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_মৌলিক->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    যদি (!c->serializer->allocate_size<শূন্য> (5 * HBUINT32::static_size)) return_trace (মিছা);
    যদি (!colr_মৌলিক->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (মিছা);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    যদি (!subset_varstore (c, colr_মৌলিক)) return_trace (মিছা);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    যদি (!colr_মৌলিক->baseGlyphList.serialize_subset (c, baseGlyphList, এতত্, instancer))
      return_trace (মিছা);

    colr_মৌলিক->layerList.serialize_subset (c, layerList, এতত্, instancer);
    colr_মৌলিক->clipList.serialize_subset (c, clipList, এতত্, instancer);

    return_trace (subset_delta_set_index_map (c, colr_মৌলিক));
  }

  ধ্রুৱ Paint *get_base_glyph_paint (hb_codepoint_t glyph) ধ্রুৱ
  {
    ধ্রুৱ BaseGlyphList &baseglyph_paintrecords = এতত্+baseGlyphList;
    ধ্রুৱ BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    যদি (record)
    {
      ধ্রুৱ Paint &paint = &baseglyph_paintrecords+record->paint;
      ঘূৰাই &paint;
    }
    অন্যথা
      ঘূৰাই nullptr;
  }

#ifndef HB_NO_PAINT
  তার্কিক
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ধ্রুৱ
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->অঙ্ক_coords));

    যদি (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      ঘূৰাই সঁচা;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    তার্কিক ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), সঁচা, scratch);

    auto e = scratch.paint_extents.get_extents ();
    যদি (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    অন্যথা
      *extents = e.to_glyph_extents ();

    ঘূৰাই ret;
  }
#endif

  তার্কিক
  has_paint_for_glyph (hb_codepoint_t glyph) ধ্রুৱ
  {
    যদি (version >= 1)
    {
      hb_barrier ();

      ধ্রুৱ Paint *paint = get_base_glyph_paint (glyph);

      ঘূৰাই paint != nullptr;
    }

    ঘূৰাই মিছা;
  }

  তার্কিক get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ধ্রুৱ ItemVarStoreInstancer instancer) ধ্রুৱ
  {
    ঘূৰাই get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  তার্কিক
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, শূন্য *data,
	       unsigned পূর্ণাঙ্ক palette_index, hb_color_t foreground,
	       তার্কিক clip,
	       hb_colr_scratch_t &scratch) ধ্রুৱ
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->অঙ্ক_coords : 0));
    hb_paint_context_t c (এতত্, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_গ্ৰন্থি_t node (c.glyphs_decycler);
    node.visit (glyph);

    যদি (version >= 1)
    {
      hb_barrier ();

      ধ্রুৱ Paint *paint = get_base_glyph_paint (glyph);
      যদি (paint)
      {
        // COLRv1 glyph

	তার্কিক is_bounded = সঁচা;
	hb_glyph_extents_t extents = {};
	যদি (clip)
	{
	  যদি (!get_clip (glyph, &extents, instancer))
	  {
	    clip = মিছা;
	    is_bounded = মিছা;
	  }

	  যদি (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 মিছা,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ঠেলো_font_transform (c.data, font);

	যদি (clip)
	  c.funcs->ঠেলো_clip_rectangle (c.data,
					(ভিন্ন) extents.x_bearing,
					(ভিন্ন) extents.y_bearing + extents.height,
					(ভিন্ন) extents.x_bearing + extents.width,
					(ভিন্ন) extents.y_bearing);

	যদি (is_bounded)
	  c.recurse (*paint);

	যদি (clip)
	  c.funcs->বেৰ_কৰো_clip (c.data);

	c.funcs->বেৰ_কৰো_transform (c.data);

        ঘূৰাই সঁচা;
      }
    }

    ধ্রুৱ BaseGlyphRecord *record = get_base_glyph_record (glyph);
    যদি (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      বাবে (ধ্রুৱ auto &r : (এতত্+layersZ).as_array (অঙ্ক_Layers)
			   .sub_array (record->firstLayerIdx, record->অঙ্ক_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      ঘূৰাই সঁচা;
    }

    ঘূৰাই মিছা;
  }
#endif

  রক্ষিত:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	অঙ্ক_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	অঙ্ক_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (14);
};

সংরচনা COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

শূন্য
hb_paint_context_t::recurse (ধ্রুৱ Paint &paint)
{
  যদি (unlikely (depth_বাম <= 0 || edge_গুনতি <= 0)) ঘূৰাই;
  depth_বাম--;
  edge_গুনতি--;
  paint.dispatch (এতত্);
  depth_বাম++;
}

template <typename Types>
শূন্য PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ধ্রুৱ
{
  TRACE_PAINT (এতত্);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  যদি (likely (c->depth_বাম > 0 && c->edge_গুনতি > 0) &&
      (এতত্+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_গুনতি--;
    c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->বেৰ_কৰো_transform (c->data);
    ঘূৰাই;
  }

  c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
  c->funcs->ঠেলো_clip_glyph (c->data, gid, c->font);
  c->funcs->ঠেলো_font_transform (c->data, c->font);
  c->recurse (এতত্+paint);
  c->funcs->বেৰ_কৰো_transform (c->data);
  c->funcs->বেৰ_কৰো_clip (c->data);
  c->funcs->বেৰ_কৰো_transform (c->data);
}

শূন্য PaintColrLayers::paint_glyph (hb_paint_context_t *c) ধ্রুৱ
{
  TRACE_PAINT (এতত্);
  ধ্রুৱ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_গ্ৰন্থি_t node (c->layers_decycler);
  বাবে (unsigned i = firstLayerIndex; i < firstLayerIndex + অঙ্ক_Layers; i++)
  {
    যদি (unlikely (!node.visit (i)))
      ঘূৰাই;

    ধ্রুৱ Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

শূন্য PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ধ্রুৱ
{
  TRACE_PAINT (এতত্);

  hb_decycler_গ্ৰন্থি_t node (c->glyphs_decycler);
  যদি (unlikely (!node.visit (gid)))
    ঘূৰাই;

  c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
  যদি (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->বেৰ_কৰো_transform (c->data);
    ঘূৰাই;
  }
  c->funcs->বেৰ_কৰো_transform (c->data);

  ধ্রুৱ COLR *colr_table = c->get_colr_table ();
  ধ্রুৱ Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  তার্কিক has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  যদি (has_clip_box)
    c->funcs->ঠেলো_clip_rectangle (c->data,
				   (ভিন্ন) extents.x_bearing,
				   (ভিন্ন) extents.y_bearing + extents.height,
				   (ভিন্ন) extents.x_bearing + extents.width,
				   (ভিন্ন) extents.y_bearing);

  যদি (paint)
    c->recurse (*paint);

  যদি (has_clip_box)
    c->funcs->বেৰ_কৰো_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
