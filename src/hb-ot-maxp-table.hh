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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_MAXP_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAXP_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"

ਨਾਮਾਕਾਸ਼ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_maxp HB_ਟੈਗ('m','a','x','p')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_MAXP HB_ਟੈਗ('M','A','X','P')

ਬਣਤਰ maxp_V1_ਪੂਛ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16 maxPoints;		  /* Maximum points in a non-composite glyph. */
  HBUINT16 maxContours;		  /* Maximum contours in a non-composite glyph. */
  HBUINT16 max_ਸੰਯੁਕਤ_Points;	  /* Maximum points in a composite glyph. */
  HBUINT16 max_ਸੰਯੁਕਤ_Contours;  /* Maximum contours in a composite glyph. */
  HBUINT16 maxZones;		  /* 1 if instructions do not use the twilight zone (Z0),
				   * or 2 if instructions do use Z0; should be set to 2 in
				   * most cases. */
  HBUINT16 maxTwilightPoints;	  /* Maximum points used in Z0. */
  HBUINT16 maxStorage;		  /* Number of Storage Area locations. */
  HBUINT16 maxFunctionDefs;	  /* Number of FDEFs, equal to the highest function number + 1. */
  HBUINT16 maxInstructionDefs;	  /* Number of IDEFs. */
  HBUINT16 max_ਢੇਰ_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_ਮਾਪ_ਦਾ_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 max_Component_ਡੂੰਘਾਈ;	  /* Maximum levels of recursion; 1 for simple components. */
 ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (26);
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ maxp_MAXP
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅੰਕ_glyphs () ਅਚਲ { ਵਾਪਸ ਅੰਕ_Glyphs; }

  ਖਾਲੀ ਸੈੱਟ_ਅੰਕ_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    ਅੰਕ_Glyphs = ਗਿਣਤੀ_2;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();
    ਜੇ (ਸੰਸਕਰਣ.major == 1)
    {
      ਅਚਲ maxp_V1_ਪੂਛ &v1 = StructAfter<maxp_V1_ਪੂਛ> (*ਇਹ);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (ਸੰਸਕਰਣ.major == 0 && ਸੰਸਕਰਣ.minor == 0x5000u));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    maxp_MAXP *maxp_ਅਭਾਜ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!maxp_ਅਭਾਜ)) return_trace (ਝੂਠ);

    maxp_ਅਭਾਜ->ਅੰਕ_Glyphs = hb_min (c->plan->ਅੰਕ_output_glyphs (),
				    (1u << (8 * Types::ਮਾਪ)) - 1);
    ਜੇ (maxp_ਅਭਾਜ->ਸੰਸਕਰਣ.major == 1)
    {
      hb_barrier ();
      ਅਚਲ maxp_V1_ਪੂਛ *src_v1 = &StructAfter<maxp_V1_ਪੂਛ> (*ਇਹ);
      maxp_V1_ਪੂਛ *dest_v1 = c->serializer->embed<maxp_V1_ਪੂਛ> (src_v1);
      ਜੇ (unlikely (!dest_v1)) return_trace (ਝੂਠ);

      ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      ਜੇ (c->plan->normalized_coords)
        instancing_update_fields (c->plan->ਸਿਰ_maxp_info, dest_v1);
    }

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ instancing_update_fields (ਸਿਰ_maxp_info_t& maxp_info, maxp_V1_ਪੂਛ* dest_v1) ਅਚਲ
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->max_ਸੰਯੁਕਤ_Points = maxp_info.max_ਸੰਯੁਕਤ_Points;
    dest_v1->max_ਸੰਯੁਕਤ_Contours = maxp_info.max_ਸੰਯੁਕਤ_Contours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->max_Component_ਡੂੰਘਾਈ = maxp_info.max_Component_ਡੂੰਘਾਈ;
  }

  ਸਥਿਰ ਖਾਲੀ drop_hint_fields (maxp_V1_ਪੂਛ* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_ਢੇਰ_Elements = 0;
    dest_v1->max_ਮਾਪ_ਦਾ_Instructions = 0;
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  ਕਿਸਮ_ਨਾਂ Types::HBUINT
		ਅੰਕ_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + Types::HBUINT::static_ਮਾਪ);
};

ਬਣਤਰ maxp : maxp_MAXP<Layout::SmallTypes>
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_maxp;
};

ਬਣਤਰ MAXP : maxp_MAXP<Layout::MediumTypes>
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_MAXP;
};

ਬਣਤਰ maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
    ਅੰਕ_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (init_ਸਾਰਣੀ<MAXP> (face))
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ

    init_ਸਾਰਣੀ<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅੰਕ_glyphs () ਅਚਲ { ਵਾਪਸ ਅੰਕ_Glyphs; }
  hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ blob; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸਾਰਣੀ_3>
  ਤਾਰਕਿਕ init_ਸਾਰਣੀ (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.ਸੈੱਟ_ਅੰਕ_glyphs (0);
    hb_blob_ptr_t<ਸਾਰਣੀ_3> ਸਾਰਣੀ = c.reference_ਸਾਰਣੀ<ਸਾਰਣੀ_3> (face);
    ਤਾਰਕਿਕ ret = ਸਾਰਣੀ.get_ਲੰਮਾਈ ();
    ਜੇ (ret)
    {
      blob = hb_blob_reference (ਸਾਰਣੀ.get_blob ());
      ਅੰਕ_Glyphs = ਸਾਰਣੀ->get_ਅੰਕ_glyphs ();
    }
    ਸਾਰਣੀ.destroy ();
    ਵਾਪਸ ret;
  }

  hb_blob_t *blob;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_Glyphs;
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_MAXP_TABLE_HH */
