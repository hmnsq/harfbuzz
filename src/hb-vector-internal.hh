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

#ಅನ್ತರ್ಭಾವ "hb-vector.h"

ಸಂರಚನಾ hb_vector_blob_meta_t
{
  ವರ್ಣ *data;
  ಪೂರ್ಣಾಙ್ಕ allocated;
  ತಾರ್ಕಿಕ transferred;
  ತಾರ್ಕಿಕ in_replace;
};

ಸ್ಥಿರ hb_user_data_ಕೀಲಿ_t hb_vector_blob_meta_user_data_ಕೀಲಿ;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				ವರ್ಣ *data,
				ಪೂರ್ಣಾಙ್ಕ allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = ಅಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  ಯದಿ (!meta)
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = ಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_vector_blob_meta_destroy (ಶೂನ್ಯ *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  ಯದಿ (meta->in_replace)
  {
    meta->in_replace = ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  hb_free (meta);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  ಪೂರ್ಣಾಙ್ಕ allocated = 0;
  ವರ್ಣ *data = buf->steal (&len, &allocated);
  ಯದಿ (!data)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_blob_t *blob = nullptr;
  ಯದಿ (*recycled_blob)
    blob = *recycled_blob;
  ತಾರ್ಕಿಕ reused_blob = blob && blob != hb_blob_get_empty ();
  ತಾರ್ಕಿಕ new_meta = ಅಸತ್ಯ;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ಕೀಲಿ)
             : nullptr;
  ಯದಿ (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (ಪರಿಮಾಣ (hb_vector_blob_meta_t));
    ಯದಿ (!meta)
    {
      hb_free (data);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = ಸತ್ಯ;
    meta->in_replace = ಅಸತ್ಯ;
    new_meta = ಸತ್ಯ;
  }

  ಯದಿ (reused_blob)
  {
    meta->in_replace = ಸತ್ಯ;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  ಅನ್ಯಥಾ
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    ಯದಿ (unlikely (!blob))
      ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (unlikely (blob == hb_blob_get_empty ()))
  {
    ಯದಿ (new_meta)
      hb_free (meta);
    hb_free (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_ಕೀಲಿ,
                              meta,
                              nullptr,
                              ಸತ್ಯ))
  {
    ಯದಿ (!reused_blob)
      hb_blob_destroy (blob);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (*recycled_blob)
    *recycled_blob = nullptr;

  ಪ್ರತ್ಯಾಗಮ blob;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  ಯದಿ (!blob)
    ಪ್ರತ್ಯಾಗಮ;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ಕೀಲಿ);
  ಯದಿ (!meta || meta->transferred || !meta->data)
    ಪ್ರತ್ಯಾಗಮ;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = ಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_vector_transform_point (ಧ್ರುವ hb_transform_t<> &t,
			   ಭಿನ್ನ x_scale_factor,
			   ಭಿನ್ನ y_scale_factor,
			   ಭಿನ್ನ x, ಭಿನ್ನ y,
			   ಭಿನ್ನ *tx, ಭಿನ್ನ *ty)
{
  ಭಿನ್ನ xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_bool_t
hb_vector_set_glyph_extents_common (ಧ್ರುವ hb_transform_t<> &transform,
				    ಭಿನ್ನ x_scale_factor,
				    ಭಿನ್ನ y_scale_factor,
				    ಧ್ರುವ hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  ಭಿನ್ನ x0 = (ಭಿನ್ನ) glyph_extents->x_bearing;
  ಭಿನ್ನ y0 = (ಭಿನ್ನ) glyph_extents->y_bearing;
  ಭಿನ್ನ x1 = x0 + glyph_extents->width;
  ಭಿನ್ನ y1 = y0 + glyph_extents->height;

  ಭಿನ್ನ px[4] = {x0, x0, x1, x1};
  ಭಿನ್ನ py[4] = {y0, y1, y0, y1};

  ಭಿನ್ನ tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  ಭಿನ್ನ tx_min = tx, tx_max = tx;
  ಭಿನ್ನ ty_min = ty, ty_max = ty;

  ಪ್ರತಿ (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  ಯದಿ (tx_max <= tx_min || ty_max <= ty_min)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (*has_extents)
  {
    ಭಿನ್ನ x0 = hb_min (extents->x, tx_min);
    ಭಿನ್ನ y0 = hb_min (extents->y, ty_min);
    ಭಿನ್ನ x1 = hb_max (extents->x + extents->width, tx_max);
    ಭಿನ್ನ y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ಅನ್ಯಥಾ
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 ಭಿನ್ನ x_scale_factor,
					 ಭಿನ್ನ y_scale_factor,
					 ಭಿನ್ನ xx, ಭಿನ್ನ yx,
					 ಭಿನ್ನ xy, ಭಿನ್ನ yy,
					 ಭಿನ್ನ tx, ಭಿನ್ನ ty)
{
  ಯದಿ (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_ಅಂಕೆ (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_ಅಂಕೆ (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  ಅನ್ಯಥಾ
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_ಅಂಕೆ (xx, sprec);
    out->append_c (',');
    out->append_ಅಂಕೆ (yx, sprec);
    out->append_c (',');
    out->append_ಅಂಕೆ (-xy, sprec);
    out->append_c (',');
    out->append_ಅಂಕೆ (-yy, sprec);
    out->append_c (',');
    out->append_ಅಂಕೆ (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_ಅಂಕೆ (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
