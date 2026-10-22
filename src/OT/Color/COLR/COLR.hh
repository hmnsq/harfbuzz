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

#నిర్వచితం_కాకపోతే OT_COLOR_COLR_COLR_HH
#నిర్వచించు OT_COLOR_COLR_COLR_HH

#అన్తర్భావ "../../../hb.hh"
#అన్తర్భావ "../../../hb-decycler.hh"
#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "../../../hb-ot-var-common.hh"
#అన్తర్భావ "../../../hb-paint.hh"
#అన్తర్భావ "../../../hb-paint-bounded.hh"
#అన్తర్భావ "../../../hb-paint-extents.hh"
#అన్తర్భావ "../../../hb-depend-data.hh"

#అన్తర్భావ "../../../hb-ot-dual.hh"
#అన్తర్భావ "../CPAL/CPAL.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
#నిర్వచించు HB_OT_ట్యాగ్_COLR HB_ట్యాగ్('C','O','L','R')

నామాకాశ OT {
నిర్మాణం hb_paint_context_t;
}

నిర్మాణం hb_colr_scratch_t
{
  hb_paint_bounded_context_t paint_bounded;
  hb_paint_extents_context_t paint_extents;
};

నామాకాశ OT {

నిర్మాణం COLR;

నిర్మాణం Paint;

నిర్మాణం hb_paint_context_t :
       hb_dispatch_context_t<hb_paint_context_t>
{
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "PAINT"; }
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.paint_glyph (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  స్థిరరాశి COLR* get_colr_పట్టిక () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి COLR *> (పునాది); }

బహిరంగ:
  స్థిరరాశి శూన్య *పునాది;
  hb_paint_funcs_t *funcs;
  శూన్య *దత్తాంశం;
  hb_font_t *font;
  hb_array_t<స్థిరరాశి BGRAColor> palette;
  hb_color_t foreground;
  అంశం_Var_Store_Instancer &instancer;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  పూర్ణాంకం లోతు_ఎడమ = HB_MAX_NESTING_LEVEL;
  పూర్ణాంకం అంచు_లెక్క = HB_MAX_లేఖాచిత్రం_అంచు_లెక్క;

  hb_paint_context_t (స్థిరరాశి శూన్య *పునాది_2,
		      hb_paint_funcs_t *funcs_,
		      శూన్య *దత్తాంశం_2,
                      hb_font_t *font_,
                      గుర్తులేని పూర్ణాంకం palette_,
                      hb_color_t foreground_,
		      అంశం_Var_Store_Instancer &instancer_) :
    పునాది (పునాది_2),
    funcs (funcs_),
    దత్తాంశం (దత్తాంశం_2),
    font (font_),
    palette (
#నిర్వచితం_కాకపోతే HB_NO_COLOR
	     // https://github.com/harfbuzz/harfbuzz/issues/5116
	     font->face->పట్టిక.CPAL->get_palette_colors (palette_ < font->face->పట్టిక.CPAL->get_palette_లెక్క () ? palette_ : 0)
#అయితే_ముగింపు
    ),
    foreground (foreground_),
    instancer (instancer_)
  {
    ఒకవేళ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_సమితి_synthetic_bold (font, 0, 0, నిజం);
      hb_font_సమితి_synthetic_slant (font, 0);
    }
    లేకపోతే
      hb_font_reference (font);
  }

  ~hb_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  స్థిర గుర్తులేని
  color_alpha (hb_color_t color, దశాంశం alpha)
  {
    /* Font-supplied alpha is F2DOT14 plus variation deltas, so it can be
     * negative or greater than one; converting that to unsigned is UB. */
    తిరిగి roundf (hb_color_get_alpha (color) * hb_clamp (alpha, 0.f, 1.f));
  }

  hb_color_t get_color (గుర్తులేని పూర్ణాంకం color_సూచీ, దశాంశం alpha, hb_bool_t *is_foreground)
  {
    hb_color_t color = foreground;

    *is_foreground = నిజం;

    ఒకవేళ (color_సూచీ != 0xffff)
    {
      ఒకవేళ (!funcs->custom_palette_color (దత్తాంశం, color_సూచీ, &color))
        color = palette[color_సూచీ];

      *is_foreground = అబద్ధం;
    }

    తిరిగి HB_COLOR (hb_color_get_blue (color),
                     hb_color_get_green (color),
                     hb_color_get_red (color),
                     color_alpha (color, alpha));
  }

  అంతఃవరుస శూన్య recurse (స్థిరరాశి Paint &paint);
};

నిర్మాణం hb_colrv1_సంవృతం_context_t :
       hb_dispatch_context_t<hb_colrv1_సంవృతం_context_t>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj)
  {
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0))
      తిరిగి hb_ఖాళీలు_t ();

    ఒకవేళ (paint_సందర్శించిన (&obj))
      తిరిగి hb_ఖాళీలు_t ();

    nesting_level_ఎడమ--;
    obj.closurev1 (ఇది);
    nesting_level_ఎడమ++;
    తిరిగి hb_ఖాళీలు_t ();
  }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  తార్కిక paint_సందర్శించిన (స్థిరరాశి శూన్య *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) పునాది);
    ఒకవేళ (సందర్శించిన_paint.లోపల_error() || సందర్శించిన_paint.has (delta))
      తిరిగి నిజం;

    సందర్శించిన_paint.కూడు (delta);
    తిరిగి అబద్ధం;
  }

  స్థిరరాశి COLR* get_colr_పట్టిక () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి COLR *> (పునాది); }

  శూన్య కూడు_glyph (గుర్తులేని glyph_id)
  { glyphs->కూడు (glyph_id); }

  శూన్య కూడు_layer_indices (గుర్తులేని మొదటి_layer_సూచీ, గుర్తులేని అంకె_యొక్క_layers)
  {
    ఒకవేళ (అంకె_యొక్క_layers == 0) తిరిగి;
    layer_indices->కూడు_range (మొదటి_layer_సూచీ, మొదటి_layer_సూచీ + అంకె_యొక్క_layers - 1);
  }

  శూన్య కూడు_palette_సూచీ (గుర్తులేని palette_సూచీ)
  { palette_indices->కూడు (palette_సూచీ); }

  శూన్య కూడు_var_idxes (గుర్తులేని మొదటి_var_idx, గుర్తులేని అంకె_idxes)
  {
    ఒకవేళ (!అంకె_idxes || మొదటి_var_idx == VarIdx::NO_VARIATION) తిరిగి;
    variation_indices->కూడు_range (మొదటి_var_idx, మొదటి_var_idx + అంకె_idxes - 1);
  }

  బహిరంగ:
  స్థిరరాశి శూన్య *పునాది;
  hb_సమితి_t సందర్శించిన_paint;
  hb_సమితి_t *glyphs;
  hb_సమితి_t *layer_indices;
  hb_సమితి_t *palette_indices;
  hb_సమితి_t *variation_indices;
  గుర్తులేని అంకె_var_idxes;
  గుర్తులేని nesting_level_ఎడమ;

  hb_colrv1_సంవృతం_context_t (స్థిరరాశి శూన్య *పునాది_2,
                               hb_సమితి_t *glyphs_,
                               hb_సమితి_t *layer_indices_,
                               hb_సమితి_t *palette_indices_,
                               hb_సమితి_t *variation_indices_,
                               గుర్తులేని అంకె_var_idxes_2 = 1,
                               గుర్తులేని nesting_level_ఎడమ_2 = HB_MAX_NESTING_LEVEL) :
                          పునాది (పునాది_2),
                          glyphs (glyphs_),
                          layer_indices (layer_indices_),
                          palette_indices (palette_indices_),
                          variation_indices (variation_indices_),
                          అంకె_var_idxes (అంకె_var_idxes_2),
                          nesting_level_ఎడమ (nesting_level_ఎడమ_2)
  {}
};


నిర్మాణం hb_colrv1_depend_context_t :
       hb_dispatch_context_t<hb_colrv1_depend_context_t>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj)
  {
    ఒకవేళ (unlikely (nesting_level_ఎడమ == 0))
      తిరిగి hb_ఖాళీలు_t ();

    ఒకవేళ (paint_సందర్శించిన (&obj))
      తిరిగి hb_ఖాళీలు_t ();

    nesting_level_ఎడమ--;
    obj.dependv1 (ఇది);
    nesting_level_ఎడమ++;
    తిరిగి hb_ఖాళీలు_t ();
  }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  తార్కిక paint_సందర్శించిన (స్థిరరాశి శూన్య *paint)
  {
    hb_codepoint_t delta = (hb_codepoint_t) ((uintptr_t) paint - (uintptr_t) colr);
    ఒకవేళ (సందర్శించిన_paint.లోపల_error() || సందర్శించిన_paint.has (delta))
      తిరిగి నిజం;

    సందర్శించిన_paint.కూడు (delta);
    ఒకవేళ (unlikely (సందర్శించిన_paint.లోపల_error ()))
      depend_దత్తాంశం->fail ();
    తిరిగి అబద్ధం;
  }

  స్థిరరాశి COLR* get_colr_పట్టిక () స్థిరరాశి
  { తిరిగి colr; }

  బహిరంగ:
  స్థిరరాశి COLR *colr;
  hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం;
  hb_సమితి_t సందర్శించిన_paint;
  గుర్తులేని nesting_level_ఎడమ;
  hb_codepoint_t ఆధారం_gid;

  hb_colrv1_depend_context_t (స్థిరరాశి COLR *colr_,
                              hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం_2,
                              గుర్తులేని nesting_level_ఎడమ_2 = HB_MAX_NESTING_LEVEL) :
                          colr (colr_),
                          depend_దత్తాంశం (depend_దత్తాంశం_2),
                          nesting_level_ఎడమ (nesting_level_ఎడమ_2),
                          ఆధారం_gid (0)
  {}
};

నిర్మాణం Layer_రికార్డు
{
  క్రియాకారకం hb_ot_color_layer_t () స్థిరరాశి { తిరిగి {glyphId, colorIdx}; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBGlyphID16	glyphId;	/* Glyph ID of layer glyph */
  సూచీ		colorIdx;	/* Index value to use with a
				 * selected color palette.
				 * An index value of 0xFFFF
				 * is a special case indicating
				 * that the text foreground
				 * color (defined by a
				 * higher-level client) should
				 * be used and shall not be
				 * treated as actual index
				 * into CPAL ColorRecord array. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం పునాది_Glyph_రికార్డు
{
  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBGlyphID16	glyphId;	/* Glyph ID of reference glyph */
  HBUINT16	మొదటి_Layer_Idx;	/* Index (from beginning of
				 * the Layer Records) to the
				 * layer record. There will be
				 * numLayers consecutive entries
				 * for this base glyph. */
  HBUINT16	అంకె_Layers;	/* Number of color layers
				 * associated with this glyph */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

మూస <రకం_పేరు T>
నిర్మాణం Variable
{
  స్థిర constexpr తార్కిక is_variable = నిజం;

  స్థిరరాశి T& get_విలువ () స్థిరరాశి { తిరిగి value; }

  Variable<T>* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (ఇది));
  }

  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
  { value.dependv1 (c); }

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    c->అంకె_var_idxes = 0;
    // update c->num_var_idxes during value closure
    value.closurev1 (c);
    c->కూడు_var_idxes (var_Idx_పునాది, c->అంకె_var_idxes);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!value.subset (c, instancer, var_Idx_పునాది)) return_trace (అబద్ధం);
    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (నిజం);

    VarIdx new_varidx;
    new_varidx = var_Idx_పునాది;
    ఒకవేళ (var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
      ఒకవేళ (!c->plan->colrv1_variation_idx_delta_map.has (var_Idx_పునాది, &new_varidx_delta))
        return_trace (అబద్ధం);

      new_varidx = hb_మొదటి (*new_varidx_delta);
    }

    return_trace (c->serializer->embed (new_varidx));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c));
  }

  శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    value.paint_glyph (c, var_Idx_పునాది);
  }

  శూన్య get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    value.get_color_stop (c, stop, var_Idx_పునాది, instancer);
  }

  శూన్య get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) స్థిరరాశి
  {
    value.get_solid_color (c, var_Idx_పునాది, is_foreground, color);
  }

  hb_paint_extend_t get_extend () స్థిరరాశి
  {
    తిరిగి value.get_extend ();
  }

  రక్షిత:
  T      value;
  బహిరంగ:
  VarIdx var_Idx_పునాది;
  బహిరంగ:
  DEFINE_కొలత_MIN (VarIdx::static_కొలత + T::min_కొలత);
};

మూస <రకం_పేరు T>
నిర్మాణం NoVariable
{
  స్థిర constexpr తార్కిక is_variable = అబద్ధం;

  స్థిరరాశి T& get_విలువ () స్థిరరాశి { తిరిగి value; }

  స్థిర constexpr uint32_t var_Idx_పునాది = VarIdx::NO_VARIATION;

  NoVariable<T>* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (ఇది));
  }

  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
  { value.dependv1 (c); }

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  { value.closurev1 (c); }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace (value.subset (c, instancer, var_Idx_పునాది));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && value.sanitize (c));
  }

  శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    value.paint_glyph (c, var_Idx_పునాది);
  }

  శూన్య get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *stop,
		       స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    value.get_color_stop (c, stop, VarIdx::NO_VARIATION, instancer);
  }

  శూన్య get_solid_color (hb_paint_context_t *c,
                        hb_bool_t *is_foreground, hb_color_t *color) స్థిరరాశి
  {
    value.get_solid_color (c, VarIdx::NO_VARIATION, is_foreground, color);
  }

  hb_paint_extend_t get_extend () స్థిరరాశి
  {
    తిరిగి value.get_extend ();
  }

  T      value;
  బహిరంగ:
  DEFINE_కొలత_MIN (T::min_కొలత);
};

// Color structures

మూస <రకం_పేరు Type, గుర్తులేని fraction_బిట్లు>
స్థిర శూన్య
సమితి_fixed_clamped (HBFixed<Type, fraction_బిట్లు> &బహిర్గతం, దశాంశం value)
{
  // Variation deltas are unbounded; clamp before the implicit integer cast.
  బహిర్గతం.సమితి_int (hb_clamp_గమ్యం<రకం_పేరు Type::type> (roundf ((value / HBFixed<Type, fraction_బిట్లు>::mult))));
}

