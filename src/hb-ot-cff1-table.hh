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

#جے_بنا_تعریف HB_OT_CFF1_جدول_HH
#تعریف_کرو HB_OT_CFF1_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-depend-data.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

#تعریف_کرو HB_STRING_ARRAY_NAME cff1_std_لڑیاں
#تعریف_کرو HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ضمن "hb-string-array.hh"
#تعریف_رد HB_STRING_ARRAY_LIST
#تعریف_رد HB_STRING_ARRAY_NAME

نطاق CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#تعریف_کرو HB_OT_ٹیگ_CFF1 HB_ٹیگ('C','F','F',' ')

#تعریف_کرو CFF_UNDEF_SID   CFF_UNDEF_کوڈ

تعداد EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
تعداد CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعريفنوع CFF1_اشاریہ          CFF1_Char_لڑیاں;
تعريفنوع Subrs<HBUINT16>    CFF1Subrs;

بنية CFF1FDSelect : FDSelect {};

/* Encoding */
بنية Encoding0 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_کوڈ (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    إذا (glyph < nCodes ())
    {
      أرجع (hb_codepoint_t)codes[glyph];
    }
    وإلا
      أرجع CFF_UNDEF_کوڈ;
  }

  HBUINT8 &nCodes () { أرجع codes.len; }
  HBUINT8 nCodes () ثابت { أرجع codes.len; }

  Array_دا<HBUINT8, HBUINT8> codes;

  DEFINE_ناپ_ARRAY_SIZED (1, codes);
};

بنية Encoding1_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   پہلا;
  HBUINT8   n_بایاں;

  DEFINE_ناپ_STATIC (2);
};

بنية Encoding1 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_کوڈ (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    لكل (بنانشانی صحيح i = 0; i < nRanges (); i++)
    {
      إذا (glyph <= ranges[i].n_بایاں)
      {
	hb_codepoint_t کوڈ = (hb_codepoint_t) ranges[i].پہلا + glyph;
	أرجع (likely (کوڈ < 0x100) ? کوڈ: CFF_UNDEF_کوڈ);
      }
      glyph -= (ranges[i].n_بایاں + 1);
    }
    أرجع CFF_UNDEF_کوڈ;
  }

  HBUINT8 &nRanges () { أرجع ranges.len; }
  HBUINT8 nRanges () ثابت { أرجع ranges.len; }

  Array_دا<Encoding1_Range, HBUINT8> ranges;

  DEFINE_ناپ_ARRAY_SIZED (1, ranges);
};

بنية SuppEncoding {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   کوڈ;
  HBUINT16  glyph;

  DEFINE_ناپ_STATIC (3);
};

بنية CFF1_Supp_Enc_انکڑے {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (supps.sanitize (c));
  }

  فراغ get_codes (hb_codepoint_t sid, hb_سمتیہ_t<hb_codepoint_t> &codes) ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < nSups (); i++)
      إذا (sid == supps[i].glyph)
	codes.push (supps[i].کوڈ);
  }

  HBUINT8 &nSups () { أرجع supps.len; }
  HBUINT8 nSups () ثابت { أرجع supps.len; }

  Array_دا<SuppEncoding, HBUINT8> supps;

  DEFINE_ناپ_ARRAY_SIZED (1, supps);
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
		  بنانشانی صحيح enc_گنتی,
		  ثابت hb_سمتیہ_t<کوڈ_pair_t>& کوڈ_ranges,
		  ثابت hb_سمتیہ_t<کوڈ_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (هذا);
    Encoding *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format | ((supp_codes.لمائی > 0) ? 0x80 : 0);
    بدل (format) {
    حالة 0:
    {
      Encoding0 *fmt0 = c->allocate_ناپ<Encoding0> (Encoding0::min_ناپ + HBUINT8::static_ناپ * enc_گنتی);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      fmt0->nCodes () = enc_گنتی;
      بنانشانی صحيح glyph = 0;
      لكل (بنانشانی صحيح i = 0; i < کوڈ_ranges.لمائی; i++)
      {
	hb_codepoint_t کوڈ = کوڈ_ranges[i].کوڈ;
	لكل (صحيح کھبے = (صحيح)کوڈ_ranges[i].glyph; کھبے >= 0; کھبے--)
	  fmt0->codes[glyph++] = کوڈ++;
	إذا (unlikely (!((glyph <= 0x100) && (کوڈ <= 0x100))))
	  return_trace (خطأ);
      }
    }
    اكسر;

    حالة 1:
    {
      Encoding1 *fmt1 = c->allocate_ناپ<Encoding1> (Encoding1::min_ناپ + Encoding1_Range::static_ناپ * کوڈ_ranges.لمائی);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      fmt1->nRanges () = کوڈ_ranges.لمائی;
      لكل (بنانشانی صحيح i = 0; i < کوڈ_ranges.لمائی; i++)
      {
	إذا (unlikely (!((کوڈ_ranges[i].کوڈ <= 0xFF) && (کوڈ_ranges[i].glyph <= 0xFF))))
	  return_trace (خطأ);
	fmt1->ranges[i].پہلا = کوڈ_ranges[i].کوڈ;
	fmt1->ranges[i].n_بایاں = کوڈ_ranges[i].glyph;
      }
    }
    اكسر;

    }

    إذا (supp_codes.لمائی)
    {
      CFF1_Supp_Enc_انکڑے *supp_انکڑے = c->allocate_ناپ<CFF1_Supp_Enc_انکڑے> (CFF1_Supp_Enc_انکڑے::min_ناپ + SuppEncoding::static_ناپ * supp_codes.لمائی);
      إذا (unlikely (!supp_انکڑے)) return_trace (خطأ);
      supp_انکڑے->nSups () = supp_codes.لمائی;
      لكل (بنانشانی صحيح i = 0; i < supp_codes.لمائی; i++)
      {
	supp_انکڑے->supps[i].کوڈ = supp_codes[i].کوڈ;
	supp_انکڑے->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (سچ);
  }

  size_t get_ناپ () ثابت
  {
    size_t ناپ = min_ناپ;
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); ناپ = hb_unsigned_جوڑو_saturate (ناپ, u.format0.get_ناپ ()); اكسر;
    حالة 1: hb_barrier (); ناپ = hb_unsigned_جوڑو_saturate (ناپ, u.format1.get_ناپ ()); اكسر;
    }
    إذا (has_supplement ())
      ناپ = hb_unsigned_جوڑو_saturate (ناپ, supp_Enc_انکڑے ().get_ناپ ());
    أرجع ناپ;
  }

  hb_codepoint_t get_کوڈ (hb_codepoint_t glyph) ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_کوڈ (glyph);
    حالة 1: hb_barrier (); أرجع u.format1.get_کوڈ (glyph);
    افتراضي:أرجع 0;
    }
  }

  uint8_t جدول_format () ثابت { أرجع format & 0x7F; }
  منطقي  has_supplement () ثابت { أرجع format & 0x80; }

  فراغ get_supplement_codes (hb_codepoint_t sid, hb_سمتیہ_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    إذا (has_supplement ())
      supp_Enc_انکڑے().get_codes (sid, codes);
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
    return_trace (likely (!has_supplement () || supp_Enc_انکڑے ().sanitize (c)));
  }

  محمي:
  ثابت CFF1_Supp_Enc_انکڑے &supp_Enc_انکڑے () ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_انکڑے> (u.format0.codes[u.format0.nCodes ()-1]);
    حالة 1: hb_barrier (); أرجع StructAfter<CFF1_Supp_Enc_انکڑے> (u.format1.ranges[u.format1.nRanges ()-1]);
    افتراضي:أرجع Null (CFF1_Supp_Enc_انکڑے);
    }
  }

  عام:
  HBUINT8	format;
  اتحاد {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_ناپ_MIN (1);
};

