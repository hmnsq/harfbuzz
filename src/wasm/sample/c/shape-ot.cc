#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#അന്തര്ഭാവ <hb-wasm-api.h>

ബാഹ്യ "C" {
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
  തിരികെ shape_with (font, buffer, features, അക്കം_features, "ot");
}