నిర్మాణం ColorStop
{
  శూన్య dependv1 (hb_colrv1_depend_context_t * c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    c->కూడు_palette_సూచీ (palette_సూచీ_2);
    c->అంకె_var_idxes = 2;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->stop_స్థానభ్రంశం, stop_స్థానభ్రంశం.గమ్యం_float(instancer (var_Idx_పునాది, 0)));
      దశాంశం a = alpha.గమ్యం_float (instancer (var_Idx_పునాది, 1));
      /* Partial instances must keep the unclamped base: retained axes can
       * bring the alpha back into range. Clamp before narrowing a full
       * instance so overflow cannot wrap an opaque alpha to transparent. */
      సమితి_fixed_clamped (బహిర్గతం->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->palette_సూచీ_2, c->plan->colr_palettes.get (palette_సూచీ_2),
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_color_stop (hb_paint_context_t *c,
                       hb_color_stop_t *బహిర్గతం,
		       uint32_t varIdx,
		       స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    బహిర్గతం->స్థానభ్రంశం = stop_స్థానభ్రంశం.గమ్యం_float(instancer (varIdx, 0));
    బహిర్గతం->color = c->get_color (palette_సూచీ_2,
                               alpha.గమ్యం_float (instancer (varIdx, 1)),
                               &బహిర్గతం->is_foreground);
  }

  F2DOT14	stop_స్థానభ్రంశం;
  HBUINT16	palette_సూచీ_2;
  F2DOT14	alpha;
  బహిరంగ:
  DEFINE_కొలత_STATIC (2 + 2 * F2DOT14::static_కొలత);
};

నిర్మాణం Extend : HBUINT8
{
  గణన {
    EXTEND_PAD     = 0,
    EXTEND_REPEAT  = 1,
    EXTEND_REFLECT = 2,
  };
  బహిరంగ:
  DEFINE_కొలత_STATIC (1);
};

మూస <మూస<రకం_పేరు> తరగతి Var>
నిర్మాణం Color_పంక్తి
{
  శూన్య dependv1 (hb_colrv1_depend_context_t * c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &stop : stops.iter ())
      stop.closurev1 (c);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->extend, extend, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->stops.len, stops.len, HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం)) return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక& stop : stops.iter ())
    {
      ఒకవేళ (!stop.subset (c, instancer)) return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  stops.sanitize (c));
  }

  /* get up to count stops from start */
  గుర్తులేని పూర్ణాంకం
  get_color_stops (hb_paint_context_t *c,
                   గుర్తులేని పూర్ణాంకం ప్రారంభం,
		   గుర్తులేని పూర్ణాంకం *లెక్క,
		   hb_color_stop_t *color_stops,
		   స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం len = stops.len;

    ఒకవేళ (లెక్క && color_stops)
    {
      గుర్తులేని పూర్ణాంకం i;
      కొరకు (i = 0; i < *లెక్క && ప్రారంభం + i < len; i++)
        stops[ప్రారంభం + i].get_color_stop (c, &color_stops[i], instancer);
      *లెక్క = i;
    }

    తిరిగి len;
  }

  HB_అంతర్గత స్థిర గుర్తులేని పూర్ణాంకం static_get_color_stops (hb_color_పంక్తి_t *color_పంక్తి,
							  శూన్య *color_పంక్తి_దత్తాంశం,
							  గుర్తులేని పూర్ణాంకం ప్రారంభం,
							  గుర్తులేని పూర్ణాంకం *లెక్క,
							  hb_color_stop_t *color_stops,
							  శూన్య *user_దత్తాంశం)
  {
    స్థిరరాశి Color_పంక్తి *thiz = (స్థిరరాశి Color_పంక్తి *) color_పంక్తి_దత్తాంశం;
    hb_paint_context_t *c = (hb_paint_context_t *) user_దత్తాంశం;
    తిరిగి thiz->get_color_stops (c, ప్రారంభం, లెక్క, color_stops, c->instancer);
  }

  hb_paint_extend_t get_extend () స్థిరరాశి
  {
    ఎంపిక ((గుర్తులేని పూర్ణాంకం) extend)
    {
      స్థితి Extend::EXTEND_REPEAT:  తిరిగి HB_PAINT_EXTEND_REPEAT;
      స్థితి Extend::EXTEND_REFLECT: తిరిగి HB_PAINT_EXTEND_REFLECT;
      స్థితి Extend::EXTEND_PAD:
      అప్రమేయ:                     తిరిగి HB_PAINT_EXTEND_PAD;
    }
  }

  HB_అంతర్గత స్థిర hb_paint_extend_t static_get_extend (hb_color_పంక్తి_t *color_పంక్తి,
							  శూన్య *color_పంక్తి_దత్తాంశం,
							  శూన్య *user_దత్తాంశం)
  {
    స్థిరరాశి Color_పంక్తి *thiz = (స్థిరరాశి Color_పంక్తి *) color_పంక్తి_దత్తాంశం;
    తిరిగి thiz->get_extend ();
  }

  Extend	extend;
  Array16_యొక్క<Var<ColorStop>>	stops;
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (3, stops);
};

// Composition modes

// Compositing modes are taken from https://www.w3.org/TR/compositing-1/
// NOTE: a brief audit of major implementations suggests most support most
// or all of the specified modes.
నిర్మాణం సంయుక్త_బహుళకం : HBUINT8
{
  బహిరంగ:
  DEFINE_కొలత_STATIC (1);
};

నిర్మాణం Affine2x3
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  { c->అంకె_var_idxes = 6; }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      స్వయంచాలక fold = [&] (F16DOT16 &target, F16DOT16 ఆధారం, గుర్తులేని స్థానభ్రంశం) {
        // Keep the stored integer precise while applying the float delta.
        target.సమితి_int (hb_clamp_గమ్యం<int32_t> (roundf ((ద్విగుణ) ఆధారం.గమ్యం_int () +
                                                    (ద్విగుణ) instancer (var_Idx_పునాది, స్థానభ్రంశం))));
      };
      fold (బహిర్గతం->xx, xx, 0);
      fold (బహిర్గతం->yx, yx, 1);
      fold (బహిర్గతం->xy, xy, 2);
      fold (బహిర్గతం->yy, yy, 3);
      fold (బహిర్గతం->dx, dx, 4);
      fold (బహిర్గతం->dy, dy, 5);
    }
    return_trace (నిజం);
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    c->funcs->నెట్టు_transform (c->దత్తాంశం,
			      xx.గమ్యం_float (c->instancer (var_Idx_పునాది, 0)),
			      yx.గమ్యం_float (c->instancer (var_Idx_పునాది, 1)),
                              xy.గమ్యం_float (c->instancer (var_Idx_పునాది, 2)),
			      yy.గమ్యం_float (c->instancer (var_Idx_పునాది, 3)),
                              dx.గమ్యం_float (c->instancer (var_Idx_పునాది, 4)),
			      dy.గమ్యం_float (c->instancer (var_Idx_పునాది, 5)));
  }

  F16DOT16 xx;
  F16DOT16 yx;
  F16DOT16 xy;
  F16DOT16 yy;
  F16DOT16 dx;
  F16DOT16 dy;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6 * F16DOT16::static_కొలత);
};

నిర్మాణం PaintColrLayers
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer HB_వాడని) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    uint32_t మొదటి_layer_సూచీ = అంకె_Layers ? c->plan->colrv1_layers.get (మొదటి_Layer_సూచీ) : 0;
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->మొదటి_Layer_సూచీ, మొదటి_layer_సూచీ,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  అంతఃవరుస శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి;

  HBUINT8	format; /* format = 1 */
  HBUINT8	అంకె_Layers;
  HBUINT32	మొదటి_Layer_సూచీ;  /* index into COLRv1::layerList */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం PaintSolid
{
  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి {}
  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    c->కూడు_palette_సూచీ (palette_సూచీ_2);
    c->అంకె_var_idxes = 1;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      దశాంశం a = alpha.గమ్యం_float (instancer (var_Idx_పునాది, 0));
      సమితి_fixed_clamped (బహిర్గతం->alpha, c->plan->all_axes_pinned ? hb_clamp (a, 0.f, 1.f) : a);
    }

    ఒకవేళ (format == 3 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 2;

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->palette_సూచీ_2, c->plan->colr_palettes.get (palette_సూచీ_2),
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_solid_color (hb_paint_context_t *c, uint32_t var_Idx_పునాది,
                        hb_bool_t *is_foreground, hb_color_t *color) స్థిరరాశి
  {
    *color = c->get_color (palette_సూచీ_2,
                           alpha.గమ్యం_float (c->instancer (var_Idx_పునాది, 0)),
                           is_foreground);
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    hb_bool_t is_foreground;
    hb_color_t color;

    get_solid_color (c, var_Idx_పునాది, &is_foreground, &color);
    c->funcs->color (c->దత్తాంశం, is_foreground, color);
  }

  HBUINT8	format; /* format = 2(noVar) or 3(Var)*/
  HBUINT16	palette_సూచీ_2;
  F2DOT14	alpha;
  బహిరంగ:
  DEFINE_కొలత_STATIC (3 + F2DOT14::static_కొలత);
};

