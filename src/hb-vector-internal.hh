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

#ଅନ୍ତର୍ଭାଵ "hb-vector.h"

ସଂରଚନା hb_vector_blob_meta_t
{
  ଅକ୍ଷର *data;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated;
  ତାର୍କିକ transferred;
  ତାର୍କିକ in_replace;
};

ସ୍ଥିର hb_user_data_ଚାବି_t hb_vector_blob_meta_user_data_ଚାବି;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				ଅକ୍ଷର *data,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = ମିଥ୍ୟା;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  ଯଦି (!meta)
    ଫେରନ୍ତୁ;
  ଯଦି (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = ସତ୍ୟ;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_vector_blob_meta_destroy (ଶୂନ୍ୟ *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  ଯଦି (meta->in_replace)
  {
    meta->in_replace = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ;
  }
  hb_free (meta);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated = 0;
  ଅକ୍ଷର *data = buf->steal (&len, &allocated);
  ଯଦି (!data)
    ଫେରନ୍ତୁ nullptr;

  hb_blob_t *blob = nullptr;
  ଯଦି (*recycled_blob)
    blob = *recycled_blob;
  ତାର୍କିକ reused_blob = blob && blob != hb_blob_get_empty ();
  ତାର୍କିକ new_meta = ମିଥ୍ୟା;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ଚାବି)
             : nullptr;
  ଯଦି (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (ଆକାର (hb_vector_blob_meta_t));
    ଯଦି (!meta)
    {
      hb_free (data);
      ଫେରନ୍ତୁ nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = ସତ୍ୟ;
    meta->in_replace = ମିଥ୍ୟା;
    new_meta = ସତ୍ୟ;
  }

  ଯଦି (reused_blob)
  {
    meta->in_replace = ସତ୍ୟ;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  ଅନ୍ୟଥା
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    ଯଦି (unlikely (!blob))
      ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (unlikely (blob == hb_blob_get_empty ()))
  {
    ଯଦି (new_meta)
      hb_free (meta);
    hb_free (data);
    ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_ଚାବି,
                              meta,
                              nullptr,
                              ସତ୍ୟ))
  {
    ଯଦି (!reused_blob)
      hb_blob_destroy (blob);
    ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (*recycled_blob)
    *recycled_blob = nullptr;

  ଫେରନ୍ତୁ blob;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  ଯଦି (!blob)
    ଫେରନ୍ତୁ;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ଚାବି);
  ଯଦି (!meta || meta->transferred || !meta->data)
    ଫେରନ୍ତୁ;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = ସତ୍ୟ;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_vector_transform_point (ଅଚଳ hb_transform_t<> &t,
			   ଦଶମିକ x_scale_factor,
			   ଦଶମିକ y_scale_factor,
			   ଦଶମିକ x, ଦଶମିକ y,
			   ଦଶମିକ *tx, ଦଶମିକ *ty)
{
  ଦଶମିକ xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_bool_t
hb_vector_set_glyph_extents_common (ଅଚଳ hb_transform_t<> &transform,
				    ଦଶମିକ x_scale_factor,
				    ଦଶମିକ y_scale_factor,
				    ଅଚଳ hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  ଦଶମିକ x0 = (ଦଶମିକ) glyph_extents->x_bearing;
  ଦଶମିକ y0 = (ଦଶମିକ) glyph_extents->y_bearing;
  ଦଶମିକ x1 = x0 + glyph_extents->width;
  ଦଶମିକ y1 = y0 + glyph_extents->height;

  ଦଶମିକ px[4] = {x0, x0, x1, x1};
  ଦଶମିକ py[4] = {y0, y1, y0, y1};

  ଦଶମିକ tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  ଦଶମିକ tx_min = tx, tx_max = tx;
  ଦଶମିକ ty_min = ty, ty_max = ty;

  ପାଇଁ (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  ଯଦି (tx_max <= tx_min || ty_max <= ty_min)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (*has_extents)
  {
    ଦଶମିକ x0 = hb_min (extents->x, tx_min);
    ଦଶମିକ y0 = hb_min (extents->y, ty_min);
    ଦଶମିକ x1 = hb_max (extents->x + extents->width, tx_max);
    ଦଶମିକ y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ଅନ୍ୟଥା
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 ଦଶମିକ x_scale_factor,
					 ଦଶମିକ y_scale_factor,
					 ଦଶମିକ xx, ଦଶମିକ yx,
					 ଦଶମିକ xy, ଦଶମିକ yy,
					 ଦଶମିକ tx, ଦଶମିକ ty)
{
  ଯଦି (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_ଅଙ୍କ (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_ଅଙ୍କ (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  ଅନ୍ୟଥା
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_ଅଙ୍କ (xx, sprec);
    out->append_c (',');
    out->append_ଅଙ୍କ (yx, sprec);
    out->append_c (',');
    out->append_ଅଙ୍କ (-xy, sprec);
    out->append_c (',');
    out->append_ଅଙ୍କ (-yy, sprec);
    out->append_c (',');
    out->append_ଅଙ୍କ (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_ଅଙ୍କ (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
