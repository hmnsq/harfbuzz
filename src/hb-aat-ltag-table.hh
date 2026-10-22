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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LTAG_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LTAG_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_ltag HB_ਟੈਗ('l','t','a','g')


ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;


ਬਣਤਰ FTStringRange
{
  ਮਿੱਤਰ ਬਣਤਰ ltag;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  (ਅਧਾਰ+ਟੈਗ_2).sanitize (c, ਲੰਮਾਈ));
  }

  ਸੁਰੱਖਿਅਤ:
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		ਟੈਗ_2;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	ਲੰਮਾਈ;		/* String length (in bytes) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ ltag
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_ltag;

  hb_language_t get_language (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਅਚਲ FTStringRange &range = ਟੈਗ_Ranges[i];
    ਵਾਪਸ hb_language_from_string ((ਅਚਲ ਅੱਖਰ *) (ਇਹ+range.ਟੈਗ_2).arrayZ,
				    range.ਲੰਮਾਈ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ >= 1 &&
			  ਟੈਗ_Ranges.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	ਸੰਸਕਰਣ;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32_ਦਾ<FTStringRange>
		ਟੈਗ_Ranges;	/* Range for each tag's string */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, ਟੈਗ_Ranges);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LTAG_TABLE_HH */
