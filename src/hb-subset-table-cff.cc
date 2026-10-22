#شامل "hb-subset-table.hh"

#شامل "hb-ot-cff1-table.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-ot-vorg-table.hh"


#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
سانچہ<>
ساخت hb_subset_plan_t::ماخذ_جدول_loader<ثابت OT::cff1>
{
  خودکار عامل () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff1_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff1_accel :
		  plan->cff1_accel)
};
سانچہ<>
ساخت hb_subset_plan_t::ماخذ_جدول_loader<ثابت OT::cff2>
{
  خودکار عامل () (hb_subset_plan_t *plan)
  HB_AUTO_RETURN (plan->accelerator ? plan->accelerator->cff2_accel :
		  plan->inprogress_accelerator ? plan->inprogress_accelerator->cff2_accel :
		  plan->cff2_accel)
};
#اگر_ختم


منطقی hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success)
{
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('C','F','F',' '): *success = hb_subset_جدول<ثابت OT::cff1> (plan, buf); واپس سچ;
  حالت HB_لیبل('C','F','F','2'): *success = hb_subset_جدول<ثابت OT::cff2> (plan, buf); واپس سچ;
  حالت HB_لیبل('V','O','R','G'): *success = hb_subset_جدول<ثابت OT::VORG> (plan, buf); واپس سچ;
  }
#اگر_ختم
  واپس جھوٹ;
}


#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
#اگر_غیر_تعریف_شدہ HB_NO_CFF

سانچہ<قسم_نام accel_t>
ساکن hb_blob_t* get_charstrings_ڈیٹا(accel_t& accel, hb_codepoint_t glyph_اشاریہ) {
  اگر (!accel.is_valid()) {
    واپس hb_blob_get_خالی_خانے ();
  }

  hb_ubytes_t بائٹیں = (*accel.char_لڑیاں)[glyph_اشاریہ];
  اگر (!بائٹیں) {
    واپس hb_blob_get_خالی_خانے ();
  }

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t لمبائی;
  ثابت حرف* cff_ڈیٹا = hb_blob_get_ڈیٹا(cff_blob, &لمبائی) ;

  طویل صحیح انحراف = (ثابت حرف*) بائٹیں.arrayZ - cff_ڈیٹا;
  اگر (انحراف < 0 || انحراف > INT32_MAX) {
    واپس hb_blob_get_خالی_خانے ();
  }

  واپس hb_blob_create_sub_blob(cff_blob, (uint32_t) انحراف, بائٹیں.لمبائی);
}

سانچہ<قسم_نام accel_t>
ساکن hb_blob_t* get_charstrings_اشاریہ(accel_t& accel) {
  اگر (!accel.is_valid()) {
    واپس hb_blob_get_خالی_خانے ();
  }

  ثابت حرف* charstrings_آغاز = (ثابت حرف*) accel.char_لڑیاں;
  غیرعلامتی charstrings_لمبائی = accel.char_لڑیاں->get_سائز();

  hb_blob_t* cff_blob = accel.get_blob();
  uint32_t لمبائی;
  ثابت حرف* cff_ڈیٹا = hb_blob_get_ڈیٹا(cff_blob, &لمبائی) ;

  طویل صحیح انحراف = charstrings_آغاز - cff_ڈیٹا;
  اگر (انحراف < 0 || انحراف > INT32_MAX) {
    واپس hb_blob_get_خالی_خانے ();
  }

  واپس hb_blob_create_sub_blob(cff_blob, (uint32_t) انحراف, charstrings_لمبائی);
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
hb_subset_cff_get_charstring_ڈیٹا(hb_face_t* face, hb_codepoint_t glyph) {
  واپس get_charstrings_ڈیٹا(*face->جدول.cff1, glyph);
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
  واپس get_charstrings_اشاریہ (*face->جدول.cff1);
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
hb_subset_cff2_get_charstring_ڈیٹا(hb_face_t* face, hb_codepoint_t glyph) {
  واپس get_charstrings_ڈیٹا(*face->جدول.cff2, glyph);
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
  واپس get_charstrings_اشاریہ (*face->جدول.cff2);
}
#اگر_ختم
#اگر_ختم
