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

#অন্তর্ভাৱ "hb-ot-cff-common.hh"
#অন্তর্ভাৱ "hb-depend-data.hh"
#অন্তর্ভাৱ "hb-subset-cff-common.hh"
#অন্তর্ভাৱ "hb-draw.hh"
#অন্তর্ভাৱ "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#অন্তর্ভাৱ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

নামাকাশ CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

গণনা EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
গণনা CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

প্রকারনির্দেশ CFF1Index          CFF1CharStrings;
প্রকারনির্দেশ Subrs<HBUINT16>    CFF1Subrs;

সংরচনা CFF1FDSelect : FDSelect {};

/* Encoding */
সংরচনা Encoding0 {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুৱ
  {
    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    যদি (glyph < nCodes ())
    {
      ঘূৰাই (hb_codepoint_t)codes[glyph];
    }
    অন্যথা
      ঘূৰাই CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { ঘূৰাই codes.len; }
  HBUINT8 nCodes () ধ্রুৱ { ঘূৰাই codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

সংরচনা Encoding1_Range {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT8   first;
  HBUINT8   n_বাম;

  DEFINE_SIZE_STATIC (2);
};

সংরচনা Encoding1 {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুৱ
  {
    /* TODO: Add cache like get_sid. */
    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < nRanges (); i++)
    {
      যদি (glyph <= ranges[i].n_বাম)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	ঘূৰাই (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_বাম + 1);
    }
    ঘূৰাই CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { ঘূৰাই ranges.len; }
  HBUINT8 nRanges () ধ্রুৱ { ঘূৰাই ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

সংরচনা SuppEncoding {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

সংরচনা CFF1SuppEncData {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (supps.sanitize (c));
  }

  শূন্য get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ধ্রুৱ
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < nSups (); i++)
      যদি (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { ঘূৰাই supps.len; }
  HBUINT8 nSups () ধ্রুৱ { ঘূৰাই supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

সংরচনা Encoding
{
  /* serialize a fullset Encoding */
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ Encoding &src)
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  তার্কিক serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned পূর্ণাঙ্ক enc_গুনতি,
		  ধ্রুৱ hb_vector_t<code_pair_t>& code_ranges,
		  ধ্রুৱ hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (এতত্);
    Encoding *dest = c->extend_min (এতত্);
    যদি (unlikely (!dest)) return_trace (মিছা);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    ৱিকল্প (format) {
    পক্ষ 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_গুনতি);
      যদি (unlikely (!fmt0)) return_trace (মিছা);
      fmt0->nCodes () = enc_গুনতি;
      unsigned পূর্ণাঙ্ক glyph = 0;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	বাবে (পূর্ণাঙ্ক left = (পূর্ণাঙ্ক)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	যদি (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (মিছা);
      }
    }
    ভঙ্গ;

    পক্ষ 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      যদি (unlikely (!fmt1)) return_trace (মিছা);
      fmt1->nRanges () = code_ranges.length;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < code_ranges.length; i++)
      {
	যদি (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (মিছা);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_বাম = code_ranges[i].glyph;
      }
    }
    ভঙ্গ;

    }

    যদি (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      যদি (unlikely (!suppData)) return_trace (মিছা);
      suppData->nSups () = supp_codes.length;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (সঁচা);
  }

  size_t get_size () ধ্রুৱ
  {
    size_t size = min_size;
    ৱিকল্প (table_format ())
    {
    পক্ষ 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); ভঙ্গ;
    পক্ষ 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); ভঙ্গ;
    }
    যদি (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    ঘূৰাই size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুৱ
  {
    ৱিকল্প (table_format ())
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_code (glyph);
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_code (glyph);
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  uint8_t table_format () ধ্রুৱ { ঘূৰাই format & 0x7F; }
  তার্কিক  has_supplement () ধ্রুৱ { ঘূৰাই format & 0x80; }

  শূন্য get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ধ্রুৱ
  {
    codes.clear ();
    যদি (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (মিছা);
    hb_barrier ();

    ৱিকল্প (table_format ())
    {
    পক্ষ 0: hb_barrier (); যদি (unlikely (!u.format0.sanitize (c))) { return_trace (মিছা); } ভঙ্গ;
    পক্ষ 1: hb_barrier (); যদি (unlikely (!u.format1.sanitize (c))) { return_trace (মিছা); } ভঙ্গ;
    স্ৱভাৱ:return_trace (মিছা);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  রক্ষিত:
  ধ্রুৱ CFF1SuppEncData &suppEncData () ধ্রুৱ
  {
    ৱিকল্প (table_format ())
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    পক্ষ 1: hb_barrier (); ঘূৰাই StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    স্ৱভাৱ:ঘূৰাই Null (CFF1SuppEncData);
    }
  }

  সার্ৱজনিক:
  HBUINT8	format;
  সঙ্ঘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
সংরচনা Charset0
{
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs, unsigned *অঙ্ক_charset_entries) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (অঙ্ক_charset_entries) *অঙ্ক_charset_entries = অঙ্ক_glyphs;
    return_trace (sids.sanitize (c, অঙ্ক_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (unlikely (glyph >= অঙ্ক_glyphs)) ঘূৰাই 0;
    যদি (unlikely (glyph == 0))
      ঘূৰাই 0;
    অন্যথা
      ঘূৰাই sids[glyph - 1];
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (!mapping->resize_dirty  (অঙ্ক_glyphs)) ঘূৰাই;
    বাবে (hb_codepoint_t gid = 1; gid < অঙ্ক_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (sid == 0)
      ঘূৰাই 0;

    বাবে (unsigned পূর্ণাঙ্ক glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
    {
      যদি (sids[glyph-1] == sid)
	ঘূৰাই glyph;
    }
    ঘূৰাই 0;
  }

  স্থির size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs)
  {
    প্রতিজ্ঞা (অঙ্ক_glyphs > 0);
    ঘূৰাই UnsizedArrayOf<HBUINT16>::get_size (অঙ্ক_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
সংরচনা Charset_Range {
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT16  first;
  TYPE      n_বাম;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
সংরচনা Charset1_2 {
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs, unsigned *অঙ্ক_charset_entries) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    অঙ্ক_glyphs--;
    unsigned i;
    বাবে (i = 0; অঙ্ক_glyphs > 0; i++)
    {
      যদি (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (অঙ্ক_glyphs >= ranges[i].n_বাম + 1))))
	return_trace (মিছা);
      অঙ্ক_glyphs -= (ranges[i].n_বাম + 1);
    }
    যদি (অঙ্ক_charset_entries)
      *অঙ্ক_charset_entries = i;
    return_trace (সঁচা);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned অঙ্ক_glyphs,
			  code_pair_t *cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (glyph >= অঙ্ক_glyphs)) ঘূৰাই 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    যদি (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    অন্যথা
    {
      যদি (unlikely (glyph == 0)) ঘূৰাই 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    বাবে (;; i++)
    {
      unsigned গুনতি = ranges[i].n_বাম;
      যদি (glyph <= গুনতি)
      {
        যদি (cache)
	  *cache = {i, start_glyph};
	ঘূৰাই ranges[i].first + glyph;
      }
      গুনতি++;
      start_glyph += গুনতি;
      glyph -= গুনতি;
    }

    ঘূৰাই 0;
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (!mapping->resize_dirty  (অঙ্ক_glyphs)) ঘূৰাই;

    hb_codepoint_t gid = 1;
    যদি (gid >= অঙ্ক_glyphs)
      ঘূৰাই;

    বাবে (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned গুনতি = ranges[i].n_বাম + 1;
      unsigned last = gid + গুনতি;
      বাবে (unsigned j = 0; j < গুনতি; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      যদি (gid >= অঙ্ক_glyphs)
        ভঙ্গ;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (sid == 0) ঘূৰাই 0;
    hb_codepoint_t  glyph = 1;
    বাবে (unsigned পূর্ণাঙ্ক i = 0;; i++)
    {
      যদি (glyph >= অঙ্ক_glyphs)
	ঘূৰাই 0;
      যদি ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_বাম))
	ঘূৰাই glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_বাম + 1);
    }

    ঘূৰাই 0;
  }

  size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    পূর্ণাঙ্ক glyph = (পূর্ণাঙ্ক) অঙ্ক_glyphs;
    unsigned অঙ্ক_ranges = 0;

    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_বাম + 1);
      অঙ্ক_ranges++;
    }

    ঘূৰাই get_size_for_ranges (অঙ্ক_ranges);
  }

  স্থির size_t get_size_for_ranges (unsigned পূর্ণাঙ্ক অঙ্ক_ranges)
  {
    ঘূৰাই UnsizedArrayOf<Charset_Range<TYPE> >::get_size (অঙ্ক_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

প্রকারনির্দেশ Charset1_2<HBUINT8>     Charset1;
প্রকারনির্দেশ Charset1_2<HBUINT16>    Charset2;
প্রকারনির্দেশ Charset_Range<HBUINT8>  Charset1_Range;
প্রকারনির্দেশ Charset_Range<HBUINT16> Charset2_Range;

সংরচনা Charset
{
  /* serialize a fullset Charset */
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ Charset &src, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed ((ধ্রুৱ ৱর্ণ *) &src, src.get_size (অঙ্ক_glyphs)));
  }

  /* serialize a subset Charset */
  তার্কিক serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
		  ধ্রুৱ hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (এতত্);
    Charset *dest = c->extend_min (এতত্);
    যদি (unlikely (!dest)) return_trace (মিছা);
    dest->format = format;
    ৱিকল্প (format)
    {
    পক্ষ 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (অঙ্ক_glyphs), মিছা);
      যদি (unlikely (!fmt0)) return_trace (মিছা);
      unsigned পূর্ণাঙ্ক glyph = 0;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	বাবে (পূর্ণাঙ্ক left = (পূর্ণাঙ্ক)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    ভঙ্গ;

    পক্ষ 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), মিছা);
      যদি (unlikely (!fmt1)) return_trace (মিছা);
      hb_codepoint_t all_glyphs = 0;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_বাম = _.glyph;
      }
      যদি (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (মিছা);
    }
    ভঙ্গ;

    পক্ষ 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), মিছা);
      যদি (unlikely (!fmt2)) return_trace (মিছা);
      hb_codepoint_t all_glyphs = 0;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_বাম = _.glyph;
      }
      যদি (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (মিছা);
    }
    ভঙ্গ;

    }
    return_trace (সঁচা);
  }

  size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই hb_unsigned_add_saturate (min_size, u.format0.get_size (অঙ্ক_glyphs));
    পক্ষ 1: hb_barrier (); ঘূৰাই hb_unsigned_add_saturate (min_size, u.format1.get_size (অঙ্ক_glyphs));
    পক্ষ 2: hb_barrier (); ঘূৰাই hb_unsigned_add_saturate (min_size, u.format2.get_size (অঙ্ক_glyphs));
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			  code_pair_t *cache = nullptr) ধ্রুৱ
  {
    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_sid (glyph, অঙ্ক_glyphs);
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_sid (glyph, অঙ্ক_glyphs, cache);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_sid (glyph, অঙ্ক_glyphs, cache);
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ঘূৰাই;
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_glyph (sid, অঙ্ক_glyphs);
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_glyph (sid, অঙ্ক_glyphs);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_glyph (sid, অঙ্ক_glyphs);
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned *অঙ্ক_charset_entries) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (মিছা);
    hb_barrier ();

    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  HBUINT8       format;
  সঙ্ঘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

