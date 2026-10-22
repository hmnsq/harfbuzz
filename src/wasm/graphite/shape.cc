#تعریف_کرو HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#شامل <hb-wasm-api.h>

#شامل <graphite2/Segment.h>

#شامل <stdlib.h>
#شامل <string.h>

ساکن int32_t
clamp_position (دوہرا value)
{
  اگر (value <= INT32_MIN) واپس INT32_MIN;
  اگر (value >= INT32_MAX) واپس INT32_MAX;
  واپس (int32_t) value;
}

ساکن int32_t
saturate_جوڑو (int32_t a, int32_t b)
{ واپس clamp_position ((int64_t) a + b); }

ساکن int32_t
saturate_sub (int32_t a, int32_t b)
{ واپس clamp_position ((int64_t) a - b); }

خالی debugprint1 (حرف *s, int32_t);
خالی debugprint2 (حرف *s, int32_t, int32_t);

ساکن ثابت خالی *copy_جدول (ثابت خالی *ڈیٹا, غیرعلامتی صحیح لیبل_2, size_t *len)
{
  face_t *face = (face_t *) ڈیٹا;
  blob_t blob = BLOB_INIT;
  اگر (!face_copy_جدول (face, لیبل_2, &blob))
    abort ();

  *len = blob.لمبائی;
  واپس blob.ڈیٹا;
}

ساکن خالی free_جدول (ثابت خالی *ڈیٹا, ثابت خالی *جدول_ڈیٹا)
{
  blob_t blob;
  blob.لمبائی = 0; // Doesn't matter
  blob.ڈیٹا = (حرف *) جدول_ڈیٹا;
  blob_free (&blob);
}

خالی *
shape_plan_create (face_t *face)
{
  ثابت gr_face_ops ops = {حجم (gr_face_ops), &copy_جدول, &free_جدول};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  واپس grface;
}

خالی
shape_plan_destroy (خالی *ڈیٹا)
{
  gr_face_destroy ((gr_face *) ڈیٹا);
}

