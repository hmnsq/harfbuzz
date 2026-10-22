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

#جے_بنا_تعریف OT_COLOR_COLR_COLRV1_گھیرا_HH
#تعریف_کرو OT_COLOR_COLR_COLRV1_گھیرا_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {

HB_اندرونی فراغ PaintColrLayers::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  c->جوڑو_layer_indices (پہلا_Layer_اشاریہ, عدد_Layers);
  ثابت LayerList &paint_وتھ_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بنانشانی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    ثابت Paint &paint = paint_وتھ_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچا <قسم_ناں Types>
HB_اندرونی فراغ PaintGlyph<Types>::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  c->جوڑو_glyph (gid);
  (هذا+paint).dispatch (c);
}

HB_اندرونی فراغ PaintColrGlyph::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت ادھار_Glyph_Paint_ریکارڈ* baseglyph_paintrecord = colr_جدول->get_ادھار_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->جوڑو_glyph (gid);

  ثابت ادھار_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچا <سانچا<قسم_ناں> صنف Var>
HB_اندرونی فراغ PaintTransform<Var>::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+transform).closurev1 (c);
}

HB_اندرونی فراغ PaintTranslate::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی فراغ PaintScale::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی فراغ PaintScaleAroundCenter::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_اندرونی فراغ Paint_Scale_اکسار::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_اندرونی فراغ Paint_Scale_اکسار_Around_Center::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_اندرونی فراغ PaintRotate::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_اندرونی فراغ PaintRotateAroundCenter::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_اندرونی فراغ PaintSkew::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی فراغ PaintSkewAroundCenter::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_اندرونی فراغ Paint_مرکب::closurev1 (hb_colrv1_گھیرا_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}

} /* namespace OT */


#جے_انت /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
