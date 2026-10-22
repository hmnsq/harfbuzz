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

#समावेश "hb.hh"

#अगर_घोषित HAVE_GRAPHITE2

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-graphite2.h"

#समावेश <graphite2/Segment.h>

#समावेश "hb-ot-layout.h"


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

प्रकार काष्ठा hb_graphite2_tablelist_t
{
  काष्ठा hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  अचिन्हित पूर्णांक tag;
} hb_graphite2_tablelist_t;

काष्ठा hb_graphite2_face_तथ्य_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

अटल स्थिर व्योम *hb_graphite2_get_table (स्थिर व्योम *तथ्य, अचिन्हित पूर्णांक tag, माप_प्रकार *len)
{
  hb_graphite2_face_तथ्य_t *face_तथ्य = (hb_graphite2_face_तथ्य_t *) तथ्य;
  hb_graphite2_tablelist_t *tlist = face_तथ्य->tlist;

  hb_blob_t *blob = nullptr;

  क्रम (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    अगर (p->tag == tag) {
      blob = p->blob;
      अवरोध;
    }

  अगर (unlikely (!blob))
  {
    blob = face_तथ्य->face->reference_table (tag);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, माप (hb_graphite2_tablelist_t));
    अगर (unlikely (!p)) {
      hb_blob_destroy (blob);
      वापस nullptr;
    }
    p->blob = blob;
    p->tag = tag;

retry:
    hb_graphite2_tablelist_t *tlist = face_तथ्य->tlist;
    p->next = tlist;

    अगर (unlikely (!face_तथ्य->tlist.cmpexch (tlist, p)))
      जाओ retry;
  }

  अचिन्हित पूर्णांक tlen;
  स्थिर अक्षर *d = hb_blob_get_तथ्य (blob, &tlen);
  *len = tlen;
  वापस d;
}

