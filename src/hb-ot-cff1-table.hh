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

enum EncodingID { StandardEncoding = 0, ExpertEncoding = 1 };
enum CharsetID { ISOAdobeCharset = 0, ExpertCharset = 1, ExpertSubsetCharset = 2 };

typedef CFF1Index          CFF1CharStrings;
typedef Subrs<HBUINT16>    CFF1Subrs;

struct CFF1FDSelect : FDSelect {};

/* Encoding */
struct Encoding0 {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (codes.sanitize (c));
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) const
  {
    assert (glyph > 0);
    glyph--;
    もし (glyph < nCodes ())
    {
      戻る (hb_codepoint_t)codes[glyph];
    }
    否则
      戻る CFF_UNDEF_CODE;
  }

  HBUINT8 &nCodes () { 戻る codes.len; }
  HBUINT8 nCodes () const { 戻る codes.len; }

  ArrayOf<HBUINT8, HBUINT8> codes;

  DEFINE_SIZE_ARRAY_SIZED (1, codes);
};

struct Encoding1_Range {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT8   first;
  HBUINT8   nLeft;

  DEFINE_SIZE_STATIC (2);
};

struct Encoding1 {
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
    ために (unsigned 整数 i = 0; i < nRanges (); i++)
    {
      もし (glyph <= ranges[i].nLeft)
      {
	hb_codepoint_t code = (hb_codepoint_t) ranges[i].first + glyph;
	戻る (likely (code < 0x100) ? code: CFF_UNDEF_CODE);
      }
      glyph -= (ranges[i].nLeft + 1);
    }
    戻る CFF_UNDEF_CODE;
  }

  HBUINT8 &nRanges () { 戻る ranges.len; }
  HBUINT8 nRanges () const { 戻る ranges.len; }

  ArrayOf<Encoding1_Range, HBUINT8> ranges;

  DEFINE_SIZE_ARRAY_SIZED (1, ranges);
};

struct SuppEncoding {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT8   code;
  HBUINT16  glyph;

  DEFINE_SIZE_STATIC (3);
};

struct CFF1SuppEncData {
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (supps.sanitize (c));
  }

  void get_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) const
  {
    ために (unsigned 整数 i = 0; i < nSups (); i++)
      もし (sid == supps[i].glyph)
	codes.push (supps[i].code);
  }

  HBUINT8 &nSups () { 戻る supps.len; }
  HBUINT8 nSups () const { 戻る supps.len; }

  ArrayOf<SuppEncoding, HBUINT8> supps;

  DEFINE_SIZE_ARRAY_SIZED (1, supps);
};

struct Encoding
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
		  unsigned 整数 enc_count,
		  const hb_vector_t<code_pair_t>& code_ranges,
		  const hb_vector_t<code_pair_t>& supp_codes)
  {
    TRACE_SERIALIZE (this);
    Encoding *dest = c->extend_min (this);
    もし (unlikely (!dest)) return_trace (false);
    dest->format = format | ((supp_codes.length > 0) ? 0x80 : 0);
    switch (format) {
    case 0:
    {
      Encoding0 *fmt0 = c->allocate_size<Encoding0> (Encoding0::min_size + HBUINT8::static_size * enc_count);
      もし (unlikely (!fmt0)) return_trace (false);
      fmt0->nCodes () = enc_count;
      unsigned 整数 glyph = 0;
      ために (unsigned 整数 i = 0; i < code_ranges.length; i++)
      {
	hb_codepoint_t code = code_ranges[i].code;
	ために (整数 left = (整数)code_ranges[i].glyph; left >= 0; left--)
	  fmt0->codes[glyph++] = code++;
	もし (unlikely (!((glyph <= 0x100) && (code <= 0x100))))
	  return_trace (false);
      }
    }
    break;

    case 1:
    {
      Encoding1 *fmt1 = c->allocate_size<Encoding1> (Encoding1::min_size + Encoding1_Range::static_size * code_ranges.length);
      もし (unlikely (!fmt1)) return_trace (false);
      fmt1->nRanges () = code_ranges.length;
      ために (unsigned 整数 i = 0; i < code_ranges.length; i++)
      {
	もし (unlikely (!((code_ranges[i].code <= 0xFF) && (code_ranges[i].glyph <= 0xFF))))
	  return_trace (false);
	fmt1->ranges[i].first = code_ranges[i].code;
	fmt1->ranges[i].nLeft = code_ranges[i].glyph;
      }
    }
    break;

    }

    もし (supp_codes.length)
    {
      CFF1SuppEncData *suppData = c->allocate_size<CFF1SuppEncData> (CFF1SuppEncData::min_size + SuppEncoding::static_size * supp_codes.length);
      もし (unlikely (!suppData)) return_trace (false);
      suppData->nSups () = supp_codes.length;
      ために (unsigned 整数 i = 0; i < supp_codes.length; i++)
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
    case 0: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format0.get_size ()); break;
    case 1: hb_barrier (); size = hb_unsigned_add_saturate (size, u.format1.get_size ()); break;
    }
    もし (has_supplement ())
      size = hb_unsigned_add_saturate (size, suppEncData ().get_size ());
    戻る size;
  }

  hb_codepoint_t get_code (hb_codepoint_t glyph) const
  {
    switch (table_format ())
    {
    case 0: hb_barrier (); 戻る u.format0.get_code (glyph);
    case 1: hb_barrier (); 戻る u.format1.get_code (glyph);
    default:戻る 0;
    }
  }

  uint8_t table_format () const { 戻る format & 0x7F; }
  bool  has_supplement () const { 戻る format & 0x80; }

  void get_supplement_codes (hb_codepoint_t sid, hb_vector_t<hb_codepoint_t> &codes) const
  {
    codes.clear ();
    もし (has_supplement ())
      suppEncData().get_codes (sid, codes);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (table_format ())
    {
    case 0: hb_barrier (); もし (unlikely (!u.format0.sanitize (c))) { return_trace (false); } break;
    case 1: hb_barrier (); もし (unlikely (!u.format1.sanitize (c))) { return_trace (false); } break;
    default:return_trace (false);
    }
    return_trace (likely (!has_supplement () || suppEncData ().sanitize (c)));
  }

  protected:
  const CFF1SuppEncData &suppEncData () const
  {
    switch (table_format ())
    {
    case 0: hb_barrier (); 戻る StructAfter<CFF1SuppEncData> (u.format0.codes[u.format0.nCodes ()-1]);
    case 1: hb_barrier (); 戻る StructAfter<CFF1SuppEncData> (u.format1.ranges[u.format1.nRanges ()-1]);
    default:戻る Null (CFF1SuppEncData);
    }
  }

  public:
  HBUINT8	format;
  union {
  Encoding0	format0;
  Encoding1	format1;
  } u;
  /* CFF1SuppEncData  suppEncData; */

  DEFINE_SIZE_MIN (1);
};

