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

#अगर_अघोषित OT_COLOR_COLR_COLR_HH
#घोषणा OT_COLOR_COLR_COLR_HH

#समावेश "../../../hb.hh"
#समावेश "../../../hb-decycler.hh"
#समावेश "../../../hb-open-type.hh"
#समावेश "../../../hb-ot-var-common.hh"
#समावेश "../../../hb-paint.hh"
#समावेश "../../../hb-paint-bounded.hh"
#समावेश "../../../hb-paint-extents.hh"
#समावेश "../../../hb-depend-data.hh"

#समावेश "../../../hb-ot-dual.hh"
#समावेश "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#घोषणा HB_OT_TAG_COLR HB_TAG('C','O','L','R')

namespace OT {
काष्ठा hb_paint_context_t;
}

काष्ठा hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

namespace OT {

काष्ठा COLR;

काष्ठा Paint;

काष्ठा hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  स्थिर अक्षर *get_name () { वापस "PAINT"; }
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.paint_glyph (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  स्थिर COLR* get_colr_table () स्थिर
  { वापस reinterpret_cast<स्थिर COLR *> (base); }

खुला:
  स्थिर व्योम *base;
  hb_paint_funcs_t *funcs;
  व्योम *तथ्य;
  hb_font_t *font;
  hb_array_t<स्थिर BGRAColor> palette;
  hb_color_t foreground;
  ItemVarStoreInstancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  पूर्णांक depth_बायाँ = HB_MAX_NESTING_LEVEL;
  पूर्णांक edge_गिनती = HB_MAX_GRAPH_EDGE_गिनती;

  hb_paint_context_t (स्थिर व्योम *base_,
		      hb_paint_funcs_t *funcs_,
		      व्योम *तथ्य_2,
                      hb_font_t *font_,
                      अचिन्हित पूर्णांक palette_,
                      hb_color_t foreground_,
		      ItemVarStoreInstancer &instancer_) :
    base (base_),
    funcs (funcs_),
    तथ्य (तथ्य_2),
    font (font_),
    palette (
#अगर_अघोषित HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->table.CPAL->get_palette_colors (palette_ < font->face->table.CPAL->get_palette_गिनती () ? palette_ : 0)
#पूर्ण_अगर
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    अगर (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, true);
      hb_font_set_synthetic_slant (font, 0);
    }
    अन्यथा
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  अटल अचिन्हित
  color_alpha (hb_color_t color, भग्न alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    वापस roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (अचिन्हित पूर्णांक color_सूचकांक, भग्न alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = true;

    अगर (color_सूचकांक != 0xffff)
    {
      अगर (!funcs->custom_palette_color (तथ्य, color_सूचकांक, &color))
        color = palette[color_सूचकांक];

      *is_foreground = false;
    }

    वापस HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  अंतरभूत व्योम recurse (स्थिर Paint &paint);
};

काष्ठा hb_colrv1_closure_context_t :
       hb_dispatch_context_t<hb_colrv1_closure_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj)
  {
    अगर (unlikely (nesting_level_बायाँ == 0))
      वापस hb_empty_t ();

    अगर (paint_visited (&obj))
      वापस hb_empty_t ();

    nesting_level_बायाँ--;
    obj.closurev1 (यह);
    nesting_level_बायाँ++;
    वापस hb_empty_t ();
  }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  bool paint_visited (स्थिर व्योम *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) base);
    अगर (visited_paint.in_error() || visited_paint.has (delta))
      वापस true;

    visited_paint.add (delta);
    वापस false;
  }

  स्थिर COLR* get_colr_table () स्थिर
  { वापस reinterpret_cast<स्थिर COLR *> (base); }

  व्योम add_glyph (अचिन्हित glyph_id)
  { glyphs->add (glyph_id); }

  व्योम add_layer_indices (अचिन्हित first_layer_सूचकांक, अचिन्हित अंक_of_layers)
  {
    अगर (अंक_of_layers == 0) वापस;
    layer_indices->add_range (first_layer_सूचकांक, first_layer_सूचकांक + अंक_of_layers - 1);
  }

  व्योम add_palette_सूचकांक (अचिन्हित palette_सूचकांक)
  { palette_indices->add (palette_सूचकांक); }

  व्योम add_var_idxes (अचिन्हित first_var_idx, अचिन्हित अंक_idxes)
  {
    अगर (!अंक_idxes || first_var_idx == VarIdx::NO_VARIATION) वापस;
    variation_indices->add_range (first_var_idx, first_var_idx + अंक_idxes - 1);
  }

  खुला:
  स्थिर व्योम *base;
  hb_set_t visited_paint;
  hb_set_t *glyphs;
  hb_set_t *layer_indices;
  hb_set_t *palette_indices;
  hb_set_t *variation_indices;
  अचिन्हित अंक_var_idxes;
  अचिन्हित nesting_level_बायाँ;

  hb_colrv1_closure_context_t (स्थिर व्योम *base_,
                               hb_set_t *glyphs_,
                               hb_set_t *layer_indices_,
                               hb_set_t *palette_indices_,
                               hb_set_t *variation_indices_,
                               अचिन्हित अंक_var_idxes_2 = 1,
                               अचिन्हित nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
                          base (base_),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          अंक_var_idxes (अंक_var_idxes_2),
                          nesting_level_बायाँ (nesting_level_बायाँ_2)
  {}
};


काष्ठा hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj)
  {
    अगर (unlikely (nesting_level_बायाँ == 0))
      वापस hb_empty_t ();

    अगर (paint_visited (&obj))
      वापस hb_empty_t ();

    nesting_level_बायाँ--;
    obj.dependv1 (यह);
    nesting_level_बायाँ++;
    वापस hb_empty_t ();
  }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  bool paint_visited (स्थिर व्योम *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    अगर (visited_paint.in_error() || visited_paint.has (delta))
      वापस true;

    visited_paint.add (delta);
    अगर (unlikely (visited_paint.in_error ()))
      depend_तथ्य->हार ();
    वापस false;
  }

  स्थिर COLR* get_colr_table () स्थिर
  { वापस colr; }

  खुला:
  स्थिर COLR *colr;
  hb_depend_तथ्य_builder_t *depend_तथ्य;
  hb_set_t visited_paint;
  अचिन्हित nesting_level_बायाँ;
  hb_codepoint_t source_gid;

  hb_colrv1_depend_context_t (स्थिर COLR *colr_,
                              hb_depend_तथ्य_builder_t *depend_तथ्य_2,
                              अचिन्हित nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_तथ्य (depend_तथ्य_2),
                          nesting_level_बायाँ (nesting_level_बायाँ_2),
                          source_gid (0)
  {}
};

काष्ठा LayerRecord
{
  चालक hb_ot_color_layer_t () स्थिर { वापस {glyphId, colorIdx}; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  सूचकांक		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा BaseGlyphRecord
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	firstLayerIdx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	अंक_Layers;	/* Number of color layers
				 * associated with this glyph */
  खुला:
  DEFINE_आकार_STATIC (6);
};

ढाँचा <typename T>
काष्ठा Variable
{
  अटल constexpr bool is_variable = true;

  स्थिर T& get_मूल्य () स्थिर { वापस value; }

  Variable<T>* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (यह));
  }

  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर
  { value.dependv1 (c); }

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    c->अंक_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->add_var_idxes (varIdxBase, c->अंक_var_idxes);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!value.subset (c, instancer, varIdxBase)) return_trace (false);
    अगर (c->plan->all_axes_pinned)
      return_trace (true);

    VarIdx new_varidx;
    new_varidx = varIdxBase;
    अगर (varIdxBase != VarIdx::NO_VARIATION)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      अगर (!c->plan->colrv1_variation_idx_delta_map.has (varIdxBase, &new_varidx_delta))
        return_trace (false);

      new_varidx = hb_first (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c));
  }

  व्योम paint_glyph (hb_paint_context_t *c) स्थिर
  {
    TRACE_PAINT (यह);
    value.paint_glyph (c, varIdxBase);
  }

  व्योम get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    value.get_color_stop (c, stop, varIdxBase, instancer);
  }

  व्योम get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिर
  {
    value.get_solid_color (c, varIdxBase, is_foreground, color);
  }

  hb_paint_extend_t get_extend () स्थिर
  {
    वापस value.get_extend ();
  }

  रक्षित:
  T      value;
  खुला:
  VarIdx varIdxBase;
  खुला:
  DEFINE_आकार_MIN (VarIdx::static_आकार + T::min_आकार);
};

ढाँचा <typename T>
काष्ठा NoVariable
{
  अटल constexpr bool is_variable = false;

  स्थिर T& get_मूल्य () स्थिर { वापस value; }

  अटल constexpr uint32_t varIdxBase = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (यह));
  }

  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर
  { value.dependv1 (c); }

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  { value.closurev1 (c); }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace (value.subset (c, instancer, varIdxBase));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c));
  }

  व्योम paint_glyph (hb_paint_context_t *c) स्थिर
  {
    TRACE_PAINT (यह);
    value.paint_glyph (c, varIdxBase);
  }

  व्योम get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  व्योम get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिर
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () स्थिर
  {
    वापस value.get_extend ();
  }

  T      value;
  खुला:
  DEFINE_आकार_MIN (T::min_आकार);
};

// Color structures

ढाँचा <typename Type, अचिन्हित fraction_bits>
अटल व्योम
set_fixed_clamped (HBFixed<Type, fraction_bits> &out, भग्न value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  out.set_int (hb_clamp_to<typename Type::type> (roundf ((value / HBFixed<Type, fraction_bits>::mult))));
}

