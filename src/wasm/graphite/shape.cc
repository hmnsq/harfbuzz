#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#include <hb-wasm-api.h>

#include <graphite2/Segment.h>

#include <stdlib.h>
#include <string.h>

static int32_t
clamp_position (dviguu_na value)
{
  အကယ်၍ (value <= INT32_MIN) ပြန်ပေး INT32_MIN;
  အကယ်၍ (value >= INT32_MAX) ပြန်ပေး INT32_MAX;
  ပြန်ပေး (int32_t) value;
}

static int32_t
saturate_add (int32_t a, int32_t b)
{ ပြန်ပေး clamp_position ((int64_t) a + b); }

static int32_t
saturate_sub (int32_t a, int32_t b)
{ ပြန်ပေး clamp_position ((int64_t) a - b); }

void debugprint1 (var_na *s, int32_t);
void debugprint2 (var_na *s, int32_t, int32_t);

static const void *copy_table (const void *data, unsigned ကိန်းပြည့် tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  အကယ်၍ (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  ပြန်ပေး blob.data;
}

static void free_table (const void *data, const void *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (var_na *) table_data;
  blob_free (&blob);
}

void *
shape_plan_create (face_t *face)
{
  const gr_face_ops ops = {parimaa_na (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ပြန်ပေး grface;
}

void
shape_plan_destroy (void *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (void *shape_plan,
       font_t *font,
       buffer_t *buffer,
       const feature_t *features,
       uint32_t num_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  အကယ်၍ ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  အကယ်၍ (!buffer_copy_contents (buffer, &contents))
    ပြန်ပေး false;

  gr_segment *seg = nullptr;
  const gr_slot *is;
  unsigned ကိန်းပြည့် ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * parimaa_na (uint32_t));
  အကယ်၍ (!chars)
    ပြန်ပေး false;
  အတွက် (unsigned ကိန်းပြည့် i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  free (chars);

  အကယ်၍ (!seg)
    ပြန်ပေး false;

  unsigned ကိန်းပြည့် glyph_count = gr_seg_n_slots (seg);

  sa_nracanaa cluster_t {
    unsigned ကိန်းပြည့် base_char;
    unsigned ကိန်းပြည့် num_chars;
    unsigned ကိန်းပြည့် base_glyph;
    unsigned ကိန်းပြည့် num_glyphs;
    unsigned ကိန်းပြည့် cluster;
    int32_t advance;
  };

  length = glyph_count;
  အကယ်၍ (!buffer_contents_realloc (&contents, length))
    ပြန်ပေး false;
  cluster_t *clusters = (cluster_t *) malloc (length * parimaa_na (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * parimaa_na (uint32_t));
  အကယ်၍ (!clusters || !gids)
  {
    free (clusters);
    free (gids);
    ပြန်ပေး false;
  }

  memset (clusters, 0, parimaa_na (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned ကိန်းပြည့် upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  float xscale = (float) font_x_scale / upem;
  float yscale = (float) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  အကယ်၍ (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((dviguu_na) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  မဟုတ်ပါက
    clusters[0].advance = 0;
  အတွက် (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned ကိန်းပြည့် before = gr_slot_before (is);
    unsigned ကိန်းပြည့် after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    နေစဉ် (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].num_chars += clusters[ci].num_chars;
      clusters[ci-1].num_glyphs += clusters[ci].num_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    အကယ်၍ (gr_slot_can_insert_before (is) && clusters[ci].num_chars && before >= clusters[ci].base_char + clusters[ci].num_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].num_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->num_chars = before - c->base_char;
      c->base_glyph = ic;
      c->num_glyphs = 0;
      အကယ်၍ (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((dviguu_na) curradv - (dviguu_na) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      မဟုတ်ပါက
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].num_glyphs++;

    အကယ်၍ (clusters[ci].base_char + clusters[ci].num_chars < after + 1)
	clusters[ci].num_chars = after + 1 - clusters[ci].base_char;
  }

  အကယ်၍ (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  မဟုတ်ပါက
    clusters[ci].advance = clamp_position ((dviguu_na) clusters[ci].advance +
					   (dviguu_na) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  အတွက် (unsigned ကိန်းပြည့် i = 0; i < ci; ++i)
  {
    အတွက် (unsigned ကိန်းပြည့် j = 0; j < clusters[i].num_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_count;

  /* Positioning. */
  unsigned ကိန်းပြည့် currclus = 0xFFFFFFFF;
  const glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  အကယ်၍ (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    အတွက် (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((dviguu_na) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((dviguu_na) (gr_slot_origin_Y (is) * yscale) - curradvy);
      အကယ်၍ (info->cluster != currclus) {
	pPos->x_advance = (ကိန်းပြည့်) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } မဟုတ်ပါက
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  မဟုတ်ပါက
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    အတွက် (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      အကယ်၍ (info->cluster != currclus)
      {
	pPos->x_advance = (ကိန်းပြည့်) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } မဟုတ်ပါက
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((dviguu_na) (gr_slot_origin_X (is) * xscale) -
					      (ကိန်းပြည့်) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((dviguu_na) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  free (clusters);
  free (gids);

  bool ret = glyph_count;

  ပြန်ပေး ret;
}
