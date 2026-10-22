#അന്തര്ഭാവ "hb-subset-table.hh"

#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gsub-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gpos-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-base-table.hh"
#അന്തര്ഭാവ "hb-ot-math-table.hh"

തർക്കം _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<അക്ഷരം> &buf, hb_tag_t tag, തർക്കം *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('G','D','E','F'): *success = _hb_subset_table<സ്ഥിരം OT::GDEF> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('G','S','U','B'): *success = _hb_subset_table<സ്ഥിരം OT::Layout::GSUB> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('G','P','O','S'): *success = _hb_subset_table<സ്ഥിരം OT::Layout::GPOS> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('B','A','S','E'): *success = _hb_subset_table<സ്ഥിരം OT::BASE> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('M','A','T','H'): *success = _hb_subset_table<സ്ഥിരം OT::MATH> (plan, buf); തിരികെ സത്യം;
  }
#endif
  തിരികെ അസത്യം;
}
