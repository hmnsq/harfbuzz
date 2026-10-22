#ଅନ୍ତର୍ଭାଵ "hb-subset-table.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-var-hvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-gvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-avar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-cvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#ଅନ୍ତର୍ଭାଵ "OT/Var/VARC/VARC-subset.hh"
#endif

ତାର୍କିକ _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success)
{
#ifndef HB_NO_VAR
  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('H','V','A','R'): *success = _hb_subset_table<ଅଚଳ OT::HVAR> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('V','V','A','R'): *success = _hb_subset_table<ଅଚଳ OT::VVAR> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('g','v','a','r'): *success = _hb_subset_table<ଅଚଳ OT::gvar> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('G','V','A','R'): *success = _hb_subset_table<ଅଚଳ OT::GVAR> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  ସ୍ଥିତି HB_TAG('V','A','R','C'): *success = _hb_subset_table<ଅଚଳ OT::VARC> (plan, buf); ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  ସ୍ଥିତି HB_TAG('f','v','a','r'):
    ଯଦି (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ଅନ୍ୟଥା
      *success = _hb_subset_table<ଅଚଳ OT::fvar> (plan, buf);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('a','v','a','r'):
    ଯଦି (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    ଅନ୍ୟଥା
      *success = _hb_subset_table<ଅଚଳ OT::avar> (plan, buf);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ଯଦି (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ଅନ୍ୟଥା
      *success = _hb_subset_table<ଅଚଳ OT::cvar> (plan, buf);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  ସ୍ଥିତି HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ଯଦି (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    ଅନ୍ୟଥା
      *success = _hb_subset_table<ଅଚଳ OT::MVAR> (plan, buf);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
