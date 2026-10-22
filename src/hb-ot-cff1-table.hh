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

#اگر_تعریف_نشده HB_OT_CFF1_جدول_HH
#تعریف_کن HB_OT_CFF1_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-depend-data.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

#تعریف_کن HB_STRING_ARRAY_NAME cff1_std_رشتهها
#تعریف_کن HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ضمن "hb-string-array.hh"
#لغو_تعریف HB_STRING_ARRAY_LIST
#لغو_تعریف HB_STRING_ARRAY_NAME

نطاق CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#تعریف_کن HB_OT_برچسب_CFF1 HB_برچسب('C','F','F',' ')

#تعریف_کن CFF_UNDEF_SID   CFF_UNDEF_کد

تعداد EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
تعداد CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعريفنوع CFF1_شاخص          CFF1_Char_رشتهها;
تعريفنوع Subrs<HBUINT16>    CFF1Subrs;

بنية CFF1FDSelect : FDSelect {};

/* Encoding */
بنية Encoding0 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_کد (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    إذا (glyph < nCodes ())
    {
      أرجع (hb_codepoint_t)codes[glyph];
    }
    وإلا
      أرجع CFF_UNDEF_کد;
  }

  HBUINT8 &nCodes () { أرجع codes.len; }
  HBUINT8 nCodes () ثابت { أرجع codes.len; }

  Array_از<HBUINT8, HBUINT8> codes;

  DEFINE_اندازه_ARRAY_SIZED (1, codes);
};

بنية Encoding1_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   اولی;
  HBUINT8   n_چپ;

  DEFINE_اندازه_STATIC (2);
};

بنية Encoding1 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_کد (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    لكل (بیعلامت صحيح i = 0; i < nRanges (); i++)
    {
      إذا (glyph <= ranges[i].n_چپ)
      {
	hb_codepoint_t کد = (hb_codepoint_t) ranges[i].اولی + glyph;
	أرجع (likely (کد < 0x100) ? کد: CFF_UNDEF_کد);
      }
      glyph -= (ranges[i].n_چپ + 1);
    }
    أرجع CFF_UNDEF_کد;
  }

  HBUINT8 &nRanges () { أرجع ranges.len; }
  HBUINT8 nRanges () ثابت { أرجع ranges.len; }

  Array_از<Encoding1_Range, HBUINT8> ranges;

  DEFINE_اندازه_ARRAY_SIZED (1, ranges);
};

بنية SuppEncoding {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   کد;
  HBUINT16  glyph;

  DEFINE_اندازه_STATIC (3);
};

بنية CFF1_Supp_Enc_داده {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (supps.sanitize (c));
  }

  فراغ get_codes (hb_codepoint_t sid, hb_وکتور_t<hb_codepoint_t> &codes) ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < nSups (); i++)
      إذا (sid == supps[i].glyph)
	codes.push (supps[i].کد);
  }

  HBUINT8 &nSups () { أرجع supps.len; }
  HBUINT8 nSups () ثابت { أرجع supps.len; }

  Array_از<SuppEncoding, HBUINT8> supps;

  DEFINE_اندازه_ARRAY_SIZED (1, supps);
};

بنية Encoding
{
  /* serialize a fullset Encoding */
  منطقي serialize (hb_serialize_context_t *c, ثابت Encoding &src)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  بیعلامت صحيح enc_شمار,
		  ثابت hb_وکتور_t<کد_pair_t>& کد_ranges,
		  ثابت hb_وکتور_t<کد_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (هذا);
    Encoding *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format | ((supp_codes.درازی > 0) ? 0x80 : 0);
    بدل (format) {
    حالة 0:
    {
      Encoding0 *fmt0 = c->allocate_اندازه<Encoding0> (Encoding0::min_اندازه + HBUINT8::static_اندازه * enc_شمار);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      fmt0->nCodes () = enc_شمار;
      بیعلامت صحيح glyph = 0;
      لكل (بیعلامت صحيح i = 0; i < کد_ranges.درازی; i++)
      {
	hb_codepoint_t کد = کد_ranges[i].کد;
	لكل (صحيح چپ = (صحيح)کد_ranges[i].glyph; چپ >= 0; چپ--)
	  fmt0->codes[glyph++] = کد++;
	إذا (unlikely (!((glyph <= 0x100) && (کد <= 0x100))))
	  return_trace (خطأ);
      }
    }
    اكسر;

    حالة 1:
    {
      Encoding1 *fmt1 = c->allocate_اندازه<Encoding1> (Encoding1::min_اندازه + Encoding1_Range::static_اندازه * کد_ranges.درازی);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      fmt1->nRanges () = کد_ranges.درازی;
      لكل (بیعلامت صحيح i = 0; i < کد_ranges.درازی; i++)
      {
	إذا (unlikely (!((کد_ranges[i].کد <= 0xFF) && (کد_ranges[i].glyph <= 0xFF))))
	  return_trace (خطأ);
	fmt1->ranges[i].اولی = کد_ranges[i].کد;
	fmt1->ranges[i].n_چپ = کد_ranges[i].glyph;
      }
    }
    اكسر;

    }

    إذا (supp_codes.درازی)
    {
      CFF1_Supp_Enc_داده *supp_داده = c->allocate_اندازه<CFF1_Supp_Enc_داده> (CFF1_Supp_Enc_داده::min_اندازه + SuppEncoding::static_اندازه * supp_codes.درازی);
      إذا (unlikely (!supp_داده)) return_trace (خطأ);
      supp_داده->nSups () = supp_codes.درازی;
      لكل (بیعلامت صحيح i = 0; i < supp_codes.درازی; i++)
      {
	supp_داده->supps[i].کد = supp_codes[i].کد;
	supp_داده->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (درست);
  }

  size_t get_اندازه () ثابت
  {
    size_t اندازه = min_اندازه;
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); اندازه = hb_unsigned_جمع_کن_saturate (اندازه, u.format0.get_اندازه ()); اكسر;
    حالة 1: hb_barrier (); اندازه = hb_unsigned_جمع_کن_saturate (اندازه, u.format1.get_اندازه ()); اكسر;
    }
    إذا (has_supplement ())
      اندازه = hb_unsigned_جمع_کن_saturate (اندازه, supp_Enc_داده ().get_اندازه ());
    أرجع اندازه;
  }

  hb_codepoint_t get_کد (hb_codepoint_t glyph) ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_کد (glyph);
    حالة 1: hb_barrier (); أرجع u.format1.get_کد (glyph);
    افتراضي:أرجع 0;
    }
  }

  uint8_t جدول_format () ثابت { أرجع format & 0x7F; }
  منطقي  has_supplement () ثابت { أرجع format & 0x80; }

  فراغ get_supplement_codes (hb_codepoint_t sid, hb_وکتور_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    إذا (has_supplement ())
      supp_Enc_داده().get_codes (sid, codes);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); إذا (unlikely (!u.format0.sanitize (c))) { return_trace (خطأ); } اكسر;
    حالة 1: hb_barrier (); إذا (unlikely (!u.format1.sanitize (c))) { return_trace (خطأ); } اكسر;
    افتراضي:return_trace (خطأ);
    }
    return_trace (likely (!has_supplement () || supp_Enc_داده ().sanitize (c)));
  }

  محمي:
  ثابت CFF1_Supp_Enc_داده &supp_Enc_داده () ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_داده> (u.format0.codes[u.format0.nCodes ()-1]);
    حالة 1: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_داده> (u.format1.ranges[u.format1.nRanges ()-1]);
    افتراضي:أرجع Null (CFF1_Supp_Enc_داده);
    }
  }

  عام:
  HBUINT8	format;
  اتحاد {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_اندازه_MIN (1);
};

