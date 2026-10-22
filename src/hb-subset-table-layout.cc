#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-math-table.hh"

منطقي hb_subset_جدول_layout		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success)
{
#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT
  بدل (برچسب_2)
  {
  حالة HB_برچسب('G','D','E','F'): *success = hb_subset_جدول<ثابت OT::GDEF> (plan, buf); أرجع درست;
  حالة HB_برچسب('G','S','U','B'): *success = hb_subset_جدول<ثابت OT::Layout::GSUB> (plan, buf); أرجع درست;
  حالة HB_برچسب('G','P','O','S'): *success = hb_subset_جدول<ثابت OT::Layout::GPOS> (plan, buf); أرجع درست;
  حالة HB_برچسب('B','A','S','E'): *success = hb_subset_جدول<ثابت OT::پایه_4> (plan, buf); أرجع درست;
  حالة HB_برچسب('M','A','T','H'): *success = hb_subset_جدول<ثابت OT::MATH> (plan, buf); أرجع درست;
  }
#پایان_اگر
  أرجع خطأ;
}
