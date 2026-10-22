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

#اگر_غیر_تعریف_شدہ OT_COLOR_COLR_COLRV1_احاطہ_HH
#تعریف_کرو OT_COLOR_COLR_COLRV1_احاطہ_HH

#شامل "../../../hb-open-type.hh"
#شامل "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نامخلا OT {

HB_اندرونی خالی PaintColrLayers::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  c->جوڑو_layer_indices (پہلا_Layer_اشاریہ, عدد_Layers);
  ثابت LayerList &paint_انحراف_lists = c->get_colr_جدول ()->get_layerList ();
  برائے (غیرعلامتی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    ثابت Paint &paint = paint_انحراف_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچہ <قسم_نام Types>
HB_اندرونی خالی PaintGlyph<Types>::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  c->جوڑو_glyph (gid);
  (یہ+paint).dispatch (c);
}

HB_اندرونی خالی PaintColrGlyph::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت بنیاد_Glyph_Paint_ریکارڈ* baseglyph_paintrecord = colr_جدول->get_بنیاد_glyph_paintrecord (gid);
  اگر (!baseglyph_paintrecord) واپس;
  c->جوڑو_glyph (gid);

  ثابت بنیاد_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچہ <سانچہ<قسم_نام> جماعت Var>
HB_اندرونی خالی PaintTransform<Var>::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  (یہ+transform).closurev1 (c);
}

HB_اندرونی خالی PaintTranslate::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی خالی PaintScale::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی خالی PaintScaleAroundCenter::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_اندرونی خالی Paint_Scale_یکساں::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_اندرونی خالی Paint_Scale_یکساں_Around_Center::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_اندرونی خالی PaintRotate::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 1;
}

HB_اندرونی خالی PaintRotateAroundCenter::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 3;
}

HB_اندرونی خالی PaintSkew::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 2;
}

HB_اندرونی خالی PaintSkewAroundCenter::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  c->عدد_var_idxes = 4;
}

HB_اندرونی خالی Paint_مرکب::closurev1 (hb_colrv1_احاطہ_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  (یہ+backdrop).dispatch (c);
}

} /* namespace OT */


#اگر_ختم /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
