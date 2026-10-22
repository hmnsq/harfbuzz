#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_GLYPHHEADER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_GLYPHHEADER_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਬਣਤਰ Glyph_ਸਿਰਲੇਖ
{
  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਖਿਆ_ਦਾ_Contours; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ accelerator_t>
  ਤਾਰਕਿਕ get_extents_without_var_scaled (hb_font_t *font, ਅਚਲ accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) ਅਚਲ
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    ਪੂਰਨ lsb = hb_min (xMin, xMax);
    (ਖਾਲੀ) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->ਚੌੜਾਈ     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->ਉਚਾਈ    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    ਵਾਪਸ ਸੱਚ;
  }

  HBINT16	ਸੰਖਿਆ_ਦਾ_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_GLYPHHEADER_HH */