మూస <మూస<రకం_పేరు> తరగతి Var>
నిర్మాణం Paint_రేఖీయ_Gradient
{
  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    (ఇది+color_పంక్తి_2).closurev1 (c);
    c->అంకె_var_idxes = 6;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      బహిర్గతం->x0 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) x0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->y0 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) y0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->x1 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) x1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
      బహిర్గతం->y1 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) y1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 3)));
      బహిర్గతం->x2 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) x2 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 4)));
      బహిర్గతం->y2 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) y2 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 5)));
    }

    ఒకవేళ (format == 5 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 4;

    return_trace (బహిర్గతం->color_పంక్తి_2.serialize_subset (c, color_పంక్తి_2, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && color_పంక్తి_2.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    hb_color_పంక్తి_t cl = {
      (శూన్య *) &(ఇది+color_పంక్తి_2),
      (ఇది+color_పంక్తి_2).static_get_color_stops, c,
      (ఇది+color_పంక్తి_2).static_get_extend, శూన్యసూచిక
    };

    c->funcs->రేఖీయ_gradient (c->దత్తాంశం, &cl,
			       x0 + c->instancer (var_Idx_పునాది, 0),
			       y0 + c->instancer (var_Idx_పునాది, 1),
			       x1 + c->instancer (var_Idx_పునాది, 2),
			       y1 + c->instancer (var_Idx_పునాది, 3),
			       x2 + c->instancer (var_Idx_పునాది, 4),
			       y2 + c->instancer (var_Idx_పునాది, 5));
  }

  HBUINT8			format; /* format = 4(noVar) or 5 (Var) */
  Offset24_గమ్యం<Color_పంక్తి<Var>>	color_పంక్తి_2; /* Offset (from beginning of PaintLinearGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  FWORD			x1;
  FWORD			y1;
  FWORD			x2;
  FWORD			y2;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 6 * FWORD::static_కొలత);
};

మూస <మూస<రకం_పేరు> తరగతి Var>
నిర్మాణం PaintRadialGradient
{
  తార్కిక constant_after_pinning (hb_subset_context_t *c,
                               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    ఒకవేళ (c->plan->has_avar2) తిరిగి అబద్ధం;
    స్థిరరాశి స్వయంచాలక &regions = instancer.varStore->get_region_list ();
    కొరకు (గుర్తులేని i = 0; i < regions.region_లెక్క; i++)
    {
      hb_hashmap_t<hb_ట్యాగ్_t, Triple> axes;
      ఒకవేళ (!regions.get_var_region (i, c->plan->axes_old_సూచీ_ట్యాగ్_map, axes))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_బహిర్గతం_యొక్క_స్థలం);
        తిరిగి అబద్ధం;
      }
      కొరకు (స్వయంచాలక axis : axes)
      {
        Triple *హద్దు;
        ఒకవేళ (!c->plan->user_axes_location.has (axis.మొదటి, &హద్దు) ||
            హద్దు->minimum != హద్దు->గరిష్ఠ)
          తిరిగి అబద్ధం;
      }
    }
    తిరిగి నిజం;
  }

  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    (ఇది+color_పంక్తి_2).closurev1 (c);
    c->అంకె_var_idxes = 6;
  }

  తార్కిక subset_nonnegative (hb_subset_context_t *c,
                           స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
                           uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    ద్విగుణ ఆధారం[6] = {
      x0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 0)),
      y0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)),
      radius0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)),
      x1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 3)),
      y1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 4)),
      radius1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 5))
    };
    స్వయంచాలక transparent = [&] {
      స్వయంచాలక *బహిర్గతం = c->serializer->allocate_కొలత<PaintSolid> (5);
      ఒకవేళ (!బహిర్గతం) తిరిగి అబద్ధం;
      బహిర్గతం->format = 2; బహిర్గతం->palette_సూచీ_2 = 0xFFFF; బహిర్గతం->alpha.సమితి_float (0);
      తిరిగి నిజం;
    };
    స్థిరరాశి స్వయంచాలక &పంక్తి = ఇది+color_పంక్తి_2;
    ఒకవేళ (ఆధారం[2] == ఆధారం[5] || !పంక్తి.stops.len) తిరిగి transparent ();
    నిర్మాణం stop_t { ద్విగుణ స్థానభ్రంశం, alpha; గుర్తులేని palette, క్రమం; };
    hb_సదిశ_t<stop_t> stops;
    ఒకవేళ (!stops.alloc (పంక్తి.stops.len)) తిరిగి అబద్ధం;
    గుర్తులేని క్రమం = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : పంక్తి.stops.iter ())
    {
      స్థిరరాశి స్వయంచాలక &stop = నమోదు_2.get_విలువ ();
      stops.push (stop_t {(ద్విగుణ) roundf (stop.stop_స్థానభ్రంశం.గమ్యం_float (instancer (నమోదు_2.var_Idx_పునాది, 0)) * 16384.f) / 16384.,
                   (ద్విగుణ) hb_clamp (stop.alpha.గమ్యం_float (instancer (నమోదు_2.var_Idx_పునాది, 1)), 0.f, 1.f),
                   stop.palette_సూచీ_2, క్రమం++});
    }
    ఒకవేళ (stops.లోపల_error ()) తిరిగి అబద్ధం;
    stops.qsort ([] (స్థిరరాశి stop_t &a, స్థిరరాశి stop_t &b) {
      తిరిగి a.స్థానభ్రంశం < b.స్థానభ్రంశం ? -1 : a.స్థానభ్రంశం > b.స్థానభ్రంశం ? 1 :
             a.క్రమం < b.క్రమం ? -1 : a.క్రమం > b.క్రమం ? 1 : 0;
    });
    ద్విగుణ dr = ఆధారం[5] - ఆధారం[2];
    // Fractional shifts must preserve both integer circles and F2Dot14 stops.
    గుర్తులేని lattice = 16384;
    కొరకు (గుర్తులేని j = 0; j < 3; j++)
    {
      ద్విగుణ remainder = fmod (fabs (ఆధారం[j + 3] - ఆధారం[j]), 16384.);
      ఒకవేళ (!(remainder >= 0 && remainder < 16384))
      {
        c->serializer->err (HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
        తిరిగి అబద్ధం;
      }
      గుర్తులేని కుడి = (గుర్తులేని) remainder;
      అయితే (కుడి) { గుర్తులేని r = lattice % కుడి; lattice = కుడి; కుడి = r; }
    }
    కొరకు (గుర్తులేని జరుపు = 0; జరుపు < 16; జరుపు++)
    {
      ద్విగుణ b = 1u << జరుపు;
      ద్విగుణ coarse = dr > 0 ? ceil (-ఆధారం[2] / dr) : floor (ఆధారం[2] / -dr) - b;
      ద్విగుణ fine = dr > 0 ? ceil (-ఆధారం[2] / dr * lattice) / lattice :
                            floor (ఆధారం[2] / -dr * lattice) / lattice - b;
      ద్విగుణ candidates[] = {coarse, fine};
      కొరకు (ద్విగుణ a : candidates)
      {
        // omega = a + b*q. Positive b preserves the largest valid root.
        ద్విగుణ geometry[6];
        తార్కిక సరిపోతుంది = నిజం;
        కొరకు (గుర్తులేని j = 0; j < 3; j++)
        {
          ద్విగుణ difference = ఆధారం[j + 3] - ఆధారం[j];
          geometry[j] = ఆధారం[j] + a * difference;
          geometry[j + 3] = ఆధారం[j] + (a + b) * difference;
          ద్విగుణ దిగువ = j == 2 ? 0 : -32768, ఎగువ = j == 2 ? 65535 : 32767;
          సరిపోతుంది &= geometry[j] >= దిగువ && geometry[j] <= ఎగువ &&
                  geometry[j + 3] >= దిగువ && geometry[j + 3] <= ఎగువ;
        }
        ఒకవేళ (!సరిపోతుంది) కొనసాగు;
        గుర్తులేని మొదటి = 0, లెక్క = stops.నిడివి;
        ద్విగుణ min = (stops[0].స్థానభ్రంశం - a) / b, max = (stops.తోక ().స్థానభ్రంశం - a) / b;
        ద్విగుణ bias = 0;
        తార్కిక constant = అబద్ధం;
        ఒకవేళ (పంక్తి.extend == Extend::EXTEND_PAD)
        {
          ద్విగుణ tip = -geometry[2] / (b * dr);
          ఒకవేళ ((dr > 0 && max <= tip) || (dr < 0 && min > tip))
          {
            మొదటి = dr > 0 ? stops.నిడివి - 1 : 0;
            లెక్క = 1; constant = నిజం;
          }
        }
        లేకపోతే
        {
          ద్విగుణ period = max - min;
          ఒకవేళ (పంక్తి.extend == Extend::EXTEND_REFLECT) period *= 2;
          ఒకవేళ (period <= 0) తిరిగి transparent ();
          ద్విగుణ n = ceil ((-2 - min) / period);
          ఒకవేళ (n > floor ((32767. / 16384. - max) / period)) కొనసాగు;
          bias = n * period;
        }
        కొరకు (గుర్తులేని j = 0; j < లెక్క; j++)
        {
          ద్విగుణ బిట్లు = constant ? 0 : ((stops[మొదటి + j].స్థానభ్రంశం - a) / b + bias) * 16384;
          సరిపోతుంది &= బిట్లు >= -32768 && బిట్లు <= 32767 && బిట్లు == round (బిట్లు);
        }
        ఒకవేళ (!సరిపోతుంది) కొనసాగు;
        స్వయంచాలక *బహిర్గతం = c->serializer->allocate_కొలత<PaintRadialGradient<NoVariable>> (16);
        ఒకవేళ (!బహిర్గతం) తిరిగి అబద్ధం;
        బహిర్గతం->format = 6;
        బహిర్గతం->x0 = (పూర్ణాంకం) geometry[0]; బహిర్గతం->y0 = (పూర్ణాంకం) geometry[1]; బహిర్గతం->radius0 = (గుర్తులేని) geometry[2];
        బహిర్గతం->x1 = (పూర్ణాంకం) geometry[3]; బహిర్గతం->y1 = (పూర్ణాంకం) geometry[4]; బహిర్గతం->radius1 = (గుర్తులేని) geometry[5];
        c->serializer->push ();
        స్వయంచాలక *బహిర్గతం_పంక్తి = c->serializer->allocate_కొలత<Color_పంక్తి<NoVariable>> (3 + లెక్క * ColorStop::static_కొలత);
        ఒకవేళ (!బహిర్గతం_పంక్తి) { c->serializer->తీయి_discard (); తిరిగి అబద్ధం; }
        బహిర్గతం_పంక్తి->extend = పంక్తి.extend;
        బహిర్గతం_పంక్తి->stops.len = లెక్క;
        కొరకు (గుర్తులేని j = 0; j < లెక్క; j++)
        {
          స్థిరరాశి స్వయంచాలక &stop = stops[మొదటి + j];
          స్వయంచాలక &బహిర్గతం_stop = బహిర్గతం_పంక్తి->stops[j].value;
          బహిర్గతం_stop.stop_స్థానభ్రంశం.సమితి_float ((దశాంశం) (constant ? 0 : (stop.స్థానభ్రంశం - a) / b + bias));
          బహిర్గతం_stop.alpha.సమితి_float ((దశాంశం) stop.alpha);
          ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం_stop.palette_సూచీ_2, c->plan->colr_palettes.get (stop.palette),
                                            HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
          { c->serializer->తీయి_discard (); తిరిగి అబద్ధం; }
        }
        c->serializer->కూడు_link (బహిర్గతం->color_పంక్తి_2, c->serializer->తీయి_pack ());
        తిరిగి !c->serializer->లోపల_error ();
      }
    }
    // Reject an unrepresentable static result instead of changing its colors.
    c->serializer->err (HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
    తిరిగి అబద్ధం;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (instancer && !c->plan->pinned_at_default &&
        (c->plan->all_axes_pinned || constant_after_pinning (c, instancer)) &&
        var_Idx_పునాది != VarIdx::NO_VARIATION &&
        (radius0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)) < 0 ||
         radius1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 5)) < 0))
      return_trace (subset_nonnegative (c, instancer, var_Idx_పునాది));
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      బహిర్గతం->x0 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) x0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->y0 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) y0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->radius0 = hb_clamp_గమ్యం<uint16_t> ((ద్విగుణ) radius0 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
      బహిర్గతం->x1 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) x1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 3)));
      బహిర్గతం->y1 = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) y1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 4)));
      బహిర్గతం->radius1 = hb_clamp_గమ్యం<uint16_t> ((ద్విగుణ) radius1 + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 5)));
    }

    ఒకవేళ (format == 7 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 6;

    return_trace (బహిర్గతం->color_పంక్తి_2.serialize_subset (c, color_పంక్తి_2, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && color_పంక్తి_2.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    hb_color_పంక్తి_t cl = {
      (శూన్య *) &(ఇది+color_పంక్తి_2),
      (ఇది+color_పంక్తి_2).static_get_color_stops, c,
      (ఇది+color_పంక్తి_2).static_get_extend, శూన్యసూచిక
    };

    c->funcs->radial_gradient (c->దత్తాంశం, &cl,
			       x0 + c->instancer (var_Idx_పునాది, 0),
			       y0 + c->instancer (var_Idx_పునాది, 1),
			       radius0 + c->instancer (var_Idx_పునాది, 2),
			       x1 + c->instancer (var_Idx_పునాది, 3),
			       y1 + c->instancer (var_Idx_పునాది, 4),
			       radius1 + c->instancer (var_Idx_పునాది, 5));
  }

  HBUINT8			format; /* format = 6(noVar) or 7 (Var) */
  Offset24_గమ్యం<Color_పంక్తి<Var>>	color_పంక్తి_2; /* Offset (from beginning of PaintRadialGradient
                                            * table) to ColorLine subtable. */
  FWORD			x0;
  FWORD			y0;
  UFWORD		radius0;
  FWORD			x1;
  FWORD			y1;
  UFWORD		radius1;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 6 * FWORD::static_కొలత);
};

మూస <మూస<రకం_పేరు> తరగతి Var>
నిర్మాణం PaintSweepGradient
{
  శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి {}

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    (ఇది+color_పంక్తి_2).closurev1 (c);
    c->అంకె_var_idxes = 4;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      బహిర్గతం->centerX = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerX + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->centerY = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerY + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
      సమితి_fixed_clamped (బహిర్గతం->ప్రారంభం_Angle, ప్రారంభం_Angle.గమ్యం_float (instancer (var_Idx_పునాది, 2)));
      సమితి_fixed_clamped (బహిర్గతం->చివర_Angle, చివర_Angle.గమ్యం_float (instancer (var_Idx_పునాది, 3)));
    }

    ఒకవేళ (format == 9 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 8;

    return_trace (బహిర్గతం->color_పంక్తి_2.serialize_subset (c, color_పంక్తి_2, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && color_పంక్తి_2.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    hb_color_పంక్తి_t cl = {
      (శూన్య *) &(ఇది+color_పంక్తి_2),
      (ఇది+color_పంక్తి_2).static_get_color_stops, c,
      (ఇది+color_పంక్తి_2).static_get_extend, శూన్యసూచిక
    };

    c->funcs->sweep_gradient (c->దత్తాంశం, &cl,
			      centerX + c->instancer (var_Idx_పునాది, 0),
			      centerY + c->instancer (var_Idx_పునాది, 1),
                              (ప్రారంభం_Angle.గమ్యం_float (c->instancer (var_Idx_పునాది, 2)) + 1) * HB_PI,
                              (చివర_Angle.గమ్యం_float   (c->instancer (var_Idx_పునాది, 3)) + 1) * HB_PI);
  }

  HBUINT8			format; /* format = 8(noVar) or 9 (Var) */
  Offset24_గమ్యం<Color_పంక్తి<Var>>	color_పంక్తి_2; /* Offset (from beginning of PaintSweepGradient
                                            * table) to ColorLine subtable. */
  FWORD			centerX;
  FWORD			centerY;
  F2DOT14		ప్రారంభం_Angle;
  F2DOT14		చివర_Angle;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * FWORD::static_కొలత + 2 * F2DOT14::static_కొలత);
};

// Paint a non-COLR glyph, filled as indicated by paint.
మూస <రకం_పేరు Types>
నిర్మాణం PaintGlyph
{
  ఉపయోగించి HBGlyphID = రకం_పేరు Types::HBGlyphID;

  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (! c->serializer->check_కేటాయించు (బహిర్గతం->gid, c->plan->glyph_map->get (gid),
                                       HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->paint.serialize_subset (c, paint, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && paint.sanitize (c, ఇది));
  }

  అంతఃవరుస శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి;

  HBUINT8		format; /* format = 10 or 33 */
  Offset24_గమ్యం<Paint>	paint;  /* Offset (from beginning of PaintGlyph table) to Paint subtable. */
  HBGlyphID		gid;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + HBGlyphID::static_కొలత);
};

నిర్మాణం PaintColrGlyph
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer HB_వాడని) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->gid, c->plan->glyph_map->get (gid),
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  అంతఃవరుస శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి;

  HBUINT8	format; /* format = 11 */
  HBUINT16	gid;
  బహిరంగ:
  DEFINE_కొలత_STATIC (3);
};

మూస <మూస<రకం_పేరు> తరగతి Var>
నిర్మాణం PaintTransform
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    ఒకవేళ (!బహిర్గతం->transform.serialize_subset (c, transform, ఇది, instancer)) return_trace (అబద్ధం);
    ఒకవేళ (format == 13 && c->plan->all_axes_pinned)
      బహిర్గతం->format = 12;
    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  src.sanitize (c, ఇది) &&
                  transform.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    (ఇది+transform).paint_glyph (c); // This does a push_transform()
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8			format; /* format = 12(noVar) or 13 (Var) */
  Offset24_గమ్యం<Paint>		src; /* Offset (from beginning of PaintTransform table) to Paint subtable. */
  Offset24_గమ్యం<Var<Affine2x3>>	transform;
  బహిరంగ:
  DEFINE_కొలత_STATIC (7);
};

నిర్మాణం PaintTranslate
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      బహిర్గతం->dx = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) dx + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->dy = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) dy + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
    }

    ఒకవేళ (format == 15 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 14;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం ddx = dx + c->instancer (var_Idx_పునాది, 0);
    దశాంశం ddy = dy + c->instancer (var_Idx_పునాది, 1);

    c->funcs->నెట్టు_translate (c->దత్తాంశం, ddx, ddy);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 14(noVar) or 15 (Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintTranslate table) to Paint subtable. */
  FWORD		dx;
  FWORD		dy;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * FWORD::static_కొలత);
};

మూస <రకం_పేరు ఆధారం_2>
స్థిర తార్కిక serialize_scale_transform (hb_subset_context_t *c,
                                       దశాంశం sx, దశాంశం sy, దశాంశం dx, దశాంశం dy,
                                       స్థిరరాశి ఆధారం_2 &ఆధారం)
{
  స్వయంచాలక *బహిర్గతం = c->serializer->allocate_కొలత<PaintTransform<NoVariable>> (7);
  ఒకవేళ (unlikely (!బహిర్గతం)) తిరిగి అబద్ధం;
  బహిర్గతం->format = 12;

  c->serializer->push ();
  స్వయంచాలక *transform = c->serializer->allocate_కొలత<Affine2x3> (Affine2x3::static_కొలత);
  ఒకవేళ (unlikely (!transform))
  {
    c->serializer->తీయి_discard ();
    తిరిగి అబద్ధం;
  }
  సమితి_fixed_clamped (transform->xx, sx);
  transform->yx.సమితి_float (0);
  transform->xy.సమితి_float (0);
  సమితి_fixed_clamped (transform->yy, sy);
  సమితి_fixed_clamped (transform->dx, dx);
  సమితి_fixed_clamped (transform->dy, dy);
  c->serializer->కూడు_link (బహిర్గతం->transform, c->serializer->తీయి_pack ());

  c->serializer->push ();
  తార్కిక ret = ఆధారం ();
  ఒకవేళ (ret)
    c->serializer->కూడు_link (బహిర్గతం->src, c->serializer->తీయి_pack ());
  లేకపోతే
    c->serializer->తీయి_discard ();
  తిరిగి ret;
}

