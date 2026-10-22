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

#अगर_अघोषित HB_OT_MAXP_TABLE_HH
#घोषणा HB_OT_MAXP_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-dual.hh"

namespace OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#घोषणा HB_OT_TAG_maxp HB_TAG('m','a','x','p')
#घोषणा HB_OT_TAG_MAXP HB_TAG('M','A','X','P')

काष्ठा maxp_V1_पुच्छ
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
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
  HBUINT16 max_ढेर_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_आकार_Of_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 maxComponentDepth;	  /* Maximum levels of recursion; 1 for simple components. */
 खुला:
  DEFINE_आकार_STATIC (26);
};


ढाँचा <typename Types>
काष्ठा maxp_MAXP
{
  अचिन्हित पूर्णांक get_अंक_glyphs () स्थिर { वापस अंक_Glyphs; }

  व्योम set_अंक_glyphs (अचिन्हित पूर्णांक गिनती)
  {
    अंक_Glyphs = गिनती;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();
    अगर (version.major == 1)
    {
      स्थिर maxp_V1_पुच्छ &v1 = StructAfter<maxp_V1_पुच्छ> (*यह);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (version.major == 0 && version.minor == 0x5000u));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    maxp_MAXP *maxp_अभाज्य = c->serializer->embed (यह);
    अगर (unlikely (!maxp_अभाज्य)) return_trace (false);

    maxp_अभाज्य->अंक_Glyphs = hb_min (c->plan->अंक_output_glyphs (),
				    (1u << (8 * Types::आकार)) - 1);
    अगर (maxp_अभाज्य->version.major == 1)
    {
      hb_barrier ();
      स्थिर maxp_V1_पुच्छ *src_v1 = &StructAfter<maxp_V1_पुच्छ> (*यह);
      maxp_V1_पुच्छ *dest_v1 = c->serializer->embed<maxp_V1_पुच्छ> (src_v1);
      अगर (unlikely (!dest_v1)) return_trace (false);

      अगर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      अगर (c->plan->normalized_coords)
        instancing_update_fields (c->plan->शीर्ष_maxp_info, dest_v1);
    }

    return_trace (true);
  }

  व्योम instancing_update_fields (शीर्ष_maxp_info_t& maxp_info, maxp_V1_पुच्छ* dest_v1) स्थिर
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->maxCompositePoints = maxp_info.maxCompositePoints;
    dest_v1->maxCompositeContours = maxp_info.maxCompositeContours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->maxComponentDepth = maxp_info.maxComponentDepth;
  }

  अटल व्योम drop_hint_fields (maxp_V1_पुच्छ* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ढेर_Elements = 0;
    dest_v1->max_आकार_Of_Instructions = 0;
  }

  रक्षित:
  FixedVersion<>version;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  typename Types::HBUINT
		अंक_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  खुला:
  DEFINE_आकार_STATIC (4 + Types::HBUINT::static_आकार);
};

काष्ठा maxp : maxp_MAXP<Layout::SmallTypes>
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_maxp;
};

काष्ठा MAXP : maxp_MAXP<Layout::MediumTypes>
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_MAXP;
};

काष्ठा maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_empty ();
    अंक_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (init_table<MAXP> (face))
      वापस;
#पूर्ण_अगर

    init_table<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  अचिन्हित पूर्णांक get_अंक_glyphs () स्थिर { वापस अंक_Glyphs; }
  hb_blob_t *get_blob () स्थिर { वापस blob; }

  निजी:
  ढाँचा <typename Table>
  bool init_table (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.set_अंक_glyphs (0);
    hb_blob_ptr_t<Table> table = c.reference_table<Table> (face);
    bool ret = table.get_लंबाई ();
    अगर (ret)
    {
      blob = hb_blob_reference (table.get_blob ());
      अंक_Glyphs = table->get_अंक_glyphs ();
    }
    table.destroy ();
    वापस ret;
  }

  hb_blob_t *blob;
  अचिन्हित पूर्णांक अंक_Glyphs;
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_MAXP_TABLE_HH */
