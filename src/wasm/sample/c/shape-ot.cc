#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#અન્તર્ભાવ <hb-wasm-api.h>

બાહ્ય "C" {
શૂન્ય debugprint1 (અચલ અક્ષર *s, int32_t);
શૂન્ય debugprint2 (અચલ અક્ષર *s, int32_t, int32_t);
}

bool_t
shape (શૂન્ય *shape_plan,
       font_t *font,
       buffer_t *buffer,
       અચલ feature_t *features,
       uint32_t અંક_features)
{
  પાછા shape_with (font, buffer, features, અંક_features, "ot");
}
