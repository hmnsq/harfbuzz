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

#ifndef HB_OT_CFF1_TABLE_HH
#define HB_OT_CFF1_TABLE_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-depend-data.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#ضمن "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

نطاق CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

تعداد EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
تعداد CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعريفنوع CFF1Index          CFF1CharStrings;
تعريفنوع Subrs<HBUINT16>    CFF1Subrs;

بنية CFF1FDSelect : FDSelect {};

/* Encoding */
بنية Encoding0 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    𐡄𐡍 (glyph < nCodes ())
    {
      أرجع (hb_codepoint_t)codes[glyph];
    }
    وإلا
      أرجع CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { أرجع codes.len; }
  HBUINT8 nCodes () ثابت { أرجع codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

بنية Encoding1_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   first;
  HBUINT8   nLeft;

  DEFINE_SIZE_STATIC (2);
};

بنية Encoding1 {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    لكل (unsigned صحيح i = 0; i < nRanges (); i++)
    {
      𐡄𐡍 (glyph <= ranges[i].nLeft)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	أرجع (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].nLeft + 1);
    }
    أرجع CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { أرجع ranges.len; }
  HBUINT8 nRanges () ثابت { أرجع ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

بنية SuppEncoding {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

بنية CFF1SuppEncData {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (supps.sanitize (c));
  }

  فراغ get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ثابت
  {
    لكل (unsigned صحيح i = 0; i < nSups (); i++)
      𐡄𐡍 (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { أرجع supps.len; }
  HBUINT8 nSups () ثابت { أرجع supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
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
		  unsigned صحيح enc_count,
		  ثابت hb_vector_t<code_pair_t>& code_ranges,
		  ثابت hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (هذا);
    Encoding *dest = c->extend_min (هذا);
    𐡄𐡍 (unlikely (!dest)) return_trace (خطأ);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    بدل (format) {
    حالة 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_count);
      𐡄𐡍 (unlikely (!fmt0)) return_trace (خطأ);
      fmt0->nCodes () = enc_count;
      unsigned صحيح glyph = 0;
      لكل (unsigned صحيح i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	لكل (صحيح left = (صحيح)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	𐡄𐡍 (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (خطأ);
      }
    }
    اكسر;

    حالة 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      𐡄𐡍 (unlikely (!fmt1)) return_trace (خطأ);
      fmt1->nRanges () = code_ranges.length;
      لكل (unsigned صحيح i = 0; i < code_ranges.length; i++)
      {
	𐡄𐡍 (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (خطأ);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].nLeft = code_ranges[i].glyph;
      }
    }
    اكسر;

    }

    𐡄𐡍 (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      𐡄𐡍 (unlikely (!suppData)) return_trace (خطأ);
      suppData->nSups () = supp_codes.length;
      لكل (unsigned صحيح i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (𐡒𐡔𐡈);
  }

  size_t get_size () ثابت
  {
    size_t size = min_size;
    بدل (table_format ())
    {
    حالة 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); اكسر;
    حالة 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); اكسر;
    }
    𐡄𐡍 (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    أرجع size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    بدل (table_format ())
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_code (glyph);
    حالة 1: hb_barrier (); أرجع u.format1.get_code (glyph);
    افتراضي:أرجع 0;
    }
  }

  uint8_t table_format () ثابت { أرجع format & 0x7F; }
  منطقي  has_supplement () ثابت { أرجع format & 0x80; }

  فراغ get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    𐡄𐡍 (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (table_format ())
    {
    حالة 0: hb_barrier (); 𐡄𐡍 (unlikely (!u.format0.sanitize (c))) { return_trace (خطأ); } اكسر;
    حالة 1: hb_barrier (); 𐡄𐡍 (unlikely (!u.format1.sanitize (c))) { return_trace (خطأ); } اكسر;
    افتراضي:return_trace (خطأ);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  محمي:
  ثابت CFF1SuppEncData &suppEncData () ثابت
  {
    بدل (table_format ())
    {
    حالة 0: hb_barrier (); أرجع StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    حالة 1: hb_barrier (); أرجع StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    افتراضي:أرجع Null (CFF1SuppEncData);
    }
  }

  عام:
  HBUINT8	format;
  اتحاد {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
بنية Charset0
{
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح num_glyphs, unsigned *num_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (num_charset_entries) *num_charset_entries = num_glyphs;
    return_trace (sids.sanitize (c, num_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs) ثابت
  {
    𐡄𐡍 (unlikely (glyph >= num_glyphs)) أرجع 0;
    𐡄𐡍 (unlikely (glyph == 0))
      أرجع 0;
    وإلا
      أرجع sids[glyph - 1];
  }

  فراغ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحيح num_glyphs) ثابت
  {
    𐡄𐡍 (!mapping->resize_dirty  (num_glyphs)) أرجع;
    لكل (hb_codepoint_t gid = 1; gid < num_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحيح num_glyphs) ثابت
  {
    𐡄𐡍 (sid == 0)
      أرجع 0;

    لكل (unsigned صحيح glyph = 1; glyph < num_glyphs; glyph++)
    {
      𐡄𐡍 (sids[glyph-1] == sid)
	أرجع glyph;
    }
    أرجع 0;
  }

  ساكن size_t get_size (unsigned صحيح num_glyphs)
  {
    أكد (num_glyphs > 0);
    أرجع UnsizedArrayOf<HBUINT16>::get_size (num_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
بنية Charset_Range {
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16  first;
  TYPE      nLeft;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
بنية Charset1_2 {
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح num_glyphs, unsigned *num_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    num_glyphs--;
    unsigned i;
    لكل (i = 0; num_glyphs > 0; i++)
    {
      𐡄𐡍 (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (num_glyphs >= ranges[i].nLeft + 1))))
	return_trace (خطأ);
      num_glyphs -= (ranges[i].nLeft + 1);
    }
    𐡄𐡍 (num_charset_entries)
      *num_charset_entries = i;
    return_trace (𐡒𐡔𐡈);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs,
			  code_pair_t *cache = nullptr) ثابت
  {
    𐡄𐡍 (unlikely (glyph >= num_glyphs)) أرجع 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    𐡄𐡍 (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    وإلا
    {
      𐡄𐡍 (unlikely (glyph == 0)) أرجع 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    لكل (;; i++)
    {
      unsigned count = ranges[i].nLeft;
      𐡄𐡍 (glyph <= count)
      {
        𐡄𐡍 (cache)
	  *cache = {i, start_glyph};
	أرجع ranges[i].first + glyph;
      }
      count++;
      start_glyph += count;
      glyph -= count;
    }

    أرجع 0;
  }

  فراغ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحيح num_glyphs) ثابت
  {
    𐡄𐡍 (!mapping->resize_dirty  (num_glyphs)) أرجع;

    hb_codepoint_t gid = 1;
    𐡄𐡍 (gid >= num_glyphs)
      أرجع;

    لكل (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned count = ranges[i].nLeft + 1;
      unsigned last = gid + count;
      لكل (unsigned j = 0; j < count; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      𐡄𐡍 (gid >= num_glyphs)
        اكسر;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحيح num_glyphs) ثابت
  {
    𐡄𐡍 (sid == 0) أرجع 0;
    hb_codepoint_t  glyph = 1;
    لكل (unsigned صحيح i = 0;; i++)
    {
      𐡄𐡍 (glyph >= num_glyphs)
	أرجع 0;
      𐡄𐡍 ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].nLeft))
	أرجع glyph + (sid - ranges[i].first);
      glyph += (ranges[i].nLeft + 1);
    }

    أرجع 0;
  }

  size_t get_size (unsigned صحيح num_glyphs) ثابت
  {
    صحيح glyph = (صحيح) num_glyphs;
    unsigned num_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    لكل (unsigned صحيح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].nLeft + 1);
      num_ranges++;
    }

    أرجع get_size_for_ranges (num_ranges);
  }

  ساكن size_t get_size_for_ranges (unsigned صحيح num_ranges)
  {
    أرجع UnsizedArrayOf<Charset_Range<TYPE> >::get_size (num_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

تعريفنوع Charset1_2<HBUINT8>     Charset1;
تعريفنوع Charset1_2<HBUINT16>    Charset2;
تعريفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعريفنوع Charset_Range<HBUINT16> Charset2_Range;

بنية Charset
{
  /* serialize a fullset Charset */
  منطقي serialize (hb_serialize_context_t *c, ثابت Charset &src, unsigned صحيح num_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed ((ثابت حرف *) &src, src.get_size (num_glyphs)));
  }

  /* serialize a subset Charset */
  منطقي serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned صحيح num_glyphs,
		  ثابت hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (هذا);
    Charset *dest = c->extend_min (هذا);
    𐡄𐡍 (unlikely (!dest)) return_trace (خطأ);
    dest->format = format;
    بدل (format)
    {
    حالة 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (num_glyphs), خطأ);
      𐡄𐡍 (unlikely (!fmt0)) return_trace (خطأ);
      unsigned صحيح glyph = 0;
      لكل (unsigned صحيح i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	لكل (صحيح left = (صحيح)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    اكسر;

    حالة 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), خطأ);
      𐡄𐡍 (unlikely (!fmt1)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (unsigned صحيح i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].nLeft = _.glyph;
      }
      𐡄𐡍 (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (خطأ);
    }
    اكسر;

    حالة 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), خطأ);
      𐡄𐡍 (unlikely (!fmt2)) return_trace (خطأ);
      hb_codepoint_t all_glyphs = 0;
      لكل (unsigned صحيح i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].nLeft = _.glyph;
      }
      𐡄𐡍 (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (خطأ);
    }
    اكسر;

    }
    return_trace (𐡒𐡔𐡈);
  }

  size_t get_size (unsigned صحيح num_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_add_saturate (min_size, u.format0.get_size (num_glyphs));
    حالة 1: hb_barrier (); أرجع hb_unsigned_add_saturate (min_size, u.format1.get_size (num_glyphs));
    حالة 2: hb_barrier (); أرجع hb_unsigned_add_saturate (min_size, u.format2.get_size (num_glyphs));
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned صحيح num_glyphs,
			  code_pair_t *cache = nullptr) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_sid (glyph, num_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_sid (glyph, num_glyphs, cache);
    حالة 2: hb_barrier (); أرجع u.format2.get_sid (glyph, num_glyphs, cache);
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحيح num_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, num_glyphs); أرجع;
    حالة 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, num_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, num_glyphs); أرجع;
    افتراضي:أرجع;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحيح num_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_glyph (sid, num_glyphs);
    حالة 1: hb_barrier (); أرجع u.format1.get_glyph (sid, num_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_glyph (sid, num_glyphs);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned *num_charset_entries) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    افتراضي:return_trace (خطأ);
    }
  }

  HBUINT8       format;
  اتحاد {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

بنية CFF1StringIndex : CFF1Index
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF1StringIndex &strings,
		  ثابت hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (هذا);
    𐡄𐡍 (unlikely ((strings.count == 0) || (sidmap.length == 0)))
    {
      𐡄𐡍 (unlikely (!c->extend_min (هذا->count)))
	return_trace (خطأ);
      count = 0;
      return_trace (𐡒𐡔𐡈);
    }

    𐡄𐡍 (unlikely (sidmap.in_error ())) return_trace (خطأ);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    𐡄𐡍 (unlikely (bytesArray.in_error ())) return_trace (خطأ);

    منطقي result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

بنية cff1_top_dict_interp_env_t : num_interp_env_t
{
  cff1_top_dict_interp_env_t ()
    : num_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_top_dict_interp_env_t (ثابت hb_ubytes_t &bytes)
    : num_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned صحيح prev_offset;
  unsigned صحيح last_offset;
};

بنية name_dict_values_t
{
  تعداد name_dict_val_index_t
  {
      version,
      notice,
      copyright,
      fullName,
      familyName,
      weight,
      postscript,
      fontName,
      baseFontName,
      registry,
      ordering,

      ValCount
  };

  فراغ init ()
  {
    لكل (unsigned صحيح i = 0; i < ValCount; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned صحيح& operator[] (unsigned صحيح i)
  { أكد (i < ValCount); أرجع values[i]; }

  unsigned صحيح operator[] (unsigned صحيح i) ثابت
  { أكد (i < ValCount); أرجع values[i]; }

  ساكن تعداد name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    بدل (op) {
      افتراضي: // can't happen - just make some compiler happy
      حالة OpCode_version:
	أرجع version;
      حالة OpCode_Notice:
	أرجع notice;
      حالة OpCode_Copyright:
	أرجع copyright;
      حالة OpCode_FullName:
	أرجع fullName;
      حالة OpCode_FamilyName:
	أرجع familyName;
      حالة OpCode_Weight:
	أرجع weight;
      حالة OpCode_PostScript:
	أرجع postscript;
      حالة OpCode_FontName:
	أرجع fontName;
      حالة OpCode_BaseFontName:
	أرجع baseFontName;
    }
  }

  unsigned صحيح  values[ValCount];
};

بنية cff1_top_dict_val_t : op_str_t
{
  unsigned صحيح  last_arg_offset;
};

بنية cff1_top_dict_values_t : top_dict_values_t<cff1_top_dict_val_t>
{
  فراغ init ()
  {
    top_dict_values_t<cff1_top_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cidCount = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  فراغ fini () { top_dict_values_t<cff1_top_dict_val_t>::fini (); }

  منطقي is_CID () ثابت
  { أرجع nameSIDs[name_dict_values_t::registry] != CFF_UNDEF_SID; }

  name_dict_values_t  nameSIDs;
  unsigned صحيح    ros_supplement_offset;
  unsigned صحيح    ros_supplement;
  unsigned صحيح    cidCount;

  صحيح             EncodingOffset;
  صحيح             CharsetOffset;
  صحيح             FDSelectOffset;
  table_info_t    privateDictInfo;
};

بنية cff1_top_dict_opset_t : top_dict_opset_t<cff1_top_dict_val_t>
{
  ساكن فراغ process_op (op_code_t op, cff1_top_dict_interp_env_t& env, cff1_top_dict_values_t& dictval)
  {
    cff1_top_dict_val_t  val {};

    بدل (op) {
      حالة OpCode_version:
      حالة OpCode_Notice:
      حالة OpCode_Copyright:
      حالة OpCode_FullName:
      حالة OpCode_FontName:
      حالة OpCode_FamilyName:
      حالة OpCode_Weight:
      حالة OpCode_PostScript:
      حالة OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_values_t::name_op_to_index (op)] = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;
      حالة OpCode_isFixedPitch:
      حالة OpCode_ItalicAngle:
      حالة OpCode_UnderlinePosition:
      حالة OpCode_UnderlineThickness:
      حالة OpCode_PaintType:
      حالة OpCode_CharstringType:
      حالة OpCode_UniqueID:
      حالة OpCode_StrokeWidth:
      حالة OpCode_SyntheticBase:
      حالة OpCode_CIDFontVersion:
      حالة OpCode_CIDFontRevision:
      حالة OpCode_CIDFontType:
      حالة OpCode_UIDBase:
      حالة OpCode_FontBBox:
      حالة OpCode_XUID:
      حالة OpCode_BaseFontBlend:
	env.clear_args ();
	اكسر;

      حالة OpCode_CIDCount:
	dictval.cidCount = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;

      حالة OpCode_ROS:
	dictval.ros_supplement = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::ordering] = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::registry] = env.argStack.pop_uint ();
	env.clear_args ();
	𐡄𐡍 (unlikely (env.in_error ())) أرجع;
	أكد (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	اكسر;

      حالة OpCode_Encoding:
	dictval.EncodingOffset = env.argStack.pop_int ();
	env.clear_args ();
	𐡄𐡍 (unlikely (dictval.EncodingOffset == 0)) أرجع;
	اكسر;

      حالة OpCode_charset:
	dictval.CharsetOffset = env.argStack.pop_int ();
	env.clear_args ();
	𐡄𐡍 (unlikely (dictval.CharsetOffset == 0)) أرجع;
	اكسر;

      حالة OpCode_FDSelect:
	dictval.FDSelectOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;

      حالة OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_int ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;

      افتراضي:
	env.last_offset = env.str_ref.get_offset ();
	top_dict_opset_t<cff1_top_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	𐡄𐡍 (!env.argStack.is_empty ()) أرجع;
	اكسر;
    }

    𐡄𐡍 (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref, val);
  }
};

بنية cff1_font_dict_values_t : dict_values_t<op_str_t>
{
  فراغ init ()
  {
    dict_values_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  فراغ fini () { dict_values_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned صحيح    fontName;
};

بنية cff1_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, num_interp_env_t& env, cff1_font_dict_values_t& dictval)
  {
    بدل (op) {
      حالة OpCode_FontName:
	dictval.fontName = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;
      حالة OpCode_FontMatrix:
      حالة OpCode_PaintType:
	env.clear_args ();
	اكسر;
      حالة OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_uint ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	𐡄𐡍 (!env.argStack.is_empty ()) أرجع;
	اكسر;
    }

    𐡄𐡍 (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
بنية cff1_private_dict_values_base_t : dict_values_t<VAL>
{
  فراغ init ()
  {
    dict_values_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  فراغ fini () { dict_values_t<VAL>::fini (); }

  صحيح                 subrsOffset;
  ثابت CFF1Subrs    *localSubrs;
};

تعريفنوع cff1_private_dict_values_base_t<op_str_t> cff1_private_dict_values_subset_t;
تعريفنوع cff1_private_dict_values_base_t<num_dict_val_t> cff1_private_dict_values_t;

بنية cff1_private_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_t& dictval)
  {
    num_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة OpCode_BlueValues:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ForceBold:
      حالة OpCode_LanguageGroup:
      حالة OpCode_ExpansionFactor:
      حالة OpCode_initialRandomSeed:
      حالة OpCode_defaultWidthX:
      حالة OpCode_nominalWidthX:
	env.clear_args ();
	اكسر;
      حالة OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	𐡄𐡍 (!env.argStack.is_empty ()) أرجع;
	اكسر;
    }

    𐡄𐡍 (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref, val);
  }
};

بنية cff1_private_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_subset_t& dictval)
  {
    بدل (op) {
      حالة OpCode_BlueValues:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ForceBold:
      حالة OpCode_LanguageGroup:
      حالة OpCode_ExpansionFactor:
      حالة OpCode_initialRandomSeed:
      حالة OpCode_defaultWidthX:
      حالة OpCode_nominalWidthX:
	env.clear_args ();
	اكسر;

      حالة OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	𐡄𐡍 (!env.argStack.is_empty ()) أرجع;
	اكسر;
    }

    𐡄𐡍 (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref);
  }
};

تعريفنوع dict_interpreter_t<cff1_top_dict_opset_t, cff1_top_dict_values_t, cff1_top_dict_interp_env_t> cff1_top_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_values_t> cff1_font_dict_interpreter_t;

تعريفنوع CFF1Index CFF1NameIndex;
تعريفنوع CFF1Index CFF1TopDictIndex;

بنية cff1_font_dict_values_mod_t
{
  cff1_font_dict_values_mod_t() { init (); }

  فراغ init () { init ( &Null (cff1_font_dict_values_t), CFF_UNDEF_SID ); }

  فراغ init (ثابت cff1_font_dict_values_t *base_,
	     unsigned صحيح fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_count () ثابت { أرجع base->get_count (); }

  ثابت op_str_t &operator [] (unsigned صحيح i) ثابت { أرجع (*base)[i]; }

  ثابت cff1_font_dict_values_t    *base;
  table_info_t		   privateDictInfo;
  unsigned صحيح		fontName;
};

بنية CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT16>::serialize<cff1_font_dict_values_mod_t, cff1_font_dict_values_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

using نطاق CFF;

بنية cff1
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      𐡄𐡍 (!face) أرجع;

      topDict.init ();
      fontDicts.init ();
      privateDicts.init ();

      هذا->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.start_processing ();

      ثابت OT::cff1 *cff = هذا->blob->template as<OT::cff1> ();

      𐡄𐡍 (cff == &Null (OT::cff1))
        اذهب fail;

      nameIndex = &cff->nameIndex (cff);
      𐡄𐡍 ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        اذهب fail;
      hb_barrier ();

      topDictIndex = &StructAtOffsetOrNull<CFF1TopDictIndex> (nameIndex, nameIndex->get_size (), sc);
      𐡄𐡍 (topDictIndex == &Null (CFF1TopDictIndex) || (topDictIndex->count == 0))
        اذهب fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t topDictStr = (*topDictIndex)[0];
	𐡄𐡍 (unlikely (!topDictStr.sanitize (&sc)))   اذهب fail;
	hb_barrier ();
	cff1_top_dict_interp_env_t env (topDictStr);
	cff1_top_dict_interpreter_t top_interp (env);
	𐡄𐡍 (unlikely (!top_interp.interpret (topDict)))   اذهب fail;
      }

      𐡄𐡍 (is_predef_charset ())
	charset = &Null (Charset);
      وإلا
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, topDict.CharsetOffset, sc, &num_charset_entries);
	𐡄𐡍 (unlikely (charset == &Null (Charset)))   اذهب fail;
      }

      fdCount = 1;
      𐡄𐡍 (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, topDict.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, topDict.FDSelectOffset, sc, fdArray->count);
	𐡄𐡍 (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  اذهب fail;

	fdCount = fdArray->count;
      }
      وإلا
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      𐡄𐡍 (is_CID ())
      {
	𐡄𐡍 (unlikely (charset == &Null (Charset)))   اذهب fail;
      }
      وإلا
      {
	𐡄𐡍 (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, topDict.EncodingOffset, sc);
	  𐡄𐡍 (unlikely (encoding == &Null (Encoding)))   اذهب fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (topDictIndex, topDictIndex->get_size (), sc);
      𐡄𐡍 (stringIndex == &Null (CFF1StringIndex))
        اذهب fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, topDict.charStringsOffset, sc);
      𐡄𐡍 (charStrings == &Null (CFF1CharStrings))
        اذهب fail;

      num_glyphs = charStrings->count;
      𐡄𐡍 (num_glyphs != sc.get_num_glyphs ())
        اذهب fail;

      𐡄𐡍 (unlikely (!privateDicts.resize (fdCount)))
        اذهب fail;
      لكل (unsigned صحيح i = 0; i < fdCount; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      𐡄𐡍 (is_CID ())
      {
	لكل (unsigned صحيح i = 0; i < fdCount; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  𐡄𐡍 (unlikely (!fontDictStr.sanitize (&sc)))   اذهب fail;
	  hb_barrier ();
	  cff1_font_dict_values_t *font;
	  cff1_top_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  𐡄𐡍 (unlikely (fontDicts.in_error ()))   اذهب fail;

	  font->init ();
	  𐡄𐡍 (unlikely (!font_interp.interpret (*font)))   اذهب fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  𐡄𐡍 (unlikely (font->privateDictInfo.size &&
			privDictStr == (ثابت unsigned حرف *) &Null (UnsizedByteStr))) اذهب fail;
	  num_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  𐡄𐡍 (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      وإلا  /* non-CID */
      {
	cff1_top_dict_values_t *font = &topDict;
	PRIVDICTVAL *priv = &privateDicts[0];

	ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	𐡄𐡍 (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ثابت unsigned حرف *) &Null (UnsizedByteStr))) اذهب fail;
	num_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	𐡄𐡍 (unlikely (!priv_interp.interpret (*priv)))   اذهب fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.end_processing ();
      topDict.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }
    منطقي   is_CID () ثابت { أرجع topDict.is_CID (); }

    منطقي is_predef_charset () ثابت { أرجع topDict.CharsetOffset <= ExpertSubsetCharset; }

    unsigned صحيح std_code_to_glyph (hb_codepoint_t code) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      𐡄𐡍 (unlikely (sid == CFF_UNDEF_SID))
	أرجع 0;

      𐡄𐡍 (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, num_glyphs);
      وإلا 𐡄𐡍 ((topDict.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) أرجع sid;
      أرجع 0;
    }

    منطقي is_predef_encoding () ثابت { أرجع topDict.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ثابت
    {
      𐡄𐡍 (encoding != &Null (Encoding))
	أرجع encoding->get_code (glyph);
      وإلا
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	𐡄𐡍 (sid == 0) أرجع 0;
	hb_codepoint_t code = 0;
	بدل (topDict.EncodingOffset)
	{
	حالة StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  اكسر;
	حالة ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  اكسر;
	افتراضي:
	  اكسر;
	}
	أرجع code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ثابت
    {
      𐡄𐡍 (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (حجم (glyph_to_sid_map_t));
	𐡄𐡍 (unlikely (!mapping)) أرجع nullptr;
	mapping = جديد (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, num_glyphs);
	أرجع mapping;
      }
      وإلا
	أرجع nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ثابت
    {
      𐡄𐡍 (charset != &Null (Charset))
	أرجع charset->get_sid (glyph, num_glyphs, cache);
      وإلا
      {
	hb_codepoint_t sid = 0;
	بدل (topDict.CharsetOffset)
	{
	  حالة ISOAdobeCharset:
	    𐡄𐡍 (glyph <= 228 /*zcaron*/) sid = glyph;
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

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ثابت
    {
      𐡄𐡍 (charset != &Null (Charset))
	أرجع charset->get_glyph (sid, num_glyphs);
      وإلا
      {
	hb_codepoint_t glyph = 0;
	بدل (topDict.CharsetOffset)
	{
	  حالة ISOAdobeCharset:
	    𐡄𐡍 (sid <= 228 /*zcaron*/) glyph = sid;
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
    hb_blob_t               *blob = nullptr;
    ثابت Encoding	    *encoding = nullptr;
    ثابت Charset	    *charset = nullptr;
    ثابت CFF1NameIndex     *nameIndex = nullptr;
    ثابت CFF1TopDictIndex  *topDictIndex = nullptr;
    ثابت CFF1StringIndex   *stringIndex = nullptr;
    ثابت CFF1Subrs	    *globalSubrs = nullptr;
    ثابت CFF1CharStrings   *charStrings = nullptr;
    ثابت CFF1FDArray       *fdArray = nullptr;
    ثابت CFF1FDSelect      *fdSelect = nullptr;
    unsigned صحيح	     fdCount = 0;

    cff1_top_dict_values_t   topDict;
    hb_vector_t<cff1_font_dict_values_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned صحيح	     num_glyphs = 0;
    unsigned صحيح	     num_charset_entries = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      𐡄𐡍 (!is_valid ()) أرجع;
      𐡄𐡍 (is_CID ()) أرجع;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      𐡄𐡍 (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, unsigned صحيح buf_len) ثابت
    {
      𐡄𐡍 (unlikely (glyph >= num_glyphs)) أرجع خطأ;
      𐡄𐡍 (unlikely (!is_valid ())) أرجع خطأ;
      𐡄𐡍 (is_CID()) أرجع خطأ;
      𐡄𐡍 (unlikely (!buf_len)) أرجع 𐡒𐡔𐡈;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ثابت حرف *str;
      size_t str_len;
      𐡄𐡍 (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      وإلا
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ثابت حرف *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      𐡄𐡍 (!str_len) أرجع خطأ;
      unsigned صحيح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت حرف*)str, len);
      buf[len] = '\0';
      أرجع 𐡒𐡔𐡈;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      𐡄𐡍 (unlikely (!is_valid ())) أرجع خطأ;
      𐡄𐡍 (is_CID()) أرجع خطأ;
      𐡄𐡍 (len < 0) len = strlen (name);
      𐡄𐡍 (unlikely (!len)) أرجع خطأ;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      𐡄𐡍 (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, حجم (hb_sorted_vector_t<gname_t>));
	𐡄𐡍 (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  لكل (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    𐡄𐡍 (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    وإلا
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ثابت حرف*) ustr.arrayZ, ustr.length);
	    }
	    𐡄𐡍 (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	𐡄𐡍 (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  𐡄𐡍 (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  اذهب retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ثابت gname_t *gname = names ? names->bsearch (key) : nullptr;
      𐡄𐡍 (!gname) أرجع خطأ;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      𐡄𐡍 (!gid && gname->sid) أرجع خطأ;
      *glyph = gid;
      أرجع 𐡒𐡔𐡈;
    }

    HB_INTERNAL منطقي get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقي get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ثابت;

    خاص:
    بنية gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      ساكن صحيح cmp (ثابت فراغ *a_, ثابت فراغ *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	صحيح ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	𐡄𐡍 (ret) أرجع ret;
	أرجع a->name.length - b->name.length;
      }

      صحيح cmp (ثابت gname_t &a) ثابت { أرجع cmp (&a, هذا); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    تعريفنوع accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t> SUPER;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      𐡄𐡍 (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL منطقي subset (hb_subset_context_t *c) ثابت;
    HB_INTERNAL منطقي serialize (hb_serialize_context_t *c,
				بنية cff1_subset_plan &plan) ثابت;
    HB_INTERNAL منطقي get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ثابت;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    تعريفنوع accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t> SUPER;
  };

  محمي:
  HB_INTERNAL ساكن hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ساكن hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ساكن hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ساكن hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ساكن hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ساكن hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ساكن hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  عام:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

بنية cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  فراغ depend (hb_depend_data_builder_t *builder) ثابت
  {
    𐡄𐡍 (!is_valid ()) أرجع;
    لكل (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      𐡄𐡍 (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
