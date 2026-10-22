#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-hdmx-table.hh"
#ضمن "hb-ot-hhea-table.hh"
#ضمن "hb-ot-hmtx-table.hh"
#ضمن "hb-ot-maxp-table.hh"
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-name-table.hh"
#ضمن "hb-ot-post-table.hh"

منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success)
{
  بدل (نښان_2)
  {
  حالة HB_نښان('g','l','y','f'): *success = hb_subset_جدول<ثابت OT::glyf> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('h','d','m','x'): *success = hb_subset_جدول<ثابت OT::hdmx> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('n','a','m','e'): *success = hb_subset_جدول<ثابت OT::name> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('h','h','e','a'): *success = ریښتیا; أرجع ریښتیا; /* skip hhea, handled by hmtx */
  حالة HB_نښان('h','m','t','x'): *success = hb_subset_جدول<ثابت OT::hmtx> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('v','h','e','a'): *success = ریښتیا; أرجع ریښتیا; /* skip vhea, handled by vmtx */
  حالة HB_نښان('v','m','t','x'): *success = hb_subset_جدول<ثابت OT::vmtx> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('m','a','x','p'): *success = hb_subset_جدول<ثابت OT::maxp> (plan, buf); أرجع ریښتیا;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('G','L','Y','F'): *success = hb_subset_جدول<ثابت OT::GLYF> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('H','H','E','A'): *success = ریښتیا; أرجع ریښتیا; /* skip HHEA, handled by HMTX */
  حالة HB_نښان('H','M','T','X'): *success = hb_subset_جدول<ثابت OT::HMTX> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('V','H','E','A'): *success = ریښتیا; أرجع ریښتیا; /* skip VHEA, handled by VMTX */
  حالة HB_نښان('V','M','T','X'): *success = hb_subset_جدول<ثابت OT::VMTX> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('M','A','X','P'): *success = hb_subset_جدول<ثابت OT::MAXP> (plan, buf); أرجع ریښتیا;
#د_که_پای
  حالة HB_نښان('l','o','c','a'): *success = ریښتیا; أرجع ریښتیا; /* skip loca, handled by glyf */
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('L','O','C','A'): *success = ریښتیا; أرجع ریښتیا; /* skip LOCA, handled by GLYF */
#د_که_پای
  حالة HB_نښان('c','m','a','p'): *success = hb_subset_جدول<ثابت OT::cmap> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('D','M','A','P'): *success = hb_subset_جدول<ثابت OT::DMAP> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('O','S','/','2'): *success = hb_subset_جدول<ثابت OT::OS2 > (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('p','o','s','t'): *success = hb_subset_جدول<ثابت OT::post> (plan, buf); أرجع ریښتیا;
  }
  أرجع خطأ;
}
