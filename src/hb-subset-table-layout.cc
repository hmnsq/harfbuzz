#অন্তর্ভাৱ "hb-subset-table.hh"

#অন্তর্ভাৱ "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-gsub-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-gpos-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-base-table.hh"
#অন্তর্ভাৱ "hb-ot-math-table.hh"

তার্কিক _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<ৱর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  ৱিকল্প (tag)
  {
  পক্ষ HB_TAG('G','D','E','F'): *success = _hb_subset_table<ধ্রুৱ OT::GDEF> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('G','S','U','B'): *success = _hb_subset_table<ধ্রুৱ OT::Layout::GSUB> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('G','P','O','S'): *success = _hb_subset_table<ধ্রুৱ OT::Layout::GPOS> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('B','A','S','E'): *success = _hb_subset_table<ধ্রুৱ OT::BASE> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('M','A','T','H'): *success = _hb_subset_table<ধ্রুৱ OT::MATH> (plan, buf); ঘূৰাই সঁচা;
  }
#endif
  ঘূৰাই মিছা;
}
