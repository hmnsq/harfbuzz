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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-open-type.hh"


ഘടന hb_utf8_t
{
  തരനിർവചനം uint8_t codepoint_t;
  സ്ഥിതം constexpr unsigned max_len = 4;

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  next (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    എങ്കിൽ (c > 0x7Fu)
    {
      എങ്കിൽ (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned പൂർണ്ണസംഖ്യ t1;
	എങ്കിൽ (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	അല്ലെങ്കിൽ
	  പോകുക error;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned പൂർണ്ണസംഖ്യ t1, t2;
	എങ്കിൽ (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  എങ്കിൽ (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    പോകുക error;
	  text += 2;
	}
	അല്ലെങ്കിൽ
	  പോകുക error;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned പൂർണ്ണസംഖ്യ t1, t2, t3;
	എങ്കിൽ (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  എങ്കിൽ (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    പോകുക error;
	  text += 3;
	}
	അല്ലെങ്കിൽ
	  പോകുക error;
      }
      അല്ലെങ്കിൽ
	പോകുക error;
    }

    *unicode = c;
    തിരികെ text;

  error:
    *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  prev (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    സ്ഥിരം codepoint_t *end = text--;
    ആകുമ്പോൾ (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    എങ്കിൽ (likely (next (text, end, unicode, replacement) == end))
      തിരികെ text;

    *unicode = replacement;
    തിരികെ end - 1;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  strlen (സ്ഥിരം codepoint_t *text)
  { തിരികെ ::strlen ((സ്ഥിരം അക്ഷരം *) text); }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  encode_len (hb_codepoint_t unicode)
  {
    എങ്കിൽ (unicode <   0x0080u) തിരികെ 1;
    എങ്കിൽ (unicode <   0x0800u) തിരികെ 2;
    എങ്കിൽ (unicode <  0x10000u) തിരികെ 3;
    എങ്കിൽ (unicode < 0x110000u) തിരികെ 4;
    തിരികെ 3;
  }

  സ്ഥിതം codepoint_t *
  encode (codepoint_t *text,
	  സ്ഥിരം codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    എങ്കിൽ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    എങ്കിൽ (unicode < 0x0080u)
     *text++ = unicode;
    അല്ലെങ്കിൽ എങ്കിൽ (unicode < 0x0800u)
    {
      എങ്കിൽ (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unicode < 0x10000u)
    {
      എങ്കിൽ (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    തിരികെ text;
  }
};


template <typename TCodepoint>
ഘടന hb_utf16_xe_t
{
  static_assert (വലുപ്പം (TCodepoint) == 2, "");
  തരനിർവചനം TCodepoint codepoint_t;
  സ്ഥിതം constexpr unsigned max_len = 2;

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  next (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    എങ്കിൽ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      തിരികെ text;
    }

    എങ്കിൽ (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      എങ്കിൽ (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 തിരികെ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  prev (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    എങ്കിൽ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      തിരികെ text;
    }

    എങ്കിൽ (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      എങ്കിൽ (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	തിരികെ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    തിരികെ text;
  }


  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  strlen (സ്ഥിരം codepoint_t *text)
  {
    unsigned പൂർണ്ണസംഖ്യ l = 0;
    ആകുമ്പോൾ (*text++) l++;
    തിരികെ l;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  encode_len (hb_codepoint_t unicode)
  {
    തിരികെ unicode < 0x10000 ? 1 : 2;
  }

  സ്ഥിതം codepoint_t *
  encode (codepoint_t *text,
	  സ്ഥിരം codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    എങ്കിൽ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    എങ്കിൽ (unicode < 0x10000u)
     *text++ = unicode;
    അല്ലെങ്കിൽ എങ്കിൽ (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    തിരികെ text;
  }
};

തരനിർവചനം hb_utf16_xe_t<uint16_t> hb_utf16_t;
തരനിർവചനം hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, തർക്കം validate=സത്യം>
ഘടന hb_utf32_xe_t
{
  static_assert (വലുപ്പം (TCodepoint) == 4, "");
  തരനിർവചനം TCodepoint codepoint_t;
  സ്ഥിതം constexpr unsigned max_len = 1;

  സ്ഥിതം വരിയിൽ സ്ഥിരം TCodepoint *
  next (സ്ഥിരം TCodepoint *text,
	സ്ഥിരം TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    എങ്കിൽ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം TCodepoint *
  prev (സ്ഥിരം TCodepoint *text,
	സ്ഥിരം TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    എങ്കിൽ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  strlen (സ്ഥിരം TCodepoint *text)
  {
    unsigned പൂർണ്ണസംഖ്യ l = 0;
    ആകുമ്പോൾ (*text++) l++;
    തിരികെ l;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    തിരികെ 1;
  }

  സ്ഥിതം codepoint_t *
  encode (codepoint_t *text,
	  സ്ഥിരം codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    എങ്കിൽ (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    തിരികെ text;
  }
};

തരനിർവചനം hb_utf32_xe_t<uint32_t> hb_utf32_t;
തരനിർവചനം hb_utf32_xe_t<uint32_t, അസത്യം> hb_utf32_novalidate_t;


ഘടന hb_latin1_t
{
  തരനിർവചനം uint8_t codepoint_t;
  സ്ഥിതം constexpr unsigned max_len = 1;

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  next (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    തിരികെ text;
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  prev (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    തിരികെ text;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  strlen (സ്ഥിരം codepoint_t *text)
  {
    unsigned പൂർണ്ണസംഖ്യ l = 0;
    ആകുമ്പോൾ (*text++) l++;
    തിരികെ l;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    തിരികെ 1;
  }

  സ്ഥിതം codepoint_t *
  encode (codepoint_t *text,
	  സ്ഥിരം codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    എങ്കിൽ (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    തിരികെ text;
  }
};


ഘടന hb_ascii_t
{
  തരനിർവചനം uint8_t codepoint_t;
  സ്ഥിതം constexpr unsigned max_len = 1;

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  next (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    എങ്കിൽ (*unicode >= 0x0080u)
      *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം codepoint_t *
  prev (സ്ഥിരം codepoint_t *text,
	സ്ഥിരം codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    എങ്കിൽ (*unicode >= 0x0080u)
      *unicode = replacement;
    തിരികെ text;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  strlen (സ്ഥിരം codepoint_t *text)
  {
    unsigned പൂർണ്ണസംഖ്യ l = 0;
    ആകുമ്പോൾ (*text++) l++;
    തിരികെ l;
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    തിരികെ 1;
  }

  സ്ഥിതം codepoint_t *
  encode (codepoint_t *text,
	  സ്ഥിരം codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    എങ്കിൽ (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    തിരികെ text;
  }
};

template <typename utf_t>
സ്ഥിതം വരിയിൽ സ്ഥിരം typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (സ്ഥിരം typename utf_t::codepoint_t *start,
			  സ്ഥിരം typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  ആകുമ്പോൾ (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ആകുമ്പോൾ (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  തിരികെ start;
}


#endif /* HB_UTF_HH */
