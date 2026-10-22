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

#অন্তর্ভাব "hb-ot-cff-common.hh"
#অন্তর্ভাব "hb-depend-data.hh"
#অন্তর্ভাব "hb-subset-cff-common.hh"
#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#অন্তর্ভাব "hb-string-array.hh"
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

প্রকারলেখ CFF1_সূচক          CFF1CharStrings;
প্রকারলেখ Subrs<HBUINT16>    CFF1Subrs;

কাঠামো CFF1FDSelect : FDSelect {};

/* Encoding */
কাঠামো Encoding0 {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুবক
  {
    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    যদি (glyph < nCodes ())
    {
      ফেরত (hb_codepoint_t)codes[glyph];
    }
    অন্যথা
      ফেরত CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { ফেরত codes.len; }
  HBUINT8 nCodes () ধ্রুবক { ফেরত codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_আকার_ARRAY_SIZED (1, codes);
};

কাঠামো Encoding1_Range {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT8   first;
  HBUINT8   n_বাম;

  DEFINE_আকার_STATIC (2);
};

কাঠামো Encoding1 {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুবক
  {
    /* TODO: Add cache like get_sid. */
    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < nRanges (); i++)
    {
      যদি (glyph <= ranges[i].n_বাম)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	ফেরত (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].n_বাম + 1);
    }
    ফেরত CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { ফেরত ranges.len; }
  HBUINT8 nRanges () ধ্রুবক { ফেরত ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_আকার_ARRAY_SIZED (1, ranges);
};

কাঠামো SuppEncoding {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_আকার_STATIC (3);
};

কাঠামো CFF1_Supp_Enc_তথ্য {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (supps.sanitize (c));
  }

  শূন্য get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < nSups (); i++)
      যদি (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { ফেরত supps.len; }
  HBUINT8 nSups () ধ্রুবক { ফেরত supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_আকার_ARRAY_SIZED (1, supps);
};

কাঠামো Encoding
{
  /* serialize a fullset Encoding */
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক Encoding &src)
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned পূর্ণসংখ্যা enc_গুনতি,
		  ধ্রুবক hb_vector_t<code_pair_t>& code_ranges,
		  ধ্রুবক hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (এই);
    Encoding *dest = c->extend_min (এই);
    যদি (unlikely (!dest)) return_trace (মিথ্যা);
    dest->format = format | ((supp_codes.দৈর্ঘ্য > 0) ? 0x80 : 0);
    নির্বাচন (format) {
    ক্ষেত্র 0:
    {
      Encoding0 *fmt0 = c->allocate_আকার<Encoding0> (Encoding0::min_আকার + HBUINT8::static_আকার * enc_গুনতি);
      যদি (unlikely (!fmt0)) return_trace (মিথ্যা);
      fmt0->nCodes () = enc_গুনতি;
      unsigned পূর্ণসংখ্যা glyph = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < code_ranges.দৈর্ঘ্য; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	জন্য (পূর্ণসংখ্যা left = (পূর্ণসংখ্যা)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	যদি (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (মিথ্যা);
      }
    }
    বিরতি;

    ক্ষেত্র 1:
    {
      Encoding1 *fmt1 = c->allocate_আকার<Encoding1> (Encoding1::min_আকার + Encoding1_Range::static_আকার * code_ranges.দৈর্ঘ্য);
      যদি (unlikely (!fmt1)) return_trace (মিথ্যা);
      fmt1->nRanges () = code_ranges.দৈর্ঘ্য;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < code_ranges.দৈর্ঘ্য; i++)
      {
	যদি (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (মিথ্যা);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].n_বাম = code_ranges[i].glyph;
      }
    }
    বিরতি;

    }

    যদি (supp_codes.দৈর্ঘ্য)
    {
      CFF1_Supp_Enc_তথ্য *supp_তথ্য = c->allocate_আকার<CFF1_Supp_Enc_তথ্য> (CFF1_Supp_Enc_তথ্য::min_আকার + SuppEncoding::static_আকার * supp_codes.দৈর্ঘ্য);
      যদি (unlikely (!supp_তথ্য)) return_trace (মিথ্যা);
      supp_তথ্য->nSups () = supp_codes.দৈর্ঘ্য;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < supp_codes.দৈর্ঘ্য; i++)
      {
	supp_তথ্য->supps[i].code = supp_codes[i].code;
	supp_তথ্য->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (সত্য);
  }

  size_t get_আকার () ধ্রুবক
  {
    size_t আকার_2 = min_আকার;
    নির্বাচন (table_format ())
    {
    ক্ষেত্র 0: hb_barrier (); আকার_2 = hb_unsigned_add_saturate (আকার_2, u.format0.get_আকার ()); বিরতি;
    ক্ষেত্র 1: hb_barrier (); আকার_2 = hb_unsigned_add_saturate (আকার_2, u.format1.get_আকার ()); বিরতি;
    }
    যদি (has_supplement ())
      আকার_2 = hb_unsigned_add_saturate (আকার_2, supp_Enc_তথ্য ().get_আকার ());
    ফেরত আকার_2;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) ধ্রুবক
  {
    নির্বাচন (table_format ())
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_code (glyph);
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_code (glyph);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  uint8_t table_format () ধ্রুবক { ফেরত format & 0x7F; }
  যৌক্তিক  has_supplement () ধ্রুবক { ফেরত format & 0x80; }

  শূন্য get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) ধ্রুবক
  {
    codes.clear ();
    যদি (has_supplement ())
      supp_Enc_তথ্য().get_codes (sid, codes);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    নির্বাচন (table_format ())
    {
    ক্ষেত্র 0: hb_barrier (); যদি (unlikely (!u.format0.sanitize (c))) { return_trace (মিথ্যা); } বিরতি;
    ক্ষেত্র 1: hb_barrier (); যদি (unlikely (!u.format1.sanitize (c))) { return_trace (মিথ্যা); } বিরতি;
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
    return_trace (likely (!has_supplement () || supp_Enc_তথ্য ().sanitize (c)));
  }

  সুরক্ষিত:
  ধ্রুবক CFF1_Supp_Enc_তথ্য &supp_Enc_তথ্য () ধ্রুবক
  {
    নির্বাচন (table_format ())
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত StructAfter<CFF1_Supp_Enc_তথ্য> (u.format0.codes[u.format0.nCodes ()-1]);
    ক্ষেত্র 1: hb_barrier (); ফেরত StructAfter<CFF1_Supp_Enc_তথ্য> (u.format1.ranges[u.format1.nRanges ()-1]);
    পূর্বনির্ধারিত:ফেরত Null (CFF1_Supp_Enc_তথ্য);
    }
  }

  সর্বজনীন:
  HBUINT8	format;
  সংঘ {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_আকার_MIN (1);
};

/* Charset */
কাঠামো Charset0
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs, unsigned *অঙ্ক_charset_entries) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (অঙ্ক_charset_entries) *অঙ্ক_charset_entries = অঙ্ক_glyphs;
    return_trace (sids.sanitize (c, অঙ্ক_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (unlikely (glyph >= অঙ্ক_glyphs)) ফেরত 0;
    যদি (unlikely (glyph == 0))
      ফেরত 0;
    অন্যথা
      ফেরত sids[glyph - 1];
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (!mapping->resize_dirty  (অঙ্ক_glyphs)) ফেরত;
    জন্য (hb_codepoint_t gid = 1; gid < অঙ্ক_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (sid == 0)
      ফেরত 0;

    জন্য (unsigned পূর্ণসংখ্যা glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
    {
      যদি (sids[glyph-1] == sid)
	ফেরত glyph;
    }
    ফেরত 0;
  }

  স্থির size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs)
  {
    প্রতিজ্ঞা (অঙ্ক_glyphs > 0);
    ফেরত UnsizedArrayOf<HBUINT16>::get_আকার (অঙ্ক_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_আকার_ARRAY(0, sids);
};

template <typename TYPE>
কাঠামো Charset_Range {
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT16  first;
  TYPE      n_বাম;

  DEFINE_আকার_STATIC (HBUINT16::static_আকার + TYPE::static_আকার);
};

template <typename TYPE>
কাঠামো Charset1_2 {
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs, unsigned *অঙ্ক_charset_entries) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    অঙ্ক_glyphs--;
    unsigned i;
    জন্য (i = 0; অঙ্ক_glyphs > 0; i++)
    {
      যদি (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (অঙ্ক_glyphs >= ranges[i].n_বাম + 1))))
	return_trace (মিথ্যা);
      অঙ্ক_glyphs -= (ranges[i].n_বাম + 1);
    }
    যদি (অঙ্ক_charset_entries)
      *অঙ্ক_charset_entries = i;
    return_trace (সত্য);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned অঙ্ক_glyphs,
			  code_pair_t *cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (glyph >= অঙ্ক_glyphs)) ফেরত 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    যদি (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    অন্যথা
    {
      যদি (unlikely (glyph == 0)) ফেরত 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    জন্য (;; i++)
    {
      unsigned গুনতি = ranges[i].n_বাম;
      যদি (glyph <= গুনতি)
      {
        যদি (cache)
	  *cache = {i, start_glyph};
	ফেরত ranges[i].first + glyph;
      }
      গুনতি++;
      start_glyph += গুনতি;
      glyph -= গুনতি;
    }

    ফেরত 0;
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (!mapping->resize_dirty  (অঙ্ক_glyphs)) ফেরত;

    hb_codepoint_t gid = 1;
    যদি (gid >= অঙ্ক_glyphs)
      ফেরত;

    জন্য (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned গুনতি = ranges[i].n_বাম + 1;
      unsigned last = gid + গুনতি;
      জন্য (unsigned j = 0; j < গুনতি; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      যদি (gid >= অঙ্ক_glyphs)
        বিরতি;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (sid == 0) ফেরত 0;
    hb_codepoint_t  glyph = 1;
    জন্য (unsigned পূর্ণসংখ্যা i = 0;; i++)
    {
      যদি (glyph >= অঙ্ক_glyphs)
	ফেরত 0;
      যদি ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].n_বাম))
	ফেরত glyph + (sid - ranges[i].first);
      glyph += (ranges[i].n_বাম + 1);
    }

    ফেরত 0;
  }

  size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    পূর্ণসংখ্যা glyph = (পূর্ণসংখ্যা) অঙ্ক_glyphs;
    unsigned অঙ্ক_ranges = 0;

    প্রতিজ্ঞা (glyph > 0);
    glyph--;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].n_বাম + 1);
      অঙ্ক_ranges++;
    }

    ফেরত get_আকার_for_ranges (অঙ্ক_ranges);
  }

  স্থির size_t get_আকার_for_ranges (unsigned পূর্ণসংখ্যা অঙ্ক_ranges)
  {
    ফেরত UnsizedArrayOf<Charset_Range<TYPE> >::get_আকার (অঙ্ক_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_আকার_ARRAY (0, ranges);
};

প্রকারলেখ Charset1_2<HBUINT8>     Charset1;
প্রকারলেখ Charset1_2<HBUINT16>    Charset2;
প্রকারলেখ Charset_Range<HBUINT8>  Charset1_Range;
প্রকারলেখ Charset_Range<HBUINT16> Charset2_Range;

কাঠামো Charset
{
  /* serialize a fullset Charset */
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক Charset &src, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed ((ধ্রুবক অক্ষর *) &src, src.get_আকার (অঙ্ক_glyphs)));
  }

  /* serialize a subset Charset */
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
		  ধ্রুবক hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (এই);
    Charset *dest = c->extend_min (এই);
    যদি (unlikely (!dest)) return_trace (মিথ্যা);
    dest->format = format;
    নির্বাচন (format)
    {
    ক্ষেত্র 0:
    {
      Charset0 *fmt0 = c->allocate_আকার<Charset0> (Charset0::get_আকার (অঙ্ক_glyphs), মিথ্যা);
      যদি (unlikely (!fmt0)) return_trace (মিথ্যা);
      unsigned পূর্ণসংখ্যা glyph = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < sid_ranges.দৈর্ঘ্য; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	জন্য (পূর্ণসংখ্যা left = (পূর্ণসংখ্যা)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    বিরতি;

    ক্ষেত্র 1:
    {
      Charset1 *fmt1 = c->allocate_আকার<Charset1> (Charset1::get_আকার_for_ranges (sid_ranges.দৈর্ঘ্য), মিথ্যা);
      যদি (unlikely (!fmt1)) return_trace (মিথ্যা);
      hb_codepoint_t all_glyphs = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < sid_ranges.দৈর্ঘ্য; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].n_বাম = _.glyph;
      }
      যদি (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (মিথ্যা);
    }
    বিরতি;

    ক্ষেত্র 2:
    {
      Charset2 *fmt2 = c->allocate_আকার<Charset2> (Charset2::get_আকার_for_ranges (sid_ranges.দৈর্ঘ্য), মিথ্যা);
      যদি (unlikely (!fmt2)) return_trace (মিথ্যা);
      hb_codepoint_t all_glyphs = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < sid_ranges.দৈর্ঘ্য; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].n_বাম = _.glyph;
      }
      যদি (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (মিথ্যা);
    }
    বিরতি;

    }
    return_trace (সত্য);
  }

  size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত hb_unsigned_add_saturate (min_আকার, u.format0.get_আকার (অঙ্ক_glyphs));
    ক্ষেত্র 1: hb_barrier (); ফেরত hb_unsigned_add_saturate (min_আকার, u.format1.get_আকার (অঙ্ক_glyphs));
    ক্ষেত্র 2: hb_barrier (); ফেরত hb_unsigned_add_saturate (min_আকার, u.format2.get_আকার (অঙ্ক_glyphs));
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			  code_pair_t *cache = nullptr) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_sid (glyph, অঙ্ক_glyphs);
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_sid (glyph, অঙ্ক_glyphs, cache);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_sid (glyph, অঙ্ক_glyphs, cache);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  শূন্য collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_glyph (sid, অঙ্ক_glyphs);
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_glyph (sid, অঙ্ক_glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_glyph (sid, অঙ্ক_glyphs);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned *অঙ্ক_charset_entries) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_অঙ্ক_glyphs (), অঙ্ক_charset_entries));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  HBUINT8       format;
  সংঘ {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_আকার_MIN (1);
};

