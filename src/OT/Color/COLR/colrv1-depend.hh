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

#अगर_अघोषित OT_COLOR_COLR_COLRV1_DEPEND_HH
#घोषणा OT_COLOR_COLR_COLRV1_DEPEND_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "COLR.hh"

/*
 * COLR -- Color
 * https://docs.microsoft.com/en-us/typography/opentype/spec/colr
 */
namespace OT {


HB_INTERNAL व्योम PaintColrLayers::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  स्थिर LayerList &paint_offset_lists = c->get_colr_table ()->get_layerList ();
  क्रम (अचिन्हित i = first_Layer_सूचकांक; i < first_Layer_सूचकांक + अंक_Layers; i++)
  {
    स्थिर Paint &paint = paint_offset_lists.get_paint (i);
    paint.dispatch (c);
  }
}

ढाँचा <typename Types>
HB_INTERNAL व्योम PaintGlyph<Types>::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  // PaintGlyph (Format 10) references glyph outlines (from glyf/CFF), not paint graphs.
  // Self-reference is valid per OpenType spec: a base glyph can reference its own
  // outline as geometry to fill. This is not a dependency in the traditional sense
  // since the outline is part of the same glyph data, so we skip it.
  अगर (gid != c->source_gid)
    c->depend_तथ्य->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);
  (यह+paint).dispatch (c);
}

HB_INTERNAL व्योम PaintColrGlyph::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  स्थिर COLR *colr_table = c->get_colr_table ();
  स्थिर BaseGlyphPaintRecord* baseglyph_paintrecord = colr_table->get_base_glyph_paintrecord (gid);
  अगर (!baseglyph_paintrecord) वापस;
  c->depend_तथ्य->add_depend(c->source_gid, HB_OT_TAG_COLR, gid);

  स्थिर BaseGlyphList &baseglyph_list = colr_table->get_baseglyphList ();
  (&baseglyph_list+baseglyph_paintrecord->paint).dispatch (c);
}

ढाँचा <ढाँचा<typename> श्रेणी Var>
HB_INTERNAL व्योम PaintTransform<Var>::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintTranslate::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintScale::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintScaleAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintScaleUniform::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintScaleUniformAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintRotate::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintRotateAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintSkew::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintSkewAroundCenter::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
}

HB_INTERNAL व्योम PaintComposite::dependv1 (hb_colrv1_depend_context_t* c) स्थिर
{
  (यह+src).dispatch (c);
  (यह+backdrop).dispatch (c);
}


} /* namespace OT */


#पूर्ण_अगर /* OT_COLOR_COLR_COLRV1_DEPEND_HH */
