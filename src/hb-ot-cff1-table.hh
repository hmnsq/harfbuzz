/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#اگر_غیر_تعریف_شدہ HB_OT_CFF1_جدول_HH
#تعریف_کرو HB_OT_CFF1_جدول_HH

#شامل "hb-ot-cff-common.hh"
#شامل "hb-depend-data.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-draw.hh"
#شامل "hb-paint.hh"

#تعریف_کرو HB_STRING_ARRAY_NAME cff1_std_لڑیاں
#تعریف_کرو HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#شامل "hb-string-array.hh"
#تعریف_منسوخ HB_STRING_ARRAY_LIST
#تعریف_منسوخ HB_STRING_ARRAY_NAME

نامخلا CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#تعریف_کرو HB_OT_لیبل_CFF1 HB_لیبل('C','F','F',' ')

#تعریف_کرو CFF_UNDEF_SID   CFF_UNDEF_رمز

شمار EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
شمار CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

قسمتعریف CFF1_اشاریہ          CFF1_Char_لڑیاں;
قسمتعریف Subrs<HBUINT16>    CFF1Subrs;

ساخت CFF1FDSelect : FDSelect {};

/* Encoding */
ساخت Encoding0 {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_رمز (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    اگر (glyph < nCodes ())
    {
      واپس (hb_codepoint_t)codes[glyph];
    }
    ورنہ
      واپس CFF_UNDEF_رمز;
  }

  HBUINT8 &nCodes () { واپس codes.len; }
  HBUINT8 nCodes () ثابت { واپس codes.len; }

  Array_کا<HBUINT8, HBUINT8> codes;

  DEFINE_سائز_ARRAY_SIZED (1, codes);
};

ساخت Encoding1_Range {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT8   پہلا;
  HBUINT8   n_بایاں;

  DEFINE_سائز_STATIC (2);
};

ساخت Encoding1 {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_رمز (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    برائے (غیرعلامتی صحیح i = 0; i < nRanges (); i++)
    {
      اگر (glyph <= ranges[i].n_بایاں)
      {
	hb_codepoint_t رمز = (hb_codepoint_t) ranges[i].پہلا + glyph;
	واپس (likely (رمز < 0x100) ? رمز: CFF_UNDEF_رمز);
      }
      glyph -= (ranges[i].n_بایاں + 1);
    }
    واپس CFF_UNDEF_رمز;
  }

  HBUINT8 &nRanges () { واپس ranges.len; }
  HBUINT8 nRanges () ثابت { واپس ranges.len; }

  Array_کا<Encoding1_Range, HBUINT8> ranges;

  DEFINE_سائز_ARRAY_SIZED (1, ranges);
};

ساخت SuppEncoding {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT8   رمز;
  HBUINT16  glyph;

  DEFINE_سائز_STATIC (3);
};

ساخت CFF1_Supp_Enc_ڈیٹا {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (supps.sanitize (c));
  }

  خالی get_codes (hb_codepoint_t sid, hb_سمتیہ_t<hb_codepoint_t> &codes) ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < nSups (); i++)
      اگر (sid == supps[i].glyph)
	codes.push (supps[i].رمز);
  }

  HBUINT8 &nSups () { واپس supps.len; }
  HBUINT8 nSups () ثابت { واپس supps.len; }

  Array_کا<SuppEncoding, HBUINT8> supps;

  DEFINE_سائز_ARRAY_SIZED (1, supps);
};

