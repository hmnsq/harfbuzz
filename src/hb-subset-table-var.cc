#അന്തര്ഭാവ "hb-subset-table.hh"

#അന്തര്ഭാവ "hb-ot-var-hvar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-gvar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-fvar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-avar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-cvar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#അന്തര്ഭാവ "OT/Var/VARC/VARC-subset.hh"
#endif

തർക്കം _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<അക്ഷരം> &buf, hb_tag_t tag, തർക്കം *success)
{
#ifndef HB_NO_VAR
  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('H','V','A','R'): *success = _hb_subset_table<സ്ഥിരം OT::HVAR> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('V','V','A','R'): *success = _hb_subset_table<സ്ഥിരം OT::VVAR> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('g','v','a','r'): *success = _hb_subset_table<സ്ഥിരം OT::gvar> (plan, buf); തിരികെ സത്യം;
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('G','V','A','R'): *success = _hb_subset_table<സ്ഥിരം OT::GVAR> (plan, buf); തിരികെ സത്യം;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  അവസ്ഥ HB_TAG('V','A','R','C'): *success = _hb_subset_table<സ്ഥിരം OT::VARC> (plan, buf); തിരികെ സത്യം;
#endif
  അവസ്ഥ HB_TAG('f','v','a','r'):
    എങ്കിൽ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    അല്ലെങ്കിൽ
      *success = _hb_subset_table<സ്ഥിരം OT::fvar> (plan, buf);
    തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('a','v','a','r'):
    എങ്കിൽ (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    അല്ലെങ്കിൽ
      *success = _hb_subset_table<സ്ഥിരം OT::avar> (plan, buf);
    തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    എങ്കിൽ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    അല്ലെങ്കിൽ
      *success = _hb_subset_table<സ്ഥിരം OT::cvar> (plan, buf);
    തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    എങ്കിൽ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    അല്ലെങ്കിൽ
      *success = _hb_subset_table<സ്ഥിരം OT::MVAR> (plan, buf);
    തിരികെ സത്യം;
  }
#endif
  തിരികെ അസത്യം;
}
