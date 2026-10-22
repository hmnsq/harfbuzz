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

#ifndef HB_AAT_LTAG_TABLE_HH
#define HB_AAT_LTAG_TABLE_HH

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#define HB_AAT_TAG_ltag HB_TAG('l','t','a','g')


ନାମାକାଶ AAT {

using ନାମାକାଶ OT;


ସଂରଚନା FTStringRange
{
  friend ସଂରଚନା ltag;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  (base+tag).sanitize (c, length));
  }

  ସୁରକ୍ଷିତ:
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		tag;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	length;		/* String length (in bytes) */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା ltag
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_ltag;

  hb_language_t get_language (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଅଚଳ FTStringRange &range = tagRanges[i];
    ଫେରନ୍ତୁ hb_language_from_string ((ଅଚଳ ଅକ୍ଷର *) (ଏହି+range.tag).arrayZ,
				    range.length);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version >= 1 &&
			  tagRanges.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT32	version;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32Of<FTStringRange>
		tagRanges;	/* Range for each tag's string */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, tagRanges);
};

} /* namespace AAT */


#endif /* HB_AAT_LTAG_TABLE_HH */
