#அந்தர்பாவ "hb-subset-table.hh"

#அந்தர்பாவ "hb-ot-var-hvar-table.hh"
#அந்தர்பாவ "hb-ot-var-gvar-table.hh"
#அந்தர்பாவ "hb-ot-var-fvar-table.hh"
#அந்தர்பாவ "hb-ot-var-avar-table.hh"
#அந்தர்பாவ "hb-ot-var-cvar-table.hh"
#அந்தர்பாவ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#அந்தர்பாவ "OT/Var/VARC/VARC-subset.hh"
#endif

தருக்க _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<எழுத்து> &buf, hb_tag_t tag, தருக்க *success)
{
#ifndef HB_NO_VAR
  தேர்வு (tag)
  {
  நிலை HB_TAG('H','V','A','R'): *success = _hb_subset_table<மாறிலி OT::HVAR> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('V','V','A','R'): *success = _hb_subset_table<மாறிலி OT::VVAR> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('g','v','a','r'): *success = _hb_subset_table<மாறிலி OT::gvar> (plan, buf); திரும்பு உண்மை;
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('G','V','A','R'): *success = _hb_subset_table<மாறிலி OT::GVAR> (plan, buf); திரும்பு உண்மை;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  நிலை HB_TAG('V','A','R','C'): *success = _hb_subset_table<மாறிலி OT::VARC> (plan, buf); திரும்பு உண்மை;
#endif
  நிலை HB_TAG('f','v','a','r'):
    எனில் (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    இல்லையெனில்
      *success = _hb_subset_table<மாறிலி OT::fvar> (plan, buf);
    திரும்பு உண்மை;
  நிலை HB_TAG('a','v','a','r'):
    எனில் (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    இல்லையெனில்
      *success = _hb_subset_table<மாறிலி OT::avar> (plan, buf);
    திரும்பு உண்மை;
  நிலை HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    எனில் (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    இல்லையெனில்
      *success = _hb_subset_table<மாறிலி OT::cvar> (plan, buf);
    திரும்பு உண்மை;
  நிலை HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    எனில் (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    இல்லையெனில்
      *success = _hb_subset_table<மாறிலி OT::MVAR> (plan, buf);
    திரும்பு உண்மை;
  }
#endif
  திரும்பு பொய்;
}
