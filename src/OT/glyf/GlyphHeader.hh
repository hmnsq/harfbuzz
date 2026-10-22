#اگر_غیر_تعریف_شدہ OT_GLYF_GLYPHHEADER_HH
#تعریف_کرو OT_GLYF_GLYPHHEADER_HH


#شامل "../../hb-open-type.hh"


نامخلا OT {
نامخلا glyf_impl {


ساخت Glyph_سرخی
{
  منطقی has_ڈیٹا () ثابت { واپس نمبر_کا_Contours; }

  سانچہ <قسم_نام accelerator_t>
  منطقی get_extents_without_var_scaled (hb_font_t *font, ثابت accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ثابت
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    صحیح lsb = hb_min (xMin, xMax);
    (خالی) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->چوڑائی     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->اونچائی    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    واپس سچ;
  }

  HBINT16	نمبر_کا_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  عوامی:
  DEFINE_سائز_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_GLYPHHEADER_HH */
