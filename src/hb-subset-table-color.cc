#ଅନ୍ତର୍ଭାଵ "hb-subset-table.hh"

#ଅନ୍ତର୍ଭାଵ "OT/Color/sbix/sbix.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/CPAL/CPAL.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/COLR.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/CBDT/CBDT.hh"

ତାର୍କିକ _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success)
{
#ifndef HB_NO_COLOR
  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('s','b','i','x'): *success = _hb_subset_table<ଅଚଳ OT::sbix> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('C','O','L','R'): *success = _hb_subset_table<ଅଚଳ OT::COLR> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('C','P','A','L'): *success = _hb_subset_table<ଅଚଳ OT::CPAL> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('C','B','L','C'): *success = _hb_subset_table<ଅଚଳ OT::CBLC> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('C','B','D','T'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip CBDT, handled by CBLC */
  }
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
