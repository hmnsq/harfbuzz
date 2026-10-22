#include "hb-subset-table.hh"

#include "hb-ot-cff1-table.hh"
#include "hb-ot-cff2-table.hh"
#include "hb-ot-vorg-table.hh"


#ifndef HB_NO_SUBSET_CFF
template<>
sa_nracanaa hb_subset_plan_t::source_table_loader<const OT::cff1>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
template<>
sa_nracanaa hb_subset_plan_t::source_table_loader<const OT::cff2>
{
  auto operator () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#endif


bool _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<var_na> &buf, hb_tag_t tag, bool *success)
{
#ifndef HB_NO_SUBSET_CFF
  switch (tag)
  {
  pak_sa HB_TAG('C','F','F',' '): *success = _hb_subset_table<const OT::cff1> (plan, buf); ᰀᰧᰌ true;
  pak_sa HB_TAG('C','F','F','2'): *success = _hb_subset_table<const OT::cff2> (plan, buf); ᰀᰧᰌ true;
  pak_sa HB_TAG('V','O','R','G'): *success = _hb_subset_table<const OT::VORG> (plan, buf); ᰀᰧᰌ true;
  }
#endif
  ᰀᰧᰌ false;
}


#ifdef HB_EXPERIMENTAL_API
#ifndef HB_NO_CFF

template<typename accel_t>
static hb_blob_t* get_charstrings_data(accel_t& accel, hb_codepoint_t glyph_index) {
  ᰀᰧ (!accel.is_valid()) {
    ᰀᰧᰌ hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_index];
  ᰀᰧ (!bytes) {
    ᰀᰧᰌ hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  const var_na* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long puur_naa_nka offset = (const var_na*) bytes.arrayZ - cff_data;
  ᰀᰧ (offset < 0 || offset > INT32_MAX) {
    ᰀᰧᰌ hb_blob_get_empty ();
  }

  ᰀᰧᰌ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.length);
}

template<typename accel_t>
static hb_blob_t* get_charstrings_index(accel_t& accel) {
  ᰀᰧ (!accel.is_valid()) {
    ᰀᰧᰌ hb_blob_get_empty ();
  }

  const var_na* charstrings_start = (const var_na*) accel.charStrings;
  unsigned charstrings_length = accel.charStrings->get_size();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t length;
  const var_na* cff_data = hb_blob_get_data(cff_blob, &length) ;

  long puur_naa_nka offset = charstrings_start - cff_data;
  ᰀᰧ (offset < 0 || offset > INT32_MAX) {
    ᰀᰧᰌ hb_blob_get_empty ();
  }

  ᰀᰧᰌ hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_length);
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
  ᰀᰧᰌ get_charstrings_data(*face->table.cff1, glyph);
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
  ᰀᰧᰌ get_charstrings_index (*face->table.cff1);
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
  ᰀᰧᰌ get_charstrings_data(*face->table.cff2, glyph);
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
  ᰀᰧᰌ get_charstrings_index (*face->table.cff2);
}
#endif
#endif
