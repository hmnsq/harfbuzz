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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"

ନାମାକାଶ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

ସଂରଚନା maxpV1Tail
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
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
  HBUINT16 max_ଗଦା_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 maxSizeOfInstructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (26);
};


template <typename Types>
ସଂରଚନା maxp_MAXP
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_glyphs () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Glyphs; }

  ଶୂନ୍ୟ set_ଅଙ୍କ_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    ଅଙ୍କ_Glyphs = ଗଣନା_2;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଯଦି (version.major == 1)
    {
      ଅଚଳ maxpV1Tail &v1 = StructAfter<maxpV1Tail> (*ଏହି);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    maxp_MAXP *maxp_ମୌଳିକ = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!maxp_ମୌଳିକ)) return_trace (ମିଥ୍ୟା);

    maxp_ମୌଳିକ->ଅଙ୍କ_Glyphs = hb_min (c->plan->ଅଙ୍କ_output_glyphs (),
				    (1u << (8 * Types::size)) - 1);
    ଯଦି (maxp_ମୌଳିକ->version.major == 1)
    {
      hb_barrier ();
      ଅଚଳ maxpV1Tail *src_v1 = &StructAfter<maxpV1Tail> (*ଏହି);
      maxpV1Tail *dest_v1 = c->serializer->embed<maxpV1Tail> (src_v1);
      ଯଦି (unlikely (!dest_v1)) return_trace (ମିଥ୍ୟା);

      ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      ଯଦି (c->plan->normalized_coords)
        instancing_update_fields (c->plan->head_maxp_info, dest_v1);
    }

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ instancing_update_fields (head_maxp_info_t& maxp_info, maxpV1Tail* dest_v1) ଅଚଳ
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  ସ୍ଥିର ଶୂନ୍ୟ drop_hint_fields (maxpV1Tail* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ଗଦା_Elements = 0;
    dest_v1->maxSizeOfInstructions = 0;
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		ଅଙ୍କ_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + Types::HBUINT::static_size);
};

ସଂରଚନା maxp : maxp_MAXP<Layout::SmallTypes>
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

ସଂରଚନା MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

ସଂରଚନା maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    ଅଙ୍କ_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (init_table<MAXP> (face))
      ଫେରନ୍ତୁ;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_glyphs () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Glyphs; }
  hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ blob; }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Table>
  ତାର୍କିକ init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_ଅଙ୍କ_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    ତାର୍କିକ ret = table.get_length ();
    ଯଦି (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      ଅଙ୍କ_Glyphs = table->get_ଅଙ୍କ_glyphs ();
    }
    table.destroy ();
    ଫେରନ୍ତୁ ret;
  }

  hb_blob_t *blob;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_Glyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