మూస <రకం_పేరు పునాది_3>
స్థిర తార్కిక subset_scale_as_transform (hb_subset_context_t *c,
                                      స్థిరరాశి పునాది_3 *పునాది,
                                      స్థిరరాశి Offset24_గమ్యం<Paint> &src,
                                      స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
                                      దశాంశం sx, దశాంశం sy,
                                      దశాంశం cx = 0, దశాంశం cy = 0)
{
  ఒకవేళ (src.is_null ()) తిరిగి అబద్ధం;
  స్వయంచాలక ఆధారం = [&] { తిరిగి c->dispatch (పునాది+src, instancer); };
  దశాంశం dx = cx * (1 - sx), dy = cy * (1 - sy);
  ఒకవేళ (dx >= -32768 && dx < 32768 && dy >= -32768 && dy < 32768)
    తిరిగి serialize_scale_transform (c, sx, sy, dx, dy, ఆధారం);

  // Preserve the center as separate translations when the combined matrix
  // exceeds Fixed. Splitting also makes the inverse of -32768 representable.
  స్వయంచాలక inner = [&] {
    తిరిగి serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, ఆధారం);
  };
  స్వయంచాలక inverse = [&] {
    తిరిగి serialize_scale_transform (c, 1, 1, -cx / 2, -cy / 2, inner);
  };
  స్వయంచాలక scale = [&] {
    తిరిగి serialize_scale_transform (c, sx, sy, 0, 0, inverse);
  };
  స్వయంచాలక outer = [&] {
    తిరిగి serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, scale);
  };
  తిరిగి serialize_scale_transform (c, 1, 1, cx / 2, cy / 2, outer);
}

నిర్మాణం PaintScale
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      దశాంశం sx = roundf (scaleX.గమ్యం_float (instancer (var_Idx_పునాది, 0)) * 16384) / 16384;
      దశాంశం sy = roundf (scaleY.గమ్యం_float (instancer (var_Idx_పునాది, 1)) * 16384) / 16384;
      ఒకవేళ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (c, ఇది, src, instancer, sx, sy));
    }
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->scaleX, scaleX.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      సమితి_fixed_clamped (బహిర్గతం->scaleY, scaleY.గమ్యం_float (instancer (var_Idx_పునాది, 1)));
    }

    ఒకవేళ (format == 17 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 16;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం sx = scaleX.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));
    దశాంశం sy = scaleY.గమ్యం_float (c->instancer (var_Idx_పునాది, 1));

    c->funcs->నెట్టు_scale (c->దత్తాంశం, sx, sy);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 16 (noVar) or 17(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintScale table) to Paint subtable. */
  F2DOT14		scaleX;
  F2DOT14		scaleY;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * F2DOT14::static_కొలత);
};

నిర్మాణం PaintScaleAroundCenter
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      దశాంశం sx = roundf (scaleX.గమ్యం_float (instancer (var_Idx_పునాది, 0)) * 16384) / 16384;
      దశాంశం sy = roundf (scaleY.గమ్యం_float (instancer (var_Idx_పునాది, 1)) * 16384) / 16384;
      ఒకవేళ (sx < -2 || sx >= 2 || sy < -2 || sy >= 2)
        return_trace (subset_scale_as_transform (
            c, ఇది, src, instancer, sx, sy,
            centerX + roundf (instancer (var_Idx_పునాది, 2)),
            centerY + roundf (instancer (var_Idx_పునాది, 3))));
    }
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->scaleX, scaleX.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      సమితి_fixed_clamped (బహిర్గతం->scaleY, scaleY.గమ్యం_float (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->centerX = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerX + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
      బహిర్గతం->centerY = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerY + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 3)));
    }

    ఒకవేళ (format == 19 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 18;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం sx = scaleX.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));
    దశాంశం sy = scaleY.గమ్యం_float (c->instancer (var_Idx_పునాది, 1));
    దశాంశం tCenterX = centerX + c->instancer (var_Idx_పునాది, 2);
    దశాంశం tCenterY = centerY + c->instancer (var_Idx_పునాది, 3);

    c->funcs->నెట్టు_scale_around_center (c->దత్తాంశం, sx, sy, tCenterX, tCenterY);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 18 (noVar) or 19(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintScaleAroundCenter table) to Paint subtable. */
  F2DOT14	scaleX;
  F2DOT14	scaleY;
  FWORD		centerX;
  FWORD		centerY;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * F2DOT14::static_కొలత + 2 * FWORD::static_కొలత);
};

నిర్మాణం Paint_Scale_ఏకరీతి
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      దశాంశం s = roundf (scale.గమ్యం_float (instancer (var_Idx_పునాది, 0)) * 16384) / 16384;
      ఒకవేళ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (c, ఇది, src, instancer, s, s));
    }
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
      సమితి_fixed_clamped (బహిర్గతం->scale, scale.గమ్యం_float (instancer (var_Idx_పునాది, 0)));

    ఒకవేళ (format == 21 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 20;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం s = scale.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));

    c->funcs->నెట్టు_scale (c->దత్తాంశం, s, s);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 20 (noVar) or 21(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintScaleUniform table) to Paint subtable. */
  F2DOT14		scale;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + F2DOT14::static_కొలత);
};

నిర్మాణం Paint_Scale_ఏకరీతి_Around_Center
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned && instancer &&
        !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      దశాంశం s = roundf (scale.గమ్యం_float (instancer (var_Idx_పునాది, 0)) * 16384) / 16384;
      ఒకవేళ (s < -2 || s >= 2)
        return_trace (subset_scale_as_transform (
            c, ఇది, src, instancer, s, s,
            centerX + roundf (instancer (var_Idx_పునాది, 1)),
            centerY + roundf (instancer (var_Idx_పునాది, 2))));
    }
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->scale, scale.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->centerX = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerX + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->centerY = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerY + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
    }

    ఒకవేళ (format == 23 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 22;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం s = scale.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));
    దశాంశం tCenterX = centerX + c->instancer (var_Idx_పునాది, 1);
    దశాంశం tCenterY = centerY + c->instancer (var_Idx_పునాది, 2);

    c->funcs->నెట్టు_scale_around_center (c->దత్తాంశం, s, s, tCenterX, tCenterY);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 22 (noVar) or 23(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintScaleUniformAroundCenter table) to Paint subtable. */
  F2DOT14	scale;
  FWORD		centerX;
  FWORD		centerY;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + F2DOT14::static_కొలత + 2 * FWORD::static_కొలత);
};

నిర్మాణం PaintRotate
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
      సమితి_fixed_clamped (బహిర్గతం->angle, angle.గమ్యం_float (instancer (var_Idx_పునాది, 0)));

    ఒకవేళ (format == 25 && c->plan->all_axes_pinned)
      బహిర్గతం->format = 24;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం a = angle.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));

    c->funcs->నెట్టు_rotate (c->దత్తాంశం, a);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 24 (noVar) or 25(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintRotate table) to Paint subtable. */
  F2DOT14		angle;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + F2DOT14::static_కొలత);
};

నిర్మాణం PaintRotateAroundCenter
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->angle, angle.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->centerX = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerX + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->centerY = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerY + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
    }

    ఒకవేళ (format ==27 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 26;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం a = angle.గమ్యం_float (c->instancer (var_Idx_పునాది, 0));
    దశాంశం tCenterX = centerX + c->instancer (var_Idx_పునాది, 1);
    దశాంశం tCenterY = centerY + c->instancer (var_Idx_పునాది, 2);

    c->funcs->నెట్టు_rotate_around_center (c->దత్తాంశం, a, tCenterX, tCenterY);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 26 (noVar) or 27(Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintRotateAroundCenter table) to Paint subtable. */
  F2DOT14	angle;
  FWORD		centerX;
  FWORD		centerY;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + F2DOT14::static_కొలత + 2 * FWORD::static_కొలత);
};

నిర్మాణం PaintSkew
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->xSkewAngle, xSkewAngle.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      సమితి_fixed_clamped (బహిర్గతం->ySkewAngle, ySkewAngle.గమ్యం_float (instancer (var_Idx_పునాది, 1)));
    }

    ఒకవేళ (format == 29 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 28;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం sx = xSkewAngle.గమ్యం_float(c->instancer (var_Idx_పునాది, 0));
    దశాంశం sy = ySkewAngle.గమ్యం_float(c->instancer (var_Idx_పునాది, 1));

    c->funcs->నెట్టు_skew (c->దత్తాంశం, sx, sy);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 28(noVar) or 29 (Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintSkew table) to Paint subtable. */
  F2DOT14		xSkewAngle;
  F2DOT14		ySkewAngle;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * F2DOT14::static_కొలత);
};

నిర్మాణం PaintSkewAroundCenter
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      సమితి_fixed_clamped (బహిర్గతం->xSkewAngle, xSkewAngle.గమ్యం_float (instancer (var_Idx_పునాది, 0)));
      సమితి_fixed_clamped (బహిర్గతం->ySkewAngle, ySkewAngle.గమ్యం_float (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->centerX = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerX + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 2)));
      బహిర్గతం->centerY = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) centerY + (ద్విగుణ) roundf (instancer (var_Idx_పునాది, 3)));
    }

    ఒకవేళ (format == 31 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 30;

    return_trace (బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && src.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c, uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    దశాంశం sx = xSkewAngle.గమ్యం_float(c->instancer (var_Idx_పునాది, 0));
    దశాంశం sy = ySkewAngle.గమ్యం_float(c->instancer (var_Idx_పునాది, 1));
    దశాంశం tCenterX = centerX + c->instancer (var_Idx_పునాది, 2);
    దశాంశం tCenterY = centerY + c->instancer (var_Idx_పునాది, 3);

    c->funcs->నెట్టు_skew_around_center (c->దత్తాంశం, sx, sy, tCenterX, tCenterY);
    c->recurse (ఇది+src);
    c->funcs->తీయి_transform (c->దత్తాంశం);
  }

  HBUINT8		format; /* format = 30(noVar) or 31 (Var) */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintSkewAroundCenter table) to Paint subtable. */
  F2DOT14	xSkewAngle;
  F2DOT14	ySkewAngle;
  FWORD		centerX;
  FWORD		centerY;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 2 * F2DOT14::static_కొలత + 2 * FWORD::static_కొలత);
};

నిర్మాణం Paint_సంయుక్త
{
  HB_అంతర్గత శూన్య dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి;
  HB_అంతర్గత శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    తార్కిక ret = అబద్ధం;
    ret |= బహిర్గతం->src.serialize_subset (c, src, ఇది, instancer);
    ret |= బహిర్గతం->backdrop.serialize_subset (c, backdrop, ఇది, instancer);
    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  c->check_ops (ఇది->min_కొలత) && // PainComposite can get exponential
                  src.sanitize (c, ఇది) &&
                  backdrop.sanitize (c, ఇది));
  }

  శూన్య paint_glyph (hb_paint_context_t *c) స్థిరరాశి
  {
    TRACE_PAINT (ఇది);
    hb_paint_సంయుక్త_బహుళకం_t సంయుక్త_బహుళకం_2;
    ఎంపిక ((గుర్తులేని పూర్ణాంకం) బహుళకం)
    {
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_XOR:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_PLUS:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SCREEN:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DARKEN:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY:
	సంయుక్త_బహుళకం_2 = (hb_paint_సంయుక్త_బహుళకం_t) (గుర్తులేని పూర్ణాంకం) బహుళకం;
	ఆపు;
      అప్రమేయ:
	సంయుక్త_బహుళకం_2 = HB_PAINT_సంయుక్త_బహుళకం_CLEAR;
	ఆపు;
    }

    c->funcs->నెట్టు_సమూహం_for (c->దత్తాంశం, HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER);
    c->recurse (ఇది+backdrop);
    c->funcs->నెట్టు_సమూహం_for (c->దత్తాంశం, సంయుక్త_బహుళకం_2);
    c->recurse (ఇది+src);
    c->funcs->తీయి_సమూహం (c->దత్తాంశం, సంయుక్త_బహుళకం_2);
    c->funcs->తీయి_సమూహం (c->దత్తాంశం, HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER);
  }

  HBUINT8		format; /* format = 32 */
  Offset24_గమ్యం<Paint>	src; /* Offset (from beginning of PaintComposite table) to source Paint subtable. */
  సంయుక్త_బహుళకం		బహుళకం;   /* If mode is unrecognized use COMPOSITE_CLEAR */
  Offset24_గమ్యం<Paint>	backdrop; /* Offset (from beginning of PaintComposite table) to backdrop Paint subtable. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Clip_Box_దత్తాంశం
{
  పూర్ణాంకం xMin, yMin, xMax, yMax;
};

నిర్మాణం ClipBoxFormat1
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_clip_box (Clip_Box_దత్తాంశం &clip_box, స్థిరరాశి అంశం_Var_Store_Instancer &instancer HB_వాడని) స్థిరరాశి
  {
    clip_box.xMin = xMin;
    clip_box.yMin = yMin;
    clip_box.xMax = xMax;
    clip_box.yMax = yMax;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
               uint32_t var_Idx_పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (instancer && !c->plan->pinned_at_default && var_Idx_పునాది != VarIdx::NO_VARIATION)
    {
      బహిర్గతం->xMin = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) xMin + (ద్విగుణ) floorf (instancer (var_Idx_పునాది, 0)));
      బహిర్గతం->yMin = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) yMin + (ద్విగుణ) floorf (instancer (var_Idx_పునాది, 1)));
      బహిర్గతం->xMax = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) xMax + (ద్విగుణ) ceilf (instancer (var_Idx_పునాది, 2)));
      బహిర్గతం->yMax = hb_clamp_గమ్యం<int16_t> ((ద్విగుణ) yMax + (ద్విగుణ) ceilf (instancer (var_Idx_పునాది, 3)));
    }

    ఒకవేళ (format == 2 && c->plan->all_axes_pinned)
        బహిర్గతం->format = 1;

    return_trace (నిజం);
  }

  బహిరంగ:
  HBUINT8	format; /* format = 1(noVar) or 2(Var)*/
  FWORD		xMin;
  FWORD		yMin;
  FWORD		xMax;
  FWORD		yMax;
  బహిరంగ:
  DEFINE_కొలత_STATIC (1 + 4 * FWORD::static_కొలత);
};