ساخت Encoding
{
  /* serialize a fullset Encoding */
  منطقی serialize (hb_serialize_context_t *c, ثابت Encoding &src)
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  منطقی serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  غیرعلامتی صحیح enc_گنتی,
		  ثابت hb_سمتیہ_t<رمز_pair_t>& رمز_ranges,
		  ثابت hb_سمتیہ_t<رمز_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (یہ);
    Encoding *dest = c->extend_min (یہ);
    اگر (unlikely (!dest)) return_trace (جھوٹ);
    dest->format = format | ((supp_codes.لمبائی > 0) ? 0x80 : 0);
    انتخاب (format) {
    حالت 0:
    {
      Encoding0 *fmt0 = c->allocate_سائز<Encoding0> (Encoding0::min_سائز + HBUINT8::static_سائز * enc_گنتی);
      اگر (unlikely (!fmt0)) return_trace (جھوٹ);
      fmt0->nCodes () = enc_گنتی;
      غیرعلامتی صحیح glyph = 0;
      برائے (غیرعلامتی صحیح i = 0; i < رمز_ranges.لمبائی; i++)
      {
	hb_codepoint_t رمز = رمز_ranges[i].رمز;
	برائے (صحیح بائیں = (صحیح)رمز_ranges[i].glyph; بائیں >= 0; بائیں--)
	  fmt0->codes[glyph++] = رمز++;
	اگر (unlikely (!((glyph <= 0x100) && (رمز <= 0x100))))
	  return_trace (جھوٹ);
      }
    }
    توڑو;

    حالت 1:
    {
      Encoding1 *fmt1 = c->allocate_سائز<Encoding1> (Encoding1::min_سائز + Encoding1_Range::static_سائز * رمز_ranges.لمبائی);
      اگر (unlikely (!fmt1)) return_trace (جھوٹ);
      fmt1->nRanges () = رمز_ranges.لمبائی;
      برائے (غیرعلامتی صحیح i = 0; i < رمز_ranges.لمبائی; i++)
      {
	اگر (unlikely (!((رمز_ranges[i].رمز <= 0xFF) && (رمز_ranges[i].glyph <= 0xFF))))
	  return_trace (جھوٹ);
	fmt1->ranges[i].پہلا = رمز_ranges[i].رمز;
	fmt1->ranges[i].n_بایاں = رمز_ranges[i].glyph;
      }
    }
    توڑو;

    }

    اگر (supp_codes.لمبائی)
    {
      CFF1_Supp_Enc_ڈیٹا *supp_ڈیٹا = c->allocate_سائز<CFF1_Supp_Enc_ڈیٹا> (CFF1_Supp_Enc_ڈیٹا::min_سائز + SuppEncoding::static_سائز * supp_codes.لمبائی);
      اگر (unlikely (!supp_ڈیٹا)) return_trace (جھوٹ);
      supp_ڈیٹا->nSups () = supp_codes.لمبائی;
      برائے (غیرعلامتی صحیح i = 0; i < supp_codes.لمبائی; i++)
      {
	supp_ڈیٹا->supps[i].رمز = supp_codes[i].رمز;
	supp_ڈیٹا->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (سچ);
  }

  size_t get_سائز () ثابت
  {
    size_t سائز = min_سائز;
    انتخاب (جدول_format ())
    {
    حالت 0: hb_barrier (); سائز = hb_unsigned_جوڑو_saturate (سائز, u.format0.get_سائز ()); توڑو;
    حالت 1: hb_barrier (); سائز = hb_unsigned_جوڑو_saturate (سائز, u.format1.get_سائز ()); توڑو;
    }
    اگر (has_supplement ())
      سائز = hb_unsigned_جوڑو_saturate (سائز, supp_Enc_ڈیٹا ().get_سائز ());
    واپس سائز;
  }

  hb_codepoint_t get_رمز (hb_codepoint_t glyph) ثابت
  {
    انتخاب (جدول_format ())
    {
    حالت 0: hb_barrier (); واپس u.format0.get_رمز (glyph);
    حالت 1: hb_barrier (); واپس u.format1.get_رمز (glyph);
    طےشدہ:واپس 0;
    }
  }

  uint8_t جدول_format () ثابت { واپس format & 0x7F; }
  منطقی  has_supplement () ثابت { واپس format & 0x80; }

  خالی get_supplement_codes (hb_codepoint_t sid, hb_سمتیہ_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    اگر (has_supplement ())
      supp_Enc_ڈیٹا().get_codes (sid, codes);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (جدول_format ())
    {
    حالت 0: hb_barrier (); اگر (unlikely (!u.format0.sanitize (c))) { return_trace (جھوٹ); } توڑو;
    حالت 1: hb_barrier (); اگر (unlikely (!u.format1.sanitize (c))) { return_trace (جھوٹ); } توڑو;
    طےشدہ:return_trace (جھوٹ);
    }
    return_trace (likely (!has_supplement () || supp_Enc_ڈیٹا ().sanitize (c)));
  }

  محفوظ:
  ثابت CFF1_Supp_Enc_ڈیٹا &supp_Enc_ڈیٹا () ثابت
  {
    انتخاب (جدول_format ())
    {
    حالت 0: hb_barrier (); واپس StructAfter<CFF1_Supp_Enc_ڈیٹا> (u.format0.codes[u.format0.nCodes ()-1]);
    حالت 1: hb_barrier (); واپس StructAfter<CFF1_Supp_Enc_ڈیٹا> (u.format1.ranges[u.format1.nRanges ()-1]);
    طےشدہ:واپس Null (CFF1_Supp_Enc_ڈیٹا);
    }
  }

  عوامی:
  HBUINT8	format;
  اتحاد {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_سائز_MIN (1);
};

/* Charset */
ساخت Charset0
{
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح عدد_glyphs, غیرعلامتی *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (عدد_charset_entries) *عدد_charset_entries = عدد_glyphs;
    return_trace (sids.sanitize (c, عدد_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, غیرعلامتی عدد_glyphs) ثابت
  {
    اگر (unlikely (glyph >= عدد_glyphs)) واپس 0;
    اگر (unlikely (glyph == 0))
      واپس 0;
    ورنہ
      واپس sids[glyph - 1];
  }

  خالی collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    اگر (!mapping->resize_dirty  (عدد_glyphs)) واپس;
    برائے (hb_codepoint_t gid = 1; gid < عدد_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    اگر (sid == 0)
      واپس 0;

    برائے (غیرعلامتی صحیح glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      اگر (sids[glyph-1] == sid)
	واپس glyph;
    }
    واپس 0;
  }

  ساکن size_t get_سائز (غیرعلامتی صحیح عدد_glyphs)
  {
    أكد (عدد_glyphs > 0);
    واپس Unsized_Array_کا<HBUINT16>::get_سائز (عدد_glyphs - 1);
  }

  Unsized_Array_کا<HBUINT16> sids;

  DEFINE_سائز_ARRAY(0, sids);
};

سانچہ <قسم_نام TYPE>
ساخت Charset_Range {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT16  پہلا;
  TYPE      n_بایاں;

  DEFINE_سائز_STATIC (HBUINT16::static_سائز + TYPE::static_سائز);
};

سانچہ <قسم_نام TYPE>
ساخت Charset1_2 {
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح عدد_glyphs, غیرعلامتی *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (یہ);
    عدد_glyphs--;
    غیرعلامتی i;
    برائے (i = 0; عدد_glyphs > 0; i++)
    {
      اگر (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (عدد_glyphs >= ranges[i].n_بایاں + 1))))
	return_trace (جھوٹ);
      عدد_glyphs -= (ranges[i].n_بایاں + 1);
    }
    اگر (عدد_charset_entries)
      *عدد_charset_entries = i;
    return_trace (سچ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, غیرعلامتی عدد_glyphs,
			  رمز_pair_t *cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (glyph >= عدد_glyphs)) واپس 0;
    غیرعلامتی i;
    hb_codepoint_t آغاز_glyph;
    اگر (cache && likely (cache->glyph <= glyph))
    {
      i = cache->رمز;
      آغاز_glyph = cache->glyph;
    }
    ورنہ
    {
      اگر (unlikely (glyph == 0)) واپس 0;
      i = 0;
      آغاز_glyph = 1;
    }
    glyph -= آغاز_glyph;
    برائے (;; i++)
    {
      غیرعلامتی گنتی = ranges[i].n_بایاں;
      اگر (glyph <= گنتی)
      {
        اگر (cache)
	  *cache = {i, آغاز_glyph};
	واپس ranges[i].پہلا + glyph;
      }
      گنتی++;
      آغاز_glyph += گنتی;
      glyph -= گنتی;
    }

    واپس 0;
  }

  خالی collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    اگر (!mapping->resize_dirty  (عدد_glyphs)) واپس;

    hb_codepoint_t gid = 1;
    اگر (gid >= عدد_glyphs)
      واپس;

    برائے (غیرعلامتی i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].پہلا;
      غیرعلامتی گنتی = ranges[i].n_بایاں + 1;
      غیرعلامتی آخری = gid + گنتی;
      برائے (غیرعلامتی j = 0; j < گنتی; j++)
	mapping->arrayZ[gid++] = {sid++, آخری - 1};

      اگر (gid >= عدد_glyphs)
        توڑو;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    اگر (sid == 0) واپس 0;
    hb_codepoint_t  glyph = 1;
    برائے (غیرعلامتی صحیح i = 0;; i++)
    {
      اگر (glyph >= عدد_glyphs)
	واپس 0;
      اگر ((ranges[i].پہلا <= sid) && (sid <= ranges[i].پہلا + ranges[i].n_بایاں))
	واپس glyph + (sid - ranges[i].پہلا);
      glyph += (ranges[i].n_بایاں + 1);
    }

    واپس 0;
  }

  size_t get_سائز (غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    صحیح glyph = (صحیح) عدد_glyphs;
    غیرعلامتی عدد_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    برائے (غیرعلامتی صحیح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_بایاں + 1);
      عدد_ranges++;
    }

    واپس get_سائز_for_ranges (عدد_ranges);
  }

  ساکن size_t get_سائز_for_ranges (غیرعلامتی صحیح عدد_ranges)
  {
    واپس Unsized_Array_کا<Charset_Range<TYPE> >::get_سائز (عدد_ranges);
  }

  Unsized_Array_کا<Charset_Range<TYPE>> ranges;

  DEFINE_سائز_ARRAY (0, ranges);
};

