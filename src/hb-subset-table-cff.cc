#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-cff1-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
ಸಂರಚನಾ hb_subset_plan_t::source_table_loader<ಧ್ರುವ OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
ಸಂರಚನಾ hb_subset_plan_t::source_table_loader<ಧ್ರುವ OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


ತಾರ್ಕಿಕ _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<ವರ್ಣ> &buf, hb_tag_t tag, ತಾರ್ಕಿಕ *success)
{
#ifndef HB_NO_SUBSET_CFF
  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('C','F','F',' '): *success = _hb_subset_table<ಧ್ರುವ OT::cff1> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('C','F','F','2'): *success = _hb_subset_table<ಧ್ರುವ OT::cff2> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('V','O','R','G'): *success = _hb_subset_table<ಧ್ರುವ OT::VORG> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
#endif
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
ಸ್ಥಿರ hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  ಯದಿ (!accel.is_valid()) {
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  ಯದಿ (!bytes) {
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  ಧ್ರುವ ವರ್ಣ* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long ಪೂರ್ಣಾಙ್ಕ offset = (ಧ್ರುವ ವರ್ಣ*) bytes.arrayZ - cff_data;
  ಯದಿ (offset < 0 || offset > INT32_MAX) {
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

template<typename accel_t>
ಸ್ಥಿರ hb_blob_t* get_charstrings_index(accel_t& accel) {
  ಯದಿ (!accel.is_valid()) {
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  ಧ್ರುವ ವರ್ಣ* charstrings_start = (ಧ್ರುವ ವರ್ಣ*) accel.charStrings;
  unsigned charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  ಧ್ರುವ ವರ್ಣ* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long ಪೂರ್ಣಾಙ್ಕ offset = charstrings_start - cff_data;
  ಯದಿ (offset < 0 || offset > INT32_MAX) {
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
  }

  ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
  ಪ್ರತ್ಯಾಗಮ get_charstrings_data(*face->table.cff1, glyph);
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
  ಪ್ರತ್ಯಾಗಮ get_charstrings_index (*face->table.cff1);
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
  ಪ್ರತ್ಯಾಗಮ get_charstrings_data(*face->table.cff2, glyph);
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
  ಪ್ರತ್ಯಾಗಮ get_charstrings_index (*face->table.cff2);
}
#endif
#endif
