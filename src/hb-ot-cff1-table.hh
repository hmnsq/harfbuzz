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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_CFF1_جدول_HH
#وصف_ڪريو HB_OT_CFF1_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-depend-data.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

#وصف_ڪريو HB_STRING_ARRAY_NAME cff1_std_لڙيون
#وصف_ڪريو HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ضمن "hb-string-array.hh"
#وصف_رد HB_STRING_ARRAY_LIST
#وصف_رد HB_STRING_ARRAY_NAME

نطاق CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#وصف_ڪريو HB_OT_ٽيگ_CFF1 HB_ٽيگ('C','F','F',' ')

#وصف_ڪريو CFF_UNDEF_SID   CFF_UNDEF_ڪوڊ

تعداد EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
تعداد CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعريفنوع CFF1_اشاريو          CFF1_Char_لڙيون;
تعريفنوع Subrs<HBUINT16>    CFF1Subrs;

بنية CFF1FDSelect : FDSelect {};

/* Encoding */
بنية Encoding0 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_ڪوڊ (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    إذا (glyph < nCodes ())
    {
      أرجع (hb_codepoint_t)codes[glyph];
    }
    وإلا
      أرجع CFF_UNDEF_ڪوڊ;
  }

  HBUINT8 &nCodes () { أرجع codes.len; }
  HBUINT8 nCodes () ثابت { أرجع codes.len; }

  Array_جو<HBUINT8, HBUINT8> codes;

  DEFINE_سائيز_ARRAY_SIZED (1, codes);
};

بنية Encoding1_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   پهريون;
  HBUINT8   n_کاٻو;

  DEFINE_سائيز_STATIC (2);
};

بنية Encoding1 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_ڪوڊ (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    لكل (بنانشان صحيح i = 0; i < nRanges (); i++)
    {
      إذا (glyph <= ranges[i].n_کاٻو)
      {
	hb_codepoint_t ڪوڊ = (hb_codepoint_t) ranges[i].پهريون + glyph;
	أرجع (likely (ڪوڊ < 0x100) ? ڪوڊ: CFF_UNDEF_ڪوڊ);
      }
      glyph -= (ranges[i].n_کاٻو + 1);
    }
    أرجع CFF_UNDEF_ڪوڊ;
  }

  HBUINT8 &nRanges () { أرجع ranges.len; }
  HBUINT8 nRanges () ثابت { أرجع ranges.len; }

  Array_جو<Encoding1_Range, HBUINT8> ranges;

  DEFINE_سائيز_ARRAY_SIZED (1, ranges);
};

بنية SuppEncoding {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   ڪوڊ;
  HBUINT16  glyph;

  DEFINE_سائيز_STATIC (3);
};

بنية CFF1_Supp_Enc_معلومات {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (supps.sanitize (c));
  }

  فراغ get_codes (hb_codepoint_t sid, hb_ويڪٽر_t<hb_codepoint_t> &codes) ثابت
  {
    لكل (بنانشان صحيح i = 0; i < nSups (); i++)
      إذا (sid == supps[i].glyph)
	codes.push (supps[i].ڪوڊ);
  }

  HBUINT8 &nSups () { أرجع supps.len; }
  HBUINT8 nSups () ثابت { أرجع supps.len; }

  Array_جو<SuppEncoding, HBUINT8> supps;

  DEFINE_سائيز_ARRAY_SIZED (1, supps);
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
		  بنانشان صحيح enc_ڳڻپ,
		  ثابت hb_ويڪٽر_t<ڪوڊ_pair_t>& ڪوڊ_ranges,
		  ثابت hb_ويڪٽر_t<ڪوڊ_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (هذا);
    Encoding *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format | ((supp_codes.ڊيگهائي > 0) ? 0x80 : 0);
    بدل (format) {
    حالة 0:
    {
      Encoding0 *fmt0 = c->allocate_سائيز<Encoding0> (Encoding0::min_سائيز + HBUINT8::static_سائيز * enc_ڳڻپ);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      fmt0->nCodes () = enc_ڳڻپ;
      بنانشان صحيح glyph = 0;
      لكل (بنانشان صحيح i = 0; i < ڪوڊ_ranges.ڊيگهائي; i++)
      {
	hb_codepoint_t ڪوڊ = ڪوڊ_ranges[i].ڪوڊ;
	لكل (صحيح کاٻي = (صحيح)ڪوڊ_ranges[i].glyph; کاٻي >= 0; کاٻي--)
	  fmt0->codes[glyph++] = ڪوڊ++;
	إذا (unlikely (!((glyph <= 0x100) && (ڪوڊ <= 0x100))))
	  return_trace (خطأ);
      }
    }
    اكسر;

    حالة 1:
    {
      Encoding1 *fmt1 = c->allocate_سائيز<Encoding1> (Encoding1::min_سائيز + Encoding1_Range::static_سائيز * ڪوڊ_ranges.ڊيگهائي);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      fmt1->nRanges () = ڪوڊ_ranges.ڊيگهائي;
      لكل (بنانشان صحيح i = 0; i < ڪوڊ_ranges.ڊيگهائي; i++)
      {
	إذا (unlikely (!((ڪوڊ_ranges[i].ڪوڊ <= 0xFF) && (ڪوڊ_ranges[i].glyph <= 0xFF))))
	  return_trace (خطأ);
	fmt1->ranges[i].پهريون = ڪوڊ_ranges[i].ڪوڊ;
	fmt1->ranges[i].n_کاٻو = ڪوڊ_ranges[i].glyph;
      }
    }
    اكسر;

    }

    إذا (supp_codes.ڊيگهائي)
    {
      CFF1_Supp_Enc_معلومات *supp_معلومات = c->allocate_سائيز<CFF1_Supp_Enc_معلومات> (CFF1_Supp_Enc_معلومات::min_سائيز + SuppEncoding::static_سائيز * supp_codes.ڊيگهائي);
      إذا (unlikely (!supp_معلومات)) return_trace (خطأ);
      supp_معلومات->nSups () = supp_codes.ڊيگهائي;
      لكل (بنانشان صحيح i = 0; i < supp_codes.ڊيگهائي; i++)
      {
	supp_معلومات->supps[i].ڪوڊ = supp_codes[i].ڪوڊ;
	supp_معلومات->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (سچ);
  }

  size_t get_سائيز () ثابت
  {
    size_t سائيز = min_سائيز;
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); سائيز = hb_unsigned_جوڙيو_saturate (سائيز, u.format0.get_سائيز ()); اكسر;
    حالة 1: hb_barrier (); سائيز = hb_unsigned_جوڙيو_saturate (سائيز, u.format1.get_سائيز ()); اكسر;
    }
    إذا (has_supplement ())
      سائيز = hb_unsigned_جوڙيو_saturate (سائيز, supp_Enc_معلومات ().get_سائيز ());
    أرجع سائيز;
  }

  hb_codepoint_t get_ڪوڊ (hb_codepoint_t glyph) ثابت
  {
    بدل (جدول_format ())
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_ڪوڊ (glyph);
    حالة 1: hb_barrier (); أرجع u.format1.get_ڪوڊ (glyph);
    افتراضي:أرجع 0;
    }
  }

  uint8_t جدول_format () ثابت { أرجع format & 0x7F; }
  منطقي  has_supplement () ثابت { أرجع format & 0x80; }

  فراغ get_supplement_codes (hb_codepoint_t sid, hb_ويڪٽر_t<hb_codepoint_t> &codes) ثابت
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

  DEFINE_سائيز_MIN (1);
};