నిర్మాణం ClipBoxFormat2 : Variable<ClipBoxFormat1>
{
  శూన్య get_clip_box (Clip_Box_దత్తాంశం &clip_box, స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    value.get_clip_box(clip_box, instancer);
    ఒకవేళ (instancer)
    {
      clip_box.xMin = hb_saturate_కూడు (clip_box.xMin, hb_clamp_గమ్యం<పూర్ణాంకం> (floorf (instancer (var_Idx_పునాది, 0))));
      clip_box.yMin = hb_saturate_కూడు (clip_box.yMin, hb_clamp_గమ్యం<పూర్ణాంకం> (floorf (instancer (var_Idx_పునాది, 1))));
      clip_box.xMax = hb_saturate_కూడు (clip_box.xMax, hb_clamp_గమ్యం<పూర్ణాంకం> (ceilf (instancer (var_Idx_పునాది, 2))));
      clip_box.yMax = hb_saturate_కూడు (clip_box.yMax, hb_clamp_గమ్యం<పూర్ణాంకం> (ceilf (instancer (var_Idx_పునాది, 3))));
    }
  }

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  { c->variation_indices->కూడు_range (var_Idx_పునాది, var_Idx_పునాది + 3); }
};

నిర్మాణం ClipBox
{
  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.subset (c, instancer, VarIdx::NO_VARIATION));
    స్థితి 2: hb_barrier (); return_trace (u.format2.subset (c, instancer));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 2: hb_barrier (); u.format2.closurev1 (c); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక get_extents (hb_glyph_extents_t *extents,
                    స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    Clip_Box_దత్తాంశం clip_box;
    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      u.format1.get_clip_box (clip_box, instancer);
      ఆపు;
    స్థితి 2:
      hb_barrier ();
      u.format2.get_clip_box (clip_box, instancer);
      ఆపు;
    అప్రమేయ:
      తిరిగి అబద్ధం;
    }

    extents->x_bearing = clip_box.xMin;
    extents->y_bearing = clip_box.yMax;
    extents->వెడల్పు = hb_saturate_sub (clip_box.xMax, clip_box.xMin);
    extents->ఎత్తు = hb_saturate_sub (clip_box.yMin, clip_box.yMax);
    తిరిగి నిజం;
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT8 v; }	format;         /* Format identifier */
  ClipBoxFormat1	format1;
  ClipBoxFormat2	format2;
  } u;
};

నిర్మాణం Clip_రికార్డు
{
  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < ప్రారంభం_Glyph_ID ? -1 : g <= చివర_Glyph_ID ? 0 : +1; }

  శూన్య closurev1 (hb_colrv1_సంవృతం_context_t* c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (!c->glyphs->intersects (ప్రారంభం_Glyph_ID, చివర_Glyph_ID)) తిరిగి;
    (పునాది+clipBox).closurev1 (c);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి శూన్య *పునాది,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    return_trace (బహిర్గతం->clipBox.serialize_subset (c, clipBox, పునాది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && clipBox.sanitize (c, పునాది));
  }

  తార్కిక get_extents (hb_glyph_extents_t *extents,
		    స్థిరరాశి శూన్య *పునాది,
		    స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    తిరిగి (పునాది+clipBox).get_extents (extents, instancer);
  }

  బహిరంగ:
  HBUINT16		ప్రారంభం_Glyph_ID;  // first gid clip applies to
  HBUINT16		చివర_Glyph_ID;    // last gid clip applies to, inclusive
  Offset24_గమ్యం<ClipBox>	clipBox;   // Box or VarBox
  బహిరంగ:
  DEFINE_కొలత_STATIC (7);
};
DECLARE_NULL_NAMESPACE_బైట్లు (OT, Clip_రికార్డు);

నిర్మాణం ClipList
{
  గుర్తులేని serialize_clip_రికార్డులు (hb_subset_context_t *c,
                                   స్థిరరాశి అంశం_Var_Store_Instancer &instancer,
                                   స్థిరరాశి hb_సమితి_t& gids,
                                   స్థిరరాశి hb_map_t& gid_స్థానభ్రంశం_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (gids.is_empty () ||
        gid_స్థానభ్రంశం_map.get_population () != gids.get_population ())
      return_trace (0);

    గుర్తులేని లెక్క  = 0;

    hb_codepoint_t ప్రారంభం_gid= gids.get_min ();
    hb_codepoint_t prev_gid = ప్రారంభం_gid;

    గుర్తులేని స్థానభ్రంశం = gid_స్థానభ్రంశం_map.get (ప్రారంభం_gid);
    గుర్తులేని prev_స్థానభ్రంశం = స్థానభ్రంశం;
    కొరకు (స్థిరరాశి hb_codepoint_t _ : gids.iter ())
    {
      ఒకవేళ (_ == ప్రారంభం_gid) కొనసాగు;

      స్థానభ్రంశం = gid_స్థానభ్రంశం_map.get (_);
      ఒకవేళ (_ == prev_gid + 1 &&  స్థానభ్రంశం == prev_స్థానభ్రంశం)
      {
        prev_gid = _;
        కొనసాగు;
      }

      Clip_రికార్డు రికార్డు;
      రికార్డు.ప్రారంభం_Glyph_ID = ప్రారంభం_gid;
      రికార్డు.చివర_Glyph_ID = prev_gid;
      రికార్డు.clipBox = prev_స్థానభ్రంశం;

      ఒకవేళ (!రికార్డు.subset (c, ఇది, instancer)) return_trace (0);
      లెక్క++;

      ప్రారంభం_gid = _;
      prev_gid = _;
      prev_స్థానభ్రంశం = స్థానభ్రంశం;
    }

    //last one
    {
      Clip_రికార్డు రికార్డు;
      రికార్డు.ప్రారంభం_Glyph_ID = ప్రారంభం_gid;
      రికార్డు.చివర_Glyph_ID = prev_gid;
      రికార్డు.clipBox = prev_స్థానభ్రంశం;
      ఒకవేళ (!రికార్డు.subset (c, ఇది, instancer)) return_trace (0);
      లెక్క++;
    }
    return_trace (లెక్క);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->format, format, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) return_trace (అబద్ధం);

    స్థిరరాశి hb_సమితి_t& glyphset = c->plan->_glyphset_colred;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_map_t new_gid_స్థానభ్రంశం_map;
    hb_సమితి_t new_gids;
    కొరకు (స్థిరరాశి Clip_రికార్డు& రికార్డు : clips.iter ())
    {
      గుర్తులేని ప్రారంభం_gid = రికార్డు.ప్రారంభం_Glyph_ID;
      గుర్తులేని చివర_gid = రికార్డు.చివర_Glyph_ID;
      కొరకు (గుర్తులేని gid = ప్రారంభం_gid; gid <= చివర_gid; gid++)
      {
        ఒకవేళ (!glyphset.has (gid) || !glyph_map.has (gid)) కొనసాగు;
        గుర్తులేని new_gid = glyph_map.get (gid);
        new_gid_స్థానభ్రంశం_map.సమితి (new_gid, రికార్డు.clipBox);
        new_gids.కూడు (new_gid);
      }
    }

    గుర్తులేని లెక్క = serialize_clip_రికార్డులు (c, instancer, new_gids, new_gid_స్థానభ్రంశం_map);
    ఒకవేళ (!లెక్క) return_trace (అబద్ధం);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->clips.len, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    // TODO Make a formatted struct!
    return_trace (c->check_struct (ఇది) && clips.sanitize (c, ఇది));
  }

  తార్కిక
  get_extents (hb_codepoint_t gid,
	       hb_glyph_extents_t *extents,
	       స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    స్వయంచాలక *rec = clips.as_array ().bsearch (gid);
    ఒకవేళ (rec)
      తిరిగి rec->get_extents (extents, ఇది, instancer);
    తిరిగి అబద్ధం;
  }

  HBUINT8			format;  // Set to 1.
  Sorted_Array32_యొక్క<Clip_రికార్డు>	clips;  // Clip records, sorted by startGlyphID
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (5, clips);
};

నిర్మాణం Paint
{

  మూస <రకం_పేరు ...Ts>
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (unlikely (!c->check_ప్రారంభం_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_విలువ ());

    return_trace (c->చివర_recursion (ఇది->dispatch (c, std::forward<Ts> (ds)...)));
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.paintformat1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.paintformat2, std::forward<Ts> (ds)...));
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.paintformat3, std::forward<Ts> (ds)...));
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.paintformat4, std::forward<Ts> (ds)...));
    స్థితి 5: hb_barrier (); return_trace (c->dispatch (u.paintformat5, std::forward<Ts> (ds)...));
    స్థితి 6: hb_barrier (); return_trace (c->dispatch (u.paintformat6, std::forward<Ts> (ds)...));
    స్థితి 7: hb_barrier (); return_trace (c->dispatch (u.paintformat7, std::forward<Ts> (ds)...));
    స్థితి 8: hb_barrier (); return_trace (c->dispatch (u.paintformat8, std::forward<Ts> (ds)...));
    స్థితి 9: hb_barrier (); return_trace (c->dispatch (u.paintformat9, std::forward<Ts> (ds)...));
    స్థితి 10: hb_barrier (); return_trace (c->dispatch (u.paintformat10, std::forward<Ts> (ds)...));
    స్థితి 11: hb_barrier (); return_trace (c->dispatch (u.paintformat11, std::forward<Ts> (ds)...));
    స్థితి 12: hb_barrier (); return_trace (c->dispatch (u.paintformat12, std::forward<Ts> (ds)...));
    స్థితి 13: hb_barrier (); return_trace (c->dispatch (u.paintformat13, std::forward<Ts> (ds)...));
    స్థితి 14: hb_barrier (); return_trace (c->dispatch (u.paintformat14, std::forward<Ts> (ds)...));
    స్థితి 15: hb_barrier (); return_trace (c->dispatch (u.paintformat15, std::forward<Ts> (ds)...));
    స్థితి 16: hb_barrier (); return_trace (c->dispatch (u.paintformat16, std::forward<Ts> (ds)...));
    స్థితి 17: hb_barrier (); return_trace (c->dispatch (u.paintformat17, std::forward<Ts> (ds)...));
    స్థితి 18: hb_barrier (); return_trace (c->dispatch (u.paintformat18, std::forward<Ts> (ds)...));
    స్థితి 19: hb_barrier (); return_trace (c->dispatch (u.paintformat19, std::forward<Ts> (ds)...));
    స్థితి 20: hb_barrier (); return_trace (c->dispatch (u.paintformat20, std::forward<Ts> (ds)...));
    స్థితి 21: hb_barrier (); return_trace (c->dispatch (u.paintformat21, std::forward<Ts> (ds)...));
    స్థితి 22: hb_barrier (); return_trace (c->dispatch (u.paintformat22, std::forward<Ts> (ds)...));
    స్థితి 23: hb_barrier (); return_trace (c->dispatch (u.paintformat23, std::forward<Ts> (ds)...));
    స్థితి 24: hb_barrier (); return_trace (c->dispatch (u.paintformat24, std::forward<Ts> (ds)...));
    స్థితి 25: hb_barrier (); return_trace (c->dispatch (u.paintformat25, std::forward<Ts> (ds)...));
    స్థితి 26: hb_barrier (); return_trace (c->dispatch (u.paintformat26, std::forward<Ts> (ds)...));
    స్థితి 27: hb_barrier (); return_trace (c->dispatch (u.paintformat27, std::forward<Ts> (ds)...));
    స్థితి 28: hb_barrier (); return_trace (c->dispatch (u.paintformat28, std::forward<Ts> (ds)...));
    స్థితి 29: hb_barrier (); return_trace (c->dispatch (u.paintformat29, std::forward<Ts> (ds)...));
    స్థితి 30: hb_barrier (); return_trace (c->dispatch (u.paintformat30, std::forward<Ts> (ds)...));
    స్థితి 31: hb_barrier (); return_trace (c->dispatch (u.paintformat31, std::forward<Ts> (ds)...));
    స్థితి 32: hb_barrier (); return_trace (c->dispatch (u.paintformat32, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 33: hb_barrier (); return_trace (c->dispatch (u.paintformat33, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక get_solid_color (hb_paint_context_t *c,
			hb_bool_t *is_foreground, hb_color_t *color) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 2: hb_barrier (); u.paintformat2.get_solid_color (c, is_foreground, color); తిరిగి నిజం;
    స్థితి 3: hb_barrier (); u.paintformat3.get_solid_color (c, is_foreground, color); తిరిగి నిజం;
    అప్రమేయ: తిరిగి అబద్ధం;
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT8 v; }				format;
  PaintColrLayers				paintformat1;
  NoVariable<PaintSolid>			paintformat2;
  Variable<PaintSolid>				paintformat3;
  NoVariable<Paint_రేఖీయ_Gradient<NoVariable>>	paintformat4;
  Variable<Paint_రేఖీయ_Gradient<Variable>>	paintformat5;
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
  NoVariable<Paint_Scale_ఏకరీతి>			paintformat20;
  Variable<Paint_Scale_ఏకరీతి>			paintformat21;
  NoVariable<Paint_Scale_ఏకరీతి_Around_Center>	paintformat22;
  Variable<Paint_Scale_ఏకరీతి_Around_Center>	paintformat23;
  NoVariable<PaintRotate>			paintformat24;
  Variable<PaintRotate>				paintformat25;
  NoVariable<PaintRotateAroundCenter>		paintformat26;
  Variable<PaintRotateAroundCenter>		paintformat27;
  NoVariable<PaintSkew>				paintformat28;
  Variable<PaintSkew>				paintformat29;
  NoVariable<PaintSkewAroundCenter>		paintformat30;
  Variable<PaintSkewAroundCenter>		paintformat31;
  Paint_సంయుక్త				paintformat32;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  PaintGlyph<Layout::MediumTypes>		paintformat33;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (2);
};

నిర్మాణం పునాది_Glyph_Paint_రికార్డు
{
  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < glyphId ? -1 : g > glyphId ? 1 : 0; }

  తార్కిక serialize (hb_serialize_context_t *s, స్థిరరాశి hb_map_t* glyph_map,
                  స్థిరరాశి శూన్య* src_పునాది, hb_subset_context_t *c,
                  స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = s->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    ఒకవేళ (!s->check_కేటాయించు (బహిర్గతం->glyphId, glyph_map->get (glyphId),
                          HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->paint.serialize_subset (c, paint, src_పునాది, instancer));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) && paint.sanitize (c, పునాది)));
  }

  బహిరంగ:
  HBGlyphID16		glyphId;    /* Glyph ID of reference glyph */
  Offset32_గమ్యం<Paint>	paint;      /* Offset (from beginning of BaseGlyphPaintRecord array) to Paint,
                                     * Typically PaintColrLayers */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం పునాది_Glyph_List : Sorted_Array32_యొక్క<పునాది_Glyph_Paint_రికార్డు>
{
  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం)))  return_trace (అబద్ధం);
    స్థిరరాశి hb_సమితి_t* glyphset = &c->plan->_glyphset_colred;

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : as_array ())
    {
      గుర్తులేని gid = _.glyphId;
      ఒకవేళ (!glyphset->has (gid)) కొనసాగు;

      ఒకవేళ (_.serialize (c->serializer, c->plan->glyph_map, ఇది, c, instancer)) బహిర్గతం->len++;
      లేకపోతే return_trace (అబద్ధం);
    }

    return_trace (బహిర్గతం->len != 0);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (Sorted_Array32_యొక్క<పునాది_Glyph_Paint_రికార్డు>::sanitize (c, ఇది));
  }
};