/* Charset */
struct Charset0
{
  bool sanitize (hb_sanitize_context_t *c, unsigned 整数 num_glyphs, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    もし (num_charset_entries) *num_charset_entries = num_glyphs;
    return_trace (sids.sanitize (c, num_glyphs - 1));
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs) const
  {
    もし (unlikely (glyph >= num_glyphs)) 戻る 0;
    もし (unlikely (glyph == 0))
      戻る 0;
    否则
      戻る sids[glyph - 1];
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned 整数 num_glyphs) const
  {
    もし (!mapping->resize_dirty  (num_glyphs)) 戻る;
    ために (hb_codepoint_t gid = 1; gid < num_glyphs; gid++)
      mapping->arrayZ[gid] = {sids[gid - 1], gid};
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned 整数 num_glyphs) const
  {
    もし (sid == 0)
      戻る 0;

    ために (unsigned 整数 glyph = 1; glyph < num_glyphs; glyph++)
    {
      もし (sids[glyph-1] == sid)
	戻る glyph;
    }
    戻る 0;
  }

  static size_t get_size (unsigned 整数 num_glyphs)
  {
    assert (num_glyphs > 0);
    戻る UnsizedArrayOf<HBUINT16>::get_size (num_glyphs - 1);
  }

  UnsizedArrayOf<HBUINT16> sids;

  DEFINE_SIZE_ARRAY(0, sids);
};

template <typename TYPE>
struct Charset_Range {
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
struct Charset1_2 {
  bool sanitize (hb_sanitize_context_t *c, unsigned 整数 num_glyphs, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    num_glyphs--;
    unsigned i;
    ために (i = 0; num_glyphs > 0; i++)
    {
      もし (unlikely (!(ranges[i].sanitize (c) &&
		      hb_barrier () &&
		      (num_glyphs >= ranges[i].nLeft + 1))))
	return_trace (false);
      num_glyphs -= (ranges[i].nLeft + 1);
    }
    もし (num_charset_entries)
      *num_charset_entries = i;
    return_trace (true);
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned num_glyphs,
			  code_pair_t *cache = nullptr) const
  {
    もし (unlikely (glyph >= num_glyphs)) 戻る 0;
    unsigned i;
    hb_codepoint_t start_glyph;
    もし (cache && likely (cache->glyph <= glyph))
    {
      i = cache->code;
      start_glyph = cache->glyph;
    }
    否则
    {
      もし (unlikely (glyph == 0)) 戻る 0;
      i = 0;
      start_glyph = 1;
    }
    glyph -= start_glyph;
    ために (;; i++)
    {
      unsigned count = ranges[i].nLeft;
      もし (glyph <= count)
      {
        もし (cache)
	  *cache = {i, start_glyph};
	戻る ranges[i].first + glyph;
      }
      count++;
      start_glyph += count;
      glyph -= count;
    }

    戻る 0;
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned 整数 num_glyphs) const
  {
    もし (!mapping->resize_dirty  (num_glyphs)) 戻る;

    hb_codepoint_t gid = 1;
    もし (gid >= num_glyphs)
      戻る;

    ために (unsigned i = 0;; i++)
    {
      hb_codepoint_t sid = ranges[i].first;
      unsigned count = ranges[i].nLeft + 1;
      unsigned last = gid + count;
      ために (unsigned j = 0; j < count; j++)
	mapping->arrayZ[gid++] = {sid++, last - 1};

      もし (gid >= num_glyphs)
        break;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned 整数 num_glyphs) const
  {
    もし (sid == 0) 戻る 0;
    hb_codepoint_t  glyph = 1;
    ために (unsigned 整数 i = 0;; i++)
    {
      もし (glyph >= num_glyphs)
	戻る 0;
      もし ((ranges[i].first <= sid) && (sid <= ranges[i].first + ranges[i].nLeft))
	戻る glyph + (sid - ranges[i].first);
      glyph += (ranges[i].nLeft + 1);
    }

    戻る 0;
  }

  size_t get_size (unsigned 整数 num_glyphs) const
  {
    整数 glyph = (整数) num_glyphs;
    unsigned num_ranges = 0;

    assert (glyph > 0);
    glyph--;
    ために (unsigned 整数 i = 0; glyph > 0; i++)
    {
      glyph -= (ranges[i].nLeft + 1);
      num_ranges++;
    }

    戻る get_size_for_ranges (num_ranges);
  }

  static size_t get_size_for_ranges (unsigned 整数 num_ranges)
  {
    戻る UnsizedArrayOf<Charset_Range<TYPE> >::get_size (num_ranges);
  }

  UnsizedArrayOf<Charset_Range<TYPE>> ranges;

  DEFINE_SIZE_ARRAY (0, ranges);
};

typedef Charset1_2<HBUINT8>     Charset1;
typedef Charset1_2<HBUINT16>    Charset2;
typedef Charset_Range<HBUINT8>  Charset1_Range;
typedef Charset_Range<HBUINT16> Charset2_Range;

struct Charset
{
  /* serialize a fullset Charset */
  bool serialize (hb_serialize_context_t *c, const Charset &src, unsigned 整数 num_glyphs)
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed ((const char *) &src, src.get_size (num_glyphs)));
  }

