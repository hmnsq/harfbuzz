/*
 * Copyright © 2011,2012,2014  Google, Inc.
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

#ifndef HB_UTF_HH
#define HB_UTF_HH

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-open-type.hh"


संरचना hb_utf8_t
{
  प्रकारनिर्देश uint8_t codepoint_t;
  स्थिर constexpr unsigned max_len = 4;

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  next (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    सचे (c > 0x7Fu)
    {
      सचे (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned पूर्णाङ्क t1;
	सचे (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	अञ्ञथा
	  गच्छ error;
      }
      अञ्ञथा सचे (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned पूर्णाङ्क t1, t2;
	सचे (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  सचे (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    गच्छ error;
	  text += 2;
	}
	अञ्ञथा
	  गच्छ error;
      }
      अञ्ञथा सचे (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned पूर्णाङ्क t1, t2, t3;
	सचे (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  सचे (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    गच्छ error;
	  text += 3;
	}
	अञ्ञथा
	  गच्छ error;
      }
      अञ्ञथा
	गच्छ error;
    }

    *unicode = c;
    पटिनिवत्त text;

  error:
    *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  prev (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ध्रुव codepoint_t *end = text--;
    याव (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    सचे (likely (next (text, end, unicode, replacement) == end))
      पटिनिवत्त text;

    *unicode = replacement;
    पटिनिवत्त end - 1;
  }

  स्थिर unsigned पूर्णाङ्क
  strlen (ध्रुव codepoint_t *text)
  { पटिनिवत्त ::strlen ((ध्रुव वर्ण *) text); }

  स्थिर unsigned पूर्णाङ्क
  encode_len (hb_codepoint_t unicode)
  {
    सचे (unicode <   0x0080u) पटिनिवत्त 1;
    सचे (unicode <   0x0800u) पटिनिवत्त 2;
    सचे (unicode <  0x10000u) पटिनिवत्त 3;
    सचे (unicode < 0x110000u) पटिनिवत्त 4;
    पटिनिवत्त 3;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  ध्रुव codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    सचे (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    सचे (unicode < 0x0080u)
     *text++ = unicode;
    अञ्ञथा सचे (unicode < 0x0800u)
    {
      सचे (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अञ्ञथा सचे (unicode < 0x10000u)
    {
      सचे (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अञ्ञथा
    {
      सचे (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    पटिनिवत्त text;
  }
};


template <typename TCodepoint>
संरचना hb_utf16_xe_t
{
  static_assert (परिमाण (TCodepoint) == 2, "");
  प्रकारनिर्देश TCodepoint codepoint_t;
  स्थिर constexpr unsigned max_len = 2;

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  next (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    सचे (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      पटिनिवत्त text;
    }

    सचे (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      सचे (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 पटिनिवत्त text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  prev (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    सचे (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      पटिनिवत्त text;
    }

    सचे (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      सचे (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	पटिनिवत्त text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    पटिनिवत्त text;
  }


  स्थिर unsigned पूर्णाङ्क
  strlen (ध्रुव codepoint_t *text)
  {
    unsigned पूर्णाङ्क l = 0;
    याव (*text++) l++;
    पटिनिवत्त l;
  }

  स्थिर unsigned पूर्णाङ्क
  encode_len (hb_codepoint_t unicode)
  {
    पटिनिवत्त unicode < 0x10000 ? 1 : 2;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  ध्रुव codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    सचे (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    सचे (unicode < 0x10000u)
     *text++ = unicode;
    अञ्ञथा सचे (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    पटिनिवत्त text;
  }
};

प्रकारनिर्देश hb_utf16_xe_t<uint16_t> hb_utf16_t;
प्रकारनिर्देश hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, तार्किक validate=सच्च>
संरचना hb_utf32_xe_t
{
  static_assert (परिमाण (TCodepoint) == 4, "");
  प्रकारनिर्देश TCodepoint codepoint_t;
  स्थिर constexpr unsigned max_len = 1;

  स्थिर अन्तःस्थ ध्रुव TCodepoint *
  next (ध्रुव TCodepoint *text,
	ध्रुव TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    सचे (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर अन्तःस्थ ध्रुव TCodepoint *
  prev (ध्रुव TCodepoint *text,
	ध्रुव TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    सचे (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर unsigned पूर्णाङ्क
  strlen (ध्रुव TCodepoint *text)
  {
    unsigned पूर्णाङ्क l = 0;
    याव (*text++) l++;
    पटिनिवत्त l;
  }

  स्थिर unsigned पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    पटिनिवत्त 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  ध्रुव codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    सचे (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    पटिनिवत्त text;
  }
};

प्रकारनिर्देश hb_utf32_xe_t<uint32_t> hb_utf32_t;
प्रकारनिर्देश hb_utf32_xe_t<uint32_t, मिच्छा> hb_utf32_novalidate_t;


संरचना hb_latin1_t
{
  प्रकारनिर्देश uint8_t codepoint_t;
  स्थिर constexpr unsigned max_len = 1;

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  next (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    पटिनिवत्त text;
  }

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  prev (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    पटिनिवत्त text;
  }

  स्थिर unsigned पूर्णाङ्क
  strlen (ध्रुव codepoint_t *text)
  {
    unsigned पूर्णाङ्क l = 0;
    याव (*text++) l++;
    पटिनिवत्त l;
  }

  स्थिर unsigned पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    पटिनिवत्त 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  ध्रुव codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    सचे (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    पटिनिवत्त text;
  }
};


संरचना hb_ascii_t
{
  प्रकारनिर्देश uint8_t codepoint_t;
  स्थिर constexpr unsigned max_len = 1;

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  next (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    सचे (*unicode >= 0x0080u)
      *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर अन्तःस्थ ध्रुव codepoint_t *
  prev (ध्रुव codepoint_t *text,
	ध्रुव codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    सचे (*unicode >= 0x0080u)
      *unicode = replacement;
    पटिनिवत्त text;
  }

  स्थिर unsigned पूर्णाङ्क
  strlen (ध्रुव codepoint_t *text)
  {
    unsigned पूर्णाङ्क l = 0;
    याव (*text++) l++;
    पटिनिवत्त l;
  }

  स्थिर unsigned पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    पटिनिवत्त 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  ध्रुव codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    सचे (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    पटिनिवत्त text;
  }
};

template <typename utf_t>
स्थिर अन्तःस्थ ध्रुव typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (ध्रुव typename utf_t::codepoint_t *start,
			  ध्रुव typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  याव (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  याव (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  पटिनिवत्त start;
}


#endif /* HB_UTF_HH */
