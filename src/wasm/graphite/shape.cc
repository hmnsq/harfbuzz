#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#അന്തര്ഭാവ <hb-wasm-api.h>

#അന്തര്ഭാവ <graphite2/Segment.h>

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

സ്ഥിതം int32_t
clamp_position (ഇരട്ട value)
{
  എങ്കിൽ (value <= INT32_MIN) തിരികെ INT32_MIN;
  എങ്കിൽ (value >= INT32_MAX) തിരികെ INT32_MAX;
  തിരികെ (int32_t) value;
}

സ്ഥിതം int32_t
saturate_add (int32_t a, int32_t b)
{ തിരികെ clamp_position ((int64_t) a + b); }

സ്ഥിതം int32_t
saturate_sub (int32_t a, int32_t b)
{ തിരികെ clamp_position ((int64_t) a - b); }

ശൂന്യം debugprint1 (അക്ഷരം *s, int32_t);
ശൂന്യം debugprint2 (അക്ഷരം *s, int32_t, int32_t);

സ്ഥിതം സ്ഥിരം ശൂന്യം *copy_table (സ്ഥിരം ശൂന്യം *data, unsigned പൂർണ്ണസംഖ്യ tag, size_t *len)
{
  face_t *face = (face_t *) data;
  blob_t blob = BLOB_INIT;
  എങ്കിൽ (!face_copy_table (face, tag, &blob))
    abort ();

  *len = blob.length;
  തിരികെ blob.data;
}

സ്ഥിതം ശൂന്യം free_table (സ്ഥിരം ശൂന്യം *data, സ്ഥിരം ശൂന്യം *table_data)
{
  blob_t blob;
  blob.length = 0; // Doesn't matter
  blob.data = (അക്ഷരം *) table_data;
  blob_free (&blob);
}

ശൂന്യം *
shape_plan_create (face_t *face)
{
  സ്ഥിരം gr_face_ops ops = {വലുപ്പം (gr_face_ops), &copy_table, &free_table};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  തിരികെ grface;
}

ശൂന്യം
shape_plan_destroy (ശൂന്യം *data)
{
  gr_face_destroy ((gr_face *) data);
}

