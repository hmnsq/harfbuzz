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

#অন্তর্ভাব "../../../hb.hh"
#অন্তর্ভাব "../../../hb-decycler.hh"
#অন্তর্ভাব "../../../hb-open-type.hh"
#অন্তর্ভাব "../../../hb-ot-var-common.hh"
#অন্তর্ভাব "../../../hb-paint.hh"
#অন্তর্ভাব "../../../hb-paint-bounded.hh"
#অন্তর্ভাব "../../../hb-paint-extents.hh"
#অন্তর্ভাব "../../../hb-depend-data.hh"

#অন্তর্ভাব "../../../hb-ot-dual.hh"
#অন্তর্ভাব "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

নামাকাশ OT {
কাঠামো hb_paint_context_t;
}

কাঠামো hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

নামাকাশ OT {

কাঠামো COLR;

কাঠামো Paint;

কাঠামো hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ধ্রুবক অক্ষর *get_name () { ফেরত "PAINT"; }
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.paint_glyph (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  ধ্রুবক COLR* get_colr_table () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক COLR *> (base); }

সর্বজনীন:
  ধ্রুবক শূন্য *base;
  hb_paint_funcs_t *funcs;
  শূন্য *তথ্য;
  hb_font_t *font;
  hb_array_t<ধ্রুবক BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  পূর্ণসংখ্যা depth_বাম = HB_MAX_NESTING_LEVEL;
  পূর্ণসংখ্যা edge_গুনতি = HB_MAX_GRAPH_EDGE_গুনতি;

  hb_paint_context_t (ধ্রুবক শূন্য *base_,
		      hb_paint_funcs_t *funcs_,
		      শূন্য *তথ্য_2,
                      hb_font_t *font_,
                      unsigned পূর্ণসংখ্যা palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    তথ্য (তথ্য_2),
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
      hb_font_set_synthetic_bold (font, 0, 0, সত্য);
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
  color_alpha (hb_color_t color, দশমিক alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    ফেরত roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned পূর্ণসংখ্যা color_সূচক, দশমিক alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = সত্য;

    যদি (color_সূচক != 0xffff)
    {
      যদি (!funcs->custom_palette_color (তথ্য, color_সূচক, &color))
        color = palette[color_সূচক];

      *is_foreground = মিথ্যা;
    }

    ফেরত HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  অন্তঃরেখ শূন্য recurse (ধ্রুবক Paint &paint);
};

কাঠামো hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj)
  {
    যদি (unlikely (nesting_level_বাম == 0))
      ফেরত hb_empty_t ();

    যদি (paint_visited (&obj))
      ফেরত hb_empty_t ();

    nesting_level_বাম--;
    obj.closurev1 (এই);
    nesting_level_বাম++;
    ফেরত hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  যৌক্তিক paint_visited (ধ্রুবক শূন্য *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    যদি (visited_paint.in_error() || visited_paint.has (delta))
      ফেরত সত্য;

    visited_paint.add (delta);
    ফেরত মিথ্যা;
  }

  ধ্রুবক COLR* get_colr_table () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক COLR *> (base); }

  শূন্য add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  শূন্য add_layer_indices (unsigned first_layer_সূচক, unsigned অঙ্ক_of_layers)
  {
    যদি (অঙ্ক_of_layers == 0) ফেরত;
    layer_indices->add_range (first_layer_সূচক, first_layer_সূচক + অঙ্ক_of_layers - 1);
  }

  শূন্য add_palette_সূচক (unsigned palette_সূচক)
  { palette_indices->add (palette_সূচক); }

  শূন্য add_var_idxes (unsigned first_var_idx, unsigned অঙ্ক_idxes)
  {
    যদি (!অঙ্ক_idxes || first_var_idx == VarIdx::NO_VARIATION) ফেরত;
    variation_indices->add_range (first_var_idx, first_var_idx + অঙ্ক_idxes - 1);
  }

  সর্বজনীন:
  ধ্রুবক শূন্য *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned অঙ্ক_var_idxes;
  unsigned nesting_level_বাম;

  hb_colrv1_closure_context_t (ধ্রুবক শূন্য *base_,
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


কাঠামো hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj)
  {
    যদি (unlikely (nesting_level_বাম == 0))
      ফেরত hb_empty_t ();

    যদি (paint_visited (&obj))
      ফেরত hb_empty_t ();

    nesting_level_বাম--;
    obj.dependv1 (এই);
    nesting_level_বাম++;
    ফেরত hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  যৌক্তিক paint_visited (ধ্রুবক শূন্য *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    যদি (visited_paint.in_error() || visited_paint.has (delta))
      ফেরত সত্য;

    visited_paint.add (delta);
    যদি (unlikely (visited_paint.in_error ()))
      depend_তথ্য->fail ();
    ফেরত মিথ্যা;
  }

  ধ্রুবক COLR* get_colr_table () ধ্রুবক
  { ফেরত colr; }

  সর্বজনীন:
  ধ্রুবক COLR *colr;
  hb_depend_তথ্য_builder_t *depend_তথ্য;
  hb_set_t visited_paint;
  unsigned nesting_level_বাম;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (ধ্রুবক COLR *colr_,
                              hb_depend_তথ্য_builder_t *depend_তথ্য_2,
                              unsigned nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_তথ্য (depend_তথ্য_2),
                          nesting_level_বাম (nesting_level_বাম_2),
                          source_gid (0)
  {}
};

কাঠামো LayerRecord
{
  operator hb_ot_color_layer_t () ধ্রুবক { ফেরত {glyphId, colorIdx}; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  সূচক		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো BaseGlyphRecord
{
  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	অঙ্ক_Layers;	/* Number of color layers
				 * associated with this glyph */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

template <typename T>
কাঠামো Variable
{
  স্থির constexpr যৌক্তিক is_variable = সত্য;

  ধ্রুবক T& get_মান () ধ্রুবক { ফেরত value; }

  Variable<T>* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (এই));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক
  { value.dependv1 (c); }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    c->অঙ্ক_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->অঙ্ক_var_idxes);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!value.subset (c, instancer, varIdxBase)) return_trace (মিথ্যা);
    যদি (c->plan->all_axes_pinned)
      return_trace (সত্য);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    যদি (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
      যদি (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (মিথ্যা);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক
  {
    TRACE_PAINT (এই);
    value.paint_glyph (c, varIdxBase);
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  শূন্য get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুবক
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ধ্রুবক
  {
    ফেরত value.get_extend ();
  }

  সুরক্ষিত:
  T      value;
  সর্বজনীন:
  VarIdx varIdxBase;
  সর্বজনীন:
  DEFINE_আকার_MIN (VarIdx::static_আকার + T::min_আকার);
};

template <typename T>
কাঠামো NoVariable
{
  স্থির constexpr যৌক্তিক is_variable = মিথ্যা;

  ধ্রুবক T& get_মান () ধ্রুবক { ফেরত value; }

  স্থির constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (এই));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক
  { value.dependv1 (c); }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  { value.closurev1 (c); }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক
  {
    TRACE_PAINT (এই);
    value.paint_glyph (c, varIdxBase);
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  শূন্য get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুবক
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ধ্রুবক
  {
    ফেরত value.get_extend ();
  }

  T      value;
  সর্বজনীন:
  DEFINE_আকার_MIN (T::min_আকার);
};

// Color structures

template <typename Type, unsigned fraction_bits>
স্থির শূন্য
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, দশমিক value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

কাঠামো ColorStop
{
  শূন্য dependv1 (hb_colrv1_depend_context_t * c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    c->add_palette_সূচক (palette_সূচক_2);
    c->অঙ্ক_var_idxes = 2;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      দশমিক a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->palette_সূচক_2, c->plan->colr_palettes.get (palette_সূচক_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (palette_সূচক_2,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	palette_সূচক_2;
  F2DOT14	alpha;
  সর্বজনীন:
  DEFINE_আকার_STATIC (2 + 2 * F2DOT14::static_আকার);
};

কাঠামো Extend : HBUINT8
{
  গণনা {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  সর্বজনীন:
  DEFINE_আকার_STATIC (1);
};

template <template<typename> শ্রেণী Var>
কাঠামো Color_সারি
{
  শূন্য dependv1 (hb_colrv1_depend_context_t * c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    যদি (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিথ্যা);
    যদি (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto& stop : stops.iter ())
    {
      যদি (!stop.subset (c, instancer)) return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned পূর্ণসংখ্যা
  get_color_stops (hb_paint_context_t *c,
                   unsigned পূর্ণসংখ্যা start,
		   unsigned পূর্ণসংখ্যা *গুনতি,
		   hb_color_stop_t *color_stops,
		   ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা len = stops.len;

    যদি (গুনতি && color_stops)
    {
      unsigned পূর্ণসংখ্যা i;
      জন্য (i = 0; i < *গুনতি && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *গুনতি = i;
    }

    ফেরত len;
  }

  HB_INTERNAL স্থির unsigned পূর্ণসংখ্যা static_get_color_stops (hb_color_সারি_t *color_সারি,
							  শূন্য *color_সারি_তথ্য,
							  unsigned পূর্ণসংখ্যা start,
							  unsigned পূর্ণসংখ্যা *গুনতি,
							  hb_color_stop_t *color_stops,
							  শূন্য *user_তথ্য)
  {
    ধ্রুবক Color_সারি *thiz = (ধ্রুবক Color_সারি *) color_সারি_তথ্য;
    hb_paint_context_t *c = (hb_paint_context_t *) user_তথ্য;
    ফেরত thiz->get_color_stops (c, start, গুনতি, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ধ্রুবক
  {
    নির্বাচন ((unsigned পূর্ণসংখ্যা) extend)
    {
      ক্ষেত্র Extend::EXTEND_REPEAT:  ফেরত HB_PAINT_EXTEND_REPEAT;
      ক্ষেত্র Extend::EXTEND_REFLECT: ফেরত HB_PAINT_EXTEND_REFLECT;
      ক্ষেত্র Extend::EXTEND_PAD:
      পূর্বনির্ধারিত:                     ফেরত HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL স্থির hb_paint_extend_t static_get_extend (hb_color_সারি_t *color_সারি,
							  শূন্য *color_সারি_তথ্য,
							  শূন্য *user_তথ্য)
  {
    ধ্রুবক Color_সারি *thiz = (ধ্রুবক Color_সারি *) color_সারি_তথ্য;
    ফেরত thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
কাঠামো CompositeMode : HBUINT8
{
  সর্বজনীন:
  DEFINE_আকার_STATIC (1);
};

কাঠামো Affine2x3
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  { c->অঙ্ক_var_idxes = 6; }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((দ্বিগুণ) source.to_int () +
                                                    (দ্বিগুণ) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (সত্য);
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    c->funcs->ঠেলো_transform (c->তথ্য,
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (6 * F16DOT16::static_আকার);
};

কাঠামো PaintColrLayers
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    uint32_t first_layer_সূচক = অঙ্ক_Layers ? c->plan->colrv1_layers.get (first_Layer_সূচক) : 0;
    return_trace (c->serializer->check_assign (out->first_Layer_সূচক, first_layer_সূচক,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  অন্তঃরেখ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক;

  HBUINT8	format; /* format = 1 */
  HBUINT8	অঙ্ক_Layers;
  HBUINT32	first_Layer_সূচক;  /* index into COLRv1::layerList */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো PaintSolid
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক {}
  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    c->add_palette_সূচক (palette_সূচক_2);
    c->অঙ্ক_var_idxes = 1;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      দশমিক a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    যদি (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->palette_সূচক_2, c->plan->colr_palettes.get (palette_সূচক_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) ধ্রুবক
  {
    *color = c->get_color (palette_সূচক_2,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->তথ্য, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_সূচক_2;
  F2DOT14	alpha;
  সর্বজনীন:
  DEFINE_আকার_STATIC (3 + F2DOT14::static_আকার);
};

template <template<typename> শ্রেণী Var>
কাঠামো PaintLinearGradient
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    (এই+color_সারি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 6;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((দ্বিগুণ) x0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((দ্বিগুণ) y0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((দ্বিগুণ) x1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((দ্বিগুণ) y1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((দ্বিগুণ) x2 + (দ্বিগুণ) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((দ্বিগুণ) y2 + (দ্বিগুণ) roundf (instancer (varIdxBase, 5)));
    }

    যদি (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_সারি_2.serialize_subset (c, color_সারি_2, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && color_সারি_2.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    hb_color_সারি_t cl = {
      (শূন্য *) &(এই+color_সারি_2),
      (এই+color_সারি_2).static_get_color_stops, c,
      (এই+color_সারি_2).static_get_extend, nullptr
    };

    c->funcs->linear_gradient (c->তথ্য, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       x1 + c->instancer (varIdxBase, 2),
			       y1 + c->instancer (varIdxBase, 3),
			       x2 + c->instancer (varIdxBase, 4),
			       y2 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24To<Color_সারি<Var>>	color_সারি_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 6 * FWORD::static_আকার);
};

template <template<typename> শ্রেণী Var>
কাঠামো PaintRadialGradient
{
  যৌক্তিক constant_after_pinning (hb_subset_context_t *c,
                               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    যদি (c->plan->has_avar2) ফেরত মিথ্যা;
    ধ্রুবক auto &regions = instancer.varStore->get_region_list ();
    জন্য (unsigned i = 0; i < regions.region_গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      যদি (!regions.get_var_region (i, c->plan->axes_old_সূচক_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        ফেরত মিথ্যা;
      }
      জন্য (auto axis : axes)
      {
        Triple *limit;
        যদি (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          ফেরত মিথ্যা;
      }
    }
    ফেরত সত্য;
  }

  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    (এই+color_সারি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 6;
  }

  যৌক্তিক subset_nonnegative (hb_subset_context_t *c,
                           ধ্রুবক ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) ধ্রুবক
  {
    দ্বিগুণ source[6] = {
      x0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 0)),
      y0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)),
      radius0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)),
      x1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 3)),
      y1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 4)),
      radius1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_আকার<PaintSolid> (5);
      যদি (!out) ফেরত মিথ্যা;
      out->format = 2; out->palette_সূচক_2 = 0xFFFF; out->alpha.set_float (0);
      ফেরত সত্য;
    };
    ধ্রুবক auto &সারি = এই+color_সারি_2;
    যদি (source[2] == source[5] || !সারি.stops.len) ফেরত transparent ();
    কাঠামো stop_t { দ্বিগুণ offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    যদি (!stops.alloc (সারি.stops.len)) ফেরত মিথ্যা;
    unsigned order = 0;
    জন্য (ধ্রুবক auto &entry : সারি.stops.iter ())
    {
      ধ্রুবক auto &stop = entry.get_মান ();
      stops.push (stop_t {(দ্বিগুণ) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (দ্বিগুণ) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.palette_সূচক_2, order++});
    }
    যদি (stops.in_error ()) ফেরত মিথ্যা;
    stops.qsort ([] (ধ্রুবক stop_t &a, ধ্রুবক stop_t &b) {
      ফেরত a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    দ্বিগুণ dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    জন্য (unsigned j = 0; j < 3; j++)
    {
      দ্বিগুণ remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      যদি (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        ফেরত মিথ্যা;
      }
      unsigned ডান = (unsigned) remainder;
      যখন (ডান) { unsigned r = lattice % ডান; lattice = ডান; ডান = r; }
    }
    জন্য (unsigned shift = 0; shift < 16; shift++)
    {
      দ্বিগুণ b = 1u << shift;
      দ্বিগুণ coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      দ্বিগুণ fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      দ্বিগুণ candidates[] = {coarse, fine};
      জন্য (দ্বিগুণ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        দ্বিগুণ geometry[6];
        যৌক্তিক fits = সত্য;
        জন্য (unsigned j = 0; j < 3; j++)
        {
          দ্বিগুণ difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          দ্বিগুণ নিচের = j == 2 ? 0 : -32768, উপরের = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= নিচের && geometry[j] <= উপরের &&
                  geometry[j + 3] >= নিচের && geometry[j + 3] <= উপরের;
        }
        যদি (!fits) চালু;
        unsigned first = 0, গুনতি = stops.দৈর্ঘ্য;
        দ্বিগুণ min = (stops[0].offset - a) / b, max = (stops.লেজ ().offset - a) / b;
        দ্বিগুণ bias = 0;
        যৌক্তিক constant = মিথ্যা;
        যদি (সারি.extend == Extend::EXTEND_PAD)
        {
          দ্বিগুণ tip = -geometry[2] / (b * dr);
          যদি ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.দৈর্ঘ্য - 1 : 0;
            গুনতি = 1; constant = সত্য;
          }
        }
        অন্যথা
        {
          দ্বিগুণ period = max - min;
          যদি (সারি.extend == Extend::EXTEND_REFLECT) period *= 2;
          যদি (period <= 0) ফেরত transparent ();
          দ্বিগুণ n = ceil ((-2 - min) / period);
          যদি (n > floor ((32767. / 16384. - max) / period)) চালু;
          bias = n * period;
        }
        জন্য (unsigned j = 0; j < গুনতি; j++)
        {
          দ্বিগুণ bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        যদি (!fits) চালু;
        auto *out = c->serializer->allocate_আকার<PaintRadialGradient<NoVariable>> (16);
        যদি (!out) ফেরত মিথ্যা;
        out->format = 6;
        out->x0 = (পূর্ণসংখ্যা) geometry[0]; out->y0 = (পূর্ণসংখ্যা) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (পূর্ণসংখ্যা) geometry[3]; out->y1 = (পূর্ণসংখ্যা) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_সারি = c->serializer->allocate_আকার<Color_সারি<NoVariable>> (3 + গুনতি * ColorStop::static_আকার);
        যদি (!out_সারি) { c->serializer->বের_করো_discard (); ফেরত মিথ্যা; }
        out_সারি->extend = সারি.extend;
        out_সারি->stops.len = গুনতি;
        জন্য (unsigned j = 0; j < গুনতি; j++)
        {
          ধ্রুবক auto &stop = stops[first + j];
          auto &out_stop = out_সারি->stops[j].value;
          out_stop.stopOffset.set_float ((দশমিক) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((দশমিক) stop.alpha);
          যদি (!c->serializer->check_assign (out_stop.palette_সূচক_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->বের_করো_discard (); ফেরত মিথ্যা; }
        }
        c->serializer->add_link (out->color_সারি_2, c->serializer->বের_করো_pack ());
        ফেরত !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    ফেরত মিথ্যা;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((দ্বিগুণ) x0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((দ্বিগুণ) y0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((দ্বিগুণ) radius0 + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((দ্বিগুণ) x1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((দ্বিগুণ) y1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((দ্বিগুণ) radius1 + (দ্বিগুণ) roundf (instancer (varIdxBase, 5)));
    }

    যদি (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_সারি_2.serialize_subset (c, color_সারি_2, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && color_সারি_2.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    hb_color_সারি_t cl = {
      (শূন্য *) &(এই+color_সারি_2),
      (এই+color_সারি_2).static_get_color_stops, c,
      (এই+color_সারি_2).static_get_extend, nullptr
    };

    c->funcs->radial_gradient (c->তথ্য, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       radius0 + c->instancer (varIdxBase, 2),
			       x1 + c->instancer (varIdxBase, 3),
			       y1 + c->instancer (varIdxBase, 4),
			       radius1 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24To<Color_সারি<Var>>	color_সারি_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 6 * FWORD::static_আকার);
};

template <template<typename> শ্রেণী Var>
কাঠামো PaintSweepGradient
{
  শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক {}

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    (এই+color_সারি_2).closurev1 (c);
    c->অঙ্ক_var_idxes = 4;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((দ্বিগুণ) centerX + (দ্বিগুণ) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((দ্বিগুণ) centerY + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    যদি (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_সারি_2.serialize_subset (c, color_সারি_2, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && color_সারি_2.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    hb_color_সারি_t cl = {
      (শূন্য *) &(এই+color_সারি_2),
      (এই+color_সারি_2).static_get_color_stops, c,
      (এই+color_সারি_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->তথ্য, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_সারি<Var>>	color_সারি_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * FWORD::static_আকার + 2 * F2DOT14::static_আকার);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
কাঠামো PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    return_trace (out->paint.serialize_subset (c, paint, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && paint.sanitize (c, এই));
  }

  অন্তঃরেখ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + HBGlyphID::static_আকার);
};

কাঠামো PaintColrGlyph
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  অন্তঃরেখ শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  সর্বজনীন:
  DEFINE_আকার_STATIC (3);
};

template <template<typename> শ্রেণী Var>
কাঠামো PaintTransform
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    যদি (!out->transform.serialize_subset (c, transform, এই, instancer)) return_trace (মিথ্যা);
    যদি (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  src.sanitize (c, এই) &&
                  transform.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক
  {
    TRACE_PAINT (এই);
    (এই+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  সর্বজনীন:
  DEFINE_আকার_STATIC (7);
};

কাঠামো PaintTranslate
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((দ্বিগুণ) dx + (দ্বিগুণ) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((দ্বিগুণ) dy + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
    }

    যদি (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক ddx = dx + c->instancer (varIdxBase, 0);
    দশমিক ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ঠেলো_translate (c->তথ্য, ddx, ddy);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * FWORD::static_আকার);
};

template <typename Source>
স্থির যৌক্তিক serialize_scale_transform (hb_subset_context_t *c,
                                       দশমিক sx, দশমিক sy, দশমিক dx, দশমিক dy,
                                       ধ্রুবক Source &source)
{
  auto *out = c->serializer->allocate_আকার<PaintTransform<NoVariable>> (7);
  যদি (unlikely (!out)) ফেরত মিথ্যা;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_আকার<Affine2x3> (Affine2x3::static_আকার);
  যদি (unlikely (!transform))
  {
    c->serializer->বের_করো_discard ();
    ফেরত মিথ্যা;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->বের_করো_pack ());

  c->serializer->push ();
  যৌক্তিক ret = source ();
  যদি (ret)
    c->serializer->add_link (out->src, c->serializer->বের_করো_pack ());
  অন্যথা
    c->serializer->বের_করো_discard ();
  ফেরত ret;
}

template <typename Base>
স্থির যৌক্তিক subset_scale_as_transform (hb_subset_context_t *c,
                                      ধ্রুবক Base *base,
                                      ধ্রুবক Offset24To<Paint> &src,
                                      ধ্রুবক ItemVarStoreInstancer &instancer,
                                      দশমিক sx, দশমিক sy,
                                      দশমিক cx = 0, দশমিক cy = 0)
{
  যদি (src.is_null ()) ফেরত মিথ্যা;
  auto source = [&] { ফেরত c->dispatch (base+src, instancer); };
  দশমিক dx = cx * (1 - sx), dy = cy * (1 - sy);
  যদি (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    ফেরত serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    ফেরত serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    ফেরত serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    ফেরত serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    ফেরত serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  ফেরত serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

কাঠামো PaintScale
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      দশমিক sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      দশমিক sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      যদি (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, এই, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    যদি (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    দশমিক sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ঠেলো_scale (c->তথ্য, sx, sy);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * F2DOT14::static_আকার);
};

কাঠামো PaintScaleAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      দশমিক sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      দশমিক sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      যদি (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, এই, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((দ্বিগুণ) centerX + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((দ্বিগুণ) centerY + (দ্বিগুণ) roundf (instancer (varIdxBase, 3)));
    }

    যদি (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    দশমিক sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    দশমিক tCenterX = centerX + c->instancer (varIdxBase, 2);
    দশমিক tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ঠেলো_scale_around_center (c->তথ্য, sx, sy, tCenterX, tCenterY);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * F2DOT14::static_আকার + 2 * FWORD::static_আকার);
};

কাঠামো PaintScaleUniform
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      দশমিক s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      যদি (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, এই, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    যদি (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ঠেলো_scale (c->তথ্য, s, s);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + F2DOT14::static_আকার);
};

কাঠামো PaintScaleUniformAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      দশমিক s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      যদি (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, এই, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((দ্বিগুণ) centerX + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((দ্বিগুণ) centerY + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
    }

    যদি (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক s = scale.to_float (c->instancer (varIdxBase, 0));
    দশমিক tCenterX = centerX + c->instancer (varIdxBase, 1);
    দশমিক tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ঠেলো_scale_around_center (c->তথ্য, s, s, tCenterX, tCenterY);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + F2DOT14::static_আকার + 2 * FWORD::static_আকার);
};

কাঠামো PaintRotate
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    যদি (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ঠেলো_rotate (c->তথ্য, a);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + F2DOT14::static_আকার);
};

কাঠামো PaintRotateAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((দ্বিগুণ) centerX + (দ্বিগুণ) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((দ্বিগুণ) centerY + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
    }

    যদি (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক a = angle.to_float (c->instancer (varIdxBase, 0));
    দশমিক tCenterX = centerX + c->instancer (varIdxBase, 1);
    দশমিক tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ঠেলো_rotate_around_center (c->তথ্য, a, tCenterX, tCenterY);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + F2DOT14::static_আকার + 2 * FWORD::static_আকার);
};

কাঠামো PaintSkew
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    যদি (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    দশমিক sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ঠেলো_skew (c->তথ্য, sx, sy);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * F2DOT14::static_আকার);
};

কাঠামো PaintSkewAroundCenter
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((দ্বিগুণ) centerX + (দ্বিগুণ) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((দ্বিগুণ) centerY + (দ্বিগুণ) roundf (instancer (varIdxBase, 3)));
    }

    যদি (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, এই, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && src.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_PAINT (এই);
    দশমিক sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    দশমিক sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    দশমিক tCenterX = centerX + c->instancer (varIdxBase, 2);
    দশমিক tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ঠেলো_skew_around_center (c->তথ্য, sx, sy, tCenterX, tCenterY);
    c->recurse (এই+src);
    c->funcs->বের_করো_transform (c->তথ্য);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 2 * F2DOT14::static_আকার + 2 * FWORD::static_আকার);
};

কাঠামো PaintComposite
{
  HB_INTERNAL শূন্য dependv1 (hb_colrv1_depend_context_t* c) ধ্রুবক;
  HB_INTERNAL শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক;

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যৌক্তিক ret = মিথ্যা;
    ret |= out->src.serialize_subset (c, src, এই, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, এই, instancer);
    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  c->check_ops (এই->min_আকার) && // PainComposite can get exponential
                  src.sanitize (c, এই) &&
                  backdrop.sanitize (c, এই));
  }

  শূন্য paint_glyph (hb_paint_context_t *c) ধ্রুবক
  {
    TRACE_PAINT (এই);
    hb_paint_composite_mode_t composite_mode;
    নির্বাচন ((unsigned পূর্ণসংখ্যা) mode)
    {
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_CLEAR:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_IN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_XOR:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_PLUS:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SCREEN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_OVERLAY:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DARKEN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned পূর্ণসংখ্যা) mode;
	বিরতি;
      পূর্বনির্ধারিত:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	বিরতি;
    }

    c->funcs->ঠেলো_group_for (c->তথ্য, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (এই+backdrop);
    c->funcs->ঠেলো_group_for (c->তথ্য, composite_mode);
    c->recurse (এই+src);
    c->funcs->বের_করো_group (c->তথ্য, composite_mode);
    c->funcs->বের_করো_group (c->তথ্য, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো Clip_Box_তথ্য
{
  পূর্ণসংখ্যা xMin, yMin, xMax, yMax;
};

কাঠামো ClipBoxFormat1
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_clip_box (Clip_Box_তথ্য &clip_box, ধ্রুবক ItemVarStoreInstancer &instancer HB_UNUSED) ধ্রুবক
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((দ্বিগুণ) xMin + (দ্বিগুণ) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((দ্বিগুণ) yMin + (দ্বিগুণ) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((দ্বিগুণ) xMax + (দ্বিগুণ) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((দ্বিগুণ) yMax + (দ্বিগুণ) ceilf (instancer (varIdxBase, 3)));
    }

    যদি (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (সত্য);
  }

  সর্বজনীন:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  সর্বজনীন:
  DEFINE_আকার_STATIC (1 + 4 * FWORD::static_আকার);
};

কাঠামো ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  শূন্য get_clip_box (Clip_Box_তথ্য &clip_box, ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    value.get_clip_box(clip_box, instancer);
    যদি (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<পূর্ণসংখ্যা> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<পূর্ণসংখ্যা> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<পূর্ণসংখ্যা> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<পূর্ণসংখ্যা> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

কাঠামো ClipBox
{
  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 2: hb_barrier (); u.format2.closurev1 (c); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক get_extents (hb_glyph_extents_t *extents,
                    ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    Clip_Box_তথ্য clip_box;
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      বিরতি;
    ক্ষেত্র 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      বিরতি;
    পূর্বনির্ধারিত:
      ফেরত মিথ্যা;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    ফেরত সত্য;
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

কাঠামো ClipRecord
{
  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  শূন্য closurev1 (hb_colrv1_closure_context_t* c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি (!c->glyphs->intersects (startGlyphID, endGlyphID)) ফেরত;
    (base+clipBox).closurev1 (c);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক শূন্য *base,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && clipBox.sanitize (c, base));
  }

  যৌক্তিক get_extents (hb_glyph_extents_t *extents,
		    ধ্রুবক শূন্য *base,
		    ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    ফেরত (base+clipBox).get_extents (extents, instancer);
  }

  সর্বজনীন:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  সর্বজনীন:
  DEFINE_আকার_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

কাঠামো ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   ধ্রুবক ItemVarStoreInstancer &instancer,
                                   ধ্রুবক hb_set_t& gids,
                                   ধ্রুবক hb_map_t& gid_offset_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    যদি (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned গুনতি  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t পূর্ব_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned পূর্ব_offset = offset;
    জন্য (ধ্রুবক hb_codepoint_t _ : gids.iter ())
    {
      যদি (_ == start_gid) চালু;

      offset = gid_offset_map.get (_);
      যদি (_ == পূর্ব_gid + 1 &&  offset == পূর্ব_offset)
      {
        পূর্ব_gid = _;
        চালু;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = পূর্ব_gid;
      record.clipBox = পূর্ব_offset;

      যদি (!record.subset (c, এই, instancer)) return_trace (0);
      গুনতি++;

      start_gid = _;
      পূর্ব_gid = _;
      পূর্ব_offset = offset;
    }

    //last one
    {
      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = পূর্ব_gid;
      record.clipBox = পূর্ব_offset;
      যদি (!record.subset (c, এই, instancer)) return_trace (0);
      গুনতি++;
    }
    return_trace (গুনতি);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    যদি (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিথ্যা);

    ধ্রুবক hb_set_t& glyphset = c->plan->_glyphset_colred;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    জন্য (ধ্রুবক ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      জন্য (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        যদি (!glyphset.has (gid) || !glyph_map.has (gid)) চালু;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned গুনতি = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    যদি (!গুনতি) return_trace (মিথ্যা);
    return_trace (c->serializer->check_assign (out->clips.len, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (এই) && clips.sanitize (c, এই));
  }

  যৌক্তিক
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    auto *rec = clips.as_array ().bsearch (gid);
    যদি (rec)
      ফেরত rec->get_extents (extents, এই, instancer);
    ফেরত মিথ্যা;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (5, clips);
};

কাঠামো Paint
{

  template <typename ...Ts>
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_মান ());

    return_trace (c->end_recursion (এই->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    ক্ষেত্র 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    ক্ষেত্র 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    ক্ষেত্র 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    ক্ষেত্র 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    ক্ষেত্র 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    ক্ষেত্র 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    ক্ষেত্র 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    ক্ষেত্র 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    ক্ষেত্র 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    ক্ষেত্র 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    ক্ষেত্র 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    ক্ষেত্র 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    ক্ষেত্র 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    ক্ষেত্র 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    ক্ষেত্র 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    ক্ষেত্র 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    ক্ষেত্র 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    ক্ষেত্র 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    ক্ষেত্র 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    ক্ষেত্র 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    ক্ষেত্র 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    ক্ষেত্র 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    ক্ষেত্র 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    ক্ষেত্র 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    ক্ষেত্র 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    ক্ষেত্র 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    ক্ষেত্র 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    ক্ষেত্র 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); ফেরত সত্য;
    ক্ষেত্র 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); ফেরত সত্য;
    পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT8 v; }				format;
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
  সর্বজনীন:
  DEFINE_আকার_MIN (2);
};

কাঠামো BaseGlyphPaintRecord
{
  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  যৌক্তিক serialize (hb_serialize_context_t *s, ধ্রুবক hb_map_t* glyph_map,
                  ধ্রুবক শূন্য* src_base, hb_subset_context_t *c,
                  ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = s->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    যদি (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) && paint.sanitize (c, base)));
  }

  সর্বজনীন:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিথ্যা);
    ধ্রুবক hb_set_t* glyphset = &c->plan->_glyphset_colred;

    জন্য (ধ্রুবক auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      যদি (!glyphset->has (gid)) চালু;

      যদি (_.serialize (c->serializer, c->plan->glyph_map, এই, c, instancer)) out->len++;
      অন্যথা return_trace (মিথ্যা);
    }

    return_trace (out->len != 0);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, এই));
  }
};

কাঠামো LayerList : Array32OfOffset32To<Paint>
{
  ধ্রুবক Paint& get_paint (unsigned i) ধ্রুবক
  { ফেরত এই+(*এই)[i]; }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক ItemVarStoreInstancer &instancer) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিথ্যা);

    যৌক্তিক ret = মিথ্যা;
    জন্য (ধ্রুবক auto& _ : + hb_enumerate (*এই)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (মিথ্যা);
      ret |= o->serialize_subset (c, _.second, এই, instancer);
    }
    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, এই));
  }
};

কাঠামো delta_set_সূচক_map_subset_plan_t
{
  unsigned get_inner_bit_গুনতি () ধ্রুবক { ফেরত inner_bit_গুনতি; }
  unsigned get_width ()           ধ্রুবক { ফেরত ((outer_bit_গুনতি + inner_bit_গুনতি + 7) / 8); }
  hb_array_t<ধ্রুবক uint32_t> get_output_map () ধ্রুবক { ফেরত output_map.as_array (); }

  delta_set_সূচক_map_subset_plan_t (ধ্রুবক hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_গুনতি = 0;
    outer_bit_গুনতি = 0;
    inner_bit_গুনতি = 1;
    output_map.init ();

    /* search backwards */
    unsigned গুনতি = new_deltaset_idx_varidx_map.get_population ();
    যদি (!গুনতি) ফেরত;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    জন্য (unsigned i = গুনতি; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      যদি (i == গুনতি)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        চালু;
      }
      যদি (var_idx != last_varidx)
        বিরতি;
      last_idx = delta_set_idx;
    }

    map_গুনতি = last_idx + 1;
  }

  যৌক্তিক remap (ধ্রুবক hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    যদি (unlikely (!output_map.resize_dirty (map_গুনতি))) ফেরত মিথ্যা;

    জন্য (unsigned idx = 0; idx < map_গুনতি; idx++)
    {
      uint32_t *var_idx;
      যদি (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) ফেরত মিথ্যা;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_গুনতি = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_গুনতি = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_গুনতি = hb_max (bit_গুনতি, inner_bit_গুনতি);
    }
    ফেরত সত্য;
  }

  ব্যক্তিগত:
  unsigned map_গুনতি;
  unsigned outer_bit_গুনতি;
  unsigned inner_bit_গুনতি;
  hb_vector_t<uint32_t> output_map;
};

কাঠামো COLR
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত has_v0_তথ্য () || version; }

  যৌক্তিক has_v0_তথ্য () ধ্রুবক { ফেরত অঙ্ক_Base_Glyphs; }
  যৌক্তিক has_v1_তথ্য () ধ্রুবক
  {
    যদি (version < 1)
      ফেরত মিথ্যা;
    hb_barrier ();

    ফেরত (এই+baseGlyphList).len > 0;
  }

  unsigned পূর্ণসংখ্যা get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned পূর্ণসংখ্যা         start_offset,
				 unsigned পূর্ণসংখ্যা        *গুনতি, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ধ্রুবক
  {
    ধ্রুবক BaseGlyphRecord &record = (এই+baseGlyphsZ).bsearch (অঙ্ক_Base_Glyphs, glyph);

    hb_array_t<ধ্রুবক LayerRecord> all_layers = (এই+layersZ).as_array (অঙ্ক_Layers);
    hb_array_t<ধ্রুবক LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.অঙ্ক_Layers);
    যদি (গুনতি && layers)
    {
      + glyph_layers.sub_array (start_offset, গুনতি)
      | hb_sink (hb_array (layers, *গুনতি))
      ;
    }
    ফেরত glyph_layers.দৈর্ঘ্য;
  }

  কাঠামো accelerator_t
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


    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত colr->has_তথ্য (); }

#ifndef HB_NO_PAINT
    যৌক্তিক
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ধ্রুবক
    {
      যদি (unlikely (!has_তথ্য ())) ফেরত মিথ্যা;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ফেরত সত্য;
      যৌক্তিক ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      ফেরত ret;
    }

    যৌক্তিক paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, শূন্য *তথ্য,
		      unsigned পূর্ণসংখ্যা palette_সূচক,
		      hb_color_t foreground,
		      যৌক্তিক clip = সত্য) ধ্রুবক
    {
      যদি (unlikely (!has_তথ্য ())) ফেরত মিথ্যা;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      যদি (unlikely (!scratch)) ফেরত সত্য;
      যৌক্তিক ret = colr->paint_glyph (font, glyph, funcs, তথ্য, palette_সূচক, foreground, clip, *scratch);
      release_scratch (scratch);
      ফেরত ret;
    }
#endif

    যৌক্তিক is_valid () { ফেরত colr.get_blob ()->দৈর্ঘ্য; }

    শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
    {
      যদি (!has_তথ্য ()) ফেরত;
      colr->depend (depend_তথ্য);
    }

    শূন্য closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) ধ্রুবক
    { colr->closure_glyphs (glyph, related_ids); }

    শূন্য closure_V0palette_indices (ধ্রুবক hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) ধ্রুবক
    { colr->closure_V0palette_indices (glyphs, palettes); }

    শূন্য closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) ধ্রুবক
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    যৌক্তিক has_var_store () ধ্রুবক
    { ফেরত colr->has_var_store (); }

    ধ্রুবক ItemVariationStore &get_var_store () ধ্রুবক
    { ফেরত colr->get_var_store (); }
    ধ্রুবক ItemVariationStore *get_var_store_ptr () ধ্রুবক
    { ফেরত colr->get_var_store_ptr (); }

    যৌক্তিক has_delta_set_সূচক_map () ধ্রুবক
    { ফেরত colr->has_delta_set_সূচক_map (); }

    ধ্রুবক Delta_Set_সূচক_Map &get_delta_set_সূচক_map () ধ্রুবক
    { ফেরত colr->get_delta_set_সূচক_map (); }
    ধ্রুবক Delta_Set_সূচক_Map *get_delta_set_সূচক_map_ptr () ধ্রুবক
    { ফেরত colr->get_delta_set_সূচক_map_ptr (); }

    ব্যক্তিগত:

    hb_colr_scratch_t *acquire_scratch () ধ্রুবক
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      যদি (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, আকার (hb_colr_scratch_t));
	যদি (unlikely (!scratch))
	  ফেরত nullptr;
      }

      ফেরত scratch;
    }
    শূন্য release_scratch (hb_colr_scratch_t *scratch) ধ্রুবক
    {
      যদি (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    সর্বজনীন:
    hb_blob_ptr_t<COLR> colr;
    ব্যক্তিগত:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
  {
    // v0
    hb_array_t<ধ্রুবক BaseGlyphRecord> baseGlyphs = (এই+baseGlyphsZ).as_array (অঙ্ক_Base_Glyphs);
    জন্য (ধ্রুবক BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (এই+layersZ).as_array (অঙ্ক_Layers).sub_array (record.firstLayerIdx,
                                                                         record.অঙ্ক_Layers);
      জন্য (ধ্রুবক LayerRecord layer : glyph_layers)
        depend_তথ্য->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    যদি (!has_v1_তথ্য())
      ফেরত;

    // v1
    hb_colrv1_depend_context_t c (এই, depend_তথ্য);
    ধ্রুবক BaseGlyphList &baseglyph_paintrecords = এই+baseGlyphList;

    জন্য (ধ্রুবক BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      ধ্রুবক Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  শূন্য closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) ধ্রুবক
  {
    ধ্রুবক BaseGlyphRecord *record = get_base_glyph_record (glyph);
    যদি (!record) ফেরত;

    auto glyph_layers = (এই+layersZ).as_array (অঙ্ক_Layers).sub_array (record->firstLayerIdx,
								       record->অঙ্ক_Layers);
    যদি (!glyph_layers.দৈর্ঘ্য) ফেরত;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.দৈর্ঘ্য, LayerRecord::min_আকার);
  }

  শূন্য closure_V0palette_indices (ধ্রুবক hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) ধ্রুবক
  {
    যদি (!অঙ্ক_Base_Glyphs || !অঙ্ক_Layers) ফেরত;
    hb_array_t<ধ্রুবক BaseGlyphRecord> baseGlyphs = (এই+baseGlyphsZ).as_array (অঙ্ক_Base_Glyphs);
    hb_array_t<ধ্রুবক LayerRecord> all_layers = (এই+layersZ).as_array (অঙ্ক_Layers);

    জন্য (ধ্রুবক BaseGlyphRecord record : baseGlyphs)
    {
      যদি (!glyphs->has (record.glyphId)) চালু;
      hb_array_t<ধ্রুবক LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.অঙ্ক_Layers);
      জন্য (ধ্রুবক LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  শূন্য closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) ধ্রুবক
  {
    যদি (version < 1) ফেরত;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (এই, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    ধ্রুবক BaseGlyphList &baseglyph_paintrecords = এই+baseGlyphList;

    জন্য (ধ্রুবক BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      যদি (!glyphset->has (gid)) চালু;

      ধ্রুবক Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    ধ্রুবক ClipList &cliplist = এই+clipList;
    c.glyphs = glyphset;
    জন্য (ধ্রুবক ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    যদি (has_delta_set_সূচক_map ())
    {
      ধ্রুবক Delta_Set_সূচক_Map &var_idx_map = এই+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      জন্য (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  ধ্রুবক LayerList& get_layerList () ধ্রুবক
  { ফেরত (এই+layerList); }

  ধ্রুবক BaseGlyphList& get_baseglyphList () ধ্রুবক
  { ফেরত (এই+baseGlyphList); }

  যৌক্তিক has_var_store () ধ্রুবক
  { ফেরত version >= 1 && hb_barrier () && varStore != 0; }

  যৌক্তিক has_delta_set_সূচক_map () ধ্রুবক
  { ফেরত version >= 1 && hb_barrier () && varIdxMap != 0; }

  যৌক্তিক has_clip_list () ধ্রুবক
  { ফেরত version >= 1 && hb_barrier () && clipList != 0; }

  ধ্রুবক Delta_Set_সূচক_Map &get_delta_set_সূচক_map () ধ্রুবক
  { ফেরত has_delta_set_সূচক_map () && hb_barrier () ? এই+varIdxMap : Null (Delta_Set_সূচক_Map); }
  ধ্রুবক Delta_Set_সূচক_Map *get_delta_set_সূচক_map_ptr () ধ্রুবক
  { ফেরত has_delta_set_সূচক_map () && hb_barrier () ? &(এই+varIdxMap) : nullptr; }

  ধ্রুবক ItemVariationStore &get_var_store () ধ্রুবক
  { ফেরত has_var_store () && hb_barrier () ? এই+varStore : Null (ItemVariationStore); }
  ধ্রুবক ItemVariationStore *get_var_store_ptr () ধ্রুবক
  { ফেরত has_var_store () && hb_barrier () ? &(এই+varStore) : nullptr; }

  ধ্রুবক ClipList &get_clip_list () ধ্রুবক
  { ফেরত has_clip_list () && hb_barrier () ? এই+clipList : Null (ClipList); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
                  (এই+baseGlyphsZ).sanitize (c, অঙ্ক_Base_Glyphs) &&
                  (এই+layersZ).sanitize (c, অঙ্ক_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, এই) &&
		    layerList.sanitize (c, এই) &&
		    clipList.sanitize (c, এই) &&
		    varIdxMap.sanitize (c, এই) &&
		    varStore.sanitize (c, এই))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  যৌক্তিক serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (base_it.len () != layer_it.len ()))
      return_trace (মিথ্যা);

    এই->version = version;
    অঙ্ক_Layers = 0;
    অঙ্ক_Base_Glyphs = base_it.len ();
    যদি (অঙ্ক_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (সত্য);
    }

    c->push ();
    জন্য (ধ্রুবক hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      যদি (unlikely (!record)) return_trace (মিথ্যা);
      record->firstLayerIdx = অঙ্ক_Layers;
      অঙ্ক_Layers += record->অঙ্ক_Layers;
    }
    c->add_link (baseGlyphsZ, c->বের_করো_pack ());

    c->push ();
    জন্য (ধ্রুবক hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->বের_করো_pack ());

    return_trace (সত্য);
  }

  ধ্রুবক BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) ধ্রুবক
  {
    ধ্রুবক BaseGlyphRecord* record = &(এই+baseGlyphsZ).bsearch (অঙ্ক_Base_Glyphs, (unsigned পূর্ণসংখ্যা) gid);
    যদি (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    ফেরত record;
  }

  ধ্রুবক BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) ধ্রুবক
  {
    ধ্রুবক BaseGlyphList &list = এই+baseGlyphList;
    unsigned পূর্ণসংখ্যা i;
    যদি (!list.bfind ((unsigned) gid, &i))
      ফেরত nullptr;
    ফেরত &list[i];
  }

  যৌক্তিক downgrade_to_V0 (ধ্রুবক hb_set_t &glyphset) ধ্রুবক
  {
    //no more COLRv1 glyphs, downgrade to version 0
    জন্য (ধ্রুবক BaseGlyphPaintRecord& _ : get_baseglyphList ())
      যদি (glyphset.has (_.glyphId))
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (সত্য);

    ধ্রুবক ItemVariationStore& var_store = এই+varStore;
    যদি (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      যৌক্তিক optimize = (varIdxMap != 0) ? সত্য : মিথ্যা;
      যদি (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (মিথ্যা);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      যদি (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (মিথ্যা);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      যদি (optimize)
      {
        ধ্রুবক hb_map_t &varidx_map = item_vars.get_varidx_map ();
        জন্য (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
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
        var_store.collect_dead_regions (c->plan->axes_old_সূচক_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      যদি (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  যৌক্তিক subset_delta_set_সূচক_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (সত্য);

    ধ্রুবক hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_সূচক_map_subset_plan_t সূচক_map_plan (deltaset_idx_varidx_map);

    যদি (unlikely (!সূচক_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (মিথ্যা);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, সূচক_map_plan));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    ধ্রুবক hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->অঙ্ক_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    যদি (glyphset.has (old_gid)) ফেরত সত্য;
		    ফেরত মিথ্যা;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				ধ্রুবক BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				যদি (unlikely (!old_record))
				  ফেরত hb_pair_t<যৌক্তিক, BaseGlyphRecord> (মিথ্যা, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.অঙ্ক_Layers = old_record->অঙ্ক_Layers;
				ফেরত hb_pair_t<যৌক্তিক, BaseGlyphRecord> (সত্য, new_record);
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
				ধ্রুবক BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				যদি (unlikely (!old_record ||
					      old_record->firstLayerIdx >= অঙ্ক_Layers ||
					      old_record->firstLayerIdx + old_record->অঙ্ক_Layers > অঙ্ক_Layers))
				  ফেরত hb_pair_t<যৌক্তিক, hb_vector_t<LayerRecord>> (মিথ্যা, out_layers);

				auto layers = (এই+layersZ).as_array (অঙ্ক_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->অঙ্ক_Layers);
				out_layers.resize (layers.দৈর্ঘ্য);
				জন্য (unsigned পূর্ণসংখ্যা i = 0; i < layers.দৈর্ঘ্য; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  যদি (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    ফেরত hb_pair_t<যৌক্তিক, hb_vector_t<LayerRecord>> (মিথ্যা, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				ফেরত hb_pair_t<যৌক্তিক, hb_vector_t<LayerRecord>> (সত্য, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    যদি (version == 0 && (!base_it || !layer_it))
      return_trace (মিথ্যা);

    auto *colr_মৌলিক = c->serializer->start_embed<COLR> ();
    যদি (unlikely (!c->serializer->extend_min (colr_মৌলিক)))  return_trace (মিথ্যা);

    যদি (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_মৌলিক->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    যদি (!c->serializer->allocate_আকার<শূন্য> (5 * HBUINT32::static_আকার)) return_trace (মিথ্যা);
    যদি (!colr_মৌলিক->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (মিথ্যা);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    যদি (!subset_varstore (c, colr_মৌলিক)) return_trace (মিথ্যা);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_সূচক_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    যদি (!colr_মৌলিক->baseGlyphList.serialize_subset (c, baseGlyphList, এই, instancer))
      return_trace (মিথ্যা);

    colr_মৌলিক->layerList.serialize_subset (c, layerList, এই, instancer);
    colr_মৌলিক->clipList.serialize_subset (c, clipList, এই, instancer);

    return_trace (subset_delta_set_সূচক_map (c, colr_মৌলিক));
  }

  ধ্রুবক Paint *get_base_glyph_paint (hb_codepoint_t glyph) ধ্রুবক
  {
    ধ্রুবক BaseGlyphList &baseglyph_paintrecords = এই+baseGlyphList;
    ধ্রুবক BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    যদি (record)
    {
      ধ্রুবক Paint &paint = &baseglyph_paintrecords+record->paint;
      ফেরত &paint;
    }
    অন্যথা
      ফেরত nullptr;
  }

#ifndef HB_NO_PAINT
  যৌক্তিক
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ধ্রুবক
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_সূচক_map_ptr (),
                                     hb_array (font->coords, font->অঙ্ক_coords));

    যদি (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      ফেরত সত্য;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    যৌক্তিক ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), সত্য, scratch);

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

    ফেরত ret;
  }
#endif

  যৌক্তিক
  has_paint_for_glyph (hb_codepoint_t glyph) ধ্রুবক
  {
    যদি (version >= 1)
    {
      hb_barrier ();

      ধ্রুবক Paint *paint = get_base_glyph_paint (glyph);

      ফেরত paint != nullptr;
    }

    ফেরত মিথ্যা;
  }

  যৌক্তিক get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ধ্রুবক ItemVarStoreInstancer instancer) ধ্রুবক
  {
    ফেরত get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  যৌক্তিক
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, শূন্য *তথ্য,
	       unsigned পূর্ণসংখ্যা palette_সূচক, hb_color_t foreground,
	       যৌক্তিক clip,
	       hb_colr_scratch_t &scratch) ধ্রুবক
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_সূচক_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->অঙ্ক_coords : 0));
    hb_paint_context_t c (এই, funcs, তথ্য, font, palette_সূচক, foreground, instancer);

    hb_decycler_গ্রন্থি_t node (c.glyphs_decycler);
    node.visit (glyph);

    যদি (version >= 1)
    {
      hb_barrier ();

      ধ্রুবক Paint *paint = get_base_glyph_paint (glyph);
      যদি (paint)
      {
        // COLRv1 glyph

	যৌক্তিক is_bounded = সত্য;
	hb_glyph_extents_t extents = {};
	যদি (clip)
	{
	  যদি (!get_clip (glyph, &extents, instancer))
	  {
	    clip = মিথ্যা;
	    is_bounded = মিথ্যা;
	  }

	  যদি (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_সূচক, foreground,
			 মিথ্যা,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ঠেলো_font_transform (c.তথ্য, font);

	যদি (clip)
	  c.funcs->ঠেলো_clip_rectangle (c.তথ্য,
					(দশমিক) extents.x_bearing,
					(দশমিক) extents.y_bearing + extents.height,
					(দশমিক) extents.x_bearing + extents.width,
					(দশমিক) extents.y_bearing);

	যদি (is_bounded)
	  c.recurse (*paint);

	যদি (clip)
	  c.funcs->বের_করো_clip (c.তথ্য);

	c.funcs->বের_করো_transform (c.তথ্য);

        ফেরত সত্য;
      }
    }

    ধ্রুবক BaseGlyphRecord *record = get_base_glyph_record (glyph);
    যদি (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      জন্য (ধ্রুবক auto &r : (এই+layersZ).as_array (অঙ্ক_Layers)
			   .sub_array (record->firstLayerIdx, record->অঙ্ক_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.তথ্য, r.glyphId, c.font, is_foreground, color);
      }

      ফেরত সত্য;
    }

    ফেরত মিথ্যা;
  }
#endif

  সুরক্ষিত:
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
  Offset32To<Delta_Set_সূচক_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  সর্বজনীন:
  DEFINE_আকার_MIN (14);
};

কাঠামো COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

শূন্য
hb_paint_context_t::recurse (ধ্রুবক Paint &paint)
{
  যদি (unlikely (depth_বাম <= 0 || edge_গুনতি <= 0)) ফেরত;
  depth_বাম--;
  edge_গুনতি--;
  paint.dispatch (এই);
  depth_বাম++;
}

template <typename Types>
শূন্য PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ধ্রুবক
{
  TRACE_PAINT (এই);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  যদি (likely (c->depth_বাম > 0 && c->edge_গুনতি > 0) &&
      (এই+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_গুনতি--;
    c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
    c->funcs->fill_glyph (c->তথ্য, gid, c->font, is_foreground, color);
    c->funcs->বের_করো_transform (c->তথ্য);
    ফেরত;
  }

  c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
  c->funcs->ঠেলো_clip_glyph (c->তথ্য, gid, c->font);
  c->funcs->ঠেলো_font_transform (c->তথ্য, c->font);
  c->recurse (এই+paint);
  c->funcs->বের_করো_transform (c->তথ্য);
  c->funcs->বের_করো_clip (c->তথ্য);
  c->funcs->বের_করো_transform (c->তথ্য);
}

শূন্য PaintColrLayers::paint_glyph (hb_paint_context_t *c) ধ্রুবক
{
  TRACE_PAINT (এই);
  ধ্রুবক LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_গ্রন্থি_t node (c->layers_decycler);
  জন্য (unsigned i = first_Layer_সূচক; i < first_Layer_সূচক + অঙ্ক_Layers; i++)
  {
    যদি (unlikely (!node.visit (i)))
      ফেরত;

    ধ্রুবক Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

শূন্য PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ধ্রুবক
{
  TRACE_PAINT (এই);

  hb_decycler_গ্রন্থি_t node (c->glyphs_decycler);
  যদি (unlikely (!node.visit (gid)))
    ফেরত;

  c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
  যদি (c->funcs->color_glyph (c->তথ্য, gid, c->font))
  {
    c->funcs->বের_করো_transform (c->তথ্য);
    ফেরত;
  }
  c->funcs->বের_করো_transform (c->তথ্য);

  ধ্রুবক COLR *colr_table = c->get_colr_table ();
  ধ্রুবক Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  যৌক্তিক has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  যদি (has_clip_box)
    c->funcs->ঠেলো_clip_rectangle (c->তথ্য,
				   (দশমিক) extents.x_bearing,
				   (দশমিক) extents.y_bearing + extents.height,
				   (দশমিক) extents.x_bearing + extents.width,
				   (দশমিক) extents.y_bearing);

  যদি (paint)
    c->recurse (*paint);

  যদি (has_clip_box)
    c->funcs->বের_করো_clip (c->তথ্য);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
