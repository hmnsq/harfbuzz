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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_GRAPHITE2

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-graphite2.h"

#అన్తర్భావ <graphite2/Segment.h>

#అన్తర్భావ "hb-ot-layout.h"


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

రకనిర్వచనం నిర్మాణం hb_graphite2_tablelist_t
{
  నిర్మాణం hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  గుర్తులేని పూర్ణాంకం ట్యాగ్_2;
} hb_graphite2_tablelist_t;

నిర్మాణం hb_graphite2_face_దత్తాంశం_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

స్థిర స్థిరరాశి శూన్య *hb_graphite2_get_పట్టిక (స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని పూర్ణాంకం ట్యాగ్_2, size_t *len)
{
  hb_graphite2_face_దత్తాంశం_t *face_దత్తాంశం = (hb_graphite2_face_దత్తాంశం_t *) దత్తాంశం;
  hb_graphite2_tablelist_t *tlist = face_దత్తాంశం->tlist;

  hb_blob_t *blob = శూన్యసూచిక;

  కొరకు (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    ఒకవేళ (p->ట్యాగ్_2 == ట్యాగ్_2) {
      blob = p->blob;
      ఆపు;
    }

  ఒకవేళ (unlikely (!blob))
  {
    blob = face_దత్తాంశం->face->reference_పట్టిక (ట్యాగ్_2);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, పరిమాణం (hb_graphite2_tablelist_t));
    ఒకవేళ (unlikely (!p)) {
      hb_blob_destroy (blob);
      తిరిగి శూన్యసూచిక;
    }
    p->blob = blob;
    p->ట్యాగ్_2 = ట్యాగ్_2;

retry:
    hb_graphite2_tablelist_t *tlist = face_దత్తాంశం->tlist;
    p->next = tlist;

    ఒకవేళ (unlikely (!face_దత్తాంశం->tlist.cmpexch (tlist, p)))
      వెళ్ళు retry;
  }

  గుర్తులేని పూర్ణాంకం tlen;
  స్థిరరాశి అక్షరం *d = hb_blob_get_దత్తాంశం (blob, &tlen);
  *len = tlen;
  తిరిగి d;
}

