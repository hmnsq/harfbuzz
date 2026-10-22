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

#جے_بنا_تعریف HB_سمتیہ_اندرونی_HH
#تعریف_کرو HB_سمتیہ_اندرونی_HH

#ضمن "hb-vector.h"

بنية hb_سمتیہ_blob_meta_t
{
  حرف *انکڑے;
  صحيح allocated;
  منطقي transferred;
  منطقي وچ_replace;
};

ساكن hb_user_انکڑے_کلید_t hb_سمتیہ_blob_meta_user_انکڑے_کلید;

ساكن مضمن فراغ
hb_سمتیہ_blob_meta_سیٹ_بفر (hb_سمتیہ_blob_meta_t *meta,
				حرف *انکڑے,
				صحيح allocated)
{
  meta->انکڑے = انکڑے;
  meta->allocated = allocated;
  meta->transferred = خطأ;
}

ساكن مضمن فراغ
hb_سمتیہ_blob_meta_release_بفر (hb_سمتیہ_blob_meta_t *meta)
{
  إذا (!meta)
    أرجع;
  إذا (!meta->transferred && meta->انکڑے)
    hb_free (meta->انکڑے);
  meta->انکڑے = خالی_سوچک;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساكن مضمن فراغ
hb_سمتیہ_blob_meta_destroy (فراغ *انکڑے)
{
  خودکار *meta = (hb_سمتیہ_blob_meta_t *) انکڑے;
  hb_سمتیہ_blob_meta_release_بفر (meta);
  إذا (meta->وچ_replace)
  {
    meta->وچ_replace = خطأ;
    أرجع;
  }
  hb_free (meta);
}

ساكن مضمن hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_سمتیہ_buf_t *buf)
{
  بنانشانی len = 0;
  صحيح allocated = 0;
  حرف *انکڑے = buf->steal (&len, &allocated);
  إذا (!انکڑے)
    أرجع خالی_سوچک;

  hb_blob_t *blob = خالی_سوچک;
  إذا (*recycled_blob)
    blob = *recycled_blob;
  منطقي reused_blob = blob && blob != hb_blob_get_خالی_تھاواں ();
  منطقي new_meta = خطأ;
  خودکار *meta = reused_blob
             ? (hb_سمتیہ_blob_meta_t *) hb_blob_get_user_انکڑے (blob, &hb_سمتیہ_blob_meta_user_انکڑے_کلید)
             : خالی_سوچک;
  إذا (!meta)
  {
    meta = (hb_سمتیہ_blob_meta_t *) hb_malloc (حجم (hb_سمتیہ_blob_meta_t));
    إذا (!meta)
    {
      hb_free (انکڑے);
      أرجع خالی_سوچک;
    }
    meta->انکڑے = خالی_سوچک;
    meta->allocated = 0;
    meta->transferred = سچ;
    meta->وچ_replace = خطأ;
    new_meta = سچ;
  }

  إذا (reused_blob)
  {
    meta->وچ_replace = سچ;
    blob->replace_بفر (انکڑے, len, HB_حافظہ_منوال_WRITABLE, meta, hb_سمتیہ_blob_meta_destroy);
    hb_سمتیہ_blob_meta_سیٹ_بفر (meta, انکڑے, allocated);
  }
  وإلا
  {
    hb_سمتیہ_blob_meta_سیٹ_بفر (meta, انکڑے, allocated);
    blob = hb_blob_create_or_fail (انکڑے, len, HB_حافظہ_منوال_WRITABLE, meta, hb_سمتیہ_blob_meta_destroy);
    إذا (unlikely (!blob))
      أرجع خالی_سوچک;
  }

  إذا (unlikely (blob == hb_blob_get_خالی_تھاواں ()))
  {
    إذا (new_meta)
      hb_free (meta);
    hb_free (انکڑے);
    أرجع خالی_سوچک;
  }

  إذا (new_meta &&
      !hb_blob_سیٹ_user_انکڑے (blob,
                              &hb_سمتیہ_blob_meta_user_انکڑے_کلید,
                              meta,
                              خالی_سوچک,
                              سچ))
  {
    إذا (!reused_blob)
      hb_blob_destroy (blob);
    أرجع خالی_سوچک;
  }

  إذا (*recycled_blob)
    *recycled_blob = خالی_سوچک;

  أرجع blob;
}

