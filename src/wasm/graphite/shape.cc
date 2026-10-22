#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাৱ <hb-wasm-api.h>

#অন্তর্ভাৱ <graphite2/Segment.h>

#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

স্থির int32_t
clamp_position (দ্ৱিগুণ value)
{
  যদি (value <= INT32_MIN) ঘূৰাই INT32_MIN;
  যদি (value >= INT32_MAX) ঘূৰাই INT32_MAX;
  ঘূৰাই (int32_t) value;
}

স্থির int32_t
saturate_add (int32_t a, int32_t b)
{ ঘূৰাই clamp_position ((int64_t) a + b); }

স্থির int32_t
saturate_sub (int32_t a, int32_t b)
{ ঘূৰাই clamp_position ((int64_t) a - b); }

শূন্য debugprint1 (ৱর্ণ *s, int32_t);
শূন্য debugprint2 (ৱর্ণ *s, int32_t, int32_t);

স্থির ধ্রুৱ শূন্য *copy_table (ধ্রুৱ শূন্য *data, unsigned পূর্ণাঙ্ক tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  যদি (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  ঘূৰাই blob.data;
}

স্থির শূন্য free_table (ধ্রুৱ শূন্য *data, ধ্রুৱ শূন্য *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (ৱর্ণ *) table_data;
  blob_free (&blob);
}

শূন্য *
shape_plan_create (face_t *face)
{
  ধ্রুৱ gr_face_ops ops = {পরিমাণ (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ঘূৰাই grface;
}

শূন্য
shape_plan_destroy (শূন্য *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ধ্রুৱ feature_t *features,
       uint32_t অঙ্ক_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  যদি ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  যদি (!buffer_copy_contents (buffer, &contents))
    ঘূৰাই মিছা;

  gr_segment *seg = nullptr;
  ধ্রুৱ gr_slot *is;
  unsigned পূর্ণাঙ্ক ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * পরিমাণ (uint32_t));
  যদি (!chars)
    ঘূৰাই মিছা;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  মুক্ত (chars);

  যদি (!seg)
    ঘূৰাই মিছা;

  unsigned পূর্ণাঙ্ক glyph_গুনতি = gr_seg_n_slots (seg);

  সংরচনা cluster_t {
    unsigned পূর্ণাঙ্ক base_char;
    unsigned পূর্ণাঙ্ক অঙ্ক_chars;
    unsigned পূর্ণাঙ্ক base_glyph;
    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs;
    unsigned পূর্ণাঙ্ক cluster;
    int32_t advance;
  };

  length = glyph_গুনতি;
  যদি (!buffer_contents_realloc (&contents, length))
    ঘূৰাই মিছা;
  cluster_t *clusters = (cluster_t *) malloc (length * পরিমাণ (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * পরিমাণ (uint32_t));
  যদি (!clusters || !gids)
  {
    মুক্ত (clusters);
    মুক্ত (gids);
    ঘূৰাই মিছা;
  }

  memset (clusters, 0, পরিমাণ (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned পূর্ণাঙ্ক upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  ভিন্ন xscale = (ভিন্ন) font_x_scale / upem;
  ভিন্ন yscale = (ভিন্ন) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  যদি (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((দ্ৱিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  অন্যথা
    clusters[0].advance = 0;
  বাবে (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned পূর্ণাঙ্ক before = gr_slot_before (is);
    unsigned পূর্ণাঙ্ক after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    যেতিয়ালৈ (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].অঙ্ক_chars += clusters[ci].অঙ্ক_chars;
      clusters[ci-1].অঙ্ক_glyphs += clusters[ci].অঙ্ক_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    যদি (gr_slot_can_ঢোকাও_before (is) && clusters[ci].অঙ্ক_chars && before >= clusters[ci].base_char + clusters[ci].অঙ্ক_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].অঙ্ক_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->অঙ্ক_chars = before - c->base_char;
      c->base_glyph = ic;
      c->অঙ্ক_glyphs = 0;
      যদি (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((দ্ৱিগুণ) curradv - (দ্ৱিগুণ) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      অন্যথা
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].অঙ্ক_glyphs++;

    যদি (clusters[ci].base_char + clusters[ci].অঙ্ক_chars < after + 1)
	clusters[ci].অঙ্ক_chars = after + 1 - clusters[ci].base_char;
  }

  যদি (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  অন্যথা
    clusters[ci].advance = clamp_position ((দ্ৱিগুণ) clusters[ci].advance +
					   (দ্ৱিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < ci; ++i)
  {
    বাবে (unsigned পূর্ণাঙ্ক j = 0; j < clusters[i].অঙ্ক_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_গুনতি;

  /* Positioning. */
  unsigned পূর্ণাঙ্ক currclus = 0xFFFFFFFF;
  ধ্রুৱ glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  যদি (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    বাবে (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((দ্ৱিগুণ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((দ্ৱিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      যদি (info->cluster != currclus) {
	pPos->x_advance = (পূর্ণাঙ্ক) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  অন্যথা
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    বাবে (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      যদি (info->cluster != currclus)
      {
	pPos->x_advance = (পূর্ণাঙ্ক) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((দ্ৱিগুণ) (gr_slot_origin_X (is) * xscale) -
					      (পূর্ণাঙ্ক) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((দ্ৱিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  মুক্ত (clusters);
  মুক্ত (gids);

  তার্কিক ret = glyph_গুনতি;

  ঘূৰাই ret;
}
