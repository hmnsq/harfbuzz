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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_ਅੰਦਰੂਨੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_ਅੰਦਰੂਨੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.h"

ਬਣਤਰ hb_ਸਦਿਸ਼_blob_meta_t
{
  ਅੱਖਰ *ਅੰਕੜੇ;
  ਪੂਰਨ allocated;
  ਤਾਰਕਿਕ transferred;
  ਤਾਰਕਿਕ ਵਿੱਚ_replace;
};

ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_ਸਦਿਸ਼_blob_meta_user_ਅੰਕੜੇ_ਕੁੰਜੀ;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_blob_meta_ਸੈੱਟ_ਬਫਰ (hb_ਸਦਿਸ਼_blob_meta_t *meta,
				ਅੱਖਰ *ਅੰਕੜੇ,
				ਪੂਰਨ allocated)
{
  meta->ਅੰਕੜੇ = ਅੰਕੜੇ;
  meta->allocated = allocated;
  meta->transferred = ਝੂਠ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_blob_meta_release_ਬਫਰ (hb_ਸਦਿਸ਼_blob_meta_t *meta)
{
  ਜੇ (!meta)
    ਵਾਪਸ;
  ਜੇ (!meta->transferred && meta->ਅੰਕੜੇ)
    hb_free (meta->ਅੰਕੜੇ);
  meta->ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  meta->allocated = 0;
  meta->transferred = ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_blob_meta_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *meta = (hb_ਸਦਿਸ਼_blob_meta_t *) ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_blob_meta_release_ਬਫਰ (meta);
  ਜੇ (meta->ਵਿੱਚ_replace)
  {
    meta->ਵਿੱਚ_replace = ਝੂਠ;
    ਵਾਪਸ;
  }
  hb_free (meta);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_ਸਦਿਸ਼_buf_t *buf)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
  ਪੂਰਨ allocated = 0;
  ਅੱਖਰ *ਅੰਕੜੇ = buf->steal (&len, &allocated);
  ਜੇ (!ਅੰਕੜੇ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_blob_t *blob = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (*recycled_blob)
    blob = *recycled_blob;
  ਤਾਰਕਿਕ reused_blob = blob && blob != hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  ਤਾਰਕਿਕ new_meta = ਝੂਠ;
  ਸਵੈਚਾਲਿਤ *meta = reused_blob
             ? (hb_ਸਦਿਸ਼_blob_meta_t *) hb_blob_get_user_ਅੰਕੜੇ (blob, &hb_ਸਦਿਸ਼_blob_meta_user_ਅੰਕੜੇ_ਕੁੰਜੀ)
             : ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!meta)
  {
    meta = (hb_ਸਦਿਸ਼_blob_meta_t *) hb_malloc (ਆਕਾਰ (hb_ਸਦਿਸ਼_blob_meta_t));
    ਜੇ (!meta)
    {
      hb_free (ਅੰਕੜੇ);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    meta->ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    meta->allocated = 0;
    meta->transferred = ਸੱਚ;
    meta->ਵਿੱਚ_replace = ਝੂਠ;
    new_meta = ਸੱਚ;
  }

  ਜੇ (reused_blob)
  {
    meta->ਵਿੱਚ_replace = ਸੱਚ;
    blob->replace_ਬਫਰ (ਅੰਕੜੇ, len, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, meta, hb_ਸਦਿਸ਼_blob_meta_destroy);
    hb_ਸਦਿਸ਼_blob_meta_ਸੈੱਟ_ਬਫਰ (meta, ਅੰਕੜੇ, allocated);
  }
  ਨਹੀਂਤਾਂ
  {
    hb_ਸਦਿਸ਼_blob_meta_ਸੈੱਟ_ਬਫਰ (meta, ਅੰਕੜੇ, allocated);
    blob = hb_blob_create_or_fail (ਅੰਕੜੇ, len, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, meta, hb_ਸਦਿਸ਼_blob_meta_destroy);
    ਜੇ (unlikely (!blob))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (unlikely (blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()))
  {
    ਜੇ (new_meta)
      hb_free (meta);
    hb_free (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (new_meta &&
      !hb_blob_ਸੈੱਟ_user_ਅੰਕੜੇ (blob,
                              &hb_ਸਦਿਸ਼_blob_meta_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
                              meta,
                              ਖਾਲੀ_ਸੂਚਕ,
                              ਸੱਚ))
  {
    ਜੇ (!reused_blob)
      hb_blob_destroy (blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (*recycled_blob)
    *recycled_blob = ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ blob;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_ਸਦਿਸ਼_buf_t *buf)
{
  ਜੇ (!blob)
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ *meta = (hb_ਸਦਿਸ਼_blob_meta_t *) hb_blob_get_user_ਅੰਕੜੇ (blob, &hb_ਸਦਿਸ਼_blob_meta_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
  ਜੇ (!meta || meta->transferred || !meta->ਅੰਕੜੇ)
    ਵਾਪਸ;

  buf->recycle_ਬਫਰ (meta->ਅੰਕੜੇ, 0, meta->allocated);
  meta->ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  meta->allocated = 0;
  meta->transferred = ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_transform_ਬਿੰਦੂ (ਅਚਲ hb_transform_t<> &t,
			   ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
			   ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ,
			   ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y,
			   ਦਸ਼ਮਲਵ *tx, ਦਸ਼ਮਲਵ *ty)
{
  ਦਸ਼ਮਲਵ xx = x, yy = y;
  t.transform_ਬਿੰਦੂ (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_ਗੁਣਕ;
  *ty = yy / y_scale_ਗੁਣਕ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_bool_t
hb_ਸਦਿਸ਼_ਸੈੱਟ_glyph_extents_common (ਅਚਲ hb_transform_t<> &transform,
				    ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
				    ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ,
				    ਅਚਲ hb_glyph_extents_t *glyph_extents,
				    hb_ਸਦਿਸ਼_extents_t *extents,
				    hb_bool_t *has_extents)
{
  ਦਸ਼ਮਲਵ x0 = (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing;
  ਦਸ਼ਮਲਵ y0 = (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing;
  ਦਸ਼ਮਲਵ x1 = x0 + glyph_extents->ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ y1 = y0 + glyph_extents->ਉਚਾਈ;

  ਦਸ਼ਮਲਵ px[4] = {x0, x0, x1, x1};
  ਦਸ਼ਮਲਵ py[4] = {y0, y1, y0, y1};

  ਦਸ਼ਮਲਵ tx, ty;
  hb_ਸਦਿਸ਼_transform_ਬਿੰਦੂ (transform, x_scale_ਗੁਣਕ, y_scale_ਗੁਣਕ, px[0], py[0], &tx, &ty);
  ਦਸ਼ਮਲਵ tx_min = tx, tx_max = tx;
  ਦਸ਼ਮਲਵ ty_min = ty, ty_max = ty;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 4; i++)
  {
    hb_ਸਦਿਸ਼_transform_ਬਿੰਦੂ (transform, x_scale_ਗੁਣਕ, y_scale_ਗੁਣਕ, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  ਜੇ (tx_max <= tx_min || ty_max <= ty_min)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (*has_extents)
  {
    ਦਸ਼ਮਲਵ x0 = hb_min (extents->x, tx_min);
    ਦਸ਼ਮਲਵ y0 = hb_min (extents->y, ty_min);
    ਦਸ਼ਮਲਵ x1 = hb_max (extents->x + extents->ਚੌੜਾਈ, tx_max);
    ਦਸ਼ਮਲਵ y1 = hb_max (extents->y + extents->ਉਚਾਈ, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ਨਹੀਂਤਾਂ
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = ਸੱਚ;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_append_instance_transform (hb_ਸਦਿਸ਼_buf_t *ਬਾਹਰਲਾ,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ precision,
					 ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
					 ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ,
					 ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
					 ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
					 ਦਸ਼ਮਲਵ tx, ਦਸ਼ਮਲਵ ty)
{
  ਜੇ (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    ਬਾਹਰਲਾ->append_str ("translate(");
    ਬਾਹਰਲਾ->append_ਅੰਕ (tx / x_scale_ਗੁਣਕ, precision);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (-ty / y_scale_ਗੁਣਕ, precision);
    ਬਾਹਰਲਾ->append_str (") scale(1,-1)");
  }
  ਨਹੀਂਤਾਂ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ sprec = ਬਾਹਰਲਾ->scale_precision ();
    ਬਾਹਰਲਾ->append_str ("matrix(");
    ਬਾਹਰਲਾ->append_ਅੰਕ (xx, sprec);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (yx, sprec);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (-xy, sprec);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (-yy, sprec);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (tx / x_scale_ਗੁਣਕ, precision);
    ਬਾਹਰਲਾ->append_c (',');
    ਬਾਹਰਲਾ->append_ਅੰਕ (-ty / y_scale_ਗੁਣਕ, precision);
    ਬਾਹਰਲਾ->append_c (')');
  }
}

#ਜੇ_ਅੰਤ /* HB_VECTOR_INTERNAL_HH */