నిర్మాణం LayerList : Array32_యొక్క_Offset32_గమ్యం<Paint>
{
  స్థిరరాశి Paint& get_paint (గుర్తులేని i) స్థిరరాశి
  { తిరిగి ఇది+(*ఇది)[i]; }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి అంశం_Var_Store_Instancer &instancer) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం)))  return_trace (అబద్ధం);

    తార్కిక ret = అబద్ధం;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (*ఇది)
                         | hb_filter (c->plan->colrv1_layers, hb_మొదటి))

    {
      స్వయంచాలక *o = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o)) return_trace (అబద్ధం);
      ret |= o->serialize_subset (c, _.second, ఇది, instancer);
    }
    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (Array32_యొక్క_Offset32_గమ్యం<Paint>::sanitize (c, ఇది));
  }
};

నిర్మాణం delta_సమితి_సూచీ_map_subset_plan_t
{
  గుర్తులేని get_inner_బిట్టు_లెక్క () స్థిరరాశి { తిరిగి inner_బిట్టు_లెక్క; }
  గుర్తులేని get_వెడల్పు ()           స్థిరరాశి { తిరిగి ((outer_బిట్టు_లెక్క + inner_బిట్టు_లెక్క + 7) / 8); }
  hb_array_t<స్థిరరాశి uint32_t> get_output_map () స్థిరరాశి { తిరిగి output_map.as_array (); }

  delta_సమితి_సూచీ_map_subset_plan_t (స్థిరరాశి hb_map_t &new_deltaset_idx_varidx_map)
  {
    map_లెక్క = 0;
    outer_బిట్టు_లెక్క = 0;
    inner_బిట్టు_లెక్క = 1;
    output_map.init ();

    /* search backwards */
    గుర్తులేని లెక్క = new_deltaset_idx_varidx_map.get_population ();
    ఒకవేళ (!లెక్క) తిరిగి;

    గుర్తులేని చివరి_idx = (గుర్తులేని)-1;
    గుర్తులేని చివరి_varidx = (గుర్తులేని)-1;

    కొరకు (గుర్తులేని i = లెక్క; i; i--)
    {
      గుర్తులేని delta_సమితి_idx = i - 1;
      గుర్తులేని var_idx = new_deltaset_idx_varidx_map.get (delta_సమితి_idx);
      ఒకవేళ (i == లెక్క)
      {
        చివరి_idx = delta_సమితి_idx;
        చివరి_varidx = var_idx;
        కొనసాగు;
      }
      ఒకవేళ (var_idx != చివరి_varidx)
        ఆపు;
      చివరి_idx = delta_సమితి_idx;
    }

    map_లెక్క = చివరి_idx + 1;
  }

  తార్కిక remap (స్థిరరాశి hb_map_t &new_deltaset_idx_varidx_map)
  {
    /* recalculate bit_count */
    outer_బిట్టు_లెక్క = 1;
    inner_బిట్టు_లెక్క = 1;

    ఒకవేళ (unlikely (!output_map.resize_dirty (map_లెక్క))) తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని idx = 0; idx < map_లెక్క; idx++)
    {
      uint32_t *var_idx;
      ఒకవేళ (!new_deltaset_idx_varidx_map.has (idx, &var_idx)) తిరిగి అబద్ధం;
      output_map.arrayZ[idx] = *var_idx;

      గుర్తులేని outer = (*var_idx) >> 16;
      గుర్తులేని బిట్టు_లెక్క = (outer == 0) ? 1 : hb_బిట్టు_storage (outer);
      outer_బిట్టు_లెక్క = hb_max (బిట్టు_లెక్క, outer_బిట్టు_లెక్క);

      గుర్తులేని inner = (*var_idx) & 0xFFFF;
      బిట్టు_లెక్క = (inner == 0) ? 1 : hb_బిట్టు_storage (inner);
      inner_బిట్టు_లెక్క = hb_max (బిట్టు_లెక్క, inner_బిట్టు_లెక్క);
    }
    తిరిగి నిజం;
  }

  అంతరంగ:
  గుర్తులేని map_లెక్క;
  గుర్తులేని outer_బిట్టు_లెక్క;
  గుర్తులేని inner_బిట్టు_లెక్క;
  hb_సదిశ_t<uint32_t> output_map;
};

నిర్మాణం COLR
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_COLR;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి has_v0_దత్తాంశం () || సంస్కరణ; }

  తార్కిక has_v0_దత్తాంశం () స్థిరరాశి { తిరిగి అంకె_పునాది_Glyphs; }
  తార్కిక has_v1_దత్తాంశం () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ < 1)
      తిరిగి అబద్ధం;
    hb_barrier ();

    తిరిగి (ఇది+పునాది_Glyph_List_2).len > 0;
  }

  గుర్తులేని పూర్ణాంకం get_glyph_layers (hb_codepoint_t       glyph,
				 గుర్తులేని పూర్ణాంకం         ప్రారంభం_స్థానభ్రంశం,
				 గుర్తులేని పూర్ణాంకం        *లెక్క, /* IN/OUT.  May be NULL. */
				 hb_ot_color_layer_t *layers /* OUT.     May be NULL. */) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Glyph_రికార్డు &రికార్డు = (ఇది+పునాది_Glyphs_Z).bsearch (అంకె_పునాది_Glyphs, glyph);

    hb_array_t<స్థిరరాశి Layer_రికార్డు> all_layers = (ఇది+layersZ).as_array (అంకె_Layers);
    hb_array_t<స్థిరరాశి Layer_రికార్డు> glyph_layers = all_layers.sub_array (రికార్డు.మొదటి_Layer_Idx,
								       రికార్డు.అంకె_Layers);
    ఒకవేళ (లెక్క && layers)
    {
      + glyph_layers.sub_array (ప్రారంభం_స్థానభ్రంశం, లెక్క)
      | hb_sink (hb_array (layers, *లెక్క))
      ;
    }
    తిరిగి glyph_layers.నిడివి;
  }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { colr = hb_sanitize_context_t ().reference_పట్టిక<COLR> (face); }

    ~accelerator_t ()
    {
      స్వయంచాలక *scratch = cached_scratch.get_relaxed ();
      ఒకవేళ (scratch)
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }

      colr.destroy ();
    }


    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి colr->has_దత్తాంశం (); }

#నిర్వచితం_కాకపోతే HB_NO_PAINT
    తార్కిక
    get_extents (hb_font_t *font,
		 hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents) స్థిరరాశి
    {
      ఒకవేళ (unlikely (!has_దత్తాంశం ())) తిరిగి అబద్ధం;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ఒకవేళ (unlikely (!scratch)) తిరిగి నిజం;
      తార్కిక ret = colr->get_extents (font, glyph, extents, *scratch);
      release_scratch (scratch);
      తిరిగి ret;
    }

    తార్కిక paint_glyph (hb_font_t *font,
		      hb_codepoint_t glyph,
		      hb_paint_funcs_t *funcs, శూన్య *దత్తాంశం,
		      గుర్తులేని పూర్ణాంకం palette_సూచీ,
		      hb_color_t foreground,
		      తార్కిక clip = నిజం) స్థిరరాశి
    {
      ఒకవేళ (unlikely (!has_దత్తాంశం ())) తిరిగి అబద్ధం;

      hb_colr_scratch_t *scratch = acquire_scratch ();
      ఒకవేళ (unlikely (!scratch)) తిరిగి నిజం;
      తార్కిక ret = colr->paint_glyph (font, glyph, funcs, దత్తాంశం, palette_సూచీ, foreground, clip, *scratch);
      release_scratch (scratch);
      తిరిగి ret;
    }
