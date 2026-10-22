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

#که_نه_تعریف_شوی OT_COLOR_COLR_COLRV1_تړون_HH
#تعریف_کړه OT_COLOR_COLR_COLRV1_تړون_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {

HB_دننی فراغ PaintColrLayers::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  c->جمع_کړه_layer_indices (لومړی_Layer_شاخص, عدد_Layers);
  ثابت LayerList &paint_دننه_واټن_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بینښه i = لومړی_Layer_شاخص; i < لومړی_Layer_شاخص + عدد_Layers; i++)
  {
    ثابت Paint &paint = paint_دننه_واټن_lists.get_paint (i);
    paint.dispatch (c);
  }
}

کينډۍ <ډول_نوم Types>
HB_دننی فراغ PaintGlyph<Types>::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  c->جمع_کړه_glyph (gid);
  (هذا+paint).dispatch (c);
}

HB_دننی فراغ PaintColrGlyph::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت بنسټ_Glyph_Paint_ریکارډ* baseglyph_paintrecord = colr_جدول->get_بنسټ_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->جمع_کړه_glyph (gid);

  ثابت بنسټ_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

کينډۍ <کينډۍ<ډول_نوم> صنف Var>
HB_دننی فراغ PaintTransform<Var>::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+transform).closurev1 (c);
}

HB_دننی فراغ PaintTranslate::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_دننی فراغ PaintScale::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_دننی فراغ PaintScaleAroundCenter::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_دننی فراغ Paint_Scale_یوشان::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_دننی فراغ Paint_Scale_یوشان_Around_Center::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_دننی فراغ PaintRotate::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_دننی فراغ PaintRotateAroundCenter::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_دننی فراغ PaintSkew::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_دننی فراغ PaintSkewAroundCenter::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_دننی فراغ Paint_مرکب::closurev1 (hb_colrv1_تړون_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}

} /* namespace OT */


#د_که_پای /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