সংরচনা CFF1StringIndex : CFF1Index
{
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ CFF1StringIndex &strings,
		  ধ্রুৱ hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely ((strings.গুনতি == 0) || (sidmap.length == 0)))
    {
      যদি (unlikely (!c->extend_min (এতত্->গুনতি)))
	return_trace (মিছা);
      গুনতি = 0;
      return_trace (সঁচা);
    }

    যদি (unlikely (sidmap.in_error ())) return_trace (মিছা);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    যদি (unlikely (bytesArray.in_error ())) return_trace (মিছা);

    তার্কিক result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

সংরচনা cff1_শীৰ্ষ_dict_interp_env_t : অঙ্ক_interp_env_t
{
  cff1_শীৰ্ষ_dict_interp_env_t ()
    : অঙ্ক_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_শীৰ্ষ_dict_interp_env_t (ধ্রুৱ hb_ubytes_t &bytes)
    : অঙ্ক_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned পূর্ণাঙ্ক prev_offset;
  unsigned পূর্ণাঙ্ক last_offset;
};

সংরচনা name_dict_মানসমূহ_t
{
  গণনা name_dict_val_index_t
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

      Val_গুনতি
  };

  শূন্য init ()
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < Val_গুনতি; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned পূর্ণাঙ্ক& operator[] (unsigned পূর্ণাঙ্ক i)
  { প্রতিজ্ঞা (i < Val_গুনতি); ঘূৰাই values[i]; }

  unsigned পূর্ণাঙ্ক operator[] (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { প্রতিজ্ঞা (i < Val_গুনতি); ঘূৰাই values[i]; }

  স্থির গণনা name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    ৱিকল্প (op) {
      স্ৱভাৱ: // can't happen - just make some compiler happy
      পক্ষ OpCode_version:
	ঘূৰাই version;
      পক্ষ OpCode_Notice:
	ঘূৰাই notice;
      পক্ষ OpCode_Copyright:
	ঘূৰাই copyright;
      পক্ষ OpCode_FullName:
	ঘূৰাই fullName;
      পক্ষ OpCode_FamilyName:
	ঘূৰাই familyName;
      পক্ষ OpCode_Weight:
	ঘূৰাই weight;
      পক্ষ OpCode_PostScript:
	ঘূৰাই postscript;
      পক্ষ OpCode_FontName:
	ঘূৰাই fontName;
      পক্ষ OpCode_BaseFontName:
	ঘূৰাই baseFontName;
    }
  }

  unsigned পূর্ণাঙ্ক  values[Val_গুনতি];
};

