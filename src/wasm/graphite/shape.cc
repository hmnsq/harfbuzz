#تعریف_کړه HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

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
saturate_جمع_کړه (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a + b); }

ساكن int32_t
saturate_sub (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a - b); }

فراغ debugprint1 (حرف *s, int32_t);
فراغ debugprint2 (حرف *s, int32_t, int32_t);

ساكن ثابت فراغ *copy_جدول (ثابت فراغ *معلومات, بینښه صحيح نښان_2, size_t *len)
{
  face_t *face = (face_t *) معلومات;
  blob_t blob = BLOB_INIT;
  إذا (!face_copy_جدول (face, نښان_2, &blob))
    abort ();

  *len = blob.اوږدتیا;
  أرجع blob.معلومات;
}

ساكن فراغ free_جدول (ثابت فراغ *معلومات, ثابت فراغ *جدول_معلومات)
{
  blob_t blob;
  blob.اوږدتیا = 0; // Doesn't matter
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
  إذا ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    بفر_سرچپه_clusters (بفر);
    direction = DIRECTION_سرچپه (direction);
  }

  بفر_contents_t contents = بفر_CONTENTS_INIT;
  إذا (!بفر_copy_contents (بفر, &contents))
    أرجع خطأ;

  gr_segment *seg = تش_اشاره;
  ثابت gr_خونه *is;
  بینښه صحيح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  بینښه اوږدتیا = contents.اوږدتیا;

  uint32_t *chars = (uint32_t *) حافظه_ورکړه (اوږدتیا * حجم (uint32_t));
  إذا (!chars)
    أرجع خطأ;
  لكل (بینښه صحيح i = 0; i < contents.اوږدتیا; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (تش_اشاره, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     تش_اشاره,
		     gr_utf32, chars, contents.اوږدتیا,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  حرر (chars);

  إذا (!seg)
    أرجع خطأ;

  بینښه صحيح glyph_شمېر = gr_seg_n_slots (seg);

  بنية cluster_t {
    بینښه صحيح بنسټ_char;
    بینښه صحيح عدد_chars;
    بینښه صحيح بنسټ_glyph;
    بینښه صحيح عدد_glyphs;
    بینښه صحيح cluster;
    int32_t advance;
  };

  اوږدتیا = glyph_شمېر;
  إذا (!بفر_contents_realloc (&contents, اوږدتیا))
    أرجع خطأ;
  cluster_t *clusters = (cluster_t *) حافظه_ورکړه (اوږدتیا * حجم (cluster_t));
  uint32_t *gids = (uint32_t *) حافظه_ورکړه (اوږدتیا * حجم (uint32_t));
  إذا (!clusters || !gids)
  {
    حرر (clusters);
    حرر (gids);
    أرجع خطأ;
  }

  memset (clusters, 0, حجم (clusters[0]) * اوږدتیا);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  بینښه صحيح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  عائم xscale = (عائم) font_x_scale / upem;
  عائم yscale = (عائم) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  إذا (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_خونه_origin_X(gr_seg_لومړی_خونه(seg)) * xscale);
    clusters[0].advance = clamp_position ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_لومړی_خونه (seg), ic = 0; is; is = gr_خونه_بل_دننه_segment (is), ic++)
  {
    بینښه صحيح before = gr_خونه_before (is);
    بینښه صحيح after = gr_خونه_after (is);
    *pg = gr_خونه_gid (is);
    pg++;
    طالما (clusters[ci].بنسټ_char > before && ci)
    {
      clusters[ci-1].عدد_chars += clusters[ci].عدد_chars;
      clusters[ci-1].عدد_glyphs += clusters[ci].عدد_glyphs;
      clusters[ci-1].advance = saturate_جمع_کړه (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_خونه_can_ورننباسه_before (is) && clusters[ci].عدد_chars && before >= clusters[ci].بنسټ_char + clusters[ci].عدد_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->بنسټ_char = clusters[ci].بنسټ_char + clusters[ci].عدد_chars;
      c->cluster = contents.info[c->بنسټ_char].cluster;
      c->عدد_chars = before - c->بنسټ_char;
      c->بنسټ_glyph = ic;
      c->عدد_glyphs = 0;
      إذا (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((مزدوج) curradv - (مزدوج) (gr_خونه_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	int32_t origin_X = clamp_position (gr_خونه_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_جمع_کړه (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].عدد_glyphs++;

    إذا (clusters[ci].بنسټ_char + clusters[ci].عدد_chars < after + 1)
	clusters[ci].عدد_chars = after + 1 - clusters[ci].بنسټ_char;
  }

  إذا (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_جمع_کړه (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = clamp_position ((مزدوج) clusters[ci].advance +
					   (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (بینښه صحيح i = 0; i < ci; ++i)
  {
    لكل (بینښه صحيح j = 0; j < clusters[i].عدد_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].بنسټ_glyph + j];
      info->codepoint = gids[clusters[i].بنسټ_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (بینښه) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.اوږدتیا = glyph_شمېر;

  /* Positioning. */
  بینښه صحيح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  إذا (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_لومړی_خونه (seg); is; pPos++, ++info, is = gr_خونه_بل_دننه_segment (is))
    {
      pPos->x_دننه_واټن_2 = clamp_position ((مزدوج) (gr_خونه_origin_X (is) * xscale) - curradvx);
      pPos->y_دننه_واټن_2 = clamp_position ((مزدوج) (gr_خونه_origin_Y (is) * yscale) - curradvy);
      إذا (info->cluster != currclus) {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_جمع_کړه (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_خونه_advance_Y (is, grface, تش_اشاره) * yscale);
      curradvy = saturate_جمع_کړه (curradvy, pPos->y_advance);
    }
    بفر_سېټ_contents (بفر, &contents);
  }
  وإلا
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_لومړی_خونه (seg); is; pPos++, info++, is = gr_خونه_بل_دننه_segment (is))
    {
      إذا (info->cluster != currclus)
      {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_خونه_advance_Y (is, grface, تش_اشاره) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_دننه_واټن_2 = clamp_position ((مزدوج) (gr_خونه_origin_X (is) * xscale) -
					      (صحيح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_دننه_واټن_2 = clamp_position ((مزدوج) (gr_خونه_origin_Y (is) * yscale) - curradvy);
    }
    بفر_سېټ_contents (بفر, &contents);
    بفر_سرچپه_clusters (بفر);
  }

  gr_seg_destroy (seg);
  حرر (clusters);
  حرر (gids);

  منطقي ret = glyph_شمېر;

  أرجع ret;
}
