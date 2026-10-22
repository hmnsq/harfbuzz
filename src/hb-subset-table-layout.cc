#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাব "hb-ot-layout-gsub-table.hh"
#অন্তর্ভাব "hb-ot-layout-gpos-table.hh"
#অন্তর্ভাব "hb-ot-layout-base-table.hh"
#অন্তর্ভাব "hb-ot-math-table.hh"

যৌক্তিক _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<অক্ষর> &buf, hb_tag_t tag, যৌক্তিক *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('G','D','E','F'): *success = _hb_subset_table<ধ্রুবক OT::GDEF> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('G','S','U','B'): *success = _hb_subset_table<ধ্রুবক OT::Layout::GSUB> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('G','P','O','S'): *success = _hb_subset_table<ধ্রুবক OT::Layout::GPOS> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('B','A','S','E'): *success = _hb_subset_table<ধ্রুবক OT::BASE> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('M','A','T','H'): *success = _hb_subset_table<ধ্রুবক OT::MATH> (plan, buf); ফেরত সত্য;
  }
#endif
  ফেরত মিথ্যা;
}
