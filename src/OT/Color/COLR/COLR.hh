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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_COLR_COLR_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_COLR_COLR_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-decycler.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-var-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint-bounded.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint-extents.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-depend-data.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_COLR HB_ਟੈਗ('C','O','L','R')

ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ hb_paint_context_t;
}

ਬਣਤਰ hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ COLR;

ਬਣਤਰ Paint;

ਬਣਤਰ hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "PAINT"; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.paint_glyph (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਅਚਲ COLR* get_colr_ਸਾਰਣੀ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ COLR *> (ਅਧਾਰ); }

ਜਨਤਕ:
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
  hb_paint_funcs_t *funcs;
  ਖਾਲੀ *ਅੰਕੜੇ;
  hb_font_t *font;
  hb_array_t<ਅਚਲ BGRAColor> palette;
  hb_color_t foreground;
  ਵਸਤੂ_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ਪੂਰਨ ਡੂੰਘਾਈ_ਖੱਬਾ = HB_MAX_NESTING_LEVEL;
  ਪੂਰਨ ਕਿਨਾਰਾ_ਗਿਣਤੀ = HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ;

  hb_paint_context_t (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2,
		      hb_paint_funcs_t *funcs_,
		      ਖਾਲੀ *ਅੰਕੜੇ_2,
                      hb_font_t *font_,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_,
                      hb_color_t foreground_,
		      ਵਸਤੂ_Var_Store_Instancer &instancer_) :
    ਅਧਾਰ (ਅਧਾਰ_2),
    funcs (funcs_),
    ਅੰਕੜੇ (ਅੰਕੜੇ_2),
    font (font_),
    palette (
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->ਸਾਰਣੀ.CPAL->get_palette_colors (palette_ < font->face->ਸਾਰਣੀ.CPAL->get_palette_ਗਿਣਤੀ () ? palette_ : 0)
#ਜੇ_ਅੰਤ
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    ਜੇ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_ਸੈੱਟ_synthetic_bold (font, 0, 0, ਸੱਚ);
      hb_font_ਸੈੱਟ_synthetic_slant (font, 0);
    }
    ਨਹੀਂਤਾਂ
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
  color_alpha (hb_color_t color, ਦਸ਼ਮਲਵ alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    ਵਾਪਸ roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ, ਦਸ਼ਮਲਵ alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = ਸੱਚ;

    ਜੇ (color_ਸੂਚਕਾਂਕ != 0xffff)
    {
      ਜੇ (!funcs->custom_palette_color (ਅੰਕੜੇ, color_ਸੂਚਕਾਂਕ, &color))
        color = palette[color_ਸੂਚਕਾਂਕ];

      *is_foreground = ਝੂਠ;
    }

    ਵਾਪਸ HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  ਸਤਰਅੰਦਰ ਖਾਲੀ recurse (ਅਚਲ Paint &paint);
};

ਬਣਤਰ hb_colrv1_ਘੇਰਾ_context_t :
       hb_dispatch_context_t<hb_colrv1_ਘੇਰਾ_context_t>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj)
  {
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0))
      ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

    ਜੇ (paint_ਦੇਖਿਆ (&obj))
      ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

    nesting_level_ਖੱਬਾ--;
    obj.closurev1 (ਇਹ);
    nesting_level_ਖੱਬਾ++;
    ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();
  }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਤਾਰਕਿਕ paint_ਦੇਖਿਆ (ਅਚਲ ਖਾਲੀ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) ਅਧਾਰ);
    ਜੇ (ਦੇਖਿਆ_paint.ਵਿੱਚ_error() || ਦੇਖਿਆ_paint.has (delta))
      ਵਾਪਸ ਸੱਚ;

    ਦੇਖਿਆ_paint.ਜੋੜੋ (delta);
    ਵਾਪਸ ਝੂਠ;
  }

  ਅਚਲ COLR* get_colr_ਸਾਰਣੀ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ COLR *> (ਅਧਾਰ); }

  ਖਾਲੀ ਜੋੜੋ_glyph (ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_id)
  { glyphs->ਜੋੜੋ (glyph_id); }

  ਖਾਲੀ ਜੋੜੋ_layer_indices (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_layer_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਦਾ_layers)
  {
    ਜੇ (ਅੰਕ_ਦਾ_layers == 0) ਵਾਪਸ;
    layer_indices->ਜੋੜੋ_range (ਪਹਿਲਾ_layer_ਸੂਚਕਾਂਕ, ਪਹਿਲਾ_layer_ਸੂਚਕਾਂਕ + ਅੰਕ_ਦਾ_layers - 1);
  }

  ਖਾਲੀ ਜੋੜੋ_palette_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਸੂਚਕਾਂਕ)
  { palette_indices->ਜੋੜੋ (palette_ਸੂਚਕਾਂਕ); }

  ਖਾਲੀ ਜੋੜੋ_var_idxes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_var_idx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_idxes)
  {
    ਜੇ (!ਅੰਕ_idxes || ਪਹਿਲਾ_var_idx == VarIdx::NO_VARIATION) ਵਾਪਸ;
    variation_indices->ਜੋੜੋ_range (ਪਹਿਲਾ_var_idx, ਪਹਿਲਾ_var_idx + ਅੰਕ_idxes - 1);
  }

  ਜਨਤਕ:
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
  hb_ਸੈੱਟ_t ਦੇਖਿਆ_paint;
  hb_ਸੈੱਟ_t *glyphs;
  hb_ਸੈੱਟ_t *layer_indices;
  hb_ਸੈੱਟ_t *palette_indices;
  hb_ਸੈੱਟ_t *variation_indices;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_var_idxes;
  ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level_ਖੱਬਾ;

  hb_colrv1_ਘੇਰਾ_context_t (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2,
                               hb_ਸੈੱਟ_t *glyphs_,
                               hb_ਸੈੱਟ_t *layer_indices_,
                               hb_ਸੈੱਟ_t *palette_indices_,
                               hb_ਸੈੱਟ_t *variation_indices_,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_var_idxes_2 = 1,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level_ਖੱਬਾ_2 = HB_MAX_NESTING_LEVEL) :
                          ਅਧਾਰ (ਅਧਾਰ_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          ਅੰਕ_var_idxes (ਅੰਕ_var_idxes_2),
                          nesting_level_ਖੱਬਾ (nesting_level_ਖੱਬਾ_2)
  {}
};


ਬਣਤਰ hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj)
  {
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0))
      ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

    ਜੇ (paint_ਦੇਖਿਆ (&obj))
      ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

    nesting_level_ਖੱਬਾ--;
    obj.dependv1 (ਇਹ);
    nesting_level_ਖੱਬਾ++;
    ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();
  }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਤਾਰਕਿਕ paint_ਦੇਖਿਆ (ਅਚਲ ਖਾਲੀ *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    ਜੇ (ਦੇਖਿਆ_paint.ਵਿੱਚ_error() || ਦੇਖਿਆ_paint.has (delta))
      ਵਾਪਸ ਸੱਚ;

    ਦੇਖਿਆ_paint.ਜੋੜੋ (delta);
    ਜੇ (unlikely (ਦੇਖਿਆ_paint.ਵਿੱਚ_error ()))
      depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਝੂਠ;
  }

  ਅਚਲ COLR* get_colr_ਸਾਰਣੀ () ਅਚਲ
  { ਵਾਪਸ colr; }

  ਜਨਤਕ:
  ਅਚਲ COLR *colr;
  hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ;
  hb_ਸੈੱਟ_t ਦੇਖਿਆ_paint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level_ਖੱਬਾ;
  hb_codepoint_t ਸਰੋਤ_gid;

  hb_colrv1_depend_context_t (ਅਚਲ COLR *colr_,
                              hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ_2,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level_ਖੱਬਾ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_ਅੰਕੜੇ (depend_ਅੰਕੜੇ_2),
                          nesting_level_ਖੱਬਾ (nesting_level_ਖੱਬਾ_2),
                          ਸਰੋਤ_gid (0)
  {}
};

ਬਣਤਰ Layer_ਰਿਕਾਰਡ
{
  ਸੰਚਾਲਕ hb_ot_color_layer_t () ਅਚਲ { ਵਾਪਸ {glyphId, colorIdx}; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  ਸੂਚਕਾਂਕ		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	ਪਹਿਲਾ_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	ਅੰਕ_Layers;	/* Number of color layers
				 * associated with this glyph */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ Variable
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_variable = ਸੱਚ;

  ਅਚਲ T& get_ਮੁੱਲ () ਅਚਲ { ਵਾਪਸ value; }

  Variable<T>* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (ਇਹ));
  }

  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
  { value.dependv1 (c); }

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    c->ਅੰਕ_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->ਜੋੜੋ_var_idxes (var_Idx_ਅਧਾਰ, c->ਅੰਕ_var_idxes);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!value.subset (c, instancer, var_Idx_ਅਧਾਰ)) return_trace (ਝੂਠ);
    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਸੱਚ);

    VarIdx new_varidx;
    new_varidx = var_Idx_ਅਧਾਰ;
    ਜੇ (var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
      ਜੇ (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_ਅਧਾਰ, &new_varidx_delta))
        return_trace (ਝੂਠ);

      new_varidx = hb_ਪਹਿਲਾ (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    value.paint_glyph (c, var_Idx_ਅਧਾਰ);
  }

  ਖਾਲੀ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    value.get_color_stop (c, stop, var_Idx_ਅਧਾਰ, instancer);
  }

  ਖਾਲੀ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ਅਚਲ
  {
    value.get_solid_color (c, var_Idx_ਅਧਾਰ, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ਅਚਲ
  {
    ਵਾਪਸ value.get_extend ();
  }

  ਸੁਰੱਖਿਅਤ:
  T      value;
  ਜਨਤਕ:
  VarIdx var_Idx_ਅਧਾਰ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (VarIdx::static_ਮਾਪ + T::min_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ NoVariable
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_variable = ਝੂਠ;

  ਅਚਲ T& get_ਮੁੱਲ () ਅਚਲ { ਵਾਪਸ value; }

  ਸਥਿਰ constexpr uint32_t var_Idx_ਅਧਾਰ = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (ਇਹ));
  }

  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
  { value.dependv1 (c); }

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  { value.closurev1 (c); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace (value.subset (c, instancer, var_Idx_ਅਧਾਰ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    value.paint_glyph (c, var_Idx_ਅਧਾਰ);
  }

  ਖਾਲੀ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  ਖਾਲੀ get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) ਅਚਲ
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () ਅਚਲ
  {
    ਵਾਪਸ value.get_extend ();
  }

  T      value;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (T::min_ਮਾਪ);
};

// Color structures

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਬਿਨਾਨਿਸ਼ਾਨੀ fraction_ਬਿੱਟ>
ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_fixed_clamped (HBFixed<Type, fraction_ਬਿੱਟ> &ਬਾਹਰਲਾ, ਦਸ਼ਮਲਵ value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  ਬਾਹਰਲਾ.ਸੈੱਟ_int (hb_clamp_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ Type::type> (roundf ((value / HBFixed<Type, fraction_ਬਿੱਟ>::mult))));
}

ਬਣਤਰ ColorStop
{
  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t * c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    c->ਜੋੜੋ_palette_ਸੂਚਕਾਂਕ (palette_ਸੂਚਕਾਂਕ_2);
    c->ਅੰਕ_var_idxes = 2;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->stop_ਵਿੱਥ, stop_ਵਿੱਥ.ਮੰਜ਼ਿਲ_float(instancer (var_Idx_ਅਧਾਰ, 0)));
      ਦਸ਼ਮਲਵ a = alpha.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->palette_ਸੂਚਕਾਂਕ_2, c->plan->colr_palettes.get (palette_ਸੂਚਕਾਂਕ_2),
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *ਬਾਹਰਲਾ,
		       uint32_t varIdx,
		       ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    ਬਾਹਰਲਾ->ਵਿੱਥ = stop_ਵਿੱਥ.ਮੰਜ਼ਿਲ_float(instancer (varIdx, 0));
    ਬਾਹਰਲਾ->color = c->get_color (palette_ਸੂਚਕਾਂਕ_2,
                               alpha.ਮੰਜ਼ਿਲ_float (instancer (varIdx, 1)),
                               &ਬਾਹਰਲਾ->is_foreground);
  }

  F2DOT14	stop_ਵਿੱਥ;
  HBUINT16	palette_ਸੂਚਕਾਂਕ_2;
  F2DOT14	alpha;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 + 2 * F2DOT14::static_ਮਾਪ);
};

