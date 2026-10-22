#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাব "hb-ot-layout-gsub-table.hh"
#অন্তর্ভাব "hb-ot-layout-gpos-table.hh"
#অন্তর্ভাব "hb-ot-layout-base-table.hh"
#অন্তর্ভাব "hb-ot-math-table.hh"

তার্কিক _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  বিকল্প (tag)
  {
  পক্ষ HB_TAG('G','D','E','F'): *success = _hb_subset_table<ধ্রুব OT::GDEF> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('G','S','U','B'): *success = _hb_subset_table<ধ্রুব OT::Layout::GSUB> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('G','P','O','S'): *success = _hb_subset_table<ধ্রুব OT::Layout::GPOS> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('B','A','S','E'): *success = _hb_subset_table<ধ্রুব OT::BASE> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('M','A','T','H'): *success = _hb_subset_table<ধ্রুব OT::MATH> (plan, buf); প্রত্যাগম সত্য;
  }
#endif
  প্রত্যাগম অসত্য;
}
