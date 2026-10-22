#ifndef OT_GLYF_GLYPHHEADER_HH
#define OT_GLYF_GLYPHHEADER_HH


#شامل "../../hb-open-type.hh"


فضاینام OT {
فضاینام glyf_impl {


ساختار GlyphHeader
{
  منطقی has_data () ثابت { بازگشت عدد_Of_Contours; }

  template <typename accelerator_t>
  منطقی get_extents_without_var_scaled (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ثابت
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    صحیح lsb = hb_min (xMin, xMax);
    (تهی) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->width     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->height    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    بازگشت درست;
  }

  HBINT16	عدد_Of_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  عمومی:
  DEFINE_SIZE_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPHHEADER_HH */