ਬਣਤਰ Extend : HBUINT8
{
  ਗਿਣਤੀ {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (1);
};

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
ਬਣਤਰ Color_ਸਤਰ
{
  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t * c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &stop : stops.iter ())
      stop.closurev1 (c);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->extend, extend, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ)) return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& stop : stops.iter ())
    {
      ਜੇ (!stop.subset (c, instancer)) return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  get_color_stops (hb_paint_context_t *c,
                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2,
		   hb_color_stop_t *color_stops,
		   ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = stops.len;

    ਜੇ (ਗਿਣਤੀ_2 && color_stops)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
      ਲਈ (i = 0; i < *ਗਿਣਤੀ_2 && ਸ਼ੁਰੂ + i < len; i++)
        stops[ਸ਼ੁਰੂ + i].get_color_stop (c, &color_stops[i], instancer);
      *ਗਿਣਤੀ_2 = i;
    }

    ਵਾਪਸ len;
  }

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ static_get_color_stops (hb_color_ਸਤਰ_t *color_ਸਤਰ,
							  ਖਾਲੀ *color_ਸਤਰ_ਅੰਕੜੇ,
							  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
							  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2,
							  hb_color_stop_t *color_stops,
							  ਖਾਲੀ *user_ਅੰਕੜੇ)
  {
    ਅਚਲ Color_ਸਤਰ *thiz = (ਅਚਲ Color_ਸਤਰ *) color_ਸਤਰ_ਅੰਕੜੇ;
    hb_paint_context_t *c = (hb_paint_context_t *) user_ਅੰਕੜੇ;
    ਵਾਪਸ thiz->get_color_stops (c, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () ਅਚਲ
  {
    ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) extend)
    {
      ਹਾਲਤ Extend::EXTEND_REPEAT:  ਵਾਪਸ HB_PAINT_EXTEND_REPEAT;
      ਹਾਲਤ Extend::EXTEND_REFLECT: ਵਾਪਸ HB_PAINT_EXTEND_REFLECT;
      ਹਾਲਤ Extend::EXTEND_PAD:
      ਮੂਲ:                     ਵਾਪਸ HB_PAINT_EXTEND_PAD;
    }
  }

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ hb_paint_extend_t static_get_extend (hb_color_ਸਤਰ_t *color_ਸਤਰ,
							  ਖਾਲੀ *color_ਸਤਰ_ਅੰਕੜੇ,
							  ਖਾਲੀ *user_ਅੰਕੜੇ)
  {
    ਅਚਲ Color_ਸਤਰ *thiz = (ਅਚਲ Color_ਸਤਰ *) color_ਸਤਰ_ਅੰਕੜੇ;
    ਵਾਪਸ thiz->get_extend ();
  }

  Extend	extend;
  Array16_ਦਾ<Var<ColorStop>>	stops;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
ਬਣਤਰ ਸੰਯੁਕਤ_ਬਹੁਲਕ : HBUINT8
{
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (1);
};

ਬਣਤਰ Affine2x3
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  { c->ਅੰਕ_var_idxes = 6; }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸਵੈਚਾਲਿਤ fold = [&] (F16DOT16 &target, F16DOT16 ਸਰੋਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ) {
        // Keep the stored integer precise while applying the float delta.
        target.ਸੈੱਟ_int (hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (roundf ((ਦੁੱਗਣਾ) ਸਰੋਤ.ਮੰਜ਼ਿਲ_int () +
                                                    (ਦੁੱਗਣਾ) instancer (var_Idx_ਅਧਾਰ, ਵਿੱਥ))));
      };
      fold (ਬਾਹਰਲਾ->xx, xx, 0);
      fold (ਬਾਹਰਲਾ->yx, yx, 1);
      fold (ਬਾਹਰਲਾ->xy, xy, 2);
      fold (ਬਾਹਰਲਾ->yy, yy, 3);
      fold (ਬਾਹਰਲਾ->dx, dx, 4);
      fold (ਬਾਹਰਲਾ->dy, dy, 5);
    }
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    c->funcs->ਧੱਕੋ_transform (c->ਅੰਕੜੇ,
			      xx.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0)),
			      yx.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 1)),
                              xy.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 2)),
			      yy.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 3)),
                              dx.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 4)),
			      dy.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6 * F16DOT16::static_ਮਾਪ);
};

