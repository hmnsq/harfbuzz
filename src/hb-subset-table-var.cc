#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-hvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-gvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-avar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-cvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-mvar-table.hh"
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Var/VARC/VARC-subset.hh"
#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_var		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('H','V','A','R'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::HVAR> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('V','V','A','R'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::VVAR> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('g','v','a','r'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::gvar> (plan, buf); ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('G','V','A','R'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::GVAR> (plan, buf); ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਹਾਲਤ HB_ਟੈਗ('V','A','R','C'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::VARC> (plan, buf); ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('f','v','a','r'):
    ਜੇ (plan->user_axes_location.is_empty ())
      *success = hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
    ਨਹੀਂਤਾਂ
      *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::fvar> (plan, buf);
    ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('a','v','a','r'):
    ਜੇ (plan->user_axes_location.is_empty ())
      *success = hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
    ਨਹੀਂਤਾਂ
      *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::avar> (plan, buf);
    ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ਜੇ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
    ਨਹੀਂਤਾਂ
      *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::cvar> (plan, buf);
    ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ਜੇ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
    ਨਹੀਂਤਾਂ
      *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::MVAR> (plan, buf);
    ਵਾਪਸ ਸੱਚ;
  }
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