সংরচনা cff1_শীৰ্ষ_dict_val_t : op_str_t
{
  unsigned পূর্ণাঙ্ক  last_arg_offset;
};

সংরচনা cff1_শীৰ্ষ_dict_মানসমূহ_t : শীৰ্ষ_dict_মানসমূহ_t<cff1_শীৰ্ষ_dict_val_t>
{
  শূন্য init ()
  {
    শীৰ্ষ_dict_মানসমূহ_t<cff1_শীৰ্ষ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_গুনতি = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  শূন্য fini () { শীৰ্ষ_dict_মানসমূহ_t<cff1_শীৰ্ষ_dict_val_t>::fini (); }

  তার্কিক is_CID () ধ্রুৱ
  { ঘূৰাই nameSIDs[name_dict_মানসমূহ_t::registry] != CFF_UNDEF_SID; }

  name_dict_মানসমূহ_t  nameSIDs;
  unsigned পূর্ণাঙ্ক    ros_supplement_offset;
  unsigned পূর্ণাঙ্ক    ros_supplement;
  unsigned পূর্ণাঙ্ক    cid_গুনতি;

  পূর্ণাঙ্ক             EncodingOffset;
  পূর্ণাঙ্ক             CharsetOffset;
  পূর্ণাঙ্ক             FDSelectOffset;
  table_info_t    privateDictInfo;
};

সংরচনা cff1_শীৰ্ষ_dict_opset_t : শীৰ্ষ_dict_opset_t<cff1_শীৰ্ষ_dict_val_t>
{
  স্থির শূন্য process_op (op_code_t op, cff1_শীৰ্ষ_dict_interp_env_t& env, cff1_শীৰ্ষ_dict_মানসমূহ_t& dictval)
  {
    cff1_শীৰ্ষ_dict_val_t  val {};

    ৱিকল্প (op) {
      পক্ষ OpCode_version:
      পক্ষ OpCode_Notice:
      পক্ষ OpCode_Copyright:
      পক্ষ OpCode_FullName:
      পক্ষ OpCode_FontName:
      পক্ষ OpCode_FamilyName:
      পক্ষ OpCode_Weight:
      পক্ষ OpCode_PostScript:
      পক্ষ OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_মানসমূহ_t::name_op_to_index (op)] = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_isFixedPitch:
      পক্ষ OpCode_ItalicAngle:
      পক্ষ OpCode_UnderlinePosition:
      পক্ষ OpCode_UnderlineThickness:
      পক্ষ OpCode_PaintType:
      পক্ষ OpCode_CharstringType:
      পক্ষ OpCode_UniqueID:
      পক্ষ OpCode_StrokeWidth:
      পক্ষ OpCode_SyntheticBase:
      পক্ষ OpCode_CIDFontVersion:
      পক্ষ OpCode_CIDFontRevision:
      পক্ষ OpCode_CIDFontType:
      পক্ষ OpCode_UIDBase:
      পক্ষ OpCode_FontBBox:
      পক্ষ OpCode_XUID:
      পক্ষ OpCode_BaseFontBlend:
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_CIDCount:
	dictval.cid_গুনতি = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_ROS:
	dictval.ros_supplement = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	dictval.nameSIDs[name_dict_মানসমূহ_t::ordering] = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	dictval.nameSIDs[name_dict_মানসমূহ_t::registry] = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	যদি (unlikely (env.in_error ())) ঘূৰাই;
	প্রতিজ্ঞা (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	ভঙ্গ;

      পক্ষ OpCode_Encoding:
	dictval.EncodingOffset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	env.clear_args ();
	যদি (unlikely (dictval.EncodingOffset == 0)) ঘূৰাই;
	ভঙ্গ;

      পক্ষ OpCode_charset:
	dictval.CharsetOffset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	env.clear_args ();
	যদি (unlikely (dictval.CharsetOffset == 0)) ঘূৰাই;
	ভঙ্গ;

      পক্ষ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	dictval.privateDictInfo.size = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	ভঙ্গ;

      স্ৱভাৱ:
	env.last_offset = env.str_ref.get_offset ();
	শীৰ্ষ_dict_opset_t<cff1_শীৰ্ষ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	যদি (!env.arg_স্তূপ.is_empty ()) ঘূৰাই;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) ঘূৰাই;

    dictval.add_op (op, env.str_ref, val);
  }
};

সংরচনা cff1_font_dict_মানসমূহ_t : dict_মানসমূহ_t<op_str_t>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  শূন্য fini () { dict_মানসমূহ_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned পূর্ণাঙ্ক    fontName;
};

সংরচনা cff1_font_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_font_dict_মানসমূহ_t& dictval)
  {
    ৱিকল্প (op) {
      পক্ষ OpCode_FontName:
	dictval.fontName = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_FontMatrix:
      পক্ষ OpCode_PaintType:
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	dictval.privateDictInfo.size = env.arg_স্তূপ.বেৰ_কৰো_uint ();
	env.clear_args ();
	ভঙ্গ;

      স্ৱভাৱ:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ঘূৰাই;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) ঘূৰাই;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
সংরচনা cff1_private_dict_মানসমূহ_base_t : dict_মানসমূহ_t<VAL>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  শূন্য fini () { dict_মানসমূহ_t<VAL>::fini (); }

  পূর্ণাঙ্ক                 subrsOffset;
  ধ্রুৱ CFF1Subrs    *localSubrs;
};

