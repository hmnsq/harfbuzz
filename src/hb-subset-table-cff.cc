#समावेश "hb-subset-table.hh"

#समावेश "hb-ot-cff1-table.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-ot-vorg-table.hh"


#अगर_अघोषित HB_NO_SUBSET_CFF
ढाँचा<>
काष्ठा hb_subset_plan_t::source_table_loader<स्थिर OT::cff1>
{
  स्वतः चालक () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
ढाँचा<>
काष्ठा hb_subset_plan_t::source_table_loader<स्थिर OT::cff2>
{
  स्वतः चालक () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#पूर्ण_अगर


bool _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success)
{
#अगर_अघोषित HB_NO_SUBSET_CFF
  चयन (tag)
  {
  हाल HB_TAG('C','F','F',' '): *success = _hb_subset_table<स्थिर OT::cff1> (plan, buf); वापस true;
  हाल HB_TAG('C','F','F','2'): *success = _hb_subset_table<स्थिर OT::cff2> (plan, buf); वापस true;
  हाल HB_TAG('V','O','R','G'): *success = _hb_subset_table<स्थिर OT::VORG> (plan, buf); वापस true;
  }
#पूर्ण_अगर
  वापस false;
}


#अगर_घोषित HB_EXPERIMENTAL_API
#अगर_अघोषित HB_NO_CFF

ढाँचा<typename accel_t>
अटल hb_blob_t* get_charstrings_तथ्य(accel_t& accel, hb_codepoint_t glyph_सूचकांक) {
  अगर (!accel.is_valid()) {
    वापस hb_blob_get_empty ();
  }

  hb_ubytes_t bytes = (*accel.charStrings)[glyph_सूचकांक];
  अगर (!bytes) {
    वापस hb_blob_get_empty ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t लंबाई;
  स्थिर अक्षर* cff_तथ्य = hb_blob_get_तथ्य(cff_blob, &लंबाई) ;

  दीर्घ पूर्णांक offset = (स्थिर अक्षर*) bytes.arrayZ - cff_तथ्य;
  अगर (offset < 0 || offset > INT32_MAX) {
    वापस hb_blob_get_empty ();
  }

  वापस hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, bytes.लंबाई);
}

ढाँचा<typename accel_t>
अटल hb_blob_t* get_charstrings_सूचकांक(accel_t& accel) {
  अगर (!accel.is_valid()) {
    वापस hb_blob_get_empty ();
  }

  स्थिर अक्षर* charstrings_start = (स्थिर अक्षर*) accel.charStrings;
  अचिन्हित charstrings_लंबाई = accel.charStrings->get_आकार();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t लंबाई;
  स्थिर अक्षर* cff_तथ्य = hb_blob_get_तथ्य(cff_blob, &लंबाई) ;

  दीर्घ पूर्णांक offset = charstrings_start - cff_तथ्य;
  अगर (offset < 0 || offset > INT32_MAX) {
    वापस hb_blob_get_empty ();
  }

  वापस hb_blob_create_sub_blob(cff_blob, (uint32_t) offset, charstrings_लंबाई);
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
hb_subset_cff_get_charstring_तथ्य(hb_face_t* face, hb_codepoint_t glyph) {
  वापस get_charstrings_तथ्य(*face->table.cff1, glyph);
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
hb_subset_cff_get_charstrings_सूचकांक (hb_face_t* face) {
  वापस get_charstrings_सूचकांक (*face->table.cff1);
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
hb_subset_cff2_get_charstring_तथ्य(hb_face_t* face, hb_codepoint_t glyph) {
  वापस get_charstrings_तथ्य(*face->table.cff2, glyph);
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
hb_subset_cff2_get_charstrings_सूचकांक (hb_face_t* face) {
  वापस get_charstrings_सूचकांक (*face->table.cff2);
}
#पूर्ण_अगर
#पूर्ण_अगर
