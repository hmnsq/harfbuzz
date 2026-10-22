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

#నిర్వచితం_కాకపోతే OT_COLOR_COLR_COLRV1_సంవృతం_HH
#నిర్వచించు OT_COLOR_COLR_COLRV1_సంవృతం_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
నామాకాశ OT {

HB_అంతర్గత శూన్య PaintColrLayers::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  c->కూడు_layer_indices (మొదటి_Layer_సూచీ, అంకె_Layers);
  స్థిరరాశి LayerList &paint_స్థానభ్రంశం_lists = c->get_colr_పట్టిక ()->get_layerList ();
  కొరకు (గుర్తులేని i = మొదటి_Layer_సూచీ; i < మొదటి_Layer_సూచీ + అంకె_Layers; i++)
  {
    స్థిరరాశి Paint &paint = paint_స్థానభ్రంశం_lists.get_paint (i);
    paint.dispatch (c);
  }
}

మూస <రకం_పేరు Types>
HB_అంతర్గత శూన్య PaintGlyph<Types>::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  c->కూడు_glyph (gid);
  (ఇది+paint).dispatch (c);
}

HB_అంతర్గత శూన్య PaintColrGlyph::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  స్థిరరాశి COLR *colr_పట్టిక = c->get_colr_పట్టిక ();
  స్థిరరాశి పునాది_Glyph_Paint_రికార్డు* baseglyph_paintrecord = colr_పట్టిక->get_పునాది_glyph_paintrecord (gid);
  ఒకవేళ (!baseglyph_paintrecord) తిరిగి;
  c->కూడు_glyph (gid);

  స్థిరరాశి పునాది_Glyph_List &baseglyph_list = colr_పట్టిక->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

మూస <మూస<రకం_పేరు> తరగతి Var>
HB_అంతర్గత శూన్య PaintTransform<Var>::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  (ఇది+transform).closurev1 (c);
}

HB_అంతర్గత శూన్య PaintTranslate::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 2;
}

HB_అంతర్గత శూన్య PaintScale::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 2;
}

HB_అంతర్గత శూన్య PaintScaleAroundCenter::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 4;
}

HB_అంతర్గత శూన్య Paint_Scale_ఏకరీతి::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 1;
}

HB_అంతర్గత శూన్య Paint_Scale_ఏకరీతి_Around_Center::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 3;
}

HB_అంతర్గత శూన్య PaintRotate::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 1;
}

HB_అంతర్గత శూన్య PaintRotateAroundCenter::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 3;
}

HB_అంతర్గత శూన్య PaintSkew::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 2;
}

HB_అంతర్గత శూన్య PaintSkewAroundCenter::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  c->అంకె_var_idxes = 4;
}

HB_అంతర్గత శూన్య Paint_సంయుక్త::closurev1 (hb_colrv1_సంవృతం_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  (ఇది+backdrop).dispatch (c);
}

} /* namespace OT */


#అయితే_ముగింపు /* OT_COLOR_COLR_COLRV1_CLOSURE_HH */
