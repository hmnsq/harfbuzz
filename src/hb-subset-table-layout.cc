#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-ot-layout-gsub-table.hh"
#अन्तर्भाव "hb-ot-layout-gpos-table.hh"
#अन्तर्भाव "hb-ot-layout-base-table.hh"
#अन्तर्भाव "hb-ot-math-table.hh"

तार्किक _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  निवड (tag)
  {
  स्थिती HB_TAG('G','D','E','F'): *success = _hb_subset_table<स्थिरांक OT::GDEF> (plan, buf); परत सत्य;
  स्थिती HB_TAG('G','S','U','B'): *success = _hb_subset_table<स्थिरांक OT::Layout::GSUB> (plan, buf); परत सत्य;
  स्थिती HB_TAG('G','P','O','S'): *success = _hb_subset_table<स्थिरांक OT::Layout::GPOS> (plan, buf); परत सत्य;
  स्थिती HB_TAG('B','A','S','E'): *success = _hb_subset_table<स्थिरांक OT::BASE> (plan, buf); परत सत्य;
  स्थिती HB_TAG('M','A','T','H'): *success = _hb_subset_table<स्थिरांक OT::MATH> (plan, buf); परत सत्य;
  }
#endif
  परत असत्य;
}
