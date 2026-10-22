#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#अन्तर्भाव <hb-wasm-api.h>

बाह्य "C" {
शून्य debugprint1 (ध्रुव वर्ण *s, int32_t);
शून्य debugprint2 (ध्रुव वर्ण *s, int32_t, int32_t);
}

bool_t
shape (शून्य *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ध्रुव feature_t *features,
       uint32_t अंक_features)
{
  प्रत्यागम shape_with (font, buffer, features, अंक_features, "ot");
}
