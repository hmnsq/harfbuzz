#اگر_تعریف_نشده OT_LAYOUT_GSUB_COMMON_HH
#تعریف_کن OT_LAYOUT_GSUB_COMMON_HH

#ضمن "../../../hb-serialize.hh"
#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب<نام_نوع Iterator>
ساكن فراغ SingleSubst_serialize (hb_serialize_context_t *c,
                                   Iterator it);

}
}
}

#پایان_اگر /* OT_LAYOUT_GSUB_COMMON_HH */
