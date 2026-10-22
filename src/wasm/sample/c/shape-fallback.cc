#घोषणा HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#समावेश <hb-wasm-api.h>

बाह्य "C" {
व्योम debugprint (स्थिर अक्षर *s);
व्योम debugprint1 (स्थिर अक्षर *s, int32_t);
व्योम debugprint2 (स्थिर अक्षर *s, int32_t, int32_t);
}

bool_t
shape (व्योम *shape_plan,
       font_t *font,
       बफर_t *बफर,
       स्थिर feature_t *features,
       uint32_t अंक_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_table (face, TAG ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.लंबाई);

  blob_free (&blob);

  बफर_contents_t contents = बफर_CONTENTS_INIT;
  अगर (!बफर_copy_contents (बफर, &contents))
    वापस false;

  debugprint1 ("buffer length", contents.लंबाई);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  क्रम (अचिन्हित i = 0; i < contents.लंबाई; i++)
  {
    अक्षर name[64];

    debugprint1 ("glyph at", i);

    font_glyph_to_string (font, contents.info[i].codepoint, name, माप (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = बफर_set_contents (बफर, &contents);

  बफर_contents_free (&contents);

  वापस ret;
}
