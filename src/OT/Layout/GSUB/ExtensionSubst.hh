#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-gsubgpos.hh"
#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା ExtensionSubst : Extension<ExtensionSubst>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା SubstLookupSubTable SubTable;
  ତାର୍କିକ is_reverse () ଅଚଳ;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
