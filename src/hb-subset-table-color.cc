#ضمن "hb-subset-table.hh"

#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

منطقي _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success)
{
#ifndef HB_NO_COLOR
  بدل (tag)
  {
  حالة HB_TAG('s','b','i','x'): *success = _hb_subset_table<ثابت OT::sbix> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('C','O','L','R'): *success = _hb_subset_table<ثابت OT::COLR> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('C','P','A','L'): *success = _hb_subset_table<ثابت OT::CPAL> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('C','B','L','C'): *success = _hb_subset_table<ثابت OT::CBLC> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('C','B','D','T'): *success = 𐡒𐡔𐡈; أرجع 𐡒𐡔𐡈; /* skip CBDT, handled by CBLC */
  }
#endif
  أرجع خطأ;
}
