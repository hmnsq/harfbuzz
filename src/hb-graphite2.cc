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

#ضمن "hb.hh"

#اگر_تعریف_شده HAVE_GRAPHITE2

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-graphite2.h"

#ضمن <graphite2/Segment.h>

#ضمن "hb-ot-layout.h"


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

تعريفنوع بنية hb_graphite2_tablelist_t
{
  بنية hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  بیعلامت صحيح برچسب_2;
} hb_graphite2_tablelist_t;

بنية hb_graphite2_face_داده_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

ساكن ثابت فراغ *hb_graphite2_get_جدول (ثابت فراغ *داده, بیعلامت صحيح برچسب_2, size_t *len)
{
  hb_graphite2_face_داده_t *face_داده = (hb_graphite2_face_داده_t *) داده;
  hb_graphite2_tablelist_t *tlist = face_داده->tlist;

  hb_blob_t *blob = اشاره_تهی;

  لكل (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    إذا (p->برچسب_2 == برچسب_2) {
      blob = p->blob;
      اكسر;
    }

  إذا (unlikely (!blob))
  {
    blob = face_داده->face->reference_جدول (برچسب_2);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, حجم (hb_graphite2_tablelist_t));
    إذا (unlikely (!p)) {
      hb_blob_destroy (blob);
      أرجع اشاره_تهی;
    }
    p->blob = blob;
    p->برچسب_2 = برچسب_2;

retry:
    hb_graphite2_tablelist_t *tlist = face_داده->tlist;
    p->next = tlist;

    إذا (unlikely (!face_داده->tlist.cmpexch (tlist, p)))
      اذهب retry;
  }

  بیعلامت صحيح tlen;
  ثابت حرف *d = hb_blob_get_داده (blob, &tlen);
  *len = tlen;
  أرجع d;
}

