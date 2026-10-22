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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_COLR_COLRV1_DEPEND_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_COLR_COLRV1_DEPEND_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
ਨਾਮਾਕਾਸ਼ OT {


HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  ਅਚਲ LayerList &paint_ਵਿੱਥ_lists = c->get_colr_ਸਾਰਣੀ ()->get_layerList ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ; i < ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ + ਅੰਕ_Layers; i++)
  {
    ਅਚਲ Paint &paint = paint_ਵਿੱਥ_lists.get_paint (i);
    paint.dispatch (c);
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  ਜੇ (gid != c->ਸਰੋਤ_gid)
    c->depend_ਅੰਕੜੇ->ਜੋੜੋ_depend(c->ਸਰੋਤ_gid, HB_OT_ਟੈਗ_COLR, gid);
  (ਇਹ+paint).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  ਅਚਲ COLR *colr_ਸਾਰਣੀ = c->get_colr_ਸਾਰਣੀ ();
  ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ* baseglyph_paintrecord = colr_ਸਾਰਣੀ->get_ਅਧਾਰ_glyph_paintrecord (gid);
  ਜੇ (!baseglyph_paintrecord) ਵਾਪਸ;
  c->depend_ਅੰਕੜੇ->ਜੋੜੋ_depend(c->ਸਰੋਤ_gid, HB_OT_ਟੈਗ_COLR, gid);

  ਅਚਲ ਅਧਾਰ_Glyph_List &baseglyph_list = colr_ਸਾਰਣੀ->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_Scale_ਇਕਸਾਰ::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_Scale_ਇਕਸਾਰ_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_ਸੰਯੁਕਤ::dependv1 (hb_colrv1_depend_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  (ਇਹ+backdrop).dispatch (c);
}


} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
