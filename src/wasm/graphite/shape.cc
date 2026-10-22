#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#અન્તર્ભાવ <hb-wasm-api.h>

#અન્તર્ભાવ <graphite2/Segment.h>

#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

સ્થિર int32_t
clamp_position (દ્વિગુણ value)
{
  જો (value <= INT32_MIN) પાછા INT32_MIN;
  જો (value >= INT32_MAX) પાછા INT32_MAX;
  પાછા (int32_t) value;
}

સ્થિર int32_t
saturate_add (int32_t a, int32_t b)
{ પાછા clamp_position ((int64_t) a + b); }

સ્થિર int32_t
saturate_sub (int32_t a, int32_t b)
{ પાછા clamp_position ((int64_t) a - b); }

શૂન્ય debugprint1 (અક્ષર *s, int32_t);
શૂન્ય debugprint2 (અક્ષર *s, int32_t, int32_t);

સ્થિર અચલ શૂન્ય *copy_table (અચલ શૂન્ય *data, unsigned પૂર્ણાંક tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  જો (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  પાછા blob.data;
}

સ્થિર શૂન્ય free_table (અચલ શૂન્ય *data, અચલ શૂન્ય *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (અક્ષર *) table_data;
  blob_free (&blob);
}

શૂન્ય *
shape_plan_create (face_t *face)
{
  અચલ gr_face_ops ops = {કદ (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  પાછા grface;
}

શૂન્ય
shape_plan_destroy (શૂન્ય *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (શૂન્ય *shape_plan,
       font_t *font,
       buffer_t *buffer,
       અચલ feature_t *features,
       uint32_t અંક_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  જો ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  જો (!buffer_copy_contents (buffer, &contents))
    પાછા ખોટું;

  gr_segment *seg = nullptr;
  અચલ gr_slot *is;
  unsigned પૂર્ણાંક ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * કદ (uint32_t));
  જો (!chars)
    પાછા ખોટું;
  માટે (unsigned પૂર્ણાંક i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  મુક્ત (chars);

  જો (!seg)
    પાછા ખોટું;

  unsigned પૂર્ણાંક glyph_ગણતરી = gr_seg_n_slots (seg);

  સંરચના cluster_t {
    unsigned પૂર્ણાંક base_char;
    unsigned પૂર્ણાંક અંક_chars;
    unsigned પૂર્ણાંક base_glyph;
    unsigned પૂર્ણાંક અંક_glyphs;
    unsigned પૂર્ણાંક cluster;
    int32_t advance;
  };

  length = glyph_ગણતરી;
  જો (!buffer_contents_realloc (&contents, length))
    પાછા ખોટું;
  cluster_t *clusters = (cluster_t *) malloc (length * કદ (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * કદ (uint32_t));
  જો (!clusters || !gids)
  {
    મુક્ત (clusters);
    મુક્ત (gids);
    પાછા ખોટું;
  }

  memset (clusters, 0, કદ (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned પૂર્ણાંક upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  દશાંશ xscale = (દશાંશ) font_x_scale / upem;
  દશાંશ yscale = (દશાંશ) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  જો (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((દ્વિગુણ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  નહીંતર
    clusters[0].advance = 0;
  માટે (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned પૂર્ણાંક before = gr_slot_before (is);
    unsigned પૂર્ણાંક after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    જ્યારે (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].અંક_chars += clusters[ci].અંક_chars;
      clusters[ci-1].અંક_glyphs += clusters[ci].અંક_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    જો (gr_slot_can_ઉમેરો_before (is) && clusters[ci].અંક_chars && before >= clusters[ci].base_char + clusters[ci].અંક_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].અંક_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->અંક_chars = before - c->base_char;
      c->base_glyph = ic;
      c->અંક_glyphs = 0;
      જો (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((દ્વિગુણ) curradv - (દ્વિગુણ) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      નહીંતર
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].અંક_glyphs++;

    જો (clusters[ci].base_char + clusters[ci].અંક_chars < after + 1)
	clusters[ci].અંક_chars = after + 1 - clusters[ci].base_char;
  }

  જો (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  નહીંતર
    clusters[ci].advance = clamp_position ((દ્વિગુણ) clusters[ci].advance +
					   (દ્વિગુણ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  માટે (unsigned પૂર્ણાંક i = 0; i < ci; ++i)
  {
    માટે (unsigned પૂર્ણાંક j = 0; j < clusters[i].અંક_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_ગણતરી;

  /* Positioning. */
  unsigned પૂર્ણાંક currclus = 0xFFFFFFFF;
  અચલ glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  જો (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    માટે (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((દ્વિગુણ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((દ્વિગુણ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      જો (info->cluster != currclus) {
	pPos->x_advance = (પૂર્ણાંક) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } નહીંતર
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  નહીંતર
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    માટે (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      જો (info->cluster != currclus)
      {
	pPos->x_advance = (પૂર્ણાંક) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } નહીંતર
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((દ્વિગુણ) (gr_slot_origin_X (is) * xscale) -
					      (પૂર્ણાંક) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((દ્વિગુણ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  મુક્ત (clusters);
  મુક્ત (gids);

  તાર્કિક ret = glyph_ગણતરી;

  પાછા ret;
}
