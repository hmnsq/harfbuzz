#അന്തര്ഭാവ "hb-subset-table.hh"

#അന്തര്ഭാവ "hb-ot-cff1-table.hh"
#അന്തര്ഭാവ "hb-ot-cff2-table.hh"
#അന്തര്ഭാവ "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
ഘടന hb_subset_plan_t::source_table_loader<സ്ഥിരം OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
ഘടന hb_subset_plan_t::source_table_loader<സ്ഥിരം OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


തർക്കം _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<അക്ഷരം> &buf, hb_tag_t tag, തർക്കം *success)
{
#ifndef HB_NO_SUBSET_CFF
  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('C','F','F',' '): *success = _hb_subset_table<സ്ഥിരം OT::cff1> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('C','F','F','2'): *success = _hb_subset_table<സ്ഥിരം OT::cff2> (plan, buf); തിരികെ സത്യം;
  അവസ്ഥ HB_TAG('V','O','R','G'): *success = _hb_subset_table<സ്ഥിരം OT::VORG> (plan, buf); തിരികെ സത്യം;
  }
#endif
  തിരികെ അസത്യം;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
സ്ഥിതം hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  എങ്കിൽ (!accel.is_valid()) {
    തിരികെ hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  എങ്കിൽ (!bytes) {
    തിരികെ hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  സ്ഥിരം അക്ഷരം* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long പൂർണ്ണസംഖ്യ offset = (സ്ഥിരം അക്ഷരം*) bytes.arrayZ - cff_data;
  എങ്കിൽ (offset < 0 || offset > INT32_MAX) {
    തിരികെ hb_blob_get_empty ();
  }

  തിരികെ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

template<typename accel_t>
സ്ഥിതം hb_blob_t* get_charstrings_index(accel_t& accel) {
  എങ്കിൽ (!accel.is_valid()) {
    തിരികെ hb_blob_get_empty ();
  }

  സ്ഥിരം അക്ഷരം* charstrings_start = (സ്ഥിരം അക്ഷരം*) accel.charStrings;
  unsigned charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  സ്ഥിരം അക്ഷരം* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long പൂർണ്ണസംഖ്യ offset = charstrings_start - cff_data;
  എങ്കിൽ (offset < 0 || offset > INT32_MAX) {
    തിരികെ hb_blob_get_empty ();
  }

  തിരികെ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
  തിരികെ get_charstrings_data(*face->table.cff1, glyph);
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
  തിരികെ get_charstrings_index (*face->table.cff1);
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
  തിരികെ get_charstrings_data(*face->table.cff2, glyph);
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
  തിരികെ get_charstrings_index (*face->table.cff2);
}
#endif
#endif
