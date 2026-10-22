#அந்தர்பாவ "hb-subset-table.hh"

#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-ot-layout-gsub-table.hh"
#அந்தர்பாவ "hb-ot-layout-gpos-table.hh"
#அந்தர்பாவ "hb-ot-layout-base-table.hh"
#அந்தர்பாவ "hb-ot-math-table.hh"

தருக்க _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<எழுத்து> &buf, hb_tag_t tag, தருக்க *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  தேர்வு (tag)
  {
  நிலை HB_TAG('G','D','E','F'): *success = _hb_subset_table<மாறிலி OT::GDEF> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('G','S','U','B'): *success = _hb_subset_table<மாறிலி OT::Layout::GSUB> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('G','P','O','S'): *success = _hb_subset_table<மாறிலி OT::Layout::GPOS> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('B','A','S','E'): *success = _hb_subset_table<மாறிலி OT::BASE> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('M','A','T','H'): *success = _hb_subset_table<மாறிலி OT::MATH> (plan, buf); திரும்பு உண்மை;
  }
#endif
  திரும்பு பொய்;
}
