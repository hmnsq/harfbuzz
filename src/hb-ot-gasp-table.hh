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

#ifndef HB_OT_GASP_TABLE_HH
#define HB_OT_GASP_TABLE_HH

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-hhea-table.hh"
#അന്തര്ഭാവ "hb-ot-os2-table.hh"
#അന്തര്ഭാവ "hb-ot-var-hvar-table.hh"

/*
 * gasp -- Grid-fitting and Scan-conversion Procedure
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gasp
 */
#define HB_OT_TAG_gasp HB_TAG('g','a','s','p')


നാമാകാശ OT {

ഘടന GaspRange
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	rangeMaxPPEM;	/* Upper limit of range, in PPEM */
  HBUINT16	rangeGaspBehavior;
				/* Flags describing desired rasterizer behavior. */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന gasp
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_gasp;

  സ്ഥിരം GaspRange &get_gasp_range (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ gaspRanges[i]; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  gaspRanges.sanitize (c));
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Version number (set to 1) */
  Array16Of<GaspRange>
		gaspRanges;	/* Number of records to follow
				 * Sorted by ppem */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, gaspRanges);
};

} /* namespace OT */


#endif /* HB_OT_GASP_TABLE_HH */