/* Charset */
بنية Charset0
{
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح رقم_glyphs, بیعلامت *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (رقم_charset_entries) *رقم_charset_entries = رقم_glyphs;
    return_trace (sids.sanitize (c, رقم_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بیعلامت رقم_glyphs) ثابت
  {
    إذا (unlikely (glyph >= رقم_glyphs)) أرجع 0;
    إذا (unlikely (glyph == 0))
      أرجع 0;
    وإلا
      أرجع sids[glyph - 1];
  }

  فراغ collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, بیعلامت صحيح رقم_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (رقم_glyphs)) أرجع;
    لكل (hb_codepoint_t gid = 1; gid < رقم_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بیعلامت صحيح رقم_glyphs) ثابت
  {
    إذا (sid == 0)
      أرجع 0;

    لكل (بیعلامت صحيح glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      إذا (sids[glyph-1] == sid)
	أرجع glyph;
    }
    أرجع 0;
  }

  ساكن size_t get_اندازه (بیعلامت صحيح رقم_glyphs)
  {
    أكد (رقم_glyphs > 0);
    أرجع Unsized_Array_از<HBUINT16>::get_اندازه (رقم_glyphs - 1);
  }

  Unsized_Array_از<HBUINT16> sids;

  DEFINE_اندازه_ARRAY(0, sids);
};

قالب <نام_نوع TYPE>
بنية Charset_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16  اولی;
  TYPE      n_چپ;

  DEFINE_اندازه_STATIC (HBUINT16::static_اندازه + TYPE::static_اندازه);
};

قالب <نام_نوع TYPE>
بنية Charset1_2 {
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح رقم_glyphs, بیعلامت *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    رقم_glyphs--;
    بیعلامت i;
    لكل (i = 0; رقم_glyphs > 0; i++)
    {
      إذا (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (رقم_glyphs >= ranges[i].n_چپ + 1))))
	return_trace (خطأ);
      رقم_glyphs -= (ranges[i].n_چپ + 1);
    }
    إذا (رقم_charset_entries)
      *رقم_charset_entries = i;
    return_trace (درست);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بیعلامت رقم_glyphs,
			  کد_pair_t *cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (glyph >= رقم_glyphs)) أرجع 0;
    بیعلامت i;
    hb_codepoint_t شروع_glyph;
    إذا (cache && likely (cache->glyph <= glyph))
    {
      i = cache->کد;
      شروع_glyph = cache->glyph;
    }
    وإلا
    {
      إذا (unlikely (glyph == 0)) أرجع 0;
      i = 0;
      شروع_glyph = 1;
    }
    glyph -= شروع_glyph;
    لكل (;; i++)
    {
      بیعلامت شمار = ranges[i].n_چپ;
      إذا (glyph <= شمار)
      {
        إذا (cache)
	  *cache = {i, شروع_glyph};
	أرجع ranges[i].اولی + glyph;
      }
      شمار++;
      شروع_glyph += شمار;
      glyph -= شمار;
    }

    أرجع 0;
  }

  فراغ collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, بیعلامت صحيح رقم_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (رقم_glyphs)) أرجع;

    hb_codepoint_t gid = 1;
    إذا (gid >= رقم_glyphs)
      أرجع;

    لكل (بیعلامت i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].اولی;
      بیعلامت شمار = ranges[i].n_چپ + 1;
      بیعلامت آخرین = gid + شمار;
      لكل (بیعلامت j = 0; j < شمار; j++)
	mapping->arrayZ[gid++] = {sid++, آخرین - 1};

      إذا (gid >= رقم_glyphs)
        اكسر;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بیعلامت صحيح رقم_glyphs) ثابت
  {
    إذا (sid == 0) أرجع 0;
    hb_codepoint_t  glyph = 1;
    لكل (بیعلامت صحيح i = 0;; i++)
    {
      إذا (glyph >= رقم_glyphs)
	أرجع 0;
      إذا ((ranges[i].اولی <= sid) && (sid <= ranges[i].اولی + ranges[i].n_چپ))
	أرجع glyph + (sid - ranges[i].اولی);
      glyph += (ranges[i].n_چپ + 1);
    }

    أرجع 0;
  }

  size_t get_اندازه (بیعلامت صحيح رقم_glyphs) ثابت
  {
    صحيح glyph = (صحيح) رقم_glyphs;
    بیعلامت رقم_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    لكل (بیعلامت صحيح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_چپ + 1);
      رقم_ranges++;
    }

    أرجع get_اندازه_for_ranges (رقم_ranges);
  }

  ساكن size_t get_اندازه_for_ranges (بیعلامت صحيح رقم_ranges)
  {
    أرجع Unsized_Array_از<Charset_Range<TYPE> >::get_اندازه (رقم_ranges);
  }

  Unsized_Array_از<Charset_Range<TYPE>> ranges;

  DEFINE_اندازه_ARRAY (0, ranges);
};

