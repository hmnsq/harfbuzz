#अगर_अघोषित OT_GLYF_GLYPHHEADER_HH
#घोषणा OT_GLYF_GLYPHHEADER_HH


#समावेश "../../hb-open-type.hh"


namespace OT {
namespace glyf_impl {


काष्ठा GlyphHeader
{
  bool has_तथ्य () स्थिर { वापस संख्या_Of_Contours; }

  ढाँचा <typename accelerator_t>
  bool get_extents_without_var_scaled (hb_font_t *font, स्थिर accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) स्थिर
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    पूर्णांक lsb = hb_min (xMin, xMax);
    (व्योम) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->width     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->height    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    वापस true;
  }

  HBINT16	संख्या_Of_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  खुला:
  DEFINE_आकार_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_GLYPHHEADER_HH */
