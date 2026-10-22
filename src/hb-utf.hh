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

#include "hb.hh"

#include "hb-open-type.hh"


struct hb_utf8_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 4;

  static inline const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    もし (c > 0x7Fu)
    {
      もし (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned 整数 t1;
	もし (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	否则
	  goto error;
      }
      否则 もし (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned 整数 t1, t2;
	もし (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  もし (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    goto error;
	  text += 2;
	}
	否则
	  goto error;
      }
      否则 もし (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned 整数 t1, t2, t3;
	もし (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  もし (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    goto error;
	  text += 3;
	}
	否则
	  goto error;
      }
      否则
	goto error;
    }

    *unicode = c;
    戻る text;

  error:
    *unicode = replacement;
    戻る text;
  }

  static inline const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    const codepoint_t *end = text--;
    ながら (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    もし (likely (next (text, end, unicode, replacement) == end))
      戻る text;

    *unicode = replacement;
    戻る end - 1;
  }

  static unsigned 整数
  strlen (const codepoint_t *text)
  { 戻る ::strlen ((const char *) text); }

  static unsigned 整数
  encode_len (hb_codepoint_t unicode)
  {
    もし (unicode <   0x0080u) 戻る 1;
    もし (unicode <   0x0800u) 戻る 2;
    もし (unicode <  0x10000u) 戻る 3;
    もし (unicode < 0x110000u) 戻る 4;
    戻る 3;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    もし (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    もし (unicode < 0x0080u)
     *text++ = unicode;
    否则 もし (unicode < 0x0800u)
    {
      もし (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    否则 もし (unicode < 0x10000u)
    {
      もし (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    否则
    {
      もし (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    戻る text;
  }
};


template <typename TCodepoint>
struct hb_utf16_xe_t
{
  static_assert (sizeof (TCodepoint) == 2, "");
  typedef TCodepoint codepoint_t;
  static constexpr unsigned max_len = 2;

  static inline const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    もし (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      戻る text;
    }

    もし (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      もし (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 戻る text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    戻る text;
  }

  static inline const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    もし (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      戻る text;
    }

    もし (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      もし (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	戻る text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    戻る text;
  }


  static unsigned 整数
  strlen (const codepoint_t *text)
  {
    unsigned 整数 l = 0;
    ながら (*text++) l++;
    戻る l;
  }

  static unsigned 整数
  encode_len (hb_codepoint_t unicode)
  {
    戻る unicode < 0x10000 ? 1 : 2;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    もし (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    もし (unicode < 0x10000u)
     *text++ = unicode;
    否则 もし (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    戻る text;
  }
};

typedef hb_utf16_xe_t<uint16_t> hb_utf16_t;
typedef hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, bool validate=true>
struct hb_utf32_xe_t
{
  static_assert (sizeof (TCodepoint) == 4, "");
  typedef TCodepoint codepoint_t;
  static constexpr unsigned max_len = 1;

  static inline const TCodepoint *
  next (const TCodepoint *text,
	const TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    もし (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    戻る text;
  }

  static inline const TCodepoint *
  prev (const TCodepoint *text,
	const TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    もし (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    戻る text;
  }

  static unsigned 整数
  strlen (const TCodepoint *text)
  {
    unsigned 整数 l = 0;
    ながら (*text++) l++;
    戻る l;
  }

  static unsigned 整数
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    戻る 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    もし (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    戻る text;
  }
};

typedef hb_utf32_xe_t<uint32_t> hb_utf32_t;
typedef hb_utf32_xe_t<uint32_t, false> hb_utf32_novalidate_t;


struct hb_latin1_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 1;

  static inline const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    戻る text;
  }

  static inline const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    戻る text;
  }

  static unsigned 整数
  strlen (const codepoint_t *text)
  {
    unsigned 整数 l = 0;
    ながら (*text++) l++;
    戻る l;
  }

  static unsigned 整数
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    戻る 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    もし (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    戻る text;
  }
};


struct hb_ascii_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 1;

  static inline const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    もし (*unicode >= 0x0080u)
      *unicode = replacement;
    戻る text;
  }

  static inline const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    もし (*unicode >= 0x0080u)
      *unicode = replacement;
    戻る text;
  }

  static unsigned 整数
  strlen (const codepoint_t *text)
  {
    unsigned 整数 l = 0;
    ながら (*text++) l++;
    戻る l;
  }

  static unsigned 整数
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    戻る 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    もし (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    戻る text;
  }
};

template <typename utf_t>
static inline const typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (const typename utf_t::codepoint_t *start,
			  const typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  ながら (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ながら (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  戻る start;
}


#endif /* HB_UTF_HH */