bool_t
shape (ശൂന്യം *shape_plan,
       font_t *font,
       buffer_t *buffer,
       സ്ഥിരം feature_t *features,
       uint32_t അക്കം_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = buffer_get_direction (buffer);
  direction_t horiz_dir = script_get_horizontal_direction (buffer_get_script (buffer));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  എങ്കിൽ ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    buffer_reverse_clusters (buffer);
    direction = DIRECTION_REVERSE (direction);
  }

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  എങ്കിൽ (!buffer_copy_contents (buffer, &contents))
    തിരികെ അസത്യം;

  gr_segment *seg = nullptr;
  സ്ഥിരം gr_slot *is;
  unsigned പൂർണ്ണസംഖ്യ ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  unsigned length = contents.length;

  uint32_t *chars = (uint32_t *) malloc (length * വലുപ്പം (uint32_t));
  എങ്കിൽ (!chars)
    തിരികെ അസത്യം;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < contents.length; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (nullptr, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     nullptr,
		     gr_utf32, chars, contents.length,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  സ്വതന്ത്രം (chars);

  എങ്കിൽ (!seg)
    തിരികെ അസത്യം;

  unsigned പൂർണ്ണസംഖ്യ glyph_എണ്ണം = gr_seg_n_slots (seg);

  ഘടന cluster_t {
    unsigned പൂർണ്ണസംഖ്യ base_char;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_chars;
    unsigned പൂർണ്ണസംഖ്യ base_glyph;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs;
    unsigned പൂർണ്ണസംഖ്യ cluster;
    int32_t advance;
  };

  length = glyph_എണ്ണം;
  എങ്കിൽ (!buffer_contents_realloc (&contents, length))
    തിരികെ അസത്യം;
  cluster_t *clusters = (cluster_t *) malloc (length * വലുപ്പം (cluster_t));
  uint32_t *gids = (uint32_t *) malloc (length * വലുപ്പം (uint32_t));
  എങ്കിൽ (!clusters || !gids)
  {
    സ്വതന്ത്രം (clusters);
    സ്വതന്ത്രം (gids);
    തിരികെ അസത്യം;
  }

  memset (clusters, 0, വലുപ്പം (clusters[0]) * length);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  unsigned പൂർണ്ണസംഖ്യ upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  ദശാംശം xscale = (ദശാംശം) font_x_scale / upem;
  ദശാംശം yscale = (ദശാംശം) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  എങ്കിൽ (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_slot_origin_X(gr_seg_first_slot(seg)) * xscale);
    clusters[0].advance = clamp_position ((ഇരട്ട) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  അല്ലെങ്കിൽ
    clusters[0].advance = 0;
  വേണ്ടി (is = gr_seg_first_slot (seg), ic = 0; is; is = gr_slot_next_in_segment (is), ic++)
  {
    unsigned പൂർണ്ണസംഖ്യ before = gr_slot_before (is);
    unsigned പൂർണ്ണസംഖ്യ after = gr_slot_after (is);
    *pg = gr_slot_gid (is);
    pg++;
    ആകുമ്പോൾ (clusters[ci].base_char > before && ci)
    {
      clusters[ci-1].അക്കം_chars += clusters[ci].അക്കം_chars;
      clusters[ci-1].അക്കം_glyphs += clusters[ci].അക്കം_glyphs;
      clusters[ci-1].advance = saturate_add (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    എങ്കിൽ (gr_slot_can_തിരുകുക_before (is) && clusters[ci].അക്കം_chars && before >= clusters[ci].base_char + clusters[ci].അക്കം_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->base_char = clusters[ci].base_char + clusters[ci].അക്കം_chars;
      c->cluster = contents.info[c->base_char].cluster;
      c->അക്കം_chars = before - c->base_char;
      c->base_glyph = ic;
      c->അക്കം_glyphs = 0;
      എങ്കിൽ (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((ഇരട്ട) curradv - (ഇരട്ട) (gr_slot_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      അല്ലെങ്കിൽ
      {
	int32_t origin_X = clamp_position (gr_slot_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_add (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].അക്കം_glyphs++;

    എങ്കിൽ (clusters[ci].base_char + clusters[ci].അക്കം_chars < after + 1)
	clusters[ci].അക്കം_chars = after + 1 - clusters[ci].base_char;
  }

  എങ്കിൽ (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_add (clusters[ci].advance, curradv);
  അല്ലെങ്കിൽ
    clusters[ci].advance = clamp_position ((ഇരട്ട) clusters[ci].advance +
					   (ഇരട്ട) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ci; ++i)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < clusters[i].അക്കം_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].base_glyph + j];
      info->codepoint = gids[clusters[i].base_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (unsigned) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.length = glyph_എണ്ണം;

  /* Positioning. */
  unsigned പൂർണ്ണസംഖ്യ currclus = 0xFFFFFFFF;
  സ്ഥിരം glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  എങ്കിൽ (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    വേണ്ടി (is = gr_seg_first_slot (seg); is; pPos++, ++info, is = gr_slot_next_in_segment (is))
    {
      pPos->x_offset = clamp_position ((ഇരട്ട) (gr_slot_origin_X (is) * xscale) - curradvx);
      pPos->y_offset = clamp_position ((ഇരട്ട) (gr_slot_origin_Y (is) * yscale) - curradvy);
      എങ്കിൽ (info->cluster != currclus) {
	pPos->x_advance = (പൂർണ്ണസംഖ്യ) info->var1;
	curradvx = saturate_add (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } അല്ലെങ്കിൽ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_add (curradvy, pPos->y_advance);
    }
    buffer_set_contents (buffer, &contents);
  }
  അല്ലെങ്കിൽ
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    വേണ്ടി (is = gr_seg_first_slot (seg); is; pPos++, info++, is = gr_slot_next_in_segment (is))
    {
      എങ്കിൽ (info->cluster != currclus)
      {
	pPos->x_advance = (പൂർണ്ണസംഖ്യ) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } അല്ലെങ്കിൽ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_slot_advance_Y (is, grface, nullptr) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_offset = clamp_position ((ഇരട്ട) (gr_slot_origin_X (is) * xscale) -
					      (പൂർണ്ണസംഖ്യ) info->var1 - curradvx + pPos->x_advance);
      pPos->y_offset = clamp_position ((ഇരട്ട) (gr_slot_origin_Y (is) * yscale) - curradvy);
    }
    buffer_set_contents (buffer, &contents);
    buffer_reverse_clusters (buffer);
  }

  gr_seg_destroy (seg);
  സ്വതന്ത്രം (clusters);
  സ്വതന്ത്രം (gids);

  തർക്കം ret = glyph_എണ്ണം;

  തിരികെ ret;
}
