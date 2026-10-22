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

منطقي _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success)
{
  بدل (tag)
  {
  حالة HB_TAG('g','l','y','f'): *success = _hb_subset_table<ثابت OT::glyf> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('h','d','m','x'): *success = _hb_subset_table<ثابت OT::hdmx> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('n','a','m','e'): *success = _hb_subset_table<ثابت OT::name> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('h','h','e','a'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip hhea, handled by hmtx */
  حالة HB_TAG('h','m','t','x'): *success = _hb_subset_table<ثابت OT::hmtx> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('v','h','e','a'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip vhea, handled by vmtx */
  حالة HB_TAG('v','m','t','x'): *success = _hb_subset_table<ثابت OT::vmtx> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('m','a','x','p'): *success = _hb_subset_table<ثابت OT::maxp> (plan, buf); ܦܢܐ ܫܪܪܐ;
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ثابت OT::GLYF> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('H','H','E','A'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip HHEA, handled by HMTX */
  حالة HB_TAG('H','M','T','X'): *success = _hb_subset_table<ثابت OT::HMTX> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('V','H','E','A'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip VHEA, handled by VMTX */
  حالة HB_TAG('V','M','T','X'): *success = _hb_subset_table<ثابت OT::VMTX> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('M','A','X','P'): *success = _hb_subset_table<ثابت OT::MAXP> (plan, buf); ܦܢܐ ܫܪܪܐ;
#endif
  حالة HB_TAG('l','o','c','a'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('L','O','C','A'): *success = ܫܪܪܐ; ܦܢܐ ܫܪܪܐ; /* skip LOCA, handled by GLYF */
#endif
  حالة HB_TAG('c','m','a','p'): *success = _hb_subset_table<ثابت OT::cmap> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('D','M','A','P'): *success = _hb_subset_table<ثابت OT::DMAP> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('O','S','/','2'): *success = _hb_subset_table<ثابت OT::OS2 > (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('p','o','s','t'): *success = _hb_subset_table<ثابت OT::post> (plan, buf); ܦܢܐ ܫܪܪܐ;
  }
  ܦܢܐ ܕܓܠܐ;
}
