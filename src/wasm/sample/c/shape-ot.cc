#నిర్వచించు HB_WASM_INTERFACE(ret_t, name) __attribute__((export_name(#name))) ret_t name

#అన్తర్భావ <hb-wasm-api.h>

బాహ్య "C" {
శూన్య debugprint1 (స్థిరరాశి అక్షరం *s, int32_t);
శూన్య debugprint2 (స్థిరరాశి అక్షరం *s, int32_t, int32_t);
}

bool_t
shape (శూన్య *shape_plan,
       font_t *font,
       బఫరు_t *బఫరు,
       స్థిరరాశి feature_t *features,
       uint32_t అంకె_features)
{
  తిరిగి shape_with (font, బఫరు, features, అంకె_features, "ot");
}
