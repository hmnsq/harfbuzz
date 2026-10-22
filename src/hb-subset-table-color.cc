#ضمن "hb-subset-table.hh"

#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success)
{
#که_نه_تعریف_شوی HB_NO_COLOR
  بدل (نښان_2)
  {
  حالة HB_نښان('s','b','i','x'): *success = hb_subset_جدول<ثابت OT::sbix> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('C','O','L','R'): *success = hb_subset_جدول<ثابت OT::COLR> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('C','P','A','L'): *success = hb_subset_جدول<ثابت OT::CPAL> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('C','B','L','C'): *success = hb_subset_جدول<ثابت OT::CBLC> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('C','B','D','T'): *success = ریښتیا; أرجع ریښتیا; /* skip CBDT, handled by CBLC */
  }
#د_که_پای
  أرجع خطأ;
}
