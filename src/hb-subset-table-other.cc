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

منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success)
{
  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('g','l','y','f'): *success = hb_subset_جدول<ثابت OT::glyf> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('h','d','m','x'): *success = hb_subset_جدول<ثابت OT::hdmx> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('n','a','m','e'): *success = hb_subset_جدول<ثابت OT::name> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('h','h','e','a'): *success = سچ; أرجع سچ; /* skip hhea, handled by hmtx */
  حالة HB_ٽيگ('h','m','t','x'): *success = hb_subset_جدول<ثابت OT::hmtx> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('v','h','e','a'): *success = سچ; أرجع سچ; /* skip vhea, handled by vmtx */
  حالة HB_ٽيگ('v','m','t','x'): *success = hb_subset_جدول<ثابت OT::vmtx> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('m','a','x','p'): *success = hb_subset_جدول<ثابت OT::maxp> (plan, buf); أرجع سچ;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('G','L','Y','F'): *success = hb_subset_جدول<ثابت OT::GLYF> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('H','H','E','A'): *success = سچ; أرجع سچ; /* skip HHEA, handled by HMTX */
  حالة HB_ٽيگ('H','M','T','X'): *success = hb_subset_جدول<ثابت OT::HMTX> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('V','H','E','A'): *success = سچ; أرجع سچ; /* skip VHEA, handled by VMTX */
  حالة HB_ٽيگ('V','M','T','X'): *success = hb_subset_جدول<ثابت OT::VMTX> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('M','A','X','P'): *success = hb_subset_جدول<ثابت OT::MAXP> (plan, buf); أرجع سچ;
#جيڪڏهن_پورو
  حالة HB_ٽيگ('l','o','c','a'): *success = سچ; أرجع سچ; /* skip loca, handled by glyf */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('L','O','C','A'): *success = سچ; أرجع سچ; /* skip LOCA, handled by GLYF */
#جيڪڏهن_پورو
  حالة HB_ٽيگ('c','m','a','p'): *success = hb_subset_جدول<ثابت OT::cmap> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('D','M','A','P'): *success = hb_subset_جدول<ثابت OT::DMAP> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('O','S','/','2'): *success = hb_subset_جدول<ثابت OT::OS2 > (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('p','o','s','t'): *success = hb_subset_جدول<ثابت OT::post> (plan, buf); أرجع سچ;
  }
  أرجع خطأ;
}
