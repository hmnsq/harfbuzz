#شامل "hb-subset-table.hh"

#شامل "hb-ot-cmap-table.hh"
#شامل "hb-ot-glyf-table.hh"
#شامل "hb-ot-hdmx-table.hh"
#شامل "hb-ot-hhea-table.hh"
#شامل "hb-ot-hmtx-table.hh"
#شامل "hb-ot-maxp-table.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-name-table.hh"
#شامل "hb-ot-post-table.hh"

منطقی hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success)
{
  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('g','l','y','f'): *success = hb_subset_جدول<ثابت OT::glyf> (plan, buf); واپس سچ;
  حالت HB_لیبل('h','d','m','x'): *success = hb_subset_جدول<ثابت OT::hdmx> (plan, buf); واپس سچ;
  حالت HB_لیبل('n','a','m','e'): *success = hb_subset_جدول<ثابت OT::name> (plan, buf); واپس سچ;
  حالت HB_لیبل('h','h','e','a'): *success = سچ; واپس سچ; /* skip hhea, handled by hmtx */
  حالت HB_لیبل('h','m','t','x'): *success = hb_subset_جدول<ثابت OT::hmtx> (plan, buf); واپس سچ;
  حالت HB_لیبل('v','h','e','a'): *success = سچ; واپس سچ; /* skip vhea, handled by vmtx */
  حالت HB_لیبل('v','m','t','x'): *success = hb_subset_جدول<ثابت OT::vmtx> (plan, buf); واپس سچ;
  حالت HB_لیبل('m','a','x','p'): *success = hb_subset_جدول<ثابت OT::maxp> (plan, buf); واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('G','L','Y','F'): *success = hb_subset_جدول<ثابت OT::GLYF> (plan, buf); واپس سچ;
  حالت HB_لیبل('H','H','E','A'): *success = سچ; واپس سچ; /* skip HHEA, handled by HMTX */
  حالت HB_لیبل('H','M','T','X'): *success = hb_subset_جدول<ثابت OT::HMTX> (plan, buf); واپس سچ;
  حالت HB_لیبل('V','H','E','A'): *success = سچ; واپس سچ; /* skip VHEA, handled by VMTX */
  حالت HB_لیبل('V','M','T','X'): *success = hb_subset_جدول<ثابت OT::VMTX> (plan, buf); واپس سچ;
  حالت HB_لیبل('M','A','X','P'): *success = hb_subset_جدول<ثابت OT::MAXP> (plan, buf); واپس سچ;
#اگر_ختم
  حالت HB_لیبل('l','o','c','a'): *success = سچ; واپس سچ; /* skip loca, handled by glyf */
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('L','O','C','A'): *success = سچ; واپس سچ; /* skip LOCA, handled by GLYF */
#اگر_ختم
  حالت HB_لیبل('c','m','a','p'): *success = hb_subset_جدول<ثابت OT::cmap> (plan, buf); واپس سچ;
  حالت HB_لیبل('D','M','A','P'): *success = hb_subset_جدول<ثابت OT::DMAP> (plan, buf); واپس سچ;
  حالت HB_لیبل('O','S','/','2'): *success = hb_subset_جدول<ثابت OT::OS2 > (plan, buf); واپس سچ;
  حالت HB_لیبل('p','o','s','t'): *success = hb_subset_جدول<ثابت OT::post> (plan, buf); واپس سچ;
  }
  واپس جھوٹ;
}
