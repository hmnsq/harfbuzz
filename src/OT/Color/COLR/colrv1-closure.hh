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

#অন্তর্ভাৱ "../../../hb-open-type.hh"
#অন্তর্ভাৱ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
নামাকাশ OT {

HB_INTERNAL শূন্য PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  c->add_layer_indices (firstLayerIndex, অঙ্ক_Layers);
  ধ্রুৱ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  বাবে (unsigned i = firstLayerIndex; i < firstLayerIndex + অঙ্ক_Layers; i++)
  {
    ধ্রুৱ Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL শূন্য PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  c->add_glyph (gid);
  (এতত্+paint).dispatch (c);
}

HB_INTERNAL শূন্য PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  ধ্রুৱ COLR *colr_table = c->get_colr_table ();
  ধ্রুৱ BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  যদি (!baseglyph_paintrecord) ঘূৰাই;
  c->add_glyph (gid);

  ধ্রুৱ BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> শ্ৰেণী Var>
HB_INTERNAL শূন্য PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  (এতত্+transform).closurev1 (c);
}

HB_INTERNAL শূন্য PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 2;
}

HB_INTERNAL শূন্য PaintScale::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 2;
}

HB_INTERNAL শূন্য PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 4;
}

HB_INTERNAL শূন্য PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 1;
}

HB_INTERNAL শূন্য PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 3;
}

HB_INTERNAL শূন্য PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 1;
}

HB_INTERNAL শূন্য PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 3;
}

HB_INTERNAL শূন্য PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 2;
}

HB_INTERNAL শূন্য PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  c->অঙ্ক_var_idxes = 4;
}

HB_INTERNAL শূন্য PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) ধ্রুৱ
{
  (এতত্+src).dispatch (c);
  (এতত্+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
