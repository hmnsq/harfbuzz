#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#ضمن "OT/Var/VARC/VARC-subset.hh"
#endif

منطقي _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success)
{
#ifndef HB_NO_VAR
  بدل (tag)
  {
  حالة HB_TAG('H','V','A','R'): *success = _hb_subset_table<ثابت OT::HVAR> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('V','V','A','R'): *success = _hb_subset_table<ثابت OT::VVAR> (plan, buf); ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('g','v','a','r'): *success = _hb_subset_table<ثابت OT::gvar> (plan, buf); ܦܢܐ ܫܪܪܐ;
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','V','A','R'): *success = _hb_subset_table<ثابت OT::GVAR> (plan, buf); ܦܢܐ ܫܪܪܐ;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  حالة HB_TAG('V','A','R','C'): *success = _hb_subset_table<ثابت OT::VARC> (plan, buf); ܦܢܐ ܫܪܪܐ;
#endif
  حالة HB_TAG('f','v','a','r'):
    ܐܢ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ܐܚܪܢܐ
      *success = _hb_subset_table<ثابت OT::fvar> (plan, buf);
    ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('a','v','a','r'):
    ܐܢ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ܐܚܪܢܐ
      *success = _hb_subset_table<ثابت OT::avar> (plan, buf);
    ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ܐܢ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ܐܚܪܢܐ
      *success = _hb_subset_table<ثابت OT::cvar> (plan, buf);
    ܦܢܐ ܫܪܪܐ;
  حالة HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ܐܢ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ܐܚܪܢܐ
      *success = _hb_subset_table<ثابت OT::MVAR> (plan, buf);
    ܦܢܐ ܫܪܪܐ;
  }
#endif
  ܦܢܐ ܕܓܠܐ;
}
