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

منطقی _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<نویسه> &buf, hb_tag_t tag, منطقی *success)
{
  انتخاب (tag)
  {
  حالت HB_TAG('g','l','y','f'): *success = _hb_subset_table<ثابت OT::glyf> (plan, buf); بازگشت درست;
  حالت HB_TAG('h','d','m','x'): *success = _hb_subset_table<ثابت OT::hdmx> (plan, buf); بازگشت درست;
  حالت HB_TAG('n','a','m','e'): *success = _hb_subset_table<ثابت OT::name> (plan, buf); بازگشت درست;
  حالت HB_TAG('h','h','e','a'): *success = درست; بازگشت درست; /* skip hhea, handled by hmtx */
  حالت HB_TAG('h','m','t','x'): *success = _hb_subset_table<ثابت OT::hmtx> (plan, buf); بازگشت درست;
  حالت HB_TAG('v','h','e','a'): *success = درست; بازگشت درست; /* skip vhea, handled by vmtx */
  حالت HB_TAG('v','m','t','x'): *success = _hb_subset_table<ثابت OT::vmtx> (plan, buf); بازگشت درست;
  حالت HB_TAG('m','a','x','p'): *success = _hb_subset_table<ثابت OT::maxp> (plan, buf); بازگشت درست;
#ifndef HB_NO_BEYOND_64K
  حالت HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ثابت OT::GLYF> (plan, buf); بازگشت درست;
  حالت HB_TAG('H','H','E','A'): *success = درست; بازگشت درست; /* skip HHEA, handled by HMTX */
  حالت HB_TAG('H','M','T','X'): *success = _hb_subset_table<ثابت OT::HMTX> (plan, buf); بازگشت درست;
  حالت HB_TAG('V','H','E','A'): *success = درست; بازگشت درست; /* skip VHEA, handled by VMTX */
  حالت HB_TAG('V','M','T','X'): *success = _hb_subset_table<ثابت OT::VMTX> (plan, buf); بازگشت درست;
  حالت HB_TAG('M','A','X','P'): *success = _hb_subset_table<ثابت OT::MAXP> (plan, buf); بازگشت درست;
#endif
  حالت HB_TAG('l','o','c','a'): *success = درست; بازگشت درست; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  حالت HB_TAG('L','O','C','A'): *success = درست; بازگشت درست; /* skip LOCA, handled by GLYF */
#endif
  حالت HB_TAG('c','m','a','p'): *success = _hb_subset_table<ثابت OT::cmap> (plan, buf); بازگشت درست;
  حالت HB_TAG('D','M','A','P'): *success = _hb_subset_table<ثابت OT::DMAP> (plan, buf); بازگشت درست;
  حالت HB_TAG('O','S','/','2'): *success = _hb_subset_table<ثابت OT::OS2 > (plan, buf); بازگشت درست;
  حالت HB_TAG('p','o','s','t'): *success = _hb_subset_table<ثابت OT::post> (plan, buf); بازگشت درست;
  }
  بازگشت نادرست;
}
