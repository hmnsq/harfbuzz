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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_DUAL_HH
#وصف_ڪريو HB_OT_DUAL_HH

#ضمن "hb-open-type.hh"


نطاق OT {
نطاق Layout {

بنية SmallTypes {
  ساكن constexpr بنانشان سائيز = 2;
  استعمال large_int = uint32_t;
  استعمال HBUINT = HBUINT16;
  استعمال HBLUINT = HBUINT16;
  استعمال HBGlyphID = HBGlyphID16;
  استعمال وٿي_2 = Offset16;
  استعمال LOffset = Offset16;
  سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
  استعمال وٿي_منزل = OT::Offset16_منزل<Type, بنياد_Type, has_null>;
  سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
  استعمال LOffset_منزل = OT::Offset16_منزل<Type, بنياد_Type, has_null>;
  سانچو <قسم_نالو Type>
  استعمال Array_جو = OT::Array16_جو<Type>;
  سانچو <قسم_نالو Type>
  استعمال Sorted_Array_جو = OT::Sorted_Array16_جو<Type>;
};

بنية MediumTypes {
  ساكن constexpr بنانشان سائيز = 3;
  استعمال large_int = uint64_t;
  استعمال HBUINT = HBUINT24;
  استعمال HBLUINT = HBUINT32;
  استعمال HBGlyphID = HBGlyphID24;
  استعمال وٿي_2 = Offset24;
  /* Long offsets for large tables. */
  استعمال LOffset = Offset32;
  سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
  استعمال وٿي_منزل = OT::Offset24_منزل<Type, بنياد_Type, has_null>;
  سانچو <قسم_نالو Type, قسم_نالو بنياد_Type=فراغ, منطقي has_null=سچ>
  استعمال LOffset_منزل = OT::Offset32_منزل<Type, بنياد_Type, has_null>;
  سانچو <قسم_نالو Type>
  استعمال Array_جو = OT::Array24_جو<Type>;
  سانچو <قسم_نالو Type>
  استعمال Sorted_Array_جو = OT::Sorted_Array24_جو<Type>;
};

}
}


سانچو <قسم_نالو هيٺاهون_2, قسم_نالو مٿانهون_2>
بنية hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    مٿانهون (face),
#جيڪڏهن_پورو
    هيٺاهون (
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
	   مٿانهون.has_معلومات () ? hb_face_get_خالي_جايون () :
#جيڪڏهن_پورو
	   face)
  {}

  منطقي has_معلومات () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (مٿانهون.has_معلومات ()) أرجع سچ;
#جيڪڏهن_پورو
    أرجع هيٺاهون.has_معلومات ();
  }

  منطقي has_مٿانهون_معلومات () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    أرجع مٿانهون.has_معلومات ();
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  مٿانهون_2 مٿانهون;
#جيڪڏهن_پورو
  هيٺاهون_2 هيٺاهون;
};

سانچو <قسم_نالو جدول_3>
بنية hb_جدول_accelerator_t
{
  hb_جدول_accelerator_t (hb_face_t *face) :
    جدول (hb_sanitize_context_t ().reference_جدول<جدول_3> (face))
  {}
  ~hb_جدول_accelerator_t () { جدول.destroy (); }

  منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

  hb_blob_ptr_t<جدول_3> جدول;
};

سانچو <قسم_نالو هيٺاهون_2, قسم_نالو مٿانهون_2>
بنية hb_dual_جدول_t : hb_dual_accelerator_t<hb_جدول_accelerator_t<هيٺاهون_2>,
					       hb_جدول_accelerator_t<مٿانهون_2>>
{
  hb_dual_جدول_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_جدول_accelerator_t<هيٺاهون_2>,
			  hb_جدول_accelerator_t<مٿانهون_2>> (face) {}
};

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
#وصف_ڪريو HB_DUAL_GET(accel, اظهار) ((accel).مٿانهون.has_معلومات () ? (accel).مٿانهون.اظهار : (accel).هيٺاهون.اظهار)
#وإلا
#وصف_ڪريو HB_DUAL_GET(accel, اظهار) ((accel).هيٺاهون.اظهار)
#جيڪڏهن_پورو


#جيڪڏهن_پورو  /* HB_OT_DUAL_HH */
