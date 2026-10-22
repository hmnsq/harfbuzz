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

#अगर_अघोषित HB_RASTER_IMAGE_HH
#घोषणा HB_RASTER_IMAGE_HH

#समावेश "hb.hh"
#समावेश "hb-raster.hh"
#समावेश "hb-raster.h"
#समावेश "hb-object.hh"
#समावेश "hb-vector.hh"


/* hb_raster_image_t — pixel artifact */
काष्ठा hb_raster_image_t
{
  hb_object_header_t  header;

  hb_vector_t<uint8_t> बफर;
  hb_raster_extents_t  extents     = {};
  hb_raster_format_t   format      = HB_RASTER_FORMAT_A8;

  HB_INTERNAL अटल अचिन्हित bytes_per_pixel (hb_raster_format_t format);
  HB_INTERNAL bool configure (hb_raster_format_t format, hb_raster_extents_t extents);
  HB_INTERNAL bool deserialize_from_png (hb_blob_t *png);
  HB_INTERNAL hb_blob_t *serialize_to_png_or_fail () स्थिर;
  HB_INTERNAL व्योम सफा ();
  HB_INTERNAL स्थिर uint8_t *get_बफर () स्थिर;
  HB_INTERNAL व्योम composite_from (स्थिर hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode);
};

/* Composite src image onto dst. */
HB_INTERNAL व्योम
hb_raster_image_composite (hb_raster_image_t *dst,
			   स्थिर hb_raster_image_t *src,
			   hb_paint_composite_mode_t mode);


#पूर्ण_अगर /* HB_RASTER_IMAGE_HH */
