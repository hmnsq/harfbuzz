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

#അന്തര്ഭാവ "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#define HB_AAT_TAG_ltag HB_TAG('l','t','a','g')


നാമാകാശ AAT {

using നാമാകാശ OT;


ഘടന FTStringRange
{
  friend ഘടന ltag;

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  (base+tag).sanitize (c, length));
  }

  സംരക്ഷിത:
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		tag;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	length;		/* String length (in bytes) */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന ltag
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_ltag;

  hb_language_t get_language (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    സ്ഥിരം FTStringRange &range = tagRanges[i];
    തിരികെ hb_language_from_string ((സ്ഥിരം അക്ഷരം *) (ഇത്+range.tag).arrayZ,
				    range.length);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version >= 1 &&
			  tagRanges.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
  HBUINT32	version;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32Of<FTStringRange>
		tagRanges;	/* Range for each tag's string */
  പൊതു:
  DEFINE_SIZE_ARRAY (12, tagRanges);
};

} /* namespace AAT */


#endif /* HB_AAT_LTAG_TABLE_HH */
