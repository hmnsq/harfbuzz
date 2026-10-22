#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-gsubgpos.hh"
#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸಂರಚನಾ ExtensionSubst : Extension<ExtensionSubst>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ SubstLookupSubTable SubTable;
  ತಾರ್ಕಿಕ is_reverse () ಧ್ರುವ;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
