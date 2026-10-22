#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#अन्तर्भाव <hb-wasm-api.h>

बाह्य "C" {
शून्य debugprint1 (अचर वर्ण *s, int32_t);
शून्य debugprint2 (अचर वर्ण *s, int32_t, int32_t);
}

bool_t
shape (शून्य *shape_plan,
       font_t *font,
       buffer_t *buffer,
       अचर feature_t *features,
       uint32_t अङ्क_features)
{
  फर्काऊ shape_with (font, buffer, features, अङ्क_features, "ot");
}
