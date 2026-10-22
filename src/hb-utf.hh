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

#अगर_अघोषित HB_UTF_HH
#घोषणा HB_UTF_HH

#समावेश "hb.hh"

#समावेश "hb-open-type.hh"


काष्ठा hb_utf8_t
{
  प्रकार uint8_t codepoint_t;
  अटल constexpr अचिन्हित max_len = 4;

  अटल अंतरभूत स्थिर codepoint_t *
  next (स्थिर codepoint_t *text,
	स्थिर codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    अगर (c > 0x7Fu)
    {
      अगर (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	अचिन्हित पूर्णांक t1;
	अगर (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	अन्यथा
	  जाओ error;
      }
      अन्यथा अगर (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	अचिन्हित पूर्णांक t1, t2;
	अगर (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  अगर (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    जाओ error;
	  text += 2;
	}
	अन्यथा
	  जाओ error;
      }
      अन्यथा अगर (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	अचिन्हित पूर्णांक t1, t2, t3;
	अगर (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  अगर (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    जाओ error;
	  text += 3;
	}
	अन्यथा
	  जाओ error;
      }
      अन्यथा
	जाओ error;
    }

    *unicode = c;
    वापस text;

  error:
    *unicode = replacement;
    वापस text;
  }

  अटल अंतरभूत स्थिर codepoint_t *
  prev (स्थिर codepoint_t *text,
	स्थिर codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    स्थिर codepoint_t *end = text--;
    जबतक (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    अगर (likely (next (text, end, unicode, replacement) == end))
      वापस text;

    *unicode = replacement;
    वापस end - 1;
  }

  अटल अचिन्हित पूर्णांक
  म_माप (स्थिर codepoint_t *text)
  { वापस ::म_माप ((स्थिर अक्षर *) text); }

  अटल अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode)
  {
    अगर (unicode <   0x0080u) वापस 1;
    अगर (unicode <   0x0800u) वापस 2;
    अगर (unicode <  0x10000u) वापस 3;
    अगर (unicode < 0x110000u) वापस 4;
    वापस 3;
  }

  अटल codepoint_t *
  encode (codepoint_t *text,
	  स्थिर codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    अगर (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    अगर (unicode < 0x0080u)
     *text++ = unicode;
    अन्यथा अगर (unicode < 0x0800u)
    {
      अगर (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा अगर (unicode < 0x10000u)
    {
      अगर (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    अन्यथा
    {
      अगर (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    वापस text;
  }
};


ढाँचा <typename TCodepoint>
काष्ठा hb_utf16_xe_t
{
  static_assert (माप (TCodepoint) == 2, "");
  प्रकार TCodepoint codepoint_t;
  अटल constexpr अचिन्हित max_len = 2;

  अटल अंतरभूत स्थिर codepoint_t *
  next (स्थिर codepoint_t *text,
	स्थिर codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    अगर (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      वापस text;
    }

    अगर (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      अगर (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 वापस text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    वापस text;
  }

  अटल अंतरभूत स्थिर codepoint_t *
  prev (स्थिर codepoint_t *text,
	स्थिर codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    अगर (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      वापस text;
    }

    अगर (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      अगर (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	वापस text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    वापस text;
  }


  अटल अचिन्हित पूर्णांक
  म_माप (स्थिर codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    जबतक (*text++) l++;
    वापस l;
  }

  अटल अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode)
  {
    वापस unicode < 0x10000 ? 1 : 2;
  }

  अटल codepoint_t *
  encode (codepoint_t *text,
	  स्थिर codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    अगर (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    अगर (unicode < 0x10000u)
     *text++ = unicode;
    अन्यथा अगर (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    वापस text;
  }
};

प्रकार hb_utf16_xe_t<uint16_t> hb_utf16_t;
प्रकार hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


ढाँचा <typename TCodepoint, bool validate=true>
काष्ठा hb_utf32_xe_t
{
  static_assert (माप (TCodepoint) == 4, "");
  प्रकार TCodepoint codepoint_t;
  अटल constexpr अचिन्हित max_len = 1;

  अटल अंतरभूत स्थिर TCodepoint *
  next (स्थिर TCodepoint *text,
	स्थिर TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    अगर (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    वापस text;
  }

  अटल अंतरभूत स्थिर TCodepoint *
  prev (स्थिर TCodepoint *text,
	स्थिर TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    अगर (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    वापस text;
  }

  अटल अचिन्हित पूर्णांक
  म_माप (स्थिर TCodepoint *text)
  {
    अचिन्हित पूर्णांक l = 0;
    जबतक (*text++) l++;
    वापस l;
  }

  अटल अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    वापस 1;
  }

  अटल codepoint_t *
  encode (codepoint_t *text,
	  स्थिर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    अगर (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    वापस text;
  }
};

प्रकार hb_utf32_xe_t<uint32_t> hb_utf32_t;
प्रकार hb_utf32_xe_t<uint32_t, false> hb_utf32_novalidate_t;


काष्ठा hb_latin1_t
{
  प्रकार uint8_t codepoint_t;
  अटल constexpr अचिन्हित max_len = 1;

  अटल अंतरभूत स्थिर codepoint_t *
  next (स्थिर codepoint_t *text,
	स्थिर codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    वापस text;
  }

  अटल अंतरभूत स्थिर codepoint_t *
  prev (स्थिर codepoint_t *text,
	स्थिर codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    वापस text;
  }

  अटल अचिन्हित पूर्णांक
  म_माप (स्थिर codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    जबतक (*text++) l++;
    वापस l;
  }

  अटल अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    वापस 1;
  }

  अटल codepoint_t *
  encode (codepoint_t *text,
	  स्थिर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    अगर (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    वापस text;
  }
};


काष्ठा hb_ascii_t
{
  प्रकार uint8_t codepoint_t;
  अटल constexpr अचिन्हित max_len = 1;

  अटल अंतरभूत स्थिर codepoint_t *
  next (स्थिर codepoint_t *text,
	स्थिर codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    अगर (*unicode >= 0x0080u)
      *unicode = replacement;
    वापस text;
  }

  अटल अंतरभूत स्थिर codepoint_t *
  prev (स्थिर codepoint_t *text,
	स्थिर codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    अगर (*unicode >= 0x0080u)
      *unicode = replacement;
    वापस text;
  }

  अटल अचिन्हित पूर्णांक
  म_माप (स्थिर codepoint_t *text)
  {
    अचिन्हित पूर्णांक l = 0;
    जबतक (*text++) l++;
    वापस l;
  }

  अटल अचिन्हित पूर्णांक
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    वापस 1;
  }

  अटल codepoint_t *
  encode (codepoint_t *text,
	  स्थिर codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    अगर (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    वापस text;
  }
};

ढाँचा <typename utf_t>
अटल अंतरभूत स्थिर typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (स्थिर typename utf_t::codepoint_t *start,
			  स्थिर typename utf_t::codepoint_t *text,
			  अचिन्हित text_len,
			  चिन्हित offset)
{
  hb_codepoint_t unicode;

  जबतक (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_बफर_REPLACEMENT_CODEPOINT_DEFAULT);

  जबतक (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_बफर_REPLACEMENT_CODEPOINT_DEFAULT);

  वापस start;
}


#पूर्ण_अगर /* HB_UTF_HH */
