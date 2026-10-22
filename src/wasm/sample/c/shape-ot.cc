#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#شامل <hb-wasm-api.h>

بیرونی "C" {
تهی debugprint1 (ثابت نویسه *s, int32_t);
تهی debugprint2 (ثابت نویسه *s, int32_t, int32_t);
}

bool_t
shape (تهی *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ثابت feature_t *features,
       uint32_t رقم_features)
{
  بازگشت shape_with (font, buffer, features, رقم_features, "ot");
}
