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
  स्थिरांक अक्षर *get_name () { परत "PAINT"; }
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.paint_glyph (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  स्थिरांक COLR* get_colr_table () स्थिरांक
  { परत reinterpret_cast<स्थिरांक COLR *> (base); }

सार्वजनिक:
  स्थिरांक शून्य *base;
  hb_paint_funcs_t *funcs;
  शून्य *data;
  hb_font_t *font;
  hb_array_t<स्थिरांक BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  पूर्णांक depth_डावा = HB_MAX_NESTING_LEVEL;
  पूर्णांक edge_मोजणी = HB_MAX_GRAPH_EDGE_मोजणी;

  hb_paint_context_t (स्थिरांक शून्य *base_,
		      hb_paint_funcs_t *funcs_,
		      शून्य *data_,
                      hb_font_t *font_,
                      अचिन्हित पूर्णांक palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    data (data_),
    font (font_),
    palette (
#ifndef HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_मोजणी () ? palette_ : 0)
#endif
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    जर (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, सत्य);
      hb_font_set_synthetic_slant (font, 0);
    }
    अन्यथा
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  स्थिर अचिन्हित
  color_alpha (hb_color_t color, दशांश alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    परत roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (अचिन्हित पूर्णांक color_index, दशांश alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = सत्य;

    जर (color_index != 0xffff)
    {
      जर (!funcs->custom_palette_color (data, color_index, &color))
        color = palette[color_index];

      *is_foreground = असत्य;
    }

    परत HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  ओळीत शून्य recurse (स्थिरांक Paint &paint);
};

संरचना hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj)
  {
    जर (unlikely (nesting_level_डावा == 0))
      परत hb_empty_t ();

    जर (paint_visited (&obj))
      परत hb_empty_t ();

    nesting_level_डावा--;
    obj.closurev1 (हे);
    nesting_level_डावा++;
    परत hb_empty_t ();
  }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  तार्किक paint_visited (स्थिरांक शून्य *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    जर (visited_paint.in_error() || visited_paint.has (delta))
      परत सत्य;

    visited_paint.add (delta);
    परत असत्य;
  }

  स्थिरांक COLR* get_colr_table () स्थिरांक
  { परत reinterpret_cast<स्थिरांक COLR *> (base); }

  शून्य add_glyph (अचिन्हित glyph_id)
  { glyphs->add (glyph_id); }

  शून्य add_layer_indices (अचिन्हित first_layer_index, अचिन्हित अंक_of_layers)
  {
    जर (अंक_of_layers == 0) परत;
    layer_indices->add_range (first_layer_index, first_layer_index + अंक_of_layers - 1);
  }

  शून्य add_palette_index (अचिन्हित palette_index)
  { palette_indices->add (palette_index); }

  शून्य add_var_idxes (अचिन्हित first_var_idx, अचिन्हित अंक_idxes)
  {
    जर (!अंक_idxes || first_var_idx == VarIdx::NO_VARIATION) परत;
    variation_indices->add_range (first_var_idx, first_var_idx + अंक_idxes - 1);
  }

  सार्वजनिक:
  स्थिरांक शून्य *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  अचिन्हित अंक_var_idxes;
  अचिन्हित nesting_level_डावा;

  hb_colrv1_closure_context_t (स्थिरांक शून्य *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               अचिन्हित अंक_var_idxes_2 = 1,
                               अचिन्हित nesting_level_डावा_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          अंक_var_idxes (अंक_var_idxes_2),
                          nesting_level_डावा (nesting_level_डावा_2)
  {}
};


संरचना hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj)
  {
    जर (unlikely (nesting_level_डावा == 0))
      परत hb_empty_t ();

    जर (paint_visited (&obj))
      परत hb_empty_t ();

    nesting_level_डावा--;
    obj.dependv1 (हे);
    nesting_level_डावा++;
    परत hb_empty_t ();
  }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  तार्किक paint_visited (स्थिरांक शून्य *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    जर (visited_paint.in_error() || visited_paint.has (delta))
      परत सत्य;

    visited_paint.add (delta);
    जर (unlikely (visited_paint.in_error ()))
      depend_data->fail ();
    परत असत्य;
  }

  स्थिरांक COLR* get_colr_table () स्थिरांक
  { परत colr; }

  सार्वजनिक:
  स्थिरांक COLR *colr;
  hb_depend_data_builder_t *depend_data;
  hb_set_t visited_paint;
  अचिन्हित nesting_level_डावा;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (स्थिरांक COLR *colr_,
                              hb_depend_data_builder_t *depend_data_,
                              अचिन्हित nesting_level_डावा_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_data (depend_data_),
                          nesting_level_डावा (nesting_level_डावा_2),
                          source_gid (0)
  {}
};

संरचना LayerRecord
{
  चालक hb_ot_color_layer_t () स्थिरांक { परत {glyphId, colorIdx}; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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
  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  सार्वजनिक:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	अंक_Layers;	/* Number of color layers
				 * associated with this glyph */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

ढाँचा <typename T>
संरचना Variable
{
  स्थिर constexpr तार्किक is_variable = सत्य;

  स्थिरांक T& get_मूल्य () स्थिरांक { परत value; }

  Variable<T>* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (हे));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक
  { value.dependv1 (c); }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    c->अंक_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->अंक_var_idxes);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!value.subset (c, instancer, varIdxBase)) return_trace (असत्य);
    जर (c->plan->all_axes_pinned)
      return_trace (सत्य);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    जर (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      जर (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (असत्य);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c));
  }

  शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक
  {
    TRACE_PAINT (हे);
    value.paint_glyph (c, varIdxBase);
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  शून्य get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिरांक
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () स्थिरांक
  {
    परत value.get_extend ();
  }

  संरक्षित:
  T      value;
  सार्वजनिक:
  VarIdx varIdxBase;
  सार्वजनिक:
  DEFINE_SIZE_MIN (VarIdx::static_size + T::min_size);
};

ढाँचा <typename T>
संरचना NoVariable
{
  स्थिर constexpr तार्किक is_variable = असत्य;

  स्थिरांक T& get_मूल्य () स्थिरांक { परत value; }

  स्थिर constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (हे));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक
  { value.dependv1 (c); }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  { value.closurev1 (c); }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c));
  }

  शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक
  {
    TRACE_PAINT (हे);
    value.paint_glyph (c, varIdxBase);
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  शून्य get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिरांक
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () स्थिरांक
  {
    परत value.get_extend ();
  }

  T      value;
  सार्वजनिक:
  DEFINE_SIZE_MIN (T::min_size);
};

// Color structures

ढाँचा <typename Type, अचिन्हित fraction_bits>
स्थिर शून्य
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, दशांश value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

