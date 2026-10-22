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

#include "hb-ot-cff-common.hh"
#include "hb-depend-data.hh"
#include "hb-subset-cff-common.hh"
#include "hb-draw.hh"
#include "hb-paint.hh"

#define HB_STRING_ARRAY_NAME cff1_std_strings
#define HB_STRING_ARRAY_LIST "hb-ot-cff1-std-str.hh"
#include "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

namespace CFF {

/*
 * CFF -- Compact Font Format (CFF)
 * https://www.adobe.com/content/dam/acom/en/devnet/font/pdfs/5176.CFF.pdf
 */
#define HB_OT_TAG_CFF1 HB_TAG('C','F','F',' ')

#define CFF_UNDEF_SID   CFF_UNDEF_CODE

ga_nanaa EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
ga_nanaa CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

typedef CFF1Index          CFF1CharStrings;
typedef Subrs<HBUINT16>    CFF1Subrs;

sa_nracanaa CFF1FDSelect : FDSelect {};

/* Encoding */
sa_nracanaa Encoding0 {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) const
  {
    assert (glyph > 0);
    glyph--;
    གལ་སྲིད (glyph < nCodes ())
    {
      ལོག (hb_codepoint_t)codes[glyph];
    }
    དེ་མིན
      ལོག CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { ལོག codes.len; }
  HBUINT8 nCodes () const { ལོག codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

sa_nracanaa Encoding1_Range {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT8   first;
  HBUINT8   nLeft;

  DEFINE_SIZE_STATIC (2);
};

sa_nracanaa Encoding1 {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (ranges.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) const
  {
    /* TODO: Add cache like get_sid. */
    assert (glyph > 0);
    glyph--;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < nRanges (); i++)
    {
      གལ་སྲིད (glyph <= ranges[i].nLeft)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	ལོག (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].nLeft + 1);
    }
    ལོག CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { ལོག ranges.len; }
  HBUINT8 nRanges () const { ལོག ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

sa_nracanaa SuppEncoding {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

sa_nracanaa CFF1SuppEncData {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (supps.sanitize (c));
  }

  void get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) const
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < nSups (); i++)
      གལ་སྲིད (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { ལོག supps.len; }
  HBUINT8 nSups () const { ལོག supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

sa_nracanaa Encoding
{
  /* serialize a fullset Encoding */
  bool serialize (hb_serialize_context_t *c, const Encoding &src)
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (src));
  }

  /* serialize a subset Encoding */
  bool serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ཧྲིལ enc_count,
		  const hb_vector_t<code_pair_t>& code_ranges,
		  const hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (this);
    Encoding *dest = c->extend_min (this);
    གལ་སྲིད (unlikely (!dest)) return_trace (false);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    switch (format) {
    pak_sa 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_count);
      གལ་སྲིད (unlikely (!fmt0)) return_trace (false);
      fmt0->nCodes () = enc_count;
      unsigned ཧྲིལ glyph = 0;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	དོན་ལུ (ཧྲིལ left = (ཧྲིལ)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	གལ་སྲིད (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (false);
      }
    }
    bha_nga;

    pak_sa 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      གལ་སྲིད (unlikely (!fmt1)) return_trace (false);
      fmt1->nRanges () = code_ranges.length;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < code_ranges.length; i++)
      {
	གལ་སྲིད (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (false);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].nLeft = code_ranges[i].glyph;
      }
    }
    bha_nga;

    }

