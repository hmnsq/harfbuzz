#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#அந்தர்பாவ <hb-wasm-api.h>

வெளி "C" {
வெறுமை debugprint (மாறிலி எழுத்து *s);
வெறுமை debugprint1 (மாறிலி எழுத்து *s, int32_t);
வெறுமை debugprint2 (மாறிலி எழுத்து *s, int32_t, int32_t);
}

bool_t
shape (வெறுமை *shape_plan,
       font_t *font,
       buffer_t *buffer,
       மாறிலி feature_t *features,
       uint32_t இலக்கம்_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_table (face, TAG ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.length);

  blob_free (&blob);

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  எனில் (!buffer_copy_contents (buffer, &contents))
    திரும்பு பொய்;

  debugprint1 ("buffer length", contents.length);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  க்கு (unsigned i = 0; i < contents.length; i++)
  {
    எழுத்து name[64];

    debugprint1 ("glyph at", i);

    font_glyph_to_string (font, contents.info[i].codepoint, name, அளவு (name));

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

  திரும்பு ret;
}
