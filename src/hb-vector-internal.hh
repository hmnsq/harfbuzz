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
  अक्षर *data;
  पूर्णांक allocated;
  तार्किक transferred;
  तार्किक in_replace;
};

स्थिर hb_user_data_किल्ली_t hb_vector_blob_meta_user_data_किल्ली;

स्थिर ओळीत शून्य
hb_vector_blob_meta_set_buffer (hb_vector_blob_meta_t *meta,
				अक्षर *data,
				पूर्णांक allocated)
{
  meta->data = data;
  meta->allocated = allocated;
  meta->transferred = असत्य;
}

स्थिर ओळीत शून्य
hb_vector_blob_meta_release_buffer (hb_vector_blob_meta_t *meta)
{
  जर (!meta)
    परत;
  जर (!meta->transferred && meta->data)
    hb_free (meta->data);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = सत्य;
}

स्थिर ओळीत शून्य
hb_vector_blob_meta_destroy (शून्य *data)
{
  स्वतः *meta = (hb_vector_blob_meta_t *) data;
  hb_vector_blob_meta_release_buffer (meta);
  जर (meta->in_replace)
  {
    meta->in_replace = असत्य;
    परत;
  }
  hb_free (meta);
}

स्थिर ओळीत hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  अचिन्हित len = 0;
  पूर्णांक allocated = 0;
  अक्षर *data = buf->steal (&len, &allocated);
  जर (!data)
    परत nullptr;

  hb_blob_t *blob = nullptr;
  जर (*recycled_blob)
    blob = *recycled_blob;
  तार्किक reused_blob = blob && blob != hb_blob_get_empty ();
  तार्किक new_meta = असत्य;
  स्वतः *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_किल्ली)
             : nullptr;
  जर (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (आकार (hb_vector_blob_meta_t));
    जर (!meta)
    {
      hb_free (data);
      परत nullptr;
    }
    meta->data = nullptr;
    meta->allocated = 0;
    meta->transferred = सत्य;
    meta->in_replace = असत्य;
    new_meta = सत्य;
  }

  जर (reused_blob)
  {
    meta->in_replace = सत्य;
    blob->replace_buffer (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
  }
  अन्यथा
  {
    hb_vector_blob_meta_set_buffer (meta, data, allocated);
    blob = hb_blob_create_or_fail (data, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    जर (unlikely (!blob))
      परत nullptr;
  }

  जर (unlikely (blob == hb_blob_get_empty ()))
  {
    जर (new_meta)
      hb_free (meta);
    hb_free (data);
    परत nullptr;
  }

  जर (new_meta &&
      !hb_blob_set_user_data (blob,
                              &hb_vector_blob_meta_user_data_किल्ली,
                              meta,
                              nullptr,
                              सत्य))
  {
    जर (!reused_blob)
      hb_blob_destroy (blob);
    परत nullptr;
  }

  जर (*recycled_blob)
    *recycled_blob = nullptr;

  परत blob;
}

स्थिर ओळीत शून्य
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  जर (!blob)
    परत;

  स्वतः *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_data (blob, &hb_vector_blob_meta_user_data_किल्ली);
  जर (!meta || meta->transferred || !meta->data)
    परत;

  buf->recycle_buffer (meta->data, 0, meta->allocated);
  meta->data = nullptr;
  meta->allocated = 0;
  meta->transferred = सत्य;
}

स्थिर ओळीत शून्य
hb_vector_transform_point (स्थिरांक hb_transform_t<> &t,
			   दशांश x_scale_factor,
			   दशांश y_scale_factor,
			   दशांश x, दशांश y,
			   दशांश *tx, दशांश *ty)
{
  दशांश xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

स्थिर ओळीत hb_bool_t
hb_vector_set_glyph_extents_common (स्थिरांक hb_transform_t<> &transform,
				    दशांश x_scale_factor,
				    दशांश y_scale_factor,
				    स्थिरांक hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  दशांश x0 = (दशांश) glyph_extents->x_bearing;
  दशांश y0 = (दशांश) glyph_extents->y_bearing;
  दशांश x1 = x0 + glyph_extents->width;
  दशांश y1 = y0 + glyph_extents->height;

  दशांश px[4] = {x0, x0, x1, x1};
  दशांश py[4] = {y0, y1, y0, y1};

  दशांश tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  दशांश tx_min = tx, tx_max = tx;
  दशांश ty_min = ty, ty_max = ty;

  साठी (अचिन्हित i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  जर (tx_max <= tx_min || ty_max <= ty_min)
    परत असत्य;

  जर (*has_extents)
  {
    दशांश x0 = hb_min (extents->x, tx_min);
    दशांश y0 = hb_min (extents->y, ty_min);
    दशांश x1 = hb_max (extents->x + extents->width, tx_max);
    दशांश y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  अन्यथा
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = सत्य;
  }
  परत सत्य;
}

स्थिर ओळीत शून्य
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 अचिन्हित precision,
					 दशांश x_scale_factor,
					 दशांश y_scale_factor,
					 दशांश xx, दशांश yx,
					 दशांश xy, दशांश yy,
					 दशांश tx, दशांश ty)
{
  जर (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    out->append_str ("translate(");
    out->append_अंक (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_अंक (-ty / y_scale_factor, precision);
    out->append_str (") scale(1,-1)");
  }
  अन्यथा
  {
    अचिन्हित sprec = out->scale_precision ();
    out->append_str ("matrix(");
    out->append_अंक (xx, sprec);
    out->append_c (',');
    out->append_अंक (yx, sprec);
    out->append_c (',');
    out->append_अंक (-xy, sprec);
    out->append_c (',');
    out->append_अंक (-yy, sprec);
    out->append_c (',');
    out->append_अंक (tx / x_scale_factor, precision);
    out->append_c (',');
    out->append_अंक (-ty / y_scale_factor, precision);
    out->append_c (')');
  }
}

#endif /* HB_VECTOR_INTERNAL_HH */
