#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "hb-ot-cff1-table.hh"
#अन्तर्भाव "hb-ot-cff2-table.hh"
#अन्तर्भाव "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
ढाँचा<>
संरचना hb_subset_plan_t::source_table_loader<अचर OT::cff1>
{
  स्वतः चालक () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
ढाँचा<>
संरचना hb_subset_plan_t::source_table_loader<अचर OT::cff2>
{
  स्वतः चालक () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


तार्किक _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_SUBSET_CFF
  विकल्प (tag)
  {
  पक्ष HB_TAG('C','F','F',' '): *success = _hb_subset_table<अचर OT::cff1> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('C','F','F','2'): *success = _hb_subset_table<अचर OT::cff2> (plan, buf); फर्काऊ सत्य;
  पक्ष HB_TAG('V','O','R','G'): *success = _hb_subset_table<अचर OT::VORG> (plan, buf); फर्काऊ सत्य;
  }
#endif
  फर्काऊ असत्य;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

ढाँचा<typename accel_t>
स्थिर hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  यदि (!accel.is_valid()) {
    फर्काऊ hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  यदि (!bytes) {
    फर्काऊ hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  अचर वर्ण* cff_data = hb_blob_get_data(cff_blob, &length) ;

  दीर्घ पूर्णाङ्क offset = (अचर वर्ण*) bytes.arrayZ - cff_data;
  यदि (offset < 0 || offset > INT32_MAX) {
    फर्काऊ hb_blob_get_empty ();
  }

  फर्काऊ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

ढाँचा<typename accel_t>
स्थिर hb_blob_t* get_charstrings_index(accel_t& accel) {
  यदि (!accel.is_valid()) {
    फर्काऊ hb_blob_get_empty ();
  }

  अचर वर्ण* charstrings_start = (अचर वर्ण*) accel.charStrings;
  अचिन्हित charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  अचर वर्ण* cff_data = hb_blob_get_data(cff_blob, &length) ;

  दीर्घ पूर्णाङ्क offset = charstrings_start - cff_data;
  यदि (offset < 0 || offset > INT32_MAX) {
    फर्काऊ hb_blob_get_empty ();
  }

  फर्काऊ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
  फर्काऊ get_charstrings_data(*face->table.cff1, glyph);
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
  फर्काऊ get_charstrings_index (*face->table.cff1);
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
  फर्काऊ get_charstrings_data(*face->table.cff2, glyph);
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
  फर्काऊ get_charstrings_index (*face->table.cff2);
}
#endif
#endif
