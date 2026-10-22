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

#که_نه_تعریف_شوی HB_OT_CFF1_جدول_HH
#تعریف_کړه HB_OT_CFF1_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-depend-data.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

#تعریف_کړه HB_STRING_ARRAY_NAME cff1_std_مزي_ګانې
#تعریف_کړه HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ضمن "hb-string-array.hh"
#تعریف_لغوه HB_STRING_ARRAY_LIST
#تعریف_لغوه HB_STRING_ARRAY_NAME

نطاق CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#تعریف_کړه HB_OT_نښان_CFF1 HB_نښان('C','F','F',' ')

#تعریف_کړه CFF_UNDEF_SID   CFF_UNDEF_کوډ

تعداد EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
تعداد CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعريفنوع CFF1_شاخص          CFF1_Char_مزي_ګانې;
تعريفنوع Subrs<HBUINT16>    CFF1Subrs;

بنية CFF1FDSelect : FDSelect {};

/* Encoding */
بنية Encoding0 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_کوډ (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    إذا (glyph < nCodes ())
    {
      أرجع (hb_codepoint_t)codes[glyph];
    }
    وإلا
      أرجع CFF_UNDEF_کوډ;
  }

  HBUINT8 &nCodes () { أرجع codes.len; }
  HBUINT8 nCodes () ثابت { أرجع codes.len; }

  Array_د<HBUINT8, HBUINT8> codes;

  DEFINE_اندازه_ARRAY_SIZED (1, codes);
};

بنية Encoding1_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   لومړی;
  HBUINT8   n_کیڼ;

  DEFINE_اندازه_STATIC (2);
};

بنية Encoding1 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_کوډ (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    لكل (بینښه صحيح i = 0; i < nRanges (); i++)
    {
      إذا (glyph <= ranges[i].n_کیڼ)
      {
	hb_codepoint_t کوډ = (hb_codepoint_t) ranges[i].لومړی + glyph;
	أرجع (likely (کوډ < 0x100) ? کوډ: CFF_UNDEF_کوډ);
      }
      glyph -= (ranges[i].n_کیڼ + 1);
    }
    أرجع CFF_UNDEF_کوډ;
  }

  HBUINT8 &nRanges () { أرجع ranges.len; }
  HBUINT8 nRanges () ثابت { أرجع ranges.len; }

  Array_د<Encoding1_Range, HBUINT8> ranges;

  DEFINE_اندازه_ARRAY_SIZED (1, ranges);
};

بنية SuppEncoding {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   کوډ;
  HBUINT16  glyph;

  DEFINE_اندازه_STATIC (3);
};

