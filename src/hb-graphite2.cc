/*
 * Copyright © 2011  Martin Hosken
 * Copyright © 2011  SIL International
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_GRAPHITE2

#شامل "hb-shaper-impl.hh"

#شامل "hb-graphite2.h"

#شامل <graphite2/Segment.h>

#شامل "hb-ot-layout.h"


/**
 * SECTION:hb-graphite2
 * @title: hb-graphite2
 * @short_description: Graphite2 integration
 * @include: hb-graphite2.h
 *
 * Functions for using HarfBuzz with fonts that include Graphite features.
 * 
 * For Graphite features to work, you must be sure that HarfBuzz was compiled
 * with the `graphite2` shaping engine enabled. Currently, the default is to
 * not enable `graphite2` shaping.
 **/


/*
 * shaper face data
 */

قسمتعریف ساخت hb_graphite2_tablelist_t
{
  ساخت hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  غیرعلامتی صحیح لیبل_2;
} hb_graphite2_tablelist_t;

ساخت hb_graphite2_face_ڈیٹا_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

ساکن ثابت خالی *hb_graphite2_get_جدول (ثابت خالی *ڈیٹا, غیرعلامتی صحیح لیبل_2, size_t *len)
{
  hb_graphite2_face_ڈیٹا_t *face_ڈیٹا = (hb_graphite2_face_ڈیٹا_t *) ڈیٹا;
  hb_graphite2_tablelist_t *tlist = face_ڈیٹا->tlist;

  hb_blob_t *blob = خالی_اشاریہ;

  برائے (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    اگر (p->لیبل_2 == لیبل_2) {
      blob = p->blob;
      توڑو;
    }

  اگر (unlikely (!blob))
  {
    blob = face_ڈیٹا->face->reference_جدول (لیبل_2);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, حجم (hb_graphite2_tablelist_t));
    اگر (unlikely (!p)) {
      hb_blob_destroy (blob);
      واپس خالی_اشاریہ;
    }
    p->blob = blob;
    p->لیبل_2 = لیبل_2;

retry:
    hb_graphite2_tablelist_t *tlist = face_ڈیٹا->tlist;
    p->next = tlist;

    اگر (unlikely (!face_ڈیٹا->tlist.cmpexch (tlist, p)))
      جاؤ retry;
  }

  غیرعلامتی صحیح tlen;
  ثابت حرف *d = hb_blob_get_ڈیٹا (blob, &tlen);
  *len = tlen;
  واپس d;
}

