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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-open-type.hh"


સંરચના hb_utf8_t
{
  પ્રકારવ્યાખ્યા uint8_t codepoint_t;
  સ્થિર constexpr unsigned max_len = 4;

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  next (અચલ codepoint_t *text,
	અચલ codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *text++;

    જો (c > 0x7Fu)
    {
      જો (hb_in_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	unsigned પૂર્ણાંક t1;
	જો (likely (text < end &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  text++;
	}
	નહીંતર
	  જાઓ error;
      }
      નહીંતર જો (hb_in_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	unsigned પૂર્ણાંક t1, t2;
	જો (likely (1 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  જો (unlikely (c < 0x0800u || hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    જાઓ error;
	  text += 2;
	}
	નહીંતર
	  જાઓ error;
      }
      નહીંતર જો (hb_in_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	unsigned પૂર્ણાંક t1, t2, t3;
	જો (likely (2 < end - text &&
		    (t1 = text[0] - 0x80u) <= 0x3Fu &&
		    (t2 = text[1] - 0x80u) <= 0x3Fu &&
		    (t3 = text[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  જો (unlikely (!hb_in_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    જાઓ error;
	  text += 3;
	}
	નહીંતર
	  જાઓ error;
      }
      નહીંતર
	જાઓ error;
    }

    *unicode = c;
    પાછા text;

  error:
    *unicode = replacement;
    પાછા text;
  }

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  prev (અચલ codepoint_t *text,
	અચલ codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    અચલ codepoint_t *end = text--;
    જ્યારે (start < text && (*text & 0xc0) == 0x80 && end - text < 4)
      text--;

    જો (likely (next (text, end, unicode, replacement) == end))
      પાછા text;

    *unicode = replacement;
    પાછા end - 1;
  }

  સ્થિર unsigned પૂર્ણાંક
  strlen (અચલ codepoint_t *text)
  { પાછા ::strlen ((અચલ અક્ષર *) text); }

  સ્થિર unsigned પૂર્ણાંક
  encode_len (hb_codepoint_t unicode)
  {
    જો (unicode <   0x0080u) પાછા 1;
    જો (unicode <   0x0800u) પાછા 2;
    જો (unicode <  0x10000u) પાછા 3;
    જો (unicode < 0x110000u) પાછા 4;
    પાછા 3;
  }

  સ્થિર codepoint_t *
  encode (codepoint_t *text,
	  અચલ codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    જો (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    જો (unicode < 0x0080u)
     *text++ = unicode;
    નહીંતર જો (unicode < 0x0800u)
    {
      જો (end - text >= 2)
      {
	*text++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    નહીંતર જો (unicode < 0x10000u)
    {
      જો (end - text >= 3)
      {
	*text++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    નહીંતર
    {
      જો (end - text >= 4)
      {
	*text++ =  0xF0u + (0x07u & (unicode >> 18));
	*text++ =  0x80u + (0x3Fu & (unicode >> 12));
	*text++ =  0x80u + (0x3Fu & (unicode >>  6));
	*text++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    પાછા text;
  }
};


template <typename TCodepoint>
સંરચના hb_utf16_xe_t
{
  static_assert (કદ (TCodepoint) == 2, "");
  પ્રકારવ્યાખ્યા TCodepoint codepoint_t;
  સ્થિર constexpr unsigned max_len = 2;

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  next (અચલ codepoint_t *text,
	અચલ codepoint_t *end,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *text++;

    જો (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      પાછા text;
    }

    જો (likely (c <= 0xDBFFu && text < end))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *text;
      જો (likely (hb_in_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 text++;
	 પાછા text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    પાછા text;
  }

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  prev (અચલ codepoint_t *text,
	અચલ codepoint_t *start,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--text;

    જો (likely (!hb_in_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      પાછા text;
    }

    જો (likely (c >= 0xDC00u && start < text))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = text[-1];
      જો (likely (hb_in_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	text--;
	પાછા text;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    પાછા text;
  }


  સ્થિર unsigned પૂર્ણાંક
  strlen (અચલ codepoint_t *text)
  {
    unsigned પૂર્ણાંક l = 0;
    જ્યારે (*text++) l++;
    પાછા l;
  }

  સ્થિર unsigned પૂર્ણાંક
  encode_len (hb_codepoint_t unicode)
  {
    પાછા unicode < 0x10000 ? 1 : 2;
  }

  સ્થિર codepoint_t *
  encode (codepoint_t *text,
	  અચલ codepoint_t *end,
	  hb_codepoint_t unicode)
  {
    જો (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    જો (unicode < 0x10000u)
     *text++ = unicode;
    નહીંતર જો (end - text >= 2)
    {
      unicode -= 0x10000u;
      *text++ =  0xD800u + (unicode >> 10);
      *text++ =  0xDC00u + (unicode & 0x03FFu);
    }
    પાછા text;
  }
};

પ્રકારવ્યાખ્યા hb_utf16_xe_t<uint16_t> hb_utf16_t;
પ્રકારવ્યાખ્યા hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


template <typename TCodepoint, તાર્કિક validate=સાચું>
સંરચના hb_utf32_xe_t
{
  static_assert (કદ (TCodepoint) == 4, "");
  પ્રકારવ્યાખ્યા TCodepoint codepoint_t;
  સ્થિર constexpr unsigned max_len = 1;

  સ્થિર અંતઃરેખ અચલ TCodepoint *
  next (અચલ TCodepoint *text,
	અચલ TCodepoint *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *text++;
    જો (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    પાછા text;
  }

  સ્થિર અંતઃરેખ અચલ TCodepoint *
  prev (અચલ TCodepoint *text,
	અચલ TCodepoint *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--text;
    જો (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    પાછા text;
  }

  સ્થિર unsigned પૂર્ણાંક
  strlen (અચલ TCodepoint *text)
  {
    unsigned પૂર્ણાંક l = 0;
    જ્યારે (*text++) l++;
    પાછા l;
  }

  સ્થિર unsigned પૂર્ણાંક
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    પાછા 1;
  }

  સ્થિર codepoint_t *
  encode (codepoint_t *text,
	  અચલ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    જો (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *text++ = unicode;
    પાછા text;
  }
};

પ્રકારવ્યાખ્યા hb_utf32_xe_t<uint32_t> hb_utf32_t;
પ્રકારવ્યાખ્યા hb_utf32_xe_t<uint32_t, ખોટું> hb_utf32_novalidate_t;


સંરચના hb_latin1_t
{
  પ્રકારવ્યાખ્યા uint8_t codepoint_t;
  સ્થિર constexpr unsigned max_len = 1;

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  next (અચલ codepoint_t *text,
	અચલ codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *text++;
    પાછા text;
  }

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  prev (અચલ codepoint_t *text,
	અચલ codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_UNUSED)
  {
    *unicode = *--text;
    પાછા text;
  }

  સ્થિર unsigned પૂર્ણાંક
  strlen (અચલ codepoint_t *text)
  {
    unsigned પૂર્ણાંક l = 0;
    જ્યારે (*text++) l++;
    પાછા l;
  }

  સ્થિર unsigned પૂર્ણાંક
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    પાછા 1;
  }

  સ્થિર codepoint_t *
  encode (codepoint_t *text,
	  અચલ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    જો (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *text++ = unicode;
    પાછા text;
  }
};


સંરચના hb_ascii_t
{
  પ્રકારવ્યાખ્યા uint8_t codepoint_t;
  સ્થિર constexpr unsigned max_len = 1;

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  next (અચલ codepoint_t *text,
	અચલ codepoint_t *end HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *text++;
    જો (*unicode >= 0x0080u)
      *unicode = replacement;
    પાછા text;
  }

  સ્થિર અંતઃરેખ અચલ codepoint_t *
  prev (અચલ codepoint_t *text,
	અચલ codepoint_t *start HB_UNUSED,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--text;
    જો (*unicode >= 0x0080u)
      *unicode = replacement;
    પાછા text;
  }

  સ્થિર unsigned પૂર્ણાંક
  strlen (અચલ codepoint_t *text)
  {
    unsigned પૂર્ણાંક l = 0;
    જ્યારે (*text++) l++;
    પાછા l;
  }

  સ્થિર unsigned પૂર્ણાંક
  encode_len (hb_codepoint_t unicode HB_UNUSED)
  {
    પાછા 1;
  }

  સ્થિર codepoint_t *
  encode (codepoint_t *text,
	  અચલ codepoint_t *end HB_UNUSED,
	  hb_codepoint_t unicode)
  {
    જો (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *text++ = unicode;
    પાછા text;
  }
};

template <typename utf_t>
સ્થિર અંતઃરેખ અચલ typename utf_t::codepoint_t *
hb_utf_offset_to_pointer (અચલ typename utf_t::codepoint_t *start,
			  અચલ typename utf_t::codepoint_t *text,
			  unsigned text_len,
			  signed offset)
{
  hb_codepoint_t unicode;

  જ્યારે (offset-- > 0)
    start = utf_t::next (start,
			 text + text_len,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  જ્યારે (offset++ < 0)
    start = utf_t::prev (start,
			 text,
			 &unicode,
			 HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT);

  પાછા start;
}


#endif /* HB_UTF_HH */