بنية CFF1_Supp_Enc_معلومات {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (supps.sanitize (c));
  }

  فراغ get_codes (hb_codepoint_t sid, hb_ویکټور_t<hb_codepoint_t> &codes) ثابت
  {
    لكل (بینښه صحيح i = 0; i < nSups (); i++)
      إذا (sid == supps[i].glyph)
	codes.push (supps[i].کوډ);
  }

  HBUINT8 &nSups () { أرجع supps.len; }
  HBUINT8 nSups () ثابت { أرجع supps.len; }

  Array_د<SuppEncoding, HBUINT8> supps;

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
		  بینښه صحيح enc_شمېر,
		  ثابت hb_ویکټور_t<کوډ_pair_t>& کوډ_ranges,
		  ثابت hb_ویکټور_t<کوډ_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (هذا);
    Encoding *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format | ((supp_codes.اوږدتیا > 0) ? 0x80 : 0);
    بدل (format) {
    حالة 0:
    {
      Encoding0 *fmt0 = c->allocate_اندازه<Encoding0> (Encoding0::min_اندازه + HBUINT8::static_اندازه * enc_شمېر);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      fmt0->nCodes () = enc_شمېر;
      بینښه صحيح glyph = 0;
      لكل (بینښه صحيح i = 0; i < کوډ_ranges.اوږدتیا; i++)
      {
	hb_codepoint_t کوډ = کوډ_ranges[i].کوډ;
	لكل (صحيح کیڼ = (صحيح)کوډ_ranges[i].glyph; کیڼ >= 0; کیڼ--)
	  fmt0->codes[glyph++] = کوډ++;
	إذا (unlikely (!((glyph <= 0x100) && (کوډ <= 0x100))))
	  return_trace (خطأ);
      }
    }
    اكسر;

    حالة 1:
    {
      Encoding1 *fmt1 = c->allocate_اندازه<Encoding1> (Encoding1::min_اندازه + Encoding1_Range::static_اندازه * کوډ_ranges.اوږدتیا);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      fmt1->nRanges () = کوډ_ranges.اوږدتیا;
      لكل (بینښه صحيح i = 0; i < کوډ_ranges.اوږدتیا; i++)
      {
	إذا (unlikely (!((کوډ_ranges[i].کوډ <= 0xFF) && (کوډ_ranges[i].glyph <= 0xFF))))
	  return_trace (خطأ);
	fmt1->ranges[i].لومړی = کوډ_ranges[i].کوډ;
	fmt1->ranges[i].n_کیڼ = کوډ_ranges[i].glyph;
      }
    }
    اكسر;

    }

    إذا (supp_codes.اوږدتیا)
    {
      CFF1_Supp_Enc_معلومات *supp_معلومات = c->allocate_اندازه<CFF1_Supp_Enc_معلومات> (CFF1_Supp_Enc_معلومات::min_اندازه + SuppEncoding::static_اندازه * supp_codes.اوږدتیا);
      إذا (unlikely (!supp_معلومات)) return_trace (خطأ);
      supp_معلومات->nSups () = supp_codes.اوږدتیا;
      لكل (بینښه صحيح i = 0; i < supp_codes.اوږدتیا; i++)
      {
	supp_معلومات->supps[i].کوډ = supp_codes[i].کوډ;
	supp_معلومات->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (ریښتیا);
  }

  size_t get_اندازه () ثابت
  {
    size_t اندازه = min_اندازه;
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); اندازه = hb_unsigned_جمع_کړه_saturate (اندازه, u.format0.get_اندازه ()); اكسر;
    حالة 1: hb_barrier (); اندازه = hb_unsigned_جمع_کړه_saturate (اندازه, u.format1.get_اندازه ()); اكسر;
    }
    إذا (has_supplement ())
      اندازه = hb_unsigned_جمع_کړه_saturate (اندازه, supp_Enc_معلومات ().get_اندازه ());
    أرجع اندازه;
  }

  hb_codepoint_t get_کوډ (hb_codepoint_t glyph) ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_کوډ (glyph);
    حالة 1: hb_barrier (); أرجع u.format1.get_کوډ (glyph);
    افتراضي:أرجع 0;
    }
  }

  uint8_t جدول_format () ثابت { أرجع format & 0x7F; }
  منطقي  has_supplement () ثابت { أرجع format & 0x80; }

  فراغ get_supplement_codes (hb_codepoint_t sid, hb_ویکټور_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    إذا (has_supplement ())
      supp_Enc_معلومات().get_codes (sid, codes);
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
    return_trace (likely (!has_supplement () || supp_Enc_معلومات ().sanitize (c)));
  }

  محمي:
  ثابت CFF1_Supp_Enc_معلومات &supp_Enc_معلومات () ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_معلومات> (u.format0.codes[u.format0.nCodes ()-1]);
    حالة 1: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_معلومات> (u.format1.ranges[u.format1.nRanges ()-1]);
    افتراضي:أرجع Null (CFF1_Supp_Enc_معلومات);
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
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح عدد_glyphs, بینښه *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (عدد_charset_entries) *عدد_charset_entries = عدد_glyphs;
    return_trace (sids.sanitize (c, عدد_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بینښه عدد_glyphs) ثابت
  {
    إذا (unlikely (glyph >= عدد_glyphs)) أرجع 0;
    إذا (unlikely (glyph == 0))
      أرجع 0;
    وإلا
      أرجع sids[glyph - 1];
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بینښه صحيح عدد_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (عدد_glyphs)) أرجع;
    لكل (hb_codepoint_t gid = 1; gid < عدد_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بینښه صحيح عدد_glyphs) ثابت
  {
    إذا (sid == 0)
      أرجع 0;

    لكل (بینښه صحيح glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      إذا (sids[glyph-1] == sid)
	أرجع glyph;
    }
    أرجع 0;
  }

  ساكن size_t get_اندازه (بینښه صحيح عدد_glyphs)
  {
    أكد (عدد_glyphs > 0);
    أرجع Unsized_Array_د<HBUINT16>::get_اندازه (عدد_glyphs - 1);
  }

  Unsized_Array_د<HBUINT16> sids;

  DEFINE_اندازه_ARRAY(0, sids);
};

کينډۍ <ډول_نوم TYPE>
بنية Charset_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16  لومړی;
  TYPE      n_کیڼ;

  DEFINE_اندازه_STATIC (HBUINT16::static_اندازه + TYPE::static_اندازه);
};

کينډۍ <ډول_نوم TYPE>
بنية Charset1_2 {
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح عدد_glyphs, بینښه *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    عدد_glyphs--;
    بینښه i;
    لكل (i = 0; عدد_glyphs > 0; i++)
    {
      إذا (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (عدد_glyphs >= ranges[i].n_کیڼ + 1))))
	return_trace (خطأ);
      عدد_glyphs -= (ranges[i].n_کیڼ + 1);
    }
    إذا (عدد_charset_entries)
      *عدد_charset_entries = i;
    return_trace (ریښتیا);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بینښه عدد_glyphs,
			  کوډ_pair_t *cache = تش_اشاره) ثابت
  {
    إذا (unlikely (glyph >= عدد_glyphs)) أرجع 0;
    بینښه i;
    hb_codepoint_t پیل_glyph;
    إذا (cache && likely (cache->glyph <= glyph))
    {
      i = cache->کوډ;
      پیل_glyph = cache->glyph;
    }
    وإلا
    {
      إذا (unlikely (glyph == 0)) أرجع 0;
      i = 0;
      پیل_glyph = 1;
    }
    glyph -= پیل_glyph;
    لكل (;; i++)
    {
      بینښه شمېر = ranges[i].n_کیڼ;
      إذا (glyph <= شمېر)
      {
        إذا (cache)
	  *cache = {i, پیل_glyph};
	أرجع ranges[i].لومړی + glyph;
      }
      شمېر++;
      پیل_glyph += شمېر;
      glyph -= شمېر;
    }

    أرجع 0;
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بینښه صحيح عدد_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (عدد_glyphs)) أرجع;

    hb_codepoint_t gid = 1;
    إذا (gid >= عدد_glyphs)
      أرجع;

    لكل (بینښه i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].لومړی;
      بینښه شمېر = ranges[i].n_کیڼ + 1;
      بینښه وروستی = gid + شمېر;
      لكل (بینښه j = 0; j < شمېر; j++)
	mapping->arrayZ[gid++] = {sid++, وروستی - 1};

      إذا (gid >= عدد_glyphs)
        اكسر;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بینښه صحيح عدد_glyphs) ثابت
  {
    إذا (sid == 0) أرجع 0;
    hb_codepoint_t  glyph = 1;
    لكل (بینښه صحيح i = 0;; i++)
    {
      إذا (glyph >= عدد_glyphs)
	أرجع 0;
      إذا ((ranges[i].لومړی <= sid) && (sid <= ranges[i].لومړی + ranges[i].n_کیڼ))
	أرجع glyph + (sid - ranges[i].لومړی);
      glyph += (ranges[i].n_کیڼ + 1);
    }

    أرجع 0;
  }

  size_t get_اندازه (بینښه صحيح عدد_glyphs) ثابت
  {
    صحيح glyph = (صحيح) عدد_glyphs;
    بینښه عدد_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    لكل (بینښه صحيح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_کیڼ + 1);
      عدد_ranges++;
    }

    أرجع get_اندازه_for_ranges (عدد_ranges);
  }

  ساكن size_t get_اندازه_for_ranges (بینښه صحيح عدد_ranges)
  {
    أرجع Unsized_Array_د<Charset_Range<TYPE> >::get_اندازه (عدد_ranges);
  }

  Unsized_Array_د<Charset_Range<TYPE>> ranges;

  DEFINE_اندازه_ARRAY (0, ranges);
};

