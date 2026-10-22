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

#جيڪڏهن_وصف_نه_ٿيل HB_ويڪٽر_اندروني_HH
#وصف_ڪريو HB_ويڪٽر_اندروني_HH

#ضمن "hb-vector.h"

بنية hb_ويڪٽر_blob_meta_t
{
  حرف *معلومات;
  صحيح allocated;
  منطقي transferred;
  منطقي اندريون_replace;
};

ساكن hb_user_معلومات_ڪنجي_t hb_ويڪٽر_blob_meta_user_معلومات_ڪنجي;

ساكن مضمن فراغ
hb_ويڪٽر_blob_meta_سيٽ_بفر (hb_ويڪٽر_blob_meta_t *meta,
				حرف *معلومات,
				صحيح allocated)
{
  meta->معلومات = معلومات;
  meta->allocated = allocated;
  meta->transferred = خطأ;
}

ساكن مضمن فراغ
hb_ويڪٽر_blob_meta_release_بفر (hb_ويڪٽر_blob_meta_t *meta)
{
  إذا (!meta)
    أرجع;
  إذا (!meta->transferred && meta->معلومات)
    hb_free (meta->معلومات);
  meta->معلومات = خالي_اشارو;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساكن مضمن فراغ
hb_ويڪٽر_blob_meta_destroy (فراغ *معلومات)
{
  خودڪار *meta = (hb_ويڪٽر_blob_meta_t *) معلومات;
  hb_ويڪٽر_blob_meta_release_بفر (meta);
  إذا (meta->اندريون_replace)
  {
    meta->اندريون_replace = خطأ;
    أرجع;
  }
  hb_free (meta);
}

ساكن مضمن hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_ويڪٽر_buf_t *buf)
{
  بنانشان len = 0;
  صحيح allocated = 0;
  حرف *معلومات = buf->steal (&len, &allocated);
  إذا (!معلومات)
    أرجع خالي_اشارو;

  hb_blob_t *blob = خالي_اشارو;
  إذا (*recycled_blob)
    blob = *recycled_blob;
  منطقي reused_blob = blob && blob != hb_blob_get_خالي_جايون ();
  منطقي new_meta = خطأ;
  خودڪار *meta = reused_blob
             ? (hb_ويڪٽر_blob_meta_t *) hb_blob_get_user_معلومات (blob, &hb_ويڪٽر_blob_meta_user_معلومات_ڪنجي)
             : خالي_اشارو;
  إذا (!meta)
  {
    meta = (hb_ويڪٽر_blob_meta_t *) hb_malloc (حجم (hb_ويڪٽر_blob_meta_t));
    إذا (!meta)
    {
      hb_free (معلومات);
      أرجع خالي_اشارو;
    }
    meta->معلومات = خالي_اشارو;
    meta->allocated = 0;
    meta->transferred = سچ;
    meta->اندريون_replace = خطأ;
    new_meta = سچ;
  }

  إذا (reused_blob)
  {
    meta->اندريون_replace = سچ;
    blob->replace_بفر (معلومات, len, HB_يادگيري_گهڻائي_WRITABLE, meta, hb_ويڪٽر_blob_meta_destroy);
    hb_ويڪٽر_blob_meta_سيٽ_بفر (meta, معلومات, allocated);
  }
  وإلا
  {
    hb_ويڪٽر_blob_meta_سيٽ_بفر (meta, معلومات, allocated);
    blob = hb_blob_create_or_fail (معلومات, len, HB_يادگيري_گهڻائي_WRITABLE, meta, hb_ويڪٽر_blob_meta_destroy);
    إذا (unlikely (!blob))
      أرجع خالي_اشارو;
  }

  إذا (unlikely (blob == hb_blob_get_خالي_جايون ()))
  {
    إذا (new_meta)
      hb_free (meta);
    hb_free (معلومات);
    أرجع خالي_اشارو;
  }

  إذا (new_meta &&
      !hb_blob_سيٽ_user_معلومات (blob,
                              &hb_ويڪٽر_blob_meta_user_معلومات_ڪنجي,
                              meta,
                              خالي_اشارو,
                              سچ))
  {
    إذا (!reused_blob)
      hb_blob_destroy (blob);
    أرجع خالي_اشارو;
  }

  إذا (*recycled_blob)
    *recycled_blob = خالي_اشارو;

  أرجع blob;
}

