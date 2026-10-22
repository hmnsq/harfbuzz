#घोषणा HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#समावेश <hb-wasm-api.h>

#समावेश <graphite2/Segment.h>

#समावेश <stdlib.h>
#समावेश <string.h>

अटल int32_t
clamp_position (द्विगुन value)
{
  अगर (value <= INT32_MIN) वापस INT32_MIN;
  अगर (value >= INT32_MAX) वापस INT32_MAX;
  वापस (int32_t) value;
}

अटल int32_t
saturate_add (int32_t a, int32_t b)
{ वापस clamp_position ((int64_t) a + b); }

अटल int32_t
saturate_sub (int32_t a, int32_t b)
{ वापस clamp_position ((int64_t) a - b); }

व्योम debugprint1 (अक्षर *s, int32_t);
व्योम debugprint2 (अक्षर *s, int32_t, int32_t);

अटल स्थिर व्योम *copy_table (स्थिर व्योम *तथ्य, अचिन्हित पूर्णांक tag, माप_प्रकार *len)
{
  face_t *face = (face_t *) तथ्य;
  blob_t blob = BLOB_INIT;
  अगर (!face_copy_table (face, tag, &blob))
    पात ();

  *len = blob.लंबाई;
  वापस blob.तथ्य;
}

अटल व्योम free_table (स्थिर व्योम *तथ्य, स्थिर व्योम *table_तथ्य)
{
  blob_t blob;
  blob.लंबाई = 0; // Doesn't matter
  blob.तथ्य = (अक्षर *) table_तथ्य;
  blob_free (&blob);
}

व्योम *
shape_plan_create (face_t *face)
{
  स्थिर gr_face_ops ops = {माप (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  वापस grface;
}

व्योम
shape_plan_destroy (व्योम *तथ्य)
{
  gr_face_destroy ((gr_face *) तथ्य);
}

bool_t
shape (व्योम *shape_plan,
       font_t *font,
       बफर_t *बफर,
       स्थिर feature_t *features,
       uint32_t अंक_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = बफर_get_direction (बफर);
  direction_t horiz_dir = script_get_horizontal_direction (बफर_get_script (बफर));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  अगर ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    बफर_reverse_clusters (बफर);
    direction = DIRECTION_REVERSE (direction);
  }

  बफर_contents_t contents = बफर_CONTENTS_INIT;
  अगर (!बफर_copy_contents (बफर, &contents))
    वापस false;

  gr_segment *seg = nullptr;
  स्थिर gr_slot *is;
  अचिन्हित पूर्णांक ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  अचिन्हित लंबाई = contents.लंबाई;

  uint32_t *chars = (uint32_t *) दो_स्मृति (लंबाई * माप (uint32_t));
  अगर (!chars)
    वापस false;
  क्रम (अचिन्हित पूर्णांक i = 0; i < contents.लंबाई; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.लंबाई,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  मुक्त (chars);

  अगर (!seg)
    वापस false;

  अचिन्हित पूर्णांक glyph_गिनती = gr_seg_n_slots (seg);

  काष्ठा cluster_t {
    अचिन्हित पूर्णांक base_char;
    अचिन्हित पूर्णांक अंक_chars;
    अचिन्हित पूर्णांक base_glyph;
    अचिन्हित पूर्णांक अंक_glyphs;
    अचिन्हित पूर्णांक cluster;
    int32_t advance;
  };

  लंबाई = glyph_गिनती;
  अगर (!बफर_contents_realloc (&contents, लंबाई))
    वापस false;
  cluster_t *clusters = (cluster_t *) दो_स्मृति (लंबाई * माप (cluster_t));
  uint32_t *gids = (uint32_t *) दो_स्मृति (लंबाई * माप (uint32_t));
  अगर (!clusters || !gids)
  {
    मुक्त (clusters);
    मुक्त (gids);
    वापस false;
  }

  स_रखो (clusters, 0, माप (clusters[0]) * लंबाई);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  अचिन्हित पूर्णांक upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  भग्न xscale = (भग्न) font_x_scale / upem;
  भग्न yscale = (भग्न) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  अगर (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((द्विगुन) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  अन्यथा
    clusters[0].advance = 0;
  क्रम (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_अग्र_in_segment (is), ic++)
  {
    अचिन्हित पूर्णांक before = gr_slot_before (is);
    अचिन्हित पूर्णांक after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    जबतक (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].अंक_chars += clusters[ci].अंक_chars;
      clusters[ci-1].अंक_glyphs += clusters[ci].अंक_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    अगर (gr_slot_can_जोड़ो_before (is) && clusters[ci].अंक_chars && before >= clusters[ci].base_char + clusters[ci].अंक_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].अंक_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->अंक_chars = before - c->base_char;
      c->base_glyph = ic;
      c->अंक_glyphs = 0;
      अगर (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((द्विगुन) curradv - (द्विगुन) (gr_slot_origin_X(is) * xscale));
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

    अगर (clusters[ci].base_char + clusters[ci].अंक_chars < after + 1)
	clusters[ci].अंक_chars = after + 1 - clusters[ci].base_char;
  }

  अगर (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  अन्यथा
    clusters[ci].advance = clamp_position ((द्विगुन) clusters[ci].advance +
					   (द्विगुन) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  क्रम (अचिन्हित पूर्णांक i = 0; i < ci; ++i)
  {
    क्रम (अचिन्हित पूर्णांक j = 0; j < clusters[i].अंक_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (अचिन्हित) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.लंबाई = glyph_गिनती;

  /* Positioning. */
  अचिन्हित पूर्णांक currclus = 0xFFFFFFFF;
  स्थिर glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  अगर (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    क्रम (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_अग्र_in_segment (is))
    {
      pPos->x_offset = clamp_position ((द्विगुन) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((द्विगुन) (gr_slot_origin_Y (is) * yscale) - curradvy);
      अगर (info->cluster != currclus) {
	pPos->x_advance = (पूर्णांक) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    बफर_set_contents (बफर, &contents);
  }
  अन्यथा
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    क्रम (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_अग्र_in_segment (is))
    {
      अगर (info->cluster != currclus)
      {
	pPos->x_advance = (पूर्णांक) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } अन्यथा
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((द्विगुन) (gr_slot_origin_X (is) * xscale) -
					      (पूर्णांक) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((द्विगुन) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    बफर_set_contents (बफर, &contents);
    बफर_reverse_clusters (बफर);
  }

  gr_seg_destroy (seg);
  मुक्त (clusters);
  मुक्त (gids);

  bool ret = glyph_गिनती;

  वापस ret;
}
