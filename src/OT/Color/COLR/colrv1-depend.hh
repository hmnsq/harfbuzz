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

#جے_بنا_تعریف OT_COLOR_COLR_COLRV1_DEPEND_HH
#تعریف_کرو OT_COLOR_COLR_COLRV1_DEPEND_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {


HB_اندرونی فراغ PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت LayerList &paint_وتھ_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بنانشانی i = پہلا_Layer_اشاریہ; i < پہلا_Layer_اشاریہ + عدد_Layers; i++)
  {
    ثابت Paint &paint = paint_وتھ_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچا <قسم_ناں Types>
HB_اندرونی فراغ PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  إذا (gid != c->سروت_gid)
    c->depend_انکڑے->جوڑو_depend(c->سروت_gid, HB_OT_ٹیگ_COLR, gid);
  (هذا+paint).dispatch (c);
}

HB_اندرونی فراغ PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت ادھار_Glyph_Paint_ریکارڈ* baseglyph_paintrecord = colr_جدول->get_ادھار_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->depend_انکڑے->جوڑو_depend(c->سروت_gid, HB_OT_ٹیگ_COLR, gid);

  ثابت ادھار_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچا <سانچا<قسم_ناں> صنف Var>
HB_اندرونی فراغ PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ Paint_Scale_اکسار::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ Paint_Scale_اکسار_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندرونی فراغ Paint_مرکب::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}


} /* namespace OT */


#جے_انت /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
