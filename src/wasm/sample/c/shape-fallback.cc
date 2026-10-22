#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাব <hb-wasm-api.h>

বাহ্যিক "C" {
শূন্য debugprint (ধ্রুবক অক্ষর *s);
শূন্য debugprint1 (ধ্রুবক অক্ষর *s, int32_t);
শূন্য debugprint2 (ধ্রুবক অক্ষর *s, int32_t, int32_t);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       বাফার_t *বাফার,
       ধ্রুবক feature_t *features,
       uint32_t অঙ্ক_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_table (face, TAG ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.দৈর্ঘ্য);

  blob_free (&blob);

  বাফার_contents_t contents = বাফার_CONTENTS_INIT;
  যদি (!বাফার_copy_contents (বাফার, &contents))
    ফেরত মিথ্যা;

  debugprint1 ("buffer length", contents.দৈর্ঘ্য);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  জন্য (unsigned i = 0; i < contents.দৈর্ঘ্য; i++)
  {
    অক্ষর name[64];

    debugprint1 ("glyph at", i);

    font_glyph_to_string (font, contents.info[i].codepoint, name, আকার (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = বাফার_set_contents (বাফার, &contents);

  বাফার_contents_free (&contents);

  ফেরত ret;
}
