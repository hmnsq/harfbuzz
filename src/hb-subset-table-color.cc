#ضمن "hb-subset-table.hh"

#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_COLOR
  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('s','b','i','x'): *success = hb_subset_جدول<ثابت OT::sbix> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('C','O','L','R'): *success = hb_subset_جدول<ثابت OT::COLR> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('C','P','A','L'): *success = hb_subset_جدول<ثابت OT::CPAL> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('C','B','L','C'): *success = hb_subset_جدول<ثابت OT::CBLC> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('C','B','D','T'): *success = سچ; أرجع سچ; /* skip CBDT, handled by CBLC */
  }
#جيڪڏهن_پورو
  أرجع خطأ;
}
