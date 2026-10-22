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

#அந்தர்பாவ "hb-vector.h"

கட்டமைப்பு hb_vector_blob_meta_t
{
  எழுத்து *data;
  முழு allocated;
  தருக்க transferred;
  தருக்க in_replace;
};

நிலையான hb_user_data_சாவி_t hb_vector_blob_meta_user_data_சாவி;

நிலையான உள்வரி வெறுமை
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				எழுத்து *data,
				முழு allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = பொய்;
}

நிலையான உள்வரி வெறுமை
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  எனில் (!meta)
    திரும்பு;
  எனில் (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = உண்மை;
}

நிலையான உள்வரி வெறுமை
hb_vector_blob_meta_destroy (வெறுமை *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  எனில் (meta->in_replace)
  {
    meta->in_replace = பொய்;
    திரும்பு;
  }
  hb_free (meta);
}

நிலையான உள்வரி hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  முழு allocated = 0;
  எழுத்து *data = buf->steal (&len, &allocated);
  எனில் (!data)
    திரும்பு nullptr;

  hb_blob_t *blob = nullptr;
  எனில் (*recycled_blob)
    blob = *recycled_blob;
  தருக்க reused_blob = blob && blob != hb_blob_get_empty ();
  தருக்க new_meta = பொய்;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_சாவி)
             : nullptr;
  எனில் (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (அளவு (hb_vector_blob_meta_t));
    எனில் (!meta)
    {
      hb_free (data);
      திரும்பு nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = உண்மை;
    meta->in_replace = பொய்;
    new_meta = உண்மை;
  }

  எனில் (reused_blob)
  {
    meta->in_replace = உண்மை;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  இல்லையெனில்
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    எனில் (unlikely (!blob))
      திரும்பு nullptr;
  }

  எனில் (unlikely (blob == hb_blob_get_empty ()))
  {
    எனில் (new_meta)
      hb_free (meta);
    hb_free (data);
    திரும்பு nullptr;
  }

  எனில் (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_சாவி,
                              meta,
                              nullptr,
                              உண்மை))
  {
    எனில் (!reused_blob)
      hb_blob_destroy (blob);
    திரும்பு nullptr;
  }

  எனில் (*recycled_blob)
    *recycled_blob = nullptr;

  திரும்பு blob;
}

நிலையான உள்வரி வெறுமை
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  எனில் (!blob)
    திரும்பு;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_சாவி);
  எனில் (!meta || meta->transferred || !meta->data)
    திரும்பு;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = உண்மை;
}

நிலையான உள்வரி வெறுமை
hb_vector_transform_point (மாறிலி hb_transform_t<> &t,
			   தசம x_scale_factor,
			   தசம y_scale_factor,
			   தசம x, தசம y,
			   தசம *tx, தசம *ty)
{
  தசம xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

நிலையான உள்வரி hb_bool_t
hb_vector_set_glyph_extents_common (மாறிலி hb_transform_t<> &transform,
				    தசம x_scale_factor,
				    தசம y_scale_factor,
				    மாறிலி hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  தசம x0 = (தசம) glyph_extents->x_bearing;
  தசம y0 = (தசம) glyph_extents->y_bearing;
  தசம x1 = x0 + glyph_extents->width;
  தசம y1 = y0 + glyph_extents->height;

  தசம px[4] = {x0, x0, x1, x1};
  தசம py[4] = {y0, y1, y0, y1};

  தசம tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  தசம tx_min = tx, tx_max = tx;
  தசம ty_min = ty, ty_max = ty;

  க்கு (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  எனில் (tx_max <= tx_min || ty_max <= ty_min)
    திரும்பு பொய்;

  எனில் (*has_extents)
  {
    தசம x0 = hb_min (extents->x, tx_min);
    தசம y0 = hb_min (extents->y, ty_min);
    தசம x1 = hb_max (extents->x + extents->width, tx_max);
    தசம y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  இல்லையெனில்
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = உண்மை;
  }
  திரும்பு உண்மை;
}

நிலையான உள்வரி வெறுமை
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 தசம x_scale_factor,
					 தசம y_scale_factor,
					 தசம xx, தசம yx,
					 தசம xy, தசம yy,
					 தசம tx, தசம ty)
{
  எனில் (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_இலக்கம் (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_இலக்கம் (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  இல்லையெனில்
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_இலக்கம் (xx, sprec);
    out->append_c (',');
    out->append_இலக்கம் (yx, sprec);
    out->append_c (',');
    out->append_இலக்கம் (-xy, sprec);
    out->append_c (',');
    out->append_இலக்கம் (-yy, sprec);
    out->append_c (',');
    out->append_இலக்கம் (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_இலக்கம் (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
