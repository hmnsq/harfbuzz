#ifndef OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#define OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#અન્તર્ભાવ "../../../hb-ot-layout-gsubgpos.hh"
#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સંરચના ExtensionSubst : Extension<ExtensionSubst>
{
  પ્રકારવ્યાખ્યા સંરચના SubstLookupSubTable SubTable;
  તાર્કિક is_reverse () અચલ;
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
