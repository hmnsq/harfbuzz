#અન્તર્ભાવ "hb-subset-table.hh"

#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-ot-layout-gsub-table.hh"
#અન્તર્ભાવ "hb-ot-layout-gpos-table.hh"
#અન્તર્ભાવ "hb-ot-layout-base-table.hh"
#અન્તર્ભાવ "hb-ot-math-table.hh"

તાર્કિક _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<અક્ષર> &buf, hb_tag_t tag, તાર્કિક *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  પસંદગી (tag)
  {
  સ્થિતિ HB_TAG('G','D','E','F'): *success = _hb_subset_table<અચલ OT::GDEF> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('G','S','U','B'): *success = _hb_subset_table<અચલ OT::Layout::GSUB> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('G','P','O','S'): *success = _hb_subset_table<અચલ OT::Layout::GPOS> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('B','A','S','E'): *success = _hb_subset_table<અચલ OT::BASE> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('M','A','T','H'): *success = _hb_subset_table<અચલ OT::MATH> (plan, buf); પાછા સાચું;
  }
#endif
  પાછા ખોટું;
}
