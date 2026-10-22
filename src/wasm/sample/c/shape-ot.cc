#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#அந்தர்பாவ <hb-wasm-api.h>

வெளி "C" {
வெறுமை debugprint1 (மாறிலி எழுத்து *s, int32_t);
வெறுமை debugprint2 (மாறிலி எழுத்து *s, int32_t, int32_t);
}

bool_t
shape (வெறுமை *shape_plan,
       font_t *font,
       buffer_t *buffer,
       மாறிலி feature_t *features,
       uint32_t இலக்கம்_features)
{
  திரும்பு shape_with (font, buffer, features, இலக்கம்_features, "ot");
}