  /* serialize a subset Charset */
  bool serialize (hb_serialize_context_t *c,
		  uint8_t format,
		  unsigned 整数 num_glyphs,
		  const hb_vector_t<code_pair_t>& sid_ranges)
  {
    TRACE_SERIALIZE (this);
    Charset *dest = c->extend_min (this);
    もし (unlikely (!dest)) return_trace (false);
    dest->format = format;
    switch (format)
    {
    case 0:
    {
      Charset0 *fmt0 = c->allocate_size<Charset0> (Charset0::get_size (num_glyphs), false);
      もし (unlikely (!fmt0)) return_trace (false);
      unsigned 整数 glyph = 0;
      ために (unsigned 整数 i = 0; i < sid_ranges.length; i++)
      {
	hb_codepoint_t sid = sid_ranges.arrayZ[i].code;
	ために (整数 left = (整数)sid_ranges.arrayZ[i].glyph; left >= 0; left--)
	  fmt0->sids[glyph++] = sid++;
      }
    }
    break;

    case 1:
    {
      Charset1 *fmt1 = c->allocate_size<Charset1> (Charset1::get_size_for_ranges (sid_ranges.length), false);
      もし (unlikely (!fmt1)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      ために (unsigned 整数 i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt1->ranges[i].first = _.code;
	fmt1->ranges[i].nLeft = _.glyph;
      }
      もし (unlikely (!(all_glyphs <= 0xFF)))
	return_trace (false);
    }
    break;

    case 2:
    {
      Charset2 *fmt2 = c->allocate_size<Charset2> (Charset2::get_size_for_ranges (sid_ranges.length), false);
      もし (unlikely (!fmt2)) return_trace (false);
      hb_codepoint_t all_glyphs = 0;
      ために (unsigned 整数 i = 0; i < sid_ranges.length; i++)
      {
        auto &_ = sid_ranges.arrayZ[i];
        all_glyphs |= _.glyph;
	fmt2->ranges[i].first = _.code;
	fmt2->ranges[i].nLeft = _.glyph;
      }
      もし (unlikely (!(all_glyphs <= 0xFFFF)))
	return_trace (false);
    }
    break;

    }
    return_trace (true);
  }

  size_t get_size (unsigned 整数 num_glyphs) const
  {
    switch (format)
    {
    case 0: hb_barrier (); 戻る hb_unsigned_add_saturate (min_size, u.format0.get_size (num_glyphs));
    case 1: hb_barrier (); 戻る hb_unsigned_add_saturate (min_size, u.format1.get_size (num_glyphs));
    case 2: hb_barrier (); 戻る hb_unsigned_add_saturate (min_size, u.format2.get_size (num_glyphs));
    default:戻る 0;
    }
  }

  hb_codepoint_t get_sid (hb_codepoint_t glyph, unsigned 整数 num_glyphs,
			  code_pair_t *cache = nullptr) const
  {
    switch (format)
    {
    case 0: hb_barrier (); 戻る u.format0.get_sid (glyph, num_glyphs);
    case 1: hb_barrier (); 戻る u.format1.get_sid (glyph, num_glyphs, cache);
    case 2: hb_barrier (); 戻る u.format2.get_sid (glyph, num_glyphs, cache);
    default:戻る 0;
    }
  }

  void collect_glyph_to_sid_map (glyph_to_sid_map_t *mapping, unsigned 整数 num_glyphs) const
  {
    switch (format)
    {
    case 0: hb_barrier (); u.format0.collect_glyph_to_sid_map (mapping, num_glyphs); 戻る;
    case 1: hb_barrier (); u.format1.collect_glyph_to_sid_map (mapping, num_glyphs); 戻る;
    case 2: hb_barrier (); u.format2.collect_glyph_to_sid_map (mapping, num_glyphs); 戻る;
    default:戻る;
    }
  }

  hb_codepoint_t get_glyph (hb_codepoint_t sid, unsigned 整数 num_glyphs) const
  {
    switch (format)
    {
    case 0: hb_barrier (); 戻る u.format0.get_glyph (sid, num_glyphs);
    case 1: hb_barrier (); 戻る u.format1.get_glyph (sid, num_glyphs);
    case 2: hb_barrier (); 戻る u.format2.get_glyph (sid, num_glyphs);
    default:戻る 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c, unsigned *num_charset_entries) const
  {
    TRACE_SANITIZE (this);
    もし (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (format)
    {
    case 0: hb_barrier (); return_trace (u.format0.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c, c->get_num_glyphs (), num_charset_entries));
    default:return_trace (false);
    }
  }

  HBUINT8       format;
  union {
    Charset0    format0;
    Charset1    format1;
    Charset2    format2;
  } u;

  DEFINE_SIZE_MIN (1);
};

struct CFF1StringIndex : CFF1Index
{
  bool serialize (hb_serialize_context_t *c, const CFF1StringIndex &strings,
		  const hb_vector_t<unsigned> &sidmap)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely ((strings.count == 0) || (sidmap.length == 0)))
    {
      もし (unlikely (!c->extend_min (this->count)))
	return_trace (false);
      count = 0;
      return_trace (true);
    }

    もし (unlikely (sidmap.in_error ())) return_trace (false);

    // Save this in a vector since serialize() iterates it twice.
    hb_vector_t<hb_ubytes_t> bytesArray (+ hb_iter (sidmap)
					 | hb_map (strings));

    もし (unlikely (bytesArray.in_error ())) return_trace (false);

    bool result = CFF1Index::serialize (c, bytesArray);
    return_trace (result);
  }
};

