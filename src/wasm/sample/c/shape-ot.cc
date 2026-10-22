#define HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#অন্তর্ভাব <hb-wasm-api.h>

বাহ্যিক "C" {
শূন্য debugprint1 (ধ্রুবক অক্ষর *s, int32_t);
শূন্য debugprint2 (ধ্রুবক অক্ষর *s, int32_t, int32_t);
}

bool_t
shape (শূন্য *shape_plan,
       font_t *font,
       বাফার_t *বাফার,
       ধ্রুবক feature_t *features,
       uint32_t অঙ্ক_features)
{
  ফেরত shape_with (font, বাফার, features, অঙ্ক_features, "ot");
}
