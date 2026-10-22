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

#अन्तर्भाव "hb-open-type.hh"


नामाकाश OT {
नामाकाश Layout {

संरचना SmallTypes {
  स्थिर constexpr अचिन्हित size = 2;
  using large_int = uint32_t;
  using HBUINT = HBUINT16;
  using HBLUINT = HBUINT16;
  using HBGlyphID = HBGlyphID16;
  using Offset = Offset16;
  using LOffset = Offset16;
  ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य>
  using OffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य>
  using LOffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  ढाँचा <typename Type>
  using ArrayOf = OT::Array16Of<Type>;
  ढाँचा <typename Type>
  using SortedArrayOf = OT::SortedArray16Of<Type>;
};

संरचना MediumTypes {
  स्थिर constexpr अचिन्हित size = 3;
  using large_int = uint64_t;
  using HBUINT = HBUINT24;
  using HBLUINT = HBUINT32;
  using HBGlyphID = HBGlyphID24;
  using Offset = Offset24;
  /* Long offsets for large tables. */
  using LOffset = Offset32;
  ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य>
  using OffsetTo = OT::Offset24To<Type, BaseType, has_null>;
  ढाँचा <typename Type, typename BaseType=शून्य, तार्किक has_null=सत्य>
  using LOffsetTo = OT::Offset32To<Type, BaseType, has_null>;
  ढाँचा <typename Type>
  using ArrayOf = OT::Array24Of<Type>;
  ढाँचा <typename Type>
  using SortedArrayOf = OT::SortedArray24Of<Type>;
};

}
}


ढाँचा <typename Lower, typename Upper>
संरचना hb_dual_accelerator_t
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

  तार्किक has_data () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (upper.has_data ()) परत सत्य;
#endif
    परत lower.has_data ();
  }

  तार्किक has_upper_data () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    परत upper.has_data ();
#अन्यथा
    परत असत्य;
#endif
  }

#ifndef HB_NO_BEYOND_64K
  Upper upper;
#endif
  Lower lower;
};

ढाँचा <typename Table>
संरचना hb_table_accelerator_t
{
  hb_table_accelerator_t (hb_face_t *face) :
    table (hb_sanitize_context_t ().reference_table<Table> (face))
  {}
  ~hb_table_accelerator_t () { table.destroy (); }

  तार्किक has_data () स्थिरांक { परत table->has_data (); }

  hb_blob_ptr_t<Table> table;
};

ढाँचा <typename Lower, typename Upper>
संरचना hb_dual_table_t : hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
					       hb_table_accelerator_t<Upper>>
{
  hb_dual_table_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
			  hb_table_accelerator_t<Upper>> (face) {}
};

#ifndef HB_NO_BEYOND_64K
#define HB_DUAL_GET(accel, अभिव्यक्ती) ((accel).upper.has_data () ? (accel).upper.अभिव्यक्ती : (accel).lower.अभिव्यक्ती)
#अन्यथा
#define HB_DUAL_GET(accel, अभिव्यक्ती) ((accel).lower.अभिव्यक्ती)
#endif


#endif  /* HB_OT_DUAL_HH */
