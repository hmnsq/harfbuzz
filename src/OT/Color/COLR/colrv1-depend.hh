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

#ifndef OT_COLOR_COLR_COLRV1_DEPEND_HH
#define OT_COLOR_COLR_COLRV1_DEPEND_HH

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
நாமாகாஶ OT {


HB_INTERNAL வெறுமை PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  மாறிலி LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  க்கு (unsigned i = firstLayerIndex; i < firstLayerIndex + இலக்கம்_Layers; i++)
  {
    மாறிலி Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL வெறுமை PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  எனில் (gid != c->source_gid)
    c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);
  (இது+paint).dispatch (c);
}

HB_INTERNAL வெறுமை PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  மாறிலி COLR *colr_table = c->get_colr_table ();
  மாறிலி BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  எனில் (!baseglyph_paintrecord) திரும்பு;
  c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);

  மாறிலி BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> வகுப்பு Var>
HB_INTERNAL வெறுமை PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintScale::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintScaleUniform::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintScaleUniformAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
}

HB_INTERNAL வெறுமை PaintComposite::dependv1 (hb_colrv1_depend_context_t* c) மாறிலி
{
  (இது+src).dispatch (c);
  (இது+backdrop).dispatch (c);
}


} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