hb_graphite2_face_ڈیٹا_t *
hb_graphite2_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_جدول (HB_GRAPHITE2_لیبل_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  اگر (!hb_blob_get_لمبائی (silf_blob))
    واپس خالی_اشاریہ;

  hb_graphite2_face_ڈیٹا_t *ڈیٹا = (hb_graphite2_face_ڈیٹا_t *) hb_calloc (1, حجم (hb_graphite2_face_ڈیٹا_t));
  اگر (unlikely (!ڈیٹا))
    واپس خالی_اشاریہ;

  ڈیٹا->face = face;
  ثابت gr_face_ops ops = {حجم(gr_face_ops), &hb_graphite2_get_جدول, تہی};
  ڈیٹا->grface = gr_make_face_with_ops (ڈیٹا, &ops, gr_face_preloadAll);

  اگر (unlikely (!ڈیٹا->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_ڈیٹا_destroy (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  واپس ڈیٹا;
}

خالی
hb_graphite2_shaper_face_ڈیٹا_destroy (hb_graphite2_face_ڈیٹا_t *ڈیٹا)
{
  hb_graphite2_tablelist_t *tlist = ڈیٹا->tlist;

  جب (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (ڈیٹا->grface);

  hb_free (ڈیٹا);
}

/**
 * hb_graphite2_face_get_gr_face: (skip)
 * @face: @hb_face_t to query
 *
 * Fetches the Graphite2 gr_face corresponding to the specified
 * #hb_face_t face object.
 *
 * Return value: the gr_face found
 *
 * Since: 0.9.10
 */
gr_face *
hb_graphite2_face_get_gr_face (hb_face_t *face)
{
  ثابت hb_graphite2_face_ڈیٹا_t *ڈیٹا = face->ڈیٹا.graphite2;
  واپس ڈیٹا ? ڈیٹا->grface : خالی_اشاریہ;
}


/*
 * shaper font data
 */

ساخت hb_graphite2_font_ڈیٹا_t {};

hb_graphite2_font_ڈیٹا_t *
hb_graphite2_shaper_font_ڈیٹا_create (hb_font_t *font HB_غیر_مستعمل)
{
  واپس (hb_graphite2_font_ڈیٹا_t *) HB_SHAPER_ڈیٹا_SUCCEEDED;
}

خالی
hb_graphite2_shaper_font_ڈیٹا_destroy (hb_graphite2_font_ڈیٹا_t *ڈیٹا HB_غیر_مستعمل)
{
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
/**
 * hb_graphite2_font_get_gr_font: (skip)
 * @font: An #hb_font_t
 *
 * Always returns `NULL`. Use hb_graphite2_face_get_gr_face() instead.
 *
 * Return value: (nullable): Graphite2 font associated with @font.
 *
 * Since: 0.9.10
 * Deprecated: 1.4.2
 */
gr_font *
hb_graphite2_font_get_gr_font (hb_font_t *font HB_غیر_مستعمل)
{
  واپس خالی_اشاریہ;
}
#اگر_ختم


/*
 * shaper
 */

ساخت hb_graphite2_cluster_t {
  غیرعلامتی صحیح بنیاد_char;
  غیرعلامتی صحیح عدد_chars;
  غیرعلامتی صحیح بنیاد_glyph;
  غیرعلامتی صحیح عدد_glyphs;
  غیرعلامتی صحیح cluster;
  hb_position_t advance;
};

ساخت hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  غیرعلامتی صحیح cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_غیر_مستعمل,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     غیرعلامتی صحیح        عدد_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->ڈیٹا.graphite2->grface;

  ثابت حرف *lang = hb_language_مقصد_string (hb_بفر_get_language (بفر));
  ثابت حرف *lang_انتہا = lang ? strchr (lang, '-') : خالی_اشاریہ;
  صحیح lang_len = lang_انتہا ? lang_انتہا - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_لیبل_from_string (lang, lang_len) : 0);

  برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
  {
    ثابت gr_feature_ref *fref = gr_face_ڈھونڈو_fref (grface, features[i].لیبل_2);
    اگر (fref)
      gr_fref_سیٹ_feature_قدر (fref, features[i].value, feats);
  }

  hb_direction_t direction = بفر->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (بفر->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  اگر ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_بفر_الٹ_clusters (بفر);
    direction = HB_DIRECTION_الٹ (direction);
  }

  gr_segment *seg = خالی_اشاریہ;
  ثابت gr_خانہ *is;
  غیرعلامتی صحیح ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  غیرعلامتی صحیح scratch_سائز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائز);

  uint32_t *chars = (uint32_t *) scratch;

  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; ++i)
    chars[i] = بفر->info[i].codepoint;

  seg = gr_make_seg (خالی_اشاریہ, grface,
		     HB_لیبل_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, بفر->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  اگر (unlikely (!seg)) {
    اگر (feats) gr_featureval_destroy (feats);
    واپس جھوٹ;
  }

  غیرعلامتی صحیح glyph_گنتی = gr_seg_n_slots (seg);
  اگر (unlikely (!glyph_گنتی)) {
    اگر (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    بفر->len = 0;
    واپس سچ;
  }

  (خالی) بفر->ensure (glyph_گنتی);
  scratch = بفر->get_scratch_بفر (&scratch_سائز);
  جب ((DIV_CEIL (حجم (hb_graphite2_cluster_t) * بفر->len, حجم (*scratch)) +
	  DIV_CEIL (حجم (hb_graphite2_glyph_t) * glyph_گنتی, حجم (*scratch))) > scratch_سائز)
  {
    اگر (unlikely (!بفر->ensure (بفر->allocated * 2)))
    {
      اگر (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      واپس جھوٹ;
    }
    scratch = بفر->get_scratch_بفر (&scratch_سائز);
  }

#تعریف_کرو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  کرو { \
    غیرعلامتی صحیح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_سائز); \
    scratch += _consumed; \
    scratch_سائز -= _consumed; \
  } جب (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, بفر->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_گنتی);

#تعریف_منسوخ ALLOCATE_ARRAY

  hb_memset (clusters, 0, حجم (clusters[0]) * بفر->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = بفر->info[0].cluster;
  غیرعلامتی صحیح upem = hb_face_get_upem (face);
  اعشاری xscale = (اعشاری) font->x_scale / upem;
  اعشاری yscale = (اعشاری) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  اگر (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_مقصد<hb_position_t> (gr_خانہ_origin_X(gr_seg_پہلا_خانہ(seg)) * xscale);
    clusters[0].advance = hb_clamp_مقصد<hb_position_t> ((دوہرا) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ورنہ
    clusters[0].advance = 0;
  برائے (is = gr_seg_پہلا_خانہ (seg), ic = 0; is; is = gr_خانہ_اگلا_اندر_segment (is), ic++)
  {
    صحیح خانہ_before = gr_خانہ_before (is);
    صحیح خانہ_after = gr_خانہ_after (is);
    اگر (unlikely (خانہ_before < 0 ||
		  خانہ_after < خانہ_before ||
		  (غیرعلامتی صحیح) خانہ_after >= بفر->len))
    {
      اگر (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      واپس جھوٹ;
    }
    غیرعلامتی صحیح before = (غیرعلامتی صحیح) خانہ_before;
    غیرعلامتی صحیح after = (غیرعلامتی صحیح) خانہ_after;
    pg->codepoint = gr_خانہ_gid (is);
    pg->cluster = hb_min (بفر->info[before].cluster,
			  بفر->info[after].cluster);
    pg++;
    جب (clusters[ci].بنیاد_char > before && ci)
    {
      clusters[ci-1].عدد_chars += clusters[ci].عدد_chars;
      clusters[ci-1].عدد_glyphs += clusters[ci].عدد_glyphs;
      clusters[ci-1].advance = hb_saturate_جوڑو (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    اگر (gr_خانہ_can_داخل_کرو_before (is) && clusters[ci].عدد_chars && before >= clusters[ci].بنیاد_char + clusters[ci].عدد_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->بنیاد_char = clusters[ci].بنیاد_char + clusters[ci].عدد_chars;
      c->cluster = بفر->info[c->بنیاد_char].cluster;
      c->عدد_chars = before - c->بنیاد_char;
      c->بنیاد_glyph = ic;
      c->عدد_glyphs = 0;
      اگر (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_مقصد<hb_position_t> ((دوہرا) curradv -
						   (دوہرا) (gr_خانہ_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      ورنہ
      {
	hb_position_t origin_X = hb_clamp_مقصد<hb_position_t> (gr_خانہ_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_جوڑو (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].عدد_glyphs++;

    اگر (clusters[ci].بنیاد_char + clusters[ci].عدد_chars < after + 1)
	clusters[ci].عدد_chars = after + 1 - clusters[ci].بنیاد_char;
  }

  اگر (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_جوڑو (clusters[ci].advance, curradv);
  ورنہ
    clusters[ci].advance = hb_clamp_مقصد<hb_position_t> ((دوہرا) clusters[ci].advance +
							       (دوہرا) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  برائے (غیرعلامتی صحیح i = 0; i < ci; ++i)
  {
    برائے (غیرعلامتی صحیح j = 0; j < clusters[i].عدد_glyphs; ++j)
    {
      hb_glyph_info_t *info = &بفر->info[clusters[i].بنیاد_glyph + j];
      info->codepoint = glyphs[clusters[i].بنیاد_glyph + j].codepoint;
      info->cluster = HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level) ?
		      glyphs[clusters[i].بنیاد_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  بفر->len = glyph_گنتی;

  برائے (غیرعلامتی صحیح i = 0; i < ci; ++i)
    بفر->ملاؤ_clusters (clusters[i].بنیاد_glyph,
			    clusters[i].بنیاد_glyph + clusters[i].عدد_glyphs);

  /* Positioning. */
  غیرعلامتی صحیح curr_gr_cluster = UINT_MAX;
  ثابت hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pPos = hb_بفر_get_glyph_positions (بفر, خالی_اشاریہ);
  اگر (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    برائے (is = gr_seg_پہلا_خانہ (seg); is; pPos++, ++info, is = gr_خانہ_اگلا_اندر_segment (is))
    {
      pPos->x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((دوہرا) (gr_خانہ_origin_X (is) * xscale) - curradvx);
      pPos->y_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((دوہرا) (gr_خانہ_origin_Y (is) * yscale) - curradvy);
      اگر (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_جوڑو (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ورنہ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_مقصد<hb_position_t> (gr_خانہ_advance_Y (is, grface, خالی_اشاریہ) * yscale);
      curradvy = hb_saturate_جوڑو (curradvy, pPos->y_advance);
    }
  }
  ورنہ
  {
    curradvx = hb_clamp_مقصد<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    برائے (is = gr_seg_پہلا_خانہ (seg); is; pPos++, info++, is = gr_خانہ_اگلا_اندر_segment (is))
    {
      اگر (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ورنہ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_مقصد<hb_position_t> (gr_خانہ_advance_Y (is, grface, خالی_اشاریہ) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((دوہرا) (gr_خانہ_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((دوہرا) (gr_خانہ_origin_Y (is) * yscale) - curradvy);
    }
    hb_بفر_الٹ_clusters (بفر);
  }

  اگر (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  بفر->clear_glyph_flags ();
  بفر->unsafe_مقصد_break ();

  واپس سچ;
}


#اگر_ختم