/* Charset */
بنية Charset0
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح انگ_glyphs, بنانشان *انگ_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (انگ_charset_entries) *انگ_charset_entries = انگ_glyphs;
    return_trace (sids.sanitize (c, انگ_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشان انگ_glyphs) ثابت
  {
    إذا (unlikely (glyph >= انگ_glyphs)) أرجع 0;
    إذا (unlikely (glyph == 0))
      أرجع 0;
    وإلا
      أرجع sids[glyph - 1];
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشان صحيح انگ_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (انگ_glyphs)) أرجع;
    لكل (hb_codepoint_t gid = 1; gid < انگ_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشان صحيح انگ_glyphs) ثابت
  {
    إذا (sid == 0)
      أرجع 0;

    لكل (بنانشان صحيح glyph = 1; glyph < انگ_glyphs; glyph++)
    {
      إذا (sids[glyph-1] == sid)
	أرجع glyph;
    }
    أرجع 0;
  }

  ساكن size_t get_سائيز (بنانشان صحيح انگ_glyphs)
  {
    أكد (انگ_glyphs > 0);
    أرجع Unsized_Array_جو<HBUINT16>::get_سائيز (انگ_glyphs - 1);
  }

  Unsized_Array_جو<HBUINT16> sids;

  DEFINE_سائيز_ARRAY(0, sids);
};

سانچو <قسم_نالو TYPE>
بنية Charset_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16  پهريون;
  TYPE      n_کاٻو;

  DEFINE_سائيز_STATIC (HBUINT16::static_سائيز + TYPE::static_سائيز);
};

سانچو <قسم_نالو TYPE>
بنية Charset1_2 {
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح انگ_glyphs, بنانشان *انگ_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    انگ_glyphs--;
    بنانشان i;
    لكل (i = 0; انگ_glyphs > 0; i++)
    {
      إذا (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (انگ_glyphs >= ranges[i].n_کاٻو + 1))))
	return_trace (خطأ);
      انگ_glyphs -= (ranges[i].n_کاٻو + 1);
    }
    إذا (انگ_charset_entries)
      *انگ_charset_entries = i;
    return_trace (سچ);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشان انگ_glyphs,
			  ڪوڊ_pair_t *cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (glyph >= انگ_glyphs)) أرجع 0;
    بنانشان i;
    hb_codepoint_t شروعات_glyph;
    إذا (cache && likely (cache->glyph <= glyph))
    {
      i = cache->ڪوڊ;
      شروعات_glyph = cache->glyph;
    }
    وإلا
    {
      إذا (unlikely (glyph == 0)) أرجع 0;
      i = 0;
      شروعات_glyph = 1;
    }
    glyph -= شروعات_glyph;
    لكل (;; i++)
    {
      بنانشان ڳڻپ = ranges[i].n_کاٻو;
      إذا (glyph <= ڳڻپ)
      {
        إذا (cache)
	  *cache = {i, شروعات_glyph};
	أرجع ranges[i].پهريون + glyph;
      }
      ڳڻپ++;
      شروعات_glyph += ڳڻپ;
      glyph -= ڳڻپ;
    }

    أرجع 0;
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشان صحيح انگ_glyphs) ثابت
  {
    إذا (!mapping->resize_dirty  (انگ_glyphs)) أرجع;

    hb_codepoint_t gid = 1;
    إذا (gid >= انگ_glyphs)
      أرجع;

    لكل (بنانشان i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].پهريون;
      بنانشان ڳڻپ = ranges[i].n_کاٻو + 1;
      بنانشان آخري = gid + ڳڻپ;
      لكل (بنانشان j = 0; j < ڳڻپ; j++)
	mapping->arrayZ[gid++] = {sid++, آخري - 1};

      إذا (gid >= انگ_glyphs)
        اكسر;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشان صحيح انگ_glyphs) ثابت
  {
    إذا (sid == 0) أرجع 0;
    hb_codepoint_t  glyph = 1;
    لكل (بنانشان صحيح i = 0;; i++)
    {
      إذا (glyph >= انگ_glyphs)
	أرجع 0;
      إذا ((ranges[i].پهريون <= sid) && (sid <= ranges[i].پهريون + ranges[i].n_کاٻو))
	أرجع glyph + (sid - ranges[i].پهريون);
      glyph += (ranges[i].n_کاٻو + 1);
    }

    أرجع 0;
  }

  size_t get_سائيز (بنانشان صحيح انگ_glyphs) ثابت
  {
    صحيح glyph = (صحيح) انگ_glyphs;
    بنانشان انگ_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    لكل (بنانشان صحيح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_کاٻو + 1);
      انگ_ranges++;
    }

    أرجع get_سائيز_for_ranges (انگ_ranges);
  }

  ساكن size_t get_سائيز_for_ranges (بنانشان صحيح انگ_ranges)
  {
    أرجع Unsized_Array_جو<Charset_Range<TYPE> >::get_سائيز (انگ_ranges);
  }

  Unsized_Array_جو<Charset_Range<TYPE>> ranges;

  DEFINE_سائيز_ARRAY (0, ranges);
};

