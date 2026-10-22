#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ଅନ୍ତର୍ଭାଵ <hb-wasm-api.h>

ବାହ୍ୟ "C" {
ଶୂନ୍ୟ debugprint1 (ଅଚଳ ଅକ୍ଷର *s, int32_t);
ଶୂନ୍ୟ debugprint2 (ଅଚଳ ଅକ୍ଷର *s, int32_t, int32_t);
}

bool_t
shape (ଶୂନ୍ୟ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ଅଚଳ feature_t *features,
       uint32_t ଅଙ୍କ_features)
{
  ଫେରନ୍ତୁ shape_with (font, buffer, features, ଅଙ୍କ_features, "ot");
}
