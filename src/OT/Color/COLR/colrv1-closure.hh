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

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {

HB_INTERNAL فراغ PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  c->add_layer_indices (firstLayerIndex, رقم_Layers);
  ثابت LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  لكل (unsigned i = firstLayerIndex; i < firstLayerIndex + رقم_Layers; i++)
  {
    ثابت Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL فراغ PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  c->add_glyph (gid);
  (هذا+paint).dispatch (c);
}

HB_INTERNAL فراغ PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  ثابت COLR *colr_table = c->get_colr_table ();
  ثابت BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) إرجاع;
  c->add_glyph (gid);

  ثابت BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> صنف Var>
HB_INTERNAL فراغ PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+transform).closurev1 (c);
}

HB_INTERNAL فراغ PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_INTERNAL فراغ PaintScale::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_INTERNAL فراغ PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 4;
}

HB_INTERNAL فراغ PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 1;
}

HB_INTERNAL فراغ PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 3;
}

HB_INTERNAL فراغ PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 1;
}

HB_INTERNAL فراغ PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 3;
}

HB_INTERNAL فراغ PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_INTERNAL فراغ PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 4;
}

HB_INTERNAL فراغ PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