ਬਣਤਰ PaintColrLayers
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    uint32_t ਪਹਿਲਾ_layer_ਸੂਚਕਾਂਕ = ਅੰਕ_Layers ? c->plan->colrv1_layers.get (ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ) : 0;
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ, ਪਹਿਲਾ_layer_ਸੂਚਕਾਂਕ,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸਤਰਅੰਦਰ ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ;

  HBUINT8	format; /* format = 1 */
  HBUINT8	ਅੰਕ_Layers;
  HBUINT32	ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ;  /* index into COLRv1::layerList */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ PaintSolid
{
  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ {}
  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    c->ਜੋੜੋ_palette_ਸੂਚਕਾਂਕ (palette_ਸੂਚਕਾਂਕ_2);
    c->ਅੰਕ_var_idxes = 1;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਦਸ਼ਮਲਵ a = alpha.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    ਜੇ (format == 3 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 2;

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->palette_ਸੂਚਕਾਂਕ_2, c->plan->colr_palettes.get (palette_ਸੂਚਕਾਂਕ_2),
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ,
                        hb_bool_t *is_foreground, hb_color_t *color) ਅਚਲ
  {
    *color = c->get_color (palette_ਸੂਚਕਾਂਕ_2,
                           alpha.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0)),
                           is_foreground);
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_ਅਧਾਰ, &is_foreground, &color);
    c->funcs->color (c->ਅੰਕੜੇ, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_ਸੂਚਕਾਂਕ_2;
  F2DOT14	alpha;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (3 + F2DOT14::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
ਬਣਤਰ Paint_ਰੇਖਿਕ_Gradient
{
  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    (ਇਹ+color_ਸਤਰ_2).closurev1 (c);
    c->ਅੰਕ_var_idxes = 6;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਬਾਹਰਲਾ->x0 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) x0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->y0 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) y0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->x1 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) x1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਬਾਹਰਲਾ->y1 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) y1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 3)));
      ਬਾਹਰਲਾ->x2 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) x2 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 4)));
      ਬਾਹਰਲਾ->y2 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) y2 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 5)));
    }

    ਜੇ (format == 5 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 4;

    return_trace (ਬਾਹਰਲਾ->color_ਸਤਰ_2.serialize_subset (c, color_ਸਤਰ_2, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && color_ਸਤਰ_2.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    hb_color_ਸਤਰ_t cl = {
      (ਖਾਲੀ *) &(ਇਹ+color_ਸਤਰ_2),
      (ਇਹ+color_ਸਤਰ_2).static_get_color_stops, c,
      (ਇਹ+color_ਸਤਰ_2).static_get_extend, ਖਾਲੀ_ਸੂਚਕ
    };

    c->funcs->ਰੇਖਿਕ_gradient (c->ਅੰਕੜੇ, &cl,
			       x0 + c->instancer (var_Idx_ਅਧਾਰ, 0),
			       y0 + c->instancer (var_Idx_ਅਧਾਰ, 1),
			       x1 + c->instancer (var_Idx_ਅਧਾਰ, 2),
			       y1 + c->instancer (var_Idx_ਅਧਾਰ, 3),
			       x2 + c->instancer (var_Idx_ਅਧਾਰ, 4),
			       y2 + c->instancer (var_Idx_ਅਧਾਰ, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Color_ਸਤਰ<Var>>	color_ਸਤਰ_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 6 * FWORD::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
ਬਣਤਰ PaintRadialGradient
{
  ਤਾਰਕਿਕ constant_after_pinning (hb_subset_context_t *c,
                               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    ਜੇ (c->plan->has_avar2) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &regions = instancer.varStore->get_region_list ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < regions.region_ਗਿਣਤੀ; i++)
    {
      hb_hashmap_t<hb_ਟੈਗ_t, Triple> axes;
      ਜੇ (!regions.get_var_region (i, c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼);
        ਵਾਪਸ ਝੂਠ;
      }
      ਲਈ (ਸਵੈਚਾਲਿਤ axis : axes)
      {
        Triple *ਹੱਦਬੰਦੀ;
        ਜੇ (!c->plan->user_axes_location.has (axis.ਪਹਿਲਾ, &ਹੱਦਬੰਦੀ) ||
            ਹੱਦਬੰਦੀ->minimum != ਹੱਦਬੰਦੀ->ਵੱਧ_ਤੋਂ_ਵੱਧ)
          ਵਾਪਸ ਝੂਠ;
      }
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    (ਇਹ+color_ਸਤਰ_2).closurev1 (c);
    c->ਅੰਕ_var_idxes = 6;
  }

  ਤਾਰਕਿਕ subset_nonnegative (hb_subset_context_t *c,
                           ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    ਦੁੱਗਣਾ ਸਰੋਤ[6] = {
      x0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 0)),
      y0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)),
      radius0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)),
      x1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 3)),
      y1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 4)),
      radius1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 5))
    };
    ਸਵੈਚਾਲਿਤ transparent = [&] {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->allocate_ਮਾਪ<PaintSolid> (5);
      ਜੇ (!ਬਾਹਰਲਾ) ਵਾਪਸ ਝੂਠ;
      ਬਾਹਰਲਾ->format = 2; ਬਾਹਰਲਾ->palette_ਸੂਚਕਾਂਕ_2 = 0xFFFF; ਬਾਹਰਲਾ->alpha.ਸੈੱਟ_float (0);
      ਵਾਪਸ ਸੱਚ;
    };
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਸਤਰ = ਇਹ+color_ਸਤਰ_2;
    ਜੇ (ਸਰੋਤ[2] == ਸਰੋਤ[5] || !ਸਤਰ.stops.len) ਵਾਪਸ transparent ();
    ਬਣਤਰ stop_t { ਦੁੱਗਣਾ ਵਿੱਥ, alpha; ਬਿਨਾਨਿਸ਼ਾਨੀ palette, ਕ੍ਰਮ; };
    hb_ਸਦਿਸ਼_t<stop_t> stops;
    ਜੇ (!stops.alloc (ਸਤਰ.stops.len)) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕ੍ਰਮ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : ਸਤਰ.stops.iter ())
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &stop = ਇੰਦਰਾਜ.get_ਮੁੱਲ ();
      stops.push (stop_t {(ਦੁੱਗਣਾ) roundf (stop.stop_ਵਿੱਥ.ਮੰਜ਼ਿਲ_float (instancer (ਇੰਦਰਾਜ.var_Idx_ਅਧਾਰ, 0)) * 16384.f) / 16384.,
                   (ਦੁੱਗਣਾ) hb_clamp (stop.alpha.ਮੰਜ਼ਿਲ_float (instancer (ਇੰਦਰਾਜ.var_Idx_ਅਧਾਰ, 1)), 0.f, 1.f),
                   stop.palette_ਸੂਚਕਾਂਕ_2, ਕ੍ਰਮ++});
    }
    ਜੇ (stops.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;
    stops.qsort ([] (ਅਚਲ stop_t &a, ਅਚਲ stop_t &b) {
      ਵਾਪਸ a.ਵਿੱਥ < b.ਵਿੱਥ ? -1 : a.ਵਿੱਥ > b.ਵਿੱਥ ? 1 :
             a.ਕ੍ਰਮ < b.ਕ੍ਰਮ ? -1 : a.ਕ੍ਰਮ > b.ਕ੍ਰਮ ? 1 : 0;
    });
    ਦੁੱਗਣਾ dr = ਸਰੋਤ[5] - ਸਰੋਤ[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    ਬਿਨਾਨਿਸ਼ਾਨੀ lattice = 16384;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 3; j++)
    {
      ਦੁੱਗਣਾ remainder = fmod (fabs (ਸਰੋਤ[j + 3] - ਸਰੋਤ[j]), 16384.);
      ਜੇ (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
        ਵਾਪਸ ਝੂਠ;
      }
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) remainder;
      ਜਦੋਂ (ਸੱਜੇ) { ਬਿਨਾਨਿਸ਼ਾਨੀ r = lattice % ਸੱਜੇ; lattice = ਸੱਜੇ; ਸੱਜੇ = r; }
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ = 0; ਖਿਸਕਾਓ < 16; ਖਿਸਕਾਓ++)
    {
      ਦੁੱਗਣਾ b = 1u << ਖਿਸਕਾਓ;
      ਦੁੱਗਣਾ coarse = dr > 0 ? ceil (-ਸਰੋਤ[2] / dr) : floor (ਸਰੋਤ[2] / -dr) - b;
      ਦੁੱਗਣਾ fine = dr > 0 ? ceil (-ਸਰੋਤ[2] / dr * lattice) / lattice :
                            floor (ਸਰੋਤ[2] / -dr * lattice) / lattice - b;
      ਦੁੱਗਣਾ candidates[] = {coarse, fine};
      ਲਈ (ਦੁੱਗਣਾ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        ਦੁੱਗਣਾ geometry[6];
        ਤਾਰਕਿਕ ਸਮਾਉਂਦਾ = ਸੱਚ;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 3; j++)
        {
          ਦੁੱਗਣਾ difference = ਸਰੋਤ[j + 3] - ਸਰੋਤ[j];
          geometry[j] = ਸਰੋਤ[j] + a * difference;
          geometry[j + 3] = ਸਰੋਤ[j] + (a + b) * difference;
          ਦੁੱਗਣਾ ਨੀਵਾਂ = j == 2 ? 0 : -32768, ਉੱਚਾ = j == 2 ? 65535 : 32767;
          ਸਮਾਉਂਦਾ &= geometry[j] >= ਨੀਵਾਂ && geometry[j] <= ਉੱਚਾ &&
                  geometry[j + 3] >= ਨੀਵਾਂ && geometry[j + 3] <= ਉੱਚਾ;
        }
        ਜੇ (!ਸਮਾਉਂਦਾ) ਜਾਰੀ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ = 0, ਗਿਣਤੀ_2 = stops.ਲੰਮਾਈ;
        ਦੁੱਗਣਾ min = (stops[0].ਵਿੱਥ - a) / b, max = (stops.ਪੂਛ ().ਵਿੱਥ - a) / b;
        ਦੁੱਗਣਾ bias = 0;
        ਤਾਰਕਿਕ constant = ਝੂਠ;
        ਜੇ (ਸਤਰ.extend == Extend::EXTEND_PAD)
        {
          ਦੁੱਗਣਾ tip = -geometry[2] / (b * dr);
          ਜੇ ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            ਪਹਿਲਾ = dr > 0 ? stops.ਲੰਮਾਈ - 1 : 0;
            ਗਿਣਤੀ_2 = 1; constant = ਸੱਚ;
          }
        }
        ਨਹੀਂਤਾਂ
        {
          ਦੁੱਗਣਾ period = max - min;
          ਜੇ (ਸਤਰ.extend == Extend::EXTEND_REFLECT) period *= 2;
          ਜੇ (period <= 0) ਵਾਪਸ transparent ();
          ਦੁੱਗਣਾ n = ceil ((-2 - min) / period);
          ਜੇ (n > floor ((32767. / 16384. - max) / period)) ਜਾਰੀ;
          bias = n * period;
        }
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਗਿਣਤੀ_2; j++)
        {
          ਦੁੱਗਣਾ ਬਿੱਟ = constant ? 0 : ((stops[ਪਹਿਲਾ + j].ਵਿੱਥ - a) / b + bias) * 16384;
          ਸਮਾਉਂਦਾ &= ਬਿੱਟ >= -32768 && ਬਿੱਟ <= 32767 && ਬਿੱਟ == round (ਬਿੱਟ);
        }
        ਜੇ (!ਸਮਾਉਂਦਾ) ਜਾਰੀ;
        ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->allocate_ਮਾਪ<PaintRadialGradient<NoVariable>> (16);
        ਜੇ (!ਬਾਹਰਲਾ) ਵਾਪਸ ਝੂਠ;
        ਬਾਹਰਲਾ->format = 6;
        ਬਾਹਰਲਾ->x0 = (ਪੂਰਨ) geometry[0]; ਬਾਹਰਲਾ->y0 = (ਪੂਰਨ) geometry[1]; ਬਾਹਰਲਾ->radius0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) geometry[2];
        ਬਾਹਰਲਾ->x1 = (ਪੂਰਨ) geometry[3]; ਬਾਹਰਲਾ->y1 = (ਪੂਰਨ) geometry[4]; ਬਾਹਰਲਾ->radius1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) geometry[5];
        c->serializer->push ();
        ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ_ਸਤਰ = c->serializer->allocate_ਮਾਪ<Color_ਸਤਰ<NoVariable>> (3 + ਗਿਣਤੀ_2 * ColorStop::static_ਮਾਪ);
        ਜੇ (!ਬਾਹਰਲਾ_ਸਤਰ) { c->serializer->ਕੱਢੋ_discard (); ਵਾਪਸ ਝੂਠ; }
        ਬਾਹਰਲਾ_ਸਤਰ->extend = ਸਤਰ.extend;
        ਬਾਹਰਲਾ_ਸਤਰ->stops.len = ਗਿਣਤੀ_2;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਗਿਣਤੀ_2; j++)
        {
          ਅਚਲ ਸਵੈਚਾਲਿਤ &stop = stops[ਪਹਿਲਾ + j];
          ਸਵੈਚਾਲਿਤ &ਬਾਹਰਲਾ_stop = ਬਾਹਰਲਾ_ਸਤਰ->stops[j].value;
          ਬਾਹਰਲਾ_stop.stop_ਵਿੱਥ.ਸੈੱਟ_float ((ਦਸ਼ਮਲਵ) (constant ? 0 : (stop.ਵਿੱਥ - a) / b + bias));
          ਬਾਹਰਲਾ_stop.alpha.ਸੈੱਟ_float ((ਦਸ਼ਮਲਵ) stop.alpha);
          ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ_stop.palette_ਸੂਚਕਾਂਕ_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
          { c->serializer->ਕੱਢੋ_discard (); ਵਾਪਸ ਝੂਠ; }
        }
        c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->color_ਸਤਰ_2, c->serializer->ਕੱਢੋ_pack ());
        ਵਾਪਸ !c->serializer->ਵਿੱਚ_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION &&
        (radius0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)) < 0 ||
         radius1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_ਅਧਾਰ));
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਬਾਹਰਲਾ->x0 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) x0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->y0 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) y0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->radius0 = hb_clamp_ਮੰਜ਼ਿਲ<uint16_t> ((ਦੁੱਗਣਾ) radius0 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਬਾਹਰਲਾ->x1 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) x1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 3)));
      ਬਾਹਰਲਾ->y1 = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) y1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 4)));
      ਬਾਹਰਲਾ->radius1 = hb_clamp_ਮੰਜ਼ਿਲ<uint16_t> ((ਦੁੱਗਣਾ) radius1 + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 5)));
    }

    ਜੇ (format == 7 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 6;

    return_trace (ਬਾਹਰਲਾ->color_ਸਤਰ_2.serialize_subset (c, color_ਸਤਰ_2, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && color_ਸਤਰ_2.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    hb_color_ਸਤਰ_t cl = {
      (ਖਾਲੀ *) &(ਇਹ+color_ਸਤਰ_2),
      (ਇਹ+color_ਸਤਰ_2).static_get_color_stops, c,
      (ਇਹ+color_ਸਤਰ_2).static_get_extend, ਖਾਲੀ_ਸੂਚਕ
    };

    c->funcs->radial_gradient (c->ਅੰਕੜੇ, &cl,
			       x0 + c->instancer (var_Idx_ਅਧਾਰ, 0),
			       y0 + c->instancer (var_Idx_ਅਧਾਰ, 1),
			       radius0 + c->instancer (var_Idx_ਅਧਾਰ, 2),
			       x1 + c->instancer (var_Idx_ਅਧਾਰ, 3),
			       y1 + c->instancer (var_Idx_ਅਧਾਰ, 4),
			       radius1 + c->instancer (var_Idx_ਅਧਾਰ, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Color_ਸਤਰ<Var>>	color_ਸਤਰ_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 6 * FWORD::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
ਬਣਤਰ PaintSweepGradient
{
  ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ {}

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    (ਇਹ+color_ਸਤਰ_2).closurev1 (c);
    c->ਅੰਕ_var_idxes = 4;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਬਾਹਰਲਾ->centerX = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerX + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->centerY = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerY + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->ਸ਼ੁਰੂ_Angle, ਸ਼ੁਰੂ_Angle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->ਸਿਰਾ_Angle, ਸਿਰਾ_Angle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 3)));
    }

    ਜੇ (format == 9 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 8;

    return_trace (ਬਾਹਰਲਾ->color_ਸਤਰ_2.serialize_subset (c, color_ਸਤਰ_2, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && color_ਸਤਰ_2.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    hb_color_ਸਤਰ_t cl = {
      (ਖਾਲੀ *) &(ਇਹ+color_ਸਤਰ_2),
      (ਇਹ+color_ਸਤਰ_2).static_get_color_stops, c,
      (ਇਹ+color_ਸਤਰ_2).static_get_extend, ਖਾਲੀ_ਸੂਚਕ
    };

    c->funcs->sweep_gradient (c->ਅੰਕੜੇ, &cl,
			      centerX + c->instancer (var_Idx_ਅਧਾਰ, 0),
			      centerY + c->instancer (var_Idx_ਅਧਾਰ, 1),
                              (ਸ਼ੁਰੂ_Angle.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 2)) + 1) * HB_PI,
                              (ਸਿਰਾ_Angle.ਮੰਜ਼ਿਲ_float   (c->instancer (var_Idx_ਅਧਾਰ, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Color_ਸਤਰ<Var>>	color_ਸਤਰ_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		ਸ਼ੁਰੂ_Angle;
  F2DOT14		ਸਿਰਾ_Angle;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * FWORD::static_ਮਾਪ + 2 * F2DOT14::static_ਮਾਪ);
};

// Paint a non-COLR glyph, filled as indicated by paint.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ PaintGlyph
{
  ਵਰਤੋਂ HBGlyphID = ਕਿਸਮ_ਨਾਂ Types::HBGlyphID;

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (! c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->paint.serialize_subset (c, paint, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && paint.sanitize (c, ਇਹ));
  }

  ਸਤਰਅੰਦਰ ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + HBGlyphID::static_ਮਾਪ);
};

ਬਣਤਰ PaintColrGlyph
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸਤਰਅੰਦਰ ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (3);
};

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
ਬਣਤਰ PaintTransform
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਜੇ (!ਬਾਹਰਲਾ->transform.serialize_subset (c, transform, ਇਹ, instancer)) return_trace (ਝੂਠ);
    ਜੇ (format == 13 && c->plan->all_axes_pinned)
      ਬਾਹਰਲਾ->format = 12;
    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  src.sanitize (c, ਇਹ) &&
                  transform.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    (ਇਹ+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_ਮੰਜ਼ਿਲ<Var<Affine2x3>>	transform;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (7);
};

ਬਣਤਰ PaintTranslate
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਬਾਹਰਲਾ->dx = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) dx + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->dy = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) dy + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
    }

    ਜੇ (format == 15 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 14;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ ddx = dx + c->instancer (var_Idx_ਅਧਾਰ, 0);
    ਦਸ਼ਮਲਵ ddy = dy + c->instancer (var_Idx_ਅਧਾਰ, 1);

    c->funcs->ਧੱਕੋ_translate (c->ਅੰਕੜੇ, ddx, ddy);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * FWORD::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸਰੋਤ_2>
ਸਥਿਰ ਤਾਰਕਿਕ serialize_scale_transform (hb_subset_context_t *c,
                                       ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
                                       ਅਚਲ ਸਰੋਤ_2 &ਸਰੋਤ)
{
  ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->allocate_ਮਾਪ<PaintTransform<NoVariable>> (7);
  ਜੇ (unlikely (!ਬਾਹਰਲਾ)) ਵਾਪਸ ਝੂਠ;
  ਬਾਹਰਲਾ->format = 12;

  c->serializer->push ();
  ਸਵੈਚਾਲਿਤ *transform = c->serializer->allocate_ਮਾਪ<Affine2x3> (Affine2x3::static_ਮਾਪ);
  ਜੇ (unlikely (!transform))
  {
    c->serializer->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }
  ਸੈੱਟ_fixed_clamped (transform->xx, sx);
  transform->yx.ਸੈੱਟ_float (0);
  transform->xy.ਸੈੱਟ_float (0);
  ਸੈੱਟ_fixed_clamped (transform->yy, sy);
  ਸੈੱਟ_fixed_clamped (transform->dx, dx);
  ਸੈੱਟ_fixed_clamped (transform->dy, dy);
  c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->transform, c->serializer->ਕੱਢੋ_pack ());

  c->serializer->push ();
  ਤਾਰਕਿਕ ret = ਸਰੋਤ ();
  ਜੇ (ret)
    c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->src, c->serializer->ਕੱਢੋ_pack ());
  ਨਹੀਂਤਾਂ
    c->serializer->ਕੱਢੋ_discard ();
  ਵਾਪਸ ret;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3>