প্রকারনির্দেশ cff1_private_dict_মানসমূহ_base_t<op_str_t> cff1_private_dict_মানসমূহ_subset_t;
প্রকারনির্দেশ cff1_private_dict_মানসমূহ_base_t<অঙ্ক_dict_val_t> cff1_private_dict_মানসমূহ_t;

সংরচনা cff1_private_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_private_dict_মানসমূহ_t& dictval)
  {
    অঙ্ক_dict_val_t val;
    val.init ();

    ৱিকল্প (op) {
      পক্ষ Op_Code_Blue_মানসমূহ:
      পক্ষ OpCode_OtherBlues:
      পক্ষ OpCode_FamilyBlues:
      পক্ষ OpCode_FamilyOtherBlues:
      পক্ষ OpCode_StemSnapH:
      পক্ষ OpCode_StemSnapV:
      পক্ষ OpCode_StdHW:
      পক্ষ OpCode_StdVW:
      পক্ষ OpCode_BlueScale:
      পক্ষ OpCode_BlueShift:
      পক্ষ OpCode_BlueFuzz:
      পক্ষ OpCode_ForceBold:
      পক্ষ OpCode_LanguageGroup:
      পক্ষ OpCode_ExpansionFactor:
      পক্ষ OpCode_initialRandomSeed:
      পক্ষ OpCode_defaultWidthX:
      পক্ষ OpCode_nominalWidthX:
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	env.clear_args ();
	ভঙ্গ;

      স্ৱভাৱ:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ঘূৰাই;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) ঘূৰাই;

    dictval.add_op (op, env.str_ref, val);
  }
};

