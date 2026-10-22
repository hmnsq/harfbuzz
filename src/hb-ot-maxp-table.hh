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

#ifndef HB_OT_MAXP_TABLE_HH
#define HB_OT_MAXP_TABLE_HH

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-dual.hh"

নামাকাশ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

কাঠামো maxp_V1_লেজ
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT16 maxPoints;		  /* Maximum points in a non-composite glyph. */
  HBUINT16 maxContours;		  /* Maximum contours in a non-composite glyph. */
  HBUINT16 maxCompositePoints;	  /* Maximum points in a composite glyph. */
  HBUINT16 maxCompositeContours;  /* Maximum contours in a composite glyph. */
  HBUINT16 maxZones;		  /* 1 if instructions do not use the twilight zone (Z0),
				   * or 2 if instructions do use Z0; should be set to 2 in
				   * most cases. */
  HBUINT16 maxTwilightPoints;	  /* Maximum points used in Z0. */
  HBUINT16 maxStorage;		  /* Number of Storage Area locations. */
  HBUINT16 maxFunctionDefs;	  /* Number of FDEFs, equal to the highest function number + 1. */
  HBUINT16 maxInstructionDefs;	  /* Number of IDEFs. */
  HBUINT16 max_স্তূপ_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_আকার_Of_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 সর্বজনীন:
  DEFINE_আকার_STATIC (26);
};


template <typename Types>
কাঠামো maxp_MAXP
{
  unsigned পূর্ণসংখ্যা get_অঙ্ক_glyphs () ধ্রুবক { ফেরত অঙ্ক_Glyphs; }

  শূন্য set_অঙ্ক_glyphs (unsigned পূর্ণসংখ্যা গুনতি)
  {
    অঙ্ক_Glyphs = গুনতি;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();
    যদি (version.major == 1)
    {
      ধ্রুবক maxp_V1_লেজ &v1 = StructAfter<maxp_V1_লেজ> (*এই);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    maxp_MAXP *maxp_মৌলিক = c->serializer->embed (এই);
    যদি (unlikely (!maxp_মৌলিক)) return_trace (মিথ্যা);

    maxp_মৌলিক->অঙ্ক_Glyphs = hb_min (c->plan->অঙ্ক_output_glyphs (),
				    (1u << (8 * Types::আকার_2)) - 1);
    যদি (maxp_মৌলিক->version.major == 1)
    {
      hb_barrier ();
      ধ্রুবক maxp_V1_লেজ *src_v1 = &StructAfter<maxp_V1_লেজ> (*এই);
      maxp_V1_লেজ *dest_v1 = c->serializer->embed<maxp_V1_লেজ> (src_v1);
      যদি (unlikely (!dest_v1)) return_trace (মিথ্যা);

      যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      যদি (c->plan->normalized_coords)
        instancing_update_fields (c->plan->শীর্ষ_maxp_info, dest_v1);
    }

    return_trace (সত্য);
  }

  শূন্য instancing_update_fields (শীর্ষ_maxp_info_t& maxp_info, maxp_V1_লেজ* dest_v1) ধ্রুবক
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  স্থির শূন্য drop_hint_fields (maxp_V1_লেজ* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_স্তূপ_Elements = 0;
    dest_v1->max_আকার_Of_Instructions = 0;
  }

  সুরক্ষিত:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		অঙ্ক_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + Types::HBUINT::static_আকার);
};

কাঠামো maxp : maxp_MAXP<Layout::SmallTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

কাঠামো MAXP : maxp_MAXP<Layout::MediumTypes>
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

কাঠামো maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    অঙ্ক_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    যদি (init_table<MAXP> (face))
      ফেরত;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned পূর্ণসংখ্যা get_অঙ্ক_glyphs () ধ্রুবক { ফেরত অঙ্ক_Glyphs; }
  hb_blob_t *get_blob () ধ্রুবক { ফেরত blob; }

  ব্যক্তিগত:
  template <typename Table>
  যৌক্তিক init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_অঙ্ক_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    যৌক্তিক ret = table.get_দৈর্ঘ্য ();
    যদি (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      অঙ্ক_Glyphs = table->get_অঙ্ক_glyphs ();
    }
    table.destroy ();
    ফেরত ret;
  }

  hb_blob_t *blob;
  unsigned পূর্ণসংখ্যা অঙ্ক_Glyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
