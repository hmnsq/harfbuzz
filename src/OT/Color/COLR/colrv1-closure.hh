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

#اگر_تعریف_نشده OT_COLOR_COLR_COLRV1_بستگی_HH
#تعریف_کن OT_COLOR_COLR_COLRV1_بستگی_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {

HB_داخلی فراغ PaintColrLayers::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  c->جمع_کن_layer_indices (اولی_Layer_شاخص, رقم_Layers);
  ثابت LayerList &paint_جابه_جایی_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بیعلامت i = اولی_Layer_شاخص; i < اولی_Layer_شاخص + رقم_Layers; i++)
  {
    ثابت Paint &paint = paint_جابه_جایی_lists.get_paint (i);
    paint.dispatch (c);
  }
}

قالب <نام_نوع Types>
HB_داخلی فراغ PaintGlyph<Types>::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  c->جمع_کن_glyph (gid);
  (هذا+paint).dispatch (c);
}

HB_داخلی فراغ PaintColrGlyph::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت پایه_Glyph_Paint_رکورد* baseglyph_paintrecord = colr_جدول->get_پایه_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->جمع_کن_glyph (gid);

  ثابت پایه_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

قالب <قالب<نام_نوع> صنف Var>
HB_داخلی فراغ PaintTransform<Var>::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+transform).closurev1 (c);
}

HB_داخلی فراغ PaintTranslate::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_داخلی فراغ PaintScale::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_داخلی فراغ PaintScaleAroundCenter::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 4;
}

HB_داخلی فراغ Paint_Scale_یکنواخت::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 1;
}

HB_داخلی فراغ Paint_Scale_یکنواخت_Around_Center::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 3;
}

HB_داخلی فراغ PaintRotate::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 1;
}

HB_داخلی فراغ PaintRotateAroundCenter::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 3;
}

HB_داخلی فراغ PaintSkew::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 2;
}

HB_داخلی فراغ PaintSkewAroundCenter::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->رقم_var_idxes = 4;
}

HB_داخلی فراغ Paint_مرکب::closurev1 (hb_colrv1_بستگی_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}

} /* namespace OT */


#پایان_اگر /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