تعريفنوع Charset1_2<HBUINT8>     Charset1;
تعريفنوع Charset1_2<HBUINT16>    Charset2;
تعريفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعريفنوع Charset_Range<HBUINT16> Charset2_Range;

بنية Charset
{
  /* serialize a fullset Charset */
  منطقي serialize (hb_serialize_context_t *c, ثابت Charset &src, بیعلامت صحيح رقم_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_اندازه (رقم_glyphs)));
  }

  /* serialize a subset Charset */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  بیعلامت صحيح رقم_glyphs,
		  ثابت hb_وکتور_t<کد_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (هذا);
    Charset *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format;
    بدل (format)
    {
    حالة 0:
    {
      Charset0 *fmt0 = c->allocate_اندازه<Charset0> (Charset0::get_اندازه (رقم_glyphs), خطأ);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      بیعلامت صحيح glyph = 0;
      لكل (بیعلامت صحيح i = 0; i < sid_ranges.درازی; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].کد;
	لكل (صحيح چپ = (صحيح)sid_ranges.arrayZ[i].glyph; چپ >= 0; چپ--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    اكسر;

    حالة 1:
    {
      Charset1 *fmt1 = c->allocate_اندازه<Charset1> (Charset1::get_اندازه_for_ranges (sid_ranges.درازی), خطأ);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بیعلامت صحيح i = 0; i < sid_ranges.درازی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].اولی = _.کد;
	fmt1->ranges[i].n_چپ = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (خطأ);
    }
    اكسر;

    حالة 2:
    {
      Charset2 *fmt2 = c->allocate_اندازه<Charset2> (Charset2::get_اندازه_for_ranges (sid_ranges.درازی), خطأ);
      إذا (unlikely (!fmt2)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بیعلامت صحيح i = 0; i < sid_ranges.درازی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].اولی = _.کد;
	fmt2->ranges[i].n_چپ = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (خطأ);
    }
    اكسر;

    }
    return_trace (درست);
  }

  size_t get_اندازه (بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جمع_کن_saturate (min_اندازه, u.format0.get_اندازه (رقم_glyphs));
    حالة 1: hb_barrier (); أرجع hb_unsigned_جمع_کن_saturate (min_اندازه, u.format1.get_اندازه (رقم_glyphs));
    حالة 2: hb_barrier (); أرجع hb_unsigned_جمع_کن_saturate (min_اندازه, u.format2.get_اندازه (رقم_glyphs));
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بیعلامت صحيح رقم_glyphs,
			  کد_pair_t *cache = اشاره_تهی) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_sid (glyph, رقم_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_sid (glyph, رقم_glyphs, cache);
    حالة 2: hb_barrier (); أرجع u.format2.get_sid (glyph, رقم_glyphs, cache);
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_glyph_مقصد_sid_map (glyph_مقصد_sid_map_t *mapping, بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); u.format0.collect_glyph_مقصد_sid_map (mapping, رقم_glyphs); أرجع;
    حالة 1: hb_barrier (); u.format1.collect_glyph_مقصد_sid_map (mapping, رقم_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyph_مقصد_sid_map (mapping, رقم_glyphs); أرجع;
    افتراضي:أرجع;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_glyph (sid, رقم_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_glyph (sid, رقم_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_glyph (sid, رقم_glyphs);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    افتراضي:return_trace (خطأ);
    }
  }

  HBUINT8       format;
  اتحاد {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_اندازه_MIN (1);
};

بنية CFF1_String_شاخص : CFF1_شاخص
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF1_String_شاخص &رشتهها,
		  ثابت hb_وکتور_t<بیعلامت> &sidmap)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely ((رشتهها.شمار == 0) || (sidmap.درازی == 0)))
    {
      إذا (unlikely (!c->extend_min (هذا->شمار)))
	return_trace (خطأ);
      شمار = 0;
      return_trace (درست);
    }

    إذا (unlikely (sidmap.درون_error ())) return_trace (خطأ);

    // Save this in a vector since serialize() iterates it twice.
    hb_وکتور_t<hb_ubytes_t> بایتها_Array (+ hb_iter (sidmap)
					 | hb_map (رشتهها));

    إذا (unlikely (بایتها_Array.درون_error ())) return_trace (خطأ);

    منطقي نتیجه = CFF1_شاخص::serialize (c, بایتها_Array);
    return_trace (نتیجه);
  }
};

