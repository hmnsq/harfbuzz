#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাব <hb-wasm-api.h>

বাহ্য "C" {
শূন্য debugprint (ধ্রুব বর্ণ *s);
শূন্য debugprint1 (ধ্রুব বর্ণ *s, int32_t);
শূন্য debugprint2 (ধ্রুব বর্ণ *s, int32_t, int32_t);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ধ্রুব feature_t *features,
       uint32_t অঙ্ক_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_table (face, TAG ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.length);

  blob_free (&blob);

  buffer_contents_t contents = BUFFER_CONTENTS_INIT;
  যদি (!buffer_copy_contents (buffer, &contents))
    প্রত্যাগম অসত্য;

  debugprint1 ("buffer length", contents.length);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  প্রতি (unsigned i = 0; i < contents.length; i++)
  {
    বর্ণ name[64];

    debugprint1 ("glyph at", i);

    font_glyph_to_string (font, contents.info[i].codepoint, name, পরিমাণ (name));

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

  প্রত্যাগম ret;
}
