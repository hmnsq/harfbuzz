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

#جيڪڏهن_وصف_نه_ٿيل HB_UTF_HH
#وصف_ڪريو HB_UTF_HH

#ضمن "hb.hh"

#ضمن "hb-open-type.hh"


بنية hb_utf8_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr بنانشان max_len = 4;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *پڇاڙي,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    /* Written to only accept well-formed sequences.
     * Based on ideas from ICU's U8_NEXT.
     * Generates one "replacement" for each ill-formed byte. */

    hb_codepoint_t c = *متن++;

    إذا (c > 0x7Fu)
    {
      إذا (hb_اندريون_range<hb_codepoint_t> (c, 0xC2u, 0xDFu)) /* Two-byte */
      {
	بنانشان صحيح t1;
	إذا (likely (متن < پڇاڙي &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x1Fu)<<6) | t1;
	  متن++;
	}
	وإلا
	  اذهب error;
      }
      وإلا إذا (hb_اندريون_range<hb_codepoint_t> (c, 0xE0u, 0xEFu)) /* Three-byte */
      {
	بنانشان صحيح t1, t2;
	إذا (likely (1 < پڇاڙي - متن &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu &&
		    (t2 = متن[1] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0xFu)<<12) | (t1<<6) | t2;
	  إذا (unlikely (c < 0x0800u || hb_اندريون_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
	    اذهب error;
	  متن += 2;
	}
	وإلا
	  اذهب error;
      }
      وإلا إذا (hb_اندريون_range<hb_codepoint_t> (c, 0xF0u, 0xF4u)) /* Four-byte */
      {
	بنانشان صحيح t1, t2, t3;
	إذا (likely (2 < پڇاڙي - متن &&
		    (t1 = متن[0] - 0x80u) <= 0x3Fu &&
		    (t2 = متن[1] - 0x80u) <= 0x3Fu &&
		    (t3 = متن[2] - 0x80u) <= 0x3Fu))
	{
	  c = ((c&0x7u)<<18) | (t1<<12) | (t2<<6) | t3;
	  إذا (unlikely (!hb_اندريون_range<hb_codepoint_t> (c, 0x10000u, 0x10FFFFu)))
	    اذهب error;
	  متن += 3;
	}
	وإلا
	  اذهب error;
      }
      وإلا
	اذهب error;
    }

    *unicode = c;
    أرجع متن;

  error:
    *unicode = replacement;
    أرجع متن;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *شروعات,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    ثابت codepoint_t *پڇاڙي = متن--;
    طالما (شروعات < متن && (*متن & 0xc0) == 0x80 && پڇاڙي - متن < 4)
      متن--;

    إذا (likely (next (متن, پڇاڙي, unicode, replacement) == پڇاڙي))
      أرجع متن;

    *unicode = replacement;
    أرجع پڇاڙي - 1;
  }

  ساكن بنانشان صحيح
  لڙي_ڊيگهه (ثابت codepoint_t *متن)
  { أرجع ::لڙي_ڊيگهه ((ثابت حرف *) متن); }

  ساكن بنانشان صحيح
  encode_len (hb_codepoint_t unicode)
  {
    إذا (unicode <   0x0080u) أرجع 1;
    إذا (unicode <   0x0800u) أرجع 2;
    إذا (unicode <  0x10000u) أرجع 3;
    إذا (unicode < 0x110000u) أرجع 4;
    أرجع 3;
  }

  ساكن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *پڇاڙي,
	  hb_codepoint_t unicode)
  {
    إذا (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    إذا (unicode < 0x0080u)
     *متن++ = unicode;
    وإلا إذا (unicode < 0x0800u)
    {
      إذا (پڇاڙي - متن >= 2)
      {
	*متن++ =  0xC0u + (0x1Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    وإلا إذا (unicode < 0x10000u)
    {
      إذا (پڇاڙي - متن >= 3)
      {
	*متن++ =  0xE0u + (0x0Fu & (unicode >> 12));
	*متن++ =  0x80u + (0x3Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    وإلا
    {
      إذا (پڇاڙي - متن >= 4)
      {
	*متن++ =  0xF0u + (0x07u & (unicode >> 18));
	*متن++ =  0x80u + (0x3Fu & (unicode >> 12));
	*متن++ =  0x80u + (0x3Fu & (unicode >>  6));
	*متن++ =  0x80u + (0x3Fu & (unicode      ));
      }
    }
    أرجع متن;
  }
};


سانچو <قسم_نالو TCodepoint>
بنية hb_utf16_xe_t
{
  static_assert (حجم (TCodepoint) == 2, "");
  تعريفنوع TCodepoint codepoint_t;
  ساكن constexpr بنانشان max_len = 2;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *پڇاڙي,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *متن++;

    إذا (likely (!hb_اندريون_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      أرجع متن;
    }

    إذا (likely (c <= 0xDBFFu && متن < پڇاڙي))
    {
      /* High-surrogate in c */
      hb_codepoint_t l = *متن;
      إذا (likely (hb_اندريون_range<hb_codepoint_t> (l, 0xDC00u, 0xDFFFu)))
      {
	/* Low-surrogate in l */
	*unicode = (c << 10) + l - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	 متن++;
	 أرجع متن;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    أرجع متن;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *شروعات,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *--متن;

    إذا (likely (!hb_اندريون_range<hb_codepoint_t> (c, 0xD800u, 0xDFFFu)))
    {
      *unicode = c;
      أرجع متن;
    }

    إذا (likely (c >= 0xDC00u && شروعات < متن))
    {
      /* Low-surrogate in c */
      hb_codepoint_t h = متن[-1];
      إذا (likely (hb_اندريون_range<hb_codepoint_t> (h, 0xD800u, 0xDBFFu)))
      {
	/* High-surrogate in h */
	*unicode = (h << 10) + c - ((0xD800u << 10) - 0x10000u + 0xDC00u);
	متن--;
	أرجع متن;
      }
    }

    /* Lonely / out-of-order surrogate. */
    *unicode = replacement;
    أرجع متن;
  }


  ساكن بنانشان صحيح
  لڙي_ڊيگهه (ثابت codepoint_t *متن)
  {
    بنانشان صحيح l = 0;
    طالما (*متن++) l++;
    أرجع l;
  }

  ساكن بنانشان صحيح
  encode_len (hb_codepoint_t unicode)
  {
    أرجع unicode < 0x10000 ? 1 : 2;
  }

  ساكن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *پڇاڙي,
	  hb_codepoint_t unicode)
  {
    إذا (unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    إذا (unicode < 0x10000u)
     *متن++ = unicode;
    وإلا إذا (پڇاڙي - متن >= 2)
    {
      unicode -= 0x10000u;
      *متن++ =  0xD800u + (unicode >> 10);
      *متن++ =  0xDC00u + (unicode & 0x03FFu);
    }
    أرجع متن;
  }
};

تعريفنوع hb_utf16_xe_t<uint16_t> hb_utf16_t;
تعريفنوع hb_utf16_xe_t<OT::HBUINT16> hb_utf16_be_t;


سانچو <قسم_نالو TCodepoint, منطقي validate=سچ>
بنية hb_utf32_xe_t
{
  static_assert (حجم (TCodepoint) == 4, "");
  تعريفنوع TCodepoint codepoint_t;
  ساكن constexpr بنانشان max_len = 1;

  ساكن مضمن ثابت TCodepoint *
  next (ثابت TCodepoint *متن,
	ثابت TCodepoint *پڇاڙي HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *متن++;
    إذا (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    أرجع متن;
  }

  ساكن مضمن ثابت TCodepoint *
  prev (ثابت TCodepoint *متن,
	ثابت TCodepoint *شروعات HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    hb_codepoint_t c = *unicode = *--متن;
    إذا (validate && unlikely (c >= 0xD800u && (c <= 0xDFFFu || c > 0x10FFFFu)))
      *unicode = replacement;
    أرجع متن;
  }

  ساكن بنانشان صحيح
  لڙي_ڊيگهه (ثابت TCodepoint *متن)
  {
    بنانشان صحيح l = 0;
    طالما (*متن++) l++;
    أرجع l;
  }

  ساكن بنانشان صحيح
  encode_len (hb_codepoint_t unicode HB_اڻ_استعمال_ٿيل)
  {
    أرجع 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *پڇاڙي HB_اڻ_استعمال_ٿيل,
	  hb_codepoint_t unicode)
  {
    إذا (validate && unlikely (unicode >= 0xD800u && (unicode <= 0xDFFFu || unicode > 0x10FFFFu)))
      unicode = 0xFFFDu;
    *متن++ = unicode;
    أرجع متن;
  }
};

تعريفنوع hb_utf32_xe_t<uint32_t> hb_utf32_t;
تعريفنوع hb_utf32_xe_t<uint32_t, خطأ> hb_utf32_novalidate_t;


بنية hb_latin1_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr بنانشان max_len = 1;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *پڇاڙي HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_اڻ_استعمال_ٿيل)
  {
    *unicode = *متن++;
    أرجع متن;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *شروعات HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement HB_اڻ_استعمال_ٿيل)
  {
    *unicode = *--متن;
    أرجع متن;
  }

  ساكن بنانشان صحيح
  لڙي_ڊيگهه (ثابت codepoint_t *متن)
  {
    بنانشان صحيح l = 0;
    طالما (*متن++) l++;
    أرجع l;
  }

  ساكن بنانشان صحيح
  encode_len (hb_codepoint_t unicode HB_اڻ_استعمال_ٿيل)
  {
    أرجع 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *پڇاڙي HB_اڻ_استعمال_ٿيل,
	  hb_codepoint_t unicode)
  {
    إذا (unlikely (unicode >= 0x0100u))
      unicode = '?';
    *متن++ = unicode;
    أرجع متن;
  }
};


بنية hb_ascii_t
{
  تعريفنوع uint8_t codepoint_t;
  ساكن constexpr بنانشان max_len = 1;

  ساكن مضمن ثابت codepoint_t *
  next (ثابت codepoint_t *متن,
	ثابت codepoint_t *پڇاڙي HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *متن++;
    إذا (*unicode >= 0x0080u)
      *unicode = replacement;
    أرجع متن;
  }

  ساكن مضمن ثابت codepoint_t *
  prev (ثابت codepoint_t *متن,
	ثابت codepoint_t *شروعات HB_اڻ_استعمال_ٿيل,
	hb_codepoint_t *unicode,
	hb_codepoint_t replacement)
  {
    *unicode = *--متن;
    إذا (*unicode >= 0x0080u)
      *unicode = replacement;
    أرجع متن;
  }

  ساكن بنانشان صحيح
  لڙي_ڊيگهه (ثابت codepoint_t *متن)
  {
    بنانشان صحيح l = 0;
    طالما (*متن++) l++;
    أرجع l;
  }

  ساكن بنانشان صحيح
  encode_len (hb_codepoint_t unicode HB_اڻ_استعمال_ٿيل)
  {
    أرجع 1;
  }

  ساكن codepoint_t *
  encode (codepoint_t *متن,
	  ثابت codepoint_t *پڇاڙي HB_اڻ_استعمال_ٿيل,
	  hb_codepoint_t unicode)
  {
    إذا (unlikely (unicode >= 0x0080u))
      unicode = '?';
    *متن++ = unicode;
    أرجع متن;
  }
};

سانچو <قسم_نالو utf_t>
ساكن مضمن ثابت قسم_نالو utf_t::codepoint_t *
hb_utf_وٿي_منزل_اشارو (ثابت قسم_نالو utf_t::codepoint_t *شروعات,
			  ثابت قسم_نالو utf_t::codepoint_t *متن,
			  بنانشان متن_len,
			  نشانوارو وٿي)
{
  hb_codepoint_t unicode;

  طالما (وٿي-- > 0)
    شروعات = utf_t::next (شروعات,
			 متن + متن_len,
			 &unicode,
			 HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT);

  طالما (وٿي++ < 0)
    شروعات = utf_t::prev (شروعات,
			 متن,
			 &unicode,
			 HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT);

  أرجع شروعات;
}


#جيڪڏهن_پورو /* HB_UTF_HH */