#అయితే_ముగింపు

    తార్కిక is_valid () { తిరిగి colr.get_blob ()->నిడివి; }

    శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
    {
      ఒకవేళ (!has_దత్తాంశం ()) తిరిగి;
      colr->depend (depend_దత్తాంశం);
    }

    శూన్య సంవృతం_glyphs (hb_codepoint_t glyph,
			 hb_సమితి_t *related_ids /*బహిర్గతం*/) స్థిరరాశి
    { colr->సంవృతం_glyphs (glyph, related_ids); }

    శూన్య సంవృతం_V0palette_indices (స్థిరరాశి hb_సమితి_t *glyphs,
				    hb_సమితి_t *palettes /*బహిర్గతం*/) స్థిరరాశి
    { colr->సంవృతం_V0palette_indices (glyphs, palettes); }

    శూన్య సంవృతం_for_V1 (hb_సమితి_t *glyphset,
                        hb_సమితి_t *layer_indices,
                        hb_సమితి_t *palette_indices,
                        hb_సమితి_t *variation_indices,
                        hb_సమితి_t *delta_సమితి_indices) స్థిరరాశి
    { colr->సంవృతం_for_V1 (glyphset, layer_indices, palette_indices, variation_indices, delta_సమితి_indices); }

    తార్కిక has_var_store () స్థిరరాశి
    { తిరిగి colr->has_var_store (); }

    స్థిరరాశి అంశం_Variation_Store &get_var_store () స్థిరరాశి
    { తిరిగి colr->get_var_store (); }
    స్థిరరాశి అంశం_Variation_Store *get_var_store_ptr () స్థిరరాశి
    { తిరిగి colr->get_var_store_ptr (); }

    తార్కిక has_delta_సమితి_సూచీ_map () స్థిరరాశి
    { తిరిగి colr->has_delta_సమితి_సూచీ_map (); }

    స్థిరరాశి Delta_సమితి_సూచీ_Map &get_delta_సమితి_సూచీ_map () స్థిరరాశి
    { తిరిగి colr->get_delta_సమితి_సూచీ_map (); }
    స్థిరరాశి Delta_సమితి_సూచీ_Map *get_delta_సమితి_సూచీ_map_ptr () స్థిరరాశి
    { తిరిగి colr->get_delta_సమితి_సూచీ_map_ptr (); }

    అంతరంగ:

    hb_colr_scratch_t *acquire_scratch () స్థిరరాశి
    {
      hb_colr_scratch_t *scratch = cached_scratch.get_acquire ();

      ఒకవేళ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, శూన్యసూచిక)))
      {
	scratch = (hb_colr_scratch_t *) hb_calloc (1, పరిమాణం (hb_colr_scratch_t));
	ఒకవేళ (unlikely (!scratch))
	  తిరిగి శూన్యసూచిక;
      }

      తిరిగి scratch;
    }
    శూన్య release_scratch (hb_colr_scratch_t *scratch) స్థిరరాశి
    {
      ఒకవేళ (!cached_scratch.cmpexch (శూన్యసూచిక, scratch))
      {
	scratch->~hb_colr_scratch_t ();
	hb_free (scratch);
      }
    }

    బహిరంగ:
    hb_blob_ptr_t<COLR> colr;
    అంతరంగ:
    మార్చదగిన hb_atomic_t<hb_colr_scratch_t *> cached_scratch;
  };

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
  {
    // v0
    hb_array_t<స్థిరరాశి పునాది_Glyph_రికార్డు> పునాది_Glyphs = (ఇది+పునాది_Glyphs_Z).as_array (అంకె_పునాది_Glyphs);
    కొరకు (స్థిరరాశి పునాది_Glyph_రికార్డు &రికార్డు : పునాది_Glyphs)
    {
      స్వయంచాలక glyph_layers = (ఇది+layersZ).as_array (అంకె_Layers).sub_array (రికార్డు.మొదటి_Layer_Idx,
                                                                         రికార్డు.అంకె_Layers);
      కొరకు (స్థిరరాశి Layer_రికార్డు layer : glyph_layers)
        depend_దత్తాంశం->కూడు_depend(రికార్డు.glyphId, HB_OT_ట్యాగ్_COLR, layer.glyphId);
    }

    ఒకవేళ (!has_v1_దత్తాంశం())
      తిరిగి;

    // v1
    hb_colrv1_depend_context_t c (ఇది, depend_దత్తాంశం);
    స్థిరరాశి పునాది_Glyph_List &baseglyph_paintrecords = ఇది+పునాది_Glyph_List_2;

    కొరకు (స్థిరరాశి పునాది_Glyph_Paint_రికార్డు &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      c.ఆధారం_gid = baseglyph_paintrecord.glyphId;
      c.సందర్శించిన_paint.clear ();

      స్థిరరాశి Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
  }

  శూన్య సంవృతం_glyphs (hb_codepoint_t glyph,
		       hb_సమితి_t *related_ids /*బహిర్గతం*/) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Glyph_రికార్డు *రికార్డు = get_పునాది_glyph_రికార్డు (glyph);
    ఒకవేళ (!రికార్డు) తిరిగి;

    స్వయంచాలక glyph_layers = (ఇది+layersZ).as_array (అంకె_Layers).sub_array (రికార్డు->మొదటి_Layer_Idx,
								       రికార్డు->అంకె_Layers);
    ఒకవేళ (!glyph_layers.నిడివి) తిరిగి;
    related_ids->కూడు_array (&glyph_layers[0].glyphId, glyph_layers.నిడివి, Layer_రికార్డు::min_కొలత);
  }

  శూన్య సంవృతం_V0palette_indices (స్థిరరాశి hb_సమితి_t *glyphs,
				  hb_సమితి_t *palettes /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (!అంకె_పునాది_Glyphs || !అంకె_Layers) తిరిగి;
    hb_array_t<స్థిరరాశి పునాది_Glyph_రికార్డు> పునాది_Glyphs = (ఇది+పునాది_Glyphs_Z).as_array (అంకె_పునాది_Glyphs);
    hb_array_t<స్థిరరాశి Layer_రికార్డు> all_layers = (ఇది+layersZ).as_array (అంకె_Layers);

    కొరకు (స్థిరరాశి పునాది_Glyph_రికార్డు రికార్డు : పునాది_Glyphs)
    {
      ఒకవేళ (!glyphs->has (రికార్డు.glyphId)) కొనసాగు;
      hb_array_t<స్థిరరాశి Layer_రికార్డు> glyph_layers = all_layers.sub_array (రికార్డు.మొదటి_Layer_Idx,
                                                                   రికార్డు.అంకె_Layers);
      కొరకు (స్థిరరాశి Layer_రికార్డు layer : glyph_layers)
        palettes->కూడు (layer.colorIdx);
    }
  }

  శూన్య సంవృతం_for_V1 (hb_సమితి_t *glyphset,
                      hb_సమితి_t *layer_indices,
                      hb_సమితి_t *palette_indices,
                      hb_సమితి_t *variation_indices,
                      hb_సమితి_t *delta_సమితి_indices) స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ < 1) తిరిగి;
    hb_barrier ();

    hb_సమితి_t సందర్శించిన_glyphs;

    hb_colrv1_సంవృతం_context_t c (ఇది, &సందర్శించిన_glyphs, layer_indices, palette_indices, variation_indices);
    స్థిరరాశి పునాది_Glyph_List &baseglyph_paintrecords = ఇది+పునాది_Glyph_List_2;

    కొరకు (స్థిరరాశి పునాది_Glyph_Paint_రికార్డు &baseglyph_paintrecord: baseglyph_paintrecords.iter ())
    {
      గుర్తులేని gid = baseglyph_paintrecord.glyphId;
      ఒకవేళ (!glyphset->has (gid)) కొనసాగు;

      స్థిరరాశి Paint &paint = &baseglyph_paintrecords+baseglyph_paintrecord.paint;
      paint.dispatch (&c);
    }
    hb_సమితి_union (glyphset, &సందర్శించిన_glyphs);

    స్థిరరాశి ClipList &cliplist = ఇది+clipList;
    c.glyphs = glyphset;
    కొరకు (స్థిరరాశి Clip_రికార్డు &clip_రికార్డు : cliplist.clips.iter())
      clip_రికార్డు.closurev1 (&c, &cliplist);

    // if a DeltaSetIndexMap is included, collected variation indices are
    // actually delta set indices, we need to map them into variation indices
    ఒకవేళ (has_delta_సమితి_సూచీ_map ())
    {
      స్థిరరాశి Delta_సమితి_సూచీ_Map &var_idx_map = ఇది+varIdxMap;
      delta_సమితి_indices->సమితి (*variation_indices);
      variation_indices->clear ();
      కొరకు (గుర్తులేని delta_సమితి_idx : *delta_సమితి_indices)
        variation_indices->కూడు (var_idx_map.map (delta_సమితి_idx));
    }
  }

  స్థిరరాశి LayerList& get_layerList () స్థిరరాశి
  { తిరిగి (ఇది+layerList); }

  స్థిరరాశి పునాది_Glyph_List& get_baseglyphList () స్థిరరాశి
  { తిరిగి (ఇది+పునాది_Glyph_List_2); }

  తార్కిక has_var_store () స్థిరరాశి
  { తిరిగి సంస్కరణ >= 1 && hb_barrier () && varStore != 0; }

  తార్కిక has_delta_సమితి_సూచీ_map () స్థిరరాశి
  { తిరిగి సంస్కరణ >= 1 && hb_barrier () && varIdxMap != 0; }

  తార్కిక has_clip_list () స్థిరరాశి
  { తిరిగి సంస్కరణ >= 1 && hb_barrier () && clipList != 0; }

  స్థిరరాశి Delta_సమితి_సూచీ_Map &get_delta_సమితి_సూచీ_map () స్థిరరాశి
  { తిరిగి has_delta_సమితి_సూచీ_map () && hb_barrier () ? ఇది+varIdxMap : Null (Delta_సమితి_సూచీ_Map); }
  స్థిరరాశి Delta_సమితి_సూచీ_Map *get_delta_సమితి_సూచీ_map_ptr () స్థిరరాశి
  { తిరిగి has_delta_సమితి_సూచీ_map () && hb_barrier () ? &(ఇది+varIdxMap) : శూన్యసూచిక; }

  స్థిరరాశి అంశం_Variation_Store &get_var_store () స్థిరరాశి
  { తిరిగి has_var_store () && hb_barrier () ? ఇది+varStore : Null (అంశం_Variation_Store); }
  స్థిరరాశి అంశం_Variation_Store *get_var_store_ptr () స్థిరరాశి
  { తిరిగి has_var_store () && hb_barrier () ? &(ఇది+varStore) : శూన్యసూచిక; }

  స్థిరరాశి ClipList &get_clip_list () స్థిరరాశి
  { తిరిగి has_clip_list () && hb_barrier () ? ఇది+clipList : Null (ClipList); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
                  (ఇది+పునాది_Glyphs_Z).sanitize (c, అంకె_పునాది_Glyphs) &&
                  (ఇది+layersZ).sanitize (c, అంకె_Layers) &&
                  (సంస్కరణ == 0 ||
		   (hb_barrier () &&
		    పునాది_Glyph_List_2.sanitize (c, ఇది) &&
		    layerList.sanitize (c, ఇది) &&
		    clipList.sanitize (c, ఇది) &&
		    varIdxMap.sanitize (c, ఇది) &&
		    varStore.sanitize (c, ఇది))));
  }

  మూస<రకం_పేరు పునాది_Iterator, రకం_పేరు LayerIterator,
	   hb_requires (hb_is_iterator (పునాది_Iterator)),
	   hb_requires (hb_is_iterator (LayerIterator))>
  తార్కిక serialize_V0 (hb_serialize_context_t *c,
		     గుర్తులేని సంస్కరణ,
		     పునాది_Iterator పునాది_it,
		     LayerIterator layer_it)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (పునాది_it.len () != layer_it.len ()))
      return_trace (అబద్ధం);

    ఇది->సంస్కరణ = సంస్కరణ;
    అంకె_Layers = 0;
    అంకె_పునాది_Glyphs = పునాది_it.len ();
    ఒకవేళ (అంకె_పునాది_Glyphs == 0)
    {
      పునాది_Glyphs_Z = 0;
      layersZ = 0;
      return_trace (నిజం);
    }

    c->push ();
    కొరకు (స్థిరరాశి hb_అంశం_type<పునాది_Iterator> _ : + పునాది_it.iter ())
    {
      స్వయంచాలక* రికార్డు = c->embed (_);
      ఒకవేళ (unlikely (!రికార్డు)) return_trace (అబద్ధం);
      రికార్డు->మొదటి_Layer_Idx = అంకె_Layers;
      అంకె_Layers += రికార్డు->అంకె_Layers;
    }
    c->కూడు_link (పునాది_Glyphs_Z, c->తీయి_pack ());

    c->push ();
    కొరకు (స్థిరరాశి hb_అంశం_type<LayerIterator>& _ : + layer_it.iter ())
      _.as_array ().copy (c);

    c->కూడు_link (layersZ, c->తీయి_pack ());

    return_trace (నిజం);
  }

  స్థిరరాశి పునాది_Glyph_రికార్డు* get_పునాది_glyph_రికార్డు (hb_codepoint_t gid) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Glyph_రికార్డు* రికార్డు = &(ఇది+పునాది_Glyphs_Z).bsearch (అంకె_పునాది_Glyphs, (గుర్తులేని పూర్ణాంకం) gid);
    ఒకవేళ (రికార్డు == &Null (పునాది_Glyph_రికార్డు) ||
        (రికార్డు && (hb_codepoint_t) రికార్డు->glyphId != gid))
      రికార్డు = శూన్యసూచిక;
    తిరిగి రికార్డు;
  }

  స్థిరరాశి పునాది_Glyph_Paint_రికార్డు* get_పునాది_glyph_paintrecord (hb_codepoint_t gid) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Glyph_List &list = ఇది+పునాది_Glyph_List_2;
    గుర్తులేని పూర్ణాంకం i;
    ఒకవేళ (!list.bfind ((గుర్తులేని) gid, &i))
      తిరిగి శూన్యసూచిక;
    తిరిగి &list[i];
  }

  తార్కిక downgrade_గమ్యం_V0 (స్థిరరాశి hb_సమితి_t &glyphset) స్థిరరాశి
  {
    //no more COLRv1 glyphs, downgrade to version 0
    కొరకు (స్థిరరాశి పునాది_Glyph_Paint_రికార్డు& _ : get_baseglyphList ())
      ఒకవేళ (glyphset.has (_.glyphId))
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక subset_varstore (hb_subset_context_t *c,
                        COLR* బహిర్గతం /*బహిర్గతం*/) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!varStore || c->plan->all_axes_pinned ||
        !c->plan->colrv1_variation_idx_delta_map)
      return_trace (నిజం);

    స్థిరరాశి అంశం_Variation_Store& var_store = ఇది+varStore;
    ఒకవేళ (c->plan->normalized_coords)
    {
      అంశం_variations_t అంశం_vars;
      /* turn off varstore optimization when varIdxMap is null, so we maintain
       * original var_idx sequence */
      తార్కిక optimize = (varIdxMap != 0) ? నిజం : అబద్ధం;
      ఒకవేళ (!అంశం_vars.instantiate (var_store, c->plan,
                                  optimize, /* optimization */
                                  optimize, /* use_no_variation_idx = false */
                                  c->plan->colrv1_varstore_inner_maps.as_array ()))
        return_trace (అబద్ధం);

      /* do not serialize varStore if there's no variation data after
       * instancing: region_list or var_data is empty */
      ఒకవేళ (అంశం_vars.get_region_list () &&
          అంశం_vars.get_vardata_encodings () &&
          !బహిర్గతం->varStore.serialize_serialize (c->serializer,
                                              అంశం_vars.has_long_మాట (),
                                              c->plan->axis_ట్యాగులు,
                                              అంశం_vars.get_region_list (),
                                              అంశం_vars.get_vardata_encodings ()))
        return_trace (అబద్ధం);

      /* if varstore is optimized, update colrv1_new_deltaset_idx_varidx_map in
       * subset plan.
       * If varstore is empty after instancing, varidx_map would be empty and
       * all var_idxes will be updated to VarIdx::NO_VARIATION */
      ఒకవేళ (optimize)
      {
        స్థిరరాశి hb_map_t &varidx_map = అంశం_vars.get_varidx_map ();
        కొరకు (స్వయంచాలక _ : c->plan->colrv1_new_deltaset_idx_varidx_map.iter_ref ())
        {
          uint32_t varidx = _.second;
          uint32_t *new_varidx;
          ఒకవేళ (varidx_map.has (varidx, &new_varidx))
            _.second = *new_varidx;
          లేకపోతే
            _.second = VarIdx::NO_VARIATION;
        }
      }
    }
    లేకపోతే
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_సమితి_t dead_regions;
      ఒకవేళ (c->plan->has_avar2)
        var_store.collect_dead_regions (c->plan->axes_old_సూచీ_ట్యాగ్_map,
                                        c->plan->avar2_reachable_ranges,
                                        dead_regions);

      ఒకవేళ (unlikely (!బహిర్గతం->varStore.serialize_serialize (c->serializer,
                                                        &var_store,
                                                        c->plan->colrv1_varstore_inner_maps.as_array (),
                                                        dead_regions.get_population () ? &dead_regions : శూన్యసూచిక)))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  తార్కిక subset_delta_సమితి_సూచీ_map (hb_subset_context_t *c,
                                   COLR* బహిర్గతం /*బహిర్గతం*/) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!varIdxMap || c->plan->all_axes_pinned ||
        !c->plan->colrv1_new_deltaset_idx_varidx_map)
      return_trace (నిజం);

    స్థిరరాశి hb_map_t &deltaset_idx_varidx_map = c->plan->colrv1_new_deltaset_idx_varidx_map;
    delta_సమితి_సూచీ_map_subset_plan_t సూచీ_map_plan (deltaset_idx_varidx_map);

    ఒకవేళ (unlikely (!సూచీ_map_plan.remap (deltaset_idx_varidx_map)))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->varIdxMap.serialize_serialize (c->serializer, సూచీ_map_plan));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_map_t &తిరగేయి_glyph_map = *c->plan->తిరగేయి_glyph_map;
    స్థిరరాశి hb_సమితి_t& glyphset = c->plan->_glyphset_colred;

    స్వయంచాలక పునాది_it =
    + hb_range (c->plan->అంకె_output_glyphs ())
    | hb_filter ([&](hb_codepoint_t new_gid)
		 {
		    hb_codepoint_t old_gid = తిరగేయి_glyph_map.get (new_gid);
		    ఒకవేళ (glyphset.has (old_gid)) తిరిగి నిజం;
		    తిరిగి అబద్ధం;
		 })
    | hb_map_retains_క్రమీకరణ ([&](hb_codepoint_t new_gid)
			      {
				hb_codepoint_t old_gid = తిరగేయి_glyph_map.get (new_gid);

				స్థిరరాశి పునాది_Glyph_రికార్డు* old_రికార్డు = get_పునాది_glyph_రికార్డు (old_gid);
				ఒకవేళ (unlikely (!old_రికార్డు))
				  తిరిగి hb_pair_t<తార్కిక, పునాది_Glyph_రికార్డు> (అబద్ధం, Null (పునాది_Glyph_రికార్డు));
				పునాది_Glyph_రికార్డు new_రికార్డు = {};
				new_రికార్డు.glyphId = new_gid;
				new_రికార్డు.అంకె_Layers = old_రికార్డు->అంకె_Layers;
				తిరిగి hb_pair_t<తార్కిక, పునాది_Glyph_రికార్డు> (నిజం, new_రికార్డు);
			      })
    | hb_filter (hb_మొదటి)
    | hb_map_retains_క్రమీకరణ (hb_రెండవ)
    ;

    స్వయంచాలక layer_it =
    + hb_range (c->plan->అంకె_output_glyphs ())
    | hb_map (తిరగేయి_glyph_map)
    | hb_filter (glyphset)
    | hb_map_retains_క్రమీకరణ ([&](hb_codepoint_t old_gid)
			      {
				స్థిరరాశి పునాది_Glyph_రికార్డు* old_రికార్డు = get_పునాది_glyph_రికార్డు (old_gid);
				hb_సదిశ_t<Layer_రికార్డు> బహిర్గతం_layers;

				ఒకవేళ (unlikely (!old_రికార్డు ||
					      old_రికార్డు->మొదటి_Layer_Idx >= అంకె_Layers ||
					      old_రికార్డు->మొదటి_Layer_Idx + old_రికార్డు->అంకె_Layers > అంకె_Layers))
				  తిరిగి hb_pair_t<తార్కిక, hb_సదిశ_t<Layer_రికార్డు>> (అబద్ధం, బహిర్గతం_layers);

				స్వయంచాలక layers = (ఇది+layersZ).as_array (అంకె_Layers).sub_array (old_రికార్డు->మొదటి_Layer_Idx,
											     old_రికార్డు->అంకె_Layers);
				బహిర్గతం_layers.resize (layers.నిడివి);
				కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < layers.నిడివి; i++) {
				  బహిర్గతం_layers[i] = layers[i];
				  hb_codepoint_t new_gid = 0;
				  ఒకవేళ (unlikely (!c->plan->new_gid_for_old_gid (బహిర్గతం_layers[i].glyphId, &new_gid)))
				    తిరిగి hb_pair_t<తార్కిక, hb_సదిశ_t<Layer_రికార్డు>> (అబద్ధం, బహిర్గతం_layers);
				  బహిర్గతం_layers[i].glyphId = new_gid;
				  బహిర్గతం_layers[i].colorIdx = c->plan->colr_palettes.get (layers[i].colorIdx);
				}

				తిరిగి hb_pair_t<తార్కిక, hb_సదిశ_t<Layer_రికార్డు>> (నిజం, బహిర్గతం_layers);
			      })
    | hb_filter (hb_మొదటి)
    | hb_map_retains_క్రమీకరణ (hb_రెండవ)
    ;

    ఒకవేళ (సంస్కరణ == 0 && (!పునాది_it || !layer_it))
      return_trace (అబద్ధం);

    స్వయంచాలక *colr_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<COLR> ();
    ఒకవేళ (unlikely (!c->serializer->extend_min (colr_ప్రధానసంఖ్య)))  return_trace (అబద్ధం);

    ఒకవేళ (సంస్కరణ == 0 || downgrade_గమ్యం_V0 (glyphset))
      return_trace (colr_ప్రధానసంఖ్య->serialize_V0 (c->serializer, 0, పునాది_it, layer_it));

    hb_barrier ();

    //start version 1
    ఒకవేళ (!c->serializer->allocate_కొలత<శూన్య> (5 * HBUINT32::static_కొలత)) return_trace (అబద్ధం);
    ఒకవేళ (!colr_ప్రధానసంఖ్య->serialize_V0 (c->serializer, సంస్కరణ, పునాది_it, layer_it)) return_trace (అబద్ధం);

    /* subset ItemVariationStore first, cause varidx_map needs to be updated
     * after instancing */
    ఒకవేళ (!subset_varstore (c, colr_ప్రధానసంఖ్య)) return_trace (అబద్ధం);

    అంశం_Var_Store_Instancer instancer (c->plan->normalized_coords ? get_var_store_ptr () : శూన్యసూచిక,
				     c->plan->normalized_coords ? get_delta_సమితి_సూచీ_map_ptr () : శూన్యసూచిక,
				     c->plan->normalized_coords.as_array ());

    ఒకవేళ (!colr_ప్రధానసంఖ్య->పునాది_Glyph_List_2.serialize_subset (c, పునాది_Glyph_List_2, ఇది, instancer))
      return_trace (అబద్ధం);

    colr_ప్రధానసంఖ్య->layerList.serialize_subset (c, layerList, ఇది, instancer);
    colr_ప్రధానసంఖ్య->clipList.serialize_subset (c, clipList, ఇది, instancer);

    return_trace (subset_delta_సమితి_సూచీ_map (c, colr_ప్రధానసంఖ్య));
  }

  స్థిరరాశి Paint *get_పునాది_glyph_paint (hb_codepoint_t glyph) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Glyph_List &baseglyph_paintrecords = ఇది+పునాది_Glyph_List_2;
    స్థిరరాశి పునాది_Glyph_Paint_రికార్డు* రికార్డు = get_పునాది_glyph_paintrecord (glyph);
    ఒకవేళ (రికార్డు)
    {
      స్థిరరాశి Paint &paint = &baseglyph_paintrecords+రికార్డు->paint;
      తిరిగి &paint;
    }
    లేకపోతే
      తిరిగి శూన్యసూచిక;
  }

