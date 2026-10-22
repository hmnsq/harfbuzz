#அந்தர்பாவ "hb-subset-table.hh"

#அந்தர்பாவ "hb-ot-cmap-table.hh"
#அந்தர்பாவ "hb-ot-glyf-table.hh"
#அந்தர்பாவ "hb-ot-hdmx-table.hh"
#அந்தர்பாவ "hb-ot-hhea-table.hh"
#அந்தர்பாவ "hb-ot-hmtx-table.hh"
#அந்தர்பாவ "hb-ot-maxp-table.hh"
#அந்தர்பாவ "hb-ot-os2-table.hh"
#அந்தர்பாவ "hb-ot-name-table.hh"
#அந்தர்பாவ "hb-ot-post-table.hh"

தருக்க _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<எழுத்து> &buf, hb_tag_t tag, தருக்க *success)
{
  தேர்வு (tag)
  {
  நிலை HB_TAG('g','l','y','f'): *success = _hb_subset_table<மாறிலி OT::glyf> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('h','d','m','x'): *success = _hb_subset_table<மாறிலி OT::hdmx> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('n','a','m','e'): *success = _hb_subset_table<மாறிலி OT::name> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('h','h','e','a'): *success = உண்மை; திரும்பு உண்மை; /* skip hhea, handled by hmtx */
  நிலை HB_TAG('h','m','t','x'): *success = _hb_subset_table<மாறிலி OT::hmtx> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('v','h','e','a'): *success = உண்மை; திரும்பு உண்மை; /* skip vhea, handled by vmtx */
  நிலை HB_TAG('v','m','t','x'): *success = _hb_subset_table<மாறிலி OT::vmtx> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('m','a','x','p'): *success = _hb_subset_table<மாறிலி OT::maxp> (plan, buf); திரும்பு உண்மை;
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('G','L','Y','F'): *success = _hb_subset_table<மாறிலி OT::GLYF> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('H','H','E','A'): *success = உண்மை; திரும்பு உண்மை; /* skip HHEA, handled by HMTX */
  நிலை HB_TAG('H','M','T','X'): *success = _hb_subset_table<மாறிலி OT::HMTX> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('V','H','E','A'): *success = உண்மை; திரும்பு உண்மை; /* skip VHEA, handled by VMTX */
  நிலை HB_TAG('V','M','T','X'): *success = _hb_subset_table<மாறிலி OT::VMTX> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('M','A','X','P'): *success = _hb_subset_table<மாறிலி OT::MAXP> (plan, buf); திரும்பு உண்மை;
#endif
  நிலை HB_TAG('l','o','c','a'): *success = உண்மை; திரும்பு உண்மை; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('L','O','C','A'): *success = உண்மை; திரும்பு உண்மை; /* skip LOCA, handled by GLYF */
#endif
  நிலை HB_TAG('c','m','a','p'): *success = _hb_subset_table<மாறிலி OT::cmap> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('D','M','A','P'): *success = _hb_subset_table<மாறிலி OT::DMAP> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('O','S','/','2'): *success = _hb_subset_table<மாறிலி OT::OS2 > (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('p','o','s','t'): *success = _hb_subset_table<மாறிலி OT::post> (plan, buf); திரும்பு உண்மை;
  }
  திரும்பு பொய்;
}