struct cff1_top_dict_interp_env_t : num_interp_env_t
{
  cff1_top_dict_interp_env_t ()
    : num_interp_env_t(), prev_offset(0), last_offset(0) {}
  cff1_top_dict_interp_env_t (const hb_ubytes_t &bytes)
    : num_interp_env_t(bytes), prev_offset(0), last_offset(0) {}

  unsigned 整数 prev_offset;
  unsigned 整数 last_offset;
};

struct name_dict_values_t
{
  enum name_dict_val_index_t
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
    ために (unsigned 整数 i = 0; i < ValCount; i++)
      values[i] = CFF_UNDEF_SID;
  }

  unsigned 整数& operator[] (unsigned 整数 i)
  { assert (i < ValCount); 戻る values[i]; }

  unsigned 整数 operator[] (unsigned 整数 i) const
  { assert (i < ValCount); 戻る values[i]; }

  static enum name_dict_val_index_t name_op_to_index (op_code_t op)
  {
    switch (op) {
      default: // can't happen - just make some compiler happy
      case OpCode_version:
	戻る version;
      case OpCode_Notice:
	戻る notice;
      case OpCode_Copyright:
	戻る copyright;
      case OpCode_FullName:
	戻る fullName;
      case OpCode_FamilyName:
	戻る familyName;
      case OpCode_Weight:
	戻る weight;
      case OpCode_PostScript:
	戻る postscript;
      case OpCode_FontName:
	戻る fontName;
      case OpCode_BaseFontName:
	戻る baseFontName;
    }
  }

  unsigned 整数  values[ValCount];
};

struct cff1_top_dict_val_t : op_str_t
{
  unsigned 整数  last_arg_offset;
};

struct cff1_top_dict_values_t : top_dict_values_t<cff1_top_dict_val_t>
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
  { 戻る nameSIDs[name_dict_values_t::registry] != CFF_UNDEF_SID; }

  name_dict_values_t  nameSIDs;
  unsigned 整数    ros_supplement_offset;
  unsigned 整数    ros_supplement;
  unsigned 整数    cidCount;

  整数             EncodingOffset;
  整数             CharsetOffset;
  整数             FDSelectOffset;
  table_info_t    privateDictInfo;
};

struct cff1_top_dict_opset_t : top_dict_opset_t<cff1_top_dict_val_t>
{
  static void process_op (op_code_t op, cff1_top_dict_interp_env_t& env, cff1_top_dict_values_t& dictval)
  {
    cff1_top_dict_val_t  val {};

    switch (op) {
      case OpCode_version:
      case OpCode_Notice:
      case OpCode_Copyright:
      case OpCode_FullName:
      case OpCode_FontName:
      case OpCode_FamilyName:
      case OpCode_Weight:
      case OpCode_PostScript:
      case OpCode_BaseFontName:
	dictval.nameSIDs[name_dict_values_t::name_op_to_index (op)] = env.argStack.pop_uint ();
	env.clear_args ();
	break;
      case OpCode_isFixedPitch:
      case OpCode_ItalicAngle:
      case OpCode_UnderlinePosition:
      case OpCode_UnderlineThickness:
      case OpCode_PaintType:
      case OpCode_CharstringType:
      case OpCode_UniqueID:
      case OpCode_StrokeWidth:
      case OpCode_SyntheticBase:
      case OpCode_CIDFontVersion:
      case OpCode_CIDFontRevision:
      case OpCode_CIDFontType:
      case OpCode_UIDBase:
      case OpCode_FontBBox:
      case OpCode_XUID:
      case OpCode_BaseFontBlend:
	env.clear_args ();
	break;

      case OpCode_CIDCount:
	dictval.cidCount = env.argStack.pop_uint ();
	env.clear_args ();
	break;

      case OpCode_ROS:
	dictval.ros_supplement = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::ordering] = env.argStack.pop_uint ();
	dictval.nameSIDs[name_dict_values_t::registry] = env.argStack.pop_uint ();
	env.clear_args ();
	もし (unlikely (env.in_error ())) 戻る;
	assert (env.last_offset > dictval.opStart);
	val.last_arg_offset = (env.last_offset - 1) - dictval.opStart;  /* offset to the last argument */
	break;

      case OpCode_Encoding:
	dictval.EncodingOffset = env.argStack.pop_int ();
	env.clear_args ();
	もし (unlikely (dictval.EncodingOffset == 0)) 戻る;
	break;

      case OpCode_charset:
	dictval.CharsetOffset = env.argStack.pop_int ();
	env.clear_args ();
	もし (unlikely (dictval.CharsetOffset == 0)) 戻る;
	break;

      case OpCode_FDSelect:
	dictval.FDSelectOffset = env.argStack.pop_int ();
	env.clear_args ();
	break;

      case OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_int ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	break;

      default:
	env.last_offset = env.str_ref.get_offset ();
	top_dict_opset_t<cff1_top_dict_val_t>::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	もし (!env.argStack.is_empty ()) 戻る;
	break;
    }