تعريفنوع Charset1_2<HBUINT8>     Charset1;
تعريفنوع Charset1_2<HBUINT16>    Charset2;
تعريفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعريفنوع Charset_Range<HBUINT16> Charset2_Range;

بنية Charset
{
  /* serialize a fullset Charset */
  منطقي serialize (hb_serialize_context_t *c, ثابت Charset &src, بینښه صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_اندازه (عدد_glyphs)));
  }

  /* serialize a subset Charset */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  بینښه صحيح عدد_glyphs,
		  ثابت hb_ویکټور_t<کوډ_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (هذا);
    Charset *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format;
    بدل (format)
    {
    حالة 0:
    {
      Charset0 *fmt0 = c->allocate_اندازه<Charset0> (Charset0::get_اندازه (عدد_glyphs), خطأ);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      بینښه صحيح glyph = 0;
      لكل (بینښه صحيح i = 0; i < sid_ranges.اوږدتیا; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].کوډ;
	لكل (صحيح کیڼ = (صحيح)sid_ranges.arrayZ[i].glyph; کیڼ >= 0; کیڼ--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    اكسر;

    حالة 1:
    {
      Charset1 *fmt1 = c->allocate_اندازه<Charset1> (Charset1::get_اندازه_for_ranges (sid_ranges.اوږدتیا), خطأ);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بینښه صحيح i = 0; i < sid_ranges.اوږدتیا; i++)
      {
        خپلکاری &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].لومړی = _.کوډ;
	fmt1->ranges[i].n_کیڼ = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (خطأ);
    }
    اكسر;

    حالة 2:
    {
      Charset2 *fmt2 = c->allocate_اندازه<Charset2> (Charset2::get_اندازه_for_ranges (sid_ranges.اوږدتیا), خطأ);
      إذا (unlikely (!fmt2)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بینښه صحيح i = 0; i < sid_ranges.اوږدتیا; i++)
      {
        خپلکاری &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].لومړی = _.کوډ;
	fmt2->ranges[i].n_کیڼ = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (خطأ);
    }
    اكسر;

    }
    return_trace (ریښتیا);
  }

  size_t get_اندازه (بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه, u.format0.get_اندازه (عدد_glyphs));
    حالة 1: hb_barrier (); أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه, u.format1.get_اندازه (عدد_glyphs));
    حالة 2: hb_barrier (); أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه, u.format2.get_اندازه (عدد_glyphs));
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بینښه صحيح عدد_glyphs,
			  کوډ_pair_t *cache = تش_اشاره) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_sid (glyph, عدد_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_sid (glyph, عدد_glyphs, cache);
    حالة 2: hb_barrier (); أرجع u.format2.get_sid (glyph, عدد_glyphs, cache);
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); u.format0.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    حالة 1: hb_barrier (); u.format1.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    افتراضي:أرجع;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_glyph (sid, عدد_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_glyph (sid, عدد_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_glyph (sid, عدد_glyphs);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_عدد_glyphs (), عدد_charset_entries));
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
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF1_String_شاخص &مزي_ګانې,
		  ثابت hb_ویکټور_t<بینښه> &sidmap)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely ((مزي_ګانې.شمېر == 0) || (sidmap.اوږدتیا == 0)))
    {
      إذا (unlikely (!c->extend_min (هذا->شمېر)))
	return_trace (خطأ);
      شمېر = 0;
      return_trace (ریښتیا);
    }

    إذا (unlikely (sidmap.دننه_error ())) return_trace (خطأ);

    // Save this in a vector since serialize() iterates it twice.
    hb_ویکټور_t<hb_ubytes_t> بایټونه_Array (+ hb_iter (sidmap)
					 | hb_map (مزي_ګانې));

    إذا (unlikely (بایټونه_Array.دننه_error ())) return_trace (خطأ);

    منطقي پایله = CFF1_شاخص::serialize (c, بایټونه_Array);
    return_trace (پایله);
  }
};

بنية cff1_سر_dict_interp_env_t : عدد_interp_env_t
{
  cff1_سر_dict_interp_env_t ()
    : عدد_interp_env_t(), prev_دننه_واټن(0), وروستی_دننه_واټن(0) {}
  cff1_سر_dict_interp_env_t (ثابت hb_ubytes_t &بایټونه)
    : عدد_interp_env_t(بایټونه), prev_دننه_واټن(0), وروستی_دننه_واټن(0) {}

  بینښه صحيح prev_دننه_واټن;
  بینښه صحيح وروستی_دننه_واټن;
};

