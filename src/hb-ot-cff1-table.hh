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

#شامل "hb-ot-cff-common.hh"
#شامل "hb-depend-data.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-draw.hh"
#شامل "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#شامل "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

فضاینام CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

شمارش EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
شمارش CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

تعریفنوع CFF1Index          CFF1CharStrings;
تعریفنوع Subrs<HBUINT16>    CFF1Subrs;

ساختار CFF1FDSelect : FDSelect {};

/* Encoding */
ساختار Encoding0 {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    أكد (glyph > 0);
    glyph--;
    اگر (glyph < nCodes ())
    {
      بازگشت (hb_codepoint_t)codes[glyph];
    }
    وگرنه
      بازگشت CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { بازگشت codes.len; }
  HBUINT8 nCodes () ثابت { بازگشت codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

ساختار Encoding1_Range {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT8   first;
  HBUINT8   n_چپ;

  DEFINE_SIZE_STATIC (2);
};

ساختار Encoding1 {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    /* TODO: Add cache like get_sid. */
    أكد (glyph > 0);
    glyph--;
    برای (unsigned صحیح i = 0; i < nRanges (); i++)
    {
      اگر (glyph <= ranges[i].n_چپ)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	بازگشت (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_چپ + 1);
    }
    بازگشت CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { بازگشت ranges.len; }
  HBUINT8 nRanges () ثابت { بازگشت ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

ساختار SuppEncoding {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

ساختار CFF1SuppEncData {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (supps.sanitize (c));
  }

  تهی get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ثابت
  {
    برای (unsigned صحیح i = 0; i < nSups (); i++)
      اگر (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { بازگشت supps.len; }
  HBUINT8 nSups () ثابت { بازگشت supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

ساختار Encoding
{
  /* serialize a fullset Encoding */
  منطقی serialize (hb_serialize_context_t *c, ثابت Encoding &src)
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  منطقی serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned صحیح enc_شمار,
		  ثابت hb_vector_t<code_pair_t>& code_ranges,
		  ثابت hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (این);
    Encoding *dest = c->extend_min (این);
    اگر (unlikely (!dest)) return_trace (نادرست);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    انتخاب (format) {
    حالت 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_شمار);
      اگر (unlikely (!fmt0)) return_trace (نادرست);
      fmt0->nCodes () = enc_شمار;
      unsigned صحیح glyph = 0;
      برای (unsigned صحیح i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	برای (صحیح left = (صحیح)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	اگر (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (نادرست);
      }
    }
    بشکن;

    حالت 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      اگر (unlikely (!fmt1)) return_trace (نادرست);
      fmt1->nRanges () = code_ranges.length;
      برای (unsigned صحیح i = 0; i < code_ranges.length; i++)
      {
	اگر (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (نادرست);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_چپ = code_ranges[i].glyph;
      }
    }
    بشکن;

    }

    اگر (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      اگر (unlikely (!suppData)) return_trace (نادرست);
      suppData->nSups () = supp_codes.length;
      برای (unsigned صحیح i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (درست);
  }

  size_t get_size () ثابت
  {
    size_t size = min_size;
    انتخاب (table_format ())
    {
    حالت 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); بشکن;
    حالت 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); بشکن;
    }
    اگر (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    بازگشت size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ثابت
  {
    انتخاب (table_format ())
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_code (glyph);
    حالت 1: hb_barrier (); بازگشت u.format1.get_code (glyph);
    پیشفرض:بازگشت 0;
    }
  }

  uint8_t table_format () ثابت { بازگشت format & 0x7F; }
  منطقی  has_supplement () ثابت { بازگشت format & 0x80; }

  تهی get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ثابت
  {
    codes.clear ();
    اگر (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این)))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (table_format ())
    {
    حالت 0: hb_barrier (); اگر (unlikely (!u.format0.sanitize (c))) { return_trace (نادرست); } بشکن;
    حالت 1: hb_barrier (); اگر (unlikely (!u.format1.sanitize (c))) { return_trace (نادرست); } بشکن;
    پیشفرض:return_trace (نادرست);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  محافظتشده:
  ثابت CFF1SuppEncData &suppEncData () ثابت
  {
    انتخاب (table_format ())
    {
    حالت 0: hb_barrier (); بازگشت StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    حالت 1: hb_barrier (); بازگشت StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    پیشفرض:بازگشت Null (CFF1SuppEncData);
    }
  }

  عمومی:
  HBUINT8	format;
  اجتماع {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
ساختار Charset0
{
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح رقم_glyphs, unsigned *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (رقم_charset_entries) *رقم_charset_entries = رقم_glyphs;
    return_trace (sids.sanitize (c, رقم_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned رقم_glyphs) ثابت
  {
    اگر (unlikely (glyph >= رقم_glyphs)) بازگشت 0;
    اگر (unlikely (glyph == 0))
      بازگشت 0;
    وگرنه
      بازگشت sids[glyph - 1];
  }

  تهی collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحیح رقم_glyphs) ثابت
  {
    اگر (!mapping->resize_dirty  (رقم_glyphs)) بازگشت;
    برای (hb_codepoint_t gid = 1; gid < رقم_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحیح رقم_glyphs) ثابت
  {
    اگر (sid == 0)
      بازگشت 0;

    برای (unsigned صحیح glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      اگر (sids[glyph-1] == sid)
	بازگشت glyph;
    }
    بازگشت 0;
  }

  ایستا size_t get_size (unsigned صحیح رقم_glyphs)
  {
    أكد (رقم_glyphs > 0);
    بازگشت UnsizedArrayOf<HBUINT16>::get_size (رقم_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
ساختار Charset_Range {
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT16  first;
  TYPE      n_چپ;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
ساختار Charset1_2 {
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح رقم_glyphs, unsigned *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (این);
    رقم_glyphs--;
    unsigned i;
    برای (i = 0; رقم_glyphs > 0; i++)
    {
      اگر (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (رقم_glyphs >= ranges[i].n_چپ + 1))))
	return_trace (نادرست);
      رقم_glyphs -= (ranges[i].n_چپ + 1);
    }
    اگر (رقم_charset_entries)
      *رقم_charset_entries = i;
    return_trace (درست);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned رقم_glyphs,
			  code_pair_t *cache = nullptr) ثابت
  {
    اگر (unlikely (glyph >= رقم_glyphs)) بازگشت 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    اگر (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    وگرنه
    {
      اگر (unlikely (glyph == 0)) بازگشت 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    برای (;; i++)
    {
      unsigned شمار = ranges[i].n_چپ;
      اگر (glyph <= شمار)
      {
        اگر (cache)
	  *cache = {i, start_glyph};
	بازگشت ranges[i].first + glyph;
      }
      شمار++;
      start_glyph += شمار;
      glyph -= شمار;
    }

    بازگشت 0;
  }

  تهی collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحیح رقم_glyphs) ثابت
  {
    اگر (!mapping->resize_dirty  (رقم_glyphs)) بازگشت;

    hb_codepoint_t gid = 1;
    اگر (gid >= رقم_glyphs)
      بازگشت;

    برای (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned شمار = ranges[i].n_چپ + 1;
      unsigned last = gid + شمار;
      برای (unsigned j = 0; j < شمار; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      اگر (gid >= رقم_glyphs)
        بشکن;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحیح رقم_glyphs) ثابت
  {
    اگر (sid == 0) بازگشت 0;
    hb_codepoint_t  glyph = 1;
    برای (unsigned صحیح i = 0;; i++)
    {
      اگر (glyph >= رقم_glyphs)
	بازگشت 0;
      اگر ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_چپ))
	بازگشت glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_چپ + 1);
    }

    بازگشت 0;
  }

  size_t get_size (unsigned صحیح رقم_glyphs) ثابت
  {
    صحیح glyph = (صحیح) رقم_glyphs;
    unsigned رقم_ranges = 0;

    أكد (glyph > 0);
    glyph--;
    برای (unsigned صحیح i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_چپ + 1);
      رقم_ranges++;
    }

    بازگشت get_size_for_ranges (رقم_ranges);
  }

  ایستا size_t get_size_for_ranges (unsigned صحیح رقم_ranges)
  {
    بازگشت UnsizedArrayOf<Charset_Range<TYPE> >::get_size (رقم_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

تعریفنوع Charset1_2<HBUINT8>     Charset1;
تعریفنوع Charset1_2<HBUINT16>    Charset2;
تعریفنوع Charset_Range<HBUINT8>  Charset1_Range;
تعریفنوع Charset_Range<HBUINT16> Charset2_Range;

ساختار Charset
{
  /* serialize a fullset Charset */
  منطقی serialize (hb_serialize_context_t *c, ثابت Charset &src, unsigned صحیح رقم_glyphs)
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed ((ثابت نویسه *) &src, src.get_size (رقم_glyphs)));
  }

  /* serialize a subset Charset */
  منطقی serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned صحیح رقم_glyphs,
		  ثابت hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (این);
    Charset *dest = c->extend_min (این);
    اگر (unlikely (!dest)) return_trace (نادرست);
    dest->format = format;
    انتخاب (format)
    {
    حالت 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (رقم_glyphs), نادرست);
      اگر (unlikely (!fmt0)) return_trace (نادرست);
      unsigned صحیح glyph = 0;
      برای (unsigned صحیح i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	برای (صحیح left = (صحیح)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    بشکن;

    حالت 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), نادرست);
      اگر (unlikely (!fmt1)) return_trace (نادرست);
      hb_codepoint_t all_glyphs = 0;
      برای (unsigned صحیح i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_چپ = _.glyph;
      }
      اگر (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (نادرست);
    }
    بشکن;

    حالت 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), نادرست);
      اگر (unlikely (!fmt2)) return_trace (نادرست);
      hb_codepoint_t all_glyphs = 0;
      برای (unsigned صحیح i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_چپ = _.glyph;
      }
      اگر (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (نادرست);
    }
    بشکن;

    }
    return_trace (درست);
  }

  size_t get_size (unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت hb_unsigned_add_saturate (min_size, u.format0.get_size (رقم_glyphs));
    حالت 1: hb_barrier (); بازگشت hb_unsigned_add_saturate (min_size, u.format1.get_size (رقم_glyphs));
    حالت 2: hb_barrier (); بازگشت hb_unsigned_add_saturate (min_size, u.format2.get_size (رقم_glyphs));
    پیشفرض:بازگشت 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned صحیح رقم_glyphs,
			  code_pair_t *cache = nullptr) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_sid (glyph, رقم_glyphs);
    حالت 1: hb_barrier (); بازگشت u.format1.get_sid (glyph, رقم_glyphs, cache);
    حالت 2: hb_barrier (); بازگشت u.format2.get_sid (glyph, رقم_glyphs, cache);
    پیشفرض:بازگشت 0;
    }
  }

  تهی collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, رقم_glyphs); بازگشت;
    حالت 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, رقم_glyphs); بازگشت;
    حالت 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, رقم_glyphs); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_glyph (sid, رقم_glyphs);
    حالت 1: hb_barrier (); بازگشت u.format1.get_glyph (sid, رقم_glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.get_glyph (sid, رقم_glyphs);
    پیشفرض:بازگشت 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned *رقم_charset_entries) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این)))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_رقم_glyphs (), رقم_charset_entries));
    پیشفرض:return_trace (نادرست);
    }
  }

  HBUINT8       format;
  اجتماع {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

ساختار CFF1StringIndex : CFF1Index
{
  منطقی serialize (hb_serialize_context_t *c, ثابت CFF1StringIndex &strings,
		  ثابت hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely ((strings.شمار == 0) || (sidmap.length == 0)))
    {
      اگر (unlikely (!c->extend_min (این->شمار)))
	return_trace (نادرست);
      شمار = 0;
      return_trace (درست);
    }

    اگر (unlikely (sidmap.in_error ())) return_trace (نادرست);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    اگر (unlikely (bytesArray.in_error ())) return_trace (نادرست);

    منطقی result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

ساختار cff1_بالاترین_dict_interp_env_t : رقم_interp_env_t
{
  cff1_بالاترین_dict_interp_env_t ()
    : رقم_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_بالاترین_dict_interp_env_t (ثابت hb_ubytes_t &bytes)
    : رقم_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned صحیح prev_offset;
  unsigned صحیح last_offset;
};

ساختار name_dict_مقادیر_t
{
  شمارش name_dict_val_index_t
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

      Val_شمار
  };

  تهی init ()
  {
    برای (unsigned صحیح i = 0; i < Val_شمار; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned صحیح& operator[] (unsigned صحیح i)
  { أكد (i < Val_شمار); بازگشت values[i]; }

  unsigned صحیح operator[] (unsigned صحیح i) ثابت
  { أكد (i < Val_شمار); بازگشت values[i]; }

  ایستا شمارش name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    انتخاب (op) {
      پیشفرض: // can't happen - just make some compiler happy
      حالت OpCode_version:
	بازگشت version;
      حالت OpCode_Notice:
	بازگشت notice;
      حالت OpCode_Copyright:
	بازگشت copyright;
      حالت OpCode_FullName:
	بازگشت fullName;
      حالت OpCode_FamilyName:
	بازگشت familyName;
      حالت OpCode_Weight:
	بازگشت weight;
      حالت OpCode_PostScript:
	بازگشت postscript;
      حالت OpCode_FontName:
	بازگشت fontName;
      حالت OpCode_BaseFontName:
	بازگشت baseFontName;
    }
  }

  unsigned صحیح  values[Val_شمار];
};

ساختار cff1_بالاترین_dict_val_t : op_str_t
{
  unsigned صحیح  last_arg_offset;
};

ساختار cff1_بالاترین_dict_مقادیر_t : بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>
{
  تهی init ()
  {
    بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_شمار = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  تهی fini () { بالاترین_dict_مقادیر_t<cff1_بالاترین_dict_val_t>::fini (); }

  منطقی is_CID () ثابت
  { بازگشت nameSIDs[name_dict_مقادیر_t::registry] != CFF_UNDEF_SID; }

  name_dict_مقادیر_t  nameSIDs;
  unsigned صحیح    ros_supplement_offset;
  unsigned صحیح    ros_supplement;
  unsigned صحیح    cid_شمار;

  صحیح             EncodingOffset;
  صحیح             CharsetOffset;
  صحیح             FDSelectOffset;
  table_info_t    privateDictInfo;
};

ساختار cff1_بالاترین_dict_opset_t : بالاترین_dict_opset_t<cff1_بالاترین_dict_val_t>
{
  ایستا تهی process_op (op_code_t op, cff1_بالاترین_dict_interp_env_t& env, cff1_بالاترین_dict_مقادیر_t& dictval)
  {
    cff1_بالاترین_dict_val_t  val {};

    انتخاب (op) {
      حالت OpCode_version:
      حالت OpCode_Notice:
      حالت OpCode_Copyright:
      حالت OpCode_FullName:
      حالت OpCode_FontName:
      حالت OpCode_FamilyName:
      حالت OpCode_Weight:
      حالت OpCode_PostScript:
      حالت OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_مقادیر_t::name_op_to_index (op)] = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;
      حالت OpCode_isFixedPitch:
      حالت OpCode_ItalicAngle:
      حالت OpCode_UnderlinePosition:
      حالت OpCode_UnderlineThickness:
      حالت OpCode_PaintType:
      حالت OpCode_CharstringType:
      حالت OpCode_UniqueID:
      حالت OpCode_StrokeWidth:
      حالت OpCode_SyntheticBase:
      حالت OpCode_CIDFontVersion:
      حالت OpCode_CIDFontRevision:
      حالت OpCode_CIDFontType:
      حالت OpCode_UIDBase:
      حالت OpCode_FontBBox:
      حالت OpCode_XUID:
      حالت OpCode_BaseFontBlend:
	env.clear_args ();
	بشکن;

      حالت OpCode_CIDCount:
	dictval.cid_شمار = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;

      حالت OpCode_ROS:
	dictval.ros_supplement = env.arg_پشته.برداشت_uint ();
	dictval.nameSIDs[name_dict_مقادیر_t::ordering] = env.arg_پشته.برداشت_uint ();
	dictval.nameSIDs[name_dict_مقادیر_t::registry] = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	اگر (unlikely (env.in_error ())) بازگشت;
	أكد (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	بشکن;

      حالت OpCode_Encoding:
	dictval.EncodingOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	اگر (unlikely (dictval.EncodingOffset == 0)) بازگشت;
	بشکن;

      حالت OpCode_charset:
	dictval.CharsetOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	اگر (unlikely (dictval.CharsetOffset == 0)) بازگشت;
	بشکن;

      حالت OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;

      حالت OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_پشته.برداشت_int ();
	dictval.privateDictInfo.size = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	env.last_offset = env.str_ref.get_offset ();
	بالاترین_dict_opset_t<cff1_بالاترین_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref, val);
  }
};

ساختار cff1_font_dict_مقادیر_t : dict_مقادیر_t<op_str_t>
{
  تهی init ()
  {
    dict_مقادیر_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  تهی fini () { dict_مقادیر_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned صحیح    fontName;
};

ساختار cff1_font_dict_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, رقم_interp_env_t& env, cff1_font_dict_مقادیر_t& dictval)
  {
    انتخاب (op) {
      حالت OpCode_FontName:
	dictval.fontName = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;
      حالت OpCode_FontMatrix:
      حالت OpCode_PaintType:
	env.clear_args ();
	بشکن;
      حالت OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_پشته.برداشت_uint ();
	dictval.privateDictInfo.size = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
ساختار cff1_private_dict_مقادیر_base_t : dict_مقادیر_t<VAL>
{
  تهی init ()
  {
    dict_مقادیر_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  تهی fini () { dict_مقادیر_t<VAL>::fini (); }

  صحیح                 subrsOffset;
  ثابت CFF1Subrs    *localSubrs;
};

تعریفنوع cff1_private_dict_مقادیر_base_t<op_str_t> cff1_private_dict_مقادیر_subset_t;
تعریفنوع cff1_private_dict_مقادیر_base_t<رقم_dict_val_t> cff1_private_dict_مقادیر_t;

ساختار cff1_private_dict_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, رقم_interp_env_t& env, cff1_private_dict_مقادیر_t& dictval)
  {
    رقم_dict_val_t val;
    val.init ();

    انتخاب (op) {
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_ForceBold:
      حالت OpCode_LanguageGroup:
      حالت OpCode_ExpansionFactor:
      حالت OpCode_initialRandomSeed:
      حالت OpCode_defaultWidthX:
      حالت OpCode_nominalWidthX:
	env.clear_args ();
	بشکن;
      حالت OpCode_Subrs:
	dictval.subrsOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref, val);
  }
};

ساختار cff1_private_dict_opset_subset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, رقم_interp_env_t& env, cff1_private_dict_مقادیر_subset_t& dictval)
  {
    انتخاب (op) {
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_ForceBold:
      حالت OpCode_LanguageGroup:
      حالت OpCode_ExpansionFactor:
      حالت OpCode_initialRandomSeed:
      حالت OpCode_defaultWidthX:
      حالت OpCode_nominalWidthX:
	env.clear_args ();
	بشکن;

      حالت OpCode_Subrs:
	dictval.subrsOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref);
  }
};

تعریفنوع dict_interpreter_t<cff1_بالاترین_dict_opset_t, cff1_بالاترین_dict_مقادیر_t, cff1_بالاترین_dict_interp_env_t> cff1_بالاترین_dict_interpreter_t;
تعریفنوع dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_مقادیر_t> cff1_font_dict_interpreter_t;

تعریفنوع CFF1Index CFF1NameIndex;
تعریفنوع CFF1Index CFF1_بالاترین_Dict_Index;

ساختار cff1_font_dict_مقادیر_mod_t
{
  cff1_font_dict_مقادیر_mod_t() { init (); }

  تهی init () { init ( &Null (cff1_font_dict_مقادیر_t), CFF_UNDEF_SID ); }

  تهی init (ثابت cff1_font_dict_مقادیر_t *base_,
	     unsigned صحیح fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_شمار () ثابت { بازگشت base->get_شمار (); }

  ثابت op_str_t &operator [] (unsigned صحیح i) ثابت { بازگشت (*base)[i]; }

  ثابت cff1_font_dict_مقادیر_t    *base;
  table_info_t		   privateDictInfo;
  unsigned صحیح		fontName;
};

ساختار CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { بازگشت FDArray<HBUINT16>::serialize<cff1_font_dict_مقادیر_mod_t, cff1_font_dict_مقادیر_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

فضاینام OT {

using فضاینام CFF;

ساختار cff1
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ساختار accelerator_templ_t
  {
    ایستا constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      اگر (!face) بازگشت;

      بالاترین_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      این->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (این->blob);
      sc.start_processing ();

      ثابت OT::cff1 *cff = این->blob->template as<OT::cff1> ();

      اگر (cff == &Null (OT::cff1))
        برو fail;

      nameIndex = &cff->nameIndex (cff);
      اگر ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        برو fail;
      hb_barrier ();

      بالاترین_Dict_Index = &StructAtOffsetOrNull<CFF1_بالاترین_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      اگر (بالاترین_Dict_Index == &Null (CFF1_بالاترین_Dict_Index) || (بالاترین_Dict_Index->شمار == 0))
        برو fail;
      hb_barrier ();

      { /* parse top dict */
	ثابت hb_ubytes_t بالاترین_Dict_Str = (*بالاترین_Dict_Index)[0];
	اگر (unlikely (!بالاترین_Dict_Str.sanitize (&sc)))   برو fail;
	hb_barrier ();
	cff1_بالاترین_dict_interp_env_t env (بالاترین_Dict_Str);
	cff1_بالاترین_dict_interpreter_t بالاترین_interp (env);
	اگر (unlikely (!بالاترین_interp.interpret (بالاترین_Dict_2)))   برو fail;
      }

      اگر (is_predef_charset ())
	charset = &Null (Charset);
      وگرنه
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, بالاترین_Dict_2.CharsetOffset, sc, &رقم_charset_entries);
	اگر (unlikely (charset == &Null (Charset)))   برو fail;
      }

      fd_شمار = 1;
      اگر (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, بالاترین_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, بالاترین_Dict_2.FDSelectOffset, sc, fdArray->شمار);
	اگر (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  برو fail;

	fd_شمار = fdArray->شمار;
      }
      وگرنه
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      اگر (is_CID ())
      {
	اگر (unlikely (charset == &Null (Charset)))   برو fail;
      }
      وگرنه
      {
	اگر (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, بالاترین_Dict_2.EncodingOffset, sc);
	  اگر (unlikely (encoding == &Null (Encoding)))   برو fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (بالاترین_Dict_Index, بالاترین_Dict_Index->get_size (), sc);
      اگر (stringIndex == &Null (CFF1StringIndex))
        برو fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, بالاترین_Dict_2.charStringsOffset, sc);
      اگر (charStrings == &Null (CFF1CharStrings))
        برو fail;

      رقم_glyphs = charStrings->شمار;
      اگر (رقم_glyphs != sc.get_رقم_glyphs ())
        برو fail;

      اگر (unlikely (!privateDicts.resize (fd_شمار)))
        برو fail;
      برای (unsigned صحیح i = 0; i < fd_شمار; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      اگر (is_CID ())
      {
	برای (unsigned صحیح i = 0; i < fd_شمار; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  اگر (unlikely (!fontDictStr.sanitize (&sc)))   برو fail;
	  hb_barrier ();
	  cff1_font_dict_مقادیر_t *font;
	  cff1_بالاترین_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  اگر (unlikely (fontDicts.in_error ()))   برو fail;

	  font->init ();
	  اگر (unlikely (!font_interp.interpret (*font)))   برو fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  اگر (unlikely (font->privateDictInfo.size &&
			privDictStr == (ثابت unsigned نویسه *) &Null (UnsizedByteStr))) برو fail;
	  رقم_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  اگر (unlikely (!priv_interp.interpret (*priv)))   برو fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      وگرنه  /* non-CID */
      {
	cff1_بالاترین_dict_مقادیر_t *font = &بالاترین_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	اگر (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ثابت unsigned نویسه *) &Null (UnsizedByteStr))) برو fail;
	رقم_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	اگر (unlikely (!priv_interp.interpret (*priv)))   برو fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      بازگشت;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    تهی _fini ()
    {
      sc.end_processing ();
      بالاترین_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ثابت { بازگشت blob; }

    منطقی is_valid () ثابت { بازگشت blob; }
    منطقی   is_CID () ثابت { بازگشت بالاترین_Dict_2.is_CID (); }

    منطقی is_predef_charset () ثابت { بازگشت بالاترین_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned صحیح std_code_to_glyph (hb_codepoint_t code) ثابت
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      اگر (unlikely (sid == CFF_UNDEF_SID))
	بازگشت 0;

      اگر (charset != &Null (Charset))
	بازگشت charset->get_glyph (sid, رقم_glyphs);
      وگرنه اگر ((بالاترین_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) بازگشت sid;
      بازگشت 0;
    }

    منطقی is_predef_encoding () ثابت { بازگشت بالاترین_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ثابت
    {
      اگر (encoding != &Null (Encoding))
	بازگشت encoding->get_code (glyph);
      وگرنه
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	اگر (sid == 0) بازگشت 0;
	hb_codepoint_t code = 0;
	انتخاب (بالاترین_Dict_2.EncodingOffset)
	{
	حالت StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  بشکن;
	حالت ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  بشکن;
	پیشفرض:
	  بشکن;
	}
	بازگشت code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ثابت
    {
      اگر (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (اندازه (glyph_to_sid_map_t));
	اگر (unlikely (!mapping)) بازگشت nullptr;
	mapping = جدید (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, رقم_glyphs);
	بازگشت mapping;
      }
      وگرنه
	بازگشت nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ثابت
    {
      اگر (charset != &Null (Charset))
	بازگشت charset->get_sid (glyph, رقم_glyphs, cache);
      وگرنه
      {
	hb_codepoint_t sid = 0;
	انتخاب (بالاترین_Dict_2.CharsetOffset)
	{
	  حالت ISOAdobeCharset:
	    اگر (glyph <= 228 /*zcaron*/) sid = glyph;
	    بشکن;
	  حالت ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    بشکن;
	  حالت ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    بشکن;
	  پیشفرض:
	    بشکن;
	}
	بازگشت sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ثابت
    {
      اگر (charset != &Null (Charset))
	بازگشت charset->get_glyph (sid, رقم_glyphs);
      وگرنه
      {
	hb_codepoint_t glyph = 0;
	انتخاب (بالاترین_Dict_2.CharsetOffset)
	{
	  حالت ISOAdobeCharset:
	    اگر (sid <= 228 /*zcaron*/) glyph = sid;
	    بشکن;
	  حالت ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    بشکن;
	  حالت ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    بشکن;
	  پیشفرض:
	    بشکن;
	}
	بازگشت glyph;
      }
    }

    محافظتشده:
    hb_sanitize_context_t   sc;

    عمومی:
    hb_blob_t               *blob = nullptr;
    ثابت Encoding	    *encoding = nullptr;
    ثابت Charset	    *charset = nullptr;
    ثابت CFF1NameIndex     *nameIndex = nullptr;
    ثابت CFF1_بالاترین_Dict_Index  *بالاترین_Dict_Index = nullptr;
    ثابت CFF1StringIndex   *stringIndex = nullptr;
    ثابت CFF1Subrs	    *globalSubrs = nullptr;
    ثابت CFF1CharStrings   *charStrings = nullptr;
    ثابت CFF1FDArray       *fdArray = nullptr;
    ثابت CFF1FDSelect      *fdSelect = nullptr;
    unsigned صحیح	     fd_شمار = 0;

    cff1_بالاترین_dict_مقادیر_t   بالاترین_Dict_2;
    hb_vector_t<cff1_font_dict_مقادیر_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned صحیح	     رقم_glyphs = 0;
    unsigned صحیح	     رقم_charset_entries = 0;
  };

  ساختار accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_مقادیر_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      اگر (!is_valid ()) بازگشت;
      اگر (is_CID ()) بازگشت;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      اگر (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    منطقی get_glyph_name (hb_codepoint_t glyph,
			 نویسه *buf, unsigned صحیح buf_len) ثابت
    {
      اگر (unlikely (glyph >= رقم_glyphs)) بازگشت نادرست;
      اگر (unlikely (!is_valid ())) بازگشت نادرست;
      اگر (is_CID()) بازگشت نادرست;
      اگر (unlikely (!buf_len)) بازگشت درست;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ثابت نویسه *str;
      size_t str_len;
      اگر (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      وگرنه
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ثابت نویسه *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      اگر (!str_len) بازگشت نادرست;
      unsigned صحیح len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ثابت نویسه*)str, len);
      buf[len] = '\0';
      بازگشت درست;
    }

    منطقی get_glyph_from_name (ثابت نویسه *name, صحیح len,
			      hb_codepoint_t *glyph) ثابت
    {
      اگر (unlikely (!is_valid ())) بازگشت نادرست;
      اگر (is_CID()) بازگشت نادرست;
      اگر (len < 0) len = strlen (name);
      اگر (unlikely (!len)) بازگشت نادرست;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      اگر (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, اندازه (hb_sorted_vector_t<gname_t>));
	اگر (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  برای (hb_codepoint_t gid = 0; gid < رقم_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    اگر (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    وگرنه
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ثابت نویسه*) ustr.arrayZ, ustr.length);
	    }
	    اگر (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	اگر (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  اگر (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  برو retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ثابت gname_t *gname = names ? names->bsearch (key) : nullptr;
      اگر (!gname) بازگشت نادرست;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      اگر (!gid && gname->sid) بازگشت نادرست;
      *glyph = gid;
      بازگشت درست;
    }

    HB_INTERNAL منطقی get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقی get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ثابت;

    خصوصی:
    ساختار gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      ایستا صحیح cmp (ثابت تهی *a_, ثابت تهی *b_)
      {
	ثابت gname_t *a = (ثابت gname_t *)a_;
	ثابت gname_t *b = (ثابت gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	صحیح ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	اگر (ret) بازگشت ret;
	بازگشت a->name.length - b->name.length;
      }

      صحیح cmp (ثابت gname_t &a) ثابت { بازگشت cmp (&a, این); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    تعریفنوع accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_مقادیر_t> SUPER;
  };

  ساختار accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_مقادیر_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      اگر (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL منطقی subset (hb_subset_context_t *c) ثابت;
    HB_INTERNAL منطقی serialize (hb_serialize_context_t *c,
				ساختار cff1_subset_plan &plan) ثابت;
    HB_INTERNAL منطقی get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ثابت;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    تعریفنوع accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_مقادیر_subset_t> SUPER;
  };

  محافظتشده:
  HB_INTERNAL ایستا hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ایستا hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL ایستا hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ایستا hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL ایستا hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ایستا hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL ایستا hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  عمومی:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

ساختار cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  تهی depend (hb_depend_data_builder_t *builder) ثابت
  {
    اگر (!is_valid ()) بازگشت;
    برای (hb_codepoint_t gid = 0; gid < رقم_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      اگر (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
