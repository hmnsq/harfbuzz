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

#که_نه_تعریف_شوی HB_OT_DUAL_HH
#تعریف_کړه HB_OT_DUAL_HH

#ضمن "hb-open-type.hh"


نطاق OT {
نطاق Layout {

بنية SmallTypes {
  ساكن constexpr بینښه اندازه = 2;
  کارول large_int = uint32_t;
  کارول HBUINT = HBUINT16;
  کارول HBLUINT = HBUINT16;
  کارول HBGlyphID = HBGlyphID16;
  کارول دننه_واټن_2 = Offset16;
  کارول LOffset = Offset16;
  کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
  کارول دننه_واټن_منزل = OT::Offset16_منزل<Type, بنسټ_Type, has_null>;
  کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
  کارول LOffset_منزل = OT::Offset16_منزل<Type, بنسټ_Type, has_null>;
  کينډۍ <ډول_نوم Type>
  کارول Array_د = OT::Array16_د<Type>;
  کينډۍ <ډول_نوم Type>
  کارول Sorted_Array_د = OT::Sorted_Array16_د<Type>;
};

بنية MediumTypes {
  ساكن constexpr بینښه اندازه = 3;
  کارول large_int = uint64_t;
  کارول HBUINT = HBUINT24;
  کارول HBLUINT = HBUINT32;
  کارول HBGlyphID = HBGlyphID24;
  کارول دننه_واټن_2 = Offset24;
  /* Long offsets for large tables. */
  کارول LOffset = Offset32;
  کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
  کارول دننه_واټن_منزل = OT::Offset24_منزل<Type, بنسټ_Type, has_null>;
  کينډۍ <ډول_نوم Type, ډول_نوم بنسټ_Type=فراغ, منطقي has_null=ریښتیا>
  کارول LOffset_منزل = OT::Offset32_منزل<Type, بنسټ_Type, has_null>;
  کينډۍ <ډول_نوم Type>
  کارول Array_د = OT::Array24_د<Type>;
  کينډۍ <ډول_نوم Type>
  کارول Sorted_Array_د = OT::Sorted_Array24_د<Type>;
};

}
}


کينډۍ <ډول_نوم ښکتنی_2, ډول_نوم پاسنی_2>
بنية hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    پاسنی (face),
#د_که_پای
    ښکتنی (
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
	   پاسنی.has_معلومات () ? hb_face_get_تشه () :
#د_که_پای
	   face)
  {}

  منطقي has_معلومات () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (پاسنی.has_معلومات ()) أرجع ریښتیا;
#د_که_پای
    أرجع ښکتنی.has_معلومات ();
  }

  منطقي has_پاسنی_معلومات () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    أرجع پاسنی.has_معلومات ();
#وإلا
    أرجع خطأ;
#د_که_پای
  }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  پاسنی_2 پاسنی;
#د_که_پای
  ښکتنی_2 ښکتنی;
};

کينډۍ <ډول_نوم جدول_3>
بنية hb_جدول_accelerator_t
{
  hb_جدول_accelerator_t (hb_face_t *face) :
    جدول (hb_sanitize_context_t ().reference_جدول<جدول_3> (face))
  {}
  ~hb_جدول_accelerator_t () { جدول.destroy (); }

  منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

  hb_blob_ptr_t<جدول_3> جدول;
};

کينډۍ <ډول_نوم ښکتنی_2, ډول_نوم پاسنی_2>
بنية hb_dual_جدول_t : hb_dual_accelerator_t<hb_جدول_accelerator_t<ښکتنی_2>,
					       hb_جدول_accelerator_t<پاسنی_2>>
{
  hb_dual_جدول_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_جدول_accelerator_t<ښکتنی_2>,
			  hb_جدول_accelerator_t<پاسنی_2>> (face) {}
};

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
#تعریف_کړه HB_DUAL_GET(accel, بیان) ((accel).پاسنی.has_معلومات () ? (accel).پاسنی.بیان : (accel).ښکتنی.بیان)
#وإلا
#تعریف_کړه HB_DUAL_GET(accel, بیان) ((accel).ښکتنی.بیان)
#د_که_پای


#د_که_پای  /* HB_OT_DUAL_HH */
