#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-ot-layout-gsub-table.hh"
#अन्तर्भाव "hb-ot-layout-gpos-table.hh"
#अन्तर्भाव "hb-ot-layout-base-table.hh"
#अन्तर्भाव "hb-ot-math-table.hh"

तार्किक _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  विकल्प (tag)
  {
  पक्ष HB_TAG('G','D','E','F'): *success = _hb_subset_table<अचर OT::GDEF> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('G','S','U','B'): *success = _hb_subset_table<अचर OT::Layout::GSUB> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('G','P','O','S'): *success = _hb_subset_table<अचर OT::Layout::GPOS> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('B','A','S','E'): *success = _hb_subset_table<अचर OT::BASE> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('M','A','T','H'): *success = _hb_subset_table<अचर OT::MATH> (plan, buf); फर्काऊ सत्य;
  }
#endif
  फर्काऊ असत्य;
}
