#شامل "hb-subset-table.hh"

#شامل "OT/Color/sbix/sbix.hh"
#شامل "OT/Color/CPAL/CPAL.hh"
#شامل "OT/Color/COLR/COLR.hh"
#شامل "OT/Color/CBDT/CBDT.hh"

منطقی _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<نویسه> &buf, hb_tag_t tag, منطقی *success)
{
#ifndef HB_NO_COLOR
  انتخاب (tag)
  {
  حالت HB_TAG('s','b','i','x'): *success = _hb_subset_table<ثابت OT::sbix> (plan, buf); بازگشت درست;
  حالت HB_TAG('C','O','L','R'): *success = _hb_subset_table<ثابت OT::COLR> (plan, buf); بازگشت درست;
  حالت HB_TAG('C','P','A','L'): *success = _hb_subset_table<ثابت OT::CPAL> (plan, buf); بازگشت درست;
  حالت HB_TAG('C','B','L','C'): *success = _hb_subset_table<ثابت OT::CBLC> (plan, buf); بازگشت درست;
  حالت HB_TAG('C','B','D','T'): *success = درست; بازگشت درست; /* skip CBDT, handled by CBLC */
  }
#endif
  بازگشت نادرست;
}