bool_t
shape (خالی *shape_plan,
       font_t *font,
       بفر_t *بفر,
       ثابت feature_t *features,
       uint32_t عدد_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = بفر_get_direction (بفر);
  direction_t horiz_dir = script_get_horizontal_direction (بفر_get_script (بفر));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  اگر ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    بفر_الٹ_clusters (بفر);
    direction = DIRECTION_الٹ (direction);
  }

  بفر_contents_t contents = بفر_CONTENTS_INIT;
  اگر (!بفر_copy_contents (بفر, &contents))
    واپس جھوٹ;

  gr_segment *seg = خالی_اشاریہ;
  ثابت gr_خانہ *is;
  غیرعلامتی صحیح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  غیرعلامتی لمبائی = contents.لمبائی;

  uint32_t *chars = (uint32_t *) حافظہ_دو (لمبائی * حجم (uint32_t));
  اگر (!chars)
    واپس جھوٹ;
  برائے (غیرعلامتی صحیح i = 0; i < contents.لمبائی; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (خالی_اشاریہ, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     خالی_اشاریہ,
		     gr_utf32, chars, contents.لمبائی,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  آزاد (chars);

  اگر (!seg)
    واپس جھوٹ;

  غیرعلامتی صحیح glyph_گنتی = gr_seg_n_slots (seg);

  ساخت cluster_t {
    غیرعلامتی صحیح بنیاد_char;
    غیرعلامتی صحیح عدد_chars;
    غیرعلامتی صحیح بنیاد_glyph;
    غیرعلامتی صحیح عدد_glyphs;
    غیرعلامتی صحیح cluster;
    int32_t advance;
  };

  لمبائی = glyph_گنتی;
  اگر (!بفر_contents_realloc (&contents, لمبائی))
    واپس جھوٹ;
  cluster_t *clusters = (cluster_t *) حافظہ_دو (لمبائی * حجم (cluster_t));
  uint32_t *gids = (uint32_t *) حافظہ_دو (لمبائی * حجم (uint32_t));
  اگر (!clusters || !gids)
  {
    آزاد (clusters);
    آزاد (gids);
    واپس جھوٹ;
  }

  memset (clusters, 0, حجم (clusters[0]) * لمبائی);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  غیرعلامتی صحیح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  اعشاری xscale = (اعشاری) font_x_scale / upem;
  اعشاری yscale = (اعشاری) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  اگر (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_خانہ_origin_X(gr_seg_پہلا_خانہ(seg)) * xscale);
    clusters[0].advance = clamp_position ((دوہرا) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  ورنہ
    clusters[0].advance = 0;
  برائے (is = gr_seg_پہلا_خانہ (seg), ic = 0; is; is = gr_خانہ_اگلا_اندر_segment (is), ic++)
  {
    غیرعلامتی صحیح before = gr_خانہ_before (is);
    غیرعلامتی صحیح after = gr_خانہ_after (is);
    *pg = gr_خانہ_gid (is);
    pg++;
    جب (clusters[ci].بنیاد_char > before && ci)
    {
      clusters[ci-1].عدد_chars += clusters[ci].عدد_chars;
      clusters[ci-1].عدد_glyphs += clusters[ci].عدد_glyphs;
      clusters[ci-1].advance = saturate_جوڑو (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    اگر (gr_خانہ_can_داخل_کرو_before (is) && clusters[ci].عدد_chars && before >= clusters[ci].بنیاد_char + clusters[ci].عدد_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->بنیاد_char = clusters[ci].بنیاد_char + clusters[ci].عدد_chars;
      c->cluster = contents.info[c->بنیاد_char].cluster;
      c->عدد_chars = before - c->بنیاد_char;
      c->بنیاد_glyph = ic;
      c->عدد_glyphs = 0;
      اگر (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((دوہرا) curradv - (دوہرا) (gr_خانہ_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      ورنہ
      {
	int32_t origin_X = clamp_position (gr_خانہ_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_جوڑو (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].عدد_glyphs++;

    اگر (clusters[ci].بنیاد_char + clusters[ci].عدد_chars < after + 1)
	clusters[ci].عدد_chars = after + 1 - clusters[ci].بنیاد_char;
  }

  اگر (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_جوڑو (clusters[ci].advance, curradv);
  ورنہ
    clusters[ci].advance = clamp_position ((دوہرا) clusters[ci].advance +
					   (دوہرا) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  برائے (غیرعلامتی صحیح i = 0; i < ci; ++i)
  {
    برائے (غیرعلامتی صحیح j = 0; j < clusters[i].عدد_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].بنیاد_glyph + j];
      info->codepoint = gids[clusters[i].بنیاد_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (غیرعلامتی) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.لمبائی = glyph_گنتی;

  /* Positioning. */
  غیرعلامتی صحیح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  اگر (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    برائے (is = gr_seg_پہلا_خانہ (seg); is; pPos++, ++info, is = gr_خانہ_اگلا_اندر_segment (is))
    {
      pPos->x_انحراف_2 = clamp_position ((دوہرا) (gr_خانہ_origin_X (is) * xscale) - curradvx);
      pPos->y_انحراف_2 = clamp_position ((دوہرا) (gr_خانہ_origin_Y (is) * yscale) - curradvy);
      اگر (info->cluster != currclus) {
	pPos->x_advance = (صحیح) info->var1;
	curradvx = saturate_جوڑو (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ورنہ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_خانہ_advance_Y (is, grface, خالی_اشاریہ) * yscale);
      curradvy = saturate_جوڑو (curradvy, pPos->y_advance);
    }
    بفر_سیٹ_contents (بفر, &contents);
  }
  ورنہ
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    برائے (is = gr_seg_پہلا_خانہ (seg); is; pPos++, info++, is = gr_خانہ_اگلا_اندر_segment (is))
    {
      اگر (info->cluster != currclus)
      {
	pPos->x_advance = (صحیح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } ورنہ
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_خانہ_advance_Y (is, grface, خالی_اشاریہ) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_انحراف_2 = clamp_position ((دوہرا) (gr_خانہ_origin_X (is) * xscale) -
					      (صحیح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_انحراف_2 = clamp_position ((دوہرا) (gr_خانہ_origin_Y (is) * yscale) - curradvy);
    }
    بفر_سیٹ_contents (بفر, &contents);
    بفر_الٹ_clusters (بفر);
  }

  gr_seg_destroy (seg);
  آزاد (clusters);
  آزاد (gids);

  منطقی ret = glyph_گنتی;

  واپس ret;
}
