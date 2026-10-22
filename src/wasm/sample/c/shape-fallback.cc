#నిర్వచించు HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#అన్తర్భావ <hb-wasm-api.h>

బాహ్య "C" {
శూన్య debugprint (స్థిరరాశి అక్షరం *s);
శూన్య debugprint1 (స్థిరరాశి అక్షరం *s, int32_t);
శూన్య debugprint2 (స్థిరరాశి అక్షరం *s, int32_t, int32_t);
}

bool_t
shape (శూన్య *shape_plan,
       font_t *font,
       బఫరు_t *బఫరు,
       స్థిరరాశి feature_t *features,
       uint32_t అంకె_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_పట్టిక (face, ట్యాగ్_3 ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.నిడివి);

  blob_free (&blob);

  బఫరు_contents_t contents = బఫరు_CONTENTS_INIT;
  ఒకవేళ (!బఫరు_copy_contents (బఫరు, &contents))
    తిరిగి అబద్ధం;

  debugprint1 ("buffer length", contents.నిడివి);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  కొరకు (గుర్తులేని i = 0; i < contents.నిడివి; i++)
  {
    అక్షరం name[64];

    debugprint1 ("glyph at", i);

    font_glyph_గమ్యం_string (font, contents.info[i].codepoint, name, పరిమాణం (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = బఫరు_సమితి_contents (బఫరు, &contents);

  బఫరు_contents_free (&contents);

  తిరిగి ret;
}
