#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-math-table.hh"

منطقي hb_subset_جدول_layout		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT
  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('G','D','E','F'): *success = hb_subset_جدول<ثابت OT::GDEF> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('G','S','U','B'): *success = hb_subset_جدول<ثابت OT::Layout::GSUB> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('G','P','O','S'): *success = hb_subset_جدول<ثابت OT::Layout::GPOS> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('B','A','S','E'): *success = hb_subset_جدول<ثابت OT::بنياد_4> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('M','A','T','H'): *success = hb_subset_جدول<ثابت OT::MATH> (plan, buf); أرجع سچ;
  }
#جيڪڏهن_پورو
  أرجع خطأ;
}
