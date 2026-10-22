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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_COLR_COLRV1_DEPEND_HH
#وصف_ڪريو OT_COLOR_COLR_COLRV1_DEPEND_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
نطاق OT {


HB_اندروني فراغ PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت LayerList &paint_وٿي_lists = c->get_colr_جدول ()->get_layerList ();
  لكل (بنانشان i = پهريون_Layer_اشاريو; i < پهريون_Layer_اشاريو + انگ_Layers; i++)
  {
    ثابت Paint &paint = paint_وٿي_lists.get_paint (i);
    paint.dispatch (c);
  }
}

سانچو <قسم_نالو Types>
HB_اندروني فراغ PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  إذا (gid != c->ذريعو_gid)
    c->depend_معلومات->جوڙيو_depend(c->ذريعو_gid, HB_OT_ٽيگ_COLR, gid);
  (هذا+paint).dispatch (c);
}

HB_اندروني فراغ PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  ثابت COLR *colr_جدول = c->get_colr_جدول ();
  ثابت بنياد_Glyph_Paint_رڪارڊ* baseglyph_paintrecord = colr_جدول->get_بنياد_glyph_paintrecord (gid);
  إذا (!baseglyph_paintrecord) أرجع;
  c->depend_معلومات->جوڙيو_depend(c->ذريعو_gid, HB_OT_ٽيگ_COLR, gid);

  ثابت بنياد_Glyph_List &baseglyph_list = colr_جدول->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

سانچو <سانچو<قسم_نالو> صنف Var>
HB_اندروني فراغ PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ Paint_Scale_هڪجهڙو::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ Paint_Scale_هڪجهڙو_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
}

HB_اندروني فراغ Paint_مرکب::dependv1 (hb_colrv1_depend_context_t* c) ثابت
{
  (هذا+src).dispatch (c);
  (هذا+backdrop).dispatch (c);
}


} /* namespace OT */


#جيڪڏهن_پورو /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