काष्ठा ColorStop
{
  व्योम dependv1 (hb_colrv1_depend_context_t * c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    c->add_palette_सूचकांक (palette_सूचकांक_2);
    c->अंक_var_idxes = 2;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->stopOffset, stopOffset.to_float(instancer (varIdxBase, 0)));
      भग्न a = alpha.to_float (instancer (varIdxBase, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_assign (out->palette_सूचकांक_2, c->plan->colr_palettes.लो (palette_सूचकांक_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *out,
		       uint32_t varIdx,
		       स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    out->offset = stopOffset.to_float(instancer (varIdx, 0));
    out->color = c->get_color (palette_सूचकांक_2,
                               alpha.to_float (instancer (varIdx, 1)),
                               &out->is_foreground);
  }

  F2DOT14	stopOffset;
  HBUINT16	palette_सूचकांक_2;
  F2DOT14	alpha;
  खुला:
  DEFINE_आकार_STATIC (2 + 2 * F2DOT14::static_आकार);
};

काष्ठा Extend : HBUINT8
{
  क्रमागत {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  खुला:
  DEFINE_आकार_STATIC (1);
};

ढाँचा <ढाँचा<typename> श्रेणी Var>
काष्ठा Color_पंक्ति
{
  व्योम dependv1 (hb_colrv1_depend_context_t * c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    क्रम (स्थिर स्वतः &stop : stops.iter ())
      stop.closurev1 (c);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    अगर (!c->serializer->check_assign (out->extend, extend, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);
    अगर (!c->serializer->check_assign (out->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)) return_trace (false);

    क्रम (स्थिर स्वतः& stop : stops.iter ())
    {
      अगर (!stop.subset (c, instancer)) return_trace (false);
    }
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  अचिन्हित पूर्णांक
  get_color_stops (hb_paint_context_t *c,
                   अचिन्हित पूर्णांक start,
		   अचिन्हित पूर्णांक *गिनती,
		   hb_color_stop_t *color_stops,
		   स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    अचिन्हित पूर्णांक len = stops.len;

    अगर (गिनती && color_stops)
    {
      अचिन्हित पूर्णांक i;
      क्रम (i = 0; i < *गिनती && start + i < len; i++)
        stops[start + i].get_color_stop (c, &color_stops[i], instancer);
      *गिनती = i;
    }

    वापस len;
  }

  HB_INTERNAL अटल अचिन्हित पूर्णांक static_get_color_stops (hb_color_पंक्ति_t *color_पंक्ति,
							  व्योम *color_पंक्ति_तथ्य,
							  अचिन्हित पूर्णांक start,
							  अचिन्हित पूर्णांक *गिनती,
							  hb_color_stop_t *color_stops,
							  व्योम *user_तथ्य)
  {
    स्थिर Color_पंक्ति *thiz = (स्थिर Color_पंक्ति *) color_पंक्ति_तथ्य;
    hb_paint_context_t *c = (hb_paint_context_t *) user_तथ्य;
    वापस thiz->get_color_stops (c, start, गिनती, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () स्थिर
  {
    चयन ((अचिन्हित पूर्णांक) extend)
    {
      हाल Extend::EXTEND_REPEAT:  वापस HB_PAINT_EXTEND_REPEAT;
      हाल Extend::EXTEND_REFLECT: वापस HB_PAINT_EXTEND_REFLECT;
      हाल Extend::EXTEND_PAD:
      शेष:                     वापस HB_PAINT_EXTEND_PAD;
    }
  }

  HB_INTERNAL अटल hb_paint_extend_t static_get_extend (hb_color_पंक्ति_t *color_पंक्ति,
							  व्योम *color_पंक्ति_तथ्य,
							  व्योम *user_तथ्य)
  {
    स्थिर Color_पंक्ति *thiz = (स्थिर Color_पंक्ति *) color_पंक्ति_तथ्य;
    वापस thiz->get_extend ();
  }

  Extend	extend;
  Array16Of<Var<ColorStop>>	stops;
  खुला:
  DEFINE_आकार_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
काष्ठा CompositeMode : HBUINT8
{
  खुला:
  DEFINE_आकार_STATIC (1);
};

काष्ठा Affine2x3
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  { c->अंक_var_idxes = 6; }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);
    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      स्वतः fold = [&] (F16DOT16 &target, F16DOT16 source, अचिन्हित offset) {
        // Keep the stored integer precise while applying the float delta.
        target.set_int (hb_clamp_to<int32_t> (roundf ((द्विगुन) source.to_int () +
                                                    (द्विगुन) instancer (varIdxBase, offset))));
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

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    c->funcs->धकेलो_transform (c->तथ्य,
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
  खुला:
  DEFINE_आकार_STATIC (6 * F16DOT16::static_आकार);
};

काष्ठा PaintColrLayers
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer HB_UNUSED) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    uint32_t first_layer_सूचकांक = अंक_Layers ? c->plan->colrv1_layers.लो (first_Layer_सूचकांक) : 0;
    return_trace (c->serializer->check_assign (out->first_Layer_सूचकांक, first_layer_सूचकांक,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  अंतरभूत व्योम paint_glyph (hb_paint_context_t *c) स्थिर;

  HBUINT8	format; /* format = 1 */
  HBUINT8	अंक_Layers;
  HBUINT32	first_Layer_सूचकांक;  /* index into COLRv1::layerList */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा PaintSolid
{
  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर {}
  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    c->add_palette_सूचकांक (palette_सूचकांक_2);
    c->अंक_var_idxes = 1;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भग्न a = alpha.to_float (instancer (varIdxBase, 0));
      set_fixed_clamped (out->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    अगर (format == 3 && c->plan->all_axes_pinned)
        out->format = 2;

    return_trace (c->serializer->check_assign (out->palette_सूचकांक_2, c->plan->colr_palettes.लो (palette_सूचकांक_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_solid_color (hb_paint_context_t *c, uint32_t varIdxBase,
                        hb_bool_t *is_foreground, hb_color_t *color) स्थिर
  {
    *color = c->get_color (palette_सूचकांक_2,
                           alpha.to_float (c->instancer (varIdxBase, 0)),
                           is_foreground);
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, varIdxBase, &is_foreground, &color);
    c->funcs->color (c->तथ्य, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_सूचकांक_2;
  F2DOT14	alpha;
  खुला:
  DEFINE_आकार_STATIC (3 + F2DOT14::static_आकार);
};

ढाँचा <ढाँचा<typename> श्रेणी Var>
काष्ठा PaintLinearGradient
{
  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    (यह+color_पंक्ति_2).closurev1 (c);
    c->अंक_var_idxes = 6;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुन) x0 + (द्विगुन) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुन) y0 + (द्विगुन) roundf (instancer (varIdxBase, 1)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुन) x1 + (द्विगुन) roundf (instancer (varIdxBase, 2)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुन) y1 + (द्विगुन) roundf (instancer (varIdxBase, 3)));
      out->x2 = hb_clamp_to<int16_t> ((द्विगुन) x2 + (द्विगुन) roundf (instancer (varIdxBase, 4)));
      out->y2 = hb_clamp_to<int16_t> ((द्विगुन) y2 + (द्विगुन) roundf (instancer (varIdxBase, 5)));
    }

    अगर (format == 5 && c->plan->all_axes_pinned)
        out->format = 4;

    return_trace (out->color_पंक्ति_2.serialize_subset (c, color_पंक्ति_2, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && color_पंक्ति_2.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    hb_color_पंक्ति_t cl = {
      (व्योम *) &(यह+color_पंक्ति_2),
      (यह+color_पंक्ति_2).static_get_color_stops, c,
      (यह+color_पंक्ति_2).static_get_extend, nullptr
    };

    c->funcs->linear_gradient (c->तथ्य, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       x1 + c->instancer (varIdxBase, 2),
			       y1 + c->instancer (varIdxBase, 3),
			       x2 + c->instancer (varIdxBase, 4),
			       y2 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24To<Color_पंक्ति<Var>>	color_पंक्ति_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  खुला:
  DEFINE_आकार_STATIC (4 + 6 * FWORD::static_आकार);
};

ढाँचा <ढाँचा<typename> श्रेणी Var>
काष्ठा PaintRadialGradient
{
  bool constant_after_pinning (hb_subset_context_t *c,
                               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    अगर (c->plan->has_avar2) वापस false;
    स्थिर स्वतः &regions = instancer.varStore->get_region_list ();
    क्रम (अचिन्हित i = 0; i < regions.region_गिनती; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axes;
      अगर (!regions.get_var_region (i, c->plan->axes_old_सूचकांक_tag_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_OUT_OF_ROOM);
        वापस false;
      }
      क्रम (स्वतः axis : axes)
      {
        Triple *limit;
        अगर (!c->plan->user_axes_location.has (axis.first, &limit) ||
            limit->minimum != limit->maximum)
          वापस false;
      }
    }
    वापस true;
  }

  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    (यह+color_पंक्ति_2).closurev1 (c);
    c->अंक_var_idxes = 6;
  }

  bool subset_nonnegative (hb_subset_context_t *c,
                           स्थिर ItemVarStoreInstancer &instancer,
                           uint32_t varIdxBase) स्थिर
  {
    द्विगुन source[6] = {
      x0 + (द्विगुन) roundf (instancer (varIdxBase, 0)),
      y0 + (द्विगुन) roundf (instancer (varIdxBase, 1)),
      radius0 + (द्विगुन) roundf (instancer (varIdxBase, 2)),
      x1 + (द्विगुन) roundf (instancer (varIdxBase, 3)),
      y1 + (द्विगुन) roundf (instancer (varIdxBase, 4)),
      radius1 + (द्विगुन) roundf (instancer (varIdxBase, 5))
    };
    स्वतः transparent = [&] {
      स्वतः *out = c->serializer->allocate_आकार<PaintSolid> (5);
      अगर (!out) वापस false;
      out->format = 2; out->palette_सूचकांक_2 = 0xFFFF; out->alpha.set_float (0);
      वापस true;
    };
    स्थिर स्वतः &पंक्ति = यह+color_पंक्ति_2;
    अगर (source[2] == source[5] || !पंक्ति.stops.len) वापस transparent ();
    काष्ठा stop_t { द्विगुन offset, alpha; अचिन्हित palette, order; };
    hb_vector_t<stop_t> stops;
    अगर (!stops.alloc (पंक्ति.stops.len)) वापस false;
    अचिन्हित order = 0;
    क्रम (स्थिर स्वतः &entry : पंक्ति.stops.iter ())
    {
      स्थिर स्वतः &stop = entry.get_मूल्य ();
      stops.push (stop_t {(द्विगुन) roundf (stop.stopOffset.to_float (instancer (entry.varIdxBase, 0)) * 16384.f) / 16384.,
                   (द्विगुन) hb_clamp (stop.alpha.to_float (instancer (entry.varIdxBase, 1)), 0.f, 1.f),
                   stop.palette_सूचकांक_2, order++});
    }
    अगर (stops.in_error ()) वापस false;
    stops.क्विक ([] (स्थिर stop_t &a, स्थिर stop_t &b) {
      वापस a.offset < b.offset ? -1 : a.offset > b.offset ? 1 :
             a.order < b.order ? -1 : a.order > b.order ? 1 : 0;
    });
    द्विगुन dr = source[5] - source[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    अचिन्हित lattice = 16384;
    क्रम (अचिन्हित j = 0; j < 3; j++)
    {
      द्विगुन remainder = भ_शेष (भ_असल (source[j + 3] - source[j]), 16384.);
      अगर (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
        वापस false;
      }
      अचिन्हित दाँए = (अचिन्हित) remainder;
      जबतक (दाँए) { अचिन्हित r = lattice % दाँए; lattice = दाँए; दाँए = r; }
    }
    क्रम (अचिन्हित shift = 0; shift < 16; shift++)
    {
      द्विगुन b = 1u << shift;
      द्विगुन coarse = dr > 0 ? उच्चमान (-source[2] / dr) : न्यूनमान (source[2] / -dr) - b;
      द्विगुन fine = dr > 0 ? उच्चमान (-source[2] / dr * lattice) / lattice :
                            न्यूनमान (source[2] / -dr * lattice) / lattice - b;
      द्विगुन candidates[] = {coarse, fine};
      क्रम (द्विगुन a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        द्विगुन geometry[6];
        bool fits = true;
        क्रम (अचिन्हित j = 0; j < 3; j++)
        {
          द्विगुन difference = source[j + 3] - source[j];
          geometry[j] = source[j] + a * difference;
          geometry[j + 3] = source[j] + (a + b) * difference;
          द्विगुन निचला = j == 2 ? 0 : -32768, ऊपरी = j == 2 ? 65535 : 32767;
          fits &= geometry[j] >= निचला && geometry[j] <= ऊपरी &&
                  geometry[j + 3] >= निचला && geometry[j + 3] <= ऊपरी;
        }
        अगर (!fits) जारी;
        अचिन्हित first = 0, गिनती = stops.लंबाई;
        द्विगुन min = (stops[0].offset - a) / b, max = (stops.पुच्छ ().offset - a) / b;
        द्विगुन bias = 0;
        bool constant = false;
        अगर (पंक्ति.extend == Extend::EXTEND_PAD)
        {
          द्विगुन tip = -geometry[2] / (b * dr);
          अगर ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            first = dr > 0 ? stops.लंबाई - 1 : 0;
            गिनती = 1; constant = true;
          }
        }
        अन्यथा
        {
          द्विगुन period = max - min;
          अगर (पंक्ति.extend == Extend::EXTEND_REFLECT) period *= 2;
          अगर (period <= 0) वापस transparent ();
          द्विगुन n = उच्चमान ((-2 - min) / period);
          अगर (n > न्यूनमान ((32767. / 16384. - max) / period)) जारी;
          bias = n * period;
        }
        क्रम (अचिन्हित j = 0; j < गिनती; j++)
        {
          द्विगुन bits = constant ? 0 : ((stops[first + j].offset - a) / b + bias) * 16384;
          fits &= bits >= -32768 && bits <= 32767 && bits == round (bits);
        }
        अगर (!fits) जारी;
        स्वतः *out = c->serializer->allocate_आकार<PaintRadialGradient<NoVariable>> (16);
        अगर (!out) वापस false;
        out->format = 6;
        out->x0 = (पूर्णांक) geometry[0]; out->y0 = (पूर्णांक) geometry[1]; out->radius0 = (अचिन्हित) geometry[2];
        out->x1 = (पूर्णांक) geometry[3]; out->y1 = (पूर्णांक) geometry[4]; out->radius1 = (अचिन्हित) geometry[5];
        c->serializer->push ();
        स्वतः *out_पंक्ति = c->serializer->allocate_आकार<Color_पंक्ति<NoVariable>> (3 + गिनती * ColorStop::static_आकार);
        अगर (!out_पंक्ति) { c->serializer->निकालो_discard (); वापस false; }
        out_पंक्ति->extend = पंक्ति.extend;
        out_पंक्ति->stops.len = गिनती;
        क्रम (अचिन्हित j = 0; j < गिनती; j++)
        {
          स्थिर स्वतः &stop = stops[first + j];
          स्वतः &out_stop = out_पंक्ति->stops[j].value;
          out_stop.stopOffset.set_float ((भग्न) (constant ? 0 : (stop.offset - a) / b + bias));
          out_stop.alpha.set_float ((भग्न) stop.alpha);
          अगर (!c->serializer->check_assign (out_stop.palette_सूचकांक_2, c->plan->colr_palettes.लो (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
          { c->serializer->निकालो_discard (); वापस false; }
        }
        c->serializer->add_link (out->color_पंक्ति_2, c->serializer->निकालो_pack ());
        वापस !c->serializer->in_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_OVERFLOW);
    वापस false;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        varIdxBase != VarIdx::NO_VARIATION &&
        (radius0 + (द्विगुन) roundf (instancer (varIdxBase, 2)) < 0 ||
         radius1 + (द्विगुन) roundf (instancer (varIdxBase, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, varIdxBase));
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->x0 = hb_clamp_to<int16_t> ((द्विगुन) x0 + (द्विगुन) roundf (instancer (varIdxBase, 0)));
      out->y0 = hb_clamp_to<int16_t> ((द्विगुन) y0 + (द्विगुन) roundf (instancer (varIdxBase, 1)));
      out->radius0 = hb_clamp_to<uint16_t> ((द्विगुन) radius0 + (द्विगुन) roundf (instancer (varIdxBase, 2)));
      out->x1 = hb_clamp_to<int16_t> ((द्विगुन) x1 + (द्विगुन) roundf (instancer (varIdxBase, 3)));
      out->y1 = hb_clamp_to<int16_t> ((द्विगुन) y1 + (द्विगुन) roundf (instancer (varIdxBase, 4)));
      out->radius1 = hb_clamp_to<uint16_t> ((द्विगुन) radius1 + (द्विगुन) roundf (instancer (varIdxBase, 5)));
    }

    अगर (format == 7 && c->plan->all_axes_pinned)
        out->format = 6;

    return_trace (out->color_पंक्ति_2.serialize_subset (c, color_पंक्ति_2, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && color_पंक्ति_2.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    hb_color_पंक्ति_t cl = {
      (व्योम *) &(यह+color_पंक्ति_2),
      (यह+color_पंक्ति_2).static_get_color_stops, c,
      (यह+color_पंक्ति_2).static_get_extend, nullptr
    };

    c->funcs->radial_gradient (c->तथ्य, &cl,
			       x0 + c->instancer (varIdxBase, 0),
			       y0 + c->instancer (varIdxBase, 1),
			       radius0 + c->instancer (varIdxBase, 2),
			       x1 + c->instancer (varIdxBase, 3),
			       y1 + c->instancer (varIdxBase, 4),
			       radius1 + c->instancer (varIdxBase, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24To<Color_पंक्ति<Var>>	color_पंक्ति_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  खुला:
  DEFINE_आकार_STATIC (4 + 6 * FWORD::static_आकार);
};

ढाँचा <ढाँचा<typename> श्रेणी Var>
काष्ठा PaintSweepGradient
{
  व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर {}

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    (यह+color_पंक्ति_2).closurev1 (c);
    c->अंक_var_idxes = 4;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->centerX = hb_clamp_to<int16_t> ((द्विगुन) centerX + (द्विगुन) roundf (instancer (varIdxBase, 0)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुन) centerY + (द्विगुन) roundf (instancer (varIdxBase, 1)));
      set_fixed_clamped (out->startAngle, startAngle.to_float (instancer (varIdxBase, 2)));
      set_fixed_clamped (out->endAngle, endAngle.to_float (instancer (varIdxBase, 3)));
    }

    अगर (format == 9 && c->plan->all_axes_pinned)
        out->format = 8;

    return_trace (out->color_पंक्ति_2.serialize_subset (c, color_पंक्ति_2, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && color_पंक्ति_2.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    hb_color_पंक्ति_t cl = {
      (व्योम *) &(यह+color_पंक्ति_2),
      (यह+color_पंक्ति_2).static_get_color_stops, c,
      (यह+color_पंक्ति_2).static_get_extend, nullptr
    };

    c->funcs->sweep_gradient (c->तथ्य, &cl,
			      centerX + c->instancer (varIdxBase, 0),
			      centerY + c->instancer (varIdxBase, 1),
                              (startAngle.to_float (c->instancer (varIdxBase, 2)) + 1) * HB_PI,
                              (endAngle.to_float   (c->instancer (varIdxBase, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24To<Color_पंक्ति<Var>>	color_पंक्ति_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		startAngle;
  F2DOT14		endAngle;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * FWORD::static_आकार + 2 * F2DOT14::static_आकार);
};

// Paint a non-COLR glyph, filled as indicated by paint.
ढाँचा <typename Types>
काष्ठा PaintGlyph
{
  using HBGlyphID = typename Types::HBGlyphID;

  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (! c->serializer->check_assign (out->gid, c->plan->glyph_map->लो (gid),
                                       HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    return_trace (out->paint.serialize_subset (c, paint, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && paint.sanitize (c, यह));
  }

  अंतरभूत व्योम paint_glyph (hb_paint_context_t *c) स्थिर;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24To<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  खुला:
  DEFINE_आकार_STATIC (4 + HBGlyphID::static_आकार);
};

काष्ठा PaintColrGlyph
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer HB_UNUSED) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    return_trace (c->serializer->check_assign (out->gid, c->plan->glyph_map->लो (gid),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  अंतरभूत व्योम paint_glyph (hb_paint_context_t *c) स्थिर;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  खुला:
  DEFINE_आकार_STATIC (3);
};

ढाँचा <ढाँचा<typename> श्रेणी Var>
काष्ठा PaintTransform
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);
    अगर (!out->transform.serialize_subset (c, transform, यह, instancer)) return_trace (false);
    अगर (format == 13 && c->plan->all_axes_pinned)
      out->format = 12;
    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  src.sanitize (c, यह) &&
                  transform.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c) स्थिर
  {
    TRACE_PAINT (यह);
    (यह+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24To<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24To<Var<Affine2x3>>	transform;
  खुला:
  DEFINE_आकार_STATIC (7);
};

काष्ठा PaintTranslate
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->dx = hb_clamp_to<int16_t> ((द्विगुन) dx + (द्विगुन) roundf (instancer (varIdxBase, 0)));
      out->dy = hb_clamp_to<int16_t> ((द्विगुन) dy + (द्विगुन) roundf (instancer (varIdxBase, 1)));
    }

    अगर (format == 15 && c->plan->all_axes_pinned)
        out->format = 14;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न ddx = dx + c->instancer (varIdxBase, 0);
    भग्न ddy = dy + c->instancer (varIdxBase, 1);

    c->funcs->धकेलो_translate (c->तथ्य, ddx, ddy);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * FWORD::static_आकार);
};

ढाँचा <typename Source>
अटल bool serialize_scale_transform (hb_subset_context_t *c,
                                       भग्न sx, भग्न sy, भग्न dx, भग्न dy,
                                       स्थिर Source &source)
{
  स्वतः *out = c->serializer->allocate_आकार<PaintTransform<NoVariable>> (7);
  अगर (unlikely (!out)) वापस false;
  out->format = 12;

  c->serializer->push ();
  स्वतः *transform = c->serializer->allocate_आकार<Affine2x3> (Affine2x3::static_आकार);
  अगर (unlikely (!transform))
  {
    c->serializer->निकालो_discard ();
    वापस false;
  }
  set_fixed_clamped (transform->xx, sx);
  transform->yx.set_float (0);
  transform->xy.set_float (0);
  set_fixed_clamped (transform->yy, sy);
  set_fixed_clamped (transform->dx, dx);
  set_fixed_clamped (transform->dy, dy);
  c->serializer->add_link (out->transform, c->serializer->निकालो_pack ());

  c->serializer->push ();
  bool ret = source ();
  अगर (ret)
    c->serializer->add_link (out->src, c->serializer->निकालो_pack ());
  अन्यथा
    c->serializer->निकालो_discard ();
  वापस ret;
}

ढाँचा <typename Base>
अटल bool subset_scale_as_transform (hb_subset_context_t *c,
                                      स्थिर Base *base,
                                      स्थिर Offset24To<Paint> &src,
                                      स्थिर ItemVarStoreInstancer &instancer,
                                      भग्न sx, भग्न sy,
                                      भग्न cx = 0, भग्न cy = 0)
{
  अगर (src.is_null ()) वापस false;
  स्वतः source = [&] { वापस c->dispatch (base+src, instancer); };
  भग्न dx = cx * (1 - sx), dy = cy * (1 - sy);
  अगर (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    वापस serialize_scale_transform (c, sx, sy, dx, dy, source);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  स्वतः inner = [&] {
    वापस serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, source);
  };
  स्वतः inverse = [&] {
    वापस serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  स्वतः scale = [&] {
    वापस serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  स्वतः outer = [&] {
    वापस serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  वापस serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

काष्ठा PaintScale
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भग्न sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      भग्न sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      अगर (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, यह, src, instancer, sx, sy));
    }
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
    }

    अगर (format == 17 && c->plan->all_axes_pinned)
        out->format = 16;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    भग्न sy = scaleY.to_float (c->instancer (varIdxBase, 1));

    c->funcs->धकेलो_scale (c->तथ्य, sx, sy);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * F2DOT14::static_आकार);
};

काष्ठा PaintScaleAroundCenter
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भग्न sx = roundf (scaleX.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      भग्न sy = roundf (scaleY.to_float (instancer (varIdxBase, 1)) * 16384) / 16384;
      अगर (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, यह, src, instancer, sx, sy,
            centerX + roundf (instancer (varIdxBase, 2)),
            centerY + roundf (instancer (varIdxBase, 3))));
    }
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scaleX, scaleX.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->scaleY, scaleY.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुन) centerX + (द्विगुन) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुन) centerY + (द्विगुन) roundf (instancer (varIdxBase, 3)));
    }

    अगर (format == 19 && c->plan->all_axes_pinned)
        out->format = 18;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न sx = scaleX.to_float (c->instancer (varIdxBase, 0));
    भग्न sy = scaleY.to_float (c->instancer (varIdxBase, 1));
    भग्न tCenterX = centerX + c->instancer (varIdxBase, 2);
    भग्न tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->धकेलो_scale_around_center (c->तथ्य, sx, sy, tCenterX, tCenterY);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * F2DOT14::static_आकार + 2 * FWORD::static_आकार);
};

काष्ठा PaintScaleUniform
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भग्न s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      अगर (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, यह, src, instancer, s, s));
    }
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));

    अगर (format == 21 && c->plan->all_axes_pinned)
        out->format = 20;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न s = scale.to_float (c->instancer (varIdxBase, 0));

    c->funcs->धकेलो_scale (c->तथ्य, s, s);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  खुला:
  DEFINE_आकार_STATIC (4 + F2DOT14::static_आकार);
};

काष्ठा PaintScaleUniformAroundCenter
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      भग्न s = roundf (scale.to_float (instancer (varIdxBase, 0)) * 16384) / 16384;
      अगर (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, यह, src, instancer, s, s,
            centerX + roundf (instancer (varIdxBase, 1)),
            centerY + roundf (instancer (varIdxBase, 2))));
    }
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->scale, scale.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुन) centerX + (द्विगुन) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुन) centerY + (द्विगुन) roundf (instancer (varIdxBase, 2)));
    }

    अगर (format == 23 && c->plan->all_axes_pinned)
        out->format = 22;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न s = scale.to_float (c->instancer (varIdxBase, 0));
    भग्न tCenterX = centerX + c->instancer (varIdxBase, 1);
    भग्न tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->धकेलो_scale_around_center (c->तथ्य, s, s, tCenterX, tCenterY);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  खुला:
  DEFINE_आकार_STATIC (4 + F2DOT14::static_आकार + 2 * FWORD::static_आकार);
};

काष्ठा PaintRotate
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));

    अगर (format == 25 && c->plan->all_axes_pinned)
      out->format = 24;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न a = angle.to_float (c->instancer (varIdxBase, 0));

    c->funcs->धकेलो_rotate (c->तथ्य, a);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  खुला:
  DEFINE_आकार_STATIC (4 + F2DOT14::static_आकार);
};

काष्ठा PaintRotateAroundCenter
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->angle, angle.to_float (instancer (varIdxBase, 0)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुन) centerX + (द्विगुन) roundf (instancer (varIdxBase, 1)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुन) centerY + (द्विगुन) roundf (instancer (varIdxBase, 2)));
    }

    अगर (format ==27 && c->plan->all_axes_pinned)
        out->format = 26;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न a = angle.to_float (c->instancer (varIdxBase, 0));
    भग्न tCenterX = centerX + c->instancer (varIdxBase, 1);
    भग्न tCenterY = centerY + c->instancer (varIdxBase, 2);

    c->funcs->धकेलो_rotate_around_center (c->तथ्य, a, tCenterX, tCenterY);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  खुला:
  DEFINE_आकार_STATIC (4 + F2DOT14::static_आकार + 2 * FWORD::static_आकार);
};