কাঠামো CFF1_String_সূচক : CFF1_সূচক
{
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক CFF1_String_সূচক &strings,
		  ধ্রুবক hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely ((strings.গুনতি == 0) || (sidmap.দৈর্ঘ্য == 0)))
    {
      যদি (unlikely (!c->extend_min (এই->গুনতি)))
	return_trace (মিথ্যা);
      গুনতি = 0;
      return_trace (সত্য);
    }

    যদি (unlikely (sidmap.in_error ())) return_trace (মিথ্যা);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    যদি (unlikely (bytesArray.in_error ())) return_trace (মিথ্যা);

    যৌক্তিক ফলাফল = CFF1_সূচক::serialize (c, bytesArray);
    return_trace (ফলাফল);
  }
};

কাঠামো cff1_শীর্ষ_dict_interp_env_t : অঙ্ক_interp_env_t
{
  cff1_শীর্ষ_dict_interp_env_t ()
    : অঙ্ক_interp_env_t(), পূর্ব_offset(0), last_offset(0) {}
  cff1_শীর্ষ_dict_interp_env_t (ধ্রুবক hb_ubytes_t &bytes)
    : অঙ্ক_interp_env_t(bytes), পূর্ব_offset(0), last_offset(0) {}

  unsigned পূর্ণসংখ্যা পূর্ব_offset;
  unsigned পূর্ণসংখ্যা last_offset;
};

