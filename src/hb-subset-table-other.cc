#ଅନ୍ତର୍ଭାଵ "hb-subset-table.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hdmx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hhea-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hmtx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-maxp-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-name-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"

ତାର୍କିକ _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success)
{
  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('g','l','y','f'): *success = _hb_subset_table<ଅଚଳ OT::glyf> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('h','d','m','x'): *success = _hb_subset_table<ଅଚଳ OT::hdmx> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('n','a','m','e'): *success = _hb_subset_table<ଅଚଳ OT::name> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('h','h','e','a'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip hhea, handled by hmtx */
  ସ୍ଥିତି HB_TAG('h','m','t','x'): *success = _hb_subset_table<ଅଚଳ OT::hmtx> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('v','h','e','a'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip vhea, handled by vmtx */
  ସ୍ଥିତି HB_TAG('v','m','t','x'): *success = _hb_subset_table<ଅଚଳ OT::vmtx> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('m','a','x','p'): *success = _hb_subset_table<ଅଚଳ OT::maxp> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ଅଚଳ OT::GLYF> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('H','H','E','A'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip HHEA, handled by HMTX */
  ସ୍ଥିତି HB_TAG('H','M','T','X'): *success = _hb_subset_table<ଅଚଳ OT::HMTX> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('V','H','E','A'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip VHEA, handled by VMTX */
  ସ୍ଥିତି HB_TAG('V','M','T','X'): *success = _hb_subset_table<ଅଚଳ OT::VMTX> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('M','A','X','P'): *success = _hb_subset_table<ଅଚଳ OT::MAXP> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  ସ୍ଥିତି HB_TAG('l','o','c','a'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('L','O','C','A'): *success = ସତ୍ୟ; ଫେରନ୍ତୁ ସତ୍ୟ; /* skip LOCA, handled by GLYF */
#endif
  ସ୍ଥିତି HB_TAG('c','m','a','p'): *success = _hb_subset_table<ଅଚଳ OT::cmap> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('D','M','A','P'): *success = _hb_subset_table<ଅଚଳ OT::DMAP> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('O','S','/','2'): *success = _hb_subset_table<ଅଚଳ OT::OS2 > (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('p','o','s','t'): *success = _hb_subset_table<ଅଚଳ OT::post> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
