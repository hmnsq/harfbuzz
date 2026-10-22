#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#अन्तर्भाव <hb-wasm-api.h>

#अन्तर्भाव <graphite2/Segment.h>

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

स्थिर int32_t
clamp_position (द्विगुण value)
{
  जर (value <= INT32_MIN) परत INT32_MIN;
  जर (value >= INT32_MAX) परत INT32_MAX;
  परत (int32_t) value;
}

स्थिर int32_t
saturate_add (int32_t a, int32_t b)
{ परत clamp_position ((int64_t) a + b); }

स्थिर int32_t
saturate_sub (int32_t a, int32_t b)
{ परत clamp_position ((int64_t) a - b); }

शून्य debugprint1 (अक्षर *s, int32_t);
शून्य debugprint2 (अक्षर *s, int32_t, int32_t);

स्थिर स्थिरांक शून्य *copy_table (स्थिरांक शून्य *data, अचिन्हित पूर्णांक tag, माप_प्रकार *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  जर (!face_copy_table (face, tag, &blob))
    पात ();

  *len = blob.length;
  परत blob.data;
}

स्थिर शून्य free_table (स्थिरांक शून्य *data, स्थिरांक शून्य *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (अक्षर *) table_data;
  blob_free (&blob);
}

शून्य *
shape_plan_create (face_t *face)
{
  स्थिरांक gr_face_ops ops = {आकार (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  परत grface;
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
       स्थिरांक feature_t *features,
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
  जर ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  जर (!buffer_copy_contents (buffer, &contents))
    परत असत्य;

  gr_segment *seg = nullptr;
  स्थिरांक gr_slot *is;
  अचिन्हित पूर्णांक ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  अचिन्हित length = contents.length;

  uint32_t *chars = (uint32_t *) दो_स्मृति (length * आकार (uint32_t));
  जर (!chars)
    परत असत्य;
  साठी (अचिन्हित पूर्णांक i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  मुक्त (chars);

  जर (!seg)
    परत असत्य;

  अचिन्हित पूर्णांक glyph_मोजणी = gr_seg_n_slots (seg);

  संरचना cluster_t {
    अचिन्हित पूर्णांक base_char;
    अचिन्हित पूर्णांक अंक_chars;
    अचिन्हित पूर्णांक base_glyph;
    अचिन्हित पूर्णांक अंक_glyphs;
    अचिन्हित पूर्णांक cluster;
    int32_t advance;
  };

  length = glyph_मोजणी;
  जर (!buffer_contents_realloc (&contents, length))
    परत असत्य;
  cluster_t *clusters = (cluster_t *) दो_स्मृति (length * आकार (cluster_t));
  uint32_t *gids = (uint32_t *) दो_स्मृति (length * आकार (uint32_t));
  जर (!clusters || !gids)
  {
    मुक्त (clusters);
    मुक्त (gids);
    परत असत्य;
  }

  स_रखो (clusters, 0, आकार (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  अचिन्हित पूर्णांक upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  दशांश xscale = (दशांश) font_x_scale / upem;
  दशांश yscale = (दशांश) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  जर (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  अन्यथा
    clusters[0].advance = 0;
  साठी (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    अचिन्हित पूर्णांक before = gr_slot_before (is);
    अचिन्हित पूर्णांक after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    पर्यंत (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अंक_chars += clusters[ci].अंक_chars;
      clusters[ci-1].अंक_glyphs += clusters[ci].अंक_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    जर (gr_slot_can_घाला_before (is) && clusters[ci].अंक_chars && before >= clusters[ci].base_char + clusters[ci].अंक_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अंक_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->अंक_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अंक_glyphs = 0;
      जर (DIRECTION_IS_BACKWARD (direction))
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

    जर (clusters[ci].base_char + clusters[ci].अंक_chars < after + 1)
	clusters[ci].अंक_chars = after + 1 - clusters[ci].base_char;
  }

  जर (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = clamp_position ((द्विगुण) clusters[ci].advance +
					   (द्विगुण) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  साठी (अचिन्हित पूर्णांक i = 0; i < ci; ++i)
  {
    साठी (अचिन्हित पूर्णांक j = 0; j < clusters[i].अंक_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (अचिन्हित) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_मोजणी;

  /* Positioning. */
  अचिन्हित पूर्णांक currclus = 0xFFFFFFFF;
  स्थिरांक glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  जर (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    साठी (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((द्विगुण) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
      जर (info->cluster != currclus) {
	pPos->x_advance = (पूर्णांक) info->var1;
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
    साठी (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      जर (info->cluster != currclus)
      {
	pPos->x_advance = (पूर्णांक) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((द्विगुण) (gr_slot_origin_X (is) * xscale) -
					      (पूर्णांक) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((द्विगुण) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  मुक्त (clusters);
  मुक्त (gids);

  तार्किक ret = glyph_मोजणी;

  परत ret;
}
