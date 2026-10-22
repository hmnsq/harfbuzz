#घोषणा HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#समावेश <hb-wasm-api.h>

बाह्य "C" {
व्योम debugprint1 (स्थिर अक्षर *s, int32_t);
व्योम debugprint2 (स्थिर अक्षर *s, int32_t, int32_t);
}

bool_t
shape (व्योम *shape_plan,
       font_t *font,
       बफर_t *बफर,
       स्थिर feature_t *features,
       uint32_t अंक_features)
{
  वापस shape_with (font, बफर, features, अंक_features, "ot");
}