ਸਥਿਰ ਤਾਰਕਿਕ subset_scale_as_transform (hb_subset_context_t *c,
                                      ਅਚਲ ਅਧਾਰ_3 *ਅਧਾਰ,
                                      ਅਚਲ Offset24_ਮੰਜ਼ਿਲ<Paint> &src,
                                      ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
                                      ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy,
                                      ਦਸ਼ਮਲਵ cx = 0, ਦਸ਼ਮਲਵ cy = 0)
{
  ਜੇ (src.is_null ()) ਵਾਪਸ ਝੂਠ;
  ਸਵੈਚਾਲਿਤ ਸਰੋਤ = [&] { ਵਾਪਸ c->dispatch (ਅਧਾਰ+src, instancer); };
  ਦਸ਼ਮਲਵ dx = cx * (1 - sx), dy = cy * (1 - sy);
  ਜੇ (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    ਵਾਪਸ serialize_scale_transform (c, sx, sy, dx, dy, ਸਰੋਤ);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  ਸਵੈਚਾਲਿਤ inner = [&] {
    ਵਾਪਸ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, ਸਰੋਤ);
  };
  ਸਵੈਚਾਲਿਤ inverse = [&] {
    ਵਾਪਸ serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  ਸਵੈਚਾਲਿਤ scale = [&] {
    ਵਾਪਸ serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  ਸਵੈਚਾਲਿਤ outer = [&] {
    ਵਾਪਸ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  ਵਾਪਸ serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

ਬਣਤਰ PaintScale
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਦਸ਼ਮਲਵ sx = roundf (scaleX.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)) * 16384) / 16384;
      ਦਸ਼ਮਲਵ sy = roundf (scaleY.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)) * 16384) / 16384;
      ਜੇ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, ਇਹ, src, instancer, sx, sy));
    }
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scaleX, scaleX.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scaleY, scaleY.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)));
    }

    ਜੇ (format == 17 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 16;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ sx = scaleX.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ sy = scaleY.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 1));

    c->funcs->ਧੱਕੋ_scale (c->ਅੰਕੜੇ, sx, sy);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * F2DOT14::static_ਮਾਪ);
};

ਬਣਤਰ PaintScaleAroundCenter
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਦਸ਼ਮਲਵ sx = roundf (scaleX.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)) * 16384) / 16384;
      ਦਸ਼ਮਲਵ sy = roundf (scaleY.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)) * 16384) / 16384;
      ਜੇ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, ਇਹ, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_ਅਧਾਰ, 2)),
            centerY + roundf (instancer (var_Idx_ਅਧਾਰ, 3))));
    }
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scaleX, scaleX.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scaleY, scaleY.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->centerX = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerX + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਬਾਹਰਲਾ->centerY = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerY + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 3)));
    }

    ਜੇ (format == 19 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 18;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ sx = scaleX.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ sy = scaleY.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 1));
    ਦਸ਼ਮਲਵ tCenterX = centerX + c->instancer (var_Idx_ਅਧਾਰ, 2);
    ਦਸ਼ਮਲਵ tCenterY = centerY + c->instancer (var_Idx_ਅਧਾਰ, 3);

    c->funcs->ਧੱਕੋ_scale_around_center (c->ਅੰਕੜੇ, sx, sy, tCenterX, tCenterY);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * F2DOT14::static_ਮਾਪ + 2 * FWORD::static_ਮਾਪ);
};

ਬਣਤਰ Paint_Scale_ਇਕਸਾਰ
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਦਸ਼ਮਲਵ s = roundf (scale.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)) * 16384) / 16384;
      ਜੇ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, ਇਹ, src, instancer, s, s));
    }
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scale, scale.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));

    ਜੇ (format == 21 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 20;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ s = scale.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));

    c->funcs->ਧੱਕੋ_scale (c->ਅੰਕੜੇ, s, s);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + F2DOT14::static_ਮਾਪ);
};

ਬਣਤਰ Paint_Scale_ਇਕਸਾਰ_Around_Center
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਦਸ਼ਮਲਵ s = roundf (scale.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)) * 16384) / 16384;
      ਜੇ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, ਇਹ, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_ਅਧਾਰ, 1)),
            centerY + roundf (instancer (var_Idx_ਅਧਾਰ, 2))));
    }
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->scale, scale.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->centerX = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerX + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->centerY = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerY + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
    }

    ਜੇ (format == 23 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 22;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ s = scale.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ tCenterX = centerX + c->instancer (var_Idx_ਅਧਾਰ, 1);
    ਦਸ਼ਮਲਵ tCenterY = centerY + c->instancer (var_Idx_ਅਧਾਰ, 2);

    c->funcs->ਧੱਕੋ_scale_around_center (c->ਅੰਕੜੇ, s, s, tCenterX, tCenterY);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + F2DOT14::static_ਮਾਪ + 2 * FWORD::static_ਮਾਪ);
};

ਬਣਤਰ PaintRotate
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->angle, angle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));

    ਜੇ (format == 25 && c->plan->all_axes_pinned)
      ਬਾਹਰਲਾ->format = 24;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ a = angle.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));

    c->funcs->ਧੱਕੋ_rotate (c->ਅੰਕੜੇ, a);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + F2DOT14::static_ਮਾਪ);
};

ਬਣਤਰ PaintRotateAroundCenter
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->angle, angle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->centerX = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerX + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->centerY = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerY + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
    }

    ਜੇ (format ==27 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 26;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ a = angle.ਮੰਜ਼ਿਲ_float (c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ tCenterX = centerX + c->instancer (var_Idx_ਅਧਾਰ, 1);
    ਦਸ਼ਮਲਵ tCenterY = centerY + c->instancer (var_Idx_ਅਧਾਰ, 2);

    c->funcs->ਧੱਕੋ_rotate_around_center (c->ਅੰਕੜੇ, a, tCenterX, tCenterY);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + F2DOT14::static_ਮਾਪ + 2 * FWORD::static_ਮਾਪ);
};

ਬਣਤਰ PaintSkew
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->xSkewAngle, xSkewAngle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->ySkewAngle, ySkewAngle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)));
    }

    ਜੇ (format == 29 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 28;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ sx = xSkewAngle.ਮੰਜ਼ਿਲ_float(c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ sy = ySkewAngle.ਮੰਜ਼ਿਲ_float(c->instancer (var_Idx_ਅਧਾਰ, 1));

    c->funcs->ਧੱਕੋ_skew (c->ਅੰਕੜੇ, sx, sy);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * F2DOT14::static_ਮਾਪ);
};

ਬਣਤਰ PaintSkewAroundCenter
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->xSkewAngle, xSkewAngle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਸੈੱਟ_fixed_clamped (ਬਾਹਰਲਾ->ySkewAngle, ySkewAngle.ਮੰਜ਼ਿਲ_float (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->centerX = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerX + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਬਾਹਰਲਾ->centerY = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) centerY + (ਦੁੱਗਣਾ) roundf (instancer (var_Idx_ਅਧਾਰ, 3)));
    }

    ਜੇ (format == 31 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 30;

    return_trace (ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && src.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    ਦਸ਼ਮਲਵ sx = xSkewAngle.ਮੰਜ਼ਿਲ_float(c->instancer (var_Idx_ਅਧਾਰ, 0));
    ਦਸ਼ਮਲਵ sy = ySkewAngle.ਮੰਜ਼ਿਲ_float(c->instancer (var_Idx_ਅਧਾਰ, 1));
    ਦਸ਼ਮਲਵ tCenterX = centerX + c->instancer (var_Idx_ਅਧਾਰ, 2);
    ਦਸ਼ਮਲਵ tCenterY = centerY + c->instancer (var_Idx_ਅਧਾਰ, 3);

    c->funcs->ਧੱਕੋ_skew_around_center (c->ਅੰਕੜੇ, sx, sy, tCenterX, tCenterY);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 2 * F2DOT14::static_ਮਾਪ + 2 * FWORD::static_ਮਾਪ);
};

ਬਣਤਰ Paint_ਸੰਯੁਕਤ
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ret |= ਬਾਹਰਲਾ->src.serialize_subset (c, src, ਇਹ, instancer);
    ret |= ਬਾਹਰਲਾ->backdrop.serialize_subset (c, backdrop, ਇਹ, instancer);
    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  c->check_ops (ਇਹ->min_ਮਾਪ) && // PainComposite can get exponential
                  src.sanitize (c, ਇਹ) &&
                  backdrop.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ paint_glyph (hb_paint_context_t *c) ਅਚਲ
  {
    TRACE_PAINT (ਇਹ);
    hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਸੰਯੁਕਤ_ਬਹੁਲਕ_2;
    ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਬਹੁਲਕ)
    {
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY:
	ਸੰਯੁਕਤ_ਬਹੁਲਕ_2 = (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਬਹੁਲਕ;
	ਰੋਕੋ;
      ਮੂਲ:
	ਸੰਯੁਕਤ_ਬਹੁਲਕ_2 = HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR;
	ਰੋਕੋ;
    }

    c->funcs->ਧੱਕੋ_ਸਮੂਹ_for (c->ਅੰਕੜੇ, HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER);
    c->recurse (ਇਹ+backdrop);
    c->funcs->ਧੱਕੋ_ਸਮੂਹ_for (c->ਅੰਕੜੇ, ਸੰਯੁਕਤ_ਬਹੁਲਕ_2);
    c->recurse (ਇਹ+src);
    c->funcs->ਕੱਢੋ_ਸਮੂਹ (c->ਅੰਕੜੇ, ਸੰਯੁਕਤ_ਬਹੁਲਕ_2);
    c->funcs->ਕੱਢੋ_ਸਮੂਹ (c->ਅੰਕੜੇ, HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  ਸੰਯੁਕਤ_ਬਹੁਲਕ		ਬਹੁਲਕ;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_ਮੰਜ਼ਿਲ<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Clip_Box_ਅੰਕੜੇ
{
  ਪੂਰਨ xMin, yMin, xMax, yMax;
};

ਬਣਤਰ ClipBoxFormat1
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_clip_box (Clip_Box_ਅੰਕੜੇ &clip_box, ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
               uint32_t var_Idx_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (instancer && !c->plan->pinned_at_default && var_Idx_ਅਧਾਰ != VarIdx::NO_VARIATION)
    {
      ਬਾਹਰਲਾ->xMin = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) xMin + (ਦੁੱਗਣਾ) floorf (instancer (var_Idx_ਅਧਾਰ, 0)));
      ਬਾਹਰਲਾ->yMin = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) yMin + (ਦੁੱਗਣਾ) floorf (instancer (var_Idx_ਅਧਾਰ, 1)));
      ਬਾਹਰਲਾ->xMax = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) xMax + (ਦੁੱਗਣਾ) ceilf (instancer (var_Idx_ਅਧਾਰ, 2)));
      ਬਾਹਰਲਾ->yMax = hb_clamp_ਮੰਜ਼ਿਲ<int16_t> ((ਦੁੱਗਣਾ) yMax + (ਦੁੱਗਣਾ) ceilf (instancer (var_Idx_ਅਧਾਰ, 3)));
    }

    ਜੇ (format == 2 && c->plan->all_axes_pinned)
        ਬਾਹਰਲਾ->format = 1;

    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (1 + 4 * FWORD::static_ਮਾਪ);
};

