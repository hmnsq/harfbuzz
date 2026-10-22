#ଅନ୍ତର୍ଭାଵ "hb-subset-table.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gdef-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gsub-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gpos-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-base-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-math-table.hh"

ତାର୍କିକ _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('G','D','E','F'): *success = _hb_subset_table<ଅଚଳ OT::GDEF> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('G','S','U','B'): *success = _hb_subset_table<ଅଚଳ OT::Layout::GSUB> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('G','P','O','S'): *success = _hb_subset_table<ଅଚଳ OT::Layout::GPOS> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('B','A','S','E'): *success = _hb_subset_table<ଅଚଳ OT::BASE> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('M','A','T','H'): *success = _hb_subset_table<ଅଚଳ OT::MATH> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  }
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
