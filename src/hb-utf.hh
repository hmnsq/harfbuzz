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

#اگر_غیر_تعریف_شدہ HB_UTF_HH
#تعریف_کرو HB_UTF_HH

#شامل "hb.hh"

#شامل "hb-open-type.hh"


ساخت hb_utf8_t
{
  قسمتعریف uint8_t codepoint_t;
  ساکن constexpr غیرعلامتی max_len = 4;

  ساکن اندرونی ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *انتہا,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *متن++;

    اگر (c > 0x7Fu)
    {
      اگر (hb_اندر_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	غیرعلامتی صحیح t1;
	اگر (likely (متن < انتہا &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  متن++;
	}
	ورنہ
	  جاؤ error;
      }
      ورنہ اگر (hb_اندر_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	غیرعلامتی صحیح t1, t2;
	اگر (likely (1 < انتہا - متن &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu &&
		    (t2 = متن[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  اگر (unlikely (c < 0x0800u || hb_اندر_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    جاؤ error;
	  متن += 2;
	}
	ورنہ
	  جاؤ error;
      }
      ورنہ اگر (hb_اندر_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	غیرعلامتی صحیح t1, t2, t3;
	اگر (likely (2 < انتہا - متن &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu &&
		    (t2 = متن[1] - 0x80u) <= 0x3Fu &&
		    (t3 = متن[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  اگر (unlikely (!hb_اندر_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    جاؤ error;
	  متن += 3;
	}
	ورنہ
	  جاؤ error;
      }
      ورنہ
	جاؤ error;
    }

    *unicode = c;
    واپس متن;

  error:
    *unicode = replacement;
    واپس متن;
  }

  ساکن اندرونی ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *آغاز,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ثابت codepoint_t *انتہا = متن--;
    جب (آغاز < متن && (*متن & 0xc0) == 0x80 && انتہا - متن < 4)
      متن--;

    اگر (likely (next (متن, انتہا, unicode, replacement) == انتہا))
      واپس متن;

    *unicode = replacement;
    واپس انتہا - 1;
  }

  ساکن غیرعلامتی صحیح
  لڑی_لمبائی (ثابت codepoint_t *متن)
  { واپس ::لڑی_لمبائی ((ثابت حرف *) متن); }

  ساکن غیرعلامتی صحیح
  encode_len (hb_codepoint_t unicode)
  {
    اگر (unicode <   0x0080u) واپس 1;
    اگر (unicode <   0x0800u) واپس 2;
    اگر (unicode <  0x10000u) واپس 3;
    اگر (unicode < 0x110000u) واپس 4;
    واپس 3;
  }

  ساکن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *انتہا,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    اگر (unicode < 0x0080u)
     *متن++ = unicode;
    ورنہ اگر (unicode < 0x0800u)
    {
      اگر (انتہا - متن >= 2)
      {
	*متن++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ورنہ اگر (unicode < 0x10000u)
    {
      اگر (انتہا - متن >= 3)
      {
	*متن++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*متن++ =  0x80u + (0x3Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    ورنہ
    {
      اگر (انتہا - متن >= 4)
      {
	*متن++ =  0xF0u + (0x07u & (unicode >> 18));
	*متن++ =  0x80u + (0x3Fu & (unicode >> 12));
	*متن++ =  0x80u + (0x3Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    واپس متن;
  }
};


سانچہ <قسم_نام TCodepoint>
ساخت hb_utf16_xe_t
{
  static_assert (حجم (TCodepoint) == 2, "");
  قسمتعریف TCodepoint codepoint_t;
  ساکن constexpr غیرعلامتی max_len = 2;

  ساکن اندرونی ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *انتہا,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *متن++;

    اگر (likely (!hb_اندر_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      واپس متن;
    }

    اگر (likely (c <= 0xDBFFu && متن < انتہا))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *متن;
      اگر (likely (hb_اندر_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 متن++;
	 واپس متن;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    واپس متن;
  }

  ساکن اندرونی ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *آغاز,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--متن;

    اگر (likely (!hb_اندر_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      واپس متن;
    }

    اگر (likely (c >= 0xDC00u && آغاز < متن))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = متن[-1];
      اگر (likely (hb_اندر_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	متن--;
	واپس متن;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    واپس متن;
  }


  ساکن غیرعلامتی صحیح
  لڑی_لمبائی (ثابت codepoint_t *متن)
  {
    غیرعلامتی صحیح l = 0;
    جب (*متن++) l++;
    واپس l;
  }

  ساکن غیرعلامتی صحیح
  encode_len (hb_codepoint_t unicode)
  {
    واپس unicode < 0x10000 ? 1 : 2;
  }

  ساکن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *انتہا,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    اگر (unicode < 0x10000u)
     *متن++ = unicode;
    ورنہ اگر (انتہا - متن >= 2)
    {
      unicode -= 0x10000u;
      *متن++ =  0xD800u + (unicode >> 10);
      *متن++ =  0xDC00u + (unicode & 0x03FFu);
    }
    واپس متن;
  }
};

قسمتعریف hb_utf16_xe_t<uint16_t> hb_utf16_t;
قسمتعریف hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


سانچہ <قسم_نام TCodepoint, منطقی validate=سچ>
ساخت hb_utf32_xe_t
{
  static_assert (حجم (TCodepoint) == 4, "");
  قسمتعریف TCodepoint codepoint_t;
  ساکن constexpr غیرعلامتی max_len = 1;

  ساکن اندرونی ثابت TCodepoint *
  next (ثابت TCodepoint *متن,
	ثابت TCodepoint *انتہا HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *متن++;
    اگر (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    واپس متن;
  }

  ساکن اندرونی ثابت TCodepoint *
  prev (ثابت TCodepoint *متن,
	ثابت TCodepoint *آغاز HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--متن;
    اگر (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    واپس متن;
  }

  ساکن غیرعلامتی صحیح
  لڑی_لمبائی (ثابت TCodepoint *متن)
  {
    غیرعلامتی صحیح l = 0;
    جب (*متن++) l++;
    واپس l;
  }

  ساکن غیرعلامتی صحیح
  encode_len (hb_codepoint_t unicode HB_غیر_مستعمل)
  {
    واپس 1;
  }

  ساکن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *انتہا HB_غیر_مستعمل,
	  hb_codepoint_t unicode)
  {
    اگر (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *متن++ = unicode;
    واپس متن;
  }
};

قسمتعریف hb_utf32_xe_t<uint32_t> hb_utf32_t;
قسمتعریف hb_utf32_xe_t<uint32_t, جھوٹ> hb_utf32_novalidate_t;


ساخت hb_latin1_t
{
  قسمتعریف uint8_t codepoint_t;
  ساکن constexpr غیرعلامتی max_len = 1;

  ساکن اندرونی ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *انتہا HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_غیر_مستعمل)
  {
    *unicode = *متن++;
    واپس متن;
  }

  ساکن اندرونی ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *آغاز HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_غیر_مستعمل)
  {
    *unicode = *--متن;
    واپس متن;
  }

  ساکن غیرعلامتی صحیح
  لڑی_لمبائی (ثابت codepoint_t *متن)
  {
    غیرعلامتی صحیح l = 0;
    جب (*متن++) l++;
    واپس l;
  }

  ساکن غیرعلامتی صحیح
  encode_len (hb_codepoint_t unicode HB_غیر_مستعمل)
  {
    واپس 1;
  }

  ساکن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *انتہا HB_غیر_مستعمل,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *متن++ = unicode;
    واپس متن;
  }
};


ساخت hb_ascii_t
{
  قسمتعریف uint8_t codepoint_t;
  ساکن constexpr غیرعلامتی max_len = 1;

  ساکن اندرونی ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *انتہا HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *متن++;
    اگر (*unicode >= 0x0080u)
      *unicode = replacement;
    واپس متن;
  }

  ساکن اندرونی ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *آغاز HB_غیر_مستعمل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--متن;
    اگر (*unicode >= 0x0080u)
      *unicode = replacement;
    واپس متن;
  }

  ساکن غیرعلامتی صحیح
  لڑی_لمبائی (ثابت codepoint_t *متن)
  {
    غیرعلامتی صحیح l = 0;
    جب (*متن++) l++;
    واپس l;
  }

  ساکن غیرعلامتی صحیح
  encode_len (hb_codepoint_t unicode HB_غیر_مستعمل)
  {
    واپس 1;
  }

  ساکن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *انتہا HB_غیر_مستعمل,
	  hb_codepoint_t unicode)
  {
    اگر (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *متن++ = unicode;
    واپس متن;
  }
};

سانچہ <قسم_نام utf_t>
ساکن اندرونی ثابت قسم_نام utf_t::codepoint_t *
hb_utf_انحراف_مقصد_اشارہ_گر (ثابت قسم_نام utf_t::codepoint_t *آغاز,
			  ثابت قسم_نام utf_t::codepoint_t *متن,
			  غیرعلامتی متن_len,
			  علامتی انحراف)
{
  hb_codepoint_t unicode;

  جب (انحراف-- > 0)
    آغاز = utf_t::next (آغاز,
			 متن + متن_len,
			 &unicode,
			 HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT);

  جب (انحراف++ < 0)
    آغاز = utf_t::prev (آغاز,
			 متن,
			 &unicode,
			 HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT);

  واپس آغاز;
}


#اگر_ختم /* HB_UTF_HH */
