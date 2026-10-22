#ضمن "hb-subset-table.hh"

#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success)
{
#جے_بنا_تعریف HB_NO_COLOR
  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('s','b','i','x'): *success = hb_subset_جدول<ثابت OT::sbix> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('C','O','L','R'): *success = hb_subset_جدول<ثابت OT::COLR> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('C','P','A','L'): *success = hb_subset_جدول<ثابت OT::CPAL> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('C','B','L','C'): *success = hb_subset_جدول<ثابت OT::CBLC> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('C','B','D','T'): *success = سچ; أرجع سچ; /* skip CBDT, handled by CBLC */
  }
#جے_انت
  أرجع خطأ;
}
