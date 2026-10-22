#അന്തര്ഭാവ "hb-subset-table.hh"

#അന്തര്ഭാവ "hb-ot-cmap-table.hh"
#അന്തര്ഭാവ "hb-ot-glyf-table.hh"
#അന്തര്ഭാവ "hb-ot-hdmx-table.hh"
#അന്തര്ഭാവ "hb-ot-hhea-table.hh"
#അന്തര്ഭാവ "hb-ot-hmtx-table.hh"
#അന്തര്ഭാവ "hb-ot-maxp-table.hh"
#അന്തര്ഭാവ "hb-ot-os2-table.hh"
#അന്തര്ഭാവ "hb-ot-name-table.hh"
#അന്തര്ഭാവ "hb-ot-post-table.hh"

തർക്കം _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<അക്ഷരം> &buf, hb_tag_t tag, തർക്കം *success)
{
  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('g','l','y','f'): *success = _hb_subset_table<സ്ഥിരം OT::glyf> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('h','d','m','x'): *success = _hb_subset_table<സ്ഥിരം OT::hdmx> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('n','a','m','e'): *success = _hb_subset_table<സ്ഥിരം OT::name> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('h','h','e','a'): *success = സത്യം; തിരികെ സത്യം; /* skip hhea, handled by hmtx */
  അവസ്ഥ HB_TAG('h','m','t','x'): *success = _hb_subset_table<സ്ഥിരം OT::hmtx> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('v','h','e','a'): *success = സത്യം; തിരികെ സത്യം; /* skip vhea, handled by vmtx */
  അവസ്ഥ HB_TAG('v','m','t','x'): *success = _hb_subset_table<സ്ഥിരം OT::vmtx> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('m','a','x','p'): *success = _hb_subset_table<സ്ഥിരം OT::maxp> (plan, buf); തിരികെ സത്യം;
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('G','L','Y','F'): *success = _hb_subset_table<സ്ഥിരം OT::GLYF> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('H','H','E','A'): *success = സത്യം; തിരികെ സത്യം; /* skip HHEA, handled by HMTX */
  അവസ്ഥ HB_TAG('H','M','T','X'): *success = _hb_subset_table<സ്ഥിരം OT::HMTX> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('V','H','E','A'): *success = സത്യം; തിരികെ സത്യം; /* skip VHEA, handled by VMTX */
  അവസ്ഥ HB_TAG('V','M','T','X'): *success = _hb_subset_table<സ്ഥിരം OT::VMTX> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('M','A','X','P'): *success = _hb_subset_table<സ്ഥിരം OT::MAXP> (plan, buf); തിരികെ സത്യം;
#endif
  അവസ്ഥ HB_TAG('l','o','c','a'): *success = സത്യം; തിരികെ സത്യം; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('L','O','C','A'): *success = സത്യം; തിരികെ സത്യം; /* skip LOCA, handled by GLYF */
#endif
  അവസ്ഥ HB_TAG('c','m','a','p'): *success = _hb_subset_table<സ്ഥിരം OT::cmap> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('D','M','A','P'): *success = _hb_subset_table<സ്ഥിരം OT::DMAP> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('O','S','/','2'): *success = _hb_subset_table<സ്ഥിരം OT::OS2 > (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('p','o','s','t'): *success = _hb_subset_table<സ്ഥിരം OT::post> (plan, buf); തിരികെ സത്യം;
  }
  തിരികെ അസത്യം;
}
