#شامل "hb-subset-table.hh"

#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gsub-table.hh"
#شامل "hb-ot-layout-gpos-table.hh"
#شامل "hb-ot-layout-base-table.hh"
#شامل "hb-ot-math-table.hh"

منطقی _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<نویسه> &buf, hb_tag_t tag, منطقی *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  انتخاب (tag)
  {
  حالت HB_TAG('G','D','E','F'): *success = _hb_subset_table<ثابت OT::GDEF> (plan, buf); بازگشت درست;
  حالت HB_TAG('G','S','U','B'): *success = _hb_subset_table<ثابت OT::Layout::GSUB> (plan, buf); بازگشت درست;
  حالت HB_TAG('G','P','O','S'): *success = _hb_subset_table<ثابت OT::Layout::GPOS> (plan, buf); بازگشت درست;
  حالت HB_TAG('B','A','S','E'): *success = _hb_subset_table<ثابت OT::BASE> (plan, buf); بازگشت درست;
  حالت HB_TAG('M','A','T','H'): *success = _hb_subset_table<ثابت OT::MATH> (plan, buf); بازگشت درست;
  }
#endif
  بازگشت نادرست;
}
