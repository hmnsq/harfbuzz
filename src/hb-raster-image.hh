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

#ifndef HB_RASTER_IMAGE_HH
#define HB_RASTER_IMAGE_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-raster.hh"
#ଅନ୍ତର୍ଭାଵ "hb-raster.h"
#ଅନ୍ତର୍ଭାଵ "hb-object.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
ସଂରଚନା hb_raster_image_t
{
  hb_object_header_t  header;

  hb_vector_t<uint8_t> buffer;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_INTERNAL ସ୍ଥିର unsigned bytes_per_pixel (hb_raster_format_t format);
  HB_INTERNAL ତାର୍କିକ configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_INTERNAL ତାର୍କିକ deserialize_from_png (hb_blob_t *png);
  HB_INTERNAL hb_blob_t *serialize_to_png_or_fail () ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ clear ();
  HB_INTERNAL ଅଚଳ uint8_t *get_buffer () ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ composite_from (ଅଚଳ hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode);
};

/* Composite src image onto dst. */
HB_INTERNAL ଶୂନ୍ୟ
hb_raster_image_composite (hb_raster_image_t *dst,
			   ଅଚଳ hb_raster_image_t *src,
			   hb_paint_composite_mode_t mode);


#endif /* HB_RASTER_IMAGE_HH */
