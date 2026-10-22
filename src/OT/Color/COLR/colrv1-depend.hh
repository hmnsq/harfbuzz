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

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
ನಾಮಾಕಾಶ OT {


HB_INTERNAL ಶೂನ್ಯ PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  ಧ್ರುವ LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  ಪ್ರತಿ (unsigned i = firstLayerIndex; i < firstLayerIndex + ಅಂಕೆ_Layers; i++)
  {
    ಧ್ರುವ Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

template <typename Types>
HB_INTERNAL ಶೂನ್ಯ PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  ಯದಿ (gid != c->source_gid)
    c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);
  (ಏತತ್+paint).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  ಧ್ರುವ COLR *colr_table = c->get_colr_table ();
  ಧ್ರುವ BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  ಯದಿ (!baseglyph_paintrecord) ಪ್ರತ್ಯಾಗಮ;
  c->depend_data->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);

  ಧ್ರುವ BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

template <template<typename> ವರ್ಗ Var>
HB_INTERNAL ಶೂನ್ಯ PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintScale::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleUniform::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintScaleUniformAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
}

HB_INTERNAL ಶೂನ್ಯ PaintComposite::dependv1 (hb_colrv1_depend_context_t* c) ಧ್ರುವ
{
  (ಏತತ್+src).dispatch (c);
  (ಏತತ್+backdrop).dispatch (c);
}


} /* namespace OT */


#endif /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
