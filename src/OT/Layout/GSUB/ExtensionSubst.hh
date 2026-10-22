#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"
#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना ExtensionSubst : Extension<ExtensionSubst>
{
  प्रकारनिर्देश संरचना SubstLookupSubTable SubTable;
  तार्किक is_reverse () अचर;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
