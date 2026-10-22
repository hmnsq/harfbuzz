#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-vorg-table.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
ਸਾਂਚਾ<>
ਬਣਤਰ hb_subset_plan_t::ਸਰੋਤ_ਸਾਰਣੀ_loader<ਅਚਲ OT::cff1>
{
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
ਸਾਂਚਾ<>
ਬਣਤਰ hb_subset_plan_t::ਸਰੋਤ_ਸਾਰਣੀ_loader<ਅਚਲ OT::cff2>
{
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#ਜੇ_ਅੰਤ


ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_cff		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('C','F','F',' '): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::cff1> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('C','F','F','2'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::cff2> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('V','O','R','G'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::VORG> (plan, buf); ਵਾਪਸ ਸੱਚ;
  }
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ accel_t>
ਸਥਿਰ hb_blob_t* get_charstrings_ਅੰਕੜੇ(accel_t& accel, hb_codepoint_t glyph_ਸੂਚਕਾਂਕ) {
  ਜੇ (!accel.is_valid()) {
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  hb_ubytes_t ਬਾਈਟਾਂ = (*accel.char_ਲੜੀਆਂ)[glyph_ਸੂਚਕਾਂਕ];
  ਜੇ (!ਬਾਈਟਾਂ) {
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ* cff_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ(cff_blob, &ਲੰਮਾਈ) ;

  ਲੰਮਾ ਪੂਰਨ ਵਿੱਥ = (ਅਚਲ ਅੱਖਰ*) ਬਾਈਟਾਂ.arrayZ - cff_ਅੰਕੜੇ;
  ਜੇ (ਵਿੱਥ < 0 || ਵਿੱਥ > INT32_MAX) {
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  ਵਾਪਸ hb_blob_create_sub_blob(cff_blob, (uint32_t) ਵਿੱਥ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ accel_t>
ਸਥਿਰ hb_blob_t* get_charstrings_ਸੂਚਕਾਂਕ(accel_t& accel) {
  ਜੇ (!accel.is_valid()) {
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  ਅਚਲ ਅੱਖਰ* charstrings_ਸ਼ੁਰੂ = (ਅਚਲ ਅੱਖਰ*) accel.char_ਲੜੀਆਂ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ charstrings_ਲੰਮਾਈ = accel.char_ਲੜੀਆਂ->get_ਮਾਪ();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ* cff_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ(cff_blob, &ਲੰਮਾਈ) ;

  ਲੰਮਾ ਪੂਰਨ ਵਿੱਥ = charstrings_ਸ਼ੁਰੂ - cff_ਅੰਕੜੇ;
  ਜੇ (ਵਿੱਥ < 0 || ਵਿੱਥ > INT32_MAX) {
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  }

  ਵਾਪਸ hb_blob_create_sub_blob(cff_blob, (uint32_t) ਵਿੱਥ, charstrings_ਲੰਮਾਈ);
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
hb_subset_cff_get_charstring_ਅੰਕੜੇ(hb_face_t* face, hb_codepoint_t glyph) {
  ਵਾਪਸ get_charstrings_ਅੰਕੜੇ(*face->ਸਾਰਣੀ.cff1, glyph);
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
hb_subset_cff_get_charstrings_ਸੂਚਕਾਂਕ (hb_face_t* face) {
  ਵਾਪਸ get_charstrings_ਸੂਚਕਾਂਕ (*face->ਸਾਰਣੀ.cff1);
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
hb_subset_cff2_get_charstring_ਅੰਕੜੇ(hb_face_t* face, hb_codepoint_t glyph) {
  ਵਾਪਸ get_charstrings_ਅੰਕੜੇ(*face->ਸਾਰਣੀ.cff2, glyph);
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
hb_subset_cff2_get_charstrings_ਸੂਚਕਾਂਕ (hb_face_t* face) {
  ਵਾਪਸ get_charstrings_ਸੂਚਕਾਂਕ (*face->ਸਾਰਣੀ.cff2);
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