تعريفنوع Charset1_2<HBUINT8>     Charset1;
تعريفنوع Charset1_2<HBUINT16>    Charset2;
تعريفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعريفنوع Charset_Range<HBUINT16> Charset2_Range;

بنية Charset
{
  /* serialize a fullset Charset */
  منطقي serialize (hb_serialize_context_t *c, ثابت Charset &src, بنانشان صحيح انگ_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_سائيز (انگ_glyphs)));
  }

  /* serialize a subset Charset */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  بنانشان صحيح انگ_glyphs,
		  ثابت hb_ويڪٽر_t<ڪوڊ_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (هذا);
    Charset *dest = c->extend_min (هذا);
    إذا (unlikely (!dest)) return_trace (خطأ);
    dest->format = format;
    بدل (format)
    {
    حالة 0:
    {
      Charset0 *fmt0 = c->allocate_سائيز<Charset0> (Charset0::get_سائيز (انگ_glyphs), خطأ);
      إذا (unlikely (!fmt0)) return_trace (خطأ);
      بنانشان صحيح glyph = 0;
      لكل (بنانشان صحيح i = 0; i < sid_ranges.ڊيگهائي; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].ڪوڊ;
	لكل (صحيح کاٻي = (صحيح)sid_ranges.arrayZ[i].glyph; کاٻي >= 0; کاٻي--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    اكسر;

    حالة 1:
    {
      Charset1 *fmt1 = c->allocate_سائيز<Charset1> (Charset1::get_سائيز_for_ranges (sid_ranges.ڊيگهائي), خطأ);
      إذا (unlikely (!fmt1)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بنانشان صحيح i = 0; i < sid_ranges.ڊيگهائي; i++)
      {
        خودڪار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].پهريون = _.ڪوڊ;
	fmt1->ranges[i].n_کاٻو = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (خطأ);
    }
    اكسر;

    حالة 2:
    {
      Charset2 *fmt2 = c->allocate_سائيز<Charset2> (Charset2::get_سائيز_for_ranges (sid_ranges.ڊيگهائي), خطأ);
      إذا (unlikely (!fmt2)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (بنانشان صحيح i = 0; i < sid_ranges.ڊيگهائي; i++)
      {
        خودڪار &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].پهريون = _.ڪوڊ;
	fmt2->ranges[i].n_کاٻو = _.glyph;
      }
      إذا (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (خطأ);
    }
    اكسر;

    }
    return_trace (سچ);
  }

  size_t get_سائيز (بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جوڙيو_saturate (min_سائيز, u.format0.get_سائيز (انگ_glyphs));
    حالة 1: hb_barrier (); أرجع hb_unsigned_جوڙيو_saturate (min_سائيز, u.format1.get_سائيز (انگ_glyphs));
    حالة 2: hb_barrier (); أرجع hb_unsigned_جوڙيو_saturate (min_سائيز, u.format2.get_سائيز (انگ_glyphs));
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, بنانشان صحيح انگ_glyphs,
			  ڪوڊ_pair_t *cache = خالي_اشارو) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_sid (glyph, انگ_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_sid (glyph, انگ_glyphs, cache);
    حالة 2: hb_barrier (); أرجع u.format2.get_sid (glyph, انگ_glyphs, cache);
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_glyph_منزل_sid_map (glyph_منزل_sid_map_t *mapping, بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); u.format0.collect_glyph_منزل_sid_map (mapping, انگ_glyphs); أرجع;
    حالة 1: hb_barrier (); u.format1.collect_glyph_منزل_sid_map (mapping, انگ_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyph_منزل_sid_map (mapping, انگ_glyphs); أرجع;
    افتراضي:أرجع;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_glyph (sid, انگ_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_glyph (sid, انگ_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_glyph (sid, انگ_glyphs);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان *انگ_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_انگ_glyphs (), انگ_charset_entries));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_انگ_glyphs (), انگ_charset_entries));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_انگ_glyphs (), انگ_charset_entries));
    افتراضي:return_trace (خطأ);
    }
  }

  HBUINT8       format;
  اتحاد {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_سائيز_MIN (1);
};

بنية CFF1_String_اشاريو : CFF1_اشاريو
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF1_String_اشاريو &لڙيون,
		  ثابت hb_ويڪٽر_t<بنانشان> &sidmap)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely ((لڙيون.ڳڻپ == 0) || (sidmap.ڊيگهائي == 0)))
    {
      إذا (unlikely (!c->extend_min (هذا->ڳڻپ)))
	return_trace (خطأ);
      ڳڻپ = 0;
      return_trace (سچ);
    }

    إذا (unlikely (sidmap.اندريون_error ())) return_trace (خطأ);

    // Save this in a vector since serialize() iterates it twice.
    hb_ويڪٽر_t<hb_ubytes_t> بائيٽون_Array (+ hb_iter (sidmap)
					 | hb_map (لڙيون));

    إذا (unlikely (بائيٽون_Array.اندريون_error ())) return_trace (خطأ);

    منطقي نتيجو = CFF1_اشاريو::serialize (c, بائيٽون_Array);
    return_trace (نتيجو);
  }
};

