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

#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
ନାମାକାଶ OT {

HB_INTERNAL ଶୂନ୍ୟ PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  c->add_layer_indices (firstLayerIndex, ଅଙ୍କ_Layers);
  ଅଚଳ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  ପାଇଁ (unsigned i = firstLayerIndex; i < firstLayerIndex + ଅଙ୍କ_Layers; i++)
  {
    ଅଚଳ Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL ଶୂନ୍ୟ PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  c->add_glyph (gid);
  (ଏହି+paint).dispatch (c);
}

HB_INTERNAL ଶୂନ୍ୟ PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  ଅଚଳ COLR *colr_table = c->get_colr_table ();
  ଅଚଳ BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  ଯଦି (!baseglyph_paintrecord) ଫେରନ୍ତୁ;
  c->add_glyph (gid);

  ଅଚଳ BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> ଶ୍ରେଣୀ Var>
HB_INTERNAL ଶୂନ୍ୟ PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  (ଏହି+transform).closurev1 (c);
}

HB_INTERNAL ଶୂନ୍ୟ PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 2;
}

HB_INTERNAL ଶୂନ୍ୟ PaintScale::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 2;
}

HB_INTERNAL ଶୂନ୍ୟ PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 4;
}

HB_INTERNAL ଶୂନ୍ୟ PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 1;
}

HB_INTERNAL ଶୂନ୍ୟ PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 3;
}

HB_INTERNAL ଶୂନ୍ୟ PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 1;
}

HB_INTERNAL ଶୂନ୍ୟ PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 3;
}

HB_INTERNAL ଶୂନ୍ୟ PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 2;
}

HB_INTERNAL ଶୂନ୍ୟ PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  c->ଅଙ୍କ_var_idxes = 4;
}

HB_INTERNAL ଶୂନ୍ୟ PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) ଅଚଳ
{
  (ଏହି+src).dispatch (c);
  (ଏହି+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
