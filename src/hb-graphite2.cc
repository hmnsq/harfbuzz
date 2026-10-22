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

#که_تعریف_شوی HAVE_GRAPHITE2

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
  بینښه صحيح نښان_2;
} hb_graphite2_tablelist_t;

بنية hb_graphite2_face_معلومات_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

ساكن ثابت فراغ *hb_graphite2_get_جدول (ثابت فراغ *معلومات, بینښه صحيح نښان_2, size_t *len)
{
  hb_graphite2_face_معلومات_t *face_معلومات = (hb_graphite2_face_معلومات_t *) معلومات;
  hb_graphite2_tablelist_t *tlist = face_معلومات->tlist;

  hb_blob_t *blob = تش_اشاره;

  لكل (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    إذا (p->نښان_2 == نښان_2) {
      blob = p->blob;
      اكسر;
    }

  إذا (unlikely (!blob))
  {
    blob = face_معلومات->face->reference_جدول (نښان_2);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, حجم (hb_graphite2_tablelist_t));
    إذا (unlikely (!p)) {
      hb_blob_destroy (blob);
      أرجع تش_اشاره;
    }
    p->blob = blob;
    p->نښان_2 = نښان_2;

retry:
    hb_graphite2_tablelist_t *tlist = face_معلومات->tlist;
    p->next = tlist;

    إذا (unlikely (!face_معلومات->tlist.cmpexch (tlist, p)))
      اذهب retry;
  }

  بینښه صحيح tlen;
  ثابت حرف *d = hb_blob_get_معلومات (blob, &tlen);
  *len = tlen;
  أرجع d;
}

