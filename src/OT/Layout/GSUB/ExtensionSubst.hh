#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساخت ExtensionSubst : Extension<ExtensionSubst>
{
  قسمتعریف ساخت Subst_Lookup_Sub_جدول Sub_جدول;
  منطقی is_الٹ () ثابت;
};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