/* Charset */
بنية Charset0
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح عدد_glyphs, بنانشانی *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (عدد_charset_entries) *عدد_charset_entries = عدد_glyphs;
    return_trace (sids.sanitize (c, عدد_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشانی عدد_glyphs) ثابت
  {
    إذا (unlikely (glyph >= عدد_glyphs)) أرجع 0;
    إذا (unlikely (glyph == 0))
      أرجع 0;
    وإلا
      أرجع sids[glyph - 1];
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشانی صحيح عدد_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (عدد_glyphs)) أرجع;
    لكل (hb_codepoint_t gid = 1; gid < عدد_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشانی صحيح عدد_glyphs) ثابت
  {
    إذا (sid == 0)
      أرجع 0;

    لكل (بنانشانی صحيح glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      إذا (sids[glyph-1] == sid)
	أرجع glyph;
    }
    أرجع 0;
  }

  ساكن size_t get_ناپ (بنانشانی صحيح عدد_glyphs)
  {
    أكد (عدد_glyphs > 0);
    أرجع Unsized_Array_دا<HBUINT16>::get_ناپ (عدد_glyphs - 1);
  }

  Unsized_Array_دا<HBUINT16> sids;

  DEFINE_ناپ_ARRAY(0, sids);
};

سانچا <قسم_ناں TYPE>
بنية Charset_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16  پہلا;
  TYPE      n_بایاں;

  DEFINE_ناپ_STATIC (HBUINT16::static_ناپ + TYPE::static_ناپ);
};

