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

#அந்தர்பாவ "../../../hb.hh"
#அந்தர்பாவ "../../../hb-decycler.hh"
#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "../../../hb-ot-var-common.hh"
#அந்தர்பாவ "../../../hb-paint.hh"
#அந்தர்பாவ "../../../hb-paint-bounded.hh"
#அந்தர்பாவ "../../../hb-paint-extents.hh"
#அந்தர்பாவ "../../../hb-depend-data.hh"

#அந்தர்பாவ "../../../hb-ot-dual.hh"
#அந்தர்பாவ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#define HB_OT_TAG_COLR HB_TAG('C','O','L','R')

நாமாகாஶ OT {
கட்டமைப்பு hb_paint_context_t;
}

கட்டமைப்பு hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

நாமாகாஶ OT {

கட்டமைப்பு COLR;

கட்டமைப்பு Paint;

கட்டமைப்பு hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  மாறிலி எழுத்து *get_name () { திரும்பு "PAINT"; }
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.paint_glyph (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  மாறிலி COLR* get_colr_table () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி COLR *> (base); }

பொது:
  மாறிலி வெறுமை *base;
  hb_paint_funcs_t *funcs;
  வெறுமை *data;
  hb_font_t *font;
  hb_array_t<மாறிலி BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  முழு depth_இடது = HB_MAX_NESTING_LEVEL;
  முழு edge_மொத்தம் = HB_MAX_GRAPH_EDGE_மொத்தம்;

  hb_paint_context_t (மாறிலி வெறுமை *base_,
		      hb_paint_funcs_t *funcs_,
		      வெறுமை *data_,
                      hb_font_t *font_,
                      unsigned முழு palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_மொத்தம் () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    எனில் (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, உண்மை);
      hb_font_set_synthetic_slant (font, 0);
    }
    இல்லையெனில்
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  நிலையான unsigned
  color_alpha (hb_color_t color, தசம alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    திரும்பு roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (unsigned முழு color_index, தசம alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = உண்மை;

    எனில் (color_index != 0xffff)
    {
      எனில் (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = பொய்;
    }

    திரும்பு HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  உள்வரி வெறுமை recurse (மாறிலி Paint &paint);
};

கட்டமைப்பு hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj)
  {
    எனில் (unlikely (nesting_level_இடது == 0))
      திரும்பு hb_empty_t ();

    எனில் (paint_visited (&obj))
      திரும்பு hb_empty_t ();

    nesting_level_இடது--;
    obj.closurev1 (இது);
    nesting_level_இடது++;
    திரும்பு hb_empty_t ();
  }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  தருக்க paint_visited (மாறிலி வெறுமை *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    எனில் (visited_paint.in_error() || visited_paint.has (delta))
      திரும்பு உண்மை;

    visited_paint.add (delta);
    திரும்பு பொய்;
  }

  மாறிலி COLR* get_colr_table () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி COLR *> (base); }

  வெறுமை add_glyph (unsigned glyph_id)
  { glyphs->add (glyph_id); }

  வெறுமை add_layer_indices (unsigned first_layer_index, unsigned இலக்கம்_of_layers)
  {
    எனில் (இலக்கம்_of_layers == 0) திரும்பு;
    layer_indices->add_range (first_layer_index, first_layer_index + இலக்கம்_of_layers - 1);
  }

  வெறுமை add_palette_index (unsigned palette_index)
  { palette_indices->add (palette_index); }

  வெறுமை add_var_idxes (unsigned first_var_idx, unsigned இலக்கம்_idxes)
  {
    எனில் (!இலக்கம்_idxes || first_var_idx == VarIdx::NO_VARIATION) திரும்பு;
    variation_indices->add_range (first_var_idx, first_var_idx + இலக்கம்_idxes - 1);
  }

  பொது:
  மாறிலி வெறுமை *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  unsigned இலக்கம்_var_idxes;
  unsigned nesting_level_இடது;

  hb_colrv1_closure_context_t (மாறிலி வெறுமை *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               unsigned இலக்கம்_var_idxes_2 = 1,
                               unsigned nesting_level_இடது_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          இலக்கம்_var_idxes (இலக்கம்_var_idxes_2),
                          nesting_level_இடது (nesting_level_இடது_2)
  {}
};


கட்டமைப்பு hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj)
  {
    எனில் (unlikely (nesting_level_இடது == 0))
      திரும்பு hb_empty_t ();

    எனில் (paint_visited (&obj))
      திரும்பு hb_empty_t ();

    nesting_level_இடது--;
    obj.dependv1 (இது);
    nesting_level_இடது++;
    திரும்பு hb_empty_t ();
  }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  தருக்க paint_visited (மாறிலி வெறுமை *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    எனில் (visited_paint.in_error() || visited_paint.has (delta))
      திரும்பு உண்மை;

    visited_paint.add (delta);
    எனில் (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    திரும்பு பொய்;
  }

  மாறிலி COLR* get_colr_table () மாறிலி
  { திரும்பு colr; }

  பொது:
  மாறிலி COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  unsigned nesting_level_இடது;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (மாறிலி COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              unsigned nesting_level_இடது_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_இடது (nesting_level_இடது_2),
                          source_gid (0)
  {}
};

கட்டமைப்பு LayerRecord
{
  operator hb_ot_color_layer_t () மாறிலி { திரும்பு {glyphId, colorIdx}; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
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
  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு BaseGlyphRecord
{
  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	இலக்கம்_Layers;	/* Number of color layers
				 * associated with this glyph */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
கட்டமைப்பு Variable
{
  நிலையான constexpr தருக்க is_variable = உண்மை;

  மாறிலி T& get_மதிப்பு () மாறிலி { திரும்பு value; }

  Variable<T>* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (இது));
  }

  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
  { value.dependv1 (c); }

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    c->இலக்கம்_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->இலக்கம்_var_idxes);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!value.subset (c, instancer, varIdxBase)) return_trace (பொய்);
    எனில் (c->plan->all_axes_pinned)
      return_trace (உண்மை);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    எனில் (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<unsigned, முழு> *new_varidx_delta;
      எனில் (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (பொய்);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி
  {
    TRACE_PAINT (இது);
    value.paint_glyph (c, varIdxBase);
  }

  வெறுமை get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  வெறுமை get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) மாறிலி
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () மாறிலி
  {
    திரும்பு value.get_extend ();
  }

  பாதுகாக்கப்பட்ட:
  T      value;
  பொது:
  VarIdx varIdxBase;
  பொது:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

template <typename T>
கட்டமைப்பு NoVariable
{
  நிலையான constexpr தருக்க is_variable = பொய்;

  மாறிலி T& get_மதிப்பு () மாறிலி { திரும்பு value; }

  நிலையான constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (இது));
  }

  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
  { value.dependv1 (c); }

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  { value.closurev1 (c); }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி
  {
    TRACE_PAINT (இது);
    value.paint_glyph (c, varIdxBase);
  }

  வெறுமை get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  வெறுமை get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) மாறிலி
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () மாறிலி
  {
    திரும்பு value.get_extend ();
  }

  T      value;
  பொது:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