بنية cff1_چوٽي_dict_interp_env_t : انگ_interp_env_t
{
  cff1_چوٽي_dict_interp_env_t ()
    : انگ_interp_env_t(), prev_وٿي(0), آخري_وٿي(0) {}
  cff1_چوٽي_dict_interp_env_t (ثابت hb_ubytes_t &بائيٽون)
    : انگ_interp_env_t(بائيٽون), prev_وٿي(0), آخري_وٿي(0) {}

  بنانشان صحيح prev_وٿي;
  بنانشان صحيح آخري_وٿي;
};

بنية name_dict_قدرون_t
{
  تعداد name_dict_val_اشاريو_t
  {
      نسخو,
      notice,
      copyright,
      ڀريل_Name,
      familyName,
      وزن,
      postscript,
      fontName,
      بنياد_Font_Name,
      registry,
      ordering,

      Val_ڳڻپ
  };

  فراغ init ()
  {
    لكل (بنانشان صحيح i = 0; i < Val_ڳڻپ; i++)
      values[i] = CFF_UNDEF_SID;
  }

  بنانشان صحيح& عامل[] (بنانشان صحيح i)
  { أكد (i < Val_ڳڻپ); أرجع values[i]; }

  بنانشان صحيح عامل[] (بنانشان صحيح i) ثابت
  { أكد (i < Val_ڳڻپ); أرجع values[i]; }

  ساكن تعداد name_dict_val_اشاريو_t name_op_منزل_اشاريو (op_ڪوڊ_t op)
  {
    بدل (op) {
      افتراضي: // can't happen - just make some compiler happy
      حالة Op_ڪوڊ_نسخو:
	أرجع نسخو;
      حالة Op_ڪوڊ_Notice:
	أرجع notice;
      حالة Op_ڪوڊ_Copyright:
	أرجع copyright;
      حالة Op_ڪوڊ_ڀريل_Name:
	أرجع ڀريل_Name;
      حالة Op_ڪوڊ_Family_Name:
	أرجع familyName;
      حالة Op_ڪوڊ_وزن:
	أرجع وزن;
      حالة Op_ڪوڊ_Post_Script:
	أرجع postscript;
      حالة Op_ڪوڊ_Font_Name:
	أرجع fontName;
      حالة Op_ڪوڊ_بنياد_Font_Name:
	أرجع بنياد_Font_Name;
    }
  }

  بنانشان صحيح  values[Val_ڳڻپ];
};

بنية cff1_چوٽي_dict_val_t : op_str_t
{
  بنانشان صحيح  آخري_arg_وٿي;
};

بنية cff1_چوٽي_dict_قدرون_t : چوٽي_dict_قدرون_t<cff1_چوٽي_dict_val_t>
{
  فراغ init ()
  {
    چوٽي_dict_قدرون_t<cff1_چوٽي_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_ڳڻپ = 8720;
    Encoding_وٿي = 0;
    Charset_وٿي = 0;
    FDSelect_وٿي = 0;
    ذاتي_Dict_Info.init ();
  }
  فراغ fini () { چوٽي_dict_قدرون_t<cff1_چوٽي_dict_val_t>::fini (); }

  منطقي is_CID () ثابت
  { أرجع nameSIDs[name_dict_قدرون_t::registry] != CFF_UNDEF_SID; }

  name_dict_قدرون_t  nameSIDs;
  بنانشان صحيح    ros_supplement_وٿي;
  بنانشان صحيح    ros_supplement;
  بنانشان صحيح    cid_ڳڻپ;

  صحيح             Encoding_وٿي;
  صحيح             Charset_وٿي;
  صحيح             FDSelect_وٿي;
  جدول_info_t    ذاتي_Dict_Info;
};

