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

منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success)
{
  بدل (برچسب_2)
  {
  حالة HB_برچسب('g','l','y','f'): *success = hb_subset_جدول<ثابت OT::glyf> (plan, buf); أرجع درست;
  حالة HB_برچسب('h','d','m','x'): *success = hb_subset_جدول<ثابت OT::hdmx> (plan, buf); أرجع درست;
  حالة HB_برچسب('n','a','m','e'): *success = hb_subset_جدول<ثابت OT::name> (plan, buf); أرجع درست;
  حالة HB_برچسب('h','h','e','a'): *success = درست; أرجع درست; /* skip hhea, handled by hmtx */
  حالة HB_برچسب('h','m','t','x'): *success = hb_subset_جدول<ثابت OT::hmtx> (plan, buf); أرجع درست;
  حالة HB_برچسب('v','h','e','a'): *success = درست; أرجع درست; /* skip vhea, handled by vmtx */
  حالة HB_برچسب('v','m','t','x'): *success = hb_subset_جدول<ثابت OT::vmtx> (plan, buf); أرجع درست;
  حالة HB_برچسب('m','a','x','p'): *success = hb_subset_جدول<ثابت OT::maxp> (plan, buf); أرجع درست;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('G','L','Y','F'): *success = hb_subset_جدول<ثابت OT::GLYF> (plan, buf); أرجع درست;
  حالة HB_برچسب('H','H','E','A'): *success = درست; أرجع درست; /* skip HHEA, handled by HMTX */
  حالة HB_برچسب('H','M','T','X'): *success = hb_subset_جدول<ثابت OT::HMTX> (plan, buf); أرجع درست;
  حالة HB_برچسب('V','H','E','A'): *success = درست; أرجع درست; /* skip VHEA, handled by VMTX */
  حالة HB_برچسب('V','M','T','X'): *success = hb_subset_جدول<ثابت OT::VMTX> (plan, buf); أرجع درست;
  حالة HB_برچسب('M','A','X','P'): *success = hb_subset_جدول<ثابت OT::MAXP> (plan, buf); أرجع درست;
#پایان_اگر
  حالة HB_برچسب('l','o','c','a'): *success = درست; أرجع درست; /* skip loca, handled by glyf */
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('L','O','C','A'): *success = درست; أرجع درست; /* skip LOCA, handled by GLYF */
#پایان_اگر
  حالة HB_برچسب('c','m','a','p'): *success = hb_subset_جدول<ثابت OT::cmap> (plan, buf); أرجع درست;
  حالة HB_برچسب('D','M','A','P'): *success = hb_subset_جدول<ثابت OT::DMAP> (plan, buf); أرجع درست;
  حالة HB_برچسب('O','S','/','2'): *success = hb_subset_جدول<ثابت OT::OS2 > (plan, buf); أرجع درست;
  حالة HB_برچسب('p','o','s','t'): *success = hb_subset_جدول<ثابت OT::post> (plan, buf); أرجع درست;
  }
  أرجع خطأ;
}