بنية cff1_بالاترین_dict_interp_env_t : رقم_interp_env_t
{
  cff1_بالاترین_dict_interp_env_t ()
    : رقم_interp_env_t(), prev_جابه_جایی(0), آخرین_جابه_جایی(0) {}
  cff1_بالاترین_dict_interp_env_t (ثابت hb_ubytes_t &بایتها)
    : رقم_interp_env_t(بایتها), prev_جابه_جایی(0), آخرین_جابه_جایی(0) {}

  بیعلامت صحيح prev_جابه_جایی;
  بیعلامت صحيح آخرین_جابه_جایی;
};

بنية name_dict_مقادیر_t
{
  تعداد name_dict_val_شاخص_t
  {
      نسخه,
      notice,
      copyright,
      پر_Name,
      familyName,
      وزن,
      postscript,
      fontName,
      پایه_Font_Name,
      registry,
      ordering,

      Val_شمار
  };

  فراغ init ()
  {
    لكل (بیعلامت صحيح i = 0; i < Val_شمار; i++)
      values[i] = CFF_UNDEF_SID;
  }

  بیعلامت صحيح& عملگر[] (بیعلامت صحيح i)
  { أكد (i < Val_شمار); أرجع values[i]; }

  بیعلامت صحيح عملگر[] (بیعلامت صحيح i) ثابت
  { أكد (i < Val_شمار); أرجع values[i]; }

  ساكن تعداد name_dict_val_شاخص_t name_op_مقصد_شاخص (op_کد_t op)
  {
    بدل (op) {
      افتراضي: // can't happen - just make some compiler happy
      حالة Op_کد_نسخه:
	أرجع نسخه;
      حالة Op_کد_Notice:
	أرجع notice;
      حالة Op_کد_Copyright:
	أرجع copyright;
      حالة Op_کد_پر_Name:
	أرجع پر_Name;
      حالة Op_کد_Family_Name:
	أرجع familyName;
      حالة Op_کد_وزن:
	أرجع وزن;
      حالة Op_کد_Post_Script:
	أرجع postscript;
      حالة Op_کد_Font_Name:
	أرجع fontName;
      حالة Op_کد_پایه_Font_Name:
	أرجع پایه_Font_Name;
    }
  }

  بیعلامت صحيح  values[Val_شمار];
};

بنية cff1_بالاترین_dict_val_t : op_str_t
{
  بیعلامت صحيح  آخرین_arg_جابه_جایی;
};

بنية cff1_بالاترین_dict_مقادیر_t : بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>
{
  فراغ init ()
  {
    بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_شمار = 8720;
    Encoding_جابه_جایی = 0;
    Charset_جابه_جایی = 0;
    FDSelect_جابه_جایی = 0;
    خصوصی_Dict_Info.init ();
  }
  فراغ fini () { بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>::fini (); }

  منطقي is_CID () ثابت
  { أرجع nameSIDs[name_dict_مقادیر_t::registry] != CFF_UNDEF_SID; }

  name_dict_مقادیر_t  nameSIDs;
  بیعلامت صحيح    ros_supplement_جابه_جایی;
  بیعلامت صحيح    ros_supplement;
  بیعلامت صحيح    cid_شمار;

  صحيح             Encoding_جابه_جایی;
  صحيح             Charset_جابه_جایی;
  صحيح             FDSelect_جابه_جایی;
  جدول_info_t    خصوصی_Dict_Info;
};

بنية cff1_بالاترین_dict_opset_t : بالاترین_dict_opset_t<cff1_بالاترین_dict_val_t>
{
  ساكن فراغ process_op (op_کد_t op, cff1_بالاترین_dict_interp_env_t& env, cff1_بالاترین_dict_مقادیر_t& dictval)
  {
    cff1_بالاترین_dict_val_t  val {};

    بدل (op) {
      حالة Op_کد_نسخه:
      حالة Op_کد_Notice:
      حالة Op_کد_Copyright:
      حالة Op_کد_پر_Name:
      حالة Op_کد_Font_Name:
      حالة Op_کد_Family_Name:
      حالة Op_کد_وزن:
      حالة Op_کد_Post_Script:
      حالة Op_کد_پایه_Font_Name:
	dictval.nameSIDs[name_dict_مقادیر_t::name_op_مقصد_شاخص (op)] = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_is_Fixed_Pitch:
      حالة Op_کد_Italic_Angle:
      حالة Op_کد_Underline_Position:
      حالة Op_کد_Underline_Thickness:
      حالة Op_کد_Paint_Type:
      حالة Op_کد_Charstring_Type:
      حالة Op_کد_Unique_ID:
      حالة Op_کد_Stroke_عرض:
      حالة Op_کد_Synthetic_پایه:
      حالة Op_کد_CIDFont_نسخه:
      حالة Op_کد_CIDFont_Revision:
      حالة Op_کد_CIDFont_Type:
      حالة Op_کد_UIDBase:
      حالة Op_کد_Font_BBox:
      حالة Op_کد_XUID:
      حالة Op_کد_پایه_Font_Blend:
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_CIDCount:
	dictval.cid_شمار = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_ROS:
	dictval.ros_supplement = env.arg_پشته.برداشت_uint ();
	dictval.nameSIDs[name_dict_مقادیر_t::ordering] = env.arg_پشته.برداشت_uint ();
	dictval.nameSIDs[name_dict_مقادیر_t::registry] = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	إذا (unlikely (env.درون_error ())) أرجع;
	أكد (env.آخرین_جابه_جایی > dictval.op_شروع);
	val.آخرین_arg_جابه_جایی = (env.آخرین_جابه_جایی - 1) - dictval.op_شروع;  /* offset to the last argument */
	اكسر;

      حالة Op_کد_Encoding:
	dictval.Encoding_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	إذا (unlikely (dictval.Encoding_جابه_جایی == 0)) أرجع;
	اكسر;

      حالة Op_کد_charset:
	dictval.Charset_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	إذا (unlikely (dictval.Charset_جابه_جایی == 0)) أرجع;
	اكسر;

      حالة Op_کد_FDSelect:
	dictval.FDSelect_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_خصوصی:
	dictval.خصوصی_Dict_Info.جابه_جایی = env.arg_پشته.برداشت_int ();
	dictval.خصوصی_Dict_Info.اندازه = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	env.آخرین_جابه_جایی = env.str_ref.get_جابه_جایی ();
	بالاترین_dict_opset_t<cff1_بالاترین_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref, val);
  }
};

