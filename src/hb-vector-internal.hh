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

#ifndef HB_VECTOR_INTERNAL_HH
#define HB_VECTOR_INTERNAL_HH

#अन्तर्भाव "hb-vector.h"

संरचना hb_vector_blob_meta_t
{
  वर्ण *data;
  पूर्णाङ्क allocated;
  तार्किक transferred;
  तार्किक in_replace;
};

स्थिर hb_user_data_कुञ्चिका_t hb_vector_blob_meta_user_data_कुञ्चिका;

स्थिर अन्तःस्थ शून्य
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				वर्ण *data,
				पूर्णाङ्क allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = मिच्छ;
}

स्थिर अन्तःस्थ शून्य
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  जइ (!meta)
    प्रत्यागम;
  जइ (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = सच्च;
}

स्थिर अन्तःस्थ शून्य
hb_vector_blob_meta_destroy (शून्य *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  जइ (meta->in_replace)
  {
    meta->in_replace = मिच्छ;
    प्रत्यागम;
  }
  hb_free (meta);
}

स्थिर अन्तःस्थ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  पूर्णाङ्क allocated = 0;
  वर्ण *data = buf->steal (&len, &allocated);
  जइ (!data)
    प्रत्यागम nullptr;

  hb_blob_t *blob = nullptr;
  जइ (*recycled_blob)
    blob = *recycled_blob;
  तार्किक reused_blob = blob && blob != hb_blob_get_empty ();
  तार्किक new_meta = मिच्छ;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_कुञ्चिका)
             : nullptr;
  जइ (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (परिमाण (hb_vector_blob_meta_t));
    जइ (!meta)
    {
      hb_free (data);
      प्रत्यागम nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = सच्च;
    meta->in_replace = मिच्छ;
    new_meta = सच्च;
  }

  जइ (reused_blob)
  {
    meta->in_replace = सच्च;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  अण्णहा
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    जइ (unlikely (!blob))
      प्रत्यागम nullptr;
  }

  जइ (unlikely (blob == hb_blob_get_empty ()))
  {
    जइ (new_meta)
      hb_free (meta);
    hb_free (data);
    प्रत्यागम nullptr;
  }

  जइ (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_कुञ्चिका,
                              meta,
                              nullptr,
                              सच्च))
  {
    जइ (!reused_blob)
      hb_blob_destroy (blob);
    प्रत्यागम nullptr;
  }

  जइ (*recycled_blob)
    *recycled_blob = nullptr;

  प्रत्यागम blob;
}

स्थिर अन्तःस्थ शून्य
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  जइ (!blob)
    प्रत्यागम;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_कुञ्चिका);
  जइ (!meta || meta->transferred || !meta->data)
    प्रत्यागम;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = सच्च;
}

स्थिर अन्तःस्थ शून्य
hb_vector_transform_point (ध्रुव hb_transform_t<> &t,
			   भिन्न x_scale_factor,
			   भिन्न y_scale_factor,
			   भिन्न x, भिन्न y,
			   भिन्न *tx, भिन्न *ty)
{
  भिन्न xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

स्थिर अन्तःस्थ hb_bool_t
hb_vector_set_glyph_extents_common (ध्रुव hb_transform_t<> &transform,
				    भिन्न x_scale_factor,
				    भिन्न y_scale_factor,
				    ध्रुव hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  भिन्न x0 = (भिन्न) glyph_extents->x_bearing;
  भिन्न y0 = (भिन्न) glyph_extents->y_bearing;
  भिन्न x1 = x0 + glyph_extents->width;
  भिन्न y1 = y0 + glyph_extents->height;

  भिन्न px[4] = {x0, x0, x1, x1};
  भिन्न py[4] = {y0, y1, y0, y1};

  भिन्न tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  भिन्न tx_min = tx, tx_max = tx;
  भिन्न ty_min = ty, ty_max = ty;

  प्रति (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  जइ (tx_max <= tx_min || ty_max <= ty_min)
    प्रत्यागम मिच्छ;

  जइ (*has_extents)
  {
    भिन्न x0 = hb_min (extents->x, tx_min);
    भिन्न y0 = hb_min (extents->y, ty_min);
    भिन्न x1 = hb_max (extents->x + extents->width, tx_max);
    भिन्न y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  अण्णहा
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = सच्च;
  }
  प्रत्यागम सच्च;
}

स्थिर अन्तःस्थ शून्य
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 भिन्न x_scale_factor,
					 भिन्न y_scale_factor,
					 भिन्न xx, भिन्न yx,
					 भिन्न xy, भिन्न yy,
					 भिन्न tx, भिन्न ty)
{
  जइ (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_अङ्कः (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_अङ्कः (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  अण्णहा
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_अङ्कः (xx, sprec);
    out->append_c (',');
    out->append_अङ्कः (yx, sprec);
    out->append_c (',');
    out->append_अङ्कः (-xy, sprec);
    out->append_c (',');
    out->append_अङ्कः (-yy, sprec);
    out->append_c (',');
    out->append_अङ्कः (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_अङ्कः (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
