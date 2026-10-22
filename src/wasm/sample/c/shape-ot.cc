#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ಅನ್ತರ್ಭಾವ <hb-wasm-api.h>

ಬಾಹ್ಯ "C" {
ಶೂನ್ಯ debugprint1 (ಧ್ರುವ ವರ್ಣ *s, int32_t);
ಶೂನ್ಯ debugprint2 (ಧ್ರುವ ವರ್ಣ *s, int32_t, int32_t);
}

bool_t
shape (ಶೂನ್ಯ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ಧ್ರುವ feature_t *features,
       uint32_t ಅಂಕೆ_features)
{
  ಪ್ರತ್ಯಾಗಮ shape_with (font, buffer, features, ಅಂಕೆ_features, "ot");
}
