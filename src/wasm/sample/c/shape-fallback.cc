#تعریف_کرو HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#شامل <hb-wasm-api.h>

خارجی "C" {
خالی debugprint (ثابت حرف *s);
خالی debugprint1 (ثابت حرف *s, int32_t);
خالی debugprint2 (ثابت حرف *s, int32_t, int32_t);
}

bool_t
shape (خالی *shape_plan,
       font_t *font,
       بفر_t *بفر,
       ثابت feature_t *features,
       uint32_t عدد_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_جدول (face, لیبل_3 ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.لمبائی);

  blob_free (&blob);

  بفر_contents_t contents = بفر_CONTENTS_INIT;
  اگر (!بفر_copy_contents (بفر, &contents))
    واپس جھوٹ;

  debugprint1 ("buffer length", contents.لمبائی);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  برائے (غیرعلامتی i = 0; i < contents.لمبائی; i++)
  {
    حرف name[64];

    debugprint1 ("glyph at", i);

    font_glyph_مقصد_string (font, contents.info[i].codepoint, name, حجم (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = بفر_سیٹ_contents (بفر, &contents);

  بفر_contents_free (&contents);

  واپس ret;
}