    もし (unlikely (env.in_error ())) 戻る;

    dictval.add_op (op, env.str_ref, val);
  }
};

struct cff1_font_dict_values_t : dict_values_t<op_str_t>
{
  void init ()
  {
    dict_values_t<op_str_t>::init ();
    privateDictInfo.init ();
    fontName = CFF_UNDEF_SID;
  }
  void fini () { dict_values_t<op_str_t>::fini (); }

  table_info_t       privateDictInfo;
  unsigned 整数    fontName;
};

struct cff1_font_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_font_dict_values_t& dictval)
  {
    switch (op) {
      case OpCode_FontName:
	dictval.fontName = env.argStack.pop_uint ();
	env.clear_args ();
	break;
      case OpCode_FontMatrix:
      case OpCode_PaintType:
	env.clear_args ();
	break;
      case OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_uint ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	break;

      default:
	dict_opset_t::process_op (op, env);
	もし (!env.argStack.is_empty ()) 戻る;
	break;
    }

    もし (unlikely (env.in_error ())) 戻る;

    dictval.add_op (op, env.str_ref);
  }
};

template <typename VAL>
struct cff1_private_dict_values_base_t : dict_values_t<VAL>
{
  void init ()
  {
    dict_values_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF1Subrs);
  }
  void fini () { dict_values_t<VAL>::fini (); }

  整数                 subrsOffset;
  const CFF1Subrs    *localSubrs;
};

typedef cff1_private_dict_values_base_t<op_str_t> cff1_private_dict_values_subset_t;
typedef cff1_private_dict_values_base_t<num_dict_val_t> cff1_private_dict_values_t;

struct cff1_private_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_t& dictval)
  {
    num_dict_val_t val;
    val.init ();

    switch (op) {
      case OpCode_BlueValues:
      case OpCode_OtherBlues:
      case OpCode_FamilyBlues:
      case OpCode_FamilyOtherBlues:
      case OpCode_StemSnapH:
      case OpCode_StemSnapV:
      case OpCode_StdHW:
      case OpCode_StdVW:
      case OpCode_BlueScale:
      case OpCode_BlueShift:
      case OpCode_BlueFuzz:
      case OpCode_ForceBold:
      case OpCode_LanguageGroup:
      case OpCode_ExpansionFactor:
      case OpCode_initialRandomSeed:
      case OpCode_defaultWidthX:
      case OpCode_nominalWidthX:
	env.clear_args ();
	break;
      case OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	break;

      default:
	dict_opset_t::process_op (op, env);
	もし (!env.argStack.is_empty ()) 戻る;
	break;
    }

    もし (unlikely (env.in_error ())) 戻る;

    dictval.add_op (op, env.str_ref, val);
  }
};

struct cff1_private_dict_opset_subset_t : dict_opset_t
{
  static void process_op (op_code_t op, num_interp_env_t& env, cff1_private_dict_values_subset_t& dictval)
  {
    switch (op) {
      case OpCode_BlueValues:
      case OpCode_OtherBlues:
      case OpCode_FamilyBlues:
      case OpCode_FamilyOtherBlues:
      case OpCode_StemSnapH:
      case OpCode_StemSnapV:
      case OpCode_StdHW:
      case OpCode_StdVW:
      case OpCode_BlueScale:
      case OpCode_BlueShift:
      case OpCode_BlueFuzz:
      case OpCode_ForceBold:
      case OpCode_LanguageGroup:
      case OpCode_ExpansionFactor:
      case OpCode_initialRandomSeed:
      case OpCode_defaultWidthX:
      case OpCode_nominalWidthX:
	env.clear_args ();
	break;

      case OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	break;

      default:
	dict_opset_t::process_op (op, env);
	もし (!env.argStack.is_empty ()) 戻る;
	break;
    }

    もし (unlikely (env.in_error ())) 戻る;

    dictval.add_op (op, env.str_ref);
  }
};

typedef dict_interpreter_t<cff1_top_dict_opset_t, cff1_top_dict_values_t, cff1_top_dict_interp_env_t> cff1_top_dict_interpreter_t;
typedef dict_interpreter_t<cff1_font_dict_opset_t, cff1_font_dict_values_t> cff1_font_dict_interpreter_t;

typedef CFF1Index CFF1NameIndex;
typedef CFF1Index CFF1TopDictIndex;

struct cff1_font_dict_values_mod_t
{
  cff1_font_dict_values_mod_t() { init (); }

  void init () { init ( &Null (cff1_font_dict_values_t), CFF_UNDEF_SID ); }

