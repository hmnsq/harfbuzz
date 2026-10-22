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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-dual.hh"

நாமாகாஶ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

கட்டமைப்பு maxpV1Tail
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
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
  HBUINT16 max_அடுக்கு_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 maxSizeOfInstructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 பொது:
  DEFINE_SIZE_STATIC (26);
};


template <typename Types>
கட்டமைப்பு maxp_MAXP
{
  unsigned முழு get_இலக்கம்_glyphs () மாறிலி { திரும்பு இலக்கம்_Glyphs; }

  வெறுமை set_இலக்கம்_glyphs (unsigned முழு மொத்தம்)
  {
    இலக்கம்_Glyphs = மொத்தம்;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();
    எனில் (version.major == 1)
    {
      மாறிலி maxpV1Tail &v1 = StructAfter<maxpV1Tail> (*இது);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    maxp_MAXP *maxp_பகாஎண் = c->serializer->embed (இது);
    எனில் (unlikely (!maxp_பகாஎண்)) return_trace (பொய்);

    maxp_பகாஎண்->இலக்கம்_Glyphs = hb_min (c->plan->இலக்கம்_output_glyphs (),
				    (1u << (8 * Types::size)) - 1);
    எனில் (maxp_பகாஎண்->version.major == 1)
    {
      hb_barrier ();
      மாறிலி maxpV1Tail *src_v1 = &StructAfter<maxpV1Tail> (*இது);
      maxpV1Tail *dest_v1 = c->serializer->embed<maxpV1Tail> (src_v1);
      எனில் (unlikely (!dest_v1)) return_trace (பொய்);

      எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      எனில் (c->plan->normalized_coords)
        instancing_update_fields (c->plan->head_maxp_info, dest_v1);
    }

    return_trace (உண்மை);
  }

  வெறுமை instancing_update_fields (head_maxp_info_t& maxp_info, maxpV1Tail* dest_v1) மாறிலி
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  நிலையான வெறுமை drop_hint_fields (maxpV1Tail* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_அடுக்கு_Elements = 0;
    dest_v1->maxSizeOfInstructions = 0;
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		இலக்கம்_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  பொது:
  DEFINE_SIZE_STATIC (4 + Types::HBUINT::static_size);
};

கட்டமைப்பு maxp : maxp_MAXP<Layout::SmallTypes>
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

கட்டமைப்பு MAXP : maxp_MAXP<Layout::MediumTypes>
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

கட்டமைப்பு maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    இலக்கம்_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (init_table<MAXP> (face))
      திரும்பு;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned முழு get_இலக்கம்_glyphs () மாறிலி { திரும்பு இலக்கம்_Glyphs; }
  hb_blob_t *get_blob () மாறிலி { திரும்பு blob; }

  தனிப்பட்ட:
  template <typename Table>
  தருக்க init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_இலக்கம்_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    தருக்க ret = table.get_length ();
    எனில் (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      இலக்கம்_Glyphs = table->get_இலக்கம்_glyphs ();
    }
    table.destroy ();
    திரும்பு ret;
  }

  hb_blob_t *blob;
  unsigned முழு இலக்கம்_Glyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
