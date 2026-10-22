#وصف_ڪريو HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

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
saturate_جوڙيو (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a + b); }

ساكن int32_t
saturate_sub (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a - b); }

فراغ debugprint1 (حرف *s, int32_t);
فراغ debugprint2 (حرف *s, int32_t, int32_t);

ساكن ثابت فراغ *copy_جدول (ثابت فراغ *معلومات, بنانشان صحيح ٽيگ_2, size_t *len)
{
  face_t *face = (face_t *) معلومات;
  blob_t blob = BLOB_INIT;
  إذا (!face_copy_جدول (face, ٽيگ_2, &blob))
    abort ();

  *len = blob.ڊيگهائي;
  أرجع blob.معلومات;
}

ساكن فراغ free_جدول (ثابت فراغ *معلومات, ثابت فراغ *جدول_معلومات)
{
  blob_t blob;
  blob.ڊيگهائي = 0; // Doesn't matter
  blob.معلومات = (حرف *) جدول_معلومات;
  blob_free (&blob);
}

فراغ *
shape_plan_create (face_t *face)
{
  ثابت gr_face_ops ops = {حجم (gr_face_ops), &copy_جدول, &free_جدول};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  أرجع grface;
}

فراغ
shape_plan_destroy (فراغ *معلومات)
{
  gr_face_destroy ((gr_face *) معلومات);
}

bool_t
shape (فراغ *shape_plan,
       font_t *font,
       بفر_t *بفر,
       ثابت feature_t *features,
       uint32_t انگ_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = بفر_get_direction (بفر);
  direction_t horiz_dir = script_get_horizontal_direction (بفر_get_script (بفر));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    بفر_ابتو_clusters (بفر);
    direction = DIRECTION_ابتو (direction);
  }

  بفر_contents_t contents = بفر_CONTENTS_INIT;
  إذا (!بفر_copy_contents (بفر, &contents))
    أرجع خطأ;

  gr_segment *seg = خالي_اشارو;
  ثابت gr_جڳهه *is;
  بنانشان صحيح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  بنانشان ڊيگهائي = contents.ڊيگهائي;

  uint32_t *chars = (uint32_t *) يادگيري_ڏيو (ڊيگهائي * حجم (uint32_t));
  إذا (!chars)
    أرجع خطأ;
  لكل (بنانشان صحيح i = 0; i < contents.ڊيگهائي; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (خالي_اشارو, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     خالي_اشارو,
		     gr_utf32, chars, contents.ڊيگهائي,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  حرر (chars);

  إذا (!seg)
    أرجع خطأ;

  بنانشان صحيح glyph_ڳڻپ = gr_seg_n_slots (seg);

  بنية cluster_t {
    بنانشان صحيح بنياد_char;
    بنانشان صحيح انگ_chars;
    بنانشان صحيح بنياد_glyph;
    بنانشان صحيح انگ_glyphs;
    بنانشان صحيح cluster;
    int32_t advance;
  };

  ڊيگهائي = glyph_ڳڻپ;
  إذا (!بفر_contents_realloc (&contents, ڊيگهائي))
    أرجع خطأ;
  cluster_t *clusters = (cluster_t *) يادگيري_ڏيو (ڊيگهائي * حجم (cluster_t));
  uint32_t *gids = (uint32_t *) يادگيري_ڏيو (ڊيگهائي * حجم (uint32_t));
  إذا (!clusters || !gids)
  {
    حرر (clusters);
    حرر (gids);
    أرجع خطأ;
  }

  memset (clusters, 0, حجم (clusters[0]) * ڊيگهائي);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  بنانشان صحيح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  عائم xscale = (عائم) font_x_scale / upem;
  عائم yscale = (عائم) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  إذا (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_جڳهه_origin_X(gr_seg_پهريون_جڳهه(seg)) * xscale);
    clusters[0].advance = clamp_position ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_پهريون_جڳهه (seg), ic = 0; is; is = gr_جڳهه_ايندڙ_اندريون_segment (is), ic++)
  {
    بنانشان صحيح before = gr_جڳهه_before (is);
    بنانشان صحيح after = gr_جڳهه_after (is);
    *pg = gr_جڳهه_gid (is);
    pg++;
    طالما (clusters[ci].بنياد_char > before && ci)
    {
      clusters[ci-1].انگ_chars += clusters[ci].انگ_chars;
      clusters[ci-1].انگ_glyphs += clusters[ci].انگ_glyphs;
      clusters[ci-1].advance = saturate_جوڙيو (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_جڳهه_can_وجهو_before (is) && clusters[ci].انگ_chars && before >= clusters[ci].بنياد_char + clusters[ci].انگ_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->بنياد_char = clusters[ci].بنياد_char + clusters[ci].انگ_chars;
      c->cluster = contents.info[c->بنياد_char].cluster;
      c->انگ_chars = before - c->بنياد_char;
      c->بنياد_glyph = ic;
      c->انگ_glyphs = 0;
      إذا (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((مزدوج) curradv - (مزدوج) (gr_جڳهه_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	int32_t origin_X = clamp_position (gr_جڳهه_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_جوڙيو (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].انگ_glyphs++;

    إذا (clusters[ci].بنياد_char + clusters[ci].انگ_chars < after + 1)
	clusters[ci].انگ_chars = after + 1 - clusters[ci].بنياد_char;
  }

  إذا (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_جوڙيو (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = clamp_position ((مزدوج) clusters[ci].advance +
					   (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (بنانشان صحيح i = 0; i < ci; ++i)
  {
    لكل (بنانشان صحيح j = 0; j < clusters[i].انگ_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].بنياد_glyph + j];
      info->codepoint = gids[clusters[i].بنياد_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (بنانشان) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.ڊيگهائي = glyph_ڳڻپ;

  /* Positioning. */
  بنانشان صحيح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  إذا (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_پهريون_جڳهه (seg); is; pPos++, ++info, is = gr_جڳهه_ايندڙ_اندريون_segment (is))
    {
      pPos->x_وٿي_2 = clamp_position ((مزدوج) (gr_جڳهه_origin_X (is) * xscale) - curradvx);
      pPos->y_وٿي_2 = clamp_position ((مزدوج) (gr_جڳهه_origin_Y (is) * yscale) - curradvy);
      إذا (info->cluster != currclus) {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_جوڙيو (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_جڳهه_advance_Y (is, grface, خالي_اشارو) * yscale);
      curradvy = saturate_جوڙيو (curradvy, pPos->y_advance);
    }
    بفر_سيٽ_contents (بفر, &contents);
  }
  وإلا
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_پهريون_جڳهه (seg); is; pPos++, info++, is = gr_جڳهه_ايندڙ_اندريون_segment (is))
    {
      إذا (info->cluster != currclus)
      {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_جڳهه_advance_Y (is, grface, خالي_اشارو) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_وٿي_2 = clamp_position ((مزدوج) (gr_جڳهه_origin_X (is) * xscale) -
					      (صحيح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_وٿي_2 = clamp_position ((مزدوج) (gr_جڳهه_origin_Y (is) * yscale) - curradvy);
    }
    بفر_سيٽ_contents (بفر, &contents);
    بفر_ابتو_clusters (بفر);
  }

  gr_seg_destroy (seg);
  حرر (clusters);
  حرر (gids);

  منطقي ret = glyph_ڳڻپ;

  أرجع ret;
}
