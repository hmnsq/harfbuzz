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

#నిర్వచితం_కాకపోతే OT_COLOR_COLR_COLRV1_DEPEND_HH
#నిర్వచించు OT_COLOR_COLR_COLRV1_DEPEND_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
నామాకాశ OT {


HB_అంతర్గత శూన్య PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  స్థిరరాశి LayerList &paint_స్థానభ్రంశం_lists = c->get_colr_పట్టిక ()->get_layerList ();
  కొరకు (గుర్తులేని i = మొదటి_Layer_సూచీ; i < మొదటి_Layer_సూచీ + అంకె_Layers; i++)
  {
    స్థిరరాశి Paint &paint = paint_స్థానభ్రంశం_lists.get_paint (i);
    paint.dispatch (c);
  }
}

మూస <రకం_పేరు Types>
HB_అంతర్గత శూన్య PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  ఒకవేళ (gid != c->ఆధారం_gid)
    c->depend_దత్తాంశం->కూడు_depend(c->ఆధారం_gid, HB_OT_ట్యాగ్_COLR, gid);
  (ఇది+paint).dispatch (c);
}

HB_అంతర్గత శూన్య PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  స్థిరరాశి COLR *colr_పట్టిక = c->get_colr_పట్టిక ();
  స్థిరరాశి పునాది_Glyph_Paint_రికార్డు* baseglyph_paintrecord = colr_పట్టిక->get_పునాది_glyph_paintrecord (gid);
  ఒకవేళ (!baseglyph_paintrecord) తిరిగి;
  c->depend_దత్తాంశం->కూడు_depend(c->ఆధారం_gid, HB_OT_ట్యాగ్_COLR, gid);

  స్థిరరాశి పునాది_Glyph_List &baseglyph_list = colr_పట్టిక->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

మూస <మూస<రకం_పేరు> తరగతి Var>
HB_అంతర్గత శూన్య PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintScale::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య Paint_Scale_ఏకరీతి::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య Paint_Scale_ఏకరీతి_Around_Center::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
}

HB_అంతర్గత శూన్య Paint_సంయుక్త::dependv1 (hb_colrv1_depend_context_t* c) స్థిరరాశి
{
  (ఇది+src).dispatch (c);
  (ఇది+backdrop).dispatch (c);
}


} /* namespace OT */


#అయితే_ముగింపు /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
