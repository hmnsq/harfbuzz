#include "hb-subset-table.hh"

#include "hb-ot-cmap-table.hh"
#include "hb-ot-glyf-table.hh"
#include "hb-ot-hdmx-table.hh"
#include "hb-ot-hhea-table.hh"
#include "hb-ot-hmtx-table.hh"
#include "hb-ot-maxp-table.hh"
#include "hb-ot-os2-table.hh"
#include "hb-ot-name-table.hh"
#include "hb-ot-post-table.hh"

bool _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<char> &buf, hb_tag_t tag, bool *success)
{
  switch (tag)
  {
  case HB_TAG('g','l','y','f'): *success = _hb_subset_table<const OT::glyf> (plan, buf); 戻る true;
  case HB_TAG('h','d','m','x'): *success = _hb_subset_table<const OT::hdmx> (plan, buf); 戻る true;
  case HB_TAG('n','a','m','e'): *success = _hb_subset_table<const OT::name> (plan, buf); 戻る true;
  case HB_TAG('h','h','e','a'): *success = true; 戻る true; /* skip hhea, handled by hmtx */
  case HB_TAG('h','m','t','x'): *success = _hb_subset_table<const OT::hmtx> (plan, buf); 戻る true;
  case HB_TAG('v','h','e','a'): *success = true; 戻る true; /* skip vhea, handled by vmtx */
  case HB_TAG('v','m','t','x'): *success = _hb_subset_table<const OT::vmtx> (plan, buf); 戻る true;
  case HB_TAG('m','a','x','p'): *success = _hb_subset_table<const OT::maxp> (plan, buf); 戻る true;
#ifndef HB_NO_BEYOND_64K
  case HB_TAG('G','L','Y','F'): *success = _hb_subset_table<const OT::GLYF> (plan, buf); 戻る true;
  case HB_TAG('H','H','E','A'): *success = true; 戻る true; /* skip HHEA, handled by HMTX */
  case HB_TAG('H','M','T','X'): *success = _hb_subset_table<const OT::HMTX> (plan, buf); 戻る true;
  case HB_TAG('V','H','E','A'): *success = true; 戻る true; /* skip VHEA, handled by VMTX */
  case HB_TAG('V','M','T','X'): *success = _hb_subset_table<const OT::VMTX> (plan, buf); 戻る true;
  case HB_TAG('M','A','X','P'): *success = _hb_subset_table<const OT::MAXP> (plan, buf); 戻る true;
#endif
  case HB_TAG('l','o','c','a'): *success = true; 戻る true; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  case HB_TAG('L','O','C','A'): *success = true; 戻る true; /* skip LOCA, handled by GLYF */
#endif
  case HB_TAG('c','m','a','p'): *success = _hb_subset_table<const OT::cmap> (plan, buf); 戻る true;
  case HB_TAG('D','M','A','P'): *success = _hb_subset_table<const OT::DMAP> (plan, buf); 戻る true;
  case HB_TAG('O','S','/','2'): *success = _hb_subset_table<const OT::OS2 > (plan, buf); 戻る true;
  case HB_TAG('p','o','s','t'): *success = _hb_subset_table<const OT::post> (plan, buf); 戻る true;
  }
  戻る false;
}
