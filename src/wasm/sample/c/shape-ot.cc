#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#ضمن <hb-wasm-api.h>

خارجي "C" {
فراغ debugprint1 (ثابت حرف *s, int32_t);
فراغ debugprint2 (ثابت حرف *s, int32_t, int32_t);
}

bool_t
shape (فراغ *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ثابت feature_t *features,
       uint32_t num_features)
{
  ܦܢܐ shape_with (font, buffer, features, num_features, "ot");
}