hb_graphite2_face_तथ्य_t *
hb_graphite2_shaper_face_तथ्य_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_table (HB_GRAPHITE2_TAG_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  अगर (!hb_blob_get_लंबाई (silf_blob))
    वापस nullptr;

  hb_graphite2_face_तथ्य_t *तथ्य = (hb_graphite2_face_तथ्य_t *) hb_calloc (1, माप (hb_graphite2_face_तथ्य_t));
  अगर (unlikely (!तथ्य))
    वापस nullptr;

  तथ्य->face = face;
  स्थिर gr_face_ops ops = {माप(gr_face_ops), &hb_graphite2_get_table, शून्य};
  तथ्य->grface = gr_make_face_with_ops (तथ्य, &ops, gr_face_preloadAll);

  अगर (unlikely (!तथ्य->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_तथ्य_destroy (तथ्य);
    वापस nullptr;
  }

  वापस तथ्य;
}

व्योम
hb_graphite2_shaper_face_तथ्य_destroy (hb_graphite2_face_तथ्य_t *तथ्य)
{
  hb_graphite2_tablelist_t *tlist = तथ्य->tlist;

  जबतक (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (तथ्य->grface);

  hb_free (तथ्य);
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
  स्थिर hb_graphite2_face_तथ्य_t *तथ्य = face->तथ्य.graphite2;
  वापस तथ्य ? तथ्य->grface : nullptr;
}


/*
 * shaper font data
 */

काष्ठा hb_graphite2_font_तथ्य_t {};

hb_graphite2_font_तथ्य_t *
hb_graphite2_shaper_font_तथ्य_create (hb_font_t *font HB_UNUSED)
{
  वापस (hb_graphite2_font_तथ्य_t *) HB_SHAPER_तथ्य_SUCCEEDED;
}

व्योम
hb_graphite2_shaper_font_तथ्य_destroy (hb_graphite2_font_तथ्य_t *तथ्य HB_UNUSED)
{
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
hb_graphite2_font_get_gr_font (hb_font_t *font HB_UNUSED)
{
  वापस nullptr;
}
#पूर्ण_अगर


/*
 * shaper
 */

काष्ठा hb_graphite2_cluster_t {
  अचिन्हित पूर्णांक base_char;
  अचिन्हित पूर्णांक अंक_chars;
  अचिन्हित पूर्णांक base_glyph;
  अचिन्हित पूर्णांक अंक_glyphs;
  अचिन्हित पूर्णांक cluster;
  hb_position_t advance;
};

काष्ठा hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  अचिन्हित पूर्णांक cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_UNUSED,
		     hb_font_t          *font,
		     hb_बफर_t        *बफर,
		     स्थिर hb_feature_t *features,
		     अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->तथ्य.graphite2->grface;

  स्थिर अक्षर *lang = hb_language_to_string (hb_बफर_get_language (बफर));
  स्थिर अक्षर *lang_end = lang ? म_अक्षर (lang, '-') : nullptr;
  पूर्णांक lang_len = lang_end ? lang_end - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_tag_from_string (lang, lang_len) : 0);

  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
  {
    स्थिर gr_feature_ref *fref = gr_face_find_fref (grface, features[i].tag);
    अगर (fref)
      gr_fref_set_feature_मूल्य (fref, features[i].value, feats);
  }

  hb_direction_t direction = बफर->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (बफर->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  अगर ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_बफर_reverse_clusters (बफर);
    direction = HB_DIRECTION_REVERSE (direction);
  }

  gr_segment *seg = nullptr;
  स्थिर gr_slot *is;
  अचिन्हित पूर्णांक ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  अचिन्हित पूर्णांक scratch_आकार;
  hb_बफर_t::scratch_बफर_t *scratch = बफर->get_scratch_बफर (&scratch_आकार);

  uint32_t *chars = (uint32_t *) scratch;

  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; ++i)
    chars[i] = बफर->info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     HB_TAG_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, बफर->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  अगर (unlikely (!seg)) {
    अगर (feats) gr_featureval_destroy (feats);
    वापस false;
  }

  अचिन्हित पूर्णांक glyph_गिनती = gr_seg_n_slots (seg);
  अगर (unlikely (!glyph_गिनती)) {
    अगर (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    बफर->len = 0;
    वापस true;
  }

  (व्योम) बफर->ensure (glyph_गिनती);
  scratch = बफर->get_scratch_बफर (&scratch_आकार);
  जबतक ((DIV_CEIL (माप (hb_graphite2_cluster_t) * बफर->len, माप (*scratch)) +
	  DIV_CEIL (माप (hb_graphite2_glyph_t) * glyph_गिनती, माप (*scratch))) > scratch_आकार)
  {
    अगर (unlikely (!बफर->ensure (बफर->allocated * 2)))
    {
      अगर (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      वापस false;
    }
    scratch = बफर->get_scratch_बफर (&scratch_आकार);
  }

#घोषणा ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  करो { \
    अचिन्हित पूर्णांक _consumed = DIV_CEIL ((len) * माप (Type), माप (*scratch)); \
    निश्चित (_consumed <= scratch_आकार); \
    scratch += _consumed; \
    scratch_आकार -= _consumed; \
  } जबतक (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, बफर->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_गिनती);

#अघोषित ALLOCATE_ARRAY

  hb_memset (clusters, 0, माप (clusters[0]) * बफर->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = बफर->info[0].cluster;
  अचिन्हित पूर्णांक upem = hb_face_get_upem (face);
  भग्न xscale = (भग्न) font->x_scale / upem;
  भग्न yscale = (भग्न) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  अगर (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_to<hb_position_t> (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = hb_clamp_to<hb_position_t> ((द्विगुन) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  अन्यथा
    clusters[0].advance = 0;
  क्रम (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_अग्र_in_segment (is), ic++)
  {
    पूर्णांक slot_before = gr_slot_before (is);
    पूर्णांक slot_after = gr_slot_after (is);
    अगर (unlikely (slot_before < 0 ||
		  slot_after < slot_before ||
		  (अचिन्हित पूर्णांक) slot_after >= बफर->len))
    {
      अगर (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      वापस false;
    }
    अचिन्हित पूर्णांक before = (अचिन्हित पूर्णांक) slot_before;
    अचिन्हित पूर्णांक after = (अचिन्हित पूर्णांक) slot_after;
    pg->codepoint = gr_slot_gid (is);
    pg->cluster = hb_min (बफर->info[before].cluster,
			  बफर->info[after].cluster);
    pg++;
    जबतक (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अंक_chars += clusters[ci].अंक_chars;
      clusters[ci-1].अंक_glyphs += clusters[ci].अंक_glyphs;
      clusters[ci-1].advance = hb_saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    अगर (gr_slot_can_जोड़ो_before (is) && clusters[ci].अंक_chars && before >= clusters[ci].base_char + clusters[ci].अंक_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अंक_chars;
      c->cluster = बफर->info[c->base_char].cluster;
      c->अंक_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अंक_glyphs = 0;
      अगर (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_to<hb_position_t> ((द्विगुन) curradv -
						   (द्विगुन) (gr_slot_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      अन्यथा
      {
	hb_position_t origin_X = hb_clamp_to<hb_position_t> (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_add (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].अंक_glyphs++;

    अगर (clusters[ci].base_char + clusters[ci].अंक_chars < after + 1)
	clusters[ci].अंक_chars = after + 1 - clusters[ci].base_char;
  }

  अगर (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = hb_clamp_to<hb_position_t> ((द्विगुन) clusters[ci].advance +
							       (द्विगुन) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  क्रम (अचिन्हित पूर्णांक i = 0; i < ci; ++i)
  {
    क्रम (अचिन्हित पूर्णांक j = 0; j < clusters[i].अंक_glyphs; ++j)
    {
      hb_glyph_info_t *info = &बफर->info[clusters[i].base_glyph + j];
      info->codepoint = glyphs[clusters[i].base_glyph + j].codepoint;
      info->cluster = HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (बफर->cluster_level) ?
		      glyphs[clusters[i].base_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  बफर->len = glyph_गिनती;

  क्रम (अचिन्हित पूर्णांक i = 0; i < ci; ++i)
    बफर->merge_clusters (clusters[i].base_glyph,
			    clusters[i].base_glyph + clusters[i].अंक_glyphs);

  /* Positioning. */
  अचिन्हित पूर्णांक curr_gr_cluster = अच_पूर्णांक_उच्च;
  स्थिर hb_glyph_info_t *info = बफर->info;
  hb_glyph_position_t *pPos = hb_बफर_get_glyph_positions (बफर, nullptr);
  अगर (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    क्रम (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_अग्र_in_segment (is))
    {
      pPos->x_offset = hb_clamp_to<hb_position_t> ((द्विगुन) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((द्विगुन) (gr_slot_origin_Y (is) * yscale) - curradvy);
      अगर (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_add (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_add (curradvy, pPos->y_advance);
    }
  }
  अन्यथा
  {
    curradvx = hb_clamp_to<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    क्रम (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_अग्र_in_segment (is))
    {
      अगर (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = hb_clamp_to<hb_position_t> ((द्विगुन) (gr_slot_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((द्विगुन) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    hb_बफर_reverse_clusters (बफर);
  }

  अगर (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  बफर->clear_glyph_flags ();
  बफर->unsafe_to_break ();

  वापस true;
}


#पूर्ण_अगर