سانچا <قسم_ناں TYPE>
بنية Charset1_2 {
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح عدد_glyphs, بنانشانی *عدد_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    عدد_glyphs--;
    بنانشانی i;
    لكل (i = 0; عدد_glyphs > 0; i++)
    {
      إذا (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (عدد_glyphs >= ranges[i].n_بایاں + 1))))
	return_trace (خطأ);
      عدد_glyphs -= (ranges[i].n_بایاں + 1);
    }
    إذا (عدد_charset_entries)
      *عدد_charset_entries = i;
    return_trace (سچ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشانی عدد_glyphs,
			  کوڈ_pair_t *cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (glyph >= عدد_glyphs)) أرجع 0;
    بنانشانی i;
    hb_codepoint_t شروع_glyph;
    إذا (cache && likely (cache->glyph <= glyph))
    {
      i = cache->کوڈ;
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
      بنانشانی گنتی = ranges[i].n_بایاں;
      إذا (glyph <= گنتی)
      {
        إذا (cache)
	  *cache = {i, شروع_glyph};
	أرجع ranges[i].پہلا + glyph;
      }
      گنتی++;
      شروع_glyph += گنتی;
      glyph -= گنتی;
    }

    أرجع 0;
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشانی صحيح عدد_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (عدد_glyphs)) أرجع;

    hb_codepoint_t gid = 1;
    إذا (gid >= عدد_glyphs)
      أرجع;

    لكل (بنانشانی i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].پہلا;
      بنانشانی گنتی = ranges[i].n_بایاں + 1;
      بنانشانی آخری = gid + گنتی;
      لكل (بنانشانی j = 0; j < گنتی; j++)
	mapping->arrayZ[gid++] = {sid++, آخری - 1};

      إذا (gid >= عدد_glyphs)
        اكسر;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشانی صحيح عدد_glyphs) ثابت
  {
    إذا (sid == 0) أرجع 0;
    hb_codepoint_t  glyph = 1;
    لكل (بنانشانی صحيح i = 0;; i++)
    {
      إذا (glyph >= عدد_glyphs)
	أرجع 0;
      إذا ((ranges[i].پہلا <= sid) && (sid <= ranges[i].پہلا + ranges[i].n_بایاں))
	أرجع glyph + (sid - ranges[i].پہلا);
      glyph += (ranges[i].n_بایاں + 1);
    }

    أرجع 0;
  }

  size_t get_ناپ (بنانشانی صحيح عدد_glyphs) ثابت
  {
    صحيح glyph = (صحيح) عدد_glyphs;
    بنانشانی عدد_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    لكل (بنانشانی صحيح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_بایاں + 1);
      عدد_ranges++;
    }

    أرجع get_ناپ_for_ranges (عدد_ranges);
  }

  ساكن size_t get_ناپ_for_ranges (بنانشانی صحيح عدد_ranges)
  {
    أرجع Unsized_Array_دا<Charset_Range<TYPE> >::get_ناپ (عدد_ranges);
  }

  Unsized_Array_دا<Charset_Range<TYPE>> ranges;

  DEFINE_ناپ_ARRAY (0, ranges);
};

تعريفنوع Charset1_2<HBUINT8>     Charset1;
تعريفنوع Charset1_2<HBUINT16>    Charset2;
تعريفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعريفنوع Charset_Range<HBUINT16> Charset2_Range;

بنية Charset
{
  /* serialize a fullset Charset */
  منطقي serialize (hb_serialize_context_t *c, ثابت Charset &src, بنانشانی صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_ناپ (عدد_glyphs)));
  }

  /* serialize a subset Charset */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  بنانشانی صحيح عدد_glyphs,
		  ثابت hb_سمتیہ_t<کوڈ_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (هذا);
    Charset *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format;
    بدل (format)
    {
    حالة 0:
    {
      Charset0 *fmt0 = c->allocate_ناپ<Charset0> (Charset0::get_ناپ (عدد_glyphs), خطأ);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      بنانشانی صحيح glyph = 0;
      لكل (بنانشانی صحيح i = 0; i < sid_ranges.لمائی; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].کوڈ;
	لكل (صحيح کھبے = (صحيح)sid_ranges.arrayZ[i].glyph; کھبے >= 0; کھبے--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    اكسر;

    حالة 1:
    {
      Charset1 *fmt1 = c->allocate_ناپ<Charset1> (Charset1::get_ناپ_for_ranges (sid_ranges.لمائی), خطأ);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بنانشانی صحيح i = 0; i < sid_ranges.لمائی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].پہلا = _.کوڈ;
	fmt1->ranges[i].n_بایاں = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (خطأ);
    }
    اكسر;

    حالة 2:
    {
      Charset2 *fmt2 = c->allocate_ناپ<Charset2> (Charset2::get_ناپ_for_ranges (sid_ranges.لمائی), خطأ);
      إذا (unlikely (!fmt2)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بنانشانی صحيح i = 0; i < sid_ranges.لمائی; i++)
      {
        خودکار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].پہلا = _.کوڈ;
	fmt2->ranges[i].n_بایاں = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (خطأ);
    }
    اكسر;

    }
    return_trace (سچ);
  }

  size_t get_ناپ (بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جوڑو_saturate (min_ناپ, u.format0.get_ناپ (عدد_glyphs));
    حالة 1: hb_barrier (); أرجع hb_unsigned_جوڑو_saturate (min_ناپ, u.format1.get_ناپ (عدد_glyphs));
    حالة 2: hb_barrier (); أرجع hb_unsigned_جوڑو_saturate (min_ناپ, u.format2.get_ناپ (عدد_glyphs));
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشانی صحيح عدد_glyphs,
			  کوڈ_pair_t *cache = خالی_سوچک) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_sid (glyph, عدد_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_sid (glyph, عدد_glyphs, cache);
    حالة 2: hb_barrier (); أرجع u.format2.get_sid (glyph, عدد_glyphs, cache);
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); u.format0.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    حالة 1: hb_barrier (); u.format1.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyph_منزل_sid_map (mapping, عدد_glyphs); أرجع;
    افتراضي:أرجع;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_glyph (sid, عدد_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_glyph (sid, عدد_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_glyph (sid, عدد_glyphs);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی *عدد_charset_entries) ثابت
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

  DEFINE_ناپ_MIN (1);
};

بنية CFF1_String_اشاریہ : CFF1_اشاریہ
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF1_String_اشاریہ &لڑیاں,
		  ثابت hb_سمتیہ_t<بنانشانی> &sidmap)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely ((لڑیاں.گنتی == 0) || (sidmap.لمائی == 0)))
    {
      إذا (unlikely (!c->extend_min (هذا->گنتی)))
	return_trace (خطأ);
      گنتی = 0;
      return_trace (سچ);
    }

    إذا (unlikely (sidmap.وچ_error ())) return_trace (خطأ);

    // Save this in a vector since serialize() iterates it twice.
    hb_سمتیہ_t<hb_ubytes_t> بائٹاں_Array (+ hb_iter (sidmap)
					 | hb_map (لڑیاں));

    إذا (unlikely (بائٹاں_Array.وچ_error ())) return_trace (خطأ);

    منطقي نتیجہ = CFF1_اشاریہ::serialize (c, بائٹاں_Array);
    return_trace (نتیجہ);
  }
};

