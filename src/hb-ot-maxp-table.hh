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

#اگر_غیر_تعریف_شدہ HB_OT_MAXP_جدول_HH
#تعریف_کرو HB_OT_MAXP_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"

نامخلا OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#تعریف_کرو HB_OT_لیبل_maxp HB_لیبل('m','a','x','p')
#تعریف_کرو HB_OT_لیبل_MAXP HB_لیبل('M','A','X','P')

ساخت maxp_V1_دم
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
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
  HBUINT16 max_ڈھیر_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_سائز_کا_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 max_Component_گہرائی;	  /* Maximum levels of recursion; 1 for simple components. */
 عوامی:
  DEFINE_سائز_STATIC (26);
};


سانچہ <قسم_نام Types>
ساخت maxp_MAXP
{
  غیرعلامتی صحیح get_عدد_glyphs () ثابت { واپس عدد_Glyphs; }

  خالی سیٹ_عدد_glyphs (غیرعلامتی صحیح گنتی)
  {
    عدد_Glyphs = گنتی;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();
    اگر (نسخہ.major == 1)
    {
      ثابت maxp_V1_دم &v1 = StructAfter<maxp_V1_دم> (*یہ);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (نسخہ.major == 0 && نسخہ.minor == 0x5000u));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    maxp_MAXP *maxp_مفرد = c->serializer->embed (یہ);
    اگر (unlikely (!maxp_مفرد)) return_trace (جھوٹ);

    maxp_مفرد->عدد_Glyphs = hb_min (c->plan->عدد_output_glyphs (),
				    (1u << (8 * Types::سائز)) - 1);
    اگر (maxp_مفرد->نسخہ.major == 1)
    {
      hb_barrier ();
      ثابت maxp_V1_دم *src_v1 = &StructAfter<maxp_V1_دم> (*یہ);
      maxp_V1_دم *dest_v1 = c->serializer->embed<maxp_V1_دم> (src_v1);
      اگر (unlikely (!dest_v1)) return_trace (جھوٹ);

      اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      اگر (c->plan->normalized_coords)
        instancing_update_fields (c->plan->سر_maxp_info, dest_v1);
    }

    return_trace (سچ);
  }

  خالی instancing_update_fields (سر_maxp_info_t& maxp_info, maxp_V1_دم* dest_v1) ثابت
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->max_مرکب_Points = maxp_info.max_مرکب_Points;
    dest_v1->max_مرکب_Contours = maxp_info.max_مرکب_Contours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->max_Component_گہرائی = maxp_info.max_Component_گہرائی;
  }

  ساکن خالی drop_hint_fields (maxp_V1_دم* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ڈھیر_Elements = 0;
    dest_v1->max_سائز_کا_Instructions = 0;
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  قسم_نام Types::HBUINT
		عدد_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  عوامی:
  DEFINE_سائز_STATIC (4 + Types::HBUINT::static_سائز);
};

ساخت maxp : maxp_MAXP<Layout::SmallTypes>
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_maxp;
};

ساخت MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_MAXP;
};

ساخت maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_خالی_خانے ();
    عدد_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (init_جدول<MAXP> (face))
      واپس;
#اگر_ختم

    init_جدول<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  غیرعلامتی صحیح get_عدد_glyphs () ثابت { واپس عدد_Glyphs; }
  hb_blob_t *get_blob () ثابت { واپس blob; }

  نجی:
  سانچہ <قسم_نام جدول_3>
  منطقی init_جدول (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.سیٹ_عدد_glyphs (0);
    hb_blob_ptr_t<جدول_3> جدول = c.reference_جدول<جدول_3> (face);
    منطقی ret = جدول.get_لمبائی ();
    اگر (ret)
    {
      blob = hb_blob_reference (جدول.get_blob ());
      عدد_Glyphs = جدول->get_عدد_glyphs ();
    }
    جدول.destroy ();
    واپس ret;
  }

  hb_blob_t *blob;
  غیرعلامتی صحیح عدد_Glyphs;
};


} /* namespace OT */


#اگر_ختم /* HB_OT_MAXP_TABLE_HH */