काष्ठा PaintSkew
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
    }

    अगर (format == 29 && c->plan->all_axes_pinned)
        out->format = 28;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    भग्न sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));

    c->funcs->धकेलो_skew (c->तथ्य, sx, sy);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * F2DOT14::static_आकार);
};

काष्ठा PaintSkewAroundCenter
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      set_fixed_clamped (out->xSkewAngle, xSkewAngle.to_float (instancer (varIdxBase, 0)));
      set_fixed_clamped (out->ySkewAngle, ySkewAngle.to_float (instancer (varIdxBase, 1)));
      out->centerX = hb_clamp_to<int16_t> ((द्विगुन) centerX + (द्विगुन) roundf (instancer (varIdxBase, 2)));
      out->centerY = hb_clamp_to<int16_t> ((द्विगुन) centerY + (द्विगुन) roundf (instancer (varIdxBase, 3)));
    }

    अगर (format == 31 && c->plan->all_axes_pinned)
        out->format = 30;

    return_trace (out->src.serialize_subset (c, src, यह, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && src.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c, uint32_t varIdxBase) स्थिर
  {
    TRACE_PAINT (यह);
    भग्न sx = xSkewAngle.to_float(c->instancer (varIdxBase, 0));
    भग्न sy = ySkewAngle.to_float(c->instancer (varIdxBase, 1));
    भग्न tCenterX = centerX + c->instancer (varIdxBase, 2);
    भग्न tCenterY = centerY + c->instancer (varIdxBase, 3);

    c->funcs->धकेलो_skew_around_center (c->तथ्य, sx, sy, tCenterX, tCenterY);
    c->recurse (यह+src);
    c->funcs->निकालो_transform (c->तथ्य);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  खुला:
  DEFINE_आकार_STATIC (4 + 2 * F2DOT14::static_आकार + 2 * FWORD::static_आकार);
};

काष्ठा PaintComposite
{
  HB_INTERNAL व्योम dependv1 (hb_colrv1_depend_context_t* c) स्थिर;
  HB_INTERNAL व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर;

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    bool ret = false;
    ret |= out->src.serialize_subset (c, src, यह, instancer);
    ret |= out->backdrop.serialize_subset (c, backdrop, यह, instancer);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  c->check_ops (यह->min_आकार) && // PainComposite can get exponential
                  src.sanitize (c, यह) &&
                  backdrop.sanitize (c, यह));
  }

  व्योम paint_glyph (hb_paint_context_t *c) स्थिर
  {
    TRACE_PAINT (यह);
    hb_paint_composite_mode_t composite_mode;
    चयन ((अचिन्हित पूर्णांक) mode)
    {
      हाल HB_PAINT_COMPOSITE_MODE_CLEAR:
      हाल HB_PAINT_COMPOSITE_MODE_SRC:
      हाल HB_PAINT_COMPOSITE_MODE_DEST:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_OVER:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_OVER:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_IN:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_IN:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      हाल HB_PAINT_COMPOSITE_MODE_XOR:
      हाल HB_PAINT_COMPOSITE_MODE_PLUS:
      हाल HB_PAINT_COMPOSITE_MODE_SCREEN:
      हाल HB_PAINT_COMPOSITE_MODE_OVERLAY:
      हाल HB_PAINT_COMPOSITE_MODE_DARKEN:
      हाल HB_PAINT_COMPOSITE_MODE_LIGHTEN:
      हाल HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:
      हाल HB_PAINT_COMPOSITE_MODE_COLOR_BURN:
      हाल HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:
      हाल HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:
      हाल HB_PAINT_COMPOSITE_MODE_DIFFERENCE:
      हाल HB_PAINT_COMPOSITE_MODE_EXCLUSION:
      हाल HB_PAINT_COMPOSITE_MODE_MULTIPLY:
      हाल HB_PAINT_COMPOSITE_MODE_HSL_HUE:
      हाल HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
      हाल HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
      हाल HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
	composite_mode = (hb_paint_composite_mode_t) (अचिन्हित पूर्णांक) mode;
	अवरोध;
      शेष:
	composite_mode = HB_PAINT_COMPOSITE_MODE_CLEAR;
	अवरोध;
    }

    c->funcs->धकेलो_group_for (c->तथ्य, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    c->recurse (यह+backdrop);
    c->funcs->धकेलो_group_for (c->तथ्य, composite_mode);
    c->recurse (यह+src);
    c->funcs->निकालो_group (c->तथ्य, composite_mode);
    c->funcs->निकालो_group (c->तथ्य, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24To<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  CompositeMode		mode;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24To<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा Clip_Box_तथ्य
{
  पूर्णांक xMin, yMin, xMax, yMax;
};

काष्ठा ClipBoxFormat1
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_clip_box (Clip_Box_तथ्य &clip_box, स्थिर ItemVarStoreInstancer &instancer HB_UNUSED) स्थिर
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer,
               uint32_t varIdxBase) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (instancer && !c->plan->pinned_at_default && varIdxBase != VarIdx::NO_VARIATION)
    {
      out->xMin = hb_clamp_to<int16_t> ((द्विगुन) xMin + (द्विगुन) floorf (instancer (varIdxBase, 0)));
      out->yMin = hb_clamp_to<int16_t> ((द्विगुन) yMin + (द्विगुन) floorf (instancer (varIdxBase, 1)));
      out->xMax = hb_clamp_to<int16_t> ((द्विगुन) xMax + (द्विगुन) ceilf (instancer (varIdxBase, 2)));
      out->yMax = hb_clamp_to<int16_t> ((द्विगुन) yMax + (द्विगुन) ceilf (instancer (varIdxBase, 3)));
    }

    अगर (format == 2 && c->plan->all_axes_pinned)
        out->format = 1;

    return_trace (true);
  }

  खुला:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  खुला:
  DEFINE_आकार_STATIC (1 + 4 * FWORD::static_आकार);
};

