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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_GRAPHITE2

#অন্তর্ভাব "hb-shaper-impl.hh"

#অন্তর্ভাব "hb-graphite2.h"

#অন্তর্ভাব <graphite2/Segment.h>

#অন্তর্ভাব "hb-ot-layout.h"


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

প্রকারলেখ কাঠামো hb_graphite2_tablelist_t
{
  কাঠামো hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  unsigned পূর্ণসংখ্যা tag;
} hb_graphite2_tablelist_t;

কাঠামো hb_graphite2_face_তথ্য_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

স্থির ধ্রুবক শূন্য *hb_graphite2_get_table (ধ্রুবক শূন্য *তথ্য, unsigned পূর্ণসংখ্যা tag, size_t *len)
{
  hb_graphite2_face_তথ্য_t *face_তথ্য = (hb_graphite2_face_তথ্য_t *) তথ্য;
  hb_graphite2_tablelist_t *tlist = face_তথ্য->tlist;

  hb_blob_t *blob = nullptr;

  জন্য (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    যদি (p->tag == tag) {
      blob = p->blob;
      বিরতি;
    }

  যদি (unlikely (!blob))
  {
    blob = face_তথ্য->face->reference_table (tag);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, আকার (hb_graphite2_tablelist_t));
    যদি (unlikely (!p)) {
      hb_blob_destroy (blob);
      ফেরত nullptr;
    }
    p->blob = blob;
    p->tag = tag;

retry:
    hb_graphite2_tablelist_t *tlist = face_তথ্য->tlist;
    p->next = tlist;

    যদি (unlikely (!face_তথ্য->tlist.cmpexch (tlist, p)))
      যাও retry;
  }

  unsigned পূর্ণসংখ্যা tlen;
  ধ্রুবক অক্ষর *d = hb_blob_get_তথ্য (blob, &tlen);
  *len = tlen;
  ফেরত d;
}

