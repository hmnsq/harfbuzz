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


sa_nracanaa hb_utf8_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 4;

  static anta_hstha const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    ꦪꦼꦤ꧀ (c > 0x7Fu)
    {
      ꦪꦼꦤ꧀ (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned puur_naa_nka t1;
	ꦪꦼꦤ꧀ (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	ꦭꦶꦪ
	  goto error;
      }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned puur_naa_nka t1, t2;
	ꦪꦼꦤ꧀ (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  ꦪꦼꦤ꧀ (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    goto error;
	  text += 2;
	}
	ꦭꦶꦪ
	  goto error;
      }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned puur_naa_nka t1, t2, t3;
	ꦪꦼꦤ꧀ (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  ꦪꦼꦤ꧀ (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    goto error;
	  text += 3;
	}
	ꦭꦶꦪ
	  goto error;
      }
      ꦭꦶꦪ
	goto error;
    }

    *unicode = c;
    ꦮꦭꦶ text;

  error:
    *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static anta_hstha const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    const codepoint_t *end = text--;
    ꦱꦼꦢꦼꦭ (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    ꦪꦼꦤ꧀ (likely (next (text, end, unicode, replacement) == end))
      ꦮꦭꦶ text;

    *unicode = replacement;
    ꦮꦭꦶ end - 1;
  }

  static unsigned puur_naa_nka
  strlen (const codepoint_t *text)
  { ꦮꦭꦶ ::strlen ((const var_na *) text); }

  static unsigned puur_naa_nka
  encode_len (hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (unicode <   0x0080u) ꦮꦭꦶ 1;
    ꦪꦼꦤ꧀ (unicode <   0x0800u) ꦮꦭꦶ 2;
    ꦪꦼꦤ꧀ (unicode <  0x10000u) ꦮꦭꦶ 3;
    ꦪꦼꦤ꧀ (unicode < 0x110000u) ꦮꦭꦶ 4;
    ꦮꦭꦶ 3;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ꦪꦼꦤ꧀ (unicode < 0x0080u)
     *text++ = unicode;
    ꦭꦶꦪ ꦪꦼꦤ꧀ (unicode < 0x0800u)
    {
      ꦪꦼꦤ꧀ (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (unicode < 0x10000u)
    {
      ꦪꦼꦤ꧀ (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀ (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ꦮꦭꦶ text;
  }
};


template <typename TCodepoint>
sa_nracanaa hb_utf16_xe_t
{
  static_assert (parimaa_na (TCodepoint) == 2, "");
  typedef TCodepoint codepoint_t;
  static constexpr unsigned max_len = 2;

  static anta_hstha const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    ꦪꦼꦤ꧀ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ꦮꦭꦶ text;
    }

    ꦪꦼꦤ꧀ (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      ꦪꦼꦤ꧀ (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 ꦮꦭꦶ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static anta_hstha const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    ꦪꦼꦤ꧀ (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ꦮꦭꦶ text;
    }

    ꦪꦼꦤ꧀ (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      ꦪꦼꦤ꧀ (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	ꦮꦭꦶ text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ꦮꦭꦶ text;
  }


  static unsigned puur_naa_nka
  strlen (const codepoint_t *text)
  {
    unsigned puur_naa_nka l = 0;
    ꦱꦼꦢꦼꦭ (*text++) l++;
    ꦮꦭꦶ l;
  }

  static unsigned puur_naa_nka
  encode_len (hb_codepoint_t unicode)
  {
    ꦮꦭꦶ unicode < 0x10000 ? 1 : 2;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ꦪꦼꦤ꧀ (unicode < 0x10000u)
     *text++ = unicode;
    ꦭꦶꦪ ꦪꦼꦤ꧀ (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    ꦮꦭꦶ text;
  }
};

typedef hb_utf16_xe_t<uint16_t> hb_utf16_t;
typedef hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, bool validate=true>
sa_nracanaa hb_utf32_xe_t
{
  static_assert (parimaa_na (TCodepoint) == 4, "");
  typedef TCodepoint codepoint_t;
  static constexpr unsigned max_len = 1;

  static anta_hstha const TCodepoint *
  next (const TCodepoint *text,
	const TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    ꦪꦼꦤ꧀ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static anta_hstha const TCodepoint *
  prev (const TCodepoint *text,
	const TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    ꦪꦼꦤ꧀ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static unsigned puur_naa_nka
  strlen (const TCodepoint *text)
  {
    unsigned puur_naa_nka l = 0;
    ꦱꦼꦢꦼꦭ (*text++) l++;
    ꦮꦭꦶ l;
  }

  static unsigned puur_naa_nka
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ꦮꦭꦶ 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    ꦮꦭꦶ text;
  }
};

typedef hb_utf32_xe_t<uint32_t> hb_utf32_t;
typedef hb_utf32_xe_t<uint32_t, false> hb_utf32_novalidate_t;


sa_nracanaa hb_latin1_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 1;

  static anta_hstha const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    ꦮꦭꦶ text;
  }

  static anta_hstha const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    ꦮꦭꦶ text;
  }

  static unsigned puur_naa_nka
  strlen (const codepoint_t *text)
  {
    unsigned puur_naa_nka l = 0;
    ꦱꦼꦢꦼꦭ (*text++) l++;
    ꦮꦭꦶ l;
  }

  static unsigned puur_naa_nka
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ꦮꦭꦶ 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    ꦮꦭꦶ text;
  }
};


sa_nracanaa hb_ascii_t
{
  typedef uint8_t codepoint_t;
  static constexpr unsigned max_len = 1;

  static anta_hstha const codepoint_t *
  next (const codepoint_t *text,
	const codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    ꦪꦼꦤ꧀ (*unicode >= 0x0080u)
      *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static anta_hstha const codepoint_t *
  prev (const codepoint_t *text,
	const codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    ꦪꦼꦤ꧀ (*unicode >= 0x0080u)
      *unicode = replacement;
    ꦮꦭꦶ text;
  }

  static unsigned puur_naa_nka
  strlen (const codepoint_t *text)
  {
    unsigned puur_naa_nka l = 0;
    ꦱꦼꦢꦼꦭ (*text++) l++;
    ꦮꦭꦶ l;
  }

  static unsigned puur_naa_nka
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    ꦮꦭꦶ 1;
  }

  static codepoint_t *
  encode (codepoint_t *text,
	  const codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    ꦪꦼꦤ꧀ (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    ꦮꦭꦶ text;
  }
};

template <typename utf_t>
static anta_hstha const typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (const typename utf_t::codepoint_t *start,
			  const typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  ꦱꦼꦢꦼꦭ (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ꦱꦼꦢꦼꦭ (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  ꦮꦭꦶ start;
}


#endif /* HB_UTF_HH */