بنية name_dict_ارزښتونه_t
{
  تعداد name_dict_val_شاخص_t
  {
      نسخه,
      notice,
      copyright,
      ډک_Name,
      familyName,
      وزن,
      postscript,
      fontName,
      بنسټ_Font_Name,
      registry,
      ordering,

      Val_شمېر
  };

  فراغ init ()
  {
    لكل (بینښه صحيح i = 0; i < Val_شمېر; i++)
      values[i] = CFF_UNDEF_SID;
  }

  بینښه صحيح& چلوونکی[] (بینښه صحيح i)
  { أكد (i < Val_شمېر); أرجع values[i]; }

  بینښه صحيح چلوونکی[] (بینښه صحيح i) ثابت
  { أكد (i < Val_شمېر); أرجع values[i]; }

  ساكن تعداد name_dict_val_شاخص_t name_op_منزل_شاخص (op_کوډ_t op)
  {
    بدل (op) {
      افتراضي: // can't happen - just make some compiler happy
      حالة Op_کوډ_نسخه:
	أرجع نسخه;
      حالة Op_کوډ_Notice:
	أرجع notice;
      حالة Op_کوډ_Copyright:
	أرجع copyright;
      حالة Op_کوډ_ډک_Name:
	أرجع ډک_Name;
      حالة Op_کوډ_Family_Name:
	أرجع familyName;
      حالة Op_کوډ_وزن:
	أرجع وزن;
      حالة Op_کوډ_Post_Script:
	أرجع postscript;
      حالة Op_کوډ_Font_Name:
	أرجع fontName;
      حالة Op_کوډ_بنسټ_Font_Name:
	أرجع بنسټ_Font_Name;
    }
  }

  بینښه صحيح  values[Val_شمېر];
};

بنية cff1_سر_dict_val_t : op_str_t
{
  بینښه صحيح  وروستی_arg_دننه_واټن;
};

بنية cff1_سر_dict_ارزښتونه_t : سر_dict_ارزښتونه_t<cff1_سر_dict_val_t>
{
  فراغ init ()
  {
    سر_dict_ارزښتونه_t<cff1_سر_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_شمېر = 8720;
    Encoding_دننه_واټن = 0;
    Charset_دننه_واټن = 0;
    FDSelect_دننه_واټن = 0;
    شخصي_Dict_Info.init ();
  }
  فراغ fini () { سر_dict_ارزښتونه_t<cff1_سر_dict_val_t>::fini (); }

  منطقي is_CID () ثابت
  { أرجع nameSIDs[name_dict_ارزښتونه_t::registry] != CFF_UNDEF_SID; }

  name_dict_ارزښتونه_t  nameSIDs;
  بینښه صحيح    ros_supplement_دننه_واټن;
  بینښه صحيح    ros_supplement;
  بینښه صحيح    cid_شمېر;

  صحيح             Encoding_دننه_واټن;
  صحيح             Charset_دننه_واټن;
  صحيح             FDSelect_دننه_واټن;
  جدول_info_t    شخصي_Dict_Info;
};

بنية cff1_سر_dict_opset_t : سر_dict_opset_t<cff1_سر_dict_val_t>
{
  ساكن فراغ process_op (op_کوډ_t op, cff1_سر_dict_interp_env_t& env, cff1_سر_dict_ارزښتونه_t& dictval)
  {
    cff1_سر_dict_val_t  val {};

    بدل (op) {
      حالة Op_کوډ_نسخه:
      حالة Op_کوډ_Notice:
      حالة Op_کوډ_Copyright:
      حالة Op_کوډ_ډک_Name:
      حالة Op_کوډ_Font_Name:
      حالة Op_کوډ_Family_Name:
      حالة Op_کوډ_وزن:
      حالة Op_کوډ_Post_Script:
      حالة Op_کوډ_بنسټ_Font_Name:
	dictval.nameSIDs[name_dict_ارزښتونه_t::name_op_منزل_شاخص (op)] = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_is_Fixed_Pitch:
      حالة Op_کوډ_Italic_Angle:
      حالة Op_کوډ_Underline_Position:
      حالة Op_کوډ_Underline_Thickness:
      حالة Op_کوډ_Paint_Type:
      حالة Op_کوډ_Charstring_Type:
      حالة Op_کوډ_Unique_ID:
      حالة Op_کوډ_Stroke_پلنوالی:
      حالة Op_کوډ_Synthetic_بنسټ:
      حالة Op_کوډ_CIDFont_نسخه:
      حالة Op_کوډ_CIDFont_Revision:
      حالة Op_کوډ_CIDFont_Type:
      حالة Op_کوډ_UIDBase:
      حالة Op_کوډ_Font_BBox:
      حالة Op_کوډ_XUID:
      حالة Op_کوډ_بنسټ_Font_Blend:
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_CIDCount:
	dictval.cid_شمېر = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_ROS:
	dictval.ros_supplement = env.arg_ډېری.راوباسه_uint ();
	dictval.nameSIDs[name_dict_ارزښتونه_t::ordering] = env.arg_ډېری.راوباسه_uint ();
	dictval.nameSIDs[name_dict_ارزښتونه_t::registry] = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	إذا (unlikely (env.دننه_error ())) أرجع;
	أكد (env.وروستی_دننه_واټن > dictval.op_پیل);
	val.وروستی_arg_دننه_واټن = (env.وروستی_دننه_واټن - 1) - dictval.op_پیل;  /* offset to the last argument */
	اكسر;

      حالة Op_کوډ_Encoding:
	dictval.Encoding_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	إذا (unlikely (dictval.Encoding_دننه_واټن == 0)) أرجع;
	اكسر;

      حالة Op_کوډ_charset:
	dictval.Charset_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	إذا (unlikely (dictval.Charset_دننه_واټن == 0)) أرجع;
	اكسر;

      حالة Op_کوډ_FDSelect:
	dictval.FDSelect_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_شخصي:
	dictval.شخصي_Dict_Info.دننه_واټن = env.arg_ډېری.راوباسه_int ();
	dictval.شخصي_Dict_Info.اندازه = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	env.وروستی_دننه_واټن = env.str_ref.get_دننه_واټن ();
	سر_dict_opset_t<cff1_سر_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref, val);
  }
};

