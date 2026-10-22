#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#അന്തര്ഭാവ <hb-wasm-api.h>

ബാഹ്യ "C" {
ശൂന്യം debugprint (സ്ഥിരം അക്ഷരം *s);
ശൂന്യം debugprint1 (സ്ഥിരം അക്ഷരം *s, int32_t);
ശൂന്യം debugprint2 (സ്ഥിരം അക്ഷരം *s, int32_t, int32_t);
}

bool_t
shape (ശൂന്യം *shape_plan,
       font_t *font,
       buffer_t *buffer,
       സ്ഥിരം feature_t *features,
       uint32_t അക്കം_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_table (face, TAG ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.length);

  blob_free (&blob);

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  എങ്കിൽ (!buffer_copy_contents (buffer, &contents))
    തിരികെ അസത്യം;

  debugprint1 ("buffer length", contents.length);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  വേണ്ടി (unsigned i = 0; i < contents.length; i++)
  {
    അക്ഷരം name[64];

    debugprint1 ("glyph at", i);

    font_glyph_to_string (font, contents.info[i].codepoint, name, വലുപ്പം (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = buffer_set_contents (buffer, &contents);

  buffer_contents_free (&contents);

  തിരികെ ret;
}