সংরচনা cff1_private_dict_opset_subset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_private_dict_মানসমূহ_subset_t& dictval)
  {
    ৱিকল্প (op) {
      পক্ষ Op_Code_Blue_মানসমূহ:
      পক্ষ OpCode_OtherBlues:
      পক্ষ OpCode_FamilyBlues:
      পক্ষ OpCode_FamilyOtherBlues:
      পক্ষ OpCode_StemSnapH:
      পক্ষ OpCode_StemSnapV:
      পক্ষ OpCode_StdHW:
      পক্ষ OpCode_StdVW:
      পক্ষ OpCode_BlueScale:
      পক্ষ OpCode_BlueShift:
      পক্ষ OpCode_BlueFuzz:
      পক্ষ OpCode_ForceBold:
      পক্ষ OpCode_LanguageGroup:
      পক্ষ OpCode_ExpansionFactor:
      পক্ষ OpCode_initialRandomSeed:
      পক্ষ OpCode_defaultWidthX:
      পক্ষ OpCode_nominalWidthX:
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বেৰ_কৰো_int ();
	env.clear_args ();
	ভঙ্গ;

      স্ৱভাৱ:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ঘূৰাই;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) ঘূৰাই;

    dictval.add_op (op, env.str_ref);
  }
};

প্রকারনির্দেশ dict_interpreter_t<cff1_শীৰ্ষ_dict_opset_t, cff1_শীৰ্ষ_dict_মানসমূহ_t, cff1_শীৰ্ষ_dict_interp_env_t> cff1_শীৰ্ষ_dict_interpreter_t;
প্রকারনির্দেশ dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_মানসমূহ_t> cff1_font_dict_interpreter_t;

প্রকারনির্দেশ CFF1Index CFF1NameIndex;
প্রকারনির্দেশ CFF1Index CFF1_শীৰ্ষ_Dict_Index;