ਬਣਤਰ ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  ਖਾਲੀ get_clip_box (Clip_Box_ਅੰਕੜੇ &clip_box, ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    value.get_clip_box(clip_box, instancer);
    ਜੇ (instancer)
    {
      clip_box.xMin = hb_saturate_ਜੋੜੋ (clip_box.xMin, hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (floorf (instancer (var_Idx_ਅਧਾਰ, 0))));
      clip_box.yMin = hb_saturate_ਜੋੜੋ (clip_box.yMin, hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (floorf (instancer (var_Idx_ਅਧਾਰ, 1))));
      clip_box.xMax = hb_saturate_ਜੋੜੋ (clip_box.xMax, hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (ceilf (instancer (var_Idx_ਅਧਾਰ, 2))));
      clip_box.yMax = hb_saturate_ਜੋੜੋ (clip_box.yMax, hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (ceilf (instancer (var_Idx_ਅਧਾਰ, 3))));
    }
  }

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  { c->variation_indices->ਜੋੜੋ_range (var_Idx_ਅਧਾਰ, var_Idx_ਅਧਾਰ + 3); }
};

ਬਣਤਰ ClipBox
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 2: hb_barrier (); u.format2.closurev1 (c); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ get_extents (hb_glyph_extents_t *extents,
                    ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    Clip_Box_ਅੰਕੜੇ clip_box;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      ਰੋਕੋ;
    ਹਾਲਤ 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      ਰੋਕੋ;
    ਮੂਲ:
      ਵਾਪਸ ਝੂਠ;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->ਚੌੜਾਈ = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->ਉਚਾਈ = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

ਬਣਤਰ Clip_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < ਸ਼ੁਰੂ_Glyph_ID ? -1 : g <= ਸਿਰਾ_Glyph_ID ? 0 : +1; }

  ਖਾਲੀ closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (!c->glyphs->intersects (ਸ਼ੁਰੂ_Glyph_ID, ਸਿਰਾ_Glyph_ID)) ਵਾਪਸ;
    (ਅਧਾਰ+clipBox).closurev1 (c);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->clipBox.serialize_subset (c, clipBox, ਅਧਾਰ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && clipBox.sanitize (c, ਅਧਾਰ));
  }

  ਤਾਰਕਿਕ get_extents (hb_glyph_extents_t *extents,
		    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		    ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    ਵਾਪਸ (ਅਧਾਰ+clipBox).get_extents (extents, instancer);
  }

  ਜਨਤਕ:
  HBUINT16		ਸ਼ੁਰੂ_Glyph_ID;  // first gid clip applies to
  HBUINT16		ਸਿਰਾ_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_ਮੰਜ਼ਿਲ<ClipBox>	clipBox;   // Box or VarBox
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (7);
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, Clip_ਰਿਕਾਰਡ);

ਬਣਤਰ ClipList
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ serialize_clip_ਰਿਕਾਰਡਾਂ (hb_subset_context_t *c,
                                   ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer,
                                   ਅਚਲ hb_ਸੈੱਟ_t& gids,
                                   ਅਚਲ hb_map_t& gid_ਵਿੱਥ_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (gids.is_empty () ||
        gid_ਵਿੱਥ_map.get_population () != gids.get_population ())
      return_trace (0);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2  = 0;

    hb_codepoint_t ਸ਼ੁਰੂ_gid= gids.get_min ();
    hb_codepoint_t prev_gid = ਸ਼ੁਰੂ_gid;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = gid_ਵਿੱਥ_map.get (ਸ਼ੁਰੂ_gid);
    ਬਿਨਾਨਿਸ਼ਾਨੀ prev_ਵਿੱਥ = ਵਿੱਥ;
    ਲਈ (ਅਚਲ hb_codepoint_t _ : gids.iter ())
    {
      ਜੇ (_ == ਸ਼ੁਰੂ_gid) ਜਾਰੀ;

      ਵਿੱਥ = gid_ਵਿੱਥ_map.get (_);
      ਜੇ (_ == prev_gid + 1 &&  ਵਿੱਥ == prev_ਵਿੱਥ)
      {
        prev_gid = _;
        ਜਾਰੀ;
      }

      Clip_ਰਿਕਾਰਡ ਰਿਕਾਰਡ;
      ਰਿਕਾਰਡ.ਸ਼ੁਰੂ_Glyph_ID = ਸ਼ੁਰੂ_gid;
      ਰਿਕਾਰਡ.ਸਿਰਾ_Glyph_ID = prev_gid;
      ਰਿਕਾਰਡ.clipBox = prev_ਵਿੱਥ;

      ਜੇ (!ਰਿਕਾਰਡ.subset (c, ਇਹ, instancer)) return_trace (0);
      ਗਿਣਤੀ_2++;

      ਸ਼ੁਰੂ_gid = _;
      prev_gid = _;
      prev_ਵਿੱਥ = ਵਿੱਥ;
    }

    //last one
    {
      Clip_ਰਿਕਾਰਡ ਰਿਕਾਰਡ;
      ਰਿਕਾਰਡ.ਸ਼ੁਰੂ_Glyph_ID = ਸ਼ੁਰੂ_gid;
      ਰਿਕਾਰਡ.ਸਿਰਾ_Glyph_ID = prev_gid;
      ਰਿਕਾਰਡ.clipBox = prev_ਵਿੱਥ;
      ਜੇ (!ਰਿਕਾਰਡ.subset (c, ਇਹ, instancer)) return_trace (0);
      ਗਿਣਤੀ_2++;
    }
    return_trace (ਗਿਣਤੀ_2);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->format, format, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) return_trace (ਝੂਠ);

    ਅਚਲ hb_ਸੈੱਟ_t& glyphset = c->plan->_glyphset_colred;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_ਵਿੱਥ_map;
    hb_ਸੈੱਟ_t new_gids;
    ਲਈ (ਅਚਲ Clip_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : clips.iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_gid = ਰਿਕਾਰਡ.ਸ਼ੁਰੂ_Glyph_ID;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_gid = ਰਿਕਾਰਡ.ਸਿਰਾ_Glyph_ID;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ gid = ਸ਼ੁਰੂ_gid; gid <= ਸਿਰਾ_gid; gid++)
      {
        ਜੇ (!glyphset.has (gid) || !glyph_map.has (gid)) ਜਾਰੀ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ new_gid = glyph_map.get (gid);
        new_gid_ਵਿੱਥ_map.ਸੈੱਟ (new_gid, ਰਿਕਾਰਡ.clipBox);
        new_gids.ਜੋੜੋ (new_gid);
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = serialize_clip_ਰਿਕਾਰਡਾਂ (c, instancer, new_gids, new_gid_ਵਿੱਥ_map);
    ਜੇ (!ਗਿਣਤੀ_2) return_trace (ਝੂਠ);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->clips.len, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (ਇਹ) && clips.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *rec = clips.as_array ().bsearch (gid);
    ਜੇ (rec)
      ਵਾਪਸ rec->get_extents (extents, ਇਹ, instancer);
    ਵਾਪਸ ਝੂਠ;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_ਦਾ<Clip_ਰਿਕਾਰਡ>	clips;  // Clip records, sorted by startGlyphID
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (5, clips);
};

ਬਣਤਰ Paint
{

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (unlikely (!c->check_ਸ਼ੁਰੂ_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ਮੁੱਲ ());

    return_trace (c->ਸਿਰਾ_recursion (ਇਹ->dispatch (c, std::forward<Ts> (ds)...)));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    ਹਾਲਤ 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    ਹਾਲਤ 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    ਹਾਲਤ 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    ਹਾਲਤ 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    ਹਾਲਤ 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    ਹਾਲਤ 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    ਹਾਲਤ 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    ਹਾਲਤ 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    ਹਾਲਤ 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    ਹਾਲਤ 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    ਹਾਲਤ 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    ਹਾਲਤ 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    ਹਾਲਤ 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    ਹਾਲਤ 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    ਹਾਲਤ 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    ਹਾਲਤ 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    ਹਾਲਤ 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    ਹਾਲਤ 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    ਹਾਲਤ 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    ਹਾਲਤ 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    ਹਾਲਤ 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    ਹਾਲਤ 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    ਹਾਲਤ 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    ਹਾਲਤ 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    ਹਾਲਤ 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    ਹਾਲਤ 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    ਹਾਲਤ 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    ਹਾਲਤ 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); ਵਾਪਸ ਸੱਚ;
    ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<Paint_ਰੇਖਿਕ_Gradient<NoVariable>>	paintformat4;
  Variable<Paint_ਰੇਖਿਕ_Gradient<Variable>>	paintformat5;
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
  NoVariable<Paint_Scale_ਇਕਸਾਰ>			paintformat20;
  Variable<Paint_Scale_ਇਕਸਾਰ>			paintformat21;
  NoVariable<Paint_Scale_ਇਕਸਾਰ_Around_Center>	paintformat22;
  Variable<Paint_Scale_ਇਕਸਾਰ_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_ਸੰਯੁਕਤ				paintformat32;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2);
};

ਬਣਤਰ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *s, ਅਚਲ hb_map_t* glyph_map,
                  ਅਚਲ ਖਾਲੀ* src_ਅਧਾਰ, hb_subset_context_t *c,
                  ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = s->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਜੇ (!s->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->paint.serialize_subset (c, paint, src_ਅਧਾਰ, instancer));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) && paint.sanitize (c, ਅਧਾਰ)));
  }

  ਜਨਤਕ:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_ਮੰਜ਼ਿਲ<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ ਅਧਾਰ_Glyph_List : Sorted_Array32_ਦਾ<ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ)))  return_trace (ਝੂਠ);
    ਅਚਲ hb_ਸੈੱਟ_t* glyphset = &c->plan->_glyphset_colred;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : as_array ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gid = _.glyphId;
      ਜੇ (!glyphset->has (gid)) ਜਾਰੀ;

      ਜੇ (_.serialize (c->serializer, c->plan->glyph_map, ਇਹ, c, instancer)) ਬਾਹਰਲਾ->len++;
      ਨਹੀਂਤਾਂ return_trace (ਝੂਠ);
    }

    return_trace (ਬਾਹਰਲਾ->len != 0);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (Sorted_Array32_ਦਾ<ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ>::sanitize (c, ਇਹ));
  }
};

ਬਣਤਰ LayerList : Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Paint>
{
  ਅਚਲ Paint& get_paint (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  { ਵਾਪਸ ਇਹ+(*ਇਹ)[i]; }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਵਸਤੂ_Var_Store_Instancer &instancer) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ)))  return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (*ਇਹ)
                         | hb_filter (c->plan->colrv1_layers, hb_ਪਹਿਲਾ))

    {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!o)) return_trace (ਝੂਠ);
      ret |= o->serialize_subset (c, _.second, ਇਹ, instancer);
    }
    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Paint>::sanitize (c, ਇਹ));
  }
};