काष्ठा ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  व्योम get_clip_box (Clip_Box_तथ्य &clip_box, स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    value.get_clip_box(clip_box, instancer);
    अगर (instancer)
    {
      clip_box.xMin = hb_saturate_add (clip_box.xMin, hb_clamp_to<पूर्णांक> (floorf (instancer (varIdxBase, 0))));
      clip_box.yMin = hb_saturate_add (clip_box.yMin, hb_clamp_to<पूर्णांक> (floorf (instancer (varIdxBase, 1))));
      clip_box.xMax = hb_saturate_add (clip_box.xMax, hb_clamp_to<पूर्णांक> (ceilf (instancer (varIdxBase, 2))));
      clip_box.yMax = hb_saturate_add (clip_box.yMax, hb_clamp_to<पूर्णांक> (ceilf (instancer (varIdxBase, 3))));
    }
  }

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  { c->variation_indices->add_range (varIdxBase, varIdxBase + 3); }
};

काष्ठा ClipBox
{
  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    हाल 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  व्योम closurev1 (hb_colrv1_closure_context_t* c) स्थिर
  {
    चयन (u.format.v) {
    हाल 2: hb_barrier (); u.format2.closurev1 (c); वापस;
    शेष:वापस;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  bool get_extents (hb_glyph_extents_t *extents,
                    स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    Clip_Box_तथ्य clip_box;
    चयन (u.format.v) {
    हाल 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      अवरोध;
    हाल 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      अवरोध;
    शेष:
      वापस false;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->width = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->height = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    वापस true;
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

काष्ठा ClipRecord
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < startGlyphID ? -1 : g <= endGlyphID ? 0 : +1; }

  व्योम closurev1 (hb_colrv1_closure_context_t* c, स्थिर व्योम *base) स्थिर
  {
    अगर (!c->glyphs->intersects (startGlyphID, endGlyphID)) वापस;
    (base+clipBox).closurev1 (c);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर व्योम *base,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    return_trace (out->clipBox.serialize_subset (c, clipBox, base, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && clipBox.sanitize (c, base));
  }

  bool get_extents (hb_glyph_extents_t *extents,
		    स्थिर व्योम *base,
		    स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    वापस (base+clipBox).get_extents (extents, instancer);
  }

  खुला:
  HBUINT16		startGlyphID;  // first gid clip applies to
  HBUINT16		endGlyphID;    // last gid clip applies to, inclusive
  Offset24To<ClipBox>	clipBox;   // Box or VarBox
  खुला:
  DEFINE_आकार_STATIC (7);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, ClipRecord);

काष्ठा ClipList
{
  अचिन्हित serialize_clip_records (hb_subset_context_t *c,
                                   स्थिर ItemVarStoreInstancer &instancer,
                                   स्थिर hb_set_t& gids,
                                   स्थिर hb_map_t& gid_offset_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    अगर (gids.is_empty () ||
        gid_offset_map.get_population () != gids.get_population ())
      return_trace (0);

    अचिन्हित गिनती  = 0;

    hb_codepoint_t start_gid= gids.get_min ();
    hb_codepoint_t पूर्व_gid = start_gid;

    अचिन्हित offset = gid_offset_map.लो (start_gid);
    अचिन्हित पूर्व_offset = offset;
    क्रम (स्थिर hb_codepoint_t _ : gids.iter ())
    {
      अगर (_ == start_gid) जारी;

      offset = gid_offset_map.लो (_);
      अगर (_ == पूर्व_gid + 1 &&  offset == पूर्व_offset)
      {
        पूर्व_gid = _;
        जारी;
      }

      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = पूर्व_gid;
      record.clipBox = पूर्व_offset;

      अगर (!record.subset (c, यह, instancer)) return_trace (0);
      गिनती++;

      start_gid = _;
      पूर्व_gid = _;
      पूर्व_offset = offset;
    }

    //last one
    {
      ClipRecord record;
      record.startGlyphID = start_gid;
      record.endGlyphID = पूर्व_gid;
      record.clipBox = पूर्व_offset;
      अगर (!record.subset (c, यह, instancer)) return_trace (0);
      गिनती++;
    }
    return_trace (गिनती);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    अगर (!c->serializer->check_assign (out->format, format, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    स्थिर hb_set_t& glyphset = c->plan->_glyphset_colred;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_offset_map;
    hb_set_t new_gids;
    क्रम (स्थिर ClipRecord& record : clips.iter ())
    {
      अचिन्हित start_gid = record.startGlyphID;
      अचिन्हित end_gid = record.endGlyphID;
      क्रम (अचिन्हित gid = start_gid; gid <= end_gid; gid++)
      {
        अगर (!glyphset.has (gid) || !glyph_map.has (gid)) जारी;
        अचिन्हित new_gid = glyph_map.लो (gid);
        new_gid_offset_map.set (new_gid, record.clipBox);
        new_gids.add (new_gid);
      }
    }

    अचिन्हित गिनती = serialize_clip_records (c, instancer, new_gids, new_gid_offset_map);
    अगर (!गिनती) return_trace (false);
    return_trace (c->serializer->check_assign (out->clips.len, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (यह) && clips.sanitize (c, यह));
  }

  bool
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    स्वतः *rec = clips.as_array ().द्वा_खोज (gid);
    अगर (rec)
      वापस rec->get_extents (extents, यह, instancer);
    वापस false;
  }

  HBUINT8			format;  // Set to 1.
  SortedArray32Of<ClipRecord>	clips;  // Clip records, sorted by startGlyphID
  खुला:
  DEFINE_आकार_ARRAY_SIZED (5, clips);
};

काष्ठा Paint
{

  ढाँचा <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मूल्य ());

    return_trace (c->end_recursion (यह->dispatch (c, std::forward<Ts> (ds)...)));
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    हाल 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    हाल 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    हाल 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    हाल 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    हाल 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    हाल 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    हाल 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    हाल 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    हाल 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    हाल 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    हाल 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    हाल 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    हाल 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    हाल 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    हाल 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    हाल 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    हाल 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    हाल 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    हाल 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    हाल 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    हाल 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    हाल 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    हाल 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    हाल 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    हाल 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    हाल 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    हाल 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    हाल 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  bool get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) स्थिर
  {
    चयन (u.format.v) {
    हाल 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); वापस true;
    हाल 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); वापस true;
    शेष: वापस false;
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT8 v; }				format;
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
#अगर_अघोषित HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_MIN (2);
};

काष्ठा BaseGlyphPaintRecord
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  bool serialize (hb_serialize_context_t *s, स्थिर hb_map_t* glyph_map,
                  स्थिर व्योम* src_base, hb_subset_context_t *c,
                  स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = s->embed (यह);
    अगर (unlikely (!out)) return_trace (false);
    अगर (!s->check_assign (out->glyphId, glyph_map->लो (glyphId),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    return_trace (out->paint.serialize_subset (c, paint, src_base, instancer));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) && paint.sanitize (c, base)));
  }

  खुला:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32To<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा BaseGlyphList : SortedArray32Of<BaseGlyphPaintRecord>
{
  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out)))  return_trace (false);
    स्थिर hb_set_t* glyphset = &c->plan->_glyphset_colred;

    क्रम (स्थिर स्वतः& _ : as_array ())
    {
      अचिन्हित gid = _.glyphId;
      अगर (!glyphset->has (gid)) जारी;

      अगर (_.serialize (c->serializer, c->plan->glyph_map, यह, c, instancer)) out->len++;
      अन्यथा return_trace (false);
    }

    return_trace (out->len != 0);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (SortedArray32Of<BaseGlyphPaintRecord>::sanitize (c, यह));
  }
};

