#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-vorg-table.hh"


#جے_بنا_تعریف HB_NO_SUBSET_CFF
سانچا<>
بنية hb_subset_plan_t::سروت_جدول_loader<ثابت OT::cff1>
{
  خودکار عامل () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
سانچا<>
بنية hb_subset_plan_t::سروت_جدول_loader<ثابت OT::cff2>
{
  خودکار عامل () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#جے_انت


منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success)
{
#جے_بنا_تعریف HB_NO_SUBSET_CFF
  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('C','F','F',' '): *success = hb_subset_جدول<ثابت OT::cff1> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('C','F','F','2'): *success = hb_subset_جدول<ثابت OT::cff2> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('V','O','R','G'): *success = hb_subset_جدول<ثابت OT::VORG> (plan, buf); أرجع سچ;
  }
#جے_انت
  أرجع خطأ;
}


#جے_تعریف_شدہ HB_EXPERIMENTAL_API
#جے_بنا_تعریف HB_NO_CFF

سانچا<قسم_ناں accel_t>
ساكن hb_blob_t* get_charstrings_انکڑے(accel_t& accel, hb_codepoint_t glyph_اشاریہ) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  hb_ubytes_t بائٹاں = (*accel.char_لڑیاں)[glyph_اشاریہ];
  إذا (!بائٹاں) {
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t لمائی;
  ثابت حرف* cff_انکڑے = hb_blob_get_انکڑے(cff_blob, &لمائی) ;

  لما صحيح وتھ = (ثابت حرف*) بائٹاں.arrayZ - cff_انکڑے;
  إذا (وتھ < 0 || وتھ > INT32_MAX) {
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) وتھ, بائٹاں.لمائی);
}

سانچا<قسم_ناں accel_t>
ساكن hb_blob_t* get_charstrings_اشاریہ(accel_t& accel) {
  إذا (!accel.is_valid()) {
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  ثابت حرف* charstrings_شروع = (ثابت حرف*) accel.char_لڑیاں;
  بنانشانی charstrings_لمائی = accel.char_لڑیاں->get_ناپ();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t لمائی;
  ثابت حرف* cff_انکڑے = hb_blob_get_انکڑے(cff_blob, &لمائی) ;

  لما صحيح وتھ = charstrings_شروع - cff_انکڑے;
  إذا (وتھ < 0 || وتھ > INT32_MAX) {
    أرجع hb_blob_get_خالی_تھاواں ();
  }

  أرجع hb_blob_create_sub_blob(cff_blob, (uint32_t) وتھ, charstrings_لمائی);
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
hb_subset_cff_get_charstring_انکڑے(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_انکڑے(*face->جدول.cff1, glyph);
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
hb_subset_cff_get_charstrings_اشاریہ (hb_face_t* face) {
  أرجع get_charstrings_اشاریہ (*face->جدول.cff1);
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
hb_subset_cff2_get_charstring_انکڑے(hb_face_t* face, hb_codepoint_t glyph) {
  أرجع get_charstrings_انکڑے(*face->جدول.cff2, glyph);
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
hb_subset_cff2_get_charstrings_اشاریہ (hb_face_t* face) {
  أرجع get_charstrings_اشاریہ (*face->جدول.cff2);
}
#جے_انت
#جے_انت