بنية cff1_چوٽي_dict_opset_t : چوٽي_dict_opset_t<cff1_چوٽي_dict_val_t>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff1_چوٽي_dict_interp_env_t& env, cff1_چوٽي_dict_قدرون_t& dictval)
  {
    cff1_چوٽي_dict_val_t  val {};

    بدل (op) {
      حالة Op_ڪوڊ_نسخو:
      حالة Op_ڪوڊ_Notice:
      حالة Op_ڪوڊ_Copyright:
      حالة Op_ڪوڊ_ڀريل_Name:
      حالة Op_ڪوڊ_Font_Name:
      حالة Op_ڪوڊ_Family_Name:
      حالة Op_ڪوڊ_وزن:
      حالة Op_ڪوڊ_Post_Script:
      حالة Op_ڪوڊ_بنياد_Font_Name:
	dictval.nameSIDs[name_dict_قدرون_t::name_op_منزل_اشاريو (op)] = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_is_Fixed_Pitch:
      حالة Op_ڪوڊ_Italic_Angle:
      حالة Op_ڪوڊ_Underline_Position:
      حالة Op_ڪوڊ_Underline_Thickness:
      حالة Op_ڪوڊ_Paint_Type:
      حالة Op_ڪوڊ_Charstring_Type:
      حالة Op_ڪوڊ_Unique_ID:
      حالة Op_ڪوڊ_Stroke_ويڪر:
      حالة Op_ڪوڊ_Synthetic_بنياد:
      حالة Op_ڪوڊ_CIDFont_نسخو:
      حالة Op_ڪوڊ_CIDFont_Revision:
      حالة Op_ڪوڊ_CIDFont_Type:
      حالة Op_ڪوڊ_UIDBase:
      حالة Op_ڪوڊ_Font_BBox:
      حالة Op_ڪوڊ_XUID:
      حالة Op_ڪوڊ_بنياد_Font_Blend:
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_CIDCount:
	dictval.cid_ڳڻپ = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_ROS:
	dictval.ros_supplement = env.arg_ڍير.ڪڍو_uint ();
	dictval.nameSIDs[name_dict_قدرون_t::ordering] = env.arg_ڍير.ڪڍو_uint ();
	dictval.nameSIDs[name_dict_قدرون_t::registry] = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	إذا (unlikely (env.اندريون_error ())) أرجع;
	أكد (env.آخري_وٿي > dictval.op_شروعات);
	val.آخري_arg_وٿي = (env.آخري_وٿي - 1) - dictval.op_شروعات;  /* offset to the last argument */
	اكسر;

      حالة Op_ڪوڊ_Encoding:
	dictval.Encoding_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	إذا (unlikely (dictval.Encoding_وٿي == 0)) أرجع;
	اكسر;

      حالة Op_ڪوڊ_charset:
	dictval.Charset_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	إذا (unlikely (dictval.Charset_وٿي == 0)) أرجع;
	اكسر;

      حالة Op_ڪوڊ_FDSelect:
	dictval.FDSelect_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_ذاتي:
	dictval.ذاتي_Dict_Info.وٿي = env.arg_ڍير.ڪڍو_int ();
	dictval.ذاتي_Dict_Info.سائيز = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	env.آخري_وٿي = env.str_ref.get_وٿي ();
	چوٽي_dict_opset_t<cff1_چوٽي_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref, val);
  }
};

بنية cff1_font_dict_قدرون_t : dict_قدرون_t<op_str_t>
{
  فراغ init ()
  {
    dict_قدرون_t<op_str_t>::init ();
    ذاتي_Dict_Info.init ();
    fontName = CFF_UNDEF_SID;
  }
  فراغ fini () { dict_قدرون_t<op_str_t>::fini (); }

  جدول_info_t       ذاتي_Dict_Info;
  بنانشان صحيح    fontName;
};

بنية cff1_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, انگ_interp_env_t& env, cff1_font_dict_قدرون_t& dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Font_Name:
	dictval.fontName = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_Font_ميٽرڪس:
      حالة Op_ڪوڊ_Paint_Type:
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_ذاتي:
	dictval.ذاتي_Dict_Info.وٿي = env.arg_ڍير.ڪڍو_uint ();
	dictval.ذاتي_Dict_Info.سائيز = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref);
  }
};

سانچو <قسم_نالو VAL>
بنية cff1_ذاتي_dict_قدرون_بنياد_t : dict_قدرون_t<VAL>
{
  فراغ init ()
  {
    dict_قدرون_t<VAL>::init ();
    subrs_وٿي = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  فراغ fini () { dict_قدرون_t<VAL>::fini (); }

  صحيح                 subrs_وٿي;
  ثابت CFF1Subrs    *localSubrs;
};

تعريفنوع cff1_ذاتي_dict_قدرون_بنياد_t<op_str_t> cff1_ذاتي_dict_قدرون_subset_t;
تعريفنوع cff1_ذاتي_dict_قدرون_بنياد_t<انگ_dict_val_t> cff1_ذاتي_dict_قدرون_t;

بنية cff1_ذاتي_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, انگ_interp_env_t& env, cff1_ذاتي_dict_قدرون_t& dictval)
  {
    انگ_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Force_Bold:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
      حالة Op_ڪوڊ_initial_Random_ٻج:
      حالة Op_ڪوڊ_default_ويڪر_X:
      حالة Op_ڪوڊ_nominal_ويڪر_X:
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_Subrs:
	dictval.subrs_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref, val);
  }
};

بنية cff1_ذاتي_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, انگ_interp_env_t& env, cff1_ذاتي_dict_قدرون_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Force_Bold:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
      حالة Op_ڪوڊ_initial_Random_ٻج:
      حالة Op_ڪوڊ_default_ويڪر_X:
      حالة Op_ڪوڊ_nominal_ويڪر_X:
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_Subrs:
	dictval.subrs_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref);
  }
};

تعريفنوع dict_interpreter_t<cff1_چوٽي_dict_opset_t, cff1_چوٽي_dict_قدرون_t, cff1_چوٽي_dict_interp_env_t> cff1_چوٽي_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_قدرون_t> cff1_font_dict_interpreter_t;

تعريفنوع CFF1_اشاريو CFF1_Name_اشاريو;
تعريفنوع CFF1_اشاريو CFF1_چوٽي_Dict_اشاريو;

بنية cff1_font_dict_قدرون_mod_t
{
  cff1_font_dict_قدرون_mod_t() { init (); }

  فراغ init () { init ( &Null (cff1_font_dict_قدرون_t), CFF_UNDEF_SID ); }

  فراغ init (ثابت cff1_font_dict_قدرون_t *بنياد_2,
	     بنانشان صحيح fontName_)
  {
    بنياد = بنياد_2;
    fontName = fontName_;
    ذاتي_Dict_Info.init ();
  }

  بنانشان get_ڳڻپ () ثابت { أرجع بنياد->get_ڳڻپ (); }

  ثابت op_str_t &عامل [] (بنانشان صحيح i) ثابت { أرجع (*بنياد)[i]; }

  ثابت cff1_font_dict_قدرون_t    *بنياد;
  جدول_info_t		   ذاتي_Dict_Info;
  بنانشان صحيح		fontName;
};

بنية CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  سانچو <قسم_نالو ITER, قسم_نالو OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT16>::serialize<cff1_font_dict_قدرون_mod_t, cff1_font_dict_قدرون_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

