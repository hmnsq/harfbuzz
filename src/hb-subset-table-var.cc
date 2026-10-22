#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-var-hvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-gvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-fvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-avar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-cvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#ಅನ್ತರ್ಭಾವ "OT/Var/VARC/VARC-subset.hh"
#endif

ತಾರ್ಕಿಕ _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<ವರ್ಣ> &buf, hb_tag_t tag, ತಾರ್ಕಿಕ *success)
{
#ifndef HB_NO_VAR
  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('H','V','A','R'): *success = _hb_subset_table<ಧ್ರುವ OT::HVAR> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('V','V','A','R'): *success = _hb_subset_table<ಧ್ರುವ OT::VVAR> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('g','v','a','r'): *success = _hb_subset_table<ಧ್ರುವ OT::gvar> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('G','V','A','R'): *success = _hb_subset_table<ಧ್ರುವ OT::GVAR> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  ಪಕ್ಷ HB_TAG('V','A','R','C'): *success = _hb_subset_table<ಧ್ರುವ OT::VARC> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
  ಪಕ್ಷ HB_TAG('f','v','a','r'):
    ಯದಿ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ಅನ್ಯಥಾ
      *success = _hb_subset_table<ಧ್ರುವ OT::fvar> (plan, buf);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('a','v','a','r'):
    ಯದಿ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ಅನ್ಯಥಾ
      *success = _hb_subset_table<ಧ್ರುವ OT::avar> (plan, buf);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ಯದಿ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ಅನ್ಯಥಾ
      *success = _hb_subset_table<ಧ್ರುವ OT::cvar> (plan, buf);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ಯದಿ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ಅನ್ಯಥಾ
      *success = _hb_subset_table<ಧ್ರುವ OT::MVAR> (plan, buf);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
#endif
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