بنية cff1_font_dict_ارزښتونه_t : dict_ارزښتونه_t<op_str_t>
{
  فراغ init ()
  {
    dict_ارزښتونه_t<op_str_t>::init ();
    شخصي_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  فراغ fini () { dict_ارزښتونه_t<op_str_t>::fini (); }

  جدول_info_t       شخصي_Dict_Info;
  بینښه صحيح    fontName;
};

بنية cff1_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, عدد_interp_env_t& env, cff1_font_dict_ارزښتونه_t& dictval)
  {
    بدل (op) {
      حالة Op_کوډ_Font_Name:
	dictval.fontName = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_Font_ماتریکس:
      حالة Op_کوډ_Paint_Type:
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_شخصي:
	dictval.شخصي_Dict_Info.دننه_واټن = env.arg_ډېری.راوباسه_uint ();
	dictval.شخصي_Dict_Info.اندازه = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref);
  }
};

کينډۍ <ډول_نوم VAL>
بنية cff1_شخصي_dict_ارزښتونه_بنسټ_t : dict_ارزښتونه_t<VAL>
{
  فراغ init ()
  {
    dict_ارزښتونه_t<VAL>::init ();
    subrs_دننه_واټن = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  فراغ fini () { dict_ارزښتونه_t<VAL>::fini (); }

  صحيح                 subrs_دننه_واټن;
  ثابت CFF1Subrs    *localSubrs;
};

تعريفنوع cff1_شخصي_dict_ارزښتونه_بنسټ_t<op_str_t> cff1_شخصي_dict_ارزښتونه_subset_t;
تعريفنوع cff1_شخصي_dict_ارزښتونه_بنسټ_t<عدد_dict_val_t> cff1_شخصي_dict_ارزښتونه_t;

بنية cff1_شخصي_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, عدد_interp_env_t& env, cff1_شخصي_dict_ارزښتونه_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کوډ_Blue_ارزښتونه:
      حالة Op_کوډ_Other_Blues:
      حالة Op_کوډ_Family_Blues:
      حالة Op_کوډ_Family_Other_Blues:
      حالة Op_کوډ_Stem_Snap_H:
      حالة Op_کوډ_Stem_Snap_V:
      حالة Op_کوډ_Std_HW:
      حالة Op_کوډ_Std_VW:
      حالة Op_کوډ_Blue_Scale:
      حالة Op_کوډ_Blue_ښويدنه:
      حالة Op_کوډ_Blue_Fuzz:
      حالة Op_کوډ_Force_Bold:
      حالة Op_کوډ_Language_ډله:
      حالة Op_کوډ_Expansion_ضریب:
      حالة Op_کوډ_initial_Random_تخم:
      حالة Op_کوډ_default_پلنوالی_X:
      حالة Op_کوډ_nominal_پلنوالی_X:
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_Subrs:
	dictval.subrs_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref, val);
  }
};

بنية cff1_شخصي_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, عدد_interp_env_t& env, cff1_شخصي_dict_ارزښتونه_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کوډ_Blue_ارزښتونه:
      حالة Op_کوډ_Other_Blues:
      حالة Op_کوډ_Family_Blues:
      حالة Op_کوډ_Family_Other_Blues:
      حالة Op_کوډ_Stem_Snap_H:
      حالة Op_کوډ_Stem_Snap_V:
      حالة Op_کوډ_Std_HW:
      حالة Op_کوډ_Std_VW:
      حالة Op_کوډ_Blue_Scale:
      حالة Op_کوډ_Blue_ښويدنه:
      حالة Op_کوډ_Blue_Fuzz:
      حالة Op_کوډ_Force_Bold:
      حالة Op_کوډ_Language_ډله:
      حالة Op_کوډ_Expansion_ضریب:
      حالة Op_کوډ_initial_Random_تخم:
      حالة Op_کوډ_default_پلنوالی_X:
      حالة Op_کوډ_nominal_پلنوالی_X:
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_Subrs:
	dictval.subrs_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref);
  }
};

تعريفنوع dict_interpreter_t<cff1_سر_dict_opset_t, cff1_سر_dict_ارزښتونه_t, cff1_سر_dict_interp_env_t> cff1_سر_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_ارزښتونه_t> cff1_font_dict_interpreter_t;

تعريفنوع CFF1_شاخص CFF1_Name_شاخص;
تعريفنوع CFF1_شاخص CFF1_سر_Dict_شاخص;

بنية cff1_font_dict_ارزښتونه_mod_t
{
  cff1_font_dict_ارزښتونه_mod_t() { init (); }

  فراغ init () { init ( &Null (cff1_font_dict_ارزښتونه_t), CFF_UNDEF_SID ); }

  فراغ init (ثابت cff1_font_dict_ارزښتونه_t *بنسټ_2,
	     بینښه صحيح fontName_)
  {
    بنسټ = بنسټ_2;
    fontName = fontName_;
    شخصي_Dict_Info.init ();
  }

  بینښه get_شمېر () ثابت { أرجع بنسټ->get_شمېر (); }

  ثابت op_str_t &چلوونکی [] (بینښه صحيح i) ثابت { أرجع (*بنسټ)[i]; }

  ثابت cff1_font_dict_ارزښتونه_t    *بنسټ;
  جدول_info_t		   شخصي_Dict_Info;
  بینښه صحيح		fontName;
};

