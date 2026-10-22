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

#اگر_غیر_تعریف_شدہ HB_OT_DUAL_HH
#تعریف_کرو HB_OT_DUAL_HH

#شامل "hb-open-type.hh"


نامخلا OT {
نامخلا Layout {

ساخت SmallTypes {
  ساکن constexpr غیرعلامتی سائز = 2;
  استعمال large_int = uint32_t;
  استعمال HBUINT = HBUINT16;
  استعمال HBLUINT = HBUINT16;
  استعمال HBGlyphID = HBGlyphID16;
  استعمال انحراف_2 = Offset16;
  استعمال LOffset = Offset16;
  سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
  استعمال انحراف_مقصد = OT::Offset16_مقصد<Type, بنیاد_Type, has_null>;
  سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
  استعمال LOffset_مقصد = OT::Offset16_مقصد<Type, بنیاد_Type, has_null>;
  سانچہ <قسم_نام Type>
  استعمال Array_کا = OT::Array16_کا<Type>;
  سانچہ <قسم_نام Type>
  استعمال Sorted_Array_کا = OT::Sorted_Array16_کا<Type>;
};

ساخت MediumTypes {
  ساکن constexpr غیرعلامتی سائز = 3;
  استعمال large_int = uint64_t;
  استعمال HBUINT = HBUINT24;
  استعمال HBLUINT = HBUINT32;
  استعمال HBGlyphID = HBGlyphID24;
  استعمال انحراف_2 = Offset24;
  /* Long offsets for large tables. */
  استعمال LOffset = Offset32;
  سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
  استعمال انحراف_مقصد = OT::Offset24_مقصد<Type, بنیاد_Type, has_null>;
  سانچہ <قسم_نام Type, قسم_نام بنیاد_Type=خالی, منطقی has_null=سچ>
  استعمال LOffset_مقصد = OT::Offset32_مقصد<Type, بنیاد_Type, has_null>;
  سانچہ <قسم_نام Type>
  استعمال Array_کا = OT::Array24_کا<Type>;
  سانچہ <قسم_نام Type>
  استعمال Sorted_Array_کا = OT::Sorted_Array24_کا<Type>;
};

}
}


سانچہ <قسم_نام زیریں_2, قسم_نام بالائی_2>
ساخت hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    بالائی (face),
#اگر_ختم
    زیریں (
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
	   بالائی.has_ڈیٹا () ? hb_face_get_خالی_خانے () :
#اگر_ختم
	   face)
  {}

  منطقی has_ڈیٹا () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (بالائی.has_ڈیٹا ()) واپس سچ;
#اگر_ختم
    واپس زیریں.has_ڈیٹا ();
  }

  منطقی has_بالائی_ڈیٹا () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    واپس بالائی.has_ڈیٹا ();
#ورنہ
    واپس جھوٹ;
#اگر_ختم
  }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  بالائی_2 بالائی;
#اگر_ختم
  زیریں_2 زیریں;
};

سانچہ <قسم_نام جدول_3>
ساخت hb_جدول_accelerator_t
{
  hb_جدول_accelerator_t (hb_face_t *face) :
    جدول (hb_sanitize_context_t ().reference_جدول<جدول_3> (face))
  {}
  ~hb_جدول_accelerator_t () { جدول.destroy (); }

  منطقی has_ڈیٹا () ثابت { واپس جدول->has_ڈیٹا (); }

  hb_blob_ptr_t<جدول_3> جدول;
};

سانچہ <قسم_نام زیریں_2, قسم_نام بالائی_2>
ساخت hb_dual_جدول_t : hb_dual_accelerator_t<hb_جدول_accelerator_t<زیریں_2>,
					       hb_جدول_accelerator_t<بالائی_2>>
{
  hb_dual_جدول_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_جدول_accelerator_t<زیریں_2>,
			  hb_جدول_accelerator_t<بالائی_2>> (face) {}
};

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
#تعریف_کرو HB_DUAL_GET(accel, اظہاریہ) ((accel).بالائی.has_ڈیٹا () ? (accel).بالائی.اظہاریہ : (accel).زیریں.اظہاریہ)
#ورنہ
#تعریف_کرو HB_DUAL_GET(accel, اظہاریہ) ((accel).زیریں.اظہاریہ)
#اگر_ختم


#اگر_ختم  /* HB_OT_DUAL_HH */
