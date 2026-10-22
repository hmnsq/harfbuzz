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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_GRAPHITE2

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

#ಅನ್ತರ್ಭಾವ "hb-graphite2.h"

#ಅನ್ತರ್ಭಾವ <graphite2/Segment.h>

#ಅನ್ತರ್ಭಾವ "hb-ot-layout.h"


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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_graphite2_tablelist_t
{
  ಸಂರಚನಾ hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  unsigned ಪೂರ್ಣಾಙ್ಕ tag;
} hb_graphite2_tablelist_t;

ಸಂರಚನಾ hb_graphite2_face_data_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

ಸ್ಥಿರ ಧ್ರುವ ಶೂನ್ಯ *hb_graphite2_get_table (ಧ್ರುವ ಶೂನ್ಯ *data, unsigned ಪೂರ್ಣಾಙ್ಕ tag, size_t *len)
{
  hb_graphite2_face_data_t *face_data = (hb_graphite2_face_data_t *) data;
  hb_graphite2_tablelist_t *tlist = face_data->tlist;

  hb_blob_t *blob = nullptr;

  ಪ್ರತಿ (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    ಯದಿ (p->tag == tag) {
      blob = p->blob;
      ಭಙ್ಗ;
    }

  ಯದಿ (unlikely (!blob))
  {
    blob = face_data->face->reference_table (tag);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_graphite2_tablelist_t));
    ಯದಿ (unlikely (!p)) {
      hb_blob_destroy (blob);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    p->blob = blob;
    p->tag = tag;

retry:
    hb_graphite2_tablelist_t *tlist = face_data->tlist;
    p->next = tlist;

    ಯದಿ (unlikely (!face_data->tlist.cmpexch (tlist, p)))
      ಗಚ್ಛ retry;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ tlen;
  ಧ್ರುವ ವರ್ಣ *d = hb_blob_get_data (blob, &tlen);
  *len = tlen;
  ಪ್ರತ್ಯಾಗಮ d;
}

hb_graphite2_face_data_t *
_hb_graphite2_shaper_face_data_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_table (HB_GRAPHITE2_TAG_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  ಯದಿ (!hb_blob_get_length (silf_blob))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_graphite2_face_data_t *data = (hb_graphite2_face_data_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_graphite2_face_data_t));
  ಯದಿ (unlikely (!data))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  data->face = face;
  ಧ್ರುವ gr_face_ops ops = {ಪರಿಮಾಣ(gr_face_ops), &hb_graphite2_get_table, ರಿಕ್ತ};
  data->grface = gr_make_face_with_ops (data, &ops, gr_face_preloadAll);

  ಯದಿ (unlikely (!data->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    _hb_graphite2_shaper_face_data_destroy (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ data;
}

ಶೂನ್ಯ
_hb_graphite2_shaper_face_data_destroy (hb_graphite2_face_data_t *data)
{
  hb_graphite2_tablelist_t *tlist = data->tlist;

  ಯಾವತ್ (tlist)
  {
    hb_graphite2_tablelist_t *old = tlist;
    hb_blob_destroy (tlist->blob);
    tlist = tlist->next;
    hb_free (old);
  }

  gr_face_destroy (data->grface);

  hb_free (data);
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
  ಧ್ರುವ hb_graphite2_face_data_t *data = face->data.graphite2;
  ಪ್ರತ್ಯಾಗಮ data ? data->grface : nullptr;
}


/*
 * shaper font data
 */

ಸಂರಚನಾ hb_graphite2_font_data_t {};

hb_graphite2_font_data_t *
_hb_graphite2_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ (hb_graphite2_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ಶೂನ್ಯ
_hb_graphite2_shaper_font_data_destroy (hb_graphite2_font_data_t *data HB_UNUSED)
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
  ಪ್ರತ್ಯಾಗಮ nullptr;
}
#endif


/*
 * shaper
 */

ಸಂರಚನಾ hb_graphite2_cluster_t {
  unsigned ಪೂರ್ಣಾಙ್ಕ base_char;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_chars;
  unsigned ಪೂರ್ಣಾಙ್ಕ base_glyph;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
  unsigned ಪೂರ್ಣಾಙ್ಕ cluster;
  hb_position_t advance;
};

ಸಂರಚನಾ hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  unsigned ಪೂರ್ಣಾಙ್ಕ cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_UNUSED,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     ಧ್ರುವ hb_feature_t *features,
		     unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->data.graphite2->grface;

  ಧ್ರುವ ವರ್ಣ *lang = hb_language_to_string (hb_buffer_get_language (buffer));
  ಧ್ರುವ ವರ್ಣ *lang_end = lang ? strchr (lang, '-') : nullptr;
  ಪೂರ್ಣಾಙ್ಕ lang_len = lang_end ? lang_end - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_tag_from_string (lang, lang_len) : 0);

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_features; i++)
  {
    ಧ್ರುವ gr_feature_ref *fref = gr_face_find_fref (grface, features[i].tag);
    ಯದಿ (fref)
      gr_fref_set_feature_ಮೌಲ್ಯ (fref, features[i].value, feats);
  }

  hb_direction_t direction = buffer->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (buffer->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ಯದಿ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_buffer_reverse_clusters (buffer);
    direction = HB_DIRECTION_REVERSE (direction);
  }

  gr_segment *seg = nullptr;
  ಧ್ರುವ gr_slot *is;
  unsigned ಪೂರ್ಣಾಙ್ಕ ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  unsigned ಪೂರ್ಣಾಙ್ಕ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

  uint32_t *chars = (uint32_t *) scratch;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; ++i)
    chars[i] = buffer->info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     HB_TAG_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, buffer->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  ಯದಿ (unlikely (!seg)) {
    ಯದಿ (feats) gr_featureval_destroy (feats);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ glyph_ಎಣಿಕೆ = gr_seg_n_slots (seg);
  ಯದಿ (unlikely (!glyph_ಎಣಿಕೆ)) {
    ಯದಿ (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    buffer->len = 0;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  (ಶೂನ್ಯ) buffer->ensure (glyph_ಎಣಿಕೆ);
  scratch = buffer->get_scratch_buffer (&scratch_size);
  ಯಾವತ್ ((DIV_CEIL (ಪರಿಮಾಣ (hb_graphite2_cluster_t) * buffer->len, ಪರಿಮಾಣ (*scratch)) +
	  DIV_CEIL (ಪರಿಮಾಣ (hb_graphite2_glyph_t) * glyph_ಎಣಿಕೆ, ಪರಿಮಾಣ (*scratch))) > scratch_size)
  {
    ಯದಿ (unlikely (!buffer->ensure (buffer->allocated * 2)))
    {
      ಯದಿ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    scratch = buffer->get_scratch_buffer (&scratch_size);
  }

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ಕುರು { \
    unsigned ಪೂರ್ಣಾಙ್ಕ _consumed = DIV_CEIL ((len) * ಪರಿಮಾಣ (Type), ಪರಿಮಾಣ (*scratch)); \
    ಪ್ರತಿಜ್ಞಾ (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ಯಾವತ್ (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, buffer->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_ಎಣಿಕೆ);

#undef ALLOCATE_ARRAY

  hb_memset (clusters, 0, ಪರಿಮಾಣ (clusters[0]) * buffer->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = buffer->info[0].cluster;
  unsigned ಪೂರ್ಣಾಙ್ಕ upem = hb_face_get_upem (face);
  ಭಿನ್ನ xscale = (ಭಿನ್ನ) font->x_scale / upem;
  ಭಿನ್ನ yscale = (ಭಿನ್ನ) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  ಯದಿ (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_to<hb_position_t> (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ಅನ್ಯಥಾ
    clusters[0].advance = 0;
  ಪ್ರತಿ (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    ಪೂರ್ಣಾಙ್ಕ slot_before = gr_slot_before (is);
    ಪೂರ್ಣಾಙ್ಕ slot_after = gr_slot_after (is);
    ಯದಿ (unlikely (slot_before < 0 ||
		  slot_after < slot_before ||
		  (unsigned ಪೂರ್ಣಾಙ್ಕ) slot_after >= buffer->len))
    {
      ಯದಿ (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ before = (unsigned ಪೂರ್ಣಾಙ್ಕ) slot_before;
    unsigned ಪೂರ್ಣಾಙ್ಕ after = (unsigned ಪೂರ್ಣಾಙ್ಕ) slot_after;
    pg->codepoint = gr_slot_gid (is);
    pg->cluster = hb_min (buffer->info[before].cluster,
			  buffer->info[after].cluster);
    pg++;
    ಯಾವತ್ (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].ಅಂಕೆ_chars += clusters[ci].ಅಂಕೆ_chars;
      clusters[ci-1].ಅಂಕೆ_glyphs += clusters[ci].ಅಂಕೆ_glyphs;
      clusters[ci-1].advance = hb_saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ಯದಿ (gr_slot_can_ಸೇರಿಸು_before (is) && clusters[ci].ಅಂಕೆ_chars && before >= clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars;
      c->cluster = buffer->info[c->base_char].cluster;
      c->ಅಂಕೆ_chars = before - c->base_char;
      c->base_glyph = ic;
      c->ಅಂಕೆ_glyphs = 0;
      ಯದಿ (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) curradv -
						   (ದ್ವಿಗುಣ) (gr_slot_origin_X(is) * xscale));
	curradv = hb_saturate_sub (curradv, c->advance);
      }
      ಅನ್ಯಥಾ
      {
	hb_position_t origin_X = hb_clamp_to<hb_position_t> (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = hb_saturate_add (clusters[ci].advance,
						      hb_saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].ಅಂಕೆ_glyphs++;

    ಯದಿ (clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars < after + 1)
	clusters[ci].ಅಂಕೆ_chars = after + 1 - clusters[ci].base_char;
  }

  ಯದಿ (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_add (clusters[ci].advance, curradv);
  ಅನ್ಯಥಾ
    clusters[ci].advance = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) clusters[ci].advance +
							       (ದ್ವಿಗುಣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ci; ++i)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < clusters[i].ಅಂಕೆ_glyphs; ++j)
    {
      hb_glyph_info_t *info = &buffer->info[clusters[i].base_glyph + j];
      info->codepoint = glyphs[clusters[i].base_glyph + j].codepoint;
      info->cluster = HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level) ?
		      glyphs[clusters[i].base_glyph + j].cluster :
		      clusters[i].cluster;
      info->var1.i32 = clusters[i].advance;     // all glyphs in the cluster get the same advance
      info->var2.u32 = i;                       // Graphite cluster index
    }
  }
  buffer->len = glyph_ಎಣಿಕೆ;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ci; ++i)
    buffer->merge_clusters (clusters[i].base_glyph,
			    clusters[i].base_glyph + clusters[i].ಅಂಕೆ_glyphs);

  /* Positioning. */
  unsigned ಪೂರ್ಣಾಙ್ಕ curr_gr_cluster = UINT_MAX;
  ಧ್ರುವ hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pPos = hb_buffer_get_glyph_positions (buffer, nullptr);
  ಯದಿ (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    ಪ್ರತಿ (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      ಯದಿ (info->var2.u32 != curr_gr_cluster) {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_add (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ಅನ್ಯಥಾ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_add (curradvy, pPos->y_advance);
    }
  }
  ಅನ್ಯಥಾ
  {
    curradvx = hb_clamp_to<hb_position_t> (gr_seg_advance_X(seg) * xscale);
    ಪ್ರತಿ (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      ಯದಿ (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } ಅನ್ಯಥಾ
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) (gr_slot_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    hb_buffer_reverse_clusters (buffer);
  }

  ಯದಿ (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


#endif
