#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাব <hb-wasm-api.h>

#অন্তর্ভাব <graphite2/Segment.h>

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

স্থির int32_t
clamp_position (দ্বিগুণ value)
{
  যদি (value <= INT32_MIN) ফেরত INT32_MIN;
  যদি (value >= INT32_MAX) ফেরত INT32_MAX;
  ফেরত (int32_t) value;
}

স্থির int32_t
saturate_add (int32_t a, int32_t b)
{ ফেরত clamp_position ((int64_t) a + b); }

স্থির int32_t
saturate_sub (int32_t a, int32_t b)
{ ফেরত clamp_position ((int64_t) a - b); }

শূন্য debugprint1 (অক্ষর *s, int32_t);
শূন্য debugprint2 (অক্ষর *s, int32_t, int32_t);

স্থির ধ্রুবক শূন্য *copy_table (ধ্রুবক শূন্য *তথ্য, unsigned পূর্ণসংখ্যা tag, size_t *len)
{
  face_t *face = (face_t *) তথ্য;
  blob_t blob = BLOB_INIT;
  যদি (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.দৈর্ঘ্য;
  ফেরত blob.তথ্য;
}

স্থির শূন্য free_table (ধ্রুবক শূন্য *তথ্য, ধ্রুবক শূন্য *table_তথ্য)
{
  blob_t blob;
  blob.দৈর্ঘ্য = 0; // Doesn't matter
  blob.তথ্য = (অক্ষর *) table_তথ্য;
  blob_free (&blob);
}

শূন্য *
shape_plan_create (face_t *face)
{
  ধ্রুবক gr_face_ops ops = {আকার (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ফেরত grface;
}

শূন্য
shape_plan_destroy (শূন্য *তথ্য)
{
  gr_face_destroy ((gr_face *) তথ্য);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       বাফার_t *বাফার,
       ধ্রুবক feature_t *features,
       uint32_t অঙ্ক_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = বাফার_get_direction (বাফার);
  direction_t horiz_dir = script_get_horizontal_direction (বাফার_get_script (বাফার));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  যদি ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    বাফার_reverse_clusters (বাফার);
    direction = DIRECTION_REVERSE (direction);
  }

  বাফার_contents_t contents = বাফার_CONTENTS_INIT;
  যদি (!বাফার_copy_contents (বাফার, &contents))
    ফেরত মিথ্যা;

  gr_segment *seg = nullptr;
  ধ্রুবক gr_slot *is;
  unsigned পূর্ণসংখ্যা ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned দৈর্ঘ্য = contents.দৈর্ঘ্য;

  uint32_t *chars = (uint32_t *) malloc (দৈর্ঘ্য * আকার (uint32_t));
  যদি (!chars)
    ফেরত মিথ্যা;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < contents.দৈর্ঘ্য; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.দৈর্ঘ্য,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  মুক্ত (chars);

  যদি (!seg)
    ফেরত মিথ্যা;

  unsigned পূর্ণসংখ্যা glyph_গুনতি = gr_seg_n_slots (seg);

  কাঠামো cluster_t {
    unsigned পূর্ণসংখ্যা base_char;
    unsigned পূর্ণসংখ্যা অঙ্ক_chars;
    unsigned পূর্ণসংখ্যা base_glyph;
    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
    unsigned পূর্ণসংখ্যা cluster;
    int32_t advance;
  };

  দৈর্ঘ্য = glyph_গুনতি;
  যদি (!বাফার_contents_realloc (&contents, দৈর্ঘ্য))
    ফেরত মিথ্যা;
  cluster_t *clusters = (cluster_t *) malloc (দৈর্ঘ্য * আকার (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (দৈর্ঘ্য * আকার (uint32_t));
  যদি (!clusters || !gids)
  {
    মুক্ত (clusters);
    মুক্ত (gids);
    ফেরত মিথ্যা;
  }

  memset (clusters, 0, আকার (clusters[0]) * দৈর্ঘ্য);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned পূর্ণসংখ্যা upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  দশমিক xscale = (দশমিক) font_x_scale / upem;
  দশমিক yscale = (দশমিক) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  যদি (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((দ্বিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  অন্যথা
    clusters[0].advance = 0;
  জন্য (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_পরবর্তী_in_segment (is), ic++)
  {
    unsigned পূর্ণসংখ্যা before = gr_slot_before (is);
    unsigned পূর্ণসংখ্যা after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    যখন (clusters[ci].base_char > before && ci)
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
	c->advance = clamp_position ((দ্বিগুণ) curradv - (দ্বিগুণ) (gr_slot_origin_X(is) * xscale));
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
    clusters[ci].advance = clamp_position ((দ্বিগুণ) clusters[ci].advance +
					   (দ্বিগুণ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ci; ++i)
  {
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < clusters[i].অঙ্ক_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.দৈর্ঘ্য = glyph_গুনতি;

  /* Positioning. */
  unsigned পূর্ণসংখ্যা currclus = 0xFFFFFFFF;
  ধ্রুবক glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  যদি (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    জন্য (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_পরবর্তী_in_segment (is))
    {
      pPos->x_offset = clamp_position ((দ্বিগুণ) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((দ্বিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
      যদি (info->cluster != currclus) {
	pPos->x_advance = (পূর্ণসংখ্যা) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    বাফার_set_contents (বাফার, &contents);
  }
  অন্যথা
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    জন্য (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_পরবর্তী_in_segment (is))
    {
      যদি (info->cluster != currclus)
      {
	pPos->x_advance = (পূর্ণসংখ্যা) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } অন্যথা
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((দ্বিগুণ) (gr_slot_origin_X (is) * xscale) -
					      (পূর্ণসংখ্যা) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((দ্বিগুণ) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    বাফার_set_contents (বাফার, &contents);
    বাফার_reverse_clusters (বাফার);
  }

  gr_seg_destroy (seg);
  মুক্ত (clusters);
  মুক্ত (gids);

  যৌক্তিক ret = glyph_গুনতি;

  ফেরত ret;
}
