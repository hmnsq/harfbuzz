#அந்தர்பாவ "hb-subset-table.hh"

#அந்தர்பாவ "hb-ot-cff1-table.hh"
#அந்தர்பாவ "hb-ot-cff2-table.hh"
#அந்தர்பாவ "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
கட்டமைப்பு hb_subset_plan_t::source_table_loader<மாறிலி OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
கட்டமைப்பு hb_subset_plan_t::source_table_loader<மாறிலி OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


தருக்க _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<எழுத்து> &buf, hb_tag_t tag, தருக்க *success)
{
#ifndef HB_NO_SUBSET_CFF
  தேர்வு (tag)
  {
  நிலை HB_TAG('C','F','F',' '): *success = _hb_subset_table<மாறிலி OT::cff1> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('C','F','F','2'): *success = _hb_subset_table<மாறிலி OT::cff2> (plan, buf); திரும்பு உண்மை;
  நிலை HB_TAG('V','O','R','G'): *success = _hb_subset_table<மாறிலி OT::VORG> (plan, buf); திரும்பு உண்மை;
  }
#endif
  திரும்பு பொய்;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
நிலையான hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  எனில் (!accel.is_valid()) {
    திரும்பு hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  எனில் (!bytes) {
    திரும்பு hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  மாறிலி எழுத்து* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long முழு offset = (மாறிலி எழுத்து*) bytes.arrayZ - cff_data;
  எனில் (offset < 0 || offset > INT32_MAX) {
    திரும்பு hb_blob_get_empty ();
  }

  திரும்பு hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

template<typename accel_t>
நிலையான hb_blob_t* get_charstrings_index(accel_t& accel) {
  எனில் (!accel.is_valid()) {
    திரும்பு hb_blob_get_empty ();
  }

  மாறிலி எழுத்து* charstrings_start = (மாறிலி எழுத்து*) accel.charStrings;
  unsigned charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  மாறிலி எழுத்து* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long முழு offset = charstrings_start - cff_data;
  எனில் (offset < 0 || offset > INT32_MAX) {
    திரும்பு hb_blob_get_empty ();
  }

  திரும்பு hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
hb_subset_cff_get_charstring_data(hb_face_t* face, hb_codepoint_t glyph) {
  திரும்பு get_charstrings_data(*face->table.cff1, glyph);
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
hb_subset_cff_get_charstrings_index (hb_face_t* face) {
  திரும்பு get_charstrings_index (*face->table.cff1);
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
hb_subset_cff2_get_charstring_data(hb_face_t* face, hb_codepoint_t glyph) {
  திரும்பு get_charstrings_data(*face->table.cff2, glyph);
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
hb_subset_cff2_get_charstrings_index (hb_face_t* face) {
  திரும்பு get_charstrings_index (*face->table.cff2);
}
#endif
#endif
