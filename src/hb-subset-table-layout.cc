#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-math-table.hh"

منطقي hb_subset_جدول_layout		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success)
{
#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT
  بدل (نښان_2)
  {
  حالة HB_نښان('G','D','E','F'): *success = hb_subset_جدول<ثابت OT::GDEF> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('G','S','U','B'): *success = hb_subset_جدول<ثابت OT::Layout::GSUB> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('G','P','O','S'): *success = hb_subset_جدول<ثابت OT::Layout::GPOS> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('B','A','S','E'): *success = hb_subset_جدول<ثابت OT::بنسټ_4> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('M','A','T','H'): *success = hb_subset_جدول<ثابت OT::MATH> (plan, buf); أرجع ریښتیا;
  }
#د_که_پای
  أرجع خطأ;
}
