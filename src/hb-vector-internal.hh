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

#నిర్వచితం_కాకపోతే HB_సదిశ_అంతర్గత_HH
#నిర్వచించు HB_సదిశ_అంతర్గత_HH

#అన్తర్భావ "hb-vector.h"

నిర్మాణం hb_సదిశ_blob_meta_t
{
  అక్షరం *దత్తాంశం;
  పూర్ణాంకం allocated;
  తార్కిక transferred;
  తార్కిక లోపల_replace;
};

స్థిర hb_user_దత్తాంశం_కీలకం_t hb_సదిశ_blob_meta_user_దత్తాంశం_కీలకం;

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_blob_meta_సమితి_బఫరు (hb_సదిశ_blob_meta_t *meta,
				అక్షరం *దత్తాంశం,
				పూర్ణాంకం allocated)
{
  meta->దత్తాంశం = దత్తాంశం;
  meta->allocated = allocated;
  meta->transferred = అబద్ధం;
}

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_blob_meta_release_బఫరు (hb_సదిశ_blob_meta_t *meta)
{
  ఒకవేళ (!meta)
    తిరిగి;
  ఒకవేళ (!meta->transferred && meta->దత్తాంశం)
    hb_free (meta->దత్తాంశం);
  meta->దత్తాంశం = శూన్యసూచిక;
  meta->allocated = 0;
  meta->transferred = నిజం;
}

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_blob_meta_destroy (శూన్య *దత్తాంశం)
{
  స్వయంచాలక *meta = (hb_సదిశ_blob_meta_t *) దత్తాంశం;
  hb_సదిశ_blob_meta_release_బఫరు (meta);
  ఒకవేళ (meta->లోపల_replace)
  {
    meta->లోపల_replace = అబద్ధం;
    తిరిగి;
  }
  hb_free (meta);
}