ਬਣਤਰ delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_subset_plan_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਚੌੜਾਈ ()           ਅਚਲ { ਵਾਪਸ ((outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + 7) / 8); }
  hb_array_t<ਅਚਲ uint32_t> get_output_map () ਅਚਲ { ਵਾਪਸ output_map.as_array (); }

  delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_subset_plan_t (ਅਚਲ hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_ਗਿਣਤੀ = 0;
    outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 0;
    inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;
    output_map.init ();

    /* search backwards */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = new_deltaset_idx_varidx_map.get_population ();
    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ)-1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_varidx = (ਬਿਨਾਨਿਸ਼ਾਨੀ)-1;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਗਿਣਤੀ_2; i; i--)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ delta_ਸੈੱਟ_idx = i - 1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ var_idx = new_deltaset_idx_varidx_map.get (delta_ਸੈੱਟ_idx);
      ਜੇ (i == ਗਿਣਤੀ_2)
      {
        ਆਖਰੀ_idx = delta_ਸੈੱਟ_idx;
        ਆਖਰੀ_varidx = var_idx;
        ਜਾਰੀ;
      }
      ਜੇ (var_idx != ਆਖਰੀ_varidx)
        ਰੋਕੋ;
      ਆਖਰੀ_idx = delta_ਸੈੱਟ_idx;
    }

    map_ਗਿਣਤੀ = ਆਖਰੀ_idx + 1;
  }

  ਤਾਰਕਿਕ remap (ਅਚਲ hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;
    inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;

    ਜੇ (unlikely (!output_map.resize_dirty (map_ਗਿਣਤੀ))) ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0; idx < map_ਗਿਣਤੀ; idx++)
    {
      uint32_t *var_idx;
      ਜੇ (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) ਵਾਪਸ ਝੂਠ;
      output_map.arrayZ[idx] = *var_idx;

      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = (*var_idx) >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (outer == 0) ? 1 : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (outer);
      outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ, outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ);

      ਬਿਨਾਨਿਸ਼ਾਨੀ inner = (*var_idx) & 0xFFFF;
      ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = (inner == 0) ? 1 : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inner);
      inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ, inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ map_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
  hb_ਸਦਿਸ਼_t<uint32_t> output_map;
};

ਬਣਤਰ COLR
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_COLR;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ has_v0_ਅੰਕੜੇ () || ਸੰਸਕਰਣ; }

  ਤਾਰਕਿਕ has_v0_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਅੰਕ_ਅਧਾਰ_Glyphs; }
  ਤਾਰਕਿਕ has_v1_ਅੰਕੜੇ () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ < 1)
      ਵਾਪਸ ਝੂਠ;
    hb_barrier ();

    ਵਾਪਸ (ਇਹ+ਅਧਾਰ_Glyph_List_2).len > 0;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_layers (hb_codepoint_t       glyph,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ         ਸ਼ੁਰੂ_ਵਿੱਥ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        *ਗਿਣਤੀ_2, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = (ਇਹ+ਅਧਾਰ_Glyphs_Z).bsearch (ਅੰਕ_ਅਧਾਰ_Glyphs, glyph);

    hb_array_t<ਅਚਲ Layer_ਰਿਕਾਰਡ> all_layers = (ਇਹ+layersZ).as_array (ਅੰਕ_Layers);
    hb_array_t<ਅਚਲ Layer_ਰਿਕਾਰਡ> glyph_layers = all_layers.sub_array (ਰਿਕਾਰਡ.ਪਹਿਲਾ_Layer_Idx,
								       ਰਿਕਾਰਡ.ਅੰਕ_Layers);
    ਜੇ (ਗਿਣਤੀ_2 && layers)
    {
      + glyph_layers.sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਗਿਣਤੀ_2)
      | hb_sink (hb_array (layers, *ਗਿਣਤੀ_2))
      ;
    }
    ਵਾਪਸ glyph_layers.ਲੰਮਾਈ;
  }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<COLR> (face); }

    ~accelerator_t ()
    {
      ਸਵੈਚਾਲਿਤ *scratch = cached_scratch.get_relaxed ();
      ਜੇ (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ colr->has_ਅੰਕੜੇ (); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
    ਤਾਰਕਿਕ
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) ਅਚਲ
    {
      ਜੇ (unlikely (!has_ਅੰਕੜੇ ())) ਵਾਪਸ ਝੂਠ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਸੱਚ;
      ਤਾਰਕਿਕ ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      ਵਾਪਸ ret;
    }

    ਤਾਰਕਿਕ paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, ਖਾਲੀ *ਅੰਕੜੇ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
		      hb_color_t foreground,
		      ਤਾਰਕਿਕ clip = ਸੱਚ) ਅਚਲ
    {
      ਜੇ (unlikely (!has_ਅੰਕੜੇ ())) ਵਾਪਸ ਝੂਠ;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ਜੇ (unlikely (!scratch)) ਵਾਪਸ ਸੱਚ;
      ਤਾਰਕਿਕ ret = colr->paint_glyph (font, glyph, funcs, ਅੰਕੜੇ, palette_ਸੂਚਕਾਂਕ, foreground, clip, *scratch);
      release_scratch (scratch);
      ਵਾਪਸ ret;
    }
