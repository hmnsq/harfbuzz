#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ਅਨ੍ਤਰ੍ਭਾਵ <hb-wasm-api.h>

ਬਾਹਰੀ "C" {
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
  ਵਾਪਸ shape_with (font, ਬਫਰ, features, ਅੰਕ_features, "ot");
}
