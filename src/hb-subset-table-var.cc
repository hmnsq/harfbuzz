#شامل "hb-subset-table.hh"

#شامل "hb-ot-var-hvar-table.hh"
#شامل "hb-ot-var-gvar-table.hh"
#شامل "hb-ot-var-fvar-table.hh"
#شامل "hb-ot-var-avar-table.hh"
#شامل "hb-ot-var-cvar-table.hh"
#شامل "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#شامل "OT/Var/VARC/VARC-subset.hh"
#endif

منطقی _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<نویسه> &buf, hb_tag_t tag, منطقی *success)
{
#ifndef HB_NO_VAR
  انتخاب (tag)
  {
  حالت HB_TAG('H','V','A','R'): *success = _hb_subset_table<ثابت OT::HVAR> (plan, buf); بازگشت درست;
  حالت HB_TAG('V','V','A','R'): *success = _hb_subset_table<ثابت OT::VVAR> (plan, buf); بازگشت درست;
  حالت HB_TAG('g','v','a','r'): *success = _hb_subset_table<ثابت OT::gvar> (plan, buf); بازگشت درست;
#ifndef HB_NO_BEYOND_64K
  حالت HB_TAG('G','V','A','R'): *success = _hb_subset_table<ثابت OT::GVAR> (plan, buf); بازگشت درست;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  حالت HB_TAG('V','A','R','C'): *success = _hb_subset_table<ثابت OT::VARC> (plan, buf); بازگشت درست;
#endif
  حالت HB_TAG('f','v','a','r'):
    اگر (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    وگرنه
      *success = _hb_subset_table<ثابت OT::fvar> (plan, buf);
    بازگشت درست;
  حالت HB_TAG('a','v','a','r'):
    اگر (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    وگرنه
      *success = _hb_subset_table<ثابت OT::avar> (plan, buf);
    بازگشت درست;
  حالت HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    اگر (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    وگرنه
      *success = _hb_subset_table<ثابت OT::cvar> (plan, buf);
    بازگشت درست;
  حالت HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    اگر (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    وگرنه
      *success = _hb_subset_table<ثابت OT::MVAR> (plan, buf);
    بازگشت درست;
  }
#endif
  بازگشت نادرست;
}
