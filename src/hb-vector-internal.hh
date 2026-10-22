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

#অন্তর্ভাৱ "hb-vector.h"

সংরচনা hb_vector_blob_meta_t
{
  ৱর্ণ *data;
  পূর্ণাঙ্ক allocated;
  তার্কিক transferred;
  তার্কিক in_replace;
};

স্থির hb_user_data_চাবি_t hb_vector_blob_meta_user_data_চাবি;

স্থির অন্তঃস্থ শূন্য
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				ৱর্ণ *data,
				পূর্ণাঙ্ক allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = মিছা;
}

স্থির অন্তঃস্থ শূন্য
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  যদি (!meta)
    ঘূৰাই;
  যদি (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = সঁচা;
}

স্থির অন্তঃস্থ শূন্য
hb_vector_blob_meta_destroy (শূন্য *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  যদি (meta->in_replace)
  {
    meta->in_replace = মিছা;
    ঘূৰাই;
  }
  hb_free (meta);
}

স্থির অন্তঃস্থ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  পূর্ণাঙ্ক allocated = 0;
  ৱর্ণ *data = buf->steal (&len, &allocated);
  যদি (!data)
    ঘূৰাই nullptr;

  hb_blob_t *blob = nullptr;
  যদি (*recycled_blob)
    blob = *recycled_blob;
  তার্কিক reused_blob = blob && blob != hb_blob_get_empty ();
  তার্কিক new_meta = মিছা;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_চাবি)
             : nullptr;
  যদি (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (পরিমাণ (hb_vector_blob_meta_t));
    যদি (!meta)
    {
      hb_free (data);
      ঘূৰাই nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = সঁচা;
    meta->in_replace = মিছা;
    new_meta = সঁচা;
  }

  যদি (reused_blob)
  {
    meta->in_replace = সঁচা;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  অন্যথা
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    যদি (unlikely (!blob))
      ঘূৰাই nullptr;
  }

  যদি (unlikely (blob == hb_blob_get_empty ()))
  {
    যদি (new_meta)
      hb_free (meta);
    hb_free (data);
    ঘূৰাই nullptr;
  }

  যদি (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_চাবি,
                              meta,
                              nullptr,
                              সঁচা))
  {
    যদি (!reused_blob)
      hb_blob_destroy (blob);
    ঘূৰাই nullptr;
  }

  যদি (*recycled_blob)
    *recycled_blob = nullptr;

  ঘূৰাই blob;
}

স্থির অন্তঃস্থ শূন্য
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  যদি (!blob)
    ঘূৰাই;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_চাবি);
  যদি (!meta || meta->transferred || !meta->data)
    ঘূৰাই;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = সঁচা;
}

স্থির অন্তঃস্থ শূন্য
hb_vector_transform_point (ধ্রুৱ hb_transform_t<> &t,
			   ভিন্ন x_scale_factor,
			   ভিন্ন y_scale_factor,
			   ভিন্ন x, ভিন্ন y,
			   ভিন্ন *tx, ভিন্ন *ty)
{
  ভিন্ন xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

স্থির অন্তঃস্থ hb_bool_t
hb_vector_set_glyph_extents_common (ধ্রুৱ hb_transform_t<> &transform,
				    ভিন্ন x_scale_factor,
				    ভিন্ন y_scale_factor,
				    ধ্রুৱ hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  ভিন্ন x0 = (ভিন্ন) glyph_extents->x_bearing;
  ভিন্ন y0 = (ভিন্ন) glyph_extents->y_bearing;
  ভিন্ন x1 = x0 + glyph_extents->width;
  ভিন্ন y1 = y0 + glyph_extents->height;

  ভিন্ন px[4] = {x0, x0, x1, x1};
  ভিন্ন py[4] = {y0, y1, y0, y1};

  ভিন্ন tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  ভিন্ন tx_min = tx, tx_max = tx;
  ভিন্ন ty_min = ty, ty_max = ty;

  বাবে (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  যদি (tx_max <= tx_min || ty_max <= ty_min)
    ঘূৰাই মিছা;

  যদি (*has_extents)
  {
    ভিন্ন x0 = hb_min (extents->x, tx_min);
    ভিন্ন y0 = hb_min (extents->y, ty_min);
    ভিন্ন x1 = hb_max (extents->x + extents->width, tx_max);
    ভিন্ন y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  অন্যথা
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = সঁচা;
  }
  ঘূৰাই সঁচা;
}

স্থির অন্তঃস্থ শূন্য
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 ভিন্ন x_scale_factor,
					 ভিন্ন y_scale_factor,
					 ভিন্ন xx, ভিন্ন yx,
					 ভিন্ন xy, ভিন্ন yy,
					 ভিন্ন tx, ভিন্ন ty)
{
  যদি (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_অঙ্ক (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_অঙ্ক (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  অন্যথা
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_অঙ্ক (xx, sprec);
    out->append_c (',');
    out->append_অঙ্ক (yx, sprec);
    out->append_c (',');
    out->append_অঙ্ক (-xy, sprec);
    out->append_c (',');
    out->append_অঙ্ক (-yy, sprec);
    out->append_c (',');
    out->append_অঙ্ক (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_অঙ্ক (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
