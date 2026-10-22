#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار ExtensionSubst : Extension<ExtensionSubst>
{
  تعریفنوع ساختار SubstLookupSubTable SubTable;
  منطقی is_reverse () ثابت;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
