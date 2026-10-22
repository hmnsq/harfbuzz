#അന്തര്ഭാവ "hb-subset-table.hh"

#അന്തര്ഭാവ "OT/Color/sbix/sbix.hh"
#അന്തര്ഭാവ "OT/Color/CPAL/CPAL.hh"
#അന്തര്ഭാവ "OT/Color/COLR/COLR.hh"
#അന്തര്ഭാവ "OT/Color/CBDT/CBDT.hh"

തർക്കം _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<അക്ഷരം> &buf, hb_tag_t tag, തർക്കം *success)
{
#ifndef HB_NO_COLOR
  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('s','b','i','x'): *success = _hb_subset_table<സ്ഥിരം OT::sbix> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('C','O','L','R'): *success = _hb_subset_table<സ്ഥിരം OT::COLR> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('C','P','A','L'): *success = _hb_subset_table<സ്ഥിരം OT::CPAL> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('C','B','L','C'): *success = _hb_subset_table<സ്ഥിരം OT::CBLC> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('C','B','D','T'): *success = സത്യം; തിരികെ സത്യം; /* skip CBDT, handled by CBLC */
  }
#endif
  തിരികെ അസത്യം;
}