بنية cff1_font_dict_مقادیر_t : dict_مقادیر_t<op_str_t>
{
  فراغ init ()
  {
    dict_مقادیر_t<op_str_t>::init ();
    خصوصی_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  فراغ fini () { dict_مقادیر_t<op_str_t>::fini (); }

  جدول_info_t       خصوصی_Dict_Info;
  بیعلامت صحيح    fontName;
};

بنية cff1_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, رقم_interp_env_t& env, cff1_font_dict_مقادیر_t& dictval)
  {
    بدل (op) {
      حالة Op_کد_Font_Name:
	dictval.fontName = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_Font_ماتریس:
      حالة Op_کد_Paint_Type:
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_خصوصی:
	dictval.خصوصی_Dict_Info.جابه_جایی = env.arg_پشته.برداشت_uint ();
	dictval.خصوصی_Dict_Info.اندازه = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref);
  }
};

قالب <نام_نوع VAL>
بنية cff1_خصوصی_dict_مقادیر_پایه_t : dict_مقادیر_t<VAL>
{
  فراغ init ()
  {
    dict_مقادیر_t<VAL>::init ();
    subrs_جابه_جایی = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  فراغ fini () { dict_مقادیر_t<VAL>::fini (); }

  صحيح                 subrs_جابه_جایی;
  ثابت CFF1Subrs    *localSubrs;
};

تعريفنوع cff1_خصوصی_dict_مقادیر_پایه_t<op_str_t> cff1_خصوصی_dict_مقادیر_subset_t;
تعريفنوع cff1_خصوصی_dict_مقادیر_پایه_t<رقم_dict_val_t> cff1_خصوصی_dict_مقادیر_t;

بنية cff1_خصوصی_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, رقم_interp_env_t& env, cff1_خصوصی_dict_مقادیر_t& dictval)
  {
    رقم_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Force_Bold:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Expansion_ضریب:
      حالة Op_کد_initial_Random_دانه:
      حالة Op_کد_default_عرض_X:
      حالة Op_کد_nominal_عرض_X:
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_Subrs:
	dictval.subrs_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref, val);
  }
};

بنية cff1_خصوصی_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, رقم_interp_env_t& env, cff1_خصوصی_dict_مقادیر_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Force_Bold:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Expansion_ضریب:
      حالة Op_کد_initial_Random_دانه:
      حالة Op_کد_default_عرض_X:
      حالة Op_کد_nominal_عرض_X:
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_Subrs:
	dictval.subrs_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref);
  }
};

تعريفنوع dict_interpreter_t<cff1_بالاترین_dict_opset_t, cff1_بالاترین_dict_مقادیر_t, cff1_بالاترین_dict_interp_env_t> cff1_بالاترین_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_مقادیر_t> cff1_font_dict_interpreter_t;

تعريفنوع CFF1_شاخص CFF1_Name_شاخص;
تعريفنوع CFF1_شاخص CFF1_بالاترین_Dict_شاخص;

بنية cff1_font_dict_مقادیر_mod_t
{
  cff1_font_dict_مقادیر_mod_t() { init (); }

  فراغ init () { init ( &Null (cff1_font_dict_مقادیر_t), CFF_UNDEF_SID ); }

  فراغ init (ثابت cff1_font_dict_مقادیر_t *پایه_2,
	     بیعلامت صحيح fontName_)
  {
    پایه = پایه_2;
    fontName = fontName_;
    خصوصی_Dict_Info.init ();
  }

  بیعلامت get_شمار () ثابت { أرجع پایه->get_شمار (); }

  ثابت op_str_t &عملگر [] (بیعلامت صحيح i) ثابت { أرجع (*پایه)[i]; }

  ثابت cff1_font_dict_مقادیر_t    *پایه;
  جدول_info_t		   خصوصی_Dict_Info;
  بیعلامت صحيح		fontName;
};

بنية CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  قالب <نام_نوع ITER, نام_نوع OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT16>::serialize<cff1_font_dict_مقادیر_mod_t, cff1_font_dict_مقادیر_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

استفاده نطاق CFF;

