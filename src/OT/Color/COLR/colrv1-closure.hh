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

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
நாமாகாஶ OT {

HB_INTERNAL வெறுமை PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  c->add_layer_indices (firstLayerIndex, இலக்கம்_Layers);
  மாறிலி LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  க்கு (unsigned i = firstLayerIndex; i < firstLayerIndex + இலக்கம்_Layers; i++)
  {
    மாறிலி Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL வெறுமை PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  c->add_glyph (gid);
  (இது+paint).dispatch (c);
}

HB_INTERNAL வெறுமை PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  மாறிலி COLR *colr_table = c->get_colr_table ();
  மாறிலி BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  எனில் (!baseglyph_paintrecord) திரும்பு;
  c->add_glyph (gid);

  மாறிலி BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> வகுப்பு Var>
HB_INTERNAL வெறுமை PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  (இது+transform).closurev1 (c);
}

HB_INTERNAL வெறுமை PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 2;
}

HB_INTERNAL வெறுமை PaintScale::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 2;
}

HB_INTERNAL வெறுமை PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 4;
}

HB_INTERNAL வெறுமை PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 1;
}

HB_INTERNAL வெறுமை PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 3;
}

HB_INTERNAL வெறுமை PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 1;
}

HB_INTERNAL வெறுமை PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 3;
}

HB_INTERNAL வெறுமை PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 2;
}

HB_INTERNAL வெறுமை PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  c->இலக்கம்_var_idxes = 4;
}

HB_INTERNAL வெறுமை PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  (இது+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
