#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-vorg-table.hh"


#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
کينډۍ<>
بنية hb_subset_plan_t::سرچینه_جدول_loader<ثابت OT::cff1>
{
  خپلکاری چلوونکی () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
کينډۍ<>
بنية hb_subset_plan_t::سرچینه_جدول_loader<ثابت OT::cff2>
{
  خپلکاری چلوونکی () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#د_که_پای


منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success)
{
#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  بدل (نښان_2)
  {
  حالة HB_نښان('C','F','F',' '): *success = hb_subset_جدول<ثابت OT::cff1> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('C','F','F','2'): *success = hb_subset_جدول<ثابت OT::cff2> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('V','O','R','G'): *success = hb_subset_جدول<ثابت OT::VORG> (plan, buf); أرجع ریښتیا;
  }
#د_که_پای
  أرجع خطأ;
}


#که_تعریف_شوی HB_EXPERIMENTAL_API
#که_نه_تعریف_شوی HB_NO_CFF

کينډۍ<ډول_نوم accel_t>
ساكن hb_blob_t* get_charstrings_معلومات(accel_t& accel, hb_codepoint_t glyph_شاخص) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_تشه ();
  }

  hb_ubytes_t بایټونه = (*accel.char_مزي_ګانې)[glyph_شاخص];
  إذا (!بایټونه) {
    أرجع hb_blob_get_تشه ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t اوږدتیا;
  ثابت حرف* cff_معلومات = hb_blob_get_معلومات(cff_blob, &اوږدتیا) ;

  اوږد صحيح دننه_واټن = (ثابت حرف*) بایټونه.arrayZ - cff_معلومات;
  إذا (دننه_واټن < 0 || دننه_واټن > INT32_MAX) {
    أرجع hb_blob_get_تشه ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) دننه_واټن, بایټونه.اوږدتیا);
}

کينډۍ<ډول_نوم accel_t>
ساكن hb_blob_t* get_charstrings_شاخص(accel_t& accel) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_تشه ();
  }

  ثابت حرف* charstrings_پیل = (ثابت حرف*) accel.char_مزي_ګانې;
  بینښه charstrings_اوږدتیا = accel.char_مزي_ګانې->get_اندازه();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t اوږدتیا;
  ثابت حرف* cff_معلومات = hb_blob_get_معلومات(cff_blob, &اوږدتیا) ;

  اوږد صحيح دننه_واټن = charstrings_پیل - cff_معلومات;
  إذا (دننه_واټن < 0 || دننه_واټن > INT32_MAX) {
    أرجع hb_blob_get_تشه ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) دننه_واټن, charstrings_اوږدتیا);
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
hb_subset_cff_get_charstring_معلومات(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_معلومات(*face->جدول.cff1, glyph);
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
hb_subset_cff2_get_charstring_معلومات(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_معلومات(*face->جدول.cff2, glyph);
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
#د_که_پای
#د_که_پای
