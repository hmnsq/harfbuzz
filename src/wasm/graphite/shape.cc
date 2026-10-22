#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ਅਨ੍ਤਰ੍ਭਾਵ <hb-wasm-api.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <graphite2/Segment.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਸਥਿਰ int32_t
clamp_position (ਦੁੱਗਣਾ value)
{
  ਜੇ (value <= INT32_MIN) ਵਾਪਸ INT32_MIN;
  ਜੇ (value >= INT32_MAX) ਵਾਪਸ INT32_MAX;
  ਵਾਪਸ (int32_t) value;
}

ਸਥਿਰ int32_t
saturate_ਜੋੜੋ (int32_t a, int32_t b)
{ ਵਾਪਸ clamp_position ((int64_t) a + b); }

ਸਥਿਰ int32_t
saturate_sub (int32_t a, int32_t b)
{ ਵਾਪਸ clamp_position ((int64_t) a - b); }

ਖਾਲੀ debugprint1 (ਅੱਖਰ *s, int32_t);
ਖਾਲੀ debugprint2 (ਅੱਖਰ *s, int32_t, int32_t);

ਸਥਿਰ ਅਚਲ ਖਾਲੀ *copy_ਸਾਰਣੀ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਟੈਗ_2, size_t *len)
{
  face_t *face = (face_t *) ਅੰਕੜੇ;
  blob_t blob = BLOB_INIT;
  ਜੇ (!face_copy_ਸਾਰਣੀ (face, ਟੈਗ_2, &blob))
    abort ();

  *len = blob.ਲੰਮਾਈ;
  ਵਾਪਸ blob.ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ free_ਸਾਰਣੀ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ_ਅੰਕੜੇ)
{
  blob_t blob;
  blob.ਲੰਮਾਈ = 0; // Doesn't matter
  blob.ਅੰਕੜੇ = (ਅੱਖਰ *) ਸਾਰਣੀ_ਅੰਕੜੇ;
  blob_free (&blob);
}

ਖਾਲੀ *
shape_plan_create (face_t *face)
{
  ਅਚਲ gr_face_ops ops = {ਆਕਾਰ (gr_face_ops), &copy_ਸਾਰਣੀ, &free_ਸਾਰਣੀ};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  ਵਾਪਸ grface;
}

ਖਾਲੀ
shape_plan_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  gr_face_destroy ((gr_face *) ਅੰਕੜੇ);
}

