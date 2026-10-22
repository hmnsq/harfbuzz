#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-cff1-table.hh"
#অন্তর্ভাব "hb-ot-cff2-table.hh"
#অন্তর্ভাব "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
কাঠামো hb_subset_plan_t::source_table_loader<ধ্রুবক OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
কাঠামো hb_subset_plan_t::source_table_loader<ধ্রুবক OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


যৌক্তিক _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<অক্ষর> &buf, hb_tag_t tag, যৌক্তিক *success)
{
#ifndef HB_NO_SUBSET_CFF
  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('C','F','F',' '): *success = _hb_subset_table<ধ্রুবক OT::cff1> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('C','F','F','2'): *success = _hb_subset_table<ধ্রুবক OT::cff2> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('V','O','R','G'): *success = _hb_subset_table<ধ্রুবক OT::VORG> (plan, buf); ফেরত সত্য;
  }
#endif
  ফেরত মিথ্যা;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
স্থির hb_blob_t* get_charstrings_তথ্য(accel_t& accel, hb_codepoint_t glyph_সূচক) {
  যদি (!accel.is_valid()) {
    ফেরত hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_সূচক];
  যদি (!bytes) {
    ফেরত hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t দৈর্ঘ্য;
  ধ্রুবক অক্ষর* cff_তথ্য = hb_blob_get_তথ্য(cff_blob, &দৈর্ঘ্য) ;

  long পূর্ণসংখ্যা offset = (ধ্রুবক অক্ষর*) bytes.arrayZ - cff_তথ্য;
  যদি (offset < 0 || offset > INT32_MAX) {
    ফেরত hb_blob_get_empty ();
  }

  ফেরত hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.দৈর্ঘ্য);
}

template<typename accel_t>
স্থির hb_blob_t* get_charstrings_সূচক(accel_t& accel) {
  যদি (!accel.is_valid()) {
    ফেরত hb_blob_get_empty ();
  }

  ধ্রুবক অক্ষর* charstrings_start = (ধ্রুবক অক্ষর*) accel.charStrings;
  unsigned charstrings_দৈর্ঘ্য = accel.charStrings->get_আকার();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t দৈর্ঘ্য;
  ধ্রুবক অক্ষর* cff_তথ্য = hb_blob_get_তথ্য(cff_blob, &দৈর্ঘ্য) ;

  long পূর্ণসংখ্যা offset = charstrings_start - cff_তথ্য;
  যদি (offset < 0 || offset > INT32_MAX) {
    ফেরত hb_blob_get_empty ();
  }

  ফেরত hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_দৈর্ঘ্য);
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
hb_subset_cff_get_charstring_তথ্য(hb_face_t* face, hb_codepoint_t glyph) {
  ফেরত get_charstrings_তথ্য(*face->table.cff1, glyph);
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
hb_subset_cff_get_charstrings_সূচক (hb_face_t* face) {
  ফেরত get_charstrings_সূচক (*face->table.cff1);
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
hb_subset_cff2_get_charstring_তথ্য(hb_face_t* face, hb_codepoint_t glyph) {
  ফেরত get_charstrings_তথ্য(*face->table.cff2, glyph);
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
hb_subset_cff2_get_charstrings_সূচক (hb_face_t* face) {
  ফেরত get_charstrings_সূচক (*face->table.cff2);
}
#endif
#endif
