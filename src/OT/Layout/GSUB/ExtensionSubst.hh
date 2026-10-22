#که_نه_تعریف_شوی OT_LAYOUT_GSUB_EXTENSIONSUBST_HH
#تعریف_کړه OT_LAYOUT_GSUB_EXTENSIONSUBST_HH

// TODO(garretrieger): move to new layout.
#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية ExtensionSubst : Extension<ExtensionSubst>
{
  تعريفنوع بنية Subst_Lookup_Sub_جدول Sub_جدول;
  منطقي is_سرچپه () ثابت;
};

}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_EXTENSIONSUBST_HH */