بنية cff1_چوٹی_dict_interp_env_t : عدد_interp_env_t
{
  cff1_چوٹی_dict_interp_env_t ()
    : عدد_interp_env_t(), prev_وتھ(0), آخری_وتھ(0) {}
  cff1_چوٹی_dict_interp_env_t (ثابت hb_ubytes_t &بائٹاں)
    : عدد_interp_env_t(بائٹاں), prev_وتھ(0), آخری_وتھ(0) {}

  بنانشانی صحيح prev_وتھ;
  بنانشانی صحيح آخری_وتھ;
};

بنية name_dict_اقدار_t
{
  تعداد name_dict_val_اشاریہ_t
  {
      نسخہ,
      notice,
      copyright,
      بھریا_Name,
      familyName,
      بھار,
      postscript,
      fontName,
      ادھار_Font_Name,
      registry,
      ordering,

      Val_گنتی
  };

  فراغ init ()
  {
    لكل (بنانشانی صحيح i = 0; i < Val_گنتی; i++)
      values[i] = CFF_UNDEF_SID;
  }

  بنانشانی صحيح& عامل[] (بنانشانی صحيح i)
  { أكد (i < Val_گنتی); أرجع values[i]; }

  بنانشانی صحيح عامل[] (بنانشانی صحيح i) ثابت
  { أكد (i < Val_گنتی); أرجع values[i]; }

  ساكن تعداد name_dict_val_اشاریہ_t name_op_منزل_اشاریہ (op_کوڈ_t op)
  {
    بدل (op) {
      افتراضي: // can't happen - just make some compiler happy
      حالة Op_کوڈ_نسخہ:
	أرجع نسخہ;
      حالة Op_کوڈ_Notice:
	أرجع notice;
      حالة Op_کوڈ_Copyright:
	أرجع copyright;
      حالة Op_کوڈ_بھریا_Name:
	أرجع بھریا_Name;
      حالة Op_کوڈ_Family_Name:
	أرجع familyName;
      حالة Op_کوڈ_بھار:
	أرجع بھار;
      حالة Op_کوڈ_Post_Script:
	أرجع postscript;
      حالة Op_کوڈ_Font_Name:
	أرجع fontName;
      حالة Op_کوڈ_ادھار_Font_Name:
	أرجع ادھار_Font_Name;
    }
  }

  بنانشانی صحيح  values[Val_گنتی];
};

بنية cff1_چوٹی_dict_val_t : op_str_t
{
  بنانشانی صحيح  آخری_arg_وتھ;
};

بنية cff1_چوٹی_dict_اقدار_t : چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>
{
  فراغ init ()
  {
    چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_گنتی = 8720;
    Encoding_وتھ = 0;
    Charset_وتھ = 0;
    FDSelect_وتھ = 0;
    نجی_Dict_Info.init ();
  }
  فراغ fini () { چوٹی_dict_اقدار_t<cff1_چوٹی_dict_val_t>::fini (); }

  منطقي is_CID () ثابت
  { أرجع nameSIDs[name_dict_اقدار_t::registry] != CFF_UNDEF_SID; }

  name_dict_اقدار_t  nameSIDs;
  بنانشانی صحيح    ros_supplement_وتھ;
  بنانشانی صحيح    ros_supplement;
  بنانشانی صحيح    cid_گنتی;

  صحيح             Encoding_وتھ;
  صحيح             Charset_وتھ;
  صحيح             FDSelect_وتھ;
  جدول_info_t    نجی_Dict_Info;
};

