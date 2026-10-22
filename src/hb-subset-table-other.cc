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

तार्किक _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, तार्किक *success)
{
  निवड (tag)
  {
  स्थिती HB_TAG('g','l','y','f'): *success = _hb_subset_table<स्थिरांक OT::glyf> (plan, buf); परत सत्य;
  स्थिती HB_TAG('h','d','m','x'): *success = _hb_subset_table<स्थिरांक OT::hdmx> (plan, buf); परत सत्य;
  स्थिती HB_TAG('n','a','m','e'): *success = _hb_subset_table<स्थिरांक OT::name> (plan, buf); परत सत्य;
  स्थिती HB_TAG('h','h','e','a'): *success = सत्य; परत सत्य; /* skip hhea, handled by hmtx */
  स्थिती HB_TAG('h','m','t','x'): *success = _hb_subset_table<स्थिरांक OT::hmtx> (plan, buf); परत सत्य;
  स्थिती HB_TAG('v','h','e','a'): *success = सत्य; परत सत्य; /* skip vhea, handled by vmtx */
  स्थिती HB_TAG('v','m','t','x'): *success = _hb_subset_table<स्थिरांक OT::vmtx> (plan, buf); परत सत्य;
  स्थिती HB_TAG('m','a','x','p'): *success = _hb_subset_table<स्थिरांक OT::maxp> (plan, buf); परत सत्य;
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('G','L','Y','F'): *success = _hb_subset_table<स्थिरांक OT::GLYF> (plan, buf); परत सत्य;
  स्थिती HB_TAG('H','H','E','A'): *success = सत्य; परत सत्य; /* skip HHEA, handled by HMTX */
  स्थिती HB_TAG('H','M','T','X'): *success = _hb_subset_table<स्थिरांक OT::HMTX> (plan, buf); परत सत्य;
  स्थिती HB_TAG('V','H','E','A'): *success = सत्य; परत सत्य; /* skip VHEA, handled by VMTX */
  स्थिती HB_TAG('V','M','T','X'): *success = _hb_subset_table<स्थिरांक OT::VMTX> (plan, buf); परत सत्य;
  स्थिती HB_TAG('M','A','X','P'): *success = _hb_subset_table<स्थिरांक OT::MAXP> (plan, buf); परत सत्य;
#endif
  स्थिती HB_TAG('l','o','c','a'): *success = सत्य; परत सत्य; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('L','O','C','A'): *success = सत्य; परत सत्य; /* skip LOCA, handled by GLYF */
#endif
  स्थिती HB_TAG('c','m','a','p'): *success = _hb_subset_table<स्थिरांक OT::cmap> (plan, buf); परत सत्य;
  स्थिती HB_TAG('D','M','A','P'): *success = _hb_subset_table<स्थिरांक OT::DMAP> (plan, buf); परत सत्य;
  स्थिती HB_TAG('O','S','/','2'): *success = _hb_subset_table<स्थिरांक OT::OS2 > (plan, buf); परत सत्य;
  स्थिती HB_TAG('p','o','s','t'): *success = _hb_subset_table<स्थिरांक OT::post> (plan, buf); परत सत्य;
  }
  परत असत्य;
}
