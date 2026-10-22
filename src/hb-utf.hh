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

#شامل "hb.hh"

#شامل "hb-open-type.hh"


ساختار hb_utf8_t
{
  تعریفنوع uint8_t codepoint_t;
  ایستا constexpr unsigned max_len = 4;

  ایستا درونخطی ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    اگر (c > 0x7Fu)
    {
      اگر (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned صحیح t1;
	اگر (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	وگرنه
	  برو error;
      }
      وگرنه اگر (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned صحیح t1, t2;
	اگر (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  اگر (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    برو error;
	  text += 2;
	}
	وگرنه
	  برو error;
      }
      وگرنه اگر (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned صحیح t1, t2, t3;
	اگر (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  اگر (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    برو error;
	  text += 3;
	}
	وگرنه
	  برو error;
      }
      وگرنه
	برو error;
    }

    *unicode = c;
    بازگشت text;

  error:
    *unicode = replacement;
    بازگشت text;
  }

  ایستا درونخطی ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ثابت codepoint_t *end = text--;
    وقتی (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    اگر (likely (next (text, end, unicode, replacement) == end))
      بازگشت text;

    *unicode = replacement;
    بازگشت end - 1;
  }

  ایستا unsigned صحیح
  strlen (ثابت codepoint_t *text)
  { بازگشت ::strlen ((ثابت نویسه *) text); }

  ایستا unsigned صحیح
  encode_len (hb_codepoint_t unicode)
  {
    اگر (unicode <   0x0080u) بازگشت 1;
    اگر (unicode <   0x0800u) بازگشت 2;
    اگر (unicode <  0x10000u) بازگشت 3;
    اگر (unicode < 0x110000u) بازگشت 4;
    بازگشت 3;
  }

  ایستا codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    اگر (unicode < 0x0080u)
     *text++ = unicode;
    وگرنه اگر (unicode < 0x0800u)
    {
      اگر (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    وگرنه اگر (unicode < 0x10000u)
    {
      اگر (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    وگرنه
    {
      اگر (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    بازگشت text;
  }
};


template <typename TCodepoint>
ساختار hb_utf16_xe_t
{
  static_assert (اندازه (TCodepoint) == 2, "");
  تعریفنوع TCodepoint codepoint_t;
  ایستا constexpr unsigned max_len = 2;

  ایستا درونخطی ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    اگر (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      بازگشت text;
    }

    اگر (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      اگر (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 بازگشت text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    بازگشت text;
  }

  ایستا درونخطی ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    اگر (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      بازگشت text;
    }

    اگر (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      اگر (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	بازگشت text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    بازگشت text;
  }


  ایستا unsigned صحیح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحیح l = 0;
    وقتی (*text++) l++;
    بازگشت l;
  }

  ایستا unsigned صحیح
  encode_len (hb_codepoint_t unicode)
  {
    بازگشت unicode < 0x10000 ? 1 : 2;
  }

  ایستا codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    اگر (unicode < 0x10000u)
     *text++ = unicode;
    وگرنه اگر (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    بازگشت text;
  }
};

تعریفنوع hb_utf16_xe_t<uint16_t> hb_utf16_t;
تعریفنوع hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, منطقی validate=درست>
ساختار hb_utf32_xe_t
{
  static_assert (اندازه (TCodepoint) == 4, "");
  تعریفنوع TCodepoint codepoint_t;
  ایستا constexpr unsigned max_len = 1;

  ایستا درونخطی ثابت TCodepoint *
  next (ثابت TCodepoint *text,
	ثابت TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    اگر (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    بازگشت text;
  }

  ایستا درونخطی ثابت TCodepoint *
  prev (ثابت TCodepoint *text,
	ثابت TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    اگر (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    بازگشت text;
  }

  ایستا unsigned صحیح
  strlen (ثابت TCodepoint *text)
  {
    unsigned صحیح l = 0;
    وقتی (*text++) l++;
    بازگشت l;
  }

  ایستا unsigned صحیح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    بازگشت 1;
  }

  ایستا codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    اگر (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    بازگشت text;
  }
};

تعریفنوع hb_utf32_xe_t<uint32_t> hb_utf32_t;
تعریفنوع hb_utf32_xe_t<uint32_t, نادرست> hb_utf32_novalidate_t;


ساختار hb_latin1_t
{
  تعریفنوع uint8_t codepoint_t;
  ایستا constexpr unsigned max_len = 1;

  ایستا درونخطی ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    بازگشت text;
  }

  ایستا درونخطی ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    بازگشت text;
  }

  ایستا unsigned صحیح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحیح l = 0;
    وقتی (*text++) l++;
    بازگشت l;
  }

  ایستا unsigned صحیح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    بازگشت 1;
  }

  ایستا codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    بازگشت text;
  }
};


ساختار hb_ascii_t
{
  تعریفنوع uint8_t codepoint_t;
  ایستا constexpr unsigned max_len = 1;

  ایستا درونخطی ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    اگر (*unicode >= 0x0080u)
      *unicode = replacement;
    بازگشت text;
  }

  ایستا درونخطی ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    اگر (*unicode >= 0x0080u)
      *unicode = replacement;
    بازگشت text;
  }

  ایستا unsigned صحیح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحیح l = 0;
    وقتی (*text++) l++;
    بازگشت l;
  }

  ایستا unsigned صحیح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    بازگشت 1;
  }

  ایستا codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    بازگشت text;
  }
};

template <typename utf_t>
ایستا درونخطی ثابت typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (ثابت typename utf_t::codepoint_t *start,
			  ثابت typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  وقتی (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  وقتی (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  بازگشت start;
}


#endif /* HB_UTF_HH */
