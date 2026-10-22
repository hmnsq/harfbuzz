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
 */

#ifndef OT_COLOR_COLR_COLRV1_CLOSURE_HH
#define OT_COLOR_COLR_COLRV1_CLOSURE_HH

#അന്തര്ഭാവ "../../../hb-open-type.hh"
#അന്തര്ഭാവ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
നാമാകാശ OT {

HB_INTERNAL ശൂന്യം PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  c->add_layer_indices (firstLayerIndex, അക്കം_Layers);
  സ്ഥിരം LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  വേണ്ടി (unsigned i = firstLayerIndex; i < firstLayerIndex + അക്കം_Layers; i++)
  {
    സ്ഥിരം Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL ശൂന്യം PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  c->add_glyph (gid);
  (ഇത്+paint).dispatch (c);
}

HB_INTERNAL ശൂന്യം PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  സ്ഥിരം COLR *colr_table = c->get_colr_table ();
  സ്ഥിരം BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  എങ്കിൽ (!baseglyph_paintrecord) തിരികെ;
  c->add_glyph (gid);

  സ്ഥിരം BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> വർഗ്ഗം Var>
HB_INTERNAL ശൂന്യം PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  (ഇത്+transform).closurev1 (c);
}

HB_INTERNAL ശൂന്യം PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 2;
}

HB_INTERNAL ശൂന്യം PaintScale::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 2;
}

HB_INTERNAL ശൂന്യം PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 4;
}

HB_INTERNAL ശൂന്യം PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 1;
}

HB_INTERNAL ശൂന്യം PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 3;
}

HB_INTERNAL ശൂന്യം PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 1;
}

HB_INTERNAL ശൂന്യം PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 3;
}

HB_INTERNAL ശൂന്യം PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 2;
}

HB_INTERNAL ശൂന്യം PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  c->അക്കം_var_idxes = 4;
}

HB_INTERNAL ശൂന്യം PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) സ്ഥിരം
{
  (ഇത്+src).dispatch (c);
  (ഇത്+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