  void init (const cff1_font_dict_values_t *base_,
	     unsigned 整数 fontName_)
  {
    base = base_;
    fontName = fontName_;
    privateDictInfo.init ();
  }

  unsigned get_count () const { 戻る base->get_count (); }

  const op_str_t &operator [] (unsigned 整数 i) const { 戻る (*base)[i]; }

  const cff1_font_dict_values_t    *base;
  table_info_t		   privateDictInfo;
  unsigned 整数		fontName;
};

struct CFF1FDArray : FDArray<HBUINT16>
{
  /* FDArray::serialize() requires this partial specialization to compile */
  template <typename ITER, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { 戻る FDArray<HBUINT16>::serialize<cff1_font_dict_values_mod_t, cff1_font_dict_values_mod_t> (c, it, opszr); }
};

} /* namespace CFF */

namespace OT {

using namespace CFF;

struct cff1
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
  struct accelerator_templ_t
  {
    static constexpr hb_tag_t tableTag = cff1::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      もし (!face) 戻る;

      topDict.init ();
      fontDicts.init ();
      privateDicts.init ();

      this->blob = sc.reference_table<cff1> (face);

      /* setup for run-time sanitization */
      sc.init (this->blob);
      sc.start_processing ();

      const OT::cff1 *cff = this->blob->template as<OT::cff1> ();

      もし (cff == &Null (OT::cff1))
        goto fail;

      nameIndex = &cff->nameIndex (cff);
      もし ((nameIndex == &Null (CFF1NameIndex)) || !nameIndex->sanitize (&sc))
        goto fail;
      hb_barrier ();

      topDictIndex = &StructAtOffsetOrNull<CFF1TopDictIndex> (nameIndex, nameIndex->get_size (), sc);
      もし (topDictIndex == &Null (CFF1TopDictIndex) || (topDictIndex->count == 0))
        goto fail;
      hb_barrier ();

      { /* parse top dict */
	const hb_ubytes_t topDictStr = (*topDictIndex)[0];
	もし (unlikely (!topDictStr.sanitize (&sc)))   goto fail;
	hb_barrier ();
	cff1_top_dict_interp_env_t env (topDictStr);
	cff1_top_dict_interpreter_t top_interp (env);
	もし (unlikely (!top_interp.interpret (topDict)))   goto fail;
      }

      もし (is_predef_charset ())
	charset = &Null (Charset);
      否则
      {
	charset = &StructAtOffsetOrNull<Charset> (cff, topDict.CharsetOffset, sc, &num_charset_entries);
	もし (unlikely (charset == &Null (Charset)))   goto fail;
      }

      fdCount = 1;
      もし (is_CID ())
      {
	fdArray = &StructAtOffsetOrNull<CFF1FDArray> (cff, topDict.FDArrayOffset, sc);
	fdSelect = &StructAtOffsetOrNull<CFF1FDSelect> (cff, topDict.FDSelectOffset, sc, fdArray->count);
	もし (unlikely (fdArray == &Null (CFF1FDArray) ||
		      fdSelect == &Null (CFF1FDSelect)))
	  goto fail;

	fdCount = fdArray->count;
      }
      否则
      {
	fdArray = &Null (CFF1FDArray);
	fdSelect = &Null (CFF1FDSelect);
      }

      encoding = &Null (Encoding);
      もし (is_CID ())
      {
	もし (unlikely (charset == &Null (Charset)))   goto fail;
      }
      否则
      {
	もし (!is_predef_encoding ())
	{
	  encoding = &StructAtOffsetOrNull<Encoding> (cff, topDict.EncodingOffset, sc);
	  もし (unlikely (encoding == &Null (Encoding)))   goto fail;
	}
      }

      stringIndex = &StructAtOffsetOrNull<CFF1StringIndex> (topDictIndex, topDictIndex->get_size (), sc);
      もし (stringIndex == &Null (CFF1StringIndex))
        goto fail;

      globalSubrs = &StructAtOffsetOrNull<CFF1Subrs> (stringIndex, stringIndex->get_size (), sc);
      charStrings = &StructAtOffsetOrNull<CFF1CharStrings> (cff, topDict.charStringsOffset, sc);
      もし (charStrings == &Null (CFF1CharStrings))
        goto fail;

      num_glyphs = charStrings->count;
      もし (num_glyphs != sc.get_num_glyphs ())
        goto fail;

      もし (unlikely (!privateDicts.resize (fdCount)))
        goto fail;
      ために (unsigned 整数 i = 0; i < fdCount; i++)
	privateDicts[i].init ();

