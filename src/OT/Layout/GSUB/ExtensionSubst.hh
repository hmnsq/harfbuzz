#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#অন্তর্ভাব "../../../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

কাঠামো ExtensionSubst : Extension<ExtensionSubst>
{
  প্রকারলেখ কাঠামো SubstLookupSubTable SubTable;
  যৌক্তিক is_reverse () ধ্রুবক;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
