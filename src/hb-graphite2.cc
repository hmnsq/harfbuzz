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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_GRAPHITE2

#अन्तर्भाव "hb-shaper-impl.hh"

#अन्तर्भाव "hb-graphite2.h"

#अन्तर्भाव <graphite2/Segment.h>

#अन्तर्भाव "hb-ot-layout.h"


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

प्रकारनिर्देश संरचना hb_graphite2_tablelist_t
{
  संरचना hb_graphite2_tablelist_t *next;
  hb_blob_t *blob;
  अचिन्हित पूर्णाङ्क tag;
} hb_graphite2_tablelist_t;

संरचना hb_graphite2_face_data_t
{
  hb_face_t *face;
  gr_face   *grface;
  hb_atomic_t<hb_graphite2_tablelist_t *> tlist;
};

स्थिर अचर शून्य *hb_graphite2_get_table (अचर शून्य *data, अचिन्हित पूर्णाङ्क tag, माप_प्रकार *len)
{
  hb_graphite2_face_data_t *face_data = (hb_graphite2_face_data_t *) data;
  hb_graphite2_tablelist_t *tlist = face_data->tlist;

  hb_blob_t *blob = nullptr;

  लागि (hb_graphite2_tablelist_t *p = tlist; p; p = p->next)
    यदि (p->tag == tag) {
      blob = p->blob;
      रोक;
    }

  यदि (unlikely (!blob))
  {
    blob = face_data->face->reference_table (tag);

    hb_graphite2_tablelist_t *p = (hb_graphite2_tablelist_t *) hb_calloc (1, परिमाण (hb_graphite2_tablelist_t));
    यदि (unlikely (!p)) {
      hb_blob_destroy (blob);
      फर्काऊ nullptr;
    }
    p->blob = blob;
    p->tag = tag;

retry:
    hb_graphite2_tablelist_t *tlist = face_data->tlist;
    p->next = tlist;

    यदि (unlikely (!face_data->tlist.cmpexch (tlist, p)))
      गच्छ retry;
  }

  अचिन्हित पूर्णाङ्क tlen;
  अचर वर्ण *d = hb_blob_get_data (blob, &tlen);
  *len = tlen;
  फर्काऊ d;
}

hb_graphite2_face_data_t *
_hb_graphite2_shaper_face_data_create (hb_face_t *face)
{
  hb_unique_ptr_t<hb_blob_t> silf_blob (face->reference_table (HB_GRAPHITE2_TAG_SILF));
  /* Umm, we just reference the table to check whether it exists.
   * Maybe add better API for this? */
  यदि (!hb_blob_get_length (silf_blob))
    फर्काऊ nullptr;

  hb_graphite2_face_data_t *data = (hb_graphite2_face_data_t *) hb_calloc (1, परिमाण (hb_graphite2_face_data_t));
  यदि (unlikely (!data))
    फर्काऊ nullptr;

  data->face = face;
  अचर gr_face_ops ops = {परिमाण(gr_face_ops), &hb_graphite2_get_table, रिक्त};
  data->grface = gr_make_face_with_ops (data, &ops, gr_face_preloadAll);

  यदि (unlikely (!data->grface)) {
    /* data->tlist has already been populated by the callback,
     * so everything needs to be destroyed */
    _hb_graphite2_shaper_face_data_destroy (data);
    फर्काऊ nullptr;
  }

  फर्काऊ data;
}

शून्य
_hb_graphite2_shaper_face_data_destroy (hb_graphite2_face_data_t *data)
{
  hb_graphite2_tablelist_t *tlist = data->tlist;

  जबसम्म (tlist)
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
  अचर hb_graphite2_face_data_t *data = face->data.graphite2;
  फर्काऊ data ? data->grface : nullptr;
}


/*
 * shaper font data
 */

संरचना hb_graphite2_font_data_t {};

hb_graphite2_font_data_t *
_hb_graphite2_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  फर्काऊ (hb_graphite2_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

शून्य
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
  फर्काऊ nullptr;
}
#endif


/*
 * shaper
 */

