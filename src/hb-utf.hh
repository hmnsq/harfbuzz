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
  प्रकारव्याख्या uint8_t codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 4;

  स्थिर ओळीत स्थिरांक codepoint_t *
  next (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    जर (c > 0x7Fu)
    {
      जर (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	अचिन्हित पूर्णांक t1;
	जर (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	अन्यथा
	  जा error;
      }
      अन्यथा जर (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	अचिन्हित पूर्णांक t1, t2;
	जर (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  जर (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    जा error;
	  text += 2;
	}
	अन्यथा
	  जा error;
      }
      अन्यथा जर (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	अचिन्हित पूर्णांक t1, t2, t3;
	जर (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  जर (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    जा error;
	  text += 3;
	}
	अन्यथा
	  जा error;
      }
      अन्यथा
	जा error;
    }

    *unicode = c;
    परत text;

  error:
    *unicode = replacement;
    परत text;
  }

  स्थिर ओळीत स्थिरांक codepoint_t *
  prev (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    स्थिरांक codepoint_t *end = text--;
    पर्यंत (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    जर (likely (next (text, end, unicode, replacement) == end))
      परत text;

    *unicode = replacement;
    परत end - 1;
  }

  स्थिर अचिन्हित पूर्णांक
  म_माप (स्थिरांक codepoint_t *text)
  { परत ::म_माप ((स्थिरांक अक्षर *) text); }

  स्थिर अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode)
  {
    जर (unicode <   0x0080u) परत 1;
    जर (unicode <   0x0800u) परत 2;
    जर (unicode <  0x10000u) परत 3;
    जर (unicode < 0x110000u) परत 4;
    परत 3;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  स्थिरांक codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    जर (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    जर (unicode < 0x0080u)
     *text++ = unicode;
    अन्यथा जर (unicode < 0x0800u)
    {
      जर (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा जर (unicode < 0x10000u)
    {
      जर (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा
    {
      जर (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    परत text;
  }
};


ढाँचा <typename TCodepoint>
संरचना hb_utf16_xe_t
{
  static_assert (आकार (TCodepoint) == 2, "");
  प्रकारव्याख्या TCodepoint codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 2;

  स्थिर ओळीत स्थिरांक codepoint_t *
  next (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    जर (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      परत text;
    }

    जर (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      जर (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 परत text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    परत text;
  }

  स्थिर ओळीत स्थिरांक codepoint_t *
  prev (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    जर (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      परत text;
    }

    जर (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      जर (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	परत text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    परत text;
  }


  स्थिर अचिन्हित पूर्णांक
  म_माप (स्थिरांक codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    पर्यंत (*text++) l++;
    परत l;
  }

  स्थिर अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode)
  {
    परत unicode < 0x10000 ? 1 : 2;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  स्थिरांक codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    जर (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    जर (unicode < 0x10000u)
     *text++ = unicode;
    अन्यथा जर (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    परत text;
  }
};

प्रकारव्याख्या hb_utf16_xe_t<uint16_t> hb_utf16_t;
प्रकारव्याख्या hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


ढाँचा <typename TCodepoint, तार्किक validate=सत्य>
संरचना hb_utf32_xe_t
{
  static_assert (आकार (TCodepoint) == 4, "");
  प्रकारव्याख्या TCodepoint codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर ओळीत स्थिरांक TCodepoint *
  next (स्थिरांक TCodepoint *text,
	स्थिरांक TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    जर (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    परत text;
  }

  स्थिर ओळीत स्थिरांक TCodepoint *
  prev (स्थिरांक TCodepoint *text,
	स्थिरांक TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    जर (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    परत text;
  }

  स्थिर अचिन्हित पूर्णांक
  म_माप (स्थिरांक TCodepoint *text)
  {
    अचिन्हित पूर्णांक l = 0;
    पर्यंत (*text++) l++;
    परत l;
  }

  स्थिर अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    परत 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  स्थिरांक codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    जर (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    परत text;
  }
};

प्रकारव्याख्या hb_utf32_xe_t<uint32_t> hb_utf32_t;
प्रकारव्याख्या hb_utf32_xe_t<uint32_t, असत्य> hb_utf32_novalidate_t;


संरचना hb_latin1_t
{
  प्रकारव्याख्या uint8_t codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर ओळीत स्थिरांक codepoint_t *
  next (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    परत text;
  }

  स्थिर ओळीत स्थिरांक codepoint_t *
  prev (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    परत text;
  }

  स्थिर अचिन्हित पूर्णांक
  म_माप (स्थिरांक codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    पर्यंत (*text++) l++;
    परत l;
  }

  स्थिर अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    परत 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  स्थिरांक codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    जर (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    परत text;
  }
};


संरचना hb_ascii_t
{
  प्रकारव्याख्या uint8_t codepoint_t;
  स्थिर constexpr अचिन्हित max_len = 1;

  स्थिर ओळीत स्थिरांक codepoint_t *
  next (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    जर (*unicode >= 0x0080u)
      *unicode = replacement;
    परत text;
  }

  स्थिर ओळीत स्थिरांक codepoint_t *
  prev (स्थिरांक codepoint_t *text,
	स्थिरांक codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    जर (*unicode >= 0x0080u)
      *unicode = replacement;
    परत text;
  }

  स्थिर अचिन्हित पूर्णांक
  म_माप (स्थिरांक codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    पर्यंत (*text++) l++;
    परत l;
  }

  स्थिर अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    परत 1;
  }

  स्थिर codepoint_t *
  encode (codepoint_t *text,
	  स्थिरांक codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    जर (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    परत text;
  }
};

ढाँचा <typename utf_t>
स्थिर ओळीत स्थिरांक typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (स्थिरांक typename utf_t::codepoint_t *start,
			  स्थिरांक typename utf_t::codepoint_t *text,
			  अचिन्हित text_len,
			  चिन्हित offset)
{
  hb_codepoint_t unicode;

  पर्यंत (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  पर्यंत (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  परत start;
}


#endif /* HB_UTF_HH */
