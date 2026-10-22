/*
 * Copyright © 2025  Skef Iterum
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

#اگر_غیر_تعریف_شدہ OT_COLOR_COLR_COLRV1_DEPEND_HH
#تعریف_کرو OT_COLOR_COLR_COLRV1_DEPEND_HH

#شامل "../../../hb-open-type.hh"
#شامل "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نامخلا OT {


HB_اندرونی خالی PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت LayerList &paint_انحراف_lists = c->get_colr_جدول ()->get_layerList ();
  برائے (غیرعلامتی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    ثابت Paint &paint = paint_انحراف_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچہ <قسم_نام Types>
HB_اندرونی خالی PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  اگر (gid != c->ماخذ_gid)
    c->depend_ڈیٹا->جوڑو_depend(c->ماخذ_gid, HB_OT_لیبل_COLR, gid);
  (یہ+paint).dispatch (c);
}

HB_اندرونی خالی PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت بنیاد_Glyph_Paint_ریکارڈ* baseglyph_paintrecord = colr_جدول->get_بنیاد_glyph_paintrecord (gid);
  اگر (!baseglyph_paintrecord) واپس;
  c->depend_ڈیٹا->جوڑو_depend(c->ماخذ_gid, HB_OT_لیبل_COLR, gid);

  ثابت بنیاد_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچہ <سانچہ<قسم_نام> جماعت Var>
HB_اندرونی خالی PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی Paint_Scale_یکساں::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی Paint_Scale_یکساں_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
}

HB_اندرونی خالی Paint_مرکب::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (یہ+src).dispatch (c);
  (یہ+backdrop).dispatch (c);
}


} /* namespace OT */


#اگر_ختم /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