بنية cff1
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_CFF1;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1));
  }

  قالب <نام_نوع PRIVOPSET, نام_نوع PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_برچسب_t جدول_برچسب = cff1::جدول_برچسب;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      بالاترین_Dict_2.init ();
      fontDicts.init ();
      خصوصی_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروع_processing ();

      ثابت OT::cff1 *cff = هذا->blob->قالب as<OT::cff1> ();

      إذا (cff == &Null (OT::cff1))
        اذهب fail;

      name_شاخص = &cff->name_شاخص (cff);
      إذا ((name_شاخص == &Null (CFF1_Name_شاخص)) || !name_شاخص->sanitize (&sc))
        اذهب fail;
      hb_barrier ();

      بالاترین_Dict_شاخص = &Struct_At_جابه_جایی_Or_Null<CFF1_بالاترین_Dict_شاخص> (name_شاخص, name_شاخص->get_اندازه (), sc);
      إذا (بالاترین_Dict_شاخص == &Null (CFF1_بالاترین_Dict_شاخص) || (بالاترین_Dict_شاخص->شمار == 0))
        اذهب fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t بالاترین_Dict_Str = (*بالاترین_Dict_شاخص)[0];
	إذا (unlikely (!بالاترین_Dict_Str.sanitize (&sc)))   اذهب fail;
	hb_barrier ();
	cff1_بالاترین_dict_interp_env_t env (بالاترین_Dict_Str);
	cff1_بالاترین_dict_interpreter_t بالاترین_interp (env);
	إذا (unlikely (!بالاترین_interp.interpret (بالاترین_Dict_2)))   اذهب fail;
      }

      إذا (is_predef_charset ())
	charset = &Null (Charset);
      وإلا
      {
	charset = &Struct_At_جابه_جایی_Or_Null<Charset> (cff, بالاترین_Dict_2.Charset_جابه_جایی, sc, &رقم_charset_entries);
	إذا (unlikely (charset == &Null (Charset)))   اذهب fail;
      }

      fd_شمار = 1;
      إذا (is_CID ())
      {
	fdArray = &Struct_At_جابه_جایی_Or_Null<CFF1FDArray> (cff, بالاترین_Dict_2.FDArray_جابه_جایی, sc);
	fdSelect = &Struct_At_جابه_جایی_Or_Null<CFF1FDSelect> (cff, بالاترین_Dict_2.FDSelect_جابه_جایی, sc, fdArray->شمار);
	إذا (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  اذهب fail;

	fd_شمار = fdArray->شمار;
      }
      وإلا
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      إذا (is_CID ())
      {
	إذا (unlikely (charset == &Null (Charset)))   اذهب fail;
      }
      وإلا
      {
	إذا (!is_predef_encoding ())
	{
	  encoding = &Struct_At_جابه_جایی_Or_Null<Encoding> (cff, بالاترین_Dict_2.Encoding_جابه_جایی, sc);
	  إذا (unlikely (encoding == &Null (Encoding)))   اذهب fail;
	}
      }

      string_شاخص = &Struct_At_جابه_جایی_Or_Null<CFF1_String_شاخص> (بالاترین_Dict_شاخص, بالاترین_Dict_شاخص->get_اندازه (), sc);
      إذا (string_شاخص == &Null (CFF1_String_شاخص))
        اذهب fail;

      globalSubrs = &Struct_At_جابه_جایی_Or_Null<CFF1Subrs> (string_شاخص, string_شاخص->get_اندازه (), sc);
      char_رشتهها = &Struct_At_جابه_جایی_Or_Null<CFF1_Char_رشتهها> (cff, بالاترین_Dict_2.char_رشتهها_جابه_جایی, sc);
      إذا (char_رشتهها == &Null (CFF1_Char_رشتهها))
        اذهب fail;

      رقم_glyphs = char_رشتهها->شمار;
      إذا (رقم_glyphs != sc.get_رقم_glyphs ())
        اذهب fail;

      إذا (unlikely (!خصوصی_Dicts.resize (fd_شمار)))
        اذهب fail;
      لكل (بیعلامت صحيح i = 0; i < fd_شمار; i++)
	خصوصی_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      إذا (is_CID ())
      {
	لكل (بیعلامت صحيح i = 0; i < fd_شمار; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  إذا (unlikely (!fontDictStr.sanitize (&sc)))   اذهب fail;
	  hb_barrier ();
	  cff1_font_dict_مقادیر_t *font;
	  cff1_بالاترین_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  إذا (unlikely (fontDicts.درون_error ()))   اذهب fail;

	  font->init ();
	  إذا (unlikely (!font_interp.interpret (*font)))   اذهب fail;
	  PRIVDICTVAL *priv = &خصوصی_Dicts[i];
	  ثابت hb_ubytes_t privDictStr = Struct_At_جابه_جایی_Or_Null<UnsizedByteStr> (cff, font->خصوصی_Dict_Info.جابه_جایی, sc, font->خصوصی_Dict_Info.اندازه).as_ubytes (font->خصوصی_Dict_Info.اندازه);
	  إذا (unlikely (font->خصوصی_Dict_Info.اندازه &&
			privDictStr == (ثابت بیعلامت حرف *) &Null (UnsizedByteStr))) اذهب fail;
	  رقم_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	  priv->localSubrs = &Struct_At_جابه_جایی_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_جابه_جایی, sc);
	}
      }
      وإلا  /* non-CID */
      {
	cff1_بالاترین_dict_مقادیر_t *font = &بالاترین_Dict_2;
	PRIVDICTVAL *priv = &خصوصی_Dicts[0];

	ثابت hb_ubytes_t privDictStr = Struct_At_جابه_جایی_Or_Null<UnsizedByteStr> (cff, font->خصوصی_Dict_Info.جابه_جایی, sc, font->خصوصی_Dict_Info.اندازه).as_ubytes (font->خصوصی_Dict_Info.اندازه);
	إذا (font->خصوصی_Dict_Info.اندازه &&
	    unlikely (privDictStr == (ثابت بیعلامت حرف *) &Null (UnsizedByteStr))) اذهب fail;
	رقم_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	priv->localSubrs = &Struct_At_جابه_جایی_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_جابه_جایی, sc);
	hb_barrier ();
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.انتها_processing ();
      بالاترین_Dict_2.fini ();
      fontDicts.fini ();
      خصوصی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = اشاره_تهی;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }
    منطقي   is_CID () ثابت { أرجع بالاترین_Dict_2.is_CID (); }

    منطقي is_predef_charset () ثابت { أرجع بالاترین_Dict_2.Charset_جابه_جایی <= ExpertSubsetCharset; }

    بیعلامت صحيح std_کد_مقصد_glyph (hb_codepoint_t کد) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (کد);
      إذا (unlikely (sid == CFF_UNDEF_SID))
	أرجع 0;

      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, رقم_glyphs);
      وإلا إذا ((بالاترین_Dict_2.Charset_جابه_جایی == ISOAdobeCharset)
	      && (کد <= 228 /*zcaron*/)) أرجع sid;
      أرجع 0;
    }

    منطقي is_predef_encoding () ثابت { أرجع بالاترین_Dict_2.Encoding_جابه_جایی <= ExpertEncoding; }

    hb_codepoint_t glyph_مقصد_کد (hb_codepoint_t glyph,
				  کد_pair_t *glyph_مقصد_sid_cache = اشاره_تهی) ثابت
    {
      إذا (encoding != &Null (Encoding))
	أرجع encoding->get_کد (glyph);
      وإلا
      {
	hb_codepoint_t sid = glyph_مقصد_sid (glyph, glyph_مقصد_sid_cache);
	إذا (sid == 0) أرجع 0;
	hb_codepoint_t کد = 0;
	بدل (بالاترین_Dict_2.Encoding_جابه_جایی)
	{
	حالة StandardEncoding:
	  کد = lookup_standard_encoding_for_کد (sid);
	  اكسر;
	حالة ExpertEncoding:
	  کد = lookup_expert_encoding_for_کد (sid);
	  اكسر;
	افتراضي:
	  اكسر;
	}
	أرجع کد;
      }
    }

    glyph_مقصد_sid_map_t *create_glyph_مقصد_sid_map () ثابت
    {
      إذا (charset != &Null (Charset))
      {
	خودکار *mapping = (glyph_مقصد_sid_map_t *) hb_malloc (حجم (glyph_مقصد_sid_map_t));
	إذا (unlikely (!mapping)) أرجع اشاره_تهی;
	mapping = جديد (mapping) glyph_مقصد_sid_map_t ();
	mapping->push (کد_pair_t {0, 1});
	charset->collect_glyph_مقصد_sid_map (mapping, رقم_glyphs);
	أرجع mapping;
      }
      وإلا
	أرجع اشاره_تهی;
    }

    hb_codepoint_t glyph_مقصد_sid (hb_codepoint_t glyph,
				 کد_pair_t *cache = اشاره_تهی) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_sid (glyph, رقم_glyphs, cache);
      وإلا
      {
	hb_codepoint_t sid = 0;
	بدل (بالاترین_Dict_2.Charset_جابه_جایی)
	{
	  حالة ISOAdobeCharset:
	    إذا (glyph <= 228 /*zcaron*/) sid = glyph;
	    اكسر;
	  حالة ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    اكسر;
	  حالة ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    اكسر;
	  افتراضي:
	    اكسر;
	}
	أرجع sid;
      }
    }

    hb_codepoint_t sid_مقصد_glyph (hb_codepoint_t sid) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, رقم_glyphs);
      وإلا
      {
	hb_codepoint_t glyph = 0;
	بدل (بالاترین_Dict_2.Charset_جابه_جایی)
	{
	  حالة ISOAdobeCharset:
	    إذا (sid <= 228 /*zcaron*/) glyph = sid;
	    اكسر;
	  حالة ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    اكسر;
	  حالة ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    اكسر;
	  افتراضي:
	    اكسر;
	}
	أرجع glyph;
      }
    }

    محمي:
    hb_sanitize_context_t   sc;

    عام:
    hb_blob_t               *blob = اشاره_تهی;
    ثابت Encoding	    *encoding = اشاره_تهی;
    ثابت Charset	    *charset = اشاره_تهی;
    ثابت CFF1_Name_شاخص     *name_شاخص = اشاره_تهی;
    ثابت CFF1_بالاترین_Dict_شاخص  *بالاترین_Dict_شاخص = اشاره_تهی;
    ثابت CFF1_String_شاخص   *string_شاخص = اشاره_تهی;
    ثابت CFF1Subrs	    *globalSubrs = اشاره_تهی;
    ثابت CFF1_Char_رشتهها   *char_رشتهها = اشاره_تهی;
    ثابت CFF1FDArray       *fdArray = اشاره_تهی;
    ثابت CFF1FDSelect      *fdSelect = اشاره_تهی;
    بیعلامت صحيح	     fd_شمار = 0;

    cff1_بالاترین_dict_مقادیر_t   بالاترین_Dict_2;
    hb_وکتور_t<cff1_font_dict_مقادیر_t>
			     fontDicts;
    hb_وکتور_t<PRIVDICTVAL> خصوصی_Dicts;

    بیعلامت صحيح	     رقم_glyphs = 0;
    بیعلامت صحيح	     رقم_charset_entries = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff1_خصوصی_dict_opset_t, cff1_خصوصی_dict_مقادیر_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_نامها.مجموعه_relaxed (اشاره_تهی);

      إذا (!is_valid ()) أرجع;
      إذا (is_CID ()) أرجع;
    }
    ~accelerator_t ()
    {
      hb_sorted_وکتور_t<gname_t> *نامها = glyph_نامها.get_relaxed ();
      إذا (نامها)
      {
	نامها->fini ();
	hb_free (نامها);
      }
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بیعلامت صحيح buf_len) ثابت
    {
      إذا (unlikely (glyph >= رقم_glyphs)) أرجع خطأ;
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (unlikely (!buf_len)) أرجع درست;
      hb_codepoint_t sid = glyph_مقصد_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      إذا (sid < cff1_std_رشتهها_درازی)
      {
	hb_بایتها_t byte_str = cff1_std_رشتهها (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.درازی;
      }
      وإلا
      {
	hb_ubytes_t ubyte_str = (*string_شاخص)[sid - cff1_std_رشتهها_درازی];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.درازی;
      }
      إذا (!str_len) أرجع خطأ;
      بیعلامت صحيح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      أرجع درست;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (len < 0) len = طول_رشته (name);
      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      hb_sorted_وکتور_t<gname_t> *نامها = glyph_نامها.get_acquire ();
      إذا (unlikely (!نامها))
      {
	نامها = (hb_sorted_وکتور_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_وکتور_t<gname_t>));
	إذا (likely (نامها))
	{
	  نامها->init ();
	  /* TODO */

	  /* fill glyph names */
	  کد_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};
	  لكل (hb_codepoint_t gid = 0; gid < رقم_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_مقصد_sid (gid, &glyph_مقصد_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    إذا (sid < cff1_std_رشتهها_درازی)
	      gname.name = cff1_std_رشتهها (sid);
	    وإلا
	    {
	      hb_ubytes_t	ustr = (*string_شاخص)[sid - cff1_std_رشتهها_درازی];
	      gname.name = hb_بایتها_t ((ثابت حرف*) ustr.arrayZ, ustr.درازی);
	    }
	    إذا (unlikely (!gname.name.arrayZ))
	      gname.name = hb_بایتها_t ("", 0); /* To avoid nullptr. */
	    نامها->push (gname);
	  }
	  نامها->qsort ();
	}
	إذا (unlikely (!glyph_نامها.cmpexch (اشاره_تهی, نامها)))
	{
	  إذا (نامها)
	  {
	    نامها->fini ();
	    hb_free (نامها);
	  }
	  اذهب retry;
	}
      }

      gname_t key = { hb_بایتها_t (name, len), 0 };
      ثابت gname_t *gname = نامها ? نامها->bsearch (key) : اشاره_تهی;
      إذا (!gname) أرجع خطأ;
      hb_codepoint_t gid = sid_مقصد_glyph (gname->sid);
      إذا (!gid && gname->sid) أرجع خطأ;
      *glyph = gid;
      أرجع درست;
    }

    HB_داخلی منطقي get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = اشاره_تهی) ثابت;
    HB_داخلی منطقي get_مسیر (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = اشاره_تهی) ثابت;

    خاص:
    بنية gname_t
    {
      hb_بایتها_t	name;
      uint16_t		sid;

      ساكن صحيح cmp (ثابت فراغ *a_, ثابت فراغ *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	بیعلامت minlen = hb_min (a->name.درازی, b->name.درازی);
	صحيح ret = مقایسه_رشته (a->name.arrayZ, b->name.arrayZ, minlen);
	إذا (ret) أرجع ret;
	أرجع a->name.درازی - b->name.درازی;
      }

      صحيح cmp (ثابت gname_t &a) ثابت { أرجع cmp (&a, هذا); }
    };

    تغییرپذیر hb_atomic_t<hb_sorted_وکتور_t<gname_t> *> glyph_نامها;

    تعريفنوع accelerator_templ_t<cff1_خصوصی_dict_opset_t, cff1_خصوصی_dict_مقادیر_t> SUPER;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff1_خصوصی_dict_opset_subset_t, cff1_خصوصی_dict_مقادیر_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_داخلی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_داخلی منطقي serialize (hb_serialize_context_t *c,
				بنية cff1_subset_plan &plan) ثابت;
    HB_داخلی منطقي get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *پایه, hb_codepoint_t *accent) ثابت;

    تغییرپذیر CFF::cff_subset_accelerator_t* cff_accelerator = اشاره_تهی;

    تعريفنوع accelerator_templ_t<cff1_خصوصی_dict_opset_subset_t, cff1_خصوصی_dict_مقادیر_subset_t> SUPER;
  };

  محمي:
  HB_داخلی ساكن hb_codepoint_t lookup_standard_encoding_for_کد (hb_codepoint_t sid);
  HB_داخلی ساكن hb_codepoint_t lookup_expert_encoding_for_کد (hb_codepoint_t sid);
  HB_داخلی ساكن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_داخلی ساكن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_داخلی ساكن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_داخلی ساكن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_داخلی ساكن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t کد);

  عام:
  Fixed_نسخه<HBUINT8> نسخه;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_مقصد<CFF1_Name_شاخص, HBUINT8> name_شاخص; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_اندازه;	  /* offset size (unused?) */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

بنية cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  فراغ depend (hb_depend_داده_builder_t *builder) ثابت
  {
    إذا (!is_valid ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < رقم_glyphs; gid++)
    {
      hb_codepoint_t پایه_gid, accent_gid;
      إذا (get_seac_components (gid, &پایه_gid, &accent_gid))
      {
        builder->جمع_کن_depend (gid, HB_برچسب('C','F','F',' '), پایه_gid);
        builder->جمع_کن_depend (gid, HB_برچسب('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#پایان_اگر /* HB_OT_CFF1_TABLE_HH */
