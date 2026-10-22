#నిర్వచించు HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#అన్తర్భావ <hb-wasm-api.h>

#అన్తర్భావ <graphite2/Segment.h>

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

స్థిర int32_t
clamp_position (ద్విగుణ value)
{
  ఒకవేళ (value <= INT32_MIN) తిరిగి INT32_MIN;
  ఒకవేళ (value >= INT32_MAX) తిరిగి INT32_MAX;
  తిరిగి (int32_t) value;
}

స్థిర int32_t
saturate_కూడు (int32_t a, int32_t b)
{ తిరిగి clamp_position ((int64_t) a + b); }

స్థిర int32_t
saturate_sub (int32_t a, int32_t b)
{ తిరిగి clamp_position ((int64_t) a - b); }

శూన్య debugprint1 (అక్షరం *s, int32_t);
శూన్య debugprint2 (అక్షరం *s, int32_t, int32_t);

స్థిర స్థిరరాశి శూన్య *copy_పట్టిక (స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని పూర్ణాంకం ట్యాగ్_2, size_t *len)
{
  face_t *face = (face_t *) దత్తాంశం;
  blob_t blob = BLOB_INIT;
  ఒకవేళ (!face_copy_పట్టిక (face, ట్యాగ్_2, &blob))
    abort ();

  *len = blob.నిడివి;
  తిరిగి blob.దత్తాంశం;
}

స్థిర శూన్య free_పట్టిక (స్థిరరాశి శూన్య *దత్తాంశం, స్థిరరాశి శూన్య *పట్టిక_దత్తాంశం)
{
  blob_t blob;
  blob.నిడివి = 0; // Doesn't matter
  blob.దత్తాంశం = (అక్షరం *) పట్టిక_దత్తాంశం;
  blob_free (&blob);
}

శూన్య *
shape_plan_create (face_t *face)
{
  స్థిరరాశి gr_face_ops ops = {పరిమాణం (gr_face_ops), &copy_పట్టిక, &free_పట్టిక};
  gr_face *grface = gr_make_face_with_ops (face, &ops, gr_face_preloadAll);
  తిరిగి grface;
}

శూన్య
shape_plan_destroy (శూన్య *దత్తాంశం)
{
  gr_face_destroy ((gr_face *) దత్తాంశం);
}