hb_graphite2_face_তথ্য_t *
hb_graphite2_shaper_face_তথ্য_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_table (HB_GRAPHITE2_TAG_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  যদি (!hb_blob_get_দৈর্ঘ্য (silf_blob))
    ফেরত nullptr;

  hb_graphite2_face_তথ্য_t *তথ্য = (hb_graphite2_face_তথ্য_t *) hb_calloc (1, আকার (hb_graphite2_face_তথ্য_t));
  যদি (unlikely (!তথ্য))
    ফেরত nullptr;

  তথ্য->face = face;
  ধ্রুবক gr_face_ops ops = {আকার(gr_face_ops), &hb_graphite2_get_table, ফাঁকা};
  তথ্য->grface = gr_make_face_with_ops (তথ্য, &ops, gr_face_preloadAll);

  যদি (unlikely (!তথ্য->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    hb_graphite2_shaper_face_তথ্য_destroy (তথ্য);
    ফেরত nullptr;
  }

  ফেরত তথ্য;
}

শূন্য
hb_graphite2_shaper_face_তথ্য_destroy (hb_graphite2_face_তথ্য_t *তথ্য)
{
  hb_graphite2_tablelist_t *tlist = তথ্য->tlist;

  যখন (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (তথ্য->grface);

  hb_free (তথ্য);
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
  ধ্রুবক hb_graphite2_face_তথ্য_t *তথ্য = face->তথ্য.graphite2;
  ফেরত তথ্য ? তথ্য->grface : nullptr;
}


/*
 * shaper font data
 */

কাঠামো hb_graphite2_font_তথ্য_t {};

hb_graphite2_font_তথ্য_t *
hb_graphite2_shaper_font_তথ্য_create (hb_font_t *font HB_UNUSED)
{
  ফেরত (hb_graphite2_font_তথ্য_t *) HB_SHAPER_তথ্য_SUCCEEDED;
}

শূন্য
hb_graphite2_shaper_font_তথ্য_destroy (hb_graphite2_font_তথ্য_t *তথ্য HB_UNUSED)
{
}

#ifndef HB_DISABLE_DEPRECATED
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
  ফেরত nullptr;
}
#endif


/*
 * shaper
 */

কাঠামো hb_graphite2_cluster_t {
  unsigned পূর্ণসংখ্যা base_char;
  unsigned পূর্ণসংখ্যা অঙ্ক_chars;
  unsigned পূর্ণসংখ্যা base_glyph;
  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  unsigned পূর্ণসংখ্যা cluster;
  hb_position_t advance;
};

কাঠামো hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  unsigned পূর্ণসংখ্যা cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_UNUSED,
		     hb_font_t          *font,
		     hb_বাফার_t        *বাফার,
		     ধ্রুবক hb_feature_t *features,
		     unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->তথ্য.graphite2->grface;

  ধ্রুবক অক্ষর *lang = hb_language_to_string (hb_বাফার_get_language (বাফার));
  ধ্রুবক অক্ষর *lang_end = lang ? strchr (lang, '-') : nullptr;
  পূর্ণসংখ্যা lang_len = lang_end ? lang_end - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_tag_from_string (lang, lang_len) : 0);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_features; i++)
  {
    ধ্রুবক gr_feature_ref *fref = gr_face_find_fref (grface, features[i].tag);
    যদি (fref)
      gr_fref_set_feature_মান (fref, features[i].value, feats);
  }

  hb_direction_t direction = বাফার->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (বাফার->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  যদি ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_বাফার_reverse_clusters (বাফার);
    direction = HB_DIRECTION_REVERSE (direction);
  }

  gr_segment *seg = nullptr;
  ধ্রুবক gr_slot *is;
  unsigned পূর্ণসংখ্যা ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  unsigned পূর্ণসংখ্যা scratch_আকার;
  hb_বাফার_t::scratch_বাফার_t *scratch = বাফার->get_scratch_বাফার (&scratch_আকার);

  uint32_t *chars = (uint32_t *) scratch;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; ++i)
    chars[i] = বাফার->info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     HB_TAG_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, বাফার->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  যদি (unlikely (!seg)) {
    যদি (feats) gr_featureval_destroy (feats);
    ফেরত মিথ্যা;
  }

  unsigned পূর্ণসংখ্যা glyph_গুনতি = gr_seg_n_slots (seg);
  যদি (unlikely (!glyph_গুনতি)) {
    যদি (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    বাফার->len = 0;
    ফেরত সত্য;
  }

  (শূন্য) বাফার->ensure (glyph_গুনতি);
  scratch = বাফার->get_scratch_বাফার (&scratch_আকার);
  যখন ((DIV_CEIL (আকার (hb_graphite2_cluster_t) * বাফার->len, আকার (*scratch)) +
	  DIV_CEIL (আকার (hb_graphite2_glyph_t) * glyph_গুনতি, আকার (*scratch))) > scratch_আকার)
  {
    যদি (unlikely (!বাফার->ensure (বাফার->allocated * 2)))
    {
      যদি (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ফেরত মিথ্যা;
    }
    scratch = বাফার->get_scratch_বাফার (&scratch_আকার);
  }

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  করো { \
    unsigned পূর্ণসংখ্যা _consumed = DIV_CEIL ((len) * আকার (Type), আকার (*scratch)); \
    প্রতিজ্ঞা (_consumed <= scratch_আকার); \
    scratch += _consumed; \
    scratch_আকার -= _consumed; \
  } যখন (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, বাফার->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_গুনতি);

#undef ALLOCATE_ARRAY

  hb_memset (clusters, 0, আকার (clusters[0]) * বাফার->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = বাফার->info[0].cluster;
  unsigned পূর্ণসংখ্যা upem = hb_face_get_upem (face);
  দশমিক xscale = (দশমিক) font->x_scale / upem;
  দশমিক yscale = (দশমিক) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  যদি (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_to<hb_position_t> (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = hb_clamp_to<hb_position_t> ((দ্বিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  অন্যথা
    clusters[0].advance = 0;
  জন্য (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_পরবর্তী_in_segment (is), ic++)
  {
    পূর্ণসংখ্যা slot_before = gr_slot_before (is);
    পূর্ণসংখ্যা slot_after = gr_slot_after (is);
    যদি (unlikely (slot_before < 0 ||
		  slot_after < slot_before ||
		  (unsigned পূর্ণসংখ্যা) slot_after >= বাফার->len))
    {
      যদি (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ফেরত মিথ্যা;
    }
    unsigned পূর্ণসংখ্যা before = (unsigned পূর্ণসংখ্যা) slot_before;
    unsigned পূর্ণসংখ্যা after = (unsigned পূর্ণসংখ্যা) slot_after;
    pg->codepoint = gr_slot_gid (is);
    pg->cluster = hb_min (বাফার->info[before].cluster,
			  বাফার->info[after].cluster);
    pg++;
    যখন (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].অঙ্ক_chars += clusters[ci].অঙ্ক_chars;
      clusters[ci-1].অঙ্ক_glyphs += clusters[ci].অঙ্ক_glyphs;
      clusters[ci-1].advance = hb_saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    যদি (gr_slot_can_ঢোকাও_before (is) && clusters[ci].অঙ্ক_chars && before >= clusters[ci].base_char + clusters[ci].অঙ্ক_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].অঙ্ক_chars;
      c->cluster = বাফার->info[c->base_char].cluster;
      c->অঙ্ক_chars = before - c->base_char;
      c->base_glyph = ic;
      c->অঙ্ক_glyphs = 0;
      যদি (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_to<hb_position_t> ((দ্বিগুণ) curradv -
						   (দ্বিগুণ) (gr_slot_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      অন্যথা
      {
	hb_position_t origin_X = hb_clamp_to<hb_position_t> (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_add (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].অঙ্ক_glyphs++;

    যদি (clusters[ci].base_char + clusters[ci].অঙ্ক_chars < after + 1)
	clusters[ci].অঙ্ক_chars = after + 1 - clusters[ci].base_char;
  }

  যদি (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_add (clusters[ci].advance, curradv);
  অন্যথা
    clusters[ci].advance = hb_clamp_to<hb_position_t> ((দ্বিগুণ) clusters[ci].advance +
							       (দ্বিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ci; ++i)
  {
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < clusters[i].অঙ্ক_glyphs; ++j)
    {
      hb_glyph_info_t *info = &বাফার->info[clusters[i].base_glyph + j];
      info->codepoint = glyphs[clusters[i].base_glyph + j].codepoint;
      info->cluster = HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (বাফার->cluster_level) ?
		      glyphs[clusters[i].base_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  বাফার->len = glyph_গুনতি;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ci; ++i)
    বাফার->merge_clusters (clusters[i].base_glyph,
			    clusters[i].base_glyph + clusters[i].অঙ্ক_glyphs);

  /* Positioning. */
  unsigned পূর্ণসংখ্যা curr_gr_cluster = UINT_MAX;
  ধ্রুবক hb_glyph_info_t *info = বাফার->info;
  hb_glyph_position_t *pPos = hb_বাফার_get_glyph_positions (বাফার, nullptr);
  যদি (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    জন্য (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_পরবর্তী_in_segment (is))
    {
      pPos->x_offset = hb_clamp_to<hb_position_t> ((দ্বিগুণ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((দ্বিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      যদি (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_add (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_add (curradvy, pPos->y_advance);
    }
  }
  অন্যথা
  {
    curradvx = hb_clamp_to<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    জন্য (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_পরবর্তী_in_segment (is))
    {
      যদি (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = hb_clamp_to<hb_position_t> ((দ্বিগুণ) (gr_slot_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((দ্বিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    hb_বাফার_reverse_clusters (বাফার);
  }

  যদি (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  বাফার->clear_glyph_flags ();
  বাফার->unsafe_to_break ();

  ফেরত সত্য;
}


#endif
