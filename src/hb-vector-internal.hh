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

#অন্তর্ভাব "hb-vector.h"

কাঠামো hb_vector_blob_meta_t
{
  অক্ষর *তথ্য;
  পূর্ণসংখ্যা allocated;
  যৌক্তিক transferred;
  যৌক্তিক in_replace;
};

স্থির hb_user_তথ্য_চাবি_t hb_vector_blob_meta_user_তথ্য_চাবি;

স্থির অন্তঃরেখ শূন্য
hb_vector_blob_meta_set_বাফার (hb_vector_blob_meta_t *meta,
				অক্ষর *তথ্য,
				পূর্ণসংখ্যা allocated)
{
  meta->তথ্য = তথ্য;
  meta->allocated = allocated;
  meta->transferred = মিথ্যা;
}

স্থির অন্তঃরেখ শূন্য
hb_vector_blob_meta_release_বাফার (hb_vector_blob_meta_t *meta)
{
  যদি (!meta)
    ফেরত;
  যদি (!meta->transferred && meta->তথ্য)
    hb_free (meta->তথ্য);
  meta->তথ্য = nullptr;
  meta->allocated = 0;
  meta->transferred = সত্য;
}

স্থির অন্তঃরেখ শূন্য
hb_vector_blob_meta_destroy (শূন্য *তথ্য)
{
  auto *meta = (hb_vector_blob_meta_t *) তথ্য;
  hb_vector_blob_meta_release_বাফার (meta);
  যদি (meta->in_replace)
  {
    meta->in_replace = মিথ্যা;
    ফেরত;
  }
  hb_free (meta);
}

স্থির অন্তঃরেখ hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  unsigned len = 0;
  পূর্ণসংখ্যা allocated = 0;
  অক্ষর *তথ্য = buf->steal (&len, &allocated);
  যদি (!তথ্য)
    ফেরত nullptr;

  hb_blob_t *blob = nullptr;
  যদি (*recycled_blob)
    blob = *recycled_blob;
  যৌক্তিক reused_blob = blob && blob != hb_blob_get_empty ();
  যৌক্তিক new_meta = মিথ্যা;
  auto *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_তথ্য (blob, &hb_vector_blob_meta_user_তথ্য_চাবি)
             : nullptr;
  যদি (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (আকার (hb_vector_blob_meta_t));
    যদি (!meta)
    {
      hb_free (তথ্য);
      ফেরত nullptr;
    }
    meta->তথ্য = nullptr;
    meta->allocated = 0;
    meta->transferred = সত্য;
    meta->in_replace = মিথ্যা;
    new_meta = সত্য;
  }

  যদি (reused_blob)
  {
    meta->in_replace = সত্য;
    blob->replace_বাফার (তথ্য, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_বাফার (meta, তথ্য, allocated);
  }
  অন্যথা
  {
    hb_vector_blob_meta_set_বাফার (meta, তথ্য, allocated);
    blob = hb_blob_create_or_fail (তথ্য, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    যদি (unlikely (!blob))
      ফেরত nullptr;
  }

  যদি (unlikely (blob == hb_blob_get_empty ()))
  {
    যদি (new_meta)
      hb_free (meta);
    hb_free (তথ্য);
    ফেরত nullptr;
  }

  যদি (new_meta &&
      !hb_blob_set_user_তথ্য (blob,
                              &hb_vector_blob_meta_user_তথ্য_চাবি,
                              meta,
                              nullptr,
                              সত্য))
  {
    যদি (!reused_blob)
      hb_blob_destroy (blob);
    ফেরত nullptr;
  }

  যদি (*recycled_blob)
    *recycled_blob = nullptr;

  ফেরত blob;
}

স্থির অন্তঃরেখ শূন্য
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  যদি (!blob)
    ফেরত;

  auto *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_তথ্য (blob, &hb_vector_blob_meta_user_তথ্য_চাবি);
  যদি (!meta || meta->transferred || !meta->তথ্য)
    ফেরত;

  buf->recycle_বাফার (meta->তথ্য, 0, meta->allocated);
  meta->তথ্য = nullptr;
  meta->allocated = 0;
  meta->transferred = সত্য;
}

স্থির অন্তঃরেখ শূন্য
hb_vector_transform_point (ধ্রুবক hb_transform_t<> &t,
			   দশমিক x_scale_factor,
			   দশমিক y_scale_factor,
			   দশমিক x, দশমিক y,
			   দশমিক *tx, দশমিক *ty)
{
  দশমিক xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

স্থির অন্তঃরেখ hb_bool_t
hb_vector_set_glyph_extents_common (ধ্রুবক hb_transform_t<> &transform,
				    দশমিক x_scale_factor,
				    দশমিক y_scale_factor,
				    ধ্রুবক hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  দশমিক x0 = (দশমিক) glyph_extents->x_bearing;
  দশমিক y0 = (দশমিক) glyph_extents->y_bearing;
  দশমিক x1 = x0 + glyph_extents->width;
  দশমিক y1 = y0 + glyph_extents->height;

  দশমিক px[4] = {x0, x0, x1, x1};
  দশমিক py[4] = {y0, y1, y0, y1};

  দশমিক tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  দশমিক tx_min = tx, tx_max = tx;
  দশমিক ty_min = ty, ty_max = ty;

  জন্য (unsigned i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  যদি (tx_max <= tx_min || ty_max <= ty_min)
    ফেরত মিথ্যা;

  যদি (*has_extents)
  {
    দশমিক x0 = hb_min (extents->x, tx_min);
    দশমিক y0 = hb_min (extents->y, ty_min);
    দশমিক x1 = hb_max (extents->x + extents->width, tx_max);
    দশমিক y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  অন্যথা
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = সত্য;
  }
  ফেরত সত্য;
}

স্থির অন্তঃরেখ শূন্য
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 unsigned precision,
					 দশমিক x_scale_factor,
					 দশমিক y_scale_factor,
					 দশমিক xx, দশমিক yx,
					 দশমিক xy, দশমিক yy,
					 দশমিক tx, দশমিক ty)
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
