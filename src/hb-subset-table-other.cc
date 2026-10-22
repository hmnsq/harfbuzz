#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "hb-ot-cmap-table.hh"
#अन्तर्भाव "hb-ot-glyf-table.hh"
#अन्तर्भाव "hb-ot-hdmx-table.hh"
#अन्तर्भाव "hb-ot-hhea-table.hh"
#अन्तर्भाव "hb-ot-hmtx-table.hh"
#अन्तर्भाव "hb-ot-maxp-table.hh"
#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-name-table.hh"
#अन्तर्भाव "hb-ot-post-table.hh"

तार्किक _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success)
{
  विकल्प (tag)
  {
  पक्ष HB_TAG('g','l','y','f'): *success = _hb_subset_table<अचर OT::glyf> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('h','d','m','x'): *success = _hb_subset_table<अचर OT::hdmx> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('n','a','m','e'): *success = _hb_subset_table<अचर OT::name> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('h','h','e','a'): *success = सत्य; फर्काऊ सत्य; /* skip hhea, handled by hmtx */
  पक्ष HB_TAG('h','m','t','x'): *success = _hb_subset_table<अचर OT::hmtx> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('v','h','e','a'): *success = सत्य; फर्काऊ सत्य; /* skip vhea, handled by vmtx */
  पक्ष HB_TAG('v','m','t','x'): *success = _hb_subset_table<अचर OT::vmtx> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('m','a','x','p'): *success = _hb_subset_table<अचर OT::maxp> (plan, buf); फर्काऊ सत्य;
#ifndef HB_NO_BEYOND_64K
  पक्ष HB_TAG('G','L','Y','F'): *success = _hb_subset_table<अचर OT::GLYF> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('H','H','E','A'): *success = सत्य; फर्काऊ सत्य; /* skip HHEA, handled by HMTX */
  पक्ष HB_TAG('H','M','T','X'): *success = _hb_subset_table<अचर OT::HMTX> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('V','H','E','A'): *success = सत्य; फर्काऊ सत्य; /* skip VHEA, handled by VMTX */
  पक्ष HB_TAG('V','M','T','X'): *success = _hb_subset_table<अचर OT::VMTX> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('M','A','X','P'): *success = _hb_subset_table<अचर OT::MAXP> (plan, buf); फर्काऊ सत्य;
#endif
  पक्ष HB_TAG('l','o','c','a'): *success = सत्य; फर्काऊ सत्य; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  पक्ष HB_TAG('L','O','C','A'): *success = सत्य; फर्काऊ सत्य; /* skip LOCA, handled by GLYF */
#endif
  पक्ष HB_TAG('c','m','a','p'): *success = _hb_subset_table<अचर OT::cmap> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('D','M','A','P'): *success = _hb_subset_table<अचर OT::DMAP> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('O','S','/','2'): *success = _hb_subset_table<अचर OT::OS2 > (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('p','o','s','t'): *success = _hb_subset_table<अचर OT::post> (plan, buf); फर्काऊ सत्य;
  }
  फर्काऊ असत्य;
}