ساكن مضمن فراغ
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_سمتیہ_buf_t *buf)
{
  إذا (!blob)
    أرجع;

  خودکار *meta = (hb_سمتیہ_blob_meta_t *) hb_blob_get_user_انکڑے (blob, &hb_سمتیہ_blob_meta_user_انکڑے_کلید);
  إذا (!meta || meta->transferred || !meta->انکڑے)
    أرجع;

  buf->recycle_بفر (meta->انکڑے, 0, meta->allocated);
  meta->انکڑے = خالی_سوچک;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساكن مضمن فراغ
hb_سمتیہ_transform_نقطہ (ثابت hb_transform_t<> &t,
			   عائم x_scale_ضارب,
			   عائم y_scale_ضارب,
			   عائم x, عائم y,
			   عائم *tx, عائم *ty)
{
  عائم xx = x, yy = y;
  t.transform_نقطہ (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_ضارب;
  *ty = yy / y_scale_ضارب;
}

ساكن مضمن hb_bool_t
hb_سمتیہ_سیٹ_glyph_extents_common (ثابت hb_transform_t<> &transform,
				    عائم x_scale_ضارب,
				    عائم y_scale_ضارب,
				    ثابت hb_glyph_extents_t *glyph_extents,
				    hb_سمتیہ_extents_t *extents,
				    hb_bool_t *has_extents)
{
  عائم x0 = (عائم) glyph_extents->x_bearing;
  عائم y0 = (عائم) glyph_extents->y_bearing;
  عائم x1 = x0 + glyph_extents->چوڑائی;
  عائم y1 = y0 + glyph_extents->اچائی;

  عائم px[4] = {x0, x0, x1, x1};
  عائم py[4] = {y0, y1, y0, y1};

  عائم tx, ty;
  hb_سمتیہ_transform_نقطہ (transform, x_scale_ضارب, y_scale_ضارب, px[0], py[0], &tx, &ty);
  عائم tx_min = tx, tx_max = tx;
  عائم ty_min = ty, ty_max = ty;

  لكل (بنانشانی i = 1; i < 4; i++)
  {
    hb_سمتیہ_transform_نقطہ (transform, x_scale_ضارب, y_scale_ضارب, px[i], py[i], &tx, &ty);
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
    عائم x1 = hb_max (extents->x + extents->چوڑائی, tx_max);
    عائم y1 = hb_max (extents->y + extents->اچائی, ty_max);
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
hb_سمتیہ_svg_append_instance_transform (hb_سمتیہ_buf_t *باہرلا,
					 بنانشانی precision,
					 عائم x_scale_ضارب,
					 عائم y_scale_ضارب,
					 عائم xx, عائم yx,
					 عائم xy, عائم yy,
					 عائم tx, عائم ty)
{
  إذا (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    باہرلا->append_str ("translate(");
    باہرلا->append_عدد (tx / x_scale_ضارب, precision);
    باہرلا->append_c (',');
    باہرلا->append_عدد (-ty / y_scale_ضارب, precision);
    باہرلا->append_str (") scale(1,-1)");
  }
  وإلا
  {
    بنانشانی sprec = باہرلا->scale_precision ();
    باہرلا->append_str ("matrix(");
    باہرلا->append_عدد (xx, sprec);
    باہرلا->append_c (',');
    باہرلا->append_عدد (yx, sprec);
    باہرلا->append_c (',');
    باہرلا->append_عدد (-xy, sprec);
    باہرلا->append_c (',');
    باہرلا->append_عدد (-yy, sprec);
    باہرلا->append_c (',');
    باہرلا->append_عدد (tx / x_scale_ضارب, precision);
    باہرلا->append_c (',');
    باہرلا->append_عدد (-ty / y_scale_ضارب, precision);
    باہرلا->append_c (')');
  }
}

#جے_انت /* HB_VECTOR_INTERNAL_HH */
