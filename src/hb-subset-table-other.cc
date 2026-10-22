#અન્તર્ભાવ "hb-subset-table.hh"

#અન્તર્ભાવ "hb-ot-cmap-table.hh"
#અન્તર્ભાવ "hb-ot-glyf-table.hh"
#અન્તર્ભાવ "hb-ot-hdmx-table.hh"
#અન્તર્ભાવ "hb-ot-hhea-table.hh"
#અન્તર્ભાવ "hb-ot-hmtx-table.hh"
#અન્તર્ભાવ "hb-ot-maxp-table.hh"
#અન્તર્ભાવ "hb-ot-os2-table.hh"
#અન્તર્ભાવ "hb-ot-name-table.hh"
#અન્તર્ભાવ "hb-ot-post-table.hh"

તાર્કિક _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<અક્ષર> &buf, hb_tag_t tag, તાર્કિક *success)
{
  પસંદગી (tag)
  {
  સ્થિતિ HB_TAG('g','l','y','f'): *success = _hb_subset_table<અચલ OT::glyf> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('h','d','m','x'): *success = _hb_subset_table<અચલ OT::hdmx> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('n','a','m','e'): *success = _hb_subset_table<અચલ OT::name> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('h','h','e','a'): *success = સાચું; પાછા સાચું; /* skip hhea, handled by hmtx */
  સ્થિતિ HB_TAG('h','m','t','x'): *success = _hb_subset_table<અચલ OT::hmtx> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('v','h','e','a'): *success = સાચું; પાછા સાચું; /* skip vhea, handled by vmtx */
  સ્થિતિ HB_TAG('v','m','t','x'): *success = _hb_subset_table<અચલ OT::vmtx> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('m','a','x','p'): *success = _hb_subset_table<અચલ OT::maxp> (plan, buf); પાછા સાચું;
#ifndef HB_NO_BEYOND_64K
  સ્થિતિ HB_TAG('G','L','Y','F'): *success = _hb_subset_table<અચલ OT::GLYF> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('H','H','E','A'): *success = સાચું; પાછા સાચું; /* skip HHEA, handled by HMTX */
  સ્થિતિ HB_TAG('H','M','T','X'): *success = _hb_subset_table<અચલ OT::HMTX> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('V','H','E','A'): *success = સાચું; પાછા સાચું; /* skip VHEA, handled by VMTX */
  સ્થિતિ HB_TAG('V','M','T','X'): *success = _hb_subset_table<અચલ OT::VMTX> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('M','A','X','P'): *success = _hb_subset_table<અચલ OT::MAXP> (plan, buf); પાછા સાચું;
#endif
  સ્થિતિ HB_TAG('l','o','c','a'): *success = સાચું; પાછા સાચું; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  સ્થિતિ HB_TAG('L','O','C','A'): *success = સાચું; પાછા સાચું; /* skip LOCA, handled by GLYF */
#endif
  સ્થિતિ HB_TAG('c','m','a','p'): *success = _hb_subset_table<અચલ OT::cmap> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('D','M','A','P'): *success = _hb_subset_table<અચલ OT::DMAP> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('O','S','/','2'): *success = _hb_subset_table<અચલ OT::OS2 > (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('p','o','s','t'): *success = _hb_subset_table<અચલ OT::post> (plan, buf); પાછા સાચું;
  }
  પાછા ખોટું;
}