bool_t
shape (శూన్య *shape_plan,
       font_t *font,
       బఫరు_t *బఫరు,
       స్థిరరాశి feature_t *features,
       uint32_t అంకె_features)
{
  face_t *face = font_get_face (font);
  gr_face *grface = (gr_face *) shape_plan;

  direction_t direction = బఫరు_get_direction (బఫరు);
  direction_t horiz_dir = script_get_horizontal_direction (బఫరు_get_script (బఫరు));
  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ఒకవేళ ((DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && horiz_dir != DIRECTION_INVALID) ||
      (DIRECTION_IS_VERTICAL   (direction) &&
       direction != DIRECTION_TTB))
  {
    బఫరు_తిరగేయి_clusters (బఫరు);
    direction = DIRECTION_తిరగేయి (direction);
  }

  బఫరు_contents_t contents = బఫరు_CONTENTS_INIT;
  ఒకవేళ (!బఫరు_copy_contents (బఫరు, &contents))
    తిరిగి అబద్ధం;

  gr_segment *seg = శూన్యసూచిక;
  స్థిరరాశి gr_స్థానం *is;
  గుర్తులేని పూర్ణాంకం ci = 0, ic = 0;
  int32_t curradvx = 0, curradvy = 0;
  గుర్తులేని నిడివి = contents.నిడివి;

  uint32_t *chars = (uint32_t *) జ్ఞాపకం_ఇవ్వు (నిడివి * పరిమాణం (uint32_t));
  ఒకవేళ (!chars)
    తిరిగి అబద్ధం;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < contents.నిడివి; ++i)
    chars[i] = contents.info[i].codepoint;

  seg = gr_make_seg (శూన్యసూచిక, grface,
		     0, // https://github.com/harfbuzz/harfbuzz/issues/3439#issuecomment-1442650148
		     శూన్యసూచిక,
		     gr_utf32, chars, contents.నిడివి,
		     2 | (direction == DIRECTION_RTL ? 1 : 0));

  విడుదల (chars);

  ఒకవేళ (!seg)
    తిరిగి అబద్ధం;

  గుర్తులేని పూర్ణాంకం glyph_లెక్క = gr_seg_n_slots (seg);

  నిర్మాణం cluster_t {
    గుర్తులేని పూర్ణాంకం పునాది_char;
    గుర్తులేని పూర్ణాంకం అంకె_chars;
    గుర్తులేని పూర్ణాంకం పునాది_glyph;
    గుర్తులేని పూర్ణాంకం అంకె_glyphs;
    గుర్తులేని పూర్ణాంకం cluster;
    int32_t advance;
  };

  నిడివి = glyph_లెక్క;
  ఒకవేళ (!బఫరు_contents_realloc (&contents, నిడివి))
    తిరిగి అబద్ధం;
  cluster_t *clusters = (cluster_t *) జ్ఞాపకం_ఇవ్వు (నిడివి * పరిమాణం (cluster_t));
  uint32_t *gids = (uint32_t *) జ్ఞాపకం_ఇవ్వు (నిడివి * పరిమాణం (uint32_t));
  ఒకవేళ (!clusters || !gids)
  {
    విడుదల (clusters);
    విడుదల (gids);
    తిరిగి అబద్ధం;
  }

  memset (clusters, 0, పరిమాణం (clusters[0]) * నిడివి);
  codepoint_t *pg = gids;
  clusters[0].cluster = contents.info[0].cluster;
  గుర్తులేని పూర్ణాంకం upem = face_get_upem (face);
  int32_t font_x_scale, font_y_scale;
  font_get_scale (font, &font_x_scale, &font_y_scale);
  దశాంశం xscale = (దశాంశం) font_x_scale / upem;
  దశాంశం yscale = (దశాంశం) font_y_scale / upem;
  yscale *= yscale / xscale;
  int32_t curradv = 0;
  ఒకవేళ (DIRECTION_IS_BACKWARD (direction))
  {
    curradv = clamp_position (gr_స్థానం_origin_X(gr_seg_మొదటి_స్థానం(seg)) * xscale);
    clusters[0].advance = clamp_position ((ద్విగుణ) (gr_seg_advance_X(seg) * xscale) - curradv);
  }
  లేకపోతే
    clusters[0].advance = 0;
  కొరకు (is = gr_seg_మొదటి_స్థానం (seg), ic = 0; is; is = gr_స్థానం_తదుపరి_లోపల_segment (is), ic++)
  {
    గుర్తులేని పూర్ణాంకం before = gr_స్థానం_before (is);
    గుర్తులేని పూర్ణాంకం after = gr_స్థానం_after (is);
    *pg = gr_స్థానం_gid (is);
    pg++;
    అయితే (clusters[ci].పునాది_char > before && ci)
    {
      clusters[ci-1].అంకె_chars += clusters[ci].అంకె_chars;
      clusters[ci-1].అంకె_glyphs += clusters[ci].అంకె_glyphs;
      clusters[ci-1].advance = saturate_కూడు (clusters[ci-1].advance, clusters[ci].advance);
      ci--;
    }

    ఒకవేళ (gr_స్థానం_can_చొప్పించు_before (is) && clusters[ci].అంకె_chars && before >= clusters[ci].పునాది_char + clusters[ci].అంకె_chars)
    {
      cluster_t *c = clusters + ci + 1;
      c->పునాది_char = clusters[ci].పునాది_char + clusters[ci].అంకె_chars;
      c->cluster = contents.info[c->పునాది_char].cluster;
      c->అంకె_chars = before - c->పునాది_char;
      c->పునాది_glyph = ic;
      c->అంకె_glyphs = 0;
      ఒకవేళ (DIRECTION_IS_BACKWARD (direction))
      {
	c->advance = clamp_position ((ద్విగుణ) curradv - (ద్విగుణ) (gr_స్థానం_origin_X(is) * xscale));
	curradv = saturate_sub (curradv, c->advance);
      }
      లేకపోతే
      {
	int32_t origin_X = clamp_position (gr_స్థానం_origin_X (is) * xscale);
	c->advance = 0;
	clusters[ci].advance = saturate_కూడు (clusters[ci].advance,
					     saturate_sub (origin_X, curradv));
	curradv = origin_X;
      }
      ci++;
    }
    clusters[ci].అంకె_glyphs++;

    ఒకవేళ (clusters[ci].పునాది_char + clusters[ci].అంకె_chars < after + 1)
	clusters[ci].అంకె_chars = after + 1 - clusters[ci].పునాది_char;
  }

  ఒకవేళ (DIRECTION_IS_BACKWARD (direction))
    clusters[ci].advance = saturate_కూడు (clusters[ci].advance, curradv);
  లేకపోతే
    clusters[ci].advance = clamp_position ((ద్విగుణ) clusters[ci].advance +
					   (ద్విగుణ) (gr_seg_advance_X(seg) * xscale) - curradv);
  ci++;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ci; ++i)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < clusters[i].అంకె_glyphs; ++j)
    {
      glyph_info_t *info = &contents.info[clusters[i].పునాది_glyph + j];
      info->codepoint = gids[clusters[i].పునాది_glyph + j];
      info->cluster = clusters[i].cluster;
      info->var1 = (గుర్తులేని) clusters[i].advance;     // all glyphs in the cluster get the same advance
    }
  }
  contents.నిడివి = glyph_లెక్క;

  /* Positioning. */
  గుర్తులేని పూర్ణాంకం currclus = 0xFFFFFFFF;
  స్థిరరాశి glyph_info_t *info = contents.info;
  glyph_position_t *pPos = contents.pos;
  ఒకవేళ (!DIRECTION_IS_BACKWARD (direction))
  {
    curradvx = 0;
    కొరకు (is = gr_seg_మొదటి_స్థానం (seg); is; pPos++, ++info, is = gr_స్థానం_తదుపరి_లోపల_segment (is))
    {
      pPos->x_స్థానభ్రంశం_2 = clamp_position ((ద్విగుణ) (gr_స్థానం_origin_X (is) * xscale) - curradvx);
      pPos->y_స్థానభ్రంశం_2 = clamp_position ((ద్విగుణ) (gr_స్థానం_origin_Y (is) * yscale) - curradvy);
      ఒకవేళ (info->cluster != currclus) {
	pPos->x_advance = (పూర్ణాంకం) info->var1;
	curradvx = saturate_కూడు (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } లేకపోతే
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_స్థానం_advance_Y (is, grface, శూన్యసూచిక) * yscale);
      curradvy = saturate_కూడు (curradvy, pPos->y_advance);
    }
    బఫరు_సమితి_contents (బఫరు, &contents);
  }
  లేకపోతే
  {
    curradvx = clamp_position (gr_seg_advance_X(seg) * xscale);
    కొరకు (is = gr_seg_మొదటి_స్థానం (seg); is; pPos++, info++, is = gr_స్థానం_తదుపరి_లోపల_segment (is))
    {
      ఒకవేళ (info->cluster != currclus)
      {
	pPos->x_advance = (పూర్ణాంకం) info->var1;
	curradvx = saturate_sub (curradvx, pPos->x_advance);
	currclus = info->cluster;
      } లేకపోతే
	pPos->x_advance = 0.;

      pPos->y_advance = clamp_position (gr_స్థానం_advance_Y (is, grface, శూన్యసూచిక) * yscale);
      curradvy = saturate_sub (curradvy, pPos->y_advance);
      pPos->x_స్థానభ్రంశం_2 = clamp_position ((ద్విగుణ) (gr_స్థానం_origin_X (is) * xscale) -
					      (పూర్ణాంకం) info->var1 - curradvx + pPos->x_advance);
      pPos->y_స్థానభ్రంశం_2 = clamp_position ((ద్విగుణ) (gr_స్థానం_origin_Y (is) * yscale) - curradvy);
    }
    బఫరు_సమితి_contents (బఫరు, &contents);
    బఫరు_తిరగేయి_clusters (బఫరు);
  }

  gr_seg_destroy (seg);
  విడుదల (clusters);
  విడుదల (gids);

  తార్కిక ret = glyph_లెక్క;

  తిరిగి ret;
}
