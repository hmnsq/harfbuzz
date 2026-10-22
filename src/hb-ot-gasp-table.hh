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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_GASP_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_GASP_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hhea-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-hvar-table.hh"

/*
 * gasp -- Grid-fitting and Scan-conversion Procedure
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gasp
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_gasp HB_ਟੈਗ('g','a','s','p')


ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ GaspRange
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	rangeMaxPPEM;	/* Upper limit of range, in PPEM */
  HBUINT16	rangeGaspBehavior;
				/* Flags describing desired rasterizer behavior. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ gasp
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_gasp;

  ਅਚਲ GaspRange &get_gasp_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ gaspRanges[i]; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  gaspRanges.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Version number (set to 1) */
  Array16_ਦਾ<GaspRange>
		gaspRanges;	/* Number of records to follow
				 * Sorted by ppem */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, gaspRanges);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_GASP_TABLE_HH */
