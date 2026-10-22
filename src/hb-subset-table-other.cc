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

منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success)
{
  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('g','l','y','f'): *success = hb_subset_جدول<ثابت OT::glyf> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('h','d','m','x'): *success = hb_subset_جدول<ثابت OT::hdmx> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('n','a','m','e'): *success = hb_subset_جدول<ثابت OT::name> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('h','h','e','a'): *success = سچ; أرجع سچ; /* skip hhea, handled by hmtx */
  حالة HB_ٹیگ('h','m','t','x'): *success = hb_subset_جدول<ثابت OT::hmtx> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('v','h','e','a'): *success = سچ; أرجع سچ; /* skip vhea, handled by vmtx */
  حالة HB_ٹیگ('v','m','t','x'): *success = hb_subset_جدول<ثابت OT::vmtx> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('m','a','x','p'): *success = hb_subset_جدول<ثابت OT::maxp> (plan, buf); أرجع سچ;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('G','L','Y','F'): *success = hb_subset_جدول<ثابت OT::GLYF> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('H','H','E','A'): *success = سچ; أرجع سچ; /* skip HHEA, handled by HMTX */
  حالة HB_ٹیگ('H','M','T','X'): *success = hb_subset_جدول<ثابت OT::HMTX> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('V','H','E','A'): *success = سچ; أرجع سچ; /* skip VHEA, handled by VMTX */
  حالة HB_ٹیگ('V','M','T','X'): *success = hb_subset_جدول<ثابت OT::VMTX> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('M','A','X','P'): *success = hb_subset_جدول<ثابت OT::MAXP> (plan, buf); أرجع سچ;
#جے_انت
  حالة HB_ٹیگ('l','o','c','a'): *success = سچ; أرجع سچ; /* skip loca, handled by glyf */
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('L','O','C','A'): *success = سچ; أرجع سچ; /* skip LOCA, handled by GLYF */
#جے_انت
  حالة HB_ٹیگ('c','m','a','p'): *success = hb_subset_جدول<ثابت OT::cmap> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('D','M','A','P'): *success = hb_subset_جدول<ثابت OT::DMAP> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('O','S','/','2'): *success = hb_subset_جدول<ثابت OT::OS2 > (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('p','o','s','t'): *success = hb_subset_جدول<ثابت OT::post> (plan, buf); أرجع سچ;
  }
  أرجع خطأ;
}
