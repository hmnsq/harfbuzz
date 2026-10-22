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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-dual.hh"

ನಾಮಾಕಾಶ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

ಸಂರಚನಾ maxpV1Tail
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
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
  HBUINT16 max_ರಾಶಿ_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 maxSizeOfInstructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (26);
};


template <typename Types>
ಸಂರಚನಾ maxp_MAXP
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_glyphs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Glyphs; }

  ಶೂನ್ಯ set_ಅಂಕೆ_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    ಅಂಕೆ_Glyphs = ಎಣಿಕೆ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಯದಿ (version.major == 1)
    {
      ಧ್ರುವ maxpV1Tail &v1 = StructAfter<maxpV1Tail> (*ಏತತ್);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    maxp_MAXP *maxp_ಅವಿಭಾಜ್ಯ = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!maxp_ಅವಿಭಾಜ್ಯ)) return_trace (ಅಸತ್ಯ);

    maxp_ಅವಿಭಾಜ್ಯ->ಅಂಕೆ_Glyphs = hb_min (c->plan->ಅಂಕೆ_output_glyphs (),
				    (1u << (8 * Types::size)) - 1);
    ಯದಿ (maxp_ಅವಿಭಾಜ್ಯ->version.major == 1)
    {
      hb_barrier ();
      ಧ್ರುವ maxpV1Tail *src_v1 = &StructAfter<maxpV1Tail> (*ಏತತ್);
      maxpV1Tail *dest_v1 = c->serializer->embed<maxpV1Tail> (src_v1);
      ಯದಿ (unlikely (!dest_v1)) return_trace (ಅಸತ್ಯ);

      ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      ಯದಿ (c->plan->normalized_coords)
        instancing_update_fields (c->plan->head_maxp_info, dest_v1);
    }

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ instancing_update_fields (head_maxp_info_t& maxp_info, maxpV1Tail* dest_v1) ಧ್ರುವ
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ drop_hint_fields (maxpV1Tail* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ರಾಶಿ_Elements = 0;
    dest_v1->maxSizeOfInstructions = 0;
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		ಅಂಕೆ_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + Types::HBUINT::static_size);
};

ಸಂರಚನಾ maxp : maxp_MAXP<Layout::SmallTypes>
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

ಸಂರಚನಾ MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

ಸಂರಚನಾ maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    ಅಂಕೆ_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (init_table<MAXP> (face))
      ಪ್ರತ್ಯಾಗಮ;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_glyphs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Glyphs; }
  hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ blob; }

  ಗುಪ್ತ:
  template <typename Table>
  ತಾರ್ಕಿಕ init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_ಅಂಕೆ_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    ತಾರ್ಕಿಕ ret = table.get_length ();
    ಯದಿ (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      ಅಂಕೆ_Glyphs = table->get_ಅಂಕೆ_glyphs ();
    }
    table.destroy ();
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_blob_t *blob;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_Glyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
