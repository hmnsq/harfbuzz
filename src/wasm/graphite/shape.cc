#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#अन्तर्भाव <hb-wasm-api.h>

#अन्तर्भाव <graphite2/Segment.h>

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

स्थिर int32_t
clamp_position (द्विगुण value)
{
  यदि (value <= INT32_MIN) फर्काऊ INT32_MIN;
  यदि (value >= INT32_MAX) फर्काऊ INT32_MAX;
  फर्काऊ (int32_t) value;
}

स्थिर int32_t
saturate_add (int32_t a, int32_t b)
{ फर्काऊ clamp_position ((int64_t) a + b); }

स्थिर int32_t
saturate_sub (int32_t a, int32_t b)
{ फर्काऊ clamp_position ((int64_t) a - b); }

शून्य debugprint1 (वर्ण *s, int32_t);
शून्य debugprint2 (वर्ण *s, int32_t, int32_t);

स्थिर अचर शून्य *copy_table (अचर शून्य *data, अचिन्हित पूर्णाङ्क tag, माप_प्रकार *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  यदि (!face_copy_table (face, tag, &blob))
    पात ();

  *len = blob.length;
  फर्काऊ blob.data;
}

स्थिर शून्य free_table (अचर शून्य *data, अचर शून्य *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (वर्ण *) table_data;
  blob_free (&blob);
}

शून्य *
shape_plan_create (face_t *face)
{
  अचर gr_face_ops ops = {परिमाण (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  फर्काऊ grface;
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
       अचर feature_t *features,
       uint32_t अङ्क_features)
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
    फर्काऊ असत्य;

  gr_segment *seg = nullptr;
  अचर gr_slot *is;
  अचिन्हित पूर्णाङ्क ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  अचिन्हित length = contents.length;

  uint32_t *chars = (uint32_t *) दो_स्मृति (length * परिमाण (uint32_t));
  यदि (!chars)
    फर्काऊ असत्य;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  मुक्त (chars);

  यदि (!seg)
    फर्काऊ असत्य;

  अचिन्हित पूर्णाङ्क glyph_गन्ती = gr_seg_n_slots (seg);

  संरचना cluster_t {
    अचिन्हित पूर्णाङ्क base_char;
    अचिन्हित पूर्णाङ्क अङ्क_chars;
    अचिन्हित पूर्णाङ्क base_glyph;
    अचिन्हित पूर्णाङ्क अङ्क_glyphs;
    अचिन्हित पूर्णाङ्क cluster;
    int32_t advance;
  };

  length = glyph_गन्ती;
  यदि (!buffer_contents_realloc (&contents, length))
    फर्काऊ असत्य;
  cluster_t *clusters = (cluster_t *) दो_स्मृति (length * परिमाण (cluster_t));
  uint32_t *gids = (uint32_t *) दो_स्मृति (length * परिमाण (uint32_t));
  यदि (!clusters || !gids)
  {
    मुक्त (clusters);
    मुक्त (gids);
    फर्काऊ असत्य;
  }

  स_रखो (clusters, 0, परिमाण (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  अचिन्हित पूर्णाङ्क upem = face_get_upem (face);
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
  लागि (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    अचिन्हित पूर्णाङ्क before = gr_slot_before (is);
    अचिन्हित पूर्णाङ्क after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    जबसम्म (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अङ्क_chars += clusters[ci].अङ्क_chars;
      clusters[ci-1].अङ्क_glyphs += clusters[ci].अङ्क_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    यदि (gr_slot_can_घुसाउ_before (is) && clusters[ci].अङ्क_chars && before >= clusters[ci].base_char + clusters[ci].अङ्क_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अङ्क_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->अङ्क_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अङ्क_glyphs = 0;
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
    clusters[ci].अङ्क_glyphs++;

    यदि (clusters[ci].base_char + clusters[ci].अङ्क_chars < after + 1)
	clusters[ci].अङ्क_chars = after + 1 - clusters[ci].base_char;
  }

  यदि (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = clamp_position ((द्विगुण) clusters[ci].advance +
					   (द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < ci; ++i)
  {
    लागि (अचिन्हित पूर्णाङ्क j = 0; j < clusters[i].अङ्क_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (अचिन्हित) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_गन्ती;

  /* Positioning. */
  अचिन्हित पूर्णाङ्क currclus = 0xFFFFFFFF;
  अचर glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  यदि (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    लागि (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
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
    लागि (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
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

  तार्किक ret = glyph_गन्ती;

  फर्काऊ ret;
}
