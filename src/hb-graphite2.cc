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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GRAPHITE2

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-graphite2.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <graphite2/Segment.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.h"


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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_graphite2_tablelist_t
{
  ਬਣਤਰ hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਟੈਗ_2;
} hb_graphite2_tablelist_t;

ਬਣਤਰ hb_graphite2_face_ਅੰਕੜੇ_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

ਸਥਿਰ ਅਚਲ ਖਾਲੀ *hb_graphite2_get_ਸਾਰਣੀ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਟੈਗ_2, size_t *len)
{
  hb_graphite2_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ = (hb_graphite2_face_ਅੰਕੜੇ_t *) ਅੰਕੜੇ;
  hb_graphite2_tablelist_t *tlist = face_ਅੰਕੜੇ->tlist;

  hb_blob_t *blob = ਖਾਲੀ_ਸੂਚਕ;

  ਲਈ (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    ਜੇ (p->ਟੈਗ_2 == ਟੈਗ_2) {
      blob = p->blob;
      ਰੋਕੋ;
    }

  ਜੇ (unlikely (!blob))
  {
    blob = face_ਅੰਕੜੇ->face->reference_ਸਾਰਣੀ (ਟੈਗ_2);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, ਆਕਾਰ (hb_graphite2_tablelist_t));
    ਜੇ (unlikely (!p)) {
      hb_blob_destroy (blob);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    p->blob = blob;
    p->ਟੈਗ_2 = ਟੈਗ_2;

retry:
    hb_graphite2_tablelist_t *tlist = face_ਅੰਕੜੇ->tlist;
    p->next = tlist;

    ਜੇ (unlikely (!face_ਅੰਕੜੇ->tlist.cmpexch (tlist, p)))
      ਜਾਓ retry;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tlen;
  ਅਚਲ ਅੱਖਰ *d = hb_blob_get_ਅੰਕੜੇ (blob, &tlen);
  *len = tlen;
  ਵਾਪਸ d;
}

hb_graphite2_face_ਅੰਕੜੇ_t *
hb_graphite2_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_ਸਾਰਣੀ (HB_GRAPHITE2_ਟੈਗ_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  ਜੇ (!hb_blob_get_ਲੰਮਾਈ (silf_blob))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_graphite2_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_graphite2_face_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (hb_graphite2_face_ਅੰਕੜੇ_t));
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅੰਕੜੇ->face = face;
  ਅਚਲ gr_face_ops ops = {ਆਕਾਰ(gr_face_ops), &hb_graphite2_get_ਸਾਰਣੀ, ਨਿਰਾ};
  ਅੰਕੜੇ->grface = gr_make_face_with_ops (ਅੰਕੜੇ, &ops, gr_face_preloadAll);

  ਜੇ (unlikely (!ਅੰਕੜੇ->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_ਅੰਕੜੇ_destroy (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਅੰਕੜੇ;
}

ਖਾਲੀ
hb_graphite2_shaper_face_ਅੰਕੜੇ_destroy (hb_graphite2_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  hb_graphite2_tablelist_t *tlist = ਅੰਕੜੇ->tlist;

  ਜਦੋਂ (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (ਅੰਕੜੇ->grface);

  hb_free (ਅੰਕੜੇ);
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
  ਅਚਲ hb_graphite2_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ = face->ਅੰਕੜੇ.graphite2;
  ਵਾਪਸ ਅੰਕੜੇ ? ਅੰਕੜੇ->grface : ਖਾਲੀ_ਸੂਚਕ;
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_graphite2_font_ਅੰਕੜੇ_t {};

hb_graphite2_font_ਅੰਕੜੇ_t *
hb_graphite2_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_graphite2_font_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_graphite2_shaper_font_ਅੰਕੜੇ_destroy (hb_graphite2_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
hb_graphite2_font_get_gr_font (hb_font_t *font HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}
#ਜੇ_ਅੰਤ


/*
 * shaper
 */

ਬਣਤਰ hb_graphite2_cluster_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_char;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_chars;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_glyph;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster;
  hb_position_t advance;
};

ਬਣਤਰ hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_ਅਣਵਰਤਿਆ,
		     hb_font_t          *font,
		     hb_ਬਫਰ_t        *ਬਫਰ,
		     ਅਚਲ hb_feature_t *features,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->ਅੰਕੜੇ.graphite2->grface;

  ਅਚਲ ਅੱਖਰ *lang = hb_language_ਮੰਜ਼ਿਲ_string (hb_ਬਫਰ_get_language (ਬਫਰ));
  ਅਚਲ ਅੱਖਰ *lang_ਸਿਰਾ = lang ? strchr (lang, '-') : ਖਾਲੀ_ਸੂਚਕ;
  ਪੂਰਨ lang_len = lang_ਸਿਰਾ ? lang_ਸਿਰਾ - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_ਟੈਗ_from_string (lang, lang_len) : 0);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
  {
    ਅਚਲ gr_feature_ref *fref = gr_face_ਲੱਭੋ_fref (grface, features[i].ਟੈਗ_2);
    ਜੇ (fref)
      gr_fref_ਸੈੱਟ_feature_ਮੁੱਲ (fref, features[i].value, feats);
  }

  hb_direction_t direction = ਬਫਰ->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (ਬਫਰ->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ਜੇ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_ਬਫਰ_ਉਲਟ_clusters (ਬਫਰ);
    direction = HB_DIRECTION_ਉਲਟ (direction);
  }

  gr_segment *seg = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ gr_ਥਾਂ *is;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scratch_ਮਾਪ;
  hb_ਬਫਰ_t::scratch_ਬਫਰ_t *scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);

  uint32_t *chars = (uint32_t *) scratch;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; ++i)
    chars[i] = ਬਫਰ->info[i].codepoint;

  seg = gr_make_seg (ਖਾਲੀ_ਸੂਚਕ, grface,
		     HB_ਟੈਗ_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, ਬਫਰ->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  ਜੇ (unlikely (!seg)) {
    ਜੇ (feats) gr_featureval_destroy (feats);
    ਵਾਪਸ ਝੂਠ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਗਿਣਤੀ = gr_seg_n_slots (seg);
  ਜੇ (unlikely (!glyph_ਗਿਣਤੀ)) {
    ਜੇ (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    ਬਫਰ->len = 0;
    ਵਾਪਸ ਸੱਚ;
  }

  (ਖਾਲੀ) ਬਫਰ->ensure (glyph_ਗਿਣਤੀ);
  scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);
  ਜਦੋਂ ((DIV_CEIL (ਆਕਾਰ (hb_graphite2_cluster_t) * ਬਫਰ->len, ਆਕਾਰ (*scratch)) +
	  DIV_CEIL (ਆਕਾਰ (hb_graphite2_glyph_t) * glyph_ਗਿਣਤੀ, ਆਕਾਰ (*scratch))) > scratch_ਮਾਪ)
  {
    ਜੇ (unlikely (!ਬਫਰ->ensure (ਬਫਰ->allocated * 2)))
    {
      ਜੇ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ਵਾਪਸ ਝੂਠ;
    }
    scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);
  }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ਕਰੋ { \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ _consumed = DIV_CEIL ((len) * ਆਕਾਰ (Type), ਆਕਾਰ (*scratch)); \
    ਪ੍ਰਤਿਜ੍ਞਾ (_consumed <= scratch_ਮਾਪ); \
    scratch += _consumed; \
    scratch_ਮਾਪ -= _consumed; \
  } ਜਦੋਂ (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, ਬਫਰ->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_ਗਿਣਤੀ);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALLOCATE_ARRAY

  hb_memset (clusters, 0, ਆਕਾਰ (clusters[0]) * ਬਫਰ->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = ਬਫਰ->info[0].cluster;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ upem = hb_face_get_upem (face);
  ਦਸ਼ਮਲਵ xscale = (ਦਸ਼ਮਲਵ) font->x_scale / upem;
  ਦਸ਼ਮਲਵ yscale = (ਦਸ਼ਮਲਵ) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  ਜੇ (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (gr_ਥਾਂ_origin_X(gr_seg_ਪਹਿਲਾ_ਥਾਂ(seg)) * xscale);
    clusters[0].advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ਨਹੀਂਤਾਂ
    clusters[0].advance = 0;
  ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg), ic = 0; is; is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is), ic++)
  {
    ਪੂਰਨ ਥਾਂ_before = gr_ਥਾਂ_before (is);
    ਪੂਰਨ ਥਾਂ_after = gr_ਥਾਂ_after (is);
    ਜੇ (unlikely (ਥਾਂ_before < 0 ||
		  ਥਾਂ_after < ਥਾਂ_before ||
		  (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਥਾਂ_after >= ਬਫਰ->len))
    {
      ਜੇ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ਵਾਪਸ ਝੂਠ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ before = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਥਾਂ_before;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ after = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਥਾਂ_after;
    pg->codepoint = gr_ਥਾਂ_gid (is);
    pg->cluster = hb_min (ਬਫਰ->info[before].cluster,
			  ਬਫਰ->info[after].cluster);
    pg++;
    ਜਦੋਂ (clusters[ci].ਅਧਾਰ_char > before && ci)
    {
      clusters[ci-1].ਅੰਕ_chars += clusters[ci].ਅੰਕ_chars;
      clusters[ci-1].ਅੰਕ_glyphs += clusters[ci].ਅੰਕ_glyphs;
      clusters[ci-1].advance = hb_saturate_ਜੋੜੋ (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ਜੇ (gr_ਥਾਂ_can_ਪਾਓ_before (is) && clusters[ci].ਅੰਕ_chars && before >= clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->ਅਧਾਰ_char = clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars;
      c->cluster = ਬਫਰ->info[c->ਅਧਾਰ_char].cluster;
      c->ਅੰਕ_chars = before - c->ਅਧਾਰ_char;
      c->ਅਧਾਰ_glyph = ic;
      c->ਅੰਕ_glyphs = 0;
      ਜੇ (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) curradv -
						   (ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      ਨਹੀਂਤਾਂ
      {
	hb_position_t origin_X = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (gr_ਥਾਂ_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_ਜੋੜੋ (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].ਅੰਕ_glyphs++;

    ਜੇ (clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars < after + 1)
	clusters[ci].ਅੰਕ_chars = after + 1 - clusters[ci].ਅਧਾਰ_char;
  }

  ਜੇ (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_ਜੋੜੋ (clusters[ci].advance, curradv);
  ਨਹੀਂਤਾਂ
    clusters[ci].advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) clusters[ci].advance +
							       (ਦੁੱਗਣਾ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ci; ++i)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < clusters[i].ਅੰਕ_glyphs; ++j)
    {
      hb_glyph_info_t *info = &ਬਫਰ->info[clusters[i].ਅਧਾਰ_glyph + j];
      info->codepoint = glyphs[clusters[i].ਅਧਾਰ_glyph + j].codepoint;
      info->cluster = HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (ਬਫਰ->cluster_level) ?
		      glyphs[clusters[i].ਅਧਾਰ_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  ਬਫਰ->len = glyph_ਗਿਣਤੀ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ci; ++i)
    ਬਫਰ->ਮਿਲਾਓ_clusters (clusters[i].ਅਧਾਰ_glyph,
			    clusters[i].ਅਧਾਰ_glyph + clusters[i].ਅੰਕ_glyphs);

  /* Positioning. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ curr_gr_cluster = UINT_MAX;
  ਅਚਲ hb_glyph_info_t *info = ਬਫਰ->info;
  hb_glyph_position_t *pPos = hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg); is; pPos++, ++info, is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is))
    {
      pPos->x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X (is) * xscale) - curradvx);
      pPos->y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_Y (is) * yscale) - curradvy);
      ਜੇ (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_ਜੋੜੋ (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ਨਹੀਂਤਾਂ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (gr_ਥਾਂ_advance_Y (is, grface, ਖਾਲੀ_ਸੂਚਕ) * yscale);
      curradvy = hb_saturate_ਜੋੜੋ (curradvy, pPos->y_advance);
    }
  }
  ਨਹੀਂਤਾਂ
  {
    curradvx = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg); is; pPos++, info++, is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is))
    {
      ਜੇ (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ਨਹੀਂਤਾਂ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (gr_ਥਾਂ_advance_Y (is, grface, ਖਾਲੀ_ਸੂਚਕ) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_Y (is) * yscale) - curradvy);
    }
    hb_ਬਫਰ_ਉਲਟ_clusters (ਬਫਰ);
  }

  ਜੇ (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  ਬਫਰ->clear_glyph_flags ();
  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();

  ਵਾਪਸ ਸੱਚ;
}


#ਜੇ_ਅੰਤ
