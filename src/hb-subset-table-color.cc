#அந்தர்பாவ "hb-subset-table.hh"

#அந்தர்பாவ "OT/Color/sbix/sbix.hh"
#அந்தர்பாவ "OT/Color/CPAL/CPAL.hh"
#அந்தர்பாவ "OT/Color/COLR/COLR.hh"
#அந்தர்பாவ "OT/Color/CBDT/CBDT.hh"

தருக்க _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<எழுத்து> &buf, hb_tag_t tag, தருக்க *success)
{
#ifndef HB_NO_COLOR
  தேர்வு (tag)
  {
  நிலை HB_TAG('s','b','i','x'): *success = _hb_subset_table<மாறிலி OT::sbix> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('C','O','L','R'): *success = _hb_subset_table<மாறிலி OT::COLR> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('C','P','A','L'): *success = _hb_subset_table<மாறிலி OT::CPAL> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('C','B','L','C'): *success = _hb_subset_table<மாறிலி OT::CBLC> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('C','B','D','T'): *success = உண்மை; திரும்பு உண்மை; /* skip CBDT, handled by CBLC */
  }
#endif
  திரும்பு பொய்;
}
