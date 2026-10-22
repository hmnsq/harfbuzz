#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-math-table.hh"

منطقي hb_subset_جدول_layout		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success)
{
#جے_بنا_تعریف HB_NO_SUBSET_LAYOUT
  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('G','D','E','F'): *success = hb_subset_جدول<ثابت OT::GDEF> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('G','S','U','B'): *success = hb_subset_جدول<ثابت OT::Layout::GSUB> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('G','P','O','S'): *success = hb_subset_جدول<ثابت OT::Layout::GPOS> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('B','A','S','E'): *success = hb_subset_جدول<ثابت OT::ادھار_4> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('M','A','T','H'): *success = hb_subset_جدول<ثابت OT::MATH> (plan, buf); أرجع سچ;
  }
#جے_انت
  أرجع خطأ;
}