      // parse CID font dicts and gather private dicts
      もし (is_CID ())
      {
	ために (unsigned 整数 i = 0; i < fdCount; i++)
	{
	  hb_ubytes_t fontDictStr = (*fdArray)[i];
	  もし (unlikely (!fontDictStr.sanitize (&sc)))   goto fail;
	  hb_barrier ();
	  cff1_font_dict_values_t *font;
	  cff1_top_dict_interp_env_t env (fontDictStr);
	  cff1_font_dict_interpreter_t font_interp (env);
	  font = fontDicts.push ();
	  もし (unlikely (fontDicts.in_error ()))   goto fail;

	  font->init ();
	  もし (unlikely (!font_interp.interpret (*font)))   goto fail;
	  PRIVDICTVAL *priv = &privateDicts[i];
	  const hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	  もし (unlikely (font->privateDictInfo.size &&
			privDictStr == (const unsigned char *) &Null (UnsizedByteStr))) goto fail;
	  num_interp_env_t env2 (privDictStr);
	  dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env2);
	  priv->init ();
	  もし (unlikely (!priv_interp.interpret (*priv)))   goto fail;

	  priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	}
      }
      否则  /* non-CID */
      {
	cff1_top_dict_values_t *font = &topDict;
	PRIVDICTVAL *priv = &privateDicts[0];

	const hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	もし (font->privateDictInfo.size &&
	    unlikely (privDictStr == (const unsigned char *) &Null (UnsizedByteStr))) goto fail;
	num_interp_env_t env (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL> priv_interp (env);
	priv->init ();
	もし (unlikely (!priv_interp.interpret (*priv)))   goto fail;

	priv->localSubrs = &StructAtOffsetOrNull<CFF1Subrs> (&privDictStr, priv->subrsOffset, sc);
	hb_barrier ();
      }

      戻る;

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

    hb_blob_t *get_blob () const { 戻る blob; }

    bool is_valid () const { 戻る blob; }
    bool   is_CID () const { 戻る topDict.is_CID (); }

    bool is_predef_charset () const { 戻る topDict.CharsetOffset <= ExpertSubsetCharset; }

    unsigned 整数 std_code_to_glyph (hb_codepoint_t code) const
    {
      hb_codepoint_t sid = lookup_standard_encoding_for_sid (code);
      もし (unlikely (sid == CFF_UNDEF_SID))
	戻る 0;

      もし (charset != &Null (Charset))
	戻る charset->get_glyph (sid, num_glyphs);
      否则 もし ((topDict.CharsetOffset == ISOAdobeCharset)
	      && (code <= 228 /*zcaron*/)) 戻る sid;
      戻る 0;
    }

    bool is_predef_encoding () const { 戻る topDict.EncodingOffset <= ExpertEncoding; }

    hb_codepoint_t glyph_to_code (hb_codepoint_t glyph,
				  code_pair_t *glyph_to_sid_cache = nullptr) const
    {
      もし (encoding != &Null (Encoding))
	戻る encoding->get_code (glyph);
      否则
      {
	hb_codepoint_t sid = glyph_to_sid (glyph, glyph_to_sid_cache);
	もし (sid == 0) 戻る 0;
	hb_codepoint_t code = 0;
	switch (topDict.EncodingOffset)
	{
	case StandardEncoding:
	  code = lookup_standard_encoding_for_code (sid);
	  break;
	case ExpertEncoding:
	  code = lookup_expert_encoding_for_code (sid);
	  break;
	default:
	  break;
	}
	戻る code;
      }
    }

    glyph_to_sid_map_t *create_glyph_to_sid_map () const
    {
      もし (charset != &Null (Charset))
      {
	auto *mapping = (glyph_to_sid_map_t *) hb_malloc (sizeof (glyph_to_sid_map_t));
	もし (unlikely (!mapping)) 戻る nullptr;
	mapping = new (mapping) glyph_to_sid_map_t ();
	mapping->push (code_pair_t {0, 1});
	charset->collect_glyph_to_sid_map (mapping, num_glyphs);
	戻る mapping;
      }
      否则
	戻る nullptr;
    }

    hb_codepoint_t glyph_to_sid (hb_codepoint_t glyph,
				 code_pair_t *cache = nullptr) const
    {
      もし (charset != &Null (Charset))
	戻る charset->get_sid (glyph, num_glyphs, cache);
      否则
      {
	hb_codepoint_t sid = 0;
	switch (topDict.CharsetOffset)
	{
	  case ISOAdobeCharset:
	    もし (glyph <= 228 /*zcaron*/) sid = glyph;
	    break;
	  case ExpertCharset:
	    sid = lookup_expert_charset_for_sid (glyph);
	    break;
	  case ExpertSubsetCharset:
	      sid = lookup_expert_subset_charset_for_sid (glyph);
	    break;
	  default:
	    break;
	}
	戻る sid;
      }
    }

    hb_codepoint_t sid_to_glyph (hb_codepoint_t sid) const
    {
      もし (charset != &Null (Charset))
	戻る charset->get_glyph (sid, num_glyphs);
      否则
      {
	hb_codepoint_t glyph = 0;
	switch (topDict.CharsetOffset)
	{
	  case ISOAdobeCharset:
	    もし (sid <= 228 /*zcaron*/) glyph = sid;
	    break;
	  case ExpertCharset:
	    glyph = lookup_expert_charset_for_glyph (sid);
	    break;
	  case ExpertSubsetCharset:
	    glyph = lookup_expert_subset_charset_for_glyph (sid);
	    break;
	  default:
	    break;
	}
	戻る glyph;
      }
    }

    protected:
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
    unsigned 整数	     fdCount = 0;

    cff1_top_dict_values_t   topDict;
    hb_vector_t<cff1_font_dict_values_t>
			     fontDicts;
    hb_vector_t<PRIVDICTVAL> privateDicts;

    unsigned 整数	     num_glyphs = 0;
    unsigned 整数	     num_charset_entries = 0;
  };

  struct accelerator_t : accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t>
  {
    accelerator_t (hb_face_t *face) : SUPER (face)
    {
      glyph_names.set_relaxed (nullptr);

      もし (!is_valid ()) 戻る;
      もし (is_CID ()) 戻る;
    }
    ~accelerator_t ()
    {
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_relaxed ();
      もし (names)
      {
	names->fini ();
	hb_free (names);
      }
    }

