#ifndef OT_LAYOUT_GSUB_COMMON_HH
#define OT_LAYOUT_GSUB_COMMON_HH

#ضمن "../../../hb-serialize.hh"
#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template<typename Iterator>
ساكن فراغ SingleSubst_serialize (hb_serialize_context_t *c,
                                   Iterator it);

}
}
}

#endif /* OT_LAYOUT_GSUB_COMMON_HH */
