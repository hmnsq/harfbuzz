#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-math-table.hh"

منطقي _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  بدل (tag)
  {
  حالة HB_TAG('G','D','E','F'): *success = _hb_subset_table<ثابت OT::GDEF> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('G','S','U','B'): *success = _hb_subset_table<ثابت OT::Layout::GSUB> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('G','P','O','S'): *success = _hb_subset_table<ثابت OT::Layout::GPOS> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('B','A','S','E'): *success = _hb_subset_table<ثابت OT::BASE> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('M','A','T','H'): *success = _hb_subset_table<ثابت OT::MATH> (plan, buf); ܦܢܐ ܫܪܪܐ;
  }
#endif
  ܦܢܐ ܕܓܠܐ;
}