ساكن مضمن فراغ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_ويڪٽر_buf_t *buf)
{
  إذا (!blob)
    أرجع;

  خودڪار *meta = (hb_ويڪٽر_blob_meta_t *) hb_blob_get_user_معلومات (blob, &hb_ويڪٽر_blob_meta_user_معلومات_ڪنجي);
  إذا (!meta || meta->transferred || !meta->معلومات)
    أرجع;

  buf->recycle_بفر (meta->معلومات, 0, meta->allocated);
  meta->معلومات = خالي_اشارو;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساكن مضمن فراغ
hb_ويڪٽر_transform_نقطو (ثابت hb_transform_t<> &t,
			   عائم x_scale_ضرب_ڪندڙ,
			   عائم y_scale_ضرب_ڪندڙ,
			   عائم x, عائم y,
			   عائم *tx, عائم *ty)
{
  عائم xx = x, yy = y;
  t.transform_نقطو (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_ضرب_ڪندڙ;
  *ty = yy / y_scale_ضرب_ڪندڙ;
}

ساكن مضمن hb_bool_t
hb_ويڪٽر_سيٽ_glyph_extents_common (ثابت hb_transform_t<> &transform,
				    عائم x_scale_ضرب_ڪندڙ,
				    عائم y_scale_ضرب_ڪندڙ,
				    ثابت hb_glyph_extents_t *glyph_extents,
				    hb_ويڪٽر_extents_t *extents,
				    hb_bool_t *has_extents)
{
  عائم x0 = (عائم) glyph_extents->x_bearing;
  عائم y0 = (عائم) glyph_extents->y_bearing;
  عائم x1 = x0 + glyph_extents->ويڪر;
  عائم y1 = y0 + glyph_extents->اوچائي;

  عائم px[4] = {x0, x0, x1, x1};
  عائم py[4] = {y0, y1, y0, y1};

  عائم tx, ty;
  hb_ويڪٽر_transform_نقطو (transform, x_scale_ضرب_ڪندڙ, y_scale_ضرب_ڪندڙ, px[0], py[0], &tx, &ty);
  عائم tx_min = tx, tx_max = tx;
  عائم ty_min = ty, ty_max = ty;

  لكل (بنانشان i = 1; i < 4; i++)
  {
    hb_ويڪٽر_transform_نقطو (transform, x_scale_ضرب_ڪندڙ, y_scale_ضرب_ڪندڙ, px[i], py[i], &tx, &ty);
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
    عائم x1 = hb_max (extents->x + extents->ويڪر, tx_max);
    عائم y1 = hb_max (extents->y + extents->اوچائي, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  وإلا
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = سچ;
  }
  أرجع سچ;
}

ساكن مضمن فراغ
hb_ويڪٽر_svg_append_instance_transform (hb_ويڪٽر_buf_t *ٻاهريون,
					 بنانشان precision,
					 عائم x_scale_ضرب_ڪندڙ,
					 عائم y_scale_ضرب_ڪندڙ,
					 عائم xx, عائم yx,
					 عائم xy, عائم yy,
					 عائم tx, عائم ty)
{
  إذا (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    ٻاهريون->append_str ("translate(");
    ٻاهريون->append_انگ (tx / x_scale_ضرب_ڪندڙ, precision);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (-ty / y_scale_ضرب_ڪندڙ, precision);
    ٻاهريون->append_str (") scale(1,-1)");
  }
  وإلا
  {
    بنانشان sprec = ٻاهريون->scale_precision ();
    ٻاهريون->append_str ("matrix(");
    ٻاهريون->append_انگ (xx, sprec);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (yx, sprec);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (-xy, sprec);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (-yy, sprec);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (tx / x_scale_ضرب_ڪندڙ, precision);
    ٻاهريون->append_c (',');
    ٻاهريون->append_انگ (-ty / y_scale_ضرب_ڪندڙ, precision);
    ٻاهريون->append_c (')');
  }
}

#جيڪڏهن_پورو /* HB_VECTOR_INTERNAL_HH */
