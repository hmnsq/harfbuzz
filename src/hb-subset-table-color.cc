#অন্তর্ভাৱ "hb-subset-table.hh"

#অন্তর্ভাৱ "OT/Color/sbix/sbix.hh"
#অন্তর্ভাৱ "OT/Color/CPAL/CPAL.hh"
#অন্তর্ভাৱ "OT/Color/COLR/COLR.hh"
#অন্তর্ভাৱ "OT/Color/CBDT/CBDT.hh"

তার্কিক _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<ৱর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_COLOR
  ৱিকল্প (tag)
  {
  পক্ষ HB_TAG('s','b','i','x'): *success = _hb_subset_table<ধ্রুৱ OT::sbix> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('C','O','L','R'): *success = _hb_subset_table<ধ্রুৱ OT::COLR> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('C','P','A','L'): *success = _hb_subset_table<ধ্রুৱ OT::CPAL> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('C','B','L','C'): *success = _hb_subset_table<ধ্রুৱ OT::CBLC> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('C','B','D','T'): *success = সঁচা; ঘূৰাই সঁচা; /* skip CBDT, handled by CBLC */
  }
#endif
  ঘূৰাই মিছা;
}
