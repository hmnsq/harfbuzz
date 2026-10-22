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

#నిర్వచితం_కాకపోతే HB_AAT_LTAG_పట్టిక_HH
#నిర్వచించు HB_AAT_LTAG_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_ltag HB_ట్యాగ్('l','t','a','g')


నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;


నిర్మాణం FTStringRange
{
  మిత్ర నిర్మాణం ltag;

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  (పునాది+ట్యాగ్_2).sanitize (c, నిడివి));
  }

  రక్షిత:
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		ట్యాగ్_2;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	నిడివి;		/* String length (in bytes) */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం ltag
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_ltag;

  hb_language_t get_language (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    స్థిరరాశి FTStringRange &range = ట్యాగ్_Ranges[i];
    తిరిగి hb_language_from_string ((స్థిరరాశి అక్షరం *) (ఇది+range.ట్యాగ్_2).arrayZ,
				    range.నిడివి);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ >= 1 &&
			  ట్యాగ్_Ranges.sanitize (c, ఇది)));
  }

  రక్షిత:
  HBUINT32	సంస్కరణ;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32_యొక్క<FTStringRange>
		ట్యాగ్_Ranges;	/* Range for each tag's string */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, ట్యాగ్_Ranges);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LTAG_TABLE_HH */
