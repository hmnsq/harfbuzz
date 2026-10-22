#تعریف_کرو HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#شامل <hb-wasm-api.h>

خارجی "C" {
خالی debugprint1 (ثابت حرف *s, int32_t);
خالی debugprint2 (ثابت حرف *s, int32_t, int32_t);
}

bool_t
shape (خالی *shape_plan,
       font_t *font,
       بفر_t *بفر,
       ثابت feature_t *features,
       uint32_t عدد_features)
{
  واپس shape_with (font, بفر, features, عدد_features, "ot");
}
