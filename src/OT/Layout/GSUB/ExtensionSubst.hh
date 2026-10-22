#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية ExtensionSubst : Extension<ExtensionSubst>
{
  تعريفنوع بنية SubstLookupSubTable SubTable;
  منطقي is_reverse () ثابت;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
