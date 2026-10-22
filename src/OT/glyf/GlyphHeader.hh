#جے_بنا_تعریف OT_GLYF_GLYPHHEADER_HH
#تعریف_کرو OT_GLYF_GLYPHHEADER_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية Glyph_سرخی
{
  منطقي has_انکڑے () ثابت { أرجع نمبر_دا_Contours; }

  سانچا <قسم_ناں accelerator_t>
  منطقي get_extents_without_var_scaled (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ثابت
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    صحيح lsb = hb_min (xMin, xMax);
    (فراغ) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->چوڑائی     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->اچائی    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    أرجع سچ;
  }

  HBINT16	نمبر_دا_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  عام:
  DEFINE_ناپ_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_GLYPHHEADER_HH */
