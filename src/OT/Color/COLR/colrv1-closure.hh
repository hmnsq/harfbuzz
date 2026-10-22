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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_COLR_COLRV1_ਘੇਰਾ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_COLR_COLRV1_ਘੇਰਾ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
ਨਾਮਾਕਾਸ਼ OT {

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintColrLayers::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  c->ਜੋੜੋ_layer_indices (ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ, ਅੰਕ_Layers);
  ਅਚਲ LayerList &paint_ਵਿੱਥ_lists = c->get_colr_ਸਾਰਣੀ ()->get_layerList ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ; i < ਪਹਿਲਾ_Layer_ਸੂਚਕਾਂਕ + ਅੰਕ_Layers; i++)
  {
    ਅਚਲ Paint &paint = paint_ਵਿੱਥ_lists.get_paint (i);
    paint.dispatch (c);
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintGlyph<Types>::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  c->ਜੋੜੋ_glyph (gid);
  (ਇਹ+paint).dispatch (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintColrGlyph::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  ਅਚਲ COLR *colr_ਸਾਰਣੀ = c->get_colr_ਸਾਰਣੀ ();
  ਅਚਲ ਅਧਾਰ_Glyph_Paint_ਰਿਕਾਰਡ* baseglyph_paintrecord = colr_ਸਾਰਣੀ->get_ਅਧਾਰ_glyph_paintrecord (gid);
  ਜੇ (!baseglyph_paintrecord) ਵਾਪਸ;
  c->ਜੋੜੋ_glyph (gid);

  ਅਚਲ ਅਧਾਰ_Glyph_List &baseglyph_list = colr_ਸਾਰਣੀ->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

ਸਾਂਚਾ <ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ> ਵਰਗ Var>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintTransform<Var>::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  (ਇਹ+transform).closurev1 (c);
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintTranslate::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 2;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintScale::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 2;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintScaleAroundCenter::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 4;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_Scale_ਇਕਸਾਰ::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 1;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_Scale_ਇਕਸਾਰ_Around_Center::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 3;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintRotate::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 1;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintRotateAroundCenter::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 3;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintSkew::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 2;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ PaintSkewAroundCenter::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  c->ਅੰਕ_var_idxes = 4;
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ Paint_ਸੰਯੁਕਤ::closurev1 (hb_colrv1_ਘੇਰਾ_context_t* c) ਅਚਲ
{
  (ਇਹ+src).dispatch (c);
  (ਇਹ+backdrop).dispatch (c);
}

} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