संरचना hb_graphite2_cluster_t {
  अचिन्हित पूर्णाङ्क base_char;
  अचिन्हित पूर्णाङ्क अङ्क_chars;
  अचिन्हित पूर्णाङ्क base_glyph;
  अचिन्हित पूर्णाङ्क अङ्क_glyphs;
  अचिन्हित पूर्णाङ्क cluster;
  hb_position_t advance;
};

संरचना hb_graphite2_glyph_t {
  hb_codepoint_t codepoint;
  अचिन्हित पूर्णाङ्क cluster;
};

hb_bool_t
_hb_graphite2_shape (hb_shape_plan_t    *shape_plan HB_UNUSED,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     अचर hb_feature_t *features,
		     अचिन्हित पूर्णाङ्क        अङ्क_features)
{
  hb_face_t *face = font->face;
  gr_face *grface = face->data.graphite2->grface;

  अचर वर्ण *lang = hb_language_to_string (hb_buffer_get_language (buffer));
  अचर वर्ण *lang_end = lang ? म_अक्षर (lang, '-') : nullptr;
  पूर्णाङ्क lang_len = lang_end ? lang_end - lang : -1;
  gr_feature_val *feats = gr_face_featureval_for_lang (grface, lang ? hb_tag_from_string (lang, lang_len) : 0);

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_features; i++)
  {
    अचर gr_feature_ref *fref = gr_face_find_fref (grface, features[i].tag);
    यदि (fref)
      gr_fref_set_feature_मान (fref, features[i].value, feats);
  }

  hb_direction_t direction = buffer->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (buffer->props.script);
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  यदि ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != HB_DIRECTION_INVALID) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_buffer_reverse_clusters (buffer);
    direction = HB_DIRECTION_REVERSE (direction);
  }

  gr_segment *seg = nullptr;
  अचर gr_slot *is;
  अचिन्हित पूर्णाङ्क ci = 0, ic = 0;
  hb_position_t curradvx = 0, curradvy = 0;

  अचिन्हित पूर्णाङ्क scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

  uint32_t *chars = (uint32_t *) scratch;

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < buffer->len; ++i)
    chars[i] = buffer->info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     HB_TAG_NONE, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     feats,
		     gr_utf32, chars, buffer->len,
		     2 | (direction == HB_DIRECTION_RTL ? 1 : 0));

  यदि (unlikely (!seg)) {
    यदि (feats) gr_featureval_destroy (feats);
    फर्काऊ असत्य;
  }

  अचिन्हित पूर्णाङ्क glyph_गन्ती = gr_seg_n_slots (seg);
  यदि (unlikely (!glyph_गन्ती)) {
    यदि (feats) gr_featureval_destroy (feats);
    gr_seg_destroy (seg);
    buffer->len = 0;
    फर्काऊ सत्य;
  }

  (शून्य) buffer->ensure (glyph_गन्ती);
  scratch = buffer->get_scratch_buffer (&scratch_size);
  जबसम्म ((DIV_CEIL (परिमाण (hb_graphite2_cluster_t) * buffer->len, परिमाण (*scratch)) +
	  DIV_CEIL (परिमाण (hb_graphite2_glyph_t) * glyph_गन्ती, परिमाण (*scratch))) > scratch_size)
  {
    यदि (unlikely (!buffer->ensure (buffer->allocated * 2)))
    {
      यदि (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      फर्काऊ असत्य;
    }
    scratch = buffer->get_scratch_buffer (&scratch_size);
  }

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  गर { \
    अचिन्हित पूर्णाङ्क _consumed = DIV_CEIL ((len) * परिमाण (Type), परिमाण (*scratch)); \
    प्रतिज्ञा (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } जबसम्म (0)

  ALLOCATE_ARRAY (hb_graphite2_cluster_t, clusters, buffer->len);
  ALLOCATE_ARRAY (hb_graphite2_glyph_t, glyphs, glyph_गन्ती);

#undef ALLOCATE_ARRAY

  hb_memset (clusters, 0, परिमाण (clusters[0]) * buffer->len);

  hb_graphite2_glyph_t *pg = glyphs;
  clusters[0].cluster = buffer->info[0].cluster;
  अचिन्हित पूर्णाङ्क upem = hb_face_get_upem (face);
  भिन्न xscale = (भिन्न) font->x_scale / upem;
  भिन्न yscale = (भिन्न) font->y_scale / upem;
  yscale *= yscale / xscale;
  hb_position_t curradv = 0;
  यदि (HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradv = hb_clamp_to<hb_position_t> (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = hb_clamp_to<hb_position_t> ((द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  अन्यथा
    clusters[0].advance = 0;
  लागि (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    पूर्णाङ्क slot_before = gr_slot_before (is);
    पूर्णाङ्क slot_after = gr_slot_after (is);
    यदि (unlikely (slot_before < 0 ||
		  slot_after < slot_before ||
		  (अचिन्हित पूर्णाङ्क) slot_after >= buffer->len))
    {
      यदि (feats) gr_featureval_destroy (feats);
      gr_seg_destroy (seg);
      फर्काऊ असत्य;
    }
    अचिन्हित पूर्णाङ्क before = (अचिन्हित पूर्णाङ्क) slot_before;
    अचिन्हित पूर्णाङ्क after = (अचिन्हित पूर्णाङ्क) slot_after;
    pg->codepoint = gr_slot_gid (is);
    pg->cluster = hb_min (buffer->info[before].cluster,
			  buffer->info[after].cluster);
    pg++;
    जबसम्म (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अङ्क_chars += clusters[ci].अङ्क_chars;
      clusters[ci-1].अङ्क_glyphs += clusters[ci].अङ्क_glyphs;
      clusters[ci-1].advance = hb_saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    यदि (gr_slot_can_घुसाउ_before (is) && clusters[ci].अङ्क_chars && before >= clusters[ci].base_char + clusters[ci].अङ्क_chars)
    {
      hb_graphite2_cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अङ्क_chars;
      c->cluster = buffer->info[c->base_char].cluster;
      c->अङ्क_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अङ्क_glyphs = 0;
      यदि (HB_DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = hb_clamp_to<hb_position_t> ((द्विगुण) curradv -
						   (द्विगुण) (gr_slot_origin_X(is) * xscale));
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
    clusters[ci].अङ्क_glyphs++;

    यदि (clusters[ci].base_char + clusters[ci].अङ्क_chars < after + 1)
	clusters[ci].अङ्क_chars = after + 1 - clusters[ci].base_char;
  }

  यदि (HB_DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = hb_saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = hb_clamp_to<hb_position_t> ((द्विगुण) clusters[ci].advance +
							       (द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < ci; ++i)
  {
    लागि (अचिन्हित पूर्णाङ्क j = 0; j < clusters[i].अङ्क_glyphs; ++j)
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
  buffer->len = glyph_गन्ती;

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < ci; ++i)
    buffer->merge_clusters (clusters[i].base_glyph,
			    clusters[i].base_glyph + clusters[i].अङ्क_glyphs);

  /* Positioning. */
  अचिन्हित पूर्णाङ्क curr_gr_cluster = अच_पूर्णांक_उच्च;
  अचर hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pPos = hb_buffer_get_glyph_positions (buffer, nullptr);
  यदि (!HB_DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    लागि (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = hb_clamp_to<hb_position_t> ((द्विगुण) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
      यदि (info->var2.u32 != curr_gr_cluster) {
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
    लागि (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      यदि (info->var2.u32 != curr_gr_cluster)
      {
	pPos->x_advance = info->var1.i32;
	curradvx = hb_saturate_sub (curradvx, pPos->x_advance);
	curr_gr_cluster = info->var2.u32;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = hb_clamp_to<hb_position_t> (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = hb_saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = hb_clamp_to<hb_position_t> ((द्विगुण) (gr_slot_origin_X (is) * xscale) -
							  info->var1.i32 - curradvx + pPos->x_advance);
      pPos->y_offset = hb_clamp_to<hb_position_t> ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    hb_buffer_reverse_clusters (buffer);
  }

  यदि (feats) gr_featureval_destroy (feats);
  gr_seg_destroy (seg);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  फर्काऊ सत्य;
}


#endif
