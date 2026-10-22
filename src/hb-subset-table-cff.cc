#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
بنية hb_subset_plan_t::source_table_loader<ثابت OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
بنية hb_subset_plan_t::source_table_loader<ثابت OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


منطقي _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success)
{
#ifndef HB_NO_SUBSET_CFF
  بدل (tag)
  {
  حالة HB_TAG('C','F','F',' '): *success = _hb_subset_table<ثابت OT::cff1> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('C','F','F','2'): *success = _hb_subset_table<ثابت OT::cff2> (plan, buf); أرجع 𐡒𐡔𐡈;
  حالة HB_TAG('V','O','R','G'): *success = _hb_subset_table<ثابت OT::VORG> (plan, buf); أرجع 𐡒𐡔𐡈;
  }
#endif
  أرجع خطأ;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
ساكن hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  𐡄𐡍 (!accel.is_valid()) {
    أرجع hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  𐡄𐡍 (!bytes) {
    أرجع hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  ثابت حرف* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long صحيح offset = (ثابت حرف*) bytes.arrayZ - cff_data;
  𐡄𐡍 (offset < 0 || offset > INT32_MAX) {
    أرجع hb_blob_get_empty ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

template<typename accel_t>
ساكن hb_blob_t* get_charstrings_index(accel_t& accel) {
  𐡄𐡍 (!accel.is_valid()) {
    أرجع hb_blob_get_empty ();
  }

  ثابت حرف* charstrings_start = (ثابت حرف*) accel.charStrings;
  unsigned charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  ثابت حرف* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long صحيح offset = charstrings_start - cff_data;
  𐡄𐡍 (offset < 0 || offset > INT32_MAX) {
    أرجع hb_blob_get_empty ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
  أرجع get_charstrings_data(*face->table.cff1, glyph);
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
  أرجع get_charstrings_index (*face->table.cff1);
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
  أرجع get_charstrings_data(*face->table.cff2, glyph);
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
  أرجع get_charstrings_index (*face->table.cff2);
}
#endif
#endif
