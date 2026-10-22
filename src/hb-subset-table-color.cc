#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "OT/Color/sbix/sbix.hh"
#অন্তর্ভাব "OT/Color/CPAL/CPAL.hh"
#অন্তর্ভাব "OT/Color/COLR/COLR.hh"
#অন্তর্ভাব "OT/Color/CBDT/CBDT.hh"

যৌক্তিক _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<অক্ষর> &buf, hb_tag_t tag, যৌক্তিক *success)
{
#ifndef HB_NO_COLOR
  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('s','b','i','x'): *success = _hb_subset_table<ধ্রুবক OT::sbix> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('C','O','L','R'): *success = _hb_subset_table<ধ্রুবক OT::COLR> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('C','P','A','L'): *success = _hb_subset_table<ধ্রুবক OT::CPAL> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('C','B','L','C'): *success = _hb_subset_table<ধ্রুবক OT::CBLC> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('C','B','D','T'): *success = সত্য; ফেরত সত্য; /* skip CBDT, handled by CBLC */
  }
#endif
  ফেরত মিথ্যা;
}
