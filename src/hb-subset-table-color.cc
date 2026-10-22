#અન્તર્ભાવ "hb-subset-table.hh"

#અન્તર્ભાવ "OT/Color/sbix/sbix.hh"
#અન્તર્ભાવ "OT/Color/CPAL/CPAL.hh"
#અન્તર્ભાવ "OT/Color/COLR/COLR.hh"
#અન્તર્ભાવ "OT/Color/CBDT/CBDT.hh"

તાર્કિક _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<અક્ષર> &buf, hb_tag_t tag, તાર્કિક *success)
{
#ifndef HB_NO_COLOR
  પસંદગી (tag)
  {
  સ્થિતિ HB_TAG('s','b','i','x'): *success = _hb_subset_table<અચલ OT::sbix> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('C','O','L','R'): *success = _hb_subset_table<અચલ OT::COLR> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('C','P','A','L'): *success = _hb_subset_table<અચલ OT::CPAL> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('C','B','L','C'): *success = _hb_subset_table<અચલ OT::CBLC> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('C','B','D','T'): *success = સાચું; પાછા સાચું; /* skip CBDT, handled by CBLC */
  }
#endif
  પાછા ખોટું;
}
