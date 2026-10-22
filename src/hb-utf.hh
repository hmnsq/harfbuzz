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
  स्थिर constexpr अचिन्हित max_len = 4;

  स्थिर अन्तःस्थ अचर codepoint_t *
  next (अचर codepoint_t *text,
	अचर codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    यदि (c > 0x7Fu)
    {
      यदि (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	अचिन्हित पूर्णाङ्क t1;
	यदि (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	अन्यथा
	  गच्छ error;
      }
      अन्यथा यदि (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	अचिन्हित पूर्णाङ्क t1, t2;
	यदि (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  यदि (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    गच्छ error;
	  text += 2;
	}
	अन्यथा
	  गच्छ error;
      }
      अन्यथा यदि (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	अचिन्हित पूर्णाङ्क t1, t2, t3;
	यदि (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  यदि (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    गच्छ error;
	  text += 3;
	}
	अन्यथा
	  गच्छ error;
      }
      अन्यथा
	गच्छ error;
    }

    *unicode = c;
    फर्काऊ text;

  error:
    *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अन्तःस्थ अचर codepoint_t *
  prev (अचर codepoint_t *text,
	अचर codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    अचर codepoint_t *end = text--;
    जबसम्म (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    यदि (likely (next (text, end, unicode, replacement) == end))
      फर्काऊ text;

    *unicode = replacement;
    फर्काऊ end - 1;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  म_माप (अचर codepoint_t *text)
  { फर्काऊ ::म_माप ((अचर वर्ण *) text); }

  स्थिर अचिन्हित पूर्णाङ्क
  encode_len (hb_codepoint_t unicode)
  {
    यदि (unicode <   0x0080u) फर्काऊ 1;
    यदि (unicode <   0x0800u) फर्काऊ 2;
    यदि (unicode <  0x10000u) फर्काऊ 3;
    यदि (unicode < 0x110000u) फर्काऊ 4;
    फर्काऊ 3;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  अचर codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    यदि (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    यदि (unicode < 0x0080u)
     *text++ = unicode;
    अन्यथा यदि (unicode < 0x0800u)
    {
      यदि (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा यदि (unicode < 0x10000u)
    {
      यदि (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा
    {
      यदि (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    फर्काऊ text;
  }
};


ढाँचा <typename TCodepoint>
संरचना hb_utf16_xe_t
{
  static_assert (परिमाण (TCodepoint) == 2, "");
  प्रकारनिर्देश TCodepoint codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 2;

  स्थिर अन्तःस्थ अचर codepoint_t *
  next (अचर codepoint_t *text,
	अचर codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    यदि (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      फर्काऊ text;
    }

    यदि (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      यदि (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 फर्काऊ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अन्तःस्थ अचर codepoint_t *
  prev (अचर codepoint_t *text,
	अचर codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    यदि (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      फर्काऊ text;
    }

    यदि (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      यदि (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	फर्काऊ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    फर्काऊ text;
  }


  स्थिर अचिन्हित पूर्णाङ्क
  म_माप (अचर codepoint_t *text)
  {
    अचिन्हित पूर्णाङ्क l = 0;
    जबसम्म (*text++) l++;
    फर्काऊ l;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  encode_len (hb_codepoint_t unicode)
  {
    फर्काऊ unicode < 0x10000 ? 1 : 2;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  अचर codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    यदि (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    यदि (unicode < 0x10000u)
     *text++ = unicode;
    अन्यथा यदि (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    फर्काऊ text;
  }
};

प्रकारनिर्देश hb_utf16_xe_t<uint16_t> hb_utf16_t;
प्रकारनिर्देश hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


ढाँचा <typename TCodepoint, तार्किक validate=सत्य>
संरचना hb_utf32_xe_t
{
  static_assert (परिमाण (TCodepoint) == 4, "");
  प्रकारनिर्देश TCodepoint codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर अन्तःस्थ अचर TCodepoint *
  next (अचर TCodepoint *text,
	अचर TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    यदि (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अन्तःस्थ अचर TCodepoint *
  prev (अचर TCodepoint *text,
	अचर TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    यदि (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  म_माप (अचर TCodepoint *text)
  {
    अचिन्हित पूर्णाङ्क l = 0;
    जबसम्म (*text++) l++;
    फर्काऊ l;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    फर्काऊ 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  अचर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    यदि (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    फर्काऊ text;
  }
};

प्रकारनिर्देश hb_utf32_xe_t<uint32_t> hb_utf32_t;
प्रकारनिर्देश hb_utf32_xe_t<uint32_t, असत्य> hb_utf32_novalidate_t;


संरचना hb_latin1_t
{
  प्रकारनिर्देश uint8_t codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर अन्तःस्थ अचर codepoint_t *
  next (अचर codepoint_t *text,
	अचर codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    फर्काऊ text;
  }

  स्थिर अन्तःस्थ अचर codepoint_t *
  prev (अचर codepoint_t *text,
	अचर codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    फर्काऊ text;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  म_माप (अचर codepoint_t *text)
  {
    अचिन्हित पूर्णाङ्क l = 0;
    जबसम्म (*text++) l++;
    फर्काऊ l;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    फर्काऊ 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  अचर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    यदि (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    फर्काऊ text;
  }
};


संरचना hb_ascii_t
{
  प्रकारनिर्देश uint8_t codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर अन्तःस्थ अचर codepoint_t *
  next (अचर codepoint_t *text,
	अचर codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    यदि (*unicode >= 0x0080u)
      *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अन्तःस्थ अचर codepoint_t *
  prev (अचर codepoint_t *text,
	अचर codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    यदि (*unicode >= 0x0080u)
      *unicode = replacement;
    फर्काऊ text;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  म_माप (अचर codepoint_t *text)
  {
    अचिन्हित पूर्णाङ्क l = 0;
    जबसम्म (*text++) l++;
    फर्काऊ l;
  }

  स्थिर अचिन्हित पूर्णाङ्क
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    फर्काऊ 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  अचर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    यदि (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    फर्काऊ text;
  }
};

ढाँचा <typename utf_t>
स्थिर अन्तःस्थ अचर typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (अचर typename utf_t::codepoint_t *start,
			  अचर typename utf_t::codepoint_t *text,
			  अचिन्हित text_len,
			  चिन्हित offset)
{
  hb_codepoint_t unicode;

  जबसम्म (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  जबसम्म (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  फर्काऊ start;
}


#endif /* HB_UTF_HH */
