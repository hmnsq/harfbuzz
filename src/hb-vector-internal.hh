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

#اگر_غیر_تعریف_شدہ HB_سمتیہ_اندرونی_HH
#تعریف_کرو HB_سمتیہ_اندرونی_HH

#شامل "hb-vector.h"

ساخت hb_سمتیہ_blob_meta_t
{
  حرف *ڈیٹا;
  صحیح allocated;
  منطقی transferred;
  منطقی اندر_replace;
};

ساکن hb_user_ڈیٹا_کلید_t hb_سمتیہ_blob_meta_user_ڈیٹا_کلید;

ساکن اندرونی خالی
hb_سمتیہ_blob_meta_سیٹ_بفر (hb_سمتیہ_blob_meta_t *meta,
				حرف *ڈیٹا,
				صحیح allocated)
{
  meta->ڈیٹا = ڈیٹا;
  meta->allocated = allocated;
  meta->transferred = جھوٹ;
}

ساکن اندرونی خالی
hb_سمتیہ_blob_meta_release_بفر (hb_سمتیہ_blob_meta_t *meta)
{
  اگر (!meta)
    واپس;
  اگر (!meta->transferred && meta->ڈیٹا)
    hb_free (meta->ڈیٹا);
  meta->ڈیٹا = خالی_اشاریہ;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساکن اندرونی خالی
hb_سمتیہ_blob_meta_destroy (خالی *ڈیٹا)
{
  خودکار *meta = (hb_سمتیہ_blob_meta_t *) ڈیٹا;
  hb_سمتیہ_blob_meta_release_بفر (meta);
  اگر (meta->اندر_replace)
  {
    meta->اندر_replace = جھوٹ;
    واپس;
  }
  hb_free (meta);
}

ساکن اندرونی hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_سمتیہ_buf_t *buf)
{
  غیرعلامتی len = 0;
  صحیح allocated = 0;
  حرف *ڈیٹا = buf->steal (&len, &allocated);
  اگر (!ڈیٹا)
    واپس خالی_اشاریہ;

  hb_blob_t *blob = خالی_اشاریہ;
  اگر (*recycled_blob)
    blob = *recycled_blob;
  منطقی reused_blob = blob && blob != hb_blob_get_خالی_خانے ();
  منطقی new_meta = جھوٹ;
  خودکار *meta = reused_blob
             ? (hb_سمتیہ_blob_meta_t *) hb_blob_get_user_ڈیٹا (blob, &hb_سمتیہ_blob_meta_user_ڈیٹا_کلید)
             : خالی_اشاریہ;
  اگر (!meta)
  {
    meta = (hb_سمتیہ_blob_meta_t *) hb_malloc (حجم (hb_سمتیہ_blob_meta_t));
    اگر (!meta)
    {
      hb_free (ڈیٹا);
      واپس خالی_اشاریہ;
    }
    meta->ڈیٹا = خالی_اشاریہ;
    meta->allocated = 0;
    meta->transferred = سچ;
    meta->اندر_replace = جھوٹ;
    new_meta = سچ;
  }

  اگر (reused_blob)
  {
    meta->اندر_replace = سچ;
    blob->replace_بفر (ڈیٹا, len, HB_حافظہ_منوال_WRITABLE, meta, hb_سمتیہ_blob_meta_destroy);
    hb_سمتیہ_blob_meta_سیٹ_بفر (meta, ڈیٹا, allocated);
  }
  ورنہ
  {
    hb_سمتیہ_blob_meta_سیٹ_بفر (meta, ڈیٹا, allocated);
    blob = hb_blob_create_or_fail (ڈیٹا, len, HB_حافظہ_منوال_WRITABLE, meta, hb_سمتیہ_blob_meta_destroy);
    اگر (unlikely (!blob))
      واپس خالی_اشاریہ;
  }

  اگر (unlikely (blob == hb_blob_get_خالی_خانے ()))
  {
    اگر (new_meta)
      hb_free (meta);
    hb_free (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  اگر (new_meta &&
      !hb_blob_سیٹ_user_ڈیٹا (blob,
                              &hb_سمتیہ_blob_meta_user_ڈیٹا_کلید,
                              meta,
                              خالی_اشاریہ,
                              سچ))
  {
    اگر (!reused_blob)
      hb_blob_destroy (blob);
    واپس خالی_اشاریہ;
  }

  اگر (*recycled_blob)
    *recycled_blob = خالی_اشاریہ;

  واپس blob;
}

ساکن اندرونی خالی
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_سمتیہ_buf_t *buf)
{
  اگر (!blob)
    واپس;

  خودکار *meta = (hb_سمتیہ_blob_meta_t *) hb_blob_get_user_ڈیٹا (blob, &hb_سمتیہ_blob_meta_user_ڈیٹا_کلید);
  اگر (!meta || meta->transferred || !meta->ڈیٹا)
    واپس;

  buf->recycle_بفر (meta->ڈیٹا, 0, meta->allocated);
  meta->ڈیٹا = خالی_اشاریہ;
  meta->allocated = 0;
  meta->transferred = سچ;
}

ساکن اندرونی خالی
hb_سمتیہ_transform_نقطہ (ثابت hb_transform_t<> &t,
			   اعشاری x_scale_ضربی_عدد,
			   اعشاری y_scale_ضربی_عدد,
			   اعشاری x, اعشاری y,
			   اعشاری *tx, اعشاری *ty)
{
  اعشاری xx = x, yy = y;
  t.transform_نقطہ (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_ضربی_عدد;
  *ty = yy / y_scale_ضربی_عدد;
}

ساکن اندرونی hb_bool_t
hb_سمتیہ_سیٹ_glyph_extents_common (ثابت hb_transform_t<> &transform,
				    اعشاری x_scale_ضربی_عدد,
				    اعشاری y_scale_ضربی_عدد,
				    ثابت hb_glyph_extents_t *glyph_extents,
				    hb_سمتیہ_extents_t *extents,
				    hb_bool_t *has_extents)
{
  اعشاری x0 = (اعشاری) glyph_extents->x_bearing;
  اعشاری y0 = (اعشاری) glyph_extents->y_bearing;
  اعشاری x1 = x0 + glyph_extents->چوڑائی;
  اعشاری y1 = y0 + glyph_extents->اونچائی;

  اعشاری px[4] = {x0, x0, x1, x1};
  اعشاری py[4] = {y0, y1, y0, y1};

  اعشاری tx, ty;
  hb_سمتیہ_transform_نقطہ (transform, x_scale_ضربی_عدد, y_scale_ضربی_عدد, px[0], py[0], &tx, &ty);
  اعشاری tx_min = tx, tx_max = tx;
  اعشاری ty_min = ty, ty_max = ty;

  برائے (غیرعلامتی i = 1; i < 4; i++)
  {
    hb_سمتیہ_transform_نقطہ (transform, x_scale_ضربی_عدد, y_scale_ضربی_عدد, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  اگر (tx_max <= tx_min || ty_max <= ty_min)
    واپس جھوٹ;

  اگر (*has_extents)
  {
    اعشاری x0 = hb_min (extents->x, tx_min);
    اعشاری y0 = hb_min (extents->y, ty_min);
    اعشاری x1 = hb_max (extents->x + extents->چوڑائی, tx_max);
    اعشاری y1 = hb_max (extents->y + extents->اونچائی, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ورنہ
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = سچ;
  }
  واپس سچ;
}

ساکن اندرونی خالی
hb_سمتیہ_svg_append_instance_transform (hb_سمتیہ_buf_t *برآمد,
					 غیرعلامتی precision,
					 اعشاری x_scale_ضربی_عدد,
					 اعشاری y_scale_ضربی_عدد,
					 اعشاری xx, اعشاری yx,
					 اعشاری xy, اعشاری yy,
					 اعشاری tx, اعشاری ty)
{
  اگر (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    برآمد->append_str ("translate(");
    برآمد->append_عدد (tx / x_scale_ضربی_عدد, precision);
    برآمد->append_c (',');
    برآمد->append_عدد (-ty / y_scale_ضربی_عدد, precision);
    برآمد->append_str (") scale(1,-1)");
  }
  ورنہ
  {
    غیرعلامتی sprec = برآمد->scale_precision ();
    برآمد->append_str ("matrix(");
    برآمد->append_عدد (xx, sprec);
    برآمد->append_c (',');
    برآمد->append_عدد (yx, sprec);
    برآمد->append_c (',');
    برآمد->append_عدد (-xy, sprec);
    برآمد->append_c (',');
    برآمد->append_عدد (-yy, sprec);
    برآمد->append_c (',');
    برآمد->append_عدد (tx / x_scale_ضربی_عدد, precision);
    برآمد->append_c (',');
    برآمد->append_عدد (-ty / y_scale_ضربی_عدد, precision);
    برآمد->append_c (')');
  }
}

#اگر_ختم /* HB_VECTOR_INTERNAL_HH */
