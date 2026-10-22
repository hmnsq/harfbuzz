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

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
ನಾಮಾಕಾಶ OT {

HB_INTERNAL ಶೂನ್ಯ PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  c->add_layer_indices (firstLayerIndex, ಅಂಕೆ_Layers);
  ಧ್ರುವ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  ಪ್ರತಿ (unsigned i = firstLayerIndex; i < firstLayerIndex + ಅಂಕೆ_Layers; i++)
  {
    ಧ್ರುವ Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL ಶೂನ್ಯ PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  c->add_glyph (gid);
  (ಏತತ್+paint).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  ಧ್ರುವ COLR *colr_table = c->get_colr_table ();
  ಧ್ರುವ BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  ಯದಿ (!baseglyph_paintrecord) ಪ್ರತ್ಯಾಗಮ;
  c->add_glyph (gid);

  ಧ್ರುವ BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> ವರ್ಗ Var>
HB_INTERNAL ಶೂನ್ಯ PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  (ಏತತ್+transform).closurev1 (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 2;
}

HB_INTERNAL ಶೂನ್ಯ PaintScale::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 2;
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 4;
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 1;
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 3;
}

HB_INTERNAL ಶೂನ್ಯ PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 1;
}

HB_INTERNAL ಶೂನ್ಯ PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 3;
}

HB_INTERNAL ಶೂನ್ಯ PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 2;
}

HB_INTERNAL ಶೂನ್ಯ PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  c->ಅಂಕೆ_var_idxes = 4;
}

HB_INTERNAL ಶೂನ್ಯ PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  (ಏತತ್+backdrop).dispatch (c);
}

} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
