#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#অন্তর্ভাৱ "../../../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

সংরচনা ExtensionSubst : Extension<ExtensionSubst>
{
  প্রকারনির্দেশ সংরচনা SubstLookupSubTable SubTable;
  তার্কিক is_reverse () ধ্রুৱ;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