काष्ठा LayerList : Array32OfOffset32To<Paint>
{
  स्थिर Paint& get_paint (अचिन्हित i) स्थिर
  { वापस यह+(*यह)[i]; }

  bool subset (hb_subset_context_t *c,
               स्थिर ItemVarStoreInstancer &instancer) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out)))  return_trace (false);

    bool ret = false;
    क्रम (स्थिर स्वतः& _ : + hb_enumerate (*यह)
                         | hb_filter (c->plan->colrv1_layers, hb_first))

    {
      स्वतः *o = out->serialize_append (c->serializer);
      अगर (unlikely (!o)) return_trace (false);
      ret |= o->serialize_subset (c, _.second, यह, instancer);
    }
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (Array32OfOffset32To<Paint>::sanitize (c, यह));
  }
};

काष्ठा delta_set_सूचकांक_map_subset_plan_t
{
  अचिन्हित get_inner_bit_गिनती () स्थिर { वापस inner_bit_गिनती; }
  अचिन्हित get_width ()           स्थिर { वापस ((outer_bit_गिनती + inner_bit_गिनती + 7) / 8); }
  hb_array_t<स्थिर uint32_t> get_output_map () स्थिर { वापस output_map.as_array (); }

  delta_set_सूचकांक_map_subset_plan_t (स्थिर hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_गिनती = 0;
    outer_bit_गिनती = 0;
    inner_bit_गिनती = 1;
    output_map.init ();

    /* search backwards */
    अचिन्हित गिनती = new_deltaset_idx_varidx_map.get_population ();
    अगर (!गिनती) वापस;

    अचिन्हित last_idx = (अचिन्हित)-1;
    अचिन्हित last_varidx = (अचिन्हित)-1;

    क्रम (अचिन्हित i = गिनती; i; i--)
    {
      अचिन्हित delta_set_idx = i - 1;
      अचिन्हित var_idx = new_deltaset_idx_varidx_map.लो (delta_set_idx);
      अगर (i == गिनती)
      {
        last_idx = delta_set_idx;
        last_varidx = var_idx;
        जारी;
      }
      अगर (var_idx != last_varidx)
        अवरोध;
      last_idx = delta_set_idx;
    }

    map_गिनती = last_idx + 1;
  }

  bool remap (स्थिर hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_bit_गिनती = 1;
    inner_bit_गिनती = 1;

    अगर (unlikely (!output_map.resize_dirty (map_गिनती))) वापस false;

    क्रम (अचिन्हित idx = 0; idx < map_गिनती; idx++)
    {
      uint32_t *var_idx;
      अगर (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) वापस false;
      output_map.arrayZ[idx] = *var_idx;

      अचिन्हित outer = (*var_idx) >> 16;
      अचिन्हित bit_गिनती = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_गिनती = hb_max (bit_गिनती, outer_bit_गिनती);

      अचिन्हित inner = (*var_idx) & 0xFFFF;
      bit_गिनती = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_गिनती = hb_max (bit_गिनती, inner_bit_गिनती);
    }
    वापस true;
  }

  निजी:
  अचिन्हित map_गिनती;
  अचिन्हित outer_bit_गिनती;
  अचिन्हित inner_bit_गिनती;
  hb_vector_t<uint32_t> output_map;
};

