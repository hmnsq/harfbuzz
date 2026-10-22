#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#அந்தர்பாவ <hb-wasm-api.h>

#அந்தர்பாவ <graphite2/Segment.h>

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

நிலையான int32_t
clamp_position (இரட்டை value)
{
  எனில் (value <= INT32_MIN) திரும்பு INT32_MIN;
  எனில் (value >= INT32_MAX) திரும்பு INT32_MAX;
  திரும்பு (int32_t) value;
}

நிலையான int32_t
saturate_add (int32_t a, int32_t b)
{ திரும்பு clamp_position ((int64_t) a + b); }

நிலையான int32_t
saturate_sub (int32_t a, int32_t b)
{ திரும்பு clamp_position ((int64_t) a - b); }

வெறுமை debugprint1 (எழுத்து *s, int32_t);
வெறுமை debugprint2 (எழுத்து *s, int32_t, int32_t);

நிலையான மாறிலி வெறுமை *copy_table (மாறிலி வெறுமை *data, unsigned முழு tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  எனில் (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  திரும்பு blob.data;
}

நிலையான வெறுமை free_table (மாறிலி வெறுமை *data, மாறிலி வெறுமை *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (எழுத்து *) table_data;
  blob_free (&blob);
}

வெறுமை *
shape_plan_create (face_t *face)
{
  மாறிலி gr_face_ops ops = {அளவு (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  திரும்பு grface;
}

வெறுமை
shape_plan_destroy (வெறுமை *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (வெறுமை *shape_plan,
       font_t *font,
       buffer_t *buffer,
       மாறிலி feature_t *features,
       uint32_t இலக்கம்_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  எனில் ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  எனில் (!buffer_copy_contents (buffer, &contents))
    திரும்பு பொய்;

  gr_segment *seg = nullptr;
  மாறிலி gr_slot *is;
  unsigned முழு ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * அளவு (uint32_t));
  எனில் (!chars)
    திரும்பு பொய்;
  க்கு (unsigned முழு i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  விடுவி (chars);

  எனில் (!seg)
    திரும்பு பொய்;

  unsigned முழு glyph_மொத்தம் = gr_seg_n_slots (seg);

  கட்டமைப்பு cluster_t {
    unsigned முழு base_char;
    unsigned முழு இலக்கம்_chars;
    unsigned முழு base_glyph;
    unsigned முழு இலக்கம்_glyphs;
    unsigned முழு cluster;
    int32_t advance;
  };

  length = glyph_மொத்தம்;
  எனில் (!buffer_contents_realloc (&contents, length))
    திரும்பு பொய்;
  cluster_t *clusters = (cluster_t *) malloc (length * அளவு (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * அளவு (uint32_t));
  எனில் (!clusters || !gids)
  {
    விடுவி (clusters);
    விடுவி (gids);
    திரும்பு பொய்;
  }

  memset (clusters, 0, அளவு (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned முழு upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  தசம xscale = (தசம) font_x_scale / upem;
  தசம yscale = (தசம) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  எனில் (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((இரட்டை) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  இல்லையெனில்
    clusters[0].advance = 0;
  க்கு (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned முழு before = gr_slot_before (is);
    unsigned முழு after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    போது (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].இலக்கம்_chars += clusters[ci].இலக்கம்_chars;
      clusters[ci-1].இலக்கம்_glyphs += clusters[ci].இலக்கம்_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    எனில் (gr_slot_can_செருகு_before (is) && clusters[ci].இலக்கம்_chars && before >= clusters[ci].base_char + clusters[ci].இலக்கம்_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].இலக்கம்_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->இலக்கம்_chars = before - c->base_char;
      c->base_glyph = ic;
      c->இலக்கம்_glyphs = 0;
      எனில் (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((இரட்டை) curradv - (இரட்டை) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      இல்லையெனில்
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].இலக்கம்_glyphs++;

    எனில் (clusters[ci].base_char + clusters[ci].இலக்கம்_chars < after + 1)
	clusters[ci].இலக்கம்_chars = after + 1 - clusters[ci].base_char;
  }

  எனில் (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  இல்லையெனில்
    clusters[ci].advance = clamp_position ((இரட்டை) clusters[ci].advance +
					   (இரட்டை) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  க்கு (unsigned முழு i = 0; i < ci; ++i)
  {
    க்கு (unsigned முழு j = 0; j < clusters[i].இலக்கம்_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_மொத்தம்;

  /* Positioning. */
  unsigned முழு currclus = 0xFFFFFFFF;
  மாறிலி glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  எனில் (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    க்கு (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((இரட்டை) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((இரட்டை) (gr_slot_origin_Y (is) * yscale) - curradvy);
      எனில் (info->cluster != currclus) {
	pPos->x_advance = (முழு) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } இல்லையெனில்
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  இல்லையெனில்
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    க்கு (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      எனில் (info->cluster != currclus)
      {
	pPos->x_advance = (முழு) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } இல்லையெனில்
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((இரட்டை) (gr_slot_origin_X (is) * xscale) -
					      (முழு) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((இரட்டை) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  விடுவி (clusters);
  விடுவி (gids);

  தருக்க ret = glyph_மொத்தம்;

  திரும்பு ret;
}
