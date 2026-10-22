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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
नामाकाश OT {

HB_INTERNAL शून्य PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  c->add_layer_indices (firstLayerIndex, अंक_Layers);
  ध्रुव LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  प्रति (अचिन्हित i = firstLayerIndex; i < firstLayerIndex + अंक_Layers; i++)
  {
    ध्रुव Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

ढाँचा <typename Types>
HB_INTERNAL शून्य PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  c->add_glyph (gid);
  (एतत्+paint).dispatch (c);
}

HB_INTERNAL शून्य PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  ध्रुव COLR *colr_table = c->get_colr_table ();
  ध्रुव BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  यदि (!baseglyph_paintrecord) प्रत्यागम;
  c->add_glyph (gid);

  ध्रुव BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

ढाँचा <ढाँचा<typename> वर्ग Var>
HB_INTERNAL शून्य PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  (एतत्+transform).closurev1 (c);
}

HB_INTERNAL शून्य PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL शून्य PaintScale::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL शून्य PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 4;
}

HB_INTERNAL शून्य PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 1;
}

HB_INTERNAL शून्य PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 3;
}

HB_INTERNAL शून्य PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 1;
}

HB_INTERNAL शून्य PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 3;
}

HB_INTERNAL शून्य PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL शून्य PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  c->अंक_var_idxes = 4;
}

HB_INTERNAL शून्य PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  (एतत्+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