काष्ठा COLR
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_COLR;

  bool has_तथ्य () स्थिर { वापस has_v0_तथ्य () || version; }

  bool has_v0_तथ्य () स्थिर { वापस अंक_Base_Glyphs; }
  bool has_v1_तथ्य () स्थिर
  {
    अगर (version < 1)
      वापस false;
    hb_barrier ();

    वापस (यह+baseGlyphList).len > 0;
  }

  अचिन्हित पूर्णांक get_glyph_layers (hb_codepoint_t       glyph,
				 अचिन्हित पूर्णांक         start_offset,
				 अचिन्हित पूर्णांक        *गिनती, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) स्थिर
  {
    स्थिर BaseGlyphRecord &record = (यह+baseGlyphsZ).द्वा_खोज (अंक_Base_Glyphs, glyph);

    hb_array_t<स्थिर LayerRecord> all_layers = (यह+layersZ).as_array (अंक_Layers);
    hb_array_t<स्थिर LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
								       record.अंक_Layers);
    अगर (गिनती && layers)
    {
      + glyph_layers.sub_array (start_offset, गिनती)
      | hb_sink (hb_array (layers, *गिनती))
      ;
    }
    वापस glyph_layers.लंबाई;
  }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_table<COLR> (face); }

    ~accelerator_t ()
    {
      स्वतः *scratch = cached_scratch.get_relaxed ();
      अगर (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    bool has_तथ्य () स्थिर { वापस colr->has_तथ्य (); }

#अगर_अघोषित HB_NO_PAINT
    bool
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) स्थिर
    {
      अगर (unlikely (!has_तथ्य ())) वापस false;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      अगर (unlikely (!scratch)) वापस true;
      bool ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      वापस ret;
    }

    bool paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, व्योम *तथ्य,
		      अचिन्हित पूर्णांक palette_सूचकांक,
		      hb_color_t foreground,
		      bool clip = true) स्थिर
    {
      अगर (unlikely (!has_तथ्य ())) वापस false;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      अगर (unlikely (!scratch)) वापस true;
      bool ret = colr->paint_glyph (font, glyph, funcs, तथ्य, palette_सूचकांक, foreground, clip, *scratch);
      release_scratch (scratch);
      वापस ret;
    }
