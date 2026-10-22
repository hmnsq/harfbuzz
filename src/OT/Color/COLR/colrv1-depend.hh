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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
नामाकाश OT {


HB_INTERNAL शून्य PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  ध्रुव LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  अत्थाय (unsigned i = firstLayerIndex; i < firstLayerIndex + अङ्कः_Layers; i++)
  {
    ध्रुव Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL शून्य PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  सचे (gid != c->source_gid)
    c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);
  (एतत्+paint).dispatch (c);
}

HB_INTERNAL शून्य PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  ध्रुव COLR *colr_table = c->get_colr_table ();
  ध्रुव BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  सचे (!baseglyph_paintrecord) पटिनिवत्त;
  c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);

  ध्रुव BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> वग्ग Var>
HB_INTERNAL शून्य PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintScaleUniform::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintScaleUniformAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
}

HB_INTERNAL शून्य PaintComposite::dependv1 (hb_colrv1_depend_context_t* c) ध्रुव
{
  (एतत्+src).dispatch (c);
  (एतत्+backdrop).dispatch (c);
}


} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
