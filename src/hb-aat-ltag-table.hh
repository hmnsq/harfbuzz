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

#अगर_अघोषित HB_AAT_LTAG_TABLE_HH
#घोषणा HB_AAT_LTAG_TABLE_HH

#समावेश "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#घोषणा HB_AAT_TAG_ltag HB_TAG('l','t','a','g')


namespace AAT {

using namespace OT;


काष्ठा FTStringRange
{
  मित्र काष्ठा ltag;

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  (base+tag).sanitize (c, लंबाई));
  }

  रक्षित:
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		tag;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	लंबाई;		/* String length (in bytes) */
  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा ltag
{
  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_ltag;

  hb_language_t get_language (अचिन्हित पूर्णांक i) स्थिर
  {
    स्थिर FTStringRange &range = tagRanges[i];
    वापस hb_language_from_string ((स्थिर अक्षर *) (यह+range.tag).arrayZ,
				    range.लंबाई);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version >= 1 &&
			  tagRanges.sanitize (c, यह)));
  }

  रक्षित:
  HBUINT32	version;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32Of<FTStringRange>
		tagRanges;	/* Range for each tag's string */
  खुला:
  DEFINE_आकार_ARRAY (12, tagRanges);
};

} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LTAG_TABLE_HH */
