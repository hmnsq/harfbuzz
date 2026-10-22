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

#ifndef HB_OT_DUAL_HH
#define HB_OT_DUAL_HH

#شامل "hb-open-type.hh"


فضاینام OT {
فضاینام Layout {

ساختار SmallTypes {
  ایستا constexpr unsigned size = 2;
  using large_int = uint32_t;
  using HBUINT = HBUINT16;
  using HBLUINT = HBUINT16;
  using HBGlyphID = HBGlyphID16;
  using Offset = Offset16;
  using LOffset = Offset16;
  template <typename Type, typename BaseType=تهی, منطقی has_null=درست>
  using OffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  template <typename Type, typename BaseType=تهی, منطقی has_null=درست>
  using LOffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  template <typename Type>
  using ArrayOf = OT::Array16Of<Type>;
  template <typename Type>
  using SortedArrayOf = OT::SortedArray16Of<Type>;
};

ساختار MediumTypes {
  ایستا constexpr unsigned size = 3;
  using large_int = uint64_t;
  using HBUINT = HBUINT24;
  using HBLUINT = HBUINT32;
  using HBGlyphID = HBGlyphID24;
  using Offset = Offset24;
  /* Long offsets for large tables. */
  using LOffset = Offset32;
  template <typename Type, typename BaseType=تهی, منطقی has_null=درست>
  using OffsetTo = OT::Offset24To<Type, BaseType, has_null>;
  template <typename Type, typename BaseType=تهی, منطقی has_null=درست>
  using LOffsetTo = OT::Offset32To<Type, BaseType, has_null>;
  template <typename Type>
  using ArrayOf = OT::Array24Of<Type>;
  template <typename Type>
  using SortedArrayOf = OT::SortedArray24Of<Type>;
};

}
}


template <typename Lower, typename Upper>
ساختار hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#ifndef HB_NO_BEYOND_64K
    upper (face),
#endif
    lower (
#ifndef HB_NO_BEYOND_64K
	   upper.has_data () ? hb_face_get_empty () :
#endif
	   face)
  {}

  منطقی has_data () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (upper.has_data ()) بازگشت درست;
#endif
    بازگشت lower.has_data ();
  }

  منطقی has_upper_data () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    بازگشت upper.has_data ();
#وگرنه
    بازگشت نادرست;
#endif
  }

#ifndef HB_NO_BEYOND_64K
  Upper upper;
#endif
  Lower lower;
};

template <typename Table>
ساختار hb_table_accelerator_t
{
  hb_table_accelerator_t (hb_face_t *face) :
    table (hb_sanitize_context_t ().reference_table<Table> (face))
  {}
  ~hb_table_accelerator_t () { table.destroy (); }

  منطقی has_data () ثابت { بازگشت table->has_data (); }

  hb_blob_ptr_t<Table> table;
};

template <typename Lower, typename Upper>
ساختار hb_dual_table_t : hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
					       hb_table_accelerator_t<Upper>>
{
  hb_dual_table_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
			  hb_table_accelerator_t<Upper>> (face) {}
};

#ifndef HB_NO_BEYOND_64K
#define HB_DUAL_GET(accel, عبارت) ((accel).upper.has_data () ? (accel).upper.عبارت : (accel).lower.عبارت)
#وگرنه
#define HB_DUAL_GET(accel, عبارت) ((accel).lower.عبارت)
#endif


#endif  /* HB_OT_DUAL_HH */