संरचना ColorStop
{
  शून्य dependv1 (hb_colrv1_depend_context_t * c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    c->add_palette_index (paletteIndex);
    c->अंक_var_idxes = 2;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      दशांश a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
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

ढाँचा <ढाँचा<typename> वर्ग Var>
संरचना Color_ओळ
{
  शून्य dependv1 (hb_colrv1_depend_context_t * c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    साठी (स्थिरांक स्वतः &stop : stops.iter ())
      stop.closurev1 (c);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    जर (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);
    जर (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (असत्य);

    साठी (स्थिरांक स्वतः& stop : stops.iter ())
    {
      जर (!stop.subset (c, instancer)) return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  अचिन्हित पूर्णांक
  get_color_stops (hb_paint_context_t *c,
                   अचिन्हित पूर्णांक start,
		   अचिन्हित पूर्णांक *मोजणी,
		   hb_color_stop_t *color_stops,
		   स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    अचिन्हित पूर्णांक len = stops.len;

    जर (मोजणी && color_stops)
    {
      अचिन्हित पूर्णांक i;
      साठी (i = 0; i < *मोजणी && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *मोजणी = i;
    }

    परत len;
  }

  HB_INTERNAL स्थिर अचिन्हित पूर्णांक static_get_color_stops (hb_color_ओळ_t *color_ओळ,
							  शून्य *color_ओळ_data,
							  अचिन्हित पूर्णांक start,
							  अचिन्हित पूर्णांक *मोजणी,
							  hb_color_stop_t *color_stops,
							  शून्य *user_data)
  {
    स्थिरांक Color_ओळ *thiz = (स्थिरांक Color_ओळ *) color_ओळ_data;
    hb_paint_context_t *c = (hb_paint_context_t *) user_data;
    परत thiz->get_color_stops (c, start, मोजणी, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () स्थिरांक
  {
    निवड ((अचिन्हित पूर्णांक) extend)
    {
      स्थिती Extend::EXTEND_REPEAT:  परत HB_PAINT_EXTEND_REPEAT;
      स्थिती Extend::EXTEND_REFLECT: परत HB_PAINT_EXTEND_REFLECT;
      स्थिती Extend::EXTEND_PAD:
      पूर्वनिर्धारित:                     परत HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL स्थिर hb_paint_extend_t static_get_extend (hb_color_ओळ_t *color_ओळ,
							  शून्य *color_ओळ_data,
							  शून्य *user_data)
  {
    स्थिरांक Color_ओळ *thiz = (स्थिरांक Color_ओळ *) color_ओळ_data;
    परत thiz->get_extend ();
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  { c->अंक_var_idxes = 6; }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);
    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      स्वतः fold = [&] (F16DOT16 &target, F16DOT16 source, अचिन्हित offset) {
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
    return_trace (सत्य);
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    c->funcs->ढकला_transform (c->data,
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer HB_UNUSED) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    uint32_t first_layer_index = अंक_Layers ? c->plan->colrv1_layers.get (firstLayerIndex) : 0;
    return_trace (c->serializer->check_assign (out->firstLayerIndex, first_layer_index,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  ओळीत शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक;

  HBUINT8	format; /* format = 1 */
  HBUINT8	अंक_Layers;
  HBUINT32	firstLayerIndex;  /* index into COLRv1::layerList */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना PaintSolid
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक {}
  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    c->add_palette_index (paletteIndex);
    c->अंक_var_idxes = 1;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      दशांश a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    जर (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->paletteIndex, c->plan->colr_palettes.get (paletteIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिरांक
  {
    *color = c->get_color (paletteIndex,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
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

ढाँचा <ढाँचा<typename> वर्ग Var>
संरचना PaintLinearGradient
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    (हे+color_ओळ_2).closurev1 (c);
    c->अंक_var_idxes = 6;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुण) x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुण) y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुण) x1 + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुण) y1 + (द्विगुण) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((द्विगुण) x2 + (द्विगुण) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((द्विगुण) y2 + (द्विगुण) roundf (instancer (varIdxBase, 5)));
    }

    जर (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_ओळ_2.serialize_subset (c, color_ओळ_2, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && color_ओळ_2.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    hb_color_ओळ_t cl = {
      (शून्य *) &(हे+color_ओळ_2),
      (हे+color_ओळ_2).static_get_color_stops, c,
      (हे+color_ओळ_2).static_get_extend, nullptr
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
  Offset24To<Color_ओळ<Var>>	color_ओळ_2; /* Offset (from beginning of PaintLinearGradient
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

ढाँचा <ढाँचा<typename> वर्ग Var>
संरचना PaintRadialGradient
{
  तार्किक constant_after_pinning (hb_subset_context_t *c,
                               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    जर (c->plan->has_avar2) परत असत्य;
    स्थिरांक स्वतः &regions = instancer.varStore->get_region_list ();
    साठी (अचिन्हित i = 0; i < regions.region_मोजणी; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      जर (!regions.get_var_region (i, c->plan->axes_old_index_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        परत असत्य;
      }
      साठी (स्वतः axis : axes)
      {
        Triple *limit;
        जर (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          परत असत्य;
      }
    }
    परत सत्य;
  }

  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    (हे+color_ओळ_2).closurev1 (c);
    c->अंक_var_idxes = 6;
  }

  तार्किक subset_nonnegative (hb_subset_context_t *c,
                           स्थिरांक ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) स्थिरांक
  {
    द्विगुण source[6] = {
      x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)),
      y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)),
      radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)),
      x1 + (द्विगुण) roundf (instancer (varIdxBase, 3)),
      y1 + (द्विगुण) roundf (instancer (varIdxBase, 4)),
      radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5))
    };
    स्वतः transparent = [&] {
      स्वतः *out = c->serializer->allocate_size<PaintSolid> (5);
      जर (!out) परत असत्य;
      out->format = 2; out->paletteIndex = 0xFFFF; out->alpha.set_float (0);
      परत सत्य;
    };
    स्थिरांक स्वतः &ओळ = हे+color_ओळ_2;
    जर (source[2] == source[5] || !ओळ.stops.len) परत transparent ();
    संरचना stop_t { द्विगुण offset, alpha; अचिन्हित palette, order; };
    hb_vector_t<stop_t> stops;
    जर (!stops.alloc (ओळ.stops.len)) परत असत्य;
    अचिन्हित order = 0;
    साठी (स्थिरांक स्वतः &entry : ओळ.stops.iter ())
    {
      स्थिरांक स्वतः &stop = entry.get_मूल्य ();
      stops.push (stop_t {(द्विगुण) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (द्विगुण) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.paletteIndex, order++});
    }
    जर (stops.in_error ()) परत असत्य;
    stops.क्विक ([] (स्थिरांक stop_t &a, स्थिरांक stop_t &b) {
      परत a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    द्विगुण dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    अचिन्हित lattice = 16384;
    साठी (अचिन्हित j = 0; j < 3; j++)
    {
      द्विगुण remainder = भ_शेष (भ_असल (source[j + 3] - source[j]), 16384.);
      जर (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        परत असत्य;
      }
      अचिन्हित उजवा = (अचिन्हित) remainder;
      पर्यंत (उजवा) { अचिन्हित r = lattice % उजवा; lattice = उजवा; उजवा = r; }
    }
    साठी (अचिन्हित shift = 0; shift < 16; shift++)
    {
      द्विगुण b = 1u << shift;
      द्विगुण coarse = dr > 0 ? उच्चमान (-source[2] / dr) : न्यूनमान (source[2] / -dr) - b;
      द्विगुण fine = dr > 0 ? उच्चमान (-source[2] / dr * lattice) / lattice :
                            न्यूनमान (source[2] / -dr * lattice) / lattice - b;
      द्विगुण candidates[] = {coarse, fine};
      साठी (द्विगुण a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        द्विगुण geometry[6];
        तार्किक fits = सत्य;
        साठी (अचिन्हित j = 0; j < 3; j++)
        {
          द्विगुण difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          द्विगुण खालचा = j == 2 ? 0 : -32768, वरचा = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= खालचा && geometry[j] <= वरचा &&
                  geometry[j + 3] >= खालचा && geometry[j + 3] <= वरचा;
        }
        जर (!fits) सुरूठेवा;
        अचिन्हित first = 0, मोजणी = stops.length;
        द्विगुण min = (stops[0].offset - a) / b, max = (stops.tail ().offset - a) / b;
        द्विगुण bias = 0;
        तार्किक constant = असत्य;
        जर (ओळ.extend == Extend::EXTEND_PAD)
        {
          द्विगुण tip = -geometry[2] / (b * dr);
          जर ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.length - 1 : 0;
            मोजणी = 1; constant = सत्य;
          }
        }
        अन्यथा
        {
          द्विगुण period = max - min;
          जर (ओळ.extend == Extend::EXTEND_REFLECT) period *= 2;
          जर (period <= 0) परत transparent ();
          द्विगुण n = उच्चमान ((-2 - min) / period);
          जर (n > न्यूनमान ((32767. / 16384. - max) / period)) सुरूठेवा;
          bias = n * period;
        }
        साठी (अचिन्हित j = 0; j < मोजणी; j++)
        {
          द्विगुण bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        जर (!fits) सुरूठेवा;
        स्वतः *out = c->serializer->allocate_size<PaintRadialGradient<NoVariable>> (16);
        जर (!out) परत असत्य;
        out->format = 6;
        out->x0 = (पूर्णांक) geometry[0]; out->y0 = (पूर्णांक) geometry[1]; out->radius0 = (अचिन्हित) geometry[2];
        out->x1 = (पूर्णांक) geometry[3]; out->y1 = (पूर्णांक) geometry[4]; out->radius1 = (अचिन्हित) geometry[5];
        c->serializer->push ();
        स्वतः *out_ओळ = c->serializer->allocate_size<Color_ओळ<NoVariable>> (3 + मोजणी * ColorStop::static_size);
        जर (!out_ओळ) { c->serializer->काढा_discard (); परत असत्य; }
        out_ओळ->extend = ओळ.extend;
        out_ओळ->stops.len = मोजणी;
        साठी (अचिन्हित j = 0; j < मोजणी; j++)
        {
          स्थिरांक स्वतः &stop = stops[first + j];
          स्वतः &out_stop = out_ओळ->stops[j].value;
          out_stop.stopOffset.set_float ((दशांश) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((दशांश) stop.alpha);
          जर (!c->serializer->check_assign (out_stop.paletteIndex, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->काढा_discard (); परत असत्य; }
        }
        c->serializer->add_link (out->color_ओळ_2, c->serializer->काढा_pack ());
        परत !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    परत असत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुण) x0 + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुण) y0 + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((द्विगुण) radius0 + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुण) x1 + (द्विगुण) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुण) y1 + (द्विगुण) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((द्विगुण) radius1 + (द्विगुण) roundf (instancer (varIdxBase, 5)));
    }

    जर (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_ओळ_2.serialize_subset (c, color_ओळ_2, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && color_ओळ_2.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    hb_color_ओळ_t cl = {
      (शून्य *) &(हे+color_ओळ_2),
      (हे+color_ओळ_2).static_get_color_stops, c,
      (हे+color_ओळ_2).static_get_extend, nullptr
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
  Offset24To<Color_ओळ<Var>>	color_ओळ_2; /* Offset (from beginning of PaintRadialGradient
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

ढाँचा <ढाँचा<typename> वर्ग Var>
संरचना PaintSweepGradient
{
  शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक {}

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    (हे+color_ओळ_2).closurev1 (c);
    c->अंक_var_idxes = 4;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    जर (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_ओळ_2.serialize_subset (c, color_ओळ_2, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && color_ओळ_2.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    hb_color_ओळ_t cl = {
      (शून्य *) &(हे+color_ओळ_2),
      (हे+color_ओळ_2).static_get_color_stops, c,
      (हे+color_ओळ_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->data, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_ओळ<Var>>	color_ओळ_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size + 2 * F2DOT14::static_size);
};

// Paint a non-COLR glyph, filled as indicated by paint.
ढाँचा <typename Types>
संरचना PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (! c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    return_trace (out->paint.serialize_subset (c, paint, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && paint.sanitize (c, हे));
  }

  ओळीत शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + HBGlyphID::static_size);
};

संरचना PaintColrGlyph
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer HB_UNUSED) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  ओळीत शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (3);
};

ढाँचा <ढाँचा<typename> वर्ग Var>
संरचना PaintTransform
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);
    जर (!out->transform.serialize_subset (c, transform, हे, instancer)) return_trace (असत्य);
    जर (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  src.sanitize (c, हे) &&
                  transform.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक
  {
    TRACE_PAINT (हे);
    (हे+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (7);
};

संरचना PaintTranslate
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((द्विगुण) dx + (द्विगुण) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((द्विगुण) dy + (द्विगुण) roundf (instancer (varIdxBase, 1)));
    }

    जर (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश ddx = dx + c->instancer (varIdxBase, 0);
    दशांश ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->ढकला_translate (c->data, ddx, ddy);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 2 * FWORD::static_size);
};

ढाँचा <typename Source>
स्थिर तार्किक serialize_scale_transform (hb_subset_context_t *c,
                                       दशांश sx, दशांश sy, दशांश dx, दशांश dy,
                                       स्थिरांक Source &source)
{
  स्वतः *out = c->serializer->allocate_size<PaintTransform<NoVariable>> (7);
  जर (unlikely (!out)) परत असत्य;
  out->format = 12;

  c->serializer->push ();
  स्वतः *transform = c->serializer->allocate_size<Affine2x3> (Affine2x3::static_size);
  जर (unlikely (!transform))
  {
    c->serializer->काढा_discard ();
    परत असत्य;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->काढा_pack ());

  c->serializer->push ();
  तार्किक ret = source ();
  जर (ret)
    c->serializer->add_link (out->src, c->serializer->काढा_pack ());
  अन्यथा
    c->serializer->काढा_discard ();
  परत ret;
}

ढाँचा <typename Base>
स्थिर तार्किक subset_scale_as_transform (hb_subset_context_t *c,
                                      स्थिरांक Base *base,
                                      स्थिरांक Offset24To<Paint> &src,
                                      स्थिरांक ItemVarStoreInstancer &instancer,
                                      दशांश sx, दशांश sy,
                                      दशांश cx = 0, दशांश cy = 0)
{
  जर (src.is_null ()) परत असत्य;
  स्वतः source = [&] { परत c->dispatch (base+src, instancer); };
  दशांश dx = cx * (1 - sx), dy = cy * (1 - sy);
  जर (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    परत serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  स्वतः inner = [&] {
    परत serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  स्वतः inverse = [&] {
    परत serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  स्वतः scale = [&] {
    परत serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  स्वतः outer = [&] {
    परत serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  परत serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

संरचना PaintScale
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      दशांश sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      दशांश sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      जर (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, हे, src, instancer, sx, sy));
    }
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    जर (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    दशांश sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->ढकला_scale (c->data, sx, sy);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      दशांश sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      दशांश sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      जर (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, हे, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 3)));
    }

    जर (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    दशांश sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    दशांश tCenterX = centerX + c->instancer (varIdxBase, 2);
    दशांश tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ढकला_scale_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      दशांश s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      जर (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, हे, src, instancer, s, s));
    }
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    जर (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ढकला_scale (c->data, s, s);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

संरचना PaintScaleUniformAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      दशांश s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      जर (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, हे, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 2)));
    }

    जर (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश s = scale.to_float (c->instancer (varIdxBase, 0));
    दशांश tCenterX = centerX + c->instancer (varIdxBase, 1);
    दशांश tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ढकला_scale_around_center (c->data, s, s, tCenterX, tCenterY);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    जर (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->ढकला_rotate (c->data, a);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + F2DOT14::static_size);
};

संरचना PaintRotateAroundCenter
{
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 2)));
    }

    जर (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश a = angle.to_float (c->instancer (varIdxBase, 0));
    दशांश tCenterX = centerX + c->instancer (varIdxBase, 1);
    दशांश tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->ढकला_rotate_around_center (c->data, a, tCenterX, tCenterY);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    जर (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    दशांश sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->ढकला_skew (c->data, sx, sy);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुण) centerX + (द्विगुण) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुण) centerY + (द्विगुण) roundf (instancer (varIdxBase, 3)));
    }

    जर (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, हे, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && src.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिरांक
  {
    TRACE_PAINT (हे);
    दशांश sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    दशांश sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    दशांश tCenterX = centerX + c->instancer (varIdxBase, 2);
    दशांश tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->ढकला_skew_around_center (c->data, sx, sy, tCenterX, tCenterY);
    c->recurse (हे+src);
    c->funcs->काढा_transform (c->data);
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
  HB_INTERNAL शून्य dependv1 (hb_colrv1_depend_context_t* c) स्थिरांक;
  HB_INTERNAL शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक;

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = असत्य;
    ret |= out->src.serialize_subset (c, src, हे, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, हे, instancer);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  c->check_ops (हे->min_size) && // PainComposite can get exponential
                  src.sanitize (c, हे) &&
                  backdrop.sanitize (c, हे));
  }

  शून्य paint_glyph (hb_paint_context_t *c) स्थिरांक
  {
    TRACE_PAINT (हे);
    hb_paint_composite_mode_t composite_mode;
    निवड ((अचिन्हित पूर्णांक) mode)
    {
      स्थिती HB_PAINT_COMPOSITE_MODE_CLEAR:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_IN:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_IN:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      स्थिती HB_PAINT_COMPOSITE_MODE_XOR:
      स्थिती HB_PAINT_COMPOSITE_MODE_PLUS:
      स्थिती HB_PAINT_COMPOSITE_MODE_SCREEN:
      स्थिती HB_PAINT_COMPOSITE_MODE_OVERLAY:
      स्थिती HB_PAINT_COMPOSITE_MODE_DARKEN:
      स्थिती HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      स्थिती HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      स्थिती HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      स्थिती HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      स्थिती HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      स्थिती HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      स्थिती HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      स्थिती HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      स्थिती HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      स्थिती HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (अचिन्हित पूर्णांक) mode;
	थांबा;
      पूर्वनिर्धारित:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	थांबा;
    }

    c->funcs->ढकला_group_for (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (हे+backdrop);
    c->funcs->ढकला_group_for (c->data, composite_mode);
    c->recurse (हे+src);
    c->funcs->काढा_group (c->data, composite_mode);
    c->funcs->काढा_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
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
  पूर्णांक xMin, yMin, xMax, yMax;
};

संरचना ClipBoxFormat1
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य get_clip_box (ClipBoxData &clip_box, स्थिरांक ItemVarStoreInstancer &instancer HB_UNUSED) स्थिरांक
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((द्विगुण) xMin + (द्विगुण) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((द्विगुण) yMin + (द्विगुण) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((द्विगुण) xMax + (द्विगुण) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((द्विगुण) yMax + (द्विगुण) ceilf (instancer (varIdxBase, 3)));
    }

    जर (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (सत्य);
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
  शून्य get_clip_box (ClipBoxData &clip_box, स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    value.get_clip_box(clip_box, instancer);
    जर (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<पूर्णांक> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<पूर्णांक> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<पूर्णांक> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<पूर्णांक> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

संरचना ClipBox
{
  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    स्थिती 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  शून्य closurev1 (hb_colrv1_closure_context_t* c) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 2: hb_barrier (); u.format2.closurev1 (c); परत;
    पूर्वनिर्धारित:परत;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक get_extents (hb_glyph_extents_t *extents,
                    स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    ClipBoxData clip_box;
    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      थांबा;
    स्थिती 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      थांबा;
    पूर्वनिर्धारित:
      परत असत्य;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    परत सत्य;
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

संरचना ClipRecord
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  शून्य closurev1 (hb_colrv1_closure_context_t* c, स्थिरांक शून्य *base) स्थिरांक
  {
    जर (!c->glyphs->intersects (startGlyphID, endGlyphID)) परत;
    (base+clipBox).closurev1 (c);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक शून्य *base,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && clipBox.sanitize (c, base));
  }

  तार्किक get_extents (hb_glyph_extents_t *extents,
		    स्थिरांक शून्य *base,
		    स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    परत (base+clipBox).get_extents (extents, instancer);
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
  अचिन्हित serialize_clip_records (hb_subset_context_t *c,
                                   स्थिरांक ItemVarStoreInstancer &instancer,
                                   स्थिरांक hb_set_t& gids,
                                   स्थिरांक hb_map_t& gid_offset_map) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    जर (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    अचिन्हित मोजणी  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t prev_gid = start_gid;

    अचिन्हित offset = gid_offset_map.get (start_gid);
    अचिन्हित prev_offset = offset;
    साठी (स्थिरांक hb_codepoint_t _ : gids.iter ())
    {
      जर (_ == start_gid) सुरूठेवा;

      offset = gid_offset_map.get (_);
      जर (_ == prev_gid + 1 &&  offset == prev_offset)
      {
        prev_gid = _;
        सुरूठेवा;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = prev_gid;
      record.clipBox = prev_offset;

      जर (!record.subset (c, हे, instancer)) return_trace (0);
      मोजणी++;

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
      जर (!record.subset (c, हे, instancer)) return_trace (0);
      मोजणी++;
    }
    return_trace (मोजणी);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    जर (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);

    स्थिरांक hb_set_t& glyphset = c->plan->_glyphset_colred;
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    साठी (स्थिरांक ClipRecord& record : clips.iter ())
    {
      अचिन्हित start_gid = record.startGlyphID;
      अचिन्हित end_gid = record.endGlyphID;
      साठी (अचिन्हित gid = start_gid; gid <= end_gid; gid++)
      {
        जर (!glyphset.has (gid) || !glyph_map.has (gid)) सुरूठेवा;
        अचिन्हित new_gid = glyph_map.get (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    अचिन्हित मोजणी = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    जर (!मोजणी) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->clips.len, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (हे) && clips.sanitize (c, हे));
  }

  तार्किक
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    स्वतः *rec = clips.as_array ().द्वा_खोज (gid);
    जर (rec)
      परत rec->get_extents (extents, हे, instancer);
    परत असत्य;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (5, clips);
};

संरचना Paint
{

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    जर (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मूल्य ());

    return_trace (c->end_recursion (हे->dispatch (c, std::forward<Ts> (ds)...)));
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    स्थिती 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    स्थिती 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    स्थिती 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    स्थिती 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    स्थिती 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    स्थिती 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    स्थिती 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    स्थिती 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    स्थिती 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    स्थिती 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    स्थिती 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    स्थिती 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    स्थिती 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    स्थिती 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    स्थिती 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    स्थिती 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    स्थिती 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    स्थिती 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    स्थिती 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    स्थिती 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    स्थिती 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    स्थिती 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    स्थिती 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    स्थिती 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    स्थिती 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    स्थिती 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    स्थिती 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    स्थिती 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); परत सत्य;
    स्थिती 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); परत सत्य;
    पूर्वनिर्धारित: परत असत्य;
    }
  }

  संरक्षित:
  संघ {
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
  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  तार्किक serialize (hb_serialize_context_t *s, स्थिरांक hb_map_t* glyph_map,
                  स्थिरांक शून्य* src_base, hb_subset_context_t *c,
                  स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = s->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);
    जर (!s->check_assign (out->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) && paint.sanitize (c, base)));
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
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);
    स्थिरांक hb_set_t* glyphset = &c->plan->_glyphset_colred;

    साठी (स्थिरांक स्वतः& _ : as_array ())
    {
      अचिन्हित gid = _.glyphId;
      जर (!glyphset->has (gid)) सुरूठेवा;

      जर (_.serialize (c->serializer, c->plan->glyph_map, हे, c, instancer)) out->len++;
      अन्यथा return_trace (असत्य);
    }

    return_trace (out->len != 0);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, हे));
  }
};

संरचना LayerList : Array32OfOffset32To<Paint>
{
  स्थिरांक Paint& get_paint (अचिन्हित i) स्थिरांक
  { परत हे+(*हे)[i]; }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक ItemVarStoreInstancer &instancer) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    तार्किक ret = असत्य;
    साठी (स्थिरांक स्वतः& _ : + hb_enumerate (*हे)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      स्वतः *o = out->serialize_append (c->serializer);
      जर (unlikely (!o)) return_trace (असत्य);
      ret |= o->serialize_subset (c, _.second, हे, instancer);
    }
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, हे));
  }
};

संरचना delta_set_index_map_subset_plan_t
{
  अचिन्हित get_inner_bit_मोजणी () स्थिरांक { परत inner_bit_मोजणी; }
  अचिन्हित get_width ()           स्थिरांक { परत ((outer_bit_मोजणी + inner_bit_मोजणी + 7) / 8); }
  hb_array_t<स्थिरांक uint32_t> get_output_map () स्थिरांक { परत output_map.as_array (); }

  delta_set_index_map_subset_plan_t (स्थिरांक hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_मोजणी = 0;
    outer_bit_मोजणी = 0;
    inner_bit_मोजणी = 1;
    output_map.init ();

    /* search backwards */
    अचिन्हित मोजणी = new_deltaset_idx_varidx_map.get_population ();
    जर (!मोजणी) परत;

    अचिन्हित last_idx = (अचिन्हित)-1;
    अचिन्हित last_varidx = (अचिन्हित)-1;

    साठी (अचिन्हित i = मोजणी; i; i--)
    {
      अचिन्हित delta_set_idx = i - 1;
      अचिन्हित var_idx = new_deltaset_idx_varidx_map.get (delta_set_idx);
      जर (i == मोजणी)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        सुरूठेवा;
      }
      जर (var_idx != last_varidx)
        थांबा;
      last_idx = delta_set_idx;
    }

    map_मोजणी = last_idx + 1;
  }

  तार्किक remap (स्थिरांक hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_मोजणी = 1;
    inner_bit_मोजणी = 1;

    जर (unlikely (!output_map.resize_dirty (map_मोजणी))) परत असत्य;

    साठी (अचिन्हित idx = 0; idx < map_मोजणी; idx++)
    {
      uint32_t *var_idx;
      जर (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) परत असत्य;
      output_map.arrayZ[idx] = *var_idx;

      अचिन्हित outer = (*var_idx) >> 16;
      अचिन्हित bit_मोजणी = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_मोजणी = hb_max (bit_मोजणी, outer_bit_मोजणी);

      अचिन्हित inner = (*var_idx) & 0xFFFF;
      bit_मोजणी = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_मोजणी = hb_max (bit_मोजणी, inner_bit_मोजणी);
    }
    परत सत्य;
  }

  खाजगी:
  अचिन्हित map_मोजणी;
  अचिन्हित outer_bit_मोजणी;
  अचिन्हित inner_bit_मोजणी;
  hb_vector_t<uint32_t> output_map;
};

संरचना COLR
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  तार्किक has_data () स्थिरांक { परत has_v0_data () || version; }

  तार्किक has_v0_data () स्थिरांक { परत अंक_Base_Glyphs; }
  तार्किक has_v1_data () स्थिरांक
  {
    जर (version < 1)
      परत असत्य;
    hb_barrier ();

    परत (हे+baseGlyphList).len > 0;
  }

  अचिन्हित पूर्णांक get_glyph_layers (hb_codepoint_t       glyph,
				 अचिन्हित पूर्णांक         start_offset,
				 अचिन्हित पूर्णांक        *मोजणी, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) स्थिरांक
  {
    स्थिरांक BaseGlyphRecord &record = (हे+baseGlyphsZ).द्वा_खोज (अंक_Base_Glyphs, glyph);

    hb_array_t<स्थिरांक LayerRecord> all_layers = (हे+layersZ).as_array (अंक_Layers);
    hb_array_t<स्थिरांक LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.अंक_Layers);
    जर (मोजणी && layers)
    {
      + glyph_layers.sub_array (start_offset, मोजणी)
      | hb_sink (hb_array (layers, *मोजणी))
      ;
    }
    परत glyph_layers.length;
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      स्वतः *scratch = cached_scratch.get_relaxed ();
      जर (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    तार्किक has_data () स्थिरांक { परत colr->has_data (); }

#ifndef HB_NO_PAINT
    तार्किक
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) स्थिरांक
    {
      जर (unlikely (!has_data ())) परत असत्य;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      जर (unlikely (!scratch)) परत सत्य;
      तार्किक ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      परत ret;
    }

    तार्किक paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, शून्य *data,
		      अचिन्हित पूर्णांक palette_index,
		      hb_color_t foreground,
		      तार्किक clip = सत्य) स्थिरांक
    {
      जर (unlikely (!has_data ())) परत असत्य;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      जर (unlikely (!scratch)) परत सत्य;
      तार्किक ret = colr->paint_glyph (font, glyph, funcs, data, palette_index, foreground, clip, *scratch);
      release_scratch (scratch);
      परत ret;
    }
#endif

    तार्किक is_valid () { परत colr.get_blob ()->length; }

    शून्य depend (hb_depend_data_builder_t *depend_data) स्थिरांक
    {
      जर (!has_data ()) परत;
      colr->depend (depend_data);
    }

    शून्य closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) स्थिरांक
    { colr->closure_glyphs (glyph, related_ids); }

    शून्य closure_V0palette_indices (स्थिरांक hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) स्थिरांक
    { colr->closure_V0palette_indices (glyphs, palettes); }

    शून्य closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) स्थिरांक
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    तार्किक has_var_store () स्थिरांक
    { परत colr->has_var_store (); }

    स्थिरांक ItemVariationStore &get_var_store () स्थिरांक
    { परत colr->get_var_store (); }
    स्थिरांक ItemVariationStore *get_var_store_ptr () स्थिरांक
    { परत colr->get_var_store_ptr (); }

    तार्किक has_delta_set_index_map () स्थिरांक
    { परत colr->has_delta_set_index_map (); }

    स्थिरांक DeltaSetIndexMap &get_delta_set_index_map () स्थिरांक
    { परत colr->get_delta_set_index_map (); }
    स्थिरांक DeltaSetIndexMap *get_delta_set_index_map_ptr () स्थिरांक
    { परत colr->get_delta_set_index_map_ptr (); }

    खाजगी:

    hb_colr_scratch_t *acquire_scratch () स्थिरांक
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      जर (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, आकार (hb_colr_scratch_t));
	जर (unlikely (!scratch))
	  परत nullptr;
      }

      परत scratch;
    }
    शून्य release_scratch (hb_colr_scratch_t *scratch) स्थिरांक
    {
      जर (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    सार्वजनिक:
    hb_blob_ptr_t<COLR> colr;
    खाजगी:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  शून्य depend (hb_depend_data_builder_t *depend_data) स्थिरांक
  {
    // v0
    hb_array_t<स्थिरांक BaseGlyphRecord> baseGlyphs = (हे+baseGlyphsZ).as_array (अंक_Base_Glyphs);
    साठी (स्थिरांक BaseGlyphRecord &record : baseGlyphs)
    {
      स्वतः glyph_layers = (हे+layersZ).as_array (अंक_Layers).sub_array (record.firstLayerIdx,
                                                                         record.अंक_Layers);
      साठी (स्थिरांक LayerRecord layer : glyph_layers)
        depend_data->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    जर (!has_v1_data())
      परत;

    // v1
    hb_colrv1_depend_context_t c (हे, depend_data);
    स्थिरांक BaseGlyphList &baseglyph_paintrecords = हे+baseGlyphList;

    साठी (स्थिरांक BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.clear ();

      स्थिरांक Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  शून्य closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) स्थिरांक
  {
    स्थिरांक BaseGlyphRecord *record = get_base_glyph_record (glyph);
    जर (!record) परत;

    स्वतः glyph_layers = (हे+layersZ).as_array (अंक_Layers).sub_array (record->firstLayerIdx,
								       record->अंक_Layers);
    जर (!glyph_layers.length) परत;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.length, LayerRecord::min_size);
  }

  शून्य closure_V0palette_indices (स्थिरांक hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) स्थिरांक
  {
    जर (!अंक_Base_Glyphs || !अंक_Layers) परत;
    hb_array_t<स्थिरांक BaseGlyphRecord> baseGlyphs = (हे+baseGlyphsZ).as_array (अंक_Base_Glyphs);
    hb_array_t<स्थिरांक LayerRecord> all_layers = (हे+layersZ).as_array (अंक_Layers);

    साठी (स्थिरांक BaseGlyphRecord record : baseGlyphs)
    {
      जर (!glyphs->has (record.glyphId)) सुरूठेवा;
      hb_array_t<स्थिरांक LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.अंक_Layers);
      साठी (स्थिरांक LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  शून्य closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) स्थिरांक
  {
    जर (version < 1) परत;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (हे, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    स्थिरांक BaseGlyphList &baseglyph_paintrecords = हे+baseGlyphList;

    साठी (स्थिरांक BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      अचिन्हित gid = baseglyph_paintrecord.glyphId;
      जर (!glyphset->has (gid)) सुरूठेवा;

      स्थिरांक Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    स्थिरांक ClipList &cliplist = हे+clipList;
    c.glyphs = glyphset;
    साठी (स्थिरांक ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    जर (has_delta_set_index_map ())
    {
      स्थिरांक DeltaSetIndexMap &var_idx_map = हे+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->clear ();
      साठी (अचिन्हित delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  स्थिरांक LayerList& get_layerList () स्थिरांक
  { परत (हे+layerList); }

  स्थिरांक BaseGlyphList& get_baseglyphList () स्थिरांक
  { परत (हे+baseGlyphList); }

  तार्किक has_var_store () स्थिरांक
  { परत version >= 1 && hb_barrier () && varStore != 0; }

  तार्किक has_delta_set_index_map () स्थिरांक
  { परत version >= 1 && hb_barrier () && varIdxMap != 0; }

  तार्किक has_clip_list () स्थिरांक
  { परत version >= 1 && hb_barrier () && clipList != 0; }

  स्थिरांक DeltaSetIndexMap &get_delta_set_index_map () स्थिरांक
  { परत has_delta_set_index_map () && hb_barrier () ? हे+varIdxMap : Null (DeltaSetIndexMap); }
  स्थिरांक DeltaSetIndexMap *get_delta_set_index_map_ptr () स्थिरांक
  { परत has_delta_set_index_map () && hb_barrier () ? &(हे+varIdxMap) : nullptr; }

  स्थिरांक ItemVariationStore &get_var_store () स्थिरांक
  { परत has_var_store () && hb_barrier () ? हे+varStore : Null (ItemVariationStore); }
  स्थिरांक ItemVariationStore *get_var_store_ptr () स्थिरांक
  { परत has_var_store () && hb_barrier () ? &(हे+varStore) : nullptr; }

  स्थिरांक ClipList &get_clip_list () स्थिरांक
  { परत has_clip_list () && hb_barrier () ? हे+clipList : Null (ClipList); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
                  (हे+baseGlyphsZ).sanitize (c, अंक_Base_Glyphs) &&
                  (हे+layersZ).sanitize (c, अंक_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, हे) &&
		    layerList.sanitize (c, हे) &&
		    clipList.sanitize (c, हे) &&
		    varIdxMap.sanitize (c, हे) &&
		    varStore.sanitize (c, हे))));
  }

  ढाँचा<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  तार्किक serialize_V0 (hb_serialize_context_t *c,
		     अचिन्हित version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (base_it.len () != layer_it.len ()))
      return_trace (असत्य);

    हे->version = version;
    अंक_Layers = 0;
    अंक_Base_Glyphs = base_it.len ();
    जर (अंक_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (सत्य);
    }

    c->push ();
    साठी (स्थिरांक hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      स्वतः* record = c->embed (_);
      जर (unlikely (!record)) return_trace (असत्य);
      record->firstLayerIdx = अंक_Layers;
      अंक_Layers += record->अंक_Layers;
    }
    c->add_link (baseGlyphsZ, c->काढा_pack ());

    c->push ();
    साठी (स्थिरांक hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->काढा_pack ());

    return_trace (सत्य);
  }

  स्थिरांक BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) स्थिरांक
  {
    स्थिरांक BaseGlyphRecord* record = &(हे+baseGlyphsZ).द्वा_खोज (अंक_Base_Glyphs, (अचिन्हित पूर्णांक) gid);
    जर (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    परत record;
  }

  स्थिरांक BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) स्थिरांक
  {
    स्थिरांक BaseGlyphList &list = हे+baseGlyphList;
    अचिन्हित पूर्णांक i;
    जर (!list.bfind ((अचिन्हित) gid, &i))
      परत nullptr;
    परत &list[i];
  }

  तार्किक downgrade_to_V0 (स्थिरांक hb_set_t &glyphset) स्थिरांक
  {
    //no more COLRv1 glyphs, downgrade to version 0
    साठी (स्थिरांक BaseGlyphPaintRecord& _ : get_baseglyphList ())
      जर (glyphset.has (_.glyphId))
        परत असत्य;

    परत सत्य;
  }

  तार्किक subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (सत्य);

    स्थिरांक ItemVariationStore& var_store = हे+varStore;
    जर (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      तार्किक optimize = (varIdxMap != 0) ? सत्य : असत्य;
      जर (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (असत्य);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      जर (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (असत्य);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      जर (optimize)
      {
        स्थिरांक hb_map_t &varidx_map = item_vars.get_varidx_map ();
        साठी (स्वतः _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          जर (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          अन्यथा
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    अन्यथा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      जर (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_index_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      जर (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  तार्किक subset_delta_set_index_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (सत्य);

    स्थिरांक hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_index_map_subset_plan_t index_map_plan (deltaset_idx_varidx_map);

    जर (unlikely (!index_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (असत्य);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, index_map_plan));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    स्थिरांक hb_set_t& glyphset = c->plan->_glyphset_colred;

    स्वतः base_it =
    + hb_range (c->plan->अंक_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);
		    जर (glyphset.has (old_gid)) परत सत्य;
		    परत असत्य;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.get (new_gid);

				स्थिरांक BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				जर (unlikely (!old_record))
				  परत hb_pair_t<तार्किक, BaseGlyphRecord> (असत्य, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.अंक_Layers = old_record->अंक_Layers;
				परत hb_pair_t<तार्किक, BaseGlyphRecord> (सत्य, new_record);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    स्वतः layer_it =
    + hb_range (c->plan->अंक_output_glyphs ())
    | hb_map (reverse_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_sorting ([&](hb_codepoint_t old_gid)
			      {
				स्थिरांक BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				जर (unlikely (!old_record ||
					      old_record->firstLayerIdx >= अंक_Layers ||
					      old_record->firstLayerIdx + old_record->अंक_Layers > अंक_Layers))
				  परत hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (असत्य, out_layers);

				स्वतः layers = (हे+layersZ).as_array (अंक_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->अंक_Layers);
				out_layers.resize (layers.length);
				साठी (अचिन्हित पूर्णांक i = 0; i < layers.length; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  जर (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    परत hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (असत्य, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				परत hb_pair_t<तार्किक, hb_vector_t<LayerRecord>> (सत्य, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    जर (version == 0 && (!base_it || !layer_it))
      return_trace (असत्य);

    स्वतः *colr_मूळसंख्या = c->serializer->start_embed<COLR> ();
    जर (unlikely (!c->serializer->extend_min (colr_मूळसंख्या)))  return_trace (असत्य);

    जर (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_मूळसंख्या->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    जर (!c->serializer->allocate_size<शून्य> (5 * HBUINT32::static_size)) return_trace (असत्य);
    जर (!colr_मूळसंख्या->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (असत्य);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    जर (!subset_varstore (c, colr_मूळसंख्या)) return_trace (असत्य);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_index_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    जर (!colr_मूळसंख्या->baseGlyphList.serialize_subset (c, baseGlyphList, हे, instancer))
      return_trace (असत्य);

    colr_मूळसंख्या->layerList.serialize_subset (c, layerList, हे, instancer);
    colr_मूळसंख्या->clipList.serialize_subset (c, clipList, हे, instancer);

    return_trace (subset_delta_set_index_map (c, colr_मूळसंख्या));
  }

  स्थिरांक Paint *get_base_glyph_paint (hb_codepoint_t glyph) स्थिरांक
  {
    स्थिरांक BaseGlyphList &baseglyph_paintrecords = हे+baseGlyphList;
    स्थिरांक BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    जर (record)
    {
      स्थिरांक Paint &paint = &baseglyph_paintrecords+record->paint;
      परत &paint;
    }
    अन्यथा
      परत nullptr;
  }

#ifndef HB_NO_PAINT
  तार्किक
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) स्थिरांक
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_index_map_ptr (),
                                     hb_array (font->coords, font->अंक_coords));

    जर (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      परत सत्य;
    }

    स्वतः *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    तार्किक ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), सत्य, scratch);

    स्वतः e = scratch.paint_extents.get_extents ();
    जर (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    अन्यथा
      *extents = e.to_glyph_extents ();

    परत ret;
  }
#endif

  तार्किक
  has_paint_for_glyph (hb_codepoint_t glyph) स्थिरांक
  {
    जर (version >= 1)
    {
      hb_barrier ();

      स्थिरांक Paint *paint = get_base_glyph_paint (glyph);

      परत paint != nullptr;
    }

    परत असत्य;
  }

  तार्किक get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 स्थिरांक ItemVarStoreInstancer instancer) स्थिरांक
  {
    परत get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ifndef HB_NO_PAINT
  तार्किक
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, शून्य *data,
	       अचिन्हित पूर्णांक palette_index, hb_color_t foreground,
	       तार्किक clip,
	       hb_colr_scratch_t &scratch) स्थिरांक
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_index_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->अंक_coords : 0));
    hb_paint_context_t c (हे, funcs, data, font, palette_index, foreground, instancer);

    hb_decycler_गाठ_t node (c.glyphs_decycler);
    node.visit (glyph);

    जर (version >= 1)
    {
      hb_barrier ();

      स्थिरांक Paint *paint = get_base_glyph_paint (glyph);
      जर (paint)
      {
        // COLRv1 glyph

	तार्किक is_bounded = सत्य;
	hb_glyph_extents_t extents = {};
	जर (clip)
	{
	  जर (!get_clip (glyph, &extents, instancer))
	  {
	    clip = असत्य;
	    is_bounded = असत्य;
	  }

	  जर (!is_bounded)
	  {
	    स्वतः *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_index, foreground,
			 असत्य,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ढकला_font_transform (c.data, font);

	जर (clip)
	  c.funcs->ढकला_clip_rectangle (c.data,
					(दशांश) extents.x_bearing,
					(दशांश) extents.y_bearing + extents.height,
					(दशांश) extents.x_bearing + extents.width,
					(दशांश) extents.y_bearing);

	जर (is_bounded)
	  c.recurse (*paint);

	जर (clip)
	  c.funcs->काढा_clip (c.data);

	c.funcs->काढा_transform (c.data);

        परत सत्य;
      }
    }

    स्थिरांक BaseGlyphRecord *record = get_base_glyph_record (glyph);
    जर (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      साठी (स्थिरांक स्वतः &r : (हे+layersZ).as_array (अंक_Layers)
			   .sub_array (record->firstLayerIdx, record->अंक_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.data, r.glyphId, c.font, is_foreground, color);
      }

      परत सत्य;
    }

    परत असत्य;
  }
#endif

  संरक्षित:
  HBUINT16	version;	/* Table version number (starts at 0). */
  HBUINT16	अंक_Base_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32To<SortedUnsizedArrayOf<BaseGlyphRecord>>
		baseGlyphsZ;	/* Offset to Base Glyph records. */
  NNOffset32To<UnsizedArrayOf<LayerRecord>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	अंक_Layers;	/* Number of Layer Records. */
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
hb_paint_context_t::recurse (स्थिरांक Paint &paint)
{
  जर (unlikely (depth_डावा <= 0 || edge_मोजणी <= 0)) परत;
  depth_डावा--;
  edge_मोजणी--;
  paint.dispatch (हे);
  depth_डावा++;
}

ढाँचा <typename Types>
शून्य PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) स्थिरांक
{
  TRACE_PAINT (हे);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  जर (likely (c->depth_डावा > 0 && c->edge_मोजणी > 0) &&
      (हे+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_मोजणी--;
    c->funcs->ढकला_inverse_font_transform (c->data, c->font);
    c->funcs->fill_glyph (c->data, gid, c->font, is_foreground, color);
    c->funcs->काढा_transform (c->data);
    परत;
  }

  c->funcs->ढकला_inverse_font_transform (c->data, c->font);
  c->funcs->ढकला_clip_glyph (c->data, gid, c->font);
  c->funcs->ढकला_font_transform (c->data, c->font);
  c->recurse (हे+paint);
  c->funcs->काढा_transform (c->data);
  c->funcs->काढा_clip (c->data);
  c->funcs->काढा_transform (c->data);
}

शून्य PaintColrLayers::paint_glyph (hb_paint_context_t *c) स्थिरांक
{
  TRACE_PAINT (हे);
  स्थिरांक LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_गाठ_t node (c->layers_decycler);
  साठी (अचिन्हित i = firstLayerIndex; i < firstLayerIndex + अंक_Layers; i++)
  {
    जर (unlikely (!node.visit (i)))
      परत;

    स्थिरांक Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

शून्य PaintColrGlyph::paint_glyph (hb_paint_context_t *c) स्थिरांक
{
  TRACE_PAINT (हे);

  hb_decycler_गाठ_t node (c->glyphs_decycler);
  जर (unlikely (!node.visit (gid)))
    परत;

  c->funcs->ढकला_inverse_font_transform (c->data, c->font);
  जर (c->funcs->color_glyph (c->data, gid, c->font))
  {
    c->funcs->काढा_transform (c->data);
    परत;
  }
  c->funcs->काढा_transform (c->data);

  स्थिरांक COLR *colr_table = c->get_colr_table ();
  स्थिरांक Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  तार्किक has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  जर (has_clip_box)
    c->funcs->ढकला_clip_rectangle (c->data,
				   (दशांश) extents.x_bearing,
				   (दशांश) extents.y_bearing + extents.height,
				   (दशांश) extents.x_bearing + extents.width,
				   (दशांश) extents.y_bearing);

  जर (paint)
    c->recurse (*paint);

  जर (has_clip_box)
    c->funcs->काढा_clip (c->data);
}

} /* namespace OT */

#endif /* OT_COLOR_COLR_COLR_HH */
