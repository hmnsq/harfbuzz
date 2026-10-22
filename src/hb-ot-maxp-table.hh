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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"

નામાકાશ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#define HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#define HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

સંરચના maxpV1Tail
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
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
  HBUINT16 max_ઢગલો_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 maxSizeOfInstructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 જાહેર:
  DEFINE_SIZE_STATIC (26);
};


template <typename Types>
સંરચના maxp_MAXP
{
  unsigned પૂર્ણાંક get_અંક_glyphs () અચલ { પાછા અંક_Glyphs; }

  શૂન્ય set_અંક_glyphs (unsigned પૂર્ણાંક ગણતરી)
  {
    અંક_Glyphs = ગણતરી;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();
    જો (version.major == 1)
    {
      અચલ maxpV1Tail &v1 = StructAfter<maxpV1Tail> (*આ);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    maxp_MAXP *maxp_અવિભાજ્ય = c->serializer->embed (આ);
    જો (unlikely (!maxp_અવિભાજ્ય)) return_trace (ખોટું);

    maxp_અવિભાજ્ય->અંક_Glyphs = hb_min (c->plan->અંક_output_glyphs (),
				    (1u << (8 * Types::size)) - 1);
    જો (maxp_અવિભાજ્ય->version.major == 1)
    {
      hb_barrier ();
      અચલ maxpV1Tail *src_v1 = &StructAfter<maxpV1Tail> (*આ);
      maxpV1Tail *dest_v1 = c->serializer->embed<maxpV1Tail> (src_v1);
      જો (unlikely (!dest_v1)) return_trace (ખોટું);

      જો (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      જો (c->plan->normalized_coords)
        instancing_update_fields (c->plan->head_maxp_info, dest_v1);
    }

    return_trace (સાચું);
  }

  શૂન્ય instancing_update_fields (head_maxp_info_t& maxp_info, maxpV1Tail* dest_v1) અચલ
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  સ્થિર શૂન્ય drop_hint_fields (maxpV1Tail* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ઢગલો_Elements = 0;
    dest_v1->maxSizeOfInstructions = 0;
  }

  સુરક્ષિત:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		અંક_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + Types::HBUINT::static_size);
};

સંરચના maxp : maxp_MAXP<Layout::SmallTypes>
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

સંરચના MAXP : maxp_MAXP<Layout::MediumTypes>
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

સંરચના maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    અંક_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ifndef HB_NO_BEYOND_64K
    જો (init_table<MAXP> (face))
      પાછા;
#endif

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  unsigned પૂર્ણાંક get_અંક_glyphs () અચલ { પાછા અંક_Glyphs; }
  hb_blob_t *get_blob () અચલ { પાછા blob; }

  ખાનગી:
  template <typename Table>
  તાર્કિક init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_અંક_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    તાર્કિક ret = table.get_length ();
    જો (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      અંક_Glyphs = table->get_અંક_glyphs ();
    }
    table.destroy ();
    પાછા ret;
  }

  hb_blob_t *blob;
  unsigned પૂર્ણાંક અંક_Glyphs;
};


} /* namespace OT */


#endif /* HB_OT_MAXP_TABLE_HH */
