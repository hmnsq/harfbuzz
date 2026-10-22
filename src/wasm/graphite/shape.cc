#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ಅನ್ತರ್ಭಾವ <hb-wasm-api.h>

#ಅನ್ತರ್ಭಾವ <graphite2/Segment.h>

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ಸ್ಥಿರ int32_t
clamp_position (ದ್ವಿಗುಣ value)
{
  ಯದಿ (value <= INT32_MIN) ಪ್ರತ್ಯಾಗಮ INT32_MIN;
  ಯದಿ (value >= INT32_MAX) ಪ್ರತ್ಯಾಗಮ INT32_MAX;
  ಪ್ರತ್ಯಾಗಮ (int32_t) value;
}

ಸ್ಥಿರ int32_t
saturate_add (int32_t a, int32_t b)
{ ಪ್ರತ್ಯಾಗಮ clamp_position ((int64_t) a + b); }

ಸ್ಥಿರ int32_t
saturate_sub (int32_t a, int32_t b)
{ ಪ್ರತ್ಯಾಗಮ clamp_position ((int64_t) a - b); }

ಶೂನ್ಯ debugprint1 (ವರ್ಣ *s, int32_t);
ಶೂನ್ಯ debugprint2 (ವರ್ಣ *s, int32_t, int32_t);

ಸ್ಥಿರ ಧ್ರುವ ಶೂನ್ಯ *copy_table (ಧ್ರುವ ಶೂನ್ಯ *data, unsigned ಪೂರ್ಣಾಙ್ಕ tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  ಯದಿ (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  ಪ್ರತ್ಯಾಗಮ blob.data;
}

ಸ್ಥಿರ ಶೂನ್ಯ free_table (ಧ್ರುವ ಶೂನ್ಯ *data, ಧ್ರುವ ಶೂನ್ಯ *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (ವರ್ಣ *) table_data;
  blob_free (&blob);
}

ಶೂನ್ಯ *
shape_plan_create (face_t *face)
{
  ಧ್ರುವ gr_face_ops ops = {ಪರಿಮಾಣ (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ಪ್ರತ್ಯಾಗಮ grface;
}

ಶೂನ್ಯ
shape_plan_destroy (ಶೂನ್ಯ *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (ಶೂನ್ಯ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ಧ್ರುವ feature_t *features,
       uint32_t ಅಂಕೆ_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ಯದಿ ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  ಯದಿ (!buffer_copy_contents (buffer, &contents))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  gr_segment *seg = nullptr;
  ಧ್ರುವ gr_slot *is;
  unsigned ಪೂರ್ಣಾಙ್ಕ ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * ಪರಿಮಾಣ (uint32_t));
  ಯದಿ (!chars)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  ಮುಕ್ತ (chars);

  ಯದಿ (!seg)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned ಪೂರ್ಣಾಙ್ಕ glyph_ಎಣಿಕೆ = gr_seg_n_slots (seg);

  ಸಂರಚನಾ cluster_t {
    unsigned ಪೂರ್ಣಾಙ್ಕ base_char;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_chars;
    unsigned ಪೂರ್ಣಾಙ್ಕ base_glyph;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
    unsigned ಪೂರ್ಣಾಙ್ಕ cluster;
    int32_t advance;
  };

  length = glyph_ಎಣಿಕೆ;
  ಯದಿ (!buffer_contents_realloc (&contents, length))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  cluster_t *clusters = (cluster_t *) malloc (length * ಪರಿಮಾಣ (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * ಪರಿಮಾಣ (uint32_t));
  ಯದಿ (!clusters || !gids)
  {
    ಮುಕ್ತ (clusters);
    ಮುಕ್ತ (gids);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  memset (clusters, 0, ಪರಿಮಾಣ (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned ಪೂರ್ಣಾಙ್ಕ upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  ಭಿನ್ನ xscale = (ಭಿನ್ನ) font_x_scale / upem;
  ಭಿನ್ನ yscale = (ಭಿನ್ನ) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  ಯದಿ (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((ದ್ವಿಗುಣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ಅನ್ಯಥಾ
    clusters[0].advance = 0;
  ಪ್ರತಿ (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ before = gr_slot_before (is);
    unsigned ಪೂರ್ಣಾಙ್ಕ after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    ಯಾವತ್ (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].ಅಂಕೆ_chars += clusters[ci].ಅಂಕೆ_chars;
      clusters[ci-1].ಅಂಕೆ_glyphs += clusters[ci].ಅಂಕೆ_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ಯದಿ (gr_slot_can_ಸೇರಿಸು_before (is) && clusters[ci].ಅಂಕೆ_chars && before >= clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->ಅಂಕೆ_chars = before - c->base_char;
      c->base_glyph = ic;
      c->ಅಂಕೆ_glyphs = 0;
      ಯದಿ (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((ದ್ವಿಗುಣ) curradv - (ದ್ವಿಗುಣ) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      ಅನ್ಯಥಾ
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].ಅಂಕೆ_glyphs++;

    ಯದಿ (clusters[ci].base_char + clusters[ci].ಅಂಕೆ_chars < after + 1)
	clusters[ci].ಅಂಕೆ_chars = after + 1 - clusters[ci].base_char;
  }

  ಯದಿ (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  ಅನ್ಯಥಾ
    clusters[ci].advance = clamp_position ((ದ್ವಿಗುಣ) clusters[ci].advance +
					   (ದ್ವಿಗುಣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ci; ++i)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < clusters[i].ಅಂಕೆ_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_ಎಣಿಕೆ;

  /* Positioning. */
  unsigned ಪೂರ್ಣಾಙ್ಕ currclus = 0xFFFFFFFF;
  ಧ್ರುವ glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  ಯದಿ (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    ಪ್ರತಿ (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((ದ್ವಿಗುಣ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((ದ್ವಿಗುಣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      ಯದಿ (info->cluster != currclus) {
	pPos->x_advance = (ಪೂರ್ಣಾಙ್ಕ) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ಅನ್ಯಥಾ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  ಅನ್ಯಥಾ
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    ಪ್ರತಿ (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      ಯದಿ (info->cluster != currclus)
      {
	pPos->x_advance = (ಪೂರ್ಣಾಙ್ಕ) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ಅನ್ಯಥಾ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((ದ್ವಿಗುಣ) (gr_slot_origin_X (is) * xscale) -
					      (ಪೂರ್ಣಾಙ್ಕ) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((ದ್ವಿಗುಣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  ಮುಕ್ತ (clusters);
  ಮುಕ್ತ (gids);

  ತಾರ್ಕಿಕ ret = glyph_ಎಣಿಕೆ;

  ಪ್ರತ್ಯಾಗಮ ret;
}
