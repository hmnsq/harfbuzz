#تعریف_کن HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ضمن <hb-wasm-api.h>

خارجي "C" {
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
  أرجع shape_with (font, میانگیر, features, رقم_features, "ot");
}
