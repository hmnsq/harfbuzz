#تعریف_کن HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ضمن <hb-wasm-api.h>

خارجي "C" {
فراغ debugprint (ثابت حرف *s);
فراغ debugprint1 (ثابت حرف *s, int32_t);
فراغ debugprint2 (ثابت حرف *s, int32_t, int32_t);
}

bool_t
shape (فراغ *shape_plan,
       font_t *font,
       میانگیر_t *میانگیر,
       ثابت feature_t *features,
       uint32_t رقم_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_جدول (face, برچسب_3 ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.درازی);

  blob_free (&blob);

  میانگیر_contents_t contents = میانگیر_CONTENTS_INIT;
  إذا (!میانگیر_copy_contents (میانگیر, &contents))
    أرجع خطأ;

  debugprint1 ("buffer length", contents.درازی);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  لكل (بیعلامت i = 0; i < contents.درازی; i++)
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

  bool_t ret = میانگیر_مجموعه_contents (میانگیر, &contents);

  میانگیر_contents_free (&contents);

  أرجع ret;
}
