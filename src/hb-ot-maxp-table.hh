/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#که_نه_تعریف_شوی HB_OT_MAXP_جدول_HH
#تعریف_کړه HB_OT_MAXP_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

نطاق OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#تعریف_کړه HB_OT_نښان_maxp HB_نښان('m','a','x','p')
#تعریف_کړه HB_OT_نښان_MAXP HB_نښان('M','A','X','P')

بنية maxp_V1_لکۍ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16 maxPoints;		  /* Maximum points in a non-composite glyph. */
  HBUINT16 maxContours;		  /* Maximum contours in a non-composite glyph. */
  HBUINT16 max_مرکب_Points;	  /* Maximum points in a composite glyph. */
  HBUINT16 max_مرکب_Contours;  /* Maximum contours in a composite glyph. */
  HBUINT16 maxZones;		  /* 1 if instructions do not use the twilight zone (Z0),
				   * or 2 if instructions do use Z0; should be set to 2 in
				   * most cases. */
  HBUINT16 maxTwilightPoints;	  /* Maximum points used in Z0. */
  HBUINT16 maxStorage;		  /* Number of Storage Area locations. */
  HBUINT16 maxFunctionDefs;	  /* Number of FDEFs, equal to the highest function number + 1. */
  HBUINT16 maxInstructionDefs;	  /* Number of IDEFs. */
  HBUINT16 max_ډېری_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_اندازه_د_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 max_Component_ژوروالی;	  /* Maximum levels of recursion; 1 for simple components. */
 عام:
  DEFINE_اندازه_STATIC (26);
};


کينډۍ <ډول_نوم Types>
بنية maxp_MAXP
{
  بینښه صحيح get_عدد_glyphs () ثابت { أرجع عدد_Glyphs; }

  فراغ سېټ_عدد_glyphs (بینښه صحيح شمېر)
  {
    عدد_Glyphs = شمېر;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();
    إذا (نسخه.major == 1)
    {
      ثابت maxp_V1_لکۍ &v1 = StructAfter<maxp_V1_لکۍ> (*هذا);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (نسخه.major == 0 && نسخه.minor == 0x5000u));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    maxp_MAXP *maxp_اولي = c->serializer->embed (هذا);
    إذا (unlikely (!maxp_اولي)) return_trace (خطأ);

    maxp_اولي->عدد_Glyphs = hb_min (c->plan->عدد_output_glyphs (),
				    (1u << (8 * Types::اندازه)) - 1);
    إذا (maxp_اولي->نسخه.major == 1)
    {
      hb_barrier ();
      ثابت maxp_V1_لکۍ *src_v1 = &StructAfter<maxp_V1_لکۍ> (*هذا);
      maxp_V1_لکۍ *dest_v1 = c->serializer->embed<maxp_V1_لکۍ> (src_v1);
      إذا (unlikely (!dest_v1)) return_trace (خطأ);

      إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      إذا (c->plan->normalized_coords)
        instancing_update_fields (c->plan->سر_maxp_info, dest_v1);
    }

    return_trace (ریښتیا);
  }

  فراغ instancing_update_fields (سر_maxp_info_t& maxp_info, maxp_V1_لکۍ* dest_v1) ثابت
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->max_مرکب_Points = maxp_info.max_مرکب_Points;
    dest_v1->max_مرکب_Contours = maxp_info.max_مرکب_Contours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->max_Component_ژوروالی = maxp_info.max_Component_ژوروالی;
  }

  ساكن فراغ drop_hint_fields (maxp_V1_لکۍ* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ډېری_Elements = 0;
    dest_v1->max_اندازه_د_Instructions = 0;
  }

  محمي:
  Fixed_نسخه<>نسخه;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  ډول_نوم Types::HBUINT
		عدد_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  عام:
  DEFINE_اندازه_STATIC (4 + Types::HBUINT::static_اندازه);
};

بنية maxp : maxp_MAXP<Layout::SmallTypes>
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_maxp;
};

بنية MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_MAXP;
};

بنية maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_تشه ();
    عدد_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (init_جدول<MAXP> (face))
      أرجع;
#د_که_پای

    init_جدول<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  بینښه صحيح get_عدد_glyphs () ثابت { أرجع عدد_Glyphs; }
  hb_blob_t *get_blob () ثابت { أرجع blob; }

  خاص:
  کينډۍ <ډول_نوم جدول_3>
  منطقي init_جدول (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.سېټ_عدد_glyphs (0);
    hb_blob_ptr_t<جدول_3> جدول = c.reference_جدول<جدول_3> (face);
    منطقي ret = جدول.get_اوږدتیا ();
    إذا (ret)
    {
      blob = hb_blob_reference (جدول.get_blob ());
      عدد_Glyphs = جدول->get_عدد_glyphs ();
    }
    جدول.destroy ();
    أرجع ret;
  }

  hb_blob_t *blob;
  بینښه صحيح عدد_Glyphs;
};


} /* namespace OT */


#د_که_پای /* HB_OT_MAXP_TABLE_HH */