bool_t
shape (ਖਾਲੀ *shape_plan,
       font_t *font,
       ਬਫਰ_t *ਬਫਰ,
       ਅਚਲ feature_t *features,
       uint32_t ਅੰਕ_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = ਬਫਰ_get_direction (ਬਫਰ);
  direction_t horiz_dir = script_get_horizontal_direction (ਬਫਰ_get_script (ਬਫਰ));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ਜੇ ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    ਬਫਰ_ਉਲਟ_clusters (ਬਫਰ);
    direction = DIRECTION_ਉਲਟ (direction);
  }

  ਬਫਰ_contents_t contents = ਬਫਰ_CONTENTS_INIT;
  ਜੇ (!ਬਫਰ_copy_contents (ਬਫਰ, &contents))
    ਵਾਪਸ ਝੂਠ;

  gr_segment *seg = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ gr_ਥਾਂ *is;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = contents.ਲੰਮਾਈ;

  uint32_t *chars = (uint32_t *) ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਲੰਮਾਈ * ਆਕਾਰ (uint32_t));
  ਜੇ (!chars)
    ਵਾਪਸ ਝੂਠ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < contents.ਲੰਮਾਈ; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (ਖਾਲੀ_ਸੂਚਕ, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     ਖਾਲੀ_ਸੂਚਕ,
		     gr_utf32, chars, contents.ਲੰਮਾਈ,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  ਮੁਕਤ (chars);

  ਜੇ (!seg)
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਗਿਣਤੀ = gr_seg_n_slots (seg);

  ਬਣਤਰ cluster_t {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_char;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_chars;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_glyph;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster;
    int32_t advance;
  };

  ਲੰਮਾਈ = glyph_ਗਿਣਤੀ;
  ਜੇ (!ਬਫਰ_contents_realloc (&contents, ਲੰਮਾਈ))
    ਵਾਪਸ ਝੂਠ;
  cluster_t *clusters = (cluster_t *) ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਲੰਮਾਈ * ਆਕਾਰ (cluster_t));
  uint32_t *gids = (uint32_t *) ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਲੰਮਾਈ * ਆਕਾਰ (uint32_t));
  ਜੇ (!clusters || !gids)
  {
    ਮੁਕਤ (clusters);
    ਮੁਕਤ (gids);
    ਵਾਪਸ ਝੂਠ;
  }

  memset (clusters, 0, ਆਕਾਰ (clusters[0]) * ਲੰਮਾਈ);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  ਦਸ਼ਮਲਵ xscale = (ਦਸ਼ਮਲਵ) font_x_scale / upem;
  ਦਸ਼ਮਲਵ yscale = (ਦਸ਼ਮਲਵ) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  ਜੇ (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_ਥਾਂ_origin_X(gr_seg_ਪਹਿਲਾ_ਥਾਂ(seg)) * xscale);
    clusters[0].advance = clamp_position ((ਦੁੱਗਣਾ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ਨਹੀਂਤਾਂ
    clusters[0].advance = 0;
  ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg), ic = 0; is; is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is), ic++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ before = gr_ਥਾਂ_before (is);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ after = gr_ਥਾਂ_after (is);
    *pg = gr_ਥਾਂ_gid (is);
    pg++;
    ਜਦੋਂ (clusters[ci].ਅਧਾਰ_char > before && ci)
    {
      clusters[ci-1].ਅੰਕ_chars += clusters[ci].ਅੰਕ_chars;
      clusters[ci-1].ਅੰਕ_glyphs += clusters[ci].ਅੰਕ_glyphs;
      clusters[ci-1].advance = saturate_ਜੋੜੋ (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ਜੇ (gr_ਥਾਂ_can_ਪਾਓ_before (is) && clusters[ci].ਅੰਕ_chars && before >= clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->ਅਧਾਰ_char = clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars;
      c->cluster = contents.info[c->ਅਧਾਰ_char].cluster;
      c->ਅੰਕ_chars = before - c->ਅਧਾਰ_char;
      c->ਅਧਾਰ_glyph = ic;
      c->ਅੰਕ_glyphs = 0;
      ਜੇ (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((ਦੁੱਗਣਾ) curradv - (ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      ਨਹੀਂਤਾਂ
      {
	int32_t origin_X = clamp_position (gr_ਥਾਂ_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_ਜੋੜੋ (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].ਅੰਕ_glyphs++;

    ਜੇ (clusters[ci].ਅਧਾਰ_char + clusters[ci].ਅੰਕ_chars < after + 1)
	clusters[ci].ਅੰਕ_chars = after + 1 - clusters[ci].ਅਧਾਰ_char;
  }

  ਜੇ (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_ਜੋੜੋ (clusters[ci].advance, curradv);
  ਨਹੀਂਤਾਂ
    clusters[ci].advance = clamp_position ((ਦੁੱਗਣਾ) clusters[ci].advance +
					   (ਦੁੱਗਣਾ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ci; ++i)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < clusters[i].ਅੰਕ_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].ਅਧਾਰ_glyph + j];
      info->codepoint = gids[clusters[i].ਅਧਾਰ_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.ਲੰਮਾਈ = glyph_ਗਿਣਤੀ;

  /* Positioning. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ currclus = 0xFFFFFFFF;
  ਅਚਲ glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  ਜੇ (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg); is; pPos++, ++info, is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is))
    {
      pPos->x_ਵਿੱਥ_2 = clamp_position ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X (is) * xscale) - curradvx);
      pPos->y_ਵਿੱਥ_2 = clamp_position ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_Y (is) * yscale) - curradvy);
      ਜੇ (info->cluster != currclus) {
	pPos->x_advance = (ਪੂਰਨ) info->var1;
	curradvx = saturate_ਜੋੜੋ (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ਨਹੀਂਤਾਂ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_ਥਾਂ_advance_Y (is, grface, ਖਾਲੀ_ਸੂਚਕ) * yscale);
      curradvy = saturate_ਜੋੜੋ (curradvy, pPos->y_advance);
    }
    ਬਫਰ_ਸੈੱਟ_contents (ਬਫਰ, &contents);
  }
  ਨਹੀਂਤਾਂ
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    ਲਈ (is = gr_seg_ਪਹਿਲਾ_ਥਾਂ (seg); is; pPos++, info++, is = gr_ਥਾਂ_ਅਗਲਾ_ਵਿੱਚ_segment (is))
    {
      ਜੇ (info->cluster != currclus)
      {
	pPos->x_advance = (ਪੂਰਨ) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ਨਹੀਂਤਾਂ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_ਥਾਂ_advance_Y (is, grface, ਖਾਲੀ_ਸੂਚਕ) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_ਵਿੱਥ_2 = clamp_position ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_X (is) * xscale) -
					      (ਪੂਰਨ) info->var1 - curradvx + pPos->x_advance);
      pPos->y_ਵਿੱਥ_2 = clamp_position ((ਦੁੱਗਣਾ) (gr_ਥਾਂ_origin_Y (is) * yscale) - curradvy);
    }
    ਬਫਰ_ਸੈੱਟ_contents (ਬਫਰ, &contents);
    ਬਫਰ_ਉਲਟ_clusters (ਬਫਰ);
  }

  gr_seg_destroy (seg);
  ਮੁਕਤ (clusters);
  ਮੁਕਤ (gids);

  ਤਾਰਕਿਕ ret = glyph_ਗਿਣਤੀ;

  ਵਾਪਸ ret;
}
