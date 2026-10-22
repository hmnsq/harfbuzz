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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

نطاق OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

بنية maxpV1Tail
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
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
  HBUINT16 maxStackElements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 maxSizeOfInstructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 عام:
  DEFINE_SIZE_STATIC (26);
};


template <typename Types>
بنية maxp_MAXP
{
  unsigned صحيح get_num_glyphs () ثابت { ܦܢܐ numGlyphs; }

  فراغ set_num_glyphs (unsigned صحيح count)
  {
    numGlyphs = count;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!c->check_struct (هذا)))
      return_trace (ܕܓܠܐ);
    hb_barrier ();
    ܐܢ (version.major == 1)
    {
      ثابت maxpV1Tail &v1 = StructAfter<maxpV1Tail> (*هذا);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    maxp_MAXP *maxp_prime = c->serializer->embed (هذا);
    ܐܢ (unlikely (!maxp_prime)) return_trace (ܕܓܠܐ);

    maxp_prime->numGlyphs = hb_min (c->plan->num_output_glyphs (),
				    (1u << (8 * Types::size)) - 1);
    ܐܢ (maxp_prime->version.major == 1)
    {
      hb_barrier ();
      ثابت maxpV1Tail *src_v1 = &StructAfter<maxpV1Tail> (*هذا);
      maxpV1Tail *dest_v1 = c->serializer->embed<maxpV1Tail> (src_v1);
      ܐܢ (unlikely (!dest_v1)) return_trace (ܕܓܠܐ);

      ܐܢ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      ܐܢ (c->plan->normalized_coords)
        instancing_update_fields (c->plan->head_maxp_info, dest_v1);
    }

    return_trace (ܫܪܪܐ);
  }

  فراغ instancing_update_fields (head_maxp_info_t& maxp_info, maxpV1Tail* dest_v1) ثابت
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  ساكن فراغ drop_hint_fields (maxpV1Tail* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->maxStackElements = 0;
    dest_v1->maxSizeOfInstructions = 0;
  }

  محمي:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		numGlyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  عام:
  DEFINE_SIZE_STATIC (4 + Types::HBUINT::static_size);
};

بنية maxp : maxp_MAXP<Layout::SmallTypes>
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

بنية MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

بنية maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    numGlyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (init_table<MAXP> (face))
      ܦܢܐ;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned صحيح get_num_glyphs () ثابت { ܦܢܐ numGlyphs; }
  hb_blob_t *get_blob () ثابت { ܦܢܐ blob; }

  خاص:
  template <typename Table>
  منطقي init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_num_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    منطقي ret = table.get_length ();
    ܐܢ (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      numGlyphs = table->get_num_glyphs ();
    }
    table.destroy ();
    ܦܢܐ ret;
  }

  hb_blob_t *blob;
  unsigned صحيح numGlyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