بنية cff1_چوٹی_dict_opset_t : چوٹی_dict_opset_t<cff1_چوٹی_dict_val_t>
{
  ساكن فراغ process_op (op_کوڈ_t op, cff1_چوٹی_dict_interp_env_t& env, cff1_چوٹی_dict_اقدار_t& dictval)
  {
    cff1_چوٹی_dict_val_t  val {};

    بدل (op) {
      حالة Op_کوڈ_نسخہ:
      حالة Op_کوڈ_Notice:
      حالة Op_کوڈ_Copyright:
      حالة Op_کوڈ_بھریا_Name:
      حالة Op_کوڈ_Font_Name:
      حالة Op_کوڈ_Family_Name:
      حالة Op_کوڈ_بھار:
      حالة Op_کوڈ_Post_Script:
      حالة Op_کوڈ_ادھار_Font_Name:
	dictval.nameSIDs[name_dict_اقدار_t::name_op_منزل_اشاریہ (op)] = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_is_Fixed_Pitch:
      حالة Op_کوڈ_Italic_Angle:
      حالة Op_کوڈ_Underline_Position:
      حالة Op_کوڈ_Underline_Thickness:
      حالة Op_کوڈ_Paint_Type:
      حالة Op_کوڈ_Charstring_Type:
      حالة Op_کوڈ_Unique_ID:
      حالة Op_کوڈ_Stroke_چوڑائی:
      حالة Op_کوڈ_Synthetic_ادھار:
      حالة Op_کوڈ_CIDFont_نسخہ:
      حالة Op_کوڈ_CIDFont_Revision:
      حالة Op_کوڈ_CIDFont_Type:
      حالة Op_کوڈ_UIDBase:
      حالة Op_کوڈ_Font_BBox:
      حالة Op_کوڈ_XUID:
      حالة Op_کوڈ_ادھار_Font_Blend:
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_CIDCount:
	dictval.cid_گنتی = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_ROS:
	dictval.ros_supplement = env.arg_ڈھیر.نکالو_uint ();
	dictval.nameSIDs[name_dict_اقدار_t::ordering] = env.arg_ڈھیر.نکالو_uint ();
	dictval.nameSIDs[name_dict_اقدار_t::registry] = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	إذا (unlikely (env.وچ_error ())) أرجع;
	أكد (env.آخری_وتھ > dictval.op_شروع);
	val.آخری_arg_وتھ = (env.آخری_وتھ - 1) - dictval.op_شروع;  /* offset to the last argument */
	اكسر;

      حالة Op_کوڈ_Encoding:
	dictval.Encoding_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	إذا (unlikely (dictval.Encoding_وتھ == 0)) أرجع;
	اكسر;

      حالة Op_کوڈ_charset:
	dictval.Charset_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	إذا (unlikely (dictval.Charset_وتھ == 0)) أرجع;
	اكسر;

      حالة Op_کوڈ_FDSelect:
	dictval.FDSelect_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_نجی:
	dictval.نجی_Dict_Info.وتھ = env.arg_ڈھیر.نکالو_int ();
	dictval.نجی_Dict_Info.ناپ = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	env.آخری_وتھ = env.str_ref.get_وتھ ();
	چوٹی_dict_opset_t<cff1_چوٹی_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

بنية cff1_font_dict_اقدار_t : dict_اقدار_t<op_str_t>
{
  فراغ init ()
  {
    dict_اقدار_t<op_str_t>::init ();
    نجی_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  فراغ fini () { dict_اقدار_t<op_str_t>::fini (); }

  جدول_info_t       نجی_Dict_Info;
  بنانشانی صحيح    fontName;
};

بنية cff1_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, عدد_interp_env_t& env, cff1_font_dict_اقدار_t& dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_Font_Name:
	dictval.fontName = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_Font_میٹرکس:
      حالة Op_کوڈ_Paint_Type:
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_نجی:
	dictval.نجی_Dict_Info.وتھ = env.arg_ڈھیر.نکالو_uint ();
	dictval.نجی_Dict_Info.ناپ = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref);
  }
};

سانچا <قسم_ناں VAL>
بنية cff1_نجی_dict_اقدار_ادھار_t : dict_اقدار_t<VAL>
{
  فراغ init ()
  {
    dict_اقدار_t<VAL>::init ();
    subrs_وتھ = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  فراغ fini () { dict_اقدار_t<VAL>::fini (); }

  صحيح                 subrs_وتھ;
  ثابت CFF1Subrs    *localSubrs;
};

تعريفنوع cff1_نجی_dict_اقدار_ادھار_t<op_str_t> cff1_نجی_dict_اقدار_subset_t;
تعريفنوع cff1_نجی_dict_اقدار_ادھار_t<عدد_dict_val_t> cff1_نجی_dict_اقدار_t;

بنية cff1_نجی_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, عدد_interp_env_t& env, cff1_نجی_dict_اقدار_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Force_Bold:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Expansion_ضارب:
      حالة Op_کوڈ_initial_Random_بیج:
      حالة Op_کوڈ_default_چوڑائی_X:
      حالة Op_کوڈ_nominal_چوڑائی_X:
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_Subrs:
	dictval.subrs_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

بنية cff1_نجی_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, عدد_interp_env_t& env, cff1_نجی_dict_اقدار_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Force_Bold:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Expansion_ضارب:
      حالة Op_کوڈ_initial_Random_بیج:
      حالة Op_کوڈ_default_چوڑائی_X:
      حالة Op_کوڈ_nominal_چوڑائی_X:
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_Subrs:
	dictval.subrs_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref);
  }
};

تعريفنوع dict_interpreter_t<cff1_چوٹی_dict_opset_t, cff1_چوٹی_dict_اقدار_t, cff1_چوٹی_dict_interp_env_t> cff1_چوٹی_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_اقدار_t> cff1_font_dict_interpreter_t;

تعريفنوع CFF1_اشاریہ CFF1_Name_اشاریہ;
تعريفنوع CFF1_اشاریہ CFF1_چوٹی_Dict_اشاریہ;

بنية cff1_font_dict_اقدار_mod_t
{
  cff1_font_dict_اقدار_mod_t() { init (); }

  فراغ init () { init ( &Null (cff1_font_dict_اقدار_t), CFF_UNDEF_SID ); }

  فراغ init (ثابت cff1_font_dict_اقدار_t *ادھار_2,
	     بنانشانی صحيح fontName_)
  {
    ادھار = ادھار_2;
    fontName = fontName_;
    نجی_Dict_Info.init ();
  }

  بنانشانی get_گنتی () ثابت { أرجع ادھار->get_گنتی (); }

  ثابت op_str_t &عامل [] (بنانشانی صحيح i) ثابت { أرجع (*ادھار)[i]; }

  ثابت cff1_font_dict_اقدار_t    *ادھار;
  جدول_info_t		   نجی_Dict_Info;
  بنانشانی صحيح		fontName;
};

بنية CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  سانچا <قسم_ناں ITER, قسم_ناں OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT16>::serialize<cff1_font_dict_اقدار_mod_t, cff1_font_dict_اقدار_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

