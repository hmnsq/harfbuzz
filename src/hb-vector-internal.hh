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

#اگر_تعریف_نشده HB_وکتور_داخلی_HH
#تعریف_کن HB_وکتور_داخلی_HH

#ضمن "hb-vector.h"

بنية hb_وکتور_blob_meta_t
{
  حرف *داده;
  صحيح allocated;
  منطقي transferred;
  منطقي درون_replace;
};

ساكن hb_user_داده_کلید_t hb_وکتور_blob_meta_user_داده_کلید;

ساكن مضمن فراغ
hb_وکتور_blob_meta_مجموعه_میانگیر (hb_وکتور_blob_meta_t *meta,
				حرف *داده,
				صحيح allocated)
{
  meta->داده = داده;
  meta->allocated = allocated;
  meta->transferred = خطأ;
}

ساكن مضمن فراغ
hb_وکتور_blob_meta_release_میانگیر (hb_وکتور_blob_meta_t *meta)
{
  إذا (!meta)
    أرجع;
  إذا (!meta->transferred && meta->داده)
    hb_free (meta->داده);
  meta->داده = اشاره_تهی;
  meta->allocated = 0;
  meta->transferred = درست;
}

ساكن مضمن فراغ
hb_وکتور_blob_meta_destroy (فراغ *داده)
{
  خودکار *meta = (hb_وکتور_blob_meta_t *) داده;
  hb_وکتور_blob_meta_release_میانگیر (meta);
  إذا (meta->درون_replace)
  {
    meta->درون_replace = خطأ;
    أرجع;
  }
  hb_free (meta);
}

ساكن مضمن hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_وکتور_buf_t *buf)
{
  بیعلامت len = 0;
  صحيح allocated = 0;
  حرف *داده = buf->steal (&len, &allocated);
  إذا (!داده)
    أرجع اشاره_تهی;

  hb_blob_t *blob = اشاره_تهی;
  إذا (*recycled_blob)
    blob = *recycled_blob;
  منطقي reused_blob = blob && blob != hb_blob_get_خالی ();
  منطقي new_meta = خطأ;
  خودکار *meta = reused_blob
             ? (hb_وکتور_blob_meta_t *) hb_blob_get_user_داده (blob, &hb_وکتور_blob_meta_user_داده_کلید)
             : اشاره_تهی;
  إذا (!meta)
  {
    meta = (hb_وکتور_blob_meta_t *) hb_malloc (حجم (hb_وکتور_blob_meta_t));
    إذا (!meta)
    {
      hb_free (داده);
      أرجع اشاره_تهی;
    }
    meta->داده = اشاره_تهی;
    meta->allocated = 0;
    meta->transferred = درست;
    meta->درون_replace = خطأ;
    new_meta = درست;
  }

  إذا (reused_blob)
  {
    meta->درون_replace = درست;
    blob->replace_میانگیر (داده, len, HB_حافظه_نما_WRITABLE, meta, hb_وکتور_blob_meta_destroy);
    hb_وکتور_blob_meta_مجموعه_میانگیر (meta, داده, allocated);
  }
  وإلا
  {
    hb_وکتور_blob_meta_مجموعه_میانگیر (meta, داده, allocated);
    blob = hb_blob_create_or_fail (داده, len, HB_حافظه_نما_WRITABLE, meta, hb_وکتور_blob_meta_destroy);
    إذا (unlikely (!blob))
      أرجع اشاره_تهی;
  }

  إذا (unlikely (blob == hb_blob_get_خالی ()))
  {
    إذا (new_meta)
      hb_free (meta);
    hb_free (داده);
    أرجع اشاره_تهی;
  }

  إذا (new_meta &&
      !hb_blob_مجموعه_user_داده (blob,
                              &hb_وکتور_blob_meta_user_داده_کلید,
                              meta,
                              اشاره_تهی,
                              درست))
  {
    إذا (!reused_blob)
      hb_blob_destroy (blob);
    أرجع اشاره_تهی;
  }

  إذا (*recycled_blob)
    *recycled_blob = اشاره_تهی;

  أرجع blob;
}