قسمتعریف Charset1_2<HBUINT8>     Charset1;
قسمتعریف Charset1_2<HBUINT16>    Charset2;
قسمتعریف Charset_Range<HBUINT8>  Charset1_Range;
قسمتعریف Charset_Range<HBUINT16> Charset2_Range;

ساخت Charset
{
  /* serialize a fullset Charset */
  منطقی serialize (hb_serialize_context_t *c, ثابت Charset &src, غیرعلامتی صحیح عدد_glyphs)
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_سائز (عدد_glyphs)));
  }

  /* serialize a subset Charset */
  منطقی serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  غیرعلامتی صحیح عدد_glyphs,
		  ثابت hb_سمتیہ_t<رمز_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (یہ);
    Charset *dest = c->extend_min (یہ);
    اگر (unlikely (!dest)) return_trace (جھوٹ);
    dest->format = format;
    انتخاب (format)
    {
    حالت 0:
    {
      Charset0 *fmt0 = c->allocate_سائز<Charset0> (Charset0::get_سائز (عدد_glyphs), جھوٹ);
      اگر (unlikely (!fmt0)) return_trace (جھوٹ);
      غیرعلامتی صحیح glyph = 0;
      برائے (غیرعلامتی صحیح i = 0; i < sid_ranges.لمبائی; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].رمز;
	برائے (صحیح بائیں = (صحیح)sid_ranges.arrayZ[i].glyph; بائیں >= 0; بائیں--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    توڑو;

    حالت 1:
    {
      Charset1 *fmt1 = c->allocate_سائز<Charset1> (Charset1::get_سائز_for_ranges (sid_ranges.لمبائی), جھوٹ);
      اگر (unlikely (!fmt1)) return_trace (جھوٹ);
      hb_codepoint_t all_glyphs = 0;
      برائے (غیرعلامتی صحیح i = 0; i < sid_ranges.لمبائی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].پہلا = _.رمز;
	fmt1->ranges[i].n_بایاں = _.glyph;
      }
      اگر (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (جھوٹ);
    }
    توڑو;

    حالت 2:
    {
      Charset2 *fmt2 = c->allocate_سائز<Charset2> (Charset2::get_سائز_for_ranges (sid_ranges.لمبائی), جھوٹ);
      اگر (unlikely (!fmt2)) return_trace (جھوٹ);
      hb_codepoint_t all_glyphs = 0;
      برائے (غیرعلامتی صحیح i = 0; i < sid_ranges.لمبائی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].پہلا = _.رمز;
	fmt2->ranges[i].n_بایاں = _.glyph;
      }
      اگر (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (جھوٹ);
    }
    توڑو;

    }
    return_trace (سچ);
  }

  size_t get_سائز (غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس hb_unsigned_جوڑو_saturate (min_سائز, u.format0.get_سائز (عدد_glyphs));
    حالت 1: hb_barrier (); واپس hb_unsigned_جوڑو_saturate (min_سائز, u.format1.get_سائز (عدد_glyphs));
    حالت 2: hb_barrier (); واپس hb_unsigned_جوڑو_saturate (min_سائز, u.format2.get_سائز (عدد_glyphs));
    طےشدہ:واپس 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, غیرعلامتی صحیح عدد_glyphs,
			  رمز_pair_t *cache = خالی_اشاریہ) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_sid (glyph, عدد_glyphs);
    حالت 1: hb_barrier (); واپس u.format1.get_sid (glyph, عدد_glyphs, cache);
    حالت 2: hb_barrier (); واپس u.format2.get_sid (glyph, عدد_glyphs, cache);
    طےشدہ:واپس 0;
    }
  }

  خالی collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); u.format0.collect_glyph_مقصد_sid_map (mapping, عدد_glyphs); واپس;
    حالت 1: hb_barrier (); u.format1.collect_glyph_مقصد_sid_map (mapping, عدد_glyphs); واپس;
    حالت 2: hb_barrier (); u.format2.collect_glyph_مقصد_sid_map (mapping, عدد_glyphs); واپس;
    طےشدہ:واپس;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_glyph (sid, عدد_glyphs);
    حالت 1: hb_barrier (); واپس u.format1.get_glyph (sid, عدد_glyphs);
    حالت 2: hb_barrier (); واپس u.format2.get_glyph (sid, عدد_glyphs);
    طےشدہ:واپس 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  HBUINT8       format;
  اتحاد {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_سائز_MIN (1);
};