ورتوں نطاق CFF;

بنية cff1
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_CFF1;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1));
  }

  سانچا <قسم_ناں PRIVOPSET, قسم_ناں PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = cff1::جدول_ٹیگ;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      چوٹی_Dict_2.init ();
      fontDicts.init ();
      نجی_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروع_processing ();

      ثابت OT::cff1 *cff = هذا->blob->سانچا as<OT::cff1> ();

      إذا (cff == &Null (OT::cff1))
        اذهب fail;

      name_اشاریہ = &cff->name_اشاریہ (cff);
      إذا ((name_اشاریہ == &Null (CFF1_Name_اشاریہ)) || !name_اشاریہ->sanitize (&sc))
        اذهب fail;
      hb_barrier ();

      چوٹی_Dict_اشاریہ = &Struct_At_وتھ_Or_Null<CFF1_چوٹی_Dict_اشاریہ> (name_اشاریہ, name_اشاریہ->get_ناپ (), sc);
      إذا (چوٹی_Dict_اشاریہ == &Null (CFF1_چوٹی_Dict_اشاریہ) || (چوٹی_Dict_اشاریہ->گنتی == 0))
        اذهب fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t چوٹی_Dict_Str = (*چوٹی_Dict_اشاریہ)[0];
	إذا (unlikely (!چوٹی_Dict_Str.sanitize (&sc)))   اذهب fail;
	hb_barrier ();
	cff1_چوٹی_dict_interp_env_t env (چوٹی_Dict_Str);
	cff1_چوٹی_dict_interpreter_t چوٹی_interp (env);
	إذا (unlikely (!چوٹی_interp.interpret (چوٹی_Dict_2)))   اذهب fail;
      }

      إذا (is_predef_charset ())
	charset = &Null (Charset);
      وإلا
      {
	charset = &Struct_At_وتھ_Or_Null<Charset> (cff, چوٹی_Dict_2.Charset_وتھ, sc, &عدد_charset_entries);
	إذا (unlikely (charset == &Null (Charset)))   اذهب fail;
      }

      fd_گنتی = 1;
      إذا (is_CID ())
      {
	fdArray = &Struct_At_وتھ_Or_Null<CFF1FDArray> (cff, چوٹی_Dict_2.FDArray_وتھ, sc);
	fdSelect = &Struct_At_وتھ_Or_Null<CFF1FDSelect> (cff, چوٹی_Dict_2.FDSelect_وتھ, sc, fdArray->گنتی);
	إذا (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  اذهب fail;

	fd_گنتی = fdArray->گنتی;
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
	  encoding = &Struct_At_وتھ_Or_Null<Encoding> (cff, چوٹی_Dict_2.Encoding_وتھ, sc);
	  إذا (unlikely (encoding == &Null (Encoding)))   اذهب fail;
	}
      }

      string_اشاریہ = &Struct_At_وتھ_Or_Null<CFF1_String_اشاریہ> (چوٹی_Dict_اشاریہ, چوٹی_Dict_اشاریہ->get_ناپ (), sc);
      إذا (string_اشاریہ == &Null (CFF1_String_اشاریہ))
        اذهب fail;

      globalSubrs = &Struct_At_وتھ_Or_Null<CFF1Subrs> (string_اشاریہ, string_اشاریہ->get_ناپ (), sc);
      char_لڑیاں = &Struct_At_وتھ_Or_Null<CFF1_Char_لڑیاں> (cff, چوٹی_Dict_2.char_لڑیاں_وتھ, sc);
      إذا (char_لڑیاں == &Null (CFF1_Char_لڑیاں))
        اذهب fail;

      عدد_glyphs = char_لڑیاں->گنتی;
      إذا (عدد_glyphs != sc.get_عدد_glyphs ())
        اذهب fail;

      إذا (unlikely (!نجی_Dicts.resize (fd_گنتی)))
        اذهب fail;
      لكل (بنانشانی صحيح i = 0; i < fd_گنتی; i++)
	نجی_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      إذا (is_CID ())
      {
	لكل (بنانشانی صحيح i = 0; i < fd_گنتی; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  إذا (unlikely (!fontDictStr.sanitize (&sc)))   اذهب fail;
	  hb_barrier ();
	  cff1_font_dict_اقدار_t *font;
	  cff1_چوٹی_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  إذا (unlikely (fontDicts.وچ_error ()))   اذهب fail;

	  font->init ();
	  إذا (unlikely (!font_interp.interpret (*font)))   اذهب fail;
	  PRIVDICTVAL *priv = &نجی_Dicts[i];
	  ثابت hb_ubytes_t privDictStr = Struct_At_وتھ_Or_Null<UnsizedByteStr> (cff, font->نجی_Dict_Info.وتھ, sc, font->نجی_Dict_Info.ناپ).as_ubytes (font->نجی_Dict_Info.ناپ);
	  إذا (unlikely (font->نجی_Dict_Info.ناپ &&
			privDictStr == (ثابت بنانشانی حرف *) &Null (UnsizedByteStr))) اذهب fail;
	  عدد_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	  priv->localSubrs = &Struct_At_وتھ_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_وتھ, sc);
	}
      }
      وإلا  /* non-CID */
      {
	cff1_چوٹی_dict_اقدار_t *font = &چوٹی_Dict_2;
	PRIVDICTVAL *priv = &نجی_Dicts[0];

	ثابت hb_ubytes_t privDictStr = Struct_At_وتھ_Or_Null<UnsizedByteStr> (cff, font->نجی_Dict_Info.وتھ, sc, font->نجی_Dict_Info.ناپ).as_ubytes (font->نجی_Dict_Info.ناپ);
	إذا (font->نجی_Dict_Info.ناپ &&
	    unlikely (privDictStr == (ثابت بنانشانی حرف *) &Null (UnsizedByteStr))) اذهب fail;
	عدد_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	priv->localSubrs = &Struct_At_وتھ_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_وتھ, sc);
	hb_barrier ();
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.سرا_processing ();
      چوٹی_Dict_2.fini ();
      fontDicts.fini ();
      نجی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالی_سوچک;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }
    منطقي   is_CID () ثابت { أرجع چوٹی_Dict_2.is_CID (); }

    منطقي is_predef_charset () ثابت { أرجع چوٹی_Dict_2.Charset_وتھ <= ExpertSubsetCharset; }

    بنانشانی صحيح std_کوڈ_منزل_glyph (hb_codepoint_t کوڈ) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (کوڈ);
      إذا (unlikely (sid == CFF_UNDEF_SID))
	أرجع 0;

      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, عدد_glyphs);
      وإلا إذا ((چوٹی_Dict_2.Charset_وتھ == ISOAdobeCharset)
	      && (کوڈ <= 228 /*zcaron*/)) أرجع sid;
      أرجع 0;
    }

    منطقي is_predef_encoding () ثابت { أرجع چوٹی_Dict_2.Encoding_وتھ <= ExpertEncoding; }

    hb_codepoint_t glyph_منزل_کوڈ (hb_codepoint_t glyph,
				  کوڈ_pair_t *glyph_منزل_sid_cache = خالی_سوچک) ثابت
    {
      إذا (encoding != &Null (Encoding))
	أرجع encoding->get_کوڈ (glyph);
      وإلا
      {
	hb_codepoint_t sid = glyph_منزل_sid (glyph, glyph_منزل_sid_cache);
	إذا (sid == 0) أرجع 0;
	hb_codepoint_t کوڈ = 0;
	بدل (چوٹی_Dict_2.Encoding_وتھ)
	{
	حالة StandardEncoding:
	  کوڈ = lookup_standard_encoding_for_کوڈ (sid);
	  اكسر;
	حالة ExpertEncoding:
	  کوڈ = lookup_expert_encoding_for_کوڈ (sid);
	  اكسر;
	افتراضي:
	  اكسر;
	}
	أرجع کوڈ;
      }
    }

    glyph_منزل_sid_map_t *create_glyph_منزل_sid_map () ثابت
    {
      إذا (charset != &Null (Charset))
      {
	خودکار *mapping = (glyph_منزل_sid_map_t *) hb_malloc (حجم (glyph_منزل_sid_map_t));
	إذا (unlikely (!mapping)) أرجع خالی_سوچک;
	mapping = جديد (mapping) glyph_منزل_sid_map_t ();
	mapping->push (کوڈ_pair_t {0, 1});
	charset->collect_glyph_منزل_sid_map (mapping, عدد_glyphs);
	أرجع mapping;
      }
      وإلا
	أرجع خالی_سوچک;
    }

    hb_codepoint_t glyph_منزل_sid (hb_codepoint_t glyph,
				 کوڈ_pair_t *cache = خالی_سوچک) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_sid (glyph, عدد_glyphs, cache);
      وإلا
      {
	hb_codepoint_t sid = 0;
	بدل (چوٹی_Dict_2.Charset_وتھ)
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
	بدل (چوٹی_Dict_2.Charset_وتھ)
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
    hb_blob_t               *blob = خالی_سوچک;
    ثابت Encoding	    *encoding = خالی_سوچک;
    ثابت Charset	    *charset = خالی_سوچک;
    ثابت CFF1_Name_اشاریہ     *name_اشاریہ = خالی_سوچک;
    ثابت CFF1_چوٹی_Dict_اشاریہ  *چوٹی_Dict_اشاریہ = خالی_سوچک;
    ثابت CFF1_String_اشاریہ   *string_اشاریہ = خالی_سوچک;
    ثابت CFF1Subrs	    *globalSubrs = خالی_سوچک;
    ثابت CFF1_Char_لڑیاں   *char_لڑیاں = خالی_سوچک;
    ثابت CFF1FDArray       *fdArray = خالی_سوچک;
    ثابت CFF1FDSelect      *fdSelect = خالی_سوچک;
    بنانشانی صحيح	     fd_گنتی = 0;

    cff1_چوٹی_dict_اقدار_t   چوٹی_Dict_2;
    hb_سمتیہ_t<cff1_font_dict_اقدار_t>
			     fontDicts;
    hb_سمتیہ_t<PRIVDICTVAL> نجی_Dicts;

    بنانشانی صحيح	     عدد_glyphs = 0;
    بنانشانی صحيح	     عدد_charset_entries = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff1_نجی_dict_opset_t, cff1_نجی_dict_اقدار_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_ناں.سیٹ_relaxed (خالی_سوچک);

      إذا (!is_valid ()) أرجع;
      إذا (is_CID ()) أرجع;
    }
    ~accelerator_t ()
    {
      hb_sorted_سمتیہ_t<gname_t> *ناں = glyph_ناں.get_relaxed ();
      إذا (ناں)
      {
	ناں->fini ();
	hb_free (ناں);
      }
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بنانشانی صحيح buf_len) ثابت
    {
      إذا (unlikely (glyph >= عدد_glyphs)) أرجع خطأ;
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (unlikely (!buf_len)) أرجع سچ;
      hb_codepoint_t sid = glyph_منزل_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      إذا (sid < cff1_std_لڑیاں_لمائی)
      {
	hb_بائٹاں_t byte_str = cff1_std_لڑیاں (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.لمائی;
      }
      وإلا
      {
	hb_ubytes_t ubyte_str = (*string_اشاریہ)[sid - cff1_std_لڑیاں_لمائی];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.لمائی;
      }
      إذا (!str_len) أرجع خطأ;
      بنانشانی صحيح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      أرجع سچ;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (len < 0) len = لڑی_لمبائی (name);
      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      hb_sorted_سمتیہ_t<gname_t> *ناں = glyph_ناں.get_acquire ();
      إذا (unlikely (!ناں))
      {
	ناں = (hb_sorted_سمتیہ_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_سمتیہ_t<gname_t>));
	إذا (likely (ناں))
	{
	  ناں->init ();
	  /* TODO */

	  /* fill glyph names */
	  کوڈ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
	  لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_منزل_sid (gid, &glyph_منزل_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    إذا (sid < cff1_std_لڑیاں_لمائی)
	      gname.name = cff1_std_لڑیاں (sid);
	    وإلا
	    {
	      hb_ubytes_t	ustr = (*string_اشاریہ)[sid - cff1_std_لڑیاں_لمائی];
	      gname.name = hb_بائٹاں_t ((ثابت حرف*) ustr.arrayZ, ustr.لمائی);
	    }
	    إذا (unlikely (!gname.name.arrayZ))
	      gname.name = hb_بائٹاں_t ("", 0); /* To avoid nullptr. */
	    ناں->push (gname);
	  }
	  ناں->qsort ();
	}
	إذا (unlikely (!glyph_ناں.cmpexch (خالی_سوچک, ناں)))
	{
	  إذا (ناں)
	  {
	    ناں->fini ();
	    hb_free (ناں);
	  }
	  اذهب retry;
	}
      }

      gname_t key = { hb_بائٹاں_t (name, len), 0 };
      ثابت gname_t *gname = ناں ? ناں->bsearch (key) : خالی_سوچک;
      إذا (!gname) أرجع خطأ;
      hb_codepoint_t gid = sid_منزل_glyph (gname->sid);
      إذا (!gid && gname->sid) أرجع خطأ;
      *glyph = gid;
      أرجع سچ;
    }

    HB_اندرونی منطقي get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = خالی_سوچک) ثابت;
    HB_اندرونی منطقي get_رستہ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالی_سوچک) ثابت;

    خاص:
    بنية gname_t
    {
      hb_بائٹاں_t	name;
      uint16_t		sid;

      ساكن صحيح cmp (ثابت فراغ *a_, ثابت فراغ *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	بنانشانی minlen = hb_min (a->name.لمائی, b->name.لمائی);
	صحيح ret = لڑی_مقابلہ (a->name.arrayZ, b->name.arrayZ, minlen);
	إذا (ret) أرجع ret;
	أرجع a->name.لمائی - b->name.لمائی;
      }

      صحيح cmp (ثابت gname_t &a) ثابت { أرجع cmp (&a, هذا); }
    };

    بدلنجوگ hb_atomic_t<hb_sorted_سمتیہ_t<gname_t> *> glyph_ناں;

    تعريفنوع accelerator_templ_t<cff1_نجی_dict_opset_t, cff1_نجی_dict_اقدار_t> SUPER;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff1_نجی_dict_opset_subset_t, cff1_نجی_dict_اقدار_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندرونی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_اندرونی منطقي serialize (hb_serialize_context_t *c,
				بنية cff1_subset_plan &plan) ثابت;
    HB_اندرونی منطقي get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *ادھار, hb_codepoint_t *accent) ثابت;

    بدلنجوگ CFF::cff_subset_accelerator_t* cff_accelerator = خالی_سوچک;

    تعريفنوع accelerator_templ_t<cff1_نجی_dict_opset_subset_t, cff1_نجی_dict_اقدار_subset_t> SUPER;
  };

  محمي:
  HB_اندرونی ساكن hb_codepoint_t lookup_standard_encoding_for_کوڈ (hb_codepoint_t sid);
  HB_اندرونی ساكن hb_codepoint_t lookup_expert_encoding_for_کوڈ (hb_codepoint_t sid);
  HB_اندرونی ساكن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_اندرونی ساكن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_اندرونی ساكن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_اندرونی ساكن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_اندرونی ساكن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t کوڈ);

  عام:
  Fixed_نسخہ<HBUINT8> نسخہ;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_منزل<CFF1_Name_اشاریہ, HBUINT8> name_اشاریہ; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_ناپ;	  /* offset size (unused?) */

  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

بنية cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  فراغ depend (hb_depend_انکڑے_builder_t *builder) ثابت
  {
    إذا (!is_valid ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
    {
      hb_codepoint_t ادھار_gid, accent_gid;
      إذا (get_seac_components (gid, &ادھار_gid, &accent_gid))
      {
        builder->جوڑو_depend (gid, HB_ٹیگ('C','F','F',' '), ادھار_gid);
        builder->جوڑو_depend (gid, HB_ٹیگ('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#جے_انت /* HB_OT_CFF1_TABLE_HH */
