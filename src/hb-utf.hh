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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-open-type.hh"


সংরচনা hb_utf8_t
{
  প্রকারনির্দেশ uint8_t codepoint_t;
  স্থির constexpr unsigned max_len = 4;

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  next (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    যদি (c > 0x7Fu)
    {
      যদি (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned পূর্ণাঙ্ক t1;
	যদি (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	অন্যথা
	  গচ্ছ error;
      }
      অন্যথা যদি (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned পূর্ণাঙ্ক t1, t2;
	যদি (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  যদি (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    গচ্ছ error;
	  text += 2;
	}
	অন্যথা
	  গচ্ছ error;
      }
      অন্যথা যদি (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned পূর্ণাঙ্ক t1, t2, t3;
	যদি (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  যদি (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    গচ্ছ error;
	  text += 3;
	}
	অন্যথা
	  গচ্ছ error;
      }
      অন্যথা
	গচ্ছ error;
    }

    *unicode = c;
    প্রত্যাগম text;

  error:
    *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  prev (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ধ্রুব codepoint_t *end = text--;
    যাবত্ (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    যদি (likely (next (text, end, unicode, replacement) == end))
      প্রত্যাগম text;

    *unicode = replacement;
    প্রত্যাগম end - 1;
  }

  স্থির unsigned পূর্ণাঙ্ক
  strlen (ধ্রুব codepoint_t *text)
  { প্রত্যাগম ::strlen ((ধ্রুব বর্ণ *) text); }

  স্থির unsigned পূর্ণাঙ্ক
  encode_len (hb_codepoint_t unicode)
  {
    যদি (unicode <   0x0080u) প্রত্যাগম 1;
    যদি (unicode <   0x0800u) প্রত্যাগম 2;
    যদি (unicode <  0x10000u) প্রত্যাগম 3;
    যদি (unicode < 0x110000u) প্রত্যাগম 4;
    প্রত্যাগম 3;
  }

  স্থির codepoint_t *
  encode (codepoint_t *text,
	  ধ্রুব codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    যদি (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    যদি (unicode < 0x0080u)
     *text++ = unicode;
    অন্যথা যদি (unicode < 0x0800u)
    {
      যদি (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    অন্যথা যদি (unicode < 0x10000u)
    {
      যদি (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    অন্যথা
    {
      যদি (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    প্রত্যাগম text;
  }
};


template <typename TCodepoint>
সংরচনা hb_utf16_xe_t
{
  static_assert (পরিমাণ (TCodepoint) == 2, "");
  প্রকারনির্দেশ TCodepoint codepoint_t;
  স্থির constexpr unsigned max_len = 2;

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  next (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    যদি (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      প্রত্যাগম text;
    }

    যদি (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      যদি (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 প্রত্যাগম text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  prev (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    যদি (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      প্রত্যাগম text;
    }

    যদি (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      যদি (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	প্রত্যাগম text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    প্রত্যাগম text;
  }


  স্থির unsigned পূর্ণাঙ্ক
  strlen (ধ্রুব codepoint_t *text)
  {
    unsigned পূর্ণাঙ্ক l = 0;
    যাবত্ (*text++) l++;
    প্রত্যাগম l;
  }

  স্থির unsigned পূর্ণাঙ্ক
  encode_len (hb_codepoint_t unicode)
  {
    প্রত্যাগম unicode < 0x10000 ? 1 : 2;
  }

  স্থির codepoint_t *
  encode (codepoint_t *text,
	  ধ্রুব codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    যদি (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    যদি (unicode < 0x10000u)
     *text++ = unicode;
    অন্যথা যদি (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    প্রত্যাগম text;
  }
};

প্রকারনির্দেশ hb_utf16_xe_t<uint16_t> hb_utf16_t;
প্রকারনির্দেশ hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, তার্কিক validate=সত্য>
সংরচনা hb_utf32_xe_t
{
  static_assert (পরিমাণ (TCodepoint) == 4, "");
  প্রকারনির্দেশ TCodepoint codepoint_t;
  স্থির constexpr unsigned max_len = 1;

  স্থির অন্তঃস্থ ধ্রুব TCodepoint *
  next (ধ্রুব TCodepoint *text,
	ধ্রুব TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    যদি (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির অন্তঃস্থ ধ্রুব TCodepoint *
  prev (ধ্রুব TCodepoint *text,
	ধ্রুব TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    যদি (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির unsigned পূর্ণাঙ্ক
  strlen (ধ্রুব TCodepoint *text)
  {
    unsigned পূর্ণাঙ্ক l = 0;
    যাবত্ (*text++) l++;
    প্রত্যাগম l;
  }

  স্থির unsigned পূর্ণাঙ্ক
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    প্রত্যাগম 1;
  }

  স্থির codepoint_t *
  encode (codepoint_t *text,
	  ধ্রুব codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    যদি (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    প্রত্যাগম text;
  }
};

প্রকারনির্দেশ hb_utf32_xe_t<uint32_t> hb_utf32_t;
প্রকারনির্দেশ hb_utf32_xe_t<uint32_t, অসত্য> hb_utf32_novalidate_t;


সংরচনা hb_latin1_t
{
  প্রকারনির্দেশ uint8_t codepoint_t;
  স্থির constexpr unsigned max_len = 1;

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  next (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    প্রত্যাগম text;
  }

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  prev (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    প্রত্যাগম text;
  }

  স্থির unsigned পূর্ণাঙ্ক
  strlen (ধ্রুব codepoint_t *text)
  {
    unsigned পূর্ণাঙ্ক l = 0;
    যাবত্ (*text++) l++;
    প্রত্যাগম l;
  }

  স্থির unsigned পূর্ণাঙ্ক
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    প্রত্যাগম 1;
  }

  স্থির codepoint_t *
  encode (codepoint_t *text,
	  ধ্রুব codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    যদি (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    প্রত্যাগম text;
  }
};


সংরচনা hb_ascii_t
{
  প্রকারনির্দেশ uint8_t codepoint_t;
  স্থির constexpr unsigned max_len = 1;

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  next (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    যদি (*unicode >= 0x0080u)
      *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির অন্তঃস্থ ধ্রুব codepoint_t *
  prev (ধ্রুব codepoint_t *text,
	ধ্রুব codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    যদি (*unicode >= 0x0080u)
      *unicode = replacement;
    প্রত্যাগম text;
  }

  স্থির unsigned পূর্ণাঙ্ক
  strlen (ধ্রুব codepoint_t *text)
  {
    unsigned পূর্ণাঙ্ক l = 0;
    যাবত্ (*text++) l++;
    প্রত্যাগম l;
  }

  স্থির unsigned পূর্ণাঙ্ক
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    প্রত্যাগম 1;
  }

  স্থির codepoint_t *
  encode (codepoint_t *text,
	  ধ্রুব codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    যদি (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    প্রত্যাগম text;
  }
};

template <typename utf_t>
স্থির অন্তঃস্থ ধ্রুব typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (ধ্রুব typename utf_t::codepoint_t *start,
			  ধ্রুব typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  যাবত্ (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  যাবত্ (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  প্রত্যাগম start;
}


#endif /* HB_UTF_HH */
