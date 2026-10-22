#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "OT/Color/sbix/sbix.hh"
#अन्तर्भाव "OT/Color/CPAL/CPAL.hh"
#अन्तर्भाव "OT/Color/COLR/COLR.hh"
#अन्तर्भाव "OT/Color/CBDT/CBDT.hh"

तार्किक _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_COLOR
  निवड (tag)
  {
  स्थिती HB_TAG('s','b','i','x'): *success = _hb_subset_table<स्थिरांक OT::sbix> (plan, buf); परत सत्य;
  स्थिती HB_TAG('C','O','L','R'): *success = _hb_subset_table<स्थिरांक OT::COLR> (plan, buf); परत सत्य;
  स्थिती HB_TAG('C','P','A','L'): *success = _hb_subset_table<स्थिरांक OT::CPAL> (plan, buf); परत सत्य;
  स्थिती HB_TAG('C','B','L','C'): *success = _hb_subset_table<स्थिरांक OT::CBLC> (plan, buf); परत सत्य;
  स्थिती HB_TAG('C','B','D','T'): *success = सत्य; परत सत्य; /* skip CBDT, handled by CBLC */
  }
#endif
  परत असत्य;
}