hb_graphite2_face_معلومات_t *
hb_graphite2_shaper_face_معلومات_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_جدول (HB_GRAPHITE2_نښان_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  إذا (!hb_blob_get_اوږدتیا (silf_blob))
    أرجع تش_اشاره;

  hb_graphite2_face_معلومات_t *معلومات = (hb_graphite2_face_معلومات_t *) hb_calloc (1, حجم (hb_graphite2_face_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع تش_اشاره;

  معلومات->face = face;
  ثابت gr_face_ops ops = {حجم(gr_face_ops), &hb_graphite2_get_جدول, عدم};
  معلومات->grface = gr_make_face_with_ops (معلومات, &ops, gr_face_preloadAll);

  إذا (unlikely (!معلومات->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_معلومات_destroy (معلومات);
    أرجع تش_اشاره;
  }

  أرجع معلومات;
}

فراغ
hb_graphite2_shaper_face_معلومات_destroy (hb_graphite2_face_معلومات_t *معلومات)
{
  hb_graphite2_tablelist_t *tlist = معلومات->tlist;

  طالما (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (معلومات->grface);

  hb_free (معلومات);
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
  ثابت hb_graphite2_face_معلومات_t *معلومات = face->معلومات.graphite2;
  أرجع معلومات ? معلومات->grface : تش_اشاره;
}


/*
 * shaper font data
 */

بنية hb_graphite2_font_معلومات_t {};

hb_graphite2_font_معلومات_t *
hb_graphite2_shaper_font_معلومات_create (hb_font_t *font HB_ناکارول_شوی)
{
  أرجع (hb_graphite2_font_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_graphite2_shaper_font_معلومات_destroy (hb_graphite2_font_معلومات_t *معلومات HB_ناکارول_شوی)
{
}

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
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
hb_graphite2_font_get_gr_font (hb_font_t *font HB_ناکارول_شوی)
{
  أرجع تش_اشاره;
}
#د_که_پای


/*
 * shaper
 */

بنية hb_graphite2_cluster_t {
  بینښه صحيح بنسټ_char;
  بینښه صحيح عدد_chars;
  بینښه صحيح بنسټ_glyph;
  بینښه صحيح عدد_glyphs;
  بینښه صحيح cluster;
  hb_position_t advance;
};

بنية hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  بینښه صحيح cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_ناکارول_شوی,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     بینښه صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->معلومات.graphite2->grface;

  ثابت حرف *lang = hb_language_منزل_string (hb_بفر_get_language (بفر));
  ثابت حرف *lang_پای_ټکی = lang ? strchr (lang, '-') : تش_اشاره;
  صحيح lang_len = lang_پای_ټکی ? lang_پای_ټکی - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_نښان_from_string (lang, lang_len) : 0);

  لكل (بینښه صحيح i = 0; i < عدد_features; i++)
  {
    ثابت gr_feature_ref *fref = gr_face_پیدا_کړه_fref (grface, features[i].نښان_2);
    إذا (fref)
      gr_fref_سېټ_feature_ارزښت (fref, features[i].value, feats);
  }

  hb_direction_t direction = بفر->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (بفر->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_بفر_سرچپه_clusters (بفر);
    direction = HB_DIRECTION_سرچپه (direction);
  }

  gr_segment *seg = تش_اشاره;
  ثابت gr_خونه *is;
  بینښه صحيح ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  بینښه صحيح scratch_اندازه;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_اندازه);

  uint32_t *chars = (uint32_t *) scratch;

  لكل (بینښه صحيح i = 0; i < بفر->len; ++i)
    chars[i] = بفر->info[i].codepoint;

  seg = gr_make_seg (تش_اشاره, grface,
		     HB_نښان_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, بفر->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  إذا (unlikely (!seg)) {
    إذا (feats) gr_featureval_destroy (feats);
    أرجع خطأ;
  }

  بینښه صحيح glyph_شمېر = gr_seg_n_slots (seg);
  إذا (unlikely (!glyph_شمېر)) {
    إذا (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    بفر->len = 0;
    أرجع ریښتیا;
  }

  (فراغ) بفر->ensure (glyph_شمېر);
  scratch = بفر->get_scratch_بفر (&scratch_اندازه);
  طالما ((DIV_CEIL (حجم (hb_graphite2_cluster_t) * بفر->len, حجم (*scratch)) +
	  DIV_CEIL (حجم (hb_graphite2_glyph_t) * glyph_شمېر, حجم (*scratch))) > scratch_اندازه)
  {
    إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
    {
      إذا (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      أرجع خطأ;
    }
    scratch = بفر->get_scratch_بفر (&scratch_اندازه);
  }

#تعریف_کړه ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بینښه صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, بفر->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_شمېر);

#تعریف_لغوه ALLOCATE_ARRAY

  hb_memset (clusters, 0, حجم (clusters[0]) * بفر->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = بفر->info[0].cluster;
  بینښه صحيح upem = hb_face_get_upem (face);
  عائم xscale = (عائم) font->x_scale / upem;
  عائم yscale = (عائم) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  إذا (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_منزل<hb_position_t> (gr_خونه_origin_X(gr_seg_لومړی_خونه(seg)) * xscale);
    clusters[0].advance = hb_clamp_منزل<hb_position_t> ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_لومړی_خونه (seg), ic = 0; is; is = gr_خونه_بل_دننه_segment (is), ic++)
  {
    صحيح خونه_before = gr_خونه_before (is);
    صحيح خونه_after = gr_خونه_after (is);
    إذا (unlikely (خونه_before < 0 ||
		  خونه_after < خونه_before ||
		  (بینښه صحيح) خونه_after >= بفر->len))
    {
      إذا (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      أرجع خطأ;
    }
    بینښه صحيح before = (بینښه صحيح) خونه_before;
    بینښه صحيح after = (بینښه صحيح) خونه_after;
    pg->codepoint = gr_خونه_gid (is);
    pg->cluster = hb_min (بفر->info[before].cluster,
			  بفر->info[after].cluster);
    pg++;
    طالما (clusters[ci].بنسټ_char > before && ci)
    {
      clusters[ci-1].عدد_chars += clusters[ci].عدد_chars;
      clusters[ci-1].عدد_glyphs += clusters[ci].عدد_glyphs;
      clusters[ci-1].advance = hb_saturate_جمع_کړه (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_خونه_can_ورننباسه_before (is) && clusters[ci].عدد_chars && before >= clusters[ci].بنسټ_char + clusters[ci].عدد_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->بنسټ_char = clusters[ci].بنسټ_char + clusters[ci].عدد_chars;
      c->cluster = بفر->info[c->بنسټ_char].cluster;
      c->عدد_chars = before - c->بنسټ_char;
      c->بنسټ_glyph = ic;
      c->عدد_glyphs = 0;
      إذا (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_منزل<hb_position_t> ((مزدوج) curradv -
						   (مزدوج) (gr_خونه_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	hb_position_t origin_X = hb_clamp_منزل<hb_position_t> (gr_خونه_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_جمع_کړه (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].عدد_glyphs++;

    إذا (clusters[ci].بنسټ_char + clusters[ci].عدد_chars < after + 1)
	clusters[ci].عدد_chars = after + 1 - clusters[ci].بنسټ_char;
  }

  إذا (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_جمع_کړه (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = hb_clamp_منزل<hb_position_t> ((مزدوج) clusters[ci].advance +
							       (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (بینښه صحيح i = 0; i < ci; ++i)
  {
    لكل (بینښه صحيح j = 0; j < clusters[i].عدد_glyphs; ++j)
    {
      hb_glyph_info_t *info = &بفر->info[clusters[i].بنسټ_glyph + j];
      info->codepoint = glyphs[clusters[i].بنسټ_glyph + j].codepoint;
      info->cluster = HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level) ?
		      glyphs[clusters[i].بنسټ_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  بفر->len = glyph_شمېر;

  لكل (بینښه صحيح i = 0; i < ci; ++i)
    بفر->یوځای_clusters (clusters[i].بنسټ_glyph,
			    clusters[i].بنسټ_glyph + clusters[i].عدد_glyphs);

  /* Positioning. */
  بینښه صحيح curr_gr_cluster = UINT_MAX;
  ثابت hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pPos = hb_بفر_get_glyph_positions (بفر, تش_اشاره);
  إذا (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_لومړی_خونه (seg); is; pPos++, ++info, is = gr_خونه_بل_دننه_segment (is))
    {
      pPos->x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((مزدوج) (gr_خونه_origin_X (is) * xscale) - curradvx);
      pPos->y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((مزدوج) (gr_خونه_origin_Y (is) * yscale) - curradvy);
      إذا (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_جمع_کړه (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_منزل<hb_position_t> (gr_خونه_advance_Y (is, grface, تش_اشاره) * yscale);
      curradvy = hb_saturate_جمع_کړه (curradvy, pPos->y_advance);
    }
  }
  وإلا
  {
    curradvx = hb_clamp_منزل<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_لومړی_خونه (seg); is; pPos++, info++, is = gr_خونه_بل_دننه_segment (is))
    {
      إذا (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_منزل<hb_position_t> (gr_خونه_advance_Y (is, grface, تش_اشاره) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((مزدوج) (gr_خونه_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((مزدوج) (gr_خونه_origin_Y (is) * yscale) - curradvy);
    }
    hb_بفر_سرچپه_clusters (بفر);
  }

  إذا (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  أرجع ریښتیا;
}


#د_که_پای
