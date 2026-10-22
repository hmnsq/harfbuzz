#تعریف_کن HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

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
saturate_جمع_کن (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a + b); }

ساكن int32_t
saturate_sub (int32_t a, int32_t b)
{ أرجع clamp_position ((int64_t) a - b); }

فراغ debugprint1 (حرف *s, int32_t);
فراغ debugprint2 (حرف *s, int32_t, int32_t);

ساكن ثابت فراغ *copy_جدول (ثابت فراغ *داده, بیعلامت صحيح برچسب_2, size_t *len)
{
  face_t *face = (face_t *) داده;
  blob_t blob = BLOB_INIT;
  إذا (!face_copy_جدول (face, برچسب_2, &blob))
    abort ();

  *len = blob.درازی;
  أرجع blob.داده;
}

ساكن فراغ free_جدول (ثابت فراغ *داده, ثابت فراغ *جدول_داده)
{
  blob_t blob;
  blob.درازی = 0; // Doesn't matter
  blob.داده = (حرف *) جدول_داده;
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
shape_plan_destroy (فراغ *داده)
{
  gr_face_destroy ((gr_face *) داده);
}

bool_t
shape (فراغ *shape_plan,
       font_t *font,
       میانگیر_t *میانگیر,
       ثابت feature_t *features,
       uint32_t رقم_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = میانگیر_get_direction (میانگیر);
  direction_t horiz_dir = script_get_horizontal_direction (میانگیر_get_script (میانگیر));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    میانگیر_معکوس_clusters (میانگیر);
    direction = DIRECTION_معکوس (direction);
  }

  میانگیر_contents_t contents = میانگیر_CONTENTS_INIT;
  إذا (!میانگیر_copy_contents (میانگیر, &contents))
    أرجع خطأ;

  gr_segment *seg = اشاره_تهی;
  ثابت gr_جایگاه *is;
  بیعلامت صحيح ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  بیعلامت درازی = contents.درازی;

  uint32_t *chars = (uint32_t *) حافظه_بده (درازی * حجم (uint32_t));
  إذا (!chars)
    أرجع خطأ;
  لكل (بیعلامت صحيح i = 0; i < contents.درازی; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (اشاره_تهی, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     اشاره_تهی,
		     gr_utf32, chars, contents.درازی,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  حرر (chars);

  إذا (!seg)
    أرجع خطأ;

  بیعلامت صحيح glyph_شمار = gr_seg_n_slots (seg);

  بنية cluster_t {
    بیعلامت صحيح پایه_char;
    بیعلامت صحيح رقم_chars;
    بیعلامت صحيح پایه_glyph;
    بیعلامت صحيح رقم_glyphs;
    بیعلامت صحيح cluster;
    int32_t advance;
  };

  درازی = glyph_شمار;
  إذا (!میانگیر_contents_realloc (&contents, درازی))
    أرجع خطأ;
  cluster_t *clusters = (cluster_t *) حافظه_بده (درازی * حجم (cluster_t));
  uint32_t *gids = (uint32_t *) حافظه_بده (درازی * حجم (uint32_t));
  إذا (!clusters || !gids)
  {
    حرر (clusters);
    حرر (gids);
    أرجع خطأ;
  }

  memset (clusters, 0, حجم (clusters[0]) * درازی);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  بیعلامت صحيح upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  عائم xscale = (عائم) font_x_scale / upem;
  عائم yscale = (عائم) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  إذا (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_جایگاه_origin_X(gr_seg_اولی_جایگاه(seg)) * xscale);
    clusters[0].advance = clamp_position ((مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  وإلا
    clusters[0].advance = 0;
  لكل (is = gr_seg_اولی_جایگاه (seg), ic = 0; is; is = gr_جایگاه_بعدی_درون_segment (is), ic++)
  {
    بیعلامت صحيح before = gr_جایگاه_before (is);
    بیعلامت صحيح after = gr_جایگاه_after (is);
    *pg = gr_جایگاه_gid (is);
    pg++;
    طالما (clusters[ci].پایه_char > before && ci)
    {
      clusters[ci-1].رقم_chars += clusters[ci].رقم_chars;
      clusters[ci-1].رقم_glyphs += clusters[ci].رقم_glyphs;
      clusters[ci-1].advance = saturate_جمع_کن (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    إذا (gr_جایگاه_can_درج_before (is) && clusters[ci].رقم_chars && before >= clusters[ci].پایه_char + clusters[ci].رقم_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->پایه_char = clusters[ci].پایه_char + clusters[ci].رقم_chars;
      c->cluster = contents.info[c->پایه_char].cluster;
      c->رقم_chars = before - c->پایه_char;
      c->پایه_glyph = ic;
      c->رقم_glyphs = 0;
      إذا (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((مزدوج) curradv - (مزدوج) (gr_جایگاه_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      وإلا
      {
	int32_t origin_X = clamp_position (gr_جایگاه_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_جمع_کن (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].رقم_glyphs++;

    إذا (clusters[ci].پایه_char + clusters[ci].رقم_chars < after + 1)
	clusters[ci].رقم_chars = after + 1 - clusters[ci].پایه_char;
  }

  إذا (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_جمع_کن (clusters[ci].advance, curradv);
  وإلا
    clusters[ci].advance = clamp_position ((مزدوج) clusters[ci].advance +
					   (مزدوج) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  لكل (بیعلامت صحيح i = 0; i < ci; ++i)
  {
    لكل (بیعلامت صحيح j = 0; j < clusters[i].رقم_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].پایه_glyph + j];
      info->codepoint = gids[clusters[i].پایه_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (بیعلامت) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.درازی = glyph_شمار;

  /* Positioning. */
  بیعلامت صحيح currclus = 0xFFFFFFFF;
  ثابت glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  إذا (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    لكل (is = gr_seg_اولی_جایگاه (seg); is; pPos++, ++info, is = gr_جایگاه_بعدی_درون_segment (is))
    {
      pPos->x_جابه_جایی_2 = clamp_position ((مزدوج) (gr_جایگاه_origin_X (is) * xscale) - curradvx);
      pPos->y_جابه_جایی_2 = clamp_position ((مزدوج) (gr_جایگاه_origin_Y (is) * yscale) - curradvy);
      إذا (info->cluster != currclus) {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_جمع_کن (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_جایگاه_advance_Y (is, grface, اشاره_تهی) * yscale);
      curradvy = saturate_جمع_کن (curradvy, pPos->y_advance);
    }
    میانگیر_مجموعه_contents (میانگیر, &contents);
  }
  وإلا
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    لكل (is = gr_seg_اولی_جایگاه (seg); is; pPos++, info++, is = gr_جایگاه_بعدی_درون_segment (is))
    {
      إذا (info->cluster != currclus)
      {
	pPos->x_advance = (صحيح) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } وإلا
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_جایگاه_advance_Y (is, grface, اشاره_تهی) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_جابه_جایی_2 = clamp_position ((مزدوج) (gr_جایگاه_origin_X (is) * xscale) -
					      (صحيح) info->var1 - curradvx + pPos->x_advance);
      pPos->y_جابه_جایی_2 = clamp_position ((مزدوج) (gr_جایگاه_origin_Y (is) * yscale) - curradvy);
    }
    میانگیر_مجموعه_contents (میانگیر, &contents);
    میانگیر_معکوس_clusters (میانگیر);
  }

  gr_seg_destroy (seg);
  حرر (clusters);
  حرر (gids);

  منطقي ret = glyph_شمار;

  أرجع ret;
}