ساخت CFF1_String_اشاریہ : CFF1_اشاریہ
{
  منطقی serialize (hb_serialize_context_t *c, ثابت CFF1_String_اشاریہ &لڑیاں,
		  ثابت hb_سمتیہ_t<غیرعلامتی> &sidmap)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely ((لڑیاں.گنتی == 0) || (sidmap.لمبائی == 0)))
    {
      اگر (unlikely (!c->extend_min (یہ->گنتی)))
	return_trace (جھوٹ);
      گنتی = 0;
      return_trace (سچ);
    }

    اگر (unlikely (sidmap.اندر_error ())) return_trace (جھوٹ);

    // Save this in a vector since serialize() iterates it twice.
    hb_سمتیہ_t<hb_ubytes_t> بائٹیں_Array (+ hb_iter (sidmap)
					 | hb_map (لڑیاں));

    اگر (unlikely (بائٹیں_Array.اندر_error ())) return_trace (جھوٹ);

    منطقی نتیجہ = CFF1_اشاریہ::serialize (c, بائٹیں_Array);
    return_trace (نتیجہ);
  }
};

ساخت cff1_چوٹی_dict_interp_env_t : عدد_interp_env_t
{
  cff1_چوٹی_dict_interp_env_t ()
    : عدد_interp_env_t(), پچھلا_انحراف(0), آخری_انحراف(0) {}
  cff1_چوٹی_dict_interp_env_t (ثابت hb_ubytes_t &بائٹیں)
    : عدد_interp_env_t(بائٹیں), پچھلا_انحراف(0), آخری_انحراف(0) {}

  غیرعلامتی صحیح پچھلا_انحراف;
  غیرعلامتی صحیح آخری_انحراف;
};

ساخت name_dict_اقدار_t
{
  شمار name_dict_val_اشاریہ_t
  {
      نسخہ,
      notice,
      copyright,
      بھرا_Name,
      familyName,
      وزن,
      postscript,
      fontName,
      بنیاد_Font_Name,
      registry,
      ordering,

      Val_گنتی
  };

  خالی init ()
  {
    برائے (غیرعلامتی صحیح i = 0; i < Val_گنتی; i++)
      values[i] = CFF_UNDEF_SID;
  }

  غیرعلامتی صحیح& عامل[] (غیرعلامتی صحیح i)
  { أكد (i < Val_گنتی); واپس values[i]; }

  غیرعلامتی صحیح عامل[] (غیرعلامتی صحیح i) ثابت
  { أكد (i < Val_گنتی); واپس values[i]; }

  ساکن شمار name_dict_val_اشاریہ_t name_op_مقصد_اشاریہ (op_رمز_t op)
  {
    انتخاب (op) {
      طےشدہ: // can't happen - just make some compiler happy
      حالت Op_رمز_نسخہ:
	واپس نسخہ;
      حالت Op_رمز_Notice:
	واپس notice;
      حالت Op_رمز_Copyright:
	واپس copyright;
      حالت Op_رمز_بھرا_Name:
	واپس بھرا_Name;
      حالت Op_رمز_Family_Name:
	واپس familyName;
      حالت Op_رمز_وزن:
	واپس وزن;
      حالت Op_رمز_Post_Script:
	واپس postscript;
      حالت Op_رمز_Font_Name:
	واپس fontName;
      حالت Op_رمز_بنیاد_Font_Name:
	واپس بنیاد_Font_Name;
    }
  }

  غیرعلامتی صحیح  values[Val_گنتی];
};

ساخت cff1_چوٹی_dict_val_t : op_str_t
{
  غیرعلامتی صحیح  آخری_arg_انحراف;
};

ساخت cff1_چوٹی_dict_اقدار_t : چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>
{
  خالی init ()
  {
    چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_گنتی = 8720;
    Encoding_انحراف = 0;
    Charset_انحراف = 0;
    FDSelect_انحراف = 0;
    نجی_Dict_Info.init ();
  }
  خالی fini () { چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>::fini (); }

  منطقی is_CID () ثابت
  { واپس nameSIDs[name_dict_اقدار_t::registry] != CFF_UNDEF_SID; }

  name_dict_اقدار_t  nameSIDs;
  غیرعلامتی صحیح    ros_supplement_انحراف;
  غیرعلامتی صحیح    ros_supplement;
  غیرعلامتی صحیح    cid_گنتی;

  صحیح             Encoding_انحراف;
  صحیح             Charset_انحراف;
  صحیح             FDSelect_انحراف;
  جدول_info_t    نجی_Dict_Info;
};

