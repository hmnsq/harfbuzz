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

#अगर_अघोषित HB_VECTOR_INTERNAL_HH
#घोषणा HB_VECTOR_INTERNAL_HH

#समावेश "hb-vector.h"

काष्ठा hb_vector_blob_meta_t
{
  अक्षर *तथ्य;
  पूर्णांक allocated;
  bool transferred;
  bool in_replace;
};

अटल hb_user_तथ्य_कुंजी_t hb_vector_blob_meta_user_तथ्य_कुंजी;

अटल अंतरभूत व्योम
hb_vector_blob_meta_set_बफर (hb_vector_blob_meta_t *meta,
				अक्षर *तथ्य,
				पूर्णांक allocated)
{
  meta->तथ्य = तथ्य;
  meta->allocated = allocated;
  meta->transferred = false;
}

अटल अंतरभूत व्योम
hb_vector_blob_meta_release_बफर (hb_vector_blob_meta_t *meta)
{
  अगर (!meta)
    वापस;
  अगर (!meta->transferred && meta->तथ्य)
    hb_free (meta->तथ्य);
  meta->तथ्य = nullptr;
  meta->allocated = 0;
  meta->transferred = true;
}

अटल अंतरभूत व्योम
hb_vector_blob_meta_destroy (व्योम *तथ्य)
{
  स्वतः *meta = (hb_vector_blob_meta_t *) तथ्य;
  hb_vector_blob_meta_release_बफर (meta);
  अगर (meta->in_replace)
  {
    meta->in_replace = false;
    वापस;
  }
  hb_free (meta);
}

अटल अंतरभूत hb_blob_t *
hb_buf_blob_from (hb_blob_t **recycled_blob,
		  hb_vector_buf_t *buf)
{
  अचिन्हित len = 0;
  पूर्णांक allocated = 0;
  अक्षर *तथ्य = buf->steal (&len, &allocated);
  अगर (!तथ्य)
    वापस nullptr;

  hb_blob_t *blob = nullptr;
  अगर (*recycled_blob)
    blob = *recycled_blob;
  bool reused_blob = blob && blob != hb_blob_get_empty ();
  bool new_meta = false;
  स्वतः *meta = reused_blob
             ? (hb_vector_blob_meta_t *) hb_blob_get_user_तथ्य (blob, &hb_vector_blob_meta_user_तथ्य_कुंजी)
             : nullptr;
  अगर (!meta)
  {
    meta = (hb_vector_blob_meta_t *) hb_malloc (माप (hb_vector_blob_meta_t));
    अगर (!meta)
    {
      hb_free (तथ्य);
      वापस nullptr;
    }
    meta->तथ्य = nullptr;
    meta->allocated = 0;
    meta->transferred = true;
    meta->in_replace = false;
    new_meta = true;
  }

  अगर (reused_blob)
  {
    meta->in_replace = true;
    blob->replace_बफर (तथ्य, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    hb_vector_blob_meta_set_बफर (meta, तथ्य, allocated);
  }
  अन्यथा
  {
    hb_vector_blob_meta_set_बफर (meta, तथ्य, allocated);
    blob = hb_blob_create_or_fail (तथ्य, len, HB_MEMORY_MODE_WRITABLE, meta, hb_vector_blob_meta_destroy);
    अगर (unlikely (!blob))
      वापस nullptr;
  }

  अगर (unlikely (blob == hb_blob_get_empty ()))
  {
    अगर (new_meta)
      hb_free (meta);
    hb_free (तथ्य);
    वापस nullptr;
  }

  अगर (new_meta &&
      !hb_blob_set_user_तथ्य (blob,
                              &hb_vector_blob_meta_user_तथ्य_कुंजी,
                              meta,
                              nullptr,
                              true))
  {
    अगर (!reused_blob)
      hb_blob_destroy (blob);
    वापस nullptr;
  }

  अगर (*recycled_blob)
    *recycled_blob = nullptr;

  वापस blob;
}

अटल अंतरभूत व्योम
hb_buf_recover_recycled (hb_blob_t *blob,
			 hb_vector_buf_t *buf)
{
  अगर (!blob)
    वापस;

  स्वतः *meta = (hb_vector_blob_meta_t *) hb_blob_get_user_तथ्य (blob, &hb_vector_blob_meta_user_तथ्य_कुंजी);
  अगर (!meta || meta->transferred || !meta->तथ्य)
    वापस;

  buf->recycle_बफर (meta->तथ्य, 0, meta->allocated);
  meta->तथ्य = nullptr;
  meta->allocated = 0;
  meta->transferred = true;
}

अटल अंतरभूत व्योम
hb_vector_transform_point (स्थिर hb_transform_t<> &t,
			   भग्न x_scale_factor,
			   भग्न y_scale_factor,
			   भग्न x, भग्न y,
			   भग्न *tx, भग्न *ty)
{
  भग्न xx = x, yy = y;
  t.transform_point (xx, yy);
  /* Setters force scale factors > 0; trust them here. */
  *tx = xx / x_scale_factor;
  *ty = yy / y_scale_factor;
}

अटल अंतरभूत hb_bool_t
hb_vector_set_glyph_extents_common (स्थिर hb_transform_t<> &transform,
				    भग्न x_scale_factor,
				    भग्न y_scale_factor,
				    स्थिर hb_glyph_extents_t *glyph_extents,
				    hb_vector_extents_t *extents,
				    hb_bool_t *has_extents)
{
  भग्न x0 = (भग्न) glyph_extents->x_bearing;
  भग्न y0 = (भग्न) glyph_extents->y_bearing;
  भग्न x1 = x0 + glyph_extents->width;
  भग्न y1 = y0 + glyph_extents->height;

  भग्न px[4] = {x0, x0, x1, x1};
  भग्न py[4] = {y0, y1, y0, y1};

  भग्न tx, ty;
  hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[0], py[0], &tx, &ty);
  भग्न tx_min = tx, tx_max = tx;
  भग्न ty_min = ty, ty_max = ty;

  क्रम (अचिन्हित i = 1; i < 4; i++)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, px[i], py[i], &tx, &ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  अगर (tx_max <= tx_min || ty_max <= ty_min)
    वापस false;

  अगर (*has_extents)
  {
    भग्न x0 = hb_min (extents->x, tx_min);
    भग्न y0 = hb_min (extents->y, ty_min);
    भग्न x1 = hb_max (extents->x + extents->width, tx_max);
    भग्न y1 = hb_max (extents->y + extents->height, ty_max);
    *extents = {x0, y0, x1 - x0, y1 - y0};
  }
  अन्यथा
  {
    *extents = {tx_min, ty_min, tx_max - tx_min, ty_max - ty_min};
    *has_extents = true;
  }
  वापस true;
}

अटल अंतरभूत व्योम
hb_vector_svg_append_instance_transform (hb_vector_buf_t *out,
					 अचिन्हित precision,
					 भग्न x_scale_factor,
					 भग्न y_scale_factor,
					 भग्न xx, भग्न yx,
					 भग्न xy, भग्न yy,
					 भग्न tx, भग्न ty)
{
  अगर (xx == 1.f && yx == 0.f && xy == 0.f && yy == 1.f)
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

#पूर्ण_अगर /* HB_VECTOR_INTERNAL_HH */