#पूर्ण_अगर

    bool is_valid () { वापस colr.get_blob ()->लंबाई; }

    व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
    {
      अगर (!has_तथ्य ()) वापस;
      colr->depend (depend_तथ्य);
    }

    व्योम closure_glyphs (hb_codepoint_t glyph,
			 hb_set_t *related_ids /* OUT */) स्थिर
    { colr->closure_glyphs (glyph, related_ids); }

    व्योम closure_V0palette_indices (स्थिर hb_set_t *glyphs,
				    hb_set_t *palettes /* OUT */) स्थिर
    { colr->closure_V0palette_indices (glyphs, palettes); }

    व्योम closure_forV1 (hb_set_t *glyphset,
                        hb_set_t *layer_indices,
                        hb_set_t *palette_indices,
                        hb_set_t *variation_indices,
                        hb_set_t *delta_set_indices) स्थिर
    { colr->closure_forV1 (glyphset, layer_indices, palette_indices, variation_indices, delta_set_indices); }

    bool has_var_store () स्थिर
    { वापस colr->has_var_store (); }

    स्थिर ItemVariationStore &get_var_store () स्थिर
    { वापस colr->get_var_store (); }
    स्थिर ItemVariationStore *get_var_store_ptr () स्थिर
    { वापस colr->get_var_store_ptr (); }

    bool has_delta_set_सूचकांक_map () स्थिर
    { वापस colr->has_delta_set_सूचकांक_map (); }

    स्थिर Delta_Set_सूचकांक_Map &get_delta_set_सूचकांक_map () स्थिर
    { वापस colr->get_delta_set_सूचकांक_map (); }
    स्थिर Delta_Set_सूचकांक_Map *get_delta_set_सूचकांक_map_ptr () स्थिर
    { वापस colr->get_delta_set_सूचकांक_map_ptr (); }

    निजी:

    hb_colr_scratch_t *acquire_scratch () स्थिर
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      अगर (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, माप (hb_colr_scratch_t));
	अगर (unlikely (!scratch))
	  वापस nullptr;
      }

      वापस scratch;
    }
    व्योम release_scratch (hb_colr_scratch_t *scratch) स्थिर
    {
      अगर (!cached_scratch.cmpexch (nullptr, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    खुला:
    hb_blob_ptr_t<COLR> colr;
    निजी:
    mutable hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
  {
    // v0
    hb_array_t<स्थिर BaseGlyphRecord> baseGlyphs = (यह+baseGlyphsZ).as_array (अंक_Base_Glyphs);
    क्रम (स्थिर BaseGlyphRecord &record : baseGlyphs)
    {
      स्वतः glyph_layers = (यह+layersZ).as_array (अंक_Layers).sub_array (record.firstLayerIdx,
                                                                         record.अंक_Layers);
      क्रम (स्थिर LayerRecord layer : glyph_layers)
        depend_तथ्य->add_depend(record.glyphId, HB_OT_TAG_COLR, layer.glyphId);
    }

    अगर (!has_v1_तथ्य())
      वापस;

    // v1
    hb_colrv1_depend_context_t c (यह, depend_तथ्य);
    स्थिर BaseGlyphList &baseglyph_paintrecords = यह+baseGlyphList;

    क्रम (स्थिर BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.source_gid = baseglyph_paintrecord.glyphId;
      c.visited_paint.सफा ();

      स्थिर Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  व्योम closure_glyphs (hb_codepoint_t glyph,
		       hb_set_t *related_ids /* OUT */) स्थिर
  {
    स्थिर BaseGlyphRecord *record = get_base_glyph_record (glyph);
    अगर (!record) वापस;

    स्वतः glyph_layers = (यह+layersZ).as_array (अंक_Layers).sub_array (record->firstLayerIdx,
								       record->अंक_Layers);
    अगर (!glyph_layers.लंबाई) वापस;
    related_ids->add_array (&glyph_layers[0].glyphId, glyph_layers.लंबाई, LayerRecord::min_आकार);
  }

  व्योम closure_V0palette_indices (स्थिर hb_set_t *glyphs,
				  hb_set_t *palettes /* OUT */) स्थिर
  {
    अगर (!अंक_Base_Glyphs || !अंक_Layers) वापस;
    hb_array_t<स्थिर BaseGlyphRecord> baseGlyphs = (यह+baseGlyphsZ).as_array (अंक_Base_Glyphs);
    hb_array_t<स्थिर LayerRecord> all_layers = (यह+layersZ).as_array (अंक_Layers);

    क्रम (स्थिर BaseGlyphRecord record : baseGlyphs)
    {
      अगर (!glyphs->has (record.glyphId)) जारी;
      hb_array_t<स्थिर LayerRecord> glyph_layers = all_layers.sub_array (record.firstLayerIdx,
                                                                   record.अंक_Layers);
      क्रम (स्थिर LayerRecord layer : glyph_layers)
        palettes->add (layer.colorIdx);
    }
  }

  व्योम closure_forV1 (hb_set_t *glyphset,
                      hb_set_t *layer_indices,
                      hb_set_t *palette_indices,
                      hb_set_t *variation_indices,
                      hb_set_t *delta_set_indices) स्थिर
  {
    अगर (version < 1) वापस;
    hb_barrier ();

    hb_set_t visited_glyphs;

    hb_colrv1_closure_context_t c (यह, &visited_glyphs, layer_indices, palette_indices, variation_indices);
    स्थिर BaseGlyphList &baseglyph_paintrecords = यह+baseGlyphList;

    क्रम (स्थिर BaseGlyphPaintRecord &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      अचिन्हित gid = baseglyph_paintrecord.glyphId;
      अगर (!glyphset->has (gid)) जारी;

      स्थिर Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_set_union (glyphset, &visited_glyphs);

    स्थिर ClipList &cliplist = यह+clipList;
    c.glyphs = glyphset;
    क्रम (स्थिर ClipRecord &clip_record : cliplist.clips.iter())
      clip_record.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    अगर (has_delta_set_सूचकांक_map ())
    {
      स्थिर Delta_Set_सूचकांक_Map &var_idx_map = यह+varIdxMap;
      delta_set_indices->set (*variation_indices);
      variation_indices->सफा ();
      क्रम (अचिन्हित delta_set_idx : *delta_set_indices)
        variation_indices->add (var_idx_map.map (delta_set_idx));
    }
  }

  स्थिर LayerList& get_layerList () स्थिर
  { वापस (यह+layerList); }

  स्थिर BaseGlyphList& get_baseglyphList () स्थिर
  { वापस (यह+baseGlyphList); }

  bool has_var_store () स्थिर
  { वापस version >= 1 && hb_barrier () && varStore != 0; }

  bool has_delta_set_सूचकांक_map () स्थिर
  { वापस version >= 1 && hb_barrier () && varIdxMap != 0; }

  bool has_clip_list () स्थिर
  { वापस version >= 1 && hb_barrier () && clipList != 0; }

  स्थिर Delta_Set_सूचकांक_Map &get_delta_set_सूचकांक_map () स्थिर
  { वापस has_delta_set_सूचकांक_map () && hb_barrier () ? यह+varIdxMap : Null (Delta_Set_सूचकांक_Map); }
  स्थिर Delta_Set_सूचकांक_Map *get_delta_set_सूचकांक_map_ptr () स्थिर
  { वापस has_delta_set_सूचकांक_map () && hb_barrier () ? &(यह+varIdxMap) : nullptr; }

  स्थिर ItemVariationStore &get_var_store () स्थिर
  { वापस has_var_store () && hb_barrier () ? यह+varStore : Null (ItemVariationStore); }
  स्थिर ItemVariationStore *get_var_store_ptr () स्थिर
  { वापस has_var_store () && hb_barrier () ? &(यह+varStore) : nullptr; }

  स्थिर ClipList &get_clip_list () स्थिर
  { वापस has_clip_list () && hb_barrier () ? यह+clipList : Null (ClipList); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
                  (यह+baseGlyphsZ).sanitize (c, अंक_Base_Glyphs) &&
                  (यह+layersZ).sanitize (c, अंक_Layers) &&
                  (version == 0 ||
		   (hb_barrier () &&
		    baseGlyphList.sanitize (c, यह) &&
		    layerList.sanitize (c, यह) &&
		    clipList.sanitize (c, यह) &&
		    varIdxMap.sanitize (c, यह) &&
		    varStore.sanitize (c, यह))));
  }

  ढाँचा<typename BaseIterator, typename LayerIterator,
	   hb_requires (hb_is_iterator (BaseIterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  bool serialize_V0 (hb_serialize_context_t *c,
		     अचिन्हित version,
		     BaseIterator base_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (base_it.len () != layer_it.len ()))
      return_trace (false);

    यह->version = version;
    अंक_Layers = 0;
    अंक_Base_Glyphs = base_it.len ();
    अगर (अंक_Base_Glyphs == 0)
    {
      baseGlyphsZ = 0;
      layersZ = 0;
      return_trace (true);
    }

    c->push ();
    क्रम (स्थिर hb_item_type<BaseIterator> _ : + base_it.iter ())
    {
      स्वतः* record = c->embed (_);
      अगर (unlikely (!record)) return_trace (false);
      record->firstLayerIdx = अंक_Layers;
      अंक_Layers += record->अंक_Layers;
    }
    c->add_link (baseGlyphsZ, c->निकालो_pack ());

    c->push ();
    क्रम (स्थिर hb_item_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->add_link (layersZ, c->निकालो_pack ());

    return_trace (true);
  }

  स्थिर BaseGlyphRecord* get_base_glyph_record (hb_codepoint_t gid) स्थिर
  {
    स्थिर BaseGlyphRecord* record = &(यह+baseGlyphsZ).द्वा_खोज (अंक_Base_Glyphs, (अचिन्हित पूर्णांक) gid);
    अगर (record == &Null (BaseGlyphRecord) ||
        (record && (hb_codepoint_t) record->glyphId != gid))
      record = nullptr;
    वापस record;
  }

  स्थिर BaseGlyphPaintRecord* get_base_glyph_paintrecord (hb_codepoint_t gid) स्थिर
  {
    स्थिर BaseGlyphList &list = यह+baseGlyphList;
    अचिन्हित पूर्णांक i;
    अगर (!list.bfind ((अचिन्हित) gid, &i))
      वापस nullptr;
    वापस &list[i];
  }

  bool downgrade_to_V0 (स्थिर hb_set_t &glyphset) स्थिर
  {
    //no more COLRv1 glyphs, downgrade to version 0
    क्रम (स्थिर BaseGlyphPaintRecord& _ : get_baseglyphList ())
      अगर (glyphset.has (_.glyphId))
        वापस false;

    वापस true;
  }

  bool subset_varstore (hb_subset_context_t *c,
                        COLR* out /* OUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (true);

    स्थिर ItemVariationStore& var_store = यह+varStore;
    अगर (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      bool optimize = (varIdxMap != 0) ? true : false;
      अगर (!item_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (false);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      अगर (item_vars.get_region_list () &&
          item_vars.get_vardata_encodings () &&
          !out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (false);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      अगर (optimize)
      {
        स्थिर hb_map_t &varidx_map = item_vars.get_varidx_map ();
        क्रम (स्वतः _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          अगर (varidx_map.has (varidx, &new_varidx))
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
      अगर (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_सूचकांक_tag_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      अगर (unlikely (!out->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : nullptr)))
        return_trace (false);
    }

    return_trace (true);
  }

  bool subset_delta_set_सूचकांक_map (hb_subset_context_t *c,
                                   COLR* out /* OUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (true);

    स्थिर hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_set_सूचकांक_map_subset_plan_t सूचकांक_map_plan (deltaset_idx_varidx_map);

    अगर (unlikely (!सूचकांक_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (false);

    return_trace (out->varIdxMap.serialize_serialize (c->serializer, सूचकांक_map_plan));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
    स्थिर hb_set_t& glyphset = c->plan->_glyphset_colred;

    स्वतः base_it =
    + hb_range (c->plan->अंक_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = reverse_glyph_map.लो (new_gid);
		    अगर (glyphset.has (old_gid)) वापस true;
		    वापस false;
		 })
    | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = reverse_glyph_map.लो (new_gid);

				स्थिर BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				अगर (unlikely (!old_record))
				  वापस hb_pair_t<bool, BaseGlyphRecord> (false, Null (BaseGlyphRecord));
				BaseGlyphRecord new_record = {};
				new_record.glyphId = new_gid;
				new_record.अंक_Layers = old_record->अंक_Layers;
				वापस hb_pair_t<bool, BaseGlyphRecord> (true, new_record);
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
				स्थिर BaseGlyphRecord* old_record = get_base_glyph_record (old_gid);
				hb_vector_t<LayerRecord> out_layers;

				अगर (unlikely (!old_record ||
					      old_record->firstLayerIdx >= अंक_Layers ||
					      old_record->firstLayerIdx + old_record->अंक_Layers > अंक_Layers))
				  वापस hb_pair_t<bool, hb_vector_t<LayerRecord>> (false, out_layers);

				स्वतः layers = (यह+layersZ).as_array (अंक_Layers).sub_array (old_record->firstLayerIdx,
											     old_record->अंक_Layers);
				out_layers.resize (layers.लंबाई);
				क्रम (अचिन्हित पूर्णांक i = 0; i < layers.लंबाई; i++) {
				  out_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  अगर (unlikely (!c->plan->new_gid_for_old_gid (out_layers[i].glyphId, &new_gid)))
				    वापस hb_pair_t<bool, hb_vector_t<LayerRecord>> (false, out_layers);
				  out_layers[i].glyphId = new_gid;
				  out_layers[i].colorIdx = c->plan->colr_palettes.लो (layers[i].colorIdx);
				}

				वापस hb_pair_t<bool, hb_vector_t<LayerRecord>> (true, out_layers);
			      })
    | hb_filter (hb_first)
    | hb_map_retains_sorting (hb_second)
    ;

    अगर (version == 0 && (!base_it || !layer_it))
      return_trace (false);

    स्वतः *colr_अभाज्य = c->serializer->start_embed<COLR> ();
    अगर (unlikely (!c->serializer->extend_min (colr_अभाज्य)))  return_trace (false);

    अगर (version == 0 || downgrade_to_V0 (glyphset))
      return_trace (colr_अभाज्य->serialize_V0 (c->serializer, 0, base_it, layer_it));

    hb_barrier ();

    //start version 1
    अगर (!c->serializer->allocate_आकार<व्योम> (5 * HBUINT32::static_आकार)) return_trace (false);
    अगर (!colr_अभाज्य->serialize_V0 (c->serializer, version, base_it, layer_it)) return_trace (false);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    अगर (!subset_varstore (c, colr_अभाज्य)) return_trace (false);

    ItemVarStoreInstancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : nullptr,
				     c->plan->normalized_coords ? get_delta_set_सूचकांक_map_ptr () : nullptr,
				     c->plan->normalized_coords.as_array ());

    अगर (!colr_अभाज्य->baseGlyphList.serialize_subset (c, baseGlyphList, यह, instancer))
      return_trace (false);

    colr_अभाज्य->layerList.serialize_subset (c, layerList, यह, instancer);
    colr_अभाज्य->clipList.serialize_subset (c, clipList, यह, instancer);

    return_trace (subset_delta_set_सूचकांक_map (c, colr_अभाज्य));
  }

  स्थिर Paint *get_base_glyph_paint (hb_codepoint_t glyph) स्थिर
  {
    स्थिर BaseGlyphList &baseglyph_paintrecords = यह+baseGlyphList;
    स्थिर BaseGlyphPaintRecord* record = get_base_glyph_paintrecord (glyph);
    अगर (record)
    {
      स्थिर Paint &paint = &baseglyph_paintrecords+record->paint;
      वापस &paint;
    }
    अन्यथा
      वापस nullptr;
  }

#अगर_अघोषित HB_NO_PAINT
  bool
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) स्थिर
  {

    ItemVarStoreInstancer instancer (get_var_store_ptr (),
                                     get_delta_set_सूचकांक_map_ptr (),
                                     hb_array (font->coords, font->अंक_coords));

    अगर (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      वापस true;
    }

    स्वतः *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.सफा ();
    bool ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), true, scratch);

    स्वतः e = scratch.paint_extents.get_extents ();
    अगर (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->width = 0;
      extents->height = 0;
    }
    अन्यथा
      *extents = e.to_glyph_extents ();

    वापस ret;
  }
#पूर्ण_अगर

  bool
  has_paint_for_glyph (hb_codepoint_t glyph) स्थिर
  {
    अगर (version >= 1)
    {
      hb_barrier ();

      स्थिर Paint *paint = get_base_glyph_paint (glyph);

      वापस paint != nullptr;
    }

    वापस false;
  }

  bool get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 स्थिर ItemVarStoreInstancer instancer) स्थिर
  {
    वापस get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#अगर_अघोषित HB_NO_PAINT
  bool
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, व्योम *तथ्य,
	       अचिन्हित पूर्णांक palette_सूचकांक, hb_color_t foreground,
	       bool clip,
	       hb_colr_scratch_t &scratch) स्थिर
  {
    ItemVarStoreInstancer instancer (get_var_store_ptr (),
				     get_delta_set_सूचकांक_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->अंक_coords : 0));
    hb_paint_context_t c (यह, funcs, तथ्य, font, palette_सूचकांक, foreground, instancer);

    hb_decycler_गाँठ_t node (c.glyphs_decycler);
    node.visit (glyph);

    अगर (version >= 1)
    {
      hb_barrier ();

      स्थिर Paint *paint = get_base_glyph_paint (glyph);
      अगर (paint)
      {
        // COLRv1 glyph

	bool is_bounded = true;
	hb_glyph_extents_t extents = {};
	अगर (clip)
	{
	  अगर (!get_clip (glyph, &extents, instancer))
	  {
	    clip = false;
	    is_bounded = false;
	  }

	  अगर (!is_bounded)
	  {
	    स्वतः *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.सफा ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_सूचकांक, foreground,
			 false,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->धकेलो_font_transform (c.तथ्य, font);

	अगर (clip)
	  c.funcs->धकेलो_clip_rectangle (c.तथ्य,
					(भग्न) extents.x_bearing,
					(भग्न) extents.y_bearing + extents.height,
					(भग्न) extents.x_bearing + extents.width,
					(भग्न) extents.y_bearing);

	अगर (is_bounded)
	  c.recurse (*paint);

	अगर (clip)
	  c.funcs->निकालो_clip (c.तथ्य);

	c.funcs->निकालो_transform (c.तथ्य);

        वापस true;
      }
    }

    स्थिर BaseGlyphRecord *record = get_base_glyph_record (glyph);
    अगर (record && ((hb_codepoint_t) record->glyphId == glyph))
    {
      // COLRv0 glyph
      क्रम (स्थिर स्वतः &r : (यह+layersZ).as_array (अंक_Layers)
			   .sub_array (record->firstLayerIdx, record->अंक_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.तथ्य, r.glyphId, c.font, is_foreground, color);
      }

      वापस true;
    }

    वापस false;
  }
#पूर्ण_अगर

  रक्षित:
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
  Offset32To<Delta_Set_सूचकांक_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32To<ItemVariationStore>	varStore;
  खुला:
  DEFINE_आकार_MIN (14);
};

काष्ठा COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

व्योम
hb_paint_context_t::recurse (स्थिर Paint &paint)
{
  अगर (unlikely (depth_बायाँ <= 0 || edge_गिनती <= 0)) वापस;
  depth_बायाँ--;
  edge_गिनती--;
  paint.dispatch (यह);
  depth_बायाँ++;
}

ढाँचा <typename Types>
व्योम PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) स्थिर
{
  TRACE_PAINT (यह);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  अगर (likely (c->depth_बायाँ > 0 && c->edge_गिनती > 0) &&
      (यह+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->edge_गिनती--;
    c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
    c->funcs->fill_glyph (c->तथ्य, gid, c->font, is_foreground, color);
    c->funcs->निकालो_transform (c->तथ्य);
    वापस;
  }

  c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
  c->funcs->धकेलो_clip_glyph (c->तथ्य, gid, c->font);
  c->funcs->धकेलो_font_transform (c->तथ्य, c->font);
  c->recurse (यह+paint);
  c->funcs->निकालो_transform (c->तथ्य);
  c->funcs->निकालो_clip (c->तथ्य);
  c->funcs->निकालो_transform (c->तथ्य);
}

व्योम PaintColrLayers::paint_glyph (hb_paint_context_t *c) स्थिर
{
  TRACE_PAINT (यह);
  स्थिर LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  hb_decycler_गाँठ_t node (c->layers_decycler);
  क्रम (अचिन्हित i = first_Layer_सूचकांक; i < first_Layer_सूचकांक + अंक_Layers; i++)
  {
    अगर (unlikely (!node.visit (i)))
      वापस;

    स्थिर Paint &paint = paint_offset_lists.get_paint (i);
    c->recurse (paint);
  }
}

व्योम PaintColrGlyph::paint_glyph (hb_paint_context_t *c) स्थिर
{
  TRACE_PAINT (यह);

  hb_decycler_गाँठ_t node (c->glyphs_decycler);
  अगर (unlikely (!node.visit (gid)))
    वापस;

  c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
  अगर (c->funcs->color_glyph (c->तथ्य, gid, c->font))
  {
    c->funcs->निकालो_transform (c->तथ्य);
    वापस;
  }
  c->funcs->निकालो_transform (c->तथ्य);

  स्थिर COLR *colr_table = c->get_colr_table ();
  स्थिर Paint *paint = colr_table->get_base_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  bool has_clip_box = colr_table->get_clip (gid, &extents, c->instancer);

  अगर (has_clip_box)
    c->funcs->धकेलो_clip_rectangle (c->तथ्य,
				   (भग्न) extents.x_bearing,
				   (भग्न) extents.y_bearing + extents.height,
				   (भग्न) extents.x_bearing + extents.width,
				   (भग्न) extents.y_bearing);

  अगर (paint)
    c->recurse (*paint);

  अगर (has_clip_box)
    c->funcs->निकालो_clip (c->तथ्य);
}

} /* namespace OT */

#पूर्ण_अगर /* OT_COLOR_COLR_COLR_HH */