#నిర్వచితం_కాకపోతే HB_NO_PAINT
  తార్కిక
  get_extents (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_glyph_extents_t *extents,
	       hb_colr_scratch_t &scratch) స్థిరరాశి
  {

    అంశం_Var_Store_Instancer instancer (get_var_store_ptr (),
                                     get_delta_సమితి_సూచీ_map_ptr (),
                                     hb_array (font->coords, font->అంకె_coords));

    ఒకవేళ (get_clip (glyph, extents, instancer))
    {
      font->scale_glyph_extents (extents);
      తిరిగి నిజం;
    }

    స్వయంచాలక *extents_funcs = hb_paint_extents_get_funcs ();
    scratch.paint_extents.clear ();
    తార్కిక ret = paint_glyph (font, glyph, extents_funcs, &scratch.paint_extents, 0, HB_COLOR(0,0,0,0), నిజం, scratch);

    స్వయంచాలక e = scratch.paint_extents.get_extents ();
    ఒకవేళ (e.is_void ())
    {
      extents->x_bearing = 0;
      extents->y_bearing = 0;
      extents->వెడల్పు = 0;
      extents->ఎత్తు = 0;
    }
    లేకపోతే
      *extents = e.గమ్యం_glyph_extents ();

    తిరిగి ret;
  }
#అయితే_ముగింపు

  తార్కిక
  has_paint_for_glyph (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ >= 1)
    {
      hb_barrier ();

      స్థిరరాశి Paint *paint = get_పునాది_glyph_paint (glyph);

      తిరిగి paint != శూన్యసూచిక;
    }

    తిరిగి అబద్ధం;
  }

  తార్కిక get_clip (hb_codepoint_t glyph,
		 hb_glyph_extents_t *extents,
		 స్థిరరాశి అంశం_Var_Store_Instancer instancer) స్థిరరాశి
  {
    తిరిగి get_clip_list ().get_extents (glyph,
					extents,
					instancer);
  }

#నిర్వచితం_కాకపోతే HB_NO_PAINT
  తార్కిక
  paint_glyph (hb_font_t *font,
	       hb_codepoint_t glyph,
	       hb_paint_funcs_t *funcs, శూన్య *దత్తాంశం,
	       గుర్తులేని పూర్ణాంకం palette_సూచీ, hb_color_t foreground,
	       తార్కిక clip,
	       hb_colr_scratch_t &scratch) స్థిరరాశి
  {
    అంశం_Var_Store_Instancer instancer (get_var_store_ptr (),
				     get_delta_సమితి_సూచీ_map_ptr (),
				     hb_array (font->coords,
					       font->has_nonzero_coords ? font->అంకె_coords : 0));
    hb_paint_context_t c (ఇది, funcs, దత్తాంశం, font, palette_సూచీ, foreground, instancer);

    hb_decycler_కణుపు_t node (c.glyphs_decycler);
    node.సందర్శించు (glyph);

    ఒకవేళ (సంస్కరణ >= 1)
    {
      hb_barrier ();

      స్థిరరాశి Paint *paint = get_పునాది_glyph_paint (glyph);
      ఒకవేళ (paint)
      {
        // COLRv1 glyph

	తార్కిక is_bounded = నిజం;
	hb_glyph_extents_t extents = {};
	ఒకవేళ (clip)
	{
	  ఒకవేళ (!get_clip (glyph, &extents, instancer))
	  {
	    clip = అబద్ధం;
	    is_bounded = అబద్ధం;
	  }

	  ఒకవేళ (!is_bounded)
	  {
	    స్వయంచాలక *bounded_funcs = hb_paint_bounded_get_funcs ();
	    scratch.paint_bounded.clear ();

	    paint_glyph (font, glyph,
			 bounded_funcs, &scratch.paint_bounded,
			 palette_సూచీ, foreground,
			 అబద్ధం,
			 scratch);

	    is_bounded = scratch.paint_bounded.is_bounded ();
	  }
	}

	c.funcs->నెట్టు_font_transform (c.దత్తాంశం, font);

	ఒకవేళ (clip)
	  c.funcs->నెట్టు_clip_rectangle (c.దత్తాంశం,
					(దశాంశం) extents.x_bearing,
					(దశాంశం) extents.y_bearing + extents.ఎత్తు,
					(దశాంశం) extents.x_bearing + extents.వెడల్పు,
					(దశాంశం) extents.y_bearing);

	ఒకవేళ (is_bounded)
	  c.recurse (*paint);

	ఒకవేళ (clip)
	  c.funcs->తీయి_clip (c.దత్తాంశం);

	c.funcs->తీయి_transform (c.దత్తాంశం);

        తిరిగి నిజం;
      }
    }

    స్థిరరాశి పునాది_Glyph_రికార్డు *రికార్డు = get_పునాది_glyph_రికార్డు (glyph);
    ఒకవేళ (రికార్డు && ((hb_codepoint_t) రికార్డు->glyphId == glyph))
    {
      // COLRv0 glyph
      కొరకు (స్థిరరాశి స్వయంచాలక &r : (ఇది+layersZ).as_array (అంకె_Layers)
			   .sub_array (రికార్డు->మొదటి_Layer_Idx, రికార్డు->అంకె_Layers))
      {
        hb_bool_t is_foreground;
        hb_color_t color = c.get_color (r.colorIdx, 1., &is_foreground);
        c.funcs->fill_glyph (c.దత్తాంశం, r.glyphId, c.font, is_foreground, color);
      }

      తిరిగి నిజం;
    }

    తిరిగి అబద్ధం;
  }
#అయితే_ముగింపు

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Table version number (starts at 0). */
  HBUINT16	అంకె_పునాది_Glyphs;	/* Number of Base Glyph Records. */
  NNOffset32_గమ్యం<Sorted_Unsized_Array_యొక్క<పునాది_Glyph_రికార్డు>>
		పునాది_Glyphs_Z;	/* Offset to Base Glyph records. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<Layer_రికార్డు>>
		layersZ;	/* Offset to Layer Records. */
  HBUINT16	అంకె_Layers;	/* Number of Layer Records. */
  // Version-1 additions
  Offset32_గమ్యం<పునాది_Glyph_List>		పునాది_Glyph_List_2;
  Offset32_గమ్యం<LayerList>			layerList;
  Offset32_గమ్యం<ClipList>			clipList;   // Offset to ClipList table (may be NULL)
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>		varIdxMap;  // Offset to DeltaSetIndexMap table (may be NULL)
  Offset32_గమ్యం<అంశం_Variation_Store>	varStore;
  బహిరంగ:
  DEFINE_కొలత_MIN (14);
};

నిర్మాణం COLR_accelerator_t : COLR::accelerator_t {
  COLR_accelerator_t (hb_face_t *face) : COLR::accelerator_t (face) {}
};

శూన్య
hb_paint_context_t::recurse (స్థిరరాశి Paint &paint)
{
  ఒకవేళ (unlikely (లోతు_ఎడమ <= 0 || అంచు_లెక్క <= 0)) తిరిగి;
  లోతు_ఎడమ--;
  అంచు_లెక్క--;
  paint.dispatch (ఇది);
  లోతు_ఎడమ++;
}

మూస <రకం_పేరు Types>
శూన్య PaintGlyph<Types>::paint_glyph (hb_paint_context_t *c) స్థిరరాశి
{
  TRACE_PAINT (ఇది);

  hb_bool_t is_foreground;
  hb_color_t color;
  /* Optimize cases that can use a simple fill-glyph operation. */
  ఒకవేళ (likely (c->లోతు_ఎడమ > 0 && c->అంచు_లెక్క > 0) &&
      (ఇది+paint).get_solid_color (c, &is_foreground, &color))
  {
    c->అంచు_లెక్క--;
    c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
    c->funcs->fill_glyph (c->దత్తాంశం, gid, c->font, is_foreground, color);
    c->funcs->తీయి_transform (c->దత్తాంశం);
    తిరిగి;
  }

  c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
  c->funcs->నెట్టు_clip_glyph (c->దత్తాంశం, gid, c->font);
  c->funcs->నెట్టు_font_transform (c->దత్తాంశం, c->font);
  c->recurse (ఇది+paint);
  c->funcs->తీయి_transform (c->దత్తాంశం);
  c->funcs->తీయి_clip (c->దత్తాంశం);
  c->funcs->తీయి_transform (c->దత్తాంశం);
}

శూన్య PaintColrLayers::paint_glyph (hb_paint_context_t *c) స్థిరరాశి
{
  TRACE_PAINT (ఇది);
  స్థిరరాశి LayerList &paint_స్థానభ్రంశం_lists = c->get_colr_పట్టిక ()->get_layerList ();
  hb_decycler_కణుపు_t node (c->layers_decycler);
  కొరకు (గుర్తులేని i = మొదటి_Layer_సూచీ; i < మొదటి_Layer_సూచీ + అంకె_Layers; i++)
  {
    ఒకవేళ (unlikely (!node.సందర్శించు (i)))
      తిరిగి;

    స్థిరరాశి Paint &paint = paint_స్థానభ్రంశం_lists.get_paint (i);
    c->recurse (paint);
  }
}

శూన్య PaintColrGlyph::paint_glyph (hb_paint_context_t *c) స్థిరరాశి
{
  TRACE_PAINT (ఇది);

  hb_decycler_కణుపు_t node (c->glyphs_decycler);
  ఒకవేళ (unlikely (!node.సందర్శించు (gid)))
    తిరిగి;

  c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
  ఒకవేళ (c->funcs->color_glyph (c->దత్తాంశం, gid, c->font))
  {
    c->funcs->తీయి_transform (c->దత్తాంశం);
    తిరిగి;
  }
  c->funcs->తీయి_transform (c->దత్తాంశం);

  స్థిరరాశి COLR *colr_పట్టిక = c->get_colr_పట్టిక ();
  స్థిరరాశి Paint *paint = colr_పట్టిక->get_పునాది_glyph_paint (gid);

  hb_glyph_extents_t extents = {0};
  తార్కిక has_clip_box = colr_పట్టిక->get_clip (gid, &extents, c->instancer);

  ఒకవేళ (has_clip_box)
    c->funcs->నెట్టు_clip_rectangle (c->దత్తాంశం,
				   (దశాంశం) extents.x_bearing,
				   (దశాంశం) extents.y_bearing + extents.ఎత్తు,
				   (దశాంశం) extents.x_bearing + extents.వెడల్పు,
				   (దశాంశం) extents.y_bearing);

  ఒకవేళ (paint)
    c->recurse (*paint);

  ఒకవేళ (has_clip_box)
    c->funcs->తీయి_clip (c->దత్తాంశం);
}

} /* namespace OT */

#అయితే_ముగింపు /* OT_COLOR_COLR_COLR_HH */
