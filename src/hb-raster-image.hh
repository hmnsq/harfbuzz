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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_RASTER_IMAGE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_IMAGE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-object.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
ਬਣਤਰ hb_raster_image_t
{
  hb_object_ਸਿਰਲੇਖ_t  ਸਿਰਲੇਖ;

  hb_ਸਦਿਸ਼_t<uint8_t> ਬਫਰ;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel (hb_raster_format_t format);
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ deserialize_from_png (hb_blob_t *png);
  HB_ਅੰਦਰੂਨੀ hb_blob_t *serialize_ਮੰਜ਼ਿਲ_png_or_fail () ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ clear ();
  HB_ਅੰਦਰੂਨੀ ਅਚਲ uint8_t *get_ਬਫਰ () ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਸੰਯੁਕਤ_from (ਅਚਲ hb_raster_image_t *src,
				   hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ);
};

/* Composite src image onto dst. */
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_raster_image_ਸੰਯੁਕਤ (hb_raster_image_t *dst,
			   ਅਚਲ hb_raster_image_t *src,
			   hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ);


#ਜੇ_ਅੰਤ /* HB_RASTER_IMAGE_HH */