    bool get_glyph_name (hb_codepoint_t glyph,
			 char *buf, unsigned 整数 buf_len) const
    {
      もし (unlikely (glyph >= num_glyphs)) 戻る false;
      もし (unlikely (!is_valid ())) 戻る false;
      もし (is_CID()) 戻る false;
      もし (unlikely (!buf_len)) 戻る true;
      hb_codepoint_t sid = glyph_to_sid (glyph);
      const char *str;
      size_t str_len;
      もし (sid < cff1_std_strings_length)
      {
	hb_bytes_t byte_str = cff1_std_strings (sid);
	str = byte_str.arrayZ;
	str_len = byte_str.length;
      }
      否则
      {
	hb_ubytes_t ubyte_str = (*stringIndex)[sid - cff1_std_strings_length];
	str = (const char *)ubyte_str.arrayZ;
	str_len = ubyte_str.length;
      }
      もし (!str_len) 戻る false;
      unsigned 整数 len = hb_min (buf_len - 1, str_len);
      strncpy (buf, (const char*)str, len);
      buf[len] = '\0';
      戻る true;
    }

    bool get_glyph_from_name (const char *name, 整数 len,
			      hb_codepoint_t *glyph) const
    {
      もし (unlikely (!is_valid ())) 戻る false;
      もし (is_CID()) 戻る false;
      もし (len < 0) len = strlen (name);
      もし (unlikely (!len)) 戻る false;

    retry:
      hb_sorted_vector_t<gname_t> *names = glyph_names.get_acquire ();
      もし (unlikely (!names))
      {
	names = (hb_sorted_vector_t<gname_t> *) hb_calloc (1, sizeof (hb_sorted_vector_t<gname_t>));
	もし (likely (names))
	{
	  names->init ();
	  /* TODO */

	  /* fill glyph names */
	  code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
	  ために (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
	  {
	    hb_codepoint_t	sid = glyph_to_sid (gid, &glyph_to_sid_cache);
	    gname_t	gname;
	    gname.sid = sid;
	    もし (sid < cff1_std_strings_length)
	      gname.name = cff1_std_strings (sid);
	    否则
	    {
	      hb_ubytes_t	ustr = (*stringIndex)[sid - cff1_std_strings_length];
	      gname.name = hb_bytes_t ((const char*) ustr.arrayZ, ustr.length);
	    }
	    もし (unlikely (!gname.name.arrayZ))
	      gname.name = hb_bytes_t ("", 0); /* To avoid nullptr. */
	    names->push (gname);
	  }
	  names->qsort ();
	}
	もし (unlikely (!glyph_names.cmpexch (nullptr, names)))
	{
	  もし (names)
	  {
	    names->fini ();
	    hb_free (names);
	  }
	  goto retry;
	}
      }

      gname_t key = { hb_bytes_t (name, len), 0 };
      const gname_t *gname = names ? names->bsearch (key) : nullptr;
      もし (!gname) 戻る false;
      hb_codepoint_t gid = sid_to_glyph (gname->sid);
      もし (!gid && gname->sid) 戻る false;
      *glyph = gid;
      戻る true;
    }

    HB_INTERNAL bool get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget = nullptr) const;
    HB_INTERNAL bool get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) const;

    private:
    struct gname_t
    {
      hb_bytes_t	name;
      uint16_t		sid;

      static 整数 cmp (const void *a_, const void *b_)
      {
	const gname_t *a = (const gname_t *)a_;
	const gname_t *b = (const gname_t *)b_;
	unsigned minlen = hb_min (a->name.length, b->name.length);
	整数 ret = strncmp (a->name.arrayZ, b->name.arrayZ, minlen);
	もし (ret) 戻る ret;
	戻る a->name.length - b->name.length;
      }

      整数 cmp (const gname_t &a) const { 戻る cmp (&a, this); }
    };

    mutable hb_atomic_t<hb_sorted_vector_t<gname_t> *> glyph_names;

    typedef accelerator_templ_t<cff1_private_dict_opset_t, cff1_private_dict_values_t> SUPER;
  };

  struct accelerator_subset_t : accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      もし (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL bool subset (hb_subset_context_t *c) const;
    HB_INTERNAL bool serialize (hb_serialize_context_t *c,
				struct cff1_subset_plan &plan) const;
    HB_INTERNAL bool get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) const;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    typedef accelerator_templ_t<cff1_private_dict_opset_subset_t, cff1_private_dict_values_subset_t> SUPER;
  };

  protected:
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

struct cff1_accelerator_t : cff1::accelerator_t {
  cff1_accelerator_t (hb_face_t *face) : cff1::accelerator_t (face) {}
};

struct cff1_subset_accelerator_t : cff1::accelerator_subset_t {
  cff1_subset_accelerator_t (hb_face_t *face) : cff1::accelerator_subset_t (face) {}

  void depend (hb_depend_data_builder_t *builder) const
  {
    もし (!is_valid ()) 戻る;
    ために (hb_codepoint_t gid = 0; gid < num_glyphs; gid++)
    {
      hb_codepoint_t base_gid, accent_gid;
      もし (get_seac_components (gid, &base_gid, &accent_gid))
      {
        builder->add_depend (gid, HB_TAG('C','F','F',' '), base_gid);
        builder->add_depend (gid, HB_TAG('C','F','F',' '), accent_gid);
      }
    }
  }
};

} /* namespace OT */

#endif /* HB_OT_CFF1_TABLE_HH */