#ਜੇ_ਅੰਤ

    ਤਾਰਕਿਕ is_valid () { ਵਾਪਸ colr.get_blob ()->ਲੰਮਾਈ; }

    ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
    {
      ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ;
      colr->depend (depend_ਅੰਕੜੇ);
    }

    ਖਾਲੀ ਘੇਰਾ_glyphs (hb_codepoint_t glyph,
			 hb_ਸੈੱਟ_t *related_ids /*ਬਾਹਰਲਾ*/) ਅਚਲ
    { colr->ਘੇਰਾ_glyphs (glyph, related_ids); }

    ਖਾਲੀ ਘੇਰਾ_V0palette_indices (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
				    hb_ਸੈੱਟ_t *palettes /*ਬਾਹਰਲਾ*/) ਅਚਲ
    { colr->ਘੇਰਾ_V0palette_indices (glyphs, palettes); }

    ਖਾਲੀ ਘੇਰਾ_for_V1 (hb_ਸੈੱਟ_t *glyphset,
                        hb_ਸੈੱਟ_t *layer_indices,
                        hb_ਸੈੱਟ_t *palette_indices,
                        hb_ਸੈੱਟ_t *variation_indices,
                        hb_ਸੈੱਟ_t *delta_ਸੈੱਟ_indices) ਅਚਲ
    { colr->ਘੇਰਾ_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_ਸੈੱਟ_indices); }

    ਤਾਰਕਿਕ has_var_store () ਅਚਲ
    { ਵਾਪਸ colr->has_var_store (); }

    ਅਚਲ ਵਸਤੂ_Variation_Store &get_var_store () ਅਚਲ
    { ਵਾਪਸ colr->get_var_store (); }
    ਅਚਲ ਵਸਤੂ_Variation_Store *get_var_store_ptr () ਅਚਲ
    { ਵਾਪਸ colr->get_var_store_ptr (); }

    ਤਾਰਕਿਕ has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () ਅਚਲ
    { ਵਾਪਸ colr->has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map (); }

    ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () ਅਚਲ
    { ਵਾਪਸ colr->get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map (); }
    ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr () ਅਚਲ
    { ਵਾਪਸ colr->get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr (); }

    ਨਿਜੀ:

    hb_colr_scratch_t *acquire_scratch () ਅਚਲ
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      ਜੇ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, ਖਾਲੀ_ਸੂਚਕ)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, ਆਕਾਰ (hb_colr_scratch_t));
	ਜੇ (unlikely (!scratch))
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      }

      ਵਾਪਸ scratch;
    }
    ਖਾਲੀ release_scratch (hb_colr_scratch_t *scratch) ਅਚਲ
    {
      ਜੇ (!cached_scratch.cmpexch (ਖਾਲੀ_ਸੂਚਕ, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    ਜਨਤਕ:
    hb_blob_ptr_t<COLR> colr;
    ਨਿਜੀ:
    ਬਦਲਣਯੋਗ hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
  {
    // v0
    hb_array_t<ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ> ਅਧਾਰ_Glyphs = (ਇਹ+ਅਧਾਰ_Glyphs_Z).as_array (ਅੰਕ_ਅਧਾਰ_Glyphs);
    ਲਈ (ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਅਧਾਰ_Glyphs)
    {
      ਸਵੈਚਾਲਿਤ glyph_layers = (ਇਹ+layersZ).as_array (ਅੰਕ_Layers).sub_array (ਰਿਕਾਰਡ.ਪਹਿਲਾ_Layer_Idx,
                                                                         ਰਿਕਾਰਡ.ਅੰਕ_Layers);
      ਲਈ (ਅਚਲ Layer_ਰਿਕਾਰਡ layer : glyph_layers)
        depend_ਅੰਕੜੇ->ਜੋੜੋ_depend(ਰਿਕਾਰਡ.glyphId, HB_OT_ਟੈਗ_COLR, layer.glyphId);
    }

    ਜੇ (!has_v1_ਅੰਕੜੇ())
      ਵਾਪਸ;

    // v1
    hb_colrv1_depend_context_t c (ਇਹ, depend_ਅੰਕੜੇ);
    ਅਚਲ ਅਧਾਰ_Glyph_List &baseglyph_paintrecords = ਇਹ+ਅਧਾਰ_Glyph_List_2;

    ਲਈ (ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.ਸਰੋਤ_gid = baseglyph_paintrecord.glyphId;
      c.ਦੇਖਿਆ_paint.clear ();

      ਅਚਲ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_codepoint_t glyph,
		       hb_ਸੈੱਟ_t *related_ids /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = get_ਅਧਾਰ_glyph_ਰਿਕਾਰਡ (glyph);
    ਜੇ (!ਰਿਕਾਰਡ) ਵਾਪਸ;

    ਸਵੈਚਾਲਿਤ glyph_layers = (ਇਹ+layersZ).as_array (ਅੰਕ_Layers).sub_array (ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx,
								       ਰਿਕਾਰਡ->ਅੰਕ_Layers);
    ਜੇ (!glyph_layers.ਲੰਮਾਈ) ਵਾਪਸ;
    related_ids->ਜੋੜੋ_array (&glyph_layers[0].glyphId, glyph_layers.ਲੰਮਾਈ, Layer_ਰਿਕਾਰਡ::min_ਮਾਪ);
  }

  ਖਾਲੀ ਘੇਰਾ_V0palette_indices (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
				  hb_ਸੈੱਟ_t *palettes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (!ਅੰਕ_ਅਧਾਰ_Glyphs || !ਅੰਕ_Layers) ਵਾਪਸ;
    hb_array_t<ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ> ਅਧਾਰ_Glyphs = (ਇਹ+ਅਧਾਰ_Glyphs_Z).as_array (ਅੰਕ_ਅਧਾਰ_Glyphs);
    hb_array_t<ਅਚਲ Layer_ਰਿਕਾਰਡ> all_layers = (ਇਹ+layersZ).as_array (ਅੰਕ_Layers);

    ਲਈ (ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ ਰਿਕਾਰਡ : ਅਧਾਰ_Glyphs)
    {
      ਜੇ (!glyphs->has (ਰਿਕਾਰਡ.glyphId)) ਜਾਰੀ;
      hb_array_t<ਅਚਲ Layer_ਰਿਕਾਰਡ> glyph_layers = all_layers.sub_array (ਰਿਕਾਰਡ.ਪਹਿਲਾ_Layer_Idx,
                                                                   ਰਿਕਾਰਡ.ਅੰਕ_Layers);
      ਲਈ (ਅਚਲ Layer_ਰਿਕਾਰਡ layer : glyph_layers)
        palettes->ਜੋੜੋ (layer.colorIdx);
    }
  }

  ਖਾਲੀ ਘੇਰਾ_for_V1 (hb_ਸੈੱਟ_t *glyphset,
                      hb_ਸੈੱਟ_t *layer_indices,
                      hb_ਸੈੱਟ_t *palette_indices,
                      hb_ਸੈੱਟ_t *variation_indices,
                      hb_ਸੈੱਟ_t *delta_ਸੈੱਟ_indices) ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ < 1) ਵਾਪਸ;
    hb_barrier ();

    hb_ਸੈੱਟ_t ਦੇਖਿਆ_glyphs;

    hb_colrv1_ਘੇਰਾ_context_t c (ਇਹ, &ਦੇਖਿਆ_glyphs, layer_indices, palette_indices, variation_indices);
    ਅਚਲ ਅਧਾਰ_Glyph_List &baseglyph_paintrecords = ਇਹ+ਅਧਾਰ_Glyph_List_2;

    ਲਈ (ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gid = baseglyph_paintrecord.glyphId;
      ਜੇ (!glyphset->has (gid)) ਜਾਰੀ;

      ਅਚਲ Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_ਸੈੱਟ_union (glyphset, &ਦੇਖਿਆ_glyphs);

    ਅਚਲ ClipList &cliplist = ਇਹ+clipList;
    c.glyphs = glyphset;
    ਲਈ (ਅਚਲ Clip_ਰਿਕਾਰਡ &clip_ਰਿਕਾਰਡ : cliplist.clips.iter())
      clip_ਰਿਕਾਰਡ.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    ਜੇ (has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map ())
    {
      ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &var_idx_map = ਇਹ+varIdxMap;
      delta_ਸੈੱਟ_indices->ਸੈੱਟ (*variation_indices);
      variation_indices->clear ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ delta_ਸੈੱਟ_idx : *delta_ਸੈੱਟ_indices)
        variation_indices->ਜੋੜੋ (var_idx_map.map (delta_ਸੈੱਟ_idx));
    }
  }

  ਅਚਲ LayerList& get_layerList () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+layerList); }

  ਅਚਲ ਅਧਾਰ_Glyph_List& get_baseglyphList () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+ਅਧਾਰ_Glyph_List_2); }

  ਤਾਰਕਿਕ has_var_store () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ >= 1 && hb_barrier () && varStore != 0; }

  ਤਾਰਕਿਕ has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ >= 1 && hb_barrier () && varIdxMap != 0; }

  ਤਾਰਕਿਕ has_clip_list () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ >= 1 && hb_barrier () && clipList != 0; }

  ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () ਅਚਲ
  { ਵਾਪਸ has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () && hb_barrier () ? ਇਹ+varIdxMap : Null (Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map); }
  ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr () ਅਚਲ
  { ਵਾਪਸ has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map () && hb_barrier () ? &(ਇਹ+varIdxMap) : ਖਾਲੀ_ਸੂਚਕ; }

  ਅਚਲ ਵਸਤੂ_Variation_Store &get_var_store () ਅਚਲ
  { ਵਾਪਸ has_var_store () && hb_barrier () ? ਇਹ+varStore : Null (ਵਸਤੂ_Variation_Store); }
  ਅਚਲ ਵਸਤੂ_Variation_Store *get_var_store_ptr () ਅਚਲ
  { ਵਾਪਸ has_var_store () && hb_barrier () ? &(ਇਹ+varStore) : ਖਾਲੀ_ਸੂਚਕ; }

  ਅਚਲ ClipList &get_clip_list () ਅਚਲ
  { ਵਾਪਸ has_clip_list () && hb_barrier () ? ਇਹ+clipList : Null (ClipList); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
                  (ਇਹ+ਅਧਾਰ_Glyphs_Z).sanitize (c, ਅੰਕ_ਅਧਾਰ_Glyphs) &&
                  (ਇਹ+layersZ).sanitize (c, ਅੰਕ_Layers) &&
                  (ਸੰਸਕਰਣ == 0 ||
		   (hb_barrier () &&
		    ਅਧਾਰ_Glyph_List_2.sanitize (c, ਇਹ) &&
		    layerList.sanitize (c, ਇਹ) &&
		    clipList.sanitize (c, ਇਹ) &&
		    varIdxMap.sanitize (c, ਇਹ) &&
		    varStore.sanitize (c, ਇਹ))));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Iterator, ਕਿਸਮ_ਨਾਂ LayerIterator,
	   hb_requires (hb_is_iterator (ਅਧਾਰ_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  ਤਾਰਕਿਕ serialize_V0 (hb_serialize_context_t *c,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਸਕਰਣ,
		     ਅਧਾਰ_Iterator ਅਧਾਰ_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (ਅਧਾਰ_it.len () != layer_it.len ()))
      return_trace (ਝੂਠ);

    ਇਹ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;
    ਅੰਕ_Layers = 0;
    ਅੰਕ_ਅਧਾਰ_Glyphs = ਅਧਾਰ_it.len ();
    ਜੇ (ਅੰਕ_ਅਧਾਰ_Glyphs == 0)
    {
      ਅਧਾਰ_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (ਸੱਚ);
    }

    c->push ();
    ਲਈ (ਅਚਲ hb_ਵਸਤੂ_type<ਅਧਾਰ_Iterator> _ : + ਅਧਾਰ_it.iter ())
    {
      ਸਵੈਚਾਲਿਤ* ਰਿਕਾਰਡ = c->embed (_);
      ਜੇ (unlikely (!ਰਿਕਾਰਡ)) return_trace (ਝੂਠ);
      ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx = ਅੰਕ_Layers;
      ਅੰਕ_Layers += ਰਿਕਾਰਡ->ਅੰਕ_Layers;
    }
    c->ਜੋੜੋ_link (ਅਧਾਰ_Glyphs_Z, c->ਕੱਢੋ_pack ());

    c->push ();
    ਲਈ (ਅਚਲ hb_ਵਸਤੂ_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->ਜੋੜੋ_link (layersZ, c->ਕੱਢੋ_pack ());

    return_trace (ਸੱਚ);
  }

  ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ* get_ਅਧਾਰ_glyph_ਰਿਕਾਰਡ (hb_codepoint_t gid) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ = &(ਇਹ+ਅਧਾਰ_Glyphs_Z).bsearch (ਅੰਕ_ਅਧਾਰ_Glyphs, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) gid);
    ਜੇ (ਰਿਕਾਰਡ == &Null (ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ) ||
        (ਰਿਕਾਰਡ && (hb_codepoint_t) ਰਿਕਾਰਡ->glyphId != gid))
      ਰਿਕਾਰਡ = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ ਰਿਕਾਰਡ;
  }

  ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ* get_ਅਧਾਰ_glyph_paintrecord (hb_codepoint_t gid) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Glyph_List &list = ਇਹ+ਅਧਾਰ_Glyph_List_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਜੇ (!list.bfind ((ਬਿਨਾਨਿਸ਼ਾਨੀ) gid, &i))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ &list[i];
  }

  ਤਾਰਕਿਕ downgrade_ਮੰਜ਼ਿਲ_V0 (ਅਚਲ hb_ਸੈੱਟ_t &glyphset) ਅਚਲ
  {
    //no more COLRv1 glyphs, downgrade to version 0
    ਲਈ (ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ& _ : get_baseglyphList ())
      ਜੇ (glyphset.has (_.glyphId))
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ subset_varstore (hb_subset_context_t *c,
                        COLR* ਬਾਹਰਲਾ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (ਸੱਚ);

    ਅਚਲ ਵਸਤੂ_Variation_Store& var_store = ਇਹ+varStore;
    ਜੇ (c->plan->normalized_coords)
    {
      ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      ਤਾਰਕਿਕ optimize = (varIdxMap != 0) ? ਸੱਚ : ਝੂਠ;
      ਜੇ (!ਵਸਤੂ_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (ਝੂਠ);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      ਜੇ (ਵਸਤੂ_vars.get_region_list () &&
          ਵਸਤੂ_vars.get_vardata_encodings () &&
          !ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
                                              ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
                                              c->plan->axis_ਟੈਗ_ਸੂਚੀ,
                                              ਵਸਤੂ_vars.get_region_list (),
                                              ਵਸਤੂ_vars.get_vardata_encodings ()))
        return_trace (ਝੂਠ);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      ਜੇ (optimize)
      {
        ਅਚਲ hb_map_t &varidx_map = ਵਸਤੂ_vars.get_varidx_map ();
        ਲਈ (ਸਵੈਚਾਲਿਤ _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          ਜੇ (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          ਨਹੀਂਤਾਂ
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    ਨਹੀਂਤਾਂ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_ਸੈੱਟ_t dead_regions;
      ਜੇ (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      ਜੇ (unlikely (!ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : ਖਾਲੀ_ਸੂਚਕ)))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map (hb_subset_context_t *c,
                                   COLR* ਬਾਹਰਲਾ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (ਸੱਚ);

    ਅਚਲ hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_subset_plan_t ਸੂਚਕਾਂਕ_map_plan (deltaset_idx_varidx_map);

    ਜੇ (unlikely (!ਸੂਚਕਾਂਕ_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->varIdxMap.serialize_serialize (c->serializer, ਸੂਚਕਾਂਕ_map_plan));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_map_t &ਉਲਟ_glyph_map = *c->plan->ਉਲਟ_glyph_map;
    ਅਚਲ hb_ਸੈੱਟ_t& glyphset = c->plan->_glyphset_colred;

    ਸਵੈਚਾਲਿਤ ਅਧਾਰ_it =
    + hb_range (c->plan->ਅੰਕ_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = ਉਲਟ_glyph_map.get (new_gid);
		    ਜੇ (glyphset.has (old_gid)) ਵਾਪਸ ਸੱਚ;
		    ਵਾਪਸ ਝੂਠ;
		 })
    | hb_map_retains_ਛਾਂਟੀ ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = ਉਲਟ_glyph_map.get (new_gid);

				ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ* old_ਰਿਕਾਰਡ = get_ਅਧਾਰ_glyph_ਰਿਕਾਰਡ (old_gid);
				ਜੇ (unlikely (!old_ਰਿਕਾਰਡ))
				  ਵਾਪਸ hb_pair_t<ਤਾਰਕਿਕ, ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ> (ਝੂਠ, Null (ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ));
				ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ new_ਰਿਕਾਰਡ = {};
				new_ਰਿਕਾਰਡ.glyphId = new_gid;
				new_ਰਿਕਾਰਡ.ਅੰਕ_Layers = old_ਰਿਕਾਰਡ->ਅੰਕ_Layers;
				ਵਾਪਸ hb_pair_t<ਤਾਰਕਿਕ, ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ> (ਸੱਚ, new_ਰਿਕਾਰਡ);
			      })
    | hb_filter (hb_ਪਹਿਲਾ)
    | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ)
    ;

    ਸਵੈਚਾਲਿਤ layer_it =
    + hb_range (c->plan->ਅੰਕ_output_glyphs ())
    | hb_map (ਉਲਟ_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_ਛਾਂਟੀ ([&](hb_codepoint_t old_gid)
			      {
				ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ* old_ਰਿਕਾਰਡ = get_ਅਧਾਰ_glyph_ਰਿਕਾਰਡ (old_gid);
				hb_ਸਦਿਸ਼_t<Layer_ਰਿਕਾਰਡ> ਬਾਹਰਲਾ_layers;

				ਜੇ (unlikely (!old_ਰਿਕਾਰਡ ||
					      old_ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx >= ਅੰਕ_Layers ||
					      old_ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx + old_ਰਿਕਾਰਡ->ਅੰਕ_Layers > ਅੰਕ_Layers))
				  ਵਾਪਸ hb_pair_t<ਤਾਰਕਿਕ, hb_ਸਦਿਸ਼_t<Layer_ਰਿਕਾਰਡ>> (ਝੂਠ, ਬਾਹਰਲਾ_layers);

				ਸਵੈਚਾਲਿਤ layers = (ਇਹ+layersZ).as_array (ਅੰਕ_Layers).sub_array (old_ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx,
											     old_ਰਿਕਾਰਡ->ਅੰਕ_Layers);
				ਬਾਹਰਲਾ_layers.resize (layers.ਲੰਮਾਈ);
				ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < layers.ਲੰਮਾਈ; i++) {
				  ਬਾਹਰਲਾ_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  ਜੇ (unlikely (!c->plan->new_gid_for_old_gid (ਬਾਹਰਲਾ_layers[i].glyphId, &new_gid)))
				    ਵਾਪਸ hb_pair_t<ਤਾਰਕਿਕ, hb_ਸਦਿਸ਼_t<Layer_ਰਿਕਾਰਡ>> (ਝੂਠ, ਬਾਹਰਲਾ_layers);
				  ਬਾਹਰਲਾ_layers[i].glyphId = new_gid;
				  ਬਾਹਰਲਾ_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				ਵਾਪਸ hb_pair_t<ਤਾਰਕਿਕ, hb_ਸਦਿਸ਼_t<Layer_ਰਿਕਾਰਡ>> (ਸੱਚ, ਬਾਹਰਲਾ_layers);
			      })
    | hb_filter (hb_ਪਹਿਲਾ)
    | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ)
    ;

    ਜੇ (ਸੰਸਕਰਣ == 0 && (!ਅਧਾਰ_it || !layer_it))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *colr_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<COLR> ();
    ਜੇ (unlikely (!c->serializer->extend_min (colr_ਅਭਾਜ)))  return_trace (ਝੂਠ);

    ਜੇ (ਸੰਸਕਰਣ == 0 || downgrade_ਮੰਜ਼ਿਲ_V0 (glyphset))
      return_trace (colr_ਅਭਾਜ->serialize_V0 (c->serializer, 0, ਅਧਾਰ_it, layer_it));

    hb_barrier ();

    //start version 1
    ਜੇ (!c->serializer->allocate_ਮਾਪ<ਖਾਲੀ> (5 * HBUINT32::static_ਮਾਪ)) return_trace (ਝੂਠ);
    ਜੇ (!colr_ਅਭਾਜ->serialize_V0 (c->serializer, ਸੰਸਕਰਣ, ਅਧਾਰ_it, layer_it)) return_trace (ਝੂਠ);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    ਜੇ (!subset_varstore (c, colr_ਅਭਾਜ)) return_trace (ਝੂਠ);

    ਵਸਤੂ_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : ਖਾਲੀ_ਸੂਚਕ,
				     c->plan->normalized_coords ? get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr () : ਖਾਲੀ_ਸੂਚਕ,
				     c->plan->normalized_coords.as_array ());

    ਜੇ (!colr_ਅਭਾਜ->ਅਧਾਰ_Glyph_List_2.serialize_subset (c, ਅਧਾਰ_Glyph_List_2, ਇਹ, instancer))
      return_trace (ਝੂਠ);

    colr_ਅਭਾਜ->layerList.serialize_subset (c, layerList, ਇਹ, instancer);
    colr_ਅਭਾਜ->clipList.serialize_subset (c, clipList, ਇਹ, instancer);

    return_trace (subset_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map (c, colr_ਅਭਾਜ));
  }

  ਅਚਲ Paint *get_ਅਧਾਰ_glyph_paint (hb_codepoint_t glyph) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Glyph_List &baseglyph_paintrecords = ਇਹ+ਅਧਾਰ_Glyph_List_2;
    ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ = get_ਅਧਾਰ_glyph_paintrecord (glyph);
    ਜੇ (ਰਿਕਾਰਡ)
    {
      ਅਚਲ Paint &paint = &baseglyph_paintrecords+ਰਿਕਾਰਡ->paint;
      ਵਾਪਸ &paint;
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
  ਤਾਰਕਿਕ
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) ਅਚਲ
  {

    ਵਸਤੂ_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr (),
                                     hb_array (font->coords, font->ਅੰਕ_coords));

    ਜੇ (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      ਵਾਪਸ ਸੱਚ;
    }

    ਸਵੈਚਾਲਿਤ *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    ਤਾਰਕਿਕ ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), ਸੱਚ, scratch);

    ਸਵੈਚਾਲਿਤ e = scratch.paint_extents.get_extents ();
    ਜੇ (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->ਚੌੜਾਈ = 0;
      extents->ਉਚਾਈ = 0;
    }
    ਨਹੀਂਤਾਂ
      *extents = e.ਮੰਜ਼ਿਲ_glyph_extents ();

    ਵਾਪਸ ret;
  }