hb_graphite2_face_దత్తాంశం_t *
hb_graphite2_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_పట్టిక (HB_GRAPHITE2_ట్యాగ్_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  ఒకవేళ (!hb_blob_get_నిడివి (silf_blob))
    తిరిగి శూన్యసూచిక;

  hb_graphite2_face_దత్తాంశం_t *దత్తాంశం = (hb_graphite2_face_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (hb_graphite2_face_దత్తాంశం_t));
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  దత్తాంశం->face = face;
  స్థిరరాశి gr_face_ops ops = {పరిమాణం(gr_face_ops), &hb_graphite2_get_పట్టిక, ఖాళీ};
  దత్తాంశం->grface = gr_make_face_with_ops (దత్తాంశం, &ops, gr_face_preloadAll);

  ఒకవేళ (unlikely (!దత్తాంశం->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_దత్తాంశం_destroy (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి దత్తాంశం;
}

శూన్య
hb_graphite2_shaper_face_దత్తాంశం_destroy (hb_graphite2_face_దత్తాంశం_t *దత్తాంశం)
{
  hb_graphite2_tablelist_t *tlist = దత్తాంశం->tlist;

  అయితే (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (దత్తాంశం->grface);

  hb_free (దత్తాంశం);
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
  స్థిరరాశి hb_graphite2_face_దత్తాంశం_t *దత్తాంశం = face->దత్తాంశం.graphite2;
  తిరిగి దత్తాంశం ? దత్తాంశం->grface : శూన్యసూచిక;
}


/*
 * shaper font data
 */

నిర్మాణం hb_graphite2_font_దత్తాంశం_t {};

hb_graphite2_font_దత్తాంశం_t *
hb_graphite2_shaper_font_దత్తాంశం_create (hb_font_t *font HB_వాడని)
{
  తిరిగి (hb_graphite2_font_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_graphite2_shaper_font_దత్తాంశం_destroy (hb_graphite2_font_దత్తాంశం_t *దత్తాంశం HB_వాడని)
{
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
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
hb_graphite2_font_get_gr_font (hb_font_t *font HB_వాడని)
{
  తిరిగి శూన్యసూచిక;
}
#అయితే_ముగింపు


/*
 * shaper
 */

నిర్మాణం hb_graphite2_cluster_t {
  గుర్తులేని పూర్ణాంకం పునాది_char;
  గుర్తులేని పూర్ణాంకం అంకె_chars;
  గుర్తులేని పూర్ణాంకం పునాది_glyph;
  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  గుర్తులేని పూర్ణాంకం cluster;
  hb_position_t advance;
};

నిర్మాణం hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  గుర్తులేని పూర్ణాంకం cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_వాడని,
		     hb_font_t          *font,
		     hb_బఫరు_t        *బఫరు,
		     స్థిరరాశి hb_feature_t *features,
		     గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->దత్తాంశం.graphite2->grface;

  స్థిరరాశి అక్షరం *lang = hb_language_గమ్యం_string (hb_బఫరు_get_language (బఫరు));
  స్థిరరాశి అక్షరం *lang_చివర = lang ? strchr (lang, '-') : శూన్యసూచిక;
  పూర్ణాంకం lang_len = lang_చివర ? lang_చివర - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_ట్యాగ్_from_string (lang, lang_len) : 0);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
  {
    స్థిరరాశి gr_feature_ref *fref = gr_face_కనుగొను_fref (grface, features[i].ట్యాగ్_2);
    ఒకవేళ (fref)
      gr_fref_సమితి_feature_విలువ (fref, features[i].value, feats);
  }

  hb_direction_t direction = బఫరు->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (బఫరు->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ఒకవేళ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_బఫరు_తిరగేయి_clusters (బఫరు);
    direction = HB_DIRECTION_తిరగేయి (direction);
  }

  gr_segment *seg = శూన్యసూచిక;
  స్థిరరాశి gr_స్థానం *is;
  గుర్తులేని పూర్ణాంకం ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  గుర్తులేని పూర్ణాంకం scratch_కొలత;
  hb_బఫరు_t::scratch_బఫరు_t *scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);

  uint32_t *chars = (uint32_t *) scratch;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; ++i)
    chars[i] = బఫరు->info[i].codepoint;

  seg = gr_make_seg (శూన్యసూచిక, grface,
		     HB_ట్యాగ్_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, బఫరు->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  ఒకవేళ (unlikely (!seg)) {
    ఒకవేళ (feats) gr_featureval_destroy (feats);
    తిరిగి అబద్ధం;
  }

  గుర్తులేని పూర్ణాంకం glyph_లెక్క = gr_seg_n_slots (seg);
  ఒకవేళ (unlikely (!glyph_లెక్క)) {
    ఒకవేళ (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    బఫరు->len = 0;
    తిరిగి నిజం;
  }

  (శూన్య) బఫరు->ensure (glyph_లెక్క);
  scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);
  అయితే ((DIV_CEIL (పరిమాణం (hb_graphite2_cluster_t) * బఫరు->len, పరిమాణం (*scratch)) +
	  DIV_CEIL (పరిమాణం (hb_graphite2_glyph_t) * glyph_లెక్క, పరిమాణం (*scratch))) > scratch_కొలత)
  {
    ఒకవేళ (unlikely (!బఫరు->ensure (బఫరు->allocated * 2)))
    {
      ఒకవేళ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      తిరిగి అబద్ధం;
    }
    scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);
  }

#నిర్వచించు ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  చేయి { \
    గుర్తులేని పూర్ణాంకం _consumed = DIV_CEIL ((len) * పరిమాణం (Type), పరిమాణం (*scratch)); \
    ప్రతిజ్ఞా (_consumed <= scratch_కొలత); \
    scratch += _consumed; \
    scratch_కొలత -= _consumed; \
  } అయితే (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, బఫరు->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_లెక్క);

#నిర్వచనం_రద్దు ALLOCATE_ARRAY

  hb_memset (clusters, 0, పరిమాణం (clusters[0]) * బఫరు->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = బఫరు->info[0].cluster;
  గుర్తులేని పూర్ణాంకం upem = hb_face_get_upem (face);
  దశాంశం xscale = (దశాంశం) font->x_scale / upem;
  దశాంశం yscale = (దశాంశం) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_గమ్యం<hb_position_t> (gr_స్థానం_origin_X(gr_seg_మొదటి_స్థానం(seg)) * xscale);
    clusters[0].advance = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  లేకపోతే
    clusters[0].advance = 0;
  కొరకు (is = gr_seg_మొదటి_స్థానం (seg), ic = 0; is; is = gr_స్థానం_తదుపరి_లోపల_segment (is), ic++)
  {
    పూర్ణాంకం స్థానం_before = gr_స్థానం_before (is);
    పూర్ణాంకం స్థానం_after = gr_స్థానం_after (is);
    ఒకవేళ (unlikely (స్థానం_before < 0 ||
		  స్థానం_after < స్థానం_before ||
		  (గుర్తులేని పూర్ణాంకం) స్థానం_after >= బఫరు->len))
    {
      ఒకవేళ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      తిరిగి అబద్ధం;
    }
    గుర్తులేని పూర్ణాంకం before = (గుర్తులేని పూర్ణాంకం) స్థానం_before;
    గుర్తులేని పూర్ణాంకం after = (గుర్తులేని పూర్ణాంకం) స్థానం_after;
    pg->codepoint = gr_స్థానం_gid (is);
    pg->cluster = hb_min (బఫరు->info[before].cluster,
			  బఫరు->info[after].cluster);
    pg++;
    అయితే (clusters[ci].పునాది_char > before && ci)
    {
      clusters[ci-1].అంకె_chars += clusters[ci].అంకె_chars;
      clusters[ci-1].అంకె_glyphs += clusters[ci].అంకె_glyphs;
      clusters[ci-1].advance = hb_saturate_కూడు (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ఒకవేళ (gr_స్థానం_can_చొప్పించు_before (is) && clusters[ci].అంకె_chars && before >= clusters[ci].పునాది_char + clusters[ci].అంకె_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->పునాది_char = clusters[ci].పునాది_char + clusters[ci].అంకె_chars;
      c->cluster = బఫరు->info[c->పునాది_char].cluster;
      c->అంకె_chars = before - c->పునాది_char;
      c->పునాది_glyph = ic;
      c->అంకె_glyphs = 0;
      ఒకవేళ (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) curradv -
						   (ద్విగుణ) (gr_స్థానం_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      లేకపోతే
      {
	hb_position_t origin_X = hb_clamp_గమ్యం<hb_position_t> (gr_స్థానం_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_కూడు (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].అంకె_glyphs++;

    ఒకవేళ (clusters[ci].పునాది_char + clusters[ci].అంకె_chars < after + 1)
	clusters[ci].అంకె_chars = after + 1 - clusters[ci].పునాది_char;
  }

  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_కూడు (clusters[ci].advance, curradv);
  లేకపోతే
    clusters[ci].advance = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) clusters[ci].advance +
							       (ద్విగుణ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ci; ++i)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < clusters[i].అంకె_glyphs; ++j)
    {
      hb_glyph_info_t *info = &బఫరు->info[clusters[i].పునాది_glyph + j];
      info->codepoint = glyphs[clusters[i].పునాది_glyph + j].codepoint;
      info->cluster = HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (బఫరు->cluster_level) ?
		      glyphs[clusters[i].పునాది_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  బఫరు->len = glyph_లెక్క;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ci; ++i)
    బఫరు->విలీనం_clusters (clusters[i].పునాది_glyph,
			    clusters[i].పునాది_glyph + clusters[i].అంకె_glyphs);

  /* Positioning. */
  గుర్తులేని పూర్ణాంకం curr_gr_cluster = UINT_MAX;
  స్థిరరాశి hb_glyph_info_t *info = బఫరు->info;
  hb_glyph_position_t *pPos = hb_బఫరు_get_glyph_positions (బఫరు, శూన్యసూచిక);
  ఒకవేళ (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    కొరకు (is = gr_seg_మొదటి_స్థానం (seg); is; pPos++, ++info, is = gr_స్థానం_తదుపరి_లోపల_segment (is))
    {
      pPos->x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) (gr_స్థానం_origin_X (is) * xscale) - curradvx);
      pPos->y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) (gr_స్థానం_origin_Y (is) * yscale) - curradvy);
      ఒకవేళ (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_కూడు (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } లేకపోతే
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_గమ్యం<hb_position_t> (gr_స్థానం_advance_Y (is, grface, శూన్యసూచిక) * yscale);
      curradvy = hb_saturate_కూడు (curradvy, pPos->y_advance);
    }
  }
  లేకపోతే
  {
    curradvx = hb_clamp_గమ్యం<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    కొరకు (is = gr_seg_మొదటి_స్థానం (seg); is; pPos++, info++, is = gr_స్థానం_తదుపరి_లోపల_segment (is))
    {
      ఒకవేళ (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } లేకపోతే
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_గమ్యం<hb_position_t> (gr_స్థానం_advance_Y (is, grface, శూన్యసూచిక) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) (gr_స్థానం_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) (gr_స్థానం_origin_Y (is) * yscale) - curradvy);
    }
    hb_బఫరు_తిరగేయి_clusters (బఫరు);
  }

  ఒకవేళ (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  బఫరు->clear_glyph_flags ();
  బఫరు->unsafe_గమ్యం_break ();

  తిరిగి నిజం;
}


#అయితే_ముగింపు