ساخت cff1_چوٹی_dict_opset_t : چوٹی_dict_opset_t<cff1_چوٹی_dict_val_t>
{
  ساکن خالی process_op (op_رمز_t op, cff1_چوٹی_dict_interp_env_t& env, cff1_چوٹی_dict_اقدار_t& dictval)
  {
    cff1_چوٹی_dict_val_t  val {};

    انتخاب (op) {
      حالت Op_رمز_نسخہ:
      حالت Op_رمز_Notice:
      حالت Op_رمز_Copyright:
      حالت Op_رمز_بھرا_Name:
      حالت Op_رمز_Font_Name:
      حالت Op_رمز_Family_Name:
      حالت Op_رمز_وزن:
      حالت Op_رمز_Post_Script:
      حالت Op_رمز_بنیاد_Font_Name:
	dictval.nameSIDs[name_dict_اقدار_t::name_op_مقصد_اشاریہ (op)] = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_is_Fixed_Pitch:
      حالت Op_رمز_Italic_Angle:
      حالت Op_رمز_Underline_Position:
      حالت Op_رمز_Underline_Thickness:
      حالت Op_رمز_Paint_Type:
      حالت Op_رمز_Charstring_Type:
      حالت Op_رمز_Unique_ID:
      حالت Op_رمز_Stroke_چوڑائی:
      حالت Op_رمز_Synthetic_بنیاد:
      حالت Op_رمز_CIDFont_نسخہ:
      حالت Op_رمز_CIDFont_Revision:
      حالت Op_رمز_CIDFont_Type:
      حالت Op_رمز_UIDBase:
      حالت Op_رمز_Font_BBox:
      حالت Op_رمز_XUID:
      حالت Op_رمز_بنیاد_Font_Blend:
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_CIDCount:
	dictval.cid_گنتی = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_ROS:
	dictval.ros_supplement = env.arg_ڈھیر.نکالو_uint ();
	dictval.nameSIDs[name_dict_اقدار_t::ordering] = env.arg_ڈھیر.نکالو_uint ();
	dictval.nameSIDs[name_dict_اقدار_t::registry] = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	اگر (unlikely (env.اندر_error ())) واپس;
	أكد (env.آخری_انحراف > dictval.op_آغاز);
	val.آخری_arg_انحراف = (env.آخری_انحراف - 1) - dictval.op_آغاز;  /* offset to the last argument */
	توڑو;

      حالت Op_رمز_Encoding:
	dictval.Encoding_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	اگر (unlikely (dictval.Encoding_انحراف == 0)) واپس;
	توڑو;

      حالت Op_رمز_charset:
	dictval.Charset_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	اگر (unlikely (dictval.Charset_انحراف == 0)) واپس;
	توڑو;

      حالت Op_رمز_FDSelect:
	dictval.FDSelect_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_نجی:
	dictval.نجی_Dict_Info.انحراف = env.arg_ڈھیر.نکالو_int ();
	dictval.نجی_Dict_Info.سائز = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	env.آخری_انحراف = env.str_ref.get_انحراف ();
	چوٹی_dict_opset_t<cff1_چوٹی_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

ساخت cff1_font_dict_اقدار_t : dict_اقدار_t<op_str_t>
{
  خالی init ()
  {
    dict_اقدار_t<op_str_t>::init ();
    نجی_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  خالی fini () { dict_اقدار_t<op_str_t>::fini (); }

  جدول_info_t       نجی_Dict_Info;
  غیرعلامتی صحیح    fontName;
};

ساخت cff1_font_dict_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, عدد_interp_env_t& env, cff1_font_dict_اقدار_t& dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_Font_Name:
	dictval.fontName = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_Font_میٹرکس:
      حالت Op_رمز_Paint_Type:
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_نجی:
	dictval.نجی_Dict_Info.انحراف = env.arg_ڈھیر.نکالو_uint ();
	dictval.نجی_Dict_Info.سائز = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref);
  }
};

سانچہ <قسم_نام VAL>
ساخت cff1_نجی_dict_اقدار_بنیاد_t : dict_اقدار_t<VAL>
{
  خالی init ()
  {
    dict_اقدار_t<VAL>::init ();
    subrs_انحراف = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  خالی fini () { dict_اقدار_t<VAL>::fini (); }

  صحیح                 subrs_انحراف;
  ثابت CFF1Subrs    *localSubrs;
};

قسمتعریف cff1_نجی_dict_اقدار_بنیاد_t<op_str_t> cff1_نجی_dict_اقدار_subset_t;
قسمتعریف cff1_نجی_dict_اقدار_بنیاد_t<عدد_dict_val_t> cff1_نجی_dict_اقدار_t;

ساخت cff1_نجی_dict_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, عدد_interp_env_t& env, cff1_نجی_dict_اقدار_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    انتخاب (op) {
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Force_Bold:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Expansion_ضربی_عدد:
      حالت Op_رمز_initial_Random_بیج:
      حالت Op_رمز_default_چوڑائی_X:
      حالت Op_رمز_nominal_چوڑائی_X:
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_Subrs:
	dictval.subrs_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

ساخت cff1_نجی_dict_opset_subset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, عدد_interp_env_t& env, cff1_نجی_dict_اقدار_subset_t& dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Force_Bold:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Expansion_ضربی_عدد:
      حالت Op_رمز_initial_Random_بیج:
      حالت Op_رمز_default_چوڑائی_X:
      حالت Op_رمز_nominal_چوڑائی_X:
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_Subrs:
	dictval.subrs_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref);
  }
};

قسمتعریف dict_interpreter_t<cff1_چوٹی_dict_opset_t, cff1_چوٹی_dict_اقدار_t, cff1_چوٹی_dict_interp_env_t> cff1_چوٹی_dict_interpreter_t;
قسمتعریف dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_اقدار_t> cff1_font_dict_interpreter_t;

قسمتعریف CFF1_اشاریہ CFF1_Name_اشاریہ;
قسمتعریف CFF1_اشاریہ CFF1_چوٹی_Dict_اشاریہ;

ساخت cff1_font_dict_اقدار_mod_t
{
  cff1_font_dict_اقدار_mod_t() { init (); }

  خالی init () { init ( &Null (cff1_font_dict_اقدار_t), CFF_UNDEF_SID ); }

  خالی init (ثابت cff1_font_dict_اقدار_t *بنیاد_2,
	     غیرعلامتی صحیح fontName_)
  {
    بنیاد = بنیاد_2;
    fontName = fontName_;
    نجی_Dict_Info.init ();
  }

  غیرعلامتی get_گنتی () ثابت { واپس بنیاد->get_گنتی (); }

  ثابت op_str_t &عامل [] (غیرعلامتی صحیح i) ثابت { واپس (*بنیاد)[i]; }

  ثابت cff1_font_dict_اقدار_t    *بنیاد;
  جدول_info_t		   نجی_Dict_Info;
  غیرعلامتی صحیح		fontName;
};

