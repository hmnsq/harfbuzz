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

#ضمن "hb.hh"

#ضمن "hb-open-type.hh"


بنية hb_utf8_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr unsigned max_len = 4;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    אם (c > 0x7Fu)
    {
      אם (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned صحيح t1;
	אם (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	אחרת
	  اذهب error;
      }
      אחרת אם (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned صحيح t1, t2;
	אם (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  אם (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    اذهب error;
	  text += 2;
	}
	אחרת
	  اذهب error;
      }
      אחרת אם (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned صحيح t1, t2, t3;
	אם (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  אם (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    اذهب error;
	  text += 3;
	}
	אחרת
	  اذهب error;
      }
      אחרת
	اذهب error;
    }

    *unicode = c;
    החזר text;

  error:
    *unicode = replacement;
    החזר text;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ثابت codepoint_t *end = text--;
    כלעוד (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    אם (likely (next (text, end, unicode, replacement) == end))
      החזר text;

    *unicode = replacement;
    החזר end - 1;
  }

  ساكن unsigned صحيح
  strlen (ثابت codepoint_t *text)
  { החזר ::strlen ((ثابت حرف *) text); }

  ساكن unsigned صحيح
  encode_len (hb_codepoint_t unicode)
  {
    אם (unicode <   0x0080u) החזר 1;
    אם (unicode <   0x0800u) החזר 2;
    אם (unicode <  0x10000u) החזר 3;
    אם (unicode < 0x110000u) החזר 4;
    החזר 3;
  }

  ساكن codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    אם (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    אם (unicode < 0x0080u)
     *text++ = unicode;
    אחרת אם (unicode < 0x0800u)
    {
      אם (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    אחרת אם (unicode < 0x10000u)
    {
      אם (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    אחרת
    {
      אם (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    החזר text;
  }
};


template <typename TCodepoint>
بنية hb_utf16_xe_t
{
  static_assert (حجم (TCodepoint) == 2, "");
  تعريفنوع TCodepoint codepoint_t;
  ساكن constexpr unsigned max_len = 2;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    אם (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      החזר text;
    }

    אם (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      אם (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 החזר text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    החזר text;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    אם (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      החזר text;
    }

    אם (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      אם (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	החזר text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    החזר text;
  }


  ساكن unsigned صحيح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحيح l = 0;
    כלעוד (*text++) l++;
    החזר l;
  }

  ساكن unsigned صحيح
  encode_len (hb_codepoint_t unicode)
  {
    החזר unicode < 0x10000 ? 1 : 2;
  }

  ساكن codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    אם (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    אם (unicode < 0x10000u)
     *text++ = unicode;
    אחרת אם (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    החזר text;
  }
};

تعريفنوع hb_utf16_xe_t<uint16_t> hb_utf16_t;
تعريفنوع hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, منطقي validate=אמת>
بنية hb_utf32_xe_t
{
  static_assert (حجم (TCodepoint) == 4, "");
  تعريفنوع TCodepoint codepoint_t;
  ساكن constexpr unsigned max_len = 1;

  ساكن مضمن ثابت TCodepoint *
  next (ثابت TCodepoint *text,
	ثابت TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    אם (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    החזר text;
  }

  ساكن مضمن ثابت TCodepoint *
  prev (ثابت TCodepoint *text,
	ثابت TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    אם (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    החזר text;
  }

  ساكن unsigned صحيح
  strlen (ثابت TCodepoint *text)
  {
    unsigned صحيح l = 0;
    כלעוד (*text++) l++;
    החזר l;
  }

  ساكن unsigned صحيح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    החזר 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    אם (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    החזר text;
  }
};

تعريفنوع hb_utf32_xe_t<uint32_t> hb_utf32_t;
تعريفنوع hb_utf32_xe_t<uint32_t, שקר> hb_utf32_novalidate_t;


بنية hb_latin1_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr unsigned max_len = 1;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    החזר text;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    החזר text;
  }

  ساكن unsigned صحيح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحيح l = 0;
    כלעוד (*text++) l++;
    החזר l;
  }

  ساكن unsigned صحيح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    החזר 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    אם (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    החזר text;
  }
};


بنية hb_ascii_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr unsigned max_len = 1;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *text,
	ثابت codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    אם (*unicode >= 0x0080u)
      *unicode = replacement;
    החזר text;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *text,
	ثابت codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    אם (*unicode >= 0x0080u)
      *unicode = replacement;
    החזר text;
  }

  ساكن unsigned صحيح
  strlen (ثابت codepoint_t *text)
  {
    unsigned صحيح l = 0;
    כלעוד (*text++) l++;
    החזר l;
  }

  ساكن unsigned صحيح
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    החזר 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *text,
	  ثابت codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    אם (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    החזר text;
  }
};

template <typename utf_t>
ساكن مضمن ثابت typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (ثابت typename utf_t::codepoint_t *start,
			  ثابت typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  כלעוד (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  כלעוד (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  החזר start;
}


#endif /* HB_UTF_HH */
