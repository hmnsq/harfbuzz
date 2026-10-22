#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ਅਨ੍ਤਰ੍ਭਾਵ <hb-wasm-api.h>

ਬਾਹਰੀ "C" {
ਖਾਲੀ debugprint (ਅਚਲ ਅੱਖਰ *s);
ਖਾਲੀ debugprint1 (ਅਚਲ ਅੱਖਰ *s, int32_t);
ਖਾਲੀ debugprint2 (ਅਚਲ ਅੱਖਰ *s, int32_t, int32_t);
}

bool_t
shape (ਖਾਲੀ *shape_plan,
       font_t *font,
       ਬਫਰ_t *ਬਫਰ,
       ਅਚਲ feature_t *features,
       uint32_t ਅੰਕ_features)
{
  face_t *face = font_get_face (font);

  blob_t blob = BLOB_INIT;
  face_copy_ਸਾਰਣੀ (face, ਟੈਗ_3 ('c','m','a','p'), &blob);

  debugprint1 ("cmap length", blob.ਲੰਮਾਈ);

  blob_free (&blob);

  ਬਫਰ_contents_t contents = ਬਫਰ_CONTENTS_INIT;
  ਜੇ (!ਬਫਰ_copy_contents (ਬਫਰ, &contents))
    ਵਾਪਸ ਝੂਠ;

  debugprint1 ("buffer length", contents.ਲੰਮਾਈ);

  glyph_outline_t outline = GLYPH_OUTLINE_INIT;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < contents.ਲੰਮਾਈ; i++)
  {
    ਅੱਖਰ name[64];

    debugprint1 ("glyph at", i);

    font_glyph_ਮੰਜ਼ਿਲ_string (font, contents.info[i].codepoint, name, ਆਕਾਰ (name));

    debugprint (name);

    contents.info[i].codepoint = font_get_glyph (font, contents.info[i].codepoint, 0);
    contents.pos[i].x_advance = font_get_glyph_h_advance (font, contents.info[i].codepoint);

    font_copy_glyph_outline (font, contents.info[i].codepoint, &outline);
    debugprint1 ("num outline points", outline.n_points);
    debugprint1 ("num outline contours", outline.n_contours);
  }

  glyph_outline_free (&outline);

  bool_t ret = ਬਫਰ_ਸੈੱਟ_contents (ਬਫਰ, &contents);

  ਬਫਰ_contents_free (&contents);

  ਵਾਪਸ ret;
}
