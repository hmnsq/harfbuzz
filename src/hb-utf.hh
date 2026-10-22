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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"


ಸಂರಚನಾ hb_utf8_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t codepoint_t;
  ಸ್ಥಿರ constexpr unsigned max_len = 4;

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  next (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    ಯದಿ (c > 0x7Fu)
    {
      ಯದಿ (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ t1;
	ಯದಿ (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	ಅನ್ಯಥಾ
	  ಗಚ್ಛ error;
      }
      ಅನ್ಯಥಾ ಯದಿ (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ t1, t2;
	ಯದಿ (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  ಯದಿ (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    ಗಚ್ಛ error;
	  text += 2;
	}
	ಅನ್ಯಥಾ
	  ಗಚ್ಛ error;
      }
      ಅನ್ಯಥಾ ಯದಿ (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ t1, t2, t3;
	ಯದಿ (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  ಯದಿ (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    ಗಚ್ಛ error;
	  text += 3;
	}
	ಅನ್ಯಥಾ
	  ಗಚ್ಛ error;
      }
      ಅನ್ಯಥಾ
	ಗಚ್ಛ error;
    }

    *unicode = c;
    ಪ್ರತ್ಯಾಗಮ text;

  error:
    *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  prev (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ಧ್ರುವ codepoint_t *end = text--;
    ಯಾವತ್ (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    ಯದಿ (likely (next (text, end, unicode, replacement) == end))
      ಪ್ರತ್ಯಾಗಮ text;

    *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ end - 1;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  strlen (ಧ್ರುವ codepoint_t *text)
  { ಪ್ರತ್ಯಾಗಮ ::strlen ((ಧ್ರುವ ವರ್ಣ *) text); }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  encode_len (hb_codepoint_t unicode)
  {
    ಯದಿ (unicode <   0x0080u) ಪ್ರತ್ಯಾಗಮ 1;
    ಯದಿ (unicode <   0x0800u) ಪ್ರತ್ಯಾಗಮ 2;
    ಯದಿ (unicode <  0x10000u) ಪ್ರತ್ಯಾಗಮ 3;
    ಯದಿ (unicode < 0x110000u) ಪ್ರತ್ಯಾಗಮ 4;
    ಪ್ರತ್ಯಾಗಮ 3;
  }

  ಸ್ಥಿರ codepoint_t *
  encode (codepoint_t *text,
	  ಧ್ರುವ codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    ಯದಿ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ಯದಿ (unicode < 0x0080u)
     *text++ = unicode;
    ಅನ್ಯಥಾ ಯದಿ (unicode < 0x0800u)
    {
      ಯದಿ (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ಅನ್ಯಥಾ ಯದಿ (unicode < 0x10000u)
    {
      ಯದಿ (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ಪ್ರತ್ಯಾಗಮ text;
  }
};


template <typename TCodepoint>
ಸಂರಚನಾ hb_utf16_xe_t
{
  static_assert (ಪರಿಮಾಣ (TCodepoint) == 2, "");
  ಪ್ರಕಾರನಿರ್ದೇಶ TCodepoint codepoint_t;
  ಸ್ಥಿರ constexpr unsigned max_len = 2;

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  next (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    ಯದಿ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ಪ್ರತ್ಯಾಗಮ text;
    }

    ಯದಿ (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      ಯದಿ (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 ಪ್ರತ್ಯಾಗಮ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  prev (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    ಯದಿ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ಪ್ರತ್ಯಾಗಮ text;
    }

    ಯದಿ (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      ಯದಿ (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	ಪ್ರತ್ಯಾಗಮ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }


  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  strlen (ಧ್ರುವ codepoint_t *text)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ l = 0;
    ಯಾವತ್ (*text++) l++;
    ಪ್ರತ್ಯಾಗಮ l;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  encode_len (hb_codepoint_t unicode)
  {
    ಪ್ರತ್ಯಾಗಮ unicode < 0x10000 ? 1 : 2;
  }

  ಸ್ಥಿರ codepoint_t *
  encode (codepoint_t *text,
	  ಧ್ರುವ codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    ಯದಿ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ಯದಿ (unicode < 0x10000u)
     *text++ = unicode;
    ಅನ್ಯಥಾ ಯದಿ (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    ಪ್ರತ್ಯಾಗಮ text;
  }
};

ಪ್ರಕಾರನಿರ್ದೇಶ hb_utf16_xe_t<uint16_t> hb_utf16_t;
ಪ್ರಕಾರನಿರ್ದೇಶ hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, ತಾರ್ಕಿಕ validate=ಸತ್ಯ>
ಸಂರಚನಾ hb_utf32_xe_t
{
  static_assert (ಪರಿಮಾಣ (TCodepoint) == 4, "");
  ಪ್ರಕಾರನಿರ್ದೇಶ TCodepoint codepoint_t;
  ಸ್ಥಿರ constexpr unsigned max_len = 1;

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ TCodepoint *
  next (ಧ್ರುವ TCodepoint *text,
	ಧ್ರುವ TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    ಯದಿ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ TCodepoint *
  prev (ಧ್ರುವ TCodepoint *text,
	ಧ್ರುವ TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    ಯದಿ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  strlen (ಧ್ರುವ TCodepoint *text)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ l = 0;
    ಯಾವತ್ (*text++) l++;
    ಪ್ರತ್ಯಾಗಮ l;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ 1;
  }

  ಸ್ಥಿರ codepoint_t *
  encode (codepoint_t *text,
	  ಧ್ರುವ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ಯದಿ (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    ಪ್ರತ್ಯಾಗಮ text;
  }
};

ಪ್ರಕಾರನಿರ್ದೇಶ hb_utf32_xe_t<uint32_t> hb_utf32_t;
ಪ್ರಕಾರನಿರ್ದೇಶ hb_utf32_xe_t<uint32_t, ಅಸತ್ಯ> hb_utf32_novalidate_t;


ಸಂರಚನಾ hb_latin1_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t codepoint_t;
  ಸ್ಥಿರ constexpr unsigned max_len = 1;

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  next (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  prev (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  strlen (ಧ್ರುವ codepoint_t *text)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ l = 0;
    ಯಾವತ್ (*text++) l++;
    ಪ್ರತ್ಯಾಗಮ l;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ 1;
  }

  ಸ್ಥಿರ codepoint_t *
  encode (codepoint_t *text,
	  ಧ್ರುವ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ಯದಿ (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    ಪ್ರತ್ಯಾಗಮ text;
  }
};


ಸಂರಚನಾ hb_ascii_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t codepoint_t;
  ಸ್ಥಿರ constexpr unsigned max_len = 1;

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  next (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    ಯದಿ (*unicode >= 0x0080u)
      *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ codepoint_t *
  prev (ಧ್ರುವ codepoint_t *text,
	ಧ್ರುವ codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    ಯದಿ (*unicode >= 0x0080u)
      *unicode = replacement;
    ಪ್ರತ್ಯಾಗಮ text;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  strlen (ಧ್ರುವ codepoint_t *text)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ l = 0;
    ಯಾವತ್ (*text++) l++;
    ಪ್ರತ್ಯಾಗಮ l;
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ 1;
  }

  ಸ್ಥಿರ codepoint_t *
  encode (codepoint_t *text,
	  ಧ್ರುವ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ಯದಿ (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    ಪ್ರತ್ಯಾಗಮ text;
  }
};

template <typename utf_t>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (ಧ್ರುವ typename utf_t::codepoint_t *start,
			  ಧ್ರುವ typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  ಯಾವತ್ (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ಯಾವತ್ (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ಪ್ರತ್ಯಾಗಮ start;
}


#endif /* HB_UTF_HH */
