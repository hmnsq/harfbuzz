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

#که_نه_تعریف_شوی HB_RASTER_IMAGE_HH
#تعریف_کړه HB_RASTER_IMAGE_HH

#ضمن "hb.hh"
#ضمن "hb-raster.hh"
#ضمن "hb-raster.h"
#ضمن "hb-object.hh"
#ضمن "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
بنية hb_raster_image_t
{
  hb_object_سرلیک_t  سرلیک;

  hb_ویکټور_t<uint8_t> بفر;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_دننی ساكن بینښه بایټونه_فی_pixel (hb_raster_format_t format);
  HB_دننی منطقي configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_دننی منطقي deserialize_from_png (hb_blob_t *png);
  HB_دننی hb_blob_t *serialize_منزل_png_or_fail () ثابت;
  HB_دننی فراغ clear ();
  HB_دننی ثابت uint8_t *get_بفر () ثابت;
  HB_دننی فراغ مرکب_from (ثابت hb_raster_image_t *src,
				   hb_paint_مرکب_ډېرپېښ_t ډېرپېښ);
};

/* Composite src image onto dst. */
HB_دننی فراغ
hb_raster_image_مرکب (hb_raster_image_t *dst,
			   ثابت hb_raster_image_t *src,
			   hb_paint_مرکب_ډېرپېښ_t ډېرپېښ);


#د_که_پای /* HB_RASTER_IMAGE_HH */
