#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#அந்தர்பாவ "../../../hb-ot-layout-gsubgpos.hh"
#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

கட்டமைப்பு ExtensionSubst : Extension<ExtensionSubst>
{
  வகைவரை கட்டமைப்பு SubstLookupSubTable SubTable;
  தருக்க is_reverse () மாறிலி;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