কাঠামো name_dict_মানসমূহ_t
{
  গণনা name_dict_val_সূচক_t
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
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < Val_গুনতি; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned পূর্ণসংখ্যা& operator[] (unsigned পূর্ণসংখ্যা i)
  { প্রতিজ্ঞা (i < Val_গুনতি); ফেরত values[i]; }

  unsigned পূর্ণসংখ্যা operator[] (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { প্রতিজ্ঞা (i < Val_গুনতি); ফেরত values[i]; }

  স্থির গণনা name_dict_val_সূচক_t name_op_to_সূচক (op_code_t op)
  {
    নির্বাচন (op) {
      পূর্বনির্ধারিত: // can't happen - just make some compiler happy
      ক্ষেত্র OpCode_version:
	ফেরত version;
      ক্ষেত্র OpCode_Notice:
	ফেরত notice;
      ক্ষেত্র OpCode_Copyright:
	ফেরত copyright;
      ক্ষেত্র OpCode_FullName:
	ফেরত fullName;
      ক্ষেত্র OpCode_FamilyName:
	ফেরত familyName;
      ক্ষেত্র OpCode_Weight:
	ফেরত weight;
      ক্ষেত্র OpCode_PostScript:
	ফেরত postscript;
      ক্ষেত্র OpCode_FontName:
	ফেরত fontName;
      ক্ষেত্র OpCode_BaseFontName:
	ফেরত baseFontName;
    }
  }

  unsigned পূর্ণসংখ্যা  values[Val_গুনতি];
};

কাঠামো cff1_শীর্ষ_dict_val_t : op_str_t
{
  unsigned পূর্ণসংখ্যা  last_arg_offset;
};

কাঠামো cff1_শীর্ষ_dict_মানসমূহ_t : শীর্ষ_dict_মানসমূহ_t<cff1_শীর্ষ_dict_val_t>
{
  শূন্য init ()
  {
    শীর্ষ_dict_মানসমূহ_t<cff1_শীর্ষ_dict_val_t>::init ();

    nameSIDs.init ();
    ros_supplement = 0;
    cid_গুনতি = 8720;
    EncodingOffset = 0;
    CharsetOffset = 0;
    FDSelectOffset = 0;
    privateDictInfo.init ();
  }
  শূন্য fini () { শীর্ষ_dict_মানসমূহ_t<cff1_শীর্ষ_dict_val_t>::fini (); }

  যৌক্তিক is_CID () ধ্রুবক
  { ফেরত nameSIDs[name_dict_মানসমূহ_t::registry] != CFF_UNDEF_SID; }

  name_dict_মানসমূহ_t  nameSIDs;
  unsigned পূর্ণসংখ্যা    ros_supplement_offset;
  unsigned পূর্ণসংখ্যা    ros_supplement;
  unsigned পূর্ণসংখ্যা    cid_গুনতি;

  পূর্ণসংখ্যা             EncodingOffset;
  পূর্ণসংখ্যা             CharsetOffset;
  পূর্ণসংখ্যা             FDSelectOffset;
  table_info_t    privateDictInfo;
};

কাঠামো cff1_শীর্ষ_dict_opset_t : শীর্ষ_dict_opset_t<cff1_শীর্ষ_dict_val_t>
{
  স্থির শূন্য process_op (op_code_t op, cff1_শীর্ষ_dict_interp_env_t& env, cff1_শীর্ষ_dict_মানসমূহ_t& dictval)
  {
    cff1_শীর্ষ_dict_val_t  val {};

    নির্বাচন (op) {
      ক্ষেত্র OpCode_version:
      ক্ষেত্র OpCode_Notice:
      ক্ষেত্র OpCode_Copyright:
      ক্ষেত্র OpCode_FullName:
      ক্ষেত্র OpCode_FontName:
      ক্ষেত্র OpCode_FamilyName:
      ক্ষেত্র OpCode_Weight:
      ক্ষেত্র OpCode_PostScript:
      ক্ষেত্র OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_মানসমূহ_t::name_op_to_সূচক (op)] = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_isFixedPitch:
      ক্ষেত্র OpCode_ItalicAngle:
      ক্ষেত্র OpCode_UnderlinePosition:
      ক্ষেত্র OpCode_UnderlineThickness:
      ক্ষেত্র OpCode_PaintType:
      ক্ষেত্র OpCode_CharstringType:
      ক্ষেত্র OpCode_UniqueID:
      ক্ষেত্র OpCode_StrokeWidth:
      ক্ষেত্র OpCode_SyntheticBase:
      ক্ষেত্র OpCode_CIDFontVersion:
      ক্ষেত্র OpCode_CIDFontRevision:
      ক্ষেত্র OpCode_CIDFontType:
      ক্ষেত্র OpCode_UIDBase:
      ক্ষেত্র OpCode_FontBBox:
      ক্ষেত্র OpCode_XUID:
      ক্ষেত্র OpCode_BaseFontBlend:
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_CIDCount:
	dictval.cid_গুনতি = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_ROS:
	dictval.ros_supplement = env.arg_স্তূপ.বের_করো_uint ();
	dictval.nameSIDs[name_dict_মানসমূহ_t::ordering] = env.arg_স্তূপ.বের_করো_uint ();
	dictval.nameSIDs[name_dict_মানসমূহ_t::registry] = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	যদি (unlikely (env.in_error ())) ফেরত;
	প্রতিজ্ঞা (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	বিরতি;

      ক্ষেত্র OpCode_Encoding:
	dictval.EncodingOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	যদি (unlikely (dictval.EncodingOffset == 0)) ফেরত;
	বিরতি;

      ক্ষেত্র OpCode_charset:
	dictval.CharsetOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	যদি (unlikely (dictval.CharsetOffset == 0)) ফেরত;
	বিরতি;

      ক্ষেত্র OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বের_করো_int ();
	dictval.privateDictInfo.আকার_2 = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	env.last_offset = env.str_ref.get_offset ();
	শীর্ষ_dict_opset_t<cff1_শীর্ষ_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref, val);
  }
};

কাঠামো cff1_font_dict_মানসমূহ_t : dict_মানসমূহ_t<op_str_t>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  শূন্য fini () { dict_মানসমূহ_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned পূর্ণসংখ্যা    fontName;
};

কাঠামো cff1_font_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_font_dict_মানসমূহ_t& dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_FontName:
	dictval.fontName = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_FontMatrix:
      ক্ষেত্র OpCode_PaintType:
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বের_করো_uint ();
	dictval.privateDictInfo.আকার_2 = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
কাঠামো cff1_private_dict_মানসমূহ_base_t : dict_মানসমূহ_t<VAL>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  শূন্য fini () { dict_মানসমূহ_t<VAL>::fini (); }

  পূর্ণসংখ্যা                 subrsOffset;
  ধ্রুবক CFF1Subrs    *localSubrs;
};