استعمال نطاق CFF;

بنية cff1
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_CFF1;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1));
  }

  سانچو <قسم_نالو PRIVOPSET, قسم_نالو PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = cff1::جدول_ٽيگ;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      چوٽي_Dict_2.init ();
      fontDicts.init ();
      ذاتي_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروعات_processing ();

      ثابت OT::cff1 *cff = هذا->blob->سانچو as<OT::cff1> ();

      إذا (cff == &Null (OT::cff1))
        اذهب fail;

      name_اشاريو = &cff->name_اشاريو (cff);
      إذا ((name_اشاريو == &Null (CFF1_Name_اشاريو)) || !name_اشاريو->sanitize (&sc))
        اذهب fail;
      hb_barrier ();

      چوٽي_Dict_اشاريو = &Struct_At_وٿي_Or_Null<CFF1_چوٽي_Dict_اشاريو> (name_اشاريو, name_اشاريو->get_سائيز (), sc);
      إذا (چوٽي_Dict_اشاريو == &Null (CFF1_چوٽي_Dict_اشاريو) || (چوٽي_Dict_اشاريو->ڳڻپ == 0))
        اذهب fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t چوٽي_Dict_Str = (*چوٽي_Dict_اشاريو)[0];
	إذا (unlikely (!چوٽي_Dict_Str.sanitize (&sc)))   اذهب fail;
	hb_barrier ();
	cff1_چوٽي_dict_interp_env_t env (چوٽي_Dict_Str);
	cff1_چوٽي_dict_interpreter_t چوٽي_interp (env);
	إذا (unlikely (!چوٽي_interp.interpret (چوٽي_Dict_2)))   اذهب fail;
      }

      إذا (is_predef_charset ())
	charset = &Null (Charset);
      وإلا
      {
	charset = &Struct_At_وٿي_Or_Null<Charset> (cff, چوٽي_Dict_2.Charset_وٿي, sc, &انگ_charset_entries);
	إذا (unlikely (charset == &Null (Charset)))   اذهب fail;
      }

      fd_ڳڻپ = 1;
      إذا (is_CID ())
      {
	fdArray = &Struct_At_وٿي_Or_Null<CFF1FDArray> (cff, چوٽي_Dict_2.FDArray_وٿي, sc);
	fdSelect = &Struct_At_وٿي_Or_Null<CFF1FDSelect> (cff, چوٽي_Dict_2.FDSelect_وٿي, sc, fdArray->ڳڻپ);
	إذا (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  اذهب fail;

	fd_ڳڻپ = fdArray->ڳڻپ;
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
	  encoding = &Struct_At_وٿي_Or_Null<Encoding> (cff, چوٽي_Dict_2.Encoding_وٿي, sc);
	  إذا (unlikely (encoding == &Null (Encoding)))   اذهب fail;
	}
      }

      string_اشاريو = &Struct_At_وٿي_Or_Null<CFF1_String_اشاريو> (چوٽي_Dict_اشاريو, چوٽي_Dict_اشاريو->get_سائيز (), sc);
      إذا (string_اشاريو == &Null (CFF1_String_اشاريو))
        اذهب fail;

      globalSubrs = &Struct_At_وٿي_Or_Null<CFF1Subrs> (string_اشاريو, string_اشاريو->get_سائيز (), sc);
      char_لڙيون = &Struct_At_وٿي_Or_Null<CFF1_Char_لڙيون> (cff, چوٽي_Dict_2.char_لڙيون_وٿي, sc);
      إذا (char_لڙيون == &Null (CFF1_Char_لڙيون))
        اذهب fail;

      انگ_glyphs = char_لڙيون->ڳڻپ;
      إذا (انگ_glyphs != sc.get_انگ_glyphs ())
        اذهب fail;

      إذا (unlikely (!ذاتي_Dicts.resize (fd_ڳڻپ)))
        اذهب fail;
      لكل (بنانشان صحيح i = 0; i < fd_ڳڻپ; i++)
	ذاتي_Dicts[i].init ();

      // parse CID font dicts and gather private dicts
      إذا (is_CID ())
      {
	لكل (بنانشان صحيح i = 0; i < fd_ڳڻپ; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  إذا (unlikely (!fontDictStr.sanitize (&sc)))   اذهب fail;
	  hb_barrier ();
	  cff1_font_dict_قدرون_t *font;
	  cff1_چوٽي_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  إذا (unlikely (fontDicts.اندريون_error ()))   اذهب fail;

	  font->init ();
	  إذا (unlikely (!font_interp.interpret (*font)))   اذهب fail;
	  PRIVDICTVAL *priv = &ذاتي_Dicts[i];
	  ثابت hb_ubytes_t privDictStr = Struct_At_وٿي_Or_Null<UnsizedByteStr> (cff, font->ذاتي_Dict_Info.وٿي, sc, font->ذاتي_Dict_Info.سائيز).as_ubytes (font->ذاتي_Dict_Info.سائيز);
	  إذا (unlikely (font->ذاتي_Dict_Info.سائيز &&
			privDictStr == (ثابت بنانشان حرف *) &Null (UnsizedByteStr))) اذهب fail;
	  انگ_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	  priv->localSubrs = &Struct_At_وٿي_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_وٿي, sc);
	}
      }
      وإلا  /* non-CID */
      {
	cff1_چوٽي_dict_قدرون_t *font = &چوٽي_Dict_2;
	PRIVDICTVAL *priv = &ذاتي_Dicts[0];

	ثابت hb_ubytes_t privDictStr = Struct_At_وٿي_Or_Null<UnsizedByteStr> (cff, font->ذاتي_Dict_Info.وٿي, sc, font->ذاتي_Dict_Info.سائيز).as_ubytes (font->ذاتي_Dict_Info.سائيز);
	إذا (font->ذاتي_Dict_Info.سائيز &&
	    unlikely (privDictStr == (ثابت بنانشان حرف *) &Null (UnsizedByteStr))) اذهب fail;
	انگ_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	إذا (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	priv->localSubrs = &Struct_At_وٿي_Or_Null<CFF1Subrs> (&privDictStr, priv->subrs_وٿي, sc);
	hb_barrier ();
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.پڇاڙي_processing ();
      چوٽي_Dict_2.fini ();
      fontDicts.fini ();
      ذاتي_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالي_اشارو;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }
    منطقي   is_CID () ثابت { أرجع چوٽي_Dict_2.is_CID (); }

    منطقي is_predef_charset () ثابت { أرجع چوٽي_Dict_2.Charset_وٿي <= ExpertSubsetCharset; }

    بنانشان صحيح std_ڪوڊ_منزل_glyph (hb_codepoint_t ڪوڊ) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (ڪوڊ);
      إذا (unlikely (sid == CFF_UNDEF_SID))
	أرجع 0;

      إذا (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, انگ_glyphs);
      وإلا إذا ((چوٽي_Dict_2.Charset_وٿي == ISOAdobeCharset)
	      && (ڪوڊ <= 228 /*zcaron*/)) أرجع sid;
      أرجع 0;
    }

    منطقي is_predef_encoding () ثابت { أرجع چوٽي_Dict_2.Encoding_وٿي <= ExpertEncoding; }

    hb_codepoint_t glyph_منزل_ڪوڊ (hb_codepoint_t glyph,
				  ڪوڊ_pair_t *glyph_منزل_sid_cache = خالي_اشارو) ثابت
    {
      إذا (encoding != &Null (Encoding))
	أرجع encoding->get_ڪوڊ (glyph);
      وإلا
      {
	hb_codepoint_t sid = glyph_منزل_sid (glyph, glyph_منزل_sid_cache);
	إذا (sid == 0) أرجع 0;
	hb_codepoint_t ڪوڊ = 0;
	بدل (چوٽي_Dict_2.Encoding_وٿي)
	{
	حالة StandardEncoding:
	  ڪوڊ = lookup_standard_encoding_for_ڪوڊ (sid);
	  اكسر;
	حالة ExpertEncoding:
	  ڪوڊ = lookup_expert_encoding_for_ڪوڊ (sid);
	  اكسر;
	افتراضي:
	  اكسر;
	}
	أرجع ڪوڊ;
      }
    }

    glyph_منزل_sid_map_t *create_glyph_منزل_sid_map () ثابت
    {
      إذا (charset != &Null (Charset))
      {
	خودڪار *mapping = (glyph_منزل_sid_map_t *) hb_malloc (حجم (glyph_منزل_sid_map_t));
	إذا (unlikely (!mapping)) أرجع خالي_اشارو;
	mapping = جديد (mapping) glyph_منزل_sid_map_t ();
	mapping->push (ڪوڊ_pair_t {0, 1});
	charset->collect_glyph_منزل_sid_map (mapping, انگ_glyphs);
	أرجع mapping;
      }
      وإلا
	أرجع خالي_اشارو;
    }

    hb_codepoint_t glyph_منزل_sid (hb_codepoint_t glyph,
				 ڪوڊ_pair_t *cache = خالي_اشارو) ثابت
    {
      إذا (charset != &Null (Charset))
	أرجع charset->get_sid (glyph, انگ_glyphs, cache);
      وإلا
      {
	hb_codepoint_t sid = 0;
	بدل (چوٽي_Dict_2.Charset_وٿي)
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
	أرجع charset->get_glyph (sid, انگ_glyphs);
      وإلا
      {
	hb_codepoint_t glyph = 0;
	بدل (چوٽي_Dict_2.Charset_وٿي)
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
    hb_blob_t               *blob = خالي_اشارو;
    ثابت Encoding	    *encoding = خالي_اشارو;
    ثابت Charset	    *charset = خالي_اشارو;
    ثابت CFF1_Name_اشاريو     *name_اشاريو = خالي_اشارو;
    ثابت CFF1_چوٽي_Dict_اشاريو  *چوٽي_Dict_اشاريو = خالي_اشارو;
    ثابت CFF1_String_اشاريو   *string_اشاريو = خالي_اشارو;
    ثابت CFF1Subrs	    *globalSubrs = خالي_اشارو;
    ثابت CFF1_Char_لڙيون   *char_لڙيون = خالي_اشارو;
    ثابت CFF1FDArray       *fdArray = خالي_اشارو;
    ثابت CFF1FDSelect      *fdSelect = خالي_اشارو;
    بنانشان صحيح	     fd_ڳڻپ = 0;

    cff1_چوٽي_dict_قدرون_t   چوٽي_Dict_2;
    hb_ويڪٽر_t<cff1_font_dict_قدرون_t>
			     fontDicts;
    hb_ويڪٽر_t<PRIVDICTVAL> ذاتي_Dicts;

    بنانشان صحيح	     انگ_glyphs = 0;
    بنانشان صحيح	     انگ_charset_entries = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff1_ذاتي_dict_opset_t, cff1_ذاتي_dict_قدرون_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_نالا.سيٽ_relaxed (خالي_اشارو);

      إذا (!is_valid ()) أرجع;
      إذا (is_CID ()) أرجع;
    }
    ~accelerator_t ()
    {
      hb_sorted_ويڪٽر_t<gname_t> *نالا = glyph_نالا.get_relaxed ();
      إذا (نالا)
      {
	نالا->fini ();
	hb_free (نالا);
      }
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بنانشان صحيح buf_len) ثابت
    {
      إذا (unlikely (glyph >= انگ_glyphs)) أرجع خطأ;
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (unlikely (!buf_len)) أرجع سچ;
      hb_codepoint_t sid = glyph_منزل_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      إذا (sid < cff1_std_لڙيون_ڊيگهائي)
      {
	hb_بائيٽون_t byte_str = cff1_std_لڙيون (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.ڊيگهائي;
      }
      وإلا
      {
	hb_ubytes_t ubyte_str = (*string_اشاريو)[sid - cff1_std_لڙيون_ڊيگهائي];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.ڊيگهائي;
      }
      إذا (!str_len) أرجع خطأ;
      بنانشان صحيح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      أرجع سچ;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      إذا (unlikely (!is_valid ())) أرجع خطأ;
      إذا (is_CID()) أرجع خطأ;
      إذا (len < 0) len = لڙي_ڊيگهه (name);
      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      hb_sorted_ويڪٽر_t<gname_t> *نالا = glyph_نالا.get_acquire ();
      إذا (unlikely (!نالا))
      {
	نالا = (hb_sorted_ويڪٽر_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_ويڪٽر_t<gname_t>));
	إذا (likely (نالا))
	{
	  نالا->init ();
	  /* TODO */

	  /* fill glyph names */
	  ڪوڊ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
	  لكل (hb_codepoint_t gid = 0; gid < انگ_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_منزل_sid (gid, &glyph_منزل_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    إذا (sid < cff1_std_لڙيون_ڊيگهائي)
	      gname.name = cff1_std_لڙيون (sid);
	    وإلا
	    {
	      hb_ubytes_t	ustr = (*string_اشاريو)[sid - cff1_std_لڙيون_ڊيگهائي];
	      gname.name = hb_بائيٽون_t ((ثابت حرف*) ustr.arrayZ, ustr.ڊيگهائي);
	    }
	    إذا (unlikely (!gname.name.arrayZ))
	      gname.name = hb_بائيٽون_t ("", 0); /* To avoid nullptr. */
	    نالا->push (gname);
	  }
	  نالا->qsort ();
	}
	إذا (unlikely (!glyph_نالا.cmpexch (خالي_اشارو, نالا)))
	{
	  إذا (نالا)
	  {
	    نالا->fini ();
	    hb_free (نالا);
	  }
	  اذهب retry;
	}
      }

      gname_t key = { hb_بائيٽون_t (name, len), 0 };
      ثابت gname_t *gname = نالا ? نالا->bsearch (key) : خالي_اشارو;
      إذا (!gname) أرجع خطأ;
      hb_codepoint_t gid = sid_منزل_glyph (gname->sid);
      إذا (!gid && gname->sid) أرجع خطأ;
      *glyph = gid;
      أرجع سچ;
    }

    HB_اندروني منطقي get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = خالي_اشارو) ثابت;
    HB_اندروني منطقي get_رستو (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالي_اشارو) ثابت;

    خاص:
    بنية gname_t
    {
      hb_بائيٽون_t	name;
      uint16_t		sid;

      ساكن صحيح cmp (ثابت فراغ *a_, ثابت فراغ *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	بنانشان minlen = hb_min (a->name.ڊيگهائي, b->name.ڊيگهائي);
	صحيح ret = لڙي_ڀيٽ (a->name.arrayZ, b->name.arrayZ, minlen);
	إذا (ret) أرجع ret;
	أرجع a->name.ڊيگهائي - b->name.ڊيگهائي;
      }

      صحيح cmp (ثابت gname_t &a) ثابت { أرجع cmp (&a, هذا); }
    };

    بدلجندڙ hb_atomic_t<hb_sorted_ويڪٽر_t<gname_t> *> glyph_نالا;

    تعريفنوع accelerator_templ_t<cff1_ذاتي_dict_opset_t, cff1_ذاتي_dict_قدرون_t> SUPER;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff1_ذاتي_dict_opset_subset_t, cff1_ذاتي_dict_قدرون_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندروني منطقي subset (hb_subset_context_t *c) ثابت;
    HB_اندروني منطقي serialize (hb_serialize_context_t *c,
				بنية cff1_subset_plan &plan) ثابت;
    HB_اندروني منطقي get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *بنياد, hb_codepoint_t *accent) ثابت;

    بدلجندڙ CFF::cff_subset_accelerator_t* cff_accelerator = خالي_اشارو;

    تعريفنوع accelerator_templ_t<cff1_ذاتي_dict_opset_subset_t, cff1_ذاتي_dict_قدرون_subset_t> SUPER;
  };

  محمي:
  HB_اندروني ساكن hb_codepoint_t lookup_standard_encoding_for_ڪوڊ (hb_codepoint_t sid);
  HB_اندروني ساكن hb_codepoint_t lookup_expert_encoding_for_ڪوڊ (hb_codepoint_t sid);
  HB_اندروني ساكن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_اندروني ساكن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_اندروني ساكن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_اندروني ساكن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_اندروني ساكن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t ڪوڊ);

  عام:
  Fixed_نسخو<HBUINT8> نسخو;	  /* Version of CFF table. set to 0x0100u */
  NNOffset_منزل<CFF1_Name_اشاريو, HBUINT8> name_اشاريو; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_سائيز;	  /* offset size (unused?) */

  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

بنية cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  فراغ depend (hb_depend_معلومات_builder_t *builder) ثابت
  {
    إذا (!is_valid ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < انگ_glyphs; gid++)
    {
      hb_codepoint_t بنياد_gid, accent_gid;
      إذا (get_seac_components (gid, &بنياد_gid, &accent_gid))
      {
        builder->جوڙيو_depend (gid, HB_ٽيگ('C','F','F',' '), بنياد_gid);
        builder->جوڙيو_depend (gid, HB_ٽيگ('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#جيڪڏهن_پورو /* HB_OT_CFF1_TABLE_HH */
