#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#അന്തര്ഭാവ "../../../hb-ot-layout-gsubgpos.hh"
#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന ExtensionSubst : Extension<ExtensionSubst>
{
  തരനിർവചനം ഘടന SubstLookupSubTable SubTable;
  തർക്കം is_reverse () സ്ഥിരം;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
