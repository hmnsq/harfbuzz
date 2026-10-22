#ifndef OT_GLYF_GLYPHHEADER_HH
#define OT_GLYF_GLYPHHEADER_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


ସଂରଚନା GlyphHeader
{
  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ ସଂଖ୍ୟା_Of_Contours; }

  template <typename accelerator_t>
  ତାର୍କିକ get_extents_without_var_scaled (hb_font_t *font, ଅଚଳ accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ଅଚଳ
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = hb_min (xMin, xMax);
    (ଶୂନ୍ୟ) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->width     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->height    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HBINT16	ସଂଖ୍ୟା_Of_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPHHEADER_HH */
