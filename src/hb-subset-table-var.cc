#అన్తర్భావ "hb-subset-table.hh"

#అన్తర్భావ "hb-ot-var-hvar-table.hh"
#అన్తర్భావ "hb-ot-var-gvar-table.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"
#అన్తర్భావ "hb-ot-var-avar-table.hh"
#అన్తర్భావ "hb-ot-var-cvar-table.hh"
#అన్తర్భావ "hb-ot-var-mvar-table.hh"
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
#అన్తర్భావ "OT/Var/VARC/VARC-subset.hh"
#అయితే_ముగింపు

తార్కిక hb_subset_పట్టిక_var		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success)
{
#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('H','V','A','R'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::HVAR> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('V','V','A','R'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::VVAR> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('g','v','a','r'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::gvar> (plan, buf); తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('G','V','A','R'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::GVAR> (plan, buf); తిరిగి నిజం;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  స్థితి HB_ట్యాగ్('V','A','R','C'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::VARC> (plan, buf); తిరిగి నిజం;
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('f','v','a','r'):
    ఒకవేళ (plan->user_axes_location.is_empty ())
      *success = hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
    లేకపోతే
      *success = hb_subset_పట్టిక<స్థిరరాశి OT::fvar> (plan, buf);
    తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('a','v','a','r'):
    ఒకవేళ (plan->user_axes_location.is_empty ())
      *success = hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
    లేకపోతే
      *success = hb_subset_పట్టిక<స్థిరరాశి OT::avar> (plan, buf);
    తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ఒకవేళ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
    లేకపోతే
      *success = hb_subset_పట్టిక<స్థిరరాశి OT::cvar> (plan, buf);
    తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ఒకవేళ (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
    లేకపోతే
      *success = hb_subset_పట్టిక<స్థిరరాశి OT::MVAR> (plan, buf);
    తిరిగి నిజం;
  }
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
