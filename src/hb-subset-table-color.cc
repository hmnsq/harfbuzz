#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "OT/Color/sbix/sbix.hh"
#अन्तर्भाव "OT/Color/CPAL/CPAL.hh"
#अन्तर्भाव "OT/Color/COLR/COLR.hh"
#अन्तर्भाव "OT/Color/CBDT/CBDT.hh"

तार्किक _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_COLOR
  विकल्प (tag)
  {
  पक्ष HB_TAG('s','b','i','x'): *success = _hb_subset_table<ध्रुव OT::sbix> (plan, buf); प्रत्यागम सच्च;
  पक्ष HB_TAG('C','O','L','R'): *success = _hb_subset_table<ध्रुव OT::COLR> (plan, buf); प्रत्यागम सच्च;
  पक्ष HB_TAG('C','P','A','L'): *success = _hb_subset_table<ध्रुव OT::CPAL> (plan, buf); प्रत्यागम सच्च;
  पक्ष HB_TAG('C','B','L','C'): *success = _hb_subset_table<ध्रुव OT::CBLC> (plan, buf); प्रत्यागम सच्च;
  पक्ष HB_TAG('C','B','D','T'): *success = सच्च; प्रत्यागम सच्च; /* skip CBDT, handled by CBLC */
  }
#endif
  प्रत्यागम मिच्छ;
}