hb_graphite2_face_داده_t *
hb_graphite2_shaper_face_داده_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_جدول (HB_GRAPHITE2_برچسب_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  إذا (!hb_blob_get_درازی (silf_blob))
    أرجع اشاره_تهی;

  hb_graphite2_face_داده_t *داده = (hb_graphite2_face_داده_t *) hb_calloc (1, حجم (hb_graphite2_face_داده_t));
  إذا (unlikely (!داده))
    أرجع اشاره_تهی;

  داده->face = face;
  ثابت gr_face_ops ops = {حجم(gr_face_ops), &hb_graphite2_get_جدول, عدم};
  داده->grface = gr_make_face_with_ops (داده, &ops, gr_face_preloadAll);

  إذا (unlikely (!داده->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_داده_destroy (داده);
    أرجع اشاره_تهی;
  }

  أرجع داده;
}

فراغ
hb_graphite2_shaper_face_داده_destroy (hb_graphite2_face_داده_t *داده)
{
  hb_graphite2_tablelist_t *tlist = داده->tlist;

  طالما (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (داده->grface);

  hb_free (داده);
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
  ثابت hb_graphite2_face_داده_t *داده = face->داده.graphite2;
  أرجع داده ? داده->grface : اشاره_تهی;
}


/*
 * shaper font data
 */

بنية hb_graphite2_font_داده_t {};

hb_graphite2_font_داده_t *
hb_graphite2_shaper_font_داده_create (hb_font_t *font HB_استفادهنشده)
{
  أرجع (hb_graphite2_font_داده_t *) HB_SHAPER_داده_SUCCEEDED;
}

فراغ
hb_graphite2_shaper_font_داده_destroy (hb_graphite2_font_داده_t *داده HB_استفادهنشده)
{
}

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
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
hb_graphite2_font_get_gr_font (hb_font_t *font HB_استفادهنشده)
{
  أرجع اشاره_تهی;
}
#پایان_اگر


/*
 * shaper
 */

بنية hb_graphite2_cluster_t {
  بیعلامت صحيح پایه_char;
  بیعلامت صحيح رقم_chars;
  بیعلامت صحيح پایه_glyph;
  بیعلامت صحيح رقم_glyphs;
  بیعلامت صحيح cluster;
  hb_position_t advance;
};

بنية hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  بیعلامت صحيح cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_استفادهنشده,
		     hb_font_t          *font,
		     hb_میانگیر_t        *میانگیر,
		     ثابت hb_feature_t *features,
		     بیعلامت صحيح        رقم_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->داده.graphite2->grface;

  ثابت حرف *lang = hb_language_مقصد_string (hb_میانگیر_get_language (میانگیر));
  ثابت حرف *lang_انتها = lang ? strchr (lang, '-') : اشاره_تهی;
  صحيح lang_len = lang_انتها ? lang_انتها - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_برچسب_from_string (lang, lang_len) : 0);

  لكل (بیعلامت صحيح i = 0; i < رقم_features; i++)
  {
    ثابت gr_feature_ref *fref = gr_face_پیدا_کن_fref (grface, features[i].برچسب_2);
    إذا (fref)
      gr_fref_مجموعه_feature_مقدار (fref, features[i].value, feats);
  }

  hb_direction_t direction = میانگیر->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (میانگیر->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_میانگیر_معکوس_clusters (میانگیر);
    direction = HB_DIRECTION_معکوس (direction);
  }

  gr_segment *seg = اشاره_تهی;
  ثابت gr_جایگاه *is;
  بیعلامت صحيح ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  بیعلامت صحيح scratch_اندازه;
  hb_میانگیر_t::scratch_میانگیر_t *scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);

  uint32_t *chars = (uint32_t *) scratch;

  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; ++i)
    chars[i] = میانگیر->info[i].codepoint;

  seg = gr_make_seg (اشاره_تهی, grface,
		     HB_برچسب_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, میانگیر->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  إذا (unlikely (!seg)) {
    إذا (feats) gr_featureval_destroy (feats);
    أرجع خطأ;
  }

  بیعلامت صحيح glyph_شمار = gr_seg_n_slots (seg);
  إذا (unlikely (!glyph_شمار)) {
    إذا (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    میانگیر->len = 0;
    أرجع درست;
  }

  (فراغ) میانگیر->ensure (glyph_شمار);
  scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);
  طالما ((DIV_CEIL (حجم (hb_graphite2_cluster_t) * میانگیر->len, حجم (*scratch)) +
	  DIV_CEIL (حجم (hb_graphite2_glyph_t) * glyph_شمار, حجم (*scratch))) > scratch_اندازه)
  {
    إذا (unlikely (!میانگیر->ensure (میانگیر->allocated * 2)))
    {
      إذا (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      أرجع خطأ;
    }
    scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);
  }

#تعریف_کن ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بیعلامت صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, میانگیر->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_شمار);

#لغو_تعریف ALLOCATE_ARRAY

  hb_memset (clusters, 0, حجم (clusters[0]) * میانگیر->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = میانگیر->info[0].cluster;
  بیعلامت صحيح upem = hb_face_get_upem (face);
  عائم xscale = (عائم) font->x_scale / upem;
  عائم yscale = (عائم) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  إذا (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_مقصد<hb_position_t> (gr_جایگاه_origin_X(gr_seg_اولی_جایگاه(seg)) * xscale);
    clusters[0].advance = hb_clamp_مقصد<hb_position_t> ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_اولی_جایگاه (seg), ic = 0; is; is = gr_جایگاه_بعدی_درون_segment (is), ic++)
  {
    صحيح جایگاه_before = gr_جایگاه_before (is);
    صحيح جایگاه_after = gr_جایگاه_after (is);
    إذا (unlikely (جایگاه_before < 0 ||
		  جایگاه_after < جایگاه_before ||
		  (بیعلامت صحيح) جایگاه_after >= میانگیر->len))
    {
      إذا (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      أرجع خطأ;
    }
    بیعلامت صحيح before = (بیعلامت صحيح) جایگاه_before;
    بیعلامت صحيح after = (بیعلامت صحيح) جایگاه_after;
    pg->codepoint = gr_جایگاه_gid (is);
    pg->cluster = hb_min (میانگیر->info[before].cluster,
			  میانگیر->info[after].cluster);
    pg++;
    طالما (clusters[ci].پایه_char > before && ci)
    {
      clusters[ci-1].رقم_chars += clusters[ci].رقم_chars;
      clusters[ci-1].رقم_glyphs += clusters[ci].رقم_glyphs;
      clusters[ci-1].advance = hb_saturate_جمع_کن (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_جایگاه_can_درج_before (is) && clusters[ci].رقم_chars && before >= clusters[ci].پایه_char + clusters[ci].رقم_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->پایه_char = clusters[ci].پایه_char + clusters[ci].رقم_chars;
      c->cluster = میانگیر->info[c->پایه_char].cluster;
      c->رقم_chars = before - c->پایه_char;
      c->پایه_glyph = ic;
      c->رقم_glyphs = 0;
      إذا (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_مقصد<hb_position_t> ((مزدوج) curradv -
						   (مزدوج) (gr_جایگاه_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	hb_position_t origin_X = hb_clamp_مقصد<hb_position_t> (gr_جایگاه_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_جمع_کن (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].رقم_glyphs++;

    إذا (clusters[ci].پایه_char + clusters[ci].رقم_chars < after + 1)
	clusters[ci].رقم_chars = after + 1 - clusters[ci].پایه_char;
  }

  إذا (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_جمع_کن (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = hb_clamp_مقصد<hb_position_t> ((مزدوج) clusters[ci].advance +
							       (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (بیعلامت صحيح i = 0; i < ci; ++i)
  {
    لكل (بیعلامت صحيح j = 0; j < clusters[i].رقم_glyphs; ++j)
    {
      hb_glyph_info_t *info = &میانگیر->info[clusters[i].پایه_glyph + j];
      info->codepoint = glyphs[clusters[i].پایه_glyph + j].codepoint;
      info->cluster = HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (میانگیر->cluster_level) ?
		      glyphs[clusters[i].پایه_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  میانگیر->len = glyph_شمار;

  لكل (بیعلامت صحيح i = 0; i < ci; ++i)
    میانگیر->ادغام_clusters (clusters[i].پایه_glyph,
			    clusters[i].پایه_glyph + clusters[i].رقم_glyphs);

  /* Positioning. */
  بیعلامت صحيح curr_gr_cluster = UINT_MAX;
  ثابت hb_glyph_info_t *info = میانگیر->info;
  hb_glyph_position_t *pPos = hb_میانگیر_get_glyph_positions (میانگیر, اشاره_تهی);
  إذا (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_اولی_جایگاه (seg); is; pPos++, ++info, is = gr_جایگاه_بعدی_درون_segment (is))
    {
      pPos->x_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> ((مزدوج) (gr_جایگاه_origin_X (is) * xscale) - curradvx);
      pPos->y_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> ((مزدوج) (gr_جایگاه_origin_Y (is) * yscale) - curradvy);
      إذا (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_جمع_کن (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_مقصد<hb_position_t> (gr_جایگاه_advance_Y (is, grface, اشاره_تهی) * yscale);
      curradvy = hb_saturate_جمع_کن (curradvy, pPos->y_advance);
    }
  }
  وإلا
  {
    curradvx = hb_clamp_مقصد<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_اولی_جایگاه (seg); is; pPos++, info++, is = gr_جایگاه_بعدی_درون_segment (is))
    {
      إذا (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_مقصد<hb_position_t> (gr_جایگاه_advance_Y (is, grface, اشاره_تهی) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> ((مزدوج) (gr_جایگاه_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> ((مزدوج) (gr_جایگاه_origin_Y (is) * yscale) - curradvy);
    }
    hb_میانگیر_معکوس_clusters (میانگیر);
  }

  إذا (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  میانگیر->clear_glyph_flags ();
  میانگیر->unsafe_مقصد_break ();

  أرجع درست;
}


#پایان_اگر