స్థిర అంతఃవరుస hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_సదిశ_buf_t *buf)
{
  గుర్తులేని len = 0;
  పూర్ణాంకం allocated = 0;
  అక్షరం *దత్తాంశం = buf->steal (&len, &allocated);
  ఒకవేళ (!దత్తాంశం)
    తిరిగి శూన్యసూచిక;

  hb_blob_t *blob = శూన్యసూచిక;
  ఒకవేళ (*recycled_blob)
    blob = *recycled_blob;
  తార్కిక reused_blob = blob && blob != hb_blob_get_ఖాళీలు ();
  తార్కిక new_meta = అబద్ధం;
  స్వయంచాలక *meta = reused_blob
             ? (hb_సదిశ_blob_meta_t *) hb_blob_get_user_దత్తాంశం (blob, &hb_సదిశ_blob_meta_user_దత్తాంశం_కీలకం)
             : శూన్యసూచిక;
  ఒకవేళ (!meta)
  {
    meta = (hb_సదిశ_blob_meta_t *) hb_malloc (పరిమాణం (hb_సదిశ_blob_meta_t));
    ఒకవేళ (!meta)
    {
      hb_free (దత్తాంశం);
      తిరిగి శూన్యసూచిక;
    }
    meta->దత్తాంశం = శూన్యసూచిక;
    meta->allocated = 0;
    meta->transferred = నిజం;
    meta->లోపల_replace = అబద్ధం;
    new_meta = నిజం;
  }

  ఒకవేళ (reused_blob)
  {
    meta->లోపల_replace = నిజం;
    blob->replace_బఫరు (దత్తాంశం, len, HB_జ్ఞాపకం_బహుళకం_WRITABLE, meta, hb_సదిశ_blob_meta_destroy);
    hb_సదిశ_blob_meta_సమితి_బఫరు (meta, దత్తాంశం, allocated);
  }
  లేకపోతే
  {
    hb_సదిశ_blob_meta_సమితి_బఫరు (meta, దత్తాంశం, allocated);
    blob = hb_blob_create_or_fail (దత్తాంశం, len, HB_జ్ఞాపకం_బహుళకం_WRITABLE, meta, hb_సదిశ_blob_meta_destroy);
    ఒకవేళ (unlikely (!blob))
      తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (unlikely (blob == hb_blob_get_ఖాళీలు ()))
  {
    ఒకవేళ (new_meta)
      hb_free (meta);
    hb_free (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (new_meta &&
      !hb_blob_సమితి_user_దత్తాంశం (blob,
                              &hb_సదిశ_blob_meta_user_దత్తాంశం_కీలకం,
                              meta,
                              శూన్యసూచిక,
                              నిజం))
  {
    ఒకవేళ (!reused_blob)
      hb_blob_destroy (blob);
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (*recycled_blob)
    *recycled_blob = శూన్యసూచిక;

  తిరిగి blob;
}

స్థిర అంతఃవరుస శూన్య
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_సదిశ_buf_t *buf)
{
  ఒకవేళ (!blob)
    తిరిగి;

  స్వయంచాలక *meta = (hb_సదిశ_blob_meta_t *) hb_blob_get_user_దత్తాంశం (blob, &hb_సదిశ_blob_meta_user_దత్తాంశం_కీలకం);
  ఒకవేళ (!meta || meta->transferred || !meta->దత్తాంశం)
    తిరిగి;

  buf->recycle_బఫరు (meta->దత్తాంశం, 0, meta->allocated);
  meta->దత్తాంశం = శూన్యసూచిక;
  meta->allocated = 0;
  meta->transferred = నిజం;
}

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_transform_బిందువు (స్థిరరాశి hb_transform_t<> &t,
			   దశాంశం x_scale_గుణకం,
			   దశాంశం y_scale_గుణకం,
			   దశాంశం x, దశాంశం y,
			   దశాంశం *tx, దశాంశం *ty)
{
  దశాంశం xx = x, yy = y;
  t.transform_బిందువు (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_గుణకం;
  *ty = yy / y_scale_గుణకం;
}

స్థిర అంతఃవరుస hb_bool_t
hb_సదిశ_సమితి_glyph_extents_common (స్థిరరాశి hb_transform_t<> &transform,
				    దశాంశం x_scale_గుణకం,
				    దశాంశం y_scale_గుణకం,
				    స్థిరరాశి hb_glyph_extents_t *glyph_extents,
				    hb_సదిశ_extents_t *extents,
				    hb_bool_t *has_extents)
{
  దశాంశం x0 = (దశాంశం) glyph_extents->x_bearing;
  దశాంశం y0 = (దశాంశం) glyph_extents->y_bearing;
  దశాంశం x1 = x0 + glyph_extents->వెడల్పు;
  దశాంశం y1 = y0 + glyph_extents->ఎత్తు;

  దశాంశం px[4] = {x0, x0, x1, x1};
  దశాంశం py[4] = {y0, y1, y0, y1};

  దశాంశం tx, ty;
  hb_సదిశ_transform_బిందువు (transform, x_scale_గుణకం, y_scale_గుణకం, px[0], py[0], &tx, &ty);
  దశాంశం tx_min = tx, tx_max = tx;
  దశాంశం ty_min = ty, ty_max = ty;

  కొరకు (గుర్తులేని i = 1; i < 4; i++)
  {
    hb_సదిశ_transform_బిందువు (transform, x_scale_గుణకం, y_scale_గుణకం, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  ఒకవేళ (tx_max <= tx_min || ty_max <= ty_min)
    తిరిగి అబద్ధం;

  ఒకవేళ (*has_extents)
  {
    దశాంశం x0 = hb_min (extents->x, tx_min);
    దశాంశం y0 = hb_min (extents->y, ty_min);
    దశాంశం x1 = hb_max (extents->x + extents->వెడల్పు, tx_max);
    దశాంశం y1 = hb_max (extents->y + extents->ఎత్తు, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  లేకపోతే
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = నిజం;
  }
  తిరిగి నిజం;
}

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_svg_append_instance_transform (hb_సదిశ_buf_t *బహిర్గతం,
					 గుర్తులేని precision,
					 దశాంశం x_scale_గుణకం,
					 దశాంశం y_scale_గుణకం,
					 దశాంశం xx, దశాంశం yx,
					 దశాంశం xy, దశాంశం yy,
					 దశాంశం tx, దశాంశం ty)
{
  ఒకవేళ (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
  {
    బహిర్గతం->append_str ("translate(");
    బహిర్గతం->append_అంకె (tx / x_scale_గుణకం, precision);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (-ty / y_scale_గుణకం, precision);
    బహిర్గతం->append_str (") scale(1,-1)");
  }
  లేకపోతే
  {
    గుర్తులేని sprec = బహిర్గతం->scale_precision ();
    బహిర్గతం->append_str ("matrix(");
    బహిర్గతం->append_అంకె (xx, sprec);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (yx, sprec);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (-xy, sprec);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (-yy, sprec);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (tx / x_scale_గుణకం, precision);
    బహిర్గతం->append_c (',');
    బహిర్గతం->append_అంకె (-ty / y_scale_గుణకం, precision);
    బహిర్గతం->append_c (')');
  }
}

#అయితే_ముగింపు /* HB_VECTOR_INTERNAL_HH */
