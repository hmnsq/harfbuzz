#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ଅନ୍ତର୍ଭାଵ <hb-wasm-api.h>

#ଅନ୍ତର୍ଭାଵ <graphite2/Segment.h>

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

ସ୍ଥିର int32_t
clamp_position (ଦ୍ୱିଗୁଣ value)
{
  ଯଦି (value <= INT32_MIN) ଫେରନ୍ତୁ INT32_MIN;
  ଯଦି (value >= INT32_MAX) ଫେରନ୍ତୁ INT32_MAX;
  ଫେରନ୍ତୁ (int32_t) value;
}

ସ୍ଥିର int32_t
saturate_add (int32_t a, int32_t b)
{ ଫେରନ୍ତୁ clamp_position ((int64_t) a + b); }

ସ୍ଥିର int32_t
saturate_sub (int32_t a, int32_t b)
{ ଫେରନ୍ତୁ clamp_position ((int64_t) a - b); }

ଶୂନ୍ୟ debugprint1 (ଅକ୍ଷର *s, int32_t);
ଶୂନ୍ୟ debugprint2 (ଅକ୍ଷର *s, int32_t, int32_t);

ସ୍ଥିର ଅଚଳ ଶୂନ୍ୟ *copy_table (ଅଚଳ ଶୂନ୍ୟ *data, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  ଯଦି (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  ଫେରନ୍ତୁ blob.data;
}

ସ୍ଥିର ଶୂନ୍ୟ free_table (ଅଚଳ ଶୂନ୍ୟ *data, ଅଚଳ ଶୂନ୍ୟ *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (ଅକ୍ଷର *) table_data;
  blob_free (&blob);
}

ଶୂନ୍ୟ *
shape_plan_create (face_t *face)
{
  ଅଚଳ gr_face_ops ops = {ଆକାର (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ଫେରନ୍ତୁ grface;
}

ଶୂନ୍ୟ
shape_plan_destroy (ଶୂନ୍ୟ *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (ଶୂନ୍ୟ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ଅଚଳ feature_t *features,
       uint32_t ଅଙ୍କ_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ଯଦି ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  ଯଦି (!buffer_copy_contents (buffer, &contents))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  gr_segment *seg = nullptr;
  ଅଚଳ gr_slot *is;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * ଆକାର (uint32_t));
  ଯଦି (!chars)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  ମୁକ୍ତ (chars);

  ଯଦି (!seg)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_ଗଣନା = gr_seg_n_slots (seg);

  ସଂରଚନା cluster_t {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_char;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_chars;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_glyph;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster;
    int32_t advance;
  };

  length = glyph_ଗଣନା;
  ଯଦି (!buffer_contents_realloc (&contents, length))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  cluster_t *clusters = (cluster_t *) malloc (length * ଆକାର (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * ଆକାର (uint32_t));
  ଯଦି (!clusters || !gids)
  {
    ମୁକ୍ତ (clusters);
    ମୁକ୍ତ (gids);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  memset (clusters, 0, ଆକାର (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  ଦଶମିକ xscale = (ଦଶମିକ) font_x_scale / upem;
  ଦଶମିକ yscale = (ଦଶମିକ) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  ଯଦି (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((ଦ୍ୱିଗୁଣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ଅନ୍ୟଥା
    clusters[0].advance = 0;
  ପାଇଁ (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା before = gr_slot_before (is);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    ଯେତେବେଳେ (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].ଅଙ୍କ_chars += clusters[ci].ଅଙ୍କ_chars;
      clusters[ci-1].ଅଙ୍କ_glyphs += clusters[ci].ଅଙ୍କ_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ଯଦି (gr_slot_can_ଭର୍ତ୍ତି_before (is) && clusters[ci].ଅଙ୍କ_chars && before >= clusters[ci].base_char + clusters[ci].ଅଙ୍କ_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].ଅଙ୍କ_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->ଅଙ୍କ_chars = before - c->base_char;
      c->base_glyph = ic;
      c->ଅଙ୍କ_glyphs = 0;
      ଯଦି (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((ଦ୍ୱିଗୁଣ) curradv - (ଦ୍ୱିଗୁଣ) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      ଅନ୍ୟଥା
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].ଅଙ୍କ_glyphs++;

    ଯଦି (clusters[ci].base_char + clusters[ci].ଅଙ୍କ_chars < after + 1)
	clusters[ci].ଅଙ୍କ_chars = after + 1 - clusters[ci].base_char;
  }

  ଯଦି (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  ଅନ୍ୟଥା
    clusters[ci].advance = clamp_position ((ଦ୍ୱିଗୁଣ) clusters[ci].advance +
					   (ଦ୍ୱିଗୁଣ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ci; ++i)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < clusters[i].ଅଙ୍କ_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_ଗଣନା;

  /* Positioning. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା currclus = 0xFFFFFFFF;
  ଅଚଳ glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  ଯଦି (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    ପାଇଁ (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((ଦ୍ୱିଗୁଣ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((ଦ୍ୱିଗୁଣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      ଯଦି (info->cluster != currclus) {
	pPos->x_advance = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ଅନ୍ୟଥା
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  ଅନ୍ୟଥା
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    ପାଇଁ (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      ଯଦି (info->cluster != currclus)
      {
	pPos->x_advance = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ଅନ୍ୟଥା
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((ଦ୍ୱିଗୁଣ) (gr_slot_origin_X (is) * xscale) -
					      (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((ଦ୍ୱିଗୁଣ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  ମୁକ୍ତ (clusters);
  ମୁକ୍ତ (gids);

  ତାର୍କିକ ret = glyph_ଗଣନା;

  ଫେରନ୍ତୁ ret;
}
