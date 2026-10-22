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

#अगर_अघोषित HB_OT_DUAL_HH
#घोषणा HB_OT_DUAL_HH

#समावेश "hb-open-type.hh"


namespace OT {
namespace Layout {

काष्ठा SmallTypes {
  अटल constexpr अचिन्हित आकार = 2;
  using large_int = uint32_t;
  using HBUINT = HBUINT16;
  using HBLUINT = HBUINT16;
  using HBGlyphID = HBGlyphID16;
  using Offset = Offset16;
  using LOffset = Offset16;
  ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true>
  using OffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true>
  using LOffsetTo = OT::Offset16To<Type, BaseType, has_null>;
  ढाँचा <typename Type>
  using ArrayOf = OT::Array16Of<Type>;
  ढाँचा <typename Type>
  using SortedArrayOf = OT::SortedArray16Of<Type>;
};

काष्ठा MediumTypes {
  अटल constexpr अचिन्हित आकार = 3;
  using large_int = uint64_t;
  using HBUINT = HBUINT24;
  using HBLUINT = HBUINT32;
  using HBGlyphID = HBGlyphID24;
  using Offset = Offset24;
  /* Long offsets for large tables. */
  using LOffset = Offset32;
  ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true>
  using OffsetTo = OT::Offset24To<Type, BaseType, has_null>;
  ढाँचा <typename Type, typename BaseType=व्योम, bool has_null=true>
  using LOffsetTo = OT::Offset32To<Type, BaseType, has_null>;
  ढाँचा <typename Type>
  using ArrayOf = OT::Array24Of<Type>;
  ढाँचा <typename Type>
  using SortedArrayOf = OT::SortedArray24Of<Type>;
};

}
}


ढाँचा <typename Lower, typename Upper>
काष्ठा hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#अगर_अघोषित HB_NO_BEYOND_64K
    upper (face),
#पूर्ण_अगर
    lower (
#अगर_अघोषित HB_NO_BEYOND_64K
	   upper.has_तथ्य () ? hb_face_get_empty () :
#पूर्ण_अगर
	   face)
  {}

  bool has_तथ्य () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (upper.has_तथ्य ()) वापस true;
#पूर्ण_अगर
    वापस lower.has_तथ्य ();
  }

  bool has_upper_तथ्य () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    वापस upper.has_तथ्य ();
#अन्यथा
    वापस false;
#पूर्ण_अगर
  }

#अगर_अघोषित HB_NO_BEYOND_64K
  Upper upper;
#पूर्ण_अगर
  Lower lower;
};

ढाँचा <typename Table>
काष्ठा hb_table_accelerator_t
{
  hb_table_accelerator_t (hb_face_t *face) :
    table (hb_sanitize_context_t ().reference_table<Table> (face))
  {}
  ~hb_table_accelerator_t () { table.destroy (); }

  bool has_तथ्य () स्थिर { वापस table->has_तथ्य (); }

  hb_blob_ptr_t<Table> table;
};

ढाँचा <typename Lower, typename Upper>
काष्ठा hb_dual_table_t : hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
					       hb_table_accelerator_t<Upper>>
{
  hb_dual_table_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_table_accelerator_t<Lower>,
			  hb_table_accelerator_t<Upper>> (face) {}
};

#अगर_अघोषित HB_NO_BEYOND_64K
#घोषणा HB_DUAL_GET(accel, व्यंजक) ((accel).upper.has_तथ्य () ? (accel).upper.व्यंजक : (accel).lower.व्यंजक)
#अन्यथा
#घोषणा HB_DUAL_GET(accel, व्यंजक) ((accel).lower.व्यंजक)
#पूर्ण_अगर


#पूर्ण_अगर  /* HB_OT_DUAL_HH */
