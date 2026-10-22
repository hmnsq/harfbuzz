#اگر_تعریف_نشده OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#تعریف_کن OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية ExtensionSubst : Extension<ExtensionSubst>
{
  تعريفنوع بنية Subst_Lookup_Sub_جدول Sub_جدول;
  منطقي is_معکوس () ثابت;
};

}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