#ਜੇ_ਅੰਤ

  ਤਾਰਕਿਕ
  has_paint_for_glyph (hb_codepoint_t glyph) ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ >= 1)
    {
      hb_barrier ();

      ਅਚਲ Paint *paint = get_ਅਧਾਰ_glyph_paint (glyph);

      ਵਾਪਸ paint != ਖਾਲੀ_ਸੂਚਕ;
    }

    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 ਅਚਲ ਵਸਤੂ_Var_Store_Instancer instancer) ਅਚਲ
  {
    ਵਾਪਸ get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
  ਤਾਰਕਿਕ
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, ਖਾਲੀ *ਅੰਕੜੇ,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ, hb_color_t foreground,
	       ਤਾਰਕਿਕ clip,
	       hb_colr_scratch_t &scratch) ਅਚਲ
  {
    ਵਸਤੂ_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->ਅੰਕ_coords : 0));
    hb_paint_context_t c (ਇਹ, funcs, ਅੰਕੜੇ, font, palette_ਸੂਚਕਾਂਕ, foreground, instancer);

    hb_decycler_ਗੰਢ_t node (c.glyphs_decycler);
    node.ਦੇਖੋ (glyph);

    ਜੇ (ਸੰਸਕਰਣ >= 1)
    {
      hb_barrier ();

      ਅਚਲ Paint *paint = get_ਅਧਾਰ_glyph_paint (glyph);
      ਜੇ (paint)
      {
        // COLRv1 glyph

	ਤਾਰਕਿਕ is_bounded = ਸੱਚ;
	hb_glyph_extents_t extents = {};
	ਜੇ (clip)
	{
	  ਜੇ (!get_clip (glyph, &extents, instancer))
	  {
	    clip = ਝੂਠ;
	    is_bounded = ਝੂਠ;
	  }

	  ਜੇ (!is_bounded)
	  {
	    ਸਵੈਚਾਲਿਤ *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_ਸੂਚਕਾਂਕ, foreground,
			 ਝੂਠ,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->ਧੱਕੋ_font_transform (c.ਅੰਕੜੇ, font);

	ਜੇ (clip)
	  c.funcs->ਧੱਕੋ_clip_rectangle (c.ਅੰਕੜੇ,
					(ਦਸ਼ਮਲਵ) extents.x_bearing,
					(ਦਸ਼ਮਲਵ) extents.y_bearing + extents.ਉਚਾਈ,
					(ਦਸ਼ਮਲਵ) extents.x_bearing + extents.ਚੌੜਾਈ,
					(ਦਸ਼ਮਲਵ) extents.y_bearing);

	ਜੇ (is_bounded)
	  c.recurse (*paint);

	ਜੇ (clip)
	  c.funcs->ਕੱਢੋ_clip (c.ਅੰਕੜੇ);

	c.funcs->ਕੱਢੋ_transform (c.ਅੰਕੜੇ);

        ਵਾਪਸ ਸੱਚ;
      }
    }

    ਅਚਲ ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = get_ਅਧਾਰ_glyph_ਰਿਕਾਰਡ (glyph);
    ਜੇ (ਰਿਕਾਰਡ && ((hb_codepoint_t) ਰਿਕਾਰਡ->glyphId == glyph))
    {
      // COLRv0 glyph
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &r : (ਇਹ+layersZ).as_array (ਅੰਕ_Layers)
			   .sub_array (ਰਿਕਾਰਡ->ਪਹਿਲਾ_Layer_Idx, ਰਿਕਾਰਡ->ਅੰਕ_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.ਅੰਕੜੇ, r.glyphId, c.font, is_foreground, color);
      }

      ਵਾਪਸ ਸੱਚ;
    }

    ਵਾਪਸ ਝੂਠ;
  }
#ਜੇ_ਅੰਤ

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Table version number (starts at 0). */
  HBUINT16	ਅੰਕ_ਅਧਾਰ_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_ਮੰਜ਼ਿਲ<Sorted_Unsized_Array_ਦਾ<ਅਧਾਰ_Glyph_ਰਿਕਾਰਡ>>
		ਅਧਾਰ_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<Layer_ਰਿਕਾਰਡ>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	ਅੰਕ_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Glyph_List>		ਅਧਾਰ_Glyph_List_2;
  Offset32_ਮੰਜ਼ਿਲ<LayerList>			layerList;
  Offset32_ਮੰਜ਼ਿਲ<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>	varStore;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (14);
};

ਬਣਤਰ COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

ਖਾਲੀ
hb_paint_context_t::recurse (ਅਚਲ Paint &paint)
{
  ਜੇ (unlikely (ਡੂੰਘਾਈ_ਖੱਬਾ <= 0 || ਕਿਨਾਰਾ_ਗਿਣਤੀ <= 0)) ਵਾਪਸ;
  ਡੂੰਘਾਈ_ਖੱਬਾ--;
  ਕਿਨਾਰਾ_ਗਿਣਤੀ--;
  paint.dispatch (ਇਹ);
  ਡੂੰਘਾਈ_ਖੱਬਾ++;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਖਾਲੀ PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) ਅਚਲ
{
  TRACE_PAINT (ਇਹ);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  ਜੇ (likely (c->ਡੂੰਘਾਈ_ਖੱਬਾ > 0 && c->ਕਿਨਾਰਾ_ਗਿਣਤੀ > 0) &&
      (ਇਹ+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->ਕਿਨਾਰਾ_ਗਿਣਤੀ--;
    c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
    c->funcs->fill_glyph (c->ਅੰਕੜੇ, gid, c->font, is_foreground, color);
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
  c->funcs->ਧੱਕੋ_clip_glyph (c->ਅੰਕੜੇ, gid, c->font);
  c->funcs->ਧੱਕੋ_font_transform (c->ਅੰਕੜੇ, c->font);
  c->recurse (ਇਹ+paint);
  c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
  c->funcs->ਕੱਢੋ_clip (c->ਅੰਕੜੇ);
  c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
}

ਖਾਲੀ PaintColrLayers::paint_glyph (hb_paint_context_t *c) ਅਚਲ
{
  TRACE_PAINT (ਇਹ);
  ਅਚਲ LayerList &paint_ਵਿੱਥ_lists = c->get_colr_ਸਾਰਣੀ ()->get_layerList ();
  hb_decycler_ਗੰਢ_t node (c->layers_decycler);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ; i < ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ + ਅੰਕ_Layers; i++)
  {
    ਜੇ (unlikely (!node.ਦੇਖੋ (i)))
      ਵਾਪਸ;

    ਅਚਲ Paint &paint = paint_ਵਿੱਥ_lists.get_paint (i);
    c->recurse (paint);
  }
}

ਖਾਲੀ PaintColrGlyph::paint_glyph (hb_paint_context_t *c) ਅਚਲ
{
  TRACE_PAINT (ਇਹ);

  hb_decycler_ਗੰਢ_t node (c->glyphs_decycler);
  ਜੇ (unlikely (!node.ਦੇਖੋ (gid)))
    ਵਾਪਸ;

  c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
  ਜੇ (c->funcs->color_glyph (c->ਅੰਕੜੇ, gid, c->font))
  {
    c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    ਵਾਪਸ;
  }
  c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);

  ਅਚਲ COLR *colr_ਸਾਰਣੀ = c->get_colr_ਸਾਰਣੀ ();
  ਅਚਲ Paint *paint = colr_ਸਾਰਣੀ->get_ਅਧਾਰ_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  ਤਾਰਕਿਕ has_clip_box = colr_ਸਾਰਣੀ->get_clip (gid, &extents, c->instancer);

  ਜੇ (has_clip_box)
    c->funcs->ਧੱਕੋ_clip_rectangle (c->ਅੰਕੜੇ,
				   (ਦਸ਼ਮਲਵ) extents.x_bearing,
				   (ਦਸ਼ਮਲਵ) extents.y_bearing + extents.ਉਚਾਈ,
				   (ਦਸ਼ਮਲਵ) extents.x_bearing + extents.ਚੌੜਾਈ,
				   (ਦਸ਼ਮਲਵ) extents.y_bearing);

  ਜੇ (paint)
    c->recurse (*paint);

  ਜੇ (has_clip_box)
    c->funcs->ਕੱਢੋ_clip (c->ਅੰਕੜੇ);
}

} /* namespace OT */

#ਜੇ_ਅੰਤ /* OT_COLOR_COLR_COLR_HH */
