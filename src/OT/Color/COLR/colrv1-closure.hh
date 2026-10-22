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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_COLR_COLRV1_گهيرو_HH
#وصف_ڪريو OT_COLOR_COLR_COLRV1_گهيرو_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {

HB_اندروني فراغ PaintColrLayers::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  c->جوڙيو_layer_indices (پهريون_Layer_اشاريو, انگ_Layers);
  ثابت LayerList &paint_وٿي_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بنانشان i = پهريون_Layer_اشاريو; i < پهريون_Layer_اشاريو + انگ_Layers; i++)
  {
    ثابت Paint &paint = paint_وٿي_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچو <قسم_نالو Types>
HB_اندروني فراغ PaintGlyph<Types>::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  c->جوڙيو_glyph (gid);
  (هذا+paint).dispatch (c);
}

HB_اندروني فراغ PaintColrGlyph::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت بنياد_Glyph_Paint_رڪارڊ* baseglyph_paintrecord = colr_جدول->get_بنياد_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->جوڙيو_glyph (gid);

  ثابت بنياد_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچو <سانچو<قسم_نالو> صنف Var>
HB_اندروني فراغ PaintTransform<Var>::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+transform).closurev1 (c);
}

HB_اندروني فراغ PaintTranslate::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 2;
}

HB_اندروني فراغ PaintScale::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 2;
}

HB_اندروني فراغ PaintScaleAroundCenter::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 4;
}

HB_اندروني فراغ Paint_Scale_هڪجهڙو::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 1;
}

HB_اندروني فراغ Paint_Scale_هڪجهڙو_Around_Center::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 3;
}

HB_اندروني فراغ PaintRotate::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 1;
}

HB_اندروني فراغ PaintRotateAroundCenter::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 3;
}

HB_اندروني فراغ PaintSkew::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 2;
}

HB_اندروني فراغ PaintSkewAroundCenter::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->انگ_var_idxes = 4;
}

HB_اندروني فراغ Paint_مرکب::closurev1 (hb_colrv1_گهيرو_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}

} /* namespace OT */


#جيڪڏهن_پورو /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
