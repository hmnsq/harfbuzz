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

#अगर_अघोषित OT_COLOR_COLR_COLRV1_CLOSURE_HH
#घोषणा OT_COLOR_COLR_COLRV1_CLOSURE_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
namespace OT {

HB_INTERNAL व्योम PaintColrLayers::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  c->add_layer_indices (first_Layer_सूचकांक, अंक_Layers);
  स्थिर LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  क्रम (अचिन्हित i = first_Layer_सूचकांक; i < first_Layer_सूचकांक + अंक_Layers; i++)
  {
    स्थिर Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

ढाँचा <typename Types>
HB_INTERNAL व्योम PaintGlyph<Types>::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  c->add_glyph (gid);
  (यह+paint).dispatch (c);
}

HB_INTERNAL व्योम PaintColrGlyph::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  स्थिर COLR *colr_table = c->get_colr_table ();
  स्थिर BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  अगर (!baseglyph_paintrecord) वापस;
  c->add_glyph (gid);

  स्थिर BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

ढाँचा <ढाँचा<typename> श्रेणी Var>
HB_INTERNAL व्योम PaintTransform<Var>::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  (यह+transform).closurev1 (c);
}

HB_INTERNAL व्योम PaintTranslate::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL व्योम PaintScale::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL व्योम PaintScaleAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 4;
}

HB_INTERNAL व्योम PaintScaleUniform::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 1;
}

HB_INTERNAL व्योम PaintScaleUniformAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 3;
}

HB_INTERNAL व्योम PaintRotate::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 1;
}

HB_INTERNAL व्योम PaintRotateAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 3;
}

HB_INTERNAL व्योम PaintSkew::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 2;
}

HB_INTERNAL व्योम PaintSkewAroundCenter::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  c->अंक_var_idxes = 4;
}

HB_INTERNAL व्योम PaintComposite::closurev1 (hb_colrv1_closure_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  (यह+backdrop).dispatch (c);
}

} /* namespace OT */


#पूर्ण_अगर /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