بنية CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  کينډۍ <ډول_نوم ITER, ډول_نوم OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT16>::serialize<cff1_font_dict_ارزښتونه_mod_t, cff1_font_dict_ارزښتونه_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

کارول نطاق CFF;

بنية cff1
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_CFF1;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1));
  }

  کينډۍ <ډول_نوم PRIVOPSET, ډول_نوم PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_نښان_t جدول_نښان = cff1::جدول_نښان;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      سر_Dict_2.init ();
      fontDicts.init ();
      شخصي_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.پیل_processing ();

      ثابت OT::cff1 *cff = هذا->blob->کينډۍ as<OT::cff1> ();

      إذا (cff == &Null (OT::cff1))
        اذهب fail;

      name_شاخص = &cff->name_شاخص (cff);
      إذا ((name_شاخص == &Null (CFF1_Name_شاخص)) || !name_شاخص->sanitize (&sc))
        اذهب fail;
      hb_barrier ();

      سر_Dict_شاخص = &Struct_At_دننه_واټن_Or_Null<CFF1_سر_Dict_شاخص> (name_شاخص, name_شاخص->get_اندازه (), sc);
      إذا (سر_Dict_شاخص == &Null (CFF1_سر_Dict_شاخص) || (سر_Dict_شاخص->شمېر == 0))
        اذهب fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t سر_Dict_Str = (*سر_Dict_شاخص)[0];
	إذا (unlikely (!سر_Dict_Str.sanitize (&sc)))   اذهب fail;
	hb_barrier ();
	cff1_سر_dict_interp_env_t env (سر_Dict_Str);
	cff1_سر_dict_interpreter_t سر_interp (env);
	إذا (unlikely (!سر_interp.interpret (سر_Dict_2)))   اذهب fail;
      }

      إذا (is_predef_charset ())
	charset = &Null (Charset);
      وإلا
      {
	charset = &Struct_At_دننه_واټن_Or_Null<Charset> (cff, سر_Dict_2.Charset_دننه_واټن, sc, &عدد_charset_entries);
	إذا (unlikely (charset == &Null (Charset)))   اذهب fail;
      }

      fd_شمېر = 1;
      إذا (is_CID ())
      {
	fdArray = &Struct_At_دننه_واټن_Or_Null<CFF1FDArray> (cff, سر_Dict_2.FDArray_دننه_واټن, sc);
	fdSelect = &Struct_At_دننه_واټن_Or_Null<CFF1FDSelect> (cff, سر_Dict_2.FDSelect_دننه_واټن, sc, fdArray->شمېر);
	إذا (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  اذهب fail;

	fd_شمېر = fdArray->شمېر;
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
	  encoding = &Struct_At_دننه_واټن_Or_Null<Encoding> (cff, سر_Dict_2.Encoding_دننه_واټن, sc);
	  إذا (unlikely (encoding == &Null (Encoding)))   اذهب fail;
	}
      }

      string_شاخص = &Struct_At_دننه_واټن_Or_Null<CFF1_String_شاخص> (سر_Dict_شاخص, سر_Dict_شاخص->get_اندازه (), sc);
      إذا (string_شاخص == &Null (CFF1_String_شاخص))
        اذهب fail;

      globalSubrs = &Struct_At_دننه_واټن_Or_Null<CFF1Subrs> (string_شاخص, string_شاخص->get_اندازه (), sc);
      char_مزي_ګانې = &Struct_At_دننه_واټن_Or_Null<CFF1_Char_مزي_ګانې> (cff, سر_Dict_2.char_مزي_ګانې_دننه_واټن, sc);
      إذا (char_مزي_ګانې == &Null (CFF1_Char_مزي_ګانې))
        اذهب fail;

      عدد_glyphs = char_مزي_ګانې->شمېر;
      إذا (عدد_glyphs != sc.get_عدد_glyphs ())
        اذهب fail;

      إذا (unlikely (!شخصي_Dicts.resize (fd_شمېر)))
        اذهب fail;
      لكل (بینښه صحيح i = 0; i < fd_شمېر; i++)
	شخصي_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      إذا (is_CID ())
      {
	لكل (بینښه صحيح i = 0; i < fd_شمېر; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  إذا (unlikely (!fontDictStr.sanitize (&sc)))   اذهب fail;
	  hb_barrier ();
	  cff1_font_dict_ارزښتونه_t *font;
	  cff1_سر_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  إذا (unlikely (fontDicts.دننه_error ()))   اذهب fail;

	  font->init ();
	  إذا (unlikely (!font_interp.interpret (*font)))   اذهب fail;
	  PRIVDICTVAL *priv = &شخصي_Dicts[i];
	  ثابت hb_ubytes_t privDictStr = Struct_At_دننه_واټن_Or_Null<UnsizedByteStr> (cff, font->شخصي_Dict_Info.دننه_واټن, sc, font->شخصي_Dict_Info.اندازه).as_ubytes (font->شخصي_Dict_Info.اندازه);
	  إذا (unlikely (font->شخصي_Dict_Info.اندازه &&
			privDictStr == (ثابت بینښه حرف *) &Null (UnsizedByteStr))) اذهب fail;
	  عدد_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	  priv->localSubrs = &Struct_At_دننه_واټن_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_دننه_واټن, sc);
	}
      }
      وإلا  /* non-CID */
      {
	cff1_سر_dict_ارزښتونه_t *font = &سر_Dict_2;
	PRIVDICTVAL *priv = &شخصي_Dicts[0];

	ثابت hb_ubytes_t privDictStr = Struct_At_دننه_واټن_Or_Null<UnsizedByteStr> (cff, font->شخصي_Dict_Info.دننه_واټن, sc, font->شخصي_Dict_Info.اندازه).as_ubytes (font->شخصي_Dict_Info.اندازه);
	إذا (font->شخصي_Dict_Info.اندازه &&
	    unlikely (privDictStr == (ثابت بینښه حرف *) &Null (UnsizedByteStr))) اذهب fail;
	عدد_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	priv->localSubrs = &Struct_At_دننه_واټن_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_دننه_واټن, sc);
	hb_barrier ();
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.پای_ټکی_processing ();
      سر_Dict_2.fini ();
      fontDicts.fini ();
      شخصي_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = تش_اشاره;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }
    منطقي   is_CID () ثابت { أرجع سر_Dict_2.is_CID (); }

    منطقي is_predef_charset () ثابت { أرجع سر_Dict_2.Charset_دننه_واټن <= ExpertSubsetCharset; }

    بینښه صحيح std_کوډ_منزل_glyph (hb_codepoint_t کوډ) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (کوډ);
      إذا (unlikely (sid == CFF_UNDEF_SID))
	أرجع 0;

      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, عدد_glyphs);
      وإلا إذا ((سر_Dict_2.Charset_دننه_واټن == ISOAdobeCharset)
	      && (کوډ <= 228 /*zcaron*/)) أرجع sid;
      أرجع 0;
    }

    منطقي is_predef_encoding () ثابت { أرجع سر_Dict_2.Encoding_دننه_واټن <= ExpertEncoding; }

    hb_codepoint_t glyph_منزل_کوډ (hb_codepoint_t glyph,
				  کوډ_pair_t *glyph_منزل_sid_cache = تش_اشاره) ثابت
    {
      إذا (encoding != &Null (Encoding))
	أرجع encoding->get_کوډ (glyph);
      وإلا
      {
	hb_codepoint_t sid = glyph_منزل_sid (glyph, glyph_منزل_sid_cache);
	إذا (sid == 0) أرجع 0;
	hb_codepoint_t کوډ = 0;
	بدل (سر_Dict_2.Encoding_دننه_واټن)
	{
	حالة StandardEncoding:
	  کوډ = lookup_standard_encoding_for_کوډ (sid);
	  اكسر;
	حالة ExpertEncoding:
	  کوډ = lookup_expert_encoding_for_کوډ (sid);
	  اكسر;
	افتراضي:
	  اكسر;
	}
	أرجع کوډ;
      }
    }

    glyph_منزل_sid_map_t *create_glyph_منزل_sid_map () ثابت
    {
      إذا (charset != &Null (Charset))
      {
	خپلکاری *mapping = (glyph_منزل_sid_map_t *) hb_malloc (حجم (glyph_منزل_sid_map_t));
	إذا (unlikely (!mapping)) أرجع تش_اشاره;
	mapping = جديد (mapping) glyph_منزل_sid_map_t ();
	mapping->push (کوډ_pair_t {0, 1});
	charset->collect_glyph_منزل_sid_map (mapping, عدد_glyphs);
	أرجع mapping;
      }
      وإلا
	أرجع تش_اشاره;
    }

    hb_codepoint_t glyph_منزل_sid (hb_codepoint_t glyph,
				 کوډ_pair_t *cache = تش_اشاره) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_sid (glyph, عدد_glyphs, cache);
      وإلا
      {
	hb_codepoint_t sid = 0;
	بدل (سر_Dict_2.Charset_دننه_واټن)
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

    hb_codepoint_t sid_منزل_glyph (hb_codepoint_t sid) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, عدد_glyphs);
      وإلا
      {
	hb_codepoint_t glyph = 0;
	بدل (سر_Dict_2.Charset_دننه_واټن)
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
    hb_blob_t               *blob = تش_اشاره;
    ثابت Encoding	    *encoding = تش_اشاره;
    ثابت Charset	    *charset = تش_اشاره;
    ثابت CFF1_Name_شاخص     *name_شاخص = تش_اشاره;
    ثابت CFF1_سر_Dict_شاخص  *سر_Dict_شاخص = تش_اشاره;
    ثابت CFF1_String_شاخص   *string_شاخص = تش_اشاره;
    ثابت CFF1Subrs	    *globalSubrs = تش_اشاره;
    ثابت CFF1_Char_مزي_ګانې   *char_مزي_ګانې = تش_اشاره;
    ثابت CFF1FDArray       *fdArray = تش_اشاره;
    ثابت CFF1FDSelect      *fdSelect = تش_اشاره;
    بینښه صحيح	     fd_شمېر = 0;

    cff1_سر_dict_ارزښتونه_t   سر_Dict_2;
    hb_ویکټور_t<cff1_font_dict_ارزښتونه_t>
			     fontDicts;
    hb_ویکټور_t<PRIVDICTVAL> شخصي_Dicts;

    بینښه صحيح	     عدد_glyphs = 0;
    بینښه صحيح	     عدد_charset_entries = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff1_شخصي_dict_opset_t, cff1_شخصي_dict_ارزښتونه_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_نومونه.سېټ_relaxed (تش_اشاره);

      إذا (!is_valid ()) أرجع;
      إذا (is_CID ()) أرجع;
    }
    ~accelerator_t ()
    {
      hb_sorted_ویکټور_t<gname_t> *نومونه = glyph_نومونه.get_relaxed ();
      إذا (نومونه)
      {
	نومونه->fini ();
	hb_free (نومونه);
      }
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بینښه صحيح buf_len) ثابت
    {
      إذا (unlikely (glyph >= عدد_glyphs)) أرجع خطأ;
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (unlikely (!buf_len)) أرجع ریښتیا;
      hb_codepoint_t sid = glyph_منزل_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      إذا (sid < cff1_std_مزي_ګانې_اوږدتیا)
      {
	hb_بایټونه_t byte_str = cff1_std_مزي_ګانې (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.اوږدتیا;
      }
      وإلا
      {
	hb_ubytes_t ubyte_str = (*string_شاخص)[sid - cff1_std_مزي_ګانې_اوږدتیا];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.اوږدتیا;
      }
      إذا (!str_len) أرجع خطأ;
      بینښه صحيح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      أرجع ریښتیا;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (len < 0) len = مزي_اوږدوالی (name);
      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      hb_sorted_ویکټور_t<gname_t> *نومونه = glyph_نومونه.get_acquire ();
      إذا (unlikely (!نومونه))
      {
	نومونه = (hb_sorted_ویکټور_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_ویکټور_t<gname_t>));
	إذا (likely (نومونه))
	{
	  نومونه->init ();
	  /* TODO */

	  /* fill glyph names */
	  کوډ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
	  لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_منزل_sid (gid, &glyph_منزل_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    إذا (sid < cff1_std_مزي_ګانې_اوږدتیا)
	      gname.name = cff1_std_مزي_ګانې (sid);
	    وإلا
	    {
	      hb_ubytes_t	ustr = (*string_شاخص)[sid - cff1_std_مزي_ګانې_اوږدتیا];
	      gname.name = hb_بایټونه_t ((ثابت حرف*) ustr.arrayZ, ustr.اوږدتیا);
	    }
	    إذا (unlikely (!gname.name.arrayZ))
	      gname.name = hb_بایټونه_t ("", 0); /* To avoid nullptr. */
	    نومونه->push (gname);
	  }
	  نومونه->qsort ();
	}
	إذا (unlikely (!glyph_نومونه.cmpexch (تش_اشاره, نومونه)))
	{
	  إذا (نومونه)
	  {
	    نومونه->fini ();
	    hb_free (نومونه);
	  }
	  اذهب retry;
	}
      }

      gname_t key = { hb_بایټونه_t (name, len), 0 };
      ثابت gname_t *gname = نومونه ? نومونه->bsearch (key) : تش_اشاره;
      إذا (!gname) أرجع خطأ;
      hb_codepoint_t gid = sid_منزل_glyph (gname->sid);
      إذا (!gid && gname->sid) أرجع خطأ;
      *glyph = gid;
      أرجع ریښتیا;
    }

    HB_دننی منطقي get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = تش_اشاره) ثابت;
    HB_دننی منطقي get_لار (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = تش_اشاره) ثابت;

    خاص:
    بنية gname_t
    {
      hb_بایټونه_t	name;
      uint16_t		sid;

      ساكن صحيح cmp (ثابت فراغ *a_, ثابت فراغ *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	بینښه minlen = hb_min (a->name.اوږدتیا, b->name.اوږدتیا);
	صحيح ret = مزي_پرتله (a->name.arrayZ, b->name.arrayZ, minlen);
	إذا (ret) أرجع ret;
	أرجع a->name.اوږدتیا - b->name.اوږدتیا;
      }

      صحيح cmp (ثابت gname_t &a) ثابت { أرجع cmp (&a, هذا); }
    };

    بدلیدونکی hb_atomic_t<hb_sorted_ویکټور_t<gname_t> *> glyph_نومونه;

    تعريفنوع accelerator_templ_t<cff1_شخصي_dict_opset_t, cff1_شخصي_dict_ارزښتونه_t> SUPER;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff1_شخصي_dict_opset_subset_t, cff1_شخصي_dict_ارزښتونه_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_دننی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_دننی منطقي serialize (hb_serialize_context_t *c,
				بنية cff1_subset_plan &plan) ثابت;
    HB_دننی منطقي get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *بنسټ, hb_codepoint_t *accent) ثابت;

    بدلیدونکی CFF::cff_subset_accelerator_t* cff_accelerator = تش_اشاره;

    تعريفنوع accelerator_templ_t<cff1_شخصي_dict_opset_subset_t, cff1_شخصي_dict_ارزښتونه_subset_t> SUPER;
  };

  محمي:
  HB_دننی ساكن hb_codepoint_t lookup_standard_encoding_for_کوډ (hb_codepoint_t sid);
  HB_دننی ساكن hb_codepoint_t lookup_expert_encoding_for_کوډ (hb_codepoint_t sid);
  HB_دننی ساكن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_دننی ساكن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_دننی ساكن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_دننی ساكن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_دننی ساكن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t کوډ);

  عام:
  Fixed_نسخه<HBUINT8> نسخه;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_منزل<CFF1_Name_شاخص, HBUINT8> name_شاخص; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_اندازه;	  /* offset size (unused?) */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

بنية cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  فراغ depend (hb_depend_معلومات_builder_t *builder) ثابت
  {
    إذا (!is_valid ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
    {
      hb_codepoint_t بنسټ_gid, accent_gid;
      إذا (get_seac_components (gid, &بنسټ_gid, &accent_gid))
      {
        builder->جمع_کړه_depend (gid, HB_نښان('C','F','F',' '), بنسټ_gid);
        builder->جمع_کړه_depend (gid, HB_نښان('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#د_که_پای /* HB_OT_CFF1_TABLE_HH */
