#నిర్వచితం_కాకపోతే OT_GLYF_GLYPHHEADER_HH
#నిర్వచించు OT_GLYF_GLYPHHEADER_HH


#అన్తర్భావ "../../hb-open-type.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


నిర్మాణం Glyph_శీర్షిక
{
  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంఖ్య_యొక్క_Contours; }

  మూస <రకం_పేరు accelerator_t>
  తార్కిక get_extents_without_var_scaled (hb_font_t *font, స్థిరరాశి accelerator_t &glyf_accelerator,
				       hb_codepoint_t gid, hb_glyph_extents_t *extents) స్థిరరాశి
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    పూర్ణాంకం lsb = hb_min (xMin, xMax);
    (శూన్య) glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
    extents->x_bearing = lsb;
    extents->y_bearing = hb_max (yMin, yMax);
    extents->వెడల్పు     = hb_max (xMin, xMax) - hb_min (xMin, xMax);
    extents->ఎత్తు    = hb_min (yMin, yMax) - hb_max (yMin, yMax);

    font->scale_glyph_extents (extents);

    తిరిగి నిజం;
  }

  HBINT16	సంఖ్య_యొక్క_Contours;
		    /* If the number of contours is
		     * greater than or equal to zero,
		     * this is a simple glyph; if negative,
		     * this is a composite glyph. */
  FWORD	xMin;	/* Minimum x for coordinate data. */
  FWORD	yMin;	/* Minimum y for coordinate data. */
  FWORD	xMax;	/* Maximum x for coordinate data. */
  FWORD	yMax;	/* Maximum y for coordinate data. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_GLYPHHEADER_HH */
