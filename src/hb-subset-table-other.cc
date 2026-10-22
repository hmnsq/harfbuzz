#అన్తర్భావ "hb-subset-table.hh"

#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-hdmx-table.hh"
#అన్తర్భావ "hb-ot-hhea-table.hh"
#అన్తర్భావ "hb-ot-hmtx-table.hh"
#అన్తర్భావ "hb-ot-maxp-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-name-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"

తార్కిక hb_subset_పట్టిక_other		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success)
{
  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('g','l','y','f'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::glyf> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('h','d','m','x'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::hdmx> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('n','a','m','e'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::name> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('h','h','e','a'): *success = నిజం; తిరిగి నిజం; /* skip hhea, handled by hmtx */
  స్థితి HB_ట్యాగ్('h','m','t','x'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::hmtx> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('v','h','e','a'): *success = నిజం; తిరిగి నిజం; /* skip vhea, handled by vmtx */
  స్థితి HB_ట్యాగ్('v','m','t','x'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::vmtx> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('m','a','x','p'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::maxp> (plan, buf); తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('G','L','Y','F'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::GLYF> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('H','H','E','A'): *success = నిజం; తిరిగి నిజం; /* skip HHEA, handled by HMTX */
  స్థితి HB_ట్యాగ్('H','M','T','X'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::HMTX> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('V','H','E','A'): *success = నిజం; తిరిగి నిజం; /* skip VHEA, handled by VMTX */
  స్థితి HB_ట్యాగ్('V','M','T','X'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::VMTX> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('M','A','X','P'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::MAXP> (plan, buf); తిరిగి నిజం;
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('l','o','c','a'): *success = నిజం; తిరిగి నిజం; /* skip loca, handled by glyf */
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('L','O','C','A'): *success = నిజం; తిరిగి నిజం; /* skip LOCA, handled by GLYF */
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('c','m','a','p'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::cmap> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('D','M','A','P'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::DMAP> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('O','S','/','2'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::OS2 > (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('p','o','s','t'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::post> (plan, buf); తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}
