#అన్తర్భావ "hb-subset-table.hh"

#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-ot-vorg-table.hh"


#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
మూస<>
నిర్మాణం hb_subset_plan_t::ఆధారం_పట్టిక_loader<స్థిరరాశి OT::cff1>
{
  స్వయంచాలక క్రియాకారకం () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
మూస<>
నిర్మాణం hb_subset_plan_t::ఆధారం_పట్టిక_loader<స్థిరరాశి OT::cff2>
{
  స్వయంచాలక క్రియాకారకం () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#అయితే_ముగింపు


తార్కిక hb_subset_పట్టిక_cff		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success)
{
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('C','F','F',' '): *success = hb_subset_పట్టిక<స్థిరరాశి OT::cff1> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('C','F','F','2'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::cff2> (plan, buf); తిరిగి నిజం;
  స్థితి HB_ట్యాగ్('V','O','R','G'): *success = hb_subset_పట్టిక<స్థిరరాశి OT::VORG> (plan, buf); తిరిగి నిజం;
  }
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}


#నిర్వచితమైతే HB_EXPERIMENTAL_API
#నిర్వచితం_కాకపోతే HB_NO_CFF

మూస<రకం_పేరు accel_t>
స్థిర hb_blob_t* get_charstrings_దత్తాంశం(accel_t& accel, hb_codepoint_t glyph_సూచీ) {
  ఒకవేళ (!accel.is_valid()) {
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  hb_ubytes_t బైట్లు = (*accel.char_అక్షరశ్రేణులు)[glyph_సూచీ];
  ఒకవేళ (!బైట్లు) {
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t నిడివి;
  స్థిరరాశి అక్షరం* cff_దత్తాంశం = hb_blob_get_దత్తాంశం(cff_blob, &నిడివి) ;

  దీర్ఘ పూర్ణాంకం స్థానభ్రంశం = (స్థిరరాశి అక్షరం*) బైట్లు.arrayZ - cff_దత్తాంశం;
  ఒకవేళ (స్థానభ్రంశం < 0 || స్థానభ్రంశం > INT32_MAX) {
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  తిరిగి hb_blob_create_sub_blob(cff_blob, (uint32_t) స్థానభ్రంశం, బైట్లు.నిడివి);
}

మూస<రకం_పేరు accel_t>
స్థిర hb_blob_t* get_charstrings_సూచీ(accel_t& accel) {
  ఒకవేళ (!accel.is_valid()) {
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  స్థిరరాశి అక్షరం* charstrings_ప్రారంభం = (స్థిరరాశి అక్షరం*) accel.char_అక్షరశ్రేణులు;
  గుర్తులేని charstrings_నిడివి = accel.char_అక్షరశ్రేణులు->get_కొలత();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t నిడివి;
  స్థిరరాశి అక్షరం* cff_దత్తాంశం = hb_blob_get_దత్తాంశం(cff_blob, &నిడివి) ;

  దీర్ఘ పూర్ణాంకం స్థానభ్రంశం = charstrings_ప్రారంభం - cff_దత్తాంశం;
  ఒకవేళ (స్థానభ్రంశం < 0 || స్థానభ్రంశం > INT32_MAX) {
    తిరిగి hb_blob_get_ఖాళీలు ();
  }

  తిరిగి hb_blob_create_sub_blob(cff_blob, (uint32_t) స్థానభ్రంశం, charstrings_నిడివి);
}

/**
 * hb_subset_cff_get_charstring_data:
 * @face: A face object
 * @glyph: Glyph index to get data for.
 *
 * Returns the raw outline data from the CFF/CFF2 table associated with the given glyph index.
 *
 * XSince: EXPERIMENTAL
 **/
HB_EXTERN hb_blob_t*
hb_subset_cff_get_charstring_దత్తాంశం(hb_face_t* face, hb_codepoint_t glyph) {
  తిరిగి get_charstrings_దత్తాంశం(*face->పట్టిక.cff1, glyph);
}

/**
 * hb_subset_cff_get_charstrings_index:
 * @face: A face object
 *
 * Returns the raw CFF CharStrings INDEX from the CFF table.
 *
 * XSince: EXPERIMENTAL
 **/
HB_EXTERN hb_blob_t*
hb_subset_cff_get_charstrings_సూచీ (hb_face_t* face) {
  తిరిగి get_charstrings_సూచీ (*face->పట్టిక.cff1);
}

/**
 * hb_subset_cff2_get_charstring_data:
 * @face: A face object
 * @glyph: Glyph index to get data for.
 *
 * Returns the raw outline data from the CFF/CFF2 table associated with the given glyph index.
 *
 * XSince: EXPERIMENTAL
 **/
HB_EXTERN hb_blob_t*
hb_subset_cff2_get_charstring_దత్తాంశం(hb_face_t* face, hb_codepoint_t glyph) {
  తిరిగి get_charstrings_దత్తాంశం(*face->పట్టిక.cff2, glyph);
}

/**
 * hb_subset_cff2_get_charstrings_index:
 * @face: A face object
 *
 * Returns the raw CFF2 CharStrings INDEX from the CFF2 table.
 *
 * XSince: EXPERIMENTAL
 **/
HB_EXTERN hb_blob_t*
hb_subset_cff2_get_charstrings_సూచీ (hb_face_t* face) {
  తిరిగి get_charstrings_సూచీ (*face->పట్టిక.cff2);
}
#అయితే_ముగింపు
#అయితే_ముగింపు
