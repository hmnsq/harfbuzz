#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-cmap-table.hh"
#অন্তর্ভাব "hb-ot-glyf-table.hh"
#অন্তর্ভাব "hb-ot-hdmx-table.hh"
#অন্তর্ভাব "hb-ot-hhea-table.hh"
#অন্তর্ভাব "hb-ot-hmtx-table.hh"
#অন্তর্ভাব "hb-ot-maxp-table.hh"
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-name-table.hh"
#অন্তর্ভাব "hb-ot-post-table.hh"

যৌক্তিক _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<অক্ষর> &buf, hb_tag_t tag, যৌক্তিক *success)
{
  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('g','l','y','f'): *success = _hb_subset_table<ধ্রুবক OT::glyf> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('h','d','m','x'): *success = _hb_subset_table<ধ্রুবক OT::hdmx> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('n','a','m','e'): *success = _hb_subset_table<ধ্রুবক OT::name> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('h','h','e','a'): *success = সত্য; ফেরত সত্য; /* skip hhea, handled by hmtx */
  ক্ষেত্র HB_TAG('h','m','t','x'): *success = _hb_subset_table<ধ্রুবক OT::hmtx> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('v','h','e','a'): *success = সত্য; ফেরত সত্য; /* skip vhea, handled by vmtx */
  ক্ষেত্র HB_TAG('v','m','t','x'): *success = _hb_subset_table<ধ্রুবক OT::vmtx> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('m','a','x','p'): *success = _hb_subset_table<ধ্রুবক OT::maxp> (plan, buf); ফেরত সত্য;
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ধ্রুবক OT::GLYF> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('H','H','E','A'): *success = সত্য; ফেরত সত্য; /* skip HHEA, handled by HMTX */
  ক্ষেত্র HB_TAG('H','M','T','X'): *success = _hb_subset_table<ধ্রুবক OT::HMTX> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('V','H','E','A'): *success = সত্য; ফেরত সত্য; /* skip VHEA, handled by VMTX */
  ক্ষেত্র HB_TAG('V','M','T','X'): *success = _hb_subset_table<ধ্রুবক OT::VMTX> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('M','A','X','P'): *success = _hb_subset_table<ধ্রুবক OT::MAXP> (plan, buf); ফেরত সত্য;
#endif
  ক্ষেত্র HB_TAG('l','o','c','a'): *success = সত্য; ফেরত সত্য; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('L','O','C','A'): *success = সত্য; ফেরত সত্য; /* skip LOCA, handled by GLYF */
#endif
  ক্ষেত্র HB_TAG('c','m','a','p'): *success = _hb_subset_table<ধ্রুবক OT::cmap> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('D','M','A','P'): *success = _hb_subset_table<ধ্রুবক OT::DMAP> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('O','S','/','2'): *success = _hb_subset_table<ধ্রুবক OT::OS2 > (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('p','o','s','t'): *success = _hb_subset_table<ধ্রুবক OT::post> (plan, buf); ফেরত সত্য;
  }
  ফেরত মিথ্যা;
}
