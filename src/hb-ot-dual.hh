/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#جے_بنا_تعریف HB_OT_DUAL_HH
#تعریف_کرو HB_OT_DUAL_HH

#ضمن "hb-open-type.hh"


نطاق OT {
نطاق Layout {

بنية SmallTypes {
  ساكن constexpr بنانشانی ناپ = 2;
  ورتوں large_int = uint32_t;
  ورتوں HBUINT = HBUINT16;
  ورتوں HBLUINT = HBUINT16;
  ورتوں HBGlyphID = HBGlyphID16;
  ورتوں وتھ_2 = Offset16;
  ورتوں LOffset = Offset16;
  سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
  ورتوں وتھ_منزل = OT::Offset16_منزل<Type, ادھار_Type, has_null>;
  سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
  ورتوں LOffset_منزل = OT::Offset16_منزل<Type, ادھار_Type, has_null>;
  سانچا <قسم_ناں Type>
  ورتوں Array_دا = OT::Array16_دا<Type>;
  سانچا <قسم_ناں Type>
  ورتوں Sorted_Array_دا = OT::Sorted_Array16_دا<Type>;
};

بنية MediumTypes {
  ساكن constexpr بنانشانی ناپ = 3;
  ورتوں large_int = uint64_t;
  ورتوں HBUINT = HBUINT24;
  ورتوں HBLUINT = HBUINT32;
  ورتوں HBGlyphID = HBGlyphID24;
  ورتوں وتھ_2 = Offset24;
  /* Long offsets for large tables. */
  ورتوں LOffset = Offset32;
  سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
  ورتوں وتھ_منزل = OT::Offset24_منزل<Type, ادھار_Type, has_null>;
  سانچا <قسم_ناں Type, قسم_ناں ادھار_Type=فراغ, منطقي has_null=سچ>
  ورتوں LOffset_منزل = OT::Offset32_منزل<Type, ادھار_Type, has_null>;
  سانچا <قسم_ناں Type>
  ورتوں Array_دا = OT::Array24_دا<Type>;
  سانچا <قسم_ناں Type>
  ورتوں Sorted_Array_دا = OT::Sorted_Array24_دا<Type>;
};

}
}


سانچا <قسم_ناں ہیٹھلا_2, قسم_ناں اتلا_2>
بنية hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#جے_بنا_تعریف HB_NO_BEYOND_64K
    اتلا (face),
#جے_انت
    ہیٹھلا (
#جے_بنا_تعریف HB_NO_BEYOND_64K
	   اتلا.has_انکڑے () ? hb_face_get_خالی_تھاواں () :
#جے_انت
	   face)
  {}

  منطقي has_انکڑے () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (اتلا.has_انکڑے ()) أرجع سچ;
#جے_انت
    أرجع ہیٹھلا.has_انکڑے ();
  }

  منطقي has_اتلا_انکڑے () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    أرجع اتلا.has_انکڑے ();
#وإلا
    أرجع خطأ;
#جے_انت
  }

#جے_بنا_تعریف HB_NO_BEYOND_64K
  اتلا_2 اتلا;
#جے_انت
  ہیٹھلا_2 ہیٹھلا;
};

سانچا <قسم_ناں جدول_3>
بنية hb_جدول_accelerator_t
{
  hb_جدول_accelerator_t (hb_face_t *face) :
    جدول (hb_sanitize_context_t ().reference_جدول<جدول_3> (face))
  {}
  ~hb_جدول_accelerator_t () { جدول.destroy (); }

  منطقي has_انکڑے () ثابت { أرجع جدول->has_انکڑے (); }

  hb_blob_ptr_t<جدول_3> جدول;
};

سانچا <قسم_ناں ہیٹھلا_2, قسم_ناں اتلا_2>
بنية hb_dual_جدول_t : hb_dual_accelerator_t<hb_جدول_accelerator_t<ہیٹھلا_2>,
					       hb_جدول_accelerator_t<اتلا_2>>
{
  hb_dual_جدول_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_جدول_accelerator_t<ہیٹھلا_2>,
			  hb_جدول_accelerator_t<اتلا_2>> (face) {}
};

#جے_بنا_تعریف HB_NO_BEYOND_64K
#تعریف_کرو HB_DUAL_GET(accel, اظہاریہ) ((accel).اتلا.has_انکڑے () ? (accel).اتلا.اظہاریہ : (accel).ہیٹھلا.اظہاریہ)
#وإلا
#تعریف_کرو HB_DUAL_GET(accel, اظہاریہ) ((accel).ہیٹھلا.اظہاریہ)
#جے_انت


#جے_انت  /* HB_OT_DUAL_HH */