ساخت CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  سانچہ <قسم_نام ITER, قسم_نام OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { واپس FDArray<HBUINT16>::serialize<cff1_font_dict_اقدار_mod_t, cff1_font_dict_اقدار_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نامخلا OT {

استعمال نامخلا CFF;

ساخت cff1
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_CFF1;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1));
  }

  سانچہ <قسم_نام PRIVOPSET, قسم_نام PRIVDICTVAL>
  ساخت accelerator_templ_t
  {
    ساکن constexpr hb_لیبل_t جدول_لیبل = cff1::جدول_لیبل;

    accelerator_templ_t (hb_face_t *face)
    {
      اگر (!face) واپس;

      چوٹی_Dict_2.init ();
      fontDicts.init ();
      نجی_Dicts.init ();

      یہ->blob = sc.reference_جدول<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (یہ->blob);
      sc.آغاز_processing ();

      ثابت OT::cff1 *cff = یہ->blob->سانچہ as<OT::cff1> ();

      اگر (cff == &Null (OT::cff1))
        جاؤ fail;

      name_اشاریہ = &cff->name_اشاریہ (cff);
      اگر ((name_اشاریہ == &Null (CFF1_Name_اشاریہ)) || !name_اشاریہ->sanitize (&sc))
        جاؤ fail;
      hb_barrier ();

      چوٹی_Dict_اشاریہ = &Struct_At_انحراف_Or_Null<CFF1_چوٹی_Dict_اشاریہ> (name_اشاریہ, name_اشاریہ->get_سائز (), sc);
      اگر (چوٹی_Dict_اشاریہ == &Null (CFF1_چوٹی_Dict_اشاریہ) || (چوٹی_Dict_اشاریہ->گنتی == 0))
        جاؤ fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t چوٹی_Dict_Str = (*چوٹی_Dict_اشاریہ)[0];
	اگر (unlikely (!چوٹی_Dict_Str.sanitize (&sc)))   جاؤ fail;
	hb_barrier ();
	cff1_چوٹی_dict_interp_env_t env (چوٹی_Dict_Str);
	cff1_چوٹی_dict_interpreter_t چوٹی_interp (env);
	اگر (unlikely (!چوٹی_interp.interpret (چوٹی_Dict_2)))   جاؤ fail;
      }

      اگر (is_predef_charset ())
	charset = &Null (Charset);
      ورنہ
      {
	charset = &Struct_At_انحراف_Or_Null<Charset> (cff, چوٹی_Dict_2.Charset_انحراف, sc, &عدد_charset_entries);
	اگر (unlikely (charset == &Null (Charset)))   جاؤ fail;
      }

      fd_گنتی = 1;
      اگر (is_CID ())
      {
	fdArray = &Struct_At_انحراف_Or_Null<CFF1FDArray> (cff, چوٹی_Dict_2.FDArray_انحراف, sc);
	fdSelect = &Struct_At_انحراف_Or_Null<CFF1FDSelect> (cff, چوٹی_Dict_2.FDSelect_انحراف, sc, fdArray->گنتی);
	اگر (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  جاؤ fail;

	fd_گنتی = fdArray->گنتی;
      }
      ورنہ
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      اگر (is_CID ())
      {
	اگر (unlikely (charset == &Null (Charset)))   جاؤ fail;
      }
      ورنہ
      {
	اگر (!is_predef_encoding ())
	{
	  encoding = &Struct_At_انحراف_Or_Null<Encoding> (cff, چوٹی_Dict_2.Encoding_انحراف, sc);
	  اگر (unlikely (encoding == &Null (Encoding)))   جاؤ fail;
	}
      }

      string_اشاریہ = &Struct_At_انحراف_Or_Null<CFF1_String_اشاریہ> (چوٹی_Dict_اشاریہ, چوٹی_Dict_اشاریہ->get_سائز (), sc);
      اگر (string_اشاریہ == &Null (CFF1_String_اشاریہ))
        جاؤ fail;

      globalSubrs = &Struct_At_انحراف_Or_Null<CFF1Subrs> (string_اشاریہ, string_اشاریہ->get_سائز (), sc);
      char_لڑیاں = &Struct_At_انحراف_Or_Null<CFF1_Char_لڑیاں> (cff, چوٹی_Dict_2.char_لڑیاں_انحراف, sc);
      اگر (char_لڑیاں == &Null (CFF1_Char_لڑیاں))
        جاؤ fail;

      عدد_glyphs = char_لڑیاں->گنتی;
      اگر (عدد_glyphs != sc.get_عدد_glyphs ())
        جاؤ fail;

      اگر (unlikely (!نجی_Dicts.resize (fd_گنتی)))
        جاؤ fail;
      برائے (غیرعلامتی صحیح i = 0; i < fd_گنتی; i++)
	نجی_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      اگر (is_CID ())
      {
	برائے (غیرعلامتی صحیح i = 0; i < fd_گنتی; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  اگر (unlikely (!fontDictStr.sanitize (&sc)))   جاؤ fail;
	  hb_barrier ();
	  cff1_font_dict_اقدار_t *font;
	  cff1_چوٹی_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  اگر (unlikely (fontDicts.اندر_error ()))   جاؤ fail;

	  font->init ();
	  اگر (unlikely (!font_interp.interpret (*font)))   جاؤ fail;
	  PRIVDICTVAL *priv = &نجی_Dicts[i];
	  ثابت hb_ubytes_t privDictStr = Struct_At_انحراف_Or_Null<UnsizedByteStr> (cff, font->نجی_Dict_Info.انحراف, sc, font->نجی_Dict_Info.سائز).as_ubytes (font->نجی_Dict_Info.سائز);
	  اگر (unlikely (font->نجی_Dict_Info.سائز &&
			privDictStr == (ثابت غیرعلامتی حرف *) &Null (UnsizedByteStr))) جاؤ fail;
	  عدد_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  اگر (unlikely (!priv_interp.interpret (*priv)))   جاؤ fail;

	  priv->localSubrs = &Struct_At_انحراف_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_انحراف, sc);
	}
      }
      ورنہ  /* non-CID */
      {
	cff1_چوٹی_dict_اقدار_t *font = &چوٹی_Dict_2;
	PRIVDICTVAL *priv = &نجی_Dicts[0];

	ثابت hb_ubytes_t privDictStr = Struct_At_انحراف_Or_Null<UnsizedByteStr> (cff, font->نجی_Dict_Info.انحراف, sc, font->نجی_Dict_Info.سائز).as_ubytes (font->نجی_Dict_Info.سائز);
	اگر (font->نجی_Dict_Info.سائز &&
	    unlikely (privDictStr == (ثابت غیرعلامتی حرف *) &Null (UnsizedByteStr))) جاؤ fail;
	عدد_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	اگر (unlikely (!priv_interp.interpret (*priv)))   جاؤ fail;

	priv->localSubrs = &Struct_At_انحراف_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_انحراف, sc);
	hb_barrier ();
      }

      واپس;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    خالی _fini ()
    {
      sc.انتہا_processing ();
      چوٹی_Dict_2.fini ();
      fontDicts.fini ();
      نجی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالی_اشاریہ;
    }

    hb_blob_t *get_blob () ثابت { واپس blob; }

    منطقی is_valid () ثابت { واپس blob; }
    منطقی   is_CID () ثابت { واپس چوٹی_Dict_2.is_CID (); }

    منطقی is_predef_charset () ثابت { واپس چوٹی_Dict_2.Charset_انحراف <= ExpertSubsetCharset; }

    غیرعلامتی صحیح std_رمز_مقصد_glyph (hb_codepoint_t رمز) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (رمز);
      اگر (unlikely (sid == CFF_UNDEF_SID))
	واپس 0;

      اگر (charset != &Null (Charset))
	واپس charset->get_glyph (sid, عدد_glyphs);
      ورنہ اگر ((چوٹی_Dict_2.Charset_انحراف == ISOAdobeCharset)
	      && (رمز <= 228 /*zcaron*/)) واپس sid;
      واپس 0;
    }

    منطقی is_predef_encoding () ثابت { واپس چوٹی_Dict_2.Encoding_انحراف <= ExpertEncoding; }

    hb_codepoint_t glyph_مقصد_رمز (hb_codepoint_t glyph,
				  رمز_pair_t *glyph_مقصد_sid_cache = خالی_اشاریہ) ثابت
    {
      اگر (encoding != &Null (Encoding))
	واپس encoding->get_رمز (glyph);
      ورنہ
      {
	hb_codepoint_t sid = glyph_مقصد_sid (glyph, glyph_مقصد_sid_cache);
	اگر (sid == 0) واپس 0;
	hb_codepoint_t رمز = 0;
	انتخاب (چوٹی_Dict_2.Encoding_انحراف)
	{
	حالت StandardEncoding:
	  رمز = lookup_standard_encoding_for_رمز (sid);
	  توڑو;
	حالت ExpertEncoding:
	  رمز = lookup_expert_encoding_for_رمز (sid);
	  توڑو;
	طےشدہ:
	  توڑو;
	}
	واپس رمز;
      }
    }

    glyph_مقصد_sid_map_t *create_glyph_مقصد_sid_map () ثابت
    {
      اگر (charset != &Null (Charset))
      {
	خودکار *mapping = (glyph_مقصد_sid_map_t *) hb_malloc (حجم (glyph_مقصد_sid_map_t));
	اگر (unlikely (!mapping)) واپس خالی_اشاریہ;
	mapping = نیا (mapping) glyph_مقصد_sid_map_t ();
	mapping->push (رمز_pair_t {0, 1});
	charset->collect_glyph_مقصد_sid_map (mapping, عدد_glyphs);
	واپس mapping;
      }
      ورنہ
	واپس خالی_اشاریہ;
    }

    hb_codepoint_t glyph_مقصد_sid (hb_codepoint_t glyph,
				 رمز_pair_t *cache = خالی_اشاریہ) ثابت
    {
      اگر (charset != &Null (Charset))
	واپس charset->get_sid (glyph, عدد_glyphs, cache);
      ورنہ
      {
	hb_codepoint_t sid = 0;
	انتخاب (چوٹی_Dict_2.Charset_انحراف)
	{
	  حالت ISOAdobeCharset:
	    اگر (glyph <= 228 /*zcaron*/) sid = glyph;
	    توڑو;
	  حالت ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    توڑو;
	  حالت ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    توڑو;
	  طےشدہ:
	    توڑو;
	}
	واپس sid;
      }
    }

    hb_codepoint_t sid_مقصد_glyph (hb_codepoint_t sid) ثابت
    {
      اگر (charset != &Null (Charset))
	واپس charset->get_glyph (sid, عدد_glyphs);
      ورنہ
      {
	hb_codepoint_t glyph = 0;
	انتخاب (چوٹی_Dict_2.Charset_انحراف)
	{
	  حالت ISOAdobeCharset:
	    اگر (sid <= 228 /*zcaron*/) glyph = sid;
	    توڑو;
	  حالت ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    توڑو;
	  حالت ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    توڑو;
	  طےشدہ:
	    توڑو;
	}
	واپس glyph;
      }
    }

    محفوظ:
    hb_sanitize_context_t   sc;

    عوامی:
    hb_blob_t               *blob = خالی_اشاریہ;
    ثابت Encoding	    *encoding = خالی_اشاریہ;
    ثابت Charset	    *charset = خالی_اشاریہ;
    ثابت CFF1_Name_اشاریہ     *name_اشاریہ = خالی_اشاریہ;
    ثابت CFF1_چوٹی_Dict_اشاریہ  *چوٹی_Dict_اشاریہ = خالی_اشاریہ;
    ثابت CFF1_String_اشاریہ   *string_اشاریہ = خالی_اشاریہ;
    ثابت CFF1Subrs	    *globalSubrs = خالی_اشاریہ;
    ثابت CFF1_Char_لڑیاں   *char_لڑیاں = خالی_اشاریہ;
    ثابت CFF1FDArray       *fdArray = خالی_اشاریہ;
    ثابت CFF1FDSelect      *fdSelect = خالی_اشاریہ;
    غیرعلامتی صحیح	     fd_گنتی = 0;

    cff1_چوٹی_dict_اقدار_t   چوٹی_Dict_2;
    hb_سمتیہ_t<cff1_font_dict_اقدار_t>
			     fontDicts;
    hb_سمتیہ_t<PRIVDICTVAL> نجی_Dicts;

    غیرعلامتی صحیح	     عدد_glyphs = 0;
    غیرعلامتی صحیح	     عدد_charset_entries = 0;
  };

  ساخت accelerator_t : accelerator_templ_t<cff1_نجی_dict_opset_t, cff1_نجی_dict_اقدار_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_نام.سیٹ_relaxed (خالی_اشاریہ);

      اگر (!is_valid ()) واپس;
      اگر (is_CID ()) واپس;
    }
    ~accelerator_t ()
    {
      hb_sorted_سمتیہ_t<gname_t> *نام = glyph_نام.get_relaxed ();
      اگر (نام)
      {
	نام->fini ();
	hb_free (نام);
      }
    }

    منطقی get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, غیرعلامتی صحیح buf_len) ثابت
    {
      اگر (unlikely (glyph >= عدد_glyphs)) واپس جھوٹ;
      اگر (unlikely (!is_valid ())) واپس جھوٹ;
      اگر (is_CID()) واپس جھوٹ;
      اگر (unlikely (!buf_len)) واپس سچ;
      hb_codepoint_t sid = glyph_مقصد_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      اگر (sid < cff1_std_لڑیاں_لمبائی)
      {
	hb_بائٹیں_t byte_str = cff1_std_لڑیاں (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.لمبائی;
      }
      ورنہ
      {
	hb_ubytes_t ubyte_str = (*string_اشاریہ)[sid - cff1_std_لڑیاں_لمبائی];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.لمبائی;
      }
      اگر (!str_len) واپس جھوٹ;
      غیرعلامتی صحیح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      واپس سچ;
    }

    منطقی get_glyph_from_name (ثابت حرف *name, صحیح len,
			      hb_codepoint_t *glyph) ثابت
    {
      اگر (unlikely (!is_valid ())) واپس جھوٹ;
      اگر (is_CID()) واپس جھوٹ;
      اگر (len < 0) len = لڑی_لمبائی (name);
      اگر (unlikely (!len)) واپس جھوٹ;

    retry:
      hb_sorted_سمتیہ_t<gname_t> *نام = glyph_نام.get_acquire ();
      اگر (unlikely (!نام))
      {
	نام = (hb_sorted_سمتیہ_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_سمتیہ_t<gname_t>));
	اگر (likely (نام))
	{
	  نام->init ();
	  /* TODO */

	  /* fill glyph names */
	  رمز_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};
	  برائے (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_مقصد_sid (gid, &glyph_مقصد_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    اگر (sid < cff1_std_لڑیاں_لمبائی)
	      gname.name = cff1_std_لڑیاں (sid);
	    ورنہ
	    {
	      hb_ubytes_t	ustr = (*string_اشاریہ)[sid - cff1_std_لڑیاں_لمبائی];
	      gname.name = hb_بائٹیں_t ((ثابت حرف*) ustr.arrayZ, ustr.لمبائی);
	    }
	    اگر (unlikely (!gname.name.arrayZ))
	      gname.name = hb_بائٹیں_t ("", 0); /* To avoid nullptr. */
	    نام->push (gname);
	  }
	  نام->qsort ();
	}
	اگر (unlikely (!glyph_نام.cmpexch (خالی_اشاریہ, نام)))
	{
	  اگر (نام)
	  {
	    نام->fini ();
	    hb_free (نام);
	  }
	  جاؤ retry;
	}
      }

      gname_t key = { hb_بائٹیں_t (name, len), 0 };
      ثابت gname_t *gname = نام ? نام->bsearch (key) : خالی_اشاریہ;
      اگر (!gname) واپس جھوٹ;
      hb_codepoint_t gid = sid_مقصد_glyph (gname->sid);
      اگر (!gid && gname->sid) واپس جھوٹ;
      *glyph = gid;
      واپس سچ;
    }

    HB_اندرونی منطقی get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = خالی_اشاریہ) ثابت;
    HB_اندرونی منطقی get_راستہ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالی_اشاریہ) ثابت;

    نجی:
    ساخت gname_t
    {
      hb_بائٹیں_t	name;
      uint16_t		sid;

      ساکن صحیح cmp (ثابت خالی *a_, ثابت خالی *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	غیرعلامتی minlen = hb_min (a->name.لمبائی, b->name.لمبائی);
	صحیح ret = لڑی_موازنہ (a->name.arrayZ, b->name.arrayZ, minlen);
	اگر (ret) واپس ret;
	واپس a->name.لمبائی - b->name.لمبائی;
      }

      صحیح cmp (ثابت gname_t &a) ثابت { واپس cmp (&a, یہ); }
    };

    تغیرپذیر hb_atomic_t<hb_sorted_سمتیہ_t<gname_t> *> glyph_نام;

    قسمتعریف accelerator_templ_t<cff1_نجی_dict_opset_t, cff1_نجی_dict_اقدار_t> SUPER;
  };

  ساخت accelerator_subset_t : accelerator_templ_t<cff1_نجی_dict_opset_subset_t, cff1_نجی_dict_اقدار_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      اگر (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندرونی منطقی subset (hb_subset_context_t *c) ثابت;
    HB_اندرونی منطقی serialize (hb_serialize_context_t *c,
				ساخت cff1_subset_plan &plan) ثابت;
    HB_اندرونی منطقی get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *بنیاد, hb_codepoint_t *accent) ثابت;

    تغیرپذیر CFF::cff_subset_accelerator_t* cff_accelerator = خالی_اشاریہ;

    قسمتعریف accelerator_templ_t<cff1_نجی_dict_opset_subset_t, cff1_نجی_dict_اقدار_subset_t> SUPER;
  };

  محفوظ:
  HB_اندرونی ساکن hb_codepoint_t lookup_standard_encoding_for_رمز (hb_codepoint_t sid);
  HB_اندرونی ساکن hb_codepoint_t lookup_expert_encoding_for_رمز (hb_codepoint_t sid);
  HB_اندرونی ساکن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_اندرونی ساکن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_اندرونی ساکن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_اندرونی ساکن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_اندرونی ساکن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t رمز);

  عوامی:
  Fixed_نسخہ<HBUINT8> نسخہ;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_مقصد<CFF1_Name_اشاریہ, HBUINT8> name_اشاریہ; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_سائز;	  /* offset size (unused?) */

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ساخت cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  خالی depend (hb_depend_ڈیٹا_builder_t *builder) ثابت
  {
    اگر (!is_valid ()) واپس;
    برائے (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
    {
      hb_codepoint_t بنیاد_gid, accent_gid;
      اگر (get_seac_components (gid, &بنیاد_gid, &accent_gid))
      {
        builder->جوڑو_depend (gid, HB_لیبل('C','F','F',' '), بنیاد_gid);
        builder->جوڑو_depend (gid, HB_لیبل('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#اگر_ختم /* HB_OT_CFF1_TABLE_HH */