প্রকারলেখ cff1_private_dict_মানসমূহ_base_t<op_str_t> cff1_private_dict_মানসমূহ_subset_t;
প্রকারলেখ cff1_private_dict_মানসমূহ_base_t<অঙ্ক_dict_val_t> cff1_private_dict_মানসমূহ_t;

কাঠামো cff1_private_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_private_dict_মানসমূহ_t& dictval)
  {
    অঙ্ক_dict_val_t val;
    val.init ();

    নির্বাচন (op) {
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_ForceBold:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র OpCode_ExpansionFactor:
      ক্ষেত্র OpCode_initialRandomSeed:
      ক্ষেত্র OpCode_defaultWidthX:
      ক্ষেত্র OpCode_nominalWidthX:
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref, val);
  }
};

কাঠামো cff1_private_dict_opset_subset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff1_private_dict_মানসমূহ_subset_t& dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_ForceBold:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র OpCode_ExpansionFactor:
      ক্ষেত্র OpCode_initialRandomSeed:
      ক্ষেত্র OpCode_defaultWidthX:
      ক্ষেত্র OpCode_nominalWidthX:
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref);
  }
};

প্রকারলেখ dict_interpreter_t<cff1_শীর্ষ_dict_opset_t, cff1_শীর্ষ_dict_মানসমূহ_t, cff1_শীর্ষ_dict_interp_env_t> cff1_শীর্ষ_dict_interpreter_t;
প্রকারলেখ dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_মানসমূহ_t> cff1_font_dict_interpreter_t;

