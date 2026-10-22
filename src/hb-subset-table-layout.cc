#అన్తర్భావ "hb-subset-table.hh"

#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"
#అన్తర్భావ "hb-ot-layout-base-table.hh"
#అన్తర్భావ "hb-ot-math-table.hh"

తార్కిక hb_subset_పట్టిక_layout		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success)
{
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT
  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('G','D','E','F'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::GDEF> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('G','S','U','B'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::Layout::GSUB> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('G','P','O','S'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::Layout::GPOS> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('B','A','S','E'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::పునాది_4> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('M','A','T','H'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::MATH> (plan, buf); తిరిగి నిజం;
  }
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
