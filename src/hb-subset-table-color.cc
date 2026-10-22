#ضمن "hb-subset-table.hh"

#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success)
{
#اگر_تعریف_نشده HB_NO_COLOR
  بدل (برچسب_2)
  {
  حالة HB_برچسب('s','b','i','x'): *success = hb_subset_جدول<ثابت OT::sbix> (plan, buf); أرجع درست;
  حالة HB_برچسب('C','O','L','R'): *success = hb_subset_جدول<ثابت OT::COLR> (plan, buf); أرجع درست;
  حالة HB_برچسب('C','P','A','L'): *success = hb_subset_جدول<ثابت OT::CPAL> (plan, buf); أرجع درست;
  حالة HB_برچسب('C','B','L','C'): *success = hb_subset_جدول<ثابت OT::CBLC> (plan, buf); أرجع درست;
  حالة HB_برچسب('C','B','D','T'): *success = درست; أرجع درست; /* skip CBDT, handled by CBLC */
  }
#پایان_اگر
  أرجع خطأ;
}
