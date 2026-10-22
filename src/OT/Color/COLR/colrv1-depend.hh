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

#اگر_تعریف_نشده OT_COLOR_COLR_COLRV1_DEPEND_HH
#تعریف_کن OT_COLOR_COLR_COLRV1_DEPEND_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {


HB_داخلی فراغ PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت LayerList &paint_جابه_جایی_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بیعلامت i = اولی_Layer_شاخص; i < اولی_Layer_شاخص + رقم_Layers; i++)
  {
    ثابت Paint &paint = paint_جابه_جایی_lists.get_paint (i);
    paint.dispatch (c);
  }
}

قالب <نام_نوع Types>
HB_داخلی فراغ PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  إذا (gid != c->منبع_gid)
    c->depend_داده->جمع_کن_depend(c->منبع_gid, HB_OT_برچسب_COLR, gid);
  (هذا+paint).dispatch (c);
}

HB_داخلی فراغ PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت پایه_Glyph_Paint_رکورد* baseglyph_paintrecord = colr_جدول->get_پایه_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->depend_داده->جمع_کن_depend(c->منبع_gid, HB_OT_برچسب_COLR, gid);

  ثابت پایه_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

قالب <قالب<نام_نوع> صنف Var>
HB_داخلی فراغ PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ Paint_Scale_یکنواخت::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ Paint_Scale_یکنواخت_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_داخلی فراغ Paint_مرکب::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}


} /* namespace OT */


#پایان_اگر /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
