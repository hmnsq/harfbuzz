#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "OT/Color/sbix/sbix.hh"
#অন্তর্ভাব "OT/Color/CPAL/CPAL.hh"
#অন্তর্ভাব "OT/Color/COLR/COLR.hh"
#অন্তর্ভাব "OT/Color/CBDT/CBDT.hh"

তার্কিক _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_COLOR
  বিকল্প (tag)
  {
  পক্ষ HB_TAG('s','b','i','x'): *success = _hb_subset_table<ধ্রুব OT::sbix> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('C','O','L','R'): *success = _hb_subset_table<ধ্রুব OT::COLR> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('C','P','A','L'): *success = _hb_subset_table<ধ্রুব OT::CPAL> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('C','B','L','C'): *success = _hb_subset_table<ধ্রুব OT::CBLC> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('C','B','D','T'): *success = সত্য; প্রত্যাগম সত্য; /* skip CBDT, handled by CBLC */
  }
#endif
  প্রত্যাগম অসত্য;
}