ساكن مضمن فراغ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_وکتور_buf_t *buf)
{
  إذا (!blob)
    أرجع;

  خودکار *meta = (hb_وکتور_blob_meta_t *) hb_blob_get_user_داده (blob, &hb_وکتور_blob_meta_user_داده_کلید);
  إذا (!meta || meta->transferred || !meta->داده)
    أرجع;

  buf->recycle_میانگیر (meta->داده, 0, meta->allocated);
  meta->داده = اشاره_تهی;
  meta->allocated = 0;
  meta->transferred = درست;
}

ساكن مضمن فراغ
hb_وکتور_transform_نقطه (ثابت hb_transform_t<> &t,
			   عائم x_scale_ضریب,
			   عائم y_scale_ضریب,
			   عائم x, عائم y,
			   عائم *tx, عائم *ty)
{
  عائم xx = x, yy = y;
  t.transform_نقطه (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_ضریب;
  *ty = yy / y_scale_ضریب;
}

ساكن مضمن hb_bool_t
hb_وکتور_مجموعه_glyph_extents_common (ثابت hb_transform_t<> &transform,
				    عائم x_scale_ضریب,
				    عائم y_scale_ضریب,
				    ثابت hb_glyph_extents_t *glyph_extents,
				    hb_وکتور_extents_t *extents,
				    hb_bool_t *has_extents)
{
  عائم x0 = (عائم) glyph_extents->x_bearing;
  عائم y0 = (عائم) glyph_extents->y_bearing;
  عائم x1 = x0 + glyph_extents->عرض;
  عائم y1 = y0 + glyph_extents->ارتفاع;

  عائم px[4] = {x0, x0, x1, x1};
  عائم py[4] = {y0, y1, y0, y1};

  عائم tx, ty;
  hb_وکتور_transform_نقطه (transform, x_scale_ضریب, y_scale_ضریب, px[0], py[0], &tx, &ty);
  عائم tx_min = tx, tx_max = tx;
  عائم ty_min = ty, ty_max = ty;

  لكل (بیعلامت i = 1; i < 4; i++)
  {
    hb_وکتور_transform_نقطه (transform, x_scale_ضریب, y_scale_ضریب, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  إذا (tx_max <= tx_min || ty_max <= ty_min)
    أرجع خطأ;

  إذا (*has_extents)
  {
    عائم x0 = hb_min (extents->x, tx_min);
    عائم y0 = hb_min (extents->y, ty_min);
    عائم x1 = hb_max (extents->x + extents->عرض, tx_max);
    عائم y1 = hb_max (extents->y + extents->ارتفاع, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  وإلا
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = درست;
  }
  أرجع درست;
}

ساكن مضمن فراغ
hb_وکتور_svg_append_instance_transform (hb_وکتور_buf_t *بیرون,
					 بیعلامت precision,
					 عائم x_scale_ضریب,
					 عائم y_scale_ضریب,
					 عائم xx, عائم yx,
					 عائم xy, عائم yy,
					 عائم tx, عائم ty)
{
  إذا (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    بیرون->append_str ("translate(");
    بیرون->append_رقم (tx / x_scale_ضریب, precision);
    بیرون->append_c (',');
    بیرون->append_رقم (-ty / y_scale_ضریب, precision);
    بیرون->append_str (") scale(1,-1)");
  }
  وإلا
  {
    بیعلامت sprec = بیرون->scale_precision ();
    بیرون->append_str ("matrix(");
    بیرون->append_رقم (xx, sprec);
    بیرون->append_c (',');
    بیرون->append_رقم (yx, sprec);
    بیرون->append_c (',');
    بیرون->append_رقم (-xy, sprec);
    بیرون->append_c (',');
    بیرون->append_رقم (-yy, sprec);
    بیرون->append_c (',');
    بیرون->append_رقم (tx / x_scale_ضریب, precision);
    بیرون->append_c (',');
    بیرون->append_رقم (-ty / y_scale_ضریب, precision);
    بیرون->append_c (')');
  }
}

#پایان_اگر /* HB_VECTOR_INTERNAL_HH */
