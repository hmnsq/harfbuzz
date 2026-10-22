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

#અન્તર્ભાવ "hb-vector.h"

સંરચના hb_vector_blob_meta_t
{
  અક્ષર *data;
  પૂર્ણાંક allocated;
  તાર્કિક transferred;
  તાર્કિક in_replace;
};

સ્થિર hb_user_data_ચાવી_t hb_vector_blob_meta_user_data_ચાવી;

સ્થિર અંતઃરેખ શૂન્ય
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				અક્ષર *data,
				પૂર્ણાંક allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = ખોટું;
}

સ્થિર અંતઃરેખ શૂન્ય
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  જો (!meta)
    પાછા;
  જો (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = સાચું;
}

સ્થિર અંતઃરેખ શૂન્ય
hb_vector_blob_meta_destroy (શૂન્ય *data)
{
  auto *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  જો (meta->in_replace)
  {
    meta->in_replace = ખોટું;
    પાછા;
  }
  hb_free (meta);
}

સ્થિર અંતઃરેખ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  પૂર્ણાંક allocated = 0;
  અક્ષર *data = buf->steal (&len, &allocated);
  જો (!data)
    પાછા nullptr;

  hb_blob_t *blob = nullptr;
  જો (*recycled_blob)
    blob = *recycled_blob;
  તાર્કિક reused_blob = blob && blob != hb_blob_get_empty ();
  તાર્કિક new_meta = ખોટું;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ચાવી)
             : nullptr;
  જો (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (કદ (hb_vector_blob_meta_t));
    જો (!meta)
    {
      hb_free (data);
      પાછા nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = સાચું;
    meta->in_replace = ખોટું;
    new_meta = સાચું;
  }

  જો (reused_blob)
  {
    meta->in_replace = સાચું;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  નહીંતર
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    જો (unlikely (!blob))
      પાછા nullptr;
  }

  જો (unlikely (blob == hb_blob_get_empty ()))
  {
    જો (new_meta)
      hb_free (meta);
    hb_free (data);
    પાછા nullptr;
  }

  જો (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_ચાવી,
                              meta,
                              nullptr,
                              સાચું))
  {
    જો (!reused_blob)
      hb_blob_destroy (blob);
    પાછા nullptr;
  }

  જો (*recycled_blob)
    *recycled_blob = nullptr;

  પાછા blob;
}

સ્થિર અંતઃરેખ શૂન્ય
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  જો (!blob)
    પાછા;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_ચાવી);
  જો (!meta || meta->transferred || !meta->data)
    પાછા;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = સાચું;
}

સ્થિર અંતઃરેખ શૂન્ય
hb_vector_transform_point (અચલ hb_transform_t<> &t,
			   દશાંશ x_scale_factor,
			   દશાંશ y_scale_factor,
			   દશાંશ x, દશાંશ y,
			   દશાંશ *tx, દશાંશ *ty)
{
  દશાંશ xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

સ્થિર અંતઃરેખ hb_bool_t
hb_vector_set_glyph_extents_common (અચલ hb_transform_t<> &transform,
				    દશાંશ x_scale_factor,
				    દશાંશ y_scale_factor,
				    અચલ hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  દશાંશ x0 = (દશાંશ) glyph_extents->x_bearing;
  દશાંશ y0 = (દશાંશ) glyph_extents->y_bearing;
  દશાંશ x1 = x0 + glyph_extents->width;
  દશાંશ y1 = y0 + glyph_extents->height;

  દશાંશ px[4] = {x0, x0, x1, x1};
  દશાંશ py[4] = {y0, y1, y0, y1};

  દશાંશ tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  દશાંશ tx_min = tx, tx_max = tx;
  દશાંશ ty_min = ty, ty_max = ty;

  માટે (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  જો (tx_max <= tx_min || ty_max <= ty_min)
    પાછા ખોટું;

  જો (*has_extents)
  {
    દશાંશ x0 = hb_min (extents->x, tx_min);
    દશાંશ y0 = hb_min (extents->y, ty_min);
    દશાંશ x1 = hb_max (extents->x + extents->width, tx_max);
    દશાંશ y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  નહીંતર
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = સાચું;
  }
  પાછા સાચું;
}

સ્થિર અંતઃરેખ શૂન્ય
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 દશાંશ x_scale_factor,
					 દશાંશ y_scale_factor,
					 દશાંશ xx, દશાંશ yx,
					 દશાંશ xy, દશાંશ yy,
					 દશાંશ tx, દશાંશ ty)
{
  જો (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_અંક (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_અંક (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  નહીંતર
  {
    unsigned sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_અંક (xx, sprec);
    out->append_c (',');
    out->append_અંક (yx, sprec);
    out->append_c (',');
    out->append_અંક (-xy, sprec);
    out->append_c (',');
    out->append_અંક (-yy, sprec);
    out->append_c (',');
    out->append_અંક (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_અંક (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
