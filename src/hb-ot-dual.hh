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

#اگر_تعریف_نشده HB_OT_DUAL_HH
#تعریف_کن HB_OT_DUAL_HH

#ضمن "hb-open-type.hh"


نطاق OT {
نطاق Layout {

بنية SmallTypes {
  ساكن constexpr بیعلامت اندازه = 2;
  استفاده large_int = uint32_t;
  استفاده HBUINT = HBUINT16;
  استفاده HBLUINT = HBUINT16;
  استفاده HBGlyphID = HBGlyphID16;
  استفاده جابه_جایی_2 = Offset16;
  استفاده LOffset = Offset16;
  قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
  استفاده جابه_جایی_مقصد = OT::Offset16_مقصد<Type, پایه_Type, has_null>;
  قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
  استفاده LOffset_مقصد = OT::Offset16_مقصد<Type, پایه_Type, has_null>;
  قالب <نام_نوع Type>
  استفاده Array_از = OT::Array16_از<Type>;
  قالب <نام_نوع Type>
  استفاده Sorted_Array_از = OT::Sorted_Array16_از<Type>;
};

بنية MediumTypes {
  ساكن constexpr بیعلامت اندازه = 3;
  استفاده large_int = uint64_t;
  استفاده HBUINT = HBUINT24;
  استفاده HBLUINT = HBUINT32;
  استفاده HBGlyphID = HBGlyphID24;
  استفاده جابه_جایی_2 = Offset24;
  /* Long offsets for large tables. */
  استفاده LOffset = Offset32;
  قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
  استفاده جابه_جایی_مقصد = OT::Offset24_مقصد<Type, پایه_Type, has_null>;
  قالب <نام_نوع Type, نام_نوع پایه_Type=فراغ, منطقي has_null=درست>
  استفاده LOffset_مقصد = OT::Offset32_مقصد<Type, پایه_Type, has_null>;
  قالب <نام_نوع Type>
  استفاده Array_از = OT::Array24_از<Type>;
  قالب <نام_نوع Type>
  استفاده Sorted_Array_از = OT::Sorted_Array24_از<Type>;
};

}
}


قالب <نام_نوع پایینی_2, نام_نوع فوقانی_2>
بنية hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    فوقانی (face),
#پایان_اگر
    پایینی (
#اگر_تعریف_نشده HB_NO_BEYOND_64K
	   فوقانی.has_داده () ? hb_face_get_خالی () :
#پایان_اگر
	   face)
  {}

  منطقي has_داده () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (فوقانی.has_داده ()) أرجع درست;
#پایان_اگر
    أرجع پایینی.has_داده ();
  }

  منطقي has_فوقانی_داده () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    أرجع فوقانی.has_داده ();
#وإلا
    أرجع خطأ;
#پایان_اگر
  }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
  فوقانی_2 فوقانی;
#پایان_اگر
  پایینی_2 پایینی;
};

قالب <نام_نوع جدول_3>
بنية hb_جدول_accelerator_t
{
  hb_جدول_accelerator_t (hb_face_t *face) :
    جدول (hb_sanitize_context_t ().reference_جدول<جدول_3> (face))
  {}
  ~hb_جدول_accelerator_t () { جدول.destroy (); }

  منطقي has_داده () ثابت { أرجع جدول->has_داده (); }

  hb_blob_ptr_t<جدول_3> جدول;
};

قالب <نام_نوع پایینی_2, نام_نوع فوقانی_2>
بنية hb_dual_جدول_t : hb_dual_accelerator_t<hb_جدول_accelerator_t<پایینی_2>,
					       hb_جدول_accelerator_t<فوقانی_2>>
{
  hb_dual_جدول_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_جدول_accelerator_t<پایینی_2>,
			  hb_جدول_accelerator_t<فوقانی_2>> (face) {}
};

#اگر_تعریف_نشده HB_NO_BEYOND_64K
#تعریف_کن HB_DUAL_GET(accel, عبارت) ((accel).فوقانی.has_داده () ? (accel).فوقانی.عبارت : (accel).پایینی.عبارت)
#وإلا
#تعریف_کن HB_DUAL_GET(accel, عبارت) ((accel).پایینی.عبارت)
#پایان_اگر


#پایان_اگر  /* HB_OT_DUAL_HH */
