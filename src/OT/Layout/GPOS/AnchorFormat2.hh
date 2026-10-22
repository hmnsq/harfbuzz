#अगर_अघोषित OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#घोषणा OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा AnchorFormat2
{

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  खुला:
  DEFINE_आकार_STATIC (8);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   भग्न *x, भग्न *y) स्थिर
  {
    hb_font_t *font = c->font;

#अगर_घोषित HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    वापस;
#पूर्ण_अगर

    अचिन्हित पूर्णांक x_ppem = font->x_ppem;
    अचिन्हित पूर्णांक y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    bool ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed<AnchorFormat2> (यह));
  }
};

}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
