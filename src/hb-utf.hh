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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_UTF_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UTF_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"


ਬਣਤਰ hb_utf8_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t codepoint_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_len = 4;

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  next (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸਿਰਾ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *ਪਾਠ++;

    ਜੇ (c > 0x7Fu)
    {
      ਜੇ (hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ t1;
	ਜੇ (likely (ਪਾਠ < ਸਿਰਾ &&
		    (t1 = ਪਾਠ[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  ਪਾਠ++;
	}
	ਨਹੀਂਤਾਂ
	  ਜਾਓ error;
      }
      ਨਹੀਂਤਾਂ ਜੇ (hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ t1, t2;
	ਜੇ (likely (1 < ਸਿਰਾ - ਪਾਠ &&
		    (t1 = ਪਾਠ[0] - 0x80u) <= 0x3Fu &&
		    (t2 = ਪਾਠ[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  ਜੇ (unlikely (c < 0x0800u || hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    ਜਾਓ error;
	  ਪਾਠ += 2;
	}
	ਨਹੀਂਤਾਂ
	  ਜਾਓ error;
      }
      ਨਹੀਂਤਾਂ ਜੇ (hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ t1, t2, t3;
	ਜੇ (likely (2 < ਸਿਰਾ - ਪਾਠ &&
		    (t1 = ਪਾਠ[0] - 0x80u) <= 0x3Fu &&
		    (t2 = ਪਾਠ[1] - 0x80u) <= 0x3Fu &&
		    (t3 = ਪਾਠ[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  ਜੇ (unlikely (!hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    ਜਾਓ error;
	  ਪਾਠ += 3;
	}
	ਨਹੀਂਤਾਂ
	  ਜਾਓ error;
      }
      ਨਹੀਂਤਾਂ
	ਜਾਓ error;
    }

    *unicode = c;
    ਵਾਪਸ ਪਾਠ;

  error:
    *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  prev (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸ਼ੁਰੂ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ਅਚਲ codepoint_t *ਸਿਰਾ = ਪਾਠ--;
    ਜਦੋਂ (ਸ਼ੁਰੂ < ਪਾਠ && (*ਪਾਠ & 0xc0) == 0x80 && ਸਿਰਾ - ਪਾਠ < 4)
      ਪਾਠ--;

    ਜੇ (likely (next (ਪਾਠ, ਸਿਰਾ, unicode, replacement) == ਸਿਰਾ))
      ਵਾਪਸ ਪਾਠ;

    *unicode = replacement;
    ਵਾਪਸ ਸਿਰਾ - 1;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਲੜੀ_ਲੰਬਾਈ (ਅਚਲ codepoint_t *ਪਾਠ)
  { ਵਾਪਸ ::ਲੜੀ_ਲੰਬਾਈ ((ਅਚਲ ਅੱਖਰ *) ਪਾਠ); }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  encode_len (hb_codepoint_t unicode)
  {
    ਜੇ (unicode <   0x0080u) ਵਾਪਸ 1;
    ਜੇ (unicode <   0x0800u) ਵਾਪਸ 2;
    ਜੇ (unicode <  0x10000u) ਵਾਪਸ 3;
    ਜੇ (unicode < 0x110000u) ਵਾਪਸ 4;
    ਵਾਪਸ 3;
  }

  ਸਥਿਰ codepoint_t *
  encode (codepoint_t *ਪਾਠ,
	  ਅਚਲ codepoint_t *ਸਿਰਾ,
	  hb_codepoint_t unicode)
  {
    ਜੇ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ਜੇ (unicode < 0x0080u)
     *ਪਾਠ++ = unicode;
    ਨਹੀਂਤਾਂ ਜੇ (unicode < 0x0800u)
    {
      ਜੇ (ਸਿਰਾ - ਪਾਠ >= 2)
      {
	*ਪਾਠ++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ਨਹੀਂਤਾਂ ਜੇ (unicode < 0x10000u)
    {
      ਜੇ (ਸਿਰਾ - ਪਾਠ >= 3)
      {
	*ਪਾਠ++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode >>  6));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (ਸਿਰਾ - ਪਾਠ >= 4)
      {
	*ਪਾਠ++ =  0xF0u + (0x07u & (unicode >> 18));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode >> 12));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode >>  6));
	*ਪਾਠ++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ਵਾਪਸ ਪਾਠ;
  }
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TCodepoint>
ਬਣਤਰ hb_utf16_xe_t
{
  static_assert (ਆਕਾਰ (TCodepoint) == 2, "");
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ TCodepoint codepoint_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_len = 2;

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  next (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸਿਰਾ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *ਪਾਠ++;

    ਜੇ (likely (!hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ਵਾਪਸ ਪਾਠ;
    }

    ਜੇ (likely (c <= 0xDBFFu && ਪਾਠ < ਸਿਰਾ))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *ਪਾਠ;
      ਜੇ (likely (hb_ਵਿੱਚ_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 ਪਾਠ++;
	 ਵਾਪਸ ਪਾਠ;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  prev (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸ਼ੁਰੂ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--ਪਾਠ;

    ਜੇ (likely (!hb_ਵਿੱਚ_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      ਵਾਪਸ ਪਾਠ;
    }

    ਜੇ (likely (c >= 0xDC00u && ਸ਼ੁਰੂ < ਪਾਠ))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = ਪਾਠ[-1];
      ਜੇ (likely (hb_ਵਿੱਚ_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	ਪਾਠ--;
	ਵਾਪਸ ਪਾਠ;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }


  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਲੜੀ_ਲੰਬਾਈ (ਅਚਲ codepoint_t *ਪਾਠ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = 0;
    ਜਦੋਂ (*ਪਾਠ++) l++;
    ਵਾਪਸ l;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  encode_len (hb_codepoint_t unicode)
  {
    ਵਾਪਸ unicode < 0x10000 ? 1 : 2;
  }

  ਸਥਿਰ codepoint_t *
  encode (codepoint_t *ਪਾਠ,
	  ਅਚਲ codepoint_t *ਸਿਰਾ,
	  hb_codepoint_t unicode)
  {
    ਜੇ (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    ਜੇ (unicode < 0x10000u)
     *ਪਾਠ++ = unicode;
    ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰਾ - ਪਾਠ >= 2)
    {
      unicode -= 0x10000u;
      *ਪਾਠ++ =  0xD800u + (unicode >> 10);
      *ਪਾਠ++ =  0xDC00u + (unicode & 0x03FFu);
    }
    ਵਾਪਸ ਪਾਠ;
  }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_utf16_xe_t<uint16_t> hb_utf16_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TCodepoint, ਤਾਰਕਿਕ validate=ਸੱਚ>
ਬਣਤਰ hb_utf32_xe_t
{
  static_assert (ਆਕਾਰ (TCodepoint) == 4, "");
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ TCodepoint codepoint_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_len = 1;

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ TCodepoint *
  next (ਅਚਲ TCodepoint *ਪਾਠ,
	ਅਚਲ TCodepoint *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *ਪਾਠ++;
    ਜੇ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ TCodepoint *
  prev (ਅਚਲ TCodepoint *ਪਾਠ,
	ਅਚਲ TCodepoint *ਸ਼ੁਰੂ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--ਪਾਠ;
    ਜੇ (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਲੜੀ_ਲੰਬਾਈ (ਅਚਲ TCodepoint *ਪਾਠ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = 0;
    ਜਦੋਂ (*ਪਾਠ++) l++;
    ਵਾਪਸ l;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  encode_len (hb_codepoint_t unicode HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ 1;
  }

  ਸਥਿਰ codepoint_t *
  encode (codepoint_t *ਪਾਠ,
	  ਅਚਲ codepoint_t *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	  hb_codepoint_t unicode)
  {
    ਜੇ (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *ਪਾਠ++ = unicode;
    ਵਾਪਸ ਪਾਠ;
  }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_utf32_xe_t<uint32_t> hb_utf32_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_utf32_xe_t<uint32_t, ਝੂਠ> hb_utf32_novalidate_t;


ਬਣਤਰ hb_latin1_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t codepoint_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_len = 1;

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  next (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_ਅਣਵਰਤਿਆ)
  {
    *unicode = *ਪਾਠ++;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  prev (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸ਼ੁਰੂ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_ਅਣਵਰਤਿਆ)
  {
    *unicode = *--ਪਾਠ;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਲੜੀ_ਲੰਬਾਈ (ਅਚਲ codepoint_t *ਪਾਠ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = 0;
    ਜਦੋਂ (*ਪਾਠ++) l++;
    ਵਾਪਸ l;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  encode_len (hb_codepoint_t unicode HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ 1;
  }

  ਸਥਿਰ codepoint_t *
  encode (codepoint_t *ਪਾਠ,
	  ਅਚਲ codepoint_t *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	  hb_codepoint_t unicode)
  {
    ਜੇ (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *ਪਾਠ++ = unicode;
    ਵਾਪਸ ਪਾਠ;
  }
};


ਬਣਤਰ hb_ascii_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t codepoint_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_len = 1;

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  next (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *ਪਾਠ++;
    ਜੇ (*unicode >= 0x0080u)
      *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ codepoint_t *
  prev (ਅਚਲ codepoint_t *ਪਾਠ,
	ਅਚਲ codepoint_t *ਸ਼ੁਰੂ HB_ਅਣਵਰਤਿਆ,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--ਪਾਠ;
    ਜੇ (*unicode >= 0x0080u)
      *unicode = replacement;
    ਵਾਪਸ ਪਾਠ;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਲੜੀ_ਲੰਬਾਈ (ਅਚਲ codepoint_t *ਪਾਠ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = 0;
    ਜਦੋਂ (*ਪਾਠ++) l++;
    ਵਾਪਸ l;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  encode_len (hb_codepoint_t unicode HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ 1;
  }

  ਸਥਿਰ codepoint_t *
  encode (codepoint_t *ਪਾਠ,
	  ਅਚਲ codepoint_t *ਸਿਰਾ HB_ਅਣਵਰਤਿਆ,
	  hb_codepoint_t unicode)
  {
    ਜੇ (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *ਪਾਠ++ = unicode;
    ਵਾਪਸ ਪਾਠ;
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ utf_t>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t *
hb_utf_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕ (ਅਚਲ ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t *ਸ਼ੁਰੂ,
			  ਅਚਲ ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t *ਪਾਠ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਠ_len,
			  ਨਿਸ਼ਾਨੀਵਾਲਾ ਵਿੱਥ)
{
  hb_codepoint_t unicode;

  ਜਦੋਂ (ਵਿੱਥ-- > 0)
    ਸ਼ੁਰੂ = utf_t::next (ਸ਼ੁਰੂ,
			 ਪਾਠ + ਪਾਠ_len,
			 &unicode,
			 HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT);

  ਜਦੋਂ (ਵਿੱਥ++ < 0)
    ਸ਼ੁਰੂ = utf_t::prev (ਸ਼ੁਰੂ,
			 ਪਾਠ,
			 &unicode,
			 HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT);

  ਵਾਪਸ ਸ਼ੁਰੂ;
}


#ਜੇ_ਅੰਤ /* HB_UTF_HH */
