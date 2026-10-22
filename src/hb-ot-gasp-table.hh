/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#अगर_अघोषित HB_OT_GASP_TABLE_HH
#घोषणा HB_OT_GASP_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-hhea-table.hh"
#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-var-hvar-table.hh"

/*
 * gasp -- Grid-fitting and Scan-conversion Procedure
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gasp
 */
#घोषणा HB_OT_TAG_gasp HB_TAG('g','a','s','p')


namespace OT {

काष्ठा GaspRange
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	rangeMaxPPEM;	/* Upper limit of range, in PPEM */
  HBUINT16	rangeGaspBehavior;
				/* Flags describing desired rasterizer behavior. */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा gasp
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_gasp;

  स्थिर GaspRange &get_gasp_range (अचिन्हित पूर्णांक i) स्थिर
  { वापस gaspRanges[i]; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  gaspRanges.sanitize (c));
  }

  रक्षित:
  HBUINT16	version;	/* Version number (set to 1) */
  Array16Of<GaspRange>
		gaspRanges;	/* Number of records to follow
				 * Sorted by ppem */
  खुला:
  DEFINE_आकार_ARRAY (4, gaspRanges);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_GASP_TABLE_HH */