    གལ་སྲིད (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      གལ་སྲིད (unlikely (!suppData)) return_trace (false);
      suppData->nSups () = supp_codes.length;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < supp_codes.length; i++)
      {
	suppData->supps[i].code = supp_codes[i].code;
	suppData->supps[i].glyph = supp_codes[i].glyph; /* actually SID */
      }
    }

    return_trace (true);
  }

  size_t get_size () const
  {
    size_t size = min_size;
    switch (table_format ())
    {
    pak_sa 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); bha_nga;
    pak_sa 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); bha_nga;
    }
    གལ་སྲིད (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    ལོག size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) const
  {
    switch (table_format ())
    {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_code (glyph);
    pak_sa 1: hb_barrier (); ལོག u.format1.get_code (glyph);
    default:ལོག 0;
    }
  }

  uint8_t table_format () const { ལོག format & 0x7F; }
  bool  has_supplement () const { ལོག format & 0x80; }

  void get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) const
  {
    codes.clear ();
    གལ་སྲིད (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (table_format ())
    {
    pak_sa 0: hb_barrier (); གལ་སྲིད (unlikely (!u.format0.sanitize (c))) { return_trace (false); } bha_nga;
    pak_sa 1: hb_barrier (); གལ་སྲིད (unlikely (!u.format1.sanitize (c))) { return_trace (false); } bha_nga;
    default:return_trace (false);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  rak_sita:
  const CFF1SuppEncData &suppEncData () const
  {
    switch (table_format ())
    {
    pak_sa 0: hb_barrier (); ལོག StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    pak_sa 1: hb_barrier (); ལོག StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    default:ལོག Null (CFF1SuppEncData);
    }
  }

  public:
  HBUINT8	format;
  sa_ngha {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
sa_nracanaa Charset0
{
  bool sanitize (hb_sanitize_context_t *c, unsigned ཧྲིལ num_glyphs, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (num_charset_entries) *num_charset_entries = num_glyphs;
    return_trace (sids.sanitize (c, num_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs) const
  {
    གལ་སྲིད (unlikely (glyph >= num_glyphs)) ལོག 0;
    གལ་སྲིད (unlikely (glyph == 0))
      ལོག 0;
    དེ་མིན
      ལོག sids[glyph - 1];
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ཧྲིལ num_glyphs) const
  {
    གལ་སྲིད (!mapping->resize_dirty  (num_glyphs)) ལོག;
    དོན་ལུ (hb_codepoint_t gid = 1; gid < num_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ཧྲིལ num_glyphs) const
  {
    གལ་སྲིད (sid == 0)
      ལོག 0;

    དོན་ལུ (unsigned ཧྲིལ glyph = 1; glyph < num_glyphs; glyph++)
    {
      གལ་སྲིད (sids[glyph-1] == sid)
	ལོག glyph;
    }
    ལོག 0;
  }

  static size_t get_size (unsigned ཧྲིལ num_glyphs)
  {
    assert (num_glyphs > 0);
    ལོག UnsizedArrayOf<HBUINT16>::get_size (num_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
sa_nracanaa Charset_Range {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT16  first;
  TYPE      nLeft;

  DEFINE_SIZE_STATIC (HBUINT16::static_size + TYPE::static_size);
};

template <typename TYPE>
sa_nracanaa Charset1_2 {
  bool sanitize (hb_sanitize_context_t *c, unsigned ཧྲིལ num_glyphs, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    num_glyphs--;
    unsigned i;
    དོན་ལུ (i = 0; num_glyphs > 0; i++)
    {
      གལ་སྲིད (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (num_glyphs >= ranges[i].nLeft + 1))))
	return_trace (false);
      num_glyphs -= (ranges[i].nLeft + 1);
    }
    གལ་སྲིད (num_charset_entries)
      *num_charset_entries = i;
    return_trace (true);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs,
			  code_pair_t *cache = nullptr) const
  {
    གལ་སྲིད (unlikely (glyph >= num_glyphs)) ལོག 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    གལ་སྲིད (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    དེ་མིན
    {
      གལ་སྲིད (unlikely (glyph == 0)) ལོག 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    དོན་ལུ (;; i++)
    {
      unsigned count = ranges[i].nLeft;
      གལ་སྲིད (glyph <= count)
      {
        གལ་སྲིད (cache)
	  *cache = {i, start_glyph};
	ལོག ranges[i].first + glyph;
      }
      count++;
      start_glyph += count;
      glyph -= count;
    }

    ལོག 0;
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ཧྲིལ num_glyphs) const
  {
    གལ་སྲིད (!mapping->resize_dirty  (num_glyphs)) ལོག;

    hb_codepoint_t gid = 1;
    གལ་སྲིད (gid >= num_glyphs)
      ལོག;

    དོན་ལུ (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned count = ranges[i].nLeft + 1;
      unsigned last = gid + count;
      དོན་ལུ (unsigned j = 0; j < count; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      གལ་སྲིད (gid >= num_glyphs)
        bha_nga;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ཧྲིལ num_glyphs) const
  {
    གལ་སྲིད (sid == 0) ལོག 0;
    hb_codepoint_t  glyph = 1;
    དོན་ལུ (unsigned ཧྲིལ i = 0;; i++)
    {
      གལ་སྲིད (glyph >= num_glyphs)
	ལོག 0;
      གལ་སྲིད ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].nLeft))
	ལོག glyph + (sid - ranges[i].first);
      glyph += (ranges[i].nLeft + 1);
    }

    ལོག 0;
  }

  size_t get_size (unsigned ཧྲིལ num_glyphs) const
  {
    ཧྲིལ glyph = (ཧྲིལ) num_glyphs;
    unsigned num_ranges = 0;

    assert (glyph > 0);
    glyph--;
    དོན་ལུ (unsigned ཧྲིལ i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].nLeft + 1);
      num_ranges++;
    }

    ལོག get_size_for_ranges (num_ranges);
  }

  static size_t get_size_for_ranges (unsigned ཧྲིལ num_ranges)
  {
    ལོག UnsizedArrayOf<Charset_Range<TYPE> >::get_size (num_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

typedef Charset1_2<HBUINT8>     Charset1;
typedef Charset1_2<HBUINT16>    Charset2;
typedef Charset_Range<HBUINT8>  Charset1_Range;
typedef Charset_Range<HBUINT16> Charset2_Range;

sa_nracanaa Charset
{
  /* serialize a fullset Charset */
  bool serialize (hb_serialize_context_t *c, const Charset &src, unsigned ཧྲིལ num_glyphs)
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed ((const var_na *) &src, src.get_size (num_glyphs)));
  }

  /* serialize a subset Charset */
  bool serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned ཧྲིལ num_glyphs,
		  const hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (this);
    Charset *dest = c->extend_min (this);
    གལ་སྲིད (unlikely (!dest)) return_trace (false);
    dest->format = format;
    switch (format)
    {
    pak_sa 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (num_glyphs), false);
      གལ་སྲིད (unlikely (!fmt0)) return_trace (false);
      unsigned ཧྲིལ glyph = 0;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	དོན་ལུ (ཧྲིལ left = (ཧྲིལ)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    bha_nga;

    pak_sa 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), false);
      གལ་སྲིད (unlikely (!fmt1)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].nLeft = _.glyph;
      }
      གལ་སྲིད (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (false);
    }
    bha_nga;

    pak_sa 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), false);
      གལ་སྲིད (unlikely (!fmt2)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].nLeft = _.glyph;
      }
      གལ་སྲིད (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (false);
    }
    bha_nga;

    }
    return_trace (true);
  }

  size_t get_size (unsigned ཧྲིལ num_glyphs) const
  {
    switch (format)
    {
    pak_sa 0: hb_barrier (); ལོག hb_unsigned_add_saturate (min_size, u.format0.get_size (num_glyphs));
    pak_sa 1: hb_barrier (); ལོག hb_unsigned_add_saturate (min_size, u.format1.get_size (num_glyphs));
    pak_sa 2: hb_barrier (); ལོག hb_unsigned_add_saturate (min_size, u.format2.get_size (num_glyphs));
    default:ལོག 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned ཧྲིལ num_glyphs,
			  code_pair_t *cache = nullptr) const
  {
    switch (format)
    {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_sid (glyph, num_glyphs);
    pak_sa 1: hb_barrier (); ལོག u.format1.get_sid (glyph, num_glyphs, cache);
    pak_sa 2: hb_barrier (); ལོག u.format2.get_sid (glyph, num_glyphs, cache);
    default:ལོག 0;
    }
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned ཧྲིལ num_glyphs) const
  {
    switch (format)
    {
    pak_sa 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, num_glyphs); ལོག;
    pak_sa 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, num_glyphs); ལོག;
    pak_sa 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, num_glyphs); ལོག;
    default:ལོག;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned ཧྲིལ num_glyphs) const
  {
    switch (format)
    {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_glyph (sid, num_glyphs);
    pak_sa 1: hb_barrier (); ལོག u.format1.get_glyph (sid, num_glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.get_glyph (sid, num_glyphs);
    default:ལོག 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (format)
    {
    pak_sa 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    pak_sa 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    default:return_trace (false);
    }
  }

  HBUINT8       format;
  sa_ngha {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

sa_nracanaa CFF1StringIndex : CFF1Index
{
  bool serialize (hb_serialize_context_t *c, const CFF1StringIndex &strings,
		  const hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely ((strings.count == 0) || (sidmap.length == 0)))
    {
      གལ་སྲིད (unlikely (!c->extend_min (this->count)))
	return_trace (false);
      count = 0;
      return_trace (true);
    }

    གལ་སྲིད (unlikely (sidmap.in_error ())) return_trace (false);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    གལ་སྲིད (unlikely (bytesArray.in_error ())) return_trace (false);

    bool result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

sa_nracanaa cff1_top_dict_interp_env_t : num_interp_env_t
{
  cff1_top_dict_interp_env_t ()
    : num_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_top_dict_interp_env_t (const hb_ubytes_t &bytes)
    : num_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned ཧྲིལ prev_offset;
  unsigned ཧྲིལ last_offset;
};

sa_nracanaa name_dict_values_t
{
  ga_nanaa name_dict_val_index_t
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

  void init ()
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ValCount; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned ཧྲིལ& operator[] (unsigned ཧྲིལ i)
  { assert (i < ValCount); ལོག values[i]; }

  unsigned ཧྲིལ operator[] (unsigned ཧྲིལ i) const
  { assert (i < ValCount); ལོག values[i]; }

  static ga_nanaa name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    switch (op) {
      default: // can't happen - just make some compiler happy
      pak_sa OpCode_version:
	ལོག version;
      pak_sa OpCode_Notice:
	ལོག notice;
      pak_sa OpCode_Copyright:
	ལོག copyright;
      pak_sa OpCode_FullName:
	ལོག fullName;
      pak_sa OpCode_FamilyName:
	ལོག familyName;
      pak_sa OpCode_Weight:
	ལོག weight;
      pak_sa OpCode_PostScript:
	ལོག postscript;
      pak_sa OpCode_FontName:
	ལོག fontName;
      pak_sa OpCode_BaseFontName:
	ལོག baseFontName;
    }
  }

  unsigned ཧྲིལ  values[ValCount];
};

sa_nracanaa cff1_top_dict_val_t : op_str_t
{
  unsigned ཧྲིལ  last_arg_offset;
};

sa_nracanaa cff1_top_dict_values_t : top_dict_values_t<cff1_top_dict_val_t>
{
  void init ()
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
  void fini () { top_dict_values_t<cff1_top_dict_val_t>::fini (); }

  bool is_CID () const
  { ལོག nameSIDs[name_dict_values_t::registry] != CFF_UNDEF_SID; }

  name_dict_values_t  nameSIDs;
  unsigned ཧྲིལ    ros_supplement_offset;
  unsigned ཧྲིལ    ros_supplement;
  unsigned ཧྲིལ    cidCount;

  ཧྲིལ             EncodingOffset;
  ཧྲིལ             CharsetOffset;
  ཧྲིལ             FDSelectOffset;
  table_info_t    privateDictInfo;
};

sa_nracanaa cff1_top_dict_opset_t : top_dict_opset_t<cff1_top_dict_val_t>
{
  static void process_op (op_code_t op, cff1_top_dict_interp_env_t& env, cff1_top_dict_values_t& dictval)
  {
    cff1_top_dict_val_t  val {};

    switch (op) {
      pak_sa OpCode_version:
      pak_sa OpCode_Notice:
      pak_sa OpCode_Copyright:
      pak_sa OpCode_FullName:
      pak_sa OpCode_FontName:
      pak_sa OpCode_FamilyName:
      pak_sa OpCode_Weight:
      pak_sa OpCode_PostScript:
      pak_sa OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_values_t::name_op_to_index (op)] = env.argStack.pop_uint ();
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_isFixedPitch:
      pak_sa OpCode_ItalicAngle:
      pak_sa OpCode_UnderlinePosition:
      pak_sa OpCode_UnderlineThickness:
      pak_sa OpCode_PaintType:
      pak_sa OpCode_CharstringType:
      pak_sa OpCode_UniqueID:
      pak_sa OpCode_StrokeWidth:
      pak_sa OpCode_SyntheticBase:
      pak_sa OpCode_CIDFontVersion:
      pak_sa OpCode_CIDFontRevision:
      pak_sa OpCode_CIDFontType:
      pak_sa OpCode_UIDBase:
      pak_sa OpCode_FontBBox:
      pak_sa OpCode_XUID:
      pak_sa OpCode_BaseFontBlend:
	env.clear_args ();
	bha_nga;

      pak_sa OpCode_CIDCount:
	dictval.cidCount = env.argStack.pop_uint ();
	env.clear_args ();
	bha_nga;

      pak_sa OpCode_ROS:
	dictval.ros_supplement = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::ordering] = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::registry] = env.argStack.pop_uint ();
	env.clear_args ();
	གལ་སྲིད (unlikely (env.in_error ())) ལོག;
	assert (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	bha_nga;

      pak_sa OpCode_Encoding:
	dictval.EncodingOffset = env.argStack.pop_int ();
	env.clear_args ();
	གལ་སྲིད (unlikely (dictval.EncodingOffset == 0)) ལོག;
	bha_nga;

      pak_sa OpCode_charset:
	dictval.CharsetOffset = env.argStack.pop_int ();
	env.clear_args ();
	གལ་སྲིད (unlikely (dictval.CharsetOffset == 0)) ལོག;
	bha_nga;

      pak_sa OpCode_FDSelect:
	dictval.FDSelectOffset = env.argStack.pop_int ();
	env.clear_args ();
	bha_nga;

      pak_sa OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_int ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	bha_nga;

      default:
	env.last_offset = env.str_ref.get_offset ();
	top_dict_opset_t<cff1_top_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	bha_nga;
    }

    གལ་སྲིད (unlikely (env.in_error ())) ལོག;

    dictval.add_op (op, env.str_ref, val);
  }
};

sa_nracanaa cff1_font_dict_values_t : dict_values_t<op_str_t>
{
  void init ()
  {
    dict_values_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  void fini () { dict_values_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned ཧྲིལ    fontName;
};

sa_nracanaa cff1_font_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_font_dict_values_t& dictval)
  {
    switch (op) {
      pak_sa OpCode_FontName:
	dictval.fontName = env.argStack.pop_uint ();
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_FontMatrix:
      pak_sa OpCode_PaintType:
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_uint ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	bha_nga;

      default:
	dict_opset_t::process_op (op, env);
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	bha_nga;
    }

    གལ་སྲིད (unlikely (env.in_error ())) ལོག;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
sa_nracanaa cff1_private_dict_values_base_t : dict_values_t<VAL>
{
  void init ()
  {
    dict_values_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  void fini () { dict_values_t<VAL>::fini (); }

  ཧྲིལ                 subrsOffset;
  const CFF1Subrs    *localSubrs;
};

typedef cff1_private_dict_values_base_t<op_str_t> cff1_private_dict_values_subset_t;
typedef cff1_private_dict_values_base_t<num_dict_val_t> cff1_private_dict_values_t;

sa_nracanaa cff1_private_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_t& dictval)
  {
    num_dict_val_t val;
    val.init ();

    switch (op) {
      pak_sa OpCode_BlueValues:
      pak_sa OpCode_OtherBlues:
      pak_sa OpCode_FamilyBlues:
      pak_sa OpCode_FamilyOtherBlues:
      pak_sa OpCode_StemSnapH:
      pak_sa OpCode_StemSnapV:
      pak_sa OpCode_StdHW:
      pak_sa OpCode_StdVW:
      pak_sa OpCode_BlueScale:
      pak_sa OpCode_BlueShift:
      pak_sa OpCode_BlueFuzz:
      pak_sa OpCode_ForceBold:
      pak_sa OpCode_LanguageGroup:
      pak_sa OpCode_ExpansionFactor:
      pak_sa OpCode_initialRandomSeed:
      pak_sa OpCode_defaultWidthX:
      pak_sa OpCode_nominalWidthX:
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	bha_nga;

      default:
	dict_opset_t::process_op (op, env);
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	bha_nga;
    }

    གལ་སྲིད (unlikely (env.in_error ())) ལོག;

    dictval.add_op (op, env.str_ref, val);
  }
};

sa_nracanaa cff1_private_dict_opset_subset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_subset_t& dictval)
  {
    switch (op) {
      pak_sa OpCode_BlueValues:
      pak_sa OpCode_OtherBlues:
      pak_sa OpCode_FamilyBlues:
      pak_sa OpCode_FamilyOtherBlues:
      pak_sa OpCode_StemSnapH:
      pak_sa OpCode_StemSnapV:
      pak_sa OpCode_StdHW:
      pak_sa OpCode_StdVW:
      pak_sa OpCode_BlueScale:
      pak_sa OpCode_BlueShift:
      pak_sa OpCode_BlueFuzz:
      pak_sa OpCode_ForceBold:
      pak_sa OpCode_LanguageGroup:
      pak_sa OpCode_ExpansionFactor:
      pak_sa OpCode_initialRandomSeed:
      pak_sa OpCode_defaultWidthX:
      pak_sa OpCode_nominalWidthX:
	env.clear_args ();
	bha_nga;

      pak_sa OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	bha_nga;

      default:
	dict_opset_t::process_op (op, env);
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	bha_nga;
    }

    གལ་སྲིད (unlikely (env.in_error ())) ལོག;

    dictval.add_op (op, env.str_ref);
  }
};

typedef dict_interpreter_t<cff1_top_dict_opset_t, cff1_top_dict_values_t, cff1_top_dict_interp_env_t> cff1_top_dict_interpreter_t;
typedef dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_values_t> cff1_font_dict_interpreter_t;

typedef CFF1Index CFF1NameIndex;
typedef CFF1Index CFF1TopDictIndex;

sa_nracanaa cff1_font_dict_values_mod_t
{
  cff1_font_dict_values_mod_t() { init (); }

  void init () { init ( &Null (cff1_font_dict_values_t), CFF_UNDEF_SID ); }

  void init (const cff1_font_dict_values_t *base_,
	     unsigned ཧྲིལ fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_count () const { ལོག base->get_count (); }

  const op_str_t &operator [] (unsigned ཧྲིལ i) const { ལོག (*base)[i]; }

  const cff1_font_dict_values_t    *base;
  table_info_t		   privateDictInfo;
  unsigned ཧྲིལ		fontName;
};

sa_nracanaa CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ལོག FDArray<HBUINT16>::serialize<cff1_font_dict_values_mod_t, cff1_font_dict_values_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

namespace OT {

using namespace CFF;

sa_nracanaa cff1
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_CFF1;

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  sa_nracanaa accelerator_templ_t
  {
    static constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      གལ་སྲིད (!face) ལོག;

      topDict.init ();
      fontDicts.init ();
      privateDicts.init ();

      this->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (this->blob);
      sc.start_processing ();

      const OT::cff1 *cff = this->blob->template as<OT::cff1> ();

      གལ་སྲིད (cff == &Null (OT::cff1))
        goto fail;

      nameIndex = &cff->nameIndex (cff);
      གལ་སྲིད ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        goto fail;
      hb_barrier ();

      topDictIndex = &StructAtOffsetOrNull<CFF1TopDictIndex> (nameIndex, nameIndex->get_size (), sc);
      གལ་སྲིད (topDictIndex == &Null (CFF1TopDictIndex) || (topDictIndex->count == 0))
        goto fail;
      hb_barrier ();

      { /* parse top dict */
	const hb_ubytes_t topDictStr = (*topDictIndex)[0];
	གལ་སྲིད (unlikely (!topDictStr.sanitize (&sc)))   goto fail;
	hb_barrier ();
	cff1_top_dict_interp_env_t env (topDictStr);
	cff1_top_dict_interpreter_t top_interp (env);
	གལ་སྲིད (unlikely (!top_interp.interpret (topDict)))   goto fail;
      }

      གལ་སྲིད (is_predef_charset ())
	charset = &Null (Charset);
      དེ་མིན
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, topDict.CharsetOffset, sc, &num_charset_entries);
	གལ་སྲིད (unlikely (charset == &Null (Charset)))   goto fail;
      }

      fdCount = 1;
      གལ་སྲིད (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, topDict.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, topDict.FDSelectOffset, sc, fdArray->count);
	གལ་སྲིད (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  goto fail;

	fdCount = fdArray->count;
      }
      དེ་མིན
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      གལ་སྲིད (is_CID ())
      {
	གལ་སྲིད (unlikely (charset == &Null (Charset)))   goto fail;
      }
      དེ་མིན
      {
	གལ་སྲིད (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, topDict.EncodingOffset, sc);
	  གལ་སྲིད (unlikely (encoding == &Null (Encoding)))   goto fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (topDictIndex, topDictIndex->get_size (), sc);
      གལ་སྲིད (stringIndex == &Null (CFF1StringIndex))
        goto fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, topDict.charStringsOffset, sc);
      གལ་སྲིད (charStrings == &Null (CFF1CharStrings))
        goto fail;

      num_glyphs = charStrings->count;
      གལ་སྲིད (num_glyphs != sc.get_num_glyphs ())
        goto fail;

      གལ་སྲིད (unlikely (!privateDicts.resize (fdCount)))
        goto fail;
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < fdCount; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      གལ་སྲིད (is_CID ())
      {
	དོན་ལུ (unsigned ཧྲིལ i = 0; i < fdCount; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  གལ་སྲིད (unlikely (!fontDictStr.sanitize (&sc)))   goto fail;
	  hb_barrier ();
	  cff1_font_dict_values_t *font;
	  cff1_top_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  གལ་སྲིད (unlikely (fontDicts.in_error ()))   goto fail;

	  font->init ();
	  གལ་སྲིད (unlikely (!font_interp.interpret (*font)))   goto fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  const hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  གལ་སྲིད (unlikely (font->privateDictInfo.size &&
			privDictStr == (const unsigned var_na *) &Null (UnsizedByteStr))) goto fail;
	  num_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  གལ་སྲིད (unlikely (!priv_interp.interpret (*priv)))   goto fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      དེ་མིན  /* non-CID */
      {
	cff1_top_dict_values_t *font = &topDict;
	PRIVDICTVAL *priv = &privateDicts[0];

	const hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	གལ་སྲིད (font->privateDictInfo.size &&
	    unlikely (privDictStr == (const unsigned var_na *) &Null (UnsizedByteStr))) goto fail;
	num_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	གལ་སྲིད (unlikely (!priv_interp.interpret (*priv)))   goto fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      ལོག;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    void _fini ()
    {
      sc.end_processing ();
      topDict.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;
    }

    hb_blob_t *get_blob () const { ལོག blob; }

    bool is_valid () const { ལོག blob; }
    bool   is_CID () const { ལོག topDict.is_CID (); }

    bool is_predef_charset () const { ལོག topDict.CharsetOffset <= ExpertSubsetCharset; }

    unsigned ཧྲིལ std_code_to_glyph (hb_codepoint_t code) const
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      གལ་སྲིད (unlikely (sid == CFF_UNDEF_SID))
	ལོག 0;

      གལ་སྲིད (charset != &Null (Charset))
	ལོག charset->get_glyph (sid, num_glyphs);
      དེ་མིན གལ་སྲིད ((topDict.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) ལོག sid;
      ལོག 0;
    }

    bool is_predef_encoding () const { ལོག topDict.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) const
    {
      གལ་སྲིད (encoding != &Null (Encoding))
	ལོག encoding->get_code (glyph);
      དེ་མིན
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	གལ་སྲིད (sid == 0) ལོག 0;
	hb_codepoint_t code = 0;
	switch (topDict.EncodingOffset)
	{
	pak_sa StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  bha_nga;
	pak_sa ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  bha_nga;
	default:
	  bha_nga;
	}
	ལོག code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () const
    {
      གལ་སྲིད (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (parimaa_na (glyph_to_sid_map_t));
	གལ་སྲིད (unlikely (!mapping)) ལོག nullptr;
	mapping = new (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, num_glyphs);
	ལོག mapping;
      }
      དེ་མིན
	ལོག nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) const
    {
      གལ་སྲིད (charset != &Null (Charset))
	ལོག charset->get_sid (glyph, num_glyphs, cache);
      དེ་མིན
      {
	hb_codepoint_t sid = 0;
	switch (topDict.CharsetOffset)
	{
	  pak_sa ISOAdobeCharset:
	    གལ་སྲིད (glyph <= 228 /*zcaron*/) sid = glyph;
	    bha_nga;
	  pak_sa ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    bha_nga;
	  pak_sa ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    bha_nga;
	  default:
	    bha_nga;
	}
	ལོག sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) const
    {
      གལ་སྲིད (charset != &Null (Charset))
	ལོག charset->get_glyph (sid, num_glyphs);
      དེ་མིན
      {
	hb_codepoint_t glyph = 0;
	switch (topDict.CharsetOffset)
	{
	  pak_sa ISOAdobeCharset:
	    གལ་སྲིད (sid <= 228 /*zcaron*/) glyph = sid;
	    bha_nga;
	  pak_sa ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    bha_nga;
	  pak_sa ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    bha_nga;
	  default:
	    bha_nga;
	}
	ལོག glyph;
      }
    }

    rak_sita:
    hb_sanitize_context_t   sc;

    public:
    hb_blob_t               *blob = nullptr;
    const Encoding	    *encoding = nullptr;
    const Charset	    *charset = nullptr;
    const CFF1NameIndex     *nameIndex = nullptr;
    const CFF1TopDictIndex  *topDictIndex = nullptr;
    const CFF1StringIndex   *stringIndex = nullptr;
    const CFF1Subrs	    *globalSubrs = nullptr;
    const CFF1CharStrings   *charStrings = nullptr;
    const CFF1FDArray       *fdArray = nullptr;
    const CFF1FDSelect      *fdSelect = nullptr;
    unsigned ཧྲིལ	     fdCount = 0;

    cff1_top_dict_values_t   topDict;
    hb_vector_t<cff1_font_dict_values_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned ཧྲིལ	     num_glyphs = 0;
    unsigned ཧྲིལ	     num_charset_entries = 0;
  };

  sa_nracanaa accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      གལ་སྲིད (!is_valid ()) ལོག;
      གལ་སྲིད (is_CID ()) ལོག;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      གལ་སྲིད (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    bool get_glyph_name (hb_codepoint_t glyph,
			 var_na *buf, unsigned ཧྲིལ buf_len) const
    {
      གལ་སྲིད (unlikely (glyph >= num_glyphs)) ལོག false;
      གལ་སྲིད (unlikely (!is_valid ())) ལོག false;
      གལ་སྲིད (is_CID()) ལོག false;
      གལ་སྲིད (unlikely (!buf_len)) ལོག true;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      const var_na *str;
      size_t str_len;
      གལ་སྲིད (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      དེ་མིན
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (const var_na *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      གལ་སྲིད (!str_len) ལོག false;
      unsigned ཧྲིལ len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (const var_na*)str, len);
      buf[len] = '\0';
      ལོག true;
    }

    bool get_glyph_from_name (const var_na *name, ཧྲིལ len,
			      hb_codepoint_t *glyph) const
    {
      གལ་སྲིད (unlikely (!is_valid ())) ལོག false;
      གལ་སྲིད (is_CID()) ལོག false;
      གལ་སྲིད (len < 0) len = strlen (name);
      གལ་སྲིད (unlikely (!len)) ལོག false;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      གལ་སྲིད (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, parimaa_na (hb_sorted_vector_t<gname_t>));
	གལ་སྲིད (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  དོན་ལུ (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    གལ་སྲིད (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    དེ་མིན
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((const var_na*) ustr.arrayZ, ustr.length);
	    }
	    གལ་སྲིད (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	གལ་སྲིད (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  གལ་སྲིད (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  goto retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      const gname_t *gname = names ? names->bsearch (key) : nullptr;
      གལ་སྲིད (!gname) ལོག false;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      གལ་སྲིད (!gid && gname->sid) ལོག false;
      *glyph = gid;
      ལོག true;
    }

    HB_INTERNAL bool get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) const;
    HB_INTERNAL bool get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) const;

    private:
    sa_nracanaa gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      static ཧྲིལ cmp (const void *a_, const void *b_)
      {
	const gname_t *a = (const gname_t *)a_;
	const gname_t *b = (const gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	ཧྲིལ ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	གལ་སྲིད (ret) ལོག ret;
	ལོག a->name.length - b->name.length;
      }

      ཧྲིལ cmp (const gname_t &a) const { ལོག cmp (&a, this); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    typedef accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t> SUPER;
  };

  sa_nracanaa accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      གལ་སྲིད (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL bool subset (hb_subset_context_t *c) const;
    HB_INTERNAL bool serialize (hb_serialize_context_t *c,
				sa_nracanaa cff1_subset_plan &plan) const;
    HB_INTERNAL bool get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) const;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    typedef accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t> SUPER;
  };

  rak_sita:
  HB_INTERNAL static hb_codepoint_t lookup_standard_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL static hb_codepoint_t lookup_expert_encoding_for_code (hb_codepoint_t sid);
  HB_INTERNAL static hb_codepoint_t lookup_expert_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL static hb_codepoint_t lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph);
  HB_INTERNAL static hb_codepoint_t lookup_expert_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL static hb_codepoint_t lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid);
  HB_INTERNAL static hb_codepoint_t lookup_standard_encoding_for_sid (hb_codepoint_t code);

  public:
  FixedVersion<HBUINT8> version;	  /* Version of CFF table. set to 0x0100u */
  NNOffsetTo<CFF1NameIndex, HBUINT8> nameIndex; /* headerSize = Offset to Name INDEX. */
  HBUINT8	       offSize;	  /* offset size (unused?) */

  public:
  DEFINE_SIZE_STATIC (4);
};

sa_nracanaa cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

sa_nracanaa cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  void depend (hb_depend_data_builder_t *builder) const
  {
    གལ་སྲིད (!is_valid ()) ལོག;
    དོན་ལུ (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      གལ་སྲིད (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
