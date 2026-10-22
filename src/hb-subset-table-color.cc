#అన్తర్భావ "hb-subset-table.hh"

#అన్తర్భావ "OT/Color/sbix/sbix.hh"
#అన్తర్భావ "OT/Color/CPAL/CPAL.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/CBDT/CBDT.hh"

తార్కిక hb_subset_పట్టిక_color		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success)
{
#నిర్వచితం_కాకపోతే HB_NO_COLOR
  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('s','b','i','x'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::sbix> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('C','O','L','R'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::COLR> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('C','P','A','L'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::CPAL> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('C','B','L','C'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::CBLC> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('C','B','D','T'): *success = నిజం; తిరిగి నిజం; /* skip CBDT, handled by CBLC */
  }
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
