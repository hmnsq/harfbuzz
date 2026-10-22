#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-vorg-table.hh"


#اگر_تعریف_نشده HB_NO_SUBSET_CFF
قالب<>
بنية hb_subset_plan_t::منبع_جدول_loader<ثابت OT::cff1>
{
  خودکار عملگر () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
قالب<>
بنية hb_subset_plan_t::منبع_جدول_loader<ثابت OT::cff2>
{
  خودکار عملگر () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#پایان_اگر


منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success)
{
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  بدل (برچسب_2)
  {
  حالة HB_برچسب('C','F','F',' '): *success = hb_subset_جدول<ثابت OT::cff1> (plan, buf); أرجع درست;
  حالة HB_برچسب('C','F','F','2'): *success = hb_subset_جدول<ثابت OT::cff2> (plan, buf); أرجع درست;
  حالة HB_برچسب('V','O','R','G'): *success = hb_subset_جدول<ثابت OT::VORG> (plan, buf); أرجع درست;
  }
#پایان_اگر
  أرجع خطأ;
}


#اگر_تعریف_شده HB_EXPERIMENTAL_API
#اگر_تعریف_نشده HB_NO_CFF

قالب<نام_نوع accel_t>
ساكن hb_blob_t* get_charstrings_داده(accel_t& accel, hb_codepoint_t glyph_شاخص) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_خالی ();
  }

  hb_ubytes_t بایتها = (*accel.char_رشتهها)[glyph_شاخص];
  إذا (!بایتها) {
    أرجع hb_blob_get_خالی ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t درازی;
  ثابت حرف* cff_داده = hb_blob_get_داده(cff_blob, &درازی) ;

  دراز صحيح جابه_جایی = (ثابت حرف*) بایتها.arrayZ - cff_داده;
  إذا (جابه_جایی < 0 || جابه_جایی > INT32_MAX) {
    أرجع hb_blob_get_خالی ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) جابه_جایی, بایتها.درازی);
}

قالب<نام_نوع accel_t>
ساكن hb_blob_t* get_charstrings_شاخص(accel_t& accel) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_خالی ();
  }

  ثابت حرف* charstrings_شروع = (ثابت حرف*) accel.char_رشتهها;
  بیعلامت charstrings_درازی = accel.char_رشتهها->get_اندازه();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t درازی;
  ثابت حرف* cff_داده = hb_blob_get_داده(cff_blob, &درازی) ;

  دراز صحيح جابه_جایی = charstrings_شروع - cff_داده;
  إذا (جابه_جایی < 0 || جابه_جایی > INT32_MAX) {
    أرجع hb_blob_get_خالی ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) جابه_جایی, charstrings_درازی);
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
hb_subset_cff_get_charstring_داده(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_داده(*face->جدول.cff1, glyph);
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
hb_subset_cff_get_charstrings_شاخص (hb_face_t* face) {
  أرجع get_charstrings_شاخص (*face->جدول.cff1);
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
hb_subset_cff2_get_charstring_داده(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_داده(*face->جدول.cff2, glyph);
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
hb_subset_cff2_get_charstrings_شاخص (hb_face_t* face) {
  أرجع get_charstrings_شاخص (*face->جدول.cff2);
}
#پایان_اگر
#پایان_اگر
