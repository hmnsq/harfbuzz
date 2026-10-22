#ifndef OT_GLYF_GLYPHHEADER_HH
#define OT_GLYF_GLYPHHEADER_HH


#அந்தர்பாவ "../../hb-open-type.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


கட்டமைப்பு GlyphHeader
{
  தருக்க has_data () மாறிலி { திரும்பு எண்_Of_Contours; }

  template <typename accelerator_t>
  தருக்க get_extents_without_var_scaled (hb_font_t *font, மாறிலி accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) மாறிலி
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    முழு lsb = hb_min (xMin, xMax);
    (வெறுமை) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->width     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->height    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    திரும்பு உண்மை;
  }

  HBINT16	எண்_Of_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  பொது:
  DEFINE_SIZE_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPHHEADER_HH */
