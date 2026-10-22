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

#నిర్వచితం_కాకపోతే HB_OT_GASP_పట్టిక_HH
#నిర్వచించు HB_OT_GASP_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-hhea-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-var-hvar-table.hh"

/*
 * gasp -- Grid-fitting and Scan-conversion Procedure
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gasp
 */
#నిర్వచించు HB_OT_ట్యాగ్_gasp HB_ట్యాగ్('g','a','s','p')


నామాకాశ OT {

నిర్మాణం GaspRange
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	rangeMaxPPEM;	/* Upper limit of range, in PPEM */
  HBUINT16	rangeGaspBehavior;
				/* Flags describing desired rasterizer behavior. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం gasp
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_gasp;

  స్థిరరాశి GaspRange &get_gasp_range (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి gaspRanges[i]; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  gaspRanges.sanitize (c));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Version number (set to 1) */
  Array16_యొక్క<GaspRange>
		gaspRanges;	/* Number of records to follow
				 * Sorted by ppem */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, gaspRanges);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_GASP_TABLE_HH */
