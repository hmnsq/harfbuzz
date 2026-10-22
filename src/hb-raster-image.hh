/*
 * Copyright © 2026  Behdad Esfahbod
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_RASTER_IMAGE_HH
#నిర్వచించు HB_RASTER_IMAGE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-raster.hh"
#అన్తర్భావ "hb-raster.h"
#అన్తర్భావ "hb-object.hh"
#అన్తర్భావ "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
నిర్మాణం hb_raster_image_t
{
  hb_object_శీర్షిక_t  శీర్షిక;

  hb_సదిశ_t<uint8_t> బఫరు;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_అంతర్గత స్థిర గుర్తులేని బైట్లు_చొప్పున_pixel (hb_raster_format_t format);
  HB_అంతర్గత తార్కిక configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_అంతర్గత తార్కిక deserialize_from_png (hb_blob_t *png);
  HB_అంతర్గత hb_blob_t *serialize_గమ్యం_png_or_fail () స్థిరరాశి;
  HB_అంతర్గత శూన్య clear ();
  HB_అంతర్గత స్థిరరాశి uint8_t *get_బఫరు () స్థిరరాశి;
  HB_అంతర్గత శూన్య సంయుక్త_from (స్థిరరాశి hb_raster_image_t *src,
				   hb_paint_సంయుక్త_బహుళకం_t బహుళకం);
};

/* Composite src image onto dst. */
HB_అంతర్గత శూన్య
hb_raster_image_సంయుక్త (hb_raster_image_t *dst,
			   స్థిరరాశి hb_raster_image_t *src,
			   hb_paint_సంయుక్త_బహుళకం_t బహుళకం);


#అయితే_ముగింపు /* HB_RASTER_IMAGE_HH */
