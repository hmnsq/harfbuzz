#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#अन्तर्भाव <hb-wasm-api.h>

#अन्तर्भाव <graphite2/Segment.h>

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

स्थिर int32_t
clamp_position (द्विगुण value)
{
  यदि (value <= INT32_MIN) प्रत्यागम INT32_MIN;
  यदि (value >= INT32_MAX) प्रत्यागम INT32_MAX;
  प्रत्यागम (int32_t) value;
}

स्थिर int32_t
saturate_add (int32_t a, int32_t b)
{ प्रत्यागम clamp_position ((int64_t) a + b); }

स्थिर int32_t
saturate_sub (int32_t a, int32_t b)
{ प्रत्यागम clamp_position ((int64_t) a - b); }

शून्य debugprint1 (वर्ण *s, int32_t);
शून्य debugprint2 (वर्ण *s, int32_t, int32_t);

स्थिर ध्रुव शून्य *copy_table (ध्रुव शून्य *data, unsigned पूर्णाङ्क tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  यदि (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  प्रत्यागम blob.data;
}

स्थिर शून्य free_table (ध्रुव शून्य *data, ध्रुव शून्य *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (वर्ण *) table_data;
  blob_free (&blob);
}

शून्य *
shape_plan_create (face_t *face)
{
  ध्रुव gr_face_ops ops = {परिमाण (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  प्रत्यागम grface;
}

शून्य
shape_plan_destroy (शून्य *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (शून्य *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ध्रुव feature_t *features,
       uint32_t अंक_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  यदि ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  यदि (!buffer_copy_contents (buffer, &contents))
    प्रत्यागम असत्य;

  gr_segment *seg = nullptr;
  ध्रुव gr_slot *is;
  unsigned पूर्णाङ्क ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * परिमाण (uint32_t));
  यदि (!chars)
    प्रत्यागम असत्य;
  प्रति (unsigned पूर्णाङ्क i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  मुक्त (chars);

  यदि (!seg)
    प्रत्यागम असत्य;

  unsigned पूर्णाङ्क glyph_गिनती = gr_seg_n_slots (seg);

  संरचना cluster_t {
    unsigned पूर्णाङ्क base_char;
    unsigned पूर्णाङ्क अंक_chars;
    unsigned पूर्णाङ्क base_glyph;
    unsigned पूर्णाङ्क अंक_glyphs;
    unsigned पूर्णाङ्क cluster;
    int32_t advance;
  };

  length = glyph_गिनती;
  यदि (!buffer_contents_realloc (&contents, length))
    प्रत्यागम असत्य;
  cluster_t *clusters = (cluster_t *) malloc (length * परिमाण (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * परिमाण (uint32_t));
  यदि (!clusters || !gids)
  {
    मुक्त (clusters);
    मुक्त (gids);
    प्रत्यागम असत्य;
  }

  memset (clusters, 0, परिमाण (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned पूर्णाङ्क upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  भिन्न xscale = (भिन्न) font_x_scale / upem;
  भिन्न yscale = (भिन्न) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  यदि (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  अन्यथा
    clusters[0].advance = 0;
  प्रति (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned पूर्णाङ्क before = gr_slot_before (is);
    unsigned पूर्णाङ्क after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    यावत् (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अंक_chars += clusters[ci].अंक_chars;
      clusters[ci-1].अंक_glyphs += clusters[ci].अंक_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    यदि (gr_slot_can_जोड़ो_before (is) && clusters[ci].अंक_chars && before >= clusters[ci].base_char + clusters[ci].अंक_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अंक_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->अंक_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अंक_glyphs = 0;
      यदि (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((द्विगुण) curradv - (द्विगुण) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      अन्यथा
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].अंक_glyphs++;

    यदि (clusters[ci].base_char + clusters[ci].अंक_chars < after + 1)
	clusters[ci].अंक_chars = after + 1 - clusters[ci].base_char;
  }

  यदि (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = clamp_position ((द्विगुण) clusters[ci].advance +
					   (द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  प्रति (unsigned पूर्णाङ्क i = 0; i < ci; ++i)
  {
    प्रति (unsigned पूर्णाङ्क j = 0; j < clusters[i].अंक_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_गिनती;

  /* Positioning. */
  unsigned पूर्णाङ्क currclus = 0xFFFFFFFF;
  ध्रुव glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  यदि (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    प्रति (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((द्विगुण) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
      यदि (info->cluster != currclus) {
	pPos->x_advance = (पूर्णाङ्क) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  अन्यथा
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    प्रति (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      यदि (info->cluster != currclus)
      {
	pPos->x_advance = (पूर्णाङ्क) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((द्विगुण) (gr_slot_origin_X (is) * xscale) -
					      (पूर्णाङ्क) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  मुक्त (clusters);
  मुक्त (gids);

  तार्किक ret = glyph_गिनती;

  प्रत्यागम ret;
}
