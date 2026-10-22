#ifndef OT_GLYF_GLYPHHEADER_HH
#define OT_GLYF_GLYPHHEADER_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"


ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ glyf_impl {


ಸಂರಚನಾ GlyphHeader
{
  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸಂಖ್ಯೆ_Of_Contours; }

  template <typename accelerator_t>
  ತಾರ್ಕಿಕ get_extents_without_var_scaled (hb_font_t *font, ಧ್ರುವ accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ಧ್ರುವ
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    ಪೂರ್ಣಾಙ್ಕ lsb = hb_min (xMin, xMax);
    (ಶೂನ್ಯ) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->width     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->height    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HBINT16	ಸಂಖ್ಯೆ_Of_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPHHEADER_HH */
