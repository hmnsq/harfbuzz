#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ضمن <hb-wasm-api.h>

#ضمن <graphite2/Segment.h>

#ضمن <stdlib.h>
#ضمن <string.h>

ساكن int32_t
clamp_position (مزدوج value)
{
  إذا (value <= INT32_MIN) أرجع INT32_MIN;
  إذا (value >= INT32_MAX) أرجع INT32_MAX;
  أرجع (int32_t) value;
}

ساكن int32_t
saturate_add (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a + b); }

ساكن int32_t
saturate_sub (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a - b); }

فراغ debugprint1 (حرف *s, int32_t);
فراغ debugprint2 (حرف *s, int32_t, int32_t);

ساكن ثابت فراغ *copy_table (ثابت فراغ *data, unsigned صحيح tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  إذا (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  أرجع blob.data;
}

ساكن فراغ free_table (ثابت فراغ *data, ثابت فراغ *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (حرف *) table_data;
  blob_free (&blob);
}

فراغ *
shape_plan_create (face_t *face)
{
  ثابت gr_face_ops ops = {حجم (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  أرجع grface;
}

فراغ
shape_plan_destroy (فراغ *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (فراغ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ثابت feature_t *features,
       uint32_t عدد_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  إذا (!buffer_copy_contents (buffer, &contents))
    أرجع خطأ;

  gr_segment *seg = nullptr;
  ثابت gr_slot *is;
  unsigned صحيح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * حجم (uint32_t));
  إذا (!chars)
    أرجع خطأ;
  لكل (unsigned صحيح i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  حرر (chars);

  إذا (!seg)
    أرجع خطأ;

  unsigned صحيح glyph_گنتی = gr_seg_n_slots (seg);

  بنية cluster_t {
    unsigned صحيح base_char;
    unsigned صحيح عدد_chars;
    unsigned صحيح base_glyph;
    unsigned صحيح عدد_glyphs;
    unsigned صحيح cluster;
    int32_t advance;
  };

  length = glyph_گنتی;
  إذا (!buffer_contents_realloc (&contents, length))
    أرجع خطأ;
  cluster_t *clusters = (cluster_t *) malloc (length * حجم (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * حجم (uint32_t));
  إذا (!clusters || !gids)
  {
    حرر (clusters);
    حرر (gids);
    أرجع خطأ;
  }

  memset (clusters, 0, حجم (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned صحيح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  عائم xscale = (عائم) font_x_scale / upem;
  عائم yscale = (عائم) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  إذا (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned صحيح before = gr_slot_before (is);
    unsigned صحيح after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    طالما (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].عدد_chars += clusters[ci].عدد_chars;
      clusters[ci-1].عدد_glyphs += clusters[ci].عدد_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_slot_can_داخل_کرو_before (is) && clusters[ci].عدد_chars && before >= clusters[ci].base_char + clusters[ci].عدد_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].عدد_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->عدد_chars = before - c->base_char;
      c->base_glyph = ic;
      c->عدد_glyphs = 0;
      إذا (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((مزدوج) curradv - (مزدوج) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].عدد_glyphs++;

    إذا (clusters[ci].base_char + clusters[ci].عدد_chars < after + 1)
	clusters[ci].عدد_chars = after + 1 - clusters[ci].base_char;
  }

  إذا (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = clamp_position ((مزدوج) clusters[ci].advance +
					   (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (unsigned صحيح i = 0; i < ci; ++i)
  {
    لكل (unsigned صحيح j = 0; j < clusters[i].عدد_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_گنتی;

  /* Positioning. */
  unsigned صحيح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  إذا (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((مزدوج) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((مزدوج) (gr_slot_origin_Y (is) * yscale) - curradvy);
      إذا (info->cluster != currclus) {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  وإلا
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      إذا (info->cluster != currclus)
      {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((مزدوج) (gr_slot_origin_X (is) * xscale) -
					      (صحيح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((مزدوج) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  حرر (clusters);
  حرر (gids);

  منطقي ret = glyph_گنتی;

  أرجع ret;
}