প্রকারলেখ CFF1_সূচক CFF1_Name_সূচক;
প্রকারলেখ CFF1_সূচক CFF1_শীর্ষ_Dict_সূচক;

কাঠামো cff1_font_dict_মানসমূহ_mod_t
{
  cff1_font_dict_মানসমূহ_mod_t() { init (); }

  শূন্য init () { init ( &Null (cff1_font_dict_মানসমূহ_t), CFF_UNDEF_SID ); }

  শূন্য init (ধ্রুবক cff1_font_dict_মানসমূহ_t *base_,
	     unsigned পূর্ণসংখ্যা fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_গুনতি () ধ্রুবক { ফেরত base->get_গুনতি (); }

  ধ্রুবক op_str_t &operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত (*base)[i]; }

  ধ্রুবক cff1_font_dict_মানসমূহ_t    *base;
  table_info_t		   privateDictInfo;
  unsigned পূর্ণসংখ্যা		fontName;
};

কাঠামো CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  যৌক্তিক serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ফেরত FDArray<HBUINT16>::serialize<cff1_font_dict_মানসমূহ_mod_t, cff1_font_dict_মানসমূহ_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

নামাকাশ OT {

using নামাকাশ CFF;

কাঠামো cff1
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  কাঠামো accelerator_templ_t
  {
    স্থির constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      যদি (!face) ফেরত;

      শীর্ষ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      এই->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (এই->blob);
      sc.start_processing ();

      ধ্রুবক OT::cff1 *cff = এই->blob->template as<OT::cff1> ();

      যদি (cff == &Null (OT::cff1))
        যাও fail;

      name_সূচক = &cff->name_সূচক (cff);
      যদি ((name_সূচক == &Null (CFF1_Name_সূচক)) || !name_সূচক->sanitize (&sc))
        যাও fail;
      hb_barrier ();

      শীর্ষ_Dict_সূচক = &StructAtOffsetOrNull<CFF1_শীর্ষ_Dict_সূচক> (name_সূচক, name_সূচক->get_আকার (), sc);
      যদি (শীর্ষ_Dict_সূচক == &Null (CFF1_শীর্ষ_Dict_সূচক) || (শীর্ষ_Dict_সূচক->গুনতি == 0))
        যাও fail;
      hb_barrier ();

      { /* parse top dict */
	ধ্রুবক hb_ubytes_t শীর্ষ_Dict_Str = (*শীর্ষ_Dict_সূচক)[0];
	যদি (unlikely (!শীর্ষ_Dict_Str.sanitize (&sc)))   যাও fail;
	hb_barrier ();
	cff1_শীর্ষ_dict_interp_env_t env (শীর্ষ_Dict_Str);
	cff1_শীর্ষ_dict_interpreter_t শীর্ষ_interp (env);
	যদি (unlikely (!শীর্ষ_interp.interpret (শীর্ষ_Dict_2)))   যাও fail;
      }

      যদি (is_predef_charset ())
	charset = &Null (Charset);
      অন্যথা
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, শীর্ষ_Dict_2.CharsetOffset, sc, &অঙ্ক_charset_entries);
	যদি (unlikely (charset == &Null (Charset)))   যাও fail;
      }

      fd_গুনতি = 1;
      যদি (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, শীর্ষ_Dict_2.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, শীর্ষ_Dict_2.FDSelectOffset, sc, fdArray->গুনতি);
	যদি (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  যাও fail;

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
	যদি (unlikely (charset == &Null (Charset)))   যাও fail;
      }
      অন্যথা
      {
	যদি (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, শীর্ষ_Dict_2.EncodingOffset, sc);
	  যদি (unlikely (encoding == &Null (Encoding)))   যাও fail;
	}
      }

      string_সূচক = &StructAtOffsetOrNull<CFF1_String_সূচক> (শীর্ষ_Dict_সূচক, শীর্ষ_Dict_সূচক->get_আকার (), sc);
      যদি (string_সূচক == &Null (CFF1_String_সূচক))
        যাও fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (string_সূচক, string_সূচক->get_আকার (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, শীর্ষ_Dict_2.charStringsOffset, sc);
      যদি (charStrings == &Null (CFF1CharStrings))
        যাও fail;

      অঙ্ক_glyphs = charStrings->গুনতি;
      যদি (অঙ্ক_glyphs != sc.get_অঙ্ক_glyphs ())
        যাও fail;

      যদি (unlikely (!privateDicts.resize (fd_গুনতি)))
        যাও fail;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fd_গুনতি; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      যদি (is_CID ())
      {
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fd_গুনতি; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  যদি (unlikely (!fontDictStr.sanitize (&sc)))   যাও fail;
	  hb_barrier ();
	  cff1_font_dict_মানসমূহ_t *font;
	  cff1_শীর্ষ_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  যদি (unlikely (fontDicts.in_error ()))   যাও fail;

	  font->init ();
	  যদি (unlikely (!font_interp.interpret (*font)))   যাও fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  ধ্রুবক hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.আকার_2).as_ubytes (font->privateDictInfo.আকার_2);
	  যদি (unlikely (font->privateDictInfo.আকার_2 &&
			privDictStr == (ধ্রুবক unsigned অক্ষর *) &Null (UnsizedByteStr))) যাও fail;
	  অঙ্ক_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  যদি (unlikely (!priv_interp.interpret (*priv)))   যাও fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      অন্যথা  /* non-CID */
      {
	cff1_শীর্ষ_dict_মানসমূহ_t *font = &শীর্ষ_Dict_2;
	PRIVDICTVAL *priv = &privateDicts[0];

	ধ্রুবক hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.আকার_2).as_ubytes (font->privateDictInfo.আকার_2);
	যদি (font->privateDictInfo.আকার_2 &&
	    unlikely (privDictStr == (ধ্রুবক unsigned অক্ষর *) &Null (UnsizedByteStr))) যাও fail;
	অঙ্ক_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	যদি (unlikely (!priv_interp.interpret (*priv)))   যাও fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      ফেরত;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    শূন্য _fini ()
    {
      sc.end_processing ();
      শীর্ষ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত blob; }

    যৌক্তিক is_valid () ধ্রুবক { ফেরত blob; }
    যৌক্তিক   is_CID () ধ্রুবক { ফেরত শীর্ষ_Dict_2.is_CID (); }

    যৌক্তিক is_predef_charset () ধ্রুবক { ফেরত শীর্ষ_Dict_2.CharsetOffset <= ExpertSubsetCharset; }

    unsigned পূর্ণসংখ্যা std_code_to_glyph (hb_codepoint_t code) ধ্রুবক
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      যদি (unlikely (sid == CFF_UNDEF_SID))
	ফেরত 0;

      যদি (charset != &Null (Charset))
	ফেরত charset->get_glyph (sid, অঙ্ক_glyphs);
      অন্যথা যদি ((শীর্ষ_Dict_2.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) ফেরত sid;
      ফেরত 0;
    }

    যৌক্তিক is_predef_encoding () ধ্রুবক { ফেরত শীর্ষ_Dict_2.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) ধ্রুবক
    {
      যদি (encoding != &Null (Encoding))
	ফেরত encoding->get_code (glyph);
      অন্যথা
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	যদি (sid == 0) ফেরত 0;
	hb_codepoint_t code = 0;
	নির্বাচন (শীর্ষ_Dict_2.EncodingOffset)
	{
	ক্ষেত্র StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  বিরতি;
	ক্ষেত্র ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  বিরতি;
	পূর্বনির্ধারিত:
	  বিরতি;
	}
	ফেরত code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () ধ্রুবক
    {
      যদি (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (আকার (glyph_to_sid_map_t));
	যদি (unlikely (!mapping)) ফেরত nullptr;
	mapping = নতুন (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, অঙ্ক_glyphs);
	ফেরত mapping;
      }
      অন্যথা
	ফেরত nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) ধ্রুবক
    {
      যদি (charset != &Null (Charset))
	ফেরত charset->get_sid (glyph, অঙ্ক_glyphs, cache);
      অন্যথা
      {
	hb_codepoint_t sid = 0;
	নির্বাচন (শীর্ষ_Dict_2.CharsetOffset)
	{
	  ক্ষেত্র ISOAdobeCharset:
	    যদি (glyph <= 228 /*zcaron*/) sid = glyph;
	    বিরতি;
	  ক্ষেত্র ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    বিরতি;
	  ক্ষেত্র ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    বিরতি;
	  পূর্বনির্ধারিত:
	    বিরতি;
	}
	ফেরত sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) ধ্রুবক
    {
      যদি (charset != &Null (Charset))
	ফেরত charset->get_glyph (sid, অঙ্ক_glyphs);
      অন্যথা
      {
	hb_codepoint_t glyph = 0;
	নির্বাচন (শীর্ষ_Dict_2.CharsetOffset)
	{
	  ক্ষেত্র ISOAdobeCharset:
	    যদি (sid <= 228 /*zcaron*/) glyph = sid;
	    বিরতি;
	  ক্ষেত্র ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    বিরতি;
	  ক্ষেত্র ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    বিরতি;
	  পূর্বনির্ধারিত:
	    বিরতি;
	}
	ফেরত glyph;
      }
    }

    সুরক্ষিত:
    hb_sanitize_context_t   sc;

    সর্বজনীন:
    hb_blob_t               *blob = nullptr;
    ধ্রুবক Encoding	    *encoding = nullptr;
    ধ্রুবক Charset	    *charset = nullptr;
    ধ্রুবক CFF1_Name_সূচক     *name_সূচক = nullptr;
    ধ্রুবক CFF1_শীর্ষ_Dict_সূচক  *শীর্ষ_Dict_সূচক = nullptr;
    ধ্রুবক CFF1_String_সূচক   *string_সূচক = nullptr;
    ধ্রুবক CFF1Subrs	    *globalSubrs = nullptr;
    ধ্রুবক CFF1CharStrings   *charStrings = nullptr;
    ধ্রুবক CFF1FDArray       *fdArray = nullptr;
    ধ্রুবক CFF1FDSelect      *fdSelect = nullptr;
    unsigned পূর্ণসংখ্যা	     fd_গুনতি = 0;

    cff1_শীর্ষ_dict_মানসমূহ_t   শীর্ষ_Dict_2;
    hb_vector_t<cff1_font_dict_মানসমূহ_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned পূর্ণসংখ্যা	     অঙ্ক_glyphs = 0;
    unsigned পূর্ণসংখ্যা	     অঙ্ক_charset_entries = 0;
  };

  কাঠামো accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_মানসমূহ_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      যদি (!is_valid ()) ফেরত;
      যদি (is_CID ()) ফেরত;
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

    যৌক্তিক get_glyph_name (hb_codepoint_t glyph,
			 অক্ষর *buf, unsigned পূর্ণসংখ্যা buf_len) ধ্রুবক
    {
      যদি (unlikely (glyph >= অঙ্ক_glyphs)) ফেরত মিথ্যা;
      যদি (unlikely (!is_valid ())) ফেরত মিথ্যা;
      যদি (is_CID()) ফেরত মিথ্যা;
      যদি (unlikely (!buf_len)) ফেরত সত্য;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      ধ্রুবক অক্ষর *str;
      size_t str_len;
      যদি (sid < cff1_std_strings_দৈর্ঘ্য)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.দৈর্ঘ্য;
      }
      অন্যথা
      {
	hb_ubytes_t ubyte_str = (*string_সূচক)[sid - cff1_std_strings_দৈর্ঘ্য];
	str = (ধ্রুবক অক্ষর *)ubyte_str.arrayZ;
	str_len = ubyte_str.দৈর্ঘ্য;
      }
      যদি (!str_len) ফেরত মিথ্যা;
      unsigned পূর্ণসংখ্যা len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (ধ্রুবক অক্ষর*)str, len);
      buf[len] = '\0';
      ফেরত সত্য;
    }

    যৌক্তিক get_glyph_from_name (ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা len,
			      hb_codepoint_t *glyph) ধ্রুবক
    {
      যদি (unlikely (!is_valid ())) ফেরত মিথ্যা;
      যদি (is_CID()) ফেরত মিথ্যা;
      যদি (len < 0) len = strlen (name);
      যদি (unlikely (!len)) ফেরত মিথ্যা;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      যদি (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, আকার (hb_sorted_vector_t<gname_t>));
	যদি (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  জন্য (hb_codepoint_t gid = 0; gid < অঙ্ক_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    যদি (sid < cff1_std_strings_দৈর্ঘ্য)
	      gname.name = cff1_std_strings (sid);
	    অন্যথা
	    {
	      hb_ubytes_t	ustr = (*string_সূচক)[sid - cff1_std_strings_দৈর্ঘ্য];
	      gname.name = hb_bytes_t ((ধ্রুবক অক্ষর*) ustr.arrayZ, ustr.দৈর্ঘ্য);
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
	  যাও retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      ধ্রুবক gname_t *gname = names ? names->bsearch (key) : nullptr;
      যদি (!gname) ফেরত মিথ্যা;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      যদি (!gid && gname->sid) ফেরত মিথ্যা;
      *glyph = gid;
      ফেরত সত্য;
    }

    HB_INTERNAL যৌক্তিক get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) ধ্রুবক;
    HB_INTERNAL যৌক্তিক get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ধ্রুবক;

    ব্যক্তিগত:
    কাঠামো gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
      {
	ধ্রুবক gname_t *a = (ধ্রুবক gname_t *)a_;
	ধ্রুবক gname_t *b = (ধ্রুবক gname_t *)b_;
	unsigned minlen = hb_min (a->name.দৈর্ঘ্য, b->name.দৈর্ঘ্য);
	পূর্ণসংখ্যা ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	যদি (ret) ফেরত ret;
	ফেরত a->name.দৈর্ঘ্য - b->name.দৈর্ঘ্য;
      }

      পূর্ণসংখ্যা cmp (ধ্রুবক gname_t &a) ধ্রুবক { ফেরত cmp (&a, এই); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    প্রকারলেখ accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_মানসমূহ_t> SUPER;
  };

  কাঠামো accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_মানসমূহ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      যদি (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক;
    HB_INTERNAL যৌক্তিক serialize (hb_serialize_context_t *c,
				কাঠামো cff1_subset_plan &plan) ধ্রুবক;
    HB_INTERNAL যৌক্তিক get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) ধ্রুবক;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    প্রকারলেখ accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_মানসমূহ_subset_t> SUPER;
  };

  সুরক্ষিত:
  HB_INTERNAL স্থির hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL স্থির hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  সর্বজনীন:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1_Name_সূচক, HBUINT8> name_সূচক; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       off_আকার;	  /* offset size (unused?) */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

কাঠামো cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  শূন্য depend (hb_depend_তথ্য_builder_t *builder) ধ্রুবক
  {
    যদি (!is_valid ()) ফেরত;
    জন্য (hb_codepoint_t gid = 0; gid < অঙ্ক_glyphs; gid++)
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