template <typename Type, unsigned fraction_bits>
நிலையான வெறுமை
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, தசம value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

கட்டமைப்பு ColorStop
{
  வெறுமை dependv1 (hb_colrv1_depend_context_t * c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    c->add_palette_index (paletteIndex);
    c->இலக்கம்_var_idxes = 2;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      தசம a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (paletteIndex,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  பொது:
  DEFINE_SIZE_STATIC (2 + 2 * F2DOT14::static_size);
};

கட்டமைப்பு Extend : HBUINT8
{
  எண்ணிக்கை {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  பொது:
  DEFINE_SIZE_STATIC (1);
};

template <template<typename> வகுப்பு Var>
கட்டமைப்பு Color_வரி
{
  வெறுமை dependv1 (hb_colrv1_depend_context_t * c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    க்கு (மாறிலி auto &stop : stops.iter ())
      stop.closurev1 (c);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    எனில் (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (பொய்);
    எனில் (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (பொய்);

    க்கு (மாறிலி auto& stop : stops.iter ())
    {
      எனில் (!stop.subset (c, instancer)) return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  unsigned முழு
  get_color_stops (hb_paint_context_t *c,
                   unsigned முழு start,
		   unsigned முழு *மொத்தம்,
		   hb_color_stop_t *color_stops,
		   மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    unsigned முழு len = stops.len;

    எனில் (மொத்தம் && color_stops)
    {
      unsigned முழு i;
      க்கு (i = 0; i < *மொத்தம் && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *மொத்தம் = i;
    }

    திரும்பு len;
  }

  HB_INTERNAL நிலையான unsigned முழு static_get_color_stops (hb_color_வரி_t *color_வரி,
							  வெறுமை *color_வரி_data,
							  unsigned முழு start,
							  unsigned முழு *மொத்தம்,
							  hb_color_stop_t *color_stops,
							  வெறுமை *user_data)
  {
    மாறிலி Color_வரி *thiz = (மாறிலி Color_வரி *) color_வரி_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    திரும்பு thiz->get_color_stops (c, start, மொத்தம், color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () மாறிலி
  {
    தேர்வு ((unsigned முழு) extend)
    {
      நிலை Extend::EXTEND_REPEAT:  திரும்பு HB_PAINT_EXTEND_REPEAT;
      நிலை Extend::EXTEND_REFLECT: திரும்பு HB_PAINT_EXTEND_REFLECT;
      நிலை Extend::EXTEND_PAD:
      இயல்பு:                     திரும்பு HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL நிலையான hb_paint_extend_t static_get_extend (hb_color_வரி_t *color_வரி,
							  வெறுமை *color_வரி_data,
							  வெறுமை *user_data)
  {
    மாறிலி Color_வரி *thiz = (மாறிலி Color_வரி *) color_வரி_data;
    திரும்பு thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
கட்டமைப்பு CompositeMode : HBUINT8
{
  பொது:
  DEFINE_SIZE_STATIC (1);
};

கட்டமைப்பு Affine2x3
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  { c->இலக்கம்_var_idxes = 6; }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      auto fold = [&] (F16DOT16 &target, F16DOT16 source, unsigned offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((இரட்டை) source.to_int () +
                                                    (இரட்டை) instancer (varIdxBase, offset))));
      };
      fold (out->xx, xx, 0);
      fold (out->yx, yx, 1);
      fold (out->xy, xy, 2);
      fold (out->yy, yy, 3);
      fold (out->dx, dx, 4);
      fold (out->dy, dy, 5);
    }
    return_trace (உண்மை);
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    c->funcs->தள்ளு_transform (c->data,
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
  பொது:
  DEFINE_SIZE_STATIC (6 * F16DOT16::static_size);
};

கட்டமைப்பு PaintColrLayers
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer HB_UNUSED) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    uint32_t first_layer_index = இலக்கம்_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  உள்வரி வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி;

  HBUINT8	format; /* format = 1 */
  HBUINT8	இலக்கம்_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு PaintSolid
{
  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி {}
  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    c->add_palette_index (paletteIndex);
    c->இலக்கம்_var_idxes = 1;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      தசம a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    எனில் (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) மாறிலி
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->data, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	paletteIndex;
  F2DOT14	alpha;
  பொது:
  DEFINE_SIZE_STATIC (3 + F2DOT14::static_size);
};

template <template<typename> வகுப்பு Var>
கட்டமைப்பு PaintLinearGradient
{
  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    (இது+color_வரி_2).closurev1 (c);
    c->இலக்கம்_var_idxes = 6;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((இரட்டை) x0 + (இரட்டை) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((இரட்டை) y0 + (இரட்டை) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((இரட்டை) x1 + (இரட்டை) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((இரட்டை) y1 + (இரட்டை) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((இரட்டை) x2 + (இரட்டை) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((இரட்டை) y2 + (இரட்டை) roundf (instancer (varIdxBase, 5)));
    }

    எனில் (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_வரி_2.serialize_subset (c, color_வரி_2, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && color_வரி_2.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    hb_color_வரி_t cl = {
      (வெறுமை *) &(இது+color_வரி_2),
      (இது+color_வரி_2).static_get_color_stops, c,
      (இது+color_வரி_2).static_get_extend, nullptr
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
  Offset24To<Color_வரி<Var>>	color_வரி_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  பொது:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> வகுப்பு Var>
கட்டமைப்பு PaintRadialGradient
{
  தருக்க constant_after_pinning (hb_subset_context_t *c,
                               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    எனில் (c->plan->has_avar2) திரும்பு பொய்;
    மாறிலி auto &regions = instancer.varStore->get_region_list ();
    க்கு (unsigned i = 0; i < regions.region_மொத்தம்; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      எனில் (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        திரும்பு பொய்;
      }
      க்கு (auto axis : axes)
      {
        Triple *limit;
        எனில் (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          திரும்பு பொய்;
      }
    }
    திரும்பு உண்மை;
  }

  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    (இது+color_வரி_2).closurev1 (c);
    c->இலக்கம்_var_idxes = 6;
  }

  தருக்க subset_nonnegative (hb_subset_context_t *c,
                           மாறிலி ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) மாறிலி
  {
    இரட்டை source[6] = {
      x0 + (இரட்டை) roundf (instancer (varIdxBase, 0)),
      y0 + (இரட்டை) roundf (instancer (varIdxBase, 1)),
      radius0 + (இரட்டை) roundf (instancer (varIdxBase, 2)),
      x1 + (இரட்டை) roundf (instancer (varIdxBase, 3)),
      y1 + (இரட்டை) roundf (instancer (varIdxBase, 4)),
      radius1 + (இரட்டை) roundf (instancer (varIdxBase, 5))
    };
    auto transparent = [&] {
      auto *out = c->serializer->allocate_size<PaintSolid> (5);
      எனில் (!out) திரும்பு பொய்;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      திரும்பு உண்மை;
    };
    மாறிலி auto &வரி = இது+color_வரி_2;
    எனில் (source[2] == source[5] || !வரி.stops.len) திரும்பு transparent ();
    கட்டமைப்பு stop_t { இரட்டை offset, alpha; unsigned palette, order; };
    hb_vector_t<stop_t> stops;
    எனில் (!stops.alloc (வரி.stops.len)) திரும்பு பொய்;
    unsigned order = 0;
    க்கு (மாறிலி auto &entry : வரி.stops.iter ())
    {
      மாறிலி auto &stop = entry.get_மதிப்பு ();
      stops.push (stop_t {(இரட்டை) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (இரட்டை) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    எனில் (stops.in_error ()) திரும்பு பொய்;
    stops.qsort ([] (மாறிலி stop_t &a, மாறிலி stop_t &b) {
      திரும்பு a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    இரட்டை dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    unsigned lattice = 16384;
    க்கு (unsigned j = 0; j < 3; j++)
    {
      இரட்டை remainder = fmod (fabs (source[j + 3] - source[j]), 16384.);
      எனில் (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        திரும்பு பொய்;
      }
      unsigned வலது = (unsigned) remainder;
      போது (வலது) { unsigned r = lattice % வலது; lattice = வலது; வலது = r; }
    }
    க்கு (unsigned shift = 0; shift < 16; shift++)
    {
      இரட்டை b = 1u << shift;
      இரட்டை coarse = dr > 0 ? ceil (-source[2] / dr) : floor (source[2] / -dr) - b;
      இரட்டை fine = dr > 0 ? ceil (-source[2] / dr * lattice) / lattice :
                            floor (source[2] / -dr * lattice) / lattice - b;
      இரட்டை candidates[] = {coarse, fine};
      க்கு (இரட்டை a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        இரட்டை geometry[6];
        தருக்க fits = உண்மை;
        க்கு (unsigned j = 0; j < 3; j++)
        {
          இரட்டை difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          இரட்டை கீழ் = j == 2 ? 0 : -32768, உயர்வு = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= கீழ் && geometry[j] <= உயர்வு &&
                  geometry[j + 3] >= கீழ் && geometry[j + 3] <= உயர்வு;
        }
        எனில் (!fits) தொடர்;
        unsigned first = 0, மொத்தம் = stops.length;
        இரட்டை min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        இரட்டை bias = 0;
        தருக்க constant = பொய்;
        எனில் (வரி.extend == Extend::EXTEND_PAD)
        {
          இரட்டை tip = -geometry[2] / (b * dr);
          எனில் ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            மொத்தம் = 1; constant = உண்மை;
          }
        }
        இல்லையெனில்
        {
          இரட்டை period = max - min;
          எனில் (வரி.extend == Extend::EXTEND_REFLECT) period *= 2;
          எனில் (period <= 0) திரும்பு transparent ();
          இரட்டை n = ceil ((-2 - min) / period);
          எனில் (n > floor ((32767. / 16384. - max) / period)) தொடர்;
          bias = n * period;
        }
        க்கு (unsigned j = 0; j < மொத்தம்; j++)
        {
          இரட்டை bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        எனில் (!fits) தொடர்;
        auto *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        எனில் (!out) திரும்பு பொய்;
        out->format = 6;
        out->x0 = (முழு) geometry[0]; out->y0 = (முழு) geometry[1]; out->radius0 = (unsigned) geometry[2];
        out->x1 = (முழு) geometry[3]; out->y1 = (முழு) geometry[4]; out->radius1 = (unsigned) geometry[5];
        c->serializer->push ();
        auto *out_வரி = c->serializer->allocate_size<Color_வரி<NoVariable>> (3 + மொத்தம் * ColorStop::static_size);
        எனில் (!out_வரி) { c->serializer->எடு_discard (); திரும்பு பொய்; }
        out_வரி->extend = வரி.extend;
        out_வரி->stops.len = மொத்தம்;
        க்கு (unsigned j = 0; j < மொத்தம்; j++)
        {
          மாறிலி auto &stop = stops[first + j];
          auto &out_stop = out_வரி->stops[j].value;
          out_stop.stopOffset.set_float ((தசம) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((தசம) stop.alpha);
          எனில் (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->எடு_discard (); திரும்பு பொய்; }
        }
        c->serializer->add_link (out->color_வரி_2, c->serializer->எடு_pack ());
        திரும்பு !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    திரும்பு பொய்;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (இரட்டை) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (இரட்டை) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((இரட்டை) x0 + (இரட்டை) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((இரட்டை) y0 + (இரட்டை) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((இரட்டை) radius0 + (இரட்டை) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((இரட்டை) x1 + (இரட்டை) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((இரட்டை) y1 + (இரட்டை) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((இரட்டை) radius1 + (இரட்டை) roundf (instancer (varIdxBase, 5)));
    }

    எனில் (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_வரி_2.serialize_subset (c, color_வரி_2, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && color_வரி_2.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    hb_color_வரி_t cl = {
      (வெறுமை *) &(இது+color_வரி_2),
      (இது+color_வரி_2).static_get_color_stops, c,
      (இது+color_வரி_2).static_get_extend, nullptr
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
  Offset24To<Color_வரி<Var>>	color_வரி_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  பொது:
  DEFINE_SIZE_STATIC (4 + 6 * FWORD::static_size);
};

template <template<typename> வகுப்பு Var>
கட்டமைப்பு PaintSweepGradient
{
  வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி {}

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    (இது+color_வரி_2).closurev1 (c);
    c->இலக்கம்_var_idxes = 4;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((இரட்டை) centerX + (இரட்டை) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((இரட்டை) centerY + (இரட்டை) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    எனில் (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_வரி_2.serialize_subset (c, color_வரி_2, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && color_வரி_2.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    hb_color_வரி_t cl = {
      (வெறுமை *) &(இது+color_வரி_2),
      (இது+color_வரி_2).static_get_color_stops, c,
      (இது+color_வரி_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_வரி<Var>>	color_வரி_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
template <typename Types>
கட்டமைப்பு PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    return_trace (out->paint.serialize_subset (c, paint, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && paint.sanitize (c, இது));
  }

  உள்வரி வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  பொது:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

கட்டமைப்பு PaintColrGlyph
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer HB_UNUSED) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  உள்வரி வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  பொது:
  DEFINE_SIZE_STATIC (3);
};

template <template<typename> வகுப்பு Var>
கட்டமைப்பு PaintTransform
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    எனில் (!out->transform.serialize_subset (c, transform, இது, instancer)) return_trace (பொய்);
    எனில் (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  src.sanitize (c, இது) &&
                  transform.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி
  {
    TRACE_PAINT (இது);
    (இது+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  பொது:
  DEFINE_SIZE_STATIC (7);
};

கட்டமைப்பு PaintTranslate
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((இரட்டை) dx + (இரட்டை) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((இரட்டை) dy + (இரட்டை) roundf (instancer (varIdxBase, 1)));
    }

    எனில் (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம ddx = dx + c->instancer (varIdxBase, 0);
    தசம ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->தள்ளு_translate (c->data, ddx, ddy);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

template <typename Source>
நிலையான தருக்க serialize_scale_transform (hb_subset_context_t *c,
                                       தசம sx, தசம sy, தசம dx, தசம dy,
                                       மாறிலி Source &source)
{
  auto *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  எனில் (unlikely (!out)) திரும்பு பொய்;
  out->format = 12;

  c->serializer->push ();
  auto *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  எனில் (unlikely (!transform))
  {
    c->serializer->எடு_discard ();
    திரும்பு பொய்;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->எடு_pack ());

  c->serializer->push ();
  தருக்க ret = source ();
  எனில் (ret)
    c->serializer->add_link (out->src, c->serializer->எடு_pack ());
  இல்லையெனில்
    c->serializer->எடு_discard ();
  திரும்பு ret;
}

template <typename Base>
நிலையான தருக்க subset_scale_as_transform (hb_subset_context_t *c,
                                      மாறிலி Base *base,
                                      மாறிலி Offset24To<Paint> &src,
                                      மாறிலி ItemVarStoreInstancer &instancer,
                                      தசம sx, தசம sy,
                                      தசம cx = 0, தசம cy = 0)
{
  எனில் (src.is_null ()) திரும்பு பொய்;
  auto source = [&] { திரும்பு c->dispatch (base+src, instancer); };
  தசம dx = cx * (1 - sx), dy = cy * (1 - sy);
  எனில் (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    திரும்பு serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  auto inner = [&] {
    திரும்பு serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  auto inverse = [&] {
    திரும்பு serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  auto scale = [&] {
    திரும்பு serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  auto outer = [&] {
    திரும்பு serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  திரும்பு serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

கட்டமைப்பு PaintScale
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      தசம sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      தசம sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      எனில் (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, இது, src, instancer, sx, sy));
    }
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    எனில் (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    தசம sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->தள்ளு_scale (c->data, sx, sy);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

கட்டமைப்பு PaintScaleAroundCenter
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      தசம sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      தசம sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      எனில் (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, இது, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((இரட்டை) centerX + (இரட்டை) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((இரட்டை) centerY + (இரட்டை) roundf (instancer (varIdxBase, 3)));
    }

    எனில் (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    தசம sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    தசம tCenterX = centerX + c->instancer (varIdxBase, 2);
    தசம tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->தள்ளு_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

கட்டமைப்பு PaintScaleUniform
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      தசம s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      எனில் (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, இது, src, instancer, s, s));
    }
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    எனில் (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->தள்ளு_scale (c->data, s, s);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  பொது:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

கட்டமைப்பு PaintScaleUniformAroundCenter
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      தசம s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      எனில் (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, இது, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((இரட்டை) centerX + (இரட்டை) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((இரட்டை) centerY + (இரட்டை) roundf (instancer (varIdxBase, 2)));
    }

    எனில் (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம s = scale.to_float (c->instancer (varIdxBase, 0));
    தசம tCenterX = centerX + c->instancer (varIdxBase, 1);
    தசம tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->தள்ளு_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  பொது:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

கட்டமைப்பு PaintRotate
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    எனில் (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->தள்ளு_rotate (c->data, a);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  பொது:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

கட்டமைப்பு PaintRotateAroundCenter
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((இரட்டை) centerX + (இரட்டை) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((இரட்டை) centerY + (இரட்டை) roundf (instancer (varIdxBase, 2)));
    }

    எனில் (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம a = angle.to_float (c->instancer (varIdxBase, 0));
    தசம tCenterX = centerX + c->instancer (varIdxBase, 1);
    தசம tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->தள்ளு_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  பொது:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size + 2 * FWORD::static_size);
};

கட்டமைப்பு PaintSkew
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    எனில் (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    தசம sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->தள்ளு_skew (c->data, sx, sy);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size);
};

கட்டமைப்பு PaintSkewAroundCenter
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((இரட்டை) centerX + (இரட்டை) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((இரட்டை) centerY + (இரட்டை) roundf (instancer (varIdxBase, 3)));
    }

    எனில் (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, இது, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && src.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) மாறிலி
  {
    TRACE_PAINT (இது);
    தசம sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    தசம sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    தசம tCenterX = centerX + c->instancer (varIdxBase, 2);
    தசம tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->தள்ளு_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (இது+src);
    c->funcs->எடு_transform (c->data);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  பொது:
  DEFINE_SIZE_STATIC (4 + 2 * F2DOT14::static_size + 2 * FWORD::static_size);
};

கட்டமைப்பு PaintComposite
{
  HB_INTERNAL வெறுமை dependv1 (hb_colrv1_depend_context_t* c) மாறிலி;
  HB_INTERNAL வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி;

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    தருக்க ret = பொய்;
    ret |= out->src.serialize_subset (c, src, இது, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, இது, instancer);
    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  c->check_ops (இது->min_size) && // PainComposite can get exponential
                  src.sanitize (c, இது) &&
                  backdrop.sanitize (c, இது));
  }

  வெறுமை paint_glyph (hb_paint_context_t *c) மாறிலி
  {
    TRACE_PAINT (இது);
    hb_paint_composite_mode_t composite_mode;
    தேர்வு ((unsigned முழு) mode)
    {
      நிலை HB_PAINT_COMPOSITE_MODE_CLEAR:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_IN:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_IN:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      நிலை HB_PAINT_COMPOSITE_MODE_XOR:
      நிலை HB_PAINT_COMPOSITE_MODE_PLUS:
      நிலை HB_PAINT_COMPOSITE_MODE_SCREEN:
      நிலை HB_PAINT_COMPOSITE_MODE_OVERLAY:
      நிலை HB_PAINT_COMPOSITE_MODE_DARKEN:
      நிலை HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      நிலை HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      நிலை HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      நிலை HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      நிலை HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      நிலை HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      நிலை HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      நிலை HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      நிலை HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      நிலை HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      நிலை HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      நிலை HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (unsigned முழு) mode;
	நிறுத்து;
      இயல்பு:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	நிறுத்து;
    }

    c->funcs->தள்ளு_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (இது+backdrop);
    c->funcs->தள்ளு_group_for (c->data, composite_mode);
    c->recurse (இது+src);
    c->funcs->எடு_group (c->data, composite_mode);
    c->funcs->எடு_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு ClipBoxData
{
  முழு xMin, yMin, xMax, yMax;
};

கட்டமைப்பு ClipBoxFormat1
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை get_clip_box (ClipBoxData &clip_box, மாறிலி ItemVarStoreInstancer &instancer HB_UNUSED) மாறிலி
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((இரட்டை) xMin + (இரட்டை) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((இரட்டை) yMin + (இரட்டை) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((இரட்டை) xMax + (இரட்டை) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((இரட்டை) yMax + (இரட்டை) ceilf (instancer (varIdxBase, 3)));
    }

    எனில் (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (உண்மை);
  }

  பொது:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  பொது:
  DEFINE_SIZE_STATIC (1 + 4 * FWORD::static_size);
};

கட்டமைப்பு ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  வெறுமை get_clip_box (ClipBoxData &clip_box, மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    value.get_clip_box(clip_box, instancer);
    எனில் (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<முழு> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<முழு> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<முழு> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<முழு> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

கட்டமைப்பு ClipBox
{
  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    நிலை 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 2: hb_barrier (); u.format2.closurev1 (c); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க get_extents (hb_glyph_extents_t *extents,
                    மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    ClipBoxData clip_box;
    தேர்வு (u.format.v) {
    நிலை 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      நிறுத்து;
    நிலை 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      நிறுத்து;
    இயல்பு:
      திரும்பு பொய்;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    திரும்பு உண்மை;
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

கட்டமைப்பு ClipRecord
{
  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  வெறுமை closurev1 (hb_colrv1_closure_context_t* c, மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் (!c->glyphs->intersects (startGlyphID, endGlyphID)) திரும்பு;
    (base+clipBox).closurev1 (c);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி வெறுமை *base,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && clipBox.sanitize (c, base));
  }

  தருக்க get_extents (hb_glyph_extents_t *extents,
		    மாறிலி வெறுமை *base,
		    மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    திரும்பு (base+clipBox).get_extents (extents, instancer);
  }

  பொது:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  பொது:
  DEFINE_SIZE_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

கட்டமைப்பு ClipList
{
  unsigned serialize_clip_records (hb_subset_context_t *c,
                                   மாறிலி ItemVarStoreInstancer &instancer,
                                   மாறிலி hb_set_t& gids,
                                   மாறிலி hb_map_t& gid_offset_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    எனில் (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    unsigned மொத்தம்  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    unsigned offset = gid_offset_map.get (start_gid);
    unsigned prev_offset = offset;
    க்கு (மாறிலி hb_codepoint_t _ : gids.iter ())
    {
      எனில் (_ == start_gid) தொடர்;

      offset = gid_offset_map.get (_);
      எனில் (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        தொடர்;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      எனில் (!record.subset (c, இது, instancer)) return_trace (0);
      மொத்தம்++;

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
      எனில் (!record.subset (c, இது, instancer)) return_trace (0);
      மொத்தம்++;
    }
    return_trace (மொத்தம்);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    எனில் (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (பொய்);

    மாறிலி hb_set_t& glyphset = c->plan->_glyphset_colred;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    க்கு (மாறிலி ClipRecord& record : clips.iter ())
    {
      unsigned start_gid = record.startGlyphID;
      unsigned end_gid = record.endGlyphID;
      க்கு (unsigned gid = start_gid; gid <= end_gid; gid++)
      {
        எனில் (!glyphset.has (gid) || !glyph_map.has (gid)) தொடர்;
        unsigned new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    unsigned மொத்தம் = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    எனில் (!மொத்தம்) return_trace (பொய்);
    return_trace (c->serializer->check_assign (out->clips.len, மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (இது) && clips.sanitize (c, இது));
  }

  தருக்க
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    auto *rec = clips.as_array ().bsearch (gid);
    எனில் (rec)
      திரும்பு rec->get_extents (extents, இது, instancer);
    திரும்பு பொய்;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

கட்டமைப்பு Paint
{

  template <typename ...Ts>
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_மதிப்பு ());

    return_trace (c->end_recursion (இது->dispatch (c, std::forward<Ts> (ds)...)));
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    நிலை 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    நிலை 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    நிலை 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    நிலை 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    நிலை 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    நிலை 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    நிலை 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    நிலை 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    நிலை 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    நிலை 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    நிலை 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    நிலை 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    நிலை 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    நிலை 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    நிலை 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    நிலை 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    நிலை 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    நிலை 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    நிலை 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    நிலை 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    நிலை 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    நிலை 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    நிலை 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    நிலை 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    நிலை 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    நிலை 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    நிலை 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    நிலை 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); திரும்பு உண்மை;
    நிலை 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); திரும்பு உண்மை;
    இயல்பு: திரும்பு பொய்;
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT8 v; }				format;
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
  பொது:
  DEFINE_SIZE_MIN (2);
};

கட்டமைப்பு BaseGlyphPaintRecord
{
  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  தருக்க serialize (hb_serialize_context_t *s, மாறிலி hb_map_t* glyph_map,
                  மாறிலி வெறுமை* src_base, hb_subset_context_t *c,
                  மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = s->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    எனில் (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) && paint.sanitize (c, base)));
  }

  பொது:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out)))  return_trace (பொய்);
    மாறிலி hb_set_t* glyphset = &c->plan->_glyphset_colred;

    க்கு (மாறிலி auto& _ : as_array ())
    {
      unsigned gid = _.glyphId;
      எனில் (!glyphset->has (gid)) தொடர்;

      எனில் (_.serialize (c->serializer, c->plan->glyph_map, இது, c, instancer)) out->len++;
      இல்லையெனில் return_trace (பொய்);
    }

    return_trace (out->len != 0);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, இது));
  }
};

கட்டமைப்பு LayerList : Array32OfOffset32To<Paint>
{
  மாறிலி Paint& get_paint (unsigned i) மாறிலி
  { திரும்பு இது+(*இது)[i]; }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி ItemVarStoreInstancer &instancer) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out)))  return_trace (பொய்);

    தருக்க ret = பொய்;
    க்கு (மாறிலி auto& _ : + hb_enumerate (*இது)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      auto *o = out->serialize_append (c->serializer);
      எனில் (unlikely (!o)) return_trace (பொய்);
      ret |= o->serialize_subset (c, _.second, இது, instancer);
    }
    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, இது));
  }
};

கட்டமைப்பு delta_set_index_map_subset_plan_t
{
  unsigned get_inner_bit_மொத்தம் () மாறிலி { திரும்பு inner_bit_மொத்தம்; }
  unsigned get_width ()           மாறிலி { திரும்பு ((outer_bit_மொத்தம் + inner_bit_மொத்தம் + 7) / 8); }
  hb_array_t<மாறிலி uint32_t> get_output_map () மாறிலி { திரும்பு output_map.as_array (); }

  delta_set_index_map_subset_plan_t (மாறிலி hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_மொத்தம் = 0;
    outer_bit_மொத்தம் = 0;
    inner_bit_மொத்தம் = 1;
    output_map.init ();

    /* search backwards */
    unsigned மொத்தம் = new_deltaset_idx_varidx_map.get_population ();
    எனில் (!மொத்தம்) திரும்பு;

    unsigned last_idx = (unsigned)-1;
    unsigned last_varidx = (unsigned)-1;

    க்கு (unsigned i = மொத்தம்; i; i--)
    {
      unsigned delta_set_idx = i - 1;
      unsigned var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      எனில் (i == மொத்தம்)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        தொடர்;
      }
      எனில் (var_idx != last_varidx)
        நிறுத்து;
      last_idx = delta_set_idx;
    }

    map_மொத்தம் = last_idx + 1;
  }

  தருக்க remap (மாறிலி hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_மொத்தம் = 1;
    inner_bit_மொத்தம் = 1;

    எனில் (unlikely (!output_map.resize_dirty (map_மொத்தம்))) திரும்பு பொய்;

    க்கு (unsigned idx = 0; idx < map_மொத்தம்; idx++)
    {
      uint32_t *var_idx;
      எனில் (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) திரும்பு பொய்;
      output_map.arrayZ[idx] = *var_idx;

      unsigned outer = (*var_idx) >> 16;
      unsigned bit_மொத்தம் = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_மொத்தம் = hb_max (bit_மொத்தம், outer_bit_மொத்தம்);

      unsigned inner = (*var_idx) & 0xFFFF;
      bit_மொத்தம் = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_மொத்தம் = hb_max (bit_மொத்தம், inner_bit_மொத்தம்);
    }
    திரும்பு உண்மை;
  }

  தனிப்பட்ட:
  unsigned map_மொத்தம்;
  unsigned outer_bit_மொத்தம்;
  unsigned inner_bit_மொத்தம்;
  hb_vector_t<uint32_t> output_map;
};

கட்டமைப்பு COLR
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  தருக்க has_data () மாறிலி { திரும்பு has_v0_data () || version; }

  தருக்க has_v0_data () மாறிலி { திரும்பு இலக்கம்_Base_Glyphs; }
  தருக்க has_v1_data () மாறிலி
  {
    எனில் (version < 1)
      திரும்பு பொய்;
    hb_barrier ();

    திரும்பு (இது+baseGlyphList).len > 0;
  }

  unsigned முழு get_glyph_layers (hb_codepoint_t       glyph,
				 unsigned முழு         start_offset,
				 unsigned முழு        *மொத்தம், /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) மாறிலி
  {
    மாறிலி BaseGlyphRecord &record = (இது+baseGlyphsZ).bsearch (இலக்கம்_Base_Glyphs, glyph);

    hb_array_t<மாறிலி LayerRecord> all_layers = (இது+layersZ).as_array (இலக்கம்_Layers);
    hb_array_t<மாறிலி LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.இலக்கம்_Layers);
    எனில் (மொத்தம் && layers)
    {
      + glyph_layers.sub_array (start_offset, மொத்தம்)
      | hb_sink (hb_array (layers, *மொத்தம்))
      ;
    }
    திரும்பு glyph_layers.length;
  }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      auto *scratch = cached_scratch.get_relaxed ();
      எனில் (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    தருக்க has_data () மாறிலி { திரும்பு colr->has_data (); }

#ifndef HB_NO_PAINT
    தருக்க
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) மாறிலி
    {
      எனில் (unlikely (!has_data ())) திரும்பு பொய்;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      எனில் (unlikely (!scratch)) திரும்பு உண்மை;
      தருக்க ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      திரும்பு ret;
    }

    தருக்க paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, வெறுமை *data,
		      unsigned முழு palette_index,
		      hb_color_t foreground,
		      தருக்க clip = உண்மை) மாறிலி
    {
      எனில் (unlikely (!has_data ())) திரும்பு பொய்;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      எனில் (unlikely (!scratch)) திரும்பு உண்மை;
      தருக்க ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      திரும்பு ret;
    }
#endif

    தருக்க is_valid () { திரும்பு colr.get_blob ()->length; }

    வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி
    {
      எனில் (!has_data ()) திரும்பு;
      colr->depend (depend_data);
    }

    வெறுமை closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) மாறிலி
    { colr->closure_glyphs (glyph, related_ids); }

    வெறுமை closure_V0palette_indices (மாறிலி hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) மாறிலி
    { colr->closure_V0palette_indices (glyphs, palettes); }

    வெறுமை closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) மாறிலி
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    தருக்க has_var_store () மாறிலி
    { திரும்பு colr->has_var_store (); }

    மாறிலி ItemVariationStore &get_var_store () மாறிலி
    { திரும்பு colr->get_var_store (); }
    மாறிலி ItemVariationStore *get_var_store_ptr () மாறிலி
    { திரும்பு colr->get_var_store_ptr (); }

    தருக்க has_delta_set_index_map () மாறிலி
    { திரும்பு colr->has_delta_set_index_map (); }

    மாறிலி DeltaSetIndexMap &get_delta_set_index_map () மாறிலி
    { திரும்பு colr->get_delta_set_index_map (); }
    மாறிலி DeltaSetIndexMap *get_delta_set_index_map_ptr () மாறிலி
    { திரும்பு colr->get_delta_set_index_map_ptr (); }

    தனிப்பட்ட:

    hb_colr_scratch_t *acquire_scratch () மாறிலி
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      எனில் (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, அளவு (hb_colr_scratch_t));
	எனில் (unlikely (!scratch))
	  திரும்பு nullptr;
      }

      திரும்பு scratch;
    }
    வெறுமை release_scratch (hb_colr_scratch_t *scratch) மாறிலி
    {
      எனில் (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    பொது:
    hb_blob_ptr_t<COLR> colr;
    தனிப்பட்ட:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி
  {
    // v0
    hb_array_t<மாறிலி BaseGlyphRecord> baseGlyphs = (இது+baseGlyphsZ).as_array (இலக்கம்_Base_Glyphs);
    க்கு (மாறிலி BaseGlyphRecord &record : baseGlyphs)
    {
      auto glyph_layers = (இது+layersZ).as_array (இலக்கம்_Layers).sub_array (record.firstLayerIdx,
                                                                         record.இலக்கம்_Layers);
      க்கு (மாறிலி LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    எனில் (!has_v1_data())
      திரும்பு;

    // v1
    hb_colrv1_depend_context_t c (இது, depend_data);
    மாறிலி BaseGlyphList &baseglyph_paintrecords = இது+baseGlyphList;

    க்கு (மாறிலி BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      மாறிலி Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  வெறுமை closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) மாறிலி
  {
    மாறிலி BaseGlyphRecord *record = get_base_glyph_record (glyph);
    எனில் (!record) திரும்பு;

    auto glyph_layers = (இது+layersZ).as_array (இலக்கம்_Layers).sub_array (record->firstLayerIdx,
								       record->இலக்கம்_Layers);
    எனில் (!glyph_layers.length) திரும்பு;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  வெறுமை closure_V0palette_indices (மாறிலி hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) மாறிலி
  {
    எனில் (!இலக்கம்_Base_Glyphs || !இலக்கம்_Layers) திரும்பு;
    hb_array_t<மாறிலி BaseGlyphRecord> baseGlyphs = (இது+baseGlyphsZ).as_array (இலக்கம்_Base_Glyphs);
    hb_array_t<மாறிலி LayerRecord> all_layers = (இது+layersZ).as_array (இலக்கம்_Layers);

    க்கு (மாறிலி BaseGlyphRecord record : baseGlyphs)
    {
      எனில் (!glyphs->has (record.glyphId)) தொடர்;
      hb_array_t<மாறிலி LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.இலக்கம்_Layers);
      க்கு (மாறிலி LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  வெறுமை closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) மாறிலி
  {
    எனில் (version < 1) திரும்பு;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (இது, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    மாறிலி BaseGlyphList &baseglyph_paintrecords = இது+baseGlyphList;

    க்கு (மாறிலி BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      unsigned gid = baseglyph_paintrecord.glyphId;
      எனில் (!glyphset->has (gid)) தொடர்;

      மாறிலி Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    மாறிலி ClipList &cliplist = இது+clipList;
    c.glyphs = glyphset;
    க்கு (மாறிலி ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    எனில் (has_delta_set_index_map ())
    {
      மாறிலி DeltaSetIndexMap &var_idx_map = இது+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      க்கு (unsigned delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  மாறிலி LayerList& get_layerList () மாறிலி
  { திரும்பு (இது+layerList); }

  மாறிலி BaseGlyphList& get_baseglyphList () மாறிலி
  { திரும்பு (இது+baseGlyphList); }

  தருக்க has_var_store () மாறிலி
  { திரும்பு version >= 1 && hb_barrier () && varStore != 0; }

  தருக்க has_delta_set_index_map () மாறிலி
  { திரும்பு version >= 1 && hb_barrier () && varIdxMap != 0; }

  தருக்க has_clip_list () மாறிலி
  { திரும்பு version >= 1 && hb_barrier () && clipList != 0; }

  மாறிலி DeltaSetIndexMap &get_delta_set_index_map () மாறிலி
  { திரும்பு has_delta_set_index_map () && hb_barrier () ? இது+varIdxMap : Null (DeltaSetIndexMap); }
  மாறிலி DeltaSetIndexMap *get_delta_set_index_map_ptr () மாறிலி
  { திரும்பு has_delta_set_index_map () && hb_barrier () ? &(இது+varIdxMap) : nullptr; }

  மாறிலி ItemVariationStore &get_var_store () மாறிலி
  { திரும்பு has_var_store () && hb_barrier () ? இது+varStore : Null (ItemVariationStore); }
  மாறிலி ItemVariationStore *get_var_store_ptr () மாறிலி
  { திரும்பு has_var_store () && hb_barrier () ? &(இது+varStore) : nullptr; }

  மாறிலி ClipList &get_clip_list () மாறிலி
  { திரும்பு has_clip_list () && hb_barrier () ? இது+clipList : Null (ClipList); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
                  (இது+baseGlyphsZ).sanitize (c, இலக்கம்_Base_Glyphs) &&
                  (இது+layersZ).sanitize (c, இலக்கம்_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, இது) &&
		    layerList.sanitize (c, இது) &&
		    clipList.sanitize (c, இது) &&
		    varIdxMap.sanitize (c, இது) &&
		    varStore.sanitize (c, இது))));
  }

  template<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  தருக்க serialize_V0 (hb_serialize_context_t *c,
		     unsigned version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (base_it.len () != layer_it.len ()))
      return_trace (பொய்);

    இது->version = version;
    இலக்கம்_Layers = 0;
    இலக்கம்_Base_Glyphs = base_it.len ();
    எனில் (இலக்கம்_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (உண்மை);
    }

    c->push ();
    க்கு (மாறிலி hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      auto* record = c->embed (_);
      எனில் (unlikely (!record)) return_trace (பொய்);
      record->firstLayerIdx = இலக்கம்_Layers;
      இலக்கம்_Layers += record->இலக்கம்_Layers;
    }
    c->add_link (baseGlyphsZ, c->எடு_pack ());

    c->push ();
    க்கு (மாறிலி hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->எடு_pack ());

    return_trace (உண்மை);
  }

  மாறிலி BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) மாறிலி
  {
    மாறிலி BaseGlyphRecord* record = &(இது+baseGlyphsZ).bsearch (இலக்கம்_Base_Glyphs, (unsigned முழு) gid);
    எனில் (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    திரும்பு record;
  }

  மாறிலி BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) மாறிலி
  {
    மாறிலி BaseGlyphList &list = இது+baseGlyphList;
    unsigned முழு i;
    எனில் (!list.bfind ((unsigned) gid, &i))
      திரும்பு nullptr;
    திரும்பு &list[i];
  }

  தருக்க downgrade_to_V0 (மாறிலி hb_set_t &glyphset) மாறிலி
  {
    //no more COLRv1 glyphs, downgrade to version 0
    க்கு (மாறிலி BaseGlyphPaintRecord& _ : get_baseglyphList ())
      எனில் (glyphset.has (_.glyphId))
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (உண்மை);

    மாறிலி ItemVariationStore& var_store = இது+varStore;
    எனில் (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      தருக்க optimize = (varIdxMap != 0) ? உண்மை : பொய்;
      எனில் (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (பொய்);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      எனில் (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (பொய்);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      எனில் (optimize)
      {
        மாறிலி hb_map_t &varidx_map = item_vars.get_varidx_map ();
        க்கு (auto _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          எனில் (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          இல்லையெனில்
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    இல்லையெனில்
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      எனில் (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      எனில் (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  தருக்க subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (உண்மை);

    மாறிலி hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    எனில் (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (பொய்);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    மாறிலி hb_set_t& glyphset = c->plan->_glyphset_colred;

    auto base_it =
    + hb_range (c->plan->இலக்கம்_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    எனில் (glyphset.has (old_gid)) திரும்பு உண்மை;
		    திரும்பு பொய்;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				மாறிலி BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				எனில் (unlikely (!old_record))
				  திரும்பு hb_pair_t<தருக்க, BaseGlyphRecord> (பொய், Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.இலக்கம்_Layers = old_record->இலக்கம்_Layers;
				திரும்பு hb_pair_t<தருக்க, BaseGlyphRecord> (உண்மை, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    auto layer_it =
    + hb_range (c->plan->இலக்கம்_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				மாறிலி BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				எனில் (unlikely (!old_record ||
					      old_record->firstLayerIdx >= இலக்கம்_Layers ||
					      old_record->firstLayerIdx + old_record->இலக்கம்_Layers > இலக்கம்_Layers))
				  திரும்பு hb_pair_t<தருக்க, hb_vector_t<LayerRecord>> (பொய், out_layers);

				auto layers = (இது+layersZ).as_array (இலக்கம்_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->இலக்கம்_Layers);
				out_layers.resize (layers.length);
				க்கு (unsigned முழு i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  எனில் (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    திரும்பு hb_pair_t<தருக்க, hb_vector_t<LayerRecord>> (பொய், out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				திரும்பு hb_pair_t<தருக்க, hb_vector_t<LayerRecord>> (உண்மை, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    எனில் (version == 0 && (!base_it || !layer_it))
      return_trace (பொய்);

    auto *colr_பகாஎண் = c->serializer->start_embed<COLR> ();
    எனில் (unlikely (!c->serializer->extend_min (colr_பகாஎண்)))  return_trace (பொய்);

    எனில் (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_பகாஎண்->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    எனில் (!c->serializer->allocate_size<வெறுமை> (5 * HBUINT32::static_size)) return_trace (பொய்);
    எனில் (!colr_பகாஎண்->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (பொய்);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    எனில் (!subset_varstore (c, colr_பகாஎண்)) return_trace (பொய்);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    எனில் (!colr_பகாஎண்->baseGlyphList.serialize_subset (c, baseGlyphList, இது, instancer))
      return_trace (பொய்);

    colr_பகாஎண்->layerList.serialize_subset (c, layerList, இது, instancer);
    colr_பகாஎண்->clipList.serialize_subset (c, clipList, இது, instancer);

    return_trace (subset_delta_set_index_map (c, colr_பகாஎண்));
  }

  மாறிலி Paint *get_base_glyph_paint (hb_codepoint_t glyph) மாறிலி
  {
    மாறிலி BaseGlyphList &baseglyph_paintrecords = இது+baseGlyphList;
    மாறிலி BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    எனில் (record)
    {
      மாறிலி Paint &paint = &baseglyph_paintrecords+record->paint;
      திரும்பு &paint;
    }
    இல்லையெனில்
      திரும்பு nullptr;
  }

#ifndef HB_NO_PAINT
  தருக்க
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) மாறிலி
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->இலக்கம்_coords));

    எனில் (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      திரும்பு உண்மை;
    }

    auto *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    தருக்க ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), உண்மை, scratch);

    auto e = scratch.paint_extents.get_extents ();
    எனில் (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    இல்லையெனில்
      *extents = e.to_glyph_extents ();

    திரும்பு ret;
  }
#endif

  தருக்க
  has_paint_for_glyph (hb_codepoint_t glyph) மாறிலி
  {
    எனில் (version >= 1)
    {
      hb_barrier ();

      மாறிலி Paint *paint = get_base_glyph_paint (glyph);

      திரும்பு paint != nullptr;
    }

    திரும்பு பொய்;
  }

  தருக்க get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 மாறிலி ItemVarStoreInstancer instancer) மாறிலி
  {
    திரும்பு get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  தருக்க
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, வெறுமை *data,
	       unsigned முழு palette_index, hb_color_t foreground,
	       தருக்க clip,
	       hb_colr_scratch_t &scratch) மாறிலி
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->இலக்கம்_coords : 0));
    hb_paint_context_t c (இது, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_முனை_t node (c.glyphs_decycler);
    node.visit (glyph);

    எனில் (version >= 1)
    {
      hb_barrier ();

      மாறிலி Paint *paint = get_base_glyph_paint (glyph);
      எனில் (paint)
      {
        // COLRv1 glyph

	தருக்க is_bounded = உண்மை;
	hb_glyph_extents_t extents = {};
	எனில் (clip)
	{
	  எனில் (!get_clip (glyph, &extents, instancer))
	  {
	    clip = பொய்;
	    is_bounded = பொய்;
	  }

	  எனில் (!is_bounded)
	  {
	    auto *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 பொய்,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->தள்ளு_font_transform (c.data, font);

	எனில் (clip)
	  c.funcs->தள்ளு_clip_rectangle (c.data,
					(தசம) extents.x_bearing,
					(தசம) extents.y_bearing + extents.height,
					(தசம) extents.x_bearing + extents.width,
					(தசம) extents.y_bearing);

	எனில் (is_bounded)
	  c.recurse (*paint);

	எனில் (clip)
	  c.funcs->எடு_clip (c.data);

	c.funcs->எடு_transform (c.data);

        திரும்பு உண்மை;
      }
    }

    மாறிலி BaseGlyphRecord *record = get_base_glyph_record (glyph);
    எனில் (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      க்கு (மாறிலி auto &r : (இது+layersZ).as_array (இலக்கம்_Layers)
			   .sub_array (record->firstLayerIdx, record->இலக்கம்_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      திரும்பு உண்மை;
    }

    திரும்பு பொய்;
  }
#endif

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	இலக்கம்_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	இலக்கம்_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32To<BaseGlyphList>		baseGlyphList;
  Offset32To<LayerList>			layerList;
  Offset32To<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32To<DeltaSetIndexMap>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  பொது:
  DEFINE_SIZE_MIN (14);
};

கட்டமைப்பு COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

வெறுமை
hb_paint_context_t::recurse (மாறிலி Paint &paint)
{
  எனில் (unlikely (depth_இடது <= 0 || edge_மொத்தம் <= 0)) திரும்பு;
  depth_இடது--;
  edge_மொத்தம்--;
  paint.dispatch (இது);
  depth_இடது++;
}

template <typename Types>
வெறுமை PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) மாறிலி
{
  TRACE_PAINT (இது);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  எனில் (likely (c->depth_இடது > 0 && c->edge_மொத்தம் > 0) &&
      (இது+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_மொத்தம்--;
    c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->எடு_transform (c->data);
    திரும்பு;
  }

  c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
  c->funcs->தள்ளு_clip_glyph (c->data, gid, c->font);
  c->funcs->தள்ளு_font_transform (c->data, c->font);
  c->recurse (இது+paint);
  c->funcs->எடு_transform (c->data);
  c->funcs->எடு_clip (c->data);
  c->funcs->எடு_transform (c->data);
}

வெறுமை PaintColrLayers::paint_glyph (hb_paint_context_t *c) மாறிலி
{
  TRACE_PAINT (இது);
  மாறிலி LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_முனை_t node (c->layers_decycler);
  க்கு (unsigned i = firstLayerIndex; i < firstLayerIndex + இலக்கம்_Layers; i++)
  {
    எனில் (unlikely (!node.visit (i)))
      திரும்பு;

    மாறிலி Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

வெறுமை PaintColrGlyph::paint_glyph (hb_paint_context_t *c) மாறிலி
{
  TRACE_PAINT (இது);

  hb_decycler_முனை_t node (c->glyphs_decycler);
  எனில் (unlikely (!node.visit (gid)))
    திரும்பு;

  c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
  எனில் (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->எடு_transform (c->data);
    திரும்பு;
  }
  c->funcs->எடு_transform (c->data);

  மாறிலி COLR *colr_table = c->get_colr_table ();
  மாறிலி Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  தருக்க has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  எனில் (has_clip_box)
    c->funcs->தள்ளு_clip_rectangle (c->data,
				   (தசம) extents.x_bearing,
				   (தசம) extents.y_bearing + extents.height,
				   (தசம) extents.x_bearing + extents.width,
				   (தசம) extents.y_bearing);

  எனில் (paint)
    c->recurse (*paint);

  எனில் (has_clip_box)
    c->funcs->எடு_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