সংরচনা cff1_font_dict_মানসমূহ_mod_t
{
  cff1_font_dict_মানসমূহ_mod_t() { init (); }

  শূন্য init () { init ( &Null (cff1_font_dict_মানসমূহ_t), CFF_UNDEF_SID ); }

  শূন্য init (ধ্রুৱ cff1_font_dict_মানসমূহ_t *base_,
	     unsigned পূর্ণাঙ্ক fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_গুনতি () ধ্রুৱ { ঘূৰাই base->get_গুনতি (); }

  ধ্রুৱ op_str_t &operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুৱ { ঘূৰাই (*base)[i]; }

  ধ্রুৱ cff1_font_dict_মানসমূহ_t    *base;
  table_info_t		   privateDictInfo;
  unsigned পূর্ণাঙ্ক		fontName;
};

সংরচনা CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  তার্কিক serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ঘূৰাই FDArray<HBUINT16>::serialize<cff1_font_dict_মানসমূহ_mod_t, cff1_font_dict_মানসমূহ_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

নামাকাশ OT {

using নামাকাশ CFF;

সংরচনা cff1
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  সংরচনা accelerator_templ_t
  {
    স্থির constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      যদি (!face) ঘূৰাই;

      শীৰ্ষ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      এতত্->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (এতত্->blob);
      sc.start_processing ();

      ধ্রুৱ OT::cff1 *cff = এতত্->blob->template as<OT::cff1> ();

      যদি (cff == &Null (OT::cff1))
        গচ্ছ fail;

      nameIndex = &cff->nameIndex (cff);
      যদি ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        গচ্ছ fail;
      hb_barrier ();

      শীৰ্ষ_Dict_Index = &StructAtOffsetOrNull<CFF1_শীৰ্ষ_Dict_Index> (nameIndex, nameIndex->get_size (), sc);
      যদি (শীৰ্ষ_Dict_Index == &Null (CFF1_শীৰ্ষ_Dict_Index) || (শীৰ্ষ_Dict_Index->গুনতি == 0))
        গচ্ছ fail;
      hb_barrier ();

      { /* parse top dict */
	ধ্রুৱ hb_ubytes_t শীৰ্ষ_Dict_Str = (*শীৰ্ষ_Dict_Index)[0];
	যদি (unlikely (!শীৰ্ষ_Dict_Str.sanitize (&sc)))   গচ্ছ fail;
	hb_barrier ();
	cff1_শীৰ্ষ_dict_interp_env_t env (শীৰ্ষ_Dict_Str);
	cff1_শীৰ্ষ_dict_interpreter_t শীৰ্ষ_interp (env);
	যদি (unlikely (!শীৰ্ষ_interp.interpret (শীৰ্ষ_Dict_2)))   গচ্ছ fail;
      }

      যদি (is_predef_charset ())
	charset = &Null (Charset);
      অন্যথা
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, শীৰ্ষ_Dict_2.CharsetOffset, sc, &অঙ্ক_charset_entries);
	যদি (unlikely (charset == &Null (Charset)))   গচ্ছ fail;
      }

      fd_গুনতি = 1;
      যদি (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, শীৰ্ষ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, শীৰ্ষ_Dict_2.FDSelectOffset, sc, fdArray->গুনতি);
	যদি (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  গচ্ছ fail;

	fd_গুনতি = fdArray->গুনতি;
      }
      অন্যথা
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      যদি (is_CID ())
      {
	যদি (unlikely (charset == &Null (Charset)))   গচ্ছ fail;
      }
      অন্যথা
      {
	যদি (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, শীৰ্ষ_Dict_2.EncodingOffset, sc);
	  যদি (unlikely (encoding == &Null (Encoding)))   গচ্ছ fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (শীৰ্ষ_Dict_Index, শীৰ্ষ_Dict_Index->get_size (), sc);
      যদি (stringIndex == &Null (CFF1StringIndex))
        গচ্ছ fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, শীৰ্ষ_Dict_2.charStringsOffset, sc);
      যদি (charStrings == &Null (CFF1CharStrings))
        গচ্ছ fail;

      অঙ্ক_glyphs = charStrings->গুনতি;
      যদি (অঙ্ক_glyphs != sc.get_অঙ্ক_glyphs ())
        গচ্ছ fail;

      যদি (unlikely (!privateDicts.resize (fd_গুনতি)))
        গচ্ছ fail;
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < fd_গুনতি; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      যদি (is_CID ())
      {
	বাবে (unsigned পূর্ণাঙ্ক i = 0; i < fd_গুনতি; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  যদি (unlikely (!fontDictStr.sanitize (&sc)))   গচ্ছ fail;
	  hb_barrier ();
	  cff1_font_dict_মানসমূহ_t *font;
	  cff1_শীৰ্ষ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  যদি (unlikely (fontDicts.in_error ()))   গচ্ছ fail;

	  font->init ();
	  যদি (unlikely (!font_interp.interpret (*font)))   গচ্ছ fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ধ্রুৱ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  যদি (unlikely (font->privateDictInfo.size &&
			privDictStr == (ধ্রুৱ unsigned ৱর্ণ *) &Null (UnsizedByteStr))) গচ্ছ fail;
	  অঙ্ক_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  যদি (unlikely (!priv_interp.interpret (*priv)))   গচ্ছ fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      অন্যথা  /* non-CID */
      {
	cff1_শীৰ্ষ_dict_মানসমূহ_t *font = &শীৰ্ষ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ধ্রুৱ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	যদি (font->privateDictInfo.size &&
	    unlikely (privDictStr == (ধ্রুৱ unsigned ৱর্ণ *) &Null (UnsizedByteStr))) গচ্ছ fail;
	অঙ্ক_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	যদি (unlikely (!priv_interp.interpret (*priv)))   গচ্ছ fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      ঘূৰাই;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    শূন্য _fini ()
    {
      sc.end_processing ();
      শীৰ্ষ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ধ্রুৱ { ঘূৰাই blob; }

    তার্কিক is_valid () ধ্রুৱ { ঘূৰাই blob; }
    তার্কিক   is_CID () ধ্রুৱ { ঘূৰাই শীৰ্ষ_Dict_2.is_CID (); }

    তার্কিক is_predef_charset () ধ্রুৱ { ঘূৰাই শীৰ্ষ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned পূর্ণাঙ্ক std_code_to_glyph (hb_codepoint_t code) ধ্রুৱ
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      যদি (unlikely (sid == CFF_UNDEF_SID))
	ঘূৰাই 0;

      যদি (charset != &Null (Charset))
	ঘূৰাই charset->get_glyph (sid, অঙ্ক_glyphs);
      অন্যথা যদি ((শীৰ্ষ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) ঘূৰাই sid;
      ঘূৰাই 0;
    }

    তার্কিক is_predef_encoding () ধ্রুৱ { ঘূৰাই শীৰ্ষ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ধ্রুৱ
    {
      যদি (encoding != &Null (Encoding))
	ঘূৰাই encoding->get_code (glyph);
      অন্যথা
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	যদি (sid == 0) ঘূৰাই 0;
	hb_codepoint_t code = 0;
	ৱিকল্প (শীৰ্ষ_Dict_2.EncodingOffset)
	{
	পক্ষ StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  ভঙ্গ;
	পক্ষ ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  ভঙ্গ;
	স্ৱভাৱ:
	  ভঙ্গ;
	}
	ঘূৰাই code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ধ্রুৱ
    {
      যদি (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (পরিমাণ (glyph_to_sid_map_t));
	যদি (unlikely (!mapping)) ঘূৰাই nullptr;
	mapping = নূতন (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs);
	ঘূৰাই mapping;
      }
      অন্যথা
	ঘূৰাই nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ধ্রুৱ
    {
      যদি (charset != &Null (Charset))
	ঘূৰাই charset->get_sid (glyph, অঙ্ক_glyphs, cache);
      অন্যথা
      {
	hb_codepoint_t sid = 0;
	ৱিকল্প (শীৰ্ষ_Dict_2.CharsetOffset)
	{
	  পক্ষ ISOAdobeCharset:
	    যদি (glyph <= 228 /*zcaron*/) sid = glyph;
	    ভঙ্গ;
	  পক্ষ ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    ভঙ্গ;
	  পক্ষ ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    ভঙ্গ;
	  স্ৱভাৱ:
	    ভঙ্গ;
	}
	ঘূৰাই sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ধ্রুৱ
    {
      যদি (charset != &Null (Charset))
	ঘূৰাই charset->get_glyph (sid, অঙ্ক_glyphs);
      অন্যথা
      {
	hb_codepoint_t glyph = 0;
	ৱিকল্প (শীৰ্ষ_Dict_2.CharsetOffset)
	{
	  পক্ষ ISOAdobeCharset:
	    যদি (sid <= 228 /*zcaron*/) glyph = sid;
	    ভঙ্গ;
	  পক্ষ ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    ভঙ্গ;
	  পক্ষ ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    ভঙ্গ;
	  স্ৱভাৱ:
	    ভঙ্গ;
	}
	ঘূৰাই glyph;
      }
    }

    রক্ষিত:
    hb_sanitize_context_t   sc;

    সার্ৱজনিক:
    hb_blob_t               *blob = nullptr;
    ধ্রুৱ Encoding	    *encoding = nullptr;
    ধ্রুৱ Charset	    *charset = nullptr;
    ধ্রুৱ CFF1NameIndex     *nameIndex = nullptr;
    ধ্রুৱ CFF1_শীৰ্ষ_Dict_Index  *শীৰ্ষ_Dict_Index = nullptr;
    ধ্রুৱ CFF1StringIndex   *stringIndex = nullptr;
    ধ্রুৱ CFF1Subrs	    *globalSubrs = nullptr;
    ধ্রুৱ CFF1CharStrings   *charStrings = nullptr;
    ধ্রুৱ CFF1FDArray       *fdArray = nullptr;
    ধ্রুৱ CFF1FDSelect      *fdSelect = nullptr;
    unsigned পূর্ণাঙ্ক	     fd_গুনতি = 0;

    cff1_শীৰ্ষ_dict_মানসমূহ_t   শীৰ্ষ_Dict_2;
    hb_vector_t<cff1_font_dict_মানসমূহ_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned পূর্ণাঙ্ক	     অঙ্ক_glyphs = 0;
    unsigned পূর্ণাঙ্ক	     অঙ্ক_charset_entries = 0;
  };

  সংরচনা accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_মানসমূহ_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      যদি (!is_valid ()) ঘূৰাই;
      যদি (is_CID ()) ঘূৰাই;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      যদি (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    তার্কিক get_glyph_name (hb_codepoint_t glyph,
			 ৱর্ণ *buf, unsigned পূর্ণাঙ্ক buf_len) ধ্রুৱ
    {
      যদি (unlikely (glyph >= অঙ্ক_glyphs)) ঘূৰাই মিছা;
      যদি (unlikely (!is_valid ())) ঘূৰাই মিছা;
      যদি (is_CID()) ঘূৰাই মিছা;
      যদি (unlikely (!buf_len)) ঘূৰাই সঁচা;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ধ্রুৱ ৱর্ণ *str;
      size_t str_len;
      যদি (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      অন্যথা
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (ধ্রুৱ ৱর্ণ *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      যদি (!str_len) ঘূৰাই মিছা;
      unsigned পূর্ণাঙ্ক len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ধ্রুৱ ৱর্ণ*)str, len);
      buf[len] = '\0';
      ঘূৰাই সঁচা;
    }

    তার্কিক get_glyph_from_name (ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক len,
			      hb_codepoint_t *glyph) ধ্রুৱ
    {
      যদি (unlikely (!is_valid ())) ঘূৰাই মিছা;
      যদি (is_CID()) ঘূৰাই মিছা;
      যদি (len < 0) len = strlen (name);
      যদি (unlikely (!len)) ঘূৰাই মিছা;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      যদি (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, পরিমাণ (hb_sorted_vector_t<gname_t>));
	যদি (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  বাবে (hb_codepoint_t gid = 0; gid < অঙ্ক_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    যদি (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    অন্যথা
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((ধ্রুৱ ৱর্ণ*) ustr.arrayZ, ustr.length);
	    }
	    যদি (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	যদি (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  যদি (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  গচ্ছ retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ধ্রুৱ gname_t *gname = names ? names->bsearch (key) : nullptr;
      যদি (!gname) ঘূৰাই মিছা;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      যদি (!gid && gname->sid) ঘূৰাই মিছা;
      *glyph = gid;
      ঘূৰাই সঁচা;
    }

    HB_INTERNAL তার্কিক get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ধ্রুৱ;
    HB_INTERNAL তার্কিক get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ধ্রুৱ;

    গুপ্ত:
    সংরচনা gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      স্থির পূর্ণাঙ্ক cmp (ধ্রুৱ শূন্য *a_, ধ্রুৱ শূন্য *b_)
      {
	ধ্রুৱ gname_t *a = (ধ্রুৱ gname_t *)a_;
	ধ্রুৱ gname_t *b = (ধ্রুৱ gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	পূর্ণাঙ্ক ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	যদি (ret) ঘূৰাই ret;
	ঘূৰাই a->name.length - b->name.length;
      }

      পূর্ণাঙ্ক cmp (ধ্রুৱ gname_t &a) ধ্রুৱ { ঘূৰাই cmp (&a, এতত্); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    প্রকারনির্দেশ accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_মানসমূহ_t> SUPER;
  };

  সংরচনা accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_মানসমূহ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      যদি (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ;
    HB_INTERNAL তার্কিক serialize (hb_serialize_context_t *c,
				সংরচনা cff1_subset_plan &plan) ধ্রুৱ;
    HB_INTERNAL তার্কিক get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ধ্রুৱ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    প্রকারনির্দেশ accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_মানসমূহ_subset_t> SUPER;
  };

  রক্ষিত:
  HB_INTERNAL স্থির hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  সার্ৱজনিক:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

সংরচনা cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  শূন্য depend (hb_depend_data_builder_t *builder) ধ্রুৱ
  {
    যদি (!is_valid ()) ঘূৰাই;
    বাবে (hb_codepoint_t gid = 0; gid < অঙ্ক_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      যদি (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
