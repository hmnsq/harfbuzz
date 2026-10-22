#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#شامل <hb-wasm-api.h>

#شامل <graphite2/Segment.h>

#شامل <stdlib.h>
#شامل <string.h>

ایستا int32_t
clamp_position (دوگانه value)
{
  اگر (value <= INT32_MIN) بازگشت INT32_MIN;
  اگر (value >= INT32_MAX) بازگشت INT32_MAX;
  بازگشت (int32_t) value;
}

ایستا int32_t
saturate_add (int32_t a, int32_t b)
{ بازگشت clamp_position ((int64_t) a + b); }

ایستا int32_t
saturate_sub (int32_t a, int32_t b)
{ بازگشت clamp_position ((int64_t) a - b); }

تهی debugprint1 (نویسه *s, int32_t);
تهی debugprint2 (نویسه *s, int32_t, int32_t);

ایستا ثابت تهی *copy_table (ثابت تهی *data, unsigned صحیح tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  اگر (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  بازگشت blob.data;
}

ایستا تهی free_table (ثابت تهی *data, ثابت تهی *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (نویسه *) table_data;
  blob_free (&blob);
}

تهی *
shape_plan_create (face_t *face)
{
  ثابت gr_face_ops ops = {اندازه (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  بازگشت grface;
}

تهی
shape_plan_destroy (تهی *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (تهی *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ثابت feature_t *features,
       uint32_t رقم_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  اگر ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  اگر (!buffer_copy_contents (buffer, &contents))
    بازگشت نادرست;

  gr_segment *seg = nullptr;
  ثابت gr_slot *is;
  unsigned صحیح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * اندازه (uint32_t));
  اگر (!chars)
    بازگشت نادرست;
  برای (unsigned صحیح i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  آزاد (chars);

  اگر (!seg)
    بازگشت نادرست;

  unsigned صحیح glyph_شمار = gr_seg_n_slots (seg);

  ساختار cluster_t {
    unsigned صحیح base_char;
    unsigned صحیح رقم_chars;
    unsigned صحیح base_glyph;
    unsigned صحیح رقم_glyphs;
    unsigned صحیح cluster;
    int32_t advance;
  };

  length = glyph_شمار;
  اگر (!buffer_contents_realloc (&contents, length))
    بازگشت نادرست;
  cluster_t *clusters = (cluster_t *) malloc (length * اندازه (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * اندازه (uint32_t));
  اگر (!clusters || !gids)
  {
    آزاد (clusters);
    آزاد (gids);
    بازگشت نادرست;
  }

  memset (clusters, 0, اندازه (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned صحیح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  اعشاری xscale = (اعشاری) font_x_scale / upem;
  اعشاری yscale = (اعشاری) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  اگر (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((دوگانه) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وگرنه
    clusters[0].advance = 0;
  برای (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned صحیح before = gr_slot_before (is);
    unsigned صحیح after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    وقتی (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].رقم_chars += clusters[ci].رقم_chars;
      clusters[ci-1].رقم_glyphs += clusters[ci].رقم_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    اگر (gr_slot_can_درج_before (is) && clusters[ci].رقم_chars && before >= clusters[ci].base_char + clusters[ci].رقم_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].رقم_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->رقم_chars = before - c->base_char;
      c->base_glyph = ic;
      c->رقم_glyphs = 0;
      اگر (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((دوگانه) curradv - (دوگانه) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      وگرنه
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].رقم_glyphs++;

    اگر (clusters[ci].base_char + clusters[ci].رقم_chars < after + 1)
	clusters[ci].رقم_chars = after + 1 - clusters[ci].base_char;
  }

  اگر (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  وگرنه
    clusters[ci].advance = clamp_position ((دوگانه) clusters[ci].advance +
					   (دوگانه) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  برای (unsigned صحیح i = 0; i < ci; ++i)
  {
    برای (unsigned صحیح j = 0; j < clusters[i].رقم_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_شمار;

  /* Positioning. */
  unsigned صحیح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  اگر (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    برای (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((دوگانه) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((دوگانه) (gr_slot_origin_Y (is) * yscale) - curradvy);
      اگر (info->cluster != currclus) {
	pPos->x_advance = (صحیح) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وگرنه
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  وگرنه
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    برای (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      اگر (info->cluster != currclus)
      {
	pPos->x_advance = (صحیح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وگرنه
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((دوگانه) (gr_slot_origin_X (is) * xscale) -
					      (صحیح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((دوگانه) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  آزاد (clusters);
  آزاد (gids);

  منطقی ret = glyph_شمار;

  بازگشت ret;
}
