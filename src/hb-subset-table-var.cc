#અન્તર્ભાવ "hb-subset-table.hh"

#અન્તર્ભાવ "hb-ot-var-hvar-table.hh"
#અન્તર્ભાવ "hb-ot-var-gvar-table.hh"
#અન્તર્ભાવ "hb-ot-var-fvar-table.hh"
#અન્તર્ભાવ "hb-ot-var-avar-table.hh"
#અન્તર્ભાવ "hb-ot-var-cvar-table.hh"
#અન્તર્ભાવ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#અન્તર્ભાવ "OT/Var/VARC/VARC-subset.hh"
#endif

તાર્કિક _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<અક્ષર> &buf, hb_tag_t tag, તાર્કિક *success)
{
#ifndef HB_NO_VAR
  પસંદગી (tag)
  {
  સ્થિતિ HB_TAG('H','V','A','R'): *success = _hb_subset_table<અચલ OT::HVAR> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('V','V','A','R'): *success = _hb_subset_table<અચલ OT::VVAR> (plan, buf); પાછા સાચું;
  સ્થિતિ HB_TAG('g','v','a','r'): *success = _hb_subset_table<અચલ OT::gvar> (plan, buf); પાછા સાચું;
#ifndef HB_NO_BEYOND_64K
  સ્થિતિ HB_TAG('G','V','A','R'): *success = _hb_subset_table<અચલ OT::GVAR> (plan, buf); પાછા સાચું;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  સ્થિતિ HB_TAG('V','A','R','C'): *success = _hb_subset_table<અચલ OT::VARC> (plan, buf); પાછા સાચું;
#endif
  સ્થિતિ HB_TAG('f','v','a','r'):
    જો (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    નહીંતર
      *success = _hb_subset_table<અચલ OT::fvar> (plan, buf);
    પાછા સાચું;
  સ્થિતિ HB_TAG('a','v','a','r'):
    જો (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    નહીંતર
      *success = _hb_subset_table<અચલ OT::avar> (plan, buf);
    પાછા સાચું;
  સ્થિતિ HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    જો (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    નહીંતર
      *success = _hb_subset_table<અચલ OT::cvar> (plan, buf);
    પાછા સાચું;
  સ્થિતિ HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    જો (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    નહીંતર
      *success = _hb_subset_table<અચલ OT::MVAR> (plan, buf);
    પાછા સાચું;
  }
#endif
  પાછા ખોટું;
}
