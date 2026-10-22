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

#اگر_غیر_تعریف_شدہ HB_RASTER_IMAGE_HH
#تعریف_کرو HB_RASTER_IMAGE_HH

#شامل "hb.hh"
#شامل "hb-raster.hh"
#شامل "hb-raster.h"
#شامل "hb-object.hh"
#شامل "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
ساخت hb_raster_image_t
{
  hb_object_سرخی_t  سرخی;

  hb_سمتیہ_t<uint8_t> بفر;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_اندرونی ساکن غیرعلامتی بائٹیں_فی_pixel (hb_raster_format_t format);
  HB_اندرونی منطقی configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_اندرونی منطقی deserialize_from_png (hb_blob_t *png);
  HB_اندرونی hb_blob_t *serialize_مقصد_png_or_fail () ثابت;
  HB_اندرونی خالی clear ();
  HB_اندرونی ثابت uint8_t *get_بفر () ثابت;
  HB_اندرونی خالی مرکب_from (ثابت hb_raster_image_t *src,
				   hb_paint_مرکب_منوال_t منوال);
};

/* Composite src image onto dst. */
HB_اندرونی خالی
hb_raster_image_مرکب (hb_raster_image_t *dst,
			   ثابت hb_raster_image_t *src,
			   hb_paint_مرکب_منوال_t منوال);


#اگر_ختم /* HB_RASTER_IMAGE_HH */
