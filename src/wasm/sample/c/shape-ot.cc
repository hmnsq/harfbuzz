#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাব <hb-wasm-api.h>

বাহ্য "C" {
শূন্য debugprint1 (ধ্রুব বর্ণ *s, int32_t);
শূন্য debugprint2 (ধ্রুব বর্ণ *s, int32_t, int32_t);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       buffer_t *buffer,
       ধ্রুব feature_t *features,
       uint32_t অঙ্ক_features)
{
  প্রত্যাগম shape_with (font, buffer, features, অঙ্ক_features, "ot");
}
